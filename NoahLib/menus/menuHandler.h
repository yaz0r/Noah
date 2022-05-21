#pragma once

#include "kernel/graphics.h"

extern s32 menuIdToOpen;
extern s32 menuOpenCount;

struct sMenuContext_1D4 {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    std::array<sTag, 16> m70_OT;
};

struct sMenuContext_32C {
    u8 m4FE6;
};

struct sMenuContext_330 {
};

struct sMenuContext_33C {
    std::array<u8, 3> m0_isInfoCardEnabled = { 0,0,0 };
    u8 m3 = 0;
    u8 m4_drawCursor = 0;
    u8 m5_drawGold = 0;
    u8 m6_drawPlayTime = 0;
    u8 m9_drawMainMenu = 0;
    u8 mA_354Enabled = 0;
    std::array<u8, 8> mC = { 0,0,0,0,0,0,0,0 };
    std::array<u8, 7> m20_menuBoxEnabled = { 0,0,0,0,0,0,0 };
    std::array<u8, 7> m27 = { 0,0,0,0,0,0,0 };
    std::array<s8, 3> m30 = { 0,0,0 };
    std::array<s8, 3> m60 = { 0,0,0 };
};

struct sMenuContext_348 {
    std::array<POLY_FT4, 2> m0;
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

struct sMenuContext_344_playTime {
    std::array<std::array<POLY_FT4, 2>, 7> m0_playTimePolys;
    std::array<std::array<POLY_FT4, 2>, 4> m230;
    s32 m370_oddOrEven;
};

struct sMenuContext_39C {
    std::array<POLY_FT4, 2> m0_portraitPoly;
    std::array<POLY_FT4, 2> m50_NamePoly;
    std::array<std::array<POLY_FT4, 2>, 20> mA0_FixedStrings;
    std::array<std::array<POLY_FT4, 2>, 3> m910_LevelPolys;
    std::array<std::array<POLY_FT4, 2>, 3> mA00_Level2Polys;
    std::array<std::array<POLY_FT4, 2>, 3> mAF0_PolyHP;
    std::array<std::array<POLY_FT4, 2>, 3> mBE0_PolyMaxHP;
    std::array<std::array<POLY_FT4, 2>, 2> mCD0_PolyMP;
    std::array<std::array<POLY_FT4, 2>, 2> mD70_PolyMaxMP;
    std::array<std::array<POLY_FT4, 2>, 7> mE10_polys;
    std::array<std::array<POLY_FT4, 2>, 7> m1040_polys;
    s8 m1270_oddOrEven;
    s8 m1271_LevelLength;
    s8 m1272_Level2Length;
    s8 m1273_HPLength;
    s8 m1274_MaxHPLength;
    s8 m1275_MPLength;
    s8 m1276_MaxMPLength;
    s8 m1277_E10Length;
    s8 m1278_E10Length2;
    s8 m1279_FixedStringLength;
};

struct sMenuContext_364 {
    std::array<std::array<POLY_FT4, 2>, 4> m0;
    std::array<std::array<POLY_FT4, 2>, 2> m140;
    std::array<std::array<POLY_FT4, 2>, 2> m1E0;
    std::array<std::array<POLY_FT4, 2>, 2> m280;
    std::array<std::array<POLY_FT4, 2>, 2> m320;
    std::array<POLY_FT4, 2> m3C0;
    std::array<std::array<POLY_FT4, 2>, 2> m410;
    std::array<POLY_G4, 2> m4B0;
    std::array<DR_MODE, 2> m4F8;
    std::array<std::array<SFP_VEC4, 4>, 4> m510;
    std::array<SFP_VEC4, 4> m590;
    std::array<SFP_VEC4, 4> m5B0;
    std::array<SFP_VEC4, 4> m5D0;
    std::array<SFP_VEC4, 4> m5F0;
    std::array<SFP_VEC4, 4> m610;
    std::array<SFP_VEC4, 4> m630;
    std::array<SFP_VEC4, 4> m650;
    std::array<SFP_VEC4, 4> m670;
    std::array<SFP_VEC4, 4> m690;
    std::array<SFP_VEC4, 4> m6B0;
    std::array<std::array<SFP_VEC4, 4>, 2> m6D0;
    s32 m710;
    s32 m714_transformDisabled;
    s32 m718;
    s8 m71C_oddOrEven;
    s8 m71D;
};

struct sMenuContext_380 {
    s16 m0;
    s16 m2;
    s16 m4;
    s16 m6;
    s16 m8;
    s16 mA;
    s16 mC;
    s8 m10;
    s8 m11_draw380;
    s8 m12;
};

struct sMenuContextMenuTile {
    s32 m0;
    s32 m4_tp;
    s32 m8_clutX;
    s32 mC_clutY;
    s32 m10_tpageX;
    s32 m14_tpageY;
};

struct sMenuContextSub0 {
    s32 m0;
    s32 m4;
    s32 m8;
    s32 mC;
    s32 m10_XStep;
    s32 m14_YStep;
    s32 m18_XOffset;
    s32 m1C_YOffset;
    s8 m20_XReversed;
    s8 m21_YReversed;
    s8 m22_numSteps;
    s8 m23;
};

struct sMenuContext {
    std::array<sMenuContextSub0, 3> m0;
    std::array<sMenuContext_1D4, 2> m6C_drawContexts;
    sMenuContext_1D4* m1D4_currentDrawContext = nullptr;
    SFP_VEC4 m1D8_menuRotation = { 0,0,0,0 };
    FP_VEC4 m1E0_menuTranslation = { 0,0,0,0 };
    SFP_VEC4 m218_menuRotation2 = { 0,0,0,0 };
    FP_VEC4 m220_menuTranslation2 = { 0,0,0,0 };
    std::vector<u8> m2DC_font;
    std::vector<u8> m2E0_textBundle;
    u32 m2E8;
    u8 m325_menuButton;
    u8 m326;
    u8 m327;
    u8 m329;
    u8 m32A;
    u32 m2D8_frameCounter;
    std::array<s32, 7> m302_playTimeString;
    u32 m308_oddOrEven;
    std::array<s8, 16> m30C;
    std::array<u8, 9> m31C_stringDisplayBuffer;
    s8 m32B = 0;
    sMenuContext_32C* m32C_memoryCardContext;
    sMenuContext_330* m330;
    s8 m334;
    s8 m335;
    s8 m336_menuSelectedEntry = 0;
    s8 m337_previousMenuSelectedEntry = 0;
    s8 m338_currentSaveState = 2;
    s8 m339;
    s8 m33B = 0;
    sMenuContext_33C* m33C;
    struct sMenuContext_340_goldOwned* m340_gold;
    sMenuContext_344_playTime* m344_playTime;
    sMenuContext_348* m348_cursor;
    sMenuContext_350* m350_mainMenu;
    sMenuContext_354* m354;
    std::array<sMenuContext_364*, 7> m364_menuBoxes;
    std::array<sMenuContext_380*, 7> m380;
    std::array<sMenuContext_39C*, 3> m39C;
    sMenuContext_428* m428;
    std::array<sMenuContextMenuTile, 4> m46C_menuBorders;
    std::array<sMenuContext_4E0, 50> m4E0;
    u8 m1E94;
    u8 m1E95;
};

extern sMenuContext *gMenuContext;

void loadAndOpenMenu(void);
void setBootMode(int);

void fillStringDisplayBuffer(uint param_1);
int setupStringInPolyFT4(std::vector<u8>& fontData, int character, std::array<POLY_FT4, 2>* polyArray, int oddOrEven, short x, short y, ushort scale);
void iniMenuContext364And380(byte param_1, short param_2, short param_3, ushort param_4, ushort param_5, char param_6, byte param_7, s32 param_8, byte param_9);
void playMenuSoundEffect(uint param_1);