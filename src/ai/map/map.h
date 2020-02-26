#ifndef MAP_H
#define MAP_H
#include <algorithm>
#include <vector>
#include "tileset.h"
#include "Block.h"
#include "tile.h"
#include "bmp.h"
#include "tile_drawer.h"
#include "../util/common.h"
#include "../util/PokeMemViewer.h"

class BMPBuilder;
class Map {
 public:
  Map(uint8_t id, Tileset* ts, PokeMemViewer* pmv, int width, int height, BMPBuilder* img_builder);
  ~Map();

  void update(Pos player_pos);
  void getBounds(int& _width, int& _height);
  void getDimensions(int& _width, int& _height);
  uint8_t getId() { return id; }
  BMP* getImg() { return img; }
  Block*** getBlocks() { return blocks; }
  bool outOfBounds(Pos p);
  bool isLogicalMove(Pos p);


  // TODO: add error checking
  Block* at(int x, int y);

 private:
  uint8_t id;
  int width, height;
  PokeMemViewer* pmv;
  Block*** blocks;
  Map* neighbors[4];
  Tileset* ts;
  BMPBuilder* img_builder;
  BMP* img;
  Pos last_pos;

  Tile** getTilesFromTileset(int* indexes, int size);
  void setScreenBounds(Pos p, int* low_x, int* high_x, int* low_y, int* high_y, int* screen_x_left, int* screen_y_top);
  void deleteScreenIndexes(int*** indexes);
};

#endif // MAP_H