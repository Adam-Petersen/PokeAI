#ifndef MAP_H
#define MAP_H
#include <algorithm>
#include <vector>
#include "tileset.h"
#include "Block.h"
#include "tile.h"
#include "bmp.h"
#include "map_bmp_builder.h"
#include "../util/common.h"
#include "../util/poke_mem_viewer.h"

enum Direction {
  NORTH=0,
  SOUTH=1,
  WEST=2,
  EAST=3
};

struct MapConnection {
  uint8_t connected_id;
  uint16_t start_ptr;
  uint8_t connection_size;
  Direction dir;
  void print();
};

class Map {
 public:
  Map(uint8_t id, Tileset* ts, PokeMemViewer* pmv, int width, int height);
  Map(uint8_t id, std::string name, uint8_t* map_data, uint8_t* connection_data, Tileset* tileset, int width, int height); // ROM way 
  ~Map();

  void update(Pos player_pos);
  void getBounds(int& _width, int& _height);
  void getDimensions(int& _width, int& _height);
  uint8_t getId() { return id; }
  std::string getName() { return name; }
  BMP* getImg() { return img; }
  Block*** getBlocks() { return blocks; }
  bool outOfBounds(Pos p);
  bool isLogicalMove(Pos p);
  std::vector<MapConnection> getConnections() { return connections; }
  MapConnection getConnection(Direction dir);
  int getNumConnections() { return connections.size(); }
  bool hasConnection(Direction dir);
  Pos getCoords() { return coords; }
  void setCoords(Pos c);
  bool defaultCoords() { return coords == Pos(0,0); }
  void printCoords();
  void setNameBlocks(std::vector<Block*> nb) { name_blocks = nb; }
  std::vector<Block*> getNameBlocks() { return name_blocks; }

  // TODO: add error checking
  Block* at(int x, int y);

 private:
  uint8_t id;
  std::string name;
  int width, height;
  PokeMemViewer* pmv;
  Block*** blocks;
  Map* neighbors[4];
  std::vector<MapConnection> connections;
  Tileset* ts;
  MapBMPBuilder img_builder;
  BMP* img;
  Pos last_pos;
  Pos coords;
  std::vector<Block*> name_blocks;

  void initBlocks(uint8_t* map_data);
  void initConnections(uint8_t* connections_data);
  void initConnection(uint8_t* connection_data, Direction dir);
  void setNameCoords(Pos pos);
  void flipNameTiles();
  Tile** getTilesFromTileset(int* indexes, int size);
  void setScreenBounds(Pos p, int* low_x, int* high_x, int* low_y, int* high_y, int* screen_x_left, int* screen_y_top);
  void deleteScreenIndexes(int*** indexes);
};

#endif // MAP_H