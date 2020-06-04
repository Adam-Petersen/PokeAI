#include "pokemon.h"

MovesAndPP::MovesAndPP(int nm, uint8_t* move_ids, uint8_t* pp_arr) {
  num_moves = nm;
  for (int i = 0; i < num_moves; i++) {
    moves[i] = GameData::move_list[move_ids[i]];
    pps[i] = pp_arr[i];
  }
}

void MovesAndPP::print() {
  printf("num_moves: %d", num_moves);
  for (int i = 0; i < num_moves; i++) {
    printf("move %d, id: %d, pp: %d", i+1, moves[i].getId(), pps[i]);
  }
}

void Pokemon::setBase(uint8_t index) {
  base = GameData::dex_to_base_map[GameData::index_to_dex_map[index]];
}


int Pokemon::getNumValidMoves() {
  if (cur_hp <= 0) { return 0; }

  int num = 0;
  for (int i = 0; i < moves_and_pp.num_moves; i++) {
    if (moves_and_pp.pps[i] > 0) {
      num += 1;
    }
  }

  return num;
}

void Pokemon::printStats() {
  printf("--- Stats ---\n");
  printf("attack: %d\n", stats.attack);
  printf("defense: %d\n", stats.defense);
  printf("speed: %d\n", stats.speed);
  printf("special: %d\n", stats.special);
  printf("-------------\n");
}

void Pokemon::printStatuses() {
  printf("--- Statuses ---\n");
  printf("paralyzed: %d\n", isParalyzed());
  printf("frozen: %d\n", isFrozen());
  printf("burned: %d\n", isBurned());
  printf("poisoned: %d\n", isPoisoned());
  printf("asleep: %d\n", isAsleep());
  printf("num_statuses: %d\n", num_statuses);
  printf("----------------\n");
}

void Pokemon::print() {
  printf("-----------Begin Mon-------------\n");
  printf("base:\n");
  base->print();
  printf("is_player_mon: %d\n", is_player_mon);
  printf("cur_hp: %d\n", cur_hp);
  moves_and_pp.print();
  printf("level: %d\n", level);
  printStats();
  printStatuses();
  printf("owned: %d\n", owned);
  printf("is_boxed: %d\n", is_boxed);
  printf("box: %d\n", box_no);
  printf("box offset: %d\n", box_offset);
  printf("---------------------------------\n");
}

void Pokemon::setStats(int max_hp, int attack, int defense, int speed, int special) {
  stats = Stats(max_hp, attack, defense, speed, special);
}