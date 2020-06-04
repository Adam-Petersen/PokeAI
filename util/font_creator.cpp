#include "font_creator.h"

FontCreator::FontCreator() {
  initCharMap();
}

void FontCreator::initCharMap() {
  for (int i = 0; i < 37; i++) {
    uint8_t* tile_data = new uint8_t[16];
    for (int j = 0; j < 8; j++) {
      tile_data[2*j] = font_data[i*8 + j];
      tile_data[2*j + 1] = font_data[i*8 + j];
    }
    char_map[chars[i]] = new Tile(tile_data);
  }
}

Tile* FontCreator::getCharTile(char c) { return char_map[c]; }

std::vector<Block*> FontCreator::getBlocksFromText(std::string name) {
  std::vector<Block*> blocks;
  Tile** tiles = getEmptyTiles();
  int block_ind = 0;
  int block_num = 0;

  for (unsigned int i = 0; i < name.length(); i++) {
    char c = toupper(name.at(i));
    if (!charExists(c)) {
      continue;
    }

    tiles[block_ind] = char_map[c];
    
    if (block_ind == 1) {
      Block* b = new Block(Pos(block_num, 0), tiles);
      blocks.push_back(b);
      tiles = getEmptyTiles();
      block_ind = 0;
      block_num++;
    } else {
      block_ind++;
    }
  }

  if (block_ind != 0) {
    Block* b = new Block(Pos(block_num, 0), tiles);
    blocks.push_back(b);
  }
  return blocks;
}

bool FontCreator::charExists(char c) {
  for (int i = 0; i < 37; i++) {
    if (chars[i] == c) {
      return true;
    }
  }
  return false;
}

Tile** FontCreator::getEmptyTiles() {
  Tile** tiles = new Tile*[4];
  for (int i = 0; i < 4; i++) {
    tiles[i] = Tile::getEmptyTile();
  }
  return tiles;
}

int FontCreator::getNameSize(std::string name) {
  int length = 0;
  for (unsigned int i = 0; i < name.length(); i++) {
    length += charExists(toupper(name.at(i)));
  }
  return length;
}

