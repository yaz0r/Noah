#pragma once

#include "kernel/filesystem.h"
struct sWalkMesh : public sLoadableDataRaw
{
    virtual ~sWalkMesh() {}

    u32 m0_count;

    struct sTriangleData
    {
        std::array<s16, 3> m0_verticeIndex;
        std::array<s16, 3> m6_connectivity;
        u8 mC_indexInWalkmeshData1;
        u8 mD;
    };

    struct sBlock
    {
        std::vector<sTriangleData> m_triangles;
        std::vector<SVECTOR> m_vertices;
    };

    std::vector<sBlock> m_blocks;
    std::vector<u32> m_materials;

    virtual void init(const std::vector<u8>& input) override;
    virtual void clear() override
    {
        sLoadableDataRaw::clear();
        m0_count = 0;
        m_blocks.clear();
    }

    void bgfxRender(int viewIndex);
};

extern sWalkMesh walkMesh;
