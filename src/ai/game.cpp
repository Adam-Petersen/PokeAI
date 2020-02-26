#define WINVER 0x0500
#include "game.h"

Game::Game() {}

Game::~Game() {
	// printf("destroying game\
	n");
	delete bm;
	delete world;
	delete input;
	delete pmv;
}

void Game::handleMemoryWrite(uint16_t addr, uint8_t val) {
	// world->handleMemoryWrite(addr, val);
	bm->handleMemoryWrite(addr, val);
}


void Game::start() {
	pmv = new PokeMemViewer();
	world = new World(pmv);
	input = new InputHandler(pmv);
	bm = new BattleManager(pmv, input);
	is_started = true;
	
	// std::unique_lock<std::mutex> lck(kill_mtx);
	// while(true) kill_thread.wait(lck);
	// while(1) {
	// 	// Sleep(200);
	// 	updateWorld();
	// }
}

// not used right now
void Game::updateWorld() {
	world->update();
}



void Game::testMovement1() {
	Movement moves[5];
	input->addMove(moves, 0, RIGHT, 3);
	input->addMove(moves, 1, DOWN, 2);
	input->addMove(moves, 2, LEFT, 4);
	input->addMove(moves, 3, UP, 1);
	input->addMove(moves, 4, RIGHT, 10);

	input->executeMoves(moves, 5);
}