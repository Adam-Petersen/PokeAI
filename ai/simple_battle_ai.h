#ifndef SIMPLE_BATTLEAI_H
#define SIMPLE_BATTLEAI_H
#include <cmath>
#include "battle_ai.h"
#include "../battle/battle_state.h"
#include "../data/game_data.h"
#include "../data/item.h"
#include "../data/move.h"
#include "../data/pokemon.h"
#include "../util/battle_input_handler.h"
#include "../util/common.h"

enum BattleMode {
  BM_WAAC, // win at all cost!
  BM_CATCH,
  BM_LEVEL,
  BM_DEFAULT
};

struct MoveChoice {
  Move move;
  uint8_t pp;
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
  Item getBestChance(Pokemon* mon, double* chance);
  double getCatchChance(Pokemon* mon, Item bt);
  MoveChoice findBestStatusMove(BattleState* battle, bool filter_sleep);
  MoveChoice findBestCatchMove(BattleState* battle);
  BattleMode override_mode{BM_DEFAULT};
  int getLevelGoal(int mon_ind, BattleState* state);

  double getTypeEffectivness(MoveType a1, MoveType d1, MoveType d2);


  BattleInput attackTest();
  BattleInput useItemTest();
  BattleInput switchTest();
  BattleInput learnMoveTest(BattleState* state);
  int move_test[5];
  int test_move;
  int test_switch;

};

#endif // SIMPLE_BATTLEAI_H