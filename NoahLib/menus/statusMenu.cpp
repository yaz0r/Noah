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
    std::array<SFP_VEC4, 4> m1EAC;
    std::array<SFP_VEC4, 4> m1EC0;
    std::array<std::array<SFP_VEC4, 4>, 2> m2AA0;
    u8 m2AE0;
    u8 m2AED;
    u8 m2AEE;
    //size 0x2AF0
};

struct sMenuContext_35C {
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
    std::vector<std::vector<u8>::iterator> relocatedFile;

    if (param_1 < 0x10) {
        setCurrentDirectory(0x10, 0);
        file.resize(getFileSizeAligned(2));
        readFile(2, file, 0, 0x80);
        waitReadCompletion(0);
        relocatedFile = doPointerRelocation(file.mData);
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
    std::array<std::array<SFP_VEC4, 4>, 4> m140;
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
        param_1->mB8_attackTotal = (ushort)gameState.m26C_party[param_2].m4[0].m0 +
            (ushort)gameState.m26C_party[param_2].m58_attack +
            (ushort)gameState.m26C_party[param_2].m28;
    }

    param_1->mBA_hitTotal = gameState.m26C_party[param_2].m5E_hitPercentage + gameState.m26C_party[param_2].m2E;
    param_1->mBC_defenceTotal = gameState.m26C_party[param_2].m2D + gameState.m26C_party[param_2].m59_defence + gameState.m26C_party[param_2].m29;
    param_1->mBE_evadeTotal = gameState.m26C_party[param_2].m5F_evadePercentage + gameState.m26C_party[param_2].m2F;
    param_1->mC0_etherTotal = gameState.m26C_party[param_2].m5B_ether + gameState.m26C_party[param_2].m2B;
    param_1->mC2_etherDefenceTotal = gameState.m26C_party[param_2].m5C_etherDefence + gameState.m26C_party[param_2].m2C;
    param_1->mC4_agilityTotal = gameState.m26C_party[param_2].m5A_agility + gameState.m26C_party[param_2].m2A;

    if (0xfa < param_1->mB8_attackTotal) {
        param_1->mB8_attackTotal = 0xfa;
    }
    if (99 < param_1->mBA_hitTotal) {
        param_1->mBA_hitTotal = 99;
    }
    if (0xfa < param_1->mBC_defenceTotal) {
        param_1->mBC_defenceTotal = 0xfa;
    }
    if (99 < param_1->mBE_evadeTotal) {
        param_1->mBE_evadeTotal = 99;
    }
    if (0xfa < param_1->mC0_etherTotal) {
        param_1->mC0_etherTotal = 0xfa;
    }
    if (0xfa < param_1->mC2_etherDefenceTotal) {
        param_1->mC2_etherDefenceTotal = 0xfa;
    }
    if (0x14 < param_1->mC4_agilityTotal) {
        param_1->mC4_agilityTotal = 0x10;
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

void setupStatusMenuForCharacter3(uint param_1, s8 param_2) {
    setupStatusMenuForCharacter3Sub0(param_1, param_2);
    setupStatusMenuForCharacter3Sub1(param_1, param_2, gameState.m22B1_isOnGear[param_1]);
    MissingCode();
}

std::array<s32, 3> setupStatusMenuForCharacter4PolyOffset = { {
    0xC, 0x6, 0xC
} };


void setupStatusMenuForCharacter4() {
    initStatusMenuSub0(0);

    auto& p = gMenuContext->m4E0[gMenuContext->m338_currentSaveState + 0xC].m0_polys[gMenuContext->m308_oddOrEven];

    p.x0 = cursorPosXTable[7 + gMenuContext->m338_currentSaveState] + setupStatusMenuForCharacter4PolyOffset[gMenuContext->m338_currentSaveState] + 0x16;
    p.y0 = cursorPosYTable[7 + gMenuContext->m338_currentSaveState] - 0x22;

    p.x1 = p.x0 + gMenuContext->m4E0[gMenuContext->m338_currentSaveState + 0xC].m7E_stringWidth;
    p.y1 = p.y0;

    p.x2 = p.x0;
    p.y2 = cursorPosYTable[7 + gMenuContext->m338_currentSaveState] - 0x15;

    p.x3 = p.x0 + gMenuContext->m4E0[gMenuContext->m338_currentSaveState + 0xC].m7E_stringWidth;
    p.y3 = cursorPosYTable[7 + gMenuContext->m338_currentSaveState] - 0x15;

    gMenuContext->m4E0[gMenuContext->m338_currentSaveState + 0xc].m7D = gMenuContext->m308_oddOrEven;
    gMenuContext->m33C->m14[gMenuContext->m338_currentSaveState] = 1;
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

    gMenuContext->m338_currentSaveState = 2;
    gMenuContext->m339 = -1;
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
        if (gMenuContext->m338_currentSaveState != gMenuContext->m339) {
            setupStatusMenuForCharacter4();
            processLoadSaveMenuSub8(0);
            gMenuContext->m339 = gMenuContext->m338_currentSaveState;
        }
        switch (gMenuContext->m325_menuButton) {
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