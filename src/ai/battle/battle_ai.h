#ifndef BATTLEAI_H
#define BATTLEAI_H
#include "../util/common.h"
#include "../util/input.h"
#include "battle_state.h"


class BattleAI {
 public:
  virtual BattleInput chooseInput(BattleState* battle);
};

#endif // BATTLEAI_H