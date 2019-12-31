#ifndef INPUT_H
#define INPUT_H
#include <windows.h>
#include "common.h"
#include "PokeMemViewer.h"

// OPTIMAL DELAY: 280ms

enum KEY_PRESS {
  UP = 0x11,
  DOWN = 0x1F,
  LEFT = 0x1E,
  RIGHT = 0x20,
  A = 0x26,
  B = 0x25,
  START = 0x39,
  SELECT
};

struct Movement {
  int dist;
  KEY_PRESS dir;
};

class InputHandler {
 public:
  InputHandler(PokeMemViewer* mem) : mem(mem) {}
  ~InputHandler() { printf("destroying input handler \n"); }

  void executeMoves(Movement* moves, int size);
  void executeMove(Movement move);
  void singleMove(KEY_PRESS key);
  void addMove(Movement* move, int ind, KEY_PRESS dir, int dist);

 private:
  void pressKey(WORD w);
  void releaseKey(WORD w);
  void sendInput(WORD w, DWORD flags);

  PokeMemViewer* mem;
};

#endif // INPUT_H