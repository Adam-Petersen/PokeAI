#ifndef GAME_H
#define GAME_H
#include <windows.h>
#include <vector>
#include "game_manager.h"
#include "chapter_factory.h"
#include "../battle/battle_manager.h"
#include "../data/game_data.h"
#include "../map/world.h"
#include "../util/common.h"
#include "../util/poke_mem_viewer.h"
#include "../util/input_handler.h"

class PokeMemViewer;

class Game {
 public:
  Game();
  ~Game();
  void start();
  bool isStarted() { return is_started; }
  void updateWorld();
  void testMovement1();
  void handleMemoryWrite(uint16_t addr, uint8_t val);
  Pos getGlobalPos();
 private:
  bool is_started{false};
  PokeMemViewer* pmv;
  World* world;
  InputHandler* input;
  BattleManager* bm;
  GameManager* gm;
  Chapter** chapters;
  int num_chapters;

  void beatGame();
};

#endif // GAME_H