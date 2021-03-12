#pragma once

struct sFieldActorSetupParams
{
    void init(std::vector<u8>::iterator& inputData)
    {
        rawData = &inputData[0];

        m4_offset = READ_LE_U32(inputData + 4);
        m4_pData.setPointer(&(*(inputData + m4_offset)));

        m8_offset = READ_LE_U32(inputData + 8);
        m8_pData.setPointer(&(*(inputData + m8_offset)));

        mC_offset = READ_LE_U32(inputData + 0xC);
        mC_pData.setPointer(&(*(inputData + mC_offset)));

        u32 end = READ_LE_U32(inputData + 0x10);
    }

    u8* rawData;

    u32 m4_offset;
    sPS1Pointer m4_pData;
    u32 m8_offset;
    sPS1Pointer m8_pData;
    u32 mC_offset;
    sPS1Pointer mC_pData;

};

extern std::vector<sFieldActorSetupParams> fieldActorSetupParams;

