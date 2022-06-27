#include "noahLib.h"
#include "battle.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battleRenderContext.h"

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

void battleMain() {

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