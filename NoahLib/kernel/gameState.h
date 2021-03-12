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

struct sGameState
{
    std::array<sGameStateA4, 11> m294;
    u16 m1D30_partyMemberBitField;
    std::array<s8, 3> m1D34_currentParty;
    std::array<s8, 5> m22B1_isOnGear;
    u16 m231A_fieldID;
};

extern sGameState gameState;
extern sGameState* pKernelGameState;

void initGameState();

