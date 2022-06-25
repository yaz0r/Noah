#include "noahLib.h"
#include "worldmapExit.h"

std::vector<sWorldmapExitDef> worldmapExitsDefs;
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
    worldmapCurrentExit = &worldmapExitsDefs[param_2];

    if (worldmapCurrentExit->m8_destinationField != -1) {
        uint uVar3 = param_1->vx >> 0xc & 0xffff;
        uint uVar2 = param_1->vz >> 0xc & 0xffff;
        do {
            if (worldmapCurrentExit->m0_X <= uVar3 &&
                uVar3 <= worldmapCurrentExit->m0_X + worldmapCurrentExit->m4_width &&
                worldmapCurrentExit->m2_Y <= uVar2 &&
                uVar2 <= worldmapCurrentExit->m2_Y + worldmapCurrentExit->m6_height
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