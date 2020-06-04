#ifndef SRC_GAME_MANAGER
#define SRC_GAME_MANAGER
#include <vector>
#include "goal.h"
#include "../ai/game_ai.h"
#include "../map/world.h"
#include "../util/common.h"
#include "../util/game_input_handler.h"
#include "../util/poke_mem_viewer.h"


class GameManager {
 public:
  GameManager(PokeMemViewer* pmv, World* world);
  ~GameManager();

  void beatChapter(Chapter* c);

 private:
  void initGameState();
  void updateGameState();

  GameAI* ai;
  std::vector<Chapter> chapters;
  PokeMemViewer* pmv;
  World* world;
  GameInputHandler* input;
  GameState* state;
};

#endif