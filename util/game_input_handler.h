#ifndef SRC_GAME_INPUT_MANAGER
#define SRC_GAME_INPUT_MANAGER
#include <string>
#include "input_handler.h"
#include "../data/item.h"
#include "../game/goal.h"

class GameInputHandler : public InputHandler {
 public:
  GameInputHandler(PokeMemViewer* pmv) : InputHandler(pmv) {}

  void useHM(uint8_t mon_offset, uint8_t move_offset);
  void learnHM(Item item, int mon_offset, int move_offset);
  void skipDialog();
  void switchWithinParty(uint8_t mon_1, uint8_t mon_2);
  void switchBox(uint8_t box_offset);
  void withdrawMon(uint8_t mon_offset);
  void depositMon(uint8_t mon_offset);
  void buyItem(uint8_t item_offset, uint8_t quantity);
  void useItemOnMon(Item item, uint8_t mon_offset);


  void fly(uint8_t mon_id);
  void enterName(std::string name);

 private:
  void pressUntilMenu(uint16_t box_pos);
  void navToBoxMenu();
  
};


#endif