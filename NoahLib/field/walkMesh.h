#pragma once

#include <vector>
#include <array>

struct sWalkMesh
{
    u32 m0_count;

    struct sTriangleData
    {
        std::array<s16, 3> m0_verticeIndex;
        std::array<s16, 3> m6_connectivity;
        u16 mC_flags;
    };

    struct sBlock
    {
        std::vector<sTriangleData> m_triangles;
        std::vector<std::array<s16, 3>> m_vertices;
    };

    std::vector<sBlock> m_blocks;

    void init(const std::vector<u8>& input);
    void clear()
    {
        m0_count = 0;
        m_blocks.clear();
    }

    void bgfxRender(int viewIndex);
};

extern sWalkMesh walkMesh;
