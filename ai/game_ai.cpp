#include "game_ai.h"

GameAI::GameAI(GameInputHandler* input) : input(input) {}


void GameAI::execGoal(GameState* state) {
  Goal* cur_goal = state->chapter->cur_goal;
  switch(cur_goal->type) {
    case GT_USE_HM:
      useHM(state);
      break;
    case GT_LEARN_HM:
      learnHM(state);
      break;
    case GT_ACTIVATE:
      input->pressButton(A);
      state->chapter->incGoal();
      break;
    case GT_SKIP_DIALOG:
      input->skipDialog();
      state->chapter->incGoal();
      break;
    case GT_SWITCH_IN_PARTY:
      switchInParty(state);
      break;
    case GT_SWITCH_BOX:
      input->switchBox(state->chapter->cur_goal->data1);
      state->chapter->incGoal();
      break;
    case GT_WITHDRAW_MON:
      withdraw(state);
      break;
    case GT_DEPOSIT_MON:
      deposit(state);
      break;
    case GT_BUY_ITEM:
      buyItem(state);
      break;
    case GT_USE_ITEM_ON_MON:
      useItemOnMon(state);
      break;
    default:
      printf("Goal type %d not yet supported\n", cur_goal->type);
      throw;
  }
}

void GameAI::useHM(GameState* state) {
  UseHM* goal = dynamic_cast<UseHM*>(state->chapter->cur_goal);
  Pokemon** mons = state->mons;
  int num_mons = state->num_mons;

  for (int i = 0; i < num_mons; i++) {
    Move* moves = mons[i]->getMovesAndPP().moves;
    int num_moves = mons[i]->getMovesAndPP().num_moves;

    for (int j = 0; j < num_moves; j++) {
      if (moves[j].getId() == goal->hm.move_id) {
        printf("HM found! Offsets: %d %d\n", i, j);
        input->useHM(i, j);
        state->chapter->incGoal();
        return;
      }
    }
  }

  printf("HM not found!\n");
}

void GameAI::learnHM(GameState* state) {
  printf("In ai learnHM\n");
  LearnHM* goal = dynamic_cast<LearnHM*>(state->chapter->cur_goal);

  goal->hm.print();
  // determine if any mons can learn
  // figure out best mon to teach
  Pokemon** mons = state->mons;
  double worst_score = 10000;
  int worst_move_ind = -1;
  int worst_mon_ind = -1;
  for (int i = 0; i < state->num_mons; i++) {
    printf("mon: %d\n", i);
    Pokemon* mon = mons[i];

    printf("checking if mon can learn hm\n");
    if (!mon->canLearnHM(goal->hm.hm_no)) { 
      printf("mon %d can't learn hm\n", i);
      continue; 
    }

    printf("checking if mon knows 4 moves\n");
    MovesAndPP move_data = mon->getMovesAndPP();
    if (move_data.num_moves != 4) {
      printf("mon doesnt know 4 moves!\n");
      worst_mon_ind = i;
      break;
    }

    printf("parsing moves\n");
    for (int j = 0; j < move_data.num_moves; j++) {
      printf("j: %d\n", j);
      Move m = move_data.moves[j];

      printf("checking if move is hm\n");
      if (m.isHM()) {
        printf("can't forget HM\n");
        continue; // can't forget HM
      }

      double score = (m.getPower() / 150.0)*70.0 + (m.getAccuracy())*10.0 + (m.getStatusPriority() / 8.0)*20.0;
      printf("score: %f, min_score: %f\n", score, worst_score);
      if (score < worst_score) {
        printf("updating worst_score\n");
        worst_score = score;
        worst_move_ind = j;
        worst_mon_ind = i;
      }
    }
    printf("on to next mon\n");
  }

  if (worst_mon_ind >= 0) {
    printf("Learning HM, Item: %d, mon_id: %d, move_id: %d\n", static_cast<int>(goal->hm.item), worst_mon_ind, worst_move_ind);
    input->learnHM(goal->hm.item, worst_mon_ind, worst_move_ind);
    state->chapter->incGoal();
  } else {
    printf("No party mons can learn hm. Implement override\n");
    throw;
  }
}

void GameAI::switchInParty(GameState* state) {
  input->switchWithinParty(state->chapter->cur_goal->data1, state->chapter->cur_goal->data2);
  state->chapter->incGoal();
}

void GameAI::withdraw(GameState* state) {
  WithdrawMon* goal = dynamic_cast<WithdrawMon*>(state->chapter->cur_goal);
  Pokemon mon = goal->mon;
  printf("In withdraw, current box: %d, printing mon:\n", state->cur_box_no);
  mon.print();
  if (state->cur_box_no != mon.getBoxNo()) {
    printf("need to switch boxes\n");
    input->switchBox(mon.getBoxNo()-1);
  }
  printf("withdrawing mon\n");
  input->withdrawMon(mon.getBoxOffset());
  GameData::updateBoxedMons(mon.getBoxNo());
  state->chapter->incGoal();
}

void GameAI::deposit(GameState* state) {
  printf("in deposit mon");
  uint8_t box_no = state->cur_box_no;

  
  if (state->mons_in_box == 20) {
    printf("current box full, switching up one\n");
    box_no = (state->cur_box_no % 12) + 1;
    input->switchBox(box_no-1);
    return;
  }

  printf("depositing\n");
  input->depositMon(state->chapter->cur_goal->data1);
  GameData::updateBoxedMons(box_no);
  state->chapter->incGoal();
}

void GameAI::buyItem(GameState* state) {
  if (state->num_mart_items == 0) {
    input->pressButton(A, 1500);
    return;
  }

  BuyItem* goal = dynamic_cast<BuyItem*>(state->chapter->cur_goal);
  for (int i = 0; i < state->num_mart_items; i++) {
    if (state->mart_items[i] == static_cast<uint8_t>(goal->item)) {
      input->buyItem(i, goal->quantity);
      state->chapter->incGoal();
      return;
    }
  }
  printf("Shop doesnt sell item %d\n", static_cast<uint8_t>(goal->item));
  throw;
}

void GameAI::useItemOnMon(GameState* state) {
  UseItemOnMon* goal = dynamic_cast<UseItemOnMon*>(state->chapter->cur_goal);
  input->useItemOnMon(goal->item, goal->mon_offset);
  state->chapter->incGoal();
}



