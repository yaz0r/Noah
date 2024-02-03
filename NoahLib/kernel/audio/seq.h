#pragma once

#include "kernel/filesystem.h"

struct sSeqFile : sLoadableData {
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

        m0_magic = READ_LE_U32(it + 0x0);
        m8 = READ_LE_U32(it + 0x8);
        mC = READ_LE_U32(it + 0xC);
        assert(READ_LE_U32(it + 0x1C) == 0);
        m1C_pNext = nullptr;
    }

    u32 m0_magic;
    u32 m8;
    u32 mC;
    sSeqFile* m1C_pNext;

    std::vector<u8> m_rawData;
};

void loadSequence(sSeqFile& param_1);
