#pragma once

struct sWorldmapDynamicCollision {
    s16 m0;
    std::array<s32, 3> m4;
    s32 m10;
    s32 m14;

    //size 0x18
};

extern std::array<sWorldmapDynamicCollision, 32> worldmapDynamicCollisions;
extern u16 currentWorldmapDynamicCollisionSlot;

void processWorldmapDynamicCollisions(VECTOR* param_1, int param_2, int param_3, u8* param_4, u8* param_5);
void addWorldmapDynamicCollisions(s16 param_1, VECTOR* param_2, s16 param_3, s16 param_4);