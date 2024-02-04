#pragma once

#include "kernel/filesystem.h"

struct sWdsFile : sLoadableData {
    void init(std::vector<u8>::iterator it, int size) {
        std::vector<u8> temp;
        temp.resize(size);
        for (int i = 0; i < size; i++) {
            temp[i] = it[i];
        }
        init(temp);
    }
    virtual void init(std::vector<u8>& data) override {
        m_rawData = data;

        auto it = data.begin();

        m10_headerSize1 = READ_LE_U32(it + 0x10);
        m14_ADPCMSize = READ_LE_U32(it + 0x14);
        m18_headerSize2 = READ_LE_U32(it + 0x18);
        m20_bankNumber = READ_LE_U16(it + 0x20);
        m28 = READ_LE_U32(it + 0x28);
        assert(READ_LE_U32(it + 0x2C) == 0);
        m2C_pNext = nullptr;
        
    }

    s32 m10_headerSize1;
    s32 m14_ADPCMSize;
    s32 m18_headerSize2;
    u16 m20_bankNumber;
    s32 m28;
    sWdsFile* m2C_pNext;

    std::vector<u8> m_rawData;
};

struct sLoadWdsSub0SubVar0 {
    u16 m0;
    u16 m2;
    std::vector<u8>::iterator m4;
    u32 m8;
    u32 mC;
    void (*m10)();
};

extern std::vector<sLoadWdsSub0SubVar0> loadWdsSub0SubVar1;
extern sWdsFile* pLoadedWdsLinkedList;

void spuDmaCompletionCallback(void);
void initLoadedSampleTable();
sWdsFile* loadWds(sWdsFile& wdsFile, s32 param_2);
sWdsFile* findInWdsLinkedList(u32 bankId);