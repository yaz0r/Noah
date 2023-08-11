#pragma once

#include "damageDisplay.h"

struct sDamageDisplayPoly : public sTaskHeaderPair {
    SVECTOR m38_transformedPos;
    VECTOR m40_pos;

    std::array<sDamageDisplaySub0, 2> m50;
};

void createDamageDisplayPolysTask2();
bool isDamageDisplayPolysTask2Running();
void initBattleUnkData0();
void updateDamageDisplayPolys();
void freeDamageDisplayPolysTask2();

extern sDamageDisplayPoly* pDamageDisplayPolysTask2;
extern u8 damageDisplayPolysTask2Disabled;
extern int createDamageDisplayPolysTask2Var0;