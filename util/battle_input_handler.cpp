#include "battle_input_handler.h"

void BattleInputHandler::hoverFight() {
  pressButton(B);
  pressButton(LEFT);
  pressButton(UP);
}

void BattleInputHandler::executeBattleInput(BattleInput move) {
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

void BattleInputHandler::attack(uint8_t move_offset) {
  pressButton(UP);
  pressButton(LEFT);
  pressButton(A);
  uint8_t cur_offset = pmv->getCurMenuItem();
  moveToItem(cur_offset, move_offset);
  pressButton(A);
}

void BattleInputHandler::useItem(uint8_t item_id) {
  pressButton(LEFT);
  pressButton(DOWN);
  pressButton(A);
  Sleep(500);
  useItem(item_id);
}

void BattleInputHandler::switchPokemon(uint8_t pokemon_offset) {
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

void BattleInputHandler::flee() {
  pressButton(RIGHT);
  pressButton(DOWN);
  pressButton(A);
  Sleep(1500);
  pressButton(A);
}