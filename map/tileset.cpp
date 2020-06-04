#include "tileset.h"

  Tileset::Tileset(uint8_t _id, uint8_t** tile_data) : id(_id) {
    tiles = new Tile*[96];
    for (int i = 0; i < num_tiles; i++) {
      tiles[i] = new Tile(tile_data[i]);
      tiles_added++;
    }

    delete [] tile_data;
  }


Tileset::Tileset(uint8_t id, std::string name, uint8_t* blockset_data, uint8_t* tileset_data, int num_tiles) :
  id(id), name(name) {
  // printf("creating blockset\n");
  for (int i = 0; i <= 255; i++) {
    blockset_map[i] = blockset_data + (i*16);
  }
  
  // printf("creating tiles\n");
  tiles = new Tile*[num_tiles];
  for (int i = 0; i < num_tiles; i++) {
    tiles[i] = new Tile(tileset_data + (i*16));
  }

  // Commented out because I already created my images
  // printf("creating BMP...\n");
  // img = img_builder.createBMP(name, tiles, num_tiles);
  // printf("bmp created, writing...\n");
  // img->write();
}

Tileset::Tileset(uint8_t id, std::string name, uint8_t* tileset_data, int num_tiles) :
  id(id), name(name) {

  // printf("creating tiles\n");
  tiles = new Tile*[num_tiles];
  for (int i = 0; i < num_tiles; i++) {
    tiles[i] = new Tile(tileset_data + (i*16));
  }

  // Commented out because I already created my images
  // printf("creating BMP...\n");
  // img = img_builder.createBMP(name, tiles, num_tiles);
  // printf("bmp created, writing...\n");
  // img->write();
}

Tileset::~Tileset() {
  // printf("destroying tileset\n");
  for (int i = 0; i < num_tiles; i++) {
    // printf("deleting tile\n");
    delete tiles[i];
  }
  delete [] tiles;
}