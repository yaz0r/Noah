#include "noahLib.h"
#include "battleFader.h"
#include "kernel/taskLists.h"
#include "kernel/gte.h"
#include "field/field.h"
#include "psx/libgpu.h"

struct sBattleFader : public sTaskHeaderPair {
    s32 m38;
    s32 m3C_duration;
    s8 m40;
    s8 m41_tpageABE;
    std::array<u8, 3> m42_targetColor;
    std::array<u8, 3> m45_startColor;
    std::array<u8, 3> m48_currentColor;
};

bool disableBattleFader = false;
s8 battleFaderMode = 0;
sBattleFader* pBattleFader1 = nullptr;
sBattleFader* pBattleFader2 = nullptr;
sBattleFader battleFader1;
sBattleFader battleFader2;

void battleFader_update(sTaskHeader* pSprite) {
    sBattleFader* pThis = (sBattleFader*)pSprite;
    if (pThis->m3C_duration == 0) {
        pThis->m48_currentColor[0] = pThis->m42_targetColor[0];
        pThis->m48_currentColor[1] = pThis->m42_targetColor[1];
        pThis->m48_currentColor[2] = pThis->m42_targetColor[2];

        if ((pThis->m48_currentColor[0] == 0) && (pThis->m48_currentColor[1] == 0) && (pThis->m48_currentColor[2] == 0)) {
            pThis->mC_deleteCallback(pSprite);
        }
    }
    else {
        pThis->m3C_duration--;
        VECTOR colorDifference;
        colorDifference.vx = pThis->m42_targetColor[0] - pThis->m45_startColor[0];
        colorDifference.vy = pThis->m42_targetColor[1] - pThis->m45_startColor[1];
        colorDifference.vz = pThis->m42_targetColor[2] - pThis->m45_startColor[2];
        gte_ldIR0(pThis->m3C_duration / 2);
        gte_ldlvl(&colorDifference);
        gte_gpf0();
        gte_stlvl(&colorDifference);
        colorDifference /= (pThis->m38 / 2);
        pThis->m48_currentColor[0] = pThis->m42_targetColor[0] - colorDifference.vx;
        pThis->m48_currentColor[1] = pThis->m42_targetColor[1] - colorDifference.vy;
        pThis->m48_currentColor[2] = pThis->m42_targetColor[2] - colorDifference.vz;
    }
}

void battleFader_draw(sTaskHeader* pSprite) {
    sBattleFader* pThis = (sBattleFader*)(pSprite->m4);
    if (shapeTransfertTableCurrentEntry + sizeof(DR_MODE) + sizeof(POLY_F4) < shapeTransfertTableEnd) {
        POLY_F4* p = (POLY_F4*)shapeTransfertTableCurrentEntry;
        DR_MODE* drMode = (DR_MODE*)(shapeTransfertTableCurrentEntry + sizeof(POLY_F4));
        shapeTransfertTableCurrentEntry += sizeof(DR_MODE) + sizeof(POLY_F4);

        SetPolyF4(p);
        SetSemiTrans(p, 1);
        p->m_colorAndCode.m0_r = pThis->m48_currentColor[0];
        p->m_colorAndCode.m1_g = pThis->m48_currentColor[1];
        p->m_colorAndCode.m2_b = pThis->m48_currentColor[2];

        p->x0y0.vx = -32;
        p->x0y0.vy = -32;

        p->x1y1.vx = 320;
        p->x1y1.vy = -32;

        p->x2y2.vx = -32;
        p->x2y2.vy = 240;

        p->x3y3.vx = 320;
        p->x3y3.vy = 240;

        SetDrawMode(drMode, 0, 0, GetTPage(0, pThis->m41_tpageABE, 0, 0), nullptr);
        AddPrim(&(*characterRenderingOT)[2], p);
        AddPrim(&(*characterRenderingOT)[2], drMode);
    }
}

void battleFader_delete(sTaskHeader* pSprite) {
    sBattleFader* pThis = (sBattleFader*)pSprite;
    registerSpriteCallback2Sub0(&pThis->m1C);
    allocateSavePointMeshDataSub0_callback(pThis);
    pBattleFader1 = nullptr;
}

void battleCreateFader(int param_1, s8 tpageABE, s8 targetR, s8 targetG, s8 targetB) {
    if (disableBattleFader)
        return;

    sBattleFader* pSprite;
    if (!battleFaderMode) {
        pSprite = pBattleFader1;
        if (pBattleFader1)
        {
            goto LAB_Battle__800b3b20;
        }
        pSprite = &battleFader1;
        pBattleFader1 = &battleFader1;
    }
    else {
        pSprite = pBattleFader2;
        if (pBattleFader1) {
            pSprite->m45_startColor = pSprite->m48_currentColor;
            goto LAB_Battle__800b3b20;
        }
        pSprite = &battleFader2;
        pBattleFader2 = &battleFader2;
    }
    allocateSavePointMeshDataSub0(nullptr, pSprite);
    registerSpriteCallback2(pSprite, &pSprite->m1C);
    pSprite->m14 &= 0x7fffffff;
    if (spriteBytecode2ExtendedE0_Var0) {
        spriteBytecode2ExtendedE0_Var0--;
    }
    setTaskUpdateFunction(pSprite, battleFader_update);
    setTaskDrawFunction(&pSprite->m1C, battleFader_draw);
    setTaskDeleteFunction(pSprite, battleFader_delete);

    pSprite->m4 = pSprite;
    pSprite->m1C.m4 = pSprite;
    pSprite->m40 = 0;
    pSprite->m45_startColor[0] = 0;
    pSprite->m45_startColor[1] = 0;
    pSprite->m45_startColor[2] = 0;
LAB_Battle__800b3b20:
    pSprite->m45_startColor = pSprite->m48_currentColor;
    pSprite->m41_tpageABE = tpageABE;
    pSprite->m42_targetColor[0] = targetR;
    pSprite->m42_targetColor[1] = targetG;
    pSprite->m42_targetColor[2] = targetB;
    pSprite->m38 = param_1 * 2;
    pSprite->m3C_duration = param_1 * 2;
    battleFader_update(pSprite);
}