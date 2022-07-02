#pragma once

struct sBattleConfig {
    void init(std::vector<u8>::iterator input) {
        m2 = READ_LE_S8(input + 2);
    }

    s8 m2;
    //size 0x20
};

extern std::array<sBattleConfig, 16> battleConfigs;

extern sBattleConfig currentBattleConfig;