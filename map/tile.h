#ifndef TILE_H
#define TILE_H
#include "../util/common.h"
#include <cstring>

class Tile {
 public:
  static int next_id;

  Tile() {
    data = new uint8_t[16];
    memset(data, 0xFF, sizeof(uint8_t)*16);
    is_empty_tile = true;
  }

  Tile(uint8_t* _data) : data(_data), id(next_id++) {
    // printf("making tile %d... data addr: %p\n", id, data);
  }
  
  ~Tile() {
    // printf("destroying tile with data: %p\n", data);
    delete [] data;
  }

  static Tile* getEmptyTile() { return empty_tile; }

  uint16_t getID();
  void getRowBytes(int row, uint8_t* low_byte, uint8_t* high_byte) {
    *low_byte = data[2*row];
    *high_byte = data[2*row + 1];
  }

 bool isEmptyTile() { return is_empty_tile; }

 private:
  static Tile* empty_tile;
  static const int data_size = 16; 
  uint8_t tileset_id;
  uint8_t id;
  bool col_flag;
  uint8_t* data;
  bool is_empty_tile{false};
};

Tile* Tile::empty_tile = new Tile();
int Tile::next_id = 0;

#endif // TILE_H