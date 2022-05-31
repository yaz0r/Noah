#pragma once

struct sWorldmapStateEntry {
    s16 m0_state;
    s16 m2;
    s16 m4;
    s32(*m18_init)(s32 index);
    s32(*m1C_update)(s32 index);
    s16 m20;
    s16 m22;
    struct sSpriteActor* m4C;
    s32 m50;
};

struct sWorldmapState {
    std::array<sWorldmapStateEntry, 64> m0;
};

extern sWorldmapState* gWorldmapState;

#include "psx/gpuprims.h"

struct sWorldmapRenderingStruct {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    std::vector<sTag> m70_OT;
    std::vector<POLY_FT3> m74_polyFT3;
};

extern sWorldmapRenderingStruct* pCurrentWorldmapRenderingStruct;


extern s8 isWorldMapOverlayLoaded;
extern s32 worldmapOddOrEven;

void worldmapEntryPoint(void);