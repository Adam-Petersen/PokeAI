#include <cstddef>
#include <map>
#include <mutex>
#include <condition_variable>
#include "../common/Types.h"

// std::map<uint16_t, uint8_t> monitoredAddresses;
// std::map<uint16_t, std::condition_variable*> monitoredAddressCVS;
// std::mutex player_pos_mtx;
// std::mutex cur_map_mtx;
// std::mutex kill_mtx;
// std::condition_variable player_pos_cv;
// std::condition_variable cur_map_cv;
// std::condition_variable kill_thread;
// int tile_bytes_changed = 0;
// bool update_map = false;
// Game game;

uint8_t* gbMemoryMap[16];

int gbRomSizeMask = 0;
int gbRomSize = 0;
int gbRamSizeMask = 0;
int gbRamSize = 0;
int gbTAMA5ramSize = 0;

uint8_t* gbMemory = NULL;
uint8_t* gbVram = NULL;
uint8_t* gbRom = NULL;
uint8_t* gbRam = NULL;
uint8_t* gbWram = NULL;
uint16_t* gbLineBuffer = NULL;
uint8_t* gbTAMA5ram = NULL;

uint16_t gbPalette[128];
uint8_t gbBgp[4] = { 0, 1, 2, 3 };
uint8_t gbObp0[4] = { 0, 1, 2, 3 };
uint8_t gbObp1[4] = { 0, 1, 2, 3 };
int gbWindowLine = -1;

bool genericflashcardEnable = false;
int gbCgbMode = 0;

uint16_t gbColorFilter[32768];
int gbColorOption = 0;
int gbPaletteOption = 0;
int gbEmulatorType = 0;
int gbBorderOn = 0;
int gbBorderAutomatic = 0;
int gbBorderLineSkip = 160;
int gbBorderRowSkip = 0;
int gbBorderColumnSkip = 0;
int gbDmaTicks = 0;

uint8_t (*gbSerialFunction)(uint8_t) = NULL;
