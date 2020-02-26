#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <map>
#include "test.h"
// #include "../gb/gbGlobals.h"
// #include "../gb/gb.h"
#include "./map/tile_drawer.h"
#include "./util/common.h"
#include "./util/PokeMemViewer.h"
#include "./util/globals.h"
#include "./util/read_rom.h"
#include <chrono> 
#include <windows.h>


void aiStart() {
  std::cout << "In thread!\n";
  int x;
  while(1) {
    std::cin >> x;
    startGame();
  }
}

void startGame() {
  // Game game;
  game.start();
}

void inputTest() {
  PokeMemViewer mv;
  // uint8_t** tiles = mv.getTilesAtAddr(0x8000, 256);
  // BMPBuilder td(tiles, 256);
  // td.draw();

  auto start = std::chrono::high_resolution_clock::now();
  uint8_t** tiles = mv.getTilesOnScreen();
  BMPBuilder td(tiles, 18*20);
  td.write_screen();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
  std::cout << duration.count() << std::endl;
}

void viewPos() {
  PokeMemViewer mv;
  Pos p = mv.getPos();
  printf("x: %u\n", p.x);
  printf("y: %u\n", p.y);
}

void simulateInput(int x) {
  Game game;
  std::cout << "Ready...\n";
  // Pause for 5 seconds.
  Sleep(5000);
  game.testMovement1();
  std::cout << "Done\n";
}