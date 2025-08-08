#pragma once

#include "psx/libgpu.h"

extern u16 textSpriteMode0;
extern u16 textSpriteMode1;

struct sDialogWindow18PerLineBufferEntry
{
    SPRT m0[2][2];
    RECT m50;
    s16 m58_widthDiv4;
    s8 m5A;
    s8 m5B;
    s16 m5C;
    u16 m5E_clut;
    // size 0x60
};

struct sDialogWindow18_8C
{
    sDialogWindow18_8C* m0_pNext;
    std::vector<u8>::iterator m4_dialogPointer;
    // size 0x8
};

struct sDialogWindow18
{
    sVec2_s16 m0;
    sVec2_s16 m4;
    s16 m8_widthTimes4;
    s16 mA_width1;
    s16 mC_height;
    s16 mE;
    u16 m10_flags;
    s16 m12_widthPadded;
    sVec2_s16 m14;
    s16 m18;
    std::vector<u8>::iterator m1C_currentStringToPrint;
    std::vector<u8>::iterator m20;
    std::vector<sDialogWindow18PerLineBufferEntry> m28_perLineBuffer;
    std::vector<u16> m2C_inRamDialogTextImage;
    DR_MODE m30_textTileDrawMode[2];
    TILE m48_textTile[2];
    s8 m68;
    s8 m69;
    s8 m6A;
    s8 m6B;
    s8 m6C_autoClose;
    s8 m6D;
    s8 m6E_selectedLineForMultiChoice;
    s32 m70[4];
    s16 m80;
    s16 m82;
    s16 m84_delay;
    s16 m86_currentDelayForNextCharacter;
    s16 m88_delayBetweenCharacters;
    sDialogWindow18_8C* m8C;
    std::vector<u8>::iterator m90_dialogPointer;
};

std::vector<u8>::iterator getDialogParamPointer(const std::vector<u8>::iterator& buffer, int param_2);
void resetDialogWindow18(sDialogWindow18* param_1);
void setupWindowSize2(sDialogWindow18* param_1, int x1, int y1, short x2, short y2, short width, short height);
void setSelectedLineForMultiChoice(sDialogWindow18* param_1, char param_2);
void clearSelectedLineForMultiChoice(sDialogWindow18* param_1);
int getWindowSubStructX(sDialogWindow18* param_1);
int getWindowSubStructY(sDialogWindow18* param_1);