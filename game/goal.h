#ifndef SRC_GAME_GOALS
#define SRC_GAME_GOALS
#include "../data/hm.h"
#include "../data/item.h"
#include "../data/pokemon.h"
#include "../util/common.h"

enum GoalType {
  GT_USE_HM,
  GT_LEARN_HM,
  GT_ACTIVATE,
  GT_SKIP_DIALOG,
  GT_SWITCH_IN_PARTY,
  GT_SWITCH_BOX,
  GT_WITHDRAW_MON,
  GT_DEPOSIT_MON,
  GT_BUY_ITEM,
  GT_USE_ITEM_ON_MON,
  GT_MOVE_TO_BLOCK,

  GT_TALK_TO_PERSON,
  GT_MOVE_TO_MAP,
  GT_FIND_HEALING,
  GT_FIND_SHOP,
  GT_GRAB_ITEM,
  

  GT_ENTER_NAME,
  GT_FLY,
  GT_USE_ITEM,
  GT_DONE
};

struct Goal {
  GoalType type;
  uint8_t data1, data2;

  virtual void print() { printf("Base Goal Class\n"); }

  Goal() {}
  Goal(GoalType type) : type(type) {}
  Goal(GoalType type, uint8_t data1) : type(type), data1(data1) {}
  Goal(GoalType type, uint8_t data1, uint8_t data2) : type(type), data1(data1), data2(data2) {}
};


struct UseHM : virtual Goal {
  HMData hm;
  UseHM(HMData hm) : Goal(GT_USE_HM), hm(hm) {}
};

struct LearnHM : virtual Goal {
  HMData hm;
  LearnHM(HMData hm) : Goal(GT_LEARN_HM), hm(hm) {}
};

struct WithdrawMon : virtual Goal {
  Pokemon mon;
  WithdrawMon(Pokemon mon) : Goal(GT_WITHDRAW_MON), mon(mon) {}
};

struct BuyItem : virtual Goal {
  Item item;
  int quantity;
  BuyItem(Item item, int quantity) : Goal(GT_BUY_ITEM), item(item), quantity(quantity) {}
};

struct UseItemOnMon : virtual Goal {
  Item item;
  uint8_t mon_offset;
  UseItemOnMon(Item item, uint8_t mon_offset) : Goal(GT_USE_ITEM_ON_MON), item(item), mon_offset(mon_offset) {}
};

struct Chapter {
  Goal** goals;
  Goal* cur_goal;
  int cur_goal_ind{0};

  Chapter() {}
  Chapter(Goal** goals) : goals(goals) {
    cur_goal = goals[0];
    cur_goal_ind = 0;
  }
  
  void incGoal() {
    cur_goal_ind++;
    cur_goal = goals[cur_goal_ind];
  }
};


#endif