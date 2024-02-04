#include "noahLib.h"
#include "spu.h"

s32 spuUpdateCounter = 0;

void SpuSetTransferMode(int) {
    MissingCode();
}

void SpuSetTransferStartAddr(int) {
    MissingCode();
}

void SpuRead(std::vector<u8>::iterator it, int size) {
    assert(0);
}

void SpuWrite(std::vector<u8>::iterator it, int size) {
    MissingCode();
}

spuDmaCompletionCallbackType SpuSetTransferCallback(spuDmaCompletionCallbackType newCallback) {
    static spuDmaCompletionCallbackType currentCallback = nullptr;

    spuDmaCompletionCallbackType previousCallback = currentCallback;
    currentCallback = newCallback;
    return previousCallback;
}

s32 audioTickEvent = 0;
void DisableEvent(s32) {
    MissingCode();
}
void EnableEvent(s32) {
    MissingCode();
}
