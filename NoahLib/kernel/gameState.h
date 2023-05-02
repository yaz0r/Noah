#pragma once

struct sGameStateA4
{
    void deserialize(std::vector<u8>::iterator& buffer) {
        m1C = READ_LE_S32(buffer + 0x1C);
        m20 = READ_LE_S32(buffer + 0x20);
        m24_HP = READ_LE_S16(buffer + 0x24);
        m26_MaxHP = READ_LE_S16(buffer + 0x26);
        m28_MP = READ_LE_S16(buffer + 0x28);
        m2A_MaxMP = READ_LE_S16(buffer + 0x2A);
        m3A_Level = READ_LE_S8(buffer + 0x3A);
        m3B_Level2 = READ_LE_S8(buffer + 0x3B);
        m56_battleCommandLoadout = READ_LE_S8(buffer + 0x56);
        m5A = READ_LE_S8(buffer + 0x5A);
        m78_partyData_gearNum = READ_LE_S8(buffer + 0x78);
        m7A_commandEnabledBF = READ_LE_U16(buffer + 0x7A);
        m7C = READ_LE_U16(buffer + 0x7C);
        m80 = READ_LE_U16(buffer + 0x80);
    }

    s32 m1C;
    s32 m20;
    s16 m24_HP;
    s16 m26_MaxHP;
    s16 m28_MP;
    s16 m2A_MaxMP;
    s16 m34;
    s8 m3A_Level;
    s8 m3B_Level2;
    s8 m56_battleCommandLoadout;
    s8 m5A;
    s8 m78_partyData_gearNum;
    u16 m7A_commandEnabledBF;
    u16 m7C;
    u16 m80;
    //size 0xA4
};

struct sGameStateA42
{
    void deserialize(std::vector<u8>::iterator& buffer) {
        m10_ether = READ_LE_S16(buffer + 0x10);
        m12_maxEther = READ_LE_S16(buffer + 0x12);
        m38_HP = READ_LE_S32(buffer + 0x38);
        m3C_maxHP = READ_LE_S32(buffer + 0x3C);
    }

    s16 m10_ether;
    s16 m12_maxEther;
    s32 m38_HP;
    s32 m3C_maxHP;
    //size 0xA4
};

struct sGameStateSize20 {
    void deserialize(std::vector<u8>::iterator& buffer) {
        m17_energyPerTurn = READ_LE_U8(buffer + 0x17);
    }

    u8 m17_energyPerTurn;
    //size 0x20
};

struct sGameState
{
    void deserialize(std::vector<u8>& rawGameState) {
        for (int i = 0; i < m26C_party.size(); i++) m26C_party[i].deserialize(rawGameState.begin() + 0x294 + i * 0xA4);
        for (int i = 0; i < m978_gears.size(); i++) m978_gears[i].deserialize(rawGameState.begin() + 0x16C0 + i * 0xA4);
        for (int i = 0; i < m16C0.size(); i++) m16C0[i].deserialize(rawGameState.begin() + 0x16C0 + i * 0x20);
        m1924_Gold = READ_LE_U32(rawGameState.begin() + 0x1924);
        for (int i = 0; i < 3; i++) m1D34_currentParty[i] = READ_LE_U8(rawGameState.begin() + 0x1D34 + i);
        m231A_fieldID = READ_LE_U16(rawGameState.begin() + 0x231A);
        m2320_worldmapMode = READ_LE_U16(rawGameState.begin() + 0x2320);
        //m231A_fieldID = 0; // override to boot in debug room
    }

    std::array<std::vector<u8>, 31> m0_names;
    std::array<sGameStateA4, 11> m26C_party;
    std::array<sGameStateA42, 20> m978_gears;
    std::array<sGameStateSize20, 11> m16C0;
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

