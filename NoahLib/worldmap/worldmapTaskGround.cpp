#include "noahLib.h"
#include "worldmap.h"
#include "worldmapTask.h"
#include "worldmapTaskGround.h"
#include "kernel/gameState.h"
#include "worldmapMinimap.h"

void drawWorlmapPolyFT4Array1(void) {
    MissingCode();
}

s32 worldmap_taskGround_init(s32 index) {
    return 1;
}

s32 worldmap_taskGround_update(s32 index) {
    MissingCode();
    drawWorlmapPolyFT4Array1();
    MissingCode();

    if (gameState.m1842_disableWorldmapMinimap == 0) {
        worldmapRenderMinimap();
    }
    return 1;
}

