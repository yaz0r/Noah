#include "noahLib.h"
#include "battle.h"

#include "damageDisplay2.h"

struct sDamageDisplayPoly {
    sTaskHeader m0;
    sTaskHeader m1C;
} damageDisplayPolyTask2;
sDamageDisplayPoly* pDamageDisplayPolysTask2 = nullptr;

u8 damageDisplayPolysTask2Disabled = 0;

void updateDamageDisplayPolysTask2(sTaskHeader* param_1) {
    // nothing
}

void drawDamageDisplayPolysTask2(sTaskHeader* param_1) {
    MissingCode();
}

void deleteDamageDisplayPolysTask2(sTaskHeader* param_1) {
    MissingCode();
}

void createDamageDisplayPolysTask2() {
    if ((pDamageDisplayPolysTask2 == nullptr) && !damageDisplayPolysTask2Disabled) {
        pDamageDisplayPolysTask2 = &damageDisplayPolyTask2;
        allocateSavePointMeshDataSub0(nullptr, &damageDisplayPolyTask2.m0);
        registerSpriteCallback2(&damageDisplayPolyTask2.m0, &damageDisplayPolyTask2.m1C);
        setTaskUpdateFunction(&damageDisplayPolyTask2.m0, updateDamageDisplayPolysTask2);
        setTaskDrawFunction(&damageDisplayPolyTask2.m1C, drawDamageDisplayPolysTask2);
        setTaskDeleteFunction(&damageDisplayPolyTask2.m0, deleteDamageDisplayPolysTask2);
        MissingCode();
    }
}

s32 initBattleUnkData0Var1 = 0;

void initBattleUnkData0() {
    pDamageDisplayPolysTask2 = 0;
    initBattleUnkData0Var1 = 0;
}

bool isDamageDisplayPolysTask2Running() {
    return pDamageDisplayPolysTask2 != 0;
}
