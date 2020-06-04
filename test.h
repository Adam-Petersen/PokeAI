#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <map>
#include "./map/bmp_builder.h"
#include "./util/common.h"
#include "./util/poke_mem_viewer.h"
#include "./util/globals.h"
#include "./util/read_rom.h"
#include <chrono> 
#include <windows.h>



void aiStart();
std::string readTileData(uint16_t addr, int size);
void simulateInput(int x);
void viewPos();
void readLevel();
void inputTest();
void startGame();

#endif // TEST_H