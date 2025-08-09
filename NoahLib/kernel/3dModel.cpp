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
    m8_offsetVertices = READ_LE_U32(blockData + 8);
    mC_offsetNormals = READ_LE_U32(blockData + 0xC);
    m10_offsetMeshBlocks = READ_LE_U32(blockData + 0x10);
    m14_offsetDisplayList = READ_LE_U32(blockData + 0x14);

    //assert(READ_LE_U32(blockData + 0x18) == 0);
    if (READ_LE_U32(blockData + 0x18)) {
        m18.resize(1);
        MissingCode();
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
    currentModelBlockNormals = pModelBlock->m_baseItForRelocation + pModelBlock->mC_offsetNormals;
    currentModelBlockVertices = pModelBlock->m_baseItForRelocation + pModelBlock->m8_offsetVertices;
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