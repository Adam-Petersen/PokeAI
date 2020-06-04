#ifndef MOVE_H
#define MOVE_H
#include <vector>
#include "../util/common.h"

enum StatusType {
  S_FREEZE,
  S_SLEEP,
  S_BURN,
  S_PARALYSIS,
  S_POISON
};

enum MoveEffect {
  POISON_SIDE_EFFECT1=0x02,
  BURN_SIDE_EFFECT1=0x04,
  FREEZE_SIDE_EFFECT=0x05,
  PARALYZE_SIDE_EFFECT1=0x06,
  EXPLODE_EFFECT=0x07,
  POISON_SIDE_EFFECT2=0x21,
  BURN_SIDE_EFFECT2=0x22,
  PARALYZE_SIDE_EFFECT2=0x24,
  SLEEP_EFFECT=0x20,
  POISON_EFFECT=0x42,
  PARALYZE_EFFECT=0x43,
  ME_NONE=0x00
};


enum MoveType {
  NORMAL=0x00,
  FIGHTING=0x01,
  FLYING=0x02,
  POISON=0x03,
  GROUND=0x04,
  ROCK=0x05,
  BUG=0x07,
  GHOST=0x08,
  FIRE=0x14,
  WATER=0x15,
  GRASS=0x16,
  ELECTRIC=0x17,
  PSYCHIC=0x18,
  ICE=0x19,
  DRAGON=0x1A,
  UNKNOWN=0xFF
};

class Move {
 public:
  Move() : effects{ 0x02, 0x04, 0x05, 0x06, 0x07, 0x21, 0x22, 0x24, 0x20, 0x42, 0x43 } {}
  Move(uint8_t id) : id(id) { Move(); }
  
  uint8_t getId() { return id; }
  void setId(uint8_t id_) { id = id_; }
  MoveEffect getEffect() { return move_effect; }
  void setMoveEffect(MoveEffect me) { move_effect = me; }
  void setEffect(uint8_t ef) { 
    for (int i = 0; i < 10; i++) {
      if (ef == effects[i]) {
        move_effect = static_cast<MoveEffect>(ef);
        return;
      }
    }
    move_effect = ME_NONE; 
  }
  int getPower() { return power; }
  void setPower(int p) { power = p; }
  MoveType getType() { return type; }
  void setType(MoveType t) { type = t; }
  double getAccuracy() { return accuracy; }
  void setAccuracy(double ac) { accuracy = ac; }
  int getPP() { return pp; }
  void setPP(int pp_) { pp = pp_; pp_set = true; }
  bool isPPSet() { return pp_set; }
  MoveEffect getMoveEffect() { return move_effect; }

  double getStatusScore() {
    switch (move_effect) {
      case POISON_SIDE_EFFECT1:
      case BURN_SIDE_EFFECT1:
      case PARALYZE_SIDE_EFFECT1:
        return 30*.10;
      case POISON_SIDE_EFFECT2:
      case BURN_SIDE_EFFECT2:
      case PARALYZE_SIDE_EFFECT2:
        return 30*.30;
      case POISON_EFFECT:
      case PARALYZE_EFFECT:
        return 30;
      case FREEZE_SIDE_EFFECT:
        return 50*.1;
      case SLEEP_EFFECT:
        return 50;
      default:
        return 0;
    }
  }

  double getStatusPriority() {
    switch (move_effect) {
      case POISON_SIDE_EFFECT1:
      case BURN_SIDE_EFFECT1:
        return 1;
      case POISON_SIDE_EFFECT2:
      case BURN_SIDE_EFFECT2:
        return 2;
      case POISON_EFFECT:
        return 3;
      case PARALYZE_SIDE_EFFECT1:
        return 4;
      case PARALYZE_SIDE_EFFECT2:
        return 5;
      case PARALYZE_EFFECT:
        return 6;
      case SLEEP_EFFECT:
        return 7;
      case FREEZE_SIDE_EFFECT:
        return 8;
      default:
        return 0;
    }
  }

  bool hasBadEffect() {
    if (move_effect == EXPLODE_EFFECT) {
      return true;
    }
    return false;
  }

  bool isHM() {
    uint8_t hm_moves[5] = { 0x0F, 0x13, 0x39, 0x46, 0x94 };
    for (int i = 0; i < 5; i++) {
      if (hm_moves[i] == id) {
        return true;
      }
    }
    return false;
  }

  void print() {
    printf("id: %d\n", id);
    printf("effect: %d\n", move_effect);
    printf("power: %d\n", power);
    printf("type: %d\n", type);
    printf("accuracy: %f\n", accuracy);
    printf("pp: %d\n\n", pp);
  }

  std::string getPrintString() {
    std::string s = "";
    s += "id: " + std::to_string(id) + "\n";
    s += "effect: " + std::to_string(move_effect) + "\n";
    s += "power: " + std::to_string(power) + "\n";
    s += "type: " + std::to_string(type) + "\n";
    s += "accuracy: " + std::to_string(accuracy) + "\n";
    s += "pp: " + std::to_string(pp) + "\n\n";
    return s;
  }

 private:

  uint8_t id;
  MoveEffect move_effect;
  int power;
  MoveType type{UNKNOWN};
  double accuracy;
  int pp;
  bool pp_set{false};
  uint8_t effects[11];



  int max_dmg_received;
  int effectiveness;
  std::vector<StatusType> status_inflicted;
};


#endif // MOVE_H