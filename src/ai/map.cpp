#include "map.h"

Map::Map(uint8_t id, Tileset* ts, PokeMemViewer* pmv, int width, int height, BMPBuilder* img_builder) :
  id(id), ts(ts), pmv(pmv), width(width), height(height), img_builder(img_builder)  {
  printf("height: %d, width: %d, tsid: %d\n", height, width, ts->getId());
  blocks = new Block**[height];
  for (int i = 0; i < height; i++) {
    blocks[i] = new Block*[width];
    for (int j = 0; j < width; j++) {
      blocks[i][j] = new Block(Pos(j,i));
    }
  }

  // printf("blocks allocated\n");

  // printf("creating bmp\n");
  img = img_builder->createMapBMP(this);
  // printf("writing bmp\n");
  img->write();
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

  img_builder->updateBMP(img, updated_blocks);
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
