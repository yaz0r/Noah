#include "noahLib.h"
#include "worldmapTask.h"
#include "worldmap.h"

#include "worldmapMinimap.h"
#include "worldmapTaskFader.h"
#include "worldmapTaskGround.h"
#include "worldmapPlayer.h"
#include "worldmapTaskCamera.h"
#include "worldmapTaskWorldCamera.h"

const std::vector<sWorldTaskDefinition> worldmapMode0_tasks = { {
    {&worldmap_taskFader_init, &worldmap_taskFader_update},
    {&worldmapMode0_taskPlayer_init, &worldmapMode0_taskPlayer_update},
    // TODO: the rest

    {&worldmap_taskWorldCamera_init, &worldmap_taskWorldCamera_update},
    {&worldmap_taskCamera_init, &worldmap_taskCamera_update},
    {&worldmap_taskMinimapY_init, &worldmap_taskMinimapY_update},

    {&worldmap_taskGround_init, &worldmap_taskGround_update}
} };

void callAllWorlmapObjects() {
    for (int i = 0; i < 0x40; i++) {
        sWorldmapStateEntry* pEntry = &gWorldmapState->m0[i];
        if (pEntry->m1C_update) {
            switch (pEntry->m0_state) {
            case 0:
                pEntry->m0_state = pEntry->m18_init(i);
                break;
            case 1:
                pEntry->m0_state = pEntry->m1C_update(i);
                break;
            case 3: // idle
                break;
            default:
                assert(0);
            }
        }
    }
}

void setupWorldmapTask(s32(*pInitFunction)(s32 index), s32(*pUpdateFunction)(s32 index))
{
    for (int i = 0; i < 0x40; i++) {
        if (gWorldmapState->m0[i].m1C_update == nullptr) {
            gWorldmapState->m0[i].m0_state = 0;
            gWorldmapState->m0[i].m2 = 0;
            gWorldmapState->m0[i].m4 = 0;
            gWorldmapState->m0[i].m18_init = pInitFunction;
            gWorldmapState->m0[i].m1C_update = pUpdateFunction;
            gWorldmapState->m0[i].m20 = 0;
            gWorldmapState->m0[i].m22 = 0;
            return;
        }
    }
}