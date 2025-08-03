#include "noahLib.h"
#include "worldmap.h"
#include "worldmapTask.h"
#include "worldmapTaskFader.h"
#include "psx/libgpu.h"
#include "kernel/graphics.h"

s32 worldmapFadeRunning;
s32 worldmapFadePolyAbr;
s32 worldmapFadePolyStep;
DR_TPAGE worldmapFadePolyDrawTpage;
std::array<POLY_G4, 2> worldmapFadePoly;

s32 worldmap_taskFader_init(s32 index)
{
    gWorldmapState->m0[index].m50 = 0xF8;

    SetDrawTPage(&worldmapFadePolyDrawTpage, 1, 0, GetTPage(0, worldmapFadePolyAbr, 0x380, 0x100));

    SetPolyG4(&worldmapFadePoly[0]);
    worldmapFadePoly[0].r0 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].g0 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].b0 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].r1 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].g1 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].b1 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].r2 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].g2 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].b2 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].r3 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].g3 = gWorldmapState->m0[index].m50;
    worldmapFadePoly[0].b3 = gWorldmapState->m0[index].m50;

    SetSemiTrans(&worldmapFadePoly[0], 1);

    worldmapFadePoly[0].x0 = 0;
    worldmapFadePoly[0].y0 = 0;
    worldmapFadePoly[0].x1 = 0x140;
    worldmapFadePoly[0].y1 = 0;
    worldmapFadePoly[0].x2 = 0;
    worldmapFadePoly[0].y2 = 0xd8;
    worldmapFadePoly[0].x3 = 0x140;
    worldmapFadePoly[0].y3 = 0xd8;

    worldmapFadePoly[1] = worldmapFadePoly[0];
    return 1;
}

s32 worldmap_taskFader_update(s32 index)
{
    s32 nextState = 1;

    if (gWorldmapState->m0[index].m4 == 0xC) {
        assert(0);
    }
    else if (gWorldmapState->m0[index].m4 == 0xD) {
        assert(0);
    }

    if (gWorldmapState->m0[index].m20 == 0) {
        worldmapFadePoly[worldmapOddOrEven].r0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r3 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g3 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b3 = gWorldmapState->m0[index].m50;

        worldmapFadePoly[worldmapOddOrEven].m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
        pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapFadePoly[worldmapOddOrEven];

        worldmapFadePolyDrawTpage.m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
        pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapFadePolyDrawTpage;

        gWorldmapState->m0[index].m50 -= worldmapFadePolyStep;

        if (gWorldmapState->m0[index].m50 <= -1) {
            nextState = 3;
            gWorldmapState->m0[index].m50 = 0;
            worldmapFadeRunning = 0;
        }
    }
    else if (gWorldmapState->m0[index].m20 == 1) {
        worldmapFadePoly[worldmapOddOrEven].r0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b0 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b1 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b2 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].r3 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].g3 = gWorldmapState->m0[index].m50;
        worldmapFadePoly[worldmapOddOrEven].b3 = gWorldmapState->m0[index].m50;

        worldmapFadePoly[worldmapOddOrEven].m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
        pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapFadePoly[worldmapOddOrEven];

        worldmapFadePolyDrawTpage.m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
        pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapFadePolyDrawTpage;

        gWorldmapState->m0[index].m50 += worldmapFadePolyStep;

        if (gWorldmapState->m0[index].m50 >= 0xFF) {
            gWorldmapState->m0[index].m50 = 0xFF;
            worldmapFadeRunning = 0;
        }
    }
    else {
        return 1;
    }

    gWorldmapState->m0[index].m22++;

    return nextState;
}