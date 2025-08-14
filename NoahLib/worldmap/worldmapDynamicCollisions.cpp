#include "noahLib.h"
#include "worldmapDynamicCollisions.h"
#include "kernel/math.h"

std::array<sWorldmapDynamicCollision, 32> worldmapDynamicCollisions;
u16 currentWorldmapDynamicCollisionSlot;

void adjustWorldModelLocation(VECTOR* param_1);

void processWorldmapDynamicCollisions(VECTOR* param_1, int param_2, int param_3, u8* param_4, u8* param_5) {
    *param_4 = 0;
    *param_5 = 0;

    for (int i = 0; i < currentWorldmapDynamicCollisionSlot; i++) {
        sWorldmapDynamicCollision& entry = worldmapDynamicCollisions[i];
        s32 iVar2;
        s32 iVar4 = param_1->vy;
        if (param_1->vy < entry.m4[1]) {
            iVar2 = iVar4 + param_3 * -0x1000;
            iVar4 = entry.m4[1];
        }
        else {
            iVar2 = entry.m4[1] + entry.m10 * -0x1000;
        }
        if ((iVar4 - iVar2) >> 0xc < param_3 + entry.m10) {
            VECTOR local_50;
            local_50.vx = (entry.m4[0] - param_1->vx) >> 0xc;
            local_50.vz = (entry.m4[2] - param_1->vz) >> 0xc;
            adjustWorldModelLocation(&local_50);
            s32 lVar3 = SquareRoot0(local_50.vx * local_50.vx + local_50.vz * local_50.vz);
            s32 bVar1 = 2;
            if (lVar3 < entry.m14 + param_2) {
            LAB_worldmap__8008c17c:
                *param_4 = bVar1;
                *param_5 = entry.m0;
                return;
            }
            if (lVar3 < entry.m14 + param_2 + 0x10) {
                bVar1 = 1;
                goto LAB_worldmap__8008c17c;
            }
        }
    }
}

void addWorldmapDynamicCollisions(s16 param_1, VECTOR* param_2, s16 param_3, s16 param_4) {

    sWorldmapDynamicCollision& slot = worldmapDynamicCollisions[currentWorldmapDynamicCollisionSlot];

    slot.m0 = param_1;
    slot.m4[0] = param_2->vx;
    slot.m4[1] = param_2->vy;
    slot.m4[2] = param_2->vz;
    slot.m10 = (int)param_4;
    slot.m14 = param_3;

    currentWorldmapDynamicCollisionSlot = currentWorldmapDynamicCollisionSlot + 1 & 0x1f;
}