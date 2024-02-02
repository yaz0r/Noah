#pragma once

#include "kernel/filesystem.h"
#include "field/mecha/mechaOverlay.h"

struct sBattleSpriteConfigsSub {
    void init(std::span<u8>::iterator data) {
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

    sMechaDataTable2 m_mechaData2;
    sMechaDataTable1 m_mechaData1;
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
            m8[i].init(beginSpan() + 8 + i * 0xC);
        }

        for (int i = 0; i < m0_numEntities; i++) {
            if (m8[i].m8_isMecha) {
                if(m8[i].m4_spriteDataOffset > 7) {
                    m8[i].m_mechaData2.init(begin() + m8[i].m0_spriteControlOffset);
                    m8[i].m_mechaData1.init(begin() + m8[i].m4_spriteDataOffset);
                }
            }
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
void defaultBattleSpriteDeleteCallback(struct sTaskHeader* param_1);

template <typename T>
T* battleLoaderAllocateMainBattleSprite(sTaskHeader* param_1, int param_2)
{
    T* psVar1;

    psVar1 = new T;
    allocateSavePointMeshDataSub0(param_1, psVar1);
    psVar1->mC_deleteCallback = defaultBattleSpriteDeleteCallback;
    psVar1->m4 = nullptr;
    return psVar1;
}

extern bool battleScriptLoaderTaskCreationInProgress;
extern int mainBattleSpriteCallback_phase5Var;