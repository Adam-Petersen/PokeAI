#ifndef SRC_HM
#define SRC_HM
#include "game_data.h"
#include "item.h"
#include "move.h"
#include "../util/common.h"

struct HMData {
  Move move;
  uint8_t move_id;
  uint8_t hm_no;
  Item item;

  HMData(Move move, uint8_t move_id, uint8_t hm_no, Item item) :
    move(move), move_id(move_id), hm_no(hm_no), item(item) {}

  void print() {
    printf("move_id: %d\n", move_id);
    printf("hm_no: %d\n", hm_no);
    printf("item: %d\n",static_cast<int>(item));
    move.print();
  }
};

class HM {
 public:
  static HMData CUT, FLY, SURF, STRENGTH, FLASH;
};

HMData HM::CUT = HMData(GameData::move_list[0x0F], 0x0F, 1, Item::HM_CUT);
HMData HM::FLY = HMData(GameData::move_list[0x13], 0x13, 2, Item::HM_FLY);
HMData HM::SURF = HMData(GameData::move_list[0x39], 0x39, 3, Item::HM_SURF);
HMData HM::STRENGTH = HMData(GameData::move_list[0x46], 0x46, 4, Item::HM_STRENGTH);
HMData HM::FLASH = HMData(GameData::move_list[0x94], 0x94, 5, Item::HM_FLASH);


#endif