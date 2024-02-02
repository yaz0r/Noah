#pragma once

struct sGameStateA4
{
    struct sGameStateA4Sub4 {
        s8 m0;

        void deserialize(std::vector<u8>::iterator buffer) {
            m0 = READ_LE_U8(buffer + 0);
        }

        // size 8
    };

    void deserialize(std::vector<u8>::iterator buffer) {
        m2 = READ_LE_S8(buffer + 0x2);
        m3 = READ_LE_S8(buffer + 0x3);
        for (int i = 0; i < m4.size(); i++) {
            m4[i].deserialize(buffer + 4 + 8 * i);
        }
        m28 = READ_LE_S8(buffer + 0x28);
        m29 = READ_LE_S8(buffer + 0x29);
        m2A = READ_LE_S8(buffer + 0x2A);
        m2B = READ_LE_S8(buffer + 0x2B);
        m2C = READ_LE_S8(buffer + 0x2C);
        m2E = READ_LE_S8(buffer + 0x2E);
        m2F = READ_LE_S8(buffer + 0x2F);
        m30 = READ_LE_S8(buffer + 0x30);
        m31 = READ_LE_S8(buffer + 0x31);
        m2D = READ_LE_S8(buffer + 0x2D);
        m32 = READ_LE_U16(buffer + 0x32);
        m34 = READ_LE_S16(buffer + 0x34);
        m36 = READ_LE_U16(buffer + 0x36);
        m38 = READ_LE_U16(buffer + 0x38);
        m3A = READ_LE_S16(buffer + 0x3A);
        m44 = READ_LE_S32(buffer + 0x44);
        m48 = READ_LE_S32(buffer + 0x48);
        m4C_HP = READ_LE_S16(buffer + 0x4C);
        m4E_MaxHP = READ_LE_S16(buffer + 0x4E);
        m50_MP = READ_LE_S16(buffer + 0x50);
        m52_MaxMP = READ_LE_S16(buffer + 0x52);
        m55 = READ_LE_S8(buffer + 0x55);
        m56_battleCommandLoadout = READ_LE_S8(buffer + 0x56);
        m58_attack = READ_LE_S8(buffer + 0x58);
        m59_defence = READ_LE_S8(buffer + 0x59);
        m5A_agility = READ_LE_S8(buffer + 0x5A);
        m5B_ether = READ_LE_S8(buffer + 0x5B);
        m5C_etherDefence = READ_LE_S8(buffer + 0x5C);
        m5E_hitPercentage = READ_LE_S8(buffer + 0x5E);
        m5F_evadePercentage = READ_LE_S8(buffer + 0x5F);
        m62_Level = READ_LE_S8(buffer + 0x62);
        m63_Level2 = READ_LE_S8(buffer + 0x63);
        m6A_weaponModifier = READ_LE_S8(buffer + 0x6A);
        mA0_partyData_gearNum = READ_LE_S8(buffer + 0xA0);
        for (int i = 0; i < m74_accessorySlots.size(); i++) {
            m74_accessorySlots[i] = READ_LE_U8(buffer + 0x74 + i);
        }
        m7A_commandEnabledBF = READ_LE_U16(buffer + 0x7A);
        m7C = READ_LE_U16(buffer + 0x7C);
        m7E = READ_LE_U16(buffer + 0x7E);
        m80 = READ_LE_U16(buffer + 0x80);
        m82 = READ_LE_U16(buffer + 0x82);
        m84 = READ_LE_U16(buffer + 0x84);
        m86 = READ_LE_U16(buffer + 0x86);
        m88 = READ_LE_U16(buffer + 0x88);
        m8A = READ_LE_U16(buffer + 0x8A);
        m8C = READ_LE_U16(buffer + 0x8C);
        m8E = READ_LE_U16(buffer + 0x8E);
        for (int i = 0; i < m90.size(); i++) {
            m90[i] = READ_LE_U16(buffer + 0x90 + 2 * i);
        }
        mA1 = READ_LE_S8(buffer + 0xA1);
    }

    s8 m2;
    s8 m3;
    std::array<sGameStateA4Sub4, 4> m4;
    s8 m2D;
    s8 m28;
    s8 m29;
    s8 m2A;
    s8 m2B;
    s8 m2C;
    s8 m2E;
    s8 m2F;
    s8 m30;
    s8 m31;
    u16 m32;
    s16 m34;
    u16 m36;
    u16 m38;
    s16 m3A;
    s32 m44;
    s32 m48;
    s16 m4C_HP;
    s16 m4E_MaxHP;
    s16 m50_MP;
    s16 m52_MaxMP;
    s8 m55;
    s8 m56_battleCommandLoadout;
    s8 m58_attack;
    s8 m59_defence;
    s8 m5A_agility;
    s8 m5B_ether;
    s8 m5C_etherDefence;
    s8 m5E_hitPercentage;
    s8 m5F_evadePercentage;
    s8 m62_Level;
    s8 m63_Level2;
    s8 m6A_weaponModifier;
    s8 mA0_partyData_gearNum;
    std::array<s8, 3> m74_accessorySlots;
    u16 m7A_commandEnabledBF;
    u16 m7C;
    u16 m7E;
    u16 m80;
    u16 m82;
    u16 m84;
    u16 m86;
    u16 m88;
    u16 m8A;
    u16 m8C;
    u16 m8E;
    std::array<u16, 8> m90;
    s8 mA1;
    //size 0xA4
};

struct sGameStateA42
{
    void deserialize(std::vector<u8>::iterator buffer) {
        m10_ether = READ_LE_S16(buffer + 0x10);
        m12_maxEther = READ_LE_S16(buffer + 0x12);
        m38_fuel = READ_LE_S32(buffer + 0x38);
        m3C_maxFuel = READ_LE_S32(buffer + 0x3C);
        m5C_extraFileIndex = READ_LE_U8(buffer + 0x5C);
        m60_hp = READ_LE_S32(buffer + 0x60);
        m64 = READ_LE_S32(buffer + 0x64);
        m70 = READ_LE_U16(buffer + 0x70);
        m7C = READ_LE_U16(buffer + 0x7C);
        m82 = READ_LE_U16(buffer + 0x82);
        m84 = READ_LE_U16(buffer + 0x84);
        m86 = READ_LE_U16(buffer + 0x86);
        m98 = READ_LE_U8(buffer + 0x98);
    }

    s16 m10_ether;
    s16 m12_maxEther;
    s32 m38_fuel;
    s32 m3C_maxFuel;
    u8 m5C_extraFileIndex;
    s32 m60_hp;
    s32 m64;
    u16 m70;
    u16 m7C;
    u16 m82;
    u16 m84;
    u16 m86;
    u8 m98;
    //size 0xA4
};

struct sGameStateSize20 {
    void deserialize(std::vector<u8>::iterator buffer) {
        for (int i = 0; i < 8; i++) {
            m0_unlockedAbilitiesBitField[i] = READ_LE_U16(buffer + i * 2);
        }
        m17_energyPerTurn = READ_LE_U8(buffer + 0x17);
    }

    std::array<u16, 8> m0_unlockedAbilitiesBitField;
    u8 m17_energyPerTurn;
    //size 0x20
};

struct sGameState
{
    void deserialize(std::vector<u8>& rawGameState) {
        for (int i = 0; i < m26C_party.size(); i++) m26C_party[i].deserialize(rawGameState.begin() + 0x26C + i * 0xA4);
        for (int i = 0; i < m978_gears.size(); i++) m978_gears[i].deserialize(rawGameState.begin() + 0x978 + i * 0xA4);
        for (int i = 0; i < m16C0.size(); i++) m16C0[i].deserialize(rawGameState.begin() + 0x16C0 + i * 0x20);
        for (int i = 0; i < m1820_worldmapPosition.size(); i++) m1820_worldmapPosition[i] = READ_LE_U16(rawGameState.begin() + 0x1820 + i * 2);
        m182C = SVECTOR::FromIt(rawGameState.begin() + 0x182C);
        m1834 = READ_LE_U16(rawGameState.begin() + 0x1834);
        m1836 = READ_LE_U16(rawGameState.begin() + 0x1836);
        m1842_disableWorldmapMinimap = READ_LE_S16(rawGameState.begin() + 0x1842);
        for (int i = 0; i < m1844.size(); i++) m1844[i] = READ_LE_S16(rawGameState.begin() + 0x1844 + i * 2);
        m1848 = READ_LE_S16(rawGameState.begin() + 0x1848);
        m184E = SVECTOR::FromIt(rawGameState.begin() + 0x184E);
        m1924_Gold = READ_LE_U32(rawGameState.begin() + 0x1924);
        for (int i = 0; i < m1930_fieldVarsBackup.size(); i++)  m1930_fieldVarsBackup[i] = READ_LE_U16(rawGameState.begin() + 0x1930 + i * 2);
        m1D30_partyMemberBitField = READ_LE_U16(rawGameState.begin() + 0x1D30);
        m1D32_partyFrameMask = READ_LE_U16(rawGameState.begin() + 0x1D32);
        for (int i = 0; i < 3; i++) m1D34_currentParty[i] = READ_LE_U8(rawGameState.begin() + 0x1D34 + i);
        for (int i = 0; i < m22B1_isOnGear.size(); i++) m22B1_isOnGear[i] = READ_LE_U8(rawGameState.begin() + 0x22b1 + i);
        m2318_partyFrameLock = READ_LE_U16(rawGameState.begin() + 0x2318);
        m231A_fieldID = READ_LE_U16(rawGameState.begin() + 0x231A);
        m231C_CameraYaw = READ_LE_S16(rawGameState.begin() + 0x231C);
        m231E_worldmapInitialPositionIndex = READ_LE_S16(rawGameState.begin() + 0x231E);
        m2320_worldmapMode = READ_LE_U16(rawGameState.begin() + 0x2320);
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

