#pragma once

#include "kernel/filesystem.h"

struct sFieldEntitySub4_110_0_frame {
    void init(std::span<u8>::iterator data) {
        rawPointer = data;
    }

    sPS1Pointer rawPointer;
};

struct sFieldEntitySub4_110_0 {
    void init(std::span<u8>::iterator data) {
        rawPointer = data;

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
        m_rawData = inputData;
        std::span<u8> dataSpan(m_rawData.begin(), m_rawData.size());
        init(dataSpan.begin());
    }

    void init(std::span<u8>::iterator inputData)
    {
        rawData = inputData;

        m0_numEntries = READ_LE_U32(inputData + 0);
        assert(m0_numEntries >= 3);

        m4_entries.reserve(m0_numEntries);
        for (int i = 0; i < m0_numEntries; i++) {
            u32 start = READ_LE_U32(inputData + 4 + i * 4);
            u32 end = READ_LE_U32(inputData + 4 + (i+1) * 4);
            m4_entries.push_back(std::span<u8>(inputData + start, end - start));
        }

        {
            auto& bundle = m4_entries[0];
            auto animationIt = bundle.begin();
            u16 header = READ_LE_U16(animationIt);
            u16 numAnimations = header & 0x3F;
            u16 unk = (header >> 6) & 0x3F; // not sure what that is yet
            m4_animations.reserve(numAnimations);
            for (int i=0; i< numAnimations; i++)
            {
                u16 offset = READ_LE_U16(animationIt + 2 + i * 2);
                int length = bundle.size() - offset; //TODO: do we need the +1?
                if(length > 0)
                {
                    std::span<u8>::iterator start = animationIt + offset;
                    // this doesn't quite work because animations are not in order. Could post-process after load and shorten the spans
                    /*if (i != numAnimations - 1) {
                        length = READ_LE_U16(animationIt + 2 + (i + 1) * 2) - READ_LE_U16(animationIt + 2 + i * 2);
                    }*/
                    m4_animations.push_back(std::span<u8>(start, length));
                }
                else {
                    assert(i == numAnimations - 1);
                }
            }
        }
        m8_pData.init(inputData + READ_LE_U32(inputData + 8));
        mC_pData = m4_entries[2];
    }

    std::vector<u8> m_rawData;
    std::span<u8>::iterator rawData;

    u32 m0_numEntries;
    std::vector<std::span<u8>> m4_entries;

    std::vector<std::span<u8>> m4_animations;
    sFieldEntitySub4_110_0 m8_pData;
    std::span<u8> mC_pData;

};

extern std::vector<sSpriteActorAnimationBundle> fieldActorSetupParams;

