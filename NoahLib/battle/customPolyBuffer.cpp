#include "noahLib.h"
#include "kernel/gte.h"
#include "psx/gpuprims.h"

#include "customPolyBuffer.h"

void copyCustomPolySubBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, int isTransparent, uint isShaded) {
    for (int i = 0; i < param_1->m14_count; i++) {
        const sCustomPolySubBufferPacket* polySource = &param_1->m1C_packets[i];

        u8 tagSize = polySource->m0_runtimeSizeNeededInWords;
        u8 code = polySource->m3_polyType1;

        u16 polyType = (polySource->m3_polyType1 & 0x1C) | ((polySource->m2_polyType0 ^ 1) & 1) << 8;
        switch (polyType) {
        case 0: // F3
        {
            sAnyPoly& newEntry = param_2.emplace_back();
            POLY_F3* poly = &newEntry.mF3;
            poly->m3_size = polySource->m0_runtimeSizeNeededInWords;
            poly->m_colorAndCode.command = polySource->m3_polyType1;
            SetShadeTex(poly, isShaded);
            if (isTransparent) {
                SetSemiTrans(poly, isTransparent);
            }
            poly->r0 = READ_LE_U8(polySource->m_rawData.begin() + 4);
            poly->g0 = READ_LE_U8(polySource->m_rawData.begin() + 5);
            poly->b0 = READ_LE_U8(polySource->m_rawData.begin() + 6);
            break;
        }
        case 0x10: // G3
        {
            sAnyPoly& newEntry = param_2.emplace_back();
            POLY_G3* poly = &newEntry.mG3;
            poly->m3_size = polySource->m0_runtimeSizeNeededInWords;
            poly->m_colorAndCode.command = polySource->m3_polyType1;
            SetShadeTex(poly, isShaded);
            if (isTransparent) {
                SetSemiTrans(poly, isTransparent);
            }
            poly->r0 = READ_LE_U8(polySource->m_rawData.begin() + 4);
            poly->g0 = READ_LE_U8(polySource->m_rawData.begin() + 5);
            poly->b0 = READ_LE_U8(polySource->m_rawData.begin() + 6);
            poly->r1 = READ_LE_U8(polySource->m_rawData.begin() + 8);
            poly->g1 = READ_LE_U8(polySource->m_rawData.begin() + 9);
            poly->b1 = READ_LE_U8(polySource->m_rawData.begin() + 10);
            poly->r2 = READ_LE_U8(polySource->m_rawData.begin() + 12);
            poly->g2 = READ_LE_U8(polySource->m_rawData.begin() + 13);
            poly->b2 = READ_LE_U8(polySource->m_rawData.begin() + 14);
            break;
        }
        default:
            assert(0);
        }
    }
}

void renderCustomPolyBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, OTTable* param_3, int param_4, int param_5, int param_6)
{
    if (param_6) {
        assert(0);
    }

    for (int i = 0; i < param_1->m14_count; i++) {
        sAnyPoly& currentAnyPoint = param_2[i];

        const sCustomPolySubBufferPacket* polySource = &param_1->m1C_packets[i];
        u16 polyType = (polySource->m3_polyType1 & 0x1C) | ((polySource->m2_polyType0 ^ 1) & 1) << 8;

        switch (polyType) {
        case 0: // F3
        {
            const SVECTOR* v0 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 8)];
            const SVECTOR* v1 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 10)];
            const SVECTOR* v2 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 12)];

            gte_ldv3(v0, v1, v2);
            gte_rtpt();
            int flag;
            gte_stflg(&flag);
            gte_nclip();
            if (flag & 0x4000) {
                break;
            }
            int z;
            gte_stopz(&z);
            gte_avsz3();
            if (z < 1) break;
            int z2;
            gte_stotz(&z2);

            z2 = (z2 >> (gDepthDivider & 0x1F)) + param_5;
            if (z2 < 5) {
                z2 = 5;
            }
            if (z2 > 0xFFF)
                break;

            POLY_F3* pF3 = &currentAnyPoint.mF3;
            gte_stsxy3(&pF3->x0y0, &pF3->x1y1, &pF3->x2y2);

            pF3->m0_pNext = (*param_3)[z2].m0_pNext;
            (*param_3)[z2].m0_pNext = pF3;

            if (param_6 != 0) {
                assert(0);
            }
            break;
        }
        case 0x10: // G3
        {
            const SVECTOR* v0 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x10)];
            const SVECTOR* v1 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x12)];
            const SVECTOR* v2 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x14)];

            gte_ldv3(v0, v1, v2);
            gte_rtpt();
            int flag;
            gte_stflg(&flag);
            gte_nclip();
            if (flag & 0x4000) {
                break;
            }
            int z;
            gte_stopz(&z);
            gte_avsz3();
            if (z < 1) break;
            int z2;
            gte_stotz(&z2);

            z2 = (z2 >> (gDepthDivider & 0x1F)) + param_5;
            if (z2 < 5) {
                z2 = 5;
            }
            if (z2 > 0xFFF)
                break;

            POLY_G3* pG3 = &currentAnyPoint.mG3;
            gte_stsxy3(&pG3->x0y0, &pG3->x1y1, &pG3->x2y2);

            pG3->m0_pNext = (*param_3)[z2].m0_pNext;
            (*param_3)[z2].m0_pNext = pG3;

            if (param_6 != 0) {
                assert(0);
            }
            break;
        }
        default:
            assert(0);
        }
    }
}

const sCustomPolySubBuffer* getCustomPolySubBuffer(const sCustomPolyBuffer* param_1, int param_2) {
    return &param_1->mC_buffers[param_2];
}

int getCustomPolySubBufferSize(const sCustomPolySubBuffer* param_1) {
    int count = 0;
    for (int i = 0; i < param_1->m14_count; i++) {
        count += (param_1->m1C_packets[i].m0_runtimeSizeNeededInWords + 1) * 4;
    }
    return count;
}