#ifndef GAME_DATA
#define GAME_DATA
#include <map>
#include <vector>
#include "move.h"
#include "pokemon.h"
#include "pokemon_base.h"
#include "type_string.h"
#include "../util/common.h"
#include "../util/read_rom.h"

struct ROMHeader {
  std::string name;
  unsigned int addr; // Map header location
  uint8_t id;

  ROMHeader(std::string name, unsigned int addr) :
    name(name), addr(addr) {}
  ROMHeader(uint8_t id, unsigned int addr, std::string name) :
    id(id), addr(addr), name(name) {}
};

class Pokemon; // TODO: Solve circular reference
class GameData {
 public:
  static void initGameData();
  static void updateBoxedMons(uint8_t cur_box_no);

  static void printMoveList();
  static void printBaseMons();
  static void printTypeMatrix();
  static void printBoxedMons();
  static void writeMoveList();


  static std::map<uint8_t, uint8_t> index_to_dex_map;
  static std::map<uint8_t, PokemonBase*> dex_to_base_map;
  static Move move_list[0xA6];
  static double type_matrix[15][15];
  static std::map<MoveType, int> type_to_ind_map;
  static std::vector<Pokemon> boxed_mons;
  static std::vector<ROMHeader> map_data_info;
  static std::vector<ROMHeader> tileset_data_info;
 private:
  static void initMoveList();
  static void initTypeMatrix();
  static void initBasePokemon();
  static void initIndexMap();
  static void initMapData();
  static void initTilesetData();
};

#endif