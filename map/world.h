#ifndef WORLD_H
#define WORLD_H
#include <queue>
#include <vector>
#include <map>
#include <math.h>
#include "map.h"
#include "tileset.h"
#include "bmp_builder.h"
#include "tile.h"
#include "../data/game_data.h"
#include "../util/common.h"
#include "../util/font_creator.h"
#include "../util/poke_mem_viewer.h"
#include "../util/read_rom.h"

class World {
 public:
  World(PokeMemViewer* _pmv);
  ~World();

  void start();
  void update() {}
  void createAndSetMap();
  void updateMap();
  // void monitorTilesetThread();
  Pos getGlobalPos();
  
  void handleMemoryWrite(uint16_t addr, uint8_t val);

 private:

  PokeMemViewer* pmv;

  std::vector<Map*> maps;
  std::vector<Tileset*> tilesets;

  Map* createMap(uint8_t tileset_id);
  Map* getMap(uint8_t id);
  Tileset* getTileset(uint8_t id);
  Pos player_pos;
  Map* cur_map;


  int tileset_writes{0};
  uint8_t cur_tileset{0};
  bool new_map{false};
  bool new_tileset{false};
  int min_x{0}, min_y{0}, max_x{0}, max_y{0};
  std::map<std::pair<int, int>, bool> pos_taken;
  FontCreator font_creator;

  // NEW METHODS
  void initTilesets();
  void initTileset(ROMHeader pair);
  void initRAMTileset();
  void initMaps();
  void initMap(ROMHeader pair);
  void setAllMapCoords();
  void setMapCoords(Map* map_to_set, Map* already_set, MapConnection connection);
  void setIndoorMapCoords(bool fill_gaps);
  int getConnectionStartOffset(Map* map, MapConnection connection);
  Direction getOppositeDirection(Direction dir);
  Pos getLocalConnectionPos(Map* map, Direction dir, int start_offset);
  Pos getGlobalConnectionPos(Map* map, Direction dir, int start_offset);
  void updateWorldBounds(Map* map);
  bool spaceIsTaken(Map* map, Pos pos);
  bool posIsTaken(Pos p);
  void createWorldImg(int scale);
};

#endif // WORLD_H