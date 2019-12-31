#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <map>
#include "test.h"
#include "../gb/gbGlobals.h"
#include "../gb/gb.h"
#include "game.h"
#include "PokeMemViewer.h"
#include "tile_drawer.h"
#include <chrono> 


void aiStart() {
  std::cout << "In thread!\n";
  // std::chrono::milliseconds timespan(1000); // or whatever
  int x;
  // bool started = false;
  // Game game;
  while(1) {
    std::cin >> x;
    startGame();
    // if (!started) {
    //   started = true;
    //   game.start();
    //   printf("game started\n");
    // } else {
    //   game.updateWorld();
    //   // printf("world updated\n");
    // }
    // // printf("DONEEEE\n");
  }
}

void startGame() {
  Game game;
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

std::string readTileData(uint16_t addr, int size) {
    char buf[100];
    int c = 0;
    for(int i = size - 1; i >= 0; i--) {
      uint8_t data = gbReadMemory(addr + i);
      c = c + snprintf(buf+c, 100-c, "%02X", data);
    }
    std::string s(buf);
    return s;
}


void readLevel() {
    uint8_t level = gbReadMemory(0xD18C);
    uint8_t h1 = gbReadMemory(0xD16C);
    uint8_t h2 = gbReadMemory(0xD16D);
    printf("Level: %u.\n", level);
    printf("H1: %u.\n", h1);
    printf("H2: %u.\n", h2);
}