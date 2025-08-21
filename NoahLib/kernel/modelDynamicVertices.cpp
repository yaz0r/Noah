#include "noahLib.h"
#include "modelDynamicVertices.h"
#include "3dModel.h"
#include "memory.h"

int dynamicVerticesDefaultUpdateFunction(sDynamicVerticesData* pData) {
    int iVar1 = pData->m8;
    if (pData->m4 < iVar1) {
        iVar1 = iVar1 - pData->mC;
        pData->m8 = iVar1;
        if (pData->m4 <= iVar1)
            return pData->m8;
        pData->m8 = pData->m4;
        iVar1 = pData->m8;
    }
    if (iVar1 < pData->m4) {
        iVar1 = iVar1 + pData->mC;
        pData->m8 = iVar1;
        if (pData->m4 < iVar1) {
            pData->m8 = pData->m4;
        }
    }

    return pData->m8;
}

sDynamicVertices* initModelDynamicVertices(sModelBlock* pModelBlock)
{
    if (pModelBlock->m1C_dynamicVertices == nullptr)
        return nullptr;

    int numDynamicVertices = pModelBlock->m1C_dynamicVertices->m0_count;

    traceNextAlloc(0x2b);

    sDynamicVertices* pNewDynamicVertices = new sDynamicVertices;
    pNewDynamicVertices->m0_modelBlock = pModelBlock;

    traceNextAlloc(0x2c);
    pNewDynamicVertices->m4_vertices = pModelBlock->m8_vertices; // this technically just move the pointers over and change ownership
    pNewDynamicVertices->m8_normals = pModelBlock->mC_normals;
    pNewDynamicVertices->mC_numDynamicVertices = numDynamicVertices;
    pNewDynamicVertices->m10_perVerticeData.resize(numDynamicVertices);

    // This is a bit weird and useless. In the original code sDynamicVertices takes the ownership of the modelblock vertices/normals buffers
    // and then create a new one in the sModelBlock, copying back the data
    for (int i = 0; i < pModelBlock->m2_numVertices; i++) {
        pModelBlock->m8_vertices[i] = pNewDynamicVertices->m4_vertices[i];
    }
    if (pModelBlock->m0_flags & 0x10) {// has normals?
        traceNextAlloc(0x2D);

        for (int i = 0; i < pModelBlock->m2_numVertices; i++) {
            pModelBlock->mC_normals[i] = pNewDynamicVertices->m8_normals[i];
        }
    }

    for (int i = 0; i < pNewDynamicVertices->mC_numDynamicVertices; i++) {
        pNewDynamicVertices->m10_perVerticeData[i].m0_getScaleFunction = dynamicVerticesDefaultUpdateFunction;
        pNewDynamicVertices->m10_perVerticeData[i].m4 = 0;
        pNewDynamicVertices->m10_perVerticeData[i].m8 = 0;
        pNewDynamicVertices->m10_perVerticeData[i].mC = 0;
    }

    return pNewDynamicVertices;
}

void dynamicVertices_resertVertices(std::vector<SVECTOR>& vertices, std::vector<SVECTOR>& pDynamicVertices, int count, std::vector<u16>& indices) {
    for (int i = 0; i < count; i++) {
        vertices[indices[i]] = pDynamicVertices[indices[i]];
    }
}

void dynamicVertices_updateVertices(std::vector<SVECTOR>& vertices, std::vector<sModelBlock::sModelDynamicVertices::sData::sOffsetData>& offsets, int count, int scale) {
    if (scale) {
        for (int i = 0; i < count; i++) {
            vertices[offsets[i].m6_index].vx += (offsets[i].m0_offset[0] * scale) >> 0xC;
            vertices[offsets[i].m6_index].vy += (offsets[i].m0_offset[1] * scale) >> 0xC;
            vertices[offsets[i].m6_index].vz += (offsets[i].m0_offset[2] * scale) >> 0xC;
        }
    }
}

void updateDynamicVertices(sDynamicVertices* pDynamicVertices) {
    if (pDynamicVertices == nullptr)
        return;

    sModelBlock* pModel = pDynamicVertices->m0_modelBlock;
    s32 numDynamicVertices = pDynamicVertices->mC_numDynamicVertices;
    auto& pData = pModel->m1C_dynamicVertices->m4_data;

    dynamicVertices_resertVertices(pModel->m8_vertices, pDynamicVertices->m4_vertices, pModel->m1C_dynamicVertices->m_indexCount, pModel->m1C_dynamicVertices->m_indices);

    for (int i = 0; i < pDynamicVertices->mC_numDynamicVertices; i++) {
        auto* perVerticesData = &pDynamicVertices->m10_perVerticeData[i];
        int scale = perVerticesData->m0_getScaleFunction(perVerticesData);

        dynamicVertices_updateVertices(pModel->m8_vertices, pModel->m1C_dynamicVertices->m4_data[i].m4_positionOffsets, pModel->m1C_dynamicVertices->m4_data[i].m0_count, scale);

        if ((pModel->m0_flags & 0x10) != 0) {
            assert(0);
        }
    }
}