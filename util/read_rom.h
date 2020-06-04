#ifndef SRC_READ_ROM_H
#define SRC_READ_ROM_H
#include <fstream>
#include <string>
#include "common.h"
#include "../data/move.h"
#include "../../gb/gbGlobals.h"
#include "../../gb/gb.h"

class ReadROM {
 public:
  static void writeROMBank(uint8_t bank_no);
  static void writeROMBank(uint8_t bank_no, std::string filename);
  static uint8_t* readROMBank(uint8_t bank_no);
  static void writeRAMBank(uint8_t bank_no);
  static void writeRAMBank(uint8_t bank_no, std::string filename);
  static uint8_t* readRAMBank(uint8_t bank_no);

 private:
  static std::string default_path;
};

std::string ReadROM::default_path = "D:\\temp\\";

#endif