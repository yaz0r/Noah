#pragma once

struct sWorldmapFollowLeader {
    VECTOR m0_position;
    s16 m10;

    // size 0x14
};

extern std::array<sWorldmapFollowLeader, 32> worldmapFollowLeaderData;
extern s16 worldmapFollowLeaderDataIndex;

void followLeaderPostProcess();