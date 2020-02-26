#include "battle_manager.h"

BattleManager::BattleManager(PokeMemViewer* pmv, InputHandler* input) : pmv(pmv), input(input) {
  bai = new SimpleBattleAI(); // CHANGE THIS TO CHANGE THE AI
  in_battle_status = new int(0);
  battle_turn = new int(-1);
}

BattleManager::~BattleManager() {
  delete in_battle_status;
  delete battle_turn;
}

void BattleManager::handleMemoryWrite(uint16_t addr, uint8_t value) {
  if (addr == 0xD057) {
    printf("new in_battle_status: %d\n", value);
    std::unique_lock<std::mutex> lck(in_battle_status_mtx);
    if (value != *in_battle_status) {
      updateBattleStatus(value);
    }
  } else if (addr == 0xFFF3 && in_battle_status > 0) {
    // printf("battle_turn: %d\n", value);
    std::unique_lock<std::mutex> lck(battle_turn_mtx);
    *battle_turn = value;
  } else if (addr == 0xCCDB) {
    // printf("Move menu type : %d\n",value);
  }
}

void BattleManager::updateBattleStatus(int ibs) {
  *in_battle_status = ibs;
  if (ibs == 0) {
    printf("ended battle\n");
  } else if (ibs == 1) {
    printf("started wild battle\n");
    std::thread t(&BattleManager::startWildBattle, this);
    t.detach();
  } else if (ibs == 2) {
    printf("started trainer battle\n");
    std::thread t(&BattleManager::startTrainerBattle, this);
    t.detach();
  } else if (ibs == -1) {
    printf("lost battle\n");
  }
}

void BattleManager::pressUntilTurnChange(int target) {
  printf("pressUntilTurnChange(%d)...\n",target);
  std::unique_lock<std::mutex> battle_turn_lck(battle_turn_mtx);
  std::unique_lock<std::mutex> ibs_lck(in_battle_status_mtx);

  while (*battle_turn != target && *in_battle_status > 0) {
    battle_turn_lck.unlock();
    ibs_lck.unlock();
    input->pressButton(A);
    Sleep(500);
    battle_turn_lck.lock();
    ibs_lck.lock();
  }

  input->pressButton(B);
  printf("done\n");
}

BattlePrompt BattleManager::pressUntilPrompt() {
  printf("pressUntilPrompt...\n");
  std::unique_lock<std::mutex> ibs_lck(in_battle_status_mtx);

  bool hit_prompt = false;

  while (*in_battle_status > 0 && !hit_prompt) {
    ibs_lck.unlock();

    uint8_t txt_id = pmv->getTextBoxID();
    uint16_t box_pos = pmv->getBoxPos();
    uint16_t tgt_hp = pmv->getTargetMonHP();
    uint16_t player_hp = pmv->getPlayerMonHP();

    if (txt_id == 0x0B) {
      return BP_BATTLE_MENU;
    } else if (txt_id == 0x14 && tgt_hp > 0 && player_hp > 0) {
      return BP_NICKNAME;
    } else if (txt_id == 0x14 && player_hp == 0) {
      return BP_SWITCH_YES_NO;
    } else if (box_pos == 0x0100 && player_hp == 0) {
      return BP_SWITCH_MENU;
    } else if (txt_id == 0x14 && tgt_hp == 0) {
      return BP_LEARN_MOVE;
    }

    input->pressButton(A);
    Sleep(500);
    ibs_lck.lock();
  }

  printf("battle over\n");
  return BP_ENDED;
}

void BattleManager::skipIntro() {
  while (pmv->getTextBoxID() != 0x0B) {
    input->pressButton(B);
  }
}

void BattleManager::battle(BattleState* bs) {
  std::unique_lock<std::mutex> ibs_lck(in_battle_status_mtx);
  do {
    ibs_lck.unlock();
    BattleInput move = bai->chooseInput(bs);
    input->executeBattleInput(move);
    BattlePrompt prompt = pressUntilPrompt();
    printPrompt(prompt);

    updateBattleState(bs, prompt);
    ibs_lck.lock();
  } while (*in_battle_status > 0);

  printf("battle ended, ibs: %d\n", *in_battle_status);
  delete bs;
}

void BattleManager::updateBattleState(BattleState* bs, BattlePrompt prompt) {
  bs->deleteOldData();
  int num_player_mons, num_enemy_mons, cur_player_mon_ind, cur_enemy_mon_ind;

  pmv->initTrainerMons(&bs->player_mons, &bs->num_player_mons, &bs->cur_player_mon_ind, true);
  bs->cur_player_mon = bs->player_mons[bs->cur_player_mon_ind];

  // if (bs->b_type == TRAINER) {
  //   pmv->initTrainerMons(&bs->enemy_mons, &bs->num_enemy_mons, &bs->cur_enemy_mon_ind, false);
  //   bs->cur_enemy_mon = bs->enemy_mons[bs->cur_enemy_mon_ind];
  // } else if (bs->b_type == ENCOUNTER) {
    pmv->initWildMon(&bs->enemy_mons);
    bs->num_enemy_mons = 1;
    bs->cur_enemy_mon_ind = 0;
    bs->cur_enemy_mon = bs->enemy_mons[0];
  // }

  bs->prompt = prompt;
  bs->num_mons_to_level = initLevelInfo(&bs->mons_to_level, bs->player_mons, bs->num_player_mons);
  // printLevelInfo(bs->mons_to_level, bs->num_mons_to_level);

  bs->leveling_mon_name = pmv->getPokemonName(0xD036);
}


void BattleManager::startWildBattle() {
  printf("wild battle started!\n");
  skipIntro();

  BattleState* bs = new BattleState();
  bs->b_type = ENCOUNTER;
  updateBattleState(bs, BP_BATTLE_MENU);
  battle(bs);

  *battle_turn = -1;
  printf("end of startWildBattle\n");
}

void BattleManager::startTrainerBattle() {
  printf("trainer battle started!\n");
  skipIntro();

  BattleState* bs = new BattleState();
  bs->b_type = TRAINER;
  updateBattleState(bs, BP_BATTLE_MENU);
  battle(bs);

  *battle_turn = -1;
  printf("end of startTrainerBattle\n");
}

void BattleManager::printPrompt(BattlePrompt p) {
  switch (p) {
    case BP_BATTLE_MENU:
      printf("BP_BATTLE_MENU\n");
      break;
    case BP_NICKNAME:
      printf("BP_NICKNAME\n");
      break;
    case BP_SWITCH_YES_NO:
      printf("BP_SWITCH_YES_NO\n");
      break;
    case BP_SWITCH_MENU:
      printf("BP_SWITCH_MENU\n");
      break;
    case BP_LEARN_MOVE:
      printf("BP_LEARN_MOVE\n");
      break;
    case BP_ENDED:
      printf("BP_ENDED\n");
      break;
    default:
      printf("Unknown prompt\n");
      break;
  }
}

int BattleManager::initLevelInfo(LevelInfo** lvl_info, Pokemon** party, int party_size) {
  uint8_t starters_list[9] = { 0x99, 0x09, 0x9A, 0xB1, 0xB3, 0x1C, 0xB0, 0xB2, 0xB4 };
  uint8_t max_lvl = 0;
  uint8_t starter_lvl = 0;
  int starter_ind = -1;

  // find starter pokemon, and max level of non-starter
  for (int i = 0; i < party_size; i++) {
    Pokemon* mon = party[i];
    bool starter = false;
    uint8_t id = mon->getIndex();
    uint8_t lvl = mon->getLevel();
  
    for (int j = 0; j < 9; j++) {
      if (id == starters_list[j]) {
        starter = true;
        break;
      }
    }

    if (starter) {
      starter_lvl = lvl;
      starter_ind = i;
    } else {
      max_lvl = std::fmax(max_lvl, lvl);
    }
  }

  // check if starter is underleveled
  bool lvl_starter = starter_ind >=0 && (starter_lvl < max_lvl + 5);
  int mon_to_lvl = (int) lvl_starter;

  // find number of underleveld mon
  for (int i = 0; i < party_size; i++) {
    if (i != starter_ind && party[i]->getLevel() < max_lvl) {
      mon_to_lvl++;
    }
  }

  if (mon_to_lvl == 0) {
    *lvl_info = NULL;
    return 0;
  }

  LevelInfo* li_arr = new LevelInfo[mon_to_lvl];
  int ind = 0;

  if (lvl_starter) {
    LevelInfo li;
    li.mon_ind = starter_ind;
    li.level_goal = max_lvl + 5;
    li_arr[ind++] = li;
  }

  for (int i = 0; i < party_size; i++) {
    if (i != starter_ind && party[i]->getLevel() < max_lvl) {
      LevelInfo li;
      li.mon_ind = i;
      li.level_goal = max_lvl;
      li_arr[ind++] = li;
    }
  }

  *lvl_info = li_arr;
  return mon_to_lvl;
}

void BattleManager::printLevelInfo(LevelInfo* lvl_info, int size) {
  printf("----- Mons to Level: %d -------\n", size);
  for (int i = 0; i < size; i++) {
    lvl_info[i].print();
  }
  printf("---------------------------\n");
}



// OLD
void BattleManager::cycleAttacks() {
  std::unique_lock<std::mutex> ibs_lck(in_battle_status_mtx);
  BattleInput bi;
  bi.type = BIT_ATTACK;
  bi.target = 0;
  do {
    ibs_lck.unlock();
    printf("using move %d\n", bi.target);
    input->executeBattleInput(bi);
    printf("pressing A until status change to 1\n");
    pressUntilTurnChange(1);
    printf("pressing A until status chagne to 0\n");
    pressUntilTurnChange(0);

    bi.target = (bi.target + 1) % 4;
    printf("new move_no: %d\n", bi.target);
    ibs_lck.lock();
  } while (*in_battle_status == 1);
}