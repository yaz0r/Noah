#include "noahLib.h"
#include "battle.h"

#include "damageDisplay.h"
#include "damageDisplay2.h"

u16 damageDisplayVar0 = 0;
u16 damageDisplayVar1 = 0;

struct sDamageDisplayTask {
    sTaskHeader m0;
    sTaskHeader m1C;

    sDamageDisplayTask* m38_pNext;
    SVECTOR m40;
    VECTOR m48;
    std::array<s32, 3> m58;
    sSpriteActorCore* m78;
    u8 m7C_direction;
    sColorAndCode m80_colorAndCode;
    s32 m88_timer;
    s32 m8C;
    // size 0x1B0
};

sDamageDisplayTask* damageTaskDisplayListHead = nullptr;

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
        pTask->m48.vx -= 0xc0000;
    }
    else {
        pTask->m48.vx += 0xc0000;
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

void createDamageDisplay(sSpriteActorCore* param_1, int damageValue, int damageType) {
    if (jumpAnimationControlStruct == nullptr) {
        return;
    }

    createDamageDisplayPolysTask2();

    MissingCode();

    {
        sDamageDisplayTask* pCurrentDamageTask = damageTaskDisplayListHead;
        while (pCurrentDamageTask) {
            assert(0);
        }
    }

    sDamageDisplayTask* pNewDamageTask = createCustomRenderableEntity<sDamageDisplayTask>(0x1b0, nullptr, updateDamageDisplayTask, drawDamageDisplayTask, deleteDamageDisplayTask);
    pNewDamageTask->m78 = param_1;
    pNewDamageTask->m88_timer = 8;
    pNewDamageTask->m38_pNext = damageTaskDisplayListHead;
    damageTaskDisplayListHead = pNewDamageTask;
    pNewDamageTask->m48.vx = param_1->m0_position.vx;
    pNewDamageTask->m48.vy = param_1->m0_position.vy;
    pNewDamageTask->m48.vz = param_1->m0_position.vz;

    pNewDamageTask->m58[0] = 0x2000;
    pNewDamageTask->m58[1] = 0x2000;
    pNewDamageTask->m58[2] = 0x2000;
    pNewDamageTask->m40[0] = 0;
    pNewDamageTask->m40[1] = 0;
    pNewDamageTask->m40[2] = 0;
    (pNewDamageTask->m80_colorAndCode).m0_r = 0x80;
    (pNewDamageTask->m80_colorAndCode).m1_g = 0x80;
    (pNewDamageTask->m80_colorAndCode).m2_b = 0x80;
    (pNewDamageTask->m80_colorAndCode).m3_code = 0x2d;
    pNewDamageTask->m8C = 0;
    pNewDamageTask->m7C_direction = (param_1->mAC >> 2) & 1;
    MissingCode();
}

