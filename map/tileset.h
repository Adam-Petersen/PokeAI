#ifndef TILESET_H
#define TILESET_H
#include <map>
#include "../util/common.h"
#include "tile.h"
#include "tileset_bmp_builder.h"

class Tileset {
 public:
  Tileset(uint8_t _id, uint8_t** tile_data);
  Tileset(uint8_t id, std::string name, uint8_t* blockset_data, uint8_t* tileset_data, int num_tiles);
  Tileset(uint8_t id, std::string name, uint8_t* tileset_data, int num_tiles);
  ~Tileset();

  uint8_t getId() { return id; }
  std::string getName() { return name; }
  int getNumTiles() { return num_tiles; }

  // add error checking
  Tile* at(int i) { return tiles[i]; }

  uint8_t* tilesAtChunck(uint8_t block) { return blockset_map[block]; }

 private:
  int num_tiles = 96;
  std::string name;
  uint8_t id;
  Tile** tiles;
  int tiles_added{0};
  std::map<uint8_t, uint8_t*> blockset_map;
  TilesetBMPBuilder img_builder;
  BMP* img;
};

#endif // TILESET_H