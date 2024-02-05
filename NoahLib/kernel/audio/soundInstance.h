#pragma once

#include "soundSystem.h"

struct sSoundInstanceEventD8 {
    u32 m4;
    u16 m10;
    u16 m14;
    u16 m16;
    u16 m18;
    u16 m1A;
    u16 m1E;
    // size 0x20
};

struct sSoundInstanceEventCallstack {
    u8 m0_loopCount;
    u8 m2;
    u8 m3;
    std::vector<u8>::iterator m4_loopStartIt;
    std::vector<u8>::iterator m8_loopEndIt;
    // size 0xC
};


struct sSoundInstanceEvent {
    u16 m0;
    s16 m2;
    s16 m4;
    u8 m6;
    u8 m7;
    u32 m8;
    u32 mC;
    std::vector<u8>::iterator m10;
    std::vector<u8>::iterator m14;
    std::optional<std::vector<u8>::iterator> m18_infiniteLoopStart;
    s32 m1C;
    s16 m20;
    s8 m22;
    s8 m23_infiniteLoopOctave;
    s8 m25;
    s8 m26_ADSRIndex;
    u8 m27;
    s8 m28;
    struct sWdsFile* m2C_pWds;
    sSoundInstanceEvent30 m30;
    u16 m5C_deltaTime;
    u16 m5E;
    s8 m60;
    s16 m62;
    s8 m64;
    s8 m65;
    s16 m66_octave;
    s32 m68;
    s16 m6C;
    s16 m6E;
    s16 m70;
    s16 m72_callstackDepth;
    s16 m74_pan;
    s16 m76;
    s32 m78_volume;
    s32 m7C;
    u16 m80;
    s16 m82;
    s32 m88_volumeSlideDelta;
    u16 m96_volumeSlideDuration;
    std::array<sSoundInstanceEventCallstack, 3> m9C_callstack;
    u16 mCA;
    s16 mCE;
    s16 mD0;
    s16 mD2;
    s16 mD4;

    std::array<sSoundInstanceEventD8, 4> mD8;
    // size 0x158
};

struct sPercussionEntry {
    u8 m0_adsr = 0;
    u8 m1 = 0;
    u8 m2 = 0;
    u8 m3_pan = 0;
};

struct sPercussionData {
    std::array<sPercussionEntry, 0x60> data;
    // size 0x180
};

struct sSoundInstance {
    sSoundInstance(int numEvents, bool allocateExtra) {
        m94_events.resize(numEvents);
    }

    void*  operator new(size_t size) {
        void* data = new u8[size];
        memset(data, 0, size);
        return data;
    }

    sSoundInstance* m0_pNext = nullptr;
    void* m4 = nullptr;
    struct sSeqFile* m8_pSeq = nullptr;
    sPercussionData* mC_pPercussionData = nullptr;
    u16 m10_flags;
    s16 m12;
    u8 m14_count;
    s16 m16_sampleBank;
    u16 m18;
    u8 m1A;
    u8 m1B;
    s16 m1E;
    s32 m20;
    s32 m24;
    s32 m28;
    s32 m2C;
    s16 m30;
    s16 m32;
    s16 m34;
    s16 m36;
    s16 m38;
    s16 m3A;
    s16 m3C;
    s16 m3E;
    u8 m41;
    u8 m42;
    u8 m43;
    s16 m44;
    s32 m48_activeVoicesBF;
    s32 m4C;
    s32 m50;
    s32 m54;
    s32 m58;
    s32 m5C;
    s16 m60;
    s16 m62;
    s32 m64;
    s32 m68;
    s16 m6C;
    s16 m6E;
    s32 m70;
    s32 m74;
    s16 m78;
    s16 m7A;
    s32 m7C;
    s16 m84;
    s32 m88;
    s16 m90;

    std::vector<sSoundInstanceEvent> m94_events;

    sPercussionData mPercussionData; // this is located after all the voices data

    // size 0x94 + events + optional percussionData
};

extern int numMaxSoundEffectInstances;
extern sSoundInstance* pPlayingSoundsLinkedList;
extern sSoundInstance* pSoundEffectsInstances;
extern int playSoundEffectVar0;
extern u32 playSoundEffectSubSub1BF2;

sSoundInstance* initSoundEffectInstances(uint param_1);
void addActiveSoundInstance(sSoundInstance* pSoundInstance);
void initSoundInstanceDefaults(sSoundInstance* pSoundInstance);
void playSoundEffectSub(uint param_1, uint param_2, short param_3, u16 param_4);
void setupAdsr(int param_1, sSoundInstanceEvent* param_2);
sSoundInstance* createMusicInstance(sSeqFile* param_1);

void playSoundEffectSubSub0(sSoundInstanceEvent30* param_1, int param_2);

void startMusicInstance(sSoundInstance* param_1, int, int);
void startMusicInstanceSub0(sSoundInstance* param_1);