#ifndef SRC_CHAPTERS
#define SRC_CHAPTERS
#include "goal.h"
#include "../data/game_data.h"
#include "../data/hm.h"
#include "../data/item.h"
#include "../util/common.h"

class ChapterFactory {
 public:
  static int createChapters(Chapter*** result) {
    Chapter** chapters = new Chapter*[1];
    getTestChapter(&chapters[0]);
    *result = chapters;
    return 1;
  }

  static void getTestChapter(Chapter** result) {
    Goal** goals = new Goal*[2];
    int i = 0;

    // goals[i++] = new UseHM(HM::CUT);
    // goals[i++] = new LearnHM(HM::CUT);
    // goals[i++] = new Goal(GT_ACTIVATE);
    // goals[i++] = new Goal(GT_SKIP_DIALOG);
    // goals[i++] = new Goal(GT_SWITCH_IN_PARTY, 1, 4);
    // goals[i++] = new Goal(GT_DEPOSIT_MON, 2);
    // goals[i++] = new BuyItem(Item::ANTIDOTE, 10);

    goals[i++] = new UseItemOnMon(Item::POTION, 0);

    goals[i++] = new Goal(GT_DONE);
    Chapter* c = new Chapter(goals);
    *result = c;
  }
};

#endif