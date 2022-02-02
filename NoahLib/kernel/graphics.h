#pragma once

#include "math.h"

struct sTag
{
	sTag* m0_pNext = nullptr;
	u8 m3_size = 0;

	virtual void execute() {}
};

typedef std::array<sTag, 4096> OTTable;

struct TILE : public sTag
{
	u8 r0;
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	s16 w;
	s16 h;

	virtual void execute() override;
};

struct SPRT : public sTag
{
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

	virtual void execute() override;
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

struct DR_ENV : public sTag
{
	u32 code[15];

	virtual void execute() override;
};

struct DR_MODE : public sTag
{
	u32 code[2];

	virtual void execute() override;
};

struct DR_MOVE : public sTag
{
    u32 code[5];

    virtual void execute() override;
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

struct POLY_F3 : public sTag
{
	u8 r0; //4
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	s16 x1;
	s16 y1;
	s16 x2;
	s16 y2;

	virtual void execute() override;
};

struct POLY_F4 : public sTag
{
	u8 r0; //4
	u8 g0;
	u8 b0;
	u8 code;
	s16 x0;
	s16 y0;
	s16 x1;
	s16 y1;
	s16 x2;
	s16 y2;
	s16 x3;
	s16 y3;

	virtual void execute() override;
};

struct POLY_FT3 : public sTag
{
	u8 r0; //4
	u8 g0;
	u8 b0;
	u8 code; //7
	sVec2_s16 x0y0;
	u8 u0; //C
	u8 v0;
	u16 clut;
	sVec2_s16 x1y1; // 10
	u8 u1; // 14
	u8 v1;
	u16 tpage;
	sVec2_s16 x2y2; //18
	u8 u2; //1C
	u8 v2;
	u16 pad1;

	virtual void execute() override;
};

struct POLY_FT4 : public sTag
{
	u8 r0; //4
	u8 g0;
	u8 b0;
	u8 code; //7
	sVec2_s16 x0y0;
	u8 u0; //C
	u8 v0;
	u16 clut;
	sVec2_s16 x1y1; // 10
	u8 u1; // 14
	u8 v1;
	u16 tpage;
	sVec2_s16 x2y2; //18
	u8 u2; //1C
	u8 v2;
	u16 pad1;
	sVec2_s16 x3y3; //20
	u8 u3; //24
	u8 v3;
	u16 pad2;

	virtual void execute() override;
};

void updatePSXVram();

void initPSXFB();
void drawPSXFB();

void SetPolyFT4(POLY_FT4* p);

void DrawSync(int);
int VSync(int);
void InitGeom();

u16 GetClut(int x, int y);
u16 GetTPage(int tp, int abr, int x, int y);

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);
void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw);

void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y);

void SetTile(TILE* p);
void SetSemiTrans(TILE* p, int abe);

void SetSprt(SPRT* p);
void SetSemiTrans(SPRT* p, int abe);
void SetSemiTrans(POLY_FT4* p, int abe);

sTag* ClearOTagR(sTag* ot, int n);
void DrawOTag(sTag* ot);

DISPENV* PutDispEnv(DISPENV* env);
DRAWENV* PutDrawEnv(DRAWENV* env);

