#pragma once

struct sSpriteActorAnimationBundle
{
    void init(std::vector<u8>::iterator& inputData)
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
        m8_pData.setPointer(&(*(inputData + m8_offset)));

        mC_offset = READ_LE_U32(inputData + 0xC);
        mC_pData.setPointer(&(*(inputData + mC_offset)));

        u32 end = READ_LE_U32(inputData + 0x10);
    }

    u8* rawData;

    u32 m4_offset;
    sPS1Pointer m4_pData;
    std::vector<sPS1Pointer> m4_animations;
    u32 m8_offset;
    sPS1Pointer m8_pData;
    u32 mC_offset;
    sPS1Pointer mC_pData;

};

extern std::vector<sSpriteActorAnimationBundle> fieldActorSetupParams;

