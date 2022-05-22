#pragma once

#include "math.h"
#include "psx/gpuprims.h"


void updatePSXVram();

void initPSXFB();
void drawPSXFB();

void DrawSync(int);
int VSync(int);
void InitGeom();

u16 GetClut(int x, int y);
u16 GetTPage(int tp, int abr, int x, int y);

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);


sTag* ClearOTagR(sTag* ot, int n);
void DrawOTag(sTag* ot);

DISPENV* PutDispEnv(DISPENV* env);
DRAWENV* PutDrawEnv(DRAWENV* env);

void SetDispMask(int mask);

void AddPrim(sTag* ot, sTag* p);

void setupPolyG4(POLY_G4* param_1, u8 param_2, u8 param_3, u8 param_4);
void ClearImage(RECT* pRect, u8 r, u8 g, u8 b);