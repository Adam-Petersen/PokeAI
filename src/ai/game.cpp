#define WINVER 0x0500
#include "game.h"

Game::Game() {
	pmv = new PokeMemViewer();
	input = new InputHandler(pmv);
}

Game::~Game() {
	// printf("destroying game\
	n");
	delete input;
	delete world;
	delete pmv;
}

void Game::start() {
	world = new World(pmv);
	world->start();
	
	std::unique_lock<std::mutex> lck(kill_mtx);
	while(true) kill_thread.wait(lck);
	// while(1) {
	// 	// Sleep(200);
	// 	updateWorld();
	// }
}

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