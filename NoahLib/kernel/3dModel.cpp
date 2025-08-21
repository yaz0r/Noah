#include "noahLib.h"
#include "kernel/3dModel.h"
#include "kernel/3dModel_bgfxRenderer.h"
#include "kernel/3dModel_psxRenderer.h"
#include "kernel/memory.h"
#include "psx/libgpu.h"

sModelBlock::~sModelBlock() {
    delete m_pBgfxRendererData;
}

sBgfxRendererData* sModelBlock::getBgfxData() {
    if (m_pBgfxRendererData == nullptr) {
        m_pBgfxRendererData = new sBgfxRendererData;
    }
    return m_pBgfxRendererData;
}

void sModelBlock::init(const u8* blockData) {
    m0_flags = READ_LE_U16(blockData + 0);
    m2_numVertices = READ_LE_U16(blockData + 2);
    m4_numPrims = READ_LE_U16(blockData + 4);
    m6_numMeshBlock = READ_LE_U16(blockData + 6);
    int offsetVertices = READ_LE_U32(blockData + 8);
    m8_vertices.resize(m2_numVertices);
    for (int i = 0; i < m2_numVertices; i++) {
        m8_vertices[i][0] = READ_LE_S16(m_baseItForRelocation + offsetVertices + i * 8 + 0);
        m8_vertices[i][1] = READ_LE_S16(m_baseItForRelocation + offsetVertices + i * 8 + 2);
        m8_vertices[i][2] = READ_LE_S16(m_baseItForRelocation + offsetVertices + i * 8 + 4);
        m8_vertices[i][3] = READ_LE_S16(m_baseItForRelocation + offsetVertices + i * 8 + 6);
    }

    int offsetNormals = READ_LE_U32(blockData + 0xC);
    if (offsetNormals != offsetVertices) {
        assert(m0_flags & 0x10);
        mC_normals.resize(m2_numVertices);
        for (int i = 0; i < m2_numVertices; i++) {
            mC_normals[i][0] = READ_LE_S16(m_baseItForRelocation + offsetNormals + i * 8 + 0);
            mC_normals[i][1] = READ_LE_S16(m_baseItForRelocation + offsetNormals + i * 8 + 2);
            mC_normals[i][2] = READ_LE_S16(m_baseItForRelocation + offsetNormals + i * 8 + 4);
            mC_normals[i][3] = READ_LE_S16(m_baseItForRelocation + offsetNormals + i * 8 + 6);
        }
    }

    m10_offsetMeshBlocks = READ_LE_U32(blockData + 0x10);
    m14_offsetDisplayList = READ_LE_U32(blockData + 0x14);

    //assert(READ_LE_U32(blockData + 0x18) == 0);
    if (READ_LE_U32(blockData + 0x18)) {
        m18.resize(1);
        MissingCode();
    }

    int offsetDynamicVertices = READ_LE_U32(blockData + 0x1C);
    if (offsetDynamicVertices) {
        m1C_dynamicVertices = new sModelDynamicVertices;
        m1C_dynamicVertices->m0_count = READ_LE_U32(m_baseItForRelocation + offsetDynamicVertices + 0);
        m1C_dynamicVertices->m4_data.resize(m1C_dynamicVertices->m0_count + 1);
        for (int i = 0; i < m1C_dynamicVertices->m0_count + 1; i++) {
            m1C_dynamicVertices->m4_data[i].m0_count = READ_LE_S32(m_baseItForRelocation + offsetDynamicVertices + 4 + i * 0xC + 0);
            m1C_dynamicVertices->m4_data[i].m4_positionOffsets.resize(m1C_dynamicVertices->m4_data[i].m0_count);
            s32 offsetToDynamicPositions = READ_LE_S32(m_baseItForRelocation + offsetDynamicVertices + 4 + i * 0xC + 4);
            for (int j = 0; j < m1C_dynamicVertices->m4_data[i].m0_count; j++) {
                m1C_dynamicVertices->m4_data[i].m4_positionOffsets[j].m0_offset[0] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicPositions + 8 * j + 0);
                m1C_dynamicVertices->m4_data[i].m4_positionOffsets[j].m0_offset[1] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicPositions + 8 * j + 2);
                m1C_dynamicVertices->m4_data[i].m4_positionOffsets[j].m0_offset[2] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicPositions + 8 * j + 4);
                m1C_dynamicVertices->m4_data[i].m4_positionOffsets[j].m6_index = READ_LE_U16(m_baseItForRelocation + offsetToDynamicPositions + 8 * j + 6);
            }
            s32 offsetToDynamicNormals = READ_LE_S32(m_baseItForRelocation + offsetDynamicVertices + 4 + i * 0xC + 8);
            if (offsetToDynamicNormals != offsetToDynamicPositions) {
                assert(m0_flags & 0x10);
                m1C_dynamicVertices->m4_data[i].m8_normalOffsets.resize(m1C_dynamicVertices->m4_data[i].m0_count);
                for (int j = 0; j < m1C_dynamicVertices->m4_data[i].m0_count; j++) {
                    m1C_dynamicVertices->m4_data[i].m8_normalOffsets[j].m0_offset[0] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicNormals + 8 * j + 0);
                    m1C_dynamicVertices->m4_data[i].m8_normalOffsets[j].m0_offset[1] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicNormals + 8 * j + 2);
                    m1C_dynamicVertices->m4_data[i].m8_normalOffsets[j].m0_offset[2] = READ_LE_S16(m_baseItForRelocation + offsetToDynamicNormals + 8 * j + 4);
                    m1C_dynamicVertices->m4_data[i].m8_normalOffsets[j].m6_index = READ_LE_U16(m_baseItForRelocation + offsetToDynamicNormals + 8 * j + 6);
                }
            }
        }
        m1C_dynamicVertices->m_indexCount = READ_LE_S32(m_baseItForRelocation + offsetDynamicVertices + 4 + m1C_dynamicVertices->m0_count * 0xC);
        m1C_dynamicVertices->m_indices.resize(m1C_dynamicVertices->m_indexCount);
        s32 offsetToIndices = READ_LE_S32(m_baseItForRelocation + offsetDynamicVertices + 4 + m1C_dynamicVertices->m0_count * 0xC + 4);
        for (int i = 0; i < m1C_dynamicVertices->m_indexCount; i++) {
            m1C_dynamicVertices->m_indices[i] = READ_LE_U16(m_baseItForRelocation + offsetToIndices + 2 * i);
        }
    }

    m20 = READ_LE_S16(blockData + 0x20);
    m22 = READ_LE_S16(blockData + 0x22);
    m24 = READ_LE_S16(blockData + 0x24);
    m28 = READ_LE_S16(blockData + 0x28);
    m2A = READ_LE_S16(blockData + 0x2A);
    m2C = READ_LE_S16(blockData + 0x2C);
    m30 = READ_LE_U32(blockData + 0x30);
    m34_instanceBufferSize = READ_LE_U32(blockData + 0x34);
}

void sModel::init(std::vector<u8>::const_iterator input, int dataSize)
{
    mRawData.resize(dataSize);
    for (int i = 0; i < dataSize; i++)
    {
        mRawData[i] = input[i];
    }

    std::vector<u8>::iterator inputData = mRawData.begin();

    m0_numBlocks = READ_LE_U32(inputData);
    inputData += 0x10;

    m10_blocks.resize(m0_numBlocks);
    for (int i = 0; i < m0_numBlocks; i++)
    {
        m10_blocks[i].m_baseItForRelocation = &mRawData[0];

        std::vector<u8>::iterator blockData = inputData + i * 0x38;

        m10_blocks[i].init(&blockData[0]);
    }
}

void initModel2Sub0(void)
{
    overrideTPageMode = 0;
    initModel2Sub0Var1 = 1;
    return;
}

void initModel2(sModelBlock* pModelBlock, std::vector<sTag*>& outputBuffer, int param_3)
{
    currentModelInstanceDrawPrims = outputBuffer.begin();

    if ((((pModelBlock->m0_flags & 1) == 0) && (pModelBlock->m30 != 0)) && (param_3 != 0)) {
        traceNextAlloc(0x26);
        pModelBlock->m18.resize(pModelBlock->m30);
        pModelBlock->m0_flags |= 1;
    }

    currentModelBlockDisplayLists = pModelBlock->m_baseItForRelocation + pModelBlock->m14_offsetDisplayList;
    g_currentModelBlockSubBlocks = pModelBlock->m_baseItForRelocation + pModelBlock->m10_offsetMeshBlocks;
    currentModelBlockNormals = &pModelBlock->mC_normals;
    currentModelBlockVertices = &pModelBlock->m8_vertices;
    currentModeBlock18 = nullptr;
    if (pModelBlock->m18.size())
    {
        currentModeBlock18 = &pModelBlock->m18[0];
    }

    int initParam;
    switch (param_3)
    {
    case 0:
        initParam = 0;
        break;
    case 1:
        initParam = 1;
        break;
    case 2:
        if (pModelBlock->m0_flags & 2)
        {
            initParam = 1;
            if (pModelBlock->m0_flags & 1)
            {
                initParam = 4;
            }
        }
        else
        {
            if (pModelBlock->m0_flags & 1)
            {
                initParam = 1;
            }
            else
            {
                initParam = 3;
                pModelBlock->m0_flags |= 2;
            }
        }
        break;
    case 3:
        initParam = 3;
        pModelBlock->m0_flags |= 2;
        break;
    default:
        assert(0);
    }

    fieldPolyCount2 += pModelBlock->m4_numPrims;

    int currentMeshBlockCount = pModelBlock->m6_numMeshBlock;
    while (currentMeshBlockCount = currentMeshBlockCount - 1, currentMeshBlockCount != 0xffffffff) {
        int primType = READ_LE_U8(g_currentModelBlockSubBlocks);
        int numPrims = READ_LE_U16(g_currentModelBlockSubBlocks + 2) - 1;

        t_primInitFunc pInitFunction = polyRenderDefs[primType].m18_init;
        g_currentModelBlockSubBlocks += 4;
        while (numPrims != -1)
        {
            if (pInitFunction(currentModelBlockDisplayLists, g_currentModelBlockSubBlocks, (int)initParam) == 0)
            {
                currentModelBlockDisplayLists += 4;
            }
            else
            {
                g_currentModelBlockSubBlocks += polyRenderDefs[primType].m1C_size;
                //currentModelInstanceArray8 += polyRenderDefs[primType].m24_sizeInstanceArray;
                currentModelBlockDisplayLists += polyRenderDefs[primType].m20_sizeDisplayList;
                numPrims = numPrims + -1;

            }
        }
    }

    initModel2Sub0();
}