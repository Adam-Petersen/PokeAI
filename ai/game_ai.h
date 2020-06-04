#ifndef SRC_GAME_AI
#define SRC_GAME_AI
#include "../game/goal.h"
#include "../game/game_state.h"
#include "../util/common.h"
#include "../util/game_input_handler.h"


class GameAI {
 public:
  GameAI(GameInputHandler* input);
  
  void execGoal(GameState* state);
 private:
  void useHM(GameState* state);
  void learnHM(GameState* state);
  void switchInParty(GameState* state);
  void withdraw(GameState* state);
  void deposit(GameState* state);
  void buyItem(GameState* state);
  void useItemOnMon(GameState* state);

  GameInputHandler* input;
};

#endif