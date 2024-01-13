#include "noahLib.h"
#include "battle.h"
#include "battleDialogWindow.h"
#include "battleRenderContext.h"
#include "field/dialogWindows.h"
#include "menus/menuHandler.h"
#include "kernel/memory.h"
#include "enemyScript.h"
#include "kernel/TIM.h"
#include "battleLoader.h"

std::vector<u8> chiData;

void setFolderChiData(void)
{
    setCurrentDirectory(0x20, 3);
}

void drawCircleMenuChi_loadData(int param_1) {
    if (battleVar1->mAE_isChiMenuDataLoaded == 0) {
        idleBattleDuringLoading();
        int fileIndex = 2;
        if (apConfigArray[param_1].m1 == 0) {
            fileIndex = 1;
        }
        setFolderChiData();
        chiData.resize(getFileSizeAligned(fileIndex));
        readFile(fileIndex, chiData, 0, 0x80);
        idleBattleDuringLoading();
        battleVar1->mAE_isChiMenuDataLoaded = true;
    }
}

s32 battleChiMenuVar0 = 0;
s8 battleChiMenuVar1 = 0;

void battleInitPoly4(std::array<POLY_FT4, 2>& param_1, char param_2, byte param_3) {
    for (int i = 0; i < 2; i++) {
        POLY_FT4* p = &param_1[i];
        SetPolyFT4(p);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        SetSemiTrans(p, 0);
        SetShadeTex(p, 1);
        switch (param_3) {
        case 0:
            p->tpage = GetTPage(0, 0, 0x380, 0);
            break;
        case 1:
            p->tpage = GetTPage(0, 0, 0x380, 0x100);
            break;
        case 2:
            p->tpage = GetTPage(0, 0, 0x3C0, 0x100);
            break;
        case 3:
            p->tpage = GetTPage(0, 0, 0x3C0, 0);
            break;
        default:
            assert(0);
        }

        p->clut = !param_2 ? textSpriteMode0 : textSpriteMode1;
    }
}

void initBattleVar0_A230_content() {
    battleVar0->mA230->m669 = 0;
    battleVar0->mA230->m66B = 0;
    battleVar0->mA230->m66D = 0;
    battleInitPoly4(battleVar0->mA230->m0_polys[0], 0, 1);
    battleInitPoly4(battleVar0->mA230->m0_polys[1], 0, 1);
    battleInitPoly4(battleVar0->mA230->m0_polys[2], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[3], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[4], 0, 1);
    battleInitPoly4(battleVar0->mA230->m0_polys[5], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[6], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[7], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[8], 0, 3);
    battleInitPoly4(battleVar0->mA230->m0_polys[9], 1, 3);
    battleInitPoly4(battleVar0->mA230->m0_polys[10], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0xb], 1, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0xc], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0xd], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0xe], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0xf], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0x10], 0, 2);
    battleInitPoly4(battleVar0->mA230->m0_polys[0x11], 0, 2);
}

void initBattleVar0_A230() {
    battleVar0->mA230 = new sBattleVar0_a230;
    memset(battleVar0->mA230, 0, sizeof(sBattleVar0_a230));
    initBattleVar0_A230_content();
}

struct sItemNameSpriteInfo {
    u8 m0_width;
    u8 m1;
    u8 m2_U;
    u8 m3_V;
    // size 4
};

std::array<sItemNameSpriteInfo, 22> ItemNameSpriteInfo;

void loadImageSync(RECT* param_1, u16* param_2)
{
    LoadImage(param_1, (u8*)param_2);
    DrawSync(0);
    return;
}

void loadItemTargetsLabel() {
    std::array<sRamTexture*, 12> ramTextureArray;
    for (int currentEntry = 0; currentEntry < 11; currentEntry++) {
        ramTextureArray[currentEntry] = allocateTextureRamForText(0x1B);
        //memset(ramTextureArray[currentEntry], 0, sizeof(sRamTexture) * 0x1E); //TODO: can't do that because of the vector
        u8 stringWidth = renderString(getItemLabelString(currentEntry + 10), *ramTextureArray[currentEntry], 0x1B, 0);

        ItemNameSpriteInfo[11].m0_width = stringWidth;
        ItemNameSpriteInfo[11].m2_U = 0x78;
        ItemNameSpriteInfo[0].m2_U = 0x78;
        ItemNameSpriteInfo[11].m3_V = 0xd + 0xd * currentEntry;
        ItemNameSpriteInfo[0].m3_V = 0xd + 0xd * currentEntry;
        ItemNameSpriteInfo[0].m1 = 0;
        ItemNameSpriteInfo[11].m1 = 1;

        RECT copyRect;
        copyRect.y = 0x10d + currentEntry * 0xd;
        copyRect.x = 0x3DE;
        copyRect.w = 0x1E;
        copyRect.h = 0xD;
        loadImageSync(&copyRect, ramTextureArray[currentEntry]->data());
    }
    for (int currentEntry = 0; currentEntry < 11; currentEntry++) {
        delete ramTextureArray[currentEntry];
    }
    for (int currentEntry = 0; currentEntry < 10; currentEntry++) {
        RECT copyRect;
        copyRect.x = 0x3DE + 2 * currentEntry;
        copyRect.y = 0x100;
        copyRect.w = 6;
        copyRect.h = 0xD;
        loadImageSync(&copyRect, itemLabelsIds[currentEntry]->data());
    }
}

void drawCircleMenuChi_updateSub0(int) {
    initBattleVar0_A230();
    loadItemTargetsLabel();
    MissingCode();
}

void drawCircleMenuChi_updateSub4(short param_1, short param_2, int* param_3, byte* param_4) {
    byte bVar1;
    int iVar3;

    bVar1 = *param_4;
    *param_4 = bVar1 + 1;
    if (2 < (byte)(bVar1 + 1)) {
        iVar3 = *param_3;
        *param_3 = iVar3 + -1;
        if (iVar3 + -1 < 0) {
            *param_3 = 4;
        }
        *param_4 = 0;
    }
    iVar3 = battleSetupStringInPolyFT4Large(*param_3 + 0xe0, &battleVar0->m27C8[0], param_1, param_2);
    battleVar1->m100_countOf27C8 = iVar3;
    battleVar1->mA7_27C8OddOrEven = battleOddOrEven;
    battleVar1->m9E_render27C8 = 1;
}

void drawCircleMenuChi_updateSub1(byte param_1, char param_2, char param_3) {
    MissingCode();
}

void drawCircleMenuChi_updateSub2(short param_1) {
    battleVar1->mF8_countOfBA8 = 0;
    for (int i = 0; i < 8; i++) {
        battleVar1->mF8_countOfBA8 += battleSetupStringInPolyFT4Large(0x66, &battleVar0->mBA8[battleVar1->mF8_countOfBA8], 0x20, i * 8 + 0x38);
    }
    battleVar1->mA5_ba8OddOrEven = battleOddOrEven;
    battleVar1->m9C_renderBA8 = 1;
}

void drawCircleMenuChi_updateSub3(int param_1)
{
    battleVar0->mA230->m0_polys[0][battleOddOrEven].clut = textSpriteMode0;
    battleVar0->mA230->m0_polys[1][battleOddOrEven].clut = textSpriteMode0;
    setupMenuPolyFT4(&battleVar0->mA230->m0_polys[0][battleOddOrEven], 0x34, 0x34, 0, param_1, 0x60, 0x60);
    setupMenuPolyFT4(&battleVar0->mA230->m0_polys[1][battleOddOrEven], 0xb8, 0x34, 0x78, param_1, 0x60, 0x60);
    setupMenuPolyFT4(&battleVar0->mA230->m0_polys[2][battleOddOrEven], 0x9c, 0x34, 0, param_1, 0x10, 0x60);
    setupMenuPolyFT4(&battleVar0->mA230->m0_polys[3][battleOddOrEven], 0x120, 0x34, 0x40, param_1, 0x10, 0x60);
    battleVar0->mA230->m668_oddOrEven = battleOddOrEven;
}

int drawCircleMenuChi_update(int param_1) {
    char cVar1;
    int iVar2;
    s8 bVar3;
    s8 bVar4;
    int iVar5;
    s8 bVar6;
    int iVar7;
    s8 bVar8;
    int local_30;
    byte local_2c;

    bVar3 = 0;
    bVar4 = 0;
    bVar8 = -1;
    bVar6 = -1;
    iVar7 = 0xffff;
    iVar5 = 2;
    local_30 = 4;
    local_2c = 0;

    battleChiMenuVar0 = 0;
    battleChiMenuVar1 = 0;

    battleVar1->mCB = 0;
    allocateAndSetupBattleDialogWindow(0, 0x1c, 0xa0, 0xac, 0x38, 0, 1);
    allocateAndSetupBattleDialogWindow(1, 0x20, 0x2c, 0x118, 0x70, 0, 1);
    allocateAndSetupBattleDialogWindow(2, 0xd0, 0xa4, 0x50, 0x18, 0, 1);
    allocateAndSetupBattleDialogWindow(3, 0xe6, 0xc0, 0x4c, 0x18, 0, 1);
    drawCircleMenuChi_updateSub0(param_1);
    initBattleG3Array();
    do {
        if ((bVar3 != bVar8) || (bVar4 != bVar6)) {
            drawCircleMenuChi_updateSub1(param_1, bVar3, bVar4);
            bVar8 = bVar3;
            bVar6 = bVar4;
        }
        if (battleChiMenuVar0 != iVar7) {
            drawCircleMenuChi_updateSub2(battleChiMenuVar0);
            drawCircleMenuChi_updateSub3(battleChiMenuVar0);
            iVar7 = battleChiMenuVar0;
        }
        drawCircleMenuChi_updateSub4((bVar3 & 1) * 0x84 + 0x2a, (uint)(bVar3 >> 1) * 0x10 + 0x38, &local_30, &local_2c);
        battleRenderDebugAndMain();
        switch (battleInputButton) {
        case 8:
            break;
        default:
            assert(0);
        }

        switch (battleChiMenuVar1) {
        case 0:
            break;
        default:
            assert(0);
        }

        if (iVar5 != 2) {
            assert(0);
        }
    } while (true);
}

void drawCircleMenuChi_sub2(int param_1) {
    MissingCode();
}

void drawCircleMenuChi(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
            }
            else {
                battleSoundEffect2(0x4f);
            }
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemHighlighted;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 4:
    case 6:
    case 7:
        drawCircleMenuChi_loadData(param_1);
        if (drawCircleMenuChi_update(param_1) == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
        }
        else {
            drawCircleMenuChi_sub2(param_1);
        }
        break;
    case 8:
        break;
    default:
        assert(0);
    }
}