#pragma once

extern class spu_device emulatedSpuDevice;
extern std::mutex spuMutex;

void SpuSetTransferMode(int);
void SpuSetTransferStartAddr(int);
void SpuRead(std::vector<u8>::const_iterator it, int size);
void SpuWrite(std::vector<u8>::const_iterator it, int size);

typedef void(*spuDmaCompletionCallbackType)();
spuDmaCompletionCallbackType SpuSetTransferCallback(spuDmaCompletionCallbackType newCallback);

extern s32 audioTickEvent;
extern s32 spuUpdateCounter;
void DisableEvent(s32);
void EnableEvent(s32);

void SpuInit();