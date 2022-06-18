#include "noahLib.h"
#include "worldmap.h"
#include "worldmapTask.h"
#include "worldmapTaskGround.h"
#include "kernel/gameState.h"
#include "worldmapMinimap.h"
#include "field/field.h"
#include "field/fieldGraphicObject.h"
#include "kernel/gte.h"
#include "worldmapWorldStreaming.h"

void drawWorlmapPolyFT4Array1(void) {
    MissingCode();
}

s32 worldmap_taskGround_init(s32 index) {
    return 1;
}

void computeWormapSpritePosition(VECTOR* v) {
    int iVar1;
    int iVar2;

    iVar2 = v->vx;
    if (iVar2 < -0x4000000) {
        iVar1 = worldmapSizeX * 0x800000;
    }
    else {
        if (iVar2 < 0x4000001) goto LAB_worldmap__800934d8;
        iVar1 = worldmapSizeX * -0x800000;
    }
    v->vx = iVar2 + iVar1;
LAB_worldmap__800934d8:
    iVar2 = v->vz;
    if (iVar2 < -0x4000000) {
        iVar1 = worldmapProjVar1 * 0x800000;
    }
    else {
        if (iVar2 < 0x4000001) {
            return;
        }
        iVar1 = worldmapProjVar1 * -0x800000;
    }
    v->vz = iVar2 + iVar1;
}

void worldmapDrawSpriteActors() {
    for (int i = 0; i < 0x40; i++) {
        if ((gWorldmapState->m0[i].m24 == 0) && (gWorldmapState->m0[i].m4C)) {
            VECTOR temp;
            temp.vx = gWorldmapState->m0[i].m28_position.vx - worldmapRadarPosition3.vx;
            temp.vz = gWorldmapState->m0[i].m28_position.vz - worldmapRadarPosition3.vz;
            computeWormapSpritePosition(&temp);
            gWorldmapState->m0[i].m4C->m0_position.vx = temp.vx;
            gWorldmapState->m0[i].m4C->m0_position.vy = (s32)(gWorldmapState->m0[i].m28_position.vy) << 4;
            gWorldmapState->m0[i].m4C->m0_position.vz = temp.vz;
        }
    }

    SetRotMatrix(&worldmapMainMatrix2);
    SetTransMatrix(&worldmapMainMatrix2);

    std::array<s32, 0x40> depths;
    for (int i = 0; i < 0x40; i++) {
        sWorldmapStateEntry* pEntity = &gWorldmapState->m0[i];
        if ((pEntity->m24 == 0) && pEntity->m4C) {
            SVECTOR tempPosition;
            tempPosition.vx = pEntity->m4C->m0_spriteActorCore.m0_position.vx.getIntegerPart();
            tempPosition.vy = pEntity->m4C->m0_spriteActorCore.m0_position.vy.getIntegerPart();
            tempPosition.vz = pEntity->m4C->m0_spriteActorCore.m0_position.vz.getIntegerPart();
            gte_ldv0(&tempPosition);
            gte_rtps();
            //gte_stsz(&depths[i]);
            depths[i] = 0;
            MissingCode();
        }
    }

    setCurrentRenderingMatrix(&worldmapMainMatrix2);

    for (int i = 0; i < 0x40; i++) {
        sWorldmapStateEntry* pEntity = &gWorldmapState->m0[i];
        if ((pEntity->m24 == 0) && pEntity->m4C) {
            renderSpriteActor(pEntity->m4C, &pCurrentWorldmapRenderingStruct->m70_OT[depths[i] >> 4]);
            
            s32 iVar4 = pEntity->m48;
            s32 iVar2 = pEntity->m5C;
            s32 iVar7 = iVar4 - iVar2;
            s32 iVar1;
            if (iVar7 < 0) {
                iVar7 = iVar7 + 0x1000;
            }
            if (iVar7 < 0x801) {
                iVar1 = iVar4;
                if (0xff < iVar7) {
                    iVar1 = iVar2 + 0x100;
                }
            }
            else {
                iVar1 = iVar2 + -0x100;
                if (-0x100 < iVar7 + -0x1000) {
                    iVar1 = iVar4;
                }
            }
            pEntity->m5C = iVar1;
            setSpriteActorAngle(pEntity->m4C, (pEntity->m5C - worldmapRotation.vy) - 0x400U & 0xfff);
            OP_INIT_ENTITY_SCRIPT_sub0Sub9(&pEntity->m4C->m0_spriteActorCore);
        }
    }
}

MATRIX worldMapIdentityMatrix = {
    {{
        {{0x1000, 0x0, 0x0}},
        {{0x0, 0x1000, 0x0}},
        {{0x0, 0x0, 0x1000}},
    }},
    {0,0,0}
};

void setupWorldmapMatrix1(SVECTOR* param_1) {
    MATRIX worldmapRotationmatrixX = worldMapIdentityMatrix;
    MATRIX worldmapRotationmatrixY = worldMapIdentityMatrix;
    MATRIX worldmapRotationmatrixZ = worldMapIdentityMatrix;
    RotMatrixX(-(int)worldmapRotation.vx, &worldmapRotationmatrixX);
    RotMatrixY(-(int)worldmapRotation.vy, &worldmapRotationmatrixY);
    RotMatrixZ(-(int)worldmapRotation.vz, &worldmapRotationmatrixZ);

    {
        MATRIX temp;
        MulMatrix0(&worldmapRotationmatrixX, &worldmapRotationmatrixY, &temp);
        MulMatrix0(&worldmapRotationmatrixZ, &temp, &worldmapMainMatrix2);
    }

    {
        SVECTOR temp;
        temp.vx = -param_1->vx;
        temp.vy = -param_1->vy;
        temp.vz = -param_1->vz;

        worldmapRotationmatrixX = worldmapMainMatrix2;

        FP_VEC4 temp2;
        ApplyMatrix(&worldmapRotationmatrixX, &temp, &temp2);
        TransMatrix(&worldmapMainMatrix2, &temp2);
    }
}

s32 worldmap_taskGround_update(s32 index) {
    if (worldmapMatrixMode == 0) {
        setupWorldmapMatrix1(&worldmapCameraVector.m0);
    }
    else {
        //setupWorldmapMatrix2(&worldmapCameraVector);
        assert(0);
    }
    MissingCode();
    drawWorlmapPolyFT4Array1();
    worldmapDrawSpriteActors();
    MissingCode();

    setWorldmapGridUpdateMask(&worldmapGridInputPosition);
    if (worldmapGridUpdateMask != 0) {
        updateWorldmapGrids(&worldmapRadarPosition3);
        streamWorldmap0();
        streamWorldmap1();
    }
    worldmapGroundPrepareRenderingTable(&worldmapRadarPosition3);
    drawWorldmapGround(pCurrentWorldmapRenderingStruct->m70_OT, pCurrentWorldmapRenderingStruct->m74_polyFT3.begin(), &worldmapRadarPosition3);
    worldmapWaterWave = worldmapWaterWave + 0x40;
    MissingCode();

    if (gameState.m1842_disableWorldmapMinimap == 0) {
        worldmapRenderMinimap();
    }
    return 1;
}

