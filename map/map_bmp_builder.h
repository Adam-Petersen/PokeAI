#ifndef SRC_MAP_BMP_BUILDER
#define SRC_MAP_BMP_BUILDER
#include <vector>
#include "bmp_builder.h"
#include "block.h"

class MapBMPBuilder : public BMPBuilder {
 public:
  BMP* createBMP(std::string map_name, int min_x, int max_x, int min_y, int max_y, std::vector<Block*> blocks, int scale);
  BMP* createBMP(std::string map_name, int map_width, int map_height, Block*** blocks, int scale);

  void updateBMP(BMP* bmp, std::vector<Block*> blocks);
 private:
  void writeTile(Tile* tile, uint8_t** pixel_data, int row, int col);
  void writeBlock(Block* block, uint8_t** pixel_data);
  void writeBlock(Block* block, uint8_t** pixel_data, Pos p);
  Pos getImgPos(Block* block, int min_x, int max_y);

};

#endif