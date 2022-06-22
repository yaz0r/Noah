#include "noahLib.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "field/fieldGraphicObject.h"
#include "kernel/trigo.h"

std::array<s16, 4> SHORT_ARRAY_worldmap__8009b18c = { {0x100, 0x100, 0x100, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b194 = { {0x1FD, 0x1FC, 0x1FB, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b19c = { {0x140, 0x160, 0x280, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b1a4 = { {0x140, 0x140, 0x100, 0x3C00} };

void createGearSpriteActor(sWorldmapStateEntry* param_1, int param_2)
{
    sSpriteActor* psVar1;
    uint uVar2;

    psVar1 = createSpriteActor(worldmapPartyGearSprites[param_2], SHORT_ARRAY_worldmap__8009b18c[param_2], SHORT_ARRAY_worldmap__8009b194[param_2], SHORT_ARRAY_worldmap__8009b19c[param_2],
        SHORT_ARRAY_worldmap__8009b1a4[param_2], 0x40);
    param_1->m4C = psVar1;
    uVar2 = 3;
    if (gameState.m22B1_isOnGear[param_2] == 1) {
        uVar2 = 0;
    }
    spriteActorSetPlayingAnimation(psVar1, uVar2);
    setGraphicEntityScale(&param_1->m4C->m0_spriteActorCore, 0x2000);
    (param_1->m4C->m0_spriteActorCore).m3C = (param_1->m4C->m0_spriteActorCore).m3C & 0xfffffffb;
    return;
}

int createWorldmapGear(sWorldmapStateEntry* param_1, int param_2)
{
    int iVar1;
    uint uVar2;

    uVar2 = (uint)(gameState.m1D34_currentParty[param_2] != 0xff);
    if (0x3ff < (gameState.m1930_fieldVarsBackup[param_2 * 3 + 0x15] & 0x3fff)) {
        uVar2 = uVar2 | 2;
    }
    if (gameState.m22B1_isOnGear[param_2] == 1) {
        uVar2 = uVar2 | 4;
    }
    if (uVar2 < 8) {
        switch (uVar2) {
        case 7:
            createGearSpriteActor(param_1, param_2);
            param_1->m24 = 0;
            (param_1->m28_position).vx = worldmapPosition.vx;
            (param_1->m28_position).vz = worldmapPosition.vz;
            (param_1->m28_position).vy = worldmapGetAltitudeFor2dPoint((param_1->m28_position).vx, worldmapPosition.vz);
            gameState.m1930_fieldVarsBackup[param_2 * 3 + 0x15] = 0x400;
            break;
        default:
            assert(0);
        }
    }
    return 1;
}

s32 worldmapMode0_taskPlayerGear_init(int param_1) {
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];
    s32 returnValue = createWorldmapGear(pEntry, 0);
    (pEntry->m38_step).vz = 0;
    (pEntry->m38_step).vy = 0;
    (pEntry->m38_step).vx = 0;
    pEntry->m48 = gameState.m1826;
    pEntry->m4A = 0xc;
    s32 iVar3 = worldMapGearMode;
    bool bVar1 = 0 < worldMapGearMode;
    pEntry->m5C = (int)pEntry->m48;

    if (bVar1) {
        if (iVar3 < 4) {
            if (gameState.m22B1_isOnGear[0] == 1) {
                pEntry->m20 = 1;
                (pEntry->m28_position).vx = worldmapPosition.vx;
                s32 lVar4 = worldmapPosition.vz;
                (pEntry->m28_position).vz = worldmapPosition.vz;
                iVar3 = worldmapGetAltitudeFor2dPoint((pEntry->m28_position).vx, lVar4);
                s32 uVar2 = worldmapGamestate1824;
                s32 iVar8 = 0;
                (pEntry->m28_position).vy = iVar3;
                pEntry->m48 = (short)uVar2;
                worldmapRadarPosition.vx = (pEntry->m28_position).vx;
                worldmapRadarPosition.vy = (pEntry->m28_position).vy;
                worldmapRadarPosition.vz = (pEntry->m28_position).vz;
                worldmapRadarPosition.pad = (pEntry->m28_position).pad;
                worldmapVar_8009d52c = pEntry->m48;
                MissingCode("Follow leader");
            }
        }
        else if (iVar3 < 8) {
            pEntry->m20 = 2;
            pEntry->m24 = 1;
        }
    }
    gameState.m1826 = pEntry->m48;
    gameState.m1930_fieldVarsBackup[22] = (ushort)((pEntry->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[23] = (ushort)((pEntry->m28_position).vz >> 0xc);
    return returnValue;
}

void selectYggdrasilExitDirection(VECTOR* param_1, VECTOR* param_2, VECTOR* param_3, short* param_4)
{
    long lVar1;
    int iVar2;
    uint uVar3;

    lVar1 = ratan2(param_2->vz - param_1->vz, param_2->vx - param_1->vx);
    uVar3 = lVar1 + 0x400U & 0xfff;
    *param_4 = (short)uVar3;
    iVar2 = getAngleCos(uVar3);
    param_3->vx = iVar2;
    iVar2 = getAngleSin((int)*param_4);
    param_3->vz = -iVar2;
    return;
}

void checkWorldmapPositionSub0(VECTOR* param_1);
byte stepGearAwayFromYggdrasil(sWorldmapStateEntry* param_1)
{
    int iVar1;
    int iVar2;
    byte bVar3;

    iVar2 = (param_1->m28_position).vx;
    iVar1 = param_1->m50 - (iVar2 >> 0xc);
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    bVar3 = iVar1 < 5;
    if (!(bool)bVar3) {
        iVar1 = (uint)(ushort)param_1->m4A << 0x10;
        (param_1->m28_position).vx = iVar2 + (param_1->m38_step).vx * ((iVar1 >> 0x10) - (iVar1 >> 0x1f) >> 1);
    }
    iVar2 = (param_1->m28_position).vz;
    iVar1 = param_1->m54 - (iVar2 >> 0xc);
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    if (iVar1 < 5) {
        bVar3 = bVar3 | 2;
    }
    else {
        iVar1 = (uint)(ushort)param_1->m4A << 0x10;
        (param_1->m28_position).vz = iVar2 + (param_1->m38_step).vz * ((iVar1 >> 0x10) - (iVar1 >> 0x1f) >> 1);
    }
    checkWorldmapPositionSub0(&param_1->m28_position);
    iVar1 = worldmapGetAltitudeFor2dPoint((param_1->m28_position).vx, (param_1->m28_position).vz);
    (param_1->m28_position).vy = iVar1;
    return bVar3;
}

s32 worldmapMode0_taskPlayerGear_update(int param_1) {

    s32 iVar11 = worldmapNumFilesPending;
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];
    s32 sVar7 = pEntry->m4;
    if (sVar7 == 4) {
        pEntry->m20 = 0x10;
        pEntry->m4 = 0;
        pEntry->m58 = 1;
        gameState.m22B1_isOnGear[0] = 1;
    }
    else if (sVar7 < 5) {
        s32 sVar3 = 0x30;
        if (sVar7 == 3) {
        LAB_worldmap__8008c928:
            pEntry->m4 = 0;
            pEntry->m20 = sVar3;
        }
    }
    else if (sVar7 == 7) {
        pEntry->m4 = 0;
        s32 iVar5 = pEntry->m58 + 1;
        pEntry->m58 = iVar5;
        if (iVar11 == iVar5) {
            pEntry->m20 = 1;
            worldMapGearMode = 2;
            adjustLocationAfterCollisionVar2 = 0;
        }
    }
    else {
        s32 sVar3 = 0x18;
        if (sVar7 == 8) goto LAB_worldmap__8008c928;
    }

    switch (pEntry->m20) {
    case 1: // normal mode
        MissingCode();
        break;
    case 2: // idle/invisible
        (pEntry->m28_position).vx = gWorldmapState->m0[7].m28_position.vx;
        (pEntry->m28_position).vz = gWorldmapState->m0[7].m28_position.vz;
        pEntry->m48 = gWorldmapState->m0[7].m48;
        break;
    case 0x30: // start exit yggdrasil
        getGamestate182C(&pEntry->m28_position);
        pEntry->m24 = 0;
        pEntry->m48 = (short)gWorldmapState->m0[7].m6C.pad;
        {
            VECTOR DAT_1f800000;
            DAT_1f800000.vx = (pEntry->m28_position).vx + getAngleCos(pEntry->m48) * 0x60;
            DAT_1f800000.vz = (pEntry->m28_position).vz + getAngleSin(pEntry->m48) * -0x60;
            selectYggdrasilExitDirection(&pEntry->m28_position, &DAT_1f800000, &pEntry->m38_step, &pEntry->m48);
            pEntry->m50 = DAT_1f800000.vx >> 0xc;
            pEntry->m54 = DAT_1f800000.vz >> 0xc;
        }
        spriteActorSetPlayingAnimation(pEntry->m4C, 1);
        pEntry->m20 = pEntry->m20 + 1;
        break;
    case 0x31: // exit yggdrasil
        iVar11 = stepGearAwayFromYggdrasil(pEntry);
        if (iVar11 == 3) {
            spriteActorSetPlayingAnimation(pEntry->m4C, 0);
            sVar7 = pEntry->m20;
            (pEntry->m38_step).vz = 0;
            (pEntry->m38_step).vy = 0;
            (pEntry->m38_step).vx = 0;
            pEntry->m20 = sVar7 + 1;
        }
        worldmapRadarPosition.vx = (pEntry->m28_position).vx;
        worldmapRadarPosition.vy = (pEntry->m28_position).vy;
        worldmapRadarPosition.vz = (pEntry->m28_position).vz;
        worldmapRadarPosition.pad = (pEntry->m28_position).pad;
        worldmapVar_8009d52c = pEntry->m48;
        break;
    case 0x32: // finish exit yggdrasil
        MissingCode("Follow leader");
        pEntry->m20 = 1;
        worldMapGearMode = 2;
        break;
    default:
        assert(0);
    }

    if ((pEntry->m20 != 2) && ((worldMapGearMode == 2 || (gameState.m1D34_currentParty[0] != 7)))) {
        MissingCode();
    }
    gameState.m1826 = pEntry->m48;
    gameState.m1930_fieldVarsBackup[22] = (ushort)((pEntry->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[23] = (ushort)((pEntry->m28_position).vz >> 0xc);
    return 1;
}