#include "noahLib.h"
#include "spu.h"
#include "spuEmu/spu.h"

#include "soloud.h"

s32 spuUpdateCounter = 0;

machine_config dummy;
spu_device emulatedSpuDevice(dummy, "SPU", nullptr, 1);

std::mutex spuMutex;

class SpuAudioInstance : public SoLoud::AudioSourceInstance
{
public:
    virtual unsigned int getAudio(float* aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize) override {
        assert(aBufferSize == aSamplesToRead);
        int16_t temp[44100][2];

        int numSamples = std::min<int>(44100, aSamplesToRead);

        spuMutex.lock();
        emulatedSpuDevice.generate(temp, numSamples * 4);
        spuMutex.unlock();

        for (int i = 0; i < aSamplesToRead; i++) {
            aBuffer[i * 2 + 0] = temp[i][0] / (float)0x7FFF;
            aBuffer[i * 2 + 1] = temp[i][1] / (float)0x7FFF;
        }

        return numSamples;
    }
    virtual bool hasEnded() override {
        return false;
    }
};

class SpuAudioSource : public SoLoud::AudioSource
{
public:
    virtual SoLoud::AudioSourceInstance* createInstance() {
        return new SpuAudioInstance;
    }
};

SoLoud::Soloud gSoloudInstance;
SpuAudioSource gSpuAudioSound;

void SpuInit() {
    emulatedSpuDevice.device_start();
    emulatedSpuDevice.device_reset();

    gSoloudInstance.init();
    gSoloudInstance.play(gSpuAudioSound);
}

void SpuSetTransferMode(int) {
    MissingCode();
}

void SpuSetTransferStartAddr(int addrs) {
    spuMutex.lock();
    emulatedSpuDevice.write(0x1a6 / 2, addrs >> 3);
    spuMutex.unlock();
}

void SpuRead(std::vector<u8>::iterator it, int size) {
    assert(0);
}

void SpuWrite(std::vector<u8>::iterator it, int size) {
    spuMutex.lock();
    emulatedSpuDevice.dma_write((u32*)&it[0], 0, size/4);
    spuMutex.unlock();
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
