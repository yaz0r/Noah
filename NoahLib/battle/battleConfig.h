#pragma once

struct sBattleConfig {
    void init(std::vector<u8>::iterator input) {
        m0 = READ_LE_S8(input + 0);
        m2 = READ_LE_S8(input + 2);
        m4[0] = READ_LE_S8(input + 4 + 0);
        m4[1] = READ_LE_S8(input + 4 + 1);
        m4[2] = READ_LE_S8(input + 4 + 2);
    }

    s8 m0;
    s8 m2;
    std::array<s8, 3> m4;
    //size 0x20
};

extern std::array<sBattleConfig, 16> battleConfigs;

extern sBattleConfig currentBattleConfig;