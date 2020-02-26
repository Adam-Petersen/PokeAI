#ifndef INPUT_H
#define INPUT_H
#include <windows.h>
#include "common.h"
#include "PokeMemViewer.h"

// OPTIMAL DELAY: 280ms

enum BattleInputType {
  BIT_ATTACK,
  BIT_SWITCH,
  BIT_USE,
  BIT_RUN,
  BIT_LEARN_MOVE,
  BIT_DONT_LEARN_MOVE,
  BIT_DONT_NAME,
  BIT_DONT_SWITCH,
  BIT_CHOOSE_NEXT,
};

struct BattleInput {
  BattleInputType type;
  uint8_t target;

  BattleInput() {}
  BattleInput(BattleInputType type, uint8_t target) : type(type), target(target) {}
};

enum KEY_PRESS {
  UP = 0x11,
  DOWN = 0x1F,
  LEFT = 0x1E,
  RIGHT = 0x20,
  A = 0x26,
  B = 0x25,
  START = 0x39,
  SELECT
};

struct Movement {
  int dist;
  KEY_PRESS dir;
};

class InputHandler {
 public:
  InputHandler(PokeMemViewer* pmv) : pmv(pmv) {}
  ~InputHandler() { printf("destroying input handler \n"); }

  void executeMoves(Movement* moves, int size);
  void executeMove(Movement move);
  void pressButton(KEY_PRESS key);
  void addMove(Movement* move, int ind, KEY_PRESS dir, int dist);

  // Battle inputs
  void executeBattleInput(BattleInput move);
  void hoverFight();

 private:
  void pressKey(WORD w);
  void releaseKey(WORD w);
  void sendInput(WORD w, DWORD flags);
  void printKey(KEY_PRESS key);

  // Battle methods
  void attack(uint8_t move_offset); // CC26 starts at 0
  void useItem(uint8_t id); // use CC36 + CC26 for item pos start at 0
  void switchPokemon(uint8_t pokemon_offset); // CC26 starts at 1
  void flee();
  void moveToItem(uint8_t cur_offset, uint8_t target_offset);



  PokeMemViewer* pmv;
};

#endif // INPUT_H