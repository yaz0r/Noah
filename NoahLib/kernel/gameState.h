#pragma once

struct sGameStateA4
{
    s32 m1C;
    s32 m20;
    s16 m24_HP;
    s16 m26_MaxHP;
    s16 m28_MP;
    s16 m2A_MaxMP;
    s16 m34;
    s8 m3A_Level;
    s8 m3B_Level2;
    s8 m5A;
    s8 m78_partyData_gearNum;
    u16 m7C;
    u16 m80;
    //size 0xA4
};

struct sGameStateA42
{
    s16 m10_ether;
    s16 m12_maxEther;
    s32 m38_HP;
    s32 m3C_maxHP;
    //size 0xA4
};

struct sGameState
{
    std::array<std::vector<u8>, 31> m0_names;
    std::array<sGameStateA4, 11> m26C_party;
    std::array<sGameStateA42, 20> m978_gears;
    std::array<s16, 6> m1820_worldmapPosition;
    SVECTOR m182C;
    u16 m1834;
    u16 m1836;
    s16 m1842_disableWorldmapMinimap;
    std::array<s16, 2> m1844;
    s16 m1848;
    SVECTOR m184E;
    s32 m1924_Gold;
    std::array<u16, 512> m1930_fieldVarsBackup;
    u16 m1D30_partyMemberBitField;
    u16 m1D32_partyFrameMask;
    std::array<u8, 3> m1D34_currentParty;
    std::array<s8, 11> m22B1_isOnGear;
    u16 m2318_partyFrameLock;
    u16 m231A_fieldID;
    s16 m231C_CameraYaw;
    s16 m231E_worldmapInitialPositionIndex;
    s16 m2320_worldmapMode;
};

extern sGameState gameState;
extern sGameState* pKernelGameState;

void initGameState();
void loadInitialGameState();

