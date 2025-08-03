#pragma once

#include "psx/libgpu.h"

struct sBattleRenderStruct {
    DRAWENV m0_drawEnv;
    DISPENV m5C_dispEnv;
    std::vector<sTag> m70_OT;
};

extern std::array<sBattleRenderStruct, 2> battleRenderStructs;

extern sBattleRenderStruct* pCurrentBattleRenderStruct;
extern std::vector<sTag>* pCurrentBattleOT;
extern s32 battleOddOrEven;

void initBattleRenderStructs(sBattleRenderStruct* param_1);
