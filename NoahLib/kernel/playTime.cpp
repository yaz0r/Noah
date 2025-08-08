#include "noahLib.h"
#include "playTime.h"

s32 timeOverflow = 0;
s32 timeVSyncCount = 0;
u32 timeHours = 0;
u32 timeMinutes = 0;
u32 timeSeconds = 0;

void incrementPlayTime()
{
    if (timeOverflow == 0) {
        timeVSyncCount += 1;
        if (timeVSyncCount == 60) {
            timeVSyncCount = 0;
            timeSeconds += 1;
        }
        if (timeSeconds == 60) {
            timeSeconds = 0;
            timeMinutes += 1;
        }
        if (timeMinutes == 60) {
            timeMinutes = 0;
            timeHours += 1;
        }
        if (timeHours == 100) {
            timeOverflow = 1;
        }
    }
}