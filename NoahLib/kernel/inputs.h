#pragma once

enum controllerButtons {
    L1 = 1 << 2, // 0x4
    R1 = 1 << 3, // 0x8
    TRIANGLE = 1 << 4, // 0x10
    INTERACT = 1 << 5, // 0x20
    CROSS = 1 << 6, // 0x40
    JUMP = 1 << 7, // 0x80
    SELECT = 1 << 8, // 0x100
    START = 1 << 11, // 0x800
    UP = 1 << 12, // 0x1000
    RIGHT = 1 << 13, // 0x2000
    DOWN = 1 << 14, // 0x4000
    LEFT = 1 << 15, // 0x8000
};

struct sGameController {
    u16 m0_buttons;
    u16 m2;
};

extern std::array<sGameController, 2> newPadButtonForScripts;
extern u16 newPadButtonForDialogs;
extern u16 newPadButtonForDialogs2;
extern u16 newPadButtonForField;
extern u16 newPadButtonForField2;

bool isControllerConnected(int port);
void getInputDuringVsync();
void saveInputs();
int loadInputFromVSyncBuffer();
void resetInputs();
int getInputOverflowed();

