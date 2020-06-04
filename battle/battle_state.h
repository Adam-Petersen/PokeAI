#ifndef BATTLE_H
#define BATTLE_H
#include "../data/move.h"
#include "../data/pokemon.h"
#include "../util/common.h"

enum BattlePrompt {
  BP_BATTLE_MENU,
  BP_NICKNAME,
  BP_SWITCH_YES_NO,
  BP_SWITCH_MENU,
  BP_LEARN_MOVE,
  BP_ENDED
};

enum BattleType {
  TRAINER,
  ENCOUNTER,
  SAFARI,
  OLDMAN
};

struct MoveUsed {
  Move* move;
  Pokemon* attacking_mon;
  Pokemon* target_mon;
  int resulting_hp;
  bool miss;
  bool failed;
  bool crit;
  StatusType status_induced;
};

struct LevelInfo {
  uint8_t mon_ind;
  uint8_t level_goal;

  void print() {
    printf("ind: %d\n", mon_ind);
    printf("goal: %d\n\n", level_goal);
  }
};

struct BattleState {
  BattleType b_type;

  int num_player_mons;
  Pokemon** player_mons;
  Pokemon* cur_player_mon;
  int cur_player_mon_ind;

  int num_enemy_mons;
  Pokemon** enemy_mons;
  Pokemon* cur_enemy_mon;
  int cur_enemy_mon_ind;
  
  BattlePrompt prompt;
  std::vector<MoveUsed> moves_used;
  int balls_used{0};

  LevelInfo* mons_to_level;
  int num_mons_to_level;
  std::string leveling_mon_name{""};

  BattleState() {
    num_player_mons = 0;
    num_enemy_mons = 0;
    num_mons_to_level = 0;
  }

  void deleteOldData() {
    for (int i = 0; i < num_player_mons; i++) {
      delete player_mons[i];
    }
    if (num_player_mons > 0) { delete [] player_mons; }
    num_player_mons = 0;

    for (int i = 0; i < num_enemy_mons; i++) {
      delete enemy_mons[i];
    }
    if (num_enemy_mons > 0) { delete [] enemy_mons; }
    num_enemy_mons = 0;

    if (num_mons_to_level > 0) {
      delete [] mons_to_level;
    }
  }

  void print() {
    printf("-------------- BattleState --------------\n");
    printf("b_type: %d\n", b_type);

    printf("num_player_mons: %d\n", num_player_mons);    
    for (int i = 0; i < num_player_mons; i++) {
      printf("--- Player Pokemon %d ---\n", i+1);
      player_mons[i]->print();
      printf("\n");
    }
    printf("cur_player_mon id: %d\n", cur_player_mon->getIndex());

    printf("num_enemy_mons: %d\n", num_enemy_mons);    
    for (int i = 0; i < num_enemy_mons; i++) {
      printf("--- Enemy Pokemon %d ---\n", i+1);
      enemy_mons[i]->print();
      printf("\n");
    }
    printf("cur_enemy_mon id: %d\n", cur_enemy_mon->getIndex());
    printf("prompt: %d\n", prompt);
    printf("moves used: not supported\n\n");
  }

  ~BattleState() {
    for (int i = 0; i < num_player_mons; i++) {
      delete player_mons[i];
    }
    delete [] player_mons;

    for (int i = 0; i < num_enemy_mons; i++) {
      delete enemy_mons[i];
    }
    delete [] enemy_mons;
  }
};

#endif // BATTLE_H