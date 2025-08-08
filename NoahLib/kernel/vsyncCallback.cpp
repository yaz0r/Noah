#include "noahLib.h"
#include "kernel/playTime.h"
#include "kernel/inputs.h"

void vsyncCallback(void)
{
    g_playTimeInVsync++;
    getInputDuringVsync();
    saveInputs();
    incrementPlayTime();
    MissingCode();
}