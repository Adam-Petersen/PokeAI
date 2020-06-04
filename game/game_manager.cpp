#include "game_manager.h"

GameManager::GameManager(PokeMemViewer* pmv, World* world) : pmv(pmv), world(world) {
  input = new GameInputHandler(pmv);
  ai = new GameAI(input);
  state = new GameState;
  state->world = world;
  initGameState();
}

GameManager::~GameManager() {
  delete input;
}

void GameManager::beatChapter(Chapter* chapter) {
  state->chapter = chapter;

  while (state->chapter->cur_goal->type != GT_DONE) {
    ai->execGoal(state);
    updateGameState();
  }
}

void GameManager::initGameState() {
  pmv->initPartyMons(&state->mons, &state->num_mons);
  state->cur_box_no = pmv->getCurBoxNo();
  state->mons_in_box = pmv->getMonsInBox();
  pmv->getMartItems(&state->mart_items, &state->num_mart_items);
}

void GameManager::updateGameState() {
  state->deleteOldData();
  initGameState();
}