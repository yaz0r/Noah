#include "noahLib.h"
#include "worldmap.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "kernel/memory.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "field/field.h"
#include "worldmapTask.h"
#include "worldmapTaskFader.h"
#include "worldmapMinimap.h"
#include "kernel/gameMode.h"

SVECTOR worldmapRotation = { 0,0,0,0 };
VECTOR worldmapRadarPosition = { 0,0,0,0 };
s32 worldmapGeometryOffsetY = 0;

s8 isWorldMapOverlayLoaded = 0;
s32 currentWorldmapMode = 0;
s32 worldMapGearMode = 0;
s32 exitWorldMapMode;
s32 currentBattleMusicId;

std::array<u8, 3> worldMapFarColor = { {0xC0, 0xD0, 0xF0} };

std::array<sWorldmapRenderingStruct, 2> worldmapRenderingStructs;

struct sWorldmapModes {
    void (*m0_init)();
    void (*m4_update)();
    void (*m8_free)();
};

std::array<std::vector<u8>, 3 > worldmapPartySprites;
std::array<std::vector<u8>, 3 > worldmapPartyGearSprites;
s32 worldmapNumFilesPending = 0;

std::array<sLoadingBatchCommands, 16> worldmapLoadingCommands;

s32 isWorldmapModeFlagSet;
s32 worldmapOddOrEven;
s32 continueWorldmapLoop;

sWorldmapRenderingStruct* pCurrentWorldmapRenderingStruct = nullptr;

void worldmapMode0_init(void) {
    for (int i = 0; i < 3; i++) {
        if (gameState.m1D34_currentParty[i] == 0xFF) {
            worldmapPartySprites[i].clear();
            worldmapPartyGearSprites[i].clear();
        }
        else {
            worldmapPartySprites[i].resize(getFileSizeAligned(gameState.m1D34_currentParty[i] + 2));
            if (gameState.m294[i].m78_partyData_gearNum == -1) {
                worldmapPartyGearSprites[i].clear();
            }
            else {
                worldmapPartyGearSprites[i].resize(getFileSizeAligned(gameState.m294[i].m78_partyData_gearNum + 0x19));
            }
        }
    }

    worldmapNumFilesPending = 0;

    std::array<sLoadingBatchCommands, 16>::iterator it = worldmapLoadingCommands.begin();
    for (int i = 0; i < 3; i++) {
        if (gameState.m1D34_currentParty[i] != 0xFF) {
            it->m0_fileIndex = gameState.m1D34_currentParty[i] + 2;
            it->m4_loadPtr = &worldmapPartySprites[i];
            it++;
            worldmapNumFilesPending++;
            if (gameState.m294[i].m78_partyData_gearNum != -1) {
                it->m0_fileIndex = gameState.m294[i].m78_partyData_gearNum + 0x13;
                it->m4_loadPtr = &worldmapPartyGearSprites[i];
                it++;
                worldmapNumFilesPending++;
            }
        }
    }

    it->m0_fileIndex = 0;
    it->m4_loadPtr = nullptr;

    batchStartLoadingFiles(&worldmapLoadingCommands[0], 0);
}

void initWorldmapGraphics(void)
{
    long a;

    ResetGraph(1);
    worldmapMinimapScale = 0x100;
    SetGeomScreen(0x100);
    SetDefDrawEnv(&worldmapRenderingStructs[0].m0_DrawEnv, 0, 0, 0x140, 0xd8);
    SetDefDrawEnv(&worldmapRenderingStructs[1].m0_DrawEnv, 0, 0xd8, 0x140, 0xd8);
    SetDefDispEnv(&worldmapRenderingStructs[0].m5C_DispEnv, 0, 0xd8, 0x140, 0xd8);
    SetDefDispEnv(&worldmapRenderingStructs[1].m5C_DispEnv, 0, 0, 0x140, 0xd8);
    worldmapRenderingStructs[1].m0_DrawEnv.isbg = 1;
    worldmapRenderingStructs[0].m0_DrawEnv.isbg = 1;
    worldmapRenderingStructs[1].m0_DrawEnv.dtd = 1;
    worldmapRenderingStructs[0].m0_DrawEnv.dtd = 1;
    if (exitWorldMapMode == 2) {
        worldmapRenderingStructs[0].m0_DrawEnv.b0 = 0;
        worldmapRenderingStructs[1].m0_DrawEnv.b0 = 0;
    }
    else {
        worldmapRenderingStructs[0].m0_DrawEnv.b0 = 0x70;
        worldmapRenderingStructs[1].m0_DrawEnv.b0 = 0x70;
    }
    worldmapRenderingStructs[1].m0_DrawEnv.g0 = 0;
    worldmapRenderingStructs[1].m0_DrawEnv.r0 = 0;
    worldmapRenderingStructs[0].m0_DrawEnv.g0 = 0;
    worldmapRenderingStructs[0].m0_DrawEnv.r0 = 0;
    worldmapRenderingStructs[1].m5C_DispEnv.screen.y = 10;
    worldmapRenderingStructs[0].m5C_DispEnv.screen.y = 10;
    worldmapRenderingStructs[1].m5C_DispEnv.screen.w = 0x100;
    worldmapRenderingStructs[0].m5C_DispEnv.screen.w = 0x100;
    worldmapRenderingStructs[1].m5C_DispEnv.screen.x = 0;
    worldmapRenderingStructs[0].m5C_DispEnv.screen.x = 0;
    worldmapRenderingStructs[1].m5C_DispEnv.screen.h = 0xd8;
    worldmapRenderingStructs[0].m5C_DispEnv.screen.h = 0xd8;
    setFogNearColor(0x80, 0x80, 0x80);
    SetBackColor(0x80, 0x80, 0x80);
    SetFarColor(worldMapFarColor[0], worldMapFarColor[1], worldMapFarColor[2]);
    a = 0x800;
    if (exitWorldMapMode == 2) {
        a = 0xb00;
    }
    SetFogNearFar(a, 0xe80, worldmapMinimapScale);
    return;
}

void worldmapFadeIn(int param_1, char param_2, char param_3, int param_4) {
    MissingCode();
}

sWorldmapState* gWorldmapState = nullptr;

void initWorldmapState(void) {
    for (int i = 0; i < 0x40; i++) {
        gWorldmapState->m0[i].m4C = nullptr;
        gWorldmapState->m0[i].m18_init = nullptr;
        gWorldmapState->m0[i].m1C_update = nullptr;
    }
}

void allocAndInitWorldmapState(void)
{
    gWorldmapState = new sWorldmapState;
    initWorldmapState();
    return;
}

std::vector<u8> worldmapFile1Buffer;
std::vector<u8> worldmapFile2Buffer;
std::vector<u8> worldmapFile3Buffer;

s32 initWorldMapVar0;
s32 worldmapFile1;
s32 worldmapFile2;
s32 worldmapFile3;
s32 worldmapFile4;
s32 worldmapFile5;
s32 worldmapFile6;
s32 worldmapFile7;
s32 worldmapFile9;
s32 worldmapFile10;

std::vector<u8>::iterator worldmapFile1Buffer_8;
std::vector<u8>::iterator worldmapFile1Buffer_C;
std::vector<u8>::iterator worldmapFile1Buffer_14;
std::vector<u8>::iterator worldmapFile1Buffer_18;
std::vector<u8>::iterator worldmapFile1Buffer_1C;
std::vector<u8>::iterator worldmapFile1Buffer_20;
std::vector<u8>::iterator worldmapFile1Buffer_24;

std::vector<u8>::iterator worldmapFile1Buffer_4_0;
std::vector<u8>::iterator worldmapFile1Buffer_4_1;

std::vector<u8>::iterator pWorldmapModelsConfig;

std::array<std::vector<u8>::iterator, 16> worldmapFile1Buffer_2C_data;

void finalizeWorldMapFileLoading() {
    worldmapFile1Buffer = mallocAndDecompress(worldmapFile1Buffer.begin());

    worldmapFile1Buffer_C = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0xC);
    worldmapFile1Buffer_8 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x8);
    worldmapFile1Buffer_14 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x14);
    pWorldmapModelsConfig = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x10);
    worldmapFile1Buffer_1C = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x1C);
    worldmapFile1Buffer_18 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x18);
    worldmapFile1Buffer_24 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x24);
    worldmapFile1Buffer_20 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x20);

    for (int i = 0; i < 0x10; i++) {
        worldmapFile1Buffer_2C_data[i] = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x2C + 4 * i);
    }

    std::vector<u8>::iterator worldmapFile1Buffer_4 = worldmapFile1Buffer.begin() + READ_LE_U32(worldmapFile1Buffer.begin() + 0x4);

    worldmapFile1Buffer_4_0 = worldmapFile1Buffer_4 + READ_LE_U32(worldmapFile1Buffer_4);

    MissingCode();
}

void worldmapMode8_init(void)
{
    size_t sVar1;

    worldmapFile1Buffer.resize(getFileSizeAligned(worldmapFile1));
    worldmapFile2Buffer.resize(getFileSizeAligned(worldmapFile2));
    worldmapFile3Buffer.resize(getFileSizeAligned(worldmapFile3));
    worldmapLoadingCommands[3].m0_fileIndex = 0;
    worldmapLoadingCommands[3].m4_loadPtr = nullptr;
    worldmapLoadingCommands[0].m0_fileIndex = (ushort)worldmapFile1;
    worldmapLoadingCommands[1].m0_fileIndex = (ushort)worldmapFile2;
    worldmapLoadingCommands[2].m0_fileIndex = (ushort)worldmapFile3;
    worldmapLoadingCommands[1].m4_loadPtr = &worldmapFile2Buffer;
    worldmapLoadingCommands[2].m4_loadPtr = &worldmapFile3Buffer;
    worldmapLoadingCommands[0].m4_loadPtr = &worldmapFile1Buffer;
    batchStartLoadingFiles(&worldmapLoadingCommands[0], 0);
    return;
}

void createWorlmapPolyFT3Array() {
    worldmapRenderingStructs[0].m74_polyFT3.resize(2048);
    worldmapRenderingStructs[1].m74_polyFT3.resize(2048);

    for (int i = 0; i < 2048; i++) {
        SetPolyFT3(&worldmapRenderingStructs[0].m74_polyFT3[i]);
        worldmapRenderingStructs[0].m74_polyFT3[i].r0 = 0x80;
        worldmapRenderingStructs[0].m74_polyFT3[i].g0 = 0x80;
        worldmapRenderingStructs[0].m74_polyFT3[i].b0 = 0x80;
    }

    for (int i = 0; i < 2048; i++) {
        worldmapRenderingStructs[1].m74_polyFT3[i] = worldmapRenderingStructs[0].m74_polyFT3[i];
    }
}

std::array<std::vector<POLY_FT4>, 2> worldmapFT4_1;

void createWorlmapPolyFT4Array1(void) {
    worldmapFT4_1[0].resize(0x100);
    worldmapFT4_1[1].resize(0x100);

    for (int i = 0; i < 0x100; i++) {
        SetPolyFT4(&worldmapFT4_1[0][i]);
        worldmapFT4_1[0][i].tpage = GetTPage(1, 1, 0x340, 0x100);
        worldmapFT4_1[0][i].clut = GetClut(0x100, 0x1FF);
        SetSemiTrans(&worldmapFT4_1[0][i], 1);
    }

    for (int i = 0; i < 0x100; i++) {
        worldmapFT4_1[1][i] = worldmapFT4_1[0][i];
    }
}

std::array<std::vector<POLY_FT4>, 2> worldmapFT4_2;

void createWorlmapPolyFT4Array2(void) {
    worldmapFT4_2[0].resize(0x120);
    worldmapFT4_2[1].resize(0x120);

    for (int i = 0; i < 0x120; i++) {
        SetPolyFT4(&worldmapFT4_2[0][i]);
        worldmapFT4_2[0][i].r0 = 0x26;
        worldmapFT4_2[0][i].g0 = 0x26;
        worldmapFT4_2[0][i].b0 = 0x26;
        worldmapFT4_2[0][i].tpage = GetTPage(0, 1, 0x3C0, 0x100);
        worldmapFT4_2[0][i].clut = GetClut(0x130, 0x1FE);
        SetSemiTrans(&worldmapFT4_2[0][i], 1);
    }

    for (int i = 0; i < 0x120; i++) {
        worldmapFT4_2[1][i] = worldmapFT4_2[0][i];
    }
}

std::array<std::vector<POLY_FT4>, 2> worldmapFT4_3;

void createWorlmapPolyFT4Array3(void) {
    worldmapFT4_3[0].resize(0x200);
    worldmapFT4_3[1].resize(0x200);

    for (int i = 0; i < 0x200; i++) {
        SetPolyFT4(&worldmapFT4_3[0][i]);
        worldmapFT4_3[0][i].r0 = 0x80;
        worldmapFT4_3[0][i].g0 = 0x80;
        worldmapFT4_3[0][i].b0 = 0x80;
        worldmapFT4_3[0][i].tpage = GetTPage(0, 0, 0x380, 0x100);
        worldmapFT4_3[0][i].clut = GetClut(0xF0, 0x1FF);


        worldmapFT4_3[0][i].u0 = 0;
        worldmapFT4_3[0][i].v0 = 0x40;
        worldmapFT4_3[0][i].u1 = 0x1f;
        worldmapFT4_3[0][i].v1 = 0x40;
        worldmapFT4_3[0][i].u2 = 0;
        worldmapFT4_3[0][i].v2 = 0x6f;
        worldmapFT4_3[0][i].u3 = 0x1f;
        worldmapFT4_3[0][i].v3 = 0x6f;
    }

    for (int i = 0; i < 0x200; i++) {
        worldmapFT4_3[1][i] = worldmapFT4_3[0][i];
    }
}

void worldmapMode0_update(void) {
    initWorldmapGraphics();

    RECT local_18;
    local_18.w = 0x140;
    local_18.x = 0;
    local_18.y = 0;
    local_18.h = 0xd8;
    MoveImage(&local_18, 0x2c0, 0x100);
    worldmapFadeIn(0x40, 0, 4, 2);
    waitReadCompletion(0);
    worldmapMode8_init();
    do {
    } while (2 < isCDBusy());
    finalizeWorldMapFileLoading();
    allocAndInitWorldmapState();
    MissingCode();

    worldmapFadePolyAbr = 2;
    worldmapFadePolyStep = 4;
    MissingCode();
    worldmapFadeRunning = 1;

    MissingCode();
    initWorldmapMinimap();
    MissingCode();

    if (isWorldmapModeFlagSet == 0) {
        MissingCode("Start worldmap music");
    }
    else {
        assert(0);
    }

    if (isWorldmapModeFlagSet == 0) {
        for (int i = 0; i < worldmapMode0_tasks.size(); i++) {
            setupWorldmapTask(worldmapMode0_tasks[i].m0_init, worldmapMode0_tasks[i].m4_update);
        }
        MissingCode();
    }
    else {
        assert(0);
    }

    MissingCode();

    createWorlmapPolyFT3Array();
    createWorlmapPolyFT4Array1();
    createWorlmapPolyFT4Array2();
    createWorlmapPolyFT4Array3();

    MissingCode();

    initFontPalettes(0x130, 0x1E0);
}

void worldmapMode0_free(void) {
    MissingCode();
}

std::array<sWorldmapModes, 19> worldmapModes = { {
        {worldmapMode0_init, worldmapMode0_update, worldmapMode0_free},
        {worldmapMode0_init, worldmapMode0_update, worldmapMode0_free},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
        {nullptr, nullptr, nullptr},
} };

void worldmapFlushCacheAndSync(void)
{
    /*
    DrawSync(0);
    VSync(0);
    EnterCriticalSection();
    DrawSync(0);
    VSync(0);
    FlushCache();
    ExitCriticalSection();
    return;*/
    MissingCode();
}


void allocateWorldMapsOT(void)
{
    worldmapRenderingStructs[0].m70_OT.resize(1024);
    worldmapRenderingStructs[1].m70_OT.resize(1024);
}

void setupWorldmapGearMode(void)

{
    if ((gameState.m1834 & 0x4000) == 0) {
        worldMapGearMode = 2;
        if ((byte)(gameState.m22B1_isOnGear[2] | gameState.m22B1_isOnGear[0] | gameState.m22B1_isOnGear[1]) == 0) {
            worldMapGearMode = 1;
        }
    }
    else if (true) {
        switch (gameState.m1834 & 0x1fff) {
        case 1:
            worldMapGearMode = 4;
            break;
        case 2:
            worldMapGearMode = 5;
            break;
        case 3:
            worldMapGearMode = 7;
            break;
        case 4:
            worldMapGearMode = 7;
        }
    }
    return;
}

void allocateWorldmapData(void) {
    MissingCode();
}

static const std::array<s16, 8> SHORT_ARRAY_worldmap__8009af80 = { {
    0x5B00, 0x4720, 0x3A78, 0x2CE3,
    0x0C00, 0x79A8, 0x6DF4, 0x6414
} };

static const std::array<s16, 8> SHORT_ARRAY_worldmap__8009af90 = { {
     0x5CB8, 0x40C0, 0x3E88, 0x40AA,
     0x3F48, 0x38D8, 0x3914, 0x4220,
} };

std::vector<s16> SHORT_ARRAY_worldmap__8009b566 = { {
    0x18, 0x36, 0x87, 0x95,
    0xBA, 0xC6, 0xCC, 0xED,
    -1
} };

std::vector<s16> SHORT_ARRAY_worldmap__8009b57a = { {
    0x36, 0xC9, 0x154, -1,
    0x0, 0x2B, 0x10, 0x10, 0x2
} };

std::vector<s16> SHORT_ARRAY_worldmap__8009b58c = { {
    0x36, 0x10, 0x10, 0x2,
    0x41, 0x10, 0x10, 0x2,
} };

void initWorldMap(int param_1, int param_2)
{
    std::vector<s16>::iterator psVar1;
    int iVar2;

    if (param_1 < 8) {
        iVar2 = 1;
        if ((int)(uint)(ushort)SHORT_ARRAY_worldmap__8009b566[0] <= param_2) {
            psVar1 = SHORT_ARRAY_worldmap__8009b566.begin();
            do {
                psVar1 = psVar1 + 1;
                iVar2 += 1;
            } while (*psVar1 <= param_2);
        }
        psVar1 = SHORT_ARRAY_worldmap__8009b57a.begin() + iVar2 * 4 + 1;
        initWorldMapVar0 = iVar2 + -1;
    }
    else {
        psVar1 = SHORT_ARRAY_worldmap__8009b58c.begin() + param_1 * 4;
    }
    currentBattleMusicId = *psVar1 + 8;
    worldmapFile9 = *psVar1 + 9;
    worldmapFile10 = *psVar1 + 10;
    worldmapFile3 = *psVar1 + 3;
    worldmapFile2 = *psVar1 + 2;
    worldmapFile4 = *psVar1 + 4;
    MissingCode();
    worldmapFile1 = *psVar1 + 1;
    worldmapFile6 = *psVar1 + 6;
    worldmapFile5 = *psVar1 + 5;
    exitWorldMapMode = (int)psVar1[3];
    worldmapFile7 = *psVar1 + 7;
    return;
}

void worldmapMainLoop(void) {
    pCurrentWorldmapRenderingStruct = &worldmapRenderingStructs[1];
    worldmapOddOrEven = 1;
    continueWorldmapLoop = 1;
    do {
        MissingCode();

        if (pCurrentWorldmapRenderingStruct == &worldmapRenderingStructs[0]) {
            pCurrentWorldmapRenderingStruct = &worldmapRenderingStructs[1];
        }
        else {
            pCurrentWorldmapRenderingStruct = &worldmapRenderingStructs[0];
        }

        worldmapOddOrEven = worldmapOddOrEven == 0;

        ClearOTagR(&pCurrentWorldmapRenderingStruct->m70_OT[0], 0x400);
        clearShapeTransfertTableEntry(worldmapOddOrEven);
        uploadCharacterSprites();
        callAllWorlmapObjects();
        DrawSync(0);
        VSync(2);
        checkSoftReboot();
        PutDispEnv(&pCurrentWorldmapRenderingStruct->m5C_DispEnv);
        PutDrawEnv(&pCurrentWorldmapRenderingStruct->m0_DrawEnv);

        MissingCode();

        shapeTransfert();
        MissingCode();
        //SetGeomOffset(0xa0, worldmapGeometryOffsetY);
        DrawOTag(&pCurrentWorldmapRenderingStruct->m70_OT[0x3ff]);

    } while (continueWorldmapLoop != 0);

    MissingCode();
}

void worldmapEntryPoint(void) {
    worldmapFlushCacheAndSync();
    DrawSync(0);
    VSync(0);
    //VSyncCallback(vsyncCallback);
    MissingCode();
    InitGeom();
    isWorldMapOverlayLoaded = 1;

    if (gameState.m2320_worldmapMode == 0) {
        gameState.m231E = 0xfff;
        gameState.m231C_CameraYaw = 0xc00;
        gameState.m231A_fieldID = 0x400;
        gameState.m2320_worldmapMode = 1;
        gameState.m1834 = 0x4003;
        gameState.m182C[0] = 0x6680;
        gameState.m182C[1] = -0x100;
        gameState.m182C[2] = 0x2a00;
        gameState.m1822 = 0x2c00;
        gameState.m1D34_currentParty[1] = 10;
        gameState.m294[0].m78_partyData_gearNum = 0xf;
        gameState.m294[1].m78_partyData_gearNum = 2;
        gameState.m294[3].m78_partyData_gearNum = 4;
        gameState.m1D34_currentParty[2] = 5;
        gameState.m294[4].m78_partyData_gearNum = 5;
        gameState.m294[5].m78_partyData_gearNum = 6;
        gameState.m294[7].m78_partyData_gearNum = 7;
        gameState.m294[8].m78_partyData_gearNum = 8;
        gameState.m294[2].m78_partyData_gearNum = 3;
        gameState.m294[9].m78_partyData_gearNum = 3;
        gameState.m1836 = 1;
        gameState.m182C[3] = 0;
        gameState.m1820 = 0x7580;
        gameState.m1824 = 0;
        gameState.m1D34_currentParty[0] = 0;
        gameState.m22B1_isOnGear[0] = 0;
        gameState.m22B1_isOnGear[1] = 0;
        gameState.m22B1_isOnGear[2] = 0;
        gameState.m294[6].m78_partyData_gearNum = 9;
        gameState.m294[10].m78_partyData_gearNum = 9;
        gameState.m1930_fieldVarsBackup[21] = 0x400;
        gameState.m1930_fieldVarsBackup[22] = 0x7500;
        gameState.m1930_fieldVarsBackup[23] = 0x2e58;
        gameState.m1930_fieldVarsBackup[24] = 0x400;
        gameState.m1930_fieldVarsBackup[25] = 0x7580;
        gameState.m1930_fieldVarsBackup[26] = 0x2e58;
        gameState.m1930_fieldVarsBackup[27] = 0x400;
        gameState.m1930_fieldVarsBackup[28] = 0x7600;
        gameState.m1930_fieldVarsBackup[29] = 0x2e58;
        gameState.m1848 = 1;
        gameState.m1844[0] = SHORT_ARRAY_worldmap__8009af80[(ushort)gameState.m1844[1]];
        gameState.m1844[1] = SHORT_ARRAY_worldmap__8009af90[(ushort)gameState.m1844[1]];
        //TODO: Check if that's correct and not endian dependent
        *(u32*)&gameState.m1930_fieldVarsBackup[254] = 0x7FFFFFFF;
    }
    resetMemoryAllocStats(3, 0);
    setCurrentDirectory(0x24, 0);
    allocateWorldmapData();
    allocateWorldMapsOT();
    setupWorldmapGearMode();
    isWorldmapModeFlagSet = (uint)((gameState.m2320_worldmapMode & 0x8000U) != 0);
    MissingCode();

    currentWorldmapMode = gameState.m2320_worldmapMode & 0x7fff;

    MissingCode();

    gameState.m2320_worldmapMode = currentWorldmapMode;

    initWorldMap(currentWorldmapMode, (uint)gameState.m1930_fieldVarsBackup[0]);

    if (worldmapModes[currentWorldmapMode].m0_init) {
        worldmapModes[currentWorldmapMode].m0_init();
    }

    while (1) {
        if (exitWorldMapMode < 2)
            break;

        worldmapModes[currentWorldmapMode].m4_update();

        MissingCode();
        DrawSync(0);
        VSync(0);
        resetInputs();
        isWorldmapModeFlagSet = exitWorldMapMode;
        worldmapMainLoop();

        worldmapModes[currentWorldmapMode].m8_free();
    }

    switch (exitWorldMapMode) {
        default:
            assert(0);
    }

    isWorldMapOverlayLoaded = 0;
    worldmapFlushCacheAndSync();
    bootGame(0);
}