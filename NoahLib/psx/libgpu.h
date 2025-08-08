#pragma once

void SetGraphDebug(int level);
s32 ResetGraph(s32 mode);

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