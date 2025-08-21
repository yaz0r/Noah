#pragma once

#include "kernel/filesystem.h"
#include "kernel/audio/seq.h"

struct sFieldEntitySub4_110_0_frame {
    void init(std::span<u8>::iterator data) {
        rawPointer = data;
    }

    std::span<u8>::iterator rawPointer;
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

    std::span<u8>::iterator rawPointer;
};

struct sSpriteActorAnimationBundle : public sLoadableDataRaw
{
    virtual void init(const std::vector<u8>& inputData) override {
        sLoadableDataRaw::init(inputData);
        init(beginSpan());
    }

    void init(std::span<u8>::iterator inputData)
    {
        rawData = inputData;

        m0_numEntries = READ_LE_U32(inputData + 0);
        assert(m0_numEntries >= 3);

        m4_entries.reserve(m0_numEntries);
        for (u32 i = 0; i < m0_numEntries; i++) {
            u32 start = READ_LE_U32(inputData + 4 + i * 4);
            u32 end = READ_LE_U32(inputData + 4 + (i+1) * 4);
            m4_entries.push_back(std::span<u8>(inputData + start, end - start));
        }

        m4_animBundle.bundle = m4_entries[0];
#if 0
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
                /*if (offset > bundle.size()) {
                    assert(i == numAnimations - 1);
                    // sometimes, the last animation go out of bound. just ignore it (happens when selecting new game)
                }
                else*/ {
                    // iterators need to access fall range of animation bundle because it can be used to call sub-functions
                    m4_animations.push_back(animationIt + offset);
                }
                
            }
        }
#endif
        m8_pData.init(inputData + READ_LE_U32(inputData + 8));
        mC_pData = m4_entries[2];

        m4_entriesAsSequences.resize(m4_entries.size());
        for (u32 i = 3; i < m0_numEntries; i++) {
            u32 magic = READ_LE_U32(m4_entries[i].begin());
            if (magic == 'sdes') {
                m4_entriesAsSequences[i].init(m4_entries[i].begin(), m4_entries[i].size());
            }
        }
    }

    std::span<u8>::iterator rawData;

    u32 m0_numEntries;
    std::vector<std::span<u8>> m4_entries;

    struct sAnimationBundle {
        std::span<u8> bundle;
        std::span<u8>::iterator getAnimation(int index) {
            auto animationIt = bundle.begin();
            u16 offset = READ_LE_U16(animationIt + 2 + index * 2);
            return animationIt + offset;
        }
    } m4_animBundle;


    sFieldEntitySub4_110_0 m8_pData;
    std::span<u8> mC_pData;

    std::vector<sSeqFile> m4_entriesAsSequences;

};

extern std::vector<sSpriteActorAnimationBundle> fieldActorSetupParams;

