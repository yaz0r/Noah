#include "noahLib.h"
#include "worldmapTaskYggdrasil.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"
#include "worldmapExit.h"
#include "worldmapDynamicCollisions.h"
#include "worldmapParticles.h"

ushort getWorldmapGroundType(VECTOR* param_1);
int checkWorldmapPosition(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, int param_5);
int checkWorldmapPositionSub1_0_1(int param_1, int param_2);

s32 worldmap_taskYggdrasil_init(int param_1)
{
    s32 returnState = 1;

    gWorldmapState->m0[param_1].m24 = 0;
    getGamestate182C(&gWorldmapState->m0[param_1].m28_position);

    gWorldmapState->m0[param_1].m38_step.vz = 0;
    gWorldmapState->m0[param_1].m38_step.vy = 0;
    gWorldmapState->m0[param_1].m38_step.vx = 0;
    gWorldmapState->m0[param_1].m64 = 0;
    gWorldmapState->m0[param_1].m60 = 0;
    gWorldmapState->m0[param_1].m74 = 0;
    gWorldmapState->m0[param_1].m68 = 0xffd80000;
    gWorldmapState->m0[param_1].m70 = 0;
    gWorldmapState->m0[param_1].m6C = 0;
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
        gWorldmapState->m0[param_1].m74 = worldmapNumActivePartyMembers;
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
    SVECTOR_1f8000a0.vx = -(short)(gWorldmapState->m0[param_1].m70 >> 0xc);
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

    if (param_1->m64 == 0) {
        param_1->m64 = 0x1;
        param_1->m5C = 0;
        param_1->m58 = (int)param_1->m48 << 0xc;
    }
    else {
        iVar7 = 0;
        if (param_1->m64 == 0x1) {
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
    iVar7 = (param_1->m6C);
    if (iVar7 == 1) {
        iVar7 = (param_1->m70);
        if (iVar7 != 0) {
            if (iVar7 < 0) {
                iVar7 = iVar7 + 0x2000;
            }
            else {
                iVar7 = iVar7 + -0x2000;
            }
            (param_1->m70) = iVar7;
        }
        if ((worldmapInput1_0 & 0x4000) != 0) {
            (param_1->m6C) = 2;
        }
        if ((worldmapInput1_0 & 0x1000) != 0) {
            (param_1->m6C) = 3;
        }
    }
    else if (iVar7 < 2) {
        if (iVar7 == 0) {
            (param_1->m6C) = 1;
            (param_1->m70) = 0;
        }
    }
    else if (iVar7 == 2) {
        lVar4 = -0x80000;
        if ((worldmapInput1_0 & 0x4000) != 0) {
            iVar7 = (param_1->m70) + -0x4000;
            (param_1->m70) = iVar7;
            bVar1 = iVar7 < -0x80000;
            goto LAB_worldmap__8009124c;
        }
    LAB_worldmap__8009125c:
        (param_1->m6C) = 1;
    }
    else if (iVar7 == 3) {
        lVar4 = 0x80000;
        if ((worldmapInput1_0 & 0x1000) == 0) goto LAB_worldmap__8009125c;
        iVar7 = (param_1->m70) + 0x4000;
        (param_1->m70) = iVar7;
        bVar1 = 0x80000 < iVar7;
    LAB_worldmap__8009124c:
        if (bVar1) {
            (param_1->m70) = lVar4;
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
    iVar7 = (param_1->m70);
    (param_1->m38_step).vx = lVar4;
    lVar4 = getAngleCos(iVar7 >> 0xc);
    (param_1->m38_step).vy = lVar4;
    iVar7 = getAngleSin((int)worldmapRotation.vy);
    (param_1->m38_step).vz = -iVar7;
    VectorNormal(&param_1->m38_step, &param_1->m38_step);

    if ((((worldmapInput2_0 & 0x20) == 0) || (iVar7 = 1, worldmapExitVar0 == -1)) && (((worldmapInput2_0 & 0x40) == 0 || (iVar7 = 4, param_1->m60 != 0 || worldmapRotation.vz != 0)))) {
        if ((worldmapInput2_0 & 0x10) != 0) {
            if (worldmapExitVar1 != -1) {
                return 0;
            }
            if (worldmapExitVar0 != -1) {
                return 0;
            }
            worldmapExitVar2 = 1;
        }
        iVar7 = 0;
    }
    return iVar7;
}

void checkWorldmapPositionSub0(VECTOR* param_1);
int worldmapCheckCollisionsAgainstWorldmapModels(VECTOR* param_1, short* param_2);
int checkWorldmapPositionSub1(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, short param_5);
void processWorldmapDynamicCollisions(VECTOR* param_1, int param_2, int param_3, u8* param_4, u8* param_5);

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

int processYggdrasilInputs_mode4(sWorldmapStateEntry* param_1) {
    short sVar1;
    ushort uVar2;
    long lVar3;
    int iVar4;

    switch (worldmapInput1_0 >> 0xc) {
    case 1:
        param_1->m48 = worldmapRotation.vy;
        break;
    case 2:
        uVar2 = worldmapRotation.vy + 0x400;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 3:
        uVar2 = worldmapRotation.vy + 0x200;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 4:
        uVar2 = worldmapRotation.vy + 0x800;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 6:
        uVar2 = worldmapRotation.vy + 0x600;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 8:
        uVar2 = worldmapRotation.vy - 0x400;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 9:
        uVar2 = worldmapRotation.vy - 0x200;
        param_1->m48 = uVar2 & 0xfff;
        break;
    case 0xc:
        uVar2 = worldmapRotation.vy - 0x600;
        param_1->m48 = uVar2 & 0xfff;
        break;
    default:
        break;
    }

    if ((worldmapInput1_0 & 0xf000) != 0) {
        lVar3 = getAngleCos((int)param_1->m48);
        sVar1 = param_1->m48;
        (param_1->m38_step).vx = lVar3;
        iVar4 = getAngleSin((int)sVar1);
        (param_1->m38_step).vz = -iVar4;
    }
    if ((((worldmapInput2_0 & 0x20) == 0) || (iVar4 = 1, worldmapExitVar0 == -1)) && (iVar4 = 4, (worldmapInput2_0 & 0x40) == 0)) {
        if ((worldmapInput2_0 & 0x10) != 0) {
            if (worldmapExitVar1 != -1) {
                return 0;
            }
            if (worldmapExitVar0 != -1) {
                return 0;
            }
            worldmapExitVar2 = 1;
        }
        iVar4 = 0;
    }
    return iVar4;
}

extern s32 worldmapWaterWave2;
extern s32 worldmapWaterWave;
void averageVector(VECTOR* param_1, VECTOR* param_2, VECTOR* param_3);
u8* getWorldChunkForPosition(int x, int y);

VECTOR VECTOR_worldmap__8009b244 = {0xB50,0,-0xB50, 0};
VECTOR VECTOR_worldmap__8009b254 = {0xB50,0,0xB50, 0};

int getWorldmapElevationWithWater(uint x, uint z) {
    short sVar1;
    byte* pbVar2;
    int iVar3;
    uint uVar4;
    uint uVar5;
    int iVar6;
    int iVar7;
    int iVar8;
    int iVar9;
    int iVar10;
    SVECTOR SVECTOR_1f8000a0;
    SVECTOR SVECTOR_1f8000a8;
    SVECTOR SVECTOR_1f8000b0;
    SVECTOR SVECTOR_1f8000b8;
    VECTOR DAT_1f800000;
    VECTOR VECTOR_1f800010;
    VECTOR VECTOR_1f800020;

    pbVar2 = getWorldChunkForPosition(x, z);
    uVar5 = x >> 0x13 & 7;
    iVar8 = uVar5 * 0x200;
    uVar4 = z >> 0x13 & 7;
    iVar7 = uVar4 * 0x200;
    iVar9 = worldmapWaterWave + iVar8;
    iVar3 = getAngleCos(worldmapWaterWave2 + iVar7);
    iVar9 = getAngleCos(iVar9);
    iVar6 = (uVar5 + 1) * 0x200;
    iVar10 = worldmapWaterWave + iVar6;
    SVECTOR_1f8000a0.vy = (short)(iVar9 * ((iVar3 << 4) >> 3) >> 0x14) + (char)*pbVar2 * 8;
    iVar3 = getAngleCos(worldmapWaterWave2 + iVar7);
    iVar9 = getAngleCos(iVar10);
    iVar7 = (uVar4 + 1) * 0x200;
    iVar8 = worldmapWaterWave + iVar8;
    SVECTOR_1f8000a8.vy = (short)(iVar9 * ((iVar3 << 4) >> 3) >> 0x14) + (char)pbVar2[4] * 8;
    iVar3 = getAngleCos(worldmapWaterWave2 + iVar7);
    iVar9 = getAngleCos(iVar8);
    iVar6 = worldmapWaterWave + iVar6;
    SVECTOR_1f8000b0.vy = (short)(iVar9 * ((iVar3 << 4) >> 3) >> 0x14) + (char)pbVar2[0x24] * 8;
    iVar3 = getAngleCos(worldmapWaterWave2 + iVar7);
    iVar9 = getAngleCos(iVar6);
    iVar3 = (iVar9 * ((iVar3 << 4) >> 3) >> 0x14) + (char)pbVar2[0x28] * 8;
    SVECTOR_1f8000b8.vy = (short)iVar3;
    uVar4 = x;
    if ((int)x < 0) {
        uVar4 = x + 7;
    }
    uVar5 = (int)uVar4 >> 3 & 0xffff;
    uVar4 = z;
    if ((int)z < 0) {
        uVar4 = z + 7;
    }
    uVar4 = (int)uVar4 >> 3 & 0xffff;
    if ((pbVar2[1] & 0x80) == 0) {
        if ((int)((uVar5 - 0x10000) * VECTOR_worldmap__8009b244.vx + -(s32)uVar4 * VECTOR_worldmap__8009b244.vz) < 0) {
            DAT_1f800000.vx = -0x80;
            DAT_1f800000.vz = -0x80;
            VECTOR_1f800010.vx = -0x80;
            VECTOR_1f800010.vz = 0;
            DAT_1f800000.vy = (int)SVECTOR_1f8000b0.vy - (int)SVECTOR_1f8000a8.vy;
            VECTOR_1f800010.vy = (int)SVECTOR_1f8000a0.vy - (int)SVECTOR_1f8000a8.vy;
            goto LAB_worldmap__80093dcc;
        }
        DAT_1f800000.vx = 0;
        VECTOR_1f800010.vx = -0x80;
        sVar1 = SVECTOR_1f8000a8.vy;
    }
    else {
        if (-1 < (int)(uVar5 * VECTOR_worldmap__8009b254.vx + -(s32)uVar4 * VECTOR_worldmap__8009b254.vz)) {
            DAT_1f800000.vx = 0x80;
            VECTOR_1f800010.vx = 0x80;
            DAT_1f800000.vz = 0;
            VECTOR_1f800010.vz = -0x80;
            DAT_1f800000.vy = (int)SVECTOR_1f8000a8.vy - (int)SVECTOR_1f8000a0.vy;
            VECTOR_1f800010.vy = iVar3 - SVECTOR_1f8000a0.vy;
            goto LAB_worldmap__80093dcc;
        }
        DAT_1f800000.vx = 0x80;
        VECTOR_1f800010.vx = 0;
        sVar1 = SVECTOR_1f8000a0.vy;
    }
    VECTOR_1f800010.vz = -0x80;
    DAT_1f800000.vz = -0x80;
    DAT_1f800000.vy = iVar3 - sVar1;
    VECTOR_1f800010.vy = (int)SVECTOR_1f8000b0.vy - (int)sVar1;
LAB_worldmap__80093dcc:
    OuterProduct0(&VECTOR_1f800010, &DAT_1f800000, &VECTOR_1f800020);
    VectorNormal(&VECTOR_1f800020, &DAT_1f800000);
    VECTOR_1f800010.vx = (int)x >> 0xc & 0x7f;
    VECTOR_1f800010.vz = -(s32)((int)z >> 0xc & 0x7fU);
    if ((pbVar2[1] & 0x80) == 0) {
        VECTOR_1f800020.vx = 0x80;
        sVar1 = SVECTOR_1f8000a8.vy;
    }
    else {
        VECTOR_1f800020.vx = 0;
        sVar1 = SVECTOR_1f8000a0.vy;
    }
    VECTOR_1f800020.vz = 0;
    VECTOR_1f800020.vy = (long)sVar1;
    averageVector(&VECTOR_1f800010, &VECTOR_1f800020, &DAT_1f800000);
    return VECTOR_1f800010.vy << 0xc;
}

int checkYggdrasilLandingPosition(VECTOR* param_1, VECTOR* param_2, int param_3) {
    int iVar1;
    int iVar2;
    VECTOR VStack56;
    VECTOR local_28;

    iVar2 = 0;
    do {
        local_28.vx = getAngleCos(iVar2);
        local_28.vz = getAngleSin(iVar2);
        local_28.vz = -local_28.vz;
        iVar1 = checkWorldmapPosition(param_1, &local_28, &VStack56, param_3, 2);
        if (iVar1 == 1) {
            return iVar2;
        }
        iVar2 = iVar2 + 0x100;
    } while (iVar2 < 0x1000);
    return -1;
}

s16 yggdrasilExit_8009b6d0 = 0xE;
s16 yggdrasilExit_8009b6e0 = 0x1D;

void setupYggdrasilInternalExit() {
    if (adjustLocationAfterCollisionVar0 != '\0') {
        if (targetVehicleEntityIndex == '\x0f') {
            worldmapCurrentExit = &worldmapExitToField312;
            worldmapExitVar0 = yggdrasilExit_8009b6d0;
        }
        else if (targetVehicleEntityIndex == '\x10') {
            worldmapCurrentExit = &worldmapExitToField440;
            worldmapExitVar0 = yggdrasilExit_8009b6e0;
        }
    }
}

s32 worldmap_taskYggdrasil_update(int param_1)
{
    s32 returnState = 1;

    SVECTOR SVECTOR_1f8000a0;

    sWorldmapStateEntry* pCurrentEntity = &gWorldmapState->m0[param_1];

    if (gWorldmapState->m0[param_1].m4 == 4) {
        pCurrentEntity->m4 = 0;
        pCurrentEntity->m74++;
        // has enough gears boarded the yggdrasil?
        if (worldmapNumActivePartyMembers == pCurrentEntity->m74) {
            // return to yggdrasil mode
            changeWorldmapEntityState(8, 9);
            worldmapRadarPosition.vx = (pCurrentEntity->m28_position).vx;
            worldmapRadarPosition.vy = (pCurrentEntity->m28_position).vy;
            worldmapRadarPosition.vz = (pCurrentEntity->m28_position).vz;
            worldmapRadarPosition.pad = (pCurrentEntity->m28_position).pad;
            worldmapVar_8009d52c = pCurrentEntity->m48;
            u16 uVar3 = gameState.m1834 | 0x4000;
            u16 uVar5 = gameState.m1834 & 0x1fff;
            gameState.m1834 = uVar3;
            // what mode is the yggdrasil in?
            switch (uVar5) {
            case 3: // fly
                Noah_MissingCode("Yggdrasil music switch");
                SVECTOR_1f8000a0.vx = (short)((pCurrentEntity->m28_position).vx >> 0xc);
                SVECTOR_1f8000a0.vy = (short)((pCurrentEntity->m28_position).vy >> 0xc);
                SVECTOR_1f8000a0.vz = (short)((pCurrentEntity->m28_position).vz >> 0xc);
                switch (getWorldmapGroundType(&pCurrentEntity->m28_position)) {
                case 3:
                    spawnWorldmapParticles(0x3E, &SVECTOR_1f8000a0, 0);
                    break;
                default:
                    spawnWorldmapParticles(0x3D, &SVECTOR_1f8000a0, 0);
                    break;
                }
                changeWorldmapEntityState(9, 9);
                changeWorldmapEntityState(10, 9);
                changeWorldmapEntityState(0xb, 9);
                returnState = 2;
                pCurrentEntity->m20 = 8;
                pCurrentEntity->m2 = 0x3c;
                worldMapGearMode = 7;
                break;
            default:
                assert(0);
            }

            currentWorldmapDynamicCollisionSlot = 0;
            gameState.m22B1_isOnGear[0] = 1;
            if (gameState.m1D34_currentParty[1] != 0xff) {
                gameState.m22B1_isOnGear[1] = 1;
            }
            if (gameState.m1D34_currentParty[2] != 0xff) {
                gameState.m22B1_isOnGear[2] = 1;
            }
            //FUN_Worldmap__80075228();
            MissingCode();
            worldmapCurrentExit = nullptr;
            worldmapExitVar0 = 0xffff;
            worldmapExitVar1 = 0xffff;
        }
    }

    VECTOR DAT_1f800090;

    switch (gWorldmapState->m0[param_1].m20) {
    case 1: // Idle on the ground
        (pCurrentEntity->m28_position).vy = worldmapGetAltitudeFor2dPoint((pCurrentEntity->m28_position).vx, (pCurrentEntity->m28_position).vz) + -0x1000;
        addWorldmapDynamicCollisions(param_1, &pCurrentEntity->m28_position, 0x40, 0x400);
        break;
    case 2: // Flying
        switch (s32 inputResult = processYggdrasilInputs(&gWorldmapState->m0[param_1]))
        {
        case 0:
            switch (s32 moveResult = checkWorldmapPositionYggdrasil(&gWorldmapState->m0[param_1].m28_position, &gWorldmapState->m0[param_1].m38_step, &DAT_1f800090, gWorldmapState->m0[param_1].m60, (short)worldMapGearMode)) {
            case 0:
                gWorldmapState->m0[param_1].m60 = 0;
                (gWorldmapState->m0[param_1].m38_step).vz = 0;
                (gWorldmapState->m0[param_1].m38_step).vx = 0;
                break;
            case 1:
                processWorldmapDynamicCollisions(&DAT_1f800090, 0x40, 0x20, &adjustLocationAfterCollisionVar0, &targetVehicleEntityIndex);
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
            setupWorldmapExits(&pCurrentEntity->m28_position, 2);
            setupYggdrasilInternalExit();
            SVECTOR SVECTOR_1f8000a0;
            SVECTOR_1f8000a0.vx = -(short)((gWorldmapState->m0[param_1].m70) >> 0xc);
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
                    clearWorldmapParticles(0x3F);
                    break;
                }
            } else {
                clearWorldmapParticles(0x3C);
                clearWorldmapParticles(0x3F);
            }
            (gWorldmapState->m0[param_1].m38_step).vz = 0;
            (gWorldmapState->m0[param_1].m38_step).vy = 0;
            (gWorldmapState->m0[param_1].m38_step).vx = 0;
            currentWorldmapDynamicCollisionSlot = 0;
            break;
        case 1:
            continueWorldmapLoop = 0;
            exitWorldMapMode = 0;
            break;
        case 4:
        {
            if (checkWorldmapPositionSub1_0_1(2, getWorldmapGroundType(&pCurrentEntity->m28_position)) != 0) {
                s32 iVar9 = checkYggdrasilLandingPosition(&pCurrentEntity->m28_position, &pCurrentEntity->m38_step, 0x68000);
                if (iVar9 != -1) {
                    s32 iVar6 = (pCurrentEntity->m28_position).vx;
                    s32 iVar7 = (pCurrentEntity->m28_position).vz;
                    pCurrentEntity->m20 = 0x10;
                    (pCurrentEntity->m78) = iVar9;
                    pCurrentEntity->m68 = worldmapGetAltitudeFor2dPoint(iVar6, iVar7);
                    changeWorldmapEntityState(0xb, 10);
                    //FUN_8003a89c(pMusic, 0, 0xf0);
                    pCurrentEntity->m7C = 1;
                    clearWorldmapParticles(0x3c);
                    clearWorldmapParticles(0x3f);
                    //worldmapExitArrayPtr = (short*)0xffffffff;
                    worldmapCurrentExit = nullptr;
                    worldmapExitVar0 = 0xffff;
                    worldmapExitVar1 = 0xffff;
                    targetVehicleEntityIndex = 0;
                    adjustLocationAfterCollisionVar0 = '\0';
                }
            }

            break;
        }
        default:
            assert(0);
        }
        break;
    case 4: // landing?
        assert(0); // implemented this by mistake, so untested
        if (processYggdrasilInputs_mode4(&gWorldmapState->m0[param_1]) != 1) {
            s32 iVar6 = (int)pCurrentEntity->m48 - (uint)(ushort)gameState.m182C.pad;
            s32 iVar9 = iVar6;
            if (iVar6 < 0) {
                iVar9 = -iVar6;
            }
            if (0x800 < iVar9) {
                if (iVar6 < 0) {
                    iVar6 = iVar6 + 0x1000;
                }
                else {
                    iVar6 = iVar6 + -0x1000;
                }
            }
            iVar9 = iVar6;
            if (iVar6 < 0) {
                iVar9 = -iVar6;
            }
            if (0x80 < iVar9) {
                s32 sVar4;
                if (iVar6 < 0) {
                    sVar4 = gameState.m182C.pad + -0x80;
                }
                else {
                    sVar4 = gameState.m182C.pad + 0x80;
                }
                pCurrentEntity->m48 = sVar4;
            }
            iVar9 = worldMapGearMode;
            pCurrentEntity->m48 = pCurrentEntity->m48 & 0xfff;
            VECTOR DAT_1f800090;
            s32 iVar7 = checkWorldmapPosition(&pCurrentEntity->m28_position, &pCurrentEntity->m38_step, &DAT_1f800090, pCurrentEntity->m4A << 0xc, iVar9);
            if (iVar7 == 0) {
                (pCurrentEntity->m38_step) = DAT_1f800090;
                iVar7 = checkWorldmapPosition(&pCurrentEntity->m28_position, &pCurrentEntity->m38_step, &DAT_1f800090, pCurrentEntity->m4A << 0xc, worldMapGearMode);
                if (iVar7 == 0) {
                    (pCurrentEntity->m38_step).vz = 0;
                    (pCurrentEntity->m38_step).vx = 0;
                }
            }
            if (iVar7 == 1) {
                iVar9 = getWorldmapElevationWithWater(DAT_1f800090.vx, DAT_1f800090.vz);
                DAT_1f800090.vy = iVar9 + 0x18000;
                processWorldmapDynamicCollisions(&DAT_1f800090, 0x40, 0x20, (byte*)&adjustLocationAfterCollisionVar0, &targetVehicleEntityIndex);
                if (adjustLocationAfterCollisionVar0 == '\x02') {
                    iVar9 = getWorldmapElevationWithWater((pCurrentEntity->m28_position).vx, (pCurrentEntity->m28_position).vz);
                    (pCurrentEntity->m28_position).vy = DAT_1f800090.vy + 0x18000;
                    pCurrentEntity->m60 = 0;
                }
                else {
                    (pCurrentEntity->m28_position) = DAT_1f800090;
                    if (((pCurrentEntity->m38_step).vx | (pCurrentEntity->m38_step).vz) != 0) {
                        //FUN_worldmap__8007528c();
                        MissingCode();
                    }
                }
            }
            else {
                iVar9 = getWorldmapElevationWithWater((pCurrentEntity->m28_position).vx, (pCurrentEntity->m28_position).vz);
                (pCurrentEntity->m28_position).vy = iVar9 + 0x18000;
            }
            worldmapRadarPosition.vx = (pCurrentEntity->m28_position).vx;
            worldmapRadarPosition.vy = (pCurrentEntity->m28_position).vy;
            worldmapRadarPosition.vz = (pCurrentEntity->m28_position).vz;
            worldmapRadarPosition.pad = (pCurrentEntity->m28_position).pad;
            worldmapVar_8009d52c = pCurrentEntity->m48;
            //FUN_worldmap__80094238(&pCurrentEntity->m28_position, 2);
            //FUN_worldmap__8008e078();
            MissingCode();
            SVECTOR_1f8000a0.vx = -(short)((pCurrentEntity->m70) >> 0xc);
            SVECTOR_1f8000a0.vy = pCurrentEntity->m48;
            SVECTOR_1f8000a0.vz = worldmapRotation.vz;
            RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[0].m20_rotationMatrix);
            RotMatrixYXZ(&SVECTOR_1f8000a0, &worldmapModels[1].m20_rotationMatrix);
            s32 uVar5 = pCurrentEntity->m22 - 1;
            pCurrentEntity->m22 = uVar5;
            if ((int)((uint)uVar5 << 0x10) < 1) {
                pCurrentEntity->m22 = 0;
                clearWorldmapParticles(3);
            }
            else {
                SVECTOR SVECTOR_1f8000a8;
                SVECTOR_1f8000a8.vx = (short)((pCurrentEntity->m28_position).vx >> 0xc);
                SVECTOR_1f8000a8.vy = (short)((pCurrentEntity->m28_position).vy >> 0xc);
                SVECTOR_1f8000a8.vz = (short)((pCurrentEntity->m28_position).vz >> 0xc);
                spawnWorldmapParticles(3, &SVECTOR_1f8000a8, &SVECTOR_1f8000a0);
            }
            if (((pCurrentEntity->m38_step).vx | (pCurrentEntity->m38_step).vz) == 0) {
                clearWorldmapParticles(4);
                pCurrentEntity->m22 = 0;
            }
            else {
                SVECTOR SVECTOR_1f8000a8;
                SVECTOR_1f8000a8.vx = (short)((pCurrentEntity->m28_position).vx >> 0xc);
                SVECTOR_1f8000a8.vy = (short)((pCurrentEntity->m28_position).vy >> 0xc);
                SVECTOR_1f8000a8.vz = (short)((pCurrentEntity->m28_position).vz >> 0xc);
                spawnWorldmapParticles(4, &SVECTOR_1f8000a8, &SVECTOR_1f8000a0);
            }
            (pCurrentEntity->m38_step).vz = 0;
            (pCurrentEntity->m38_step).vy = 0;
            (pCurrentEntity->m38_step).vx = 0;
            s32 sVar4 = getWorldmapGroundType(&DAT_1f800090);
            if (sVar4 == 3) {
                clearWorldmapParticles(3);
                clearWorldmapParticles(4);
                pCurrentEntity->m20 = 3;
            }
            currentWorldmapDynamicCollisionSlot = 0;
        }
        else {
            if (worldmapCurrentExit->mE_type == 2) {
                pCurrentEntity->m20 = 0x20;
                s32 iVar9 = (pCurrentEntity->m28_position).vx;
                pCurrentEntity->m68 = 0x30000;
                SVECTOR SVECTOR_1f8000a8;
                SVECTOR_1f8000a8.vx = (short)(iVar9 >> 0xc);
                SVECTOR_1f8000a8.vy = (short)((pCurrentEntity->m28_position).vy >> 0xc);
                SVECTOR_1f8000a8.vz = (short)((pCurrentEntity->m28_position).vz >> 0xc);
                spawnWorldmapParticles(2, &SVECTOR_1f8000a8, &SVECTOR_1f8000a0);
            }

            continueWorldmapLoop = 0;
            exitWorldMapMode = 0;
        }
        break;
    case 8:
        (pCurrentEntity->m28_position).vy = (pCurrentEntity->m28_position).vy + -0x8000;
        if ((pCurrentEntity->m28_position).vy < -0x1dffff) {
            (pCurrentEntity->m28_position).vy = -0x1e0000;
            if (pCurrentEntity->m4 == 0xb) {
                pCurrentEntity->m64 = 0x0;
                pCurrentEntity->m20 = 2;
                pCurrentEntity->m4 = 0;
                //switchWorldmapMusic(DAT_8009c888, woldmapFile7);
                Noah_MissingCode("Yggdrasil music");
            }
        }
        break;
    case 0x10: // landing
        (pCurrentEntity->m28_position).vy = (pCurrentEntity->m28_position).vy + 0x8000;
        if (pCurrentEntity->m28_position.vy < pCurrentEntity->m68) {
            if ((-0x200000 < pCurrentEntity->m28_position.vy) && (pCurrentEntity->m7C != 0)) {
                s32 iVar9 = (pCurrentEntity->m28_position).vx;
                pCurrentEntity->m7C = 0;
                SVECTOR_1f8000a0.vx = (short)(iVar9 >> 0xc);
                SVECTOR_1f8000a0.vy = (short)((int)pCurrentEntity->m68 >> 0xc);
                SVECTOR_1f8000a0.vz = (short)((pCurrentEntity->m28_position).vz >> 0xc);
                s32 sVar4 = getWorldmapGroundType(&pCurrentEntity->m28_position);
                s32 uVar11 = 0x3d;
                if (sVar4 == 3) {
                    uVar11 = 0x3e;
                }
                spawnWorldmapParticles(uVar11, &SVECTOR_1f8000a0, 0);
            }
        }
        else {
            // Landed!
            (pCurrentEntity->m28_position).vy = pCurrentEntity->m68;
            pCurrentEntity->m20 = 1; //go into idle mode
            //switchWorldmapMusic(uVar11, uVar2);
            Noah_MissingCode("Switch worldmap music");
            changeWorldmapEntityState(8, 10);
            changeWorldmapEntityState(9, 10);
            changeWorldmapEntityState(10, 10);
            changeWorldmapEntityState(4, 3);
            changeWorldmapEntityState(1, 3);
            gameState.m22B1_isOnGear[0] = 1;
            if (gameState.m1D34_currentParty[1] != 0xff) {
                changeWorldmapEntityState(5, 3);
                changeWorldmapEntityState(2, 3);
                gameState.m22B1_isOnGear[1] = 1;
            }
            if (gameState.m1D34_currentParty[2] != 0xff) {
                changeWorldmapEntityState(6, 3);
                changeWorldmapEntityState(3, 3);
                gameState.m22B1_isOnGear[2] = 1;
            }
            (pCurrentEntity->m38_step).vz = 0;
            (pCurrentEntity->m38_step).vy = 0;
            (pCurrentEntity->m38_step).vx = 0;
            pCurrentEntity->m74 = 0;
            gameState.m1834 = gameState.m1834 & 0x3fff;
            clearWorldmapParticles(0);
            clearWorldmapParticles(1);
        LAB_worldmap__8008f9bc:
            //FUN_worldmap__80075228();
            MissingCode();
        }
        worldmapRadarPosition.vy = (pCurrentEntity->m28_position).vy;
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