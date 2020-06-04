#ifndef SRC_TILESET_BMP_BUILDER
#define SRC_TILESET_BMP_BUILDER
#include "bmp_builder.h"
#include "tile.h"

class TilesetBMPBuilder : public BMPBuilder {
 public:
  BMP* createBMP(std::string ts_name, Tile** tiles, int num_tiles);
 private:
  void writeTile(Tile* tile, uint8_t** pixel_data, int row, int col);
};

#endif