#pragma once

void SetGraphDebug(int level);
s32 ResetGraph(s32 mode);

s32 SetVideoMode(s32 mode);
s32 GetVideoMode();

#include "gpuprims.h"

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);

