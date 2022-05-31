#include "noahLib.h"
#include "psx/gpuprims.h"

DR_MODE gCurrentDrawMode;

void TermPrim(sTag* p) {
    p->m0_pNext = nullptr;
    p->m3_size = 0;
}

void SetPolyFT4(POLY_FT4* p)
{
    p->m3_size = 9;
    p->code = 0x2C;
}

void SetPolyF3(POLY_F3* p)
{
    p->m3_size = 4;
    p->code = 0x20;
}

void SetPolyFT3(POLY_FT3* p)
{
    p->m3_size = 7;
    p->code = 0x24;
}

void SetTile(TILE* p)
{
    p->m3_size = 3;
    p->code = 0x60;
}

void SetDrawMove(DR_MOVE* p, RECT* rect, int x, int y)
{
    MissingCode();
}

void SetShadeTex(POLY_FT4* p, int tge)
{
    if (tge == 0) {
        p->code &= ~1;
    }
    else {
        p->code |= 1;
    }
}

void SetPolyG4(POLY_G4* p)
{
    p->m3_size = 8;
    p->code = 0x38;
    return;
}

void SetSprt(SPRT* p)
{
    p->m3_size = 4;
    p->code = 0x64;
}

void SetLineF3(LINE_F3* p)
{
    p->m3_size = 5;
    p->code = 0x48;
    p->pad = 0x55555555;
    return;
}

void SetPolyF4(POLY_F4* p)
{
    p->m3_size = 5;
    p->code = 0x28;
    return;
}

void setPolyUV(POLY_FT4* poly, ushort u0, ushort v0, ushort u1, ushort v1, ushort u2, ushort v2, ushort u3, ushort v3)
{
    if ((int)((uint)u0 << 0x10) < 0) {
        u0 = 0;
    }
    if ((int)((uint)u1 << 0x10) < 0) {
        u1 = 0;
    }
    if ((int)((uint)u2 << 0x10) < 0) {
        u2 = 0;
    }
    if ((int)((uint)u3 << 0x10) < 0) {
        u3 = 0;
    }
    if ((int)((uint)v0 << 0x10) < 0) {
        v0 = 0;
    }
    if ((int)((uint)v1 << 0x10) < 0) {
        v1 = 0;
    }
    if ((int)((uint)v2 << 0x10) < 0) {
        v2 = 0;
    }
    if ((int)((uint)v3 << 0x10) < 0) {
        v3 = 0;
    }
    if (0xff < (short)u0) {
        u0 = 0xff;
    }
    if (0xff < (short)u1) {
        u1 = 0xff;
    }
    if (0xff < (short)u2) {
        u2 = 0xff;
    }
    if (0xff < (short)u3) {
        u3 = 0xff;
    }
    if (0xff < (short)v0) {
        v0 = 0xff;
    }
    if (0xff < (short)v1) {
        v1 = 0xff;
    }
    if (0xff < (short)v2) {
        v2 = 0xff;
    }
    if (0xff < (short)v3) {
        v3 = 0xff;
    }
    poly->u0 = u0;
    poly->v0 = v0;
    poly->u1 = u1;
    poly->v1 = v1;
    poly->u2 = u2;
    poly->v2 = v2;
    poly->u3 = u3;
    poly->v3 = v3;
}

void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage) {
    uint uVar1;
    uint uVar2;

    p->m3_size = 1;
    uVar2 = 0xe1000000;
    if (dtd != 0) {
        uVar2 = 0xe1000200;
    }
    uVar1 = tpage & 0x9ff;
    if (dfe != 0) {
        uVar1 |= 0x400;
    }
    p->code[0] = uVar2 | uVar1;
}