#include "noahLib.h"
#include "field/fieldGraphicObject.h"
#include "worldmap.h"
#include "kernel/gameState.h"

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

    gWorldmapState->m0[param_1].m40 = 0;
    gWorldmapState->m0[param_1].m3C = 0;
    gWorldmapState->m0[param_1].m38 = 0;
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

s32 worldmapMode0_taskPlayer_update(int param_1)
{
    MissingCode();
    return 1;
}