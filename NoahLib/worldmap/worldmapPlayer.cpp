#include "noahLib.h"
#include "field/fieldGraphicObject.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"
#include "worldmapWorldStreaming.h"

u8* getWorldChunkForPosition(int x, int y)
{
    bool bVar1;
    int iVar3;
    int iVar5;
    ushort uVar8;

    int tileY = y >> 0x14;
    int tileX = x >> 0x14;
    if (tileY < 0) {
        tileY = tileY + 7;
    }
    if (tileX < 0) {
        tileX = tileX + 7;
    }

    uint inTileX = x >> 0xc & 0x7ff;
    uint inTileY = y >> 0xc & 0x7ff;
    bVar1 = 0x3ff < inTileX;
    if (bVar1) {
        inTileX = inTileX - 0x400;
    }
    uVar8 = (ushort)bVar1;
    if (0x3ff < inTileY) {
        uVar8 = bVar1 | 2;
        inTileY = inTileY - 0x400;
    }
    iVar3 = (int)inTileY >> 4;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 7;
    }
    iVar5 = (int)inTileX >> 4;
    if (iVar5 < 0) {
        iVar5 = iVar5 + 7;
    }

    int tileId = (((tileY/8) * worldmapSizeX + (tileX/8)) * 0x10000 >> 0xe) / 4;
    return worldmapChunks[tileId] + ((short)uVar8 * 0x144 + (((iVar3 >> 3) * 9 + (iVar5 >> 3)) * 0x10000 >> 0xe));
}

void averageVector(VECTOR* param_1, VECTOR* param_2, VECTOR* param_3)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar2 = param_3->vy;
    iVar1 = -(param_3->vz * (param_1->vz - param_2->vz)) - param_3->vx * (param_1->vx - param_2->vx);
    iVar3 = iVar1 / iVar2;
    if (iVar2 == 0) {
        assert(0);
    }
    if ((iVar2 == -1) && (iVar1 == -0x80000000)) {
        assert(0);
    }
    param_1->vy = iVar3;
    param_1->vy = iVar3 + param_2->vy;
    return;
}


s32 DAT_worldmap__8009b244 = 0xB50;
s32 DAT_worldmap__8009b24c = 0xFFFFF4B0;
s32 DAT_worldmap__8009b254 = 0xB50;
s32 DAT_worldmap__8009b25c = 0xB50;

void interpolateWorldPosition(VECTOR* param_1, int x, int y)
{
    byte bVar1;
    byte* pcVar2;
    uint uVar2;
    uint uVar3;

    VECTOR DAT_1f800000;
    VECTOR VECTOR_1f800010;

    pcVar2 = getWorldChunkForPosition(x, y);
    if (x < 0) {
        x = x + 7;
    }
    uVar2 = x >> 3 & 0xffff;
    if (y < 0) {
        y = y + 7;
    }
    uVar3 = y >> 3 & 0xffff;
    if ((pcVar2[1] & 0x80) == 0) {
        if ((int)((uVar2 - 0x10000) * DAT_worldmap__8009b244 + -uVar3 * DAT_worldmap__8009b24c) < 0) {
            DAT_1f800000.vx = -0x10;
            VECTOR_1f800010.vz = 0;
            DAT_1f800000.vy = (int)(char)pcVar2[0x24] - (int)(char)pcVar2[4];
            bVar1 = *pcVar2;
        }
        else {
            DAT_1f800000.vx = 0;
            VECTOR_1f800010.vz = -0x10;
            DAT_1f800000.vy = (int)(char)pcVar2[0x28] - (int)(char)pcVar2[4];
            bVar1 = pcVar2[0x24];
        }
        VECTOR_1f800010.vx = -0x10;
        DAT_1f800000.vz = -0x10;
        VECTOR_1f800010.vy = (int)(char)bVar1 - (int)(char)pcVar2[4];
    }
    else if ((int)(uVar2 * DAT_worldmap__8009b254 + -uVar3 * DAT_worldmap__8009b25c) < 0) {
        DAT_1f800000.vx = 0x10;
        DAT_1f800000.vz = -0x10;
        VECTOR_1f800010.vx = 0;
        VECTOR_1f800010.vz = -0x10;
        DAT_1f800000.vy = (int)(char)pcVar2[0x28] - (int)(char)*pcVar2;
        VECTOR_1f800010.vy = (int)(char)pcVar2[0x24] - (int)(char)*pcVar2;
    }
    else {
        DAT_1f800000.vx = 0x10;
        VECTOR_1f800010.vx = 0x10;
        DAT_1f800000.vz = 0;
        VECTOR_1f800010.vz = -0x10;
        DAT_1f800000.vy = (int)(char)pcVar2[4] - (int)(char)*pcVar2;
        VECTOR_1f800010.vy = (int)(char)pcVar2[0x28] - (int)(char)*pcVar2;
    }

    VECTOR VECTOR_1f800020;
    OuterProduct0(&VECTOR_1f800010, &DAT_1f800000, &VECTOR_1f800020);
    VectorNormal(&VECTOR_1f800020, param_1);
    return;
}

s32 worldmapGetAltitudeFor2dPoint(s32 x, s32 y) {
    u8 bVar1;
    int iVar2;
    VECTOR local_40;
    VECTOR local_30;
    VECTOR VStack32;

    byte* pcVar2 = getWorldChunkForPosition(x, y);
    if (pcVar2 == nullptr)
        return 0;
    iVar2 = x;
    if (x < 0) {
        iVar2 = x + 7;
    }
    local_40.vx = iVar2 >> 3 & 0xffff;
    iVar2 = y;
    if (y < 0) {
        iVar2 = y + 7;
    }
    local_40.vz = -(iVar2 >> 3 & 0xffffU);
    if ((pcVar2[1] & 0x80) == 0) {
        local_30.vx = 0x10000;
        bVar1 = pcVar2[4];
    }
    else {
        local_30.vx = 0;
        bVar1 = *pcVar2;
    }
    local_30.vz = 0;
    local_30.vy = (int)(char)bVar1 << 0xc;
    interpolateWorldPosition(&VStack32, x, y);
    averageVector(&local_40, &local_30, &VStack32);
    return local_40.vy << 3;
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
    if (worldmapSizeX * 0x800000 <= param_1->vx) {
        param_1->vx = param_1->vx + worldmapSizeX * -0x800000;
    }
    if (param_1->vx < 0) {
        param_1->vx = param_1->vx + worldmapSizeX * 0x800000;
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