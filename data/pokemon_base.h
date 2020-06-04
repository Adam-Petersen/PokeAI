#ifndef POKEMON_BASE
#define POKEMON_BASE
#include <map>
#include "move.h"
#include "../util/common.h"

enum EvoType {
  EV_LEVEL=0x01,
  EV_ITEM=0x02,
  EV_TRADE=0x03
};

struct EvoInfo {
  EvoType type;
  uint8_t item_id;
  uint8_t level;
  uint8_t new_mon_id;

  void print() {
    printf("type: %d\n", type);
    if (type == EV_ITEM) { printf("item_id: %d\n", item_id); }
    printf("level: %d\n", level);
    printf("new_mon_id: %d\n", new_mon_id);
  }
};

struct Stats {
  uint16_t max_hp{0};
  uint16_t attack{0};
  uint16_t defense{0};
  uint16_t speed{0};
  uint16_t special{0};

  Stats() {}
  Stats(uint16_t max_hp, uint16_t attack, uint16_t defense, uint16_t speed, uint16_t special) :
    max_hp(max_hp), attack(attack), defense(defense), speed(speed), special(special) {}

  double average() {
    return (attack + defense + speed + special) / 4.0;
  }

  double total() {
    return attack + defense + speed + special;
  }
};

class PokemonBase {
 public:
  PokemonBase() {}

  Move getMoveLearned(uint8_t level);
  bool evolvesAtLvl(uint8_t level);
  EvoInfo getEvoAtLvl(uint8_t level);
  void setTMFlag(uint8_t tm, bool b) { tmlearn[tm] = b;}
  bool canLearnTM(uint8_t tm) { return tmlearn[tm]; }
  bool canLearnHM(uint8_t hm) { return tmlearn[hm+50]; }
  void print();

  uint8_t getIndex() { return index; }
  void setIndex(uint8_t i) { index = i; }
  uint8_t getDexNo() { return dex_no; }
  void setDexNo(uint8_t dn) { dex_no = dn; }
  MoveType getType1() { return type1; }
  void setType1(MoveType t) { type1 = t; }
  void setType1(uint8_t t) { type1 = static_cast<MoveType>(t); }
  MoveType getType2() { return type2; }
  void setType2(MoveType t) { type2 = t; }
  void setType2(uint8_t t) { type2 = static_cast<MoveType>(t); }
  uint8_t getCatchRate() { return catch_rate; }
  void setCatchRate(uint8_t cr) { catch_rate = cr; }
  Stats getBaseStats() { return base_stats; }
  void setBaseStats(int max_hp, int attack, int defense, int speed, int special) {
    base_stats = Stats(max_hp, attack, defense, speed, special);
  }

  void addLvlEvoPair(uint8_t lvl, EvoInfo ei) { lvl_evo_map[lvl] = ei; }
  void addLvlMovePair(uint8_t lvl, Move move) { lvl_move_map[lvl] = move; }

 private:
  uint8_t index;
  uint8_t dex_no;
  uint8_t catch_rate{0};
  MoveType type1;
  MoveType type2;
  std::map<uint8_t, EvoInfo> lvl_evo_map;
  std::map<uint8_t, Move> lvl_move_map;
  bool tmlearn[57]{0};
  Stats base_stats;
};

#endif