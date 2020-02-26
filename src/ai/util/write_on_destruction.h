#ifndef WRITE_ON_DESTRUCTION_H
#define WRITE_ON_DESTRUCTION_H
#include "common.h"
#include "globals.h"

// Kind of jank, but whatever
class WriteOnDestruction {
 public:
  WriteOnDestruction(uint16_t addr, uint8_t value) : addr(addr), value(value) {}

  ~WriteOnDestruction() {
    if (game.isStarted()) {
        game.handleMemoryWrite(addr, value);
    }
  }

 private:
  uint16_t addr;
  uint8_t value;
};

#endif // WRITE_ON_DESTRUCTION_H