#pragma once

#include "kernel/math.h"
#include "kernel/color.h"

struct sColorAndCode
{
    u8 m0_r;
    u8 m1_g;
    u8 m2_b;
    union {
        struct {
            u8 textureBlending : 1;
            u8 semiTransparent : 1;
            u8 textured : 1;
            u8 quad : 1;
            u8 gouraud : 1;
            u8 command : 3;
        };
        u8 m3_code;
    };
    
};

struct sTag
{
    sTag* m0_pNext = nullptr;
    u8 m3_size = 0;

    void execute();
};

typedef std::vector<sTag> OTTable;

struct TILE : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    s16 x0;
    s16 y0;
    s16 w;
    s16 h;

    void execute();
};

struct SPRT_8 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;

    void execute();
};

struct SPRT : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 w;
    s16 h;

    void execute();
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

    void execute();
};

struct DR_MODE : public sTag
{
    u32 code[2];

    void execute();
};

struct DR_MOVE : public sTag
{
    u32 code[5];

    void execute();
};

struct DR_TPAGE : public sTag
{
    u32 code[1];

    void execute();
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
    union {
        sColor color;
        struct {
            u8 r0;
            u8 g0;
            u8 b0;
        };
    };

    DR_ENV dr_env;
};

struct POLY_F3 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    sVec2_s16 x0y0;
    sVec2_s16 x1y1;
    sVec2_s16 x2y2;
    // size 4, code 0x20

    void execute();
};

struct POLY_F4 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    sVec2_s16 x0y0;
    sVec2_s16 x1y1;
    sVec2_s16 x2y2;
    sVec2_s16 x3y3;
    // size 5, code 0x28

    void execute();
};

struct POLY_FT3 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
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

    void execute();
};

struct POLY_G3 : public sTag {
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    union {
        struct {
            s16 x0;
            s16 y0;
        };
        sVec2_s16 x0y0;
    };
    uint8_t r1, g1, b1, pad1;
    union {
        struct {
            s16 x1;
            s16 y1;
        };
        sVec2_s16 x1y1;
    };
    uint8_t r2, g2, b2, pad2;
    union {
        struct {
            s16 x2;
            s16 y2;
        };
        sVec2_s16 x2y2;
    };

    void execute();
};

void SetPolyG3(POLY_G3* p);

struct POLY_FT4 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    union {
        struct {
            s16 x0;
            s16 y0;
        };
        sVec2_s16 x0y0;
    };
    u8 u0; //C
    u8 v0;
    u16 clut;
    union {
        struct {
            s16 x1;
            s16 y1;
        };
        sVec2_s16 x1y1;
    };
    u8 u1; // 14
    u8 v1;
    u16 tpage;
    union {
        struct {
            s16 x2;
            s16 y2;
        };
        sVec2_s16 x2y2;
    };
    u8 u2; //1C
    u8 v2;
    u16 pad1;
    union {
        struct {
            s16 x3;
            s16 y3;
        };
        sVec2_s16 x3y3;
    };
    u8 u3; //24
    u8 v3;
    u16 pad2;
    // size 9, code 0x2C

    void execute();
};

struct POLY_G4 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    union {
        struct {
            s16 x0;
            s16 y0;
        };
        sVec2_s16 x0y0;
    };
    // 0xC
    u8 r1;
    u8 g1;
    u8 b1;
    u8 pad1;
    // 0x10
    union {
        struct {
            s16 x1;
            s16 y1;
        };
        sVec2_s16 x1y1;
    };
    // 0x14
    u8 r2;
    u8 g2;
    u8 b2;
    u8 pad2;
    // 0x18
    union {
        struct {
            s16 x2;
            s16 y2;
        };
        sVec2_s16 x2y2;
    };
    // 0x1C
    u8 r3;
    u8 g3;
    u8 b3;
    u8 pad3;
    union {
        struct {
            s16 x3;
            s16 y3;
        };
        sVec2_s16 x3y3;
    };

    void execute();
};

struct LINE_F2 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    sVec2_s16 x0y0;
    sVec2_s16 x1y1; // 10

    void execute() {}
};

struct LINE_F3 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
    sVec2_s16 x0y0;
    sVec2_s16 x1y1; // 10
    sVec2_s16 x2y2;
    u32 pad;

    void execute() {}
};

struct POLY_GT3 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
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

    void execute();
};

struct POLY_GT4 : public sTag
{
    union {
        struct {
            u8 r0; //4
            u8 g0;
            u8 b0;
            u8 code; //7
        };
        sColorAndCode m_colorAndCode;
    };
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
    u16 pad2;
    // 0x28
    u8 r3;
    u8 g3;
    u8 b3;
    u8 p3;
    // 0x2C
    sVec2_s16 x3y3;
    // 0x30
    u8 u3;
    u8 v3;
    u16 pad3;
    // size 0xC, code 0x3C

    void execute();
};

void SetPolyFT4(POLY_FT4* p);
void SetPolyG4(POLY_G4* p);
void SetLineF3(LINE_F3* p);
void SetPolyF4(POLY_F4* p);
void SetPolyF3(POLY_F3* p);
void SetPolyFT3(POLY_FT3* p);

void TermPrim(sTag*);

void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw);
void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y);

void SetTile(TILE* p);

void SetSprt(SPRT* p);

void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage);

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

extern DR_MODE gCurrentDrawMode;