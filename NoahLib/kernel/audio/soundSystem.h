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
    u8 m24_ADSR_AttackMode;
    u8 m25_ADSR_SustainMode;
    u8 m26_ADSR_ReleaseMode;
    u8 m27_ADSR_Attack;
    u8 m28_ADSR_Decay;
    u8 m29_ADSR_Sustain;
    u8 m2A_ADSR_Release;
    u8 m2B_ADSR_SustainLevel;
};
extern std::array<sSoundInstanceEvent30*, 24> playSoundEffectSubSub1Var0;

void initSoundSystem(ushort param_1);
void setSoundError(u16 errorCode);
int processSoundMenuSub0(void);
void setupReverb(int mode, s16 param_2, long delay, long feedback);

void updateSPUDebugger();