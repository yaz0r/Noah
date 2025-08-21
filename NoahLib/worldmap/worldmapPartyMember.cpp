#include "noahLib.h"
#include "worldmapPartyMember.h"
#include "worldmap.h"
#include "field/fieldGraphicObject.h"
#include "kernel/gameState.h"
#include "worldmapFollowLeader.h"
#include "worldmapParticles.h"

s32 worldmapMode0_task_partyMember1_init(s32  param_1) {
    sWorldmapState* psVar1;
    long y;
    short sVar2;
    sSpriteActor* psVar3;
    int iVar4;
    int iVar5;
    uint uVar6;
    int iVar7;

    psVar1 = gWorldmapState;
    iVar7 = 1;
    if (gameState.m1D34_currentParty[1] == 0xff) {
        gWorldmapState->m0[param_1].m24 = 1;
        iVar7 = 3;
    }
    else {
        psVar3 = createSpriteActor(&worldmapPartySprites[1], 0x110, 0x1e0, 0x150, 0x100, 0x40);
        psVar1->m0[param_1].m4C = psVar3;
        spriteActorSetPlayingAnimation(psVar3, 0);
        setGraphicEntityScale(&(psVar1->m0[param_1].m4C)->m0_spriteActorCore, 0x1800);
        psVar3 = psVar1->m0[param_1].m4C;
        (psVar3->m0_spriteActorCore).m3C = (psVar3->m0_spriteActorCore).m3C & 0xfffffffb;
        psVar1->m0[param_1].m24 = 0;
    }
    psVar1->m0[param_1].m28_position.vx = (uint)(ushort)gameState.m1820_worldmapPosition[0] << 0xc;
    uVar6 = (uint)(ushort)gameState.m1820_worldmapPosition[1];
    iVar5 = psVar1->m0[param_1].m28_position.vx;
    psVar1->m0[param_1].m28_position.vz = uVar6 << 0xc;
    iVar5 = worldmapGetAltitudeFor2dPoint(iVar5, uVar6 << 0xc);
    psVar1->m0[param_1].m28_position.vy = iVar5;
    psVar1->m0[param_1].m38_step.vz = 0;
    psVar1->m0[param_1].m38_step.vy = 0;
    psVar1->m0[param_1].m38_step.vx = 0;
    iVar5 = worldMapGearMode;
    sVar2 = gameState.m1820_worldmapPosition[1];
    psVar1->m0[param_1].m4A = 8;
    psVar1->m0[param_1].m58 = 0xf;
    psVar1->m0[param_1].m48 = sVar2;
    psVar1->m0[param_1].m5C = (int)psVar1->m0[param_1].m48;
    if (false) {
        return iVar7;
    }
    switch (iVar5) {
    case 1:
    case 2:
    case 3:
        if (gameState.m22B1_isOnGear[1] == 0) {
            psVar1->m0[param_1].m28_position.vx = worldmapPosition.vx;
            y = worldmapPosition.vz;
            iVar5 = psVar1->m0[param_1].m28_position.vx;
            psVar1->m0[param_1].m28_position.vz = worldmapPosition.vz;
            iVar4 = worldmapGetAltitudeFor2dPoint(iVar5, y);
            iVar5 = worldmapGamestate1824;
            psVar1->m0[param_1].m28_position.vy = iVar4;
            psVar1->m0[param_1].m48 = (short)iVar5;
            return iVar7;
        }
        psVar1->m0[param_1].m20 = 1;
        psVar1->m0[param_1].m24 = 1;
        return iVar7;
    case 4:
    case 5:
    case 6:
        sVar2 = 3;
        break;
    case 7:
        sVar2 = 2;
        break;
    default:
        return iVar7;
    }
    psVar1->m0[param_1].m20 = sVar2;
    psVar1->m0[param_1].m24 = 1;
    if (gameState.m1D34_currentParty[1] != 0xff) {
        gameState.m22B1_isOnGear[1] = 1;
    }
    return iVar7;
}

s32 worldmapMode0_task_partyMember2_init(s32 param_1) {
    sWorldmapState* psVar1;
    long y;
    short sVar2;
    sSpriteActor* psVar3;
    int iVar4;
    int iVar5;
    uint uVar6;
    int uVar7;

    psVar1 = gWorldmapState;
    uVar7 = 1;
    if (gameState.m1D34_currentParty[2] == 0xff) {
        gWorldmapState->m0[param_1].m24 = 1;
        uVar7 = 3;
    }
    else {
        psVar3 = createSpriteActor(&worldmapPartySprites[2], 0x120, 0x1e0, 0x160, 0x100, 0x40);
        psVar1->m0[param_1].m4C = psVar3;
        spriteActorSetPlayingAnimation(psVar3, 0);
        setGraphicEntityScale(&(psVar1->m0[param_1].m4C)->m0_spriteActorCore, 0x1800);
        psVar3 = psVar1->m0[param_1].m4C;
        (psVar3->m0_spriteActorCore).m3C = (psVar3->m0_spriteActorCore).m3C & 0xfffffffb;
        psVar1->m0[param_1].m24 = 0;
    }
    psVar1->m0[param_1].m28_position.vx = (uint)(ushort)gameState.m1820_worldmapPosition[0] << 0xc;
    uVar6 = (uint)(ushort)gameState.m1820_worldmapPosition[1];
    iVar5 = psVar1->m0[param_1].m28_position.vx;
    psVar1->m0[param_1].m28_position.vz = uVar6 << 0xc;
    iVar5 = worldmapGetAltitudeFor2dPoint(iVar5, uVar6 << 0xc);
    psVar1->m0[param_1].m28_position.vy = iVar5;
    psVar1->m0[param_1].m38_step.vz = 0;
    psVar1->m0[param_1].m38_step.vy = 0;
    psVar1->m0[param_1].m38_step.vx = 0;
    iVar5 = worldMapGearMode;
    sVar2 = gameState.m1820_worldmapPosition[2];
    psVar1->m0[param_1].m4A = 8;
    psVar1->m0[param_1].m58 = 0x1e;
    psVar1->m0[param_1].m48 = sVar2;
    psVar1->m0[param_1].m5C = (int)psVar1->m0[param_1].m48;
    if (false) {
        return uVar7;
    }
    switch (iVar5) {
    case 1:
    case 2:
    case 3:
        if (gameState.m22B1_isOnGear[2] == 0) {
            psVar1->m0[param_1].m28_position.vx = worldmapPosition.vx;
            y = worldmapPosition.vz;
            iVar5 = psVar1->m0[param_1].m28_position.vx;
            psVar1->m0[param_1].m28_position.vz = worldmapPosition.vz;
            iVar4 = worldmapGetAltitudeFor2dPoint(iVar5, y);
            iVar5 = worldmapGamestate1824;
            psVar1->m0[param_1].m28_position.vy = iVar4;
            psVar1->m0[param_1].m48 = (short)iVar5;
            return uVar7;
        }
        psVar1->m0[param_1].m20 = 1;
        psVar1->m0[param_1].m24 = 1;
        return uVar7;
    case 4:
    case 5:
    case 6:
        sVar2 = 3;
        break;
    case 7:
        sVar2 = 2;
        break;
    default:
        return uVar7;
    }
    psVar1->m0[param_1].m20 = sVar2;
    psVar1->m0[param_1].m24 = 1;
    if (gameState.m1D34_currentParty[2] != 0xff) {
        gameState.m22B1_isOnGear[2] = 1;
    }
    return uVar7;
}

s32 worldmapMode0_task_partyMember_update(s32 param_1) {


    sWorldmapState* psVar3 = gWorldmapState;
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];
    short sVar1 = pEntry->m4;
    short sVar4;

    sWorldmapStateEntry* pParent;

    switch (pEntry->m4) {
    case 0:
        break;
    case 1:
        pEntry->m4 = 0;
        pEntry->m20 = 8;
        pParent = &gWorldmapState->m0[pEntry->m6];
        break;
    case 2:
        pEntry->m4 = 0;
        pEntry->m20 = 0x28;
        break;
    case 3:
        pEntry->m4 = 0;
        pEntry->m20 = 1;
        break;
    default:
        assert(0);
        break;
    }

    switch (pEntry->m20) {
    case 0:
    case 1:
        if (gameState.m22B1_isOnGear[param_1 + -1] == 0) {
            s32 uVar7 = (int)worldmapFollowLeaderDataIndex - pEntry->m58 & 0x1f;
            if (((pEntry->m28_position).vx == worldmapFollowLeaderData[uVar7].m0_position.vx && (pEntry->m28_position).vy == worldmapFollowLeaderData[uVar7].m0_position.vy) &&
                (pEntry->m28_position).vz == worldmapFollowLeaderData[uVar7].m0_position.vz) {
                if (((pEntry->m4C->m0_spriteActorCore).mAC.mx18) != '\x00') {
                    spriteActorSetPlayingAnimation(pEntry->m4C, 0);
                    clearWorldmapParticles(param_1 + 0x2e);
                }
            }
            else {
                if (((pEntry->m4C->m0_spriteActorCore).mAC.mx18) != '\x01') {
                    spriteActorSetPlayingAnimation(pEntry->m4C, 1);
                }
                std::array<short, 1> temp;
                spawnParticlesForTerrainType3(param_1 + 0x2e, pEntry, temp);
            }
            (pEntry->m28_position).vx = worldmapFollowLeaderData[uVar7].m0_position.vx;
            (pEntry->m28_position).vy = worldmapFollowLeaderData[uVar7].m0_position.vy;
            (pEntry->m28_position).vz = worldmapFollowLeaderData[uVar7].m0_position.vz;
            sVar1 = worldmapFollowLeaderData[uVar7].m10;
            pEntry->m24 = 0;
            pEntry->m48 = sVar1;
        }
        else {
            (pEntry->m28_position).vx = gWorldmapState->m0[param_1 + 3].m28_position.vx;
            (pEntry->m28_position).vy = psVar3->m0[param_1 + 3].m28_position.vy;
            (pEntry->m28_position).vz = psVar3->m0[param_1 + 3].m28_position.vz;
            sVar1 = psVar3->m0[param_1 + 3].m48;
            pEntry->m24 = 1;
            pEntry->m48 = sVar1;
            clearWorldmapParticles(param_1 + 0x2e);
        }
        break;
    case 2: // in yggdrasil
        (pEntry->m28_position).vx = gWorldmapState->m0[7].m28_position.vx;
        (pEntry->m28_position).vy = psVar3->m0[7].m28_position.vy;
        (pEntry->m28_position).vz = psVar3->m0[7].m28_position.vz;
        pEntry->m48 = psVar3->m0[7].m48;
        break;
    default:
        assert(0);
    }

    if (pEntry->m24 == 0) {
        //addShadow(0, &psVar9->m28_position);
        Noah_MissingCode("addShadow for party memeber");
    }

    return 1;
}