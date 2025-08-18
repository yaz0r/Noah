#pragma once

#include "kernel/filesystem.h"

struct sSeqFile : sLoadableDataRaw {
    void init(std::vector<u8>::const_iterator it, size_t size) {
        std::vector<u8> temp;
        temp.resize(size);
        for (int i = 0; i < size; i++) {
            temp[i] = it[i];
        }
        init(temp);
    }
    void init(std::span<u8>::iterator it, size_t size) {
        std::vector<u8> temp;
        temp.resize(size);
        for (int i = 0; i < size; i++) {
            temp[i] = it[i];
        }
        init(temp);
    }

    virtual void init(const std::vector<u8>& data) override {
        sLoadableDataRaw::init(data);
        auto it = data.begin();

        m0_magic = READ_LE_U32(it + 0x0);
        m8 = READ_LE_U32(it + 0x8);
        mC_checksum = READ_LE_U32(it + 0xC);
        m10 = READ_LE_U16(it + 0x10);
        m14 = READ_LE_U16(it + 0x14);
        m16_sampleBank = READ_LE_U16(it + 0x16);
        m18 = READ_LE_U16(it + 0x18);
        m1A = READ_LE_S8(it + 0x1A);
        m1B = READ_LE_U8(it + 0x1B);
        m1C_Original = READ_LE_U8(it + 0x1C);
        m1D_Original = READ_LE_U8(it + 0x1D);
        m20_drumKitOffset = READ_LE_U16(it + 0x20);
        m1C_pNext = nullptr;
    }

    u32 m0_magic;
    u32 m8;
    u32 mC_checksum;
    u16 m10;
    u16 m14;
    u16 m16_sampleBank;
    u16 m18;
    s8 m1A;
    u8 m1B;

    u8 m1C_Original;
    u8 m1D_Original;
    sSeqFile* m1C_pNext;

    u16 m20_drumKitOffset;
};

extern sSeqFile* pLoadedSequencesLinkedList;

void loadSequence(sSeqFile* param_1);
void stopSequence(sSeqFile* param_1);
void unloadSequence(sSeqFile* param_1);

void playSoundEffect(u32 param_1);