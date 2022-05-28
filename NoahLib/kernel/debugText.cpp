#include "noahLib.h"
#include "debugText.h"
#include "debugFont.h"
#include "decompress.h"
#include "TIM.h"
#include "graphics.h"
#include <stdarg.h>

RECT debugFontClutRect;

std::array<u16, 0x40> debugTextPalette;

sDebugText* debugFontRam = nullptr;

void initDebugFontClut(ushort param_1, ushort param_2) {
    bool bVar1;
    int iVar2;
    ushort* puVar3;
    int iVar4;

    puVar3 = &debugTextPalette[0];
    iVar4 = 0;
    do {
        *puVar3 = param_2;
        puVar3[1] = param_1;
        iVar4 += 1;
        puVar3 = puVar3 + 2;
    } while (iVar4 < 8);
    bVar1 = iVar4 < 0xe;
    if (iVar4 < 0xc) {
        do {
            *puVar3 = param_2;
            puVar3[1] = param_2;
            puVar3[2] = param_1;
            puVar3[3] = param_1;
            iVar4 += 1;
            puVar3 = puVar3 + 4;
        } while (iVar4 < 0xc);
        bVar1 = iVar4 < 0xe;
    }
    while (iVar2 = 3, bVar1) {
        do {
            *puVar3 = param_2;
            iVar2 += -1;
            puVar3 = puVar3 + 1;
        } while (-1 < iVar2);
        iVar2 = 3;
        do {
            *puVar3 = param_1;
            iVar2 += -1;
            puVar3 = puVar3 + 1;
        } while (-1 < iVar2);
        iVar4 += 1;
        bVar1 = iVar4 < 0xe;
    }
    iVar4 = 7;
    do {
        *puVar3 = param_2;
        iVar4 += -1;
        puVar3 = puVar3 + 1;
    } while (-1 < iVar4);
    iVar4 = 8;
    do {
        *puVar3 = param_1;
        iVar4 += 1;
        puVar3 = puVar3 + 1;
    } while (iVar4 < 0x10);
    LoadImage(&debugFontClutRect, (u8*)&debugTextPalette[0]);
}

void resetDebugPrint(void)
{
    debugFontRam->m34_numPrims = 0;
    debugFontRam->m32 = debugFontRam->mE;
    debugFontRam->mCE = debugFontRam->mE;
    debugFontRam->m18_colorAndCode.m3_code &= 0xFE;
    debugFontRam->m30 = debugFontRam->mC;
    debugFontRam->mCC = debugFontRam->mC;
    debugFontRam->m36 = debugFontRam->mC;
    debugFontRam->mD0 = debugFontRam->mC;
    debugFontRam->m38_pCurrentPrim = debugFontRam->m4_Buffer[debugFontRam->m2E & 1];
}

sDebugText* initDebugFont(uint x, ushort y, uint w, ushort h, int bufferSize, ushort flags, uint tpageX, int tpageY, uint clutX, int clutY, void* font) {

    sDebugText* pNewDebugText = new sDebugText;
    pNewDebugText->buffer.resize(flags & 1 ? bufferSize : bufferSize * 2);

    pNewDebugText->m4_Buffer[0] = pNewDebugText->buffer.begin();
    pNewDebugText->m4_Buffer[1] = flags & 1 ? pNewDebugText->buffer.begin() : pNewDebugText->buffer.begin() + bufferSize;

    if (font) {
        assert(0);
    }

    std::vector<u8> pDecompressedDebugFont = mallocAndDecompress(debugFont_bin.begin(), debugFont_bin.size());

    u8 bVar1 = pDecompressedDebugFont[0];
    u8 bVar2 = bVar1 & 2;
    
    pNewDebugText->m14 = pDecompressedDebugFont[2];
    pNewDebugText->m16 = pDecompressedDebugFont[3];

    if ((flags & 2) != 0) {
        bVar2 = 0;
    }
    pNewDebugText->mC = x;
    pNewDebugText->m30 = x;
    pNewDebugText->m0 = flags;
    pNewDebugText->mE = y;
    pNewDebugText->m32 = y;
    pNewDebugText->m10 = w;
    pNewDebugText->m18_colorAndCode.m0_r = 0xFF;
    pNewDebugText->m18_colorAndCode.m1_g = 0xFF;
    pNewDebugText->m18_colorAndCode.m2_b = 0xFF;
    pNewDebugText->m2C_maxPrims = bufferSize;
    pNewDebugText->m2E = 0;
    pNewDebugText->m12 = h;

    if (pDecompressedDebugFont[0] & 1) {
        pNewDebugText->m18_colorAndCode.m3_code = 0x75;
    }
    else {
        pNewDebugText->m18_colorAndCode.m3_code = 0x7D;
    }

    pNewDebugText->mD2 = (char)tpageY;
    if (bVar2 == 0) {
        pNewDebugText->m2E = pNewDebugText->m2E | 4;
    }

    s32 uVar5;
    if ((bVar1 & 1) == 0) {
        uVar5 = 8;
        if (bVar2 != 0) {
            uVar5 = 0x10;
        }
    }
    else {
        uVar5 = 0x20;
        pNewDebugText->m2E = pNewDebugText->m2E | 2;
        if (bVar2 != 0) {
            uVar5 = 0x30;
        }
    }

    RECT loadRect;
    loadRect.w = 0x20;
    loadRect.h = uVar5;
    loadRect.x = tpageX;
    loadRect.y = tpageY;

    std::vector<u8>::iterator textureStart = pDecompressedDebugFont.begin() + 4;

    if (pNewDebugText->m14 == 0) {
        assert(0);
    }

    LoadImage(&loadRect, &textureStart[0]);

    pNewDebugText->m2 = GetTPage(0, 0, tpageX, tpageY);

    loadRect.w = 0x40;
    loadRect.h = 1;
    loadRect.x = clutX;
    loadRect.y = clutY;

    pNewDebugText->m3C_cluts[0] = GetClut(clutX, clutY);
    pNewDebugText->m3C_cluts[1] = GetClut(clutX + 0x10, clutY);
    pNewDebugText->m3C_cluts[2] = GetClut(clutX + 0x20, clutY);
    pNewDebugText->m3C_cluts[3] = GetClut(clutX + 0x30, clutY);

    debugFontClutRect = loadRect;

    initDebugFontClut(0x7fff, 0);
    SetDrawTPage(&pNewDebugText->m1C_DR_TPage[0], 0, 0, pNewDebugText->m2);
    SetDrawTPage(&pNewDebugText->m1C_DR_TPage[1], 0, 0, pNewDebugText->m2);

    SetTile(&pNewDebugText->m44_tiles[0]);
    pNewDebugText->m44_tiles[0].r0 = 0;
    pNewDebugText->m44_tiles[0].g0 = 0;
    pNewDebugText->m44_tiles[0].b0 = 0;
    pNewDebugText->m44_tiles[0].x0 = x;
    pNewDebugText->m44_tiles[0].y0 = y;
    pNewDebugText->m44_tiles[0].w = w;
    pNewDebugText->m44_tiles[0].h = h;
    SetSemiTrans(&pNewDebugText->m44_tiles[0], 1);
    pNewDebugText->m44_tiles[1] = pNewDebugText->m44_tiles[0];
    debugFontRam = pNewDebugText;

    resetDebugPrint();

    return pNewDebugText;
}

void addDebugTextPrim(int param_1) {
    u16* psVar1;
    ushort* puVar2;
    byte bVar3;
    sDebugText* psVar4;
    short sVar5;
    ushort uVar6;
    uint uVar7;
    uint uVar8;

    psVar4 = debugFontRam;
    if (((debugFontRam != (sDebugText*)0x0) && ((int)debugFontRam->m32 + (int)(short)debugFontRam->m16 <= (int)(short)debugFontRam->mE + (int)(short)debugFontRam->m12)) &&
        (debugFontRam->m34_numPrims <= debugFontRam->m2C_maxPrims)) {
        if (param_1 < 0x20) {
            if (param_1 == 10) {
                psVar1 = &debugFontRam->m32;
                puVar2 = &debugFontRam->m16;
                debugFontRam->m30 = debugFontRam->m36;
                psVar4->m32 = *psVar1 + *puVar2;
            }
        }
        else {
            if (((debugFontRam->m2E & 4) != 0) && (0x5f < param_1)) {
                param_1 += -0x20;
            }
            uVar8 = param_1 - 0x20;
            if ((debugFontRam->m2E & 8) == 0) {
                uVar7 = (uint)debugFontRam->m14;
            }
            else {
                assert(0);
                //uVar7 = (uint) * (byte*)((int)&debugFontRam->m44_tiles[0].tag + param_1);
            }
            if ((int)debugFontRam->mC + (int)debugFontRam->m10 <= (int)((int)debugFontRam->m30 + uVar7)) {
                if ((debugFontRam->m0 & 8U) != 0) {
                    return;
                }
                psVar1 = &debugFontRam->m32;
                puVar2 = &debugFontRam->m16;
                debugFontRam->m30 = debugFontRam->m36;
                psVar4->m32 = *psVar1 + *puVar2;
            }
            if (uVar8 != 0) {
                psVar4->m38_pCurrentPrim->m_colorAndCode = psVar4->m18_colorAndCode;
                psVar4->m38_pCurrentPrim->x0 = psVar4->m30;
                psVar4->m38_pCurrentPrim->y0 = psVar4->m32;

                if ((psVar4->m2E & 2) == 0) {
                    uVar6 = (ushort)((uVar8 & 0xf) << 3);
                    psVar4->m38_pCurrentPrim->clut = psVar4->m3C_cluts[(uVar8 & 0x30) >> 4];
                    bVar3 = psVar4->mD2;
                    sVar5 = (short)((int)(uVar8 & 0xc0) >> 3);
                }
                else {
                    uVar6 = (ushort)((uVar8 & 7) << 4);
                    psVar4->m38_pCurrentPrim->clut = psVar4->m3C_cluts[(uVar8&0x18) >>3];
                    bVar3 = psVar4->mD2;
                    sVar5 = (short)((int)(uVar8 & 0x60) >> 1);
                }
                psVar4->m38_pCurrentPrim->u0 = uVar6;
                psVar4->m38_pCurrentPrim->v0 = bVar3 + sVar5;

                psVar4->m38_pCurrentPrim = psVar4->m38_pCurrentPrim + 1;
                psVar4->m34_numPrims = psVar4->m34_numPrims + 1;
            }
            psVar4->m30 = psVar4->m30 + (short)uVar7;
        }
    }
}

void (*pAddDebugTextPrim)(int) = addDebugTextPrim;

void callAddDebugTextPrim(int param_1)
{
    pAddDebugTextPrim(param_1);
}

void logToScreenProcessString(int, const char* format, va_list args) {
    char string[1024];
    {
        vsprintf(string, format, args);
    }

    char* current = &string[0];
    while (*current) {
        callAddDebugTextPrim(*current);
        current++;
    }
}

void logToScreen(const char* fmt, ...) {
    if (debugFontRam != nullptr) {
        va_list args;
        va_start(args, fmt);
        logToScreenProcessString(0, fmt, args);
        va_end(args);
    }
}

void addDebugTextPrim2(sTag* param_1, sTag* param_2)
{
    sTag* puVar1;

    puVar1 = param_1->m0_pNext;
    param_1->m0_pNext = param_2;
    param_2->m0_pNext = puVar1;
    param_2->m3_size = 3;
    return;
}

void printDebugTextOnScreen(sTag* param_1) {
    SPRT_8* pSVar1;
    sDebugText* psVar2;
    ushort uVar3;
    int iVar4;
    uint uVar5;
    bool shouldDraw;

    psVar2 = debugFontRam;
    if (debugFontRam != nullptr) {
        shouldDraw = false;
        if ((debugFontRam->m0 & 1U) == 0) {
            uVar3 = debugFontRam->m2E;
            uVar5 = uVar3 & 1;
            if ((uVar3 & 1) == 0) {
                uVar3 |= 1;
            }
            else {
                uVar3 &= 0xfffe;
            }
        }
        else {
            uVar5 = 0;
            uVar3 = debugFontRam->m2E & 0xfffe;
        }
        debugFontRam->m2E = uVar3;
        if ((param_1 == (sTag*)0x0) || (param_1 == (sTag*)0xffffffff)) {
            DrawSync(0);
            assert(0);
            //param_1 = (sTag*)(psVar2->m64 + uVar5 * 4 + 0x60);
            shouldDraw = true;
            TermPrim(param_1);
        }
        pSVar1 = &psVar2->m4_Buffer[uVar5][0];
        for (iVar4 = (int)psVar2->m34_numPrims; iVar4 != 0; iVar4 += -1) {
            addDebugTextPrim2(param_1, pSVar1);
            pSVar1 = pSVar1 + 1;
        }
        AddPrim(param_1, &psVar2->m1C_DR_TPage[uVar5]);
        if ((psVar2->m0 & 0x10U) != 0) {
            //FUN_80031804(param_1, psVar2->m44_tiles + uVar5);
            assert(0);
        }
        resetDebugPrint();
        if (shouldDraw) {
            DrawOTag(param_1);
        }
    }
}