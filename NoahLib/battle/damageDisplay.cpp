#include "noahLib.h"
#include "battle.h"

#include "damageDisplay.h"
#include "damageDisplay2.h"

u16 damageDisplayVar0 = 0;
u16 damageDisplayVar1 = 0;

struct sDamageDisplayTask {
    sTaskHeader m0;
    sTaskHeader m1C;

    s32 m48;
    u8 m7C_direction;
    sColorAndCode m80_colorAndCode;
    s32 m88_timer;
    // size 0x1B0
};

void updateDamageDisplayPolys() {
    MissingCode();
}

void updateDamageDisplayTask_FadeOut(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    pTask->m80_colorAndCode.m0_r = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);
    pTask->m80_colorAndCode.m1_g = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);
    pTask->m80_colorAndCode.m2_b = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);

    if ((--pTask->m88_timer < 0) || ((pTask->m80_colorAndCode.m0_r | pTask->m80_colorAndCode.m1_g | pTask->m80_colorAndCode.m2_b) == 0)) {
        param_1->mC_deleteCallback(param_1);
    }
}

void updateDamageDisplayTask2(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    // start fadeout
    if (--pTask->m88_timer < 0) {
        pTask->m88_timer = 0x10;
        pTask->m80_colorAndCode.m3_code = (pTask->m80_colorAndCode.m3_code & 0xfe) | 2;
        setTaskUpdateFunction(param_1, updateDamageDisplayTask_FadeOut);
    }
}

void updateDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    if (pTask->m7C_direction == '\0') {
        pTask->m48 -= 0xc0000;
    }
    else {
        pTask->m48 += 0xc0000;
    }

    if (--pTask->m88_timer < 0) {
        pTask->m88_timer = 0x10;
        setTaskUpdateFunction(param_1, updateDamageDisplayTask2);
    }
}

void drawDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;
    MissingCode();
}

void deleteDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;
    MissingCode();
}

void createDamageDisplay(sSpriteActorCore* param_1, int param_2, int param_3) {
    if (jumpAnimationControlStruct == nullptr) {
        return;
    }

    createDamageDisplayPolysTask2();

    MissingCode();

    createCustomRenderableEntity<sDamageDisplayTask>(0x1b0, nullptr, updateDamageDisplayTask, drawDamageDisplayTask, deleteDamageDisplayTask);

    MissingCode();
}

