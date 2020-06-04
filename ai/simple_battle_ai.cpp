#include "simple_battle_ai.h"

SimpleBattleAI::SimpleBattleAI() {
  // printEvosMovesList();

  // writeMoveList();

  // printMoveList();
  // printTypeMatrix();
  move_test[0] = 2;
  move_test[1] = 4;
  move_test[2] = 4;
  move_test[3] = 1;
  move_test[4] = 3;
  test_move = 0;
  test_switch = 1;
}

BattleInput SimpleBattleAI::chooseInput(BattleState* battle) {
  // battle->print();

  if (override_mode != BM_DEFAULT) {
    switch(override_mode) {
      case BM_WAAC:
        return chooseWAACMove(battle);
      default:
        printf("bad override mode, running test\n");
        return attackTest();
    }
  }

  BattleMode mode;
  if (battle->b_type == TRAINER) {
    printf("WAAC\n");
    mode = BM_WAAC;
  } else if (battle->b_type == ENCOUNTER && !battle->cur_enemy_mon->isOwned()) {
    printf("Trying to catch\n");
    mode = BM_CATCH;
  } else if (battle->b_type == ENCOUNTER) {
    printf("Leveling\n");
    mode = BM_LEVEL;
  } else {
    printf("WAAC\n");
    mode = BM_WAAC;
  }

  // battle->print();
  BattleInput bi;
  BattlePrompt prompt = battle->prompt;

  if (prompt == BP_BATTLE_MENU || prompt == BP_SWITCH_MENU || prompt == BP_SWITCH_YES_NO) {
    switch(mode) {
      case BM_WAAC:
        return chooseWAACMove(battle);
      case BM_CATCH:
        return chooseCatchMove(battle);
      case BM_LEVEL:
        return chooseLevelMove(battle);
      default:
        printf("bad mode, running test\n");
        return attackTest();
    }
  } else if (prompt == BP_NICKNAME) {
    bi.type = BIT_DONT_NAME;
  } else if (prompt == BP_LEARN_MOVE) {
    return chooseNewMove(battle);
  } else {
    throw ("Dont know what to do\n");
  }

  return bi;
}

////////////// CHOSE METHODS ////////////////////////////////////////////////////////////

BattleInput SimpleBattleAI::chooseWAACMove(BattleState* battle)  {
  BattleInput bi;

  // battle->print();

  if (battle->prompt == BP_SWITCH_YES_NO) {
    return BattleInput(BIT_DONT_SWITCH, 0);
  }

  // If move can kill right now, use one with best accuracy
  Pokemon* attacker = battle->cur_player_mon;
  Pokemon* target = battle->cur_enemy_mon;

  MoveChoice* choices;
  int num_choices = initMoveChoices(&choices, battle, true);

  // printMoveChoices(choices, num_choices);

  // Find best moves
  MoveChoice best_cur_mc(true);
  MoveChoice best_non_status_mc(true);
  MoveChoice best_status_mc(true);
  MoveChoice best_mc(true);
  bool other_mon_move = false;

  for (int i = 0; i < num_choices; i++) {
    MoveChoice mc = choices[i];

    if (mc.move_owner == battle->cur_player_mon_ind) {
      if (best_cur_mc.is_default || isBetterMove(mc, best_cur_mc, 1)) {
        best_cur_mc = mc;
      }
      if (best_non_status_mc.is_default || isBetterMove(mc, best_non_status_mc, 2)) {
        best_non_status_mc = mc;
      }
      if (best_status_mc.is_default || isBetterMove(mc, best_status_mc, 3)) {
        best_status_mc = mc;
      }
    } else {
      if (best_mc.is_default || isBetterMove(mc, best_mc, 1)) {
        best_mc = mc;
      }
    }
  }
  delete [] choices;

  // If at switch menu, switch
  if (battle->prompt == BP_SWITCH_MENU) {
    printf("Mon died, switching to next best\n");
    bi.type = BIT_SWITCH;
    bi.target = best_mc.move_owner;
    return bi;
  }

  // If best current move can KO, use
  if (best_cur_mc.min_damage >= target->getCurHp()) {
    printf("One hit KO Path\n");
    bi.type = BIT_ATTACK;
    bi.target = best_cur_mc.move_pos;
    return bi;
  }

  // If best overall attack does twice as much damage as best current attack, switch
  if (best_mc.min_damage >= best_cur_mc.min_damage * 2.0) {
    printf("Other pokemon has better attack path\n");
    bi.type = BIT_SWITCH;
    bi.target = best_mc.move_owner;
    return bi;
  }

  // If enemy can kill you on next attack, switch
  MoveChoice* enemy_choices;
  int num_enemy_choices = initMoveChoices(&enemy_choices, battle, false);
  for (int i = 0; i < num_enemy_choices; i++) {
    // if (best_mc.is_default) { break; }
    MoveChoice mc = enemy_choices[i];
    if (mc.min_damage >= attacker->getCurHp()) {
      printf("About to die Path\n");
      delete [] enemy_choices;
      bi.type = BIT_SWITCH;
      bi.target = best_mc.move_owner;
      return bi;
    }
  }
  delete [] enemy_choices;

  // If best non status attack can do > 50% damage, use it
  if (best_non_status_mc.min_damage >= (target->getCurHp() / 2.0)) {
    printf("Non status Path\n");
    bi.type = BIT_ATTACK;
    bi.target = best_non_status_mc.move_pos;
    return bi;
  }

  // Use move that maximizes status score
  printf("Status Path\n");
  bi.type = BIT_ATTACK;
  bi.target = best_status_mc.move_pos;
  return bi;
}

BattleInput SimpleBattleAI::chooseCatchMove(BattleState* battle) {
  Pokemon* target = battle->cur_enemy_mon;
  double cc;
  Item bt = getBestChance(target, &cc);
  printf("catch prob with %d: %f\n", static_cast<int>(bt), cc);
  double base_cr = target->getCatchRate();
  double hp_pct = (target->getCurHp() * 1.0) / (target->getMaxHp() * 1.0);

  if (battle->prompt == BP_SWITCH_YES_NO) {
    printf("mon die, clicking yes\n");
    return BattleInput(BIT_CHOOSE_NEXT, 0);
  } if (battle->prompt == BP_SWITCH_MENU) {
    printf("switching\n");
    MoveChoice* choices;
    int num_choices = initMoveChoices(&choices, battle, true);
    printMoveChoices(choices, num_choices);
    uint8_t owner = choices[0].move_owner;
    delete [] choices;
    return BattleInput(BIT_SWITCH, owner);
  }

  if ((base_cr <= 30 && battle->balls_used >= 20) ||
      (base_cr > 30 && base_cr < 200 && battle->balls_used >= 10) ||
      (base_cr >= 200 && battle->balls_used >= 7)) {
    printf("Too many balls moved, taking mon out\n");
    override_mode = BM_WAAC;
    return chooseWAACMove(battle);
  }

  if (cc >= .75) {
    printf("Using ball %d, chance %f\n", static_cast<int>(bt), cc);
    battle->balls_used += 1;
    return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
  }

  if (base_cr <= 30) {
    printf("hard catch, rate %f\n", base_cr);
    MoveChoice best_move = findBestStatusMove(battle, false);
    if (best_move.is_default) {
      printf("No good moves, throwing %d with chance %f\n", static_cast<int>(bt), cc);
      battle->balls_used += 1;
      return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
    } else if (battle->cur_player_mon_ind != best_move.move_owner) {
      printf("Switching to %d who has best status move\n", best_move.move_owner);
      return BattleInput(BIT_SWITCH, best_move.move_owner);
    } else if (target->getNumStatuses() == 0) {
      printf("Using best_move to inflict status\n");
      return BattleInput(BIT_ATTACK, best_move.move_pos);
    } else {
      printf("Status inflicted, throwing %d with chance %f\n", static_cast<int>(bt), cc);
      battle->balls_used +=- 1;
      return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
    }
  } else if (base_cr < 200) {
    printf("medium catch, rate: %f\n", base_cr);
    MoveChoice best_status_move = findBestStatusMove(battle, true);
    MoveChoice best_catch_move = findBestCatchMove(battle);

    if (target->getNumStatuses() == 0 && !best_status_move.is_default && hp_pct <= .33 && best_status_move.move.getEffect() == SLEEP_EFFECT && battle->cur_player_mon_ind != best_status_move.move_owner) {
      printf("Pokemon hurt, but switching to sleep\n");
      return BattleInput(BIT_SWITCH, best_status_move.move_owner);
    } else if (target->getNumStatuses() == 0 && !best_status_move.is_default && hp_pct <= .33 && best_status_move.move.getEffect() == SLEEP_EFFECT) {
      printf("Pokemon hurt, but trying to sleep\n");
      return BattleInput(BIT_ATTACK, best_status_move.move_pos);
    } else if (hp_pct <= .33 || (best_catch_move.is_default && best_status_move.is_default)) {
      printf("HP low or no moves, throwing %d with chance %f\n", static_cast<int>(bt), cc);
      battle->balls_used += 1;
      return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
    } else if (target->getNumStatuses() == 0 && !best_status_move.is_default && battle->cur_player_mon_ind != best_status_move.move_owner) {
      printf("Switching to %d who has best status move\n", best_status_move.move_owner);
      return BattleInput(BIT_SWITCH, best_status_move.move_owner);
    } else if (target->getNumStatuses() == 0 && !best_status_move.is_default) {
      printf("Using best_status_move to inflict status\n");
      return BattleInput(BIT_ATTACK, best_status_move.move_pos);
    } else if (!best_catch_move.is_default && battle->cur_player_mon_ind != best_catch_move.move_owner) {
      printf("Switching to %d who has best catch move\n", best_catch_move.move_owner);
      return BattleInput(BIT_SWITCH, best_catch_move.move_owner);
    } else if (!best_catch_move.is_default) {
      printf("Using best_catch_move\n");
      return BattleInput(BIT_ATTACK, best_catch_move.move_pos);
    } else {
      printf("Hit default, throwing ball I guess\n");
      battle->balls_used += 1;
      return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
    }
  } else {
    printf("easy catch, rate: %f\n", base_cr);
    MoveChoice best_move = findBestCatchMove(battle);

    if (hp_pct <= .33 || best_move.is_default) {
      printf("HP low or no moves, throwing %d with chance %f\n", static_cast<int>(bt), cc);
      battle->balls_used += 1;
      return BattleInput(BIT_USE, static_cast<uint8_t>(bt));
    } else if (best_move.move_owner != battle->cur_player_mon_ind) {
      printf("Switching to mon %d with best attack\n", best_move.move_owner);
      return BattleInput(BIT_SWITCH, best_move.move_owner);
    } else {
      printf("Using best_catch_move\n");
      return BattleInput(BIT_ATTACK, best_move.move_pos);    
    }
  }
}

BattleInput SimpleBattleAI::chooseLevelMove(BattleState* state) {
  if (state->prompt == BP_SWITCH_YES_NO) {
    printf("mon die, clicking yes\n");
    return BattleInput(BIT_CHOOSE_NEXT, 0);
  } if (state->prompt == BP_SWITCH_MENU) {
    printf("switching\n");
    MoveChoice* choices;
    int num_choices = initMoveChoices(&choices, state, true);
    uint8_t owner = choices[0].move_owner;
    delete [] choices;
    return BattleInput(BIT_SWITCH, owner);
  }

  Pokemon* attacker = state->cur_player_mon;
  Pokemon* target = state->cur_enemy_mon;
  MoveChoice* choices;
  int num_choices = initMoveChoices(&choices, state, true);
  int level_goal = getLevelGoal(state->cur_player_mon_ind, state);

  // Find best moves
  MoveChoice best_cur_mc(true);
  MoveChoice best_other_training_mc(true);
  MoveChoice best_other_mc(true);
  bool other_mon_move = false;

  for (int i = 0; i < num_choices; i++) {
    MoveChoice mc = choices[i];

    if (mc.move_owner == state->cur_player_mon_ind) {
      if (best_cur_mc.is_default || isBetterMove(mc, best_cur_mc, 2)) {
        best_cur_mc = mc;
      }
    } else if (getLevelGoal(mc.move_owner, state) >= 0) {
      if (best_other_training_mc.is_default || isBetterMove(mc, best_other_training_mc, 2)) {
        best_other_training_mc = mc;
      }
    } else if (best_other_mc.is_default || isBetterMove(mc, best_other_mc, 2)) {
      best_other_mc = mc;
    }
  }
  delete [] choices;


  // If no mons need levelling attack
  if (state->num_mons_to_level == 0) {
    printf("No mons need leveling, attacking\n");
    return BattleInput(BIT_ATTACK, best_cur_mc.move_pos);
  }

  // If current mon doesnt need leveling, switch
  if (level_goal < 0) {
    printf("Other mon needs leveling, switching\n");
    return BattleInput(BIT_SWITCH, state->mons_to_level[0].mon_ind);
  }

  // If about to die, switch to next mon that needs leveling
  MoveChoice* enemy_choices;
  int num_enemy_choices = initMoveChoices(&enemy_choices, state, false);
  for (int i = 0; i < num_enemy_choices; i++) {
    MoveChoice mc = enemy_choices[i];
    if (mc.min_damage >= attacker->getCurHp()) {
      printf("About to die Path\n");
      delete [] enemy_choices;

      if (!best_other_training_mc.is_default) {
        printf("Switching to other training mon\n");
        return BattleInput(BIT_SWITCH, best_other_training_mc.move_owner);
      } else if (!best_other_mc.is_default) {
        printf("Switching to other non training mon\n");
        return BattleInput(BIT_SWITCH, best_other_mc.move_owner);
      }
    }
  }
  delete [] enemy_choices;

  // Use attack with highest damage
  printf("Using best attack\n");
  return BattleInput(BIT_ATTACK, best_cur_mc.move_pos);
}

BattleInput SimpleBattleAI::chooseNewMove(BattleState* state) {
  Pokemon* mon = NULL;
  for (int i = 0; i < state->num_player_mons; i++) {
    if (state->leveling_mon_name == state->player_mons[i]->getName()) {
      std::cout << "Leveling mon found at index " << i << ": " << state->leveling_mon_name << "\n";
      mon = state->player_mons[i];
    }
  }

  if (mon == NULL) {
    printf("Could not find leveling mon\n");
    throw;
  }

  uint8_t level = mon->getLevel();
  PokemonBase* pb = mon->getBase();
  Move new_move = pb->getMoveLearned(level);

  if (new_move.getPower() == 0) {
    printf("New move is useless\n");
    return BattleInput(BIT_DONT_LEARN_MOVE, 0);
  }

  if (new_move.hasBadEffect()) {
    printf("Move has bad effect, not learning\n");
    return BattleInput(BIT_DONT_LEARN_MOVE, 0);
  }

  MovesAndPP move_data = mon->getMovesAndPP();
  double min_score = 1000;
  int worst_move_ind = 0;

  for (int i = 0; i < move_data.num_moves; i++) {
    Move move = move_data.moves[i];

    if (move.isHM()) {
      continue; // can't forget HMs
    }

    if (move.getPower() == 0) {
      printf("Forgetting useless move %d\n", i);
      return BattleInput(BIT_LEARN_MOVE, i);
    }

    double score = (move.getPower() / 150.0)*70.0 + (move.getAccuracy())*10.0 + (move.getStatusPriority() / 8.0)*20.0;
    if (score < min_score) {
      min_score = score;
      worst_move_ind = i;
    }
  }

  double new_move_score = (new_move.getPower() / 150.0)*70.0 + (new_move.getAccuracy())*10.0 + (new_move.getStatusPriority() / 8.0)*20.0;
  if (new_move_score > min_score) {
    printf("worst move is %d, forgetting that\n", worst_move_ind);
    return BattleInput(BIT_LEARN_MOVE, worst_move_ind);
  } else {
    printf("new move is worse than all others, not learning\n");
    return BattleInput(BIT_DONT_LEARN_MOVE, 0);
  }

}

///////////////////////// END CHOOSE METHODS //////////////////////////////////////////////////////////

///////////////////////// CHOOSE HELPER METHODS  //////////////////////////////////////////////////////

int SimpleBattleAI::initMoveChoices(MoveChoice** ptr, BattleState* battle, bool player) {
  int num_moves = 0;
  int num_mons = player ? battle->num_player_mons : battle->num_enemy_mons;
  Pokemon** mon_arr = player ? battle->player_mons : battle->enemy_mons;
  Pokemon* target_mon = player ? battle->cur_enemy_mon : battle->cur_player_mon;

  for (int i = 0; i < num_mons; i++) {
    num_moves += mon_arr[i]->getNumValidMoves();
  }

  MoveChoice* choices = new MoveChoice[num_moves];
  int ind = 0;
  for (int i = 0; i < num_mons; i++) {
    Pokemon* mon = mon_arr[i];

    if (mon->getCurHp() == 0) {
      continue;
    }

    MovesAndPP move_data = mon->getMovesAndPP();
    for (int j = 0; j < move_data.num_moves; j++) {
      MoveChoice mc;
      Move m = move_data.moves[j];
      uint8_t pp = move_data.pps[j];
      if (pp == 0) { continue; }
      mc.move = m;
      mc.move_owner = i;
      mc.move_pos = j+1;
      mc.min_damage = getMinDamagePossible(mc.move, mon, target_mon);
      mc.max_damage = getMaxDamagePossible(mc.move, mon, target_mon);
      mc.no_status_score = (mc.min_damage * 100 / (target_mon->getMaxHp() * 1.0)) - (1.0 - mc.move.getAccuracy())*100;
      mc.status_score = (mc.min_damage * 100 / (target_mon->getMaxHp() * 1.0)) + mc.move.getStatusScore() - (1.0 - mc.move.getAccuracy())*100;
      choices[ind] = mc;
      ind++;
    }
  }

  *ptr = choices;
  return num_moves;
}

void SimpleBattleAI::printMoveChoices(MoveChoice* mcs, int size) {
  printf("---- Move Choices ---\n");
  for (int i = 0; i < size; i++) {
    mcs[i].print();
  }
  printf("\n");
}

// return true if a is better than b
// 1: compare min_damage
// 2: compare non_status_damage
// 3: compare status_damage
bool SimpleBattleAI::isBetterMove(MoveChoice a, MoveChoice b, int score_compare) {
  double a_score, b_score;

  if (score_compare == 1) {
    a_score = a.min_damage;
    b_score = b.min_damage;
  } else if (score_compare == 2) {
    a_score = a.no_status_score;
    b_score = b.no_status_score;
  } else if (score_compare == 3) {
    a_score = a.status_score;
    b_score = b.status_score;
  } else {
    throw "invalid score_compare";
  }

  if (a_score > b_score) {
    return true;
  } else if (a_score == b_score && a.pp > b.pp) {
    return true;
  } else {
    return false;
  }
}

double SimpleBattleAI::getMaxDamagePossible(Move move, Pokemon* attacker, Pokemon* target) {
  return getDamage(move, attacker, target, true);
}

double SimpleBattleAI::getMinDamagePossible(Move move, Pokemon* attacker, Pokemon* target) {
  return getDamage(move, attacker, target, false);
}

double SimpleBattleAI::getDamage(Move move, Pokemon* attacker, Pokemon* target, bool max) {
  if (move.getPower() == 0) {
    return 0;
  }

  bool special_move = isSpecialMove(move);
  double effective_attack, effective_defense;
  if (special_move) {
    effective_attack = attacker->getStats().special;
    effective_defense = target->getStats().special;
  } else {
    effective_attack = attacker->getStats().attack;
    effective_defense = target->getStats().defense;
  }

  double level = max ? attacker->getLevel() * 2 : attacker->getLevel();

  double move_type_matches_user_type = 1;
  if (move.getType() == attacker->getType1() || (move.getType() != NORMAL && move.getType() == attacker->getType2())) {
    move_type_matches_user_type = 1.5;
  }

  double move_effectivness = getTypeEffectivness(move.getType(), target->getType1(), target->getType2());

  return std::fmin(((((((2*level)/5.0) + 2) * move.getPower() * (effective_attack / effective_defense)) / 50.0) + 2)*((max ? 1.0 : 0.85) * move_type_matches_user_type * move_effectivness * 1), target->getCurHp());
}


bool SimpleBattleAI::isSpecialMove(Move move) {
  MoveType special_moves[7] = { WATER, GRASS, FIRE, ICE, ELECTRIC, PSYCHIC, DRAGON };
  for (int i = 0; i < 7; i++) {
    if (move.getType() == special_moves[i]) {
      return true;
    }
  }
  return false;
}

MoveChoice SimpleBattleAI::findBestStatusMove(BattleState* battle, bool filter_sleep) {
  Pokemon* target = battle->cur_enemy_mon;
  MoveChoice* choices;
  int num_choices = initMoveChoices(&choices, battle, true);
  double hp_pct = (target->getCurHp() * 1.0) / (target->getMaxHp() * 1.0);

  MoveChoice best_move(true);
  for (int i = 0; i < num_choices; i++) {
    MoveChoice mc = choices[i];

    if (mc.max_damage >= target->getCurHp()) {
      continue;
    } else if (filter_sleep && mc.move.getEffect() == SLEEP_EFFECT && hp_pct > .33) {
      continue;
    } else if (getTypeEffectivness(mc.move.getType(), target->getType1(), target->getType2()) < .01) {
      continue;
    }

    if (best_move.is_default || mc.move.getStatusPriority() > best_move.move.getStatusPriority()) {
      best_move = mc;
    }
  }
  delete [] choices;

  return best_move;
}

// Move can't kill'
// if target hp < 33, return default
// Maximinse curent mon moves
// use other moves otherwise
// dont use moves with 0 attack
MoveChoice SimpleBattleAI::findBestCatchMove(BattleState* battle) {
  Pokemon* target = battle->cur_enemy_mon;
  double hp_pct = (target->getCurHp() * 1.0) / (target->getMaxHp() * 1.0);

  if (hp_pct <= .33) {
    return MoveChoice(true);
  }

  MoveChoice* choices;
  int num_choices = initMoveChoices(&choices, battle, true);
  MoveChoice best_cur_mc(true);
  MoveChoice best_mc(true);

  for (int i = 0; i < num_choices; i++) {
    MoveChoice mc = choices[i];

    if (mc.max_damage >= target->getCurHp() || mc.max_damage == 0) { continue; }
    
    if (mc.move_owner == battle->cur_player_mon_ind && (best_cur_mc.is_default || mc.max_damage > best_cur_mc.max_damage)) {
      best_cur_mc = mc;
    } else if (best_mc.is_default || mc.max_damage > best_mc.max_damage) {
      best_mc = mc;
    }
  }
  delete [] choices;

  if (!best_cur_mc.is_default) {
    return best_cur_mc;
  } else {
    return best_mc;
  }
}

Item SimpleBattleAI::getBestChance(Pokemon* mon, double* chance) {
  Item btypes[] = {Item::POKEBALL, Item::GREATBALL, Item::ULTRABALL};
  double best_chance = -1;
  Item best_ball;

  for (int i = 0; i < 3; i++) {
    Item bt = btypes[i];
    double c = getCatchChance(mon, bt);

    if (c > best_chance) {
      best_chance = c;
      best_ball = bt;
    }
  }

  *chance = best_chance;
  return best_ball;
}

// source: https://www.dragonflycave.com/mechanics/gen-i-capturing
double SimpleBattleAI::getCatchChance(Pokemon* mon, Item bt) {
  double S, B, C, H, M, G;

  // set S
  if (mon->isAsleep() || mon->isFrozen()) {
    S = 25;
  } else if (mon->isPoisoned() || mon->isBurned() || mon->isParalyzed()) {
    S = 12;
  } else {
    S = 0;
  }

  // set B
  if (bt == Item::POKEBALL) {
    B = 256;
  } else if (bt == Item::GREATBALL) {
    B = 201;
  } else if (bt == Item::ULTRABALL || bt == Item::SAFARIBALL) {
    B = 151;
  } else {
    printf("Unknown ball type\n");
    throw;
  }

  // set G
  if (bt == Item::GREATBALL) {
    G = 8;
  } else {
    G = 12;
  }
  
  // set the rest
  C = mon->getCatchRate();
  H = mon->getCurHp();
  M = mon->getMaxHp();
  
  return (S/B) + (std::fmin(C+1, B - S) / B) * ((std::fmin(255, std::floor(std::floor(M * 255.0 / G) / std::fmax(1, H / 4.0))) + 1) / 256.0);
}

int SimpleBattleAI::getLevelGoal(int mon_ind, BattleState* state) {
  for (int i = 0; i < state->num_mons_to_level; i++) {
    LevelInfo li = state->mons_to_level[i];
    if (mon_ind == li.mon_ind) {
      return li.level_goal;
    }
  }
  return -1;
}

//////////////////////// END HELPER METHODS //////////////////////////////////////////////////////////////



double SimpleBattleAI::getTypeEffectivness(MoveType a1, MoveType d1, MoveType d2) {
  int a1_ind = GameData::type_to_ind_map[a1];
  int d1_ind = GameData::type_to_ind_map[d1];
  int d2_ind = GameData::type_to_ind_map[d2];

  return GameData::type_matrix[a1_ind][d1_ind] * GameData::type_matrix[a1_ind][d2_ind];
}






BattleInput SimpleBattleAI::attackTest() {
  // printf("Using attack %d\n", move_test[test_move]);

  BattleInput bi;
  bi.type = BIT_ATTACK;
  // bi.target = move_test[test_move];
  // test_move = (test_move + 1) % 5;
  bi.target = 0;
  return bi;
}

BattleInput SimpleBattleAI::useItemTest() {
  printf("Using pokeball\n");

  BattleInput bi;
  bi.type = BIT_USE;
  bi.target = 0x04;
  return bi;
}

BattleInput SimpleBattleAI::switchTest() {
  BattleInput bi;
  bi.type = BIT_SWITCH;
  bi.target = 4;
  test_switch = (test_switch + 1) % 6;
  return bi;
}

BattleInput SimpleBattleAI::learnMoveTest(BattleState* state) {
  if (state->prompt == BP_LEARN_MOVE) {
    return chooseNewMove(state);
  }
  if (state->cur_player_mon_ind != 5) {
    return BattleInput(BIT_SWITCH, state->cur_player_mon_ind + 1);
  }
  return BattleInput(BIT_ATTACK,2);
}

