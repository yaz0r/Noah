#pragma once

struct sModelBlock;

struct sDynamicVerticesData {
    int (*m0_getScaleFunction)(sDynamicVerticesData*);
    s32 m4;
    s32 m8;
    s32 mC;
    // size 0x20
};
struct sDynamicVertices {
    sModelBlock* m0_modelBlock;
    std::vector<SVECTOR> m4_vertices;
    std::vector<SVECTOR> m8_normals;
    int mC_numDynamicVertices;
    std::vector<sDynamicVerticesData> m10_perVerticeData;
    // Size 0x14?
};

sDynamicVertices* initModelDynamicVertices(sModelBlock* pModelBlock);
void updateDynamicVertices(sDynamicVertices* pDynamicVertices);