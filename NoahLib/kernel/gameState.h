#pragma once

struct sGameStateA4
{
    s16 m24_HP;
    s16 m26_MaxHP;
    s16 m28_MP;
    s16 m2A_MaxMP;
    s8 m78_partyData_gearNum;
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
    std::array<sGameStateA4, 11> m294;
    std::array<sGameStateA42, 20> m9A0_gears;
    std::array<u16, 512> m1930_fieldVarsBackup;
    u16 m1D30_partyMemberBitField;
    u16 m1D32_partyFrameMask;
    std::array<u8, 3> m1D34_currentParty;
    std::array<s8, 5> m22B1_isOnGear;
    u16 m2318_partyFrameLock;
    u16 m231A_fieldID;
    s16 m231C_CameraYaw;
    s16 m231E;
    s16 m2320;
};

extern sGameState gameState;
extern sGameState* pKernelGameState;

void initGameState();
void loadInitialGameState();

