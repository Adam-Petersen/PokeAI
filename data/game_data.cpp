#include "game_data.h"
std::map<uint8_t, uint8_t> GameData::index_to_dex_map;
std::map<uint8_t, PokemonBase*> GameData::dex_to_base_map;
Move GameData::move_list[0xA6];
double GameData::type_matrix[15][15];
std::map<MoveType, int> GameData::type_to_ind_map;
std::vector<Pokemon> GameData::boxed_mons;
std::vector<ROMHeader> GameData::map_data_info;
std::vector<ROMHeader> GameData::tileset_data_info;



void GameData::initGameData() {
  initIndexMap();
  initMoveList();
  initTypeMatrix();
  initBasePokemon();
  // printBoxedMons();
  initMapData();
  initTilesetData();
}


void GameData::initMoveList() {
  int addr = 0;
  uint8_t* rom = ReadROM::readROMBank(0x0E);
  for (uint8_t i = 0x01; i <= 0xA5; i++) {
    Move m;
    m.setId(rom[addr++]);
    m.setEffect(rom[addr++]);
    m.setPower(rom[addr++]);
    m.setType(static_cast<MoveType>(rom[addr++]));
    m.setAccuracy((rom[addr++] * 1.0) / (0xFF * 1.0));
    m.setPP(rom[addr++]);
    move_list[i] = m;
  }
}

void GameData::initBasePokemon() {
  int addr = 0x31D8;
  uint8_t* rom = ReadROM::readROMBank(0x0E);
  uint8_t cur_mon = 0x01;

  // init base pokemon
  while (addr <= 0x39EB) {
    // printf("New EvosAndMoves, mon: %X\n", cur_mon);
    PokemonBase* pb = new PokemonBase();
    pb->setIndex(cur_mon);
    pb->setDexNo(index_to_dex_map[cur_mon]);
    
    bool has_evos = rom[addr] != 0;
    while (has_evos) {
      // printf("New Evolution\n");
      EvoInfo ei;
      ei.type = static_cast<EvoType>(rom[addr++]);
      if (ei.type == EV_LEVEL || ei.type == EV_TRADE) {
        ei.level = rom[addr++];
        ei.new_mon_id = rom[addr++];
      } else if (ei.type == EV_ITEM) {
        ei.item_id = rom[addr++];
        ei.level = rom[addr++]; // always 1
        ei.new_mon_id = rom[addr++];
      } else {
        printf("Unknown evolution type %d\n", ei.type);
        throw;
      }
      pb->addLvlEvoPair(ei.level, ei);
      has_evos = rom[addr] != 0; // should only ever repeat for EVEE
    }
    addr++;

    bool has_new_move = rom[addr] != 0;
    while (has_new_move) {
      // printf("New Move\n");
      uint8_t lvl = rom[addr++];
      uint8_t move_id = rom[addr++];
      Move move = move_list[move_id];
      pb->addLvlMovePair(lvl, move);
      has_new_move = rom[addr] != 0;
    }
    addr++;

    // printf("adding %d\n", pb->getDexNo());
    dex_to_base_map[pb->getDexNo()] = pb;
    cur_mon++;
  }

  // add base stats and tm
  // printf("adding base stats\n");
  addr = 0x3DE;
  uint8_t masks[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

  for (uint8_t i = 1; i <= 150; i++) {
    // printf("i: %d\n", i);
    PokemonBase* pb = dex_to_base_map[i];
    addr++; // skip dex no
    int max_hp = rom[addr++];
    int attack = rom[addr++];
    int defense = rom[addr++];
    int speed = rom[addr++];
    int special = rom[addr++];
    pb->setBaseStats(max_hp, attack, defense, speed, special);
    pb->setType1(rom[addr++]);
    pb->setType2(rom[addr++]);
    pb->setCatchRate(rom[addr++]);
    addr += 11; // get to TM flags
    int tm = 1;
    for (int j = 0; j < 7; j++) {
      uint8_t flag = rom[addr++];
      for (int k = 0; k < 8; k++) {
        pb->setTMFlag(tm++, flag & masks[k]);
      }
    }
    addr++; // skip padding
  }
}

void GameData::initTypeMatrix() {
  std::string** temp_matrix = new std::string*[15];
  for (int i = 0; i < 15; i++) {
    temp_matrix[i] = new std::string[15];
    for (int j = 0; j < 15; j++) {
      temp_matrix[i][j] = "";
    }
  }
  // printf("temp matrix made\n");

  int i = 0;
  int j = 0;
  for (int k = 0; k < (int) type_string.length(); k++) {
    char c = type_string.at(k);
    if (c == '\n') {
      i++;
      j = 0;
    } else if (c == ',') {
      j++;
    } else {
      temp_matrix[i][j] += c;
    }
  }

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      type_matrix[i][j] = std::stod(temp_matrix[i][j]);
    }
    delete [] temp_matrix[i];
  }
  delete [] temp_matrix;

  type_to_ind_map[NORMAL] = 0;
  type_to_ind_map[FIRE] = 1;
  type_to_ind_map[WATER] = 2;
  type_to_ind_map[ELECTRIC] = 3;
  type_to_ind_map[GRASS] = 4;
  type_to_ind_map[ICE] = 5;
  type_to_ind_map[FIGHTING] = 6;
  type_to_ind_map[POISON] = 7;
  type_to_ind_map[GROUND] = 8;
  type_to_ind_map[FLYING] = 9;
  type_to_ind_map[PSYCHIC] = 10;
  type_to_ind_map[BUG] = 11;
  type_to_ind_map[ROCK] = 12;
  type_to_ind_map[GHOST] = 13;
  type_to_ind_map[DRAGON] = 14;
}


void GameData::updateBoxedMons(uint8_t cur_box) {
  // printf("in updateBoxedMons\n");
  // ReadROM::writeRAMBank(0x01);
  // ReadROM::writeRAMBank(0x02);
  // ReadROM::writeRAMBank(0x03);
  uint8_t* bank1 = ReadROM::readRAMBank(0x01);
  uint8_t* bank2 = ReadROM::readRAMBank(0x02);
  uint8_t* bank3 = ReadROM::readRAMBank(0x03);
  // printf("banks are made\n");
  uint8_t box;
  uint16_t cur_box_addr = 0x10C0;

  for (uint8_t i = 0; i < 12; i++) {
    // printf("i: %d\n", i);
    box = i+1;
    uint8_t* bank = (box == cur_box) ? bank1 : ((box <= 6) ? bank2 : bank3); // get correct bank
    uint16_t addr = (box == cur_box) ? cur_box_addr : (i % 6) * 0x462; // start at correct box
    int num_mons = bank[addr];
    // printf("addr: %X, num_mons: %d\n", addr, num_mons);
    addr += 22; // skip count, species, padding
    for (int j = 0; j < num_mons; j++) {
      // printf("j: %d\n", j);
      Pokemon mon;
      mon.setIsBoxed(true);
      mon.setBox(box);
      mon.setBoxOffset(j);
      mon.setBase(bank[addr++]);
      uint16_t hp = (bank[addr] << 8) | bank[addr+1];
      mon.setCurHp(hp);
      addr+=2;
      mon.setLevel(bank[addr++]);
      mon.setStatuses(bank[addr++]);
      addr += 3; // types and held item
    
      int num_moves = 0;
      uint8_t moves[4];
      for (int i = 0; i < 4; i++) {
        moves[i] = bank[addr++];
        if (moves[i] != 0) {
          num_moves++;
        }
      }

      addr += 17; // skip useless junk

      uint8_t pps[4];
      for (int i = 0; i < 4; i++) {
        pps[i] = bank[addr++];
      }
      mon.setMovesAndPP(num_moves, moves, pps);
      boxed_mons.push_back(mon);
    }
  }
  // printf("done\n");
}

void GameData::initIndexMap() {
  index_to_dex_map[0x01] = 112;
  index_to_dex_map[0x02] = 115;
  index_to_dex_map[0x03] = 32;
  index_to_dex_map[0x04] = 35;
  index_to_dex_map[0x05] = 21;
  index_to_dex_map[0x06] = 100;
  index_to_dex_map[0x07] = 34;
  index_to_dex_map[0x08] = 80;
  index_to_dex_map[0x09] = 2;
  index_to_dex_map[0x0A] = 103;
  index_to_dex_map[0x0B] = 108;
  index_to_dex_map[0x0C] = 102;
  index_to_dex_map[0x0D] = 88;
  index_to_dex_map[0x0E] = 94;
  index_to_dex_map[0x0F] = 29;
  index_to_dex_map[0x10] = 31;
  index_to_dex_map[0x11] = 104;
  index_to_dex_map[0x12] = 111;
  index_to_dex_map[0x13] = 131;
  index_to_dex_map[0x14] = 59;
  index_to_dex_map[0x15] = 151;
  index_to_dex_map[0x16] = 130;
  index_to_dex_map[0x17] = 90;
  index_to_dex_map[0x18] = 72;
  index_to_dex_map[0x19] = 92;
  index_to_dex_map[0x1A] = 123;
  index_to_dex_map[0x1B] = 120;
  index_to_dex_map[0x1C] = 9;
  index_to_dex_map[0x1D] = 127;
  index_to_dex_map[0x1E] = 114;
  index_to_dex_map[0x21] = 58;
  index_to_dex_map[0x22] = 95;
  index_to_dex_map[0x23] = 22;
  index_to_dex_map[0x24] = 16;
  index_to_dex_map[0x25] = 79;
  index_to_dex_map[0x26] = 64;
  index_to_dex_map[0x27] = 75;
  index_to_dex_map[0x28] = 113;
  index_to_dex_map[0x29] = 67;
  index_to_dex_map[0x2A] = 122;
  index_to_dex_map[0x2B] = 106;
  index_to_dex_map[0x2C] = 107;
  index_to_dex_map[0x2D] = 24;
  index_to_dex_map[0x2E] = 47;
  index_to_dex_map[0x2F] = 54;
  index_to_dex_map[0x30] = 96;
  index_to_dex_map[0x31] = 76;
  index_to_dex_map[0x33] = 126;
  index_to_dex_map[0x35] = 125;
  index_to_dex_map[0x36] = 82;
  index_to_dex_map[0x37] = 109;
  index_to_dex_map[0x39] = 56;
  index_to_dex_map[0x3A] = 86;
  index_to_dex_map[0x3B] = 50;
  index_to_dex_map[0x3C] = 128;
  index_to_dex_map[0x40] = 83;
  index_to_dex_map[0x41] = 48;
  index_to_dex_map[0x42] = 149;
  index_to_dex_map[0x46] = 84;
  index_to_dex_map[0x47] = 60;
  index_to_dex_map[0x48] = 124;
  index_to_dex_map[0x49] = 146;
  index_to_dex_map[0x4A] = 144;
  index_to_dex_map[0x4B] = 145;
  index_to_dex_map[0x4C] = 132;
  index_to_dex_map[0x4D] = 52;
  index_to_dex_map[0x4E] = 98;
  index_to_dex_map[0x52] = 37;
  index_to_dex_map[0x53] = 38;
  index_to_dex_map[0x54] = 25;
  index_to_dex_map[0x55] = 26;
  index_to_dex_map[0x58] = 147;
  index_to_dex_map[0x59] = 148;
  index_to_dex_map[0x5A] = 140;
  index_to_dex_map[0x5B] = 141;
  index_to_dex_map[0x5C] = 116;
  index_to_dex_map[0x5D] = 117;
  index_to_dex_map[0x60] = 27;
  index_to_dex_map[0x61] = 28;
  index_to_dex_map[0x62] = 138;
  index_to_dex_map[0x63] = 139;
  index_to_dex_map[0x64] = 39;
  index_to_dex_map[0x65] = 40;
  index_to_dex_map[0x66] = 133;
  index_to_dex_map[0x67] = 136;
  index_to_dex_map[0x68] = 135;
  index_to_dex_map[0x69] = 134;
  index_to_dex_map[0x6A] = 66;
  index_to_dex_map[0x6B] = 41;
  index_to_dex_map[0x6C] = 23;
  index_to_dex_map[0x6D] = 46;
  index_to_dex_map[0x6E] = 61;
  index_to_dex_map[0x6F] = 62;
  index_to_dex_map[0x70] = 13;
  index_to_dex_map[0x71] = 14;
  index_to_dex_map[0x72] = 15;
  index_to_dex_map[0x74] = 85;
  index_to_dex_map[0x75] = 57;
  index_to_dex_map[0x76] = 51;
  index_to_dex_map[0x77] = 49;
  index_to_dex_map[0x78] = 87;
  index_to_dex_map[0x7B] = 10;
  index_to_dex_map[0x7C] = 11;
  index_to_dex_map[0x7D] = 12;
  index_to_dex_map[0x7E] = 68;
  index_to_dex_map[0x80] = 55;
  index_to_dex_map[0x81] = 97;
  index_to_dex_map[0x82] = 42;
  index_to_dex_map[0x83] = 150;
  index_to_dex_map[0x84] = 143;
  index_to_dex_map[0x85] = 129;
  index_to_dex_map[0x88] = 89;
  index_to_dex_map[0x8A] = 99;
  index_to_dex_map[0x8B] = 91;
  index_to_dex_map[0x8D] = 101;
  index_to_dex_map[0x8E] = 36;
  index_to_dex_map[0x8F] = 110;
  index_to_dex_map[0x90] = 53;
  index_to_dex_map[0x91] = 105;
  index_to_dex_map[0x93] = 93;
  index_to_dex_map[0x94] = 63;
  index_to_dex_map[0x95] = 65;
  index_to_dex_map[0x96] = 17;
  index_to_dex_map[0x97] = 18;
  index_to_dex_map[0x98] = 121;
  index_to_dex_map[0x99] = 1;
  index_to_dex_map[0x9A] = 3;
  index_to_dex_map[0x9B] = 73;
  index_to_dex_map[0x9D] = 118;
  index_to_dex_map[0x9E] = 119;
  index_to_dex_map[0xA3] = 77;
  index_to_dex_map[0xA4] = 78;
  index_to_dex_map[0xA5] = 19;
  index_to_dex_map[0xA6] = 20;
  index_to_dex_map[0xA7] = 33;
  index_to_dex_map[0xA8] = 30;
  index_to_dex_map[0xA9] = 74;
  index_to_dex_map[0xAA] = 137;
  index_to_dex_map[0xAB] = 142;
  index_to_dex_map[0xAD] = 81;
  index_to_dex_map[0xB0] = 4;
  index_to_dex_map[0xB1] = 7;
  index_to_dex_map[0xB2] = 5;
  index_to_dex_map[0xB3] = 8;
  index_to_dex_map[0xB4] = 6;
  index_to_dex_map[0xB9] = 43;
  index_to_dex_map[0xBA] = 44;
  index_to_dex_map[0xBB] = 45;
  index_to_dex_map[0xBC] = 69;
  index_to_dex_map[0xBD] = 70;
  index_to_dex_map[0xBE] = 71;
}

void GameData::initMapData() {
  map_data_info.push_back(ROMHeader(0x0, 0x182A1, "Pallet Town"));
  map_data_info.push_back(ROMHeader(0x1, 0x18357, "Viridian City"));
  map_data_info.push_back(ROMHeader(0x2, 0x18554, "Pewter City"));
  map_data_info.push_back(ROMHeader(0x3, 0x1874E, "Cerulean City"));
  map_data_info.push_back(ROMHeader(0x4, 0x44000, "Lavender Town"));
  map_data_info.push_back(ROMHeader(0x5, 0x18998, "Vermilion City"));
  map_data_info.push_back(ROMHeader(0x6, 0x18000, "Celadon City"));
  map_data_info.push_back(ROMHeader(0x7, 0x18BA7, "Fuchsia City"));
  map_data_info.push_back(ROMHeader(0x8, 0x1C000, "Cinnabar Island"));
  map_data_info.push_back(ROMHeader(0x9, 0x5091E, "Indigo Plateau"));
  map_data_info.push_back(ROMHeader(0xa, 0x509A4, "Saffron City"));
  map_data_info.push_back(ROMHeader(0xc, 0x1C0C3, "Route 1"));
  map_data_info.push_back(ROMHeader(0xd, 0x54000, "Route 2"));
  map_data_info.push_back(ROMHeader(0xe, 0x541E6, "Route 3"));
  map_data_info.push_back(ROMHeader(0xf, 0x54390, "Route 4"));
  map_data_info.push_back(ROMHeader(0x10, 0x54581, "Route 5"));
  map_data_info.push_back(ROMHeader(0x11, 0x58000, "Route 6"));
  map_data_info.push_back(ROMHeader(0x12, 0x48000, "Route 7"));
  map_data_info.push_back(ROMHeader(0x13, 0x5812D, "Route 8"));
  map_data_info.push_back(ROMHeader(0x14, 0x54686, "Route 9"));
  map_data_info.push_back(ROMHeader(0x15, 0x582D4, "Route 10"));
  map_data_info.push_back(ROMHeader(0x16, 0x584BE, "Route 11"));
  map_data_info.push_back(ROMHeader(0x17, 0x5866D, "Route 12"));
  map_data_info.push_back(ROMHeader(0x18, 0x5480C, "Route 13"));
  map_data_info.push_back(ROMHeader(0x19, 0x54999, "Route 14"));
  map_data_info.push_back(ROMHeader(0x1a, 0x5892C, "Route 15"));
  map_data_info.push_back(ROMHeader(0x1b, 0x58ADA, "Route 16"));
  map_data_info.push_back(ROMHeader(0x1c, 0x54B20, "Route 17"));
  map_data_info.push_back(ROMHeader(0x1d, 0x58C38, "Route 18"));
  map_data_info.push_back(ROMHeader(0x1e, 0x54E78, "Route 19"));
  map_data_info.push_back(ROMHeader(0x1f, 0x500F1, "Route 20"));
  map_data_info.push_back(ROMHeader(0x20, 0x54FFF, "Route 21"));
  map_data_info.push_back(ROMHeader(0x21, 0x50000, "Route 22"));
  map_data_info.push_back(ROMHeader(0x22, 0x5033F, "Route 23"));
  map_data_info.push_back(ROMHeader(0x23, 0x50682, "Route 24"));
  map_data_info.push_back(ROMHeader(0x24, 0x5079B, "Route 25"));
  map_data_info.push_back(ROMHeader(0x25, 0x4815C, "Red's house 1F"));
  map_data_info.push_back(ROMHeader(0x26, 0x5C0A4, "Red's house 2F"));
  map_data_info.push_back(ROMHeader(0x27, 0x19B2F, "Blue's house"));
  map_data_info.push_back(ROMHeader(0x28, 0x1CB02, "Professor Oak's Laboratory"));
  map_data_info.push_back(ROMHeader(0x29, 0x44251, "Viridian Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x2a, 0x1D462, "Viridian Poke Mart"));
  map_data_info.push_back(ROMHeader(0x2b, 0x1D540, "Pokemon academy"));
  map_data_info.push_back(ROMHeader(0x2c, 0x1D57D, "Viridian house"));
  map_data_info.push_back(ROMHeader(0x2d, 0x74897, "Viridian Gym"));
  map_data_info.push_back(ROMHeader(0x2e, 0x1DEA4, "Diglett's Cave Route 2 cave"));
  map_data_info.push_back(ROMHeader(0x2f, 0x5D57B, "Viridian Forest north gate"));
  map_data_info.push_back(ROMHeader(0x30, 0x1DEE1, "Route 2 house"));
  map_data_info.push_back(ROMHeader(0x31, 0x5D5C8, "Route 2 gate"));
  map_data_info.push_back(ROMHeader(0x32, 0x5D650, "Viridian Forest south gate"));
  map_data_info.push_back(ROMHeader(0x33, 0x61101, "Viridian Forest"));
  map_data_info.push_back(ROMHeader(0x34, 0x5C0EB, "Pewter Museum of Science 1F"));
  map_data_info.push_back(ROMHeader(0x35, 0x5C30B, "Pewter Museum of Science 2F"));
  map_data_info.push_back(ROMHeader(0x36, 0x5C37B, "Pewter Gym"));
  map_data_info.push_back(ROMHeader(0x37, 0x1D5E7, "Pewter NE house"));
  map_data_info.push_back(ROMHeader(0x38, 0x74CA1, "Pewter Poke Mart"));
  map_data_info.push_back(ROMHeader(0x39, 0x1D63C, "Pewter SW house"));
  map_data_info.push_back(ROMHeader(0x3a, 0x5C57B, "Pewter Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x3b, 0x499BC, "Mt. Moon 1F"));
  map_data_info.push_back(ROMHeader(0x3c, 0x51A36, "Mt. Moon B1F"));
  map_data_info.push_back(ROMHeader(0x3d, 0x49CFF, "Mt. Moon B2F"));
  map_data_info.push_back(ROMHeader(0x3e, 0x1D679, "Cerulean trashed house"));
  map_data_info.push_back(ROMHeader(0x3f, 0x1D6EA, "Cerulean trade house"));
  map_data_info.push_back(ROMHeader(0x40, 0x5C639, "Cerulean Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x41, 0x5C6A7, "Cerulean Gym"));
  map_data_info.push_back(ROMHeader(0x42, 0x1D730, "Bike Shop"));
  map_data_info.push_back(ROMHeader(0x43, 0x5C889, "Cerulean Poke Mart"));
  map_data_info.push_back(ROMHeader(0x44, 0x492C3, "Mt. Moon Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x46, 0x1DF27, "Route 5 gate"));
  map_data_info.push_back(ROMHeader(0x47, 0x5D69D, "Route 5 Underground Path entrance"));
  map_data_info.push_back(ROMHeader(0x48, 0x56243, "Pokemon Day Care"));
  map_data_info.push_back(ROMHeader(0x49, 0x1E031, "Route 6 gate"));
  map_data_info.push_back(ROMHeader(0x4a, 0x5D6E3, "Route 6 Underground Path entrance"));
  map_data_info.push_back(ROMHeader(0x4c, 0x1E0F4, "Route 7 gate"));
  map_data_info.push_back(ROMHeader(0x4d, 0x5D720, "Route 7 Underground Path entrance"));
  map_data_info.push_back(ROMHeader(0x4f, 0x1E1BB, "Route 8 gate"));
  map_data_info.push_back(ROMHeader(0x50, 0x1E27D, "Route 8 Underground Path entrance"));
  map_data_info.push_back(ROMHeader(0x51, 0x493AE, "Rock Tunnel Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x52, 0x444D0, "Rock Tunnel 1F"));
  map_data_info.push_back(ROMHeader(0x53, 0x1E2BA, "Power Plant"));
  map_data_info.push_back(ROMHeader(0x54, 0x49400, "Route 11 gate 1F"));
  map_data_info.push_back(ROMHeader(0x55, 0x1E5AE, "Diglett's Cave Route 11 cave"));
  map_data_info.push_back(ROMHeader(0x56, 0x49448, "Route 11 gate 2F"));
  map_data_info.push_back(ROMHeader(0x57, 0x494F8, "Route 12 gate 1F"));
  map_data_info.push_back(ROMHeader(0x58, 0x1E75E, "Bill's house"));
  map_data_info.push_back(ROMHeader(0x59, 0x5C983, "Vermilion Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x5a, 0x59B64, "Pokemon Fan Club"));
  map_data_info.push_back(ROMHeader(0x5b, 0x5C9D5, "Vermilion Poke Mart"));
  map_data_info.push_back(ROMHeader(0x5c, 0x5CA1A, "Vermilion Gym"));
  map_data_info.push_back(ROMHeader(0x5d, 0x1DAF0, "Vermilion SE house"));
  map_data_info.push_back(ROMHeader(0x5e, 0x1DB46, "Vermilion harbor"));
  map_data_info.push_back(ROMHeader(0x5f, 0x61259, "S.S. Anne 1F corridor"));
  map_data_info.push_back(ROMHeader(0x60, 0x61393, "S.S. Anne 2F corridor"));
  map_data_info.push_back(ROMHeader(0x61, 0x44926, "S.S. Anne corridor to deck"));
  map_data_info.push_back(ROMHeader(0x62, 0x61622, "S.S. Anne B1F corridor"));
  map_data_info.push_back(ROMHeader(0x63, 0x616A2, "S.S. Anne deck"));
  map_data_info.push_back(ROMHeader(0x64, 0x617A7, "S.S. Anne kitchen"));
  map_data_info.push_back(ROMHeader(0x65, 0x61889, "S.S. Anne Captain's room"));
  map_data_info.push_back(ROMHeader(0x66, 0x6196A, "S.S. Anne 1F rooms"));
  map_data_info.push_back(ROMHeader(0x67, 0x61B3F, "S.S. Anne 2F rooms"));
  map_data_info.push_back(ROMHeader(0x68, 0x61D49, "S.S. Anne B1F rooms"));
  map_data_info.push_back(ROMHeader(0x6c, 0x5D9FE, "Victory Road 1F"));
  map_data_info.push_back(ROMHeader(0x71, 0x5A2A2, "Indigo Plateau Lance's room"));
  map_data_info.push_back(ROMHeader(0x76, 0x5A492, "Indigo Plateau Hall of Fame room"));
  map_data_info.push_back(ROMHeader(0x77, 0x61F1A, "Route 5-6 Underground Path"));
  map_data_info.push_back(ROMHeader(0x78, 0x75F11, "Indigo Plateau Champion's room"));
  map_data_info.push_back(ROMHeader(0x79, 0x61F3E, "Route 7-8 Underground Path"));
  map_data_info.push_back(ROMHeader(0x7a, 0x60F7A, "Celadon Department Store 1F"));
  map_data_info.push_back(ROMHeader(0x7b, 0x560E9, "Celadon Department Store 2F"));
  map_data_info.push_back(ROMHeader(0x7c, 0x48219, "Celadon Department Store 3F"));
  map_data_info.push_back(ROMHeader(0x7d, 0x4834A, "Celadon Department Store 4F"));
  map_data_info.push_back(ROMHeader(0x7e, 0x483C9, "Celadon Department Store roof"));
  map_data_info.push_back(ROMHeader(0x7f, 0x485F4, "Celadon Department Store elevator"));
  map_data_info.push_back(ROMHeader(0x80, 0x48688, "Celadon Mansion 1F"));
  map_data_info.push_back(ROMHeader(0x81, 0x4872E, "Celadon Mansion 2F"));
  map_data_info.push_back(ROMHeader(0x82, 0x48784, "Celadon Mansion 3F"));
  map_data_info.push_back(ROMHeader(0x83, 0x4885F, "Celadon Mansion rooftop"));
  map_data_info.push_back(ROMHeader(0x84, 0x1DD2E, "Celadon Mansion rooftop room"));
  map_data_info.push_back(ROMHeader(0x85, 0x488AC, "Celadon Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x86, 0x488FE, "Celadon Gym"));
  map_data_info.push_back(ROMHeader(0x87, 0x48BB1, "Celadon Game Corner"));
  map_data_info.push_back(ROMHeader(0x88, 0x4905D, "Celadon Department Store 5F"));
  map_data_info.push_back(ROMHeader(0x89, 0x490E4, "Game Corner Prize Exchange"));
  map_data_info.push_back(ROMHeader(0x8a, 0x49145, "Celadon diner"));
  map_data_info.push_back(ROMHeader(0x8b, 0x49202, "Celadon house"));
  map_data_info.push_back(ROMHeader(0x8c, 0x4925D, "Celadon hotel"));
  map_data_info.push_back(ROMHeader(0x8d, 0x5C8CE, "Lavender Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x8e, 0x60420, "Pokemon Tower 1F"));
  map_data_info.push_back(ROMHeader(0x8f, 0x604E6, "Pokemon Tower 2F"));
  map_data_info.push_back(ROMHeader(0x90, 0x606C0, "Pokemon Tower 3F"));
  map_data_info.push_back(ROMHeader(0x91, 0x607EA, "Pokemon Tower 4F"));
  map_data_info.push_back(ROMHeader(0x92, 0x60926, "Pokemon Tower 5F"));
  map_data_info.push_back(ROMHeader(0x93, 0x60AE3, "Pokemon Tower 6F"));
  map_data_info.push_back(ROMHeader(0x94, 0x60CF9, "Pokemon Tower 7F"));
  map_data_info.push_back(ROMHeader(0x95, 0x1D89C, "Lavender Volunteer Pokemon House"));
  map_data_info.push_back(ROMHeader(0x96, 0x5C920, "Lavender Poke Mart"));
  map_data_info.push_back(ROMHeader(0x97, 0x1D9A2, "Lavender SW house"));
  map_data_info.push_back(ROMHeader(0x98, 0x1DD7C, "Fuchsia Poke Mart"));
  map_data_info.push_back(ROMHeader(0x99, 0x7500C, "Fuchsia SW house"));
  map_data_info.push_back(ROMHeader(0x9a, 0x75057, "Fuchsia Pokemon Center"));
  map_data_info.push_back(ROMHeader(0x9b, 0x750A9, "Warden's home"));
  map_data_info.push_back(ROMHeader(0x9c, 0x751C1, "Safari Zone entrance"));
  map_data_info.push_back(ROMHeader(0x9d, 0x75431, "Fuchsia Gym"));
  map_data_info.push_back(ROMHeader(0x9e, 0x756D7, "Fuchsia Safari employee building"));
  map_data_info.push_back(ROMHeader(0x9f, 0x46309, "Seafoam Islands B1F"));
  map_data_info.push_back(ROMHeader(0xa0, 0x46445, "Seafoam Islands B2F"));
  map_data_info.push_back(ROMHeader(0xa1, 0x46581, "Seafoam Islands B3F"));
  map_data_info.push_back(ROMHeader(0xa2, 0x4678D, "Seafoam Islands B4F"));
  map_data_info.push_back(ROMHeader(0xa3, 0x56064, "Fishing Guru's house"));
  map_data_info.push_back(ROMHeader(0xa4, 0x56170, "Fishing Guru's older brother's house"));
  map_data_info.push_back(ROMHeader(0xa5, 0x442A3, "Pokemon Mansion 1F"));
  map_data_info.push_back(ROMHeader(0xa6, 0x7573E, "Cinnabar Gym"));
  map_data_info.push_back(ROMHeader(0xa7, 0x75B80, "Pokemon Lab entrance"));
  map_data_info.push_back(ROMHeader(0xa8, 0x75C15, "Pokemon Lab Meeting Room"));
  map_data_info.push_back(ROMHeader(0xa9, 0x75C7B, "Pokemon Lab R-and-D Room"));
  map_data_info.push_back(ROMHeader(0xaa, 0x75D25, "Pokemon Lab Testing Room"));
  map_data_info.push_back(ROMHeader(0xab, 0x75E20, "Cinnabar Pokemon Center"));
  map_data_info.push_back(ROMHeader(0xac, 0x75E72, "Cinnabar Poke Mart"));
  map_data_info.push_back(ROMHeader(0xae, 0x19C4F, "Indigo Plateau lobby"));
  map_data_info.push_back(ROMHeader(0xaf, 0x75EB7, "Copycat's house 1F"));
  map_data_info.push_back(ROMHeader(0xb0, 0x5CC65, "Copycat's house 2F"));
  map_data_info.push_back(ROMHeader(0xb1, 0x5CD51, "Fighting Dojo"));
  map_data_info.push_back(ROMHeader(0xb2, 0x5D001, "Saffron Gym"));
  map_data_info.push_back(ROMHeader(0xb3, 0x1DDD1, "Saffron NW house"));
  map_data_info.push_back(ROMHeader(0xb4, 0x5D3FD, "Saffron Poke Mart"));
  map_data_info.push_back(ROMHeader(0xb5, 0x5D442, "Silph Co. 1F"));
  map_data_info.push_back(ROMHeader(0xb6, 0x5D529, "Saffron Pokemon Center"));
  map_data_info.push_back(ROMHeader(0xb7, 0x1DE30, "Mr. Psychic's house"));
  map_data_info.push_back(ROMHeader(0xb8, 0x495F6, "Route 15 gate 1F"));
  map_data_info.push_back(ROMHeader(0xb9, 0x4963E, "Route 15 gate 2F"));
  map_data_info.push_back(ROMHeader(0xba, 0x496B2, "Route 16 gate 1F"));
  map_data_info.push_back(ROMHeader(0xbb, 0x497FF, "Route 16 gate 2F"));
  map_data_info.push_back(ROMHeader(0xbc, 0x1E5EC, "Route 16 house"));
  map_data_info.push_back(ROMHeader(0xbd, 0x56473, "Route 12 house"));
  map_data_info.push_back(ROMHeader(0xbe, 0x4986A, "Route 18 gate 1F"));
  map_data_info.push_back(ROMHeader(0xbf, 0x49969, "Route 18 gate 2F"));
  map_data_info.push_back(ROMHeader(0xc0, 0x447DD, "Seafoam Islands 1F"));
  map_data_info.push_back(ROMHeader(0xc1, 0x1E677, "Route 22 gate"));
  map_data_info.push_back(ROMHeader(0xc2, 0x51791, "Victory Road 2F"));
  map_data_info.push_back(ROMHeader(0xc3, 0x49554, "Route 12 gate 2F"));
  map_data_info.push_back(ROMHeader(0xc4, 0x19C06, "Vermilion trade house"));
  map_data_info.push_back(ROMHeader(0xc5, 0x61F62, "Diglett's Cave"));
  map_data_info.push_back(ROMHeader(0xc6, 0x44974, "Victory Road 3F"));
  map_data_info.push_back(ROMHeader(0xc7, 0x44BBE, "Rocket Hideout B1F"));
  map_data_info.push_back(ROMHeader(0xc8, 0x44E1B, "Rocket Hideout B2F"));
  map_data_info.push_back(ROMHeader(0xc9, 0x45219, "Rocket Hideout B3F"));
  map_data_info.push_back(ROMHeader(0xca, 0x45451, "Rocket Hideout B4F"));
  map_data_info.push_back(ROMHeader(0xcb, 0x45704, "Rocket Hideout elevator"));
  map_data_info.push_back(ROMHeader(0xcf, 0x59CE5, "Silph Co. 2F"));
  map_data_info.push_back(ROMHeader(0xd0, 0x59F4F, "Silph Co. 3F"));
  map_data_info.push_back(ROMHeader(0xd1, 0x19CFF, "Silph Co. 4F"));
  map_data_info.push_back(ROMHeader(0xd2, 0x19F2B, "Silph Co. 5F"));
  map_data_info.push_back(ROMHeader(0xd3, 0x1A19D, "Silph Co. 6F"));
  map_data_info.push_back(ROMHeader(0xd4, 0x51B55, "Silph Co. 7F"));
  map_data_info.push_back(ROMHeader(0xd5, 0x564F8, "Silph Co. 8F"));
  map_data_info.push_back(ROMHeader(0xd6, 0x51FCC, "Pokemon Mansion 2F"));
  map_data_info.push_back(ROMHeader(0xd7, 0x521E2, "Pokemon Mansion 3F"));
  map_data_info.push_back(ROMHeader(0xd8, 0x523AD, "Pokemon Mansion B1F"));
  map_data_info.push_back(ROMHeader(0xd9, 0x4585F, "Safari Zone east"));
  map_data_info.push_back(ROMHeader(0xda, 0x4599F, "Safari Zone north"));
  map_data_info.push_back(ROMHeader(0xdb, 0x4A1A9, "Safari Zone west"));
  map_data_info.push_back(ROMHeader(0xdc, 0x45BA6, "Safari Zone center"));
  map_data_info.push_back(ROMHeader(0xdd, 0x45CE1, "Safari Zone center rest house"));
  map_data_info.push_back(ROMHeader(0xde, 0x4A30B, "Safari Zone secret house"));
  map_data_info.push_back(ROMHeader(0xdf, 0x45D1E, "Safari Zone west rest house"));
  map_data_info.push_back(ROMHeader(0xe0, 0x45D69, "Safari Zone east rest house"));
  map_data_info.push_back(ROMHeader(0xe1, 0x45DB4, "Safari Zone north rest house"));
  map_data_info.push_back(ROMHeader(0xe2, 0x45DFF, "Cerulean Cave 2F"));
  map_data_info.push_back(ROMHeader(0xe3, 0x45EE4, "Cerulean Cave B1F"));
  map_data_info.push_back(ROMHeader(0xe4, 0x74D00, "Cerulean Cave 1F"));
  map_data_info.push_back(ROMHeader(0xe5, 0x1DA06, "Name Rater's house"));
  map_data_info.push_back(ROMHeader(0xe6, 0x74DFD, "Cerulean badges house"));
  map_data_info.push_back(ROMHeader(0xe8, 0x45FDF, "Rock Tunnel B1F"));
  map_data_info.push_back(ROMHeader(0xe9, 0x5D7AF, "Silph Co. 9F"));
  map_data_info.push_back(ROMHeader(0xea, 0x5A12D, "Silph Co. 10F"));
  map_data_info.push_back(ROMHeader(0xeb, 0x620EE, "Silph Co. 11F"));
  map_data_info.push_back(ROMHeader(0xec, 0x457B4, "Silph Co. elevator"));
  map_data_info.push_back(ROMHeader(0xef, 0x4FD04, "Battle Center (Cable Club)"));
  map_data_info.push_back(ROMHeader(0xf0, 0x4FD71, "Trade Center (Cable Club)"));
  map_data_info.push_back(ROMHeader(0xf5, 0x7616F, "Indigo Plateau Lorelei's room"));
  map_data_info.push_back(ROMHeader(0xf6, 0x762CA, "Indigo Plateau Bruno's room"));
  map_data_info.push_back(ROMHeader(0xf7, 0x76421, "Indigo Plateau Agatha's room"));
}

void GameData::initTilesetData() {
  tileset_data_info.push_back(ROMHeader(0x00, 0xC7BE, "Outside"));
  tileset_data_info.push_back(ROMHeader(0x01, 0xC7CA, "Ash's House (#1)"));
  tileset_data_info.push_back(ROMHeader(0x02, 0xC7D6, "Pokemon Center (#1)"));
  tileset_data_info.push_back(ROMHeader(0x03, 0xC7E2, "Viridian Forest"));
  tileset_data_info.push_back(ROMHeader(0x04, 0xC7EE, "Ash's House (#2)"));
  tileset_data_info.push_back(ROMHeader(0x05, 0xC7FA, "Gym (#1)"));
  tileset_data_info.push_back(ROMHeader(0x06, 0xC806, "Pokemon Center (#2)"));
  tileset_data_info.push_back(ROMHeader(0x07, 0xC812, "Gym (#2)"));
  tileset_data_info.push_back(ROMHeader(0x08, 0xC81E, "House"));
  tileset_data_info.push_back(ROMHeader(0x09, 0xC82A, "Museum (#1)"));
  tileset_data_info.push_back(ROMHeader(0x0A, 0xC836, "Museum (#2)"));
  tileset_data_info.push_back(ROMHeader(0x0B, 0xC842, "Underground Path"));
  tileset_data_info.push_back(ROMHeader(0x0C, 0xC84E, "Museum (#3)"));
  tileset_data_info.push_back(ROMHeader(0x0D, 0xC85A, "S.S. Anne"));
  tileset_data_info.push_back(ROMHeader(0x0E, 0xC866, "Vermilion Port"));
  tileset_data_info.push_back(ROMHeader(0x0F, 0xC872, "Pokemon Cemetery"));
  tileset_data_info.push_back(ROMHeader(0x10, 0xC87E, "Silph Co."));
  tileset_data_info.push_back(ROMHeader(0x11, 0xC88A, "Cave"));
  tileset_data_info.push_back(ROMHeader(0x12, 0xC896, "Celadon Mart"));
  tileset_data_info.push_back(ROMHeader(0x13, 0xC8A2, "Game Freak HQ"));
  tileset_data_info.push_back(ROMHeader(0x14, 0xC8AE, "Lab"));
  tileset_data_info.push_back(ROMHeader(0x15, 0xC8BA, "Bike Shop&Cable Center"));
  tileset_data_info.push_back(ROMHeader(0x16, 0xC8C6, "Cinnabar Mansion&Power Plant etc"));
  tileset_data_info.push_back(ROMHeader(0x17, 0xC8D2, "Indigo Plateau"));
}

void GameData::printMoveList() {
  for (uint8_t i = 0x01; i <= 0xA5; i++) {
    move_list[i].print();
  }
}

void GameData::printBaseMons() {
  for (uint8_t i = 0x01; i <= 0xBE; i++) {
    printf("------------\n");
    dex_to_base_map[i]->print();
  }
}

void GameData::printTypeMatrix() {
  printf("-------Type Matrix----------\n");
  for(int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      printf("%1.1f ", type_matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void GameData::printBoxedMons() {
  for (auto mon : boxed_mons) {
    mon.print();
    printf("\n");
  }
}


void GameData::writeMoveList() {
  std::ofstream of;
  of.open("D:\\temp\\move_list.txt");
  if (!of) {
    throw std::runtime_error("Unable to open the output file.");
  }

  for (uint8_t i = 0x01; i <= 0xA5; i++) {
    of << move_list[i].getPrintString();
  }
  
  of.close();
  printf("Wrote move list\n");
}