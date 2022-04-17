#include "noahLib.h"
#include "menuHandler.h"
#include "field/field.h"
#include "field/dialogWindows.h"
#include "kernel/filesystem.h"
#include "kernel/TIM.h"
#include "kernel/decompress.h"

s32 menuIdToOpen = 0xFF;
s32 menuOpenCount;

u8 useDebugMenuList = 1;
u8 menuToEnter = 0;

std::vector<u8> menuSharedResources;
std::vector<u8> menuOverlay;

struct sMenuContext_1D4 {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    std::array<sTag, 16> m70_OT;
};

struct sMenuContext_32C {
};

struct sMenuContext_330 {
};

struct sMenuContext_33C {
    u8 m3;
    u8 m4_drawCursor;
    u8 m9_drawMainMenu;
};

struct sMenuContext_348 {
    std::array<POLY_FT4,2> m0;
    std::array<POLY_G4, 2> m50_polyG4;
    std::array<POLY_F4, 2> m98_poly;
    std::array<LINE_F3, 2> mC8_lines;
    std::array<LINE_F3, 2> mF8_lines;
    std::array<DR_MODE, 2> m128;
    std::array<DR_MODE, 2> m140;
    u8 m158_oddOrEven = 0;
    u8 m159_oddOrEven = 0;
    u8 m15B = 0;
};

struct sMenuContext_350 {
    std::array<std::array<POLY_FT4, 2>, 28> m0_polys;
    std::array<std::array<POLY_FT4, 2>, 28> m8C0_polys;
    RECT m1180;
    u32 m1188_numEntries2;
    u32 m118C_numEntries;
    u8 m1190;
    u8 m1191;
    u8 m1192;
    u8 m1193;
};

struct sMenuContext_354 {
};

struct sMenuContext_428 {
};

struct sMenuContext_4E0 {
    std::array<POLY_FT4, 2> m0_polys;
    std::vector<u16> m78_imageData;
    RECT m70_rect;
    u8 m7C_colorMode;
    u8 m7E_stringWidth;
    u8 m7F;
};

struct sMenuContext {
    std::array<sMenuContext_1D4, 2> m6C_drawContexts;
    sMenuContext_1D4* m1D4_currentDrawContext = nullptr;
    SFP_VEC4 m1D8_menuRotation = { 0,0,0,0};
    FP_VEC4 m1E0_menuTranslation = { 0,0,0,0 };
    SFP_VEC4 m218_menuRotation2 = { 0,0,0,0 };
    FP_VEC4 m220_menuTranslation2 = { 0,0,0,0 };
    std::vector<u8> m2DC_font;
    std::vector<u8> m2E0_textBundle;
    u32 m2E8;
    u8 m325_menuButton;
    u8 m327;
    u8 m329;
    u8 m32A;
    u32 m2D8_frameCounter;
    u32 m308_oddOrEven;
    sMenuContext_32C* m32C;
    sMenuContext_330* m330;
    s8 m336_menuSelectedEntry = 0;
    s8 m337_previousMenuSelectedEntry = 0;
    sMenuContext_33C* m33C;
    sMenuContext_348* m348_cursor;
    sMenuContext_350* m350_mainMenu;
    sMenuContext_354* m354;
    sMenuContext_428* m428;
    std::array<sMenuContext_4E0,50> m4E0;
    u8 m1E94;
    u8 m1E95;
}*gMenuContext;

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

void setBootMode(int) {
    MissingCode();
}

void initMenuContext33C(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m33C;
    }
    else {
        gMenuContext->m33C = new sMenuContext_33C;
        memset(gMenuContext->m33C, 0, sizeof(sMenuContext_33C));
    }
}

void initMenuContext350MainMenu(char param_1)
{
    if (!param_1) {
        delete gMenuContext->m350_mainMenu;
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
        delete gMenuContext->m32C;
    }
    else {
        gMenuContext->m32C = new sMenuContext_32C;
        memset(gMenuContext->m32C, 0, sizeof(sMenuContext_32C));
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
            assert(0);
            /*initMenuContext32C(1);
            initMenuContext340(1);
            initMenuContext344(1);
            initMenuContext39C(1);
            */
            return;
        }
        if (menuToEnter != 6) {
            return;
        }
    }
    initMenuContext32C(1);
}

void loadMenuImageBundle(std::vector<u8>& imageBundle) {
    s32 numImages = READ_LE_U32(imageBundle.begin());

    while (numImages != -1) {
        std::vector<u8>::iterator imageData = imageBundle.begin() + READ_LE_U32(imageBundle.begin() + 4 * numImages);

        TIM_IMAGE tempTim;
        OpenTIM(imageData);
        ReadTIM(&tempTim);
        if (tempTim.caddr != nullptr) {
            DrawSync(0);
            LoadImage(tempTim.crect, (u8*)tempTim.caddr);
        }
        DrawSync(0);
        LoadImage(tempTim.prect, (u8*)tempTim.paddr);

        numImages--;
    }
}

void loadMenuSharedResources() {

    std::vector<std::vector<u8>::iterator> relocatedPtr = doPointerRelocation(menuSharedResources);

    MissingCode();

    loadMenuImageBundle(mallocAndDecompress(relocatedPtr[1]));

    gMenuContext->m2DC_font = mallocAndDecompress(relocatedPtr[2]);
    gMenuContext->m2E0_textBundle = mallocAndDecompress(relocatedPtr[3]);

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

    delete buffer;
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
    //DAT_8005a0c2 = param_4 & 1;
    sDialogWindow18_temp.m28_perLineBuffer.push_back(sDialogWindow18PerLineBufferEntry());
    //DAT_8005a0c0 = 0;
    sDialogWindow18_temp.m1C = buffer;
    sDialogWindow18_temp.m2C_inRamDialogTextImage = param_2;
    updateDialogTextImage(&sDialogWindow18_temp);
    param_2 = sDialogWindow18_temp.m2C_inRamDialogTextImage;
    return sDialogWindow18_temp.m28_perLineBuffer[0].m58_widthDiv4 << 2;
}

void SetShadeTex(POLY_FT4* p, int tge)
{
    if (tge == 0) {
        p->code &= ~1;
    }
    else {
        p->code |= 1;
    }
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

void setupMenuContext4E0(sMenuContext_4E0* param_1, std::vector<std::array<u8,2>>::iterator& param_2, int startY, int count) {
    for (int i = 0; i < count; i +=2) {
        sMenuContext_4E0* pEntry = &param_1[i];
        sMenuContext_4E0* pEntry2 = &param_1[i+1];

        pEntry->m7E_stringWidth = renderString(getDialogParamPointer(gMenuContext->m2E0_textBundle, (*param_2)[0]), gMenuContext->m4E0[0].m78_imageData, 0x18, 0);
        pEntry2->m7E_stringWidth = renderString(getDialogParamPointer(gMenuContext->m2E0_textBundle, (*param_2)[1]), gMenuContext->m4E0[0].m78_imageData, 0x18, 1);

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

void SetPolyG4(POLY_G4* p)
{
    p->m3_size = 8;
    p->code = 0x38;
    return;
}

void setupPolyG4(POLY_G4* param_1, u8 param_2, u8 param_3, u8 param_4)
{
    SetPolyG4(param_1);
    param_1->r0 = param_2;
    param_1->g0 = param_3;
    param_1->b0 = param_4;
    param_1->r1 = param_2;
    param_1->g1 = param_3;
    param_1->b1 = param_4;
    param_1->r2 = 0;
    param_1->g2 = 0;
    param_1->b2 = 0;
    param_1->r3 = 0;
    param_1->g3 = 0;
    param_1->b3 = 0;
    return;
}

void SetLineF3(LINE_F3* p)
{
    p->m3_size = 5;
    p->code = 0x48;
    p->pad = 0x55555555;
    return;
}

void SetPolyF4(POLY_F4* p)
{
    p->m3_size = 5;
    p->code = 0x28;
    return;
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
    MissingCode();

    switch (menuToEnter) {
    case 2:
        gMenuContext->m348_cursor->m15B = 0x4C;
        break;
    default:
        assert(0);
    }
    MissingCode();
}

u8 menuReturnState0 = 0;

int MoveImage(RECT* rect, int x, int y) {
    MissingCode();
    return 0;
}

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

void drawMenu2(void)
{
    MissingCode();
    //drawMenu_380();
    //drawMenu2_sub();
    //drawMenu_428();
    drawMenu_348_cursor();
    //drawMenu_34C();
    //drawMenuMemoryCard();
    drawMainMenu();
    //drawMenuContext354();
    //drawMenuBoxes();
    MissingCode();
}

void drawMenuSpecificElements(void)
{
    if (gMenuContext->m327 != 0) {
        switch (menuToEnter) {
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
        if ((newPadButtonForField & 0x2000) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 0;
        }
        if ((newPadButtonForField & 0x4000) != 0) {
            newValue = 1;
        }
        if ((newPadButtonForField & 0x8000) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 2;
        }
        if ((newPadButtonForField & 0x1000) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 3;
        }
        if ((newPadButtonForDialogs & 0x20) != 0) {
            FUN_Menu2__801c8574(2);
            newValue = 4;
        }
        if ((newPadButtonForDialogs & 0x40) != 0) {
            FUN_Menu2__801c8574(3);
            newValue = 5;
        }
        if ((newPadButtonForDialogs & 0x80) != 0) {
            newValue = 6;
        }
        if ((newPadButtonForDialogs & 0x10) != 0) {
            newValue = 7;
        }
        if ((newPadButtonForField & 4) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 10;
        }
        if ((newPadButtonForField & 8) != 0) {
            FUN_Menu2__801c8574(1);
            newValue = 9;
        }
    } while (0);
    MissingCode();
    //while ((newPadButtonForDialogs & 0x100) == 0);

    gMenuContext->m325_menuButton = newValue;
}

void menuDraw() {
    MissingCode();

    updateMenuInput();

    MissingCode();

    ClearOTagR(&gMenuContext->m1D4_currentDrawContext->m70_OT[0], 0x10);

    MissingCode();

    drawMenuSpecificElements();

    DrawSync(0);
    VSync(0);
    PutDrawEnv(&gMenuContext->m1D4_currentDrawContext->m0_DrawEnv);
    PutDispEnv(&gMenuContext->m1D4_currentDrawContext->m5C_DispEnv);
    MoveImage(&gMenuContext->m350_mainMenu->m1180, 0, (gMenuContext->m308_oddOrEven == 0) * 0xe0);
    DrawOTag(&gMenuContext->m1D4_currentDrawContext->m70_OT[0xf]);

    MissingCode();

    noahFrame_end();
    noahFrame_start();
}

std::vector<std::array<u32, 2>> saveGameMainMenuConfig = { {
    {{0x109, 0x125}},
    {{0x10A, 0x124}},
    {{0x10B, 0x123}},
    {{0x110, 0x142}},
} };

int setupStringInPolyFT4(std::vector<u8>& fontData, int character, std::array<POLY_FT4, 2>* polyArray, int oddOrEven, short x, short y, ushort scale) {
    std::vector<u8>::iterator characterData = fontData.begin() + READ_LE_U16(fontData.begin() + character * 2 + 4);
    if (READ_LE_U16(characterData)) {
        int offsetToCharacterData = 4;
        for(int i=0; i< READ_LE_U16(characterData); i++)
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

void j_setupMenuContext4E0(uint param_1, sMenuContext_4E0* param_2, std::vector<std::array<u8, 2>>::iterator& param_3)
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
            assert(0);
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
            //FUN_Menu2__801e8044(8, &gMenuContext->m33C->field_0xc);
            assert(0);
            return;
        }
    } while (true);
}

void menu2_loadGame_entryPoint() {
    initMenuContext();
    setupMainMenu();
    gMenuContext->m327 = 1;
    gMenuContext->m32A = 1;
    switch (menuToEnter)
    {
    case 2:
        menuReturnState0 = 0;
        loadSaveGameMenuExecute();
/*        gMenuContext->m33C->m9_drawMainMenu = 0;
        gMenuContext->m33C->m4_drawCursor = 0;
        gMenuContext->m33C->m3 = 0;
        */
        assert(0);
        break;
    default:
        assert(0);
        break;
    }
    assert(0);
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
    case 2:
    case 6:
        menu2_loadGame_entryPoint();
        setBootMode(1);
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
    initModel3(2, 0);

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

        if(0)
        {
            FILE* fHandle = fopen((std::string("menuSharedResources") + std::string(".bin")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&menuSharedResources[0], 1, menuSharedResources.size(), fHandle);
                fclose(fHandle);
            }
        }

        MissingCode();

        useDebugMenuList = 0;
        menuToEnter = menuIdToOpen & 0x7f;

        MissingCode();

        ClearCacheAfterOverlayLoad();
        enterMenu();
        ClearCacheAfterOverlayLoad();
    }
}