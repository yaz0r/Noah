#include "noahLib.h"
#include "playTime.h"

u32 g_playTimeInVsync = 0;

bool g_playTimeOverflow = false;
s8 g_timeVSyncCount = 0;
u8 g_playTimeHours = 0;
u8 g_playTimeMinutes = 0;
u8 g_playTimeSeconds = 0;

void incrementPlayTime()
{
    if (!g_playTimeOverflow) {
        g_timeVSyncCount++;
        if (g_timeVSyncCount == 60) {
            g_timeVSyncCount = 0;
            g_playTimeSeconds++;
        }
        if (g_playTimeSeconds == 60) {
            g_playTimeSeconds = 0;
            g_playTimeMinutes++;
        }
        if (g_playTimeMinutes == 60) {
            g_playTimeMinutes = 0;
            g_playTimeHours++;
        }
        if (g_playTimeHours == 100) {
            g_playTimeOverflow = true;
        }
    }
}