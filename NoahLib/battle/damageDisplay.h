#pragma once

extern u16 damageDisplayVar0;
extern u16 damageDisplayVar1;

struct sDamageDisplaySub0 {
    std::array<s16, 2> m0_screenPosition;
    u8 m4_U;
    u8 m5_V;
    u8 m6_width;
    u8 m7_height;
    u8 m8;
    u16 mA_tpage;
    u16 mC_clut;
    sColorAndCode m10;
    u32 m14;
    // 0x18
};

void createDamageDisplay(sSpriteActorCore* param_1, int param_2, int param_3);
void drawDamageDisplayCharacter(sDamageDisplaySub0* param_1, sColorAndCode colorAndCode);
