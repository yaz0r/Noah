#include "noahLib.h"
#include "kernel/3dModel_psxRenderer.h"
#include "kernel/3dModel.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "kernel/gte.h"

s32 overrideTPageValue = 0;
s16 primD_initSub0Sub0Var0 = 0;
int overrideTPageMode = 0;

s16 primD_initSub0Sub1Var0 = 0;
s32 primD_initSub0Sub1Var1 = 0;

int initModel2Sub0Var1 = 1;

u8* currentModeBlock18;
std::vector<SVECTOR>* currentModelBlockVertices;
std::vector<SVECTOR>* currentModelBlockNormals;

s32 fieldPolyCount;
s32 objectClippingMask = 1;
s32 fieldPolyCount2;

u8* g_currentModelBlockDisplayLists;
std::vector<sModelBlock::sMeshBlock>::iterator g_currentModelBlockSubBlocks;


std::vector<sTag*>::iterator currentModelInstanceDrawPrims;

void setupOverrideTPage(uint x, uint y)
{
    overrideTPageMode = 1;
    overrideTPageValue = GetTPage(0, 0, x & 0xffff, y & 0xffff) & 0x1f;
}

void setupOverrideClut(uint x, uint y)
{
    u16 uVar1 = GetClut(x & 0xffff, y & 0xffff);
    initModel2Sub0Var1 = 0;
    primD_initSub0Sub1Var1 = uVar1 & 0xfff0;
    return;
}

void setupPrimClut(ushort param_1)
{
    primD_initSub0Sub1Var0 = param_1;
    if (initModel2Sub0Var1 == 0) {
        primD_initSub0Sub1Var0 = primD_initSub0Sub1Var0 & 0xf | (ushort)primD_initSub0Sub1Var1;
    }
    return;
}

void setupPrimTexturePage(ushort param_1)

{
    primD_initSub0Sub0Var0 = (ushort)overrideTPageValue;
    if (overrideTPageMode == 1) {
        primD_initSub0Sub0Var0 = param_1 & 0xffe0 | primD_initSub0Sub0Var0;
    }
    else {
        if (overrideTPageMode != 2) {
            primD_initSub0Sub0Var0 = param_1;
            return;
        }
    }
    return;
}

int primD_isValid(u8* displayList)
{
    byte bVar1;
    int iVar2;

    bVar1 = READ_LE_U8(displayList + 3);
    iVar2 = 1;
    if ((bVar1 & 0xf0) == 0xc0) {
        if (bVar1 == 0xc4) {
            setupPrimTexturePage(READ_LE_U16(displayList));
            iVar2 = 0;
        }
        else {
            iVar2 = 1;
            if (bVar1 == 0xc8) {
                setupPrimClut(READ_LE_U16(displayList));
                iVar2 = 0;
            }
        }
    }
    return iVar2;
}


void computeFaceNormal(const SVECTOR& v0, const SVECTOR& v1, const SVECTOR& v2, SVECTOR* pNormal)
{
    MissingCode();
}

void NormalColorCol(SVECTOR* faceNormal, u8* displayList, POLY_FT3* pNewPoly)
{
    MissingCode();
}

void NormalColor(SVECTOR* $2, std::array<u8, 4> $3)
{
    /*    gte_ldv0((SVECTOR*)$2);
        gte_ncs_b();
        gte_strgb($3);
        return;*/
    MissingCode();
}

int prim0_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    POLY_FT3* pNewPoly = new POLY_FT3;
    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;

    pNewPoly->m3_size = 4;

    if ((initParam & 1) == 0) {
        if ((initParam & 4) == 0) {
            pNewPoly->r0 = READ_LE_U8(displayList + 0);
            pNewPoly->g0 = READ_LE_U8(displayList + 1);
            pNewPoly->b0 = READ_LE_U8(displayList + 2);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }
        currentModeBlock18 = currentModeBlock18 + 4;
    }
    else {
        if ((initParam & 2) == 0) {
            SVECTOR faceNormal;
            computeFaceNormal(
                currentModelBlockVertices->at(primitiveIndices[0]),
                currentModelBlockVertices->at(primitiveIndices[1]),
                currentModelBlockVertices->at(primitiveIndices[2]),
                &faceNormal
            );
            NormalColorCol(&faceNormal, displayList, pNewPoly);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }

        *(u32*)&(*currentModeBlock18) = READ_LE_U32(displayList);
        currentModeBlock18 = currentModeBlock18 + 4;

        computeFaceNormal(
            currentModelBlockVertices->at(primitiveIndices[0]),
            currentModelBlockVertices->at(primitiveIndices[1]),
            currentModelBlockVertices->at(primitiveIndices[2]),
            (SVECTOR*)&(*currentModeBlock18)
        );
    }
    NormalColorCol((SVECTOR*)&(*currentModeBlock18), displayList, pNewPoly);
    currentModeBlock18 = currentModeBlock18 + 4;
    pNewPoly->code = READ_LE_U8(displayList + 3);
    return 1;
}

int prim1_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    if (primD_isValid(displayList))
    {
        //assert(READ_LE_U8(displayList + 3) == 0x24); // triangle with texture

        POLY_FT3* pNewPoly = new POLY_FT3;

        pNewPoly->m3_size = 7;
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        if ((initParam & 1) == 0) {
            if (initParam & 4) {
                //NormalColor(currentModeBlock18, pNewPoly);
                MissingCode();
            }
        }
        else
        {
            MissingCode();
        }
        currentModeBlock18 += 8;

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim2_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam) {
    MissingCode();
    return 1;
}

struct CVECTOR {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
};

void NormalColor3(SVECTOR* $2, SVECTOR* $3, SVECTOR* v2, CVECTOR* v3, CVECTOR* v4, CVECTOR* v5)
{
    MissingCode();
}

int prim3_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    if (primD_isValid(displayList))
    {
        //assert((READ_LE_U8(displayList + 3) & ~(16 | 2 | 1)) == 0x34);

        POLY_GT3* pNewPoly = new POLY_GT3;

        pNewPoly->m3_size = 9;

        SVECTOR* n0 = &currentModelBlockNormals->at(primitiveIndices[0]);
        SVECTOR* n1 = &currentModelBlockNormals->at(primitiveIndices[1]);
        SVECTOR* n2 = &currentModelBlockNormals->at(primitiveIndices[2]);

        NormalColor3(n0, n1, n2, (CVECTOR*)&pNewPoly->r0, (CVECTOR*)&pNewPoly->r1, (CVECTOR*)&pNewPoly->r2);

        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim4_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    //assert(READ_LE_U8(displayList + 3) == 0x20); // colored triangle

    POLY_F3* pNewPoly = new POLY_F3;

    pNewPoly->m3_size = 4;
    pNewPoly->r0 = READ_LE_U8(displayList + 0);
    pNewPoly->g0 = READ_LE_U8(displayList + 1);
    pNewPoly->b0 = READ_LE_U8(displayList + 2);
    pNewPoly->code = READ_LE_U8(displayList + 3);

    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;
    return 1;
}

int prim5_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    if (primD_isValid(displayList))
    {
        assert((READ_LE_U8(displayList + 3) & ~(16 | 2 | 1)) == 0x24);

        POLY_FT3* pNewPoly = new POLY_FT3;

        pNewPoly->m3_size = 7;
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim8_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    assert(READ_LE_U8(displayList + 3) == 0x28); // quad with color

    POLY_F4* pNewPoly = new POLY_F4;

    pNewPoly->m3_size = 4;
    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;

    if ((initParam & 1) == 0) {
        if ((initParam & 4) == 0) {
            pNewPoly->r0 = READ_LE_U8(displayList + 0);
            pNewPoly->g0 = READ_LE_U8(displayList + 1);
            pNewPoly->b0 = READ_LE_U8(displayList + 2);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }
        currentModeBlock18 = currentModeBlock18 + 4;
    }
    else
    {
        assert(0);
    }
    pNewPoly->code = READ_LE_U8(displayList + 3);
    return 1;
}

int prim9_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    if (primD_isValid(displayList))
    {
        assert(READ_LE_U8(displayList + 3) == 0x2C); // quad with texture

        POLY_FT4* pNewPoly = new POLY_FT4;

        pNewPoly->m3_size = 9;
        pNewPoly->r0 = READ_LE_U8(displayList + 0);
        pNewPoly->g0 = READ_LE_U8(displayList + 1);
        pNewPoly->b0 = READ_LE_U8(displayList + 2);
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 8);
        pNewPoly->v2 = READ_LE_U8(displayList + 9);
        pNewPoly->u3 = READ_LE_U8(displayList + 10);
        pNewPoly->v3 = READ_LE_U8(displayList + 11);

        currentModeBlock18 += 8;
        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}


int primC_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    //assert(READ_LE_U8(displayList + 3) == 0x28); // quad with color

    POLY_F4* pNewPoly = new POLY_F4;

    pNewPoly->m3_size = 5;
    pNewPoly->r0 = READ_LE_U8(displayList + 0);
    pNewPoly->g0 = READ_LE_U8(displayList + 1);
    pNewPoly->b0 = READ_LE_U8(displayList + 2);
    pNewPoly->code = READ_LE_U8(displayList + 3);

    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;
    return 1;
}

s32 screenClippingX = 319;
s32 screenClippingY = 238 << 0x10;

void initFieldDrawEnvsSub0(int param_1, int param_2)
{
    screenClippingX = param_1;
    screenClippingY = (param_2 + -1) * 0x10000;
    return;
}


OTTable::iterator currentOTEntry;

void genericTrianglePrim_14(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_F3* pOutputPrim = (POLY_F3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x14);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 4);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 4);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void genericTrianglePrim_28(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_GT3* pOutputPrim = (POLY_GT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 0xC);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void F4_FAKE(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count) {
    MissingCode();
    Hack("TODO: this is not correct and implemented as a shortcut");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_F4* pOutputPrim = (POLY_F4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    pOutputPrim->m3_size = 7;
                }
            }
        }
    }
}

void FT3_FAKE(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count) {
    MissingCode();
    Hack("TODO: this is not correct and implemented as a shortcut");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    pOutputPrim->m3_size = 7;
                }
            }
        }
    }
}

void genericTrianglePrim_20(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x20);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 7);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim5_0(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    genericTrianglePrim_20(primitiveIndices, count, 0x20, 0x7, 0x8);
}

void prim5_4(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count) {
    Noah_MissingCode("Incorrect prim5_4");
    genericTrianglePrim_20(primitiveIndices, count, 0x20, 0x7, 0x8);
}

void prim5_2generic(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x20);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 0x8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 7);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim3_0(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    genericTrianglePrim_28(primitiveIndices, count, 0x28, 0x9, 0xc);
}

void GT3_FAKE(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    Hack("Not how it's supposed to work");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_GT3* pOutputPrim = (POLY_GT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    //pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim4_0(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    genericTrianglePrim_14(primitiveIndices, count, 0x14, 0x4, 4);
}

void prim5_2(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    prim5_2generic(primitiveIndices, count, 0x20, 0x7, 0x8);
}

void prim6_2(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    prim5_2generic(primitiveIndices, count, 0x28, 0x9, 0xC);
}

void prim6_3(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    assert(0);
}

void primD_2generic(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT4* pOutputPrim = (POLY_FT4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        SVECTOR* pVertices4 = &currentModelBlockVertices->at((*primitiveIndices)[3]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                gte_ldv0(pVertices4); // load last vertex and compute 3rd point
                gte_rtps();

                int flagsTriangle1;
                gte_stlzc(&flagsTriangle1);

                sVec2_s16 xy3;
                gte_stsxy2(&xy3);

                if (flagsTriangle1 < 0)
                {
                    continue;
                }

                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;
                pOutputPrim->x3y3 = xy3;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void primD_2(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    primD_2generic(primitiveIndices, count, 0x28, 0x9, 0x8);
}

void primD_0generic(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT4* pOutputPrim = (POLY_FT4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        SVECTOR* pVertices1 = &currentModelBlockVertices->at((*primitiveIndices)[0]);
        SVECTOR* pVertices2 = &currentModelBlockVertices->at((*primitiveIndices)[1]);
        SVECTOR* pVertices3 = &currentModelBlockVertices->at((*primitiveIndices)[2]);
        SVECTOR* pVertices4 = &currentModelBlockVertices->at((*primitiveIndices)[3]);
        primitiveIndices++;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                gte_ldv0(pVertices4); // load last vertex and compute 3rd point
                gte_rtps();

                int flagsTriangle1;
                gte_stlzc(&flagsTriangle1);

                sVec2_s16 xy3;
                gte_stsxy2(&xy3);

                if (flagsTriangle1 < 0)
                {
                    continue;
                }

                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;
                pOutputPrim->x3y3 = xy3;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void primD_0(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    primD_0generic(primitiveIndices, count, 0x28, 0x9, 0x8);
}

void primD_1(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    Hack("hacked primD_1");
    primD_0generic(primitiveIndices, count, 0x28, 0x9, 0x8);
}


void primD_4(std::vector<std::array<u16, 4>>::const_iterator& primitiveIndices, int count)
{
    Noah_MissingCode("primD_4");
    primD_0generic(primitiveIndices, count, 0x28, 0x9, 0x8);
}

int primD_init(u8* displayList, const std::array<u16, 4>& primitiveIndices, int initParam)
{
    if (primD_isValid(displayList))
    {
        POLY_FT4* pNewPoly = new POLY_FT4;

        pNewPoly->m3_size = 9;
        pNewPoly->r0 = READ_LE_U8(displayList + 0);
        pNewPoly->g0 = READ_LE_U8(displayList + 1);
        pNewPoly->b0 = READ_LE_U8(displayList + 2);
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 8);
        pNewPoly->v2 = READ_LE_U8(displayList + 9);
        pNewPoly->u3 = READ_LE_U8(displayList + 10);
        pNewPoly->v3 = READ_LE_U8(displayList + 11);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

const std::array<sPolyTypeRenderDefinition, 17> polyRenderDefs = { {
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim0_init,	8,	0x4,	0x04}, // 0x0 POLY_FT3 with light
    {	prim5_0,	FT3_FAKE,	prim5_2,	nullptr,	nullptr,	nullptr,	prim1_init,	8,	0x8,	0x20}, // 0x1 POLY_FT3
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim2_init,	8,	0x4,	0x1C}, // 0x2
    {	prim3_0,	GT3_FAKE,	nullptr,	nullptr,	prim3_0,	prim3_0,	prim3_init,	8,	0x8,	0x28}, // 0x3 POLY_GT3
    {	prim4_0,	prim4_0,	nullptr,	nullptr,	prim4_0,	prim4_0,	prim4_init,	8,	0x4,	0x14}, // 0x4 POLY_F3
    {	prim5_0,	nullptr,	prim5_2,	nullptr,	prim5_4,	nullptr,	prim5_init,	8,	0x8,	0x20}, // 0x5 POLY_FT3
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x1C}, // 0x6
    {	prim3_0,	prim3_0,	prim6_2,	prim6_3,	prim3_0,	prim3_0,	prim3_init,	8,	0x8,	0x28}, // 0x7
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim8_init,	8,	0x4,	0x18}, // 0x8 POLY_F4
    {	nullptr,	primD_1,	nullptr,	nullptr,	nullptr,	nullptr,	prim9_init,	8,	0xC,	0x28}, // 0x9 POLY_FT4
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xA
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xB
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	primC_init,	8,	0x4,	0x18}, // 0xC POLY_F4
    {	primD_0,	nullptr,	primD_2,	nullptr,	primD_4,	nullptr,	primD_init,	8,	0xC,	0x28}, // 0xD POLY_FT4
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xE
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xF
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x20}, // 0x10
} };

bool submitModelForRendering(sModelBlock* param_1, std::vector<sTag*>& param_2, OTTable::iterator OT, int renderMode)
{
    if ((objectClippingMask != 0) && (isObjectClipped(param_1, objectClippingMask))) {
        return 0;
    }

    currentModeBlock18 = param_1->m18.data();
    currentModelBlockNormals = &param_1->mC_normals;
    currentModelBlockVertices = &param_1->m8_vertices;
    fieldPolyCount2 += param_1->m4_numPrims;
    currentModelInstanceDrawPrims = param_2.begin();
    currentOTEntry = OT;

    for(int i=0; i< param_1->m6_numMeshBlock; i++) {
        g_currentModelBlockSubBlocks = param_1->m10_meshBlocks.begin() + i;
        auto& currentModelBlockSubBlocks = param_1->m10_meshBlocks[i];
        int primType = currentModelBlockSubBlocks.m0_primType;
        t_primRenderFunc primRenderFunc = nullptr;
        switch (renderMode) {
        case 0:
            primRenderFunc = polyRenderDefs[primType].m0_type0;
            break;
        case 1:
            primRenderFunc = polyRenderDefs[primType].m4_type1;
            break;
        case 2:
            primRenderFunc = polyRenderDefs[primType].m8_type2;
            break;
        case 3:
            primRenderFunc = polyRenderDefs[primType].mC_type3;
            break;
        case 4:
            primRenderFunc = polyRenderDefs[primType].m10_type4;
            break;
        case 5:
            primRenderFunc = polyRenderDefs[primType].m14_type5;
            break;
        default:
            assert(0);
        }

        if (primRenderFunc)
        {
            auto it = currentModelBlockSubBlocks.m4_indices.begin();
            primRenderFunc(it, currentModelBlockSubBlocks.m2_primCount);
        }
        else
        {
            currentModelInstanceDrawPrims += currentModelBlockSubBlocks.m2_primCount;
        }
        assert(polyRenderDefs[primType].m1C_size == 0x8);
    }
    return 1;
}

int isObjectClipped(sModelBlock* param_1, uint param_2)
{
    MissingCode();
    return 0;
}
