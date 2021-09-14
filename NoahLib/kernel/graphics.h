#pragma once

struct sTag
{
	sTag* m0_pNext;
	u8 m3_size;
};

struct TILE
{
	sTag tag;
	u8 r0;
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	s16 w;
	s16 h;
};

struct SPRT
{
	sTag tag;
	u8 r0;
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	u8 u0;
	u8 v0;
	u16 clut;
	s16 w;
	s16 h;
};

struct RECT
{
	s16 x;
	s16 y;
	s16 w;
	s16 h;
};

struct DISPENV
{
	RECT disp;
	RECT screen;
	u8 isinter;
	u8 isrgb24;
	u8 pad0;
	u8 pad1;
};

struct DR_ENV
{
	sTag tag;
	u32 code[15];
};

struct DR_MODE
{
	sTag tag;
	u32 code[2];
};

struct DRAWENV
{
	RECT clip;
	s16 ofs[2];
	RECT tw;
	u16 tpage;
	u8 dtd;
	u8 dfe;
	u8 isbg;
	u8 r0;
	u8 g0;
	u8 b0;
	DR_ENV dr_env;
};

struct POLY_FT4
{
	sTag tag;
	u8 r0;
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	u8 u0;
	u8 v0;
	u16 clut;
	s16 x1;
	s16 y1;
	u8 u1;
	u8 v1;
	u16 tpage;
	s16 x2;
	s16 y2;
	u8 u2;
	u8 v2;
	u16 pad1;
	s16 x3;
	s16 y3;
	u8 u3;
	u8 v3;
	u16 pad2;
};

void SetPolyFT4(POLY_FT4* p);

void DrawSync(int);
void VSync(int);
void InitGeom();

u16 GetClut(int x, int y);
u16 GetTPage(int tp, int abr, int x, int y);

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);
void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw);

void SetTile(TILE* p);
void SetSemiTrans(TILE* p, int abe);

void SetSprt(SPRT* p);
void SetSemiTrans(SPRT* p, int abe);

sTag* ClearOTagR(sTag* ot, int n);

DISPENV* PutDispEnv(DISPENV* env);
DRAWENV* PutDrawEnv(DRAWENV* env);
