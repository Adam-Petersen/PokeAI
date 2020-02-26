#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H
#include "battle_state.h"
#include "pokemon.h"
#include "battle_ai.h"
#include "./simple_ai/simple_battle_ai.h"
#include "../util/common.h"
#include "../util/input.h"
#include "../util/PokeMemViewer.h"

class BattleManager {
 public:
  BattleManager(PokeMemViewer* pmv, InputHandler* input);
  ~BattleManager();
  void handleMemoryWrite(uint16_t addr, uint8_t val);

 private:
  BattleAI* bai;
  PokeMemViewer* pmv;

  std::mutex battle_turn_mtx;
  std::mutex in_battle_status_mtx;
  int* in_battle_status;
  int* battle_turn;
  InputHandler* input;


  void initPokemon();
  void updateBattleStatus(int ibs);
  void pressUntilTurnChange(int target_status);
  BattlePrompt pressUntilPrompt();
  void skipIntro();
  void cycleAttacks();
  void updateBattleState(BattleState* bs, BattlePrompt prompt);
  void startWildBattle();
  void startTrainerBattle();
  void battle(BattleState* battle);
  void printPrompt(BattlePrompt p);

  int initLevelInfo(LevelInfo** lvl_info, Pokemon** party, int party_size);
  void printLevelInfo(LevelInfo* lvl_info, int size);





};

#endif // BATTLE_MANAGER_H