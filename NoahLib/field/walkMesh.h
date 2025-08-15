#pragma once

#include "kernel/filesystem.h"
struct sWalkMeshBundle : public sLoadableDataRaw
{
    virtual ~sWalkMeshBundle() {}

    struct sTriangleData
    {
        std::array<s16, 3> m0_verticeIndex;
        std::array<s16, 3> m6_connectivity;
        u8 mC_indexInWalkmeshData1;
        u8 mD;
    };

    struct sWalkMesh
    {
        std::vector<sTriangleData> m_triangles;
        std::vector<SVECTOR> m_vertices;
    };

    u32 m0_numWalkMeshes;
    std::vector<sWalkMesh> m_walkMeshes;
    std::vector<u32> m_materials;

    virtual void init(const std::vector<u8>& input) override;
    virtual void clear() override
    {
        sLoadableDataRaw::clear();
        m0_numWalkMeshes = 0;
        m_walkMeshes.clear();
    }

    void bgfxRender(int viewIndex);
};

extern sWalkMeshBundle g_walkMesh;

s16 findTriangleInWalkMesh(int posX, int posZ, int walkmeshId, SVECTOR* param_4, VECTOR* param_5);

extern std::vector<u32>* walkMeshVar1;
extern std::array<std::vector<sWalkMeshBundle::sTriangleData>*, 4> walkMeshTriangle;
extern std::array<std::vector<SVECTOR>*, 4> walkMeshVertices;
extern std::array<s32, 4> g_walkMeshNumTrianglePerBlock;
extern s16 g_numWalkMesh;
