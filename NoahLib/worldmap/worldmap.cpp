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
#include "sprite/spriteSetup.h"
#include "kernel/TIM.h"
#include "worldmapWorldStreaming.h"
#include "worldmapExit.h"
#include "kernel/audio/wds.h"
#include "kernel/audio/soundSystem.h"
#include "kernel/audio/seq.h"
#include "kernel/audio/soundInstance.h"
#include "kernel/inputs.h"
#include "kernel/3dModel.h"
#include "kernel/kernelVariables.h"

#include "psx/libgpu.h"
#include "psx/libapi.h"

VECTOR worldmapPosition;
u32 worldmapGamestate1824;
SVECTOR worldmapRotation = { 0,0,0,0 };
VECTOR worldmapRadarPosition = { 0,0,0,0 };
s32 worldmapGeometryOffsetY = 0;
s32 worldmapInitialPositionIndex;

s32 worldmapExitToFieldVar0;
s32 fieldBootModeFromWorldmap;

s8 isWorldMapOverlayLoaded = 0;
s32 currentWorldmapMode = 0;
s32 worldMapGearMode = 0;
s32 exitWorldMapMode;
s32 currentBattleMusicId;
s16 worldmapVar_8009d52c;

s32 worldmapModelVar0;
s32 worldmapModelVar1;

MATRIX worldmapMainMatrix2;
s32 worldmapMatrixMode;

s32 worldmapSizeX;
s32 worldmapSizeY;

u16 worldmapInput1_0;
u16 worldmapInput1_1;
u16 worldmapInput2_0;
u16 worldmapInput2_1;
u16 worldmapInput3_0;
u16 worldmapInput3_1;

s16 worldmapExitVar2;

VECTOR worldmapRadarPosition3 = { 0,0,0 };
VECTOR worldmapGridInputPosition = { 0,0,0 };

std::array<u8, 3> worldMapFarColor = { {0xC0, 0xD0, 0xF0} };

std::array<sWorldmapRenderingStruct, 2> worldmapRenderingStructs;

struct sWorldmapModes {
    void (*m0_init)();
    void (*m4_update)();
    void (*m8_free)();
};

std::array<sSpriteActorAnimationBundle*, 3 > worldmapPartySprites;
std::array<sLoadableDataRaw, 3 > worldmapPartySpritesRaw;
std::array<sSpriteActorAnimationBundle*, 3 > worldmapPartyGearSprites;
std::array<sLoadableDataRaw, 3 > worldmapPartyGearSpritesRaw;
s32 worldmapNumActivePartyMembers = 0;

std::array<sLoadingBatchCommands, 16> worldmapLoadingCommands;

s32 isWorldmapModeFlagSet;
s32 worldmapOddOrEven;
s32 continueWorldmapLoop;

sWorldmapRenderingStruct* pCurrentWorldmapRenderingStruct = nullptr;
sWorldmapCameraState worldmapCameraVector;

void worldmapMode0_init(void) {
    for (int i = 0; i < 3; i++) {
        if (gameState.m1D34_currentParty[i] == 0xFF) {
            worldmapPartySpritesRaw[i].clear();
            worldmapPartyGearSpritesRaw[i].clear();
        }
        else {
            worldmapPartySpritesRaw[i].resize(getFileSizeAligned(gameState.m1D34_currentParty[i] + 2));
            if (gameState.m26C_party[i].mA0_partyData_gearNum == -1) {
                worldmapPartyGearSpritesRaw[i].clear();
            }
            else {
                worldmapPartyGearSpritesRaw[i].resize(getFileSizeAligned(gameState.m26C_party[i].mA0_partyData_gearNum + 0x19));
            }
        }
    }

    worldmapNumActivePartyMembers = 0;

    std::array<sLoadingBatchCommands, 16>::iterator it = worldmapLoadingCommands.begin();
    for (int i = 0; i < 3; i++) {
        if (gameState.m1D34_currentParty[i] != 0xFF) {
            it->m0_fileIndex = gameState.m1D34_currentParty[i] + 2;
            it->m4_loadPtr = &worldmapPartySpritesRaw[i];
            it++;
            worldmapNumActivePartyMembers++;
            if (gameState.m26C_party[i].mA0_partyData_gearNum != -1) {
                it->m0_fileIndex = gameState.m26C_party[i].mA0_partyData_gearNum + 0x13;
                it->m4_loadPtr = &worldmapPartyGearSpritesRaw[i];
                it++;
            }
        }
    }

    it->m0_fileIndex = 0;
    it->m4_loadPtr = nullptr;

    batchStartLoadingFiles(&worldmapLoadingCommands[0], 0);
}

void initWorldmapGraphics(void)
{
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
    long a = 0x800;
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

sLoadableDataRaw worldmapFile1Buffer;
sLoadableDataRaw worldmapFile2Buffer;
sLoadableDataRaw worldmapFile3Buffer;

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

sModel worldmapModelFiles;
std::vector<u8>::const_iterator worldmapFile1Buffer_C;
std::vector<u8>::const_iterator worldmapFile1Buffer_14;
std::vector<u8>::const_iterator worldmapFile1Buffer_18;
std::vector<u8>::const_iterator worldmapFile1Buffer_1C;
std::vector<u8>::const_iterator worldmapFile1Buffer_20;
std::vector<u8>::const_iterator worldmapFile1Buffer_24;

std::vector<u8>::const_iterator worldmapFile1Buffer_4_0;

std::vector<u8>::const_iterator pWorldmapModelsConfig;

std::array<std::vector<u8>::const_iterator, 16> worldmapFile1Buffer_2C_data;

void finalizeWorldMapFileLoading() {
    worldmapFile1Buffer.init(mallocAndDecompress(worldmapFile1Buffer.getRawData().begin()));

    worldmapFile1Buffer_C = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0xC);
    worldmapModelFiles.init(worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x8), READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0xC) - READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x8));
    worldmapFile1Buffer_14 = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x14);
    pWorldmapModelsConfig = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x10);
    worldmapFile1Buffer_1C = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x1C);
    worldmapFile1Buffer_18 = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x18);
    worldmapFile1Buffer_24 = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x24);
    worldmapFile1Buffer_20 = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x20);

    for (int i = 0; i < 0x10; i++) {
        worldmapFile1Buffer_2C_data[i] = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x2C + 4 * i);
    }

    std::vector<u8>::const_iterator worldmapFile1Buffer_4 = worldmapFile1Buffer.getRawData().begin() + READ_LE_U32(worldmapFile1Buffer.getRawData().begin() + 0x4);

    worldmapFile1Buffer_4_0 = worldmapFile1Buffer_4 + READ_LE_U32(worldmapFile1Buffer_4);

    // init the worldmap exits. Original code was a relocation + cast
    {
        std::vector<u8>::const_iterator data = worldmapFile1Buffer_4 + READ_LE_U32(worldmapFile1Buffer_4 + 4);
        int numExitGroup = (READ_LE_U32(data) - READ_LE_U32(worldmapFile1Buffer_4 + 4)) / 4;
        assert(numExitGroup == 4);

        for (int i = 0; i < numExitGroup; i++) {
            std::vector<u8>::const_iterator exitData = worldmapFile1Buffer_4 + READ_LE_U32(data + 4 * i);

            do {
                sWorldmapExitDef newExit;
                newExit.m0_X = READ_LE_S16(exitData + 0x00);
                newExit.m2_Y = READ_LE_S16(exitData + 0x02);
                newExit.m4_width = READ_LE_S16(exitData + 0x04);
                newExit.m6_height = READ_LE_S16(exitData + 0x06);
                newExit.m8_destinationField = READ_LE_S16(exitData + 0x08);
                newExit.mA = READ_LE_S16(exitData + 0x0A);
                newExit.mC_newWorldmapMode = READ_LE_S16(exitData + 0x0C);
                newExit.mE_type = READ_LE_S16(exitData + 0x0E);
                worldmapExitsDefs[i].push_back(newExit);

                if (newExit.m8_destinationField == -1) {
                    break;
                }

                exitData += 0x10;
            } while (1);
        }
    }

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

void getGamestate182C(VECTOR* param_1)
{
    param_1->vx = (int)gameState.m182C.vx << 0xc;
    param_1->vy = (int)gameState.m182C.vy << 0xc;
    param_1->vz = (int)gameState.m182C.vz << 0xc;
    return;
}

void setGamestate182C(VECTOR* param_1) {
    gameState.m182C.vx = (short)(param_1->vx >> 0xc);
    gameState.m182C.vy = (short)(param_1->vy >> 0xc);
    gameState.m182C.vz = (short)(param_1->vz >> 0xc);
}

void setupWorldmapPositionFromField() {
    gameState.m1836 = 0;
    if (true) {
        switch (worldMapGearMode) {
        case 1:
        case 2:
            worldmapGamestate1824 = (uint)(ushort)gameState.m1820_worldmapPosition[2];
            worldmapPosition.vx = (uint)(ushort)gameState.m1820_worldmapPosition[0] << 0xc;
            worldmapPosition.vz = (uint)(ushort)gameState.m1820_worldmapPosition[1] << 0xc;
            break;
        default:
            getGamestate182C(&worldmapPosition);
            worldmapGamestate1824 = (uint)(ushort)gameState.m182C.pad;
        }
    }
}

void setupWorldmapPositionFromFileData(int param_1) {
    if ((gameState.m1834 & 0x2000) == 0) {
        std::vector<u8>::const_iterator worldmapInitialPosition = worldmapFile1Buffer_4_0;

        while (READ_LE_S16(worldmapInitialPosition + 2) != -1) {
            if (READ_LE_S16(worldmapInitialPosition + 2) == param_1) {
                worldmapPosition.vx = ((s32)READ_LE_S16(worldmapInitialPosition)) << 0xc;
                worldmapPosition.vy = 0;
                worldmapPosition.vz = ((s32)READ_LE_S16(worldmapInitialPosition + 4)) << 0xc;
                return;
            }

            worldmapInitialPosition += 8;
        }

        worldmapPosition.vx = 0;
        worldmapPosition.vy = 0;
        worldmapPosition.vz = 0;
    }
    else {
        gameState.m1834 = gameState.m1834 & 0xdfff;
        getGamestate182C(&worldmapPosition);
        worldmapGamestate1824 = (uint)(ushort)gameState.m182C.pad;
    }
}

std::array<u8, 3> DAT_worldmap__800704dc = { { 0xE0, 0xF5, 0xFF } };
std::array<u16, 16> SHORT_ARRAY_8009bce0;

void tweakWorldmapTexture(std::vector<u16>::iterator param_1, std::vector<u16>::iterator param_2, int param_3, const std::array<u8, 3>& param_4) {
    u8 bVar1 = param_4[0];
    u8 bVar2 = param_4[1];
    u8 bVar3 = param_4[2];
    s32 iVar8 = 0;
    if (0 < param_3) {
        s32 iVar5 = 0;
        do {
            s32 iVar9 = iVar5 / param_3;
            if (param_3 == 0) {
                assert(0);
            }
            if ((param_3 == -1) && (iVar5 == -(s32)0x80000000)) {
                assert(0);
            }
            s32 iVar7 = 0;
            iVar5 = 0x1000 - iVar9;
            std::vector<u16>::iterator puVar6 = param_1;
            do {
                u16 uVar4 = *puVar6;
                if (uVar4 == 0) {
                    *param_2 = uVar4;
                }
                else {
                    *param_2 = *puVar6 & 0x8000 | (ushort)((uVar4 & 0x1f) * 8 * iVar5 + (uint)bVar1 * iVar9 >> 0xf) | (ushort)(((*puVar6 >> 2 & 0xf8) * iVar5 + (uint)bVar2 * iVar9 >> 0xf) << 5) |
                        (ushort)(((*puVar6 >> 7 & 0xf8) * iVar5 + (uint)bVar3 * iVar9 >> 0xf) << 10);
                }
                iVar7 = iVar7 + 1;
                puVar6 = puVar6 + 1;
                param_2 = param_2 + 1;
            } while (iVar7 < 0x100);
            iVar8 = iVar8 + 1;
            iVar5 = iVar8 * 0x1000;
        } while (iVar8 < param_3);
    }
}

std::array<u16, 0x40> USHORT_ARRAY_8009ccb4;
std::array<u16, 8> USHORT_ARRAY_8009cd54;

void loadWorldmapTextureFromFile2(void) {
    loadMenuImageBundle(mallocAndDecompress(worldmapFile2Buffer.getRawData().begin())); // Those are ground textures
    DrawSync(0);
    worldmapFile2Buffer.clear();

    std::vector<u16> buffer0;
    buffer0.resize(0x200);

    std::vector<u16> buffer1;
    buffer1.resize(0x4000);

    RECT local_30;
    local_30.y = 0x1e0;
    local_30.x = 0;
    local_30.w = 0x100;
    local_30.h = 2;
    StoreImage(&local_30, buffer0);
    DrawSync(0);
    tweakWorldmapTexture(buffer0.begin(), buffer1.begin(), 0x20, worldMapFarColor);
    tweakWorldmapTexture(buffer0.begin() + 0x100, buffer1.begin() + 0x2000, 0x20, worldMapFarColor);
    local_30.y = 0x1b0;
    local_30.x = 0;
    local_30.w = 0x100;
    local_30.h = 0x40;
    LoadImage(&local_30, (u8*)&buffer1[0]);
    DrawSync(0);

    for (int i = 0; i < 0x40; i++) {
        USHORT_ARRAY_8009ccb4[i] = GetClut((int)local_30.x, (int)local_30.y + i);
    }

    for (int i = 0; i < 4; i++) {
        USHORT_ARRAY_8009cd54[i] = GetTPage(1, 0, 0x200 + 0x80 * i, 0);
    }
    for (int i = 4; i < 8; i++) {
        USHORT_ARRAY_8009cd54[i] = GetTPage(1, 0, 0x180 + 0x80 * (i - 4), 0x100);
    }
}

void loadWorldmapTextureFromFile3(void) {
    // there was some code dealing with aligning the color in stack memory, but that seems unnecessary

    loadMenuImageBundle(mallocAndDecompress(worldmapFile3Buffer.getRawData().begin())); // Those are generic worlmap shared resources
    DrawSync(0);
    worldmapFile3Buffer.clear();

    std::vector<u16> buffer0;
    buffer0.resize(0x100);

    std::vector<u16> buffer1;
    buffer1.resize(0x1000);

    RECT local_30;
    local_30.x = 0;
    local_30.y = 0x1F0;
    local_30.w = 0x100;
    local_30.h = 1;
    StoreImage(&local_30, buffer0);

    DrawSync(0);
    tweakWorldmapTexture(buffer0.begin(), buffer1.begin(), 0x10, DAT_worldmap__800704dc);

    local_30.x = 0;
    local_30.y = 0x1f0;
    local_30.w = 0x100;
    local_30.h = 0xf;
    LoadImage(&local_30, (u8*)&buffer1[0]);
    DrawSync(0);

    for (int i = 0; i < 0x10; i++) {
        SHORT_ARRAY_8009bce0[i] = GetClut((int)local_30.x, (int)local_30.y + i);
    }
}

s32 worldmapFile1Buffer_8_numEntries;
s32 worldmapNumModels;

std::vector<sWorldmapModel> worldmapModels;
MATRIX worldmapColorMatrix;
MATRIX worldmapLightMatrix;

void worldmapInitAllModels() {
    //worldmapFile1Buffer_8_numEntries = fieldModelRelocation(worldmapModelFiles);
    MissingCode();


    worldmapNumModels = READ_LE_U16(pWorldmapModelsConfig);
    worldmapModels.resize(worldmapNumModels);

    SetColorMatrix(&worldmapColorMatrix);
    SetLightMatrix(&worldmapLightMatrix);

    for (int i = 0; i < worldmapNumModels; i++) {
        sWorldmapModel* pModel = &worldmapModels[i];

        pModel->m0_hidden = 0;
        pModel->m2_modelBlockIndex = READ_LE_U16(pWorldmapModelsConfig + 2 + 0x10 * i + 0x0);
        pModel->m4_flags = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0x2);

        pModel->m8.vx = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0x4);
        pModel->m8.vy = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0x6);
        pModel->m8.vz = -READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0x8);

        pModel->m18_rotation.vx = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0xA);
        pModel->m18_rotation.vy = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0xC);
        pModel->m18_rotation.vz = READ_LE_S16(pWorldmapModelsConfig + 2 + 0x10 * i + 0xE);

        RotMatrixYXZ(&pModel->m18_rotation, &pModel->m20_rotationMatrix);

        pModel->m40_modelBlock = &worldmapModelFiles.m10_blocks[pModel->m2_modelBlockIndex];

        initModel1(*pModel->m40_modelBlock, pModel->m48[0], pModel->m48[1]);
        initModel2(pModel->m40_modelBlock, pModel->m48[0], 1);
        pModel->m48[1] = pModel->m48[0];

        pModel->m44_collisionMesh = worldmapFile1Buffer_C + READ_LE_U32(worldmapFile1Buffer_C + 4 + pModel->m2_modelBlockIndex * 4);

        MissingCode();

        pModel->m50_parentModel = nullptr;
    }
    gDepthDivider = 2;

    worldmapModelVar0 = -1;
    worldmapModelVar1 = -1;

    worldmapModels[4].m0_hidden = 1;
}

sWdsFile* worldmapWds;
sSeqFile worldmapSound1;
sSeqFile worldmapSound2;
sLoadableDataRaw worldmapSound3;
sSeqFile* fieldSeq;

void startLoadingWorldmapSoundFiles() {
    worldmapSoundFilesPendingCount++;

    auto it = worldmapLoadingCommands.begin();

    it->m0_fileIndex = worldmapFile4;
    it->m4_loadPtr = worldmapWds = new sWdsFile;
    it++;

    it->m0_fileIndex = worldmapFile5;
    it->m4_loadPtr = &worldmapSound1;
    it++;

    it->m0_fileIndex = worldmapFile6;
    it->m4_loadPtr = fieldSeq = new sSeqFile;
    it++;

    it->m0_fileIndex = worldmapFile7;
    it->m4_loadPtr = &worldmapSound2;
    it++;

    it->m0_fileIndex = currentBattleMusicId;
    it->m4_loadPtr = &worldmapSound3;
    it++;

    it->m0_fileIndex = 0;
    it->m4_loadPtr = nullptr;

    batchStartLoadingFiles(worldmapLoadingCommands.data(), 0);
}

sWdsFile* worldmapWdsResult;
sSoundInstance* pMusic;

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
    //################
    for (int i = 0; i < 3; i++) {
        if(worldmapPartySpritesRaw[i].getRawData().size())
        {
            worldmapPartySprites[i] = new sSpriteActorAnimationBundle();
            worldmapPartySprites[i]->init(worldmapPartySpritesRaw[i].beginSpan());
        }
        else {
            worldmapPartySprites[i] = nullptr;
        }

        if (worldmapPartyGearSpritesRaw[i].getRawData().size())
        {
            worldmapPartyGearSprites[i] = new sSpriteActorAnimationBundle();
            worldmapPartyGearSprites[i]->init(worldmapPartyGearSpritesRaw[i].beginSpan());
        }
        else {
            worldmapPartyGearSprites[i] = nullptr;
        }
    }
    //################
    worldmapMode8_init();
    do {
    } while (2 < isCDBusy());
    finalizeWorldMapFileLoading();
    allocAndInitWorldmapState();
    MissingCode();

    worldmapFadePolyAbr = 2;
    worldmapFadePolyStep = 4;
    worldmapExitVar2 = 0;
    MissingCode();
    worldmapMatrixMode = 0;
    fieldDrawEnvsInitialized = 1;
    worldmapFadeRunning = 1;
    MissingCode();
    worldmapDotInit();

    MissingCode();
    if (gameState.m1836 == 0) {
        if (isWorldmapModeFlagSet == 0) {
            setupWorldmapPositionFromFileData(worldmapInitialPositionIndex);
        }
        else {
            assert(0);
        }
    }
    else {
        setupWorldmapPositionFromField();
    }
    waitReadCompletion(0);
    loadWorldmapTextureFromFile3();
    loadWorldmapTextureFromFile2();
    worldmapInitAllModels();
    startLoadingWorldmapSoundFiles();

    MissingCode();
    initWorldmapMinimap();
    MissingCode();
    allocateShapeTransfert(0x1400);
    MissingCode();

    if (isWorldmapModeFlagSet == 0) {
        worldmapWdsResult = loadWds(*worldmapWds, 0);
    }
    else {
        assert(0);
    }

    setCurrentDirectory(0x24, 0);
    if (isWorldmapModeFlagSet == 0) {
        setupInitialGrid(&worldmapPosition);
        do {
            loadWorldmapChunk();
            VSync(0);
        } while (1 < isWorldStreamingDone());
    }
    else {
        assert(0);
    }

    if (isWorldmapModeFlagSet == 0) {
        // Missing code
        /*
        do {
        } while ((musicStatusFlag & 0x10) != 0);
        */
        delete worldmapWds;
        loadSequence(fieldSeq);
        if (worldMapGearMode == 7) {
            battleMusic = worldmapSound2;
        }
        else {
            battleMusic = worldmapSound1;
        }
        pMusic = createMusicInstance(&battleMusic);
        startMusicInstance(pMusic, 0x7f, 0);
    }
    else {
        // resume music from half-way to prevent petition?
        assert(0);
    }

    MissingCode();

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
    if (exitWorldMapMode == 0) {
        playBattleMusic(pMusic, 0, 0xf0);
    }
    MissingCode();
    unloadSequence(fieldSeq);
    delete fieldSeq; fieldSeq = nullptr;
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

    DrawSync(0);
    VSync(0);
    EnterCriticalSection();
    DrawSync(0);
    VSync(0);
    FlushCache();
    ExitCriticalSection();
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

static const std::array<s16, 8> SHORT_ARRAY_worldmap__8009af80 = { {
    0x5B00, 0x4720, 0x3A78, 0x2CE3,
    0x0C00, 0x79A8, 0x6DF4, 0x6414
} };

static const std::array<s16, 8> SHORT_ARRAY_worldmap__8009af90 = { {
     0x5CB8, 0x40C0, 0x3E88, 0x40AA,
     0x3F48, 0x38D8, 0x3914, 0x4220,
} };

std::vector<s16> scenarioStateToWorldmapConfigsMapping = { {
    0x18, // until Lahan attacked and need to head to the forest
    0x36,
    0x87,
    0x95,
    0xBA,
    0xC6,
    0xCC,
    0xED,
    -1
} };

struct sWorldmapConfig {
    s16 m0_baseFileId;
    s16 m2_sizeX;
    s16 m4_sizeZ;
    s16 m6_exitMode;
};

std::array<sWorldmapConfig, 10> worldmapConfigs = { {
    {0x2B, 0x10, 0x10, 2},
    {0x36, 0x10, 0x10, 2},
    {0x41, 0x10, 0x10, 2},
    {0x4C, 0x10, 0x10, 2},
    {0x57, 0x10, 0x10, 2},
    {0x62, 0x10, 0x10, 2},
    {0x6D, 0x10, 0x10, 2},
    {0x78, 0x10, 0x10, 2},
    {0x83, 0x10, 0x10, 2},

    {0x2B, 0x10, 0x10, 2},
    // ... to be continued
} };

std::vector<s16> SHORT_ARRAY_worldmap__8009b57a = { {
    0x36, 0xC9, 0x154, -1,
    0x0, 0x2B, 0x10, 0x10, 0x2
} };

std::vector<s16> SHORT_ARRAY_worldmap__8009b58c = { {
    0x36, 0x10, 0x10, 0x2,
    0x41, 0x10, 0x10, 0x2,
} };

void initWorldMap(int worldmapMode, int scenarioFlag)
{
    sWorldmapConfig* pConfig;

    if (worldmapMode < 8) {
        int configId = 0;
        while (scenarioStateToWorldmapConfigsMapping[configId] <= scenarioFlag) {
            configId++;
        }

        pConfig = &worldmapConfigs[configId];
        initWorldMapVar0 = configId;
    }
    else {
        pConfig = &worldmapConfigs[worldmapMode + 1];
    }
    currentBattleMusicId = pConfig->m0_baseFileId + 8;
    worldmapFile9 = pConfig->m0_baseFileId + 9;
    worldmapFile10 = pConfig->m0_baseFileId + 10;
    worldmapFile3 = pConfig->m0_baseFileId + 3;
    worldmapFile2 = pConfig->m0_baseFileId + 2;
    worldmapFile4 = pConfig->m0_baseFileId + 4;
    worldmapSizeX = pConfig->m2_sizeX;
    worldmapSizeY = pConfig->m4_sizeZ;
    worldmapFile1 = pConfig->m0_baseFileId + 1;
    worldmapFile6 = pConfig->m0_baseFileId + 6;
    worldmapFile5 = pConfig->m0_baseFileId + 5;
    exitWorldMapMode = pConfig->m6_exitMode;
    worldmapFile7 = pConfig->m0_baseFileId + 7;
    return;
}

void worldmapMainLoop(void) {
    pCurrentWorldmapRenderingStruct = &worldmapRenderingStructs[1];
    worldmapOddOrEven = 1;
    continueWorldmapLoop = 1;
    do {
        MissingCode();

        worldmapInput3_1 = 0;
        worldmapInput2_1 = 0;
        worldmapInput1_1 = 0;
        worldmapInput3_0 = 0;
        worldmapInput2_0 = 0;
        worldmapInput1_0 = 0;

        while (loadInputFromVSyncBuffer()) {
            worldmapInput1_0 = worldmapInput1_0 | newPadButtonForScripts[0].m0_buttons;
            worldmapInput1_1 = worldmapInput1_1 | newPadButtonForScripts[1].m0_buttons;
            worldmapInput2_0 = worldmapInput2_0 | newPadButtonForDialogs;
            worldmapInput2_1 = worldmapInput2_1 | newPadButtonForDialogs2;
            worldmapInput3_0 = worldmapInput3_0 | newPadButtonForField;
            worldmapInput3_1 = worldmapInput3_1 | newPadButtonForField2;
        }

        while (loadWorldmapChunk() == 3) {
            VSync(0);
        }

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

        if (worldmapFadeRunning == 0) {
            Noah_MissingCode("worldmap fade prevents inputs");
        }
        MissingCode();

        // toggle worlmap on and off
        if ((worldmapInput2_0 & 0x100) != 0) {
            gameState.m1842_disableWorldmapMinimap = gameState.m1842_disableWorldmapMinimap ^ 1;
        }

        if (((worldmapFadeRunning == 0) && (worldmapExitVar2 != 0)) && (continueWorldmapLoop != 0)) {
            if (0 < worldMapGearMode) {
                if (worldMapGearMode < 4) {
                    Noah_MissingCode("Menu from worldmap");
                    /*
                    worldmapPrepareEnterMenu();
                    menuToEnter = 0;
                    useDebugMenuList = 0;
                    menuOpenArg = 1;
                    worldmapFlushCacheAndSync();
                    enterMenu();
                    worldmapFlushCacheAndSync();
                    worldmapRestoreAfterMenu(); */
                }
                else if (worldMapGearMode < 8) {
                    continueWorldmapLoop = 0;
                    exitWorldMapMode = 0;
                    worldmapCurrentExit = &worldmapExitToField290_yggdrasilDeck;
                    gameState.m1834 = gameState.m1834 | 0x2000;
                }
            }
        }
        else {
            worldmapExitVar2 = 0;
        }

        shapeTransfert();
        MissingCode();
        SetGeomOffset(0xa0, worldmapGeometryOffsetY);
        DrawOTag(&pCurrentWorldmapRenderingStruct->m70_OT[0x3ff]);

    } while (continueWorldmapLoop != 0);

    MissingCode();
}

void readGameOverlay(int) {
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
        gameState.m231E_worldmapInitialPositionIndex = 0xfff;
        gameState.m231C_CameraYaw = 0xc00;
        gameState.m231A_fieldID = 0x400;
        gameState.m2320_worldmapMode = 1;
        
        gameState.m1834 = 0x4003;
        //gameState.m1834 = 0x0000; Hack("Don't use yggdrazil for worldmap debug mode");

        gameState.m182C[0] = 0x6680;
        gameState.m182C[1] = -0x100;
        gameState.m182C[2] = 0x2a00;
        gameState.m1820_worldmapPosition[1] = 0x2c00;
        gameState.m1D34_currentParty[1] = 10;
        gameState.m26C_party[0].mA0_partyData_gearNum = 0xf;
        gameState.m26C_party[1].mA0_partyData_gearNum = 2;
        gameState.m26C_party[3].mA0_partyData_gearNum = 4;
        gameState.m1D34_currentParty[2] = 5;
        gameState.m26C_party[4].mA0_partyData_gearNum = 5;
        gameState.m26C_party[5].mA0_partyData_gearNum = 6;
        gameState.m26C_party[7].mA0_partyData_gearNum = 7;
        gameState.m26C_party[8].mA0_partyData_gearNum = 8;
        gameState.m26C_party[2].mA0_partyData_gearNum = 3;
        gameState.m26C_party[9].mA0_partyData_gearNum = 3;
        gameState.m1836 = 1;
        gameState.m182C[3] = 0;
        gameState.m1820_worldmapPosition[0] = 0x7580;
        gameState.m1820_worldmapPosition[1] = 0x2c00;
        gameState.m1820_worldmapPosition[2] = 0;
        gameState.m1D34_currentParty[0] = 0;
        gameState.m22B1_isOnGear[0] = 0;
        gameState.m22B1_isOnGear[1] = 0;
        gameState.m22B1_isOnGear[2] = 0;
        gameState.m26C_party[6].mA0_partyData_gearNum = 9;
        gameState.m26C_party[10].mA0_partyData_gearNum = 9;
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
    worldmapInitialPositionIndex = gameState.m231E_worldmapInitialPositionIndex;
    isWorldmapModeFlagSet = (uint)((gameState.m2320_worldmapMode & 0x8000U) != 0);
    worldmapInitialPositionIndex = (uint)gameState.m231E_worldmapInitialPositionIndex;
    worldmapGamestate1824 = (uint)(ushort)gameState.m231C_CameraYaw;
    worldmapExitToFieldVar0 = 0;
    currentWorldmapMode = (ushort)gameState.m2320_worldmapMode & 0x7fff;
    fieldBootModeFromWorldmap = (gameState.m231A_fieldID & 0x3fff) - 0x400;
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
    case 0: // to field
        readGameOverlay(1);
        setGameMode(1);
        if (worldmapExitToFieldVar0 == 0) {
            if (worldmapCurrentExit->mE_type == 3) {
                //FUN_worldmap__80094364(&worldmapRadarPosition, 3, gameState.m1930_fieldVarsBackup[0]);
                MissingCode();
            }
            gameState.m231A_fieldID = worldmapCurrentExit->m8_destinationField;
            gameState.m231C_CameraYaw = worldmapRotation.vy;
            gameState.m2320_worldmapMode = worldmapCurrentExit->mA;
        }
        gameState.m1930_fieldVarsBackup[2] = fieldBootModeFromWorldmap + 0x400; //setup the map param
        break;
    default:
        assert(0);
    }

    isWorldMapOverlayLoaded = 0;
    worldmapFlushCacheAndSync();
    bootGame(0);
}

bool changeWorldmapEntityState(int param_1, short param_2) {
    bool bVar1;
    sWorldmapStateEntry* psVar2;

    psVar2 = &gWorldmapState->m0[param_1];
    bVar1 = psVar2->m4 == 0;
    if (bVar1) {
        psVar2->m0_state = 1;
        psVar2->m4 = param_2;
    }
    return bVar1;
}