#ifndef POKEMEMVIEWER_H
#define POKEMEMVIEWER_H
#include <string>
#include "common.h"
#include "../data/game_data.h"
#include "../data/pokemon.h"
#include "../../gb/gbGlobals.h"
#include "../../gb/gb.h"

enum PokeOwner {
  PLAYER_OWNER,
  TRAINER_OWNER,
  NO_OWNER
};


class PokeMemViewer {
 public:
  PokeMemViewer();
  Pos getPos();
  void printTileset();
  void printMap();
  void printCollisions();
  void printTileCollisionData();
  void printSpriteData();
  void printMapData();
  uint8_t** getTilesAtAddr(uint16_t addr, int num_tiles);
  uint8_t** getTilesOnScreen();

  // Battle methods
  void initPartyMons(Pokemon*** mons, int* num_mons);
  void initTrainerMons(Pokemon*** mons, int* num_mons, int* cur_mon_ind, bool is_player);
  void initWildMon(Pokemon*** mons);
  Pokemon* initPokemon(uint16_t start_addr, PokeOwner owner, bool in_battle);
  std::string getPokemonName(uint16_t name_addr);
  void printBattleData();
  bool caughtPokemon() { return gbReadMemory(0xCC24) == 0x08 && gbReadMemory(0xCC25) == 0x0F; }
  uint8_t getTextBoxID() { return gbReadMemory(0xD125); }
  uint16_t getBoxPos() { return read2Bytes(0xCC24); }
  uint16_t getTargetMonHP() { return read2Bytes(0xCFE6); }
  uint16_t getPlayerMonHP() { return read2Bytes(0xD015); }
  bool isPokemonOwned(uint8_t index);

  // Input Methods
  uint8_t getCurMenuItem() { return gbReadMemory(0xCC26); }
  bool isInTextbox() { return gbReadMemory(0xCFC4) == 0x01; }
  uint8_t getLongListOffset() { return gbReadMemory(0xCC36) + gbReadMemory(0xCC26); }
  int getAllItems(std::pair<uint8_t, uint8_t>** items);
  uint8_t getCurBoxNo() { return (gbReadMemory(0xD5A0) & 0x7F) + 1; }
  uint8_t getMonsInBox() { return gbReadMemory(0xDA80); }
  void getMartItems(uint8_t** items, int* num_items);


  // These are for real now
  uint8_t getCurrentMapNumber();
  void setMapDimensions(int& width, int& height);
  Pos getPlayerPos();
  uint8_t getTilesetID();
  uint8_t** getTilesetData();
  uint8_t* getCommonTileset();
  int*** getScreenBlocksTileIndexes();

  const static int tile_rows = 18;
  const static int tile_cols = 20;
 private:

  const int num_sprites = 16;
  const int sprite_bytes = 10;

  bool collides(uint8_t tile, uint8_t* collisions, int size);
  int setCollisions(uint8_t** collisions);
  uint8_t** getTiles();
  uint8_t** getSpriteData();
  void delete2DArray(uint8_t** tiles, int rows);
  uint16_t getPointer(uint16_t addr);
  uint16_t read2Bytes(uint16_t addr);
  uint16_t read2BytesAndInc(uint16_t* addr);

  unsigned int getNumConnections(uint8_t connection_byte);
  void printScreenTiles();
};

#endif // POKEMEMVIEWER_H