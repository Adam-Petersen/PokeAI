#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <math.h>
#include "map.h"
#include "tileset.h"
#include "tile_drawer.h"
#include "tile.h"
#include "../util/common.h"
#include "../util/PokeMemViewer.h"

class World {
 public:
  World(PokeMemViewer* _pmv);
  ~World();

  void start();
  void update() {}
  void createAndSetMap();
  void updateMap();
  // void monitorTilesetThread();
  
  void handleMemoryWrite(uint16_t addr, uint8_t val);

 private:

  PokeMemViewer* pmv;

  std::vector<Map*> maps;
  std::vector<Tileset*> tilesets;

  Map* createMap(uint8_t tileset_id);
  Map* getMap(uint8_t id);
  Tileset* getTileset(uint8_t id);
  BMPBuilder* img_builder;
  Pos player_pos;
  Map* cur_map;

  int tileset_writes{0};
  uint8_t cur_tileset{0};
  bool new_map{false};
  bool new_tileset{false};

};

#endif // WORLD_H