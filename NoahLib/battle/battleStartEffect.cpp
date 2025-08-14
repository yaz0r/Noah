#include "noahLib.h"
#include "battleStartEffect.h"
#include "field/fieldGraphicObject.h"
#include "battleRenderContext.h"
#include "kernel/graphics.h"
#include "kernel/trigo.h"
#include "kernel/gte.h"
#include "kernel/TIM.h"
#include "kernel/filesystem.h"
#include "battleLoader.h"
#include "battleConfig.h"
#include "field/field.h"
#include "kernel/memory.h"
#include "battle.h"

void checkSoftReboot();

struct sBattleStartEffectSub {
    SVECTOR m0_rotation;
    SVECTOR m8_rotationStep;
    VECTOR m10_position;
    std::array<POLY_FT3, 2> m24_poly;
    s32 m64;
    VECTOR m68_velocity;
    s32 m78_gravity;
};

struct sBattleStartEffect : public sTaskHeaderPair {
    s32 m38_frameCounter;
    sBattleStartEffectSub m3C_triangleData[2][14][20];
};

VECTOR effectGravity = { 0,0,-0x6000000 };

std::vector<sTag>* pBattleStartEffectOT;

sBattleStartEffect* initBattleStartEffect(sBattleStartEffect* param_1) {
    param_1->m38_frameCounter = 0;

    s32 lVar3 = SquareRoot0(0x9500);
    s32 local_58 = lVar3 << 10;

    for (int i = 0; i < 2; i++) {
        int local_38 = -0xCA0;
        int local_34 = -0xD60;
        for (int j = 0; j < 14; j++) {
            for (int k = 0; k < 20; k++) {
                s32 local_4c = j << 4;
                sBattleStartEffectSub* pSub = &param_1->m3C_triangleData[i][j][k];
                (pSub->m0_rotation).vx = 0;
                (pSub->m0_rotation).vy = 0;
                (pSub->m0_rotation).vz = 0;
                if (i == 0) {
                    (pSub->m10_position).vx = (k * 0x10 + -0x9b) * 0x20;
                    (pSub->m10_position).vz = 0x4000;
                    (pSub->m10_position).vy = local_34;
                }
                else {
                    (pSub->m10_position).vx = (k * 0x10 + -0x95) * 0x20;
                    (pSub->m10_position).vz = 0x4000;
                    (pSub->m10_position).vy = local_38;
                }

                effectGravity.vz = -0x1f40000;
                s32 randValue = xenoRand();
                effectGravity.vz = effectGravity.vz + ((randValue / 1000) * 1000 - randValue) * 0x10000;

                VECTOR local_90;
                Square0(&pSub->m10_position, &local_90);
                lVar3 = SquareRoot0(local_90.vx + local_90.vy);

                {
                    s32 iVar4 = local_58;
                    if (local_58 < 0) {
                        iVar4 = local_58 + 0x1f;
                    }
                    iVar4 = (iVar4 >> 5) - lVar3;
                    if (iVar4 < 0) {
                        iVar4 = iVar4 + 0x7ff;
                    }
                    pSub->m64 = iVar4 >> 0xb;

                    s32 lVar5 = lVar3;
                    if (lVar3 < 0) {
                        lVar5 = lVar3 + 0x3ff;
                    }
                    pSub->m64 = lVar5 >> 10;
                }

                {
                    s32 y = (pSub->m10_position).vy;
                    s32 x = (pSub->m10_position).vx;
                    s32 ratan2Value = ratan2(y, x);
                    s32 iVar5 = xenoRand();
                    s32 iVar4 = iVar5;
                    if (iVar5 < 0) {
                        iVar4 = iVar5 + 0x3ff;
                    }
                    s32 iVar9 = (lVar3 << 0xb) / local_58;
                    s32 iVar6 = xenoRand();
                    s32 l = iVar6;
                    if (iVar6 < 0) {
                        l = iVar6 + 0x3f;
                    }
                    SVECTOR local_80;
                    local_80.vy = (short)iVar9 + -0x20 + (short)iVar6 + (short)(l >> 6) * -0x40;
                    local_80.vx = 0;
                    local_80.vz = (short)ratan2Value + 0x600 + (short)iVar5 + (short)(iVar4 >> 10) * -0x400;
                    MATRIX MStack120;
                    RotMatrixZYX(&local_80, &MStack120);
                    ApplyMatrixLV(&MStack120, &effectGravity, &pSub->m68_velocity);

                    iVar4 = xenoRand();
                    s8 bVar1 = (byte)(k*0x10) & 0x3f;
                    s32 uVar8 = bVar1 + 0x10;
                    iVar5 = 0x24;
                    s32 uVar7 = (char)local_4c + '\x10';
                    pSub->m78_gravity = (iVar4 % 0x640) * -0x100 + 0x70800;
                    s32 local_40 = k * 0x10;
                    iVar4 = xenoRand();
                    (pSub->m8_rotationStep).vx = ((ushort)iVar4 & 0xff) - 0x7f;
                    iVar4 = xenoRand();
                    (pSub->m8_rotationStep).vy = ((ushort)iVar4 & 0xff) - 0x7f;
                    iVar4 = xenoRand();
                    (pSub->m8_rotationStep).vz = ((ushort)iVar4 & 0x1ff) - 0xff;

                    for (l = 0; l < 2; l++) {
                        POLY_FT3* p = &pSub->m24_poly[l];
                        SetPolyFT3(p);
                        SetShadeTex(p, 0);
                        p->r0 = 0xff;
                        p->b0 = 0xff;
                        p->g0 = 0xff;
                        p->code &= ~3;
                        p->tpage = GetTPage(2, 1, local_40 + 0x2c0, 0x100);

                        if (i == 0) {
                            p->u0 = bVar1;
                            p->u1 = uVar8;
                            p->v0 = (u8)local_4c;
                            p->u2 = bVar1;
                            p->v2 = uVar7;
                            p->v1 = (u8)local_4c;
                        }
                        else {
                            p->u0 = uVar8;
                            p->u1 = uVar8;
                            p->v1 = uVar7;
                            p->u2 = bVar1;
                            p->v2 = uVar7;
                            p->v0 = (u8)local_4c;
                        }
                    }
                }
            }
            local_38 += 0x200;
            local_34 += 0x200;
        }
    }

    return param_1;
}

sBattleStartEffect* createBattleStartEffect() {
    sBattleStartEffect* psVar1 = new sBattleStartEffect;
    psVar1->m4 = psVar1;
    psVar1->m1C.m4 = psVar1;
    return initBattleStartEffect(psVar1);
}



void renderBattleStartEffect(sBattleStartEffect* param_1) {
    sBattleStartEffect* pEffect = (sBattleStartEffect*)param_1->m4;

    s32 oldScreenOffsetX;
    s32 oldScreenOffsetY;
    ReadGeomOffset(&oldScreenOffsetX, &oldScreenOffsetY);
    s32 oldScreenGeom = ReadGeomScreen();

    SetGeomOffset(0xa0, 0x70);
    SetGeomScreen(0x200);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 14; j++) {
            for (int k = 0; k < 20; k++) {
                sBattleStartEffectSub* pSub = &pEffect->m3C_triangleData[i][j][k];
                POLY_FT3* p = &pSub->m24_poly[battleOddOrEven];
                if (pSub->m10_position.vz > 0x3F) {
                    MATRIX localMatrix;
                    createRotationMatrix(&pSub->m0_rotation, &localMatrix);
                    TransMatrix(&localMatrix, &pSub->m10_position);
                    SetRotMatrix(&localMatrix);
                    SetTransMatrix(&localMatrix);

                    static const std::array<SVECTOR, 3> SVECTOR_ARRAY_Battle__800c3594 = { { { -0xA0, -0xA0, 0 }, {0x160, -0xA0, 0}, {-0xA0, 0x160, 0} } };
                    static const std::array<SVECTOR, 3> SVECTOR_ARRAY_Battle__800c35ac = { { { 0xA0, -0x160, 0 }, {0xA0, 0xA0, 0}, {-0x160, 0xA0, 0} } };

                    const std::array<SVECTOR, 3>* pVector;
                    if (i == 0) {
                        pVector = &SVECTOR_ARRAY_Battle__800c3594;
                    }
                    else {
                        pVector = &SVECTOR_ARRAY_Battle__800c35ac;
                    }

                    s32 lStack64;
                    s32 lStack60;
                    s32 depth = RotTransPers3(&pVector->at(0), &pVector->at(1), &pVector->at(2), &p->x0y0, &p->x1y1, &p->x2y2, &lStack64, &lStack60);
                    AddPrim(&(*pBattleStartEffectOT)[depth >> 6], p);
                }
            }
        }
    }

    SetGeomOffset(oldScreenOffsetX, oldScreenOffsetY);
    SetGeomScreen(oldScreenGeom);
}

void updateBattleStartEffect(sBattleStartEffect* param_1) {
    sBattleStartEffect* pEffect = (sBattleStartEffect*)param_1->m4;
    pEffect->m38_frameCounter++;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 14; j++) {
            for (int k = 0; k < 20; k++) {
                sBattleStartEffectSub* pSub = &pEffect->m3C_triangleData[i][j][k];
                if (pSub->m64 == 0) {
                    pSub->m24_poly[battleOddOrEven].r0 = addAndClamp(pSub->m24_poly[battleOddOrEven].r0, -6);
                    pSub->m24_poly[battleOddOrEven].g0 = addAndClamp(pSub->m24_poly[battleOddOrEven].g0, -6);
                    pSub->m24_poly[battleOddOrEven].b0 = addAndClamp(pSub->m24_poly[battleOddOrEven].b0, -6);

                    (pSub->m10_position).vx += pSub->m68_velocity.vx.getIntegerPart();
                    (pSub->m10_position).vy += pSub->m68_velocity.vy.getIntegerPart();
                    (pSub->m10_position).vz += pSub->m68_velocity.vz.getIntegerPart();

                    pSub->m0_rotation.vx += pSub->m8_rotationStep.vx;
                    pSub->m0_rotation.vy += pSub->m8_rotationStep.vy;
                    pSub->m0_rotation.vz += pSub->m8_rotationStep.vz;

                    s32 iVar7 = (pSub->m68_velocity).vy;
                    if (pSub->m68_velocity.vy < 0) {
                        iVar7 = pSub->m68_velocity.vy + 0xf;
                    }
                    (pSub->m68_velocity).vy -= (iVar7 >> 4);

                    s32 iVar8 = (pSub->m68_velocity).vx;
                    iVar7 = iVar8;
                    if (iVar8 < 0) {
                        iVar7 = iVar8 + 0xf;
                    }
                    (pSub->m68_velocity).vx = iVar8 - (iVar7 >> 4);

                    s32 iVar6 = (pSub->m68_velocity).vz;
                    iVar7 = iVar6;
                    if (iVar6 < 0) {
                        iVar7 = iVar6 + 0xf;
                    }
                    (pSub->m68_velocity).vz = iVar6 - (iVar7 >> 4);

                    (pSub->m68_velocity).vy = (pSub->m68_velocity).vy + pSub->m78_gravity;
                }
                else {
                    pSub->m64--;
                }
            }
        }
    }
}

std::array<sLoadingBatchCommands, 4> battleEnvLoadingCommands;

int loadBattleMechaAndEnvironment(int battleArenaId, int param_2, sLoadableDataRaw*& param_3, std::vector<u8>::iterator& param_4, sBattleMechaInitData*& param_5) {
    int local_30;
    int local_2c;

    int result = 0;
    getCurrentDirectory(&local_30, &local_2c);
    setCurrentDirectory(0xc, 3);
    resetMemoryAllocStats(4, 0);
    int iVar1 = getNegativeFileSize(5);
    int iVar5 = battleArenaId * 2;
    if (battleArenaId < (((iVar1 << 0x10) >> 0x10) - ((iVar1 << 0x10) >> 0x1f)) >> 1) {
        sBattleMechaInitDataFile* pvVar3 = new sBattleMechaInitDataFile(getFileSizeAligned(iVar5 + param_2 + 7));
        flagAllocation(*pvVar3);
        sLoadableDataRaw* pvVar4 = new sLoadableDataRaw(getFileSizeAligned(iVar5 + 6));
        flagAllocation(*pvVar4);
        battleEnvLoadingCommands[1].m0_fileIndex = (short)iVar5 + 7 + (short)param_2;
        battleEnvLoadingCommands[0].m0_fileIndex = (ushort)(iVar5 + 6);
        battleEnvLoadingCommands[2].m0_fileIndex = 0;
        battleEnvLoadingCommands[2].m4_loadPtr = nullptr;
        battleEnvLoadingCommands[0].m4_loadPtr = pvVar4;
        battleEnvLoadingCommands[1].m4_loadPtr = pvVar3;
        batchStartLoadingFiles(&battleEnvLoadingCommands[0], 0);
        Hack("This should be done after waiting for load to complete");
        param_3 = pvVar4;
        param_4 = std::vector<u8>::iterator();
        param_5 = &pvVar3->m4;
        battleMechaInitData = &pvVar3->m4;
    }
    else {
        result = -1;
        param_3 = nullptr;
        param_4 = std::vector<u8>::iterator();
        param_5 = nullptr;
    }
    setCurrentDirectory(local_30, local_2c);
    return result;
}

void startBattleLoadingDuringEffect2() {
    loadBattleMechaAndEnvironment(currentBattleConfig.m2_arenaId, 0, battleLoadDataVar0_raw, battleLoadDataVar1, battleLoadDataVar2);
}

void battleStartEffect() {
    resetSpriteCallbacks();
    
    {
        std::array<u16, 0x30000> buffer;
        Noah_MissingCode("make the back buffer transparent");
        buffer.fill(0x8757);

        RECT local_28;
        local_28.x = 0x2c0;
        local_28.y = 0x100;
        local_28.w = 0x140;
        local_28.h = 0xe0;
        LoadImage(&local_28, (u8*)&buffer[0]);
        DrawSync(0);
    }

    if (pCurrentBattleRenderStruct == &battleRenderStructs[0]) {
        pCurrentBattleRenderStruct = &battleRenderStructs[1];
        pCurrentBattleOT = &battleRenderStructs[1].m70_OT;
    }
    else {
        pCurrentBattleRenderStruct = &battleRenderStructs[0];
        pCurrentBattleOT = &battleRenderStructs[0].m70_OT;
    }

    ClearOTagR(&(*pCurrentBattleOT)[0], 0x1000);

    battleOddOrEven = 0;
    pCurrentBattleRenderStruct = &battleRenderStructs[0];

    battleRenderStructs[0].m0_drawEnv.isbg = 1;
    battleRenderStructs[1].m0_drawEnv.isbg = 1;
    battleRenderStructs[0].m0_drawEnv.r0 = 0xff;
    battleRenderStructs[1].m0_drawEnv.r0 = 0xff;
    battleRenderStructs[0].m0_drawEnv.g0 = 0xff;
    battleRenderStructs[1].m0_drawEnv.g0 = 0xff;
    battleRenderStructs[0].m0_drawEnv.b0 = 0xff;
    battleRenderStructs[1].m0_drawEnv.b0 = 0xff;

    sBattleStartEffect* pBattleStartEffect = createBattleStartEffect();

    s32 effectFrameCounter = 0x56;
    s32 s2_tickState = 1;
    s32 s5 = 0;
    while (effectFrameCounter || s2_tickState != 5) {
        if (0 < effectFrameCounter) {
            effectFrameCounter = effectFrameCounter + -1;
        }

        if (pCurrentBattleRenderStruct == &battleRenderStructs[0]) {
            pCurrentBattleRenderStruct = &battleRenderStructs[1];
        }
        else {
            pCurrentBattleRenderStruct = &battleRenderStructs[0];
        }

        pCurrentBattleOT = &pCurrentBattleRenderStruct->m70_OT;

        ClearOTagR(&(*pCurrentBattleOT)[0], 0x1000);

        battleOddOrEven = 1 - battleOddOrEven;
        pBattleStartEffectOT = pCurrentBattleOT;

        MissingCode();

        if (isCDBusy() == 0) {
            switch (s2_tickState) {
            case 1:
            case 3:
            case 4:
                battleLoaderTick(s5);
                s5++;
                s2_tickState++;
                break;
            case 2:
                startBattleLoadingDuringEffect2();
                //assert(0);
                s2_tickState++;
                break;
            }
        }

        checkSoftReboot();
        //sCurrentCameraVectors temp;
        //pCurrentCameraVectors = &sCurrentCameraVectors;
        MissingCode();

        updateBattleStartEffect(pBattleStartEffect);
        renderBattleStartEffect(pBattleStartEffect);

        MissingCode();

        DrawSync(0);
        VSync(2);

        battleRenderStructs[battleOddOrEven].m0_drawEnv.r0 = addAndClamp(battleRenderStructs[battleOddOrEven].m0_drawEnv.r0, 0xC);
        battleRenderStructs[battleOddOrEven].m0_drawEnv.g0 = addAndClamp(battleRenderStructs[battleOddOrEven].m0_drawEnv.g0, 0xC);
        battleRenderStructs[battleOddOrEven].m0_drawEnv.b0 = addAndClamp(battleRenderStructs[battleOddOrEven].m0_drawEnv.b0, 0xC);

        PutDispEnv(&pCurrentBattleRenderStruct->m5C_dispEnv);
        PutDrawEnv(&pCurrentBattleRenderStruct->m0_drawEnv);
        DrawOTag(&pCurrentBattleRenderStruct->m70_OT[0xfff]);
    }

    MissingCode();
/*    freeBattleStartEffect(pBattleStartEffect);
    SetDispMask(0);
    waitReadCompletion(0);*/
    battleLoaderTick(3);
    return;
}