#pragma once
#include "psx/gpuprims.h"

struct sDebugText {
    u16 m0;
    u16 m2;
    std::array<std::vector<SPRT_8>::iterator, 2> m4_Buffer;
    u16 mC;
    u16 mE;
    u16 m10;
    u16 m12;
    u16 m14;
    sColorAndCode m18_colorAndCode;
    u16 m16;
    std::array<DR_TPAGE, 2> m1C_DR_TPage;
    u16 m2C_maxPrims;
    u16 m2E;
    u16 m30;
    u16 m32;
    u16 m34_numPrims;
    u16 m36;
    std::vector<SPRT_8>::iterator m38_pCurrentPrim;
    std::array<u16, 4> m3C_cluts;
    std::array<TILE, 2> m44_tiles;
    u16 mCC;
    u16 mCE;
    u16 mD0;
    u8 mD2;
    // size: 0xD4


    std::vector<SPRT_8> buffer;
};

sDebugText* initDebugFont(uint x, ushort y, uint w, ushort h, int bufferSize, ushort flags, uint param_7, int param_8, uint param_9, int param_10, void* font);
void deinitDebugFont();
void logToScreen(const char* fmt, ...);
void printDebugTextOnScreen(sTag* param_1);