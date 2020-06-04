#include "read_rom.h"

void ReadROM::writeROMBank(uint8_t bank_no) {
  writeROMBank(bank_no, default_path + std::to_string(bank_no) + ".bin");
}

void ReadROM::writeROMBank(uint8_t bank_no, std::string filename) {
  uint8_t* data = readROMBank(bank_no);

  std::ofstream of;
  of.open(filename, std::ios_base::binary);
  if (of) {
    std::cout << "writing to " << filename << "\n";
    of.write((const char*)data, 1024*16);
    of.close();
  } else {
      throw std::runtime_error("Unable to open the output file.");
  }
}

uint8_t* ReadROM::readROMBank(uint8_t bank_no) {
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

void ReadROM::writeRAMBank(uint8_t bank_no) {
  writeRAMBank(bank_no, default_path + "RAM_" + std::to_string(bank_no) + ".bin");
}

void ReadROM::writeRAMBank(uint8_t bank_no, std::string filename) {
  uint8_t* data = readRAMBank(bank_no);

  std::ofstream of;
  of.open(filename, std::ios_base::binary);
  if (of) {
    std::cout << "writing to " << filename << "\n";
    of.write((const char*)data, 1024*8);
    of.close();
  } else {
      throw std::runtime_error("Unable to open the output file.");
  }
}

uint8_t* ReadROM::readRAMBank(uint8_t bank_no) {
  if (bank_no > 0x03) {
    throw "invalid bank_no";
  }

  gbWriteMemory(0x6000, 0x01); // Switch to RAM mode
  gbWriteMemory(0x4000, bank_no); // Enable RAM bank
  gbWriteMemory(0x0000, 0x0A); // Enable RAM
  uint16_t addr = 0xA000;
  int data_size = 8*1024;
  uint8_t* data = new uint8_t[data_size];
  
  for (int i = 0; i < data_size; i++) {
    data[i] = gbReadMemory(addr++);
  }

  gbWriteMemory(0x0000, 0x00); // Disable RAM
  gbWriteMemory(0x6000, 0x00); // Switch to ROM mode
  
  return data;
}
