#include "noahLib.h"
#include "field/fieldGraphicObject.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"

s32 worldmapGetAltitudeFor2dPoint(s32, s32) {
    return 0;
}

s32 worldmapMode0_taskPlayer_init(int param_1)
{
    sSpriteActor* pSprite = createSpriteActor(worldmapPartySprites[0], 0x100, 0x1e0, 0x140, 0x100, 0x40);
    gWorldmapState->m0[param_1].m4C = pSprite;
    spriteActorSetPlayingAnimation(pSprite, 0);
    setGraphicEntityScale(&pSprite->m0_spriteActorCore, 0x1800);

    (pSprite->m0_spriteActorCore).m3C = (pSprite->m0_spriteActorCore).m3C & 0xfffffffb;
    gWorldmapState->m0[param_1].m24 = 0;

    gWorldmapState->m0[param_1].m28_position.vx = gameState.m1930_fieldVarsBackup[0] << 0xc;
    gWorldmapState->m0[param_1].m28_position.vz = gameState.m1930_fieldVarsBackup[1] << 0xc;
    gWorldmapState->m0[param_1].m28_position.vy = worldmapGetAltitudeFor2dPoint(gameState.m1930_fieldVarsBackup[0], gameState.m1930_fieldVarsBackup[1] << 0xc);

    gWorldmapState->m0[param_1].m38_step.vz = 0;
    gWorldmapState->m0[param_1].m38_step.vy = 0;
    gWorldmapState->m0[param_1].m38_step.vx = 0;
    gWorldmapState->m0[param_1].m4A = 8;
    gWorldmapState->m0[param_1].m48 = worldmapGamestate1824;
    gWorldmapState->m0[param_1].m5C = gWorldmapState->m0[param_1].m48;

    switch (worldMapGearMode) {
    case 1:
    case 2:
    case 3:
        if (gameState.m22B1_isOnGear[0] == 0) {

            gWorldmapState->m0[param_1].m28_position.vx = worldmapPosition.vx;
            gWorldmapState->m0[param_1].m28_position.vz = worldmapGetAltitudeFor2dPoint(worldmapPosition.vx, worldmapPosition.vz);
            gWorldmapState->m0[param_1].m28_position.vz = worldmapPosition.vz;
            gWorldmapState->m0[param_1].m48 = worldmapGamestate1824;

            worldmapRadarPosition.vx = gWorldmapState->m0[param_1].m28_position.vx;
            worldmapRadarPosition.vy = gWorldmapState->m0[param_1].m28_position.vy;
            worldmapRadarPosition.vz = gWorldmapState->m0[param_1].m28_position.vz;
            worldmapRadarPosition.pad = gWorldmapState->m0[param_1].m28_position.pad;
            worldmapVar_8009d52c = gWorldmapState->m0[param_1].m48;
        }
        else {
            gWorldmapState->m0[param_1].m20 = 1;
            gWorldmapState->m0[param_1].m24 = 1;
        }
        break;
    default:
        assert(0);
    }

    MissingCode("Follow leader init");

    gameState.m1820_worldmapPosition[0] = (short)(gWorldmapState->m0[param_1].m28_position.vx >> 0xc);
    gameState.m1820_worldmapPosition[1] = (short)(gWorldmapState->m0[param_1].m28_position.vz >> 0xc);
    gameState.m1824 = gWorldmapState->m0[param_1].m48;

    return 1;
}

s32 worldmapUpdatePlayerControls(sWorldmapStateEntry* param_1) {
    switch (worldmapInput1_0 >> 0xc) {
    case 1:
        param_1->m48 = worldmapRotation.vy;
        break;
    case 2:
        param_1->m48 = (worldmapRotation.vy + 0x400) & 0xFFF;
        break;
    case 3:
        param_1->m48 = (worldmapRotation.vy + 0x200) & 0xFFF;
        break;
    case 4:
        param_1->m48 = (worldmapRotation.vy + 0x800) & 0xFFF;
        break;
    case 6:
        param_1->m48 = (worldmapRotation.vy + 0x600) & 0xFFF;
        break;
    case 8:
        param_1->m48 = (worldmapRotation.vy - 0x400) & 0xFFF;
        break;
    case 9:
        param_1->m48 = (worldmapRotation.vy - 0x200) & 0xFFF;
        break;
    case 0xc:
        param_1->m48 = (worldmapRotation.vy - 0x600) & 0xFFF;
    default:
        break;
    }
    if ((worldmapInput1_0 & 0xf000) != 0) {
        param_1->m38_step.vx = getAngleCos((int)param_1->m48);
        param_1->m38_step.vz = -getAngleSin((int)param_1->m48);
    }

    MissingCode();

    return 0;
}

void checkWorldmapPositionSub0(VECTOR* param_1)
{
    if (worldmapProjVar0 * 0x800000 <= param_1->vx) {
        param_1->vx = param_1->vx + worldmapProjVar0 * -0x800000;
    }
    if (param_1->vx < 0) {
        param_1->vx = param_1->vx + worldmapProjVar0 * 0x800000;
    }
    if (worldmapProjVar1 * 0x800000 <= param_1->vz) {
        param_1->vz = param_1->vz + worldmapProjVar1 * -0x800000;
    }
    if (param_1->vz < 0) {
        param_1->vz = param_1->vz + worldmapProjVar1 * 0x800000;
    }
    return;
}

void checkWorldmapPosition(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, int param_5) {
    VECTOR VECTOR_1f800060;

    VECTOR_1f800060.vx = position->vx + (step->vx * stepScale >> 0xc);
    VECTOR_1f800060.vz = position->vz + (step->vz * stepScale >> 0xc);
    checkWorldmapPositionSub0(&VECTOR_1f800060);
    VECTOR_1f800060.vy = worldmapGetAltitudeFor2dPoint(VECTOR_1f800060.vx, VECTOR_1f800060.vz) + -0x4000;

    output->vx = position->vx + (step->vx * stepScale >> 0xc);
    output->vz = position->vz + (step->vz * stepScale >> 0xc);
    checkWorldmapPositionSub0(output);
    output->vy = worldmapGetAltitudeFor2dPoint(output->vx, output->vz) - 0x4000;

    MissingCode();
}

s32 worldmapMode0_taskPlayer_update(int param_1)
{
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];

    switch (pEntry->m4) {
    case 0:
        break;
    case 2:
        pEntry->m4 = 0;
        pEntry->m20 = 0x28;
        break;
    case 3:
        pEntry->m4 = 0;
        pEntry->m20 = 1;
        break;
    case 6:
        assert(0);
        break;
    default:
        assert(0);
        break;
    }

    switch (pEntry->m20) {
    case 0:
    case 1:
        if (gameState.m22B1_isOnGear[0] == 0) {
            switch (worldmapUpdatePlayerControls(pEntry)) {
            case 1:
                pEntry->m20 = 0x40;
                continueWorldmapLoop = 0;
                exitWorldMapMode = 0;
                break;
            case 2:
            case 4:
            case 5:
                break;
            default:
                if ((pEntry->m38_step.vx == 0 && pEntry->m38_step.vy == 0) && pEntry->m38_step.vz == 0) {
                    if ((pEntry->m4C->m0_spriteActorCore.mAC >> 24) != '\0') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 0);
                        MissingCode();
                    }
                }
                else {
                    if ((pEntry->m4C->m0_spriteActorCore.mAC >> 24) != '\x01') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 1);
                    }
                    MissingCode();
                }

                s32 oldGearMode = worldMapGearMode;

                VECTOR DAT_1f800090;
                checkWorldmapPosition(&pEntry->m28_position, &pEntry->m38_step, &DAT_1f800090, (int)pEntry->m4A << 0xc, worldMapGearMode);

                if ((oldGearMode & 0xffff) == 0) {
                    pEntry->m38_step = DAT_1f800090;
                    MissingCode();
                }
                if ((oldGearMode & 0xffff) == 1) {
                    MissingCode();
                    pEntry->m28_position = DAT_1f800090;
                    if (((pEntry->m38_step).vx | (pEntry->m38_step).vz) != 0) {
                        MissingCode("Update worldamp follow data");
                    }
                }
                else {
                    MissingCode();
                }

                MissingCode();

                (pEntry->m38_step).vz = 0;
                (pEntry->m38_step).vy = 0;
                (pEntry->m38_step).vx = 0;
                worldmapRadarPosition.vx = (pEntry->m28_position).vx;
                worldmapRadarPosition.vy = (pEntry->m28_position).vy;
                worldmapRadarPosition.vz = (pEntry->m28_position).vz;
                worldmapRadarPosition.pad = (pEntry->m28_position).pad;
                worldmapVar_8009d52c = pEntry->m48;
                break;
            }
            MissingCode();
            pEntry->m24 = 0;
        }
        else {
            assert(0);
        }
        break;
    default:
        assert(0);
    }

    gameState.m1820_worldmapPosition[0] = (short)((pEntry->m28_position).vx >> 0xc);
    gameState.m1820_worldmapPosition[1] = (short)((pEntry->m28_position).vz >> 0xc);
    gameState.m1824 = pEntry->m48;
    if (pEntry->m24 == 0) {
        MissingCode();
    }
    return 1;
}