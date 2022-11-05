#include "noahLib.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "field/fieldGraphicObject.h"
#include "kernel/trigo.h"
#include "worldmapExit.h"
#include "worldmapDynamicCollisions.h"
#include "worldmapParticles.h"
#include "worldmapFollowLeader.h"

std::array<s16, 4> SHORT_ARRAY_worldmap__8009b18c = { {0x100, 0x100, 0x100, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b194 = { {0x1FD, 0x1FC, 0x1FB, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b19c = { {0x140, 0x160, 0x280, 0} };
std::array<s16, 4> SHORT_ARRAY_worldmap__8009b1a4 = { {0x140, 0x140, 0x100, 0x3C00} };

extern std::array<s16, 6> SHORT_ARRAY_worldmap__8009b180;

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
        case 0:
        case 2:
        case 4:
        case 6:
            Hack("check that the party member id is not negative to avoid getting out of bound");
            if (gameState.m1D34_currentParty[param_2] == 0xFF || gameState.m26C_party[gameState.m1D34_currentParty[param_2]].m78_partyData_gearNum == -1) {
                return 3;
            }
            else {
                createGearSpriteActor(param_1, param_2);
            }
            param_1->m24 = 1;
            (param_1->m28_position).vy = 0;
            (param_1->m28_position).vz = 0;
            (param_1->m28_position).vx = 0;
            break;
        case 1:
            if (gameState.m26C_party[gameState.m1D34_currentParty[param_2]].m78_partyData_gearNum == -1) {
                return 3;
            }
            else {
                createGearSpriteActor(param_1, param_2);
            }
            param_1->m24 = 1;
            (param_1->m28_position).vy = 0;
            (param_1->m28_position).vz = 0;
            (param_1->m28_position).vx = 0;
            break;
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
    pEntry->m48 = gameState.m1820_worldmapPosition[3];
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
                worldmapFollowLeaderDataIndex = 0;
                for (int i = 0; i < 32; i++) {
                    worldmapFollowLeaderData[i].m0_position = pEntry->m28_position;
                    worldmapFollowLeaderData[i].m10 = pEntry->m48;
                }
            }
        }
        else if (iVar3 < 8) {
            pEntry->m20 = 2;
            pEntry->m24 = 1;
        }
    }
    gameState.m1820_worldmapPosition[3] = pEntry->m48;
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

int worldmapUpdatePlayerGearsControls(sWorldmapStateEntry* param_1)
{
    short sVar1;
    ushort uVar2;
    int iVar3;

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
        break;
    }
    if ((worldmapInput1_0 & 0xf000) != 0) {
        iVar3 = getAngleCos((int)param_1->m48);
        sVar1 = param_1->m48;
        (param_1->m38_step).vx = iVar3;
        iVar3 = getAngleSin((int)sVar1);
        (param_1->m38_step).vz = -iVar3;
    }
    if (((worldmapInput2_0 & 0x20) == 0) || ((iVar3 = 3, adjustLocationAfterCollisionVar0 == '\0' && (iVar3 = 1, worldmapExitVar0 == -1)))) {
        if (((worldmapInput2_0 & 0x10) != 0) && ((worldmapExitVar1 == -1 && (worldmapExitVar0 == -1)))) {
            worldmapExitVar2 = 1;
        }
        //FUN_worldmap__80090a18();
        MissingCode();
        iVar3 = 0;
    }
    return iVar3;
}

int checkWorldmapPosition(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, int param_5);
void processWorldmapDynamicCollisions(VECTOR* param_1, int param_2, int param_3, u8* param_4, u8* param_5);
ushort getWorldmapGroundType(VECTOR* param_1);
int checkWorldmapPositionSub1_0_1(int param_1, int param_2);

s32 worldmapMode0_taskPlayerGear_update(int param_1) {

    s32 iVar11 = worldmapNumActivePartyMembers;
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];
    s32 sVar7 = pEntry->m4;
    if (sVar7 == 4) {
        pEntry->m20 = 0x10;
        pEntry->m4 = 0;
        pEntry->m58 = 1;
        gameState.m22B1_isOnGear[0] = 1;
    }
    else if (sVar7 < 5) {
        if (sVar7 == 3) {
            pEntry->m4 = 0;
            pEntry->m20 = 0x30;
        }
    }
    else if (sVar7 == 7) {
        pEntry->m4 = 0;
        pEntry->m58++;
        if (iVar11 == pEntry->m58) {
            pEntry->m20 = 1;
            worldMapGearMode = 2;
            currentWorldmapDynamicCollisionSlot = 0;
        }
    }
    else {
        if (sVar7 == 8) {
            pEntry->m4 = 0;
            pEntry->m20 = 0x18;
        }
    }

    switch (pEntry->m20) {
    case 0:
    case 1: // normal mode
        if (gameState.m22B1_isOnGear[0] != 1) {
            if ((pEntry->m4C->m0_spriteActorCore.mAC >> 24) != '\x03') {
                spriteActorSetPlayingAnimation(pEntry->m4C, 3);
                clearWorldmapParticles(0x2c);
            }
            break;
        }
        iVar11 = worldmapUpdatePlayerGearsControls(pEntry);
        if (iVar11 == 3) {
            pEntry->m20 = 8;
        }
        else if (iVar11 < 4) {
            if (iVar11 == 1) {
                continueWorldmapLoop = 0;
                exitWorldMapMode = 0;
            }
            else {
            LAB_worldmap__8008ca04:
                if (((pEntry->m38_step).vx == 0 && (pEntry->m38_step).vy == 0) && (pEntry->m38_step).vz == 0) {
                    if (((pEntry->m4C->m0_spriteActorCore).mAC >> 24) != '\0') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 0);
                        clearWorldmapParticles(0x2c);
                    }
                }
                else {
                    if (((pEntry->m4C->m0_spriteActorCore).mAC >> 24) != '\x01') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 1);
                    }
                    std::array<short, 1> temp;
                    spawnParticlesForTerrainType3(0x2c, pEntry, temp);
                }
                VECTOR DAT_1f800090;
                iVar11 = checkWorldmapPosition(&pEntry->m28_position, &pEntry->m38_step, &DAT_1f800090, (int)pEntry->m4A << 0xc, worldMapGearMode);
                if (iVar11 == 0) {
                    pEntry->m38_step = DAT_1f800090;
                    iVar11 = checkWorldmapPosition(&pEntry->m28_position, &pEntry->m38_step, &DAT_1f800090, (int)pEntry->m4A << 0xc, worldMapGearMode);
                    if (iVar11 == 0) {
                        (pEntry->m38_step).vz = 0;
                        (pEntry->m38_step).vx = 0;
                    }
                }
                if (iVar11 == 1) {
                    processWorldmapDynamicCollisions(&DAT_1f800090, 0x18, 0x30, &adjustLocationAfterCollisionVar0, &targetVehicleEntityIndex);
                    iVar11 = (uint)adjustLocationAfterCollisionVar0;
                    if (targetVehicleEntityIndex == 7) {
                        iVar11 = (adjustLocationAfterCollisionVar0 + 3);
                    }
                    if (SHORT_ARRAY_worldmap__8009b180[iVar11] != 0) {
                        (pEntry->m28_position) = DAT_1f800090;
                        if (((pEntry->m38_step).vx | (pEntry->m38_step).vz) != 0) {
                            worldmapFollowLeaderDataIndex = (worldmapFollowLeaderDataIndex + 1) & 0x1F;
                            worldmapFollowLeaderData[worldmapFollowLeaderDataIndex].m0_position = pEntry->m28_position;
                            worldmapFollowLeaderData[worldmapFollowLeaderDataIndex].m10 = pEntry->m48;
                            followLeaderPostProcess();
                        }
                    }
                }
                else {
                    processWorldmapDynamicCollisions(&pEntry->m28_position, 0x18, 0x30, &adjustLocationAfterCollisionVar0, &targetVehicleEntityIndex);
                }
                setupWorldmapExits(&pEntry->m28_position, 1);
                (pEntry->m38_step).vz = 0;
                (pEntry->m38_step).vy = 0;
                (pEntry->m38_step).vx = 0;
                worldmapRadarPosition.vx = (pEntry->m28_position).vx;
                worldmapRadarPosition.vy = (pEntry->m28_position).vy;
                worldmapRadarPosition.vz = (pEntry->m28_position).vz;
                worldmapRadarPosition.pad = (pEntry->m28_position).pad;
                worldmapVar_8009d52c = pEntry->m48;
            }
        }
        else {
            if (iVar11 != 4) goto LAB_worldmap__8008ca04;
            sVar7 = getWorldmapGroundType(&pEntry->m28_position);
            sVar7 = checkWorldmapPositionSub1_0_1(1, (int)sVar7);
            if (sVar7 != 0) {
                pEntry->m20 = 0x20;
            }
        }
        currentWorldmapDynamicCollisionSlot = 0;
        break;
    case 2: // idle/invisible
        (pEntry->m28_position).vx = gWorldmapState->m0[7].m28_position.vx;
        (pEntry->m28_position).vz = gWorldmapState->m0[7].m28_position.vz;
        pEntry->m48 = gWorldmapState->m0[7].m48;
        break;
    case 0x8:
        if (gameState.m1D34_currentParty[1] != 0xff) {
            if (gameState.m22B1_isOnGear[1] == 1) {
                iVar11 = changeWorldmapEntityState(5, 1);
                if (iVar11 != 0) {
                    sVar7 = pEntry->m20;
                    pEntry[1].m6 = (ushort)targetVehicleEntityIndex;
                    pEntry->m20 = sVar7 + 1;
                }
                break;
            }
            changeWorldmapEntityState(2, 1);
            gWorldmapState->m0[2].m6 = (ushort)targetVehicleEntityIndex;
            changeWorldmapEntityState(5, 8);
        }
        sVar7 = pEntry->m20;
        pEntry->m20 = sVar7 + 1;
        break;
    case 0x9:
        if (gameState.m1D34_currentParty[2] != 0xff) {
            if (gameState.m22B1_isOnGear[2] == 1) {
                iVar11 = changeWorldmapEntityState(6, 1);
                if (iVar11 != 0) {
                    sVar7 = pEntry->m20;
                    pEntry[2].m6 = (ushort)targetVehicleEntityIndex;
                    pEntry->m20 = sVar7 + 1;
                }
                break;
            }
            changeWorldmapEntityState(3, 1);
            gWorldmapState->m0[3].m6 = (ushort)targetVehicleEntityIndex;
            changeWorldmapEntityState(6, 8);
        }
        break;
    case 0xA:
        selectYggdrasilExitDirection(&pEntry->m28_position, &gWorldmapState->m0[targetVehicleEntityIndex].m28_position, &pEntry->m38_step, &pEntry->m48);
        pEntry->m50 = gWorldmapState->m0[targetVehicleEntityIndex].m28_position.vx >> 0xc;
        pEntry->m54 = gWorldmapState->m0[targetVehicleEntityIndex].m28_position.vz >> 0xc;
        spriteActorSetPlayingAnimation(pEntry->m4C, 1);
        pEntry->m20 = pEntry->m20 + 1;
        break;
    case 0xB:
        if (stepGearAwayFromYggdrasil(pEntry) == 3) {
            pEntry->m20 = pEntry->m20 + 1;
        }
        worldmapRadarPosition.vx = (pEntry->m28_position).vx;
        worldmapRadarPosition.vy = (pEntry->m28_position).vy;
        worldmapRadarPosition.vz = (pEntry->m28_position).vz;
        worldmapRadarPosition.pad = (pEntry->m28_position).pad;
        worldmapVar_8009d52c = pEntry->m48;
        {
            std::array<short, 1> temp;
            spawnParticlesForTerrainType3(0x2c, pEntry, temp);
        }
        break;
    case 0xC: // signal Yggdrasil that one more gear boarded
        if (changeWorldmapEntityState(targetVehicleEntityIndex, 4) != 0) {
            pEntry->m24 = 1;
            pEntry->m20 = 2;
            clearWorldmapParticles(0x2c);
        }
        break;
    case 0x30: // start exit yggdrasil
        getGamestate182C(&pEntry->m28_position);
        pEntry->m24 = 0;
        pEntry->m48 = (short)gWorldmapState->m0[7].m78;
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
        worldmapFollowLeaderDataIndex = 0;
        for (int i = 0; i < 32; i++) {
            worldmapFollowLeaderData[i].m0_position = pEntry->m28_position;
            worldmapFollowLeaderData[i].m10 = pEntry->m48;
        }
        pEntry->m20 = 1;
        worldMapGearMode = 2;
        break;
    default:
        assert(0);
    }

    if ((pEntry->m20 != 2) && ((worldMapGearMode == 2 || (gameState.m1D34_currentParty[0] != 7)))) {
        MissingCode();
    }
    gameState.m1820_worldmapPosition[3] = pEntry->m48;
    gameState.m1930_fieldVarsBackup[22] = (ushort)((pEntry->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[23] = (ushort)((pEntry->m28_position).vz >> 0xc);
    return 1;
}