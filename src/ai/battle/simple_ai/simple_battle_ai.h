#ifndef SIMPLE_BATTLEAI_H
#define SIMPLE_BATTLEAI_H
#include <map>
#include <cmath>
#include "../battle_ai.h"
#include "../pokemon.h"
#include "../battle_state.h"
#include "../move.h"
#include "../../util/common.h"
#include "../../util/input.h"
#include "../../util/read_rom.h"
#include "type_string.h"

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

struct EvosAndMoves {
  uint8_t mon_id;
  std::map<uint8_t, EvoInfo>* lvl_evo_pairs;
  std::map<uint8_t, uint8_t>* lvl_move_pairs;
  
  uint8_t getMoveLearned(uint8_t level) {
    std::map<uint8_t, uint8_t>::iterator it;
    it = lvl_move_pairs->find(level);
    if (it == lvl_move_pairs->end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  bool evolvesAtLvl(uint8_t level) {
    std::map<uint8_t, EvoInfo>::iterator it;
    it = lvl_evo_pairs->find(level);
    return it != lvl_evo_pairs->end();
  }

  EvoInfo getEvoAtLvl(uint8_t level) {
    return lvl_evo_pairs->find(level)->second;
  }

  void print() {
    printf("mon_id: %d\n", mon_id);
    printf("lvl_evo_pairs: \n");
    for (std::map<uint8_t,EvoInfo>::iterator it=lvl_evo_pairs->begin(); it!=lvl_evo_pairs->end(); ++it) {
      it->second.print();
    }
    printf("lvl_move_pairs: \n");
    for (std::map<uint8_t,uint8_t>::iterator it=lvl_move_pairs->begin(); it!=lvl_move_pairs->end(); ++it) {
      printf("level: %d, move_id: %d\n", it->first, it->second);
    }
  }
};

enum BattleMode {
  BM_WAAC, // win at all cost!
  BM_CATCH,
  BM_LEVEL,
  BM_DEFAULT
};

enum BallType {
  BT_POKEBALL=0x04,
  BT_GREATBALL=0x03,
  BT_ULTRABALL=0x02,
  BT_SAFARIBALL=0x08
};

struct MoveChoice {
  MoveInstance move_instance;
  Move move;
  uint8_t move_owner;
  uint8_t move_pos;
  double max_damage;
  double min_damage;
  double status_score;
  double no_status_score;
  bool is_default;

  MoveChoice() : is_default(false) {}
  MoveChoice(bool is_default) : is_default(is_default) {}

  void print() {
    move.print();
    move_instance.print();
    printf("move_owner: %d\n", move_owner);
    printf("move_pos: %d\n", move_pos);
    printf("max_damage: %.2f\n", max_damage);
    printf("min_damage: %.2f\n", min_damage);
    printf("no_status_score: %.2f\n", no_status_score);
    printf("status_score: %.2f\n", status_score);
    printf("default: %d\n", is_default);
    printf("------------\n");
  }
};

class SimpleBattleAI : public BattleAI {
 public: 
  SimpleBattleAI();
  BattleInput chooseInput(BattleState* battle) override;

 private:
  // AI's
  BattleInput chooseWAACMove(BattleState* battle);
  BattleInput chooseCatchMove(BattleState* battle);
  BattleInput chooseLevelMove(BattleState* state);
  BattleInput chooseNewMove(BattleState* state);

  // Helpers
  int initMoveChoices(MoveChoice** ptr, BattleState* battle, bool player);
  void printMoveChoices(MoveChoice* mcs, int size);
  bool isBetterMove(MoveChoice a, MoveChoice b, int score_compare);
  double getMaxDamagePossible(Move move, Pokemon* attacker, Pokemon* target);
  double getMinDamagePossible(Move move, Pokemon* attacker, Pokemon* target);
  double getDamage(Move move, Pokemon* attacker, Pokemon* target, bool max);
  bool isSpecialMove(Move move);
  BallType getBestChance(Pokemon* mon, double* chance);
  double getCatchChance(Pokemon* mon, BallType bt);
  MoveChoice findBestStatusMove(BattleState* battle, bool filter_sleep);
  MoveChoice findBestCatchMove(BattleState* battle);
  BattleMode override_mode{BM_DEFAULT};
  int getLevelGoal(int mon_ind, BattleState* state);

  void initMoveList();
  void printMoveList();
  void writeMoveList();
  void initEvosMovesList();
  void printEvosMovesList();
  void initTypeMatrix();
  double getTypeEffectivness(MoveType a1, MoveType d1, MoveType d2);
  void printTypeMatrix();
  Move move_list[0xA6];
  std::map<uint8_t,EvosAndMoves> evos_and_moves_map; 
  std::map<MoveType, int> type_to_ind_map;
  double type_matrix[15][15];

  BattleInput attackTest();
  BattleInput useItemTest();
  BattleInput switchTest();
  BattleInput learnMoveTest(BattleState* state);
  int move_test[5];
  int test_move;
  int test_switch;

};

#endif // SIMPLE_BATTLEAI_H