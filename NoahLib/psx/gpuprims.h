#pragma once

#include "kernel/math.h"

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
    sVec2_s16 x0y0;
    sVec2_s16 x1y1;
    sVec2_s16 x2y2;
    // size 4, code 0x20

    virtual void execute() override;
};

struct POLY_F4 : public sTag
{
    u8 r0; //4
    u8 g0;
    u8 b0;
    u8 code;
    sVec2_s16 x0y0;
    sVec2_s16 x1y1;
    sVec2_s16 x2y2;
    sVec2_s16 x3y3;
    // size 5, code 0x28

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
    // size 7, code 0x24

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
    // size 9, code 0x2C

    virtual void execute() override;
};

struct POLY_G4 : public sTag
{
    u8 r0; //4
    u8 g0;
    u8 b0;
    u8 code; //7
    sVec2_s16 x0y0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 pad1;
    sVec2_s16 x1y1; // 10
    u8 r2;
    u8 g2;
    u8 b2;
    u8 pad2;
    sVec2_s16 x2y2; //18
    u8 r3;
    u8 g3;
    u8 b3;
    u8 pad3;
    sVec2_s16 x3y3; //20

    virtual void execute() override {}
};

struct LINE_F3 : public sTag
{
    u8 r0; //4
    u8 g0;
    u8 b0;
    u8 code; //7
    sVec2_s16 x0y0;
    sVec2_s16 x1y1; // 10
    sVec2_s16 x2y2;
    u32 pad;

    virtual void execute() override {}
};

struct POLY_GT3 : public sTag
{
    // 4
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    // 8
    sVec2_s16 x0y0;
    // 0xC
    u8 u0;
    u8 v0;
    u16 clut;
    // 0x10
    u8 r1;
    u8 g1;
    u8 b1;
    u8 p1;
    // 0x14
    sVec2_s16 x1y1;
    // 0x18
    u8 u1;
    u8 v1;
    // 0x1A
    u16 tpage;
    // 0x1C
    u8 r2;
    u8 g2;
    u8 b2;
    u8 p2;
    // 0x20
    sVec2_s16 x2y2;
    // 0x24
    u8 u2;
    u8 v2;
    u16 pad1;
    // size 9, code 0x34

    virtual void execute() override;
};

void SetPolyFT4(POLY_FT4* p);
void SetPolyG4(POLY_G4* p);
void SetLineF3(LINE_F3* p);
void SetPolyF4(POLY_F4* p);

void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw);
void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y);

void SetTile(TILE* p);

void SetSprt(SPRT* p);

void setPolyUV(POLY_FT4* poly, ushort u0, ushort v0, ushort u1, ushort v1, ushort u2, ushort v2, ushort u3, ushort v3);

template<typename T>
void SetShadeTex(T* p, int tge)
{
    if (tge == 0) {
        p->code &= ~1;
    }
    else {
        p->code |= 1;
    }
}

template<typename T>
void SetSemiTrans(T* p, int abe)
{
    if (abe == 0)
    {
        p->code &= ~2;
    }
    else
    {
        p->code |= 2;
    }
}