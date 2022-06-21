#include "noahLib.h"
#include "worldmapTaskYggdrasil.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"

ushort getWorldmapGroundType(VECTOR* param_1);

s32 worldmap_taskYggdrasil_init(int param_1)
{
    s32 returnState = 1;

    gWorldmapState->m0[param_1].m24 = 0;
    getGamestate182C(&gWorldmapState->m0[param_1].m28_position);

    gWorldmapState->m0[param_1].m38_step.vz = 0;
    gWorldmapState->m0[param_1].m38_step.vy = 0;
    gWorldmapState->m0[param_1].m38_step.vx = 0;
    gWorldmapState->m0[param_1].m64 = nullptr;
    gWorldmapState->m0[param_1].m60 = 0;
    gWorldmapState->m0[param_1].m6C.vz = 0;
    //gWorldmapState->m0[param_1].m68 = (short*)0xffd80000;
    gWorldmapState->m0[param_1].m6C.vy = 0;
    gWorldmapState->m0[param_1].m6C.vx = 0;
    gWorldmapState->m0[param_1].m48 = gameState.m182C.pad;

    switch (gameState.m1834 & 0x1fff) {
    case 0: // on foot, yggdrasil not active
        returnState = 3;
        break;
    case 3:
        gWorldmapState->m0[param_1].m4A = 0x20;
        break;
    default:
        assert(0);
    }

    switch (worldMapGearMode) {
    case 1:
        gWorldmapState->m0[param_1].m28_position.vy = worldmapGetAltitudeFor2dPoint(gWorldmapState->m0[param_1].m28_position.vx, gWorldmapState->m0[param_1].m28_position.vz);
        break;
    case 7:
        gWorldmapState->m0[param_1].m20 = 2;
        gWorldmapState->m0[param_1].m28_position.vx = worldmapPosition.vx;
        gWorldmapState->m0[param_1].m28_position.vz = worldmapPosition.vz;
        gWorldmapState->m0[param_1].m48 = worldmapGamestate1824;
        gWorldmapState->m0[param_1].m6C.vz = worldmapNumFilesPending;
        gWorldmapState->m0[param_1].m28_position.vy = worldmapPosition.vy;
        worldmapRadarPosition = gWorldmapState->m0[param_1].m28_position;
        worldmapVar_8009d52c = gWorldmapState->m0[param_1].m48;
        break;
    default:
        assert(0);
    }

    worldmapModels[0].m8 = gWorldmapState->m0[param_1].m28_position;
    worldmapModels[0].m0_hidden = gWorldmapState->m0[param_1].m24;

    SVECTOR SVECTOR_1f8000a0;
    SVECTOR_1f8000a0.vx = -(short)(gWorldmapState->m0[param_1].m6C.vy >> 0xc);
    SVECTOR_1f8000a0.vy = gWorldmapState->m0[param_1].m48;
    SVECTOR_1f8000a0.vz = worldmapRotation.vz;
    RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[0].m20_rotationMatrix);
    RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[1].m20_rotationMatrix);
    setGamestate182C(&gWorldmapState->m0[param_1].m28_position);
    gameState.m182C.pad = gWorldmapState->m0[param_1].m48;

    s32 sVar5;
    if (currentWorldmapMode == 3) {
        sVar5 = 0x28;
    }
    else {
        if (currentWorldmapMode < 4) {
            if (currentWorldmapMode != 2) {
                return returnState;
            }
            gWorldmapState->m0[param_1].m20 = 0x24;
            return returnState;
        }
        if (currentWorldmapMode == 4) {
            sVar5 = 0x30;
        }
        else {
            sVar5 = 0x34;
            if (currentWorldmapMode != 5) {
                return returnState;
            }
        }
    }
    gWorldmapState->m0[param_1].m20 = sVar5;
    gWorldmapState->m0[param_1].m7C = 0;
    return returnState;
}

int processYggdrasilInputs(sWorldmapStateEntry* param_1) {
    bool bVar1;
    short sVar2;
    int iVar3;
    long lVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;

    if (param_1->m64 == nullptr) {
        param_1->m64 = (const std::array<s16, 4>*)0x1;
        param_1->m5C = 0;
        param_1->m58 = (int)param_1->m48 << 0xc;
    }
    else {
        iVar7 = 0;
        if (param_1->m64 == (const std::array<s16, 4>*)0x1) {
            iVar5 = 0;
            iVar8 = 8;
            iVar9 = 0x1200;
            if ((worldmapInput1_0 & 0x8000) != 0) {
                iVar5 = -0x60;
                iVar7 = -0x10000;
            }
            if ((worldmapInput1_0 & 0x2000) != 0) {
                iVar5 = iVar5 + 0x60;
                iVar7 = iVar7 + 0x10000;
            }
            if ((worldmapInput1_0 & 4) != 0) {
                iVar5 = iVar5 + -0x60;
                iVar7 = iVar7 + -0x10000;
            }
            if ((worldmapInput1_0 & 8) != 0) {
                iVar5 = iVar5 + 0x60;
                iVar7 = iVar7 + 0x10000;
            }
            if ((worldmapInput1_0 & 0xa00c) != 0) {
                iVar9 = 0x1000;
                iVar8 = worldmapRotation.vz - iVar5;
                if (iVar8 < 0) {
                    iVar8 = -iVar8;
                }
                iVar8 = iVar8 >> 4;
            }
            iVar6 = (int)worldmapRotation.vz;
            if (iVar6 != iVar5) {
                iVar3 = iVar6 - iVar5;
                if (iVar3 < 0) {
                    iVar3 = -iVar3;
                }
                if (iVar3 < iVar8) {
                    iVar8 = iVar3;
                }
                sVar2 = (short)iVar8;
                if (iVar5 <= iVar6) {
                    sVar2 = -sVar2;
                }
                worldmapRotation.vz = worldmapRotation.vz + sVar2;
            }
            iVar5 = param_1->m5C;
            iVar8 = iVar5 - iVar7;
            if (iVar5 != iVar7) {
                if (iVar8 < 0) {
                    iVar8 = -iVar8;
                }
                if (iVar8 < iVar9) {
                    iVar9 = iVar8;
                }
                if (iVar7 <= iVar5) {
                    iVar9 = -iVar9;
                }
                param_1->m5C = iVar5 + iVar9;
            }
        }
    }
    iVar7 = param_1->m58 + param_1->m5C;
    param_1->m58 = iVar7;
    worldmapRotation.vy = (ushort)(iVar7 >> 0xc) & 0xfff;
    param_1->m48 = worldmapRotation.vy;
    iVar7 = (param_1->m6C).vx;
    if (iVar7 == 1) {
        iVar7 = (param_1->m6C).vy;
        if (iVar7 != 0) {
            if (iVar7 < 0) {
                iVar7 = iVar7 + 0x2000;
            }
            else {
                iVar7 = iVar7 + -0x2000;
            }
            (param_1->m6C).vy = iVar7;
        }
        if ((worldmapInput1_0 & 0x4000) != 0) {
            (param_1->m6C).vx = 2;
        }
        if ((worldmapInput1_0 & 0x1000) != 0) {
            (param_1->m6C).vx = 3;
        }
    }
    else if (iVar7 < 2) {
        if (iVar7 == 0) {
            (param_1->m6C).vx = 1;
            (param_1->m6C).vy = 0;
        }
    }
    else if (iVar7 == 2) {
        lVar4 = -0x80000;
        if ((worldmapInput1_0 & 0x4000) != 0) {
            iVar7 = (param_1->m6C).vy + -0x4000;
            (param_1->m6C).vy = iVar7;
            bVar1 = iVar7 < -0x80000;
            goto LAB_worldmap__8009124c;
        }
    LAB_worldmap__8009125c:
        (param_1->m6C).vx = 1;
    }
    else if (iVar7 == 3) {
        lVar4 = 0x80000;
        if ((worldmapInput1_0 & 0x1000) == 0) goto LAB_worldmap__8009125c;
        iVar7 = (param_1->m6C).vy + 0x4000;
        (param_1->m6C).vy = iVar7;
        bVar1 = 0x80000 < iVar7;
    LAB_worldmap__8009124c:
        if (bVar1) {
            (param_1->m6C).vy = lVar4;
        }
    }
    if ((worldmapInput1_0 & 0x80) != 0) {
        if ((worldmapInput1_0 & 2) == 0) {
            iVar7 = param_1->m60 + 0x1000;
        }
        else {
            iVar7 = param_1->m60 + -0x1000;
        }
        param_1->m60 = iVar7;
        if ((worldmapInput1_0 & 0x80) != 0) {
            iVar7 = param_1->m4A * -0x1000;
            if (param_1->m60 < iVar7) {
                param_1->m60 = iVar7;
            }
            iVar7 = (int)param_1->m4A << 0xc;
            if (iVar7 < param_1->m60) {
                param_1->m60 = iVar7;
            }
            goto LAB_worldmap__8009131c;
        }
    }
    iVar7 = param_1->m60;
    if (iVar7 != 0) {
        if (iVar7 < 0) {
            iVar7 = iVar7 + 0x1000;
        }
        else {
            iVar7 = iVar7 + -0x1000;
        }
        param_1->m60 = iVar7;
    }
LAB_worldmap__8009131c:
    lVar4 = getAngleCos((int)worldmapRotation.vy);
    iVar7 = (param_1->m6C).vy;
    (param_1->m38_step).vx = lVar4;
    lVar4 = getAngleCos(iVar7 >> 0xc);
    (param_1->m38_step).vy = lVar4;
    iVar7 = getAngleSin((int)worldmapRotation.vy);
    (param_1->m38_step).vz = -iVar7;
    VectorNormal(&param_1->m38_step, &param_1->m38_step);

    MissingCode();

    return 0;
}

void checkWorldmapPositionSub0(VECTOR* param_1);
int worldmapCheckCollisionsAgainstWorldmapModels(VECTOR* param_1, short* param_2);
int checkWorldmapPositionSub1(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, short param_5);
void adjustLocationAfterCollision(VECTOR* param_1, int param_2, int param_3, u8* param_4, u8* param_5);

extern u8 adjustLocationAfterCollisionVar0;
extern u8 adjustLocationAfterCollisionVar1;
extern u16 adjustLocationAfterCollisionVar2;


int checkWorldmapPositionYggdrasil(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, s16 param_5) {
    VECTOR VECTOR_1f800060;

    VECTOR_1f800060.vx = position->vx + (step->vx * stepScale >> 0xc);
    VECTOR_1f800060.vy = position->vy + (step->vy * stepScale >> 0xc);
    VECTOR_1f800060.vz = position->vz + (step->vz * stepScale >> 0xc);
    checkWorldmapPositionSub0(&VECTOR_1f800060);
    output->vx = position->vx + (step->vx * stepScale >> 0xc);
    output->vy = position->vy + (step->vy * stepScale >> 0xc);
    output->vz = position->vz + (step->vz * stepScale >> 0xc);

    checkWorldmapPositionSub0(output);
    s32 iVar1 = worldmapGetAltitudeFor2dPoint(VECTOR_1f800060.vx, VECTOR_1f800060.vz);
    iVar1 = iVar1 + -0x20000;
    if (0x20000 < iVar1) {
        iVar1 = 0x20000;
    }
    if (iVar1 < VECTOR_1f800060.vy || iVar1 < -0x280000) {
        VECTOR_1f800060.vy = iVar1;
        output->vy = iVar1;
    }
    if (-0x280000 < iVar1 && VECTOR_1f800060.vy < -0x280000) {
        VECTOR_1f800060.vy = -0x280000;
        output->vy = -0x280000;
    }

    short local_38;
    iVar1 = worldmapCheckCollisionsAgainstWorldmapModels(&VECTOR_1f800060, &local_38);
    if (iVar1 != 0) {
        assert(0);
    }
    iVar1 = checkWorldmapPositionSub1(position, step, output, stepScale, param_5);
    return iVar1;
}

void spawnWorldmapParticles(s32 type, SVECTOR*, SVECTOR*) {
    MissingCode();
}

void clearWorldmapParticles(s32 type) {
    MissingCode();
}

s32 worldmap_taskYggdrasil_update(int param_1)
{
    s32 returnState = 1;

    if (gWorldmapState->m0[param_1].m4 == 4) {
        assert(0);
    }

    VECTOR DAT_1f800090;

    switch (gWorldmapState->m0[param_1].m20) {
    case 2:
        switch (processYggdrasilInputs(&gWorldmapState->m0[param_1]))
        {
        case 0:
            switch (checkWorldmapPositionYggdrasil(&gWorldmapState->m0[param_1].m28_position, &gWorldmapState->m0[param_1].m38_step, &DAT_1f800090, gWorldmapState->m0[param_1].m60, (short)worldMapGearMode)) {
            case 0:
                gWorldmapState->m0[param_1].m60 = 0;
                (gWorldmapState->m0[param_1].m38_step).vz = 0;
                (gWorldmapState->m0[param_1].m38_step).vx = 0;
                break;
            case 1:
                adjustLocationAfterCollision(&DAT_1f800090, 0x40, 0x20, &adjustLocationAfterCollisionVar0, &adjustLocationAfterCollisionVar1);
                if (adjustLocationAfterCollisionVar0 == '\x02') {
                    gWorldmapState->m0[param_1].m60 = 0;
                }
                else {
                    (gWorldmapState->m0[param_1].m28_position) = DAT_1f800090;
                }
                break;
            default:
                assert(0);
            }
            worldmapRadarPosition.vx = (gWorldmapState->m0[param_1].m28_position).vx;
            worldmapRadarPosition.vy = (gWorldmapState->m0[param_1].m28_position).vy;
            worldmapRadarPosition.vz = (gWorldmapState->m0[param_1].m28_position).vz;
            worldmapRadarPosition.pad = (gWorldmapState->m0[param_1].m28_position).pad;
            worldmapVar_8009d52c = gWorldmapState->m0[param_1].m48;
            MissingCode();
            SVECTOR SVECTOR_1f8000a0;
            SVECTOR_1f8000a0.vx = -(short)((gWorldmapState->m0[param_1].m6C).vy >> 0xc);
            SVECTOR_1f8000a0.vy = gWorldmapState->m0[param_1].m48;
            SVECTOR_1f8000a0.vz = worldmapRotation.vz;
            RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[0].m20_rotationMatrix);
            RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[1].m20_rotationMatrix);

            // Close enough to the ground to emit particles?
            if (0x60000 > std::abs(worldmapGetAltitudeFor2dPoint((gWorldmapState->m0[param_1].m28_position).vx, (gWorldmapState->m0[param_1].m28_position).vz) - (gWorldmapState->m0[param_1].m28_position).vy)) {
                SVECTOR SVECTOR_1f8000a8;
                SVECTOR_1f8000a8.vx = (short)((gWorldmapState->m0[param_1].m28_position).vx >> 0xc);
                SVECTOR_1f8000a8.vy = (short)((gWorldmapState->m0[param_1].m28_position).vy >> 0xc);
                SVECTOR_1f8000a8.vz = (short)((gWorldmapState->m0[param_1].m28_position).vz >> 0xc);
                s32 groundType = getWorldmapGroundType(&gWorldmapState->m0[param_1].m28_position);
                switch (groundType) {
                case 2: //water
                    spawnWorldmapParticles(0x3f, &SVECTOR_1f8000a8, &SVECTOR_1f8000a0);
                    clearWorldmapParticles(0x3c);
                    break;
                case 3: //sand
                    spawnWorldmapParticles(0x3C, &SVECTOR_1f8000a8, &SVECTOR_1f8000a0);
                    clearWorldmapParticles(0x3cF);
                    break;
                }
            } else {
                clearWorldmapParticles(0x3C);
                clearWorldmapParticles(0x3F);
            }
            (gWorldmapState->m0[param_1].m38_step).vz = 0;
            (gWorldmapState->m0[param_1].m38_step).vy = 0;
            (gWorldmapState->m0[param_1].m38_step).vx = 0;
            adjustLocationAfterCollisionVar2 = 0;
            break;
        default:
            assert(0);
        }
        break;
    default:
        assert(0);
    }

    worldmapModels[0].m8.vx = gWorldmapState->m0[param_1].m28_position.vx >> 0xC;
    worldmapModels[0].m8.vy = gWorldmapState->m0[param_1].m28_position.vy >> 0xC;
    worldmapModels[0].m8.vz = gWorldmapState->m0[param_1].m28_position.vz >> 0xC;

    worldmapModels[1].m8.vx = gWorldmapState->m0[param_1].m28_position.vx >> 0xC;
    worldmapModels[1].m8.vy = gWorldmapState->m0[param_1].m28_position.vy >> 0xC;
    worldmapModels[1].m8.vz = gWorldmapState->m0[param_1].m28_position.vz >> 0xC;

    setGamestate182C(&gWorldmapState->m0[param_1].m28_position);
    gameState.m182C.pad = gWorldmapState->m0[param_1].m48;

    s32 sVar4 = gWorldmapState->m0[param_1].m20;
    if (sVar4 != 8) {
        if (sVar4 < 9) {
            if (sVar4 != 2) {
                return returnState;
            }
        }
        else if (sVar4 != 0x10) {
            return returnState;
        }
    }

    return 1;
}

void attachWorldmapModels(int param_1, int param_2)
{
    worldmapModels[param_2].m50_parentModel = &worldmapModels[param_1];
    return;
}

int worldmap_taskYggdrasilSubModels_init(int param_1)

{
    int iVar1;
    sWorldmapState* psVar2;
    sWorldmapModel* psVar3;
    long lVar4;
    long lVar5;
    long lVar6;

    // attach spinning bits to Yggdrasil main body
    attachWorldmapModels(0, 2);
    attachWorldmapModels(0, 3);
    psVar3 = &worldmapModels[0];
    psVar2 = gWorldmapState;
    gWorldmapState->m0[param_1].m20 = 0;
    lVar4 = psVar3[2].m8.vy;
    lVar5 = psVar3[2].m8.vz;
    lVar6 = psVar3[2].m8.pad;
    psVar2->m0[param_1].m28_position.vx = psVar3[2].m8.vx;
    psVar2->m0[param_1].m28_position.vy = lVar4;
    psVar2->m0[param_1].m28_position.vz = lVar5;
    psVar2->m0[param_1].m28_position.pad = lVar6;
    lVar4 = worldmapModels[3].m8.vy;
    lVar5 = worldmapModels[3].m8.vz;
    lVar6 = worldmapModels[3].m8.pad;
    psVar2->m0[param_1].m38_step.vx = worldmapModels[3].m8.vx;
    psVar2->m0[param_1].m38_step.vy = lVar4;
    psVar2->m0[param_1].m38_step.vz = lVar5;
    psVar2->m0[param_1].m38_step.pad = lVar6;
    iVar1 = worldMapGearMode;
    psVar2->m0[param_1].m50 = 0;
    psVar2->m0[param_1].m54 = 0x40;
    if ((iVar1 < 8) && (3 < iVar1)) {
        psVar2->m0[param_1].m20 = 3;
        psVar2->m0[param_1].m5C = 0x80;
        psVar2->m0[param_1].m58 = 0x80;
    }
    return 1;
}

int worldmap_taskYggdrasilSubModels_update(int param_1)
{
    short sVar1;
    sWorldmapState* psVar2;
    sWorldmapModel* psVar3;
    short sVar4;
    int iVar5;
    int iVar6;

    // spin Yggdrasil engines

    psVar3 = &worldmapModels[0];
    psVar2 = gWorldmapState;
    sVar1 = gWorldmapState->m0[param_1].m4;
    if (sVar1 == 9) {
        sVar4 = 1;
    }
    else {
        sVar4 = 2;
        if (sVar1 != 10) goto LAB_worldmap__80090850;
    }
    gWorldmapState->m0[param_1].m4 = 0;
    psVar2->m0[param_1].m20 = sVar4;
LAB_worldmap__80090850:
    sVar1 = psVar2->m0[param_1].m20;
    if (sVar1 == 1) {
        iVar5 = psVar2->m0[param_1].m58 + 4;
        psVar2->m0[param_1].m58 = iVar5;
        if (0x10 < iVar5) {
            psVar2->m0[param_1].m5C = psVar2->m0[param_1].m5C + 4;
        }
        if (0x7f < psVar2->m0[param_1].m58) {
            psVar2->m0[param_1].m58 = 0x80;
        }
        if (0x7f < psVar2->m0[param_1].m5C) {
            psVar2->m0[param_1].m5C = 0x80;
        }
        if (0x7f < psVar2->m0[param_1].m58 && 0x7f < psVar2->m0[param_1].m5C) {
            psVar2->m0[param_1].m20 = 3;
        }
    }
    else if (sVar1 < 2) {
        if (sVar1 == 0) {
            psVar2->m0[param_1].m58 = 0;
            psVar2->m0[param_1].m5C = 0;
        }
    }
    else if (sVar1 == 2) {
        iVar5 = psVar2->m0[param_1].m58 + -4;
        psVar2->m0[param_1].m58 = iVar5;
        if (iVar5 < 0x70) {
            psVar2->m0[param_1].m5C = psVar2->m0[param_1].m5C + -4;
        }
        if (psVar2->m0[param_1].m58 < 0) {
            psVar2->m0[param_1].m58 = 0;
        }
        if (psVar2->m0[param_1].m5C < 0) {
            psVar2->m0[param_1].m5C = 0;
        }
        if (psVar2->m0[param_1].m58 == 0 && psVar2->m0[param_1].m5C == 0) {
            psVar2->m0[param_1].m20 = 0;
        }
    }
    iVar5 = psVar2->m0[param_1].m54;
    iVar6 = psVar2->m0[param_1].m5C;
    psVar2->m0[param_1].m50 = psVar2->m0[param_1].m50 + psVar2->m0[param_1].m58;
    psVar2->m0[param_1].m54 = iVar5 - iVar6;

    SVECTOR SVECTOR_1f8000a0;
    SVECTOR SVECTOR_1f8000a8;
    SVECTOR_1f8000a8.vx = 0;
    SVECTOR_1f8000a0.vx = 0;
    SVECTOR_1f8000a0.vy = (short)psVar2->m0[param_1].m50;
    SVECTOR_1f8000a8.vy = (short)psVar2->m0[param_1].m54;
    SVECTOR_1f8000a0.vz = psVar3[2].m18_rotation.vz;
    SVECTOR_1f8000a8.vz = psVar3[3].m18_rotation.vz;
    RotMatrixZYX(&SVECTOR_1f8000a0, &psVar3[2].m20_rotationMatrix);
    RotMatrixZYX(&SVECTOR_1f8000a8, &psVar3[3].m20_rotationMatrix);
    return 1;
}