#ifndef BATTLEAI_H
#define BATTLEAI_H
#include "../battle/battle_state.h"
#include "../util/battle_input_handler.h"
#include "../util/common.h"


class BattleAI {
 public:
  virtual BattleInput chooseInput(BattleState* battle);
};

#endif // BATTLEAI_H