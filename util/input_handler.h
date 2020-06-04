#ifndef INPUT_H
#define INPUT_H
#include <windows.h>
#include "../data/item.h"
#include "common.h"
#include "poke_mem_viewer.h"

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
  InputHandler(PokeMemViewer* pmv) : pmv(pmv) {}
  ~InputHandler() { printf("destroying input handler \n"); }

  void executeMoves(Movement* moves, int size);
  void executeMove(Movement move);
  void pressButton(KEY_PRESS key);
  void pressButton(KEY_PRESS key, int sleep);
  void addMove(Movement* move, int ind, KEY_PRESS dir, int dist);

 protected:
  void pressKey(WORD w);
  void releaseKey(WORD w);
  void sendInput(WORD w, DWORD flags);
  void printKey(KEY_PRESS key);
  void moveToItem(uint8_t cur_offset, uint8_t target_offset);
  void useItem(uint8_t item_id);
  void useItem(Item item);

  PokeMemViewer* pmv;
};

#endif // INPUT_H