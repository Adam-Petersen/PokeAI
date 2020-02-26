#ifndef POKEMON_H
#define POKEMON_H
#include <map>
#include "../util/common.h"
#include "init_caught_mon.h"
#include "move.h"

struct Stats {
  uint16_t attack{0};
  uint16_t defense{0};
  uint16_t speed{0};
  uint16_t special{0};

  Stats() {}
  Stats(uint16_t attack, uint16_t defense, uint16_t speed, uint16_t special) :
    attack(attack), defense(defense), speed(speed), special(special) {}

  double average() {
    return (attack + defense + speed + special) / 4.0;
  }

  double total() {
    return attack + defense + speed + special;
  }
};


class Pokemon {
 public:
  Pokemon() {}
  ~Pokemon() {}
  static std::map<uint8_t, Move*> moves_list;
  static Move* findMove(uint8_t id);
  static std::map<uint8_t, uint8_t> index_to_dex_map;

  void printMoves();
  void printStats();
  void printStatuses();
  void print();

  bool isPlayerMon() { return is_player_mon; }
  void setIsPlayerMon(bool pm) { is_player_mon = pm; }
  std::string getName() { return name; }
  void setName(std::string n) { name = n; }
  uint8_t getIndex() { return index; }
  void setSpecies(uint8_t ind) {
    index = ind;
    dex_no = index_to_dex_map[ind];
  }
  void setIndex(uint8_t i) { index = i; }
  uint8_t getDexNo() { return dex_no; }
  void setDexNo(uint8_t dn) { dex_no = dn; }
  uint16_t getCurHp() { return cur_hp; }
  void setCurHp(uint16_t chp) { cur_hp = chp; }
  MoveType getType1() { return type1; }
  void setType1(MoveType t) { type1 = t; }
  void setType1(uint8_t t) { type1 = static_cast<MoveType>(t); }
  MoveType getType2() { return type2; }
  void setType2(MoveType t) { type2 = t; }
  void setType2(uint8_t t) { type2 = static_cast<MoveType>(t); }
  
  void setMoves(int nm, Move* m) { num_moves = nm; moves = m; }
  Move* getMoves() { return moves; }
  uint8_t getNumMoves() { return num_moves; }
  void setMoveInstances(uint8_t nm, MoveInstance* mi) {num_moves = nm;  move_instances = mi; }
  MoveInstance* getMoveInstances() { return move_instances; }

  uint8_t getLevel() { return level; }
  void setLevel(uint8_t l) { level = l; }
  uint8_t getMaxHp() { return max_hp; }
  void setMaxHp(uint8_t mhp) { max_hp = mhp; }
  Stats getStats() { return stats; }
  void setStats(int attack, int defense, int speed, int special);
  uint8_t getCatchRate() { return catch_rate; }
  void setCatchRate(uint8_t cr) { catch_rate = cr; }

  void setStatuses(uint8_t status_byte) {
    int masks[] = { 0x40, 0x20, 0x10, 0x08, 0x07};
    num_statuses = 0;
    for (int i = 0; i < 5; i++) {
      if ((status_byte & masks[i]) > 0) {
        num_statuses++;
      }
    }

    statuses[S_PARALYSIS] = (status_byte & 0x40) > 0;
    statuses[S_FREEZE] = (status_byte & 0x20) > 0;
    statuses[S_BURN] = (status_byte & 0x10) > 0;
    statuses[S_POISON] = (status_byte & 0x08) > 0;
    statuses[S_SLEEP] = (status_byte & 0x07) > 0;
  }

  bool isParalyzed() { return statuses[S_PARALYSIS]; }
  bool isFrozen () { return statuses[S_FREEZE]; }
  bool isBurned() { return statuses[S_BURN]; }
  bool isPoisoned() { return statuses[S_POISON]; }
  bool isAsleep() { return statuses[S_SLEEP]; }
  int getNumStatuses() { return num_statuses; }
  bool isOwned() { return owned; }
  void setOwned(bool o) { owned = o; }

 private:
  bool is_player_mon;
  std::string name;
  uint8_t index;
  uint8_t dex_no;
  uint16_t cur_hp;
  MoveType type1;
  MoveType type2;
  int num_moves;
  Move* moves;
  MoveInstance* move_instances;
  uint8_t level;
  uint8_t max_hp;
  Stats stats;
  uint8_t catch_rate{0};
  int num_statuses; 
  std::map<StatusType, bool> statuses;
  bool owned;

};

#endif // POKEMON_H