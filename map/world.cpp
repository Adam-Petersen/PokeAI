#include "world.h"
#include <windows.h>

World::World(PokeMemViewer* _pmv) : pmv(_pmv), new_map(false) {
  // init player pos
  player_pos = pmv->getPlayerPos();

  // Load from ROM
  initTilesets();
  initMaps();
  setAllMapCoords();
  setIndoorMapCoords(true);
  printf("min_x: %d, max_x: %d, min_y: %d, max_y: %d\n", min_x, max_x, min_y, max_y);
  // createWorldImg(2);

  // // init current map
  // // printf("world initing map\n");
  // Map* map = createMap(pmv->getCurrentMapNumber());
  // maps.push_back(map);
  // cur_map = map;
  // cur_map->update(player_pos);

  // cur_tileset = pmv->getTilesetID();
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
  Map* map = new Map(map_no, cur_ts, pmv, width, height);
  return map;
}

//////////// ROM METHODS ///////////////////

void World::initTilesets() {
  for (auto data : GameData::tileset_data_info) {
    initTileset(data);
  }
  initRAMTileset();
}

void World::initTileset(ROMHeader pair) {
  unsigned int header_addr = pair.addr;
  uint8_t bank = header_addr / 0x4000;
  uint16_t offset = header_addr % 0x4000;
  uint8_t* header_rom = ReadROM::readROMBank(bank);

  uint8_t tileset_bank = header_rom[offset];
  uint16_t blockset_addr = ((header_rom[offset+2] << 8) | header_rom[offset+1]) - 0x4000;
  uint16_t tiles_addr = ((header_rom[offset+4] << 8) | header_rom[offset+3]) - 0x4000;

  // printf("tileset_bank: %X, blockset_addr: %X, tiles_addr: %X\n", tileset_bank, blockset_addr, tiles_addr);
  uint8_t* tileset_rom = ReadROM::readROMBank(tileset_bank);
  tilesets.push_back(new Tileset(pair.id, pair.name, tileset_rom + blockset_addr, tileset_rom + tiles_addr, 96));
}

void World::initRAMTileset() {
  uint8_t id = 0xFF;
  std::string name = "common tiles";
  uint8_t* tile_data = pmv->getCommonTileset();

  tilesets.push_back(new Tileset(id, name, tile_data, 256));
}


void World::initMaps() {
  for (auto data : GameData::map_data_info) {
    initMap(data);
  }
}

void World::initMap(ROMHeader data) {
  unsigned int header_addr = data.addr;
  uint8_t bank = header_addr / 0x4000;
  uint16_t offset = header_addr % 0x4000;
  uint8_t* rom = ReadROM::readROMBank(bank);

  uint8_t tileset_id = rom[offset];
  uint8_t height = rom[offset+1];
  uint8_t width = rom[offset+2];
  uint16_t data_addr = ((rom[offset+4] << 8) | rom[offset+3]) - 0x4000;
  uint8_t* connection_data = rom+offset+9;  

  // printf("tileset_id: %X, height: %X, width: %X, data_addr: %X\n", tileset_id, height, width, data_addr);
  Tileset* tileset = getTileset(tileset_id);
  Map* map = new Map(data.id, data.name, rom + data_addr, connection_data, tileset, width*2, height*2);

  // add labels to indoor maps
  // if (map->getNumConnections() == 0) {
    // printf("adding label to %s\n", map->getName().c_str());
    map->setNameBlocks(font_creator.getBlocksFromText(map->getName()));
  // }

  maps.push_back(map);


}

void World::setAllMapCoords() {
  // printf("in setAllMapCoords\n");
  std::map<uint8_t,bool> visited;
  std::queue<Map*> queue;

  // printf("setting visited\n");
  // Set visited to false for all maps
  for (auto m : maps) {
    visited[m->getId()] = false;
  }

  // printf("setting root map\n");
  Map* root = getMap(0x00);
  if (root == NULL) { printf("bad root\n"); throw; }
  root->setCoords(Pos(-5, 6)); // origin arbitrarily set to first step outside home in Pallet
  updateWorldBounds(root);
  // root->printCoords();
  queue.push(root);
  visited[root->getId()] = true;

  // printf("while loop\n");
  while (!queue.empty()) {
    Map* map = queue.front();
    queue.pop();
    
    // printf("map popped\n");
    std::vector<MapConnection> connections = map->getConnections();
    for (auto c : connections) {
      Map* neighbor = getMap(c.connected_id);

      if (neighbor == NULL) {
        printf("Map %X not found... continuing\n", c.connected_id);
        continue;
      }

      if (!visited[neighbor->getId()]) {
        setMapCoords(neighbor, map, c);
        queue.push(neighbor);
        visited[neighbor->getId()] = true;
      }
    }
  }
}

void World::setMapCoords(Map* map_to_set, Map* already_set, MapConnection connection) {
  // printf("in setMapCoords for %s\n", map_to_set->getName().c_str());
  Direction opp_dir = getOppositeDirection(connection.dir);
  MapConnection unset_connection = map_to_set->getConnection(opp_dir);
  // unset_connection.print();
  int unset_offset = getConnectionStartOffset(map_to_set, unset_connection);
  int set_offset = getConnectionStartOffset(already_set, connection);
  // printf("unset_offset: %d, set_offset: %d\n", unset_offset, set_offset);
  Pos local_pos = getLocalConnectionPos(map_to_set, opp_dir, unset_offset);
  Pos global_pos = getGlobalConnectionPos(already_set, connection.dir, set_offset);
  // printf("local_pos: (%d, %d)\n", local_pos.x, local_pos.y);
  // printf("global_pos: (%d, %d)\n", global_pos.x, global_pos.y);
  map_to_set->setCoords(Pos(global_pos.x - local_pos.x, global_pos.y - (local_pos.y * -1)));
  updateWorldBounds(map_to_set);
  // map_to_set->printCoords();
}

int World::getConnectionStartOffset(Map* map, MapConnection connection) {
  int width, height;
  map->getDimensions(width, height);
  width /= 2; // convert dimensions to blocks, not steps
  height /= 2; 
  // printf("getConnectionStartOffset for %s, width: %d, height: %d, start_ptr: %X, dir: %d\n", map->getName().c_str(), width, height, connection.start_ptr, connection.dir);

  switch(connection.dir) {
   case NORTH:
    return connection.start_ptr - 0xC6EB;
   case SOUTH:
    return connection.start_ptr - 0xC6EB - ((height + 3) * (width + 6));
   case WEST:
    return ((connection.start_ptr - 0xC6E8)/(width + 6)) - 3;
   case EAST:
    return ((connection.start_ptr - 0xC6EB - width)/(width + 6)) - 3;
   default:
    printf("unknown direction\n");
    throw;
  }
}

Direction World::getOppositeDirection(Direction dir) {
  switch(dir) {
    case NORTH:
     return SOUTH;
    case SOUTH:
     return NORTH;
    case WEST:
     return EAST;
    case EAST:
     return WEST;
    default:
     printf("Unknown direction\n");
     throw;
  }
}

Pos World::getLocalConnectionPos(Map* map, Direction dir, int start_offset) {
  int width, height;
  map->getDimensions(width, height);
  // printf("setting local ps, width: %d, height: %d\n", width, height);
  int real_offset = std::fmax(start_offset*2, 0);

  switch(dir) {
    case NORTH:
     return Pos(real_offset, 0);
    case SOUTH:
     return Pos(real_offset, height-1);
    case WEST:
     return Pos(0, real_offset);
    case EAST:
     return Pos(width-1, real_offset);
    default:
     printf("unknown direction\n");
     throw;
  }
}

Pos World::getGlobalConnectionPos(Map* map, Direction dir, int start_offset) {
  int width, height;
  map->getDimensions(width, height);
  Pos coords = map->getCoords();

  // printf("setting globalPos, width: %d, height: %d, c.x: %d, c.y: %d\n", width, height, map->getCoords().x, map->getCoords().y);
  int real_offset = std::fmax(start_offset*2, 0);

  switch(dir) {
    case NORTH:
     return Pos(coords.x + real_offset, coords.y + 1);
    case SOUTH:
     return Pos(coords.x + real_offset, coords.y - height);
    case WEST:
     return Pos(coords.x - 1, coords.y - real_offset);
    case EAST:
     return Pos(coords.x + width, coords.y - real_offset);
    default:
     printf("unknown direction\n");
     throw;
  }
}

void World::setIndoorMapCoords(bool fill_gaps) {
  int x = min_x + 1;
  int start_min_y = fill_gaps ? max_y - 3 : min_y - 3;
  int real_max_x = std::fmax(200, max_x);

  for (auto map : maps) {
    if (map->getNumConnections() > 0) {
      continue; // skip outdoor maps
    }

    int width, height;
    map->getDimensions(width, height);

    int y = start_min_y;

    while (true) {
      if ((x + width) >= (real_max_x - 1)) {
        x = min_x + 1;
        y -= 1;
        // printf("moving down, looking at (%d, %d)\n", x, y);
      }

      if (!spaceIsTaken(map, Pos(x,y))) {
        // printf("setting coords for %s to (%d, %d)\n", map->getName().c_str(), x, y);
        map->setCoords(Pos(x,y));
        // for (auto b : map->getNameBlocks()) {
        //   printf("coords: (%d, %d)\n", b->getGlobalPos().x, b->getGlobalPos().y);
        // }
        updateWorldBounds(map);
        break;
      }
      x++;
    }
  }
}


void World::updateWorldBounds(Map* map) {
  Pos coords = map->getCoords();
  int width, height;
  map->getDimensions(width, height);
  std::vector<Block*> name_blocks = map->getNameBlocks();
  Block* first_letter = name_blocks.at(0);
  Block* last_letter = name_blocks.at(name_blocks.size() - 1);

  int temp_min_x = std::fmin(coords.x, first_letter->getGlobalPos().x) - 1;
  int temp_max_x = std::fmax(coords.x + width - 1, last_letter->getGlobalPos().x) + 1;
  int temp_min_y = std::fmin(coords.y - height + 1, first_letter->getGlobalPos().y) - 1;
  int temp_max_y = std::fmax(coords.y, first_letter->getGlobalPos().y) + 1;

  min_x = std::fmin(min_x, temp_min_x);
  max_x = std::fmax(max_x, temp_max_x);
  min_y = std::fmin(min_y, temp_min_y);
  max_y = std::fmax(max_y, temp_max_y);

  int boundary = 3;
  for (int y = coords.y + boundary; y > coords.y - height - boundary; y--) {
    for (int x = coords.x - boundary; x < coords.x + width + boundary; x++) {
      // printf("inserting into pos_taken (%d, %d)\n", x, y);
      pos_taken[std::pair<int, int>(x,y)] = true;
    }
  }

  Pos first_pos = first_letter->getGlobalPos();
  Pos last_pos = last_letter->getGlobalPos();
  pos_taken[std::pair<int, int>(first_pos.x - 1, first_pos.y)] = true;
  pos_taken[std::pair<int, int>(last_pos.x + 1, last_pos.y)] = true; 
  for (auto block : name_blocks) {
    Pos p = block->getGlobalPos();
    pos_taken[std::pair<int, int>(p.x, p.y)] = true;
  }
}

void World::createWorldImg(int scale) {
  std::vector<Block*> blocks;
  for (auto map : maps) {
    if (map->defaultCoords()) {
      continue;
    }

    int width, height;
    map->getDimensions(width, height);
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        blocks.push_back(map->at(x, y));
      }
    }

    for (auto block : map->getNameBlocks()) {
      blocks.push_back(block);
    }
  }

  MapBMPBuilder img_builder;
  // printf("calling createBMP with %lld blocks\n", blocks.size());
  BMP* img = img_builder.createBMP("world", min_x, max_x, min_y, max_y, blocks, scale);
  img->write();
}

Pos World::getGlobalPos() {
  Pos local_pos = pmv->getPlayerPos();
  Pos global_pos = getMap(pmv->getCurrentMapNumber())->getCoords();
  return Pos(global_pos.x + local_pos.x, global_pos.y - local_pos.y);
}

bool World::spaceIsTaken(Map* map, Pos pos) {
  // printf("tl: (%d, %d), br: (%d, %d)\n", top_left.x, top_left.y, bottom_right.x, bottom_right.y);
  int width, height;
  map->getDimensions(width, height);


  for (int y = pos.y ; y > pos.y - height; y--) {
    for (int x = pos.x; x < pos.x + width; x++) {
      if (posIsTaken(Pos(x,y))) {
        return true;
      }
    }
  }

  // printf("%s should fit, checking label\n", map->getName().c_str());
  int x = (width / 2) - (map->getNameBlocks().size() / 2);
  for (auto block : map->getNameBlocks()) {
    // printf("checking block at (%d, %d)\n", block->getGlobalPos().x, block->getGlobalPos().y);
    Pos p = Pos(pos.x + x, pos.y + 1);
    if (posIsTaken(p)) { 
      return true; 
    }
    x++;
  }

  return false;
}

bool World::posIsTaken(Pos p) { return pos_taken[std::pair<int, int>(p.x,p.y)]; }


