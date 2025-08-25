#pragma once

struct sModelBlock
{
    ~sModelBlock();
    void init(const u8* input, u8* baseItForRelocation);

    struct sMeshBlock
    {
        u8 m0_primType;
        u8 m1;
        u16 m2_primCount;
        std::vector<std::array<u16, 4>> m4_indices;
        // size 0x8
    };

    u16 m0_flags;
    s16 m2_numVertices;
    s16 m4_numPrims;
    s16 m6_numMeshBlock;
    std::vector<SVECTOR> m8_vertices;
    std::vector<SVECTOR> mC_normals;
    u32 m10_offsetMeshBlocks;
    std::vector<sMeshBlock> m10_meshBlocks;
    u32 m14_offsetDisplayList;
    std::vector<u8> m18;
    struct sModelDynamicVertices {
        u32 m0_count = 0;
        struct sData {
            s32 m0_count;
            struct sOffsetData {
                std::array<s16, 3> m0_offset;
                u16 m6_index;
            };
            std::vector<sOffsetData> m4_positionOffsets;
            std::vector<sOffsetData> m8_normalOffsets;
        };
        std::vector<sData> m4_data;
        s32 m_indexCount;
        std::vector<u16> m_indices;
    }* m1C_dynamicVertices = nullptr;
    s16 m20;
    s16 m22;
    s16 m24;
    s16 m28;
    s16 m2A;
    s16 m2C;
    int m30;
    int m34_instanceBufferSize;
    // size 0x38

    u8* m_baseItForRelocation;

    class sBgfxRendererData* getBgfxData();

private:
    class sBgfxRendererData* m_pBgfxRendererData = nullptr;
};

struct sModel
{
    void init(std::vector<u8>::const_iterator inputData, int dataSize);

    u32 m0_numBlocks = 0;
    std::vector<sModelBlock> m10_blocks;

    std::vector<u8> mRawData;
};

int isObjectClipped(sModelBlock* param_1, uint param_2);
