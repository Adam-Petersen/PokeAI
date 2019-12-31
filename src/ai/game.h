#ifndef GAME_H
#define GAME_H
#include <windows.h>
#include "common.h"
#include "world.h"
#include "PokeMemViewer.h"
#include "input.h"

class Game {
 public:
  Game();
  ~Game();
  void start();
  void updateWorld();
  void testMovement1();
 private:
  PokeMemViewer* pmv;
  World* world;
  InputHandler* input;
};

#endif // GAME_H