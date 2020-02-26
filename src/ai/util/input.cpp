#define WINVER 0x0500
#include "input.h"

void InputHandler::executeMoves(Movement* moves, int size) {
  for(int i = 0; i < size; i++) {
      executeMove(moves[i]);
      Sleep(200);
  }
}

void InputHandler::executeMove(Movement move) {
  Pos cur_pos = pmv->getPos();
  Pos tgt_pos = cur_pos;

  if (move.dir == UP) {
    tgt_pos.y -= move.dist;
  } else if (move.dir == DOWN) {
    tgt_pos.y += move.dist;
  } else if (move.dir == LEFT) {
    tgt_pos.x -= move.dist;
  } else if (move.dir == RIGHT) {
    tgt_pos.x += move.dist;
  } else {
    throw "Invalid movement direction!";
  }

  printf("\ncurx, cury: %u, %u\nnewx, newy: %u, %u\n\n", cur_pos.x, cur_pos.y, tgt_pos.x, tgt_pos.y);


  // Perform movement
  Pos last_pos = cur_pos;
  printf("itrx, itry: %u, %u\n", last_pos.x, last_pos.y);
  pressKey(move.dir);
  while(1) {
    Pos new_pos = pmv->getPos();
    if (new_pos != last_pos) {
      printf("itrx, itry: %u, %u\n", new_pos.x, new_pos.y);
      last_pos = new_pos;
    }
    if (new_pos == tgt_pos) {
       break;
    }
  }
  releaseKey(move.dir);
}

void InputHandler::pressButton(KEY_PRESS key) {
  // printKey(key);
  pressKey(key);
  Sleep(100);
  releaseKey(key);
  Sleep(500);
}

void InputHandler::addMove(Movement* move, int ind, KEY_PRESS dir, int dist) {
  Movement m;
  m.dir = dir;
  m.dist = dist;
  move[ind] = m;
}


void InputHandler::hoverFight() {
  pressButton(B);
  pressButton(LEFT);
  pressButton(UP);
}

void InputHandler::executeBattleInput(BattleInput move) {
  if (move.type == BIT_ATTACK) {
    attack(move.target);
  } else if (move.type == BIT_SWITCH) {
    switchPokemon(move.target);
  } else if (move.type == BIT_USE) {
    useItem(move.target);
  } else if (move.type == BIT_RUN) {
    flee();
  } else if (move.type == BIT_LEARN_MOVE) {
    pressButton(A);
    Sleep(2000);
    moveToItem(0, move.target);
    pressButton(A);
  } else if (move.type == BIT_DONT_LEARN_MOVE) {
    pressButton(DOWN);
    pressButton(A);
    Sleep(1500);
    pressButton(A);
  } else if (move.type == BIT_DONT_NAME || move.type == BIT_DONT_SWITCH) {
    pressButton(DOWN);
    pressButton(A);
  } else if (move.type == BIT_CHOOSE_NEXT) {
    pressButton(A);
  }
}

void InputHandler::moveToItem(uint8_t cur_offset, uint8_t target_offset) {
  printf("cur_offset: %d, target_offset: %d\n", cur_offset, target_offset);
  int dif = target_offset - cur_offset;

  bool move_down = dif > 0;
  for (int i = 0; i < std::abs(dif); i++) {
    if (move_down) {
      // printf("Moving down!\n");
      pressButton(DOWN);
    } else {
      // printf("Moving up!\n");
      pressButton(UP);
    }
  }
}

void InputHandler::attack(uint8_t move_offset) {
  pressButton(UP);
  pressButton(LEFT);
  pressButton(A);
  uint8_t cur_offset = pmv->getCurMenuItem();
  moveToItem(cur_offset, move_offset);
  pressButton(A);
}

void InputHandler::useItem(uint8_t item_id) {
  pressButton(LEFT);
  pressButton(DOWN);
  pressButton(A);
  Sleep(500);
  uint8_t cur_offset = pmv->getLongListOffset();
  std::pair<uint8_t, uint8_t>* items;
  int num_items = pmv->getAllItems(&items);
  
  int item_offset = 0;
  bool found = false;
  for (int i = 0; i < num_items; i++) {
    if (items[i].first == item_id) {
      found = true;
      break;
    }
    item_offset++;
  }

  if (!found) {
    throw "item not found";
  }

  delete [] items;

  moveToItem(cur_offset, item_offset);
  printf("Pressing A\n");
  pressButton(A);
}

void InputHandler::switchPokemon(uint8_t pokemon_offset) {
  if (pmv->getTextBoxID() == 0x0B) {
    pressButton(RIGHT);
    pressButton(UP);
    pressButton(A);
  }

  uint8_t cur_offset = pmv->getCurMenuItem();
  if (cur_offset == pokemon_offset) {
    printf("CANT SWITCH TO YOURSELF! cur_offset: %d, pokemon_offset: %d\n", cur_offset, pokemon_offset);
    throw 10;
  }

  moveToItem(cur_offset, pokemon_offset);
  pressButton(A);
  pressButton(A);
}

void InputHandler::flee() {
  pressButton(RIGHT);
  pressButton(DOWN);
  pressButton(A);
  Sleep(1500);
  pressButton(A);
}



void InputHandler::pressKey(WORD w) {
  sendInput(w, KEYEVENTF_SCANCODE);
}

void InputHandler::releaseKey(WORD w) {
  sendInput(w, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
}

void InputHandler::sendInput(WORD w, DWORD flags) {
  INPUT ip;
  ip.type = INPUT_KEYBOARD;
  ip.ki.wVk = 0;
  ip.ki.wScan = w;
  ip.ki.time = 0;
  ip.ki.dwExtraInfo = 0;
  ip.ki.dwFlags = flags;
  SendInput(1, &ip, sizeof(INPUT));
}

void InputHandler::printKey(KEY_PRESS key) {
  switch (key) {
    case UP:
      printf("pressing UP\n");
      break;
    case DOWN:
      printf("pressing DOWN\n");
      break;
    case LEFT:
      printf("pressing LEFT\n");
      break;
    case RIGHT:
      printf("pressing RIGHT\n");
      break;
    case A:
      printf("pressing A\n");
      break;
    case B:
      printf("pressing B\n");
      break;
    case START:
      printf("pressing START\n");
      break;
    case SELECT:
      printf("pressing SELECT\n");
      break;
    default:
      printf("pressing unknown key\n");
      break;
  }
}

