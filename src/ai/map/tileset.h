#ifndef TILESET_H
#define TILESET_H
#include "../util/common.h"
#include "tile.h"

class Tileset {
 public:
  Tileset(uint8_t _id, uint8_t** tile_data) : id(_id) {
    for (int i = 0; i < num_tiles; i++) {
      tiles[i] = new Tile(tile_data[i]);
      tiles_added++;
    }

    delete [] tile_data;
  }

  ~Tileset() {
    // printf("destroying tileset\n");
    for (int i = 0; i < num_tiles; i++) {
      // printf("deleting tile\n");
      delete tiles[i];
    }
  }

  uint8_t getId() { return id; }

  // add error checking
  Tile* at(int i) { return tiles[i]; }

 private:
  static const int num_tiles = 96;
  uint8_t id;
  Tile* tiles[num_tiles];
  int tiles_added{0};
};

#endif // TILESET_H