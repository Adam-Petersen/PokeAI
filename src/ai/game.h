#ifndef GAME_H
#define GAME_H
#include <windows.h>
#include "./battle/battle_manager.h"
#include "./map/world.h"
#include "./util/common.h"
#include "./util/PokeMemViewer.h"
#include "./util/input.h"

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
 private:
  bool is_started{false};
  PokeMemViewer* pmv;
  World* world;
  InputHandler* input;
  BattleManager* bm;
};

#endif // GAME_H