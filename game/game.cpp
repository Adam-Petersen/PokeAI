#define WINVER 0x0500
#include "game.h"

Game::Game() {}

Game::~Game() {
	// printf("destroying game\
	n");
	delete bm;
	delete gm;
	delete world;
	delete input;
	delete pmv;
}

void Game::handleMemoryWrite(uint16_t addr, uint8_t val) {
	// world->handleMemoryWrite(addr, val);
	bm->handleMemoryWrite(addr, val);
}


void Game::start() {
	GameData::initGameData();
	pmv = new PokeMemViewer();
	GameData::updateBoxedMons(pmv->getCurBoxNo());
	Sleep(1000);
	// world = new World(pmv);
	input = new InputHandler(pmv);
	bm = new BattleManager(pmv);
	// gm = new GameManager(pmv, world);
	is_started = true;
	// num_chapters = ChapterFactory::createChapters(&chapters);

	// beatGame();
	printf("Game started!\n");


	// std::unique_lock<std::mutex> lck(kill_mtx);
	// while(true) kill_thread.wait(lck);
	// while(1) {
	// 	// Sleep(200);
	// 	updateWorld();
	// }
}

void Game::beatGame() { 
	for (int i = 0; i < num_chapters; i++) {
		gm->beatChapter(chapters[i]);
	}
}

// not used right now
void Game::updateWorld() {
	world->update();
}

Pos Game::getGlobalPos() {
	return world->getGlobalPos();
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