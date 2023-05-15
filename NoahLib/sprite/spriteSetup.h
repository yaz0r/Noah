#pragma once

#include "kernel/filesystem.h"

struct sFieldEntitySub4_110_0_frame {
    void init(std::vector<u8>::iterator data) {
        rawPointer.setPointer(&data[0]);
    }

    sPS1Pointer rawPointer;
};

struct sFieldEntitySub4_110_0 {
    void init(std::vector<u8>::iterator data) {
        rawPointer.setPointer(&data[0]);

        m0_header.rawValue = READ_LE_U16(data);

        m4_frames.resize(m0_header.mx01FF_frameCount + 1);
        for (int i = 1; i < m0_header.mx01FF_frameCount + 1; i++) {
            m4_frames[i].init(data + READ_LE_U16(data + 2 * i));
        }
    }
    
    union {
        struct {
            u16 mx01FF_frameCount : 9;
            u16 m_0x7E00 : 6;
            u16 mx8000_isVramPrebacked : 1;
        };
        u16 rawValue;
    }m0_header;

    std::vector<sFieldEntitySub4_110_0_frame> m4_frames;

    sPS1Pointer rawPointer;
};

struct sSpriteActorAnimationBundle : public sLoadableData
{
    virtual void init(std::vector<u8>& inputData) override {
        init(inputData.begin());
    }

    void init(std::vector<u8>::iterator inputData)
    {
        rawData = &inputData[0];

        m4_offset = READ_LE_U32(inputData + 4);
        m4_pData.setPointer(&(*(inputData + m4_offset)));

        {
            m4_animations.resize(READ_LE_U16(m4_pData) & 0x3F);
            for (int i=0; i< m4_animations.size(); i++)
            {
                m4_animations[i] = m4_pData + READ_LE_U16(m4_pData + 2 + i * 2);
            }
        }

        m8_offset = READ_LE_U32(inputData + 8);
        m8_pData.init(inputData + m8_offset);

        mC_offset = READ_LE_U32(inputData + 0xC);
        mC_pData.setPointer(&(*(inputData + mC_offset)));

        u32 end = READ_LE_U32(inputData + 0x10);
    }

    u8* rawData;

    u32 m4_offset;
    sPS1Pointer m4_pData;
    std::vector<sPS1Pointer> m4_animations;
    u32 m8_offset;
    sFieldEntitySub4_110_0 m8_pData;
    u32 mC_offset;
    sPS1Pointer mC_pData;

};

extern std::vector<sSpriteActorAnimationBundle> fieldActorSetupParams;

