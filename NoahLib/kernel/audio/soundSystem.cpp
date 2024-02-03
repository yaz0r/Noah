#include "noahLib.h"
#include "soundSystem.h"
#include "wds.h"
#include "spu.h"

u16 musicStatusFlag = 0;

void setSoundError(u16 errorCode) {
    assert(0);
}

void initSoundSystem(ushort param_1) {
    if ((short)musicStatusFlag < 0) {
        setSoundError(0x28);
    }
    else {
        musicStatusFlag = param_1 | 0xb801;
        MissingCode();
        initLoadedSampleTable();
        loadWdsSub0SubVar1.resize(8);
        MissingCode();
        SpuSetTransferCallback(spuDmaCompletionCallback);
        MissingCode();
    }
}

int processSoundMenuSub0(void)
{
    int uVar1;

    if ((musicStatusFlag & 0x700) == 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = 1;
        if ((musicStatusFlag & 0x600) != 0) {
            uVar1 = 2;
        }
    }
    return uVar1;
}