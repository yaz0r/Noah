#pragma once

struct sBattleDialogWindow {
    std::array<std::array<POLY_FT4, 2>, 4> m0_corners;
    std::array<std::array<POLY_FT4, 2>, 2> m140;
    std::array<std::array<POLY_FT4, 2>, 2> m1E0;
    std::array<std::array<POLY_FT4, 2>, 2> m280;
    std::array<std::array<POLY_FT4, 2>, 2> m320;
    std::array<POLY_G4, 2> m3C0_polyG4;
    std::array<DR_MODE, 2> m408_drawMode;
    s32 m5A0;
    s8 m5A4_oddOrEven;
    // size 0x5a8
};

struct sBattleDialogWindow2 {
    s16 m0_x;
    s16 m2_y;
    s16 m4_width;
    s16 m6_height;
    s16 m8;
    s16 mA;
    s8 mC_windowIndex;
    // size 0xE
};

extern std::array<sBattleDialogWindow*, 7> battleDialogWindows;
extern std::array<sBattleDialogWindow2*, 7> battleDialogWindows2;

void allocateAndSetupBattleDialogWindow(byte param_1, short x, short y, short width, short height, char param_6, char param_7);
void drawBattleDialogWindows();
void clearBattleDialogWindow(uint param_1);