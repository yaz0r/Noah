#include "noahLib.h"
#include "worldmapPartyGear.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"
#include "field/fieldGraphicObject.h"
#include "worldmapParticles.h"
#include "worldmapFollowLeader.h"

int createWorldmapGear(sWorldmapStateEntry* param_1, int param_2);
void selectYggdrasilExitDirection(VECTOR* param_1, VECTOR* param_2, VECTOR* param_3, short* param_4);
byte stepGearAwayFromYggdrasil(sWorldmapStateEntry* param_1);

s32 worldmapMode0_task_partyGear1_init(s32 param_1) {
    short sVar1;
    int iVar2;
    long y;
    int iVar3;
    int iVar4;
    sWorldmapStateEntry* psVar5;

    psVar5 = &gWorldmapState->m0[param_1];
    iVar3 = createWorldmapGear(psVar5, 1);
    (psVar5->m38_step).vz = 0;
    (psVar5->m38_step).vy = 0;
    (psVar5->m38_step).vx = 0;
    sVar1 = gameState.m1820_worldmapPosition[4];
    psVar5->m4A = 0xc;
    psVar5->m58 = 0xf;
    iVar2 = worldMapGearMode;
    psVar5->m48 = sVar1;
    psVar5->m5C = (int)psVar5->m48;
    if (0 < iVar2) {
        if (iVar2 < 4) {
            if (gameState.m22B1_isOnGear[1] == 1) {
                psVar5->m20 = 1;
                (psVar5->m28_position).vx = worldmapPosition.vx;
                y = worldmapPosition.vz;
                (psVar5->m28_position).vz = worldmapPosition.vz;
                iVar4 = worldmapGetAltitudeFor2dPoint((psVar5->m28_position).vx, y);
                iVar2 = worldmapGamestate1824;
                (psVar5->m28_position).vy = iVar4;
                psVar5->m48 = (short)iVar2;
            }
        }
        else if (iVar2 < 8) {
            psVar5->m20 = 2;
            psVar5->m24 = 1;
        }
    }
    gameState.m1820_worldmapPosition[4] = psVar5->m48;
    gameState.m1930_fieldVarsBackup[25] = (ushort)((psVar5->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[26] = (ushort)((psVar5->m28_position).vz >> 0xc);
    return iVar3;
}

s32 worldmapMode0_task_partyGear2_init(s32 param_1) {
    short sVar1;
    int iVar2;
    long y;
    int iVar3;
    int iVar4;
    sWorldmapStateEntry* psVar5;

    psVar5 = &gWorldmapState->m0[param_1];
    iVar3 = createWorldmapGear(psVar5, 2);
    (psVar5->m38_step).vz = 0;
    (psVar5->m38_step).vy = 0;
    (psVar5->m38_step).vx = 0;
    sVar1 = gameState.m1820_worldmapPosition[5];
    psVar5->m4A = 0xc;
    psVar5->m58 = 0x1f;
    iVar2 = worldMapGearMode;
    psVar5->m48 = sVar1;
    psVar5->m5C = (int)psVar5->m48;
    if (0 < iVar2) {
        if (iVar2 < 4) {
            if (gameState.m22B1_isOnGear[2] == 1) {
                psVar5->m20 = 1;
                (psVar5->m28_position).vx = worldmapPosition.vx;
                y = worldmapPosition.vz;
                (psVar5->m28_position).vz = worldmapPosition.vz;
                iVar4 = worldmapGetAltitudeFor2dPoint((psVar5->m28_position).vx, y);
                iVar2 = worldmapGamestate1824;
                (psVar5->m28_position).vy = iVar4;
                psVar5->m48 = (short)iVar2;
            }
        }
        else if (iVar2 < 8) {
            psVar5->m20 = 2;
            psVar5->m24 = 1;
        }
    }
    gameState.m1820_worldmapPosition[5] = psVar5->m48;
    gameState.m1930_fieldVarsBackup[28] = (ushort)((psVar5->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[29] = (ushort)((psVar5->m28_position).vz >> 0xc);
    return iVar3;
}

s32 worldmapMode0_task_partyGear_update(s32 param_1) {
    sWorldmapState* psVar1;
    ushort uVar2;
    long lVar3;
    int iVar4;
    short sVar5;
    sSpriteActor* psVar7;
    sWorldmapStateEntry* unaff_s0;
    sWorldmapStateEntry* psVar8;

    psVar8 = &gWorldmapState->m0[param_1];
    switch ((int)(((ushort)psVar8->m4 - 1) * 0x10000) >> 0x10) {
    case -1:
        break;
    case 0: // set to enter yggdrasil
        psVar8->m20 = 8;
        psVar1 = gWorldmapState;
        psVar8->m4 = 0;
        unaff_s0 = &psVar1->m0[psVar8->m6];
        break;
    case 2: // set to exit yggdrasil
        psVar8->m4 = 0;
        psVar8->m20 = 0x30;
        break;
    default:
        assert(0);
    }

    switch (psVar8->m20) {
    case 0:
    case 1:
        if (gameState.m22B1_isOnGear[param_1 + -4] == 1) {
            uint followDataIndex = (worldmapFollowLeaderDataIndex - psVar8->m58) & 0x1f;
            if (((psVar8->m28_position).vx == worldmapFollowLeaderData[followDataIndex].m0_position.vx && (psVar8->m28_position).vy == worldmapFollowLeaderData[followDataIndex].m0_position.vy) &&
                (psVar8->m28_position).vz == worldmapFollowLeaderData[followDataIndex].m0_position.vz) {
                if (((psVar8->m4C->m0_spriteActorCore).mAC >> 24) != '\0') {
                    spriteActorSetPlayingAnimation(psVar8->m4C, 0);
                    clearWorldmapParticles(param_1 + 0x28);
                }
            }
            else {
                if (((psVar8->m4C->m0_spriteActorCore).mAC >> 24) != '\x01') {
                    spriteActorSetPlayingAnimation(psVar8->m4C, 1);
                }
                std::array<short, 1> temp;
                spawnParticlesForTerrainType3(param_1 + 0x28, psVar8, temp);
            }
            (psVar8->m28_position).vx = worldmapFollowLeaderData[followDataIndex].m0_position.vx;
            (psVar8->m28_position).vy = worldmapFollowLeaderData[followDataIndex].m0_position.vy;
            (psVar8->m28_position).vz = worldmapFollowLeaderData[followDataIndex].m0_position.vz;
            psVar8->m48 = worldmapFollowLeaderData[followDataIndex].m10;
        }
        else if (((psVar8->m4C->m0_spriteActorCore).mAC >> 24) != '\x03') {
            spriteActorSetPlayingAnimation(psVar8->m4C, 3);
            clearWorldmapParticles(param_1 + 0x28);
        }
        break;
    case 2:
        (psVar8->m28_position).vx = gWorldmapState->m0[7].m28_position.vx;
        (psVar8->m28_position).vz = gWorldmapState->m0[7].m28_position.vz;
        psVar8->m48 = gWorldmapState->m0[7].m48;
        break;
    case 8: // start enter yggdrasil
        selectYggdrasilExitDirection(&psVar8->m28_position, &unaff_s0->m28_position, &psVar8->m38_step, &psVar8->m48);
        psVar8->m50 = (unaff_s0->m28_position).vx >> 0xc;
        psVar8->m54 = (unaff_s0->m28_position).vz >> 0xc;
        spriteActorSetPlayingAnimation(psVar8->m4C, 1);
        psVar8->m20 = psVar8->m20 + 1;
        [[fallthrough]];
    case 9: // entering yggdrasil
        iVar4 = stepGearAwayFromYggdrasil(psVar8);
        if (iVar4 == 3) {
            psVar8->m20 = psVar8->m20 + 1;
        }
        {
            std::array<short, 1> temp;
            spawnParticlesForTerrainType3(param_1 + 0x28, psVar8, temp);
        }
        break;
    case 10: // signal Yggdrasil that one more gear boarded
        iVar4 = changeWorldmapEntityState((int)(short)psVar8->m6, 4);
        if (iVar4 != 0) {
            psVar8->m24 = 1;
            psVar8->m20 = 2;
            clearWorldmapParticles(param_1 + 0x28);
        }
        break;
    case 0x30: // start exit yggdrasil
        getGamestate182C(&psVar8->m28_position);
        psVar8->m24 = 0;
        psVar8->m48 = gWorldmapState->m0[7].m78;
        {
            VECTOR DAT_1f800000;
            DAT_1f800000.vx = (psVar8->m28_position).vx + getAngleCos((int)psVar8->m48) * 0x60;
            DAT_1f800000.vz = (psVar8->m28_position).vz + getAngleSin((int)psVar8->m48) * -0x60;
            selectYggdrasilExitDirection(&psVar8->m28_position, &DAT_1f800000, &psVar8->m38_step, &psVar8->m48);
            psVar8->m50 = DAT_1f800000.vx >> 0xc;
            psVar8->m54 = DAT_1f800000.vz >> 0xc;
        }
        spriteActorSetPlayingAnimation(psVar8->m4C, 1);
        psVar8->m20++;
        break;
    case 0x31: // exit yggdrasil in progress
        iVar4 = stepGearAwayFromYggdrasil(psVar8);
        if (stepGearAwayFromYggdrasil(psVar8) == 3) {
            spriteActorSetPlayingAnimation(psVar8->m4C, 0);
            (psVar8->m38_step).vz = 0;
            (psVar8->m38_step).vy = 0;
            (psVar8->m38_step).vx = 0;
            psVar8->m20++;
        }
        break;
    case 0x32: // finished exit yggdrasil
        psVar8->m20 = 1;
        break;
    default:
        assert(0);
    }

    if ((psVar8->m20 != 2) && ((worldMapGearMode == 2 || (gameState.m1D34_currentParty[param_1 + -4] != 7)))) {
        //FUN_Worldmap__80074794(1, &psVar8->m28_position);
        //assert(0);
        MissingCode();
    }

    gameState.m1930_fieldVarsBackup[param_1 * 3 + 10] = (ushort)((psVar8->m28_position).vx >> 0xc);
    gameState.m1930_fieldVarsBackup[param_1 * 3 + 0xb] = (ushort)((psVar8->m28_position).vz >> 0xc);
    gameState.m1820_worldmapPosition[param_1 + -1] = psVar8->m48;
    return 1;
}
