#include "noahLib.h"
#include "walkMesh.h"

#include "bgfx/bgfx.h"

sWalkMesh walkMesh;

void sWalkMesh::init(const std::vector<u8>& input)
{
    m0_count = READ_LE_U32(input.begin());
    assert(m0_count <= 4);

    int numMaxMaterials = -1;

    m_blocks.resize(m0_count);
    for (int i=0; i<m0_count; i++)
    {
        sBlock& newBlock = m_blocks[i];

        u32 block_size = READ_LE_U32(input.begin() + 4 + i * 4);

        u32 block_start = READ_LE_U32(input.begin() + 4 + 0x14 + i * 8 + 0);
        u32 block_vertex_start = READ_LE_U32(input.begin() + 4 + 0x14 + i * 8 + 4);

        int numTrianglesInBlock = block_size / 0xE;
        assert((block_size % 0xE) == 0);

        int maxVertexIndex = -1;

        newBlock.m_triangles.resize(numTrianglesInBlock);
        for (int triangleId = 0; triangleId < numTrianglesInBlock; triangleId++)
        {
            std::vector<u8>::const_iterator startOfTriangleBlock = input.begin() + block_start + 0xE * triangleId;

            sTriangleData& newTriangleData = newBlock.m_triangles[triangleId];
            for (int j=0; j<3; j++)
            {
                newTriangleData.m0_verticeIndex[j] = READ_LE_S16(startOfTriangleBlock + j * 2);
                maxVertexIndex = std::max<int>(maxVertexIndex, newTriangleData.m0_verticeIndex[j]);
            }
            for (int j = 0; j < 3; j++)
            {
                newTriangleData.m6_connectivity[j] = READ_LE_S16(startOfTriangleBlock + 6 + j * 2);
            }
            newTriangleData.mC_indexInWalkmeshData1 = READ_LE_U8(startOfTriangleBlock + 0xC);
            newTriangleData.mD = READ_LE_U8(startOfTriangleBlock + 0xD);

            numMaxMaterials = std::max<int>(numMaxMaterials, newTriangleData.mC_indexInWalkmeshData1);
        }

        newBlock.m_vertices.resize(maxVertexIndex + 1);
        for (int vertexId = 0; vertexId < maxVertexIndex + 1; vertexId ++)
        {
            SFP_VEC4& vertice = newBlock.m_vertices[vertexId];

            vertice.vx = READ_LE_S16(input.begin() + block_vertex_start + vertexId * 4 * 2 + 0);
            vertice.vy = READ_LE_S16(input.begin() + block_vertex_start + vertexId * 4 * 2 + 2);
            vertice.vz = READ_LE_S16(input.begin() + block_vertex_start + vertexId * 4 * 2 + 4);
            vertice.pad = READ_LE_S16(input.begin() + block_vertex_start + vertexId * 4 * 2 + 6);
        }
    }

    m_materials.reserve(numMaxMaterials);
    u32 material_offset = READ_LE_U32(input.begin() + 4 + 0x10);
    std::vector<u8>::const_iterator material_data = input.begin() + material_offset;
    for (int i=0; i<numMaxMaterials; i++)
    {
        m_materials.push_back(READ_LE_U32(material_data + 4 * i));
    }
}


bgfx::ShaderHandle loadBgfxShader(const std::string& filename)
{
    std::vector<u8> memBlob;
    FILE* fHandle = fopen(filename.c_str(), "rb");
    if (fHandle == nullptr)
        return BGFX_INVALID_HANDLE;
    fseek(fHandle, 0, SEEK_END);
    u32 size = ftell(fHandle);
    fseek(fHandle, 0, SEEK_SET);
    memBlob.resize(size);
    fread(&memBlob[0], 1, size, fHandle);
    fclose(fHandle);

    bgfx::ShaderHandle handle = bgfx::createShader(bgfx::copy(&memBlob[0], size));
    bgfx::setName(handle, filename.c_str());

    return handle;
}

bgfx::ProgramHandle loadBgfxProgram(const std::string& VSFile, const std::string& PSFile)
{
    std::string shaderFileExtension = "";
    switch (bgfx::getRendererType())
    {
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderFileExtension = ".dx11.debug.bin";  break;
    //case bgfx::RendererType::Direct3D12: shaderFileExtension = ".dx11.bin";  break;
    case bgfx::RendererType::OpenGL:     shaderFileExtension = ".glsl.bin";  break;
    case bgfx::RendererType::Metal:      shaderFileExtension = ".metal.bin";  break;
    case bgfx::RendererType::Vulkan:     shaderFileExtension = ".spirv.bin";  break;
    default:
        assert(0);
    }

    bgfx::ShaderHandle vsh = loadBgfxShader(std::string("shaders/generated/") + VSFile + shaderFileExtension);
    bgfx::ShaderHandle psh = loadBgfxShader(std::string("shaders/generated/") + PSFile + shaderFileExtension);

    assert(bgfx::isValid(vsh));
    assert(bgfx::isValid(psh));

    bgfx::ProgramHandle ProgramHandle = bgfx::createProgram(vsh, psh, true /* destroy shaders when program is destroyed */);
    assert(bgfx::isValid(ProgramHandle));
    return ProgramHandle;
}

bgfx::ProgramHandle getLineShader()
{
    static bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
    if (!bgfx::isValid(programHandle))
    {
        programHandle = loadBgfxProgram("vertexColor_vs", "vertexColor_ps");
    }

    return programHandle;
}

bgfx::ProgramHandle getModelShader() // TODO: move!
{
    static bgfx::ProgramHandle programHandle = BGFX_INVALID_HANDLE;
    if (!bgfx::isValid(programHandle))
    {
        programHandle = loadBgfxProgram("model3d_vs", "model3d_ps");
    }

    return programHandle;
}

void sWalkMesh::bgfxRender(int viewIndex)
{
    if (m_blocks.size() == 0)
    {
        return;
    }

    const sBlock& newBlock = m_blocks[0];
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, newBlock.m_vertices.size(), &indexBuffer, newBlock.m_triangles.size() * 3 * 2);

        struct sVertice
        {
            float v[3];
            float color[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i=0; i<newBlock.m_vertices.size(); i++)
        {
            pVertices[i].v[0] = newBlock.m_vertices[i].vx;
            pVertices[i].v[1] = newBlock.m_vertices[i].vy;
            pVertices[i].v[2] = newBlock.m_vertices[i].vz;

            pVertices[i].color[0] = 1.f;
            pVertices[i].color[1] = 0.f;
            pVertices[i].color[2] = 0.f;
            pVertices[i].color[3] = 1.f;
        }

        for (int i=0; i<newBlock.m_triangles.size(); i++)
        {
            pIndices[i * 3 * 2 + 0] = newBlock.m_triangles[i].m0_verticeIndex[0];
            pIndices[i * 3 * 2 + 1] = newBlock.m_triangles[i].m0_verticeIndex[1];

            pIndices[i * 3 * 2 + 2] = newBlock.m_triangles[i].m0_verticeIndex[1];
            pIndices[i * 3 * 2 + 3] = newBlock.m_triangles[i].m0_verticeIndex[2];

            pIndices[i * 3 * 2 + 4] = newBlock.m_triangles[i].m0_verticeIndex[2];
            pIndices[i * 3 * 2 + 5] = newBlock.m_triangles[i].m0_verticeIndex[0];
        }

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
}
