#include "noahLib.h"
#include "battle.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battleRenderContext.h"
#include "kernel/memory.h"
#include "battleStartEffect.h"

s8 battleDebugDisplay = 0;

void initGraphicsForBattle(void) {
    ResetGraph(1);
    MissingCode("Some dummy function calls");
    InitGeom();
    SetGeomOffset(0xa0, 0xb4);
    SetGeomScreen(0x200);
    SetDefDispEnv(&battleRenderStructs[0].m5C_dispEnv, 0, 0xe0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
    SetDefDispEnv(&battleRenderStructs[1].m5C_dispEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[1].m0_drawEnv, 0, 0xe0, 0x140, 0xe0);
    initBattleRenderStructs(&battleRenderStructs[0]);
    initBattleRenderStructs(&battleRenderStructs[1]);
}

struct sBattleVar0 {
    // size 0xA2B4
}*battleVar0 = nullptr;

struct sBattleVar1 {
    // size 0x10C
}*battleVar1 = nullptr;

struct sBattleVar2 {
    // size 0x2F8
}*battleVar2 = nullptr;

s8 battleTransitionEffect = 0;
s8 currentBattleLoaderTransitionEffect;

void initBattleDefDrawContext() {
    SetGeomOffset(0xa0, 0xa4);
    SetDefDispEnv(&battleRenderStructs[0].m5C_dispEnv, 0, 0xe0, 0x140, 0xe0);
    SetDefDispEnv(&battleRenderStructs[1].m5C_dispEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[1].m0_drawEnv, 0, 0xe0, 0x140, 0xe0);
    battleRenderStructs[0].m5C_dispEnv.screen.x = 0;
    battleRenderStructs[0].m5C_dispEnv.screen.y = 10;
    battleRenderStructs[0].m5C_dispEnv.screen.w = 0x100;
    battleRenderStructs[0].m5C_dispEnv.screen.h = 0xd8;
    battleRenderStructs[1].m5C_dispEnv.screen.x = 0;
    battleRenderStructs[1].m5C_dispEnv.screen.y = 10;
    battleRenderStructs[1].m5C_dispEnv.screen.w = 0x100;
    battleRenderStructs[1].m5C_dispEnv.screen.h = 0xd8;
}

sLoadingBatchCommands battleLoadingCommands[4];

std::vector<u8> battleStartSeq;
std::vector<u8> battleCharacterConfigFile;
std::vector<u8> battleLoaderBinary;

void loadBattleLoader() {
    resetMemoryAllocStats(2, 0);
    setCurrentDirectory(0xc, 0);

    MissingCode("Code to get the amount of free memory before battle loader");

    battleStartSeq.resize(getFileSizeAligned(2));
    battleCharacterConfigFile.resize(getFileSizeAligned(3));

    battleLoadingCommands[0].m0_fileIndex = 2;
    battleLoadingCommands[0].m4_loadPtr = &battleStartSeq;
    battleLoadingCommands[1].m0_fileIndex = 3;
    battleLoadingCommands[1].m4_loadPtr = &battleCharacterConfigFile;
    battleLoadingCommands[2].m0_fileIndex = 4;
    battleLoadingCommands[2].m4_loadPtr = &battleLoaderBinary;
    battleLoadingCommands[3].m0_fileIndex = 0;
    battleLoadingCommands[3].m4_loadPtr = nullptr;
    batchStartLoadingFiles(battleLoadingCommands, 0);

    while (isCDBusy() == 3) {};

    MissingCode("Start battle transition seq");
}

void startBattleLoader(int param_1)
{
    currentBattleLoaderTransitionEffect = (byte)param_1;
    initBattleDefDrawContext();
    loadBattleLoader();

    switch (param_1) {
    case 0:
        battleStartEffect();
        break;
    default:
        assert(0);
    }

    MissingCode();
}

void battleMain() {
    battleVar0 = new sBattleVar0;
    battleVar1 = new sBattleVar1;
    battleVar2 = new sBattleVar2;

    memset(battleVar0, 0, sizeof(sBattleVar0));
    memset(battleVar1, 0, sizeof(sBattleVar1));
    memset(battleVar2, 0, sizeof(sBattleVar2));

    MissingCode();

    startBattleLoader(battleTransitionEffect);

    MissingCode();
}

void battleEntryPoint(void) {
    battleDebugDisplay = 1;
    waitReadCompletion(0);
    setCurrentDirectory(0xc, 0);
    MissingCode("DTL codepath init");
    initGraphicsForBattle();
    battleMain();

    MissingCode("Battle epilog");
}