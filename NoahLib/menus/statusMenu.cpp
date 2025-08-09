#include "noahLib.h"
#include "statusMenu.h"
#include "menuHandler.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/gameState.h"
#include "field/dialogWindows.h"

struct sMenuContext_358 {
    std::array<POLY_FT4, 2> m0_portrait;
    std::array<POLY_FT4, 2> m50_name;
    std::array<std::array<POLY_FT4, 2>, 2> m1E00_polys;
    std::array<SVECTOR, 4> m1EAC;
    std::array<SVECTOR, 4> m1EC0;
    std::array<std::array<SVECTOR, 4>, 2> m2AA0;
    u8 m2AE0;
    u8 m2AED;
    u8 m2AEE;
    //size 0x2AF0
};

struct sMenuContext_35C {
    std::array<std::array<POLY_FT4, 2>, 47> m0;
    std::array<std::array<std::array<POLY_FT4, 2>, 4>, 7> mEB0;
    std::array<std::array<std::array<POLY_FT4, 2>, 4>, 7> m1770;
    std::array<std::array<POLY_G4, 2>, 7> m2030;
    std::array<std::array<SVECTOR, 4>, 47> m2420;
    std::array<std::array<std::array<SVECTOR, 4>, 4>, 7> m2A00;
    std::array<std::array<std::array<SVECTOR, 4>, 4>, 7> m2D80;
    std::array<std::array<SVECTOR, 4>, 7> m3100;
    std::array<s8, 7> m32C0;
    std::array<s8, 7> m32C7;
    std::array<s8, 7> m32CE;
    std::array<s8, 7> m32D5;
    std::array<s8, 7> m32DC;
    std::array<s8, 7> m32EA;
    u8 m32F1;
    u8 m32F2;
    u8 m32F3;
    //size 0x32F4
};

struct sMenuContext_360 {
    //size 0x2AC
};

std::array<std::array<u8, 2>, 4> saveGameMainMenuConfig6 = { {
    { {0x12, 0x11} },
    { {0x10, 0x13} },
    { {0x14, 0x15} },
    { {0x0, 0x0} },
} };

void initStatusMenuSub0(char param_1) {
    if (param_1 == 0) {
        for (int i = 0; i < 3; i++) {
            gMenuContext->m33C->m14[i] = 0;
        }
    }
    else {
        static const std::array<std::array<s32, 3>, 2> statusMenuPositions = { {
            {0x98, 0x70, 0xA8},
            {0x76, 0x92, 0xAE},
        } };

        setupMenuContext4E0(&gMenuContext->m4E0[0xc],
            std::span(saveGameMainMenuConfig6.begin(), saveGameMainMenuConfig6.end()).begin(),
            4, 6);

        for (int i = 0; i < 3; i++) {
            initMemoryCardPolyVerts(gMenuContext->m4E0[0xF + i].m50_vertex, statusMenuPositions[0][i], statusMenuPositions[1][i],
                gMenuContext->m4E0[0xC + i].m7E_stringWidth, 0xD);
            gMenuContext->m4E0[0xC + i].m7D = gMenuContext->m308_oddOrEven;
            gMenuContext->m4E0[0xC + i].m7F = 1;
            gMenuContext->m33C->m14[i + 3] = 1;
        }
    }
}

void loadSharedMenuData2(s8 param_1) {
    sLoadableDataRaw file;
    std::vector<std::vector<u8>::const_iterator> relocatedFile;

    if (param_1 < 0x10) {
        setCurrentDirectory(0x10, 0);
        file.resize(getFileSizeAligned(2));
        readFile(2, file, 0, 0x80);
        waitReadCompletion(0);
        relocatedFile = doPointerRelocation(file.getRawData());
    }

    switch (param_1) {
    case 3:
        gMenuContext->m330->m0_weaponStats = sMenuContext_330::readWeaponStats(mallocAndDecompress(relocatedFile[4]));
        gMenuContext->m330->m4_accessoryData = sMenuContext_330::readAccesoryStats(mallocAndDecompress(relocatedFile[5]));
        gMenuContext->m330->m18 = mallocAndDecompress(relocatedFile[42]);
        gMenuContext->m330->m14 = mallocAndDecompress(relocatedFile[19]);
        break;
    default:
        assert(0);
    }
}

struct sMenuContext_440 {
    std::array<std::array<POLY_FT4, 2>, 4> m0;
    std::array<std::array<SVECTOR, 4>, 4> m140;
    u8 m1C0;
    // size 0x1C4
};

std::array<std::array<int, 2>, 2> statusMenuPositions2 = { {
    {{0, 0x114}},
    {{0x66, 0x76}}
} };

void initStatusMenuSub1(uint param_1, char param_2) {
    
    bool isEnabled;
    if (param_2 == 0) {
        isEnabled = gMenuContext->m32B;
    }
    else {
        isEnabled = gMenuContext->m33B;
    }

    if (!isEnabled) {
        if (!gMenuContext->m33C->m67) {
            gMenuContext->m440 = new sMenuContext_440;
            memset(gMenuContext->m440, 0, sizeof(sMenuContext_440));
            gMenuContext->m33C->m67 = 1;
        }
        for (int i = 0; i < 2; i++) {
            setupStringInPolyFT4(gMenuContext->m2DC_font, 0x164 + i, &gMenuContext->m440->m0[i], gMenuContext->m308_oddOrEven,
                statusMenuPositions2[0][i], statusMenuPositions2[1][i], 0x1000);
        }
        for (int i = 0; i < 4; i++) {
            auto& p = gMenuContext->m440->m0[i][gMenuContext->m308_oddOrEven];
            initMemoryCardPolyVerts(gMenuContext->m440->m140[i], p.x0, p.y0, p.x1 - p.x0, p.y3 - p.y0);
        }
        gMenuContext->m440->m1C0 = gMenuContext->m308_oddOrEven;
        gMenuContext->m33C->m53 = 1;
    }
}

void initStatusMenu() {
    initStatusMenuSub0(1);

    gMenuContext->m358 = new sMenuContext_358;
    memset(gMenuContext->m358, 0, sizeof(sMenuContext_358));

    gMenuContext->m35C = new sMenuContext_35C;
    memset(gMenuContext->m35C, 0, sizeof(sMenuContext_35C));

    gMenuContext->m360 = new sMenuContext_360;
    memset(gMenuContext->m360, 0, sizeof(sMenuContext_360));

    loadSharedMenuData2(3);
    initStatusMenuSub1(0, 0);
}

void setupStatusMenuForCharacter2(sMenuContext_330* param_1, int param_2) {
    if (gameState.m26C_party[param_2].m56_battleCommandLoadout == 4) {
        assert(0);
    }
    else {
        param_1->mB8_computedStats[0] = (ushort)gameState.m26C_party[param_2].m4[0].m0 +
            (ushort)gameState.m26C_party[param_2].m58_attack +
            (ushort)gameState.m26C_party[param_2].m28;
    }

    param_1->mB8_computedStats[1] = gameState.m26C_party[param_2].m5E_hitPercentage + gameState.m26C_party[param_2].m2E;
    param_1->mB8_computedStats[2] = gameState.m26C_party[param_2].m2D + gameState.m26C_party[param_2].m59_defence + gameState.m26C_party[param_2].m29;
    param_1->mB8_computedStats[3] = gameState.m26C_party[param_2].m5F_evadePercentage + gameState.m26C_party[param_2].m2F;
    param_1->mB8_computedStats[4] = gameState.m26C_party[param_2].m5B_ether + gameState.m26C_party[param_2].m2B;
    param_1->mB8_computedStats[5] = gameState.m26C_party[param_2].m5C_etherDefence + gameState.m26C_party[param_2].m2C;
    param_1->mB8_computedStats[6] = gameState.m26C_party[param_2].m5A_agility + gameState.m26C_party[param_2].m2A;

    if (250 < param_1->mB8_computedStats[0]) {
        param_1->mB8_computedStats[0] = 250;
    }
    if (99 < param_1->mB8_computedStats[1]) {
        param_1->mB8_computedStats[1] = 99;
    }
    if (250 < param_1->mB8_computedStats[2]) {
        param_1->mB8_computedStats[2] = 250;
    }
    if (99 < param_1->mB8_computedStats[3]) {
        param_1->mB8_computedStats[3] = 99;
    }
    if (250 < param_1->mB8_computedStats[4]) {
        param_1->mB8_computedStats[4] = 250;
    }
    if (250 < param_1->mB8_computedStats[5]) {
        param_1->mB8_computedStats[5] = 250;
    }
    if (20 < param_1->mB8_computedStats[6]) {
        param_1->mB8_computedStats[6] = 16;
    }
}

void setupStatusMenuForCharacter3Sub0Sub0(uint param_1, byte param_2) {
    u16 uVar1 = -(ushort)(param_2 != 0) & 0x18;
    u32 uVar6 = param_1 & 0xff;
    setupStringInPolyFT4(gMenuContext->m2DC_font, uVar6 + 0x14b, &gMenuContext->m358->m0_portrait, gMenuContext->m308_oddOrEven, 0x18 - uVar1, 0xe, 0x1000);
    initMemoryCardPolyVerts(gMenuContext->m358->m1EAC, gMenuContext->m358->m0_portrait[gMenuContext->m308_oddOrEven].x0, gMenuContext->m358->m0_portrait[gMenuContext->m308_oddOrEven].y0, 0x30, 0x30);
    initMemoryCardTransparentPoly(&gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven]);
    gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven].tpage = GetTPage(0, 0, 0x180, 0);

    if (param_2 == 0) {
        if ((gMenuContext->m33C->m30_partyMemberId[uVar6] & 1) == 0) {
            gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven].clut = textSpriteMode0;
        }
        else {
            gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven].clut = textSpriteMode1;
        }
    }
    else if ((gameState.m26C_party[gMenuContext->m33C->m30_partyMemberId[uVar6]].mA0_partyData_gearNum + 0xbU & 1) == 0) {
        gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven].clut = textSpriteMode0;
    }
    else {
        gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven].clut = textSpriteMode1;
    }

    ushort sVar7 = (ushort)param_2 * 0x18 + 0x48;
    uint iVar4 = (uint)param_2 * 3 + (param_1 & 0xff);
    setupMenuPolyFT4(&gMenuContext->m358->m50_name[gMenuContext->m308_oddOrEven], 0, 0,
        (infocardNameU[iVar4] & 0x3f) << 2, infocardNameV[iVar4],
        sVar7, 0xd);
    initMemoryCardPolyVerts(gMenuContext->m358->m1EC0, 0x48 - uVar1, 0x20, sVar7, 0xd);
}

void setupStatusMenuForCharacter3Sub0(byte param_1, byte param_2) {
    setupStatusMenuForCharacter3Sub0Sub0(param_1, param_2);
    MissingCode();

    gMenuContext->m33C->m7_drawCurrentSelectedCharacterInfoCard = 1;
    gMenuContext->m358->m2AE0 = gMenuContext->m308_oddOrEven;
}

// handles the icon for being on gear or not
void setupStatusMenuForCharacter3Sub1(int param_1, char param_2, char param_3) {
    gMenuContext->m358->m2AED = 0;
    if (param_2) {
        int x = 0x78;
        for (int i = 0; i < 2; i++) {
            static const std::array<s32, 2> isOnGearCharacters = { {0x159, 0x15A} };

            setupStringInPolyFT4(gMenuContext->m2DC_font, isOnGearCharacters[i], &gMenuContext->m358->m1E00_polys[i], gMenuContext->m308_oddOrEven, x, 0x5A, 0x1000);

            auto& p = gMenuContext->m358->m1E00_polys[i][gMenuContext->m308_oddOrEven];
            initMemoryCardPolyVerts(gMenuContext->m358->m2AA0[i],
                p.x0,
                p.y0,
                p.x1 - p.x0,
                p.y3 - p.y0);
            x += 0x20;
        }

        initMenuTiles1Sub(&gMenuContext->m358->m1E00_polys[param_3 == 0][gMenuContext->m308_oddOrEven]);

        gMenuContext->m358->m1E00_polys[gMenuContext->m308_oddOrEven][gMenuContext->m308_oddOrEven].tpage |= 0x20;
        gMenuContext->m358->m1E00_polys[gMenuContext->m308_oddOrEven][gMenuContext->m308_oddOrEven].r0 = 32;
        gMenuContext->m358->m1E00_polys[gMenuContext->m308_oddOrEven][gMenuContext->m308_oddOrEven].g0 = 32;
        gMenuContext->m358->m1E00_polys[gMenuContext->m308_oddOrEven][gMenuContext->m308_oddOrEven].b0 = 32;
        gMenuContext->m358->m2AEE = gMenuContext->m308_oddOrEven;
        gMenuContext->m358->m2AED = 2;
    }
}

static const std::array<std::array<s32, 7>, 2> setupStatusMenuForCharacter3Sub2Sub0String = { {
    {{0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE4, 0xEB}},
    {{0xDE, 0xF3, 0xF4, 0xF6, 0xEB, 0xF7, 0xF7}},
} };

static const std::array<std::array<s32, 7>, 2> setupStatusMenuForCharacter3Sub2Sub0Pos = { {
    {{8,8,8,8,8,8,8}},
    {{0,8,0x10,0x18,0x20,0x28,0x30}},
} };

// This prepare the poly for the name of the 7 stats
void setupStatusMenuForCharacter3Sub2Sub0(short param_1, short param_2, uint param_3) {
    int numPoly;
    int tp;
    int clutX;
    int clutY;
    int tpageX;
    int tpageY;
    computeMenuBorder(gMenuContext->m2DC_font, 0xe0, &numPoly, &tp, &clutX, &clutY, &tpageX, &tpageY);
    gMenuContext->m35C->m32F3 = 0;
    for (int i = 0; i < 7 - param_3; i++) {
        int startOfString = gMenuContext->m35C->m32F3;
        gMenuContext->m35C->m32F3 += setupStringInPolyFT4(gMenuContext->m2DC_font, setupStatusMenuForCharacter3Sub2Sub0String[param_3][i],
            &gMenuContext->m35C->m0[gMenuContext->m35C->m32F3], gMenuContext->m308_oddOrEven,
            param_1 + setupStatusMenuForCharacter3Sub2Sub0Pos[0][i],
            param_2 + setupStatusMenuForCharacter3Sub2Sub0Pos[1][i],
            0x1000);
        // Alternate shading
        if (i & 1) {
            for (int j = startOfString; j < gMenuContext->m35C->m32F3; j++) {
                SetShadeTex(&gMenuContext->m35C->m0[j][gMenuContext->m308_oddOrEven], 0);
                gMenuContext->m35C->m0[j][gMenuContext->m308_oddOrEven].r0 = 0x40;
                gMenuContext->m35C->m0[j][gMenuContext->m308_oddOrEven].g0 = 0x40;
                gMenuContext->m35C->m0[j][gMenuContext->m308_oddOrEven].b0 = 0x40;
            }
        }
    }
    for (int i = 0; i < gMenuContext->m35C->m32F3; i++) {
        auto& p = gMenuContext->m35C->m0[i][gMenuContext->m308_oddOrEven];
        initMemoryCardPolyVerts(gMenuContext->m35C->m2420[i], p.x0, p.y0, p.x1 - p.x0, p.y3 - p.y0);
    }
}

s32 scaledStatsVar0;
s32 scaledStatsVar1;
s32 scaledStatsVar2;
s32 scaledStatsVar3;
s32 scaledStatsVar4;
s8 scaledStatsVar5;
s8 scaledStatsVar6;

void scaleStatsForMax(uint param_1, uint param_2, int param_3)
{
    scaledStatsVar0 = param_1 & 0xffff;
    scaledStatsVar1 = param_2 & 0xffff;
    scaledStatsVar2 = scaledStatsVar1 - scaledStatsVar0;
    scaledStatsVar3 = (((int)(scaledStatsVar0 * 100) / param_3) * 0x1900) / 10000;
    if (scaledStatsVar2 < 0) {
        scaledStatsVar5 = 3;
        scaledStatsVar6 = 0xe5;
        scaledStatsVar2 = scaledStatsVar0 - scaledStatsVar1;
    }
    else {
        scaledStatsVar5 = 2;
        scaledStatsVar6 = 0xe3;
    }
    scaledStatsVar4 = (((scaledStatsVar2 * 100) / param_3) * 0x1900) / 10000;
    return;
}

ushort computeStatsMax(s32 param_1, std::array<u16, 7>::iterator param_2, std::array<u16, 7>::iterator param_3)
{
    int iVar1;
    ushort uVar2;

    uVar2 = 0;
    iVar1 = 0;
    do {
        iVar1 = iVar1 + 1;
        if (uVar2 < *param_2) {
            uVar2 = *param_2;
        }
        param_2 = param_2 + 1;
    } while (iVar1 < 7);
    iVar1 = 0;
    do {
        iVar1 = iVar1 + 1;
        if (uVar2 < *param_3) {
            uVar2 = *param_3;
        }
        param_3 = param_3 + 1;
    } while (iVar1 < 7);
    return uVar2;
}

void setupStatsPolyG4(std::array<POLY_G4, 2>& param_1, byte param_2) {
    int iVar1;
    u8 local_18;
    u8 local_17;
    u8 local_16;

    if (param_2 == 1) {
        local_18 = 0x80;
        local_17 = 0xff;
        local_16 = 0x80;
    }
    else if (param_2 < 2) {
        if (param_2 == 0) {
            local_18 = 0xff;
            local_17 = 0x80;
            local_16 = 0x80;
        }
    }
    else if (param_2 == 2) {
        local_18 = 0xff;
        local_17 = '\0';
        local_16 = '\0';
    }
    else if (param_2 == 3) {
        local_18 = '\0';
        local_17 = '\0';
        local_16 = 0xff;
    }
    for (int i = 0; i < 2; i++) {
        SetPolyG4(&param_1[i]);
        param_1[i].r0 = local_18;
        param_1[i].g0 = local_17;
        param_1[i].b0 = local_16;
        param_1[i].r1 = local_18;
        param_1[i].g1 = local_17;
        param_1[i].r2 = '\0';
        param_1[i].g2 = '\0';
        param_1[i].b2 = '\0';
        param_1[i].r3 = '\0';
        param_1[i].g3 = '\0';
        param_1[i].b3 = '\0';
        param_1[i].b1 = local_16;
    }
}

// setup the values of stats + bars
void setupStatusMenuForCharacter3Sub2Sub1(int param_1, short param_2, short param_3, char param_4, byte param_5) {
    std::array<u16, 7>* stats1;
    std::array<u16, 7>* stats2;

    if (param_4 == 0) {
        stats1 = stats2 = &gMenuContext->m330->mB8_computedStats;
    }
    else {
        assert(0);
    }

    s32 statsMax = computeStatsMax(0, stats1->begin(), stats2->begin());

    for (int i = 0; i < 7 - param_5; i++) {
        gMenuContext->m35C->m32C0[i] = 0;
        gMenuContext->m35C->m32CE[i] = 0;
        // setup the rendering of the scaled bar for the stat
        scaleStatsForMax(stats1->at(i), stats2->at(i), statsMax);
        setupStatsPolyG4(gMenuContext->m35C->m2030[i], 0);
        initMemoryCardPolyVerts(gMenuContext->m35C->m3100[i], param_2 + 0x28, param_3 + 2 + i * 8, scaledStatsVar3, 6);
        gMenuContext->m35C->m32DC[i] = gMenuContext->m308_oddOrEven;
        // setup the stat numerical value display:
        fillStringDisplayBuffer(scaledStatsVar0);
        for (int j = 0; j < 4; j++) {
            if (gMenuContext->m31C_stringDisplayBuffer[j + 5] != 0xFF) {
                gMenuContext->m35C->m32C0[i] += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[j + 5],
                    &gMenuContext->m35C->mEB0[i][gMenuContext->m35C->m32C0[i]], gMenuContext->m308_oddOrEven, param_2 + 0x68 + j * 8
                    , param_3 + i * 8, 0x1000);
            }
        }
        for (int j = 0; j < gMenuContext->m35C->m32C0[i]; j++) {
            auto& p = gMenuContext->m35C->mEB0[i][j][gMenuContext->m308_oddOrEven];
            initMemoryCardPolyVerts(gMenuContext->m35C->m2A00[i][j], p.x0, p.y0, p.x1 - p.x0, p.y3 - p.y0);
        }
        if (i & 1) {
            MissingCode(); // shading
        }
        gMenuContext->m35C->m32C7[i] = gMenuContext->m308_oddOrEven;
        if (param_4) {
            assert(0); // compute the stats difference bars
        }
        gMenuContext->m35C->m32EA[i] = 1;
    }
}

void setupStatusMenuForCharacter3Sub2(s8 param_1, char param_2, s8 param_3, s8 param_4) {
    int uVar1;
    int uVar2;

    uVar2 = 0x98;
    uVar1 = 0x26;
    if (param_2) {
        uVar2 = 0x80;
        uVar1 = 0x90;
    }
    setupStatusMenuForCharacter3Sub2Sub0(uVar2, uVar1, param_4);
    setupStatusMenuForCharacter3Sub2Sub1(param_1, uVar2, uVar1, param_3, param_4);
    gMenuContext->m33C->m8_drawCurrentSelectedCharacterStats = 1;
    gMenuContext->m35C->m32F1 = gMenuContext->m308_oddOrEven;
}

void setupStatusMenuForCharacter3(uint param_1, s8 param_2) {
    setupStatusMenuForCharacter3Sub0(param_1, param_2);
    setupStatusMenuForCharacter3Sub1(param_1, param_2, gameState.m22B1_isOnGear[param_1]);
    menuDraw();
    setupStatusMenuForCharacter3Sub2(param_1, 0, 0, param_2);
    menuDraw();
    MissingCode();
}

std::array<s32, 3> setupStatusMenuForCharacter4PolyOffset = { {
    0xC, 0x6, 0xC
} };


void setupStatusMenuForCharacter4() {
    initStatusMenuSub0(0);

    auto& p = gMenuContext->m4E0[gMenuContext->m338_currentSelectedSubMenuEntry + 0xC].m0_polys[gMenuContext->m308_oddOrEven];

    p.x0 = cursorPosXTable[7 + gMenuContext->m338_currentSelectedSubMenuEntry] + setupStatusMenuForCharacter4PolyOffset[gMenuContext->m338_currentSelectedSubMenuEntry] + 0x16;
    p.y0 = cursorPosYTable[7 + gMenuContext->m338_currentSelectedSubMenuEntry] - 0x22;

    p.x1 = p.x0 + gMenuContext->m4E0[gMenuContext->m338_currentSelectedSubMenuEntry + 0xC].m7E_stringWidth;
    p.y1 = p.y0;

    p.x2 = p.x0;
    p.y2 = cursorPosYTable[7 + gMenuContext->m338_currentSelectedSubMenuEntry] - 0x15;

    p.x3 = p.x0 + gMenuContext->m4E0[gMenuContext->m338_currentSelectedSubMenuEntry + 0xC].m7E_stringWidth;
    p.y3 = cursorPosYTable[7 + gMenuContext->m338_currentSelectedSubMenuEntry] - 0x15;

    gMenuContext->m4E0[gMenuContext->m338_currentSelectedSubMenuEntry + 0xc].m7D = gMenuContext->m308_oddOrEven;
    gMenuContext->m33C->m14[gMenuContext->m338_currentSelectedSubMenuEntry] = 1;
}

void setupStatusMenuForCharacter1(sMenuContext_330* param_1, int param_2) {
    gameState.m26C_party[param_2].m2D = 0;
    gameState.m26C_party[param_2].m32 = 0;
    gameState.m26C_party[param_2].m28 = 0;
    gameState.m26C_party[param_2].m29 = 0;
    gameState.m26C_party[param_2].m2A = 0;
    gameState.m26C_party[param_2].m2B = 0;
    gameState.m26C_party[param_2].m2C = 0;
    gameState.m26C_party[param_2].m2E = 0;
    gameState.m26C_party[param_2].m2F = 0;
    gameState.m26C_party[param_2].m30 = 0;
    gameState.m26C_party[param_2].m31 = 0;
    gameState.m26C_party[param_2].m7E = 0;
    gameState.m26C_party[param_2].m82 = 0;
    gameState.m26C_party[param_2].m86 = 0;
    gameState.m26C_party[param_2].m8A = 0;
    gameState.m26C_party[param_2].m8E = 0;
    gameState.m26C_party[param_2].mA1 = 0;

    for (int i = 0; i < 3; i++) {
        s8 accessoryIndex = gameState.m26C_party[param_2].m74_accessorySlots[i];

        gameState.m26C_party[param_2].m2D += param_1->m4_accessoryData[accessoryIndex].m8;
        switch (param_1->m4_accessoryData[accessoryIndex].m9_type) {
        case 0:
            break;
        default:
            assert(0);
            break;
        }

        u16 accessoryFlags = param_1->m4_accessoryData[accessoryIndex].mC_flags;
        s8 accessoryFlagsValue = (s8)accessoryFlags; // lower 8 bits
        if (accessoryFlags & 0x8000) {
            gameState.m26C_party[param_2].m28 += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x4000) {
            gameState.m26C_party[param_2].m29 += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x2000) {
            gameState.m26C_party[param_2].m2A += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x1000) {
            gameState.m26C_party[param_2].m2B += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x800) {
            gameState.m26C_party[param_2].m2C += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x400) {
            gameState.m26C_party[param_2].m2E += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x200) {
            gameState.m26C_party[param_2].m2F += accessoryFlagsValue;
        }
        if (accessoryFlags & 0x100) {
            gameState.m26C_party[param_2].m2D += accessoryFlagsValue;
        }
    }

    s8 weaponModifierIndex = gameState.m26C_party[param_2].m6A_weaponModifier;

    gameState.m26C_party[param_2].m4[0].m0 = param_1->m0_weaponStats[weaponModifierIndex].mC;
    gameState.m26C_party[param_2].m2 = param_1->m0_weaponStats[weaponModifierIndex].mA;
    gameState.m26C_party[param_2].m3 = param_1->m0_weaponStats[weaponModifierIndex].mB;

    if (gameState.m26C_party[param_2].m56_battleCommandLoadout == 4) {
        assert(0);
    }
    if (gameState.m26C_party[param_2].m3 == 100) {
        assert(0);
    }
}

int openStatusMenu(void) {
    int previouslySelectedCharacter = 0xF3;
    bool firstTime = true;
    bool stayOnMenu = true;

    gMenuContext->m338_currentSelectedSubMenuEntry = 2;
    gMenuContext->m339_previousSelectedSubMenuEntry = -1;
    initStatusMenu();

    int currentSelectedCharacter = 0;
    for (int i = 0; i < 3; i++) {
        currentSelectedCharacter = i;
        if (gMenuContext->m33C->m30_partyMemberId[i] != -1)
            break;
        currentSelectedCharacter = gMenuContext->m4DC_currentlySelectedCharacter;
    }

    do {
        menuDraw();
        if (currentSelectedCharacter != previouslySelectedCharacter) {
            setupStatusMenuForCharacter1(gMenuContext->m330, gMenuContext->m33C->m30_partyMemberId[currentSelectedCharacter]);
            setupStatusMenuForCharacter2(gMenuContext->m330, gMenuContext->m33C->m30_partyMemberId[currentSelectedCharacter]);
            setupStatusMenuForCharacter3(currentSelectedCharacter, 0);
            if (firstTime) {
                firstTime = false;
                processLoadSaveMenuSub1();
                processLoadSaveMenuSub2(0, 0);
                processLoadSaveMenuSub3(0);
            }
        }
        if (gMenuContext->m338_currentSelectedSubMenuEntry != gMenuContext->m339_previousSelectedSubMenuEntry) {
            setupStatusMenuForCharacter4();
            processLoadSaveMenuSub8(0);
            gMenuContext->m339_previousSelectedSubMenuEntry = gMenuContext->m338_currentSelectedSubMenuEntry;
        }
        switch (gMenuContext->m325_menuButton) {
        case 1:
            if (gMenuContext->m338_currentSelectedSubMenuEntry == 0) {
                gMenuContext->m338_currentSelectedSubMenuEntry = gMenuContext->m33A_numMenuEntries - 1;
            }
            else {
                gMenuContext->m338_currentSelectedSubMenuEntry--;
            }
            break;
        case 3:
            gMenuContext->m338_currentSelectedSubMenuEntry++;
            if (gMenuContext->m33A_numMenuEntries <= gMenuContext->m338_currentSelectedSubMenuEntry) {
                gMenuContext->m338_currentSelectedSubMenuEntry = 0;
            }
            break;
        case 4: // enter sub-menu
            resetCursorState();
            MissingCode();
            gMenuContext->m33C->m7_drawCurrentSelectedCharacterInfoCard = 0;
            gMenuContext->m33C->m8_drawCurrentSelectedCharacterStats = 0;
            gMenuContext->m33C->m4B_draw360 = 0;
            switch (gMenuContext->m338_currentSelectedSubMenuEntry) {
            case 0: // Equip
                MissingCode();
                break;
            case 1: // Abilities
                MissingCode();
                break;
            case 2: // Skills
                MissingCode();
                break;
            default:
                assert(0);
            }
            initStatusMenuSub0(1);
            gMenuContext->m33C->m7_drawCurrentSelectedCharacterInfoCard = 1;
            gMenuContext->m33C->m8_drawCurrentSelectedCharacterStats = 1;
            gMenuContext->m33C->m4B_draw360 = 1;
            updateCursorPolysLocation(gMenuContext->m338_currentSelectedSubMenuEntry + 7, 1);
            gMenuContext->m33C->m4_drawCursor = 1;
            gMenuContext->m339_previousSelectedSubMenuEntry = -1;
            initStatusMenuSub1(0, 0);
            break;
        case 5:
            stayOnMenu = false;
            break;
        case 7:
        case 8:
            break;
        default:
            assert(0);
            break;
        }
    } while (stayOnMenu);

    initStatusMenuSub0(0);
    gMenuContext->m33C->m4_drawCursor = 0;
    gMenuContext->m33C->m3 = 0;

    return 1;
}

void drawCharacterInfoCardExtended() {
    if (gMenuContext->m33C->m7_drawCurrentSelectedCharacterInfoCard != 0) {
        transformAndAddPrim(1, &gMenuContext->m358->m1EAC, &gMenuContext->m358->m0_portrait, gMenuContext->m358->m2AE0); // character portrait
        transformAndAddPrim(1, &gMenuContext->m358->m1EC0, &gMenuContext->m358->m50_name, gMenuContext->m358->m2AE0); // character name
        transformAndAddPrim(gMenuContext->m358->m2AED, &gMenuContext->m358->m2AA0[0], &gMenuContext->m358->m1E00_polys[0], gMenuContext->m358->m2AEE);
        MissingCode();
    }
}

void drawMenu_35CSub0() {
    for (int i = 0; i < 7; i++) {
        if (gMenuContext->m35C->m32EA[i]) {
            if (gMenuContext->m35C->m32F2) {
                assert(0);
            }
            long dummy1, dummy2;
            RotTransPers4(&gMenuContext->m35C->m3100[i][0], &gMenuContext->m35C->m3100[i][1], &gMenuContext->m35C->m3100[i][2], &gMenuContext->m35C->m3100[i][3],
                &gMenuContext->m35C->m2030[i][gMenuContext->m35C->m32DC[i]].x0y0,
                &gMenuContext->m35C->m2030[i][gMenuContext->m35C->m32DC[i]].x1y1,
                &gMenuContext->m35C->m2030[i][gMenuContext->m35C->m32DC[i]].x2y2,
                &gMenuContext->m35C->m2030[i][gMenuContext->m35C->m32DC[i]].x3y3,
                &dummy1, &dummy2);
            AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &gMenuContext->m35C->m2030[i][gMenuContext->m35C->m32DC[i]]);
            transformAndAddPrim(gMenuContext->m35C->m32C0[i], &gMenuContext->m35C->m2A00[i][0], &gMenuContext->m35C->mEB0[i][0], gMenuContext->m35C->m32C7[i]);
        }
    }
}

void drawMenu_35C() {
    if (gMenuContext->m33C->m8_drawCurrentSelectedCharacterStats) {
        transformAndAddPrim(gMenuContext->m35C->m32F3, &gMenuContext->m35C->m2420[0], &gMenuContext->m35C->m0[0], gMenuContext->m35C->m32F1);
        drawMenu_35CSub0();
    }
}