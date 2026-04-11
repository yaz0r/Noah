#pragma once

void SetGraphDebug(int level);
s32 ResetGraph(s32 mode);

// PSX 0x80030988: patches three shift-amount immediates in place inside a
// texture-uv helper function so callers can cheaply change the UV step size.
// Genuine self-modifying code — stubbed on PC.
void patchSelfModifyingCode(s16 shiftA, s16 shiftB, u16 addA, u16 addB);

s32 SetVideoMode(s32 mode);
s32 GetVideoMode();

#include "gpuprims.h"

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);

u16 GetClut(int x, int y);
u16 GetTPage(int tp, int abr, int x, int y);

void DrawSync(int);
int VSync(int);

sTag* ClearOTagR(sTag* ot, int n);
sTag* ClearOTagR(std::vector<sTag>& ot, int n);
void DrawOTag(sTag* ot);

void AddPrim(sTag* ot, sTag* p);