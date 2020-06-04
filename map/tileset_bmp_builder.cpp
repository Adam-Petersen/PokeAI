#include "tileset_bmp_builder.h"

BMP* TilesetBMPBuilder::createBMP(std::string ts_name, Tile** tiles, int num_tiles) {
  int tile_height = num_tiles / 16;
  // Number of tiles tall * 10 rows per tile (8 for tile and 2 for border)
  int img_height = tile_height * 10 + 2;
  // Number of tiles wide * 5 bytes per row
  int img_width = 16 * 5 + 1;

  // allocate memory
  uint8_t** pixel_data = new uint8_t*[img_height];
  for (int i = 0; i < img_height; i++) {
    pixel_data[i] = new uint8_t[img_width];
    for (int j = 0; j < img_width; j++) {
      pixel_data[i][j] = 0x44;
    }
  }

  int ind = 0;
  for (int i = 0; i < tile_height; i++) {
    for (int j = 0; j < 16; j++) {
      writeTile(tiles[ind], pixel_data, i, j);
      ind++;
    }
  }

  std::string file_path = "D:/Games/Emulators/VBA/PokeAI/img/tileset/" + ts_name + ".bmp";
  return initBMP(file_path, img_width, img_height, pixel_data, 8);
}

void TilesetBMPBuilder::writeTile(Tile* tile, uint8_t** pixel_data, int row, int col) {
  // iterate through tile rows
  int x_ind = 1 + col*5; // 5 bytes per tile 
  for (int i = 0; i < 8; i++) {
    int y_ind = 2 + row*10 + i; // 10 rows per tile + current row

    // get row
    uint8_t lb, hb;
    tile->getRowBytes(i, &lb, &hb);
    set_single_tile_row_data(pixel_data, x_ind, y_ind, lb, hb);
  }
}