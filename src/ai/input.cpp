#define WINVER 0x0500
#include "input.h"

void InputHandler::executeMoves(Movement* moves, int size) {
    for(int i = 0; i < size; i++) {
        executeMove(moves[i]);
        Sleep(200);
    }
}

void InputHandler::executeMove(Movement move) {
    Pos cur_pos = mem->getPos();
    Pos tgt_pos = cur_pos;

    if (move.dir == UP) {
        tgt_pos.y -= move.dist;
    } else if (move.dir == DOWN) {
        tgt_pos.y += move.dist;
    } else if (move.dir == LEFT) {
        tgt_pos.x -= move.dist;
    } else if (move.dir == RIGHT) {
        tgt_pos.x += move.dist;
    } else {
        throw "Invalid movement direction!";
    }

    printf("\ncurx, cury: %u, %u\nnewx, newy: %u, %u\n\n", cur_pos.x, cur_pos.y, tgt_pos.x, tgt_pos.y);


    // Perform movement
    Pos last_pos = cur_pos;
    printf("itrx, itry: %u, %u\n", last_pos.x, last_pos.y);
    pressKey(move.dir);
    while(1) {
        Pos new_pos = mem->getPos();
        if (new_pos != last_pos) {
            printf("itrx, itry: %u, %u\n", new_pos.x, new_pos.y);
            last_pos = new_pos;
        }
        if (new_pos == tgt_pos) {
            break;
        }
    }
    releaseKey(move.dir);
}

void InputHandler::singleMove(KEY_PRESS key) {
    pressKey(key);
    Sleep(100);
    releaseKey(key);
}

void InputHandler::addMove(Movement* move, int ind, KEY_PRESS dir, int dist) {
    Movement m;
    m.dir = dir;
    m.dist = dist;
    move[ind] = m;
}


void InputHandler::pressKey(WORD w) {
    sendInput(w, KEYEVENTF_SCANCODE);
}

void InputHandler::releaseKey(WORD w) {
    sendInput(w, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);
}

void InputHandler::sendInput(WORD w, DWORD flags) {
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = 0;
    ip.ki.wScan = w;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.dwFlags = flags;
    SendInput(1, &ip, sizeof(INPUT));
}
