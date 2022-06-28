#include "noahLib.h"
#include "worldmapTaskCamera.h"
#include "worldmap.h"

const std::array<s16, 4> SHORT_ARRAY_worldmap__8009b224 = { {
        (s16)0xFEAB, (s16)0xFE39, (s16)0xFE00, (s16)0xFDC8,
} };

const std::array<s16, 4> SHORT_ARRAY_worldmap__8009b234 = { {
        (s16)0xFF00, (s16)0xFE20, (s16)0xFD48, (s16)0xFC58,
} };

const std::array<s16, 4> SHORT_ARRAY_worldmap__8009b22c = { {
        (s16)0xFF1D, (s16)0xFE39, (s16)0xFE00, (s16)0xFDC8,
} };

const std::array<s16, 4> SHORT_ARRAY_worldmap__8009b23c = { {
        (s16)0xFF50, (s16)0xFE20, (s16)0xFD48, (s16)0xFC58,
} };

const std::array<s32, 4> INT_ARRAY_worldmap__8009b214 = { {
        0x280000,      0x370000,      0x460000,      0x550000,
} };

s32 worldmapCameraVar0;

s32 worldmap_taskCamera_init(int param_1) {
    sWorldmapState* psVar1;

    psVar1 = gWorldmapState;
    if (worldMapGearMode != 6) {
        if (worldMapGearMode < 7) {
            if (0 < worldMapGearMode) {
                gWorldmapState->m0[param_1].m50 = 3;
                worldmapCameraVar0 = 0x460000;
                worldmapRotation.vx = -0x260;
                psVar1->m0[param_1].m64_array = &SHORT_ARRAY_worldmap__8009b224;
                psVar1->m0[param_1].m68_array = &SHORT_ARRAY_worldmap__8009b234;
            }
        }
        else if (worldMapGearMode == 7) {
            gWorldmapState->m0[param_1].m50 = 3;
            worldmapCameraVar0 = 0x280000;
            worldmapRotation.vx = -0x260;
            psVar1->m0[param_1].m64_array = &SHORT_ARRAY_worldmap__8009b22c;
            psVar1->m0[param_1].m68_array = &SHORT_ARRAY_worldmap__8009b23c;
        }
    }
    return 1;
}

MATRIX worldmapRotationmatrixX;

void computeCameraVector(sWorldmapCameraState* param_1, VECTOR* param_2, int param_3, SVECTOR* param_4)

{
    int iVar1;

    (param_1->m8).vx = 0;
    iVar1 = param_2->vy;
    (param_1->m8).vz = 0;
    (param_1->m8).vy = (short)(iVar1 >> 0xc);
    SVECTOR SFP_VEC4_1f8000a0;
    SFP_VEC4_1f8000a0.vx = param_4->vx;
    SFP_VEC4_1f8000a0.vy = param_4->vy;
    SFP_VEC4_1f8000a0.vz = 0;
    RotMatrixYXZ(&SFP_VEC4_1f8000a0, &worldmapRotationmatrixX);
    VECTOR VECTOR_1f800000;
    VECTOR_1f800000.vz = -(param_3 >> 0xc);
    VECTOR_1f800000.vx = 0;
    VECTOR_1f800000.vy = 0;
    VECTOR VECTOR_1f800010;
    ApplyMatrixLV(&worldmapRotationmatrixX, &VECTOR_1f800000, &VECTOR_1f800010);
    (param_1->m0).vx = (short)VECTOR_1f800010.vx;
    (param_1->m0).vy = (param_1->m8).vy + (short)VECTOR_1f800010.vy;
    (param_1->m0).vz = (short)VECTOR_1f800010.vz;
    SFP_VEC4_1f8000a0.vx = 0;
    SFP_VEC4_1f8000a0.vy = param_4->vy;
    SFP_VEC4_1f8000a0.vz = param_4->vz;
    RotMatrixYXZ(&SFP_VEC4_1f8000a0, &worldmapRotationmatrixX);
    SFP_VEC4_1f8000a0.vx = 0;
    SFP_VEC4_1f8000a0.vy = -0x1000;
    SFP_VEC4_1f8000a0.vz = 0;
    ApplyMatrix(&worldmapRotationmatrixX, &SFP_VEC4_1f8000a0, &param_1->m10);
    return;
}

int worldmap_taskCamera_updateSub0(int param_1, const std::array<s16, 4>* param_2, const std::array<s16, 4>* param_3) {
    MissingCode();
    return 0;
}

s32 worldmap_taskCamera_update(int param_1) {
    short sVar1;
    sWorldmapState* psVar2;
    const std::array<s16, 4>* psVar3;
    int iVar4;
    int iVar5;
    int iVar6;

    psVar2 = gWorldmapState;
    switch (gWorldmapState->m0[param_1].m4) {
    case 9: // yggdrasil take off
        gWorldmapState->m0[param_1].m64_array = &SHORT_ARRAY_worldmap__8009b22c;
        psVar2->m0[param_1].m4 = 0;
        psVar2->m0[param_1].m68_array = &SHORT_ARRAY_worldmap__8009b23c;
        if (gWorldmapState->m0[param_1].m50 == 0) {
            psVar2->m0[param_1].m50 = 1;
        }
        break;
    case 10: // yggdrasil landing
        gWorldmapState->m0[param_1].m64_array = &SHORT_ARRAY_worldmap__8009b224;
        psVar2->m0[param_1].m4 = 0;
        psVar2->m0[param_1].m68_array = &SHORT_ARRAY_worldmap__8009b234;
        if (gWorldmapState->m0[param_1].m50 == 0) {
            psVar2->m0[param_1].m50 = 1;
        }
        break;
    case 14:
        gWorldmapState->m0[param_1].m4 = 0;
        psVar2->m0[param_1].m20 = 0;
        worldmapMatrixMode = 0;
        break;
    case 17:
        worldmapCameraVar0 = 0x400000;
        gWorldmapState->m0[param_1].m20 = 3;
        psVar2->m0[param_1].m4 = 0;
        psVar2->m0[param_1].m58 = -0x100;
        break;
    }

    switch (psVar2->m0[param_1].m20)
    {
    case 0:
        iVar4 = worldmap_taskCamera_updateSub0(psVar2->m0[param_1].m50, psVar2->m0[param_1].m64_array, psVar2->m0[param_1].m68_array);
        if (psVar2->m0[param_1].m50 != iVar4) {
            psVar2->m0[param_1].m20 = 1;
            psVar2->m0[param_1].m22 = 0;
            psVar2->m0[param_1].m50 = iVar4;
            psVar2->m0[param_1].m54 = INT_ARRAY_worldmap__8009b214[iVar4];
            psVar2->m0[param_1].m58 = (*psVar2->m0[param_1].m64_array)[psVar2->m0[param_1].m50];
            psVar2->m0[param_1].m60 = (int)worldmapRotation.vx << 0xc;
        }
        computeCameraVector(&worldmapCameraVector, &worldmapRadarPosition3, worldmapCameraVar0, &worldmapRotation);
        break;
    case 1:
        if (4 < psVar2->m0[param_1].m22) {
            iVar4 = worldmap_taskCamera_updateSub0(psVar2->m0[param_1].m50, psVar2->m0[param_1].m64_array, psVar2->m0[param_1].m68_array);
            if (psVar2->m0[param_1].m50 != iVar4) {
                psVar2->m0[param_1].m50 = iVar4;
                psVar2->m0[param_1].m54 = INT_ARRAY_worldmap__8009b214[iVar4];
                psVar2->m0[param_1].m58 = (*psVar2->m0[param_1].m64_array)[psVar2->m0[param_1].m50];
            }
            psVar2->m0[param_1].m22 = 0;
        }
        iVar5 = psVar2->m0[param_1].m54;
        iVar4 = iVar5 - worldmapCameraVar0;
        if (iVar5 != worldmapCameraVar0) {
            if (iVar4 < 1) {
                iVar4 = worldmapCameraVar0 + -0x2000;
            }
            else {
                iVar6 = iVar4 >> 3;
                if (0x40000 < iVar4) {
                    iVar6 = 0x8000;
                }
                iVar4 = worldmapCameraVar0 + iVar6;
                worldmapCameraVar0 = iVar5;
                if (iVar6 < 0x40) goto LAB_worldmap__80091eac;
            }
            worldmapCameraVar0 = iVar4;
        }
    LAB_worldmap__80091eac:
        iVar4 = psVar2->m0[param_1].m58;
        if (iVar4 != worldmapRotation.vx) {
            iVar4 = (iVar4 - worldmapRotation.vx) * 0x1000;
            if (iVar4 < 0) {
                iVar4 = psVar2->m0[param_1].m60 + (iVar4 >> 5);
            }
            else if (psVar2->m0[param_1].m50 == 0) {
                iVar4 = psVar2->m0[param_1].m60 + 0xf32;
            }
            else {
                iVar4 = psVar2->m0[param_1].m60 + 0x799;
            }
            psVar2->m0[param_1].m60 = iVar4;
            worldmapRotation.vx = (short)(psVar2->m0[param_1].m60 >> 0xc);
        }
        if (psVar2->m0[param_1].m54 == worldmapCameraVar0 && psVar2->m0[param_1].m58 == (int)worldmapRotation.vx) {
            psVar2->m0[param_1].m20 = 0;
        }
        computeCameraVector(&worldmapCameraVector, &worldmapRadarPosition3, worldmapCameraVar0, &worldmapRotation);
        break;
    default:
        assert(0);
    }

    psVar2->m0[param_1].m22 = psVar2->m0[param_1].m22 + 1;
    return 1;
}