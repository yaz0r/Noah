#pragma once

#include "math.h"
#include "psx/libgpu.h"


void updatePSXVram();

void initPSXFB();
void drawPSXFB();

DISPENV* PutDispEnv(DISPENV* env);
DRAWENV* PutDrawEnv(DRAWENV* env);

void SetDispMask(int mask);

void AddPrim(sTag* ot, sTag* p);

void setupPolyG4(POLY_G4* param_1, u8 param_2, u8 param_3, u8 param_4);
void ClearImage(RECT* pRect, u8 r, u8 g, u8 b);

void SetBackColor(long rbk, long gbk, long bbk);
void setFogNearColor(byte param_1, byte param_2, byte param_3);
void SetFarColor(long rfc, long gfc, long bfc);
void SetFogNearFar(long a, long b, long h);
int MoveImage(RECT* rect, int x, int y);

void SetColorMatrix(MATRIX*);
void SetLightMatrix(MATRIX*);
