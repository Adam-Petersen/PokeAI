#include "pokemon.h"
std::map<uint8_t, Move*> Pokemon::moves_list;
std::map<uint8_t, uint8_t> Pokemon::index_to_dex_map = initCaughtMonMap();

Move* Pokemon::findMove(uint8_t id) {
  auto m = moves_list.find(id);
  return m == moves_list.end() ? NULL : moves_list[id];
}

void Pokemon::printMoves() {
  printf("--- moves ---\n");
  for(int i = 0; i < num_moves; i++) {
    move_instances[i].print();
  }
  printf("-------------\n");
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
  printf("is_player_mon: %d\n", is_player_mon);
  printf("index: %d\n", index);
  printf("dex_no: %d\n", dex_no);
  printf("cur_hp: %d\n", cur_hp);
  printf("type1: %d\n", type1);
  printf("type2: %d\n", type2);
  printMoves();
  printf("level: %d\n", level);
  printf("max_hp: %d\n", max_hp);
  printStats();
  printf("catch_rate: %d\n", catch_rate);
  printStatuses();
  printf("owned: %d\n", owned);
  printf("---------------------------------\n");
}

void Pokemon::setStats(int attack, int defense, int speed, int special) {
  stats = Stats(attack, defense, speed, special);
}