#ifndef POKEMON_H
#define POKEMON_H
#include <map>
#include "move.h"
#include "game_data.h"
#include "pokemon_base.h"
#include "../util/common.h"

struct MovesAndPP {
  Move moves[4];
  uint8_t pps[4];
  int num_moves;

  MovesAndPP() {}
  MovesAndPP(int nm, uint8_t* move_ids, uint8_t* pp_arr);
  void print();
};

class Pokemon {
 public:
  Pokemon() {}

  void printStats();
  void printStatuses();
  void print();

  PokemonBase* getBase() { return base; }
  void setBase(uint8_t index);
  uint8_t getIndex() { return base->getIndex(); }
  uint8_t getDexNo() { return base->getDexNo(); }
  MoveType getType1() { return base->getType1(); }
  MoveType getType2() { return base->getType2(); }
  bool canLearnTM(int tm) { return base->canLearnTM(tm); }
  bool canLearnHM(int hm) { return base->canLearnHM(hm); }

  bool isPlayerMon() { return is_player_mon; }
  void setIsPlayerMon(bool pm) { is_player_mon = pm; }
  std::string getName() { return name; }
  void setName(std::string n) { name = n; }
  uint16_t getCurHp() { return cur_hp; }
  void setCurHp(uint16_t chp) { cur_hp = chp; }
  void setMovesAndPP(int num_moves, uint8_t* move_ids, uint8_t* pps) {
    moves_and_pp = MovesAndPP(num_moves, move_ids, pps);
  }
  MovesAndPP getMovesAndPP() { return moves_and_pp; }
  int getNumValidMoves();
  int getNumMoves() { return moves_and_pp.num_moves; }
  uint8_t getLevel() { return level; }
  void setLevel(uint8_t l) { level = l; }
  uint8_t getMaxHp() { return stats.max_hp; }
  Stats getStats() { return stats; }
  void setStats(int max_hp, int attack, int defense, int speed, int special);
  uint8_t getCatchRate() { return base->getCatchRate(); }

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
  bool isBoxed() { return is_boxed; }
  void setIsBoxed(bool ib) { is_boxed = ib; }
  uint8_t getBoxNo() { return box_no; }
  void setBox(uint8_t bn) { box_no = bn; }
  uint8_t getBoxOffset() { return box_offset; }
  void setBoxOffset(uint8_t bo) { box_offset = bo; }

 private:
  PokemonBase* base;
  bool is_player_mon;
  std::string name;
  uint16_t cur_hp;
  MovesAndPP moves_and_pp;
  uint8_t level;
  Stats stats;
  int num_statuses; 
  std::map<StatusType, bool> statuses;
  bool owned;
  bool is_boxed{false};
  uint8_t box_no;
  uint8_t box_offset;
};

#endif // POKEMON_H