#include "noahLib.h"
#include "battle/battle.h"
#include "battle/battleDialogWindow.h"
#include "field/dialogWindows.h"
#include "battle/battleRenderContext.h"

std::array<sBattleDialogWindow*, 7> battleDialogWindows;
std::array<sBattleDialogWindow2*, 7> battleDialogWindows2;

void setupBattleDialogWindow_sub0(std::array<std::array<POLY_FT4, 2>, 2>& param_1, uint param_2) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            POLY_FT4* p = &param_1[i][j];
            SetPolyFT4(p);
            SetShadeTex(p, 1);
            p->r0 = 0xFF;
            p->g0 = 0xFF;
            p->b0 = 0xFF;
            p->tpage = GetTPage(battleVar0->mA234[param_2].m4_tpage_tp, 0, battleVar0->mA234[param_2].m10_tpage_X, battleVar0->mA234[param_2].m14_tpage_Y);
            p->clut = GetClut(battleVar0->mA234[param_2].m8_clutX, battleVar0->mA234[param_2].mC_clutY);
        }
    }
}

void setupBattleDialogWindow(byte param_1) {
    u16 uVar1;
    sBattleDialogWindow* psVar2;
    byte bVar3;
    byte bVar4;

    psVar2 = battleDialogWindows[param_1 & 0xff];
    bVar3 = 0;
    do {
        SetPolyG4(&psVar2->m3C0_polyG4[bVar3]);
        psVar2->m3C0_polyG4[bVar3].r0 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[bVar3].g0 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[bVar3].b0 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[bVar3].r1 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[bVar3].g1 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[bVar3].b1 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[bVar3].r2 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[bVar3].g2 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[bVar3].b2 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[bVar3].r3 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[bVar3].g3 = dialogWindowColor[1];
        bVar4 = bVar3 + 1;
        psVar2->m3C0_polyG4[bVar3].b3 = dialogWindowColor[2];
        SetSemiTrans(&psVar2->m3C0_polyG4[bVar3], 1);
        uVar1 = GetTPage(0, dialogBattleWindowAbr, battleVar0->mA234[1].m10_tpage_X, battleVar0->mA234[1].m14_tpage_Y);
        SetDrawMode(&psVar2->m408_drawMode[bVar3], 0, 0, (uint)uVar1, &battleVar1->m0);
        bVar3 = bVar4;
    } while (bVar4 < 2);
    setupBattleDialogWindow_sub0(psVar2->m140_poly1, 1);
    setupBattleDialogWindow_sub0(psVar2->m1E0_poly2, 2);
    setupBattleDialogWindow_sub0(psVar2->m280_poly3, 3);
    setupBattleDialogWindow_sub0(psVar2->m320_poly4, 4);
}

void drawBattleDialogWindow(uint param_1, short x, short y, short width, short height) {
    sBattleDialogWindow* psVar1;
    uint uVar2;

    uVar2 = param_1 & 0xff;
    psVar1 = battleDialogWindows[uVar2];
    battleVar1->mB0_isDialogWindowInitialized[uVar2] = 0;
    psVar1->m3C0_polyG4[battleOddOrEven].x0 = x;
    psVar1->m3C0_polyG4[battleOddOrEven].y0 = y;
    psVar1->m3C0_polyG4[battleOddOrEven].x1 = x + width;
    psVar1->m3C0_polyG4[battleOddOrEven].y1 = y;
    psVar1->m3C0_polyG4[battleOddOrEven].x2 = x;
    psVar1->m3C0_polyG4[battleOddOrEven].y2 = y + height;
    psVar1->m3C0_polyG4[battleOddOrEven].x3 = x + width;
    psVar1->m3C0_polyG4[battleOddOrEven].y3 = y + height;
    /*FUN_Battle__8008dc34(uVar2, x, y, width, height);
    FUN_Battle__8008de04(uVar2, x, y, width);
    FUN_Battle__8008e430(uVar2, x, y, width, height);
    FUN_Battle__8008ea70(uVar2, x, y, height);
    FUN_Battle__8008f0a8(uVar2, x, y, width, height);
    */
    MissingCode();
    psVar1->m5A4_oddOrEven = (byte)battleOddOrEven;
    battleVar1->mB0_isDialogWindowInitialized[uVar2] = 1;
}

void allocateAndSetupBattleDialogWindow(byte param_1, short param_2, short param_3, short param_4, short param_5, char param_6, char param_7) {
    if (battleVar1->mB0_isDialogWindowInitialized[param_1] == 0) {
        battleDialogWindows[param_1] = new sBattleDialogWindow;
        memset(battleDialogWindows[param_1], 0, sizeof(sBattleDialogWindow));

        battleDialogWindows2[param_1] = new sBattleDialogWindow2;
        memset(battleDialogWindows2[param_1], 0, sizeof(sBattleDialogWindow2));

        setupBattleDialogWindow(param_1);
    }
    if (param_6 == '\0') {
        drawBattleDialogWindow(param_1, param_2, param_3, param_4, param_5);
        if (param_7 != '\0') {
            battleRenderDebugAndMain();
        }
    }
    else {
        auto& psVar3 = battleDialogWindows2[param_1];
        psVar3->mC_windowIndex = param_1;
        psVar3->m0 = param_2;
        psVar3->m2 = param_3;
        psVar3->m4 = param_4;
        psVar3->m6 = param_5;
        psVar3->m8 = 0;
        psVar3->mA = 0;
        battleVar1->mBF_isDialogWindowInitialized3[param_1] = 0;
        battleVar1->mB8_isDialogWindowInitialized2[param_1] = 1;
    }
}