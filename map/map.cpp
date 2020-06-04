#include "map.h"

Map::Map(uint8_t id, Tileset* ts, PokeMemViewer* pmv, int width, int height) :
  id(id), ts(ts), pmv(pmv), width(width), height(height)  {
  // printf("height: %d, width: %d, tsid: %d\n", height, width, ts->getId());
  blocks = new Block**[height];
  for (int i = 0; i < height; i++) {
    blocks[i] = new Block*[width];
    for (int j = 0; j < width; j++) {
      blocks[i][j] = new Block(Pos(j,i));
    }
  }

  // printf("blocks allocated\n");

  // printf("creating bmp\n");
  img = img_builder.createBMP(std::to_string(id), width, height, blocks, 1);
  // printf("writing bmp\n");
  img->write();
}

Map::Map(uint8_t id, std::string name, uint8_t* map_data, uint8_t* connection_data, Tileset* ts, int width, int height) :
  id(id), name(name), ts(ts), width(width), height(height) {
  // printf("height: %d, width: %d, tsid: %d\n", height, width, ts->getId());
  initBlocks(map_data);
  initConnections(connection_data);
  coords = Pos(0,0);
  
  img = img_builder.createBMP(name, width, height, blocks, 1);
  // img->write();
}


Map::~Map() {
  // printf("destroying map\n");
  for (int y = 0; y < height; y++) {
    // printf("deleting block array\n");
    for (int x = 0; x < width; x++) {
      // printf("deleting block\n");
      delete blocks[y][x];
    }
    delete [] blocks[y];
  }
  delete [] blocks;

  // destroying img
  // printf("destroying img\n");
  delete img;
}

void Map::initBlocks(uint8_t* map_data) {
  blocks = new Block**[height];
  for (int i = 0; i < height; i++) {
    blocks[i] = new Block*[width];
    for (int j = 0; j < width; j++) {
      uint8_t chunk_id = map_data[(width/2)*(i/2) + (j/2)];
      // printf("%X ", chunk_id);
      uint8_t* chunk_data = ts->tilesAtChunck(chunk_id);
      int pos = (i%2)*2 + j%2;
      uint8_t p0[4] = {0,1,4,5};
      uint8_t p1[4] = {2,3,6,7};
      uint8_t p2[4] = {8,9,12,13};
      uint8_t p3[4] = {10,11,14,15};
      uint8_t* pinds;

      switch(pos) {
        case 0:
          pinds = p0;
          break;
        case 1:
          pinds = p1;
          break;
        case 2:
          pinds = p2;
          break;
        case 3:
          pinds = p3;
          break;
        default:
          printf("bad pos\n");
          throw;
      }

      Tile** tiles = new Tile*[4];
      for (int k = 0; k < 4; k++) {
        tiles[k] = ts->at(chunk_data[pinds[k]]);
      }
      blocks[i][j] = new Block(Pos(j,i), tiles); 
    }
    // printf("\n");
  }
}

void Map::initConnections(uint8_t* connection_data) {
  // printf("in initConnections for %d\n", id);
  uint8_t connection_byte = connection_data[0];
  // printf("connection_byte: %X\n", connection_byte);
  Direction dirs[4] = {NORTH, SOUTH, WEST, EAST};

  int offset = 1;
  int dir_ind = 0;
  for (uint8_t i = 0x08; i >= 0x01; i/=2) {
    if ((connection_byte & i) > 0) {
      // printf("connection found in direction %d\n", dirs[dir_ind]);
      initConnection(connection_data+offset, dirs[dir_ind]);
      offset += 11;
    }
    dir_ind++;
  }
}

void Map::initConnection(uint8_t* connection_data, Direction dir) {
  // printf("in initConnection, dir: %d\n",dir);
  // for (int i = 0; i < 11; i++) { printf("%X ", connection_data[i]); }
  // printf("\n");
  MapConnection mc;
  mc.dir = dir;
  mc.connected_id = connection_data[0];
  mc.start_ptr = ((connection_data[4] << 8) | connection_data[3]);
  mc.connection_size = connection_data[5];
  // mc.print();
  connections.push_back(mc);
}

MapConnection Map::getConnection(Direction dir) {
  for (auto c : connections) {
    if (c.dir == dir) {
      return c;
    }
  }
  printf("connection in %d direction not found\n",dir);
  throw;
}

bool Map::hasConnection(Direction dir) {
  for (auto c: connections) {
    if (c.dir == dir) {
      return true;
    }
  }
  return false;
}


void Map::printCoords() {
  std::cout << name << " ";
  printf("coords: (%d, %d)\n", coords.x, coords.y);
}

void Map::setCoords(Pos c) {
   coords = c;

   for (int i = 0; i < height; i++) {
     for (int j = 0; j < width; j++) {
       blocks[i][j]->setGlobalPos(Pos(c.x + j, c.y - i));
     }
   }

  if (getNumConnections() == 0 || !hasConnection(NORTH)) {
    setNameCoords(Pos(coords.x + (width / 2) - (name_blocks.size() / 2), coords.y + 1));
  } else if (!hasConnection(SOUTH)) {
    setNameCoords(Pos(coords.x + (width / 2) - (name_blocks.size() / 2), coords.y - height));
    flipNameTiles();
  } else if (!hasConnection(WEST)) {
    setNameCoords(Pos(coords.x - name_blocks.size() - 1, coords.y - (height/2)));
  } else if (!hasConnection(EAST)) {
    setNameCoords(Pos(coords.x + width + 1, coords.y - (height/2)));
  } else {
    // If the map has 4 connections, just add the label to the top left and hope for the best...
    // This works fine for the only 2 maps that have 4 connections (Cerulean and Saffron)
    setNameCoords(Pos(coords.x, coords.y + 1));
  }
}

void Map::setNameCoords(Pos pos) {
  int x = 0;
  for (auto block : name_blocks) {
    block->setGlobalPos(Pos(pos.x + x, pos.y));
    x++;
  }
}

void Map::flipNameTiles() {
  for (auto block : name_blocks) {
    block->flipTiles();
  }
}




void Map::update(Pos player_pos) {
  last_pos = player_pos;
  // printf("in Map::update\n");
  Block* cur_b = blocks[player_pos.y][player_pos.x];
  // printf("cur_b: %d, visted: %d\n", cur_b->getId(), cur_b->wasVisited());

  if (cur_b->wasVisited()) {
    // printf("block was visited already... returning\n");
    return;
  }

  // printf("block hasn't been visted yet\n");

  cur_b->setVisited();
  std::vector<Block*> updated_blocks;
  int*** indexes = pmv->getScreenBlocksTileIndexes();
  int  x_left, x_right, y_top, y_bottom, screen_x_left, screen_y_top;
  setScreenBounds(player_pos, &x_left, &x_right, &y_top, &y_bottom, &screen_x_left, &screen_y_top);
  // printf("x_left: %d, x_right: %d, y_top: %d, y_bottom: %d\n", x_left, x_right, y_top, y_bottom);

  for(int y = y_top; y <= y_bottom; y++) {
    for (int x = x_left; x <= x_right; x++) {
      Block* b = blocks[y][x];
      if (b->isEmpty()) {
        // printf("setting tiles and pushing back\n");
        // printf("(x,y): (%d, %d), b coords (%d,%d), y_ind: %d, x_ind: %d\n", x, y, b->getPos().x, b->getPos().y, y_ind, x_ind);
        Tile** tile_ptrs = getTilesFromTileset(indexes[y-screen_y_top][x-screen_x_left], 4);
        b->setTiles(tile_ptrs);
        updated_blocks.push_back(b);
      }
    }
  }

  img_builder.updateBMP(img, updated_blocks);
  img->write();

  deleteScreenIndexes(indexes);
}

void Map::getBounds(int& _width, int& _height) {
  _width = width;
  _height = height;
}

void Map::getDimensions(int& _width, int& _height) {
  _width = width;
  _height = height;
}

bool Map::outOfBounds(Pos p) {
  return p.x >= width || p.y >= height;
}

bool Map::isLogicalMove(Pos p) {
  return std::abs(p.x - last_pos.x) <= 1 && std::abs(p.y - last_pos.y) <= 1;
}


Tile** Map::getTilesFromTileset(int* indexes, int size) {
  Tile** tile_ptrs = new Tile*[size];
  for (int i = 0; i < size; i++) {
    tile_ptrs[i] = ts->at(indexes[i]);
  }

  return tile_ptrs;
}

Block* Map::at(int x, int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return NULL;
  }
  return blocks[y][x]; 
}

void Map::setScreenBounds(Pos p, int* low_x, int* high_x, int* low_y, int* high_y, int* screen_x_left, int* screen_y_top) {
  *low_x = std::max(0, p.x - 4);
  *high_x = std::min(width - 1, p.x + 5);
  *low_y = std::max(0, p.y - 4);
  *high_y = std::min(height - 1, p.y + 4);
  *screen_x_left = p.x - 4;
  *screen_y_top = p.y-4;
}

void Map::deleteScreenIndexes(int*** indexes) {
  for (int i = 0; i < PokeMemViewer::tile_rows / 2; i++) {
    for (int j = 0; j < PokeMemViewer::tile_cols / 2; j++) {
      delete [] indexes[i][j];
    }
    delete [] indexes[i];
  }
  delete [] indexes;
}

void MapConnection::print() {
  printf("connected id: %d\n", connected_id);
  printf("start_ptr: %X\n", start_ptr);
  printf("connection_size: %d\n", connection_size);
  printf("dir: %d\n", dir);
}
