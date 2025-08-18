#pragma once

struct sCustomPolySubBufferPacket {
    std::vector<u8>::iterator init(std::vector<u8>::iterator primBuffer) {
        int size = READ_LE_U8(primBuffer) * 4;
        m_rawData.insert(m_rawData.begin(), primBuffer, primBuffer + size);

        m0_runtimeSizeNeededInWords = READ_LE_U8(m_rawData.begin() + 0);
        m1_offsetToNextPacketSizeInWords = READ_LE_U8(m_rawData.begin() + 1);
        m2_polyType0 = READ_LE_U8(m_rawData.begin() + 2);
        m3_polyType1 = READ_LE_U8(m_rawData.begin() + 3);

        return primBuffer + size;
    }
    u8 m0_runtimeSizeNeededInWords;
    u8 m1_offsetToNextPacketSizeInWords;
    u8 m2_polyType0;
    u8 m3_polyType1;
    std::vector<u8> m_rawData;
};

struct sCustomPolySubBuffer {
    void init(std::vector<u8>::iterator currentSubBuffer) {
        m0_offsetToVertices = READ_LE_U32(currentSubBuffer + 0x00);
        m4_numVertices = READ_LE_U32(currentSubBuffer + 0x04);
        m10_offset = READ_LE_U32(currentSubBuffer + 0x10);
        m14_count = READ_LE_U32(currentSubBuffer + 0x14);

        std::vector<u8>::iterator currentPrim = currentSubBuffer + m10_offset;
        m1C_packets.resize(m14_count);
        for (u32 i = 0; i < m14_count; i++) {
            currentPrim = m1C_packets[i].init(currentPrim);
        }

        m_vertices.resize(m4_numVertices);
        std::vector<u8>::iterator currentVertice = currentSubBuffer + m0_offsetToVertices;
        for (u32 i = 0; i < m4_numVertices; i++) {
            m_vertices[i].vx = READ_LE_S16(currentVertice + 0);
            m_vertices[i].vy = READ_LE_S16(currentVertice + 2);
            m_vertices[i].vz = READ_LE_S16(currentVertice + 4);
            m_vertices[i].pad = READ_LE_S16(currentVertice + 6);
            currentVertice += 8;
        }

    }
    u32 m0_offsetToVertices;
    u32 m4_numVertices;
    u32 m10_offset;
    u32 m14_count;
    std::vector<sCustomPolySubBufferPacket> m1C_packets;
    std::vector<SVECTOR> m_vertices;
};

struct sCustomPolyBuffer {
    sCustomPolyBuffer(u8* rawBuffer, u32 rawBufferSize) {
        buffer.insert(buffer.begin(), rawBuffer, rawBuffer + rawBufferSize),

        m8_count = READ_LE_U16(buffer.begin() + 0x8);
        mC_buffers.resize(m8_count);
        assert(m8_count == 1);
        std::vector<u8>::iterator currentSubBuffer = buffer.begin() + 0xC;
        for (u32 i = 0; i < m8_count; i++) {
            mC_buffers[i].init(currentSubBuffer);
        }
    }
    u32 m8_count;
    std::vector<sCustomPolySubBuffer> mC_buffers;
    std::vector<u8> buffer;
};

struct sAnyPoly {
    POLY_F3 mF3;
    POLY_G3 mG3;
};

void copyCustomPolySubBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, int isTransparent, uint isShaded);
void renderCustomPolyBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, OTTable* param_3, int param_4, int param_5, int param_6);
const sCustomPolySubBuffer* getCustomPolySubBuffer(const sCustomPolyBuffer* param_1, int param_2);
int getCustomPolySubBufferSize(const sCustomPolySubBuffer* param_1);
