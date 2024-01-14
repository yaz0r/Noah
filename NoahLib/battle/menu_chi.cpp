#include "noahLib.h"
#include "battle.h"
#include "battleDialogWindow.h"
#include "battleRenderContext.h"
#include "field/dialogWindows.h"
#include "menus/menuHandler.h"
#include "kernel/memory.h"
#include "enemyScript.h"
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
    battleVar0->mA230->m669_drawEPCost = 0;
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
        loadImageSync(&copyRect, *ramTextureArray[currentEntry]);
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
        loadImageSync(&copyRect, *itemLabelsIds[currentEntry]);
    }
}

int initMenuTiles6Sub(sFont& param_1, int param_2, std::array<POLY_FT4, 2>* param_3, int param_4, short param_5, short param_6, ushort param_7, char param_8, char param_9) {
    MissingCode();
    return 0;
}

void drawCircleMenuChi_updateSub0Sub0(short param_1, short param_2, short param_3, short param_4, byte param_5) {
    battleVar1->mFC_countOF1E68 = 0;
    for (int i = 0; i < param_5; i++) {
        battleVar1->mFC_countOF1E68 += battleSetupStringInPolyFT4Large(0x65, &battleVar0->m1E68[battleVar1->mFC_countOF1E68], param_1, param_4);
    }
    battleVar1->mFC_countOF1E68 += battleSetupStringInPolyFT4Large(100, &battleVar0->m1E68[battleVar1->mFC_countOF1E68], param_1, param_2);
    battleVar1->mFC_countOF1E68 += initMenuTiles6Sub(battleFont, 100, &battleVar0->m1E68[battleVar1->mFC_countOF1E68], battleOddOrEven, param_1, param_3, 0x1000, 0, 1);
    battleVar1->mA6_1E68OddOrEven = battleOddOrEven;
    battleVar1->m9D_render1E68 = 1;
}

sRamTexture* tempRamTexture = nullptr;

std::vector<u8>::iterator drawCircleMenuChi_updateSub0Sub1(int param_1)
{
    return getDialogParamPointer(printDialogTextVar[0x50 / 4], param_1);
}

std::vector<u8>::iterator drawCircleMenuChi_updateSub0Sub2(int param_1)
{
    return getDialogParamPointer(printDialogTextVar[0xC0 / 4], param_1);
}

void drawCircleMenuChi_updateSub0Sub3Sub0(uint param_1)
{
    ushort uVar1;
    uint uVar2;

    param_1 = param_1 & 0xff;
    uVar2 = (ushort)battleEntities[param_1].m0_base.m50_MP / 10;
    if (uVar2 != 0) {
        repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0xe][battleOddOrEven], 0x104, 0xc6, (char)uVar2 * '\b' + 'x', 0, 8);
    }
    uVar1 = battleEntities[param_1].m0_base.m50_MP;
    repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0xf][battleOddOrEven], 0x10c, 0xc6, ((char)uVar1 + (char)(uVar1 / 10) * -10) * '\b' + 'x', 0, 8);
    uVar2 = (ushort)battleEntities[param_1].m0_base.m52_MaxMP / 10;
    if (uVar2 != 0) {
        repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0x10][battleOddOrEven], 0x11c, 0xc6,(char)uVar2 * '\b' + 'x', 0, 8);
    }
    uVar1 = battleEntities[param_1].m0_base.m52_MaxMP;
    repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0x11][battleOddOrEven], 0x124, 0xc6, ((char)uVar1 + (char)(uVar1 / 10) * -10) * '\b' + 'x', 0, 8);
    return;
}

void drawCircleMenuChi_updateSub0Sub3(int param_1)
{
    repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0xc][battleOddOrEven], 0xa8, 0xa6, ItemNameSpriteInfo[6].m2_U, ItemNameSpriteInfo[6].m3_V, ItemNameSpriteInfo[6].m0_width);
    u16 uVar1 = textSpriteMode0;
    if (ItemNameSpriteInfo[6].m1 != 0) {
        uVar1 = textSpriteMode1;
    }
    battleVar0->mA230->m0_polys[0xc][battleOddOrEven].clut = uVar1;
    repositionTextRenderingPlanePrim(&battleVar0->mA230->m0_polys[0xd][battleOddOrEven], 0xec, 0xc6, ItemNameSpriteInfo[5].m2_U, ItemNameSpriteInfo[5].m3_V, ItemNameSpriteInfo[5].m0_width);
    uVar1 = textSpriteMode0;
    if (ItemNameSpriteInfo[5].m1 != 0) {
        uVar1 = textSpriteMode1;
    }
    battleVar0->mA230->m0_polys[0xd][battleOddOrEven].clut = uVar1;
    drawCircleMenuChi_updateSub0Sub3Sub0(param_1);
    battleSetupStringInPolyFT4Large(0x71, &battleVar0->mA230->m5A0[0], 0x118, 0xd1);
    battleVar0->mA230->m66C = battleOddOrEven;
    return;
}

void drawCircleMenuChi_updateSub0(byte param_1) {
    initBattleVar0_A230();
    loadItemTargetsLabel();
    drawCircleMenuChi_updateSub0Sub0(0x20, 0x30, 0x98, 0x38, 0xc);
    tempRamTexture = allocateTextureRamForText(0x39);
    // TODO: memset 0, but done in allocation function

    RECT localRect;
    localRect.x = 0x3C0;
    localRect.w = 0x3C;
    localRect.y = 0;
    localRect.h = 0xD;
    loadImageSync(&localRect, *tempRamTexture);

    std::array<u8, 16> flagArray;
    std::array<u8, 16> flagArray2;

    if (apConfigArray[param_1].m1 == 0) {
        for (int entryIndex = 0; entryIndex < 0x10; entryIndex++) {
            if (!bitmaskCharacterCheck(gameState.m16C0[battleCharacters[param_1]].m0_unlockedAbilitiesBitField[1], entryIndex & 0xff)) {
                flagArray[entryIndex] = 0;
                flagArray2[entryIndex] = 0;
            }
            else {
                flagArray[entryIndex] = 1;
                flagArray2[entryIndex] = partyBattleStats[param_1].m370[0].m13_cost;
            }
        }
    }
    else {
        assert(0);
    }

    std::array<sRamTexture*, 16> allTextures;

    for (int entryIndex = 0; entryIndex < 0x10; entryIndex++) {
        allTextures[entryIndex] = allocateTextureRamForText(0x1B);
        // TODO: memset but done in allocation

        RECT localRect2;
        localRect2.x = (short)((int)entryIndex % 2) * 0x1e + 0x380;
        localRect2.y = (short)((int)entryIndex / 2) * 0x10 + 0x100;
        localRect2.w = 0x1b;
        localRect2.h = 0x10;
        loadImageSync(&localRect2, *tempRamTexture);
        if (flagArray[entryIndex]) {
            std::vector<u8>::iterator stringData;
            if (apConfigArray[param_1].m1 == 0) {
                stringData = drawCircleMenuChi_updateSub0Sub1((uint)battleEntities[param_1].m0_base.m56_battleCommandLoadout * 0x10 + entryIndex);
            }
            else {
                stringData = drawCircleMenuChi_updateSub0Sub2((uint)(byte)battleEntities[param_1].m0_base.mA0_partyData_gearNum * 0x10 + entryIndex);
            }
            renderString(stringData, *allTextures[entryIndex], 0x1b, 0);
            RECT local_b0;
            local_b0.x = (short)((int)entryIndex % 2) * 0x1e + 0x380;
            local_b0.y = (short)((int)entryIndex / 2) * 0x10 + 0x102;
            local_b0.w = 0x1e;
            local_b0.h = 0xd;
            loadImageSync(&local_b0, *allTextures[entryIndex]);
        }
        if ((entryIndex & 1) == 0) {
            RECT local_90;
            local_90.x = 0x3c0;
            local_90.y = (short)((int)entryIndex / 2) * 0x10 + 0x100;
            local_90.w = 0x1b;
            local_90.h = 0x10;
            loadImageSync(&local_90, *tempRamTexture);
        }

        {
            s16 sVar3 = (short)((int)entryIndex / 2);
            RECT local_a8;
            local_a8.x = ((short)entryIndex + sVar3 * -2) * 0x10 + 0x3c0;
            local_a8.y = sVar3 * 0x10 + 0x102;
            local_a8.w = 6;
            local_a8.h = 0xd;
            sRamTexture* psVar4 = tempRamTexture;
            if (flagArray2[entryIndex] / 10 != 0) {
                psVar4 = itemLabelsIds[flagArray2[entryIndex] / 10];
            }
            loadImageSync(&local_a8, *psVar4);
        }

        {
            s16 sVar3 = (short)((int)entryIndex / 2);
            RECT local_a0;
            local_a0.x = ((short)entryIndex + sVar3 * -2) * 0x10 + 0x3c2;
            local_a0.y = sVar3 * 0x10 + 0x102;
            local_a0.w = 6;
            local_a0.h = 0xd;
            sRamTexture* psVar4 = tempRamTexture;
            if (flagArray[entryIndex] != '\0') {
                psVar4 = itemLabelsIds[flagArray2[entryIndex] % 10];
            }
            loadImageSync(&local_a0, *psVar4);
        }
    }

    drawCircleMenuChi_updateSub0Sub3(param_1);

    for (int i = 0; i < 0x10; i++) {
        delete allTextures[i];
    }
    delete tempRamTexture;

    battleVar0->mA230->m0_polys[4][0].clut = textSpriteMode0;
    battleVar0->mA230->m0_polys[4][1].clut = textSpriteMode0;
    battleVar0->mA230->m669_drawEPCost = 1;
    battleVar1->mB7 = 1;
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

void drawCircleMenuChi_updateSub5(char param_1)
{
    auto pmVar1 = battleVar1;
    battleVar1->m9E_render27C8 = 0;
    pmVar1->m9D_render1E68 = 0;
    pmVar1->m9C_renderBA8 = 0;
    pmVar1 = battleVar1;
    battleVar1->mB0_isDialogWindowInitialized[3] = 0;
    pmVar1->mB0_isDialogWindowInitialized[2] = 0;
    pmVar1->mB0_isDialogWindowInitialized[1] = 0;
    pmVar1->mB0_isDialogWindowInitialized[0] = 0;
    battleVar1->mB7 = 0;
    if (param_1 == '\0') {
        battleVar1->mCB = 1;
    }
    return;
}

void abilityCancelSelection(uint param_1)
{
    u16 uVar2;
    uint uVar3;
    uint uVar4;

    auto pmVar1 = battleVar1;
    battleVar1->m9E_render27C8 = 1;
    pmVar1->m9D_render1E68 = 1;
    pmVar1->m9C_renderBA8 = 1;
    pmVar1 = battleVar1;
    param_1 = param_1 & 0xff;
    battleVar1->mB0_isDialogWindowInitialized[3] = 1;
    pmVar1->mB0_isDialogWindowInitialized[2] = 1;
    pmVar1->mB0_isDialogWindowInitialized[1] = 1;
    pmVar1->mB0_isDialogWindowInitialized[0] = 1;
    battleVar1->mB7 = 1;
    uVar3 = characterIdToTargetBitmask(param_1);
    uVar4 = characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget);
    setCameraVisibleEntities((uVar3 | uVar4) & 0xffff);
    uVar2 = characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget);
    updateCharacterBlinkingTask(uVar2);
    return;
}

byte abilitySelectedTarget;
u16 abilityTargetBitmask;

byte abilityTargetSelectionInitSub1(byte param_1)

{
    char cVar1;
    ushort uVar2;
    byte bVar4;
    int iVar5;
    int iVar6;

    iVar5 = 0xb;
    auto pbVar3 = targetsPerPriority.begin() +0xb;
    do {
        *pbVar3 = 0xff;
        iVar5 = iVar5 + -1;
        pbVar3 = pbVar3 + -1;
    } while (-1 < iVar5);
    iVar6 = 0;
    iVar5 = 7;
    numValidTarget = '\0';
    abilityTargetBitmask = 0;
    bVar4 = 3;
    if (true) {
        do {
            cVar1 = isTargetValid(param_1, bVar4);
            if (cVar1 != '\0') {
                targetsPerPriority[iVar6] = bVar4;
                uVar2 = characterIdToTargetBitmask(bVar4);
                iVar6 = iVar6 + 1;
                numValidTarget = numValidTarget + '\x01';
                abilityTargetBitmask = abilityTargetBitmask | uVar2;
            }
            iVar5 = iVar5 + -1;
            bVar4 = bVar4 + 1;
        } while (-1 < iVar5);
    }
    return targetsPerPriority[0];
}

bool abilityTargetSelectionInitSub0Sub0(uint param_1, char param_2)
{
    bool bVar1;

    param_1 = param_1 & 0xff;
    bVar1 = false;
    if ((isBattleSlotFilled[param_1] != 0) && (battleVisualEntities[param_1].m3 == 0)) {
        if (apConfigArray[param_1].m1 == 0) {
            bVar1 = true;
            if (param_2 == '\0') {
                bVar1 = (battleEntities[param_1].m0_base.m7C & 0xc001) == 0;
            }
        }
        else if ((param_2 != '\0') || ((battleEntities[param_1].mA4_gear.m7C & 0xc001) == 0)) {
            bVar1 = true;
        }
    }
    return bVar1;
}

byte abilityTargetSelectionInitSub0(byte param_1, byte param_2, char param_3, byte param_4)
{
    byte bVar1;
    char cVar2;
    ushort uVar3;
    int iVar5;
    int unaff_s4;
    int iVar6;
    byte unaff_s7;

    iVar6 = 0;
    iVar5 = 0xb;
    auto pbVar4 = targetsPerPriority.begin() + 0xb;
    do {
        *pbVar4 = 0xff;
        iVar5 = iVar5 + -1;
        pbVar4 = pbVar4 + -1;
    } while (-1 < iVar5);
    if (param_1 == 1) {
        param_4 = 0;
        unaff_s4 = 3;
    }
    else if (param_1 < 2) {
        if (param_1 == 0) {
            param_4 = 3;
            unaff_s4 = 8;
        }
    }
    else if (param_1 == 2) {
        param_4 = 0;
        if (param_3 == '\0') {
            param_4 = 3;
            unaff_s4 = 8;
            unaff_s7 = 0;
            iVar6 = 3;
        }
        else {
            unaff_s4 = 3;
            unaff_s7 = 3;
            iVar6 = 8;
        }
    }
    iVar5 = 0;
    numValidTarget = '\0';
    abilityTargetBitmask = 0;
    while (bVar1 = param_4, unaff_s4 = unaff_s4 + -1, -1 < unaff_s4) {
        cVar2 = abilityTargetSelectionInitSub0Sub0(bVar1, param_2);
        param_4 = bVar1 + 1;
        if (cVar2 != '\0') {
            targetsPerPriority[iVar5] = bVar1;
            uVar3 = characterIdToTargetBitmask(bVar1);
            iVar5 = iVar5 + 1;
            numValidTarget = numValidTarget + '\x01';
            abilityTargetBitmask = abilityTargetBitmask | uVar3;
        }
    }
    while (bVar1 = unaff_s7, iVar6 = iVar6 + -1, -1 < iVar6) {
        cVar2 = abilityTargetSelectionInitSub0Sub0(bVar1, param_2);
        unaff_s7 = bVar1 + 1;
        if (cVar2 != '\0') {
            targetsPerPriority[iVar5] = bVar1;
            uVar3 = characterIdToTargetBitmask(bVar1);
            iVar5 = iVar5 + 1;
            numValidTarget = numValidTarget + '\x01';
            abilityTargetBitmask = abilityTargetBitmask | uVar3;
        }
    }
    return targetsPerPriority[0];
}

ushort abilityTargetSelectionInit(ushort param_1, ushort param_2, byte param_3, byte param_4, char param_5)

{
    bool bVar1;
    short sVar2;
    ushort uVar3;
    byte* pbVar4;
    byte uVar5;
    ushort uVar6;
    int iVar7;
    int iVar8;
    uint uVar9;
    int iVar10;

    if (param_4 == 1) {
        param_2 = 0x3000;
    }
    else if (param_4 < 2) {
        uVar3 = param_1 & 0x8000;
        if (param_4 != 0) goto LAB_Battle__80084e6c;
    }
    else {
        uVar3 = param_1 & 0x8000;
        if (param_4 != 2) goto LAB_Battle__80084e6c;
        param_2 = 0x2001;
    }
    param_1 = param_2;
    uVar3 = param_1 & 0x8000;
LAB_Battle__80084e6c:
    bVar1 = (param_1 & 0x1000) == 0;
    if (bVar1) {
        uVar6 = 7;
    }
    else {
        uVar6 = 0xfff8;
    }
    uVar5 = bVar1;
    if ((param_1 & 0x2000) != 0) {
        uVar5 = 2;
        uVar6 = 0xffff;
    }
    if (param_5 == '\0') {
        abilitySelectedTarget = abilityTargetSelectionInitSub0(uVar5, uVar3 != 0, bVar1, param_4);
    }
    else {
        abilitySelectedTarget = abilityTargetSelectionInitSub1(param_3);
    }
    abilityTargetBitmask = uVar6 & abilityTargetBitmask;
    if ((param_1 & 0x8000) == 0) {
        if ((param_1 & 0x4000) != 0) {
            abilitySelectedTarget = param_3;
            abilityTargetBitmask = characterIdToTargetBitmask(param_3);
            iVar7 = 10;
            auto pbVar4 = targetsPerPriority.begin() + 10;
            do {
                *pbVar4 = 0xff;
                iVar7 = iVar7 + -1;
                pbVar4 = pbVar4 + -1;
            } while (-1 < iVar7);
        }
    }
    else {
        iVar7 = 0;
        abilitySelectedTarget = 0xff;
        uVar9 = 0;
        do {
            sVar2 = bitmaskCharacterCheck(abilityTargetBitmask, uVar9 & 0xff);
            if (sVar2 != 0) {
                if ((&apConfigArray[uVar9].m1)[iVar10] == 0) {
                    uVar3 = (battleEntities[uVar9].m0_base.m7C);
                }
                else {
                    uVar3 = (battleEntities[uVar9].mA4_gear.m7C);
                }
                if ((uVar3 & 0x8000) == 0) {
                    uVar3 = characterOdToInverseTargetBitmask(uVar9 & 0xff);
                    abilityTargetBitmask = abilityTargetBitmask & uVar3;
                }
                else {
                    abilitySelectedTarget = (byte)uVar9;
                    targetsPerPriority[iVar7] = abilitySelectedTarget;
                    iVar7 = iVar7 + 1;
                }
            }
            uVar9 = uVar9 + 1;
        } while ((int)uVar9 < 0xb);
        for (; iVar7 < 0xb; iVar7 = iVar7 + 1) {
            targetsPerPriority[iVar7] = 0xff;
        }
    }
    return param_1;
}


ushort abilitySelectTargetSub0(void)

{
    short sVar1;
    ushort uVar2;
    uint uVar3;
    uint uVar4;
    int iVar5;
    ushort uVar6;

    uVar6 = 0;
    uVar3 = 0;
    iVar5 = 0;
    do {
        uVar4 = uVar3 & 0xff;
        sVar1 = bitmaskCharacterCheck(abilityTargetBitmask, uVar4);
        uVar3 = uVar3 + 1;
        if ((sVar1 != 0) &&
            (battleVisualEntities[abilitySelectedTarget].m0_positionSlot ==
                (&battleVisualEntities[0].m0_positionSlot)[iVar5])) {
            uVar2 = characterIdToTargetBitmask(uVar4);
            uVar6 = uVar6 | uVar2;
        }
        iVar5 = iVar5 + 0x1c;
    } while ((int)uVar3 < 0xb);
    return uVar6;
}



char abilitySelectTarget(ushort param_1, byte param_2, byte param_3)

{
    byte bVar1;
    char cVar2;
    ushort uVar3;
    ushort uVar4;
    short sVar5;
    ushort uVar6;    
    uint uVar7;
    char selectedTarget;
    byte bVar8;

    bVar8 = 0xfe;
    abilityTargetSelectionInit(param_1, param_1, param_2, 0, param_3);
    selectedTarget = abilitySelectedTarget == -1;
    uVar4 = abilityTargetBitmask;
    if (!(bool)selectedTarget) {
        do {
            battleRenderDebugAndMain();
            uVar6 = param_1 & 0xf;
            if (uVar6 == 1) {
                updateCharacterBlinkingTask(abilityTargetBitmask);
                uVar7 = (uint)abilityTargetBitmask;
            LAB_Battle__800851a4:
                setCameraVisibleEntities(uVar7);
            }
            else if (uVar6 < 2) {
                if ((param_1 & 0xf) == 0) {
                    uVar3 = characterIdToTargetBitmask(abilitySelectedTarget);
                    updateCharacterBlinkingTask(uVar3);
                    uVar7 = characterIdToTargetBitmask(abilitySelectedTarget);
                    uVar7 = uVar7 & 0xffff;
                    goto LAB_Battle__800851a4;
                }
            }
            else if (uVar6 == 2) {
                uVar4 = abilitySelectTargetSub0();
                uVar7 = (uint)uVar4;
                updateCharacterBlinkingTask(uVar7);
                goto LAB_Battle__800851a4;
            }
            if (false) goto switchD_Battle_800851d4_caseD_6;
            battleInputDirection direction;
            switch (battleInputButton) {
            case 0:
                direction = BDIR_RIGHT;
                break;
            case 1:
                direction = BDIR_DOWN;
                break;
            case 2:
                direction = BDIR_LEFT;
                break;
            case 3:
                direction = BDIR_UP;
                break;
            case 4:
                selectedTarget = '\x02';
                abilityTargetBitmask = uVar4;
                goto switchD_Battle_800851d4_caseD_6;
            case 5:
                selectedTarget = '\x01';
            default:
                goto switchD_Battle_800851d4_caseD_6;
            }
            cVar2 = selectNewSlotByDirection(abilitySelectedTarget, direction);
            sVar5 = bitmaskCharacterCheck(abilityTargetBitmask, cVar2);
            if (sVar5 != 0) {
                abilitySelectedTarget = cVar2;
            }
        switchD_Battle_800851d4_caseD_6:
            bVar1 = battleInputButton;
            if (battleInputButton != bVar8) {
                uVar7 = 0;
                do {
                    cVar2 = selectNewSlotByDirection(abilitySelectedTarget, (battleInputDirection)uVar7);
                    if (cVar2 == abilitySelectedTarget) {
                        battleG3->mE6_isDirectionEnabled[uVar7] = 0;
                    }
                    else {
                        battleG3->mE6_isDirectionEnabled[uVar7] = 1;
                    }
                    uVar7 = uVar7 + 1;
                    bVar8 = bVar1;
                } while ((int)uVar7 < 4);
            }
        } while (selectedTarget == '\0');
    }
    return selectedTarget + -1;
}


bool useAbility(uint param_1, uint param_2, uint param_3) {
    int sVar9;
    int abilityCost;
    int bVar1 = 0;
    if (apConfigArray[param_1].m1 == 0) {
        sVar9 = partyBattleStats[param_1].m370[param_3 * 2 + param_2].m0;
        abilityCost = partyBattleStats[param_1].m370[param_3 * 2 + param_2].m13_cost;
        bVar1 = bitmaskCharacterCheck(gameState.m16C0[battleCharacters[param_1]].m0_unlockedAbilitiesBitField[1], param_2 + (param_3 * 2)) != 0;
    }
    else {
        // mecha
        assert(0);
    }

    int bVar2 = 1;
    int bVar10 = 0;
    if (abilityCost <= battleEntities[param_1].m0_base.m50_MP) {
        bVar2 = 1;
        int targetSelected;
        if (bVar1) {
            drawCircleMenuChi_updateSub5(1);
            battleVar1->mC6_isTargetSelectionCursorVisible = 1;
            targetSelected = abilitySelectTarget(sVar9, param_1, 0);
            if (targetSelected == 0) {
                battleVar1->mC6_isTargetSelectionCursorVisible = 0;
                abilityCancelSelection(param_1);
            }
            else {
                battleEntities[param_1].m0_base.m50_MP -= abilityCost;
            }
        }
        bVar10 = targetSelected != '\0';
        bVar2 = false;
    }
    if (bVar2) {
        battleSoundEffect2(0x4f);
    }
    return bVar10;
}

void freeBattleVar0_A230(void)
{
    battleRenderDebugAndMain();
    delete battleVar0->mA230;
    battleVar0->mA230 = nullptr;
    return;
}

void drawCircleMenuChi_updateSub6(uint param_1)
{
    param_1 = param_1 & 0xff;
    battleVar1->mB0_isDialogWindowInitialized[param_1] = 0;
    battleVar1->mB8_isDialogWindowInitialized2[param_1] = 0;
    battleRenderDebugAndMain();
    delete battleDialogWindows[param_1];
    delete battleDialogWindows2[param_1];
    return;
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
        case 0:
            if ((uint)bVar4 * 2 + (uint)bVar3 != 0xf) {
                if (bVar3 == 0xb) {
                    battleChiMenuVar1 = 1;
                }
                else {
                    bVar3 = bVar3 + 1;
                }
            }
            break;
        case 1:
            if ((uint)bVar4 * 2 + (uint)bVar3 < 0xe) {
                if (bVar3 + 2 < 0xc) {
                    bVar3 = bVar3 + 2;
                }
                else {
                    battleChiMenuVar1 = 3;
                }
            }
            break;
        case 2:
            if ((uint)bVar4 * 2 + (uint)bVar3 != 0) {
                if ((bVar3 == 0) && (bVar4 != 0)) {
                    battleChiMenuVar1 = 2;
                }
                else {
                    bVar3 = bVar3 - 1;
                }
            }
            break;
        case 3:
            if (1 < (uint)bVar4 * 2 + (uint)bVar3) {
                if ((bVar3 < 2) && (bVar4 != 0)) {
                    battleChiMenuVar1 = 4;
                }
                else {
                    bVar3 = bVar3 - 2;
                }
            }
            break;
        case 4:
            if (useAbility(param_1, bVar3, bVar4)) {
                iVar5 = 1;
                battleVar2->m2E6 = bVar3 + bVar4 * '\x02';
            }
        case 5:
            iVar5 = 0;
            break;
        case 8:
            break;
        default:
            assert(0);
        }

        switch (battleChiMenuVar1) {
        case 0:
            break;
        case 1:
            iVar2 = (bVar4 + 1) * 0x10;
            if (iVar2 <= battleChiMenuVar0) {
                bVar4 = bVar4 + 1;
                bVar3 = bVar3 - 1;
                battleChiMenuVar1 = 0;
                battleChiMenuVar0 = iVar2;
            }
            break;
        case 3: // scroll the list up
            iVar2 = (bVar4 + 1) * 0x10;
            if (iVar2 <= battleChiMenuVar0) {
                bVar4 = bVar4 + 1;
                battleChiMenuVar1 = 0;
                battleChiMenuVar0 = iVar2;
            }
            break;
        default:
            assert(0);
        }

        if (iVar5 != 2) {
            drawCircleMenuChi_updateSub5(iVar5);
            freeBattleVar0_A230();
            removeTargetSelectionCursors();
            drawCircleMenuChi_updateSub6(0);
            drawCircleMenuChi_updateSub6(1);
            drawCircleMenuChi_updateSub6(2);
            drawCircleMenuChi_updateSub6(3);
            return iVar5;
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
        battleSoundEffect2(0x4f);
        break;
    case 5:
        break;
    case 8:
        break;
    default:
        assert(0);
    }
}

void freeBattleVar1_AE() {
    if (battleVar1->mAE_isChiMenuDataLoaded) {
        chiData.clear();
        battleVar1->mAE_isChiMenuDataLoaded = false;
    }
}