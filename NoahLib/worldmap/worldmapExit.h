#pragma once

struct sWorldmapExitDef {
    s16 m0_X;
    s16 m2_Y;
    s16 m4_width;
    s16 m6_height;
    s16 m8_destinationField;
    s16 mA;
    s16 mC_newWorldmapMode;
    s16 mE_type;
    // size 0x10
};

extern std::vector<sWorldmapExitDef> worldmapExitsDefs;
extern sWorldmapExitDef* worldmapCurrentExit;
extern s16 worldmapExitVar0;
extern s16 worldmapExitVar1;

extern sWorldmapExitDef worldmapExitToField312;
extern sWorldmapExitDef worldmapExitToField440;
extern sWorldmapExitDef worldmapExitToField290_yggdrasilDeck;

int setupWorldmapExits(VECTOR* param_1, int param_2);