#pragma once

#include "kernel/filesystem.h"

struct sBattleSpriteConfigsSub {
    void init(std::vector<u8>::iterator data) {
        m0_spriteControlOffset = READ_LE_U32(data + 0);
        m4_spriteDataOffset = READ_LE_U32(data + 4);
        m8_isMecha = READ_LE_U8(data + 8);
        mB = READ_LE_U8(data + 0xB);
    }

    u32 m0_spriteControlOffset;
    u32 m4_spriteDataOffset;
    u8 m8_isMecha;
    u8 mB;

    // size 0xC
};

struct sBattleSpriteConfigs : public sLoadableDataRaw {
    void init(std::vector<u8>& data) {
        sLoadableDataRaw::init(data);

        m0_numEntities = READ_LE_S8(begin() + 0);
        m1 = READ_LE_S8(begin() + 1);
        m4_bufferSize = READ_LE_U32(begin() + 4);

        int headerSize = m0_numEntities * 0xC + 8;

        m8.resize(m0_numEntities);
        for (int i = 0; i < m0_numEntities; i++) {
            m8[i].init(begin() + 8 + i * 0xC);
        }

        m_spriteData.insert(m_spriteData.begin(), begin() + headerSize, begin() + m4_bufferSize - headerSize);
    }

    u8 m0_numEntities;
    u8 m1;
    u32 m4_bufferSize;
    std::vector<sBattleSpriteConfigsSub> m8;
    std::vector<u8> m_spriteData;
};
extern sBattleSpriteConfigs battleConfigFile3;

void createBattleSpriteLoadingTask(sBattleSpriteConfigs* param_1);