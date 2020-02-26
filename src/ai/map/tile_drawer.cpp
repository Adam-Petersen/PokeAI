#include "tile_drawer.h"

BMP* BMPBuilder::createMapBMP(Map* map) {
  // printf("In createMapBMP\n");
  int map_width, map_height;
  map->getDimensions(map_width, map_height);
  Block*** blocks = map->getBlocks();
  // printf("init variables set\n");

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

  std::string map_path = "D:/Games/Emulators/VBA/PokeAI/img/map/" + std::to_string(map->getId()) + ".bmp";
  BMP* bmp = new BMP(data_width, data_height, pixel_data, map_path);
  // printf("done!\n");

  return bmp;

}

void BMPBuilder::updateBMP(BMP* bmp, std::vector<Block*> blocks) {
  for (auto block : blocks) {
    writeBlock(block, bmp->getData());
  }
}

void BMPBuilder::writeBlock(Block* block, uint8_t** pixel_data) {
    Pos p = block->getPos();
    // printf("pos.x: %d, pos.y: %d\n", block->getPos().x, block->getPos().y);

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

void BMPBuilder::set_single_tile_row_data(uint8_t** pixels, int x, int y, uint8_t lb, uint8_t hb) {
  // printf("setting pixel_data[%d][%d+(0..4)]\n", y, x);
  for (int k = 0; k < 8; k+=2) {
    uint8_t low_bit_1, low_bit_2, hi_bit_1, hi_bit_2, row_data, nibble1, nibble2;

    // set bits
    low_bit_1 = lb << k;
    low_bit_1 = low_bit_1 >> 7;
    low_bit_2 = lb << (k+1);
    low_bit_2 = low_bit_2 >> 7;
    hi_bit_1 = hb << k;
    hi_bit_1 = hi_bit_1 >> 7;
    hi_bit_2 = hb << (k+1);
    hi_bit_2 = hi_bit_2 >> 7;

    // build nibbles and combine
    nibble1 = (hi_bit_1 << 1) | low_bit_1;
    nibble1 = nibble1 << 4;
    nibble2 = (hi_bit_2 << 1) | low_bit_2;
    row_data = nibble1 | nibble2;

    pixels[y][x++] = row_data;
  }
}








// NOT LONG FOR THIS WORLD //

int BMPBuilder::set_single_tile_row_data(uint8_t* pixels, int i, uint8_t lb, uint8_t hb) {
  for (int k = 0; k < 8; k+=2) {
    uint8_t low_bit_1, low_bit_2, hi_bit_1, hi_bit_2, row_data, nibble1, nibble2;

    // set bits
    low_bit_1 = lb << k;
    low_bit_1 = low_bit_1 >> 7;
    low_bit_2 = lb << (k+1);
    low_bit_2 = low_bit_2 >> 7;
    hi_bit_1 = hb << k;
    hi_bit_1 = hi_bit_1 >> 7;
    hi_bit_2 = hb << (k+1);
    hi_bit_2 = hi_bit_2 >> 7;

    // build nibbles and combine
    nibble1 = (hi_bit_1 << 1) | low_bit_1;
    nibble1 = nibble1 << 4;
    nibble2 = (hi_bit_2 << 1) | low_bit_2;
    row_data = nibble1 | nibble2;

    pixels[i++] = row_data;
  }

  return i;
}


void BMPBuilder::write_screen() {
  int rows = 18;
  int cols = 20;
  int itr = 0;
  int size = rows*cols*32;
  // // printf("%d\n",size);
  uint8_t* pixel_data = new uint8_t[size];
  for (int i = 0; i < 8*rows; i++) {
    int row_offset = i%8;
    int row = i/8;
    for (int j = 0; j < cols; j++) {
      uint8_t* tile_data = tiles[cols*row + j];
      uint8_t lb = tile_data[2*row_offset]; // low byte
      uint8_t hb = tile_data[2*row_offset + 1]; // high byte
      itr = set_single_tile_row_data(pixel_data, itr, lb, hb); // adds 4 bytes
    }
  }

  // BMP bmp(cols*8,-1*rows*8, pixel_data, size);
  // // bmp.write(bmp_path);
  // bmp.write_multiple(bmp_path, 2500, 50, 50);
}


BMP* BMPBuilder::createMapBMPOLD(Map* map) {
  printf("In drawMap\n");
  // int x_min, x_max, y_min, y_max;
  // map->getBounds(x_min, x_max, y_min, y_max);
  // printf("x range: (%d,%d), y range: (%d,%d)\n", x_min, x_max, y_min, y_max);

  int map_width, map_height;
  map->getDimensions(map_width, map_height);


  // 32 Bytes per tile, so number of blocks * 4 * 32

  // Number of blocks tall * 2 tiles * 8 rows per tile
  int height = map_height * 2 * 8;
  // Number of blocks wide * 2 tiles * 4 bytes per row
  int width = map_width * 2 * 4;
  // 32 Bytes per tile, so number of blocks * 4 * 32
  // int size = (x_max - x_min + 1) * (y_max - y_min + 1) * 4 * 32;

  // printf("height: %d, width: %d, size: %d\n", height, width, size);

  // allocate memory
  uint8_t** pixel_data = new uint8_t*[height];
  for (int i = 0; i < height; i++) {
    pixel_data[i] = new uint8_t[width];
  }

  // iterate through block rows
  for (int y = 0; y < map_height; y++) {
    // printf("iterate through block rows\n");
    // iterate through block columns
    for (int x = 0; x < map_width; x++) {
      // printf("iterate through block columns\n");
      Block* block = map->at(x,y);
      // printf("Block %d\n", block->getId());
    

      // iterate through tiles in block
      for (int i = 0; i < 4; i++) {
        // printf("iterate through tiles in block\n");
        Tile* t = block->at(i); 
        int is_right = i % 2; // 0 if left, 1 if right
        int is_top = i / 2; // 0 if top, 1 if bottom
        int x_ind = x*8 + 4*is_right; // 8 bytes per block + 4 bytes if right tile in current block

        // iterate through tile rows
        for (int k = 0; k < 8; k++) {
          // printf("iterate through tile rows\n");
          int y_ind = y*16 + 8*is_top + k; // 16 rows per block + 8 rows if bottom tile in current block + current row in tile (k)

          // get row
          uint8_t lb, hb;
          t->getRowBytes(k, &lb, &hb);
          set_single_tile_row_data(pixel_data, x_ind, y_ind, lb, hb);
        }
      }
    }
  }

  printf("done with data\n");
  fflush(stdout);  

  int img_width = width * 2; // number of bytes per row * 2 (1 byte = 2 pixels)
  int img_height = -1 * height;
  std::string map_path = "D:/Games/Emulators/VBA/visualboyadvance-m/py/test_map.bmp";
  BMP* bmp = new BMP(width, height, pixel_data, map_path);
  printf("done!\n");

  return bmp;
}

