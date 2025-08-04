#pragma once

#include "psx/libgpu.h"

struct sBattleRunningVar0Sub {
    std::array<s16, 8> m0_scriptBytecodeOffset;
    std::array<s8, 8> m10_isScriptSlotActive;
    std::array<s8, 8> m18;
    s8 m20_currentActiveEntry = 0;
    s8 m21 = 0;
    s8 m22 = 0;
    s8 m23 = 0;
    s8 m24 = 0;
    s16 m26 = 0;
    s8 m28 = 0;
    struct sSpriteActorAnimationBundle* m2C = nullptr;
    struct sTaskHeader* m30 = nullptr;
    s8 m34 = 0;
    s8 m35 = 0;
    // size 0x38
};

struct sDialogParams {
    union {
        struct {
            s16 m0_x;
            s16 m2_y;
            s16 m4_width;
            s16 m6_height;
            s16 m8_flags;
        };
        s16 mRaw[5];
    };
};

struct sBattleRunningVar0 {
    std::array<sBattleRunningVar0Sub, 0x10> m0_scriptEntities;
    std::array<s16, 8> m380_currentArgs;
    std::vector<u8>::iterator m390_scriptByteCode;
    std::array<s16, 512> m394_vars;
    std::array<s8, 0x10> m794;
    s8 m7F5;
    std::array<POLY_FT4, 2> m7A4;
    s8 m7F4_battleDialogPortraitOddOrEven;
    sDialogParams m7F6_dialogParams;
    s8 m800 = 0;
    s8 m802_dialogBoxInitialized = 0;
    std::array<s8, 0x10> m804;
    struct sSeqFile* m818 = nullptr;
    s8 m81F;
    s8 m820;
    // size 0x828
};

extern struct sBattleRunningVar0* battleEventVar0;

void battleEventEntry(void);
void battleEvent_update(int);

extern struct sDialogWindow18* battleEventVar1;