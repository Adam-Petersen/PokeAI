#include "pokemon_base.h"
Move PokemonBase::getMoveLearned(uint8_t level) {
  std::map<uint8_t, Move>::iterator it;
  it = lvl_move_map.find(level);
  if (it == lvl_move_map.end()) {
    return 0;
  } else {
    return it->second;
  }
}

bool PokemonBase::evolvesAtLvl(uint8_t level) {
  std::map<uint8_t, EvoInfo>::iterator it;
  it = lvl_evo_map.find(level);
  return it != lvl_evo_map.end();
}

EvoInfo PokemonBase::getEvoAtLvl(uint8_t level) {
  return lvl_evo_map.find(level)->second;
}

void PokemonBase::print() {
  printf("index: %d\n", index);
  printf("dex_no: %d\n", dex_no);
  printf("catch rate: %d\n", catch_rate);
  printf("type 1: %d\n", type1);
  printf("type 2: %d\n", type2);
  printf("lvl_evo_pairs: \n");

  for (std::map<uint8_t,EvoInfo>::iterator it=lvl_evo_map.begin(); it!=lvl_evo_map.end(); ++it) {
    it->second.print();
  }
  printf("lvl_move_pairs: \n");
  for (std::map<uint8_t,Move>::iterator it=lvl_move_map.begin(); it!=lvl_move_map.end(); ++it) {
    printf("level: %d, move_id: %d\n", it->first, it->second.getId());
  }

  // printf("hm learns\n");
  // for (int i = 0; i < 57; i++) {
  //   printf("TM %d: %d\n", i, tmlearn[i]);
  // }

  printf("base attack: %d\n\n", base_stats.attack);
}

