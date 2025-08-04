#include "noahLib.h"
#include "wds.h"
#include "soundSystem.h"
#include "psx/libapi.h"
#include "spu.h"

struct structsLoadedSample {
    s8 m0;
    s8 m1;
    s16 m2;
    s32 m4;
    s32 m8_adpcmSize;
};

std::array<structsLoadedSample, 0xC> spuAllocatedBlocks;

void startTransfert();

void initLoadedSampleTable() {
    for (int i = 0; i < 0xC; i++) {
        spuAllocatedBlocks[i].m0 = 0;
    }

    spuAllocatedBlocks[0].m0 = 0x81;
    spuAllocatedBlocks[0].m1 = 5;
    spuAllocatedBlocks[0].m4 = 0;
    spuAllocatedBlocks[0].m8_adpcmSize = 0x1010;
    spuAllocatedBlocks[0].m2 = 0;
}

int findFreeSlotInLoadedSamples(void)
{
    for (int i = 0; i < 12; i++) {
        if (spuAllocatedBlocks[i].m0 == 0) {
            return i;
        }
    }
    return 0;
}

int allocateSPUSpaceForSamples(int sampleSizes, int param_2)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    auto psVar5 = spuAllocatedBlocks.begin();
    iVar4 = 0;
    iVar3 = spuAllocatedBlocks[0].m4 + spuAllocatedBlocks[0].m8_adpcmSize;
    if (spuAllocatedBlocks[0].m4 < param_2) {
        do {
            iVar1 = (int)psVar5->m2;
            if (iVar1 == 0) {
                iVar4 = 0x80000 - iVar3;
                break;
            }
            iVar2 = spuAllocatedBlocks[iVar1].m4;
            if (param_2 + sampleSizes <= iVar2) {
                iVar4 = iVar2 - iVar3;
                break;
            }
            iVar3 = iVar2 + spuAllocatedBlocks[iVar1].m8_adpcmSize;
            psVar5 = spuAllocatedBlocks.begin() + iVar1;
        } while (iVar2 < param_2);
    }
    iVar1 = 0;
    if ((sampleSizes <= iVar4) && (iVar1 = 0, iVar3 <= param_2)) {
        iVar3 = findFreeSlotInLoadedSamples();
        if (iVar3 < 0) {
            iVar1 = 0;
        }
        else {
            spuAllocatedBlocks[iVar3].m0 = 0x80;
            spuAllocatedBlocks[iVar3].m1 = 0;
            spuAllocatedBlocks[iVar3].m4 = param_2;
            spuAllocatedBlocks[iVar3].m8_adpcmSize = sampleSizes;
            spuAllocatedBlocks[iVar3].m2 = psVar5->m2;
            psVar5->m2 = (short)iVar3;
            iVar1 = param_2;
        }
    }
    return iVar1;
}

int getSamplePtr(sWdsFile* param_1, s32 param_2) {
    if (param_2 == 0) {
        param_2 = param_1->m28_adpcmAddress;
    }
    else if (param_2 == -1) {
        param_2 = 0;
    }
    if (param_2 == 0) {
        assert(0);
    }
    else {
        return allocateSPUSpaceForSamples(param_1->m14_ADPCMSize, param_1->m28_adpcmAddress);
    }
}

std::vector<sLoadWdsSub0SubVar0> loadWdsSub0SubVar1;

int loadWdsSub0SubVar0 = 0;
u16 spuDmaTransertEntry = 0;

bool loadWdsSub0SubSub(void)
{
    uint uVar1;

    uVar1 = (uint)loadWdsSub0SubVar0;
    if (uVar1 < spuDmaTransertEntry) {
        uVar1 = uVar1 + 8;
    }
    return 5 < (int)((uVar1 & 0xffff) - (uint)spuDmaTransertEntry);
}

void spuDmaCompletionCallback(void) {
    musicStatusFlag = musicStatusFlag | 4;
    if (loadWdsSub0SubVar1[spuDmaTransertEntry].m10_callback) {
        loadWdsSub0SubVar1[spuDmaTransertEntry].m10_callback();
    }
    musicStatusFlag = musicStatusFlag & 0xffef;
    if (spuDmaTransertEntry != loadWdsSub0SubVar0) {
        startTransfert();
    }
    musicStatusFlag = musicStatusFlag & 0xfffb;
}

void startTransfert() {
    spuDmaTransertEntry = spuDmaTransertEntry + 1;
    if (7 < spuDmaTransertEntry) {
        spuDmaTransertEntry = 0;
    }

    musicStatusFlag |= 0x10;

    sLoadWdsSub0SubVar0* pEntry = &loadWdsSub0SubVar1[spuDmaTransertEntry];
    auto previousCallback = SpuSetTransferCallback(spuDmaCompletionCallback);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(pEntry->m8_spuAddress);
    switch (pEntry->m0) {
    case 1:
        SpuWrite(pEntry->m4_size, pEntry->mC_size);
        break;
    case 2:
        SpuRead(pEntry->m4_size, pEntry->mC_size);
        break;
    default:
        assert(0);
    }
    if (previousCallback != spuDmaCompletionCallback) {
        setSoundError(0x26);
    }

    // HACK: clear the dma transfer in progress indicator so we don't defer the next bank upload
    musicStatusFlag &= ~0x10;
}

void loadWdsSub0Sub(int destSpuAddress, std::vector<u8>::iterator data, int adpcmSize, void(*param_4)(), int param_5) {
    int iVar2;
    u16 originalMusicStatusFlag = musicStatusFlag;
    if ((musicStatusFlag & 4) == 0) {
        do {
            iVar2 = loadWdsSub0SubSub();
        } while (iVar2 != 0);
        EnterCriticalSection();
    }
    loadWdsSub0SubVar0++;
    if (7 < loadWdsSub0SubVar0) {
        loadWdsSub0SubVar0 = 0;
    }
    sLoadWdsSub0SubVar0* psVar3 = &loadWdsSub0SubVar1[loadWdsSub0SubVar0];
    psVar3->m0 = param_5 & 0xf;
    psVar3->m2 = 0;
    psVar3->m4_size = data;
    psVar3->m8_spuAddress = destSpuAddress & 0x7fff8;
    psVar3->mC_size = adpcmSize;
    psVar3->m10_callback = param_4;
    if ((musicStatusFlag & 0x10) == 0) {
        startTransfert();
    }
    if ((originalMusicStatusFlag & 4) == 0) {
        ExitCriticalSection();
    }
}

void loadWdsSub0(int sampleEntry, std::vector<u8>::iterator data, int adpcmSize, void(*param_4)()) {
    loadWdsSub0Sub(sampleEntry, data, adpcmSize, param_4, 1);
}

sWdsFile* pLoadedWdsLinkedList = nullptr;

void freeBackToSoundArena(sWdsFile* pFile) {
    delete pFile; // TODO: implement sound arena
}


int freeSamples(int param_1) {
    structsLoadedSample* psVar1;
    structsLoadedSample* psVar2;
    structsLoadedSample* psVar3;

    psVar1 = spuAllocatedBlocks.data();
    psVar3 = (structsLoadedSample*)0x0;
    while (true) {
        psVar2 = psVar1;
        if (psVar2->m4 == param_1) {
            psVar3->m2 = psVar2->m2;
            psVar2->m0 = 0;
            psVar2->m1 = 0;
            psVar2->m4 = 0;
            psVar2->m2 = 0;
            return param_1;
        }
        if (psVar2->m2 == 0) break;
        psVar1 = spuAllocatedBlocks.data() + psVar2->m2;
        psVar3 = psVar2;
    }
    return 0;
}

void unloadWds(sWdsFile* pWdsFiled) {
    if (pLoadedWdsLinkedList) {
        sWdsFile* pCurrent = pLoadedWdsLinkedList;
        sWdsFile* pPrevious = nullptr;
        do {
            sWdsFile* psVar1 = pCurrent;
            pCurrent = psVar1;
            if (psVar1 == pWdsFiled)
                break;
            pCurrent = psVar1->m2C_pNext;
            pPrevious = psVar1;
        } while (pCurrent != (sWdsFile*)0x0);

        if (pCurrent != (sWdsFile*)0x0) {
            DisableEvent(audioTickEvent);
            if (pPrevious == (sWdsFile*)0x0) {
                pLoadedWdsLinkedList = pWdsFiled->m2C_pNext;
            }
            else {
                pPrevious->m2C_pNext = pWdsFiled->m2C_pNext;
            }
            EnableEvent(audioTickEvent);
            int iVar3 = freeSamples(pWdsFiled->m28_adpcmAddress);
            if (pWdsFiled->m28_adpcmAddress != iVar3) {
                setSoundError(0x24);
            }
            freeBackToSoundArena(pWdsFiled);
        }
    }
    else {
        setSoundError(0x11);
    }
}

sWdsFile* loadWds(sWdsFile& wdsFile, s32 param_2) {
    int samplePtrToLoad = getSamplePtr(&wdsFile, param_2);
    if (samplePtrToLoad == 0) {
        setSoundError(0x1F);
        return nullptr;
    }

    loadWdsSub0(samplePtrToLoad, wdsFile.m_rawData.begin() + wdsFile.m18_headerSize2 , wdsFile.m14_ADPCMSize, nullptr);

    sWdsFile* wdsCopy = new sWdsFile; // TODO: this was allocated from the sound arena
    wdsCopy->init(wdsFile.m_rawData.begin(), wdsFile.m10_headerSize1);
    wdsCopy->m28_adpcmAddress = samplePtrToLoad;

    DisableEvent(audioTickEvent);
    sWdsFile** ppWdsList = &pLoadedWdsLinkedList;
    while (*ppWdsList) {
        ppWdsList = &(*ppWdsList)->m2C_pNext;
    }
    *ppWdsList = wdsCopy;
    wdsCopy->m2C_pNext = nullptr;

    EnableEvent(audioTickEvent);
    return wdsCopy;
}

sWdsFile* findInWdsLinkedList(u32 bankId) {
    sWdsFile* psVar1;

    psVar1 = pLoadedWdsLinkedList;
    if (pLoadedWdsLinkedList != nullptr) {
        do {
            if (psVar1->m20_bankNumber == bankId) {
                return psVar1;
            }
            psVar1 = psVar1->m2C_pNext;
        } while (psVar1 != nullptr);
    }
    return psVar1;
}

void setupSamplesPtr(int, int) {
    MissingCode();
}

void sendSamples2(std::vector<u8>::iterator, int) {
    MissingCode();
}

sWdsFile* playMusic2(sWdsFile* wdsFile, int param_2, int param_3) {
    sWdsFile* psVar2 = findInWdsLinkedList(wdsFile->m20_bankNumber);
    if (psVar2 == nullptr) {
        if (int samplePtrToLoad = getSamplePtr(wdsFile, param_3)) {
            //setupSamplesPtr(samplePtr, wdsFile->m14_ADPCMSize);
            //sendSamples2(wdsFile->m_rawData.begin() + wdsFile->m18_headerSize2, param_2 - wdsFile->m10_headerSize1);

            loadWdsSub0(samplePtrToLoad, wdsFile->m_rawData.begin() + wdsFile->m18_headerSize2, wdsFile->m14_ADPCMSize, nullptr);

            sWdsFile* wdsCopy = new sWdsFile; // TODO: this was allocated from the sound arena

            if (wdsCopy) {
                wdsCopy->init(wdsFile->m_rawData.begin(), wdsFile->m10_headerSize1);
                wdsCopy->m28_adpcmAddress = samplePtrToLoad;
                DisableEvent(audioTickEvent);

                auto ppsVar5 = &pLoadedWdsLinkedList;
                for (auto psVar3 = pLoadedWdsLinkedList; psVar3 != nullptr; psVar3 = psVar3->m2C_pNext) {
                    psVar3 = *ppsVar5;
                    ppsVar5 = &psVar3->m2C_pNext;
                }
                *ppsVar5 = wdsCopy;
                wdsCopy->m2C_pNext = nullptr;
                EnableEvent(audioTickEvent);
                return wdsCopy;
            }
            assert(0);
        }
        setSoundError(0x1F);
        return nullptr;
    }
    setSoundError(0x16);
    return nullptr;
}
