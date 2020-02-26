#ifndef TILE_DRAWER_H
#define TILE_DRAWER_H
#include <vector>
#include "bmp.h"
#include "map.h"
#include "block.h"
#include "../util/common.h"

class Map;

class BMPBuilder {
 public:

  BMPBuilder() {}
  BMPBuilder(uint8_t** _tiles, int _num_tiles) : tiles(_tiles), num_tiles(_num_tiles) {}

  void write_screen();
  BMP* createMapBMPOLD(Map* m);
  BMP* createMapBMP(Map* m);

  void updateBMP(BMP* bmp, std::vector<Block*> blocks);

 private:
  int set_single_tile_row_data(uint8_t* pixels, int i, uint8_t lb, uint8_t hb);
  void set_single_tile_row_data(uint8_t** pixels, int x, int y, uint8_t lb, uint8_t hb);
  void writeBlock(Block* block, uint8_t** pixel_data);

  const char* bmp_path = "D:/Games/Emulators/VBA/visualboyadvance-m/py/test_write.bmp";
  uint8_t** tiles;
  int num_tiles;
};

#endif // TILE_DRAWER_H