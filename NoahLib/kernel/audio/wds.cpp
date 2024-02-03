#include "noahLib.h"
#include "wds.h"
#include "soundSystem.h"
#include "kernel/criticalSection.h"
#include "spu.h"

struct structsLoadedSample {
    s8 m0;
    s8 m1;
    s16 m2;
    s32 m4;
    s32 m8_adpcmSize;
};

std::array<structsLoadedSample, 0xC> loadedSamples;

void startTransfert();

void initLoadedSampleTable() {
    for (int i = 0; i < 0xC; i++) {
        loadedSamples[i].m0 = 0;
    }

    loadedSamples[0].m0 = 0x81;
    loadedSamples[0].m1 = 5;
    loadedSamples[0].m4 = 0;
    loadedSamples[0].m8_adpcmSize = 0x1010;
    loadedSamples[0].m2 = 0;
}

int findFreeSlotInLoadedSamples(void)
{
    int iVar1;

    iVar1 = 0;
    do {
        if (loadedSamples[iVar1].m0){
            return iVar1;
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0xc);
    return 0;
}

int getSamplesPtrSub(int param_1, int param_2)

{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    auto psVar5 = loadedSamples.begin();
    iVar4 = 0;
    iVar3 = loadedSamples[0].m4 + loadedSamples[0].m8_adpcmSize;
    if (loadedSamples[0].m4 < param_2) {
        do {
            iVar1 = (int)psVar5->m2;
            if (iVar1 == 0) {
                iVar4 = 0x80000 - iVar3;
                break;
            }
            iVar2 = loadedSamples[iVar1].m4;
            if (param_2 + param_1 <= iVar2) {
                iVar4 = iVar2 - iVar3;
                break;
            }
            iVar3 = iVar2 + loadedSamples[iVar1].m8_adpcmSize;
            psVar5 = loadedSamples.begin() + iVar1;
        } while (iVar2 < param_2);
    }
    iVar1 = 0;
    if ((param_1 <= iVar4) && (iVar1 = 0, iVar3 <= param_2)) {
        iVar3 = findFreeSlotInLoadedSamples();
        if (iVar3 < 0) {
            iVar1 = 0;
        }
        else {
            loadedSamples[iVar3].m0 = 0x80;
            loadedSamples[iVar3].m1 = 0;
            loadedSamples[iVar3].m4 = param_2;
            loadedSamples[iVar3].m8_adpcmSize = param_1;
            loadedSamples[iVar3].m2 = psVar5->m2;
            psVar5->m2 = (short)iVar3;
            iVar1 = param_2;
        }
    }
    return iVar1;
}

int getSamplePtr(sWdsFile* param_1, s32 param_2) {
    if (param_2 == 0) {
        param_2 = param_1->m28;
    }
    else if (param_2 == -1) {
        param_2 = 0;
    }
    if (param_2 == 0) {
        assert(0);
    }
    else {
        return getSamplesPtrSub(param_1->m14_ADPCMSize, param_1->m28);
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
    if (loadWdsSub0SubVar1[spuDmaTransertEntry].m10) {
        loadWdsSub0SubVar1[spuDmaTransertEntry].m10();
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
    SpuSetTransferStartAddr(pEntry->m8);
    switch (pEntry->m0) {
    case 1:
        SpuWrite(pEntry->m4, pEntry->mC);
        break;
    case 2:
        SpuRead(pEntry->m4, pEntry->mC);
        break;
    default:
        assert(0);
    }
    if (previousCallback != spuDmaCompletionCallback) {
        setSoundError(0x26);
    }
}

void loadWdsSub0Sub(int sampleEntry, std::vector<u8>::iterator data, int adpcmSize, void(*param_4)(), int param_5) {
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
    psVar3->m4 = data;
    psVar3->m8 = sampleEntry & 0x7fff8;
    psVar3->mC = adpcmSize;
    psVar3->m10 = param_4;
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

sWdsFile* loadWds(sWdsFile& wdsFile, s32 param_2) {
    int sampleEntry = getSamplePtr(&wdsFile, param_2);
    if (sampleEntry == 0) {
        setSoundError(0x1F);
        return nullptr;
    }

    loadWdsSub0(sampleEntry, wdsFile.m_rawData.begin() + wdsFile.m18_headerSize2 , wdsFile.m14_ADPCMSize, nullptr);

    sWdsFile* wdsCopy = new sWdsFile; // TODO: this was allocated from the sound arena
    wdsCopy->init(wdsFile.m_rawData.begin(), wdsFile.m10_headerSize1);
    wdsCopy->m28 = sampleEntry;

    DisableEvent(spuEvent);
    sWdsFile** ppWdsList = &pLoadedWdsLinkedList;
    while (*ppWdsList) {
        ppWdsList = &(*ppWdsList)->m2C_pNext;
    }
    *ppWdsList = wdsCopy;
    wdsCopy->m2C_pNext = nullptr;

    EnableEvent(spuEvent);
    return wdsCopy;
}
