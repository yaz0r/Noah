#include "noahLib.h"
#include "memoryCardTiles.h"
#include "menuHandler.h"

std::array<s32, 15 * 2> memoryCardTilesPosition2 = { {
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23,
        24,25,26,27,
        28,29
} };

std::array<s32, 32> memoryCardTilesPositionX = { {
     0x20,          0x28,          0x30,          0x38,
     0x40,          0x48,          0x50,          0x58,
     0x60,          0x68,          0x70,          0x78,
     0x80,          0x88,          0x90,         0x140,
     0xB0,          0xB8,          0xC0,          0xC8,
     0xD0,          0xD8,          0xE0,          0xE8,
     0xF0,          0xF8,         0x100,         0x108,
    0x110,         0x118,         0x120,         0x140
} };

std::array<s32, 32> memoryCardTilesPositionY = { {
 0xE,          0x22,          0x36,           0xE,
0x22,          0x36,           0xE,          0x22,
0x36,           0xE,          0x22,          0x36,
 0xE,          0x22,          0x36,         0x100,
 0xE,          0x22,          0x36,           0xE,
0x22,          0x36,           0xE,          0x22,
0x36,           0xE,          0x22,          0x36,
 0xE,          0x22,          0x36,         0x100
} };

void initMenuContext3A8_Lines(int param_1) {
    auto pX = memoryCardTilesPositionX.begin() + param_1;
    auto pY = memoryCardTilesPositionY.begin() + param_1;
    auto pTile = gMenuContext->m3A8_memoryCardTiles[param_1];
    for(int i=0; i<2; i++) {
        SetLineF3(&pTile->m50[i]);
        pTile->m50[i].r0 = 0;
        pTile->m50[i].g0 = 0xff;
        pTile->m50[i].b0 = 0;
        pTile->m50[i].x0y0.vx = *pX;
        pTile->m50[i].x0y0.vy = *pY;
        pTile->m50[i].x1y1.vx = *pX + 0x10;
        pTile->m50[i].x1y1.vy = *pY;
        pTile->m50[i].x2y2.vx = *pX + 0x10;
        pTile->m50[i].x2y2.vy = *pY + 0x10;
        initMemoryCardPolyVerts(pTile->m100, *pX, *pY, 0x10, 0x10);
        SetLineF3(&pTile->m80[i]);
        pTile->m80[i].r0 = 0;
        pTile->m80[i].g0 = 0xff;
        pTile->m80[i].b0 = 0;
        pTile->m80[i].x0y0.vx = *pX;
        pTile->m80[i].x0y0.vy = *pY;
        pTile->m80[i].x1y1.vx = *pX;
        pTile->m80[i].x1y1.vy = *pY + 0x10;
        pTile->m80[i].x2y2.vx = *pX + 0x10;
        pTile->m80[i].x2y2.vy = *pY + 0x10;
        initMemoryCardPolyVerts(pTile->m120, *pX, *pY, 0x10, 0x10);
    }
}

void drawMenu_memoycard_tilesLines(void) {
    if (gMenuContext->m4D8) {
        auto sVar1 = gMenuContext->m32C_memoryCardContext->m4FAE
            [memoryCardTilesPosition2[gMenuContext->m32C_memoryCardContext->m4F7C]];
        for (int i = 0; i < 0x20; i++) {
            auto pTile = gMenuContext->m3A8_memoryCardTiles[i];
            auto pContext = gMenuContext->m32C_memoryCardContext;

            int i2 = i;
            if (i < 0) {
                i2 = i + 0xf;
            }
            if ((pContext->m4FE4[i2 >> 4] != 0) && ((i2 >> 4) << 4 != i + -0xf)) {
                bool isSelected = false;
                if ((sVar1 == pContext->m4FAE[i]) &&
                    ((gMenuContext->m4D8 == 2 && (isSelected = true, sVar1 == -1)))) {
                    isSelected = i == memoryCardTilesPosition2[pContext->m4F7C];
                }
                if (isSelected) {
                    pTile->m50[gMenuContext->m308_oddOrEven].r0 = 0xff;
                    pTile->m50[gMenuContext->m308_oddOrEven].g0 = '\0';
                    pTile->m50[gMenuContext->m308_oddOrEven].b0 = '\0';
                    pTile->m80[gMenuContext->m308_oddOrEven].r0 = 0xff;
                    pTile->m80[gMenuContext->m308_oddOrEven].g0 = '\0';
                }
                else {
                    pTile->m50[gMenuContext->m308_oddOrEven].r0 = '\0';
                    pTile->m50[gMenuContext->m308_oddOrEven].g0 = 0xff;
                    pTile->m50[gMenuContext->m308_oddOrEven].b0 = '\0';
                    pTile->m80[gMenuContext->m308_oddOrEven].r0 = '\0';
                    pTile->m80[gMenuContext->m308_oddOrEven].g0 = 0xff;
                }
                pTile->m80[gMenuContext->m308_oddOrEven].b0 = '\0';

                s32 dummy0;
                s32 dummy1;

                RotTransPers3(&pTile->m100[0], &pTile->m100[1], &pTile->m100[3],
                    &pTile->m50[gMenuContext->m308_oddOrEven].x0y0,
                    &pTile->m50[gMenuContext->m308_oddOrEven].x1y1,
                    &pTile->m50[gMenuContext->m308_oddOrEven].x2y2, &dummy0, &dummy1);
                AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4],
                    &pTile->m50[gMenuContext->m308_oddOrEven]);
                RotTransPers3(&pTile->m120[0], &pTile->m120[2], &pTile->m120[3],
                    &pTile->m80[gMenuContext->m308_oddOrEven].x0y0,
                    &pTile->m80[gMenuContext->m308_oddOrEven].x1y1,
                    &pTile->m80[gMenuContext->m308_oddOrEven].x2y2, &dummy0, &dummy1);
                AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4],
                    &pTile->m80[gMenuContext->m308_oddOrEven]);
            }
        }
    }
}

void drawMenu_4D8_text() {

    std::array<u8, 2> memoryCardHasData;
    memoryCardHasData[1] = 0;
    memoryCardHasData[0] = 0;

    if (gMenuContext->m4D8 != 0) {
        int x = 0x1e;
        for (int i = 0; i < 2; i++) {
            if ((gMenuContext->m32C_memoryCardContext->m4FE4[i] != 0) && (gMenuContext->m33C->m68 != 0)) {
                bool bIsValid = true;
                if (gMenuContext->m33C->m2F_draw428 != 0) {
                    int iVar1 = memoryCardTilesPosition2[gMenuContext->m32C_memoryCardContext->m4F7C];
                    if (iVar1 < 0) {
                        iVar1 = iVar1 + 0xf;
                    }
                    bIsValid = true;
                    if (i == iVar1 >> 4) {
                        bIsValid = false;
                    }
                    iVar1 = 0x115;
                    if (!bIsValid) {
                        iVar1 = 0x122;
                    }
                    setupStringInPolyFT4(gMenuContext->m2DC_font, iVar1, &gMenuContext->m32C_memoryCardContext->m4D94, gMenuContext->m308_oddOrEven, x, 0x36, 0x1000);
                    assert(0);
                    memoryCardHasData[i] = 1;
                }
            }
            if (memoryCardHasData[i] != 0) {
                assert(0);
            }
        }
    }
}

void initMenuContext3A8_MemoryCardPolys(int param_1) {
    auto pX = memoryCardTilesPositionX.begin() + param_1;
    auto pY = memoryCardTilesPositionY.begin() + param_1;
    auto pTile = gMenuContext->m3A8_memoryCardTiles[param_1];
    for (int i = 0; i < 2; i++) {
        initMemoryCardTransparentPoly(&pTile->m0_memoryCardTransparentPolys[i]);
        pTile->m0_memoryCardTransparentPolys[i].x0 = *pX;
        pTile->m0_memoryCardTransparentPolys[i].y0 = *pY;
        pTile->m0_memoryCardTransparentPolys[i].x1 = *pX + 0x10;
        pTile->m0_memoryCardTransparentPolys[i].y1 = *pY;
        pTile->m0_memoryCardTransparentPolys[i].x2 = *pX;
        pTile->m0_memoryCardTransparentPolys[i].y2 = *pY + 0x10;
        pTile->m0_memoryCardTransparentPolys[i].x3 = *pX + 0x10;
        pTile->m0_memoryCardTransparentPolys[i].y3 = *pY + 0x10;
        pTile->m0_memoryCardTransparentPolys[i].tpage = GetTPage(0, 0, 0x140, 0x80);
        SetPolyF4(&pTile->mB0_memoryCardOpaquePolys[i]);
        pTile->mB0_memoryCardOpaquePolys[i].r0 = 0x80;
        pTile->mB0_memoryCardOpaquePolys[i].g0 = 0x80;
        pTile->mB0_memoryCardOpaquePolys[i].b0 = 0x80;
        SetSemiTrans(&pTile->mB0_memoryCardOpaquePolys[i], 1);
        pTile->mB0_memoryCardOpaquePolys[i].x0y0.vx = *pX;
        pTile->mB0_memoryCardOpaquePolys[i].x0y0.vy = *pY;
        pTile->mB0_memoryCardOpaquePolys[i].x1y1.vx = *pX + 0x10;
        pTile->mB0_memoryCardOpaquePolys[i].x1y1.vy = *pY;
        pTile->mB0_memoryCardOpaquePolys[i].x2y2.vx = *pX;
        pTile->mB0_memoryCardOpaquePolys[i].x2y2.vy = *pY + 0x10;
        pTile->mB0_memoryCardOpaquePolys[i].x3y3.vx = *pX + 0x10;
        pTile->mB0_memoryCardOpaquePolys[i].x3y3.vy = *pY + 0x10;
        initMemoryCardPolyVerts(pTile->mE0_vertices, *pX, *pY, 0x10, 0x10);
    }

    RECT tempRect;
    tempRect.y = 0;
    tempRect.x = 0;
    tempRect.h = 0x100;
    tempRect.w = 0x100;

    u16 tPage = GetTPage(0, 2, 0x140, 0x80);
    SetDrawMode(&pTile->m140[0], 0, 0, tPage, &tempRect);
    tPage = GetTPage(0, 2, 0x140, 0x80);
    SetDrawMode(&pTile->m140[1], 0, 0, tPage, &tempRect);
}

void initMenuContext3A8_memoryCardDisplay(void)
{
    for(int i=0; i<0x20; i++) {
        gMenuContext->m3A8_memoryCardTiles[i] = new sMenuContext_3A8;
        memset(gMenuContext->m3A8_memoryCardTiles[i], 0, sizeof(sMenuContext_3A8));
        initMenuContext3A8_MemoryCardPolys(i);
        initMenuContext3A8_Lines(i);
    }
}