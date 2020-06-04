#ifndef SRC_BATTLE_INPUT_HANDLER
#define SRC_BATTLE_INPUT_HANDLER
#include "input_handler.h"

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

class BattleInputHandler : public InputHandler {
 public:
  BattleInputHandler(PokeMemViewer* pmv) : InputHandler(pmv) {}

   // Battle inputs
  void executeBattleInput(BattleInput move);
  void hoverFight();

 private:
  // Battle methods
  void attack(uint8_t move_offset); // CC26 starts at 0
  void useItem(uint8_t id); // use CC36 + CC26 for item pos start at 0
  void switchPokemon(uint8_t pokemon_offset); // CC26 starts at 1
  void flee();
};


#endif