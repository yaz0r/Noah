#include "noahLib.h"
#include "worldmapExit.h"
#include "field/dialogWindows.h"
#include "worldmap.h"

/*
There is 4 exit tables:
0 for on foot
1 for gears
2 for yggdrasil
3 for ?????
*/
std::array<std::vector<sWorldmapExitDef>, 4> worldmapExitsDefs;

sWorldmapExitDef* worldmapCurrentExit;
s16 worldmapExitVar0;
s16 worldmapExitVar1;

sWorldmapExitDef worldmapExitToField312 = {
        0,0,0,0,0x138, 1, 0xE, 0
};
sWorldmapExitDef worldmapExitToField440 = {
        0,0,0,0,0x1B8, 1, 0xD, 0
};
sWorldmapExitDef worldmapExitToField290_yggdrasilDeck = {
        0,0,0,0,0x122, 3, -1, 0
};

int setupWorldmapExits(VECTOR* param_1, int param_2) {
    worldmapCurrentExit = &worldmapExitsDefs[param_2][0];

    if (worldmapCurrentExit->m8_destinationField != -1) {
        uint X = param_1->vx >> 0xc & 0xffff;
        uint Y = param_1->vz >> 0xc & 0xffff;
        do {
            if ((worldmapCurrentExit->m0_X <= X) &&
                (X <= (worldmapCurrentExit->m0_X + worldmapCurrentExit->m4_width)) &&
                (worldmapCurrentExit->m2_Y <= Y) &&
                (Y <= (worldmapCurrentExit->m2_Y + worldmapCurrentExit->m6_height))
                ) {
                if (worldmapCurrentExit->mE_type == 4) {
                    worldmapExitVar0 = -1;
                    worldmapExitVar1 = worldmapCurrentExit->mC_newWorldmapMode;
                    worldmapCurrentExit = nullptr;
                    return 1;
                }
                worldmapExitVar0 = worldmapCurrentExit->mC_newWorldmapMode;
                worldmapExitVar1 = -1;
                return 1;
            }
            worldmapCurrentExit++;
        } while (worldmapCurrentExit->m8_destinationField != -1);
    }
    worldmapExitVar0 = -1;
    worldmapExitVar1 = -1;
    worldmapCurrentExit = nullptr;
    return 0;
}

sDialogWindow18 worldmapDialogWindow18;

s32 worldmap_taskExit_init(s32) {
    worldmapExitVar0 = 0xffff;
    setupWindowSize2(&worldmapDialogWindow18, 0x3c0, 0x180, 0xa0, 0x78, 0x20, 1);
    worldmapDialogWindow18.m68 = 8;
    worldmapDialogWindow18.m10_flags = worldmapDialogWindow18.m10_flags | 2;
    resetDialogWindow18(&worldmapDialogWindow18);
    return 1;
}

s32 worldmap_taskExit_update(s32 param_1) {
    short sVar1;
    sWorldmapState* psVar2;
    int iVar4;

    psVar2 = gWorldmapState;
    sVar1 = gWorldmapState->m0[param_1].m20;
    if (sVar1 == 0) {
        if (worldmapExitVar0 != -1) {
            resetDialogWindow18(&worldmapDialogWindow18);
            addDialogWindowsToOTSub2(&worldmapDialogWindow18, getDialogParamPointer(worldmapFile1Buffer_1C, (int)worldmapExitVar0));
            iVar4 = (int)worldmapExitVar0;
            psVar2->m0[param_1].m20 = 1;
            psVar2->m0[param_1].m50 = iVar4;
        }
    }
    else if (sVar1 == 1) {
        if (worldmapExitVar0 == -1) {
            resetDialogWindow18(&worldmapDialogWindow18);
            psVar2->m0[param_1].m20 = 0;
        }
        else if ((int)worldmapExitVar0 != gWorldmapState->m0[param_1].m50) {
            resetDialogWindow18(&worldmapDialogWindow18);
            addDialogWindowsToOTSub2(&worldmapDialogWindow18, getDialogParamPointer(worldmapFile1Buffer_1C, (int)worldmapExitVar0));
            psVar2->m0[param_1].m50 = (int)worldmapExitVar0;
        }
    }
    updateAndRenderTextForDialogWindow(&worldmapDialogWindow18, &pCurrentWorldmapRenderingStruct->m70_OT[0], worldmapOddOrEven);
    return 1;
}