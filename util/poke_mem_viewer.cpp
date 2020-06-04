#include "poke_mem_viewer.h"

PokeMemViewer::PokeMemViewer() {
  // monitoredAddresses[0xD361] = gbReadMemory(0xD361); // player y
  // monitoredAddresses[0xD362] = gbReadMemory(0xD362); // player x
  // monitoredAddresses[0xD35E] = gbReadMemory(0xD35E); // current map number
  // // printf("added addresses... monigbReadMemorytoredAddress size: %llu\n", monitoredAddresses.size());

  // monitoredAddressCVS[0xD362] = &player_pos_cv;
}


  Pos PokeMemViewer::getPos() {
    Pos p;
    p.x = gbReadMemory(0xD362);
    p.y = gbReadMemory(0xD361);
    return p;
  }

  void PokeMemViewer::printTileset() {}

  void PokeMemViewer::printMap() {
    uint16_t addr = 0xC3A0;
    int x = 1;
    int y = 1;
    for (int i = 0; i < 18; i++) {
      for (int j = 0; j < 20; j++) {
        uint8_t data = gbReadMemory(addr);
        addr += 1;
        if (x++%2 == 0) {
          printf("%03u ", data);
        } else {
          printf("%03u", data);
        }
      }
      if (y++%2 == 0) {
        printf("\n\n");
      } else {
        printf("\n");
      }
    }
    printf("----------------------------------------------\n");
}

void PokeMemViewer::printCollisions() {
  uint8_t lower = gbReadMemory(0xD530);
  uint8_t upper = gbReadMemory(0xD531);
  uint16_t ptr = (upper << 8) | lower;

  
  uint8_t bit = gbReadMemory(ptr);
  while (bit != 0xFF) {
    printf("%02X\n", bit);
    ptr += 1;
    bit = gbReadMemory(ptr);
  }
  printf("\n");

}

void PokeMemViewer::printTileCollisionData() {
  uint8_t* collisions;
  int col_size = setCollisions(&collisions);

  uint8_t** tiles = getTiles();

  int x = 1;
  int y = 1;
  for (int i = 0; i < tile_rows; i++) {
    for (int j = 0; j < tile_cols; j++) {
      bool col = collides(tiles[i][j], collisions, col_size);
      if (x++%2 == 0) {
        printf("%d ", col);
      } else {
        printf("%d", col);
      }
    }
    if (y++%2 == 0) {
      printf("\n\n");
    } else {
      printf("\n");
    }
  }

  delete2DArray(tiles, tile_rows);
  delete [] collisions;
}

void PokeMemViewer::printSpriteData() {
  uint8_t** sprite_data = getSpriteData();
  printf("Sprite Data:\n");

  for (int i = 0; i < num_sprites; i++) {
    printf("Sprite %d:\n", i);
    printf("Picture ID     : %u\n", sprite_data[i][0]);
    printf("Movement Status: %u\n", sprite_data[i][1]);
    printf("Image Index    : %u\n", sprite_data[i][2]);
    printf("Y delta        : %u\n", sprite_data[i][3]);
    printf("Y position     : %u\n", sprite_data[i][4]);
    printf("X delta        : %u\n", sprite_data[i][5]);
    printf("X position     : %u\n", sprite_data[i][6]);
    printf("intra-frame    : %u\n", sprite_data[i][7]);
    printf("animation-fame : %u\n", sprite_data[i][8]);
    printf("facing-dir     : %u\n", sprite_data[i][9]);
    printf("\n");
  }

  delete2DArray(sprite_data, num_sprites);
}

void PokeMemViewer::printMapData() {
  printf("\n--- Current Map ---\n");
  printf("Map's tileset: %u\n", gbReadMemory(0xD367));
  printf("Map's height (blocks): %u\n", gbReadMemory(0xD368));
  printf("Map's width (blocks): %u\n", gbReadMemory(0xD369));
  printf("Pointer to map data: 0x%02X\n", getPointer(0xD36A));
  printf("Map's Text Pointer Table (For Each NPC) Location: 0x%02X\n", getPointer(0xD36C));
  printf("Map's Level-Script Pointer: 0x%02X\n", getPointer(0xD36E));

  uint8_t connection_byte = gbReadMemory(0xD370);
  unsigned int num_connections = getNumConnections(connection_byte);
  printf("Map's Connection Byte: 0x%02X\n", gbReadMemory(0xD370));
  printf("Number of Connections: %u\n", num_connections);
  
  uint16_t addr = 0xD371;
  for(unsigned int i = 0; i < num_connections; i++) {
    printf("\n--- Connection %u ---\n", i+1);
    printf("Map ID of connected map: %u\n", gbReadMemory(addr));
    printf("Pointer to \"connection strip\"s upper-left block (Connected map): 0x%02X\n", getPointer(addr+1));
    printf("Pointer to \"connection strip\"s upper-left block (Current map): 0x%02X\n", getPointer(addr+3));
    printf("\"Bigness\": %u\n", gbReadMemory(addr+5));
    printf("Map Width: %u\n", gbReadMemory(addr+6));
    printf("Y alignment: %u\n", gbReadMemory(addr+7));
    printf("X alignment: %u\n", gbReadMemory(addr+8));
    printf("Window: 0x%02X\n", getPointer(addr+9));
    addr += 11;
  }

  // addr = getPointer(addr);

  // printf("\nPointer to object data: 0x%02X\n", addr);
  // printf("Border block id: %u\n", gbReadMemory(addr));

  // uint8_t num_warps = gbReadMemory(addr+1);
  // printf("Number of warps: %u\n", num_warps);
  // addr = addr+2;
  // for (uint8_t i = 0; i < num_warps; i++) {
  //   printf("\n--- Warp %u ---\n", i+1);
  //   printf("Y Position: %u\n", gbReadMemory(addr));
  //   printf("X Position: %u\n", gbReadMemory(addr+1));
  //   printf("Destination warp-to's ID (within target map): %u\n", gbReadMemory(addr+2));
  //   printf("Destination map: %u\n", gbReadMemory(addr+3));
  //   addr += 4;
  // }

  // uint8_t num_signs = gbReadMemory(addr);
  // printf("\nNumber of signs: %u\n", gbReadMemory(addr));
  // addr = addr+1;
  // for(uint8_t i = 0; i < num_signs; i++) {
  //   printf("\n--- Signpost %u ---\n", i+1);
  //   printf("Y Position: %u\n", gbReadMemory(addr));
  //   printf("X Position: %u\n", gbReadMemory(addr+1));
  //   printf("Text String ID: %u\n", gbReadMemory(addr+2));
  //   addr += 3;
  // }

  // printf("\nNumber of NPCs (total): %u\n", gbReadMemory(addr));
}

uint8_t** PokeMemViewer::getTilesAtAddr(uint16_t addr, int num_tiles) {
  uint8_t** tiles = new uint8_t*[num_tiles];

  for (int i = 0; i < num_tiles; i++) {
    tiles[i] = new uint8_t[16];
    for (int j = 0; j < 16; j++) {
      tiles[i][j] = gbReadMemory(addr++);
    }
  }

  return tiles;
}

uint8_t** PokeMemViewer::getTilesOnScreen() {
  uint8_t** tile_ids = getTiles();
  uint8_t** all_tiles = getTilesAtAddr(0x9000, 96);

  int num_tiles = tile_rows * tile_cols;
  uint8_t** screen_tiles = new uint8_t*[num_tiles];
  int k = 0;
  for (int i = 0; i < tile_rows; i++) {
    for (int j = 0; j < tile_cols; j++) {
      int tile_id = (int) tile_ids[i][j];
      screen_tiles[k++] = all_tiles[tile_id];
    }
  }
  
  return screen_tiles;
}

uint8_t PokeMemViewer::getCurrentMapNumber() {
  return gbReadMemory(0xD35E);
}

void PokeMemViewer::setMapDimensions(int& width, int& height) {
  height = (int) gbReadMemory(0xD368) * 2;
  width = (int) gbReadMemory(0xD369) * 2;
}

Pos PokeMemViewer::getPlayerPos() {
  Pos p;
  p.y = (int) gbReadMemory(0xD361);
  p.x = (int) gbReadMemory(0xD362);
  // p.y = monitoredAddresses[0xD361];
  // p.x = monitoredAddresses[0xD362];
  return p;
}

uint8_t PokeMemViewer::getTilesetID() {
  return gbReadMemory(0xD367);
}

uint8_t** PokeMemViewer::getTilesetData() {
  return getTilesAtAddr(0x9000, 96);
}

uint8_t* PokeMemViewer::getCommonTileset() {
  uint8_t* data = new uint8_t[4096];
  int ind = 0;
  for (int i = 0x8000; i < 0x9000; i++) {
    data[ind++] = gbReadMemory(i);
  }
  return data;
}


// Returns 3 dimensional aray, going by row, col, then 4 tiles per block
int*** PokeMemViewer::getScreenBlocksTileIndexes() {
  // create memory 
  int*** data = new int**[tile_rows/2];
  for (int i = 0; i < tile_rows/2; i++) {
    data[i] = new int*[tile_cols/2];
    for (int j = 0; j < tile_cols/2; j++) {
      data[i][j] = new int[4];
    }
  }

  // on-screen tile indexes
  uint8_t** tiles = getTiles();

  // populate data
  for (int i = 0; i < tile_rows; i++) {
    for (int j = 0; j < tile_cols; j++) {
      data[i/2][j/2][j%2 + (2*(i%2))] = (int) tiles[i][j];
    }
  }

  delete2DArray(tiles, tile_rows);

  return data;
}

void PokeMemViewer::initPartyMons(Pokemon*** mons, int* num_mons) {
  *num_mons = gbReadMemory(0xD163);
  uint16_t addr = 0xD16B;

  uint16_t name_addr = 0xD2B5;
  Pokemon** mons_arr = new Pokemon*[*num_mons];
  for (int i = 0; i < *num_mons; i++) {
    mons_arr[i] = initPokemon(addr, PLAYER_OWNER, false);
    mons_arr[i]->setName(getPokemonName(name_addr));
    name_addr += 11;
    addr += 44;
  }

  *mons = mons_arr;
}


void PokeMemViewer::initTrainerMons(Pokemon*** mons, int* num_mons, int* cur_mon_ind, bool is_player) {
  *cur_mon_ind = gbReadMemory(0xCC2F);

  uint16_t battle_mon_addr, other_mon_addr;
  PokeOwner owner;
  if (is_player) {
    *num_mons = gbReadMemory(0xD163);
    battle_mon_addr = 0xD014;
    other_mon_addr = 0xD16B;
    owner = PLAYER_OWNER;
  } else {
    *num_mons = gbReadMemory(0xD89C);
    battle_mon_addr = 0xCFE5;
    other_mon_addr = 0xD8A4;
    owner = TRAINER_OWNER;
  }

  uint16_t name_addr = 0xD2B5;
  Pokemon** mons_arr = new Pokemon*[*num_mons];
  for (int i = 0; i < *num_mons; i++) {
    if (i == *cur_mon_ind) {
      mons_arr[i] = initPokemon(battle_mon_addr, owner, true);
    } else {
      mons_arr[i] = initPokemon(other_mon_addr, owner, false);
    }
    
    if (is_player) {
      mons_arr[i]->setName(getPokemonName(name_addr));
      name_addr += 11;
    }
    other_mon_addr += 44;
  }

  *mons = mons_arr;
}

void PokeMemViewer::initWildMon(Pokemon*** mons) {
  Pokemon** mons_arr = new Pokemon*[1];
  mons_arr[0] = initPokemon(0xCFE5, NO_OWNER, true);
  *mons = mons_arr;
}


Pokemon* PokeMemViewer::initPokemon(uint16_t start_addr, PokeOwner owner, bool in_battle) {
  // printf("initting at %X\n", start_addr);
  Pokemon* mon = new Pokemon();
  uint16_t addr = start_addr;
  mon->setIsPlayerMon(owner == PLAYER_OWNER);
  uint8_t species_index = gbReadMemory(addr++);
  mon->setBase(species_index);
  mon->setOwned(isPokemonOwned(species_index));
  mon->setCurHp(read2BytesAndInc(&addr));
  addr++; // skip fake level
  mon->setStatuses(gbReadMemory(addr++)); // SEE WHAT THIS IS
  addr+=3; // skip types, catch rate/held item

  int num_moves = 0;
  uint8_t moves[4];
  for (int i = 0; i < 4; i++) {
    moves[i] = gbReadMemory(addr++);
    if (moves[i] != 0) {
      num_moves++;
    }
  }
  
  // Jumbled mess upcoming, prepare yourself
  if (in_battle) {
    addr += 2; // skip DV's
  } else {
    addr += 17; // skip trainer id, experience, EV/IV values
  }

  if (in_battle) {
    mon->setLevel(gbReadMemory(addr++));
    uint16_t max_hp = read2BytesAndInc(&addr);
    uint16_t attack = read2BytesAndInc(&addr);
    uint16_t defense = read2BytesAndInc(&addr);
    uint16_t speed = read2BytesAndInc(&addr);
    uint16_t special = read2BytesAndInc(&addr);
    mon->setStats(max_hp, attack, defense, speed, special);
  }

  uint8_t pps[4];
  for (int i = 0; i < 4; i++) {
    pps[i] = gbReadMemory(addr++);
  }
  mon->setMovesAndPP(num_moves, moves, pps);

  if (!in_battle) {
    mon->setLevel(gbReadMemory(addr++));
    uint16_t max_hp = read2BytesAndInc(&addr);
    uint16_t attack = read2BytesAndInc(&addr);
    uint16_t defense = read2BytesAndInc(&addr);
    uint16_t speed = read2BytesAndInc(&addr);
    uint16_t special = read2BytesAndInc(&addr);
    mon->setStats(max_hp, attack, defense, speed, special);
  }

  return mon;
}

std::string PokeMemViewer::getPokemonName(uint16_t name_addr) {
  uint16_t addr = name_addr;
  std::string name = "";
  for (int i = 0; i < 11; i++) {
    name += std::to_string(gbReadMemory(addr++));
  }
  return name;
}


void PokeMemViewer::printBattleData() {
  printf("wIsInBattle: %d\n", gbReadMemory(0xD057));
  
}

int PokeMemViewer::getAllItems(std::pair<uint8_t, uint8_t>** items) {
  uint8_t total_items = gbReadMemory(0xD31D);
  // printf("total items: %d\n", total_items);
  std::pair<uint8_t, uint8_t>* arr = new std::pair<uint8_t, uint8_t>[total_items];
  uint16_t addr = 0xD31E; 

  for (int i = 0; i < (int) total_items; i++) {
    // printf("i: %d\n", i);
    std::pair<uint8_t, uint8_t> p;
    p.first = gbReadMemory(addr++);
    p.second = gbReadMemory(addr++);

    arr[i] = p;
  }

  *items = arr;
  // printf("returning %d\n", total_items);
  return (int) total_items;
}

bool PokeMemViewer::isPokemonOwned(uint8_t index) {
  uint8_t dex_no = GameData::index_to_dex_map[index];
  uint16_t first_addr = 0xD2F7;
  int offset = (dex_no - 1) / 8;
  uint16_t addr = first_addr + offset;
  int ind = (dex_no - 1) % 8;
  uint8_t masks[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
  
  return (gbReadMemory(addr) & masks[ind]) != 0;
}

void PokeMemViewer::getMartItems(uint8_t** items, int* num_items) {
  *num_items = gbReadMemory(0xCF7B);
  uint8_t* arr = new uint8_t[*num_items];
  uint16_t addr = 0xCF7C;
  for (int i = 0; i < *num_items; i++) {
    arr[i] = gbReadMemory(addr++);
  }
  *items = arr;
}








/////////// PRIVATE FUNCTIONS /////////////

bool PokeMemViewer::collides(uint8_t tile, uint8_t* collisions, int size) {
  for (int i = 0; i < size; i++) {
    if (collisions[i] == tile) {
      return true;
    }
  }
  return false;
}

int PokeMemViewer::setCollisions(uint8_t** collisions) {
  uint16_t ptr = getPointer(0xD530);
  uint16_t itr = ptr;

  int size = 0;
  while (gbReadMemory(itr) != 0xFF) {
    size++;
    itr += 1;
  }

  uint8_t* cols = new uint8_t[size];
  for (unsigned int i = 0; i < size; i++) {
    cols[i] = gbReadMemory(ptr+i);
  }
  *collisions = cols;

  return size;
}

uint8_t** PokeMemViewer::getTiles() {
  uint16_t addr = 0xC3A0;
  uint8_t** tiles = new uint8_t*[tile_rows];
  for (int i = 0; i < tile_rows; i++) {
    tiles[i] = new uint8_t[tile_cols];
    for (int j = 0; j < tile_cols; j++) {
      tiles[i][j] = gbReadMemory(addr++);
    }
  }
  return tiles;
}

uint8_t** PokeMemViewer::getSpriteData() {
  uint16_t sprite_addr = 0xC100;
  uint8_t** data = new uint8_t*[num_sprites];
  for(int i = 0; i < num_sprites; i++) {
    data[i] = new uint8_t[sprite_bytes];
    for(unsigned int j = 0; j < 10; j++) {
      data[i][j] = gbReadMemory(sprite_addr + j);
    }
    sprite_addr += 0x10;
  }

  return data;
}

void PokeMemViewer::delete2DArray(uint8_t** arr, int rows) {
  for (int i = 0; i < rows; i++) {
    delete [] arr[i];
  }
  delete [] arr;
}

uint16_t PokeMemViewer::read2Bytes(uint16_t addr) {
  return (gbReadMemory(addr) << 8) | gbReadMemory((addr)+1);
}

uint16_t PokeMemViewer::read2BytesAndInc(uint16_t* addr) {
  uint16_t data = read2Bytes(*addr);
  *addr += 2;
  return data;
}

uint16_t PokeMemViewer::getPointer(uint16_t addr) {
  return (gbReadMemory(addr+1) << 8) | gbReadMemory(addr);
}

unsigned int PokeMemViewer::getNumConnections(uint8_t connection_byte) {
  unsigned int count = 0;
  while (connection_byte) {
    count += connection_byte & 1;
    connection_byte >>= 1;
  }
  return count;
}

void PokeMemViewer::printScreenTiles() {
  uint8_t** tile_ids = getTiles();
  for (int i = 0; i < tile_rows; i++) {
    for (int j = 0; j < tile_cols; j++) {
      printf("%u ",tile_ids[i][j]);
    }
    printf("\n");
  }
}


