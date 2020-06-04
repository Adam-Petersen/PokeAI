#ifndef TILE_DRAWER_H
#define TILE_DRAWER_H
#include <vector>
#include "bmp.h"
#include "../util/common.h"


class BMPBuilder {
 public:

  BMPBuilder() {}
  // BMPBuilder(uint8_t** _tiles, int _num_tiles) : tiles(_tiles), num_tiles(_num_tiles) {}

  // void write_screen();


 protected:
  // int set_single_tile_row_data(uint8_t* pixels, int i, uint8_t lb, uint8_t hb);
  void set_single_tile_row_data(uint8_t** pixels, int x, int y, uint8_t lb, uint8_t hb);
  void scaleData(uint8_t*** new_data, uint8_t** cur_data, int width, int height, int scale);
  BMP* initBMP(std::string file_path, int data_width, int data_height, uint8_t** pixel_data, int scale);

  // const char* bmp_path = "D:/Games/Emulators/VBA/visualboyadvance-m/py/test_write.bmp";
  // uint8_t** tiles;
  // int num_tiles;
};

#endif // TILE_DRAWER_H