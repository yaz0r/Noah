#include "noahLib.h"
#include "battle/battle.h"
#include "battle/battleDialogWindow.h"
#include "field/dialogWindows.h"
#include "battle/battleRenderContext.h"

std::array<sBattleDialogWindow*, 7> battleDialogWindows;
std::array<sBattleDialogWindow2*, 7> battleDialogWindows2;

void setupBattleDialogWindow_sub0(std::array<POLY_FT4, 2>* param_1, uint param_2) {
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
    byte bVar4;

    psVar2 = battleDialogWindows[param_1 & 0xff];
    for (int i = 0; i < 2; i++) {
        SetPolyG4(&psVar2->m3C0_polyG4[i]);
        psVar2->m3C0_polyG4[i].r0 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[i].g0 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[i].b0 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[i].r1 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[i].g1 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[i].b1 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[i].r2 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[i].g2 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[i].b2 = dialogWindowColor[2];
        psVar2->m3C0_polyG4[i].r3 = dialogWindowColor[0];
        psVar2->m3C0_polyG4[i].g3 = dialogWindowColor[1];
        psVar2->m3C0_polyG4[i].b3 = dialogWindowColor[2];
        SetSemiTrans(&psVar2->m3C0_polyG4[i], 1);
        uVar1 = GetTPage(0, dialogBattleWindowAbr, battleVar0->mA234[1].m10_tpage_X, battleVar0->mA234[1].m14_tpage_Y);
        SetDrawMode(&psVar2->m408_drawMode[i], 0, 0, (uint)uVar1, &battleVar1->m0);
    }
    setupBattleDialogWindow_sub0(&psVar2->m140[0], 1);
    setupBattleDialogWindow_sub0(&psVar2->m1E0[0], 2);
    setupBattleDialogWindow_sub0(&psVar2->m280[0], 3);
    setupBattleDialogWindow_sub0(&psVar2->m320[0], 4);
}

void drawBattleDialogWindow_sub0Sub(POLY_FT4* param_1)
{
    ushort uVar1;

    battleSetupTextPolySub(param_1);
    param_1->r0 = 0x80;
    param_1->g0 = 0x80;
    param_1->b0 = 0x80;
    if (dialogBattleWindowAbr == 0) {
        uVar1 = param_1->tpage & 0xffbf;
    }
    else {
        uVar1 = param_1->tpage | 0x40;
    }
    param_1->tpage = uVar1;
    return;
}


void drawBattleDialogWindow_sub0(uint param_1, short param_2, short param_3, short param_4, short param_5) {
    u8 sprite0;
    u8 sprite1;
    u8 sprite2;
    u8 sprite3;

    sBattleDialogWindow* psVar6 = battleDialogWindows[param_1 & 0xff];
    if (currentBattleMode == 0) {
        sprite0 = 0xf0;
        sprite1 = 0xf2;
        sprite2 = 0xf5;
        sprite3 = 0xf7;
    }
    else {
        sprite0 = 0x4a;
        sprite1 = 0x4c;
        sprite2 = 0x4f;
        sprite3 = 0x51;
    }
    psVar6->m5A0 = 0;
    psVar6->m5A0 += battleSetupStringInPolyFT4Large(sprite0, &psVar6->m0_corners[psVar6->m5A0], param_2, param_3);
    psVar6->m5A0 += battleSetupStringInPolyFT4Large(sprite1, &psVar6->m0_corners[psVar6->m5A0], param_2 + param_4 + -8, param_3);
    psVar6->m5A0 += battleSetupStringInPolyFT4Large(sprite2, &psVar6->m0_corners[psVar6->m5A0], param_2, param_3 + param_5 - 8);
    psVar6->m5A0 += battleSetupStringInPolyFT4Large(sprite3, &psVar6->m0_corners[psVar6->m5A0], param_2 + param_4 + -8, param_3 + param_5 - 8);
    
    for (int i = 0; i < 4; i++) {
        drawBattleDialogWindow_sub0Sub(&psVar6->m0_corners[i][battleOddOrEven]);
    }
}

void drawBattleDialogWindow_sub1(uint param_1, short param_2, short param_3, uint param_4)
{
    int iVar1;
    short sVar2;
    short sVar3;
    short sVar4;
    int iVar5;
    sBattleDialogWindow* psVar6;

    param_2 = param_2 + 8;
    psVar6 = battleDialogWindows[param_1 & 0xff];
    sVar2 = param_3 + -8;
    psVar6->m140[0][battleOddOrEven].x0 = param_2;
    psVar6->m140[0][battleOddOrEven].y0 = sVar2;
    sVar3 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    sVar4 = param_2 + sVar3;
    psVar6->m140[0][battleOddOrEven].x1 = sVar4;
    psVar6->m140[0][battleOddOrEven].y1 = sVar2;
    psVar6->m140[0][battleOddOrEven].x2 = param_2;
    param_3 = param_3 + 8;
    psVar6->m140[0][battleOddOrEven].y2 = param_3;
    psVar6->m140[0][battleOddOrEven].x3 = sVar4;
    psVar6->m140[0][battleOddOrEven].y3 = param_3;
    psVar6->m140[1][battleOddOrEven].x0 = sVar4;
    psVar6->m140[1][battleOddOrEven].y0 = sVar2;
    sVar3 = sVar4 + sVar3;
    psVar6->m140[1][battleOddOrEven].x1 = sVar3;
    psVar6->m140[1][battleOddOrEven].y1 = sVar2;
    psVar6->m140[1][battleOddOrEven].x2 = sVar4;
    psVar6->m140[1][battleOddOrEven].y2 = param_3;
    psVar6->m140[1][battleOddOrEven].x3 = sVar3;
    psVar6->m140[1][battleOddOrEven].y3 = param_3;
    psVar6->m140[0][battleOddOrEven].u0 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) << 1;
    psVar6->m140[0][battleOddOrEven].v0 = battleVar0->mA234[1].m14_tpage_Y;
    psVar6->m140[0][battleOddOrEven].u1 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) * '\x02' + '\a';
    psVar6->m140[0][battleOddOrEven].v1 = battleVar0->mA234[1].m14_tpage_Y;
    psVar6->m140[0][battleOddOrEven].u2 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) << 1;
    psVar6->m140[0][battleOddOrEven].v2 = battleVar0->mA234[1].m14_tpage_Y + '\x10';
    psVar6->m140[0][battleOddOrEven].u3 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) * '\x02' + '\a';
    psVar6->m140[0][battleOddOrEven].v3 = battleVar0->mA234[1].m14_tpage_Y + '\x10';
    psVar6->m140[1][battleOddOrEven].u0 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) << 1;
    psVar6->m140[1][battleOddOrEven].v0 = battleVar0->mA234[1].m14_tpage_Y;
    psVar6->m140[1][battleOddOrEven].u1 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) * '\x02' + '\a';
    psVar6->m140[1][battleOddOrEven].v1 = battleVar0->mA234[1].m14_tpage_Y;
    psVar6->m140[1][battleOddOrEven].u2 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) << 1;
    psVar6->m140[1][battleOddOrEven].v2 = battleVar0->mA234[1].m14_tpage_Y + '\x10';
    psVar6->m140[1][battleOddOrEven].u3 = (battleVar0->mA234[1].m10_tpage_X & 0x3f) * '\x02' + '\a';
    psVar6->m140[1][battleOddOrEven].v3 = battleVar0->mA234[1].m14_tpage_Y + '\x10';
    for (int i = 0; i < 2; i++) {
        drawBattleDialogWindow_sub0Sub(&psVar6->m140[i][battleOddOrEven]);
    }
}

void drawBattleDialogWindow_sub2
(uint param_1, short param_2, short param_3, uint param_4, short param_5)

{
    int iVar1;
    short sVar2;
    short sVar3;
    short sVar4;
    short sVar5;
    int iVar6;
    sBattleDialogWindow* psVar7;

    param_2 = param_2 + 8;
    psVar7 = battleDialogWindows[param_1 & 0xff];
    psVar7->m1E0[0][battleOddOrEven].x0 = param_2;
    sVar5 = param_3 + param_5 + -8;
    psVar7->m1E0[0][battleOddOrEven].y0 = sVar5;
    sVar4 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    sVar2 = param_2 + sVar4;
    psVar7->m1E0[0][battleOddOrEven].x1 = sVar2;
    psVar7->m1E0[0][battleOddOrEven].y1 = sVar5;
    psVar7->m1E0[0][battleOddOrEven].x2 = param_2;
    sVar3 = param_3 + param_5 + 8;
    psVar7->m1E0[0][battleOddOrEven].y2 = sVar3;
    psVar7->m1E0[0][battleOddOrEven].x3 = sVar2;
    psVar7->m1E0[0][battleOddOrEven].y3 = sVar3;
    psVar7->m1E0[1][battleOddOrEven].x0 = sVar2;
    psVar7->m1E0[1][battleOddOrEven].y0 = sVar5;
    sVar4 = sVar2 + sVar4;
    psVar7->m1E0[1][battleOddOrEven].x1 = sVar4;
    psVar7->m1E0[1][battleOddOrEven].y1 = sVar5;
    psVar7->m1E0[1][battleOddOrEven].x2 = sVar2;
    psVar7->m1E0[1][battleOddOrEven].y2 = sVar3;
    psVar7->m1E0[1][battleOddOrEven].x3 = sVar4;
    psVar7->m1E0[1][battleOddOrEven].y3 = sVar3;
    psVar7->m1E0[0][battleOddOrEven].u0 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xf8;
    psVar7->m1E0[0][battleOddOrEven].v0 = battleVar0->mA234[2].m14_tpage_Y;
    psVar7->m1E0[0][battleOddOrEven].u1 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xff;
    psVar7->m1E0[0][battleOddOrEven].v1 = battleVar0->mA234[2].m14_tpage_Y;
    psVar7->m1E0[0][battleOddOrEven].u2 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xf8;
    psVar7->m1E0[0][battleOddOrEven].v2 = battleVar0->mA234[2].m14_tpage_Y + '\x10';
    psVar7->m1E0[0][battleOddOrEven].u3 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xff;
    psVar7->m1E0[0][battleOddOrEven].v3 = battleVar0->mA234[2].m14_tpage_Y + '\x10';
    psVar7->m1E0[1][battleOddOrEven].u0 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xf8;
    psVar7->m1E0[1][battleOddOrEven].v0 = battleVar0->mA234[2].m14_tpage_Y;
    psVar7->m1E0[1][battleOddOrEven].u1 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xff;
    psVar7->m1E0[1][battleOddOrEven].v1 = battleVar0->mA234[2].m14_tpage_Y;
    psVar7->m1E0[1][battleOddOrEven].u2 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xf8;
    psVar7->m1E0[1][battleOddOrEven].v2 = battleVar0->mA234[2].m14_tpage_Y + '\x10';
    psVar7->m1E0[1][battleOddOrEven].u3 =
        (battleVar0->mA234[2].m10_tpage_X & 0x3f) * '\x02' + 0xff;
    psVar7->m1E0[1][battleOddOrEven].v3 = battleVar0->mA234[2].m14_tpage_Y + '\x10';
    for (int i = 0; i < 2; i++) {
        drawBattleDialogWindow_sub0Sub(&psVar7->m1E0[i][battleOddOrEven]);
    }
}

void drawBattleDialogWindow_sub3(uint param_1, short param_2, short param_3, uint param_4)

{
    short sVar1;
    int iVar2;
    short sVar3;
    int iVar4;
    sBattleDialogWindow* psVar5;

    psVar5 = battleDialogWindows[param_1 & 0xff];
    sVar3 = param_2 + -8;
    psVar5->m280[0][battleOddOrEven].x0 = sVar3;
    param_3 = param_3 + 8;
    psVar5->m280[0][battleOddOrEven].y0 = param_3;
    param_2 = param_2 + 8;
    psVar5->m280[0][battleOddOrEven].x1 = param_2;
    psVar5->m280[0][battleOddOrEven].y1 = param_3;
    psVar5->m280[0][battleOddOrEven].x2 = sVar3;
    sVar1 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    param_3 = param_3 + sVar1;
    psVar5->m280[0][battleOddOrEven].y2 = param_3;
    psVar5->m280[0][battleOddOrEven].x3 = param_2;
    psVar5->m280[0][battleOddOrEven].y3 = param_3;
    psVar5->m280[1][battleOddOrEven].x0 = sVar3;
    psVar5->m280[1][battleOddOrEven].y0 = param_3;
    psVar5->m280[1][battleOddOrEven].x1 = param_2;
    psVar5->m280[1][battleOddOrEven].y1 = param_3;
    psVar5->m280[1][battleOddOrEven].x2 = sVar3;
    param_3 = param_3 + sVar1;
    psVar5->m280[1][battleOddOrEven].y2 = param_3;
    psVar5->m280[1][battleOddOrEven].x3 = param_2;
    psVar5->m280[1][battleOddOrEven].y3 = param_3;
    psVar5->m280[0][battleOddOrEven].u0 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar5->m280[0][battleOddOrEven].v0 = battleVar0->mA234[3].m14_tpage_Y;
    psVar5->m280[0][battleOddOrEven].u1 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar5->m280[0][battleOddOrEven].v1 = battleVar0->mA234[3].m14_tpage_Y;
    psVar5->m280[0][battleOddOrEven].u2 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar5->m280[0][battleOddOrEven].v2 = battleVar0->mA234[3].m14_tpage_Y + '\a';
    psVar5->m280[0][battleOddOrEven].u3 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar5->m280[0][battleOddOrEven].v3 = battleVar0->mA234[3].m14_tpage_Y + '\a';
    psVar5->m280[1][battleOddOrEven].u0 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar5->m280[1][battleOddOrEven].v0 = battleVar0->mA234[3].m14_tpage_Y;
    psVar5->m280[1][battleOddOrEven].u1 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar5->m280[1][battleOddOrEven].v1 = battleVar0->mA234[3].m14_tpage_Y;
    psVar5->m280[1][battleOddOrEven].u2 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar5->m280[1][battleOddOrEven].v2 = battleVar0->mA234[3].m14_tpage_Y + '\a';
    psVar5->m280[1][battleOddOrEven].u3 = (battleVar0->mA234[3].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar5->m280[1][battleOddOrEven].v3 = battleVar0->mA234[3].m14_tpage_Y + '\a';
    for(int i=0; i<2; i++) {
        drawBattleDialogWindow_sub0Sub(&psVar5->m280[i][battleOddOrEven]);
    }
}


void drawBattleDialogWindow_sub4
(uint param_1, short param_2, short param_3, short param_4, ushort param_5)

{
    short sVar1;
    int iVar2;
    short sVar3;
    short sVar4;
    int iVar5;
    sBattleDialogWindow* psVar6;

    sVar4 = param_2 + param_4 + -8;
    psVar6 = battleDialogWindows[param_1 & 0xff];
    psVar6->m320[0][battleOddOrEven].x0 = sVar4;
    param_3 = param_3 + 8;
    psVar6->m320[0][battleOddOrEven].y0 = param_3;
    sVar3 = param_2 + param_4 + 8;
    psVar6->m320[0][battleOddOrEven].x1 = sVar3;
    psVar6->m320[0][battleOddOrEven].y1 = param_3;
    psVar6->m320[0][battleOddOrEven].x2 = sVar4;
    sVar1 = (short)((int)(param_5 - 0x10) / 2);
    param_3 = param_3 + sVar1;
    psVar6->m320[0][battleOddOrEven].y2 = param_3;
    psVar6->m320[0][battleOddOrEven].x3 = sVar3;
    psVar6->m320[0][battleOddOrEven].y3 = param_3;
    psVar6->m320[1][battleOddOrEven].x0 = sVar4;
    psVar6->m320[1][battleOddOrEven].y0 = param_3;
    psVar6->m320[1][battleOddOrEven].x1 = sVar3;
    psVar6->m320[1][battleOddOrEven].y1 = param_3;
    psVar6->m320[1][battleOddOrEven].x2 = sVar4;
    param_3 = param_3 + sVar1;
    psVar6->m320[1][battleOddOrEven].y2 = param_3;
    psVar6->m320[1][battleOddOrEven].x3 = sVar3;
    psVar6->m320[1][battleOddOrEven].y3 = param_3;
    psVar6->m320[0][battleOddOrEven].u0 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar6->m320[0][battleOddOrEven].v0 = battleVar0->mA234[4].m14_tpage_Y;
    psVar6->m320[0][battleOddOrEven].u1 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar6->m320[0][battleOddOrEven].v1 = battleVar0->mA234[4].m14_tpage_Y;
    psVar6->m320[0][battleOddOrEven].u2 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar6->m320[0][battleOddOrEven].v2 = battleVar0->mA234[4].m14_tpage_Y + '\a';
    psVar6->m320[0][battleOddOrEven].u3 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar6->m320[0][battleOddOrEven].v3 = battleVar0->mA234[4].m14_tpage_Y + '\a';
    psVar6->m320[1][battleOddOrEven].u0 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar6->m320[1][battleOddOrEven].v0 = battleVar0->mA234[4].m14_tpage_Y;
    psVar6->m320[1][battleOddOrEven].u1 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar6->m320[1][battleOddOrEven].v1 = battleVar0->mA234[4].m14_tpage_Y;
    psVar6->m320[1][battleOddOrEven].u2 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x0e';
    psVar6->m320[1][battleOddOrEven].v2 = battleVar0->mA234[4].m14_tpage_Y + '\a';
    psVar6->m320[1][battleOddOrEven].u3 = (battleVar0->mA234[4].m10_tpage_X & 0x3f) * '\x02' + '\x1e';
    psVar6->m320[1][battleOddOrEven].v3 = battleVar0->mA234[4].m14_tpage_Y + '\a';

    for (int i = 0; i < 2; i++) {
        drawBattleDialogWindow_sub0Sub(&psVar6->m320[i][battleOddOrEven]);
    }
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
    drawBattleDialogWindow_sub0(uVar2, x, y, width, height);
    drawBattleDialogWindow_sub1(uVar2, x, y, width);
    drawBattleDialogWindow_sub2(uVar2, x, y, width, height);
    drawBattleDialogWindow_sub3(uVar2, x, y, height);
    drawBattleDialogWindow_sub4(uVar2, x, y, width, height);
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

void drawBattleDialogWindows() {
    bool bVar1;
    short sVar2;
    sBattleDialogWindow2* psVar3;
    char cVar4;
    int iVar5;

    iVar5 = 0;
    do {
        psVar3 = battleDialogWindows2[iVar5];
        if ((battleVar1->mB8_isDialogWindowInitialized2[iVar5] != 0) &&
            (battleVar1->mBF_isDialogWindowInitialized3[iVar5] == 0)) {
            bVar1 = (ushort)psVar3->m8 + 0x20 < (uint)(ushort)psVar3->m4;
            if (bVar1) {
                sVar2 = psVar3->m8 + 0x20;
            }
            else {
                sVar2 = psVar3->m4;
            }
            cVar4 = !bVar1;
            psVar3->m8 = sVar2;
            if ((ushort)psVar3->mA + 0x20 < (uint)(ushort)psVar3->m6) {
                sVar2 = psVar3->mA + 0x20;
            }
            else {
                sVar2 = psVar3->m6;
                cVar4 = cVar4 + '\x01';
            }
            psVar3->mA = sVar2;
            if (cVar4 == '\x02') {
                battleVar1->mBF_isDialogWindowInitialized3[iVar5] = 1;
            }
            drawBattleDialogWindow
            ((uint)psVar3->mC_windowIndex,
                (psVar3->m0 + ((ushort)psVar3->m4 >> 1)) - ((ushort)psVar3->m8 >> 1),
                (psVar3->m2 + ((ushort)psVar3->m6 >> 1)) - ((ushort)psVar3->mA >> 1), psVar3->m8,
                psVar3->mA);
        }
        iVar5 = iVar5 + 1;
    } while (iVar5 < 7);
}
