#include "noahLib.h"
#include "memoryCardMenu.h"
#include "menuHandler.h"

std::array<int, 21> memoryCardSlotsX = { {
     0x30,          0x3C,          0x48,          0x54,
     0x60,          0x6C,          0x78,          0x84,
     0x90,          0x9C,          0xA8,          0xB4,
     0xC0,          0xCC,          0xD8,          0xE4,
     0xF0,          0xFC,         0x108,         0x114,
    0x120
} };

std::array<int, 2> memoryCardSlotsY = { {
        0x56, 0x66
} };

void initMenuContext34C_polys() {
    for (int i = 0; i < 0x20; i++) {
        for (int j = 0; j < 2; j++) {
            initMemoryCardTransparentPoly(&gMenuContext->m34C->m0_polys[i][j]);
            int X = memoryCardSlotsX[i % 0x15];
            short Xoffset = (short)(i / 0x15) * 8;
            int Y = memoryCardSlotsY[i / 0x15];
            gMenuContext->m34C->m0_polys[i][j].x0 = X + Xoffset;
            gMenuContext->m34C->m0_polys[i][j].y0 = Y;
            gMenuContext->m34C->m0_polys[i][j].x1 = X + Xoffset + 0xc;
            gMenuContext->m34C->m0_polys[i][j].y1 = Y;
            gMenuContext->m34C->m0_polys[i][j].x2 = X + Xoffset;
            gMenuContext->m34C->m0_polys[i][j].y2 = Y + 0x10;
            gMenuContext->m34C->m0_polys[i][j].x3 = X + Xoffset + 0xc;
            gMenuContext->m34C->m0_polys[i][j].y3 = Y + 0x10;
            int iVar3 = i;
            if (i < 0) {
                iVar3 = i + 0xf;
            }

            int cVar1 = (char)(iVar3 >> 4) * '\x10';
            int u1 = (char)i * '\x10';
            gMenuContext->m34C->m0_polys[i][j].u0 = u1;
            int v0 = cVar1 + 0xe0;
            gMenuContext->m34C->m0_polys[i][j].v0 = v0;
            gMenuContext->m34C->m0_polys[i][j].u1 = u1 + '\f';
            gMenuContext->m34C->m0_polys[i][j].v1 = v0;
            gMenuContext->m34C->m0_polys[i][j].u2 = u1;
            v0 = cVar1 + 0xef;
            gMenuContext->m34C->m0_polys[i][j].v2 = v0;
            gMenuContext->m34C->m0_polys[i][j].u3 = u1 + '\f';
            gMenuContext->m34C->m0_polys[i][j].v3 = v0;

            gMenuContext->m34C->m0_polys[i][j].tpage = GetTPage(0, 0, 0x140, 0x80);
            gMenuContext->m34C->m0_polys[i][j].clut = GetClut(0, 0x1c0);
        }
    }
}

void initMenuContext34C_polys2() {
    for (int i = 0; i < 2; i++) {
        initMemoryCardTransparentPoly(&gMenuContext->m34C->mA00_polys2[i]);
        gMenuContext->m34C->mA00_polys2[i].x0 = 0x10;
        gMenuContext->m34C->mA00_polys2[i].y0 = 0x56;
        gMenuContext->m34C->mA00_polys2[i].x1 = 0x30;
        gMenuContext->m34C->mA00_polys2[i].y1 = 0x56;
        gMenuContext->m34C->mA00_polys2[i].x2 = 0x10;
        gMenuContext->m34C->mA00_polys2[i].y2 = 0x76;
        gMenuContext->m34C->mA00_polys2[i].x3 = 0x30;
        gMenuContext->m34C->mA00_polys2[i].y3 = 0x76;
        gMenuContext->m34C->mA00_polys2[i].tpage = GetTPage(0, 0, 0x140, 0x80);

        SetPolyG4(&gMenuContext->m34C->mA50_polyG4[i]);
        (gMenuContext->m34C->mA50_polyG4[i].r0) = 0x80;
        (gMenuContext->m34C->mA50_polyG4[i].g0) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].b0) = 0x80;
        (gMenuContext->m34C->mA50_polyG4[i].r1) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].g1) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].b1) = 0x80;
        (gMenuContext->m34C->mA50_polyG4[i].r2) = '\x10';
        (gMenuContext->m34C->mA50_polyG4[i].g2) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].b2) = '\x10';
        (gMenuContext->m34C->mA50_polyG4[i].r3) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].g3) = '\0';
        (gMenuContext->m34C->mA50_polyG4[i].b3) = '\x10';

        gMenuContext->m34C->mA50_polyG4[i].x0 = 0;
        gMenuContext->m34C->mA50_polyG4[i].y0 = 0x4a;
        gMenuContext->m34C->mA50_polyG4[i].x1 = 0x140;
        gMenuContext->m34C->mA50_polyG4[i].y1 = 0x4a;
        gMenuContext->m34C->mA50_polyG4[i].x2 = 0;
        gMenuContext->m34C->mA50_polyG4[i].y2 = 0x8a;
        gMenuContext->m34C->mA50_polyG4[i].x3 = 0x140;
        gMenuContext->m34C->mA50_polyG4[i].y3 = 0x8a;
    }
}

void initMenuContext34C() {
    gMenuContext->m34C = new sMenuContext_34C;
    memset(gMenuContext->m34C, 0, sizeof(sMenuContext_34C));
    initMenuContext34C_polys();
    initMenuContext34C_polys2();
}