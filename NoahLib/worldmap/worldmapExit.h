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

extern std::array<std::vector<sWorldmapExitDef>,4> worldmapExitsDefs;
extern sWorldmapExitDef* worldmapCurrentExit;
extern s16 worldmapExitVar0;
extern s16 worldmapExitVar1;

extern sWorldmapExitDef worldmapExitToField312;
extern sWorldmapExitDef worldmapExitToField440;
extern sWorldmapExitDef worldmapExitToField290_yggdrasilDeck;

extern std::vector<u8>::const_iterator worldmapFile1Buffer_1C;


int setupWorldmapExits(VECTOR* param_1, int param_2);

s32 worldmap_taskExit_init(s32);
s32 worldmap_taskExit_update(s32);