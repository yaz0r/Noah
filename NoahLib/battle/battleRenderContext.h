#pragma once

#include "psx/gpuprims.h"

struct sBattleRenderStruct {
    DRAWENV m0_drawEnv;
    DISPENV m5C_dispEnv;
    std::vector<sTag> m70_OT;
};

extern std::array<sBattleRenderStruct, 2> battleRenderStructs;

void initBattleRenderStructs(sBattleRenderStruct* param_1);