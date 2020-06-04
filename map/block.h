#ifndef BLOCK_H
#define BLOCK_H
#include "../util/common.h"
#include "tileset.h"
#include "tile.h"

class Block {
 public:
  // TODO: UPDATE
  // Block(int x, int y, Tileset* tileset, bool is_wall, bool is_exit) : 
  //   x(x), y(y), id(next_id++), tileset(tileset), is_wall(is_wall), is_exit(is_exit) {
  //     printf("next_id: %d\n", next_id);
  //   }

  Block() {}

  Block(Pos pos) : pos(pos), id(next_id++), is_default(false) {
    tiles = new Tile*[4];
    for (int i = 0; i < 4; i++) {
      tiles[i] = Tile::getEmptyTile();
    }
  }

  Block(Pos pos, Tile** tiles) : pos(pos), id(next_id++), is_default(false), tiles(tiles) {}
  
  ~Block() {
    // printf("destroying block with tiles addr %p\n", tiles);

    if (!is_default) {
      delete [] tiles;
    }
  }

  int getId() { return id; }
  Block** get_neighbors() { return adj; }
  Pos getPos() { return pos; }
  Pos getGlobalPos() { return global_pos; }
  void setGlobalPos(Pos p) { global_pos = p; }

  void setTiles(Tile** tile_ptrs) {
    delete [] tiles;
    tiles = tile_ptrs;
    empty = false;
    // printf("tiles set for block %d, addr is %p\n", id, tiles);
  }

  void flipTiles() {
    Tile* t0 = tiles[0];
    Tile* t1 = tiles[1];
    tiles[0] = tiles[2];
    tiles[1] = tiles[3];
    tiles[2] = t0;
    tiles[3] = t1;
  }

  void set_neighbors(Block* u, Block* d, Block* l, Block* r);
  void set_neighbors(Block** neighbors);
  void set_up(Block* b);
  void set_down(Block* b);
  void set_left(Block* b);
  void set_right(Block* b);

  // TODO: add error checking
  Tile* at(int i) { return tiles[i]; }

  bool isEmpty() { return empty; }
  bool wasVisited() { return visited; }
  void setVisited() { visited = true; }

 private:
  static int next_id;
  int id;
  Pos pos;
  Pos global_pos;
  Tileset* tileset;
  bool is_wall;
  bool is_exit;
  Block* adj[4];
  Block* up;
  Block* down;
  Block* left;
  Block* right;
  Tile** tiles;
  bool empty{true};
  bool visited{false};
  bool is_default{true};
};

int Block::next_id = 0;

#endif // BLOCK_H