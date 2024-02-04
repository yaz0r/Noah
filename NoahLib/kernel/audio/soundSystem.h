#pragma once

extern u16 musicStatusFlag;

struct sSoundInstanceEvent30 {
    s16 m0;
    s16 m2;
    s16 m4;
    s16 m6;
    s16 m8_volumeLeft;
    s16 mA_volumeRight;
    s16 mC;
    s16 mE;
    u16 m14_ADPCM_SampleRate;
    u32 m1C_ADPCM_StartAddress;
    u32 m20_ADPCM_RepeatAddress;
    u8 m24;
    u8 m25;
    u8 m26;
    u8 m27;
    u8 m28;
    u8 m29;
    u8 m2A;
    u8 m2B;
};
extern std::array<sSoundInstanceEvent30*, 24> playSoundEffectSubSub1Var0;

void initSoundSystem(ushort param_1);
void setSoundError(u16 errorCode);
int processSoundMenuSub0(void);
void setupReverb(uint param_1, s16 param_2, long param_3, long param_4);