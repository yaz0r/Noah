#pragma once

#include "kernel/graphics.h"
#include "kernel/font.h"

extern s32 menuIdToOpen;

struct sMenuContext_1D4 {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    std::array<sTag, 16> m70_OT;
};

struct sMenuContext_32C_5C {
    int m0;
    int m4;
    int m8;
    int mC;
    int m10;
    int m14;
    std::array<char, 0xC> m18_name;
    s8 m58;
    //size 0x5C
};

// First few blocks of a save, containing basic info + icons
struct sMenuContext_32C_B94 {
    u8 m2_IconDisplayFlag;
    s8 m3_blockNumber;
    std::array<u16, 16> m60_iconsPalette;
    std::array<std::array<u8, 128>, 3> m80_icons;
    //size 0x200
};

struct sMenuContext_32C {
    std::array<sMenuContext_32C_5C, 32> m0;
    std::array<sMenuContext_32C_B94, 32> mB94;
    std::array<POLY_FT4, 2> m4D94;
    std::array<s32, 2> m4F74;
    s32 m4F7C;
    s32 m4F80;
    s32 m4F84;
    std::array<u8, 2> m4F88;
    std::array<u8, 2> m4F8A;
    std::array<u8, 2> m4F8C;
    std::array<s8, 32> m4F8E;
    std::array<s8, 32> m4FAE;
    std::array<s8, 12> m4FCE;
    std::array<u8, 4> m4FE4;
    std::array<s8, 4> m4FE8;
    struct {
        u32 m0_end_io;
        u32 m4_error;
        u32 m8_timeout;
        u32 mC_newdev;
    } m4FEC_events;
};

struct sWeaponStats {
    void init(std::vector<u8>::const_iterator it) {
        m8 = READ_LE_S16(it + 0x8);
        mA = READ_LE_S8(it + 0xA);
        mB = READ_LE_S8(it + 0xB);
        mC = READ_LE_S8(it + 0xC);
    }
    s16 m8;
    s8 mA;
    s8 mB;
    s8 mC;
};


struct sAccessoryStats {
    void init(std::vector<u8>::const_iterator it) {
        m8 = READ_LE_S8(it + 0x8);
        m9_type = READ_LE_S8(it + 0x9);
        mC_flags = READ_LE_U16(it + 0xC);
    }
    s8 m8;
    s8 m9_type;
    u16 mC_flags;
};

struct sMenuContext_330 {
    std::vector<sWeaponStats> m0_weaponStats;
    std::vector<sAccessoryStats> m4_accessoryData;
    std::vector<u8> m14;
    std::vector<u8> m18;
    std::array<u16, 7> mB8_computedStats; // order is Attack, Hit, Defense, Evade, Ether, EtherDef, Agility
    // size 0xCC

    static std::vector<sWeaponStats> readWeaponStats(const std::vector<u8>& data) {
        std::vector<sWeaponStats> result;
        int numStats = data.size() / 0x10;
        //assert(numStats * 0x10 == data.size());
        result.resize(numStats);
        for (int i = 0; i < numStats; i++) {
            result[i].init(data.begin() + 0x10 * i);
        }
        return result;
    }

    static std::vector<sAccessoryStats> readAccesoryStats(const std::vector<u8>& data) {
        std::vector<sAccessoryStats> result;
        int numStats = data.size() / 0x10;
        //assert(numStats * 0x10 == data.size());
        result.resize(numStats);
        for (int i = 0; i < numStats; i++) {
            result[i].init(data.begin() + 0x10 * i);
        }
        return result;
    }
};

struct sMenuContext_33C {
    std::array<u8, 3> m0_isInfoCardEnabled = { 0,0,0 };
    u8 m3 = 0;
    u8 m4_drawCursor = 0;
    u8 m5_drawGold = 0;
    u8 m6_drawPlayTime = 0;
    u8 m7_drawCurrentSelectedCharacterInfoCard;
    u8 m8_drawCurrentSelectedCharacterStats;
    u8 m9_drawMainMenu = 0;
    u8 mA_354Enabled = 0;
    u8 mB = 0;
    std::array<s8, 8> mC = { 0,0,0,0,0,0,0,0 };
    std::array<s8, 6> m14;
    std::array<s8, 6> m1A;
    std::array<u8, 7> m20_menuBoxEnabled = { 0,0,0,0,0,0,0 };
    std::array<u8, 7> m27 = { 0,0,0,0,0,0,0 };
    u8 m2E_drawCheckingMemorycardText;
    u8 m2F_draw428;
    std::array<s8, 4> m30_partyMemberId;
    std::array<s8, 4> m34;
    std::array<s8, 8> m38;
    std::array<s8, 6> m40;
    u8 m4B_draw360;
    s8 m4E;
    s8 m52;
    u8 m53;
    std::array<s8, 6> m54;
    std::array<s8, 7> m5C = { 0,0,0,0, 0,0,0 };
    u8 m67;
    u8 m68;
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
    std::array<std::array<POLY_FT4, 2>, 16> m0_polys;
    std::array<std::array<POLY_FT4, 2>, 48> m500_polys;
    s32 m1400_0Length;
    s32 m1404_500Length;
    s8 m1408_0OddOrEven;
    s8 m1409_500OddOrEven;
};

struct sMenuContext_428 {
    u8 m144;
    u8 m145;
};

struct sMenuContext_4E0 {
    std::array<POLY_FT4, 2> m0_polys;
    std::array<SVECTOR, 4> m50_vertex;
    std::vector<u16> m78_imageData;
    RECT m70_rect;
    u8 m7C_colorMode;
    u8 m7D;
    u8 m7E_stringWidth;
    u8 m7F;

    // Original code do a bzero on this structure
    void clear() {
        m0_polys.fill(POLY_FT4::zero());
        m50_vertex.fill(SVECTOR::zero());
        m78_imageData.clear();
        m70_rect = RECT::zero();
        m7C_colorMode = 0;
        m7D = 0;
        m7E_stringWidth = 0;
        m7F = 0;
    }
    // size 0x80
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
    std::array<std::array<SVECTOR, 4>, 4> m510;
    std::array<SVECTOR, 4> m590;
    std::array<SVECTOR, 4> m5B0;
    std::array<SVECTOR, 4> m5D0;
    std::array<SVECTOR, 4> m5F0;
    std::array<SVECTOR, 4> m610;
    std::array<SVECTOR, 4> m630;
    std::array<SVECTOR, 4> m650;
    std::array<SVECTOR, 4> m670;
    std::array<SVECTOR, 4> m690;
    std::array<SVECTOR, 4> m6B0;
    std::array<std::array<SVECTOR, 4>, 2> m6D0;
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
    s8 m13;
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
    s8 m23_isAnimationDone;
};

struct sMenuContext {
    std::array<sMenuContextSub0, 3> m0;
    std::array<sMenuContext_1D4, 2> m6C_drawContexts;
    sMenuContext_1D4* m1D4_currentDrawContext = nullptr;
    SVECTOR m1D8_menuRotation = { 0,0,0,0 };
    VECTOR m1E0_menuTranslation = { 0,0,0,0 };
    MATRIX m1F0_menuMatrix;
    SVECTOR m218_menuRotation2 = { 0,0,0,0 };
    VECTOR m220_menuTranslation2 = { 0,0,0,0 };
    sFont m2DC_font;
    std::vector<u8> m2E0_textBundle;
    struct sSeqFile* m2E4_musicSequence;
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
    s8 m338_currentSelectedSubMenuEntry = 2;
    s8 m339_previousSelectedSubMenuEntry;
    s8 m33A_numMenuEntries;
    s8 m33B = 0;
    sMenuContext_33C* m33C;
    struct sMenuContext_340_goldOwned* m340_gold;
    sMenuContext_344_playTime* m344_playTime;
    sMenuContext_348* m348_cursor;
    struct sMenuContext_34C* m34C;
    sMenuContext_350* m350_mainMenu;
    sMenuContext_354* m354;
    struct sMenuContext_358* m358;
    struct sMenuContext_35C* m35C;
    struct sMenuContext_360* m360;
    std::array<sMenuContext_364*, 7> m364_menuBoxes;
    std::array<sMenuContext_380*, 7> m380;
    std::array<sMenuContext_39C*, 3> m39C;
    std::array<struct sMenuContext_3A8*, 0x20> m3A8_memoryCardTiles;
    sMenuContext_428* m428;
    struct sMenuContext_440* m440;
    std::array<sMenuContextMenuTile, 4> m46C_menuBorders;
    s32 m4CC;
    s32 m4D0;
    s32 m4D4_fadeColor;
    std::array<sMenuContext_4E0, 50> m4E0;
    s8 m4D8;
    s8 m4D9;
    s8 m4DC_currentlySelectedCharacter;
    std::array<sMenuContext_4E0*, 4> m1DE0;
    u8 m1E94;
    u8 m1E95;
};

extern sMenuContext *gMenuContext;

extern std::array<s32, 11> cursorPosXTable;
extern std::array<s32, 11> cursorPosYTable;

extern std::array<int, 19> infocardNameU;
extern std::array<int, 19> infocardNameV;


void enterMenu(void);

void resetCursorState(void);
void updateCursorPolysLocation(int param_1, char param_2);
void drawMenuContext354();
void drawMenu2_sub(void);
void drawMenuMemoryCard();
void drawMenu_34C(void);
void drawMenu_428();
void loadAndOpenMenu(void);
void menuDraw();
void processLoadSaveMenuSub1(void);
void processLoadSaveMenuSub3(uint param_1);
void processLoadSaveMenuSub8(uint param_1);
void fillStringDisplayBuffer(uint param_1);
int setupStringInPolyFT4(struct sFont& font, int character, std::array<POLY_FT4, 2>* polyArray, int oddOrEven, short x, short y, ushort scale);
void setupMenuPolyFT4(POLY_FT4* param_1, short x, short y, u8 u, u8 v, short width, short height);
void iniMenuContext364And380(byte param_1, short param_2, short param_3, ushort param_4, ushort param_5, char param_6, byte param_7, s32 param_8, byte param_9);
void playMenuSoundEffect(uint param_1);
u8 renderString(std::vector<u8>::const_iterator buffer, std::vector<u16>& param_2, ushort param_3, byte param_4);
void processLoadSaveMenuSub2(bool show, bool hideGold);
void initMemoryCardTransparentPoly(POLY_FT4* param_1);
void initMemoryCardPolyVerts(std::array<SVECTOR, 4>& param_1, short param_2, short param_3, short param_4, short param_5);
void setupMenuContext4E0(sMenuContext_4E0* param_1, const std::span<std::array<u8, 2>>::iterator& param_2, int startY, int count);
void transformAndAddPrim(int param_1, std::array<SVECTOR, 4>* param_2, std::array<POLY_FT4, 2>* param_3, int param_4);
void initMenuTiles1Sub(POLY_FT4* param_1);
void computeMenuBorder(sFont& font, int param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7, int* param_8);