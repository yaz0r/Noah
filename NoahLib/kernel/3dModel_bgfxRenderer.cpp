#include "noahLib.h"
#include "kernel/3dModel_bgfxRenderer.h"
#include "kernel/3dModel.h"

#include "bgfx/bgfx.h"
#include <map>

std::vector<sModel> gCurrentFieldModels;

struct sVertice
{
    float v[3];
    float n[3];
    float uv[3];
    float color[4];
};

std::vector<sVertice> currentVertices;
std::vector<u16> currentIndices;

bgfx::ProgramHandle getModelShader();

struct TexInfo {
    u16 status;
    u16 clut;
    u8 alpha;
    bool abe;
    bool operator<(const TexInfo& rhs) const {
        if (status != rhs.status) {
            return status < rhs.status;
        }
        if (clut != rhs.clut) {
            return clut < rhs.clut;
        }
        if (alpha != rhs.alpha) {
            return alpha < rhs.alpha;
        }
        return abe < rhs.abe;
    }
};

std::map<TexInfo, bgfx::TextureHandle> texInfoMap;
extern std::array<u8, 2048 * 512> gVram;

void clearModelTextureCache()
{
    texInfoMap.clear();
}

bgfx::TextureHandle getTexture(u16 status, u16 clut, bool abe, int alpha)
{
    if (currentIndices.size() == 0)
    {
        return BGFX_INVALID_HANDLE;
    }

    alpha = 128;
    TexInfo texInfo;
    texInfo.status = status;
    texInfo.clut = clut;
    texInfo.abe = abe;
    texInfo.alpha = alpha;
    if (texInfoMap.find(texInfo) != texInfoMap.end()) {
        return texInfoMap[texInfo];
    }

    u8* texture = new u8[256 * 256 * 4];

    int tx = (status & 0xF) * 64 * 2;
    int ty = ((status >> 4) & 1) * 256;
    int tp = (status >> 7) & 3;

    int px = clut & 63;
    int py = clut >> 6;

    switch (tp) {
    case 0: {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                u8 val = gVram[(y + ty) * 2048 + (x / 2) + tx];
                int idx = (x & 1) ? val >> 4 : val & 0xF;
                u16 col = READ_LE_U16(&gVram[0] + (py) * 2048 + idx * 2 + px * 2);
                bool stp = (col & 0x8000) != 0;

                int r = (((col) & 31) * 255 + 15) / 31;
                int g = (((col >> 5) & 31) * 255 + 15) / 31;
                int b = (((col >> 10) & 31) * 255 + 15) / 31;
                int a = /*(idx == 0) || */(col == 0) ? 0 : (abe && stp && ((col & 0x7FFF) != 0)) ? alpha : 255;

                int addr = (y * 256 + x) * 4;
                texture[addr + 0] = r;
                texture[addr + 1] = g;
                texture[addr + 2] = b;
                texture[addr + 3] = a;
            }
        }
        break;
    }
    case 1: {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                int idx = gVram[(y + ty) * 2048 + x + tx];
                u16 col = READ_LE_U16(&gVram[0] + (py) * 2048 + idx * 2 + px * 2);
                bool stp = (col & 0x8000) != 0;

                int r = (((col) & 31) * 255 + 15) / 31;
                int g = (((col >> 5) & 31) * 255 + 15) / 31;
                int b = (((col >> 10) & 31) * 255 + 15) / 31;
                int a = /* (idx == 0)  ||*/ (col == 0) ? 0 : (abe && stp && ((col & 0x7FFF) != 0)) ? alpha : 255;

                int addr = (y * 256 + x) * 4;
                texture[addr + 0] = r;
                texture[addr + 1] = g;
                texture[addr + 2] = b;
                texture[addr + 3] = a;
            }
        }
        break;
    }
    case 2: {
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                u16 col = READ_LE_U16(&gVram[0] + (y + ty) * 2048 + x * 2 + tx);
                bool stp = (col & 0x8000) != 0;

                int r = (((col) & 31) * 255 + 15) / 31;
                int g = (((col >> 5) & 31) * 255 + 15) / 31;
                int b = (((col >> 10) & 31) * 255 + 15) / 31;
                int a = (col == 0) ? 0 : (abe && stp && ((col & 0x7FFF) != 0)) ? alpha : 255;

                int addr = (y * 256 + x) * 4;
                texture[addr + 0] = r;
                texture[addr + 1] = g;
                texture[addr + 2] = b;
                texture[addr + 3] = a;
            }
        }
        break;
    }
    }

    bgfx::TextureHandle newHandle = bgfx::createTexture2D(256, 256, false, 1, bgfx::TextureFormat::RGBA8, 0, bgfx::copy(texture, 256 * 256 * 4));
    delete[] texture;

    texInfoMap[texInfo] = newHandle;

    return newHandle;
}

void flushTriangles(sModelBlock* pThis, int viewIndex, bgfx::TextureHandle textureHandle)
{
    if (currentIndices.size() == 0)
    {
        return;
    }

    bgfx::VertexLayout layout;
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
        .end();

    bgfx::TransientVertexBuffer vertexBuffer;
    bgfx::TransientIndexBuffer indexBuffer;
    bgfx::allocTransientBuffers(&vertexBuffer, layout, currentVertices.size(), &indexBuffer, currentIndices.size());

    sDrawCall newDrawCall;
    newDrawCall.m_vb = bgfx::createVertexBuffer(bgfx::copy(&currentVertices[0], currentVertices.size() * sizeof(sVertice)), layout);
    newDrawCall.m_ib = bgfx::createIndexBuffer(bgfx::copy(&currentIndices[0], currentIndices.size() * 2));
    newDrawCall.m_texture = textureHandle;

    pThis->getBgfxData()->m_drawCalls.push_back(newDrawCall);

    currentVertices.clear();
    currentIndices.clear();
}

void renderTriangleWithTexture(int viewIndex, u8*& mesh_blocks, u8*& displaylist, u8*& vertices, u32 cmd)
{
    std::array<float, 2> uv[4];
    uv[0][0] = displaylist[0];
    uv[0][1] = displaylist[1];
    uv[1][0] = displaylist[2];
    uv[1][1] = displaylist[3];
    uv[2][0] = cmd & 255;
    uv[2][1] = (cmd >> 8) & 255;
    displaylist += 4;

    int startIndex = currentVertices.size();

    for (int j = 0; j < 3; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        sVertice newVertice;

        newVertice.v[0] = x;
        newVertice.v[1] = y;
        newVertice.v[2] = z;

        newVertice.uv[0] = uv[j][0] / 256.f;
        newVertice.uv[1] = uv[j][1] / 256.f;
        newVertice.uv[2] = 1.f;

        newVertice.color[0] = 1.f;
        newVertice.color[1] = 1.f;
        newVertice.color[2] = 1.f;
        newVertice.color[3] = 1.f;

        currentVertices.push_back(newVertice);
    }

    currentIndices.push_back(startIndex + 0);
    currentIndices.push_back(startIndex + 1);
    currentIndices.push_back(startIndex + 2);
}

void renderTriangleWithColor(int viewIndex, u8*& mesh_blocks, u8*& displaylist, u8*& vertices, u32 cmd)
{
    int startIndex = currentVertices.size();

    for (int j = 0; j < 3; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        sVertice newVertice;

        newVertice.v[0] = x;
        newVertice.v[1] = y;
        newVertice.v[2] = z;

        newVertice.uv[0] = 0;
        newVertice.uv[1] = 0;
        newVertice.uv[2] = 0;

        newVertice.color[2] = ((cmd >> 16) & 255) / 255.f;
        newVertice.color[1] = ((cmd >> 8) & 255) / 255.f;
        newVertice.color[0] = ((cmd) & 255) / 255.f;
        newVertice.color[3] = 0.f;

        currentVertices.push_back(newVertice);
    }

    currentIndices.push_back(startIndex + 0);
    currentIndices.push_back(startIndex + 1);
    currentIndices.push_back(startIndex + 2);
}

void renderQuadWithColor(int viewIndex, u8*& mesh_blocks, u8*& displaylist, u8*& vertices, u32 cmd)
{
    int startIndex = currentVertices.size();

    for (int j = 0; j < 4; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        sVertice newVertice;

        newVertice.v[0] = x;
        newVertice.v[1] = y;
        newVertice.v[2] = z;

        newVertice.uv[0] = 0;
        newVertice.uv[1] = 0;
        newVertice.uv[2] = 0;

        newVertice.color[2] = ((cmd >> 16) & 255) / 255.f;
        newVertice.color[1] = ((cmd >> 8) & 255) / 255.f;
        newVertice.color[0] = ((cmd) & 255) / 255.f;
        newVertice.color[3] = 0.f;

        currentVertices.push_back(newVertice);
    }

    currentIndices.push_back(startIndex + 0);
    currentIndices.push_back(startIndex + 1);
    currentIndices.push_back(startIndex + 2);

    currentIndices.push_back(startIndex + 3);
    currentIndices.push_back(startIndex + 2);
    currentIndices.push_back(startIndex + 1);
}

void renderQuadWithTexture(int viewIndex, u8*& mesh_blocks, u8*& displaylist, u8*& vertices)
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

    int startIndex = currentVertices.size();

    for (int j = 0; j < 4; j++)
    {
        int vtx = READ_LE_U16(mesh_blocks + j * 2);
        int x = (s16)READ_LE_S16(vertices + vtx * 8 + 0);
        int y = (s16)READ_LE_S16(vertices + vtx * 8 + 2);
        int z = (s16)READ_LE_S16(vertices + vtx * 8 + 4);

        sVertice newVertice;

        newVertice.v[0] = x;
        newVertice.v[1] = y;
        newVertice.v[2] = z;

        newVertice.uv[0] = uv[j][0] / 256.f;
        newVertice.uv[1] = uv[j][1] / 256.f;
        newVertice.uv[2] = 1;

        newVertice.color[0] = 1.f;
        newVertice.color[1] = 1.f;
        newVertice.color[2] = 1.f;
        newVertice.color[3] = 1.f;

        currentVertices.push_back(newVertice);
    }

    currentIndices.push_back(startIndex + 0);
    currentIndices.push_back(startIndex + 1);
    currentIndices.push_back(startIndex + 2);

    currentIndices.push_back(startIndex + 3);
    currentIndices.push_back(startIndex + 2);
    currentIndices.push_back(startIndex + 1);
}

void bgfxRender(sModel* pModel, int viewIndex, float* modelMatrix)
{
    for (int i = 0; i < pModel->m0_numBlocks; i++)
    {
        pModel->m10_blocks[i].getBgfxData()->bgfxRender(&pModel->m10_blocks[i], viewIndex, modelMatrix);
    }
}

void sBgfxRendererData::bgfxRender(sModelBlock* pModelBlock, int viewIndex, float* modelMatrix)
{
    if (m_drawCalls.size() == 0)
    {
        buildDrawcall(pModelBlock, viewIndex);
    }

    for (int i = 0; i < m_drawCalls.size(); i++)
    {
        static bgfx::UniformHandle textureUniform = BGFX_INVALID_HANDLE;
        if (!bgfx::isValid(textureUniform))
        {
            textureUniform = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
        }
        bgfx::setTexture(0, textureUniform, m_drawCalls[i].m_texture);

        bgfx::setState(0 | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LEQUAL
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_MSAA
        );

        if (modelMatrix)
        {
            bgfx::setTransform(modelMatrix);
        }

        bgfx::setVertexBuffer(0, m_drawCalls[i].m_vb);
        bgfx::setIndexBuffer(m_drawCalls[i].m_ib);

        bgfx::submit(viewIndex, getModelShader());
    }
}

void sBgfxRendererData::buildDrawcall(sModelBlock* pModelBlock, int viewIndex)
{
    u8* mesh_blocks = pModelBlock->m_baseItForRelocation + pModelBlock->m10_offsetMeshBlocks;
    u8* displaylist = pModelBlock->m_baseItForRelocation + pModelBlock->m14_offsetDisplayList;
    u8* vertices = pModelBlock->m_baseItForRelocation + pModelBlock->m8_offsetVertices;

    bool quad_block = false;
    int poly_available = 0;
    int number_mesh_block = pModelBlock->m6_numMeshBlock;
    u16 status = 0;
    u16 clut = 0;
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

        switch ((cmd >> 24) & ~(16 | 2 | 1)) // (careful not to mask 0xc4 or 0xc8!)
        {
        case 0xC4: // texture page
            flushTriangles(pModelBlock, viewIndex, getTexture(status, clut, abe, srcAlpha));

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
            flushTriangles(pModelBlock, viewIndex, getTexture(status, clut, abe, srcAlpha));
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

        case 0x28: { // quad with color
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

        if (!((poly_available > 0 || number_mesh_block > 0)))
        {
            flushTriangles(pModelBlock, viewIndex, getTexture(status, clut, abe, srcAlpha)); //last flush
        }
    }


}
