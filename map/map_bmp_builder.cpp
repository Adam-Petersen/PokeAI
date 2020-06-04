#include "map_bmp_builder.h"



BMP* MapBMPBuilder::createBMP(std::string map_name, int min_x, int max_x, int min_y, int max_y, std::vector<Block*> blocks, int scale) {
  printf("In createMapBMP\n");
  int map_height = max_y - min_y + 1;
  int map_width = max_x - min_x + 1;
  printf("map_height: %d, map_width: %d\n", map_height, map_width);

  // Number of blocks tall * 2 tiles * 8 rows per tile
  int data_height = map_height * 2 * 8;
  // Number of blocks wide * 2 tiles * 4 bytes per row
  int data_width = map_width * 2 * 4;

  // allocate memory
  uint8_t** pixel_data = new uint8_t*[data_height];
  for (int i = 0; i < data_height; i++) {
    pixel_data[i] = new uint8_t[data_width];
    for (int j = 0; j < data_width; j++) {
      pixel_data[i][j] = 0x44;
    }
  }

  printf("going through blocks\n");
  for (auto block : blocks) {
    writeBlock(block, pixel_data, getImgPos(block, min_x, max_y));
  }
  
  std::string map_path = "D:/Games/Emulators/VBA/PokeAI/img/world/" + map_name + ".bmp";

  printf("calling initBMP\n");
  return initBMP(map_path, data_width, data_height, pixel_data, scale);
}

BMP* MapBMPBuilder::createBMP(std::string map_name, int map_width, int map_height, Block*** blocks, int scale) {
  // printf("In createMapBMP\n");

  // Number of blocks tall * 2 tiles * 8 rows per tile
  int data_height = map_height * 2 * 8;
  // Number of blocks wide * 2 tiles * 4 bytes per row
  int data_width = map_width * 2 * 4;

  // allocate memory
  uint8_t** pixel_data = new uint8_t*[data_height];
  for (int i = 0; i < data_height; i++) {
    pixel_data[i] = new uint8_t[data_width];
  }

  // printf("memory allocated\n");

  for (int y = 0; y < map_height; y++) {
    for (int x = 0; x < map_width; x++) {
      Block* block = blocks[y][x];
      // printf("block set\n");
      writeBlock(block, pixel_data);
    }
  }

  std::string map_path = "D:/Games/Emulators/VBA/PokeAI/img/map/" + map_name + ".bmp";

  return initBMP(map_path, data_width, data_height, pixel_data, scale);
}


Pos MapBMPBuilder::getImgPos(Block* block, int min_x, int max_y) {
  // printf("local_pos: (%d, %d)\n", block->getPos().x, block->getPos().y);
  Pos gp = block->getGlobalPos();
  // printf("global pos: (%d, %d)\n", gp.x, gp.y);
  Pos img_pos = Pos(gp.x - min_x, max_y - gp.y);
  // printf("img_pos: (%d, %d)\n", img_pos.x, img_pos.y);
  return img_pos;
}


void MapBMPBuilder::updateBMP(BMP* bmp, std::vector<Block*> blocks) {
  for (auto block : blocks) {
    writeBlock(block, bmp->getData());
  }
}

void MapBMPBuilder::writeBlock(Block* block, uint8_t** pixel_data) {
  writeBlock(block, pixel_data, block->getPos());
}

void MapBMPBuilder::writeBlock(Block* block, uint8_t** pixel_data, Pos p) {

    // iterate through tiles in block
    for (int i = 0; i < 4; i++) {
      // printf("iterate through tiles in block\n");
      Tile* t = block->at(i);
      // printf("tile got\n");

      int is_right = i % 2; // 0 if left, 1 if right
      int is_top = i / 2; // 0 if top, 1 if bottom
      int x_ind = p.x*8 + 4*is_right; // 8 bytes per block + 4 bytes if right tile in current block

      // iterate through tile rows
      for (int k = 0; k < 8; k++) {
        // printf("iterate through tile rows\n");
        int y_ind = p.y*16 + 8*is_top + k; // 16 rows per block + 8 rows if bottom tile in current block + current row in tile (k)

        if (t->isEmptyTile()) {
          for (int i = 0; i < 4; i++) {
            pixel_data[y_ind][x_ind+i] = 0x44;
          }
        } else {
          // get row
          uint8_t lb, hb;
          if (t != NULL) {
            t->getRowBytes(k, &lb, &hb);
          } else {
            lb = 0xFF;
            hb = 0xFF;
          }
          set_single_tile_row_data(pixel_data, x_ind, y_ind, lb, hb);
        }
      }
    }
}