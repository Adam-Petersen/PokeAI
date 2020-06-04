#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>
// #include "../gb/gbGlobals.h"

struct Pos {
  int x;
  int y;

  const inline bool operator==(const Pos& pos) {
    return (x == pos.x && y == pos.y);
  }

  const inline bool operator!=(const Pos& pos) {
    return (x != pos.x || y != pos.y);
  }

  Pos(int x, int y) : x(x), y(y) {}
  Pos() : x(-1), y(-1) {}
};

namespace debug {
  void b() {
    printf("in break\n");
  }

  void print(const char* str) {

  }
}

#endif // COMMON_H