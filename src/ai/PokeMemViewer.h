#ifndef POKEMEMVIEWER_H
#define POKEMEMVIEWER_H
#include "../gb/gbGlobals.h"
#include "../gb/gb.h"
#include "common.h"

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

  // These are for real now
  uint8_t getCurrentMapNumber();
  void setMapDimensions(int& width, int& height);
  Pos getPlayerPos();
  uint8_t getTilesetID();
  uint8_t** getTilesetData();
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
  unsigned int getNumConnections(uint8_t connection_byte);
  void printScreenTiles();
};

#endif // POKEMEMVIEWER_H