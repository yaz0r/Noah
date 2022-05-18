#include "noahLib.h"
#include "kernel/gameState.h"
#include "menus/menuHandler.h"
#include "menus/menuGold.h"

void updateGoldPolys(short x, short y)
{
    int iVar2;
    int iVar3;
    short x_00;

    fillStringDisplayBuffer(gameState.m1924_Gold);
    iVar3 = 0;
    gMenuContext->m340_gold->m320_goldPolyLength = 0;
    x_00 = x;
    do {
        u8 character = gMenuContext->m31C_stringDisplayBuffer[iVar3];
        iVar3 = iVar3 + 1;
        if (character != 0xff) {
            iVar2 = setupStringInPolyFT4
            (gMenuContext->m2DC_font, character, &gMenuContext->m340_gold->m0_goldPolys[gMenuContext->m340_gold->m320_goldPolyLength], gMenuContext->m308_oddOrEven, x_00, y,
                0x1000);
            gMenuContext->m340_gold->m320_goldPolyLength = iVar2 + gMenuContext->m340_gold->m320_goldPolyLength;
        }
        x_00 = x_00 + 8;
    } while (iVar3 < 9);
    setupStringInPolyFT4(gMenuContext->m2DC_font, 0x10, &gMenuContext->m340_gold->m2D0, gMenuContext->m308_oddOrEven, x + 0x50, y, 0x1000);
    gMenuContext->m33C->m5_drawGold = 1;
    gMenuContext->m340_gold->m324_oddOrEven = gMenuContext->m308_oddOrEven;
    return;
}


void updateGoldDisplay() {
    iniMenuContext364And380(0, 0xd4, 0xb2, 0x60, 0x10, 0, 0, 4, 0);
    updateGoldPolys(0xd8, 0xb6);
}