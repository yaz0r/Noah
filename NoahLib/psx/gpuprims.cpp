#include "noahLib.h"
#include "psx/gpuprims.h"

void SetPolyFT4(POLY_FT4* p)
{
    p->m3_size = 9;
    p->code = 0x2C;
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