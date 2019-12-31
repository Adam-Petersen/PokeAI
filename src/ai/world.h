#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <math.h>
#include "common.h"
#include "map.h"
#include "PokeMemViewer.h"
#include "tileset.h"
#include "tile_drawer.h"
#include "tile.h"

class World {
 public:
  World(PokeMemViewer* _pmv);
  ~World();

  void start();
  void update();
  void createMapThread();
  void updateMapThread();
  void monitorTilesetThread();
  
  static void handleMemoryWrite(uint16_t addr, uint8_t val);
  static int tileset_writes;
  static uint8_t cur_tileset;
  static bool update_map_;


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
  bool new_map;

};

#endif // WORLD_H