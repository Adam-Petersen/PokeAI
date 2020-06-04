#ifndef SRC_GAME_STATE
#define SRC_GAME_STATE
#include "goal.h"
#include "../map/world.h"
#include "../data/pokemon.h"
#include "../util/common.h"

struct GameState {
  Chapter* chapter;
  bool should_override;
  Goal* override_goal;

  World* world;
  int num_mons;
  Pokemon** mons;

  uint8_t cur_box_no;
  uint8_t mons_in_box;
  int num_mart_items{0};
  uint8_t* mart_items;

  void deleteOldData() {
    for (int i = 0; i < num_mons; i++) {
      delete mons[i];
    }
    delete [] mons;

    if (num_mart_items > 0) {
      delete [] mart_items;
    }
  }
};

#endif