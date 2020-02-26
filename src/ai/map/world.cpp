#include "world.h"
#include <windows.h>

World::World(PokeMemViewer* _pmv) : pmv(_pmv), new_map(false) {
  // printf("creating world\n");
  img_builder = new BMPBuilder();

  // init player pos
  player_pos = pmv->getPlayerPos();

  // init current map
  // printf("world initing map\n");
  Map* map = createMap(pmv->getCurrentMapNumber());
  maps.push_back(map);
  cur_map = map;
  cur_map->update(player_pos);

  cur_tileset = pmv->getTilesetID();
}

World::~World() {
  // printf("destroying world\n");
  for (auto map : maps) {
    // printf("deleting map\n");
    delete map;
  }

  // printf("tilesets size: %d\n", tilesets.size());
  for (auto tileset : tilesets) {
    // printf("deleting tileset\n");
    delete tileset;
  }

  // printf("deleting img_builder\n");
  delete img_builder;
  // printf("done destroying world\n");
}

void World::start() {
  // std::thread create_thread(&World::createMapThread, this);
  // std::thread update_thread(&World::updateMapThread, this);

  // // God speed thread dudes
  // create_thread.detach();
  // update_thread.detach();
}

void World::createAndSetMap() {
  // std::unique_lock<std::mutex> lck(cur_map_mtx);
  // while (cur_map->getId() == pmv->getCurrentMapNumber()) cur_map_cv.wait(lck);
  printf("in createAndSetMap\n");
  uint8_t map_no = pmv->getCurrentMapNumber();
  cur_map = getMap(map_no);
  if (cur_map == NULL) {
    // new map
    cur_map = createMap(map_no);
    maps.push_back(cur_map);
  }
  new_map = true;
  updateMap();
}

void World::updateMap() {
  // std::unique_lock<std::mutex> lck(player_pos_mtx);
  // while (player_pos == pmv->getPlayerPos() && !new_map) player_pos_cv.wait(lck);
  Pos new_pos = pmv->getPlayerPos();

  if (new_map) {
    printf("updateMap for new map\n");
    new_map = false;
    player_pos = new_pos;
    cur_map->update(player_pos);
  } else {
    printf("updateMap for same map\n");
    player_pos = new_pos;
    if (!cur_map->outOfBounds(new_pos) && cur_map->isLogicalMove(new_pos)) {
      printf("actually updating same map\n");
      cur_map->update(player_pos);
    } else {
      printf("pos out of bounds\n");
    }
  }
}


void World::handleMemoryWrite(uint16_t address, uint8_t value) {
  if (address == 0xD362) { // pos write
      printf("writing player pos\n");
      if (!new_map) {
          printf("notifying map update\n");
          // std::unique_lock<std::mutex> lck(player_pos_mtx);
          // player_pos_cv.notify_all();
          updateMap();
      }
  } else if (address == 0xD35E) { // map no. write
      printf("map number updating, setting new_map to true\n");
      new_map = true;
      tileset_writes = 0;
  } else if (address == 0xD367) { // tileset no. write
      printf("Updating tileset addr to %d\n", value);
      tileset_writes++;
      if (tileset_writes == 1) {
        printf("tileset  wrote once...\n");
      } else if (tileset_writes == 2 && value != cur_tileset) {
        printf("tileset is different... wait until data is written\n");
        cur_tileset = value;
        new_tileset = true;
      } else if (tileset_writes == 2 && value == cur_tileset) {
        printf("tileset is the same... wait for map dimensions\n");
        new_tileset = false;
      }
  } else if (address == 0xD369 && new_map && !new_tileset) {
    printf("map dimmensions done writing... calling notify\n");
    createAndSetMap();
  } else if (address == 0x95FF && new_map && new_tileset) { // tileset data write
      printf("tileset done writing.... calling notify\n");
      // std::unique_lock<std::mutex> lck(cur_map_mtx);
      new_tileset = false;
      createAndSetMap();
  }
}

Map* World::getMap(uint8_t id) {
  for (auto map : maps) {
    if (map->getId() == id) {
      return map;
    }
  }
  return NULL;
}

Tileset* World::getTileset(uint8_t id) {
  for (auto tileset : tilesets) {
    if (tileset->getId() == id) {
      return tileset;
    }
  }
  return NULL;
}

Map* World::createMap(uint8_t map_no) {
  // printf("in createMap\n");
  int width, height;
  pmv->setMapDimensions(width, height);

  uint8_t tsid = pmv->getTilesetID();
  printf("tsid: %d\n", tsid);
  Tileset* cur_ts = getTileset(tsid);

  if (cur_ts == NULL) {
    printf("creating tileset\n");
    uint8_t** ts_data = pmv->getTilesetData();
    cur_ts = new Tileset(tsid, ts_data); // ts_data freed
    tilesets.push_back(cur_ts);
  }

  // printf("allocating map\n");
  Map* map = new Map(map_no, cur_ts, pmv, width, height, img_builder);
  return map;
}