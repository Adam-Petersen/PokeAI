#include "test.h"

void aiStart() {
  std::cout << "In thread!\n";
  int x;
  while(1) {
    std::cin >> x;

    if (x == 2) {
      viewPos();
    } else {
      Sleep(2000);
      startGame();
    }
  }
}

void startGame() {
  game.start();
  // ReadROM::writeROMBank(0x06);
  // ReadROM::writeRAMBank(0x03);
  // printf("wrote ROM\n");
}

void inputTest() {
  PokeMemViewer mv;
  // uint8_t** tiles = mv.getTilesAtAddr(0x8000, 256);
  // BMPBuilder td(tiles, 256);
  // td.draw();

  // auto start = std::chrono::high_resolution_clock::now();
  // uint8_t** tiles = mv.getTilesOnScreen();
  // BMPBuilder td(tiles, 18*20);
  // td.write_screen();
  // auto stop = std::chrono::high_resolution_clock::now();
  // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
  // std::cout << duration.count() << std::endl;
}

void viewPos() {
  Pos p = game.getGlobalPos();
  printf("Global pos: (%d, %d)\n", p.x, p.y);
}

void simulateInput(int x) {
  Game game;
  std::cout << "Ready...\n";
  // Pause for 5 seconds.
  Sleep(5000);
  game.testMovement1();
  std::cout << "Done\n";
}