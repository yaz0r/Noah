#include "noahLib.h"
#include "menuHandler.h"
#include "field/field.h"
#include "field/dialogWindows.h"
#include "kernel/filesystem.h"
#include "kernel/TIM.h"
#include "kernel/decompress.h"
#include "kernel/gameState.h"
#include "kernel/memory.h"
#include "kernel/gameMode.h"

#include "menus/menuGold.h"
#include "menus/statusMenu.h"

sMenuContext *gMenuContext = nullptr;

void updateMenuSelection(u8 count, u8 selectedEntry, std::vector<std::array<u32, 2>>::iterator config);

s32 menuIdToOpen = 0xFF;
s32 menuOpenCount;

u8 useDebugMenuList = 1;
u8 menuToEnter = 0;

sLoadableDataRaw menuSharedResources;
sLoadableDataRaw menuOverlay;

void setupMenuContext1D4(sMenuContext_1D4* param_1)
{
    (param_1->m0_DrawEnv).dtd = 1;
    (param_1->m5C_DispEnv).screen.y = 10;
    (param_1->m5C_DispEnv).screen.w = 0x100;
    (param_1->m0_DrawEnv).isbg = '\0';
    (param_1->m0_DrawEnv).r0 = 0;
    (param_1->m0_DrawEnv).g0 = 0;
    (param_1->m0_DrawEnv).b0 = 0;
    (param_1->m5C_DispEnv).screen.x = 0;
    (param_1->m5C_DispEnv).screen.h = 0xd8;
}

void setupScreenGeometryForMenu(void) {
    SetGeomOffset(0xa0, 0x70);
    SetGeomScreen(0x200);
    SetDefDispEnv(&gMenuContext->m6C_drawContexts[0].m5C_DispEnv, 0, 0xe0, 0x140, 0xe0);
    SetDefDrawEnv(&gMenuContext->m6C_drawContexts[0].m0_DrawEnv, 0, 0, 0x140, 0xe0);
    SetDefDispEnv(&gMenuContext->m6C_drawContexts[1].m5C_DispEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&gMenuContext->m6C_drawContexts[1].m0_DrawEnv, 0, 0xe0, 0x140, 0xe0);
    setupMenuContext1D4(&gMenuContext->m6C_drawContexts[0]);
    setupMenuContext1D4(&gMenuContext->m6C_drawContexts[1]);
}

void setupMenuContextTransforms(void)
{
    gMenuContext->m1E0_menuTranslation.vx = 0;
    gMenuContext->m1E0_menuTranslation.vy = 0;
    gMenuContext->m1E0_menuTranslation.vz = 0x800;

    gMenuContext->m1D8_menuRotation.vz = 0;
    gMenuContext->m1D8_menuRotation.vy = 0;
    gMenuContext->m1D8_menuRotation.vx = 0;

    gMenuContext->m220_menuTranslation2.vx = 0;
    gMenuContext->m220_menuTranslation2.vy = 0;
    gMenuContext->m220_menuTranslation2.vz = 0x800;

    gMenuContext->m218_menuRotation2.vz = 0;
    gMenuContext->m218_menuRotation2.vy = 0;
    gMenuContext->m218_menuRotation2.vx = 0;

    gMenuContext->m2E8 = 1;
    gMenuContext->m329 = 0;
}

void initMenuContext33C(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m33C;
        gMenuContext->m33C = nullptr;
    }
    else {
        gMenuContext->m33C = new sMenuContext_33C;
    }
}

void initMenuContext350MainMenu(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m350_mainMenu;
        gMenuContext->m350_mainMenu = nullptr;
    }
    else {
        gMenuContext->m350_mainMenu = new sMenuContext_350;
        //memset(gMenuContext->m350_mainMenu, 0, sizeof(sMenuContext_350));
    }
}

void initMenuContext354(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m354;
    }
    else {
        gMenuContext->m354 = new sMenuContext_354;
        memset(gMenuContext->m354, 0, sizeof(sMenuContext_354));
    }
}

void initMenuContext330(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m330;
    }
    else {
        gMenuContext->m330 = new sMenuContext_330;
        memset(gMenuContext->m330, 0, sizeof(sMenuContext_330));
    }
}

void initMenuContext348(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m348_cursor;
    }
    else {
        gMenuContext->m348_cursor = new sMenuContext_348;
        //memset(gMenuContext->m348_cursor, 0, sizeof(sMenuContext_348));
    }
}

void initMenuContext32C(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m32C_memoryCardContext;
    }
    else {
        gMenuContext->m32C_memoryCardContext = new sMenuContext_32C;
        memset(gMenuContext->m32C_memoryCardContext, 0, sizeof(sMenuContext_32C));
    }
}

void initMenuContext340_goldOwned(int mode) {
    if (mode == 0) {
        delete gMenuContext->m340_gold;
        gMenuContext->m340_gold = nullptr;
    }
    else {
        gMenuContext->m340_gold = new sMenuContext_340_goldOwned;
    }
}

void initMenuContext344_playTime(int mode) {
    if (mode == 0) {
        delete gMenuContext->m344_playTime;
        gMenuContext->m344_playTime = nullptr;
    }
    else {
        gMenuContext->m344_playTime = new sMenuContext_344_playTime;
    }
}

void initMenuContext39C(int mode) {
    if (mode == 0) {
        for (int i = 0; i < 3; i++) {
            delete gMenuContext->m39C[i];
            gMenuContext->m39C[i] = nullptr;
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            gMenuContext->m39C[i] = new sMenuContext_39C;
        }
    }
}

void initMenuContext(void)
{
    initMenuContext33C(1);
    initMenuContext350MainMenu(1);
    initMenuContext354(1);
    initMenuContext330(1);
    initMenuContext348(1);
    gMenuContext->m428 = new sMenuContext_428;
    memset(gMenuContext->m428, 0, sizeof(sMenuContext_428));
    if (menuToEnter != 2) {
        if (menuToEnter < 3) {
            if (menuToEnter != 0) {
                return;
            }
            initMenuContext32C(1);
            initMenuContext340_goldOwned(1);
            initMenuContext344_playTime(1);
            initMenuContext39C(1);
            return;
        }
        if (menuToEnter != 6) {
            return;
        }
    }
    initMenuContext32C(1);
}

void computeMenuBorder(std::vector<u8>& param_1, int param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7, int* param_8)
{
    int iVar2;

    std::vector<u8>::iterator data = param_1.begin() + READ_LE_U16(param_1.begin() + 4 + param_2 * 2);

    *param_3 = READ_LE_S16(data + 0);
    if (READ_LE_S16(data + 0x14) == 0) {
        iVar2 = (int)((uint)(ushort)READ_LE_S16(data + 4) << 0x10) >> 0x14;
    }
    else {
        iVar2 = (int)((uint)(ushort)READ_LE_S16(data + 4) << 0x10) >> 0x12;
    }
    *param_4 = READ_LE_S16(data + 0x14);
    *param_5 = READ_LE_S16(data + 0x16);
    *param_6 = READ_LE_S16(data + 0x18);
    *param_7 = (short)(READ_LE_S16(data + 0x1A) & 0xffc0) + iVar2;
    *param_8 = (int)(short)(READ_LE_S16(data + 0x1C) & 0xff00) + READ_LE_S16(data + 0x6);
    return;
}


void loadMenuSharedResources() {

    std::vector<std::vector<u8>::iterator> relocatedPtr = doPointerRelocation(menuSharedResources.mData);

    MissingCode();

    loadMenuImageBundle(mallocAndDecompress(relocatedPtr[1]));

    gMenuContext->m2DC_font = mallocAndDecompress(relocatedPtr[2]);
    gMenuContext->m2E0_textBundle = mallocAndDecompress(relocatedPtr[3]);

    int menuBorderData[3][6];

    computeMenuBorder(gMenuContext->m2DC_font, 0x0e0, &menuBorderData[0][0], &menuBorderData[0][1], &menuBorderData[0][2], &menuBorderData[0][3], &menuBorderData[0][4], &menuBorderData[0][5]);
    computeMenuBorder(gMenuContext->m2DC_font, 0x14b, &menuBorderData[0][0], &menuBorderData[0][1], &menuBorderData[0][2], &menuBorderData[0][3], &menuBorderData[0][4], &menuBorderData[0][5]);
    computeMenuBorder(gMenuContext->m2DC_font, 0x14c, &menuBorderData[1][0], &menuBorderData[1][1], &menuBorderData[1][2], &menuBorderData[1][3], &menuBorderData[1][4], &menuBorderData[1][5]);
    computeMenuBorder(gMenuContext->m2DC_font, 0x14d, &menuBorderData[2][0], &menuBorderData[2][1], &menuBorderData[2][2], &menuBorderData[2][3], &menuBorderData[2][4], &menuBorderData[2][5]);

    menuBorderData[1][4] += 0xC;

    std::vector<u8> portraits = mallocAndDecompress(relocatedPtr[4]);
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m33C->m30[i] != -1) {
            OpenTIM(portraits.begin() + gMenuContext->m33C->m30[i] * 0xB20);

            TIM_IMAGE portraitTim;
            ReadTIM(&portraitTim);
            (portraitTim.crect)->x = menuBorderData[i][2];
            (portraitTim.crect)->y = menuBorderData[i][3];
            (portraitTim.prect)->x = menuBorderData[i][4];
            (portraitTim.prect)->y = menuBorderData[i][5];
            LoadImage(portraitTim.crect, (u8*)portraitTim.caddr);
            LoadImage(portraitTim.prect, (u8*)portraitTim.paddr);
        }
    }

    MissingCode();
}

u8 menuOpenArg = 0;
u8 defaultSelectedMenuEntry = 0;

void initMenuGlobals() {

    if ((menuToEnter == '\0') && (menuOpenArg == '\0')) {
        gMenuContext->m336_menuSelectedEntry = defaultSelectedMenuEntry;
    }
    else {
        gMenuContext->m336_menuSelectedEntry = 1;
    }
    gMenuContext->m337_previousMenuSelectedEntry = -1;
    gMenuContext->m326 = 0x3c;
    gMenuContext->m334 = 0;
    gMenuContext->m335 = 0;

    MissingCode();
    for (int i = 0; i < 3; i++) {
        gMenuContext->m33C->m60[i] = 0;
        s32 partyMemberIndex = gameState.m1D34_currentParty[i];
        if ((partyMemberIndex == 0xff) || (gMenuContext->m30C[partyMemberIndex] == 0)) {
            gMenuContext->m33C->m30[i] = -1;
        }
        else {
            gMenuContext->m33C->m30[i] = partyMemberIndex;
            gMenuContext->m32B++;
            if (gameState.m26C[gMenuContext->m33C->m30[i]].m78_partyData_gearNum != -1) {
                gMenuContext->m33C->m60[i] = 1;
                gMenuContext->m33B++;
            }
        }
    }

    MissingCode();
    loadMenuSharedResources();
}

void resetMenuOddOrEven(void)
{
    gMenuContext->m308_oddOrEven = 0;
    return;
}

void loadBlankImage(void)
{
    u8* buffer = new u8[0x20];
    memset(buffer, 0, 0x20);
    *(u16*)(buffer + 2) = 0x7FFF;

    RECT localRect;
    localRect.y = 0x1c0;
    localRect.w = 0x10;
    localRect.x = 0;
    localRect.h = 1;

    LoadImage(&localRect, buffer);
    DrawSync(0);

    delete[] buffer;
}

std::vector<std::array<u8, 2>> DAT_Menu2__801ea524 = { {
    {{0x9, 0xA}},
    {{0xB, 0xC}},
}};

u8 renderString(std::vector<u8>::iterator buffer, std::vector<u16>& param_2, ushort param_3, byte param_4) {
    sDialogWindow18 sDialogWindow18_temp;
    sDialogWindow18_temp.mA_width1 = param_3 | 1;
    sDialogWindow18_temp.mC_height = 1;
    sDialogWindow18_temp.m12_widthPadded = sDialogWindow18_temp.mA_width1 + 3;
    sDialogWindow18_temp.m8_widthTimes4 = (short)((int)((uint)(ushort)sDialogWindow18_temp.mA_width1 << 0x10) >> 0xe);
    sDialogWindow18_temp.m68 = 1;
    sDialogWindow18_temp.m84_delay = 0;
    sDialogWindow18_temp.m6C_autoClose = 0;
    sDialogWindow18_temp.m6A = 0;
    sDialogWindow18_temp.m10_flags = 0;
    sDialogWindow18_temp.m0.vy = 0;
    sDialogWindow18_temp.m0.vx = 0;
    sDialogWindow18_temp.m69 = 100;
    sDialogWindow18_temp.m28_perLineBuffer.push_back(sDialogWindow18PerLineBufferEntry());
    sDialogWindow18_temp.m28_perLineBuffer[0].m5A = param_4 & 1;
    sDialogWindow18_temp.m28_perLineBuffer[0].m58_widthDiv4 = 0;
    sDialogWindow18_temp.m1C_currentStringToPrint = buffer;
    sDialogWindow18_temp.m2C_inRamDialogTextImage = param_2;
    updateDialogTextImage(&sDialogWindow18_temp);
    param_2 = sDialogWindow18_temp.m2C_inRamDialogTextImage;
    return sDialogWindow18_temp.m28_perLineBuffer[0].m58_widthDiv4 << 2;
}

void initMemoryCardTransparentPoly(POLY_FT4* param_1)
{
    SetPolyFT4(param_1);
    SetSemiTrans(param_1, 0);
    SetShadeTex(param_1, 0);
    param_1->r0 = 0x80;
    param_1->g0 = 0x80;
    param_1->b0 = 0x80;
    return;
}

void initMenuContext_1DE0(sMenuContext_4E0* param_1, int param_2, int param_3, byte param_4) {
    for (int i = 0; i < 2; i++) {
        POLY_FT4* p = &param_1->m0_polys[i];
        initMemoryCardTransparentPoly(p);
        if (param_4 == 0) {
            param_1->m7C_colorMode = param_2 & 1;
            p->tpage = GetTPage(0, 0, 0x140, 0);
            p->u0 = (char)(param_2 / 2) * -0x80;
            
            int width = param_2 + param_3;
            if (width < 0) {
                width += 3;
            }
            int width2 = (char)(width >> 2) * 0xD;
            p->v0 = width2;
            p->u1 = p->u0 + param_1->m7E_stringWidth;
            p->v1 = width2;
            p->u2 = p->u0;
            p->v2 = width2 + 0xD;
            p->u3 = p->u0 + param_1->m7E_stringWidth;
            p->v3 = width2 + 0xD;
        } else {
            assert(0);
        }
        if (param_1->m7C_colorMode == 0) {
            param_1->m0_polys[i].clut = textSpriteMode0;
        }
        else {
            param_1->m0_polys[i].clut = textSpriteMode1;
        }
    }
    param_1->m7F = 0;
}

void setupMenuContext4E0(sMenuContext_4E0* param_1, const std::vector<std::array<u8,2>>::iterator& param_2, int startY, int count) {
    for (int i = 0; i < count; i +=2) {
        sMenuContext_4E0* pEntry = &param_1[i];
        sMenuContext_4E0* pEntry2 = &param_1[i+1];

        pEntry->m7E_stringWidth = renderString(getDialogParamPointer(gMenuContext->m2E0_textBundle.begin(), (*param_2)[0]), gMenuContext->m4E0[0].m78_imageData, 0x18, 0);
        pEntry2->m7E_stringWidth = renderString(getDialogParamPointer(gMenuContext->m2E0_textBundle.begin(), (*param_2)[1]), gMenuContext->m4E0[0].m78_imageData, 0x18, 1);

        int rectY = i + startY;
        if (rectY < 0) {
            rectY += 3;
        }

        pEntry->m70_rect.x = (((short)i - (short)(i >> 0x1f)) * 0x10 & 0x20U) + 0x140;
        pEntry->m70_rect.y = (rectY >> 2) * 0xd;
        pEntry->m70_rect.w = 0x1c;
        pEntry->m70_rect.h = 0xd;

        MissingCode(); // some weird weird math

        initMenuContext_1DE0(pEntry, i, startY, 0);
        initMenuContext_1DE0(pEntry2, i+1, startY, 0);

        LoadImage(&pEntry->m70_rect, (u8*)&gMenuContext->m4E0[0].m78_imageData[0]);
        DrawSync(0);
    }
}

void initMenuContext4E0(void)
{
    initFontPalettes(0, 0x1d1);
    gMenuContext->m4E0[0].m78_imageData.resize(0x1C7);
    setupMenuContext4E0(&gMenuContext->m4E0[0], DAT_Menu2__801ea524.begin(), 0, 4);
    loadBlankImage();
}

void initPrimitives_348_cursor()
{
    for (int i = 0; i < 2; i++) {
        setupPolyG4(&gMenuContext->m348_cursor->m50_polyG4[i], 0x80, 0x80, 0);
        SetSemiTrans(&gMenuContext->m348_cursor->m50_polyG4[i], 1);

        SetLineF3(&gMenuContext->m348_cursor->mC8_lines[i]);
        gMenuContext->m348_cursor->mC8_lines[i].r0 = 0;
        gMenuContext->m348_cursor->mC8_lines[i].g0 = 0x40;
        gMenuContext->m348_cursor->mC8_lines[i].b0 = 0;

        SetLineF3(&gMenuContext->m348_cursor->mF8_lines[i]);
        gMenuContext->m348_cursor->mF8_lines[i].r0 = 0;
        gMenuContext->m348_cursor->mF8_lines[i].g0 = 0x40;
        gMenuContext->m348_cursor->mF8_lines[i].b0 = 0;

        SetPolyF4(&gMenuContext->m348_cursor->m98_poly[i]);
        gMenuContext->m348_cursor->m98_poly[i].x0y0.set(0, 0);
        gMenuContext->m348_cursor->m98_poly[i].x1y1.set(0x140, 0);
        gMenuContext->m348_cursor->m98_poly[i].x2y2.set(0, 0xe0);
        gMenuContext->m348_cursor->m98_poly[i].x3y3.set(0x140, 0xe0);
        gMenuContext->m348_cursor->m98_poly[i].r0 = 0x80;
        gMenuContext->m348_cursor->m98_poly[i].g0 = 0x80;
        gMenuContext->m348_cursor->m98_poly[i].b0 = 0x80;
        SetSemiTrans(&gMenuContext->m348_cursor->m98_poly[i], 1);

        RECT localRect;
        localRect.y = 0;
        localRect.x = 0;
        localRect.h = 0x100;
        localRect.w = 0x100;

        SetDrawMode(&gMenuContext->m348_cursor->m128[i], 0, 0, GetTPage(0, 0, 0x140, 0x80), &localRect);
        SetDrawMode(&gMenuContext->m348_cursor->m140[i], 0, 0, GetTPage(0, 2, 0x180, 0x00), &localRect);
    }
}

void computeMenuBorders(void)
{
    computeMenuBorder(gMenuContext->m2DC_font, 0xfe, &gMenuContext->m46C_menuBorders[0].m0, &gMenuContext->m46C_menuBorders[0].m4_tp, &gMenuContext->m46C_menuBorders[0].m8_clutX,
        &gMenuContext->m46C_menuBorders[0].mC_clutY, &gMenuContext->m46C_menuBorders[0].m10_tpageX, &gMenuContext->m46C_menuBorders[0].m14_tpageY);
    computeMenuBorder(gMenuContext->m2DC_font, 0x103, &gMenuContext->m46C_menuBorders[1].m0, &gMenuContext->m46C_menuBorders[1].m4_tp, &gMenuContext->m46C_menuBorders[1].m8_clutX,
        &gMenuContext->m46C_menuBorders[1].mC_clutY, &gMenuContext->m46C_menuBorders[1].m10_tpageX, &gMenuContext->m46C_menuBorders[1].m14_tpageY);
    computeMenuBorder(gMenuContext->m2DC_font, 0x100, &gMenuContext->m46C_menuBorders[2].m0, &gMenuContext->m46C_menuBorders[2].m4_tp, &gMenuContext->m46C_menuBorders[2].m8_clutX,
        &gMenuContext->m46C_menuBorders[2].mC_clutY, &gMenuContext->m46C_menuBorders[2].m10_tpageX, &gMenuContext->m46C_menuBorders[2].m14_tpageY);
    computeMenuBorder(gMenuContext->m2DC_font, 0x101, &gMenuContext->m46C_menuBorders[3].m0, &gMenuContext->m46C_menuBorders[3].m4_tp, &gMenuContext->m46C_menuBorders[3].m8_clutX,
        &gMenuContext->m46C_menuBorders[3].mC_clutY, &gMenuContext->m46C_menuBorders[3].m10_tpageX, &gMenuContext->m46C_menuBorders[3].m14_tpageY);
    return;
}

void setupMainMenu(void)
{
    (gMenuContext->m350_mainMenu->m1180).x = 0x2c0;
    (gMenuContext->m350_mainMenu->m1180).y = 0x100;
    (gMenuContext->m350_mainMenu->m1180).w = 0x140;
    (gMenuContext->m350_mainMenu->m1180).h = 0xe0;
    gMenuContext->m348_cursor->m15B = 0x40;
    initMenuGlobals();
    resetMenuOddOrEven();
    initMenuContext4E0();
    initPrimitives_348_cursor();
    computeMenuBorders();

    switch (menuToEnter) {
    case 0:
        MissingCode(); //?
        break;
    case 2:
        gMenuContext->m348_cursor->m15B = 0x4C;
        break;
    default:
        assert(0);
    }
    MissingCode();
}

u8 menuReturnState0 = 0;

void drawMenuBackground(void)
{
    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[8], &gMenuContext->m348_cursor->m98_poly[gMenuContext->m308_oddOrEven]);
    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[8], &gMenuContext->m348_cursor->m140[gMenuContext->m308_oddOrEven]);
}

void drawMultiPolys(int param_1, std::array<POLY_FT4, 2>* param_2, int param_3)
{
    for (int i = 0; i < param_1; i++) {
        AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &param_2[i][param_3]);
    }
}


void drawMainMenu(void)
{
    if (gMenuContext->m33C->m9_drawMainMenu != 0) {

        if (gMenuContext->m350_mainMenu->m1192 != gMenuContext->m350_mainMenu->m1193) {
            if (gMenuContext->m350_mainMenu->m1192 == 0) {
                for (int i = 0; i < gMenuContext->m350_mainMenu->m118C_numEntries; i++) {
                    SetSemiTrans(&gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191], 0);
                    SetShadeTex(&gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191], 0);
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].tpage |= 0x20;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].r0 = 0x80;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].g0 = 0x80;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].b0 = 0x80;
                }
                for (int i = 0; i < gMenuContext->m350_mainMenu->m1188_numEntries2; i++) {
                    SetSemiTrans(&gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190], 0);
                    SetShadeTex(&gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190], 0);
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].tpage |= 0x20;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].r0 = 0x80;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].g0 = 0x80;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].b0 = 0x80;
                }
            }
            else
            {
                for (int i = 0; i < gMenuContext->m350_mainMenu->m118C_numEntries; i++) {
                    SetSemiTrans(&gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191], 1);
                    SetShadeTex(&gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191], 0);
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].tpage |= 0x20;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].r0 = 0x20;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].g0 = 0x20;
                    gMenuContext->m350_mainMenu->m8C0_polys[i][gMenuContext->m350_mainMenu->m1191].b0 = 0x20;
                }
                for (int i = 0; i < gMenuContext->m350_mainMenu->m1188_numEntries2; i++) {
                    SetSemiTrans(&gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190], 1);
                    SetShadeTex(&gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190], 0);
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].tpage |= 0x20;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].r0 = 0x20;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].g0 = 0x20;
                    gMenuContext->m350_mainMenu->m0_polys[i][gMenuContext->m350_mainMenu->m1190].b0 = 0x20;
                }
            }
            gMenuContext->m350_mainMenu->m1193 = gMenuContext->m350_mainMenu->m1192;
        }

        drawMultiPolys(gMenuContext->m350_mainMenu->m118C_numEntries, &gMenuContext->m350_mainMenu->m8C0_polys[0], gMenuContext->m350_mainMenu->m1191);
        drawMultiPolys(gMenuContext->m350_mainMenu->m1188_numEntries2, &gMenuContext->m350_mainMenu->m0_polys[0], gMenuContext->m350_mainMenu->m1190);
    }
}

void drawMenu_348_cursor(void)
{
    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &gMenuContext->m348_cursor->m128[gMenuContext->m348_cursor->m159_oddOrEven]);
    if (gMenuContext->m33C->m4_drawCursor != 0) {
        AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &gMenuContext->m348_cursor->m0[gMenuContext->m348_cursor->m158_oddOrEven]);
    }
    return;
}

void displayGoldAndPlayTime(void)
{
    if (gMenuContext->m33C->m5_drawGold != 0) {
        sMenuContext_340_goldOwned* psVar1 = gMenuContext->m340_gold;
        drawMultiPolys(psVar1->m320_goldPolyLength, &psVar1->m0_goldPolys[0], gMenuContext->m340_gold->m324_oddOrEven);
        AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &gMenuContext->m340_gold->m2D0[gMenuContext->m340_gold->m324_oddOrEven]);
    }

    if (gMenuContext->m33C->m6_drawPlayTime != 0) {
        drawMultiPolys(7, &gMenuContext->m344_playTime->m0_playTimePolys[0], (uint)(byte)gMenuContext->m344_playTime->m370_oddOrEven);
        drawMultiPolys(4, &gMenuContext->m344_playTime->m230[0], (uint)(byte)gMenuContext->m344_playTime->m370_oddOrEven);
    }

    return;
}

void transformAndDrawPolyFT4(std::array<SFP_VEC4, 4>& param_1, std::array<POLY_FT4, 2>& param_2, int param_3, int param_4)
{
    long lStack24;
    long lStack20;

    POLY_FT4* p = &param_2[param_3];
    RotTransPers4(&param_1[0], &param_1[1], &param_1[2], &param_1[3], &p->x0y0, &p->x1y1, &p->x2y2, &p->x3y3, &lStack24, &lStack20);
    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[param_4], p);
    return;
}

void drawMenuContext364_menuBox(int param_1, char param_2) {
    sMenuContext_364* pMenuBox = gMenuContext->m364_menuBoxes[param_1];
    for (int i = 0; i < 4; i++)
    {
        transformAndDrawPolyFT4(pMenuBox->m510[i], pMenuBox->m0[i], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    }

    if (param_2 == 0) {
        for (int i = 0; i < 2; i++) {
            transformAndDrawPolyFT4(pMenuBox->m6D0[i], pMenuBox->m410[i], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
        }
        transformAndDrawPolyFT4(pMenuBox->m6B0, pMenuBox->m3C0, pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    }

    transformAndDrawPolyFT4(pMenuBox->m590, pMenuBox->m140[0], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m5B0, pMenuBox->m140[1], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m5D0, pMenuBox->m1E0[0], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m5F0, pMenuBox->m1E0[1], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m610, pMenuBox->m280[0], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m630, pMenuBox->m280[1], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m650, pMenuBox->m320[0], pMenuBox->m71C_oddOrEven, pMenuBox->m718);
    transformAndDrawPolyFT4(pMenuBox->m670, pMenuBox->m320[1], pMenuBox->m71C_oddOrEven, pMenuBox->m718);

    long lStack32;
    long lStack28;
    RotTransPers4(&pMenuBox->m690[0], &pMenuBox->m690[1], &pMenuBox->m690[2], &pMenuBox->m690[3],
        (&pMenuBox->m4B0[pMenuBox->m71C_oddOrEven].x0y0),
        (&pMenuBox->m4B0[pMenuBox->m71C_oddOrEven].x1y1),
        (&pMenuBox->m4B0[pMenuBox->m71C_oddOrEven].x2y2),
        (&pMenuBox->m4B0[pMenuBox->m71C_oddOrEven].x3y3),
        &lStack32, &lStack28);

    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[pMenuBox->m718], &pMenuBox->m4B0[pMenuBox->m71C_oddOrEven]);
    AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[pMenuBox->m718], &pMenuBox->m4F8[pMenuBox->m71C_oddOrEven]);

}

void drawMenuBoxes(void)
{
    int iVar1;
    SFP_VEC4 local_50;
    FP_VEC4 local_48;
    MATRIX MStack56;

    iVar1 = 0;
    do {
        if (gMenuContext->m33C->m20_menuBoxEnabled[iVar1] != 0) {
            if (gMenuContext->m364_menuBoxes[iVar1]->m714_transformDisabled == 0) {
                PushMatrix();
                local_50.vz = 0;
                local_50.vy = 0;
                local_50.vx = 0;
                local_48.vy = 0;
                local_48.vx = 0;
                local_48.vz = 0x200;
                createRotationMatrix(&local_50, &MStack56);
                TransMatrix(&MStack56, &local_48);
                SetRotMatrix(&MStack56);
                SetTransMatrix(&MStack56);
                drawMenuContext364_menuBox(iVar1, gMenuContext->m364_menuBoxes[iVar1]->m71D);
                PopMatrix();
            }
            else {
                drawMenuContext364_menuBox(iVar1, gMenuContext->m364_menuBoxes[iVar1]->m71D);
            }
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 7);
    return;
}

void drawCharacterInfoCards() {
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m33C->m0_isInfoCardEnabled[i]) {
            sMenuContext_39C* pInfoCard = gMenuContext->m39C[i];
            AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &pInfoCard->m0_portraitPoly[pInfoCard->m1270_oddOrEven]);
            AddPrim(&gMenuContext->m1D4_currentDrawContext->m70_OT[4], &pInfoCard->m50_NamePoly[pInfoCard->m1270_oddOrEven]);
            drawMultiPolys(pInfoCard->m1279_FixedStringLength, &pInfoCard->mA0_FixedStrings[0], pInfoCard->m1270_oddOrEven);
            drawMultiPolys(pInfoCard->m1273_HPLength, &pInfoCard->mAF0_PolyHP[0], (uint)pInfoCard->m1270_oddOrEven);
            drawMultiPolys(pInfoCard->m1274_MaxHPLength, &pInfoCard->mBE0_PolyMaxHP[0], (uint)pInfoCard->m1270_oddOrEven);
            drawMultiPolys(pInfoCard->m1275_MPLength, &pInfoCard->mCD0_PolyMP[0], (uint)pInfoCard->m1270_oddOrEven);
            drawMultiPolys(pInfoCard->m1276_MaxMPLength, &pInfoCard->mD70_PolyMaxMP[0], (uint)pInfoCard->m1270_oddOrEven);
            drawMultiPolys(pInfoCard->m1277_E10Length, &pInfoCard->mE10_polys[0], (uint)pInfoCard->m1270_oddOrEven);
            //drawMultiPolys(pInfoCard->m1278_E10Length2, &pInfoCard->m1040_polys[0], (uint)pInfoCard->m1270_oddOrEven); // Disabled in original exe
            drawMultiPolys(pInfoCard->m1271_LevelLength, &pInfoCard->m910_LevelPolys[0], (uint)pInfoCard->m1270_oddOrEven);
            //drawMultiPolys(pInfoCard->m1272_Level2Length, &pInfoCard->mA00_Level2Polys[0], (uint)pInfoCard->m1270_oddOrEven); // Disabled in original exe
        }
    }
}

void drawMenu0(void)
{
    MissingCode();
    /*drawMenu_380();
    drawMenu2_sub();
    drawMenu_428();*/
    drawMenu_348_cursor();
    MissingCode();
    /*drawMenu_34C();
    drawMenuMemoryCard();*/
    drawCharacterInfoCards();
    /*FUN_Menu2__801ce660();
    drawMenu_35C();
    drawMenu_360();*/
    displayGoldAndPlayTime();
    /*drawMenu_1E08();
    FUN_Menu2__801d1aac();
    FUN_Menu2__801d14fc();
    FUN_Menu2__801d1640();
    FUN_Menu2__801d17c4();
    FUN_Menu2__801d1914();
    FUN_Menu2__801d1464();
    FUN_Menu2__801d14b0();*/
    drawMenuBoxes();
    drawMainMenu();
    MissingCode();
    //drawMenuContext354();
    return;
}

void drawMenu2(void)
{
    MissingCode();
    //drawMenu_380();
    //drawMenu2_sub();
    //drawMenu_428();
    drawMenu_348_cursor();
    MissingCode();
    //drawMenu_34C();
    //drawMenuMemoryCard();
    drawMainMenu();
    //drawMenuContext354();
    drawMenuBoxes();
    MissingCode();
}

void drawMenuSpecificElements(void)
{
    if (gMenuContext->m327 != 0) {
        switch (menuToEnter) {
        case 0:
            drawMenu0();
            break;
        case 2:
            drawMenu2();
            break;
        default:
            assert(0);
        }
    }
    drawMenuBackground();
}

void FUN_Menu2__801c8574(int) {
    MissingCode();
}

void updateMenuInput(void) {
    MissingCode();

    u8 newValue = 8;

    do {
        if (loadInputFromVSyncBuffer() == 0) {
            break;
        }
        if ((newPadButtonForField & controllerButtons::RIGHT) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 0;
        }
        if ((newPadButtonForField & controllerButtons::DOWN) != 0) {
            newValue = 1;
        }
        if ((newPadButtonForField & controllerButtons::LEFT) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 2;
        }
        if ((newPadButtonForField & controllerButtons::UP) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 3;
        }
        if ((newPadButtonForDialogs & controllerButtons::INTERACT) != 0) {
            FUN_Menu2__801c8574(2);
            newValue = 4;
        }
        if ((newPadButtonForDialogs & controllerButtons::CROSS) != 0) {
            FUN_Menu2__801c8574(3);
            newValue = 5;
        }
        if ((newPadButtonForDialogs & controllerButtons::JUMP) != 0) {
            newValue = 6;
        }
        if ((newPadButtonForDialogs & controllerButtons::TRIANGLE) != 0) {
            newValue = 7;
        }
        if ((newPadButtonForField & controllerButtons::L1) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 10;
        }
        if ((newPadButtonForField & controllerButtons::R1) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 9;
        }
    } while (0);
    MissingCode();
    //while ((newPadButtonForDialogs & 0x100) == 0);

    gMenuContext->m325_menuButton = newValue;
}

void updatePlayTimeString(uint param_1)
{
    sMenuContext* psVar1;
    uint uVar2;
    uint uVar3;
    uint uVar4;
    uint uVar5;

    psVar1 = gMenuContext;
    uVar2 = (param_1 % 21600000) % 0x20f580;
    uVar5 = (uVar2 >> 6) / 0xd2f;
    uVar2 = uVar2 + uVar5 * -0x34bc0;
    uVar3 = uVar2 % 36000;
    uVar4 = uVar3 % 0xe10;
    gMenuContext->m302_playTimeString[0] = param_1 / 21600000;
    psVar1->m302_playTimeString[1] = (param_1 % 21600000) / 0x20f580;
    psVar1->m302_playTimeString[2] = uVar5;
    psVar1->m302_playTimeString[3] = uVar2 / 36000;
    psVar1->m302_playTimeString[4] = uVar3 / 0xe10;
    psVar1->m302_playTimeString[5] = uVar4 / 600;
    psVar1->m302_playTimeString[6] = (uVar4 % 600) / 0x3c;
    return;
}

int setupStringInPolyFT4(std::vector<u8>& fontData, int character, std::array<POLY_FT4, 2>* polyArray, int oddOrEven, short x, short y, ushort scale) {
    std::vector<u8>::iterator characterData = fontData.begin() + READ_LE_U16(fontData.begin() + character * 2 + 4);
    if (READ_LE_U16(characterData)) {
        int offsetToCharacterData = 4;
        for (int i = 0; i < READ_LE_U16(characterData); i++)
        {
            std::vector<u8>::iterator characterData2 = characterData + offsetToCharacterData;

            s32 x0Tmp = READ_LE_S16(characterData2 + 4 * 2) * scale;
            if (x0Tmp < 0) {
                x0Tmp = x0Tmp + 0xfff;
            }
            s32 y0Tmp = READ_LE_S16(characterData2 + 5 * 2) * scale;
            if ((int)y0Tmp < 0) {
                y0Tmp = y0Tmp + 0xfff;
            }
            s32 x1Tmp = READ_LE_S16(characterData2 + 2 * 2) * scale;
            if (x1Tmp < 0) {
                x1Tmp = x1Tmp + 0xfff;
            }
            s32 y1Tmp = READ_LE_S16(characterData2 + 3 * 2) * scale;
            if ((int)y1Tmp < 0) {
                y1Tmp = y1Tmp + 0xfff;
            }

            POLY_FT4* p = &(*polyArray)[oddOrEven];

            SetPolyFT4(p);
            SetSemiTrans(p, 0);
            SetShadeTex(p, 1);

            p->tpage = GetTPage(READ_LE_U16(characterData2 + 8 * 2), 0, READ_LE_U16(characterData2 + 11 * 2), READ_LE_U16(characterData2 + 12 * 2));
            p->clut = GetClut(READ_LE_U16(characterData2 + 9 * 2), READ_LE_U16(characterData2 + 10 * 2));

            //setup UVs
            s32 v0Tmp = READ_LE_U16(characterData2 + 0 * 2);
            s32 uVar1 = READ_LE_U16(characterData2 + 1 * 2);
            s32 width = READ_LE_U16(characterData2 + 2 * 2);
            s32 height = READ_LE_U16(characterData2 + 3 * 2);
            s32 u0Tmp = v0Tmp - 1;
            s32 x1 = (short)(x1Tmp >> 0xc);
            s32 x0 = (short)(x0Tmp >> 0xc);
            if (READ_LE_U16(characterData2 + 0xD * 2) == '\0') {
                x0 = x + x0;
                x1 = x1 + x0;
                p->x0y0.vx = x0;
                p->x1y1.vx = x1;
                p->x2y2.vx = x0;
                p->x3y3.vx = x1;
                u0Tmp = v0Tmp;
            }
            else {
                x0 = x + x0;
                x1 = x1 + x0;
                p->x0y0.vx = x1;
                p->x1y1.vx = x0;
                p->x2y2.vx = x1;
                p->x3y3.vx = x0;
                if ((int)((uint)u0Tmp << 0x10) < 0) {
                    u0Tmp = 0;
                    width = width + -1;
                }
            }
            v0Tmp = uVar1 - 1;
            s32 y1 = (short)(y1Tmp >> 0xc);
            s32 y0 = (short)(y0Tmp >> 0xc);
            if (READ_LE_U16(characterData2 + 0xD * 2) == '\0') {
                s32 yTemp = y + y0;
                p->x0y0.vy = yTemp;
                p->x1y1.vy = yTemp;
                p->x2y2.vy = y1 + yTemp;
                p->x3y3.vy = y1 + yTemp;
                v0Tmp = uVar1;
            }
            else {
                x1 = y + y0;
                p->x0y0.vy = y1 + x1;
                p->x1y1.vy = y1 + x1;
                p->x2y2.vy = x1;
                p->x3y3.vy = x1;
                if ((int)((uint)v0Tmp << 0x10) < 0) {
                    v0Tmp = 0;
                    height = height + -1;
                }
            }
            s32 u0 = (u8)u0Tmp;
            s32 v0 = (u8)v0Tmp;
            p->u0 = u0;
            p->v0 = v0;
            p->u1 = u0 + width;
            p->v1 = v0;
            p->u2 = u0;
            p->v2 = v0 + height;
            p->u3 = u0 + width;
            p->v3 = v0 + height;

            offsetToCharacterData += 0x1c;
            polyArray++;
        }
    }
    return READ_LE_U16(characterData);
}


void initMenuContext33C_playTime(short x, short y)
{
    int currentX = x;
    for (int i = 0; i < 3; i++) {
        setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m302_playTimeString[i], &gMenuContext->m344_playTime->m0_playTimePolys[i], gMenuContext->m308_oddOrEven, currentX, y, 0x1000);
        currentX = currentX + 8;
    }

    currentX = x + 0x18;
    for (int i = 3; i < 5; i++) {
        setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m302_playTimeString[i], &gMenuContext->m344_playTime->m0_playTimePolys[i], gMenuContext->m308_oddOrEven, currentX + 8, y, 0x1000);
        currentX = currentX + 8;
    }

    currentX = x + 0x28;
    for (int i = 5; i < 7; i++) {
        setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m302_playTimeString[i], &gMenuContext->m344_playTime->m0_playTimePolys[i], gMenuContext->m308_oddOrEven, currentX + 0x10, y, 0x1000);
        currentX = currentX + 8;
    }

    setupStringInPolyFT4(gMenuContext->m2DC_font, 0xee, &gMenuContext->m344_playTime->m230[0], gMenuContext->m308_oddOrEven, x + 0x18, y, 0x1000);
    setupStringInPolyFT4(gMenuContext->m2DC_font, 0xee, &gMenuContext->m344_playTime->m230[2], gMenuContext->m308_oddOrEven, x + 0x30, y, 0x1000);
    gMenuContext->m344_playTime->m370_oddOrEven = gMenuContext->m308_oddOrEven;
}


void drawMenuPlayTime(void)
{
    if (gMenuContext->m33C->m6_drawPlayTime != 0) {
        initMenuContext33C_playTime(0xd0, 0xca);
    }
    return;
}

void menuDraw() {
    MissingCode();

    updateMenuInput();

    MissingCode();

    ClearOTagR(&gMenuContext->m1D4_currentDrawContext->m70_OT[0], 0x10);

    MissingCode();

    gMenuContext->m2D8_frameCounter = gMenuContext->m2D8_frameCounter + 1;
    updatePlayTimeString(playTimeInVsync);
    drawMenuPlayTime();
    drawMenuSpecificElements();

    DrawSync(0);
    VSync(0);
    PutDrawEnv(&gMenuContext->m1D4_currentDrawContext->m0_DrawEnv);
    PutDispEnv(&gMenuContext->m1D4_currentDrawContext->m5C_DispEnv);
    MoveImage(&gMenuContext->m350_mainMenu->m1180, 0, (gMenuContext->m308_oddOrEven == 0) * 0xe0);
    DrawOTag(&gMenuContext->m1D4_currentDrawContext->m70_OT[0xf]);

    MissingCode();
}

std::vector<std::array<u32, 2>> saveGameMainMenuConfig = { {
    {{0x109, 0x125}},
    {{0x10A, 0x124}},
    {{0x10B, 0x123}},
    {{0x110, 0x142}},
} };

void setupMainMenuWithConfig(int count, std::vector<std::array<u32, 2>>::iterator config) {
    gMenuContext->m350_mainMenu->m1192 = 0;
    gMenuContext->m350_mainMenu->m1193 = 0;
    gMenuContext->m33C->m9_drawMainMenu = 1;

    int j = 0;
    for (int i = 1; i <= count; i++) {
        if (i != count) {
            gMenuContext->m350_mainMenu->m1188_numEntries2 = 0;
            for (int k = 0; k < i; k++) {
                gMenuContext->m350_mainMenu->m1188_numEntries2 += setupStringInPolyFT4(gMenuContext->m2DC_font, config[k][0], &gMenuContext->m350_mainMenu->m0_polys[gMenuContext->m350_mainMenu->m1188_numEntries2], gMenuContext->m308_oddOrEven, 0xa0, 0x96, 0x1000);
            }
            gMenuContext->m350_mainMenu->m1190 = gMenuContext->m308_oddOrEven;
        }
        gMenuContext->m350_mainMenu->m118C_numEntries = 0;
        if (i != 1) {
            for (int k = 0; k < j; k++) {
                gMenuContext->m350_mainMenu->m118C_numEntries += setupStringInPolyFT4(gMenuContext->m2DC_font, config[k][1], &gMenuContext->m350_mainMenu->m8C0_polys[gMenuContext->m350_mainMenu->m118C_numEntries], gMenuContext->m308_oddOrEven, 0xa0, 0x96, 0x1000);
            }
            gMenuContext->m350_mainMenu->m1191 = gMenuContext->m308_oddOrEven;
        }
        for (int k = 0; k < 2; k++) {
            menuDraw();
        }
        j++;
    }
}

void j_setupMenuContext4E0(uint param_1, sMenuContext_4E0* param_2, const std::vector<std::array<u8, 2>>::iterator& param_3)
{
    setupMenuContext4E0(param_2, param_3, 4, param_1 & 0xff);
    return;
}

std::vector<std::array<u8, 2>> saveGameMainMenuConfig2 = { {
    {{0x8, 0x1}},
    {{0x0, 0x0}},
    {{0x12, 0x11}},
    {{0x10, 0x13}},
} };

std::array<s32, 7> cursorPosXTable = { {
        0x49, 0x48, 0x43, 0x3C, 0x34, 0x26, 0x17
} };

std::array<s32, 7> cursorPosYTable = { {
        0xC9, 0xB5, 0xA2, 0x90, 0x7E, 0x6F, 0x61
} };

void updateCursorPolysLocation(int param_1, char param_2) {
    int X = cursorPosXTable[param_1];
    int Y = cursorPosYTable[param_1];
    setupStringInPolyFT4(gMenuContext->m2DC_font, 0x108, &gMenuContext->m348_cursor->m0, gMenuContext->m308_oddOrEven, X, Y, 0x1000);
    gMenuContext->m348_cursor->m158_oddOrEven = gMenuContext->m308_oddOrEven;

    if (param_2) {
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x0y0.vx = X + 0x14;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x0y0.vy = Y + -0x24;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x1y1.vx = X + gMenuContext->m348_cursor->m15B + 0x14;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x1y1.vy = Y + -0x24;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x2y2.vx = X + 0x14;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x2y2.vy = Y + -0x14;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x3y3.vx = X + gMenuContext->m348_cursor->m15B + 0x14;
        gMenuContext->m348_cursor->m50_polyG4[gMenuContext->m308_oddOrEven].x3y3.vy = Y + -0x14;

        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x0y0.vx = X + 0x14;
        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x0y0.vy = Y + -0x24;
        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x1y1.vx = X + gMenuContext->m348_cursor->m15B + 0x14;
        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x1y1.vy = Y + -0x24;
        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x2y2.vx = X + gMenuContext->m348_cursor->m15B + 0x14;
        gMenuContext->m348_cursor->mC8_lines[gMenuContext->m308_oddOrEven].x2y2.vy = Y + -0x14;

        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x0y0.vx = X + 0x14;
        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x0y0.vy = Y + -0x24;
        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x1y1.vx = X + 0x14;
        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x1y1.vy = Y + -0x14;
        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x2y2.vx = X + gMenuContext->m348_cursor->m15B + 0x14;
        gMenuContext->m348_cursor->mF8_lines[gMenuContext->m308_oddOrEven].x2y2.vy = Y + -0x14;
        gMenuContext->m33C->m3 = 1;
    }
}

void updateMenuSelection(u8 count, u8 selectedEntry, std::vector<std::array<u32, 2>>::iterator config) {
    gMenuContext->m350_mainMenu->m1188_numEntries2 = 0;
    gMenuContext->m350_mainMenu->m118C_numEntries = 0;

    for (int i = 0; i < count; i++) {
        s32 character;
        if (i == selectedEntry) {
            character = config[i][0] + 0xd;
        }
        else {
            character = config[i][0];
        }

        gMenuContext->m350_mainMenu->m1188_numEntries2 += setupStringInPolyFT4(gMenuContext->m2DC_font, character, &gMenuContext->m350_mainMenu->m0_polys[gMenuContext->m350_mainMenu->m1188_numEntries2], gMenuContext->m308_oddOrEven, 0xa0, 0x96, 0x1000);
        gMenuContext->m350_mainMenu->m118C_numEntries += setupStringInPolyFT4(gMenuContext->m2DC_font, config[i][1], &gMenuContext->m350_mainMenu->m8C0_polys[gMenuContext->m350_mainMenu->m118C_numEntries], gMenuContext->m308_oddOrEven, 0xa0, 0x96, 0x1000);
    }

    gMenuContext->m350_mainMenu->m1190 = gMenuContext->m308_oddOrEven;
    gMenuContext->m350_mainMenu->m1191 = gMenuContext->m308_oddOrEven;
    updateCursorPolysLocation(selectedEntry, 1);
    gMenuContext->m33C->m4_drawCursor = 1;
}

void resetCursorState(void)
{
    gMenuContext->m33C->m4_drawCursor = 0;
    gMenuContext->m33C->m3 = 0;
    return;
}

void countField33C_C(u8 count, std::array<u8, 8>& array)
{
    for(int i=0; i<count; i++) {
        array[i] = 0;
    }
}

u8 menuVarUnk0 = 1;
u8 menuVarUnk1 = 1;

int processLoadSaveMenu(char param_1, byte param_2)
{
    MissingCode();

    return 0;
}

int menu2_executeMainMenuSelection(byte param_1) {
    
    u8 flag0 = 1;
    menuVarUnk1 = 1;
    u8 flag1 = 1;

    switch (gMenuContext->m336_menuSelectedEntry + param_1) {
    case 0: // exit
        flag1 = 0;
        flag0 = 0;
        break;        
    case 6:
        flag1 = openStatusMenu();
        break;
    case 8:
        if (processLoadSaveMenu(1, 0)) {
            menuReturnState0 = 2;
            flag0 = 0;
        }
        break;
    case 9:
        loadInitialGameState();
        flag1 = 0;
        flag0 = 0;
        break;
    default:
        assert(0);
    }

    gMenuContext->m32C_memoryCardContext->m4FE6 = 0;
    if (flag1 != 0) {
        assert(0);
    }

    MissingCode();

    gMenuContext->m350_mainMenu->m1192 = 0;
    gMenuContext->m350_mainMenu->m1193 = 1;
    gMenuContext->m33C->m4_drawCursor = 1;
    gMenuContext->m33C->m3 = 1;
    gMenuContext->m337_previousMenuSelectedEntry = -1;
    gMenuContext->m33C->mA_354Enabled = 0;
    menuVarUnk0 = 1;
    return flag0;
}

void loadSaveGameMenuExecute() {
    u8 cVar1 = 1;

    setupMainMenuWithConfig(4, saveGameMainMenuConfig.begin());
    j_setupMenuContext4E0(8, &gMenuContext->m4E0[4], saveGameMainMenuConfig2.begin());
    gMenuContext->m2D8_frameCounter = 0;

    do {
        menuDraw();
        switch (gMenuContext->m325_menuButton) {
        case 0:
            break;
        case 1: // up
            if (gMenuContext->m336_menuSelectedEntry == 0) {
                gMenuContext->m336_menuSelectedEntry = 2;
            }
            else {
                gMenuContext->m336_menuSelectedEntry--;
            }
            gMenuContext->m2D8_frameCounter = 0;
            break;
        case 3: // down
            if (++gMenuContext->m336_menuSelectedEntry > 2) {
                gMenuContext->m336_menuSelectedEntry = 0;
            }
            gMenuContext->m2D8_frameCounter = 0;
            break;
        case 4:
            gMenuContext->m350_mainMenu->m1192 = 1;
            resetCursorState();
            countField33C_C(8, gMenuContext->m33C->mC);
            gMenuContext->m348_cursor->m15B = 0x40;
            cVar1 = menu2_executeMainMenuSelection(7);
            menuVarUnk0 = 0;
            gMenuContext->m2D8_frameCounter = 0;
            break;
        default:
            break;
        }

        if (gMenuContext->m336_menuSelectedEntry != gMenuContext->m337_previousMenuSelectedEntry) {
            updateMenuSelection(3, gMenuContext->m336_menuSelectedEntry, saveGameMainMenuConfig.begin());
            //updateMenuSelection2(8, gMenuContext->m4E0 + 4, saveGameMainMenuConfig2, &saveGameMainMenuConfig3, &gMenuContext->m33C->field_0xc, gMenuContext->m336_menuSelectedEntry, 0, 0);
            gMenuContext->m337_previousMenuSelectedEntry = gMenuContext->m336_menuSelectedEntry;
        }

        if ((getCurrentDiscNumber() == 1) && (600 < (uint)gMenuContext->m2D8_frameCounter)) {
            cVar1 = '\0';
            menuReturnState0 = 1;
        }
        if (cVar1 == '\0') {
            countField33C_C(8, gMenuContext->m33C->mC);
            return;
        }
    } while (true);
}

void checkCurrentDiscNumber(uint param_1) {
    MissingCode();
}

std::vector<std::array<u32, 2>> mainMenuConfig = { {
    {{0x109, 0x131}},
    {{0x10A, 0x130}},
    {{0x10B, 0x12F}},
    {{0x10C, 0x12E}},
    {{0x10D, 0x12D}},
    {{0x10E, 0x12C}},
    {{0x10F, 0x12B}},
} };

void mainMenuExecute() {
    int continueMenu = 1;
    do {
        menuDraw();

        switch (gMenuContext->m325_menuButton) {
        case 1: // down
            if (gMenuContext->m336_menuSelectedEntry == 0) {
                gMenuContext->m336_menuSelectedEntry = 6;
            }
            else {
                gMenuContext->m336_menuSelectedEntry = gMenuContext->m336_menuSelectedEntry + -1;
            }
            break;
        case 3: // up
            gMenuContext->m336_menuSelectedEntry++;
            if (6 < gMenuContext->m336_menuSelectedEntry) {
                gMenuContext->m336_menuSelectedEntry = 0;
            }
            break;
        case 4: // select
        {
            bool bVar1 = true;
            MissingCode();
            if (bVar1) {
                gMenuContext->m350_mainMenu->m1192 = 1;
                resetCursorState();
                countField33C_C(8, gMenuContext->m33C->mC);
                continueMenu = menu2_executeMainMenuSelection(0);
            }
            break;
        }
        case 5:
            continueMenu = 0;
            break;
        case 7:
        case 8:
            break;
        default:
            assert(0);
        }

        if (gMenuContext->m336_menuSelectedEntry != gMenuContext->m337_previousMenuSelectedEntry) {
            MissingCode();
            updateMenuSelection(7, gMenuContext->m336_menuSelectedEntry, mainMenuConfig.begin());
            //updateMenuSelection2(8, gMenuContext->m4E0 + 4, &DAT_Menu2__801ea528, &DAT_Menu2__801e9e64, &gMenuContext->m33C->mC, gMenuContext->m336_menuSelectedEntry, 0, 0);
            gMenuContext->m337_previousMenuSelectedEntry = gMenuContext->m336_menuSelectedEntry;
        }

    } while (continueMenu & 0xFF);
}


void setupMenuContext364(uint param_1)
{
    u16 uVar2;
    uint uVar4;
    byte bVar5;
    sMenuContext_364* psVar6;
    RECT local_28;

    param_1 = param_1 & 0xff;
    psVar6 = gMenuContext->m364_menuBoxes[param_1];
    local_28.y = 0;
    local_28.x = 0;
    local_28.h = 0x100;
    local_28.w = 0x100;
    gMenuContext->m33C->m20_menuBoxEnabled[param_1] = 0;
    bVar5 = 0;
    gMenuContext->m33C->m27[param_1] = 0;
    uVar4 = 0;
    do {
        SetPolyG4(&psVar6->m4B0[uVar4]);
        psVar6->m4B0[uVar4].r0 = 0x68;
        psVar6->m4B0[uVar4].g0 = 0x68;
        psVar6->m4B0[uVar4].b0 = 0x68;
        psVar6->m4B0[uVar4].r1 = 0x68;
        psVar6->m4B0[uVar4].g1 = 0x68;
        psVar6->m4B0[uVar4].b1 = 0x68;
        psVar6->m4B0[uVar4].r2 = 0x68;
        psVar6->m4B0[uVar4].g2 = 0x68;
        psVar6->m4B0[uVar4].b2 = 0x68;
        psVar6->m4B0[uVar4].r3 = 0x68;
        psVar6->m4B0[uVar4].g3 = 0x68;
        psVar6->m4B0[uVar4].b3 = 0x68;
        SetSemiTrans(&psVar6->m4B0[uVar4], 1);
        bVar5 = bVar5 + 1;
        SetDrawMode(&psVar6->m4F8[uVar4], 0, 0, GetTPage(0, 0, gMenuContext->m46C_menuBorders[0].m10_tpageX, gMenuContext->m46C_menuBorders[0].m14_tpageY), &local_28);
        uVar4 = (uint)bVar5;
    } while (bVar5 < 2);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            SetPolyFT4(&psVar6->m140[i][j]);
            SetShadeTex(&psVar6->m140[i][j], 1);
            psVar6->m140[i][j].r0 = 0xff;
            psVar6->m140[i][j].g0 = 0xff;
            psVar6->m140[i][j].b0 = 0xff;
            psVar6->m140[i][j].tpage = GetTPage(gMenuContext->m46C_menuBorders[0].m4_tp, 0, gMenuContext->m46C_menuBorders[0].m10_tpageX, gMenuContext->m46C_menuBorders[0].m14_tpageY);
            psVar6->m140[i][j].clut = GetClut(gMenuContext->m46C_menuBorders[0].m8_clutX, gMenuContext->m46C_menuBorders[0].mC_clutY);

            SetPolyFT4(&psVar6->m1E0[i][j]);
            SetShadeTex(&psVar6->m1E0[i][j], 1);
            psVar6->m1E0[i][j].r0 = 0xff;
            psVar6->m1E0[i][j].g0 = 0xff;
            psVar6->m1E0[i][j].b0 = 0xff;
            psVar6->m1E0[i][j].tpage = GetTPage(gMenuContext->m46C_menuBorders[1].m4_tp, 0, gMenuContext->m46C_menuBorders[1].m10_tpageX, gMenuContext->m46C_menuBorders[1].m14_tpageY);
            psVar6->m1E0[i][j].clut = GetClut(gMenuContext->m46C_menuBorders[1].m8_clutX, gMenuContext->m46C_menuBorders[1].mC_clutY);

            SetPolyFT4(&psVar6->m280[i][j]);
            SetShadeTex(&psVar6->m280[i][j], 1);
            psVar6->m280[i][j].r0 = 0xff;
            psVar6->m280[i][j].g0 = 0xff;
            psVar6->m280[i][j].b0 = 0xff;
            psVar6->m280[i][j].tpage = GetTPage(gMenuContext->m46C_menuBorders[2].m4_tp, 0, gMenuContext->m46C_menuBorders[2].m10_tpageX, gMenuContext->m46C_menuBorders[2].m14_tpageY);
            psVar6->m280[i][j].clut = GetClut(gMenuContext->m46C_menuBorders[2].m8_clutX, gMenuContext->m46C_menuBorders[2].mC_clutY);

            SetPolyFT4(&psVar6->m320[i][j]);
            SetShadeTex(&psVar6->m320[i][j], 1);
            psVar6->m320[i][j].r0 = 0xff;
            psVar6->m320[i][j].g0 = 0xff;
            psVar6->m320[i][j].b0 = 0xff;
            psVar6->m320[i][j].tpage = GetTPage(gMenuContext->m46C_menuBorders[3].m4_tp, 0, gMenuContext->m46C_menuBorders[3].m10_tpageX, gMenuContext->m46C_menuBorders[3].m14_tpageY);
            psVar6->m320[i][j].clut = GetClut(gMenuContext->m46C_menuBorders[3].m8_clutX, gMenuContext->m46C_menuBorders[3].mC_clutY);
        }
    }
}

void initMemoryCardPolyVerts(std::array<SFP_VEC4, 4>& param_1, short param_2, short param_3, short param_4, short param_5)
{
    short sVar1;

    param_1[0].vx = param_2 + -0xa0;
    param_1[0].vy = param_3 + -0x70;
    param_1[0].vz = 0;
    sVar1 = param_2 + param_4 + -0xa0;
    param_1[1].vx = sVar1;
    param_1[1].vy = param_3 + -0x70;
    param_1[1].vz = 0;
    param_1[2].vx = param_2 + -0xa0;
    param_1[2].vz = 0;
    param_1[3].vx = sVar1;
    param_1[3].vz = 0;
    sVar1 = param_3 + param_5 + -0x70;
    param_1[2].vy = sVar1;
    param_1[3].vy = sVar1;
    return;
}

void initMenuTiles1Sub(POLY_FT4* param_1)
{
    SetSemiTrans(param_1, 1);
    SetShadeTex(param_1, 0);
    param_1->r0 = 0x80;
    param_1->g0 = 0x80;
    param_1->b0 = 0x80;
    return;
}

void initMenuTiles1(uint param_1, short param_2, short param_3, short param_4, short param_5)

{
    short sVar1;
    sMenuContext* psVar2;
    int iVar3;
    short sVar4;
    sMenuContext_364* polyArray;
    int iVar5;

    psVar2 = gMenuContext;
    polyArray = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    polyArray->m710 = 0;
    iVar3 = setupStringInPolyFT4(psVar2->m2DC_font, 0xfd, &polyArray->m0[0], psVar2->m308_oddOrEven, 0, 0, 0x1000);
    psVar2 = gMenuContext;
    iVar3 = iVar3 + polyArray->m710;
    polyArray->m710 = iVar3;
    iVar3 = setupStringInPolyFT4(psVar2->m2DC_font, 0xff, &polyArray->m0[iVar3], psVar2->m308_oddOrEven, 0, 0, 0x1000);
    psVar2 = gMenuContext;
    iVar3 = iVar3 + polyArray->m710;
    polyArray->m710 = iVar3;
    iVar3 = setupStringInPolyFT4(psVar2->m2DC_font, 0x102, &polyArray->m0[iVar3], psVar2->m308_oddOrEven, 0, 0, 0x1000);
    psVar2 = gMenuContext;
    iVar3 = iVar3 + polyArray->m710;
    polyArray->m710 = iVar3;
    iVar3 = setupStringInPolyFT4(psVar2->m2DC_font, 0x104, &polyArray->m0[iVar3], psVar2->m308_oddOrEven, 0, 0, 0x1000);
    polyArray->m710 = iVar3 + polyArray->m710;
    initMemoryCardPolyVerts(polyArray->m510[0], param_2 + -8, param_3 + 8, 0x10, -0x10);
    sVar4 = param_2 + param_4 + 8;
    initMemoryCardPolyVerts(polyArray->m510[1], sVar4, param_3 + 8, -0x10, -0x10);
    sVar1 = param_3 + param_5 + -8;
    initMemoryCardPolyVerts(polyArray->m510[2], param_2 + -8, sVar1, 0x10, 0x10);
    initMemoryCardPolyVerts(polyArray->m510[3], sVar4, sVar1, -0x10, 0x10);
    iVar3 = 0;
    do {
        iVar5 = iVar3 + 1;
        initMenuTiles1Sub(&polyArray->m0[iVar3][gMenuContext->m308_oddOrEven]);
        iVar3 = iVar5;
    } while (iVar5 < 4);
    return;
}

void initMenuTilesTopFrame(uint param_1, short param_2, short param_3, uint param_4)
{
    short sVar1;
    sMenuContext_364* psVar4;

    psVar4 = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    psVar4->m140[0][gMenuContext->m308_oddOrEven].u0 = '\0';
    psVar4->m140[0][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar4->m140[0][gMenuContext->m308_oddOrEven].u1 = '\a';
    psVar4->m140[0][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar4->m140[0][gMenuContext->m308_oddOrEven].u2 = '\0';
    psVar4->m140[0][gMenuContext->m308_oddOrEven].v2 = 0x94;
    psVar4->m140[0][gMenuContext->m308_oddOrEven].u3 = '\a';
    psVar4->m140[0][gMenuContext->m308_oddOrEven].v3 = 0x94;
    psVar4->m140[1][gMenuContext->m308_oddOrEven].u0 = '\0';
    psVar4->m140[1][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar4->m140[1][gMenuContext->m308_oddOrEven].u1 = '\a';
    psVar4->m140[1][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar4->m140[1][gMenuContext->m308_oddOrEven].u2 = '\0';
    psVar4->m140[1][gMenuContext->m308_oddOrEven].v2 = 0x94;
    psVar4->m140[1][gMenuContext->m308_oddOrEven].u3 = '\a';
    psVar4->m140[1][gMenuContext->m308_oddOrEven].v3 = 0x94;
    sVar1 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    initMemoryCardPolyVerts(psVar4->m590, param_2 + 8, param_3 + -8, sVar1, 0x10);
    initMemoryCardPolyVerts(psVar4->m5B0, param_2 + sVar1 + 8, param_3 + -8, sVar1, 0x10);

    for (int i = 0; i < 2; i++) {
        initMenuTiles1Sub(&psVar4->m140[i][gMenuContext->m308_oddOrEven]);
    }

    return;
}

void initMenuTilesBottomFrame(uint param_1, short param_2, short param_3, uint param_4, short param_5)
{
    short sVar1;
    short sVar2;
    int iVar3;
    int iVar4;
    sMenuContext_364* psVar5;

    psVar5 = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].u0 = 0x08;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].u1 = 0x0F;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].u2 = 0x08;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].v2 = 0x94;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].u3 = 0x0F;
    psVar5->m1E0[0][gMenuContext->m308_oddOrEven].v3 = 0x94;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].u0 = 0x08;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].u1 = 0x0F;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].u2 = 0x08;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].v2 = 0x94;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].u3 = 0x0F;
    psVar5->m1E0[1][gMenuContext->m308_oddOrEven].v3 = 0x94;

    iVar4 = 0;    
    sVar1 = param_3 + param_5 + -8;
    sVar2 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    initMemoryCardPolyVerts(psVar5->m5D0, param_2 + 8, sVar1, sVar2, 0x10);
    initMemoryCardPolyVerts(psVar5->m5F0, param_2 + sVar2 + 8, sVar1, sVar2, 0x10);
    for (int i = 0; i < 2; i++) {
        initMenuTiles1Sub(&psVar5->m1E0[i][gMenuContext->m308_oddOrEven]);
    }
    return;
}

void initMenuTiles4(uint param_1, short param_2, short param_3, uint param_4)

{
    short sVar1;
    int iVar2;
    int iVar3;
    sMenuContext_364* psVar4;

    psVar4 = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    psVar4->m280[0][gMenuContext->m308_oddOrEven].u0 = '\x10';
    psVar4->m280[0][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar4->m280[0][gMenuContext->m308_oddOrEven].u1 = ' ';
    psVar4->m280[0][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar4->m280[0][gMenuContext->m308_oddOrEven].u2 = '\x10';
    psVar4->m280[0][gMenuContext->m308_oddOrEven].v2 = 0x8b;
    psVar4->m280[0][gMenuContext->m308_oddOrEven].u3 = ' ';
    psVar4->m280[0][gMenuContext->m308_oddOrEven].v3 = 0x8b;
    psVar4->m280[1][gMenuContext->m308_oddOrEven].u0 = '\x10';
    psVar4->m280[1][gMenuContext->m308_oddOrEven].v0 = 0x84;
    psVar4->m280[1][gMenuContext->m308_oddOrEven].u1 = ' ';
    psVar4->m280[1][gMenuContext->m308_oddOrEven].v1 = 0x84;
    psVar4->m280[1][gMenuContext->m308_oddOrEven].u2 = '\x10';
    psVar4->m280[1][gMenuContext->m308_oddOrEven].v2 = 0x8b;
    psVar4->m280[1][gMenuContext->m308_oddOrEven].u3 = ' ';
    psVar4->m280[1][gMenuContext->m308_oddOrEven].v3 = 0x8b;
    sVar1 = (short)((int)((param_4 & 0xffff) - 0x10) / 2);
    initMemoryCardPolyVerts(psVar4->m610, param_2 + -8, param_3 + 8, 0x10, sVar1);
    initMemoryCardPolyVerts(psVar4->m630, param_2 + -8, param_3 + sVar1 + 8, 0x10, sVar1);
    iVar2 = 0;
    do {
        iVar3 = iVar2 + 1;
        initMenuTiles1Sub(&psVar4->m280[iVar2][gMenuContext->m308_oddOrEven]);
        iVar2 = iVar3;
    } while (iVar3 < 2);
    return;
}

void initMenuTiles5(uint param_1, short param_2, short param_3, short param_4, ushort param_5)
{
    short sVar1;
    short sVar2;
    int iVar3;
    int iVar4;
    sMenuContext_364* psVar5;

    psVar5 = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    psVar5->m320[0][gMenuContext->m308_oddOrEven].u0 = '\x10';
    psVar5->m320[0][gMenuContext->m308_oddOrEven].v0 = 0x8c;
    psVar5->m320[0][gMenuContext->m308_oddOrEven].u1 = ' ';
    psVar5->m320[0][gMenuContext->m308_oddOrEven].v1 = 0x8c;
    psVar5->m320[0][gMenuContext->m308_oddOrEven].u2 = '\x10';
    psVar5->m320[0][gMenuContext->m308_oddOrEven].v2 = 0x93;
    psVar5->m320[0][gMenuContext->m308_oddOrEven].u3 = ' ';
    psVar5->m320[0][gMenuContext->m308_oddOrEven].v3 = 0x93;
    psVar5->m320[1][gMenuContext->m308_oddOrEven].u0 = '\x10';
    psVar5->m320[1][gMenuContext->m308_oddOrEven].v0 = 0x8c;
    psVar5->m320[1][gMenuContext->m308_oddOrEven].u1 = ' ';
    sVar2 = param_2 + param_4 + -8;
    psVar5->m320[1][gMenuContext->m308_oddOrEven].v1 = 0x8c;
    psVar5->m320[1][gMenuContext->m308_oddOrEven].u2 = '\x10';
    psVar5->m320[1][gMenuContext->m308_oddOrEven].v2 = 0x93;
    psVar5->m320[1][gMenuContext->m308_oddOrEven].u3 = ' ';
    psVar5->m320[1][gMenuContext->m308_oddOrEven].v3 = 0x93;
    sVar1 = (short)((int)(param_5 - 0x10) / 2);
    initMemoryCardPolyVerts(psVar5->m650, sVar2, param_3 + 8, 0x10, sVar1);
    initMemoryCardPolyVerts(psVar5->m670, sVar2, param_3 + sVar1 + 8, 0x10, sVar1);
    iVar3 = 0;
    do {
        iVar4 = iVar3 + 1;
        initMenuTiles1Sub(&psVar5->m320[iVar3][gMenuContext->m308_oddOrEven]);
        iVar3 = iVar4;
    } while (iVar4 < 2);
    return;
}

void initMenuTiles6(uint param_1, short param_2, ushort param_3, s32 param_4, ushort param_5)
{
    sMenuContext_364* psVar1;

    psVar1 = gMenuContext->m364_menuBoxes[param_1 & 0xff];
    setupStringInPolyFT4(gMenuContext->m2DC_font, 0x105, &psVar1->m410[0], gMenuContext->m308_oddOrEven, param_2, param_3, 0x1000);
    //FUN_800263e4(gMenuContext->m2DC_font, 0x105, psVar1->field6_0x410[1], gMenuContext->m308_oddOrEven, param_2, (uint)param_3 + (uint)param_5 + -8, 0x1000, 0, 1);
    setupStringInPolyFT4(gMenuContext->m2DC_font, 0x106, &psVar1->m3C0, gMenuContext->m308_oddOrEven, param_2, param_3 + 8, 0x1000);
    initMemoryCardPolyVerts(psVar1->m6D0[0], param_2, param_3, 8, 8);
    initMemoryCardPolyVerts(psVar1->m6D0[1], param_2, param_3 + param_5, 8, -8);
    initMemoryCardPolyVerts(psVar1->m6B0, param_2, param_3 + 8, 8, param_5 - 8);
    return;
}

void draw380(uint param_1, short param_2, short param_3, short param_4, short param_5, byte param_6, s32 param_7, char param_8)
{
    sMenuContext* psVar1;
    sMenuContext_364* psVar2;

    param_1 = param_1 & 0xff;
    psVar2 = gMenuContext->m364_menuBoxes[param_1];
    gMenuContext->m33C->m20_menuBoxEnabled[param_1] = 0;
    initMemoryCardPolyVerts(psVar2->m690, param_2, param_3, param_4, param_5);
    initMenuTiles1(param_1, param_2, param_3, param_4, param_5);
    initMenuTilesTopFrame(param_1, param_2, param_3, param_4);
    initMenuTilesBottomFrame(param_1, param_2, param_3, param_4, param_5);
    initMenuTiles4(param_1, param_2, param_3, param_5);
    initMenuTiles5(param_1, param_2, param_3, param_4, param_5);
    if (param_8 != '\0') {
        initMenuTiles6(param_1, param_2, param_3, param_4, param_5);
    }
    psVar2->m71D = param_8;
    psVar1 = gMenuContext;
    psVar2->m714_transformDisabled = (uint)param_6;
    psVar2->m718 = param_7;
    psVar2->m71C_oddOrEven = psVar1->m308_oddOrEven;
    gMenuContext->m33C->m20_menuBoxEnabled[param_1] = 1;
}

void iniMenuContext364And380(byte param_1, short param_2, short param_3, ushort param_4, ushort param_5, char param_6, byte param_7, s32 param_8, byte param_9)
{
    if (1 < param_1) {
        gMenuContext->m364_menuBoxes[param_1] = new sMenuContext_364;
        gMenuContext->m380[param_1] = new sMenuContext_380;
        setupMenuContext364(param_1);
    }

    if (param_6 == 0) {
        draw380(param_1, param_2, param_3, param_4, param_5, param_7, param_8, param_9);
    }
    else {
        sMenuContext_380* psVar2 = gMenuContext->m380[param_1];
        psVar2->m10 = param_1;
        psVar2->m11_draw380 = 0;
        psVar2->m0 = param_2;
        psVar2->m2 = param_3;
        psVar2->m4 = param_4;
        psVar2->m6 = param_5;
        psVar2->m8 = 0;
        psVar2->mA = 0;
        gMenuContext->m33C->m27[param_1] = 1;
        psVar2->m12 = param_7;
        psVar2->mC = param_8;
    }
    return;
}

void fillStringDisplayBuffer(uint param_1)
{
    int iVar1;
    int iVar2;
    uint uVar3;
    uint uVar4;

    uVar3 = 100000000;
    iVar2 = 0;
    do {
        uVar4 = param_1 / uVar3;
        param_1 = param_1 % uVar3;
        iVar1 = iVar2 + 1;
        uVar3 = uVar3 / 10;
        gMenuContext->m31C_stringDisplayBuffer[iVar2] = (byte)uVar4;
        iVar2 = iVar1;
    } while (iVar1 < 9);
    iVar2 = 1;
    do {
        if (gMenuContext->m31C_stringDisplayBuffer[iVar2] != 0) {
            if (gMenuContext->m31C_stringDisplayBuffer[iVar2 + -1] != 0) {
                return;
            }
            gMenuContext->m31C_stringDisplayBuffer[iVar2 + -1] = 0xff;
            return;
        }
        iVar1 = iVar2 + 1;
        gMenuContext->m31C_stringDisplayBuffer[iVar2 + -1] = 0xff;
        iVar2 = iVar1;
    } while (iVar1 < 9);
    return;
}

void setupMenuPolyFT4(POLY_FT4* param_1, short x, short y, u8 u, u8 v, short width, short height)
{
    u8 u1 = u + width;
    u8 v1 = v + height;

    param_1->x0y0.vx = x;
    param_1->x0y0.vy = y;
    param_1->x1y1.vy = y;
    param_1->x2y2.vx = x;
    param_1->u0 = u;
    param_1->u2 = u;
    param_1->x1y1.vx = x + width;
    param_1->x2y2.vy = y + height;
    param_1->x3y3.vx = x + width;
    param_1->x3y3.vy = y + height;
    param_1->v0 = v;
    param_1->u1 = u1;
    param_1->v1 = v;
    param_1->v2 = v1;
    param_1->u3 = u1;
    param_1->v3 = v1;
}

static const std::array<u32, 19> infocardNameU = { {0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18} };
static const std::array<u32, 19> infocardNameV = { {0x3B,0x48,0x48,0x55,0x55,0x62,0x62,0x6F,0x6F,0x7C,0x7C,0x89,0x89,0x96,0x96,0xA3,0xA3,0xB0,0xB0} };

void updateCharacterPortraitAndName(s8 slot, uint characterId, short param_3, short param_4)
{
    setupStringInPolyFT4(gMenuContext->m2DC_font, slot + 0x14b, &gMenuContext->m39C[slot]->m0_portraitPoly, gMenuContext->m308_oddOrEven, param_3, param_4, 0x1000);
    initMemoryCardTransparentPoly(&gMenuContext->m39C[slot]->m50_NamePoly[gMenuContext->m308_oddOrEven]);
    gMenuContext->m39C[slot]->m50_NamePoly[gMenuContext->m308_oddOrEven].tpage = GetTPage(0, 0, 0x180, 0);
    if ((gMenuContext->m33C->m30[slot] & 1U) == 0) {
        gMenuContext->m39C[slot]->m50_NamePoly[gMenuContext->m308_oddOrEven].clut = textSpriteMode0;
    }
    else {
        gMenuContext->m39C[slot]->m50_NamePoly[gMenuContext->m308_oddOrEven].clut = textSpriteMode1;
    }

    static short infocardNameX = 0x30;
    static short infocardNameY = 0x11;
    setupMenuPolyFT4(&gMenuContext->m39C[slot]->m50_NamePoly[gMenuContext->m308_oddOrEven], infocardNameX + param_3, infocardNameY + param_4, (infocardNameU[slot] & 0x3f) * 4, infocardNameV[slot], 0x48, 0xd);
}

void updateCharacterInfoCardFixedStrings(s8 slot, short param_2, short param_3) {
    static const std::array<s8, 20> updateCharacterFixedStringsTable = { { //Note, this was a u32 in the original exe, but was using 0xFFFF as -1 and other values where never larger than a byte
            -1, 0x15, 0x1F,
            -1, 0x11, 0x19,
            0x3E, 0xE, 0x19,
            0x3E, 0x17, 0x28,
            0x3B, 0x37, 0x37,
            0x15, 0x1F, 0x1F,
            -1, -1
    } };

    static const std::array<u8, 20> updateCharacterFixedStringsX = { {
            0x48,          0x30,          0x38,          0x58,
            0x30,          0x38,          0x58,          0x30,
            0x38,          0x58,          0x80,          0x88,
            0x90,          0x98,          0x98,          0xA8,
            0xB0,          0xB0,          0x80,          0x80,

    } };

    static const std::array<u8, 20> updateCharacterFixedStringsY = { {
            0x00,          0x08,          0x08,          0x08,
            0x20,          0x20,          0x20,          0x28,
            0x28,          0x28,          0x18,          0x18,
            0x18,          0x18,          0x18,          0x18,
            0x18,          0x18,          0x20,          0x28,
    } };

    sMenuContext_39C* pSlot = gMenuContext->m39C[slot];
    pSlot->m1279_FixedStringLength = 0;
    for (int i = 0; i < 20; i++) {
        if (updateCharacterFixedStringsTable[i] != -1) {
            pSlot->m1279_FixedStringLength += setupStringInPolyFT4(gMenuContext->m2DC_font, updateCharacterFixedStringsTable[i], &pSlot->mA0_FixedStrings[pSlot->m1279_FixedStringLength], gMenuContext->m308_oddOrEven, param_2 + updateCharacterFixedStringsX[i], param_3 + updateCharacterFixedStringsY[i], 0x1000);
        }
    }
}

void updateCharacterHP(uint slot, uint characterId, short X, short Y) {
    int characterInfocardHPPositionX = 0x40;
    int characterInfocardHPPositionY = 0x20;

    sMenuContext_39C* pSlot = gMenuContext->m39C[slot];
    fillStringDisplayBuffer(gameState.m26C[characterId].m24_HP);
    pSlot->m1273_HPLength = 0;
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 6] != 0xff) {
            pSlot->m1273_HPLength += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 6], &pSlot->mAF0_PolyHP[pSlot->m1273_HPLength], gMenuContext->m308_oddOrEven, X + characterInfocardHPPositionX + 8 * i, Y + characterInfocardHPPositionY, 0x1000);
        }
    }

    int characterInfocardMaxHPPositionX = 0x60;
    int characterInfocardMaxHPPositionY = 0x20;

    fillStringDisplayBuffer(gameState.m26C[characterId].m26_MaxHP);
    pSlot->m1274_MaxHPLength = 0;
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 6] != 0xff) {
            pSlot->m1274_MaxHPLength += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 6], &pSlot->mBE0_PolyMaxHP[pSlot->m1274_MaxHPLength], gMenuContext->m308_oddOrEven, X + characterInfocardMaxHPPositionX + 8 * i, Y + characterInfocardMaxHPPositionY, 0x1000);
        }
    }
}

void updateCharacterMP(uint slot, uint characterId, short X, short Y) {
    int characterInfocardMPPositionX = 0x48;
    int characterInfocardMPPositionY = 0x28;

    sMenuContext_39C* pSlot = gMenuContext->m39C[slot];
    fillStringDisplayBuffer(gameState.m26C[characterId].m28_MP);
    pSlot->m1275_MPLength = 0;
    for (int i = 0; i < 2; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 7] != 0xff) {
            pSlot->m1275_MPLength += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 7], &pSlot->mCD0_PolyMP[pSlot->m1275_MPLength], gMenuContext->m308_oddOrEven, X + characterInfocardMPPositionX + 8 * i, Y + characterInfocardMPPositionY, 0x1000);
        }
    }

    int characterInfocardMaxMPPositionX = 0x60;
    int characterInfocardMaxMPPositionY = 0x28;

    fillStringDisplayBuffer(gameState.m26C[characterId].m28_MP);
    pSlot->m1276_MaxMPLength = 0;
    for (int i = 0; i < 2; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 7] != 0xff) {
            pSlot->m1276_MaxMPLength += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 7], &pSlot->mD70_PolyMaxMP[pSlot->m1276_MaxMPLength], gMenuContext->m308_oddOrEven, X + characterInfocardMaxMPPositionX + 8 * i, Y + characterInfocardMaxMPPositionY, 0x1000);
        }
    }
}

void updateCharacterXP(uint slot, uint characterId, short X, short Y) {
    sMenuContext_39C* pSlot = gMenuContext->m39C[slot];
    fillStringDisplayBuffer(gameState.m26C[characterId].m1C);
    pSlot->m1277_E10Length = 0;
    for (int i = 0; i < 7; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 2] != 0xff) {
            pSlot->m1277_E10Length += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 2], &pSlot->mE10_polys[pSlot->m1277_E10Length], gMenuContext->m308_oddOrEven, X + 0x88 + 8 * i, Y + 0x20, 0x1000);
        }
    }

    fillStringDisplayBuffer(gameState.m26C[characterId].m20);
    pSlot->m1278_E10Length2 = 0;
    for (int i = 0; i < 7; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 2] != 0xff) {
            pSlot->m1278_E10Length2 += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 2], &pSlot->m1040_polys[pSlot->m1278_E10Length2], gMenuContext->m308_oddOrEven, X + 0x88 + 8 * i, Y + 0x28, 0x1000);
        }
    }
}

void updateCharacterLevel(uint slot, uint characterId, short X, short Y) {
    sMenuContext_39C* pSlot = gMenuContext->m39C[slot];
    fillStringDisplayBuffer(gameState.m26C[characterId].m3A_Level);
    pSlot->m1271_LevelLength = 0;
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 6] != 0xff) {
            pSlot->m1271_LevelLength += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 6], &pSlot->m910_LevelPolys[pSlot->m1271_LevelLength], gMenuContext->m308_oddOrEven, X + 0x40 + 8 * i, Y + 0x8, 0x1000);
        }
    }

    fillStringDisplayBuffer(gameState.m26C[characterId].m3B_Level2);
    pSlot->m1272_Level2Length = 0;
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m31C_stringDisplayBuffer[i + 6] != 0xff) {
            pSlot->m1272_Level2Length += setupStringInPolyFT4(gMenuContext->m2DC_font, gMenuContext->m31C_stringDisplayBuffer[i + 6], &pSlot->mA00_Level2Polys[pSlot->m1272_Level2Length], gMenuContext->m308_oddOrEven, X + 0x60 + 8 * i, Y + 0x8, 0x1000);
        }
    }

    for(int i=0; i< pSlot->m1272_Level2Length; i++){
        SetShadeTex(&pSlot->mA00_Level2Polys[i][gMenuContext->m308_oddOrEven], 0);
        pSlot->mA00_Level2Polys[i][gMenuContext->m308_oddOrEven].r0 = 0;
        pSlot->mA00_Level2Polys[i][gMenuContext->m308_oddOrEven].g0 = 0x80;
        pSlot->mA00_Level2Polys[i][gMenuContext->m308_oddOrEven].b0 = 0x0;
    }
}

void updateCharacterInfoCard(s8 slot, s8 characterId) {
    if (slot != -1) {
        sMenuContextSub0* psVar2 = &gMenuContext->m0[slot];
        int infocardX = psVar2->m18_XOffset;
        if (infocardX < 0) {
            infocardX = infocardX + 0xff;
        }
        int infocardY = psVar2->m1C_YOffset;
        if (infocardY < 0) {
            infocardY = infocardY + 0xff;
        }

        infocardX = (infocardX >> 8) + psVar2->m0;
        infocardY = (infocardY >> 8) + psVar2->m8;

        updateCharacterPortraitAndName(slot, characterId, infocardX, infocardY);
        updateCharacterInfoCardFixedStrings(slot, infocardX, infocardY);
        updateCharacterHP(slot, characterId, infocardX, infocardY);
        updateCharacterMP(slot, characterId, infocardX, infocardY);
        updateCharacterXP(slot, characterId, infocardX, infocardY);
        updateCharacterLevel(slot, characterId, infocardX, infocardY);

        gMenuContext->m33C->m0_isInfoCardEnabled[slot] = 1;
        gMenuContext->m39C[slot]->m1270_oddOrEven = gMenuContext->m308_oddOrEven;
    }
}

void infocardAnimationStep(s8 slot) {
    for(int i=0; i< gMenuContext->m0[slot].m22_numSteps; i++) {
        if (gMenuContext->m0[slot].m20_XReversed == '\0') {
            gMenuContext->m0[slot].m18_XOffset += gMenuContext->m0[slot].m10_XStep;
        }
        else {
            gMenuContext->m0[slot].m18_XOffset -= gMenuContext->m0[slot].m10_XStep;
        }

        if (gMenuContext->m0[slot].m21_YReversed == '\0') {
            gMenuContext->m0[slot].m1C_YOffset += gMenuContext->m0[slot].m14_YStep;
        }
        else {
            gMenuContext->m0[slot].m1C_YOffset -= gMenuContext->m0[slot].m14_YStep;
        }
    }

    int iVar7;
    int iVar5;
    int iVar2;

    sMenuContextSub0* psVar6 = &gMenuContext->m0[slot];
    if (psVar6->m10_XStep == 0x100) {
        if (psVar6->m20_XReversed != '\0') {
            iVar7 = psVar6->m18_XOffset;
            if (iVar7 < 0) {
                iVar7 = iVar7 + 0xff;
            }
            if (psVar6->m4 <= (iVar7 >> 8) + psVar6->m0) {
                return;
            }
            goto LAB_Menu2__801c850c;
        }
        iVar7 = psVar6->m18_XOffset;
        if (iVar7 < 0) {
            iVar7 = iVar7 + 0xff;
        }
        iVar5 = psVar6->m0;
        iVar2 = psVar6->m4;
    }
    else {
        if (psVar6->m21_YReversed != '\0') {
            iVar7 = psVar6->m1C_YOffset;
            if (iVar7 < 0) {
                iVar7 = iVar7 + 0xff;
            }
            if (psVar6->mC <= (iVar7 >> 8) + psVar6->m8) {
                return;
            }
            goto LAB_Menu2__801c850c;
        }
        iVar7 = psVar6->m1C_YOffset;
        if (iVar7 < 0) {
            iVar7 = iVar7 + 0xff;
        }
        iVar5 = psVar6->m8;
        iVar2 = psVar6->mC;
    }
    if ((iVar7 >> 8) + iVar5 <= iVar2) {
        return;
    }
LAB_Menu2__801c850c:
    psVar6->m23 = 1;
    return;
}

void setupInfoCardAnimation(int X1, int X2, int Y1, int Y2, byte numSteps, byte slot) {
    int iVar3;
    int iVar4;

    gMenuContext->m0[slot].m0 = X1;
    gMenuContext->m0[slot].m8 = X2;
    gMenuContext->m0[slot].m4 = Y1;
    gMenuContext->m0[slot].mC = Y2;

    if (Y1 < X1) {
        iVar4 = X1 - Y1;
        gMenuContext->m0[slot].m20_XReversed = 1;
    }
    else {
        iVar4 = Y1 - X1;
        gMenuContext->m0[slot].m20_XReversed = 0;
    }

    if (Y2 < X2) {
        iVar3 = X2 - Y2;
        gMenuContext->m0[slot].m21_YReversed = 1;
    }
    else {
        iVar3 = Y2 - X2;
        gMenuContext->m0[slot].m21_YReversed = 0;
    }

    if (iVar4 < iVar3) {
        gMenuContext->m0[slot].m14_YStep = 0x100;
        gMenuContext->m0[slot].m10_XStep = (iVar4 << 8) / iVar3;
    }
    else {
        gMenuContext->m0[slot].m10_XStep = 0x100;
        gMenuContext->m0[slot].m14_YStep = (iVar3 << 8) / iVar4;
    }

    gMenuContext->m0[slot].m22_numSteps = numSteps;
    gMenuContext->m0[slot].m18_XOffset = 0;
    gMenuContext->m0[slot].m1C_YOffset = 0;
    gMenuContext->m0[slot].m23 = 0;
}

std::vector<std::array<u8, 2>> mainMenuConfig2 = { {
    {{0xA, 0x7}},
    {{0x6, 0x5}},
    {{0x4, 0x3}},
    {{0x2, 0x2}},
} };

void processLoadSaveMenuSub2(bool show, bool hideGold)
{
    if (!show) {
        countField33C_C(8, gMenuContext->m33C->mC);
        setupInfoCardAnimation(0x60, 6, 0x100, 0x86, 8, 0);
        setupInfoCardAnimation(0x68, 0x3e, 0x108, 0x3e, 8, 1);
        setupInfoCardAnimation(0x70, 0x76, 0x110, -10, 8, 2);
    }
    else {
        j_setupMenuContext4E0(8, &gMenuContext->m4E0[1], mainMenuConfig2.begin());
        setupInfoCardAnimation(0x100, 0x86, 0x60, 6, 8, 0);
        setupInfoCardAnimation(0x108, 0x3e, 0x68, 0x3e, 8, 1);
        setupInfoCardAnimation(0x110, -10, 0x70, 0x76, 8, 2);
    }

    MissingCode();

    while ((gMenuContext->m0[0].m23 == 0) || (gMenuContext->m0[1].m23 == 0) || (gMenuContext->m0[1].m23 == 0)) {
        for (int i = 0; i < 3; i++) {
            if (gMenuContext->m33C->m30[i] != -1) {
                updateCharacterInfoCard(i, gMenuContext->m33C->m30[i]);
            }
        }
        menuDraw();
        for (int i = 0; i < 3; i++) {
            if (gMenuContext->m33C->m30[i] != -1) {
                infocardAnimationStep(i);
            }
        }
    }

    MissingCode();

    if (!show) {
        gMenuContext->m33C->m0_isInfoCardEnabled[2] = 0;
        gMenuContext->m33C->m0_isInfoCardEnabled[1] = 0;
        gMenuContext->m33C->m0_isInfoCardEnabled[0] = 0;
        if (!hideGold) {
            gMenuContext->m33C->m20_menuBoxEnabled[0] = 0;
            gMenuContext->m33C->m5_drawGold = 0;
        }
    }
    else {
        gMenuContext->m0[0].m0 = 0x60;
        gMenuContext->m0[0].m8 = 6;
        gMenuContext->m0[1].m0 = 0x68;
        gMenuContext->m0[1].m8 = 0x3e;
        gMenuContext->m0[2].m0 = 0x70;
        gMenuContext->m0[2].m8 = 0x76;
        gMenuContext->m0[0].m1C_YOffset = 0;
        gMenuContext->m0[0].m18_XOffset = 0;
        gMenuContext->m0[1].m1C_YOffset = 0;
        gMenuContext->m0[1].m18_XOffset = 0;
        gMenuContext->m0[2].m1C_YOffset = 0;
        gMenuContext->m0[2].m18_XOffset = 0;
        for (int i = 0; i < 3; i++) {
            if (gMenuContext->m33C->m30[i] != -1) {
                updateCharacterInfoCard(i, gMenuContext->m33C->m30[i]);
            }
        }
        playMenuSoundEffect(0x5d);
        if (!hideGold) {
            updateGoldDisplay();
        }
        gMenuContext->m33C->m20_menuBoxEnabled[1] = 1;
        gMenuContext->m33C->m6_drawPlayTime = 1;
    }
}

void initMenuForPlaytime(void)
{
    iniMenuContext364And380(1, 0xcc, 0xc6, 0x50, 0x10, 0, 0, 4, 0);
    initMenuContext33C_playTime(0xd0, 0xca);
    gMenuContext->m33C->m6_drawPlayTime = 1;
    return;
}

void renderCharacterNameInVram(uint param_1, uint param_2)
{
    std::vector<u16> inRamName;
    inRamName.resize(0x3F6 / 2, 0);

    if ((param_1 & 0xff) != 0xff) {
        renderString(gameState.m0_names[param_1 & 0xfe].begin(), inRamName, 0x24, 0);
        renderString(gameState.m0_names[(param_1 & 0xfe) + 1].begin(), inRamName, 0x24, 1);
    }
    int iVar1 = (param_2 & 0xfe) /2;
    RECT local_20;
    local_20.x = infocardNameU[iVar1] + 0x180;
    local_20.y = infocardNameV[iVar1];
    local_20.w = 0x28;
    local_20.h = 0xd;
    LoadImage(&local_20, (u8*)&inRamName[0]);
    DrawSync(0);
}

void setupMenu0()
{
    if (menuToEnter == 0) {
        for (int i = 0; i < 2; i++) {
            gMenuContext->m364_menuBoxes[i] = new sMenuContext_364;
            gMenuContext->m380[i] = new sMenuContext_380;
            setupMenuContext364(i);
        }
        MissingCode();
    }
    for (int i = 0; i < 3; i++) {
        if (gMenuContext->m33C->m30[i] != -1) {
            renderCharacterNameInVram(gMenuContext->m33C->m30[i], i * 2);
            s8 gearIndex = gameState.m26C[(byte)gMenuContext->m33C->m30[i]].m78_partyData_gearNum;
            if (gearIndex == -1)
            {
                renderCharacterNameInVram(-1, 6 + 2 * i);
            }
            else {
                renderCharacterNameInVram(gearIndex + 0xB, 6 + 2 * i);
            }
        }
    }
    setupMainMenuWithConfig(8, mainMenuConfig.begin());
    processLoadSaveMenuSub2(true, false);
    initMenuForPlaytime();
}

void menu2_loadGame_entryPoint() {
    initMenuContext();
    setupMainMenu();
    gMenuContext->m327 = 1;
    gMenuContext->m32A = 1;
    switch (menuToEnter)
    {
    case 0:
        setupMenu0();
        mainMenuExecute();
        break;
    case 2:
        menuReturnState0 = 0;
        loadSaveGameMenuExecute();
        gMenuContext->m33C->m9_drawMainMenu = 0;
        gMenuContext->m33C->m4_drawCursor = 0;
        gMenuContext->m33C->m3 = 0;
        if (menuReturnState0 == '\0') {
            checkCurrentDiscNumber(0);
        }
        else if (menuReturnState0 == '\x02') {
            assert(0);
        }
        break;
    default:
        assert(0);
        break;
    }
    MissingCode();
    //menu2_mainLoop();
}

void processDebugMenuForMenuList(void) {
    if (useDebugMenuList) {
        assert(0);
    }
    setCurrentDirectory(0x10, 0);
    if (useDebugMenuList) {
        assert(0);
    }
    setCurrentDirectory(0x10, 0);

    switch (menuToEnter) {
    case 0:
        menu2_loadGame_entryPoint();
        break;
    case 2:
    case 6:
        menu2_loadGame_entryPoint();
        setGameMode(1);
        break;
    default:
        assert(0);
    }

    if (useDebugMenuList) {
        assert(0);
    }
}

void enterMenu(void)
{
    gMenuContext = new sMenuContext;

    gMenuContext->m325_menuButton = 8;
    resetMemoryAllocStats(2, 0);

    gMenuContext->m1D4_currentDrawContext = &gMenuContext->m6C_drawContexts[1];

    gMenuContext->m1E94 = 0;
    gMenuContext->m1E95 = 1;

    gMenuContext->m2D8_frameCounter = 0;
    gMenuContext->m327 = 0;

    setupScreenGeometryForMenu();

    if (useDebugMenuList != 0) {
        gMenuContext->m6C_drawContexts[0].m0_DrawEnv.isbg = 1;
        gMenuContext->m6C_drawContexts[1].m0_DrawEnv.isbg = 1;
    }
    setupMenuContextTransforms();
    VSync(0);
    PutDrawEnv(&gMenuContext->m6C_drawContexts[0].m0_DrawEnv);
    PutDrawEnv(&gMenuContext->m6C_drawContexts[1].m0_DrawEnv);
    PutDispEnv(&gMenuContext->m6C_drawContexts[0].m5C_DispEnv);
    PutDispEnv(&gMenuContext->m6C_drawContexts[1].m5C_DispEnv);
    SetDispMask(1);
    processDebugMenuForMenuList();
    useDebugMenuList = 1;
}

void loadAndOpenMenu(void)
{
    if ((menuIdToOpen != 0x80) || (menuDisabled == 0)) {
        MissingCode();

        setCurrentDirectory(0x10, 0);

        MissingCode();

        sLoadingBatchCommands menuLoadCommands[3];
        menuLoadCommands[0].m0_fileIndex = 1;
        menuLoadCommands[0].m4_loadPtr = &menuSharedResources;
        menuLoadCommands[1].m0_fileIndex = (menuIdToOpen & 0x7f) + 5;
        menuLoadCommands[1].m4_loadPtr = &menuOverlay;
        menuLoadCommands[2].m0_fileIndex = 0;
        menuLoadCommands[2].m4_loadPtr = nullptr;

        // TODO: figure that out
        /*if (((menuIdToOpen & 0x7f) == 5) && (mechaOverlayLoadingMode == 0)) {
            assert(0);
        }*/

        waitReadCompletion(0);
        batchStartLoadingFiles(menuLoadCommands, 0);
        setCurrentDirectory(4, 0);

        MissingCode(); // fade out and stuff

        waitReadCompletion(0);

        MissingCode();

        useDebugMenuList = 0;
        menuToEnter = menuIdToOpen & 0x7f;

        MissingCode();

        ClearCacheAfterOverlayLoad();
        enterMenu();
        ClearCacheAfterOverlayLoad();
        gDepthDivider = 2;
        if ((menuReturnState0 == '\0') && ((menuIdToOpen & 0x7f) == 2)) {
            //kernelAndFieldStatesSynced = 1;
            MissingCode();
            setVar(0x46, 0);
            setVar(4, 4);
            fieldMapNumber = 4;
            //pKernelGameState->field1464_0x2320 = 0;
            MissingCode();
            pKernelGameState->m1930_fieldVarsBackup[1] = 0;
            pKernelGameState->m231A_fieldID = 4;
        }
        if (menuReturnState0 == '\x02') {
            MissingCode(); //kernelAndFieldStatesSynced = 1;
            setVar(0x46, 2);
            setVar(4, pKernelGameState->m231A_fieldID & 0x3fff);
            if ((pKernelGameState->m231A_fieldID & 0x3fff) < 0x400) {
                MissingCode();//pKernelGameState->field1464_0x2320 = pKernelGameState->m1930_fieldVarsBackup[0x2a];
            }
        }

        MissingCode();

        menuIdToOpen = 0xff;
        initFontSystem();
        menuOpenCount = 0;
    }
}

void playMenuSoundEffect(uint param_1) {
    MissingCode();
}