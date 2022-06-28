#include "noahLib.h"
#include "battleRenderContext.h"

std::array<sBattleRenderStruct, 2> battleRenderStructs;
sBattleRenderStruct* pCurrentBattleRenderStruct = nullptr;
std::vector<sTag>* pCurrentBattleOT = nullptr;
s32 battleOddOrEven = 0;

void initBattleRenderStructs(sBattleRenderStruct* param_1) {
    param_1->m70_OT.resize(4096);
    (param_1->m0_drawEnv).isbg = '\x01';
    (param_1->m0_drawEnv).dtd = 1;
    (param_1->m0_drawEnv).r0 = 0x3c;
    (param_1->m0_drawEnv).g0 = 0x78;
    (param_1->m0_drawEnv).b0 = 0x78;
}