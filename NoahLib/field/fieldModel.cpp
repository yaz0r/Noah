#include "noahLib.h"
#include "fieldModel.h"

#include "bgfx/bgfx.h"

std::vector<sModel> gCurrentFieldModels;

void sModel::init(std::vector<u8>::iterator& input, int dataSize)
{
    mRawData.resize(dataSize);
    for (int i=0; i<dataSize; i++)
    {
        mRawData[i] = input[i];
    }

    std::vector<u8>::iterator inputData = mRawData.begin();

    m0_numBlocks = READ_LE_U32(inputData);
    inputData += 0x10;

    m10_blocks.resize(m0_numBlocks);
    for (int i = 0; i < m0_numBlocks; i++)
    {
        m10_blocks[i].m_model = this;

        std::vector<u8>::iterator blockData = inputData + i * 0x38;

        m10_blocks[i].m2_numVertices = READ_LE_U16(blockData + 2);
        m10_blocks[i].m4_numMesh = READ_LE_U16(blockData + 4);
        m10_blocks[i].m6_numMeshBlock = READ_LE_U16(blockData + 6);
        m10_blocks[i].m8_offsetVertices = READ_LE_U32(blockData + 8);
        m10_blocks[i].mC_offsetNormals = READ_LE_U32(blockData + 0xC);
        m10_blocks[i].m10_offsetMeshBlocks = READ_LE_U32(blockData + 0x10);
        m10_blocks[i].m14_offsetDisplayList = READ_LE_U32(blockData + 0x14);
        m10_blocks[i].m34_count = READ_LE_U32(blockData + 0x34);

        std::vector<u8>::iterator vertexData = inputData + m10_blocks[i].m8_offsetVertices;
    }
}

bgfx::ProgramHandle getLineShader();

void renderTriangleWithTexture(int viewIndex, std::vector<u8>::iterator& mesh_blocks, std::vector<u8>::iterator& displaylist, std::vector<u8>::iterator& vertices, u32 cmd)
{
    std::array<float, 2> uv[4];
    uv[0][0] = displaylist[0];
    uv[0][1] = displaylist[1];
    uv[1][0] = displaylist[2];
    uv[1][1] = displaylist[3];
    uv[2][0] = cmd & 255;
    uv[2][1] = (cmd >> 8) & 255;
    displaylist += 4;

    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
        .end();

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientBuffers(&vertexBuffer, layout, 3, &indexBuffer, 3 * 2);

    struct sVertice
    {
        float v[3];
        float color[4];
    };

    sVertice* pVertices = (sVertice*)vertexBuffer.data;
    u16* pIndices = (u16*)indexBuffer.data;

    for (int j = 0; j < 3; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        pVertices[j].v[0] = x;
        pVertices[j].v[1] = y;
        pVertices[j].v[2] = z;

        pVertices[j].color[0] = 1.f;
        pVertices[j].color[1] = 1.f;
        pVertices[j].color[2] = 1.f;
        pVertices[j].color[3] = 1.f;
    }

    pIndices[0] = 0;
    pIndices[1] = 1;

    pIndices[2] = 1;
    pIndices[3] = 2;

    pIndices[4] = 2;
    pIndices[5] = 0;

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LEQUAL
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_PT_LINES
    );

    bgfx::setVertexBuffer(0, &vertexBuffer);
    bgfx::setIndexBuffer(&indexBuffer);

    bgfx::submit(viewIndex, getLineShader());

}

void renderTriangleWithColor(int viewIndex, std::vector<u8>::iterator& mesh_blocks, std::vector<u8>::iterator& displaylist, std::vector<u8>::iterator& vertices, u32 cmd)
{
    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .end();

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientBuffers(&vertexBuffer, layout, 3, &indexBuffer, 3 * 2);

    struct sVertice
    {
        float v[3];
        u8 color[4];
    };

    sVertice* pVertices = (sVertice*)vertexBuffer.data;
    u16* pIndices = (u16*)indexBuffer.data;

    for (int j = 0; j < 3; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        pVertices[j].v[0] = x;
        pVertices[j].v[1] = y;
        pVertices[j].v[2] = z;

        pVertices[j].color[0] = (cmd >> 16) & 255;
        pVertices[j].color[1] = (cmd >> 8) & 255;
        pVertices[j].color[2] = (cmd) & 255;
        pVertices[j].color[3] = 0xFF;
    }

    pIndices[0] = 0;
    pIndices[1] = 1;

    pIndices[2] = 1;
    pIndices[3] = 2;

    pIndices[4] = 2;
    pIndices[5] = 0;

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LEQUAL
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_PT_LINES
    );

    bgfx::setVertexBuffer(0, &vertexBuffer);
    bgfx::setIndexBuffer(&indexBuffer);

    bgfx::submit(viewIndex, getLineShader());

}

void renderQuadWithColor(int viewIndex, std::vector<u8>::iterator& mesh_blocks, std::vector<u8>::iterator& displaylist, std::vector<u8>::iterator& vertices, u32 cmd)
{
    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
        .end();

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4 * 2);

    struct sVertice
    {
        float v[3];
        u8 color[4];
    };

    sVertice* pVertices = (sVertice*)vertexBuffer.data;
    u16* pIndices = (u16*)indexBuffer.data;

    const int quad_idx[] = { 0, 1, 3, 2 };
    for (int j = 0; j < 4; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + quad_idx[j] * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        pVertices[j].v[0] = x;
        pVertices[j].v[1] = y;
        pVertices[j].v[2] = z;

        pVertices[j].color[0] = (cmd >> 16) & 255;
        pVertices[j].color[1] = (cmd >> 8) & 255;
        pVertices[j].color[2] = (cmd) & 255;
        pVertices[j].color[3] = 0xFF;
    }

    pIndices[0] = 0;
    pIndices[1] = 1;

    pIndices[2] = 1;
    pIndices[3] = 2;

    pIndices[4] = 2;
    pIndices[5] = 3;

    pIndices[6] = 3;
    pIndices[7] = 0;

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LEQUAL
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_PT_LINES
    );

    bgfx::setVertexBuffer(0, &vertexBuffer);
    bgfx::setIndexBuffer(&indexBuffer);

    bgfx::submit(viewIndex, getLineShader());
}

void renderQuadWithTexture(int viewIndex, std::vector<u8>::iterator& mesh_blocks, std::vector<u8>::iterator& displaylist, std::vector<u8>::iterator& vertices)
{
    std::array<float, 2> uv[4];
    uv[0][0] = displaylist[0];
    uv[0][1] = displaylist[1];
    uv[1][0] = displaylist[2];
    uv[1][1] = displaylist[3];
    uv[2][0] = displaylist[4];
    uv[2][1] = displaylist[5];
    uv[3][0] = displaylist[6];
    uv[3][1] = displaylist[7];
    displaylist += 8;

    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
        .end();

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 4 * 2);

    struct sVertice
    {
        float v[3];
        float color[4];
    };

    sVertice* pVertices = (sVertice*)vertexBuffer.data;
    u16* pIndices = (u16*)indexBuffer.data;

    const int quad_idx[] = { 0, 1, 3, 2 };
    for (int j = 0; j < 4; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + quad_idx[j] * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        pVertices[j].v[0] = x;
        pVertices[j].v[1] = y;
        pVertices[j].v[2] = z;

        pVertices[j].color[0] = 1.f;
        pVertices[j].color[1] = 1.f;
        pVertices[j].color[2] = 1.f;
        pVertices[j].color[3] = 1.f;
    }

    pIndices[0] = 0;
    pIndices[1] = 1;

    pIndices[2] = 1;
    pIndices[3] = 2;

    pIndices[4] = 2;
    pIndices[5] = 3;

    pIndices[6] = 3;
    pIndices[7] = 0;

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LEQUAL
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | BGFX_STATE_PT_LINES
    );

    bgfx::setVertexBuffer(0, &vertexBuffer);
    bgfx::setIndexBuffer(&indexBuffer);

    bgfx::submit(viewIndex, getLineShader());

}

void sModel::bgfxRender(int viewIndex)
{
    for (int i = 0; i < m0_numBlocks; i++)
    {
        m10_blocks[i].bgfxRender(viewIndex);
    }
}

void sModelBlock::bgfxRender(int viewIndex)
{
    std::vector<u8>::iterator mesh_blocks = m_model->mRawData.begin() + m10_offsetMeshBlocks;
    std::vector<u8>::iterator displaylist = m_model->mRawData.begin() + m14_offsetDisplayList;
    std::vector<u8>::iterator vertices = m_model->mRawData.begin() + m8_offsetVertices;

    bool quad_block = false;
    int poly_available = 0;
    int number_mesh_block = m6_numMeshBlock;
    while (poly_available > 0 || number_mesh_block > 0)
    {
        // init the mesh block
        if (poly_available == 0) {
            quad_block = (mesh_blocks[0] & 16) != 0;
            poly_available = READ_LE_U16(mesh_blocks + 2);
            mesh_blocks += 4;
            number_mesh_block--;
        }

        u32 cmd = READ_LE_U32(displaylist);
        displaylist += 4;

        bool hp = ((cmd >> 24) & 16) != 0; // gouraud shading
        bool quad = ((cmd >> 24) & 8) != 0; // quad or tri
        bool tme = ((cmd >> 24) & 4) != 0; // texture mapping
        bool abe = ((cmd >> 24) & 2) != 0; // semi transparency
        bool fullbright = ((cmd >> 24) & 1) != 0; // bypass lighting

        int srcAlpha = 255;
        int dstAlpha = 255;
        u16 status = 0;
        u16 clut = 0;
        switch ((cmd >> 24) & ~(16 | 2 | 1)) // (careful not to mask 0xc4 or 0xc8!)
        {
        case 0xC4: // texture page
            status = cmd & 0xFFFF;
            switch ((status >> 3) & 7) {
            case 0:
                srcAlpha = dstAlpha = 128;
                break;
            case 1:
                srcAlpha = dstAlpha = 255;
                break;
            case 2:
                srcAlpha = dstAlpha = 255;
                break;
            case 3:
                srcAlpha = 64;
                dstAlpha = 255;
                break;
            }
            break;
        case 0xC8: // clut
            clut = cmd & 0xFFFF;
            break;

        case 0x20: { // colored triangle
            renderTriangleWithColor(viewIndex, mesh_blocks, displaylist, vertices, cmd);

            mesh_blocks += 8;
            poly_available--;
            break;
        }

        case 0x24: { // triangle with texture
            renderTriangleWithTexture(viewIndex, mesh_blocks, displaylist, vertices, cmd);

            mesh_blocks += 8;
            poly_available--;
            break;
        }

        case 0x28: { // quad with texture
            renderQuadWithColor(viewIndex, mesh_blocks, displaylist, vertices, cmd);

            mesh_blocks += 8;
            poly_available--;
            break;
        }

        case 0x2c: { // quad with texture
            renderQuadWithTexture(viewIndex, mesh_blocks, displaylist, vertices);

            mesh_blocks += 8;
            poly_available--;
            break;
        }
        default:
            assert(0);
        }
    }
}

