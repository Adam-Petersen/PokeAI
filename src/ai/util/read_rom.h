#ifndef SRC_READ_ROM_H
#define SRC_READ_ROM_H

#include <string>
#include <fstream>
#include "../../gb/gbGlobals.h"
#include "../../gb/gb.h"
#include "../util/common.h"

class ReadROM {
 public:
  static void writeROMBank(uint8_t bank_no) {
    writeROMBank(bank_no, default_path + std::to_string(bank_no) + ".bin");
  }

  static void writeROMBank(uint8_t bank_no, std::string filename) {
    uint8_t* data = getROMBank(bank_no);

    std::ofstream of;
    of.open(filename, std::ios_base::binary);
    if (of) {
      of.write((const char*)data, 1024*16);
      of.close();
    } else {
        throw std::runtime_error("Unable to open the output file.");
    }
  }

  static uint8_t* getROMBank(uint8_t bank_no) {
    if (bank_no >= 0x40) {
      throw "invalid bank_no";
    }

    if (bank_no < 0x20) {
      gbWriteMemory(0x2000, bank_no);
    } else {
      gbWriteMemory(0x6000, 0x00); // Set ROM Mode
      gbWriteMemory(0x2000, bank_no & 0x1F); // Set lower 5 bits
      gbWriteMemory(0x4000, (bank_no & 0x60) >> 5); // Set upper 2 bits
    }

    uint16_t addr = 0x4000;
    int data_size = 16*1024;
    uint8_t* data = new uint8_t[data_size];
    
    for (int i = 0; i < data_size; i++) {
      data[i] = gbReadMemory(addr++);
    }
    
    return data;
  }

 private:
  static std::string default_path;
};

std::string ReadROM::default_path = "D:\\temp\\";

#endif