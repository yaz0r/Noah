#pragma once

#include "bgfx/bgfx.h"

struct sDrawCall
{
    bgfx::VertexBufferHandle m_vb;
    bgfx::IndexBufferHandle m_ib;
    bgfx::TextureHandle m_texture;
};

struct sModelBlock
{
    s16 m2_numVertices;
    s16 m4_numMesh;
    s16 m6_numMeshBlock;
    u32 m8_offsetVertices;
    u32 mC_offsetNormals;
    u32 m10_offsetMeshBlocks;
    u32 m14_offsetDisplayList;
    int m34_count;
    // size 0x38

    std::vector<std::array<s16, 3>> m_vertices;
    std::vector<std::array<s16, 3>> m_normals;

    struct sMeshBlock
    {
        u16 m0;
        u16 m2;
    };

    std::vector<sMeshBlock> m_meshBlocks;
    std::vector<sDrawCall> m_drawCalls;

    struct sModel* m_model;
    void bgfxRender(int viewIndex, float* modelMatrix);
    void buildDrawcall(int viewIndex);
};

struct sModel
{
    void init(std::vector<u8>::iterator& inputData, int dataSize);

    u32 m0_numBlocks;
    std::vector<sModelBlock> m10_blocks;

    std::vector<u8> mRawData;

    void bgfxRender(int viewIndex, float* modelMatrix);
};

extern std::vector<sModel> gCurrentFieldModels;

