#include "game_input_handler.h"

void GameInputHandler::useHM(uint8_t mon_offset, uint8_t move_offset) {

  pressButton(START);
  moveToItem(pmv->getCurMenuItem(), 1);
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), mon_offset);
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), move_offset);
  pressButton(A);

  // Get out of menu. Kind of jank, but idk
  for (int i = 0; i < 3; i++) {
    Sleep(500);
    pressButton(B);
  }
}

void GameInputHandler::learnHM(Item item, int mon_offset, int move_offset) {
  pressButton(START);
  moveToItem(pmv->getCurMenuItem(), 2);
  pressButton(A);
  useItem(item);
  pressButton(A); // use
  pressUntilMenu(0x0100);
  moveToItem(pmv->getCurMenuItem(), mon_offset);
  pressButton(A); // choose mon

  if (move_offset >= 0) {
    printf("has 4 moves\n");
    // mon has 4 moves  
    pressUntilMenu(0x080F); // skip dialog
    pressButton(A); // yes
    Sleep(1500);
    moveToItem(pmv->getCurMenuItem(), move_offset);
    pressButton(A); // forget move
  }

  pressUntilMenu(0x0405); // skip dialog
  pressButton(B); // exit item menu
  pressButton(B); // exit menu
}

void GameInputHandler::pressUntilMenu(uint16_t box_pos) {
  while(true) {
    if (pmv->getBoxPos() == box_pos) {
      return;
    }

    pressButton(A);
    Sleep(500);
  }
}

void GameInputHandler::skipDialog() {
  while (pmv->isInTextbox()) {
    pressButton(B);
  }
}

void GameInputHandler::switchWithinParty(uint8_t mon_1, uint8_t mon_2) {
  pressButton(START);
  moveToItem(pmv->getCurMenuItem(), 1);
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), mon_1);
  pressButton(A);

  // Get to bottom of menu
  for (int i = 0; i < 6; i++) {
    pressButton(DOWN);
  }
  
  pressButton(UP);
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), mon_2);
  pressButton(A);
  Sleep(500);
  pressButton(B);
  Sleep(500);
  pressButton(B);
}

void GameInputHandler::switchBox(uint8_t box_offset) {
  navToBoxMenu();
  moveToItem(pmv->getCurMenuItem(), 0x03);
  pressButton(A);
  pressUntilMenu(0x080F); // get to yes/no
  pressButton(A); // yes
  Sleep(500);
  moveToItem(pmv->getCurMenuItem(), box_offset);
  pressButton(A);
  Sleep(1000);
  pressButton(B);
  Sleep(500);
  pressButton(B);
  Sleep(200);
}

void GameInputHandler::withdrawMon(uint8_t mon_offset) {
  navToBoxMenu();
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), mon_offset);
  pressButton(A);
  pressButton(A);
  Sleep(1500); // Mon is taken out
  pressButton(A);
  Sleep(1000); // Got mon
  pressButton(A);
  pressButton(B);
  pressButton(B);
}

void GameInputHandler::depositMon(uint8_t mon_offset) {
  navToBoxMenu();
  moveToItem(pmv->getCurMenuItem(), 0x01);
  pressButton(A);
  moveToItem(pmv->getCurMenuItem(), mon_offset);
  pressButton(A);
  pressButton(A);
  Sleep(2000);
  pressButton(A);
  pressButton(B);
  pressButton(B);
}

void GameInputHandler::navToBoxMenu() {
  pressButton(A); // Activate PC
  Sleep(1500); // RED turned on The PC
  pressButton(A);
  Sleep(1000); // BILL's PC
  pressButton(A);
  Sleep(2000); // RED turned on The PC
  pressButton(A);
  Sleep(1500); // Accessed POKEMON Storage System
  pressButton(A);
  Sleep(500); // Withdraw 
}

void GameInputHandler::buyItem(uint8_t item_offset, uint8_t quantity) {
  pressButton(A, 1500);
  moveToItem(pmv->getCurMenuItem(), item_offset);
  pressButton(A);

  for (int i = 1; i < quantity; i++) {
    pressButton(UP);
  }

  pressButton(A, 2000);
  pressButton(A, 2000);
  pressButton(A, 2000);
  pressButton(A);
  pressButton(B);
  pressButton(B);
  pressButton(B);
  pressButton(B);
}

void GameInputHandler::useItemOnMon(Item item, uint8_t mon_offset) {
  pressButton(START);
  moveToItem(pmv->getCurMenuItem(), 2);
  pressButton(A);
  useItem(item);
  pressButton(A, 1500); // use
  moveToItem(pmv->getCurMenuItem(), mon_offset);
  pressButton(A, 3000);
  pressButton(A, 2000);
  pressButton(B);
  pressButton(B);
}
