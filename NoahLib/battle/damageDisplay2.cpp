#include "noahLib.h"
#include "battle.h"

#include "damageDisplay2.h"
#include "damageDisplay.h"

/*
This task is responsible for displaying the "Total damage" text on screen during attack phase.
*/

extern OTTable* characterRenderingOT;
extern MATRIX damageDisplayMatrix;

struct sDamageDisplayPoly {
    sTaskHeader m0;
    sTaskHeader m1C;

    SVECTOR m38_transformedPos;
    VECTOR m40_pos;

    std::array<sDamageDisplaySub0,2> m50;
} damageDisplayPolyTask2;
sDamageDisplayPoly* pDamageDisplayPolysTask2 = nullptr;

u8 damageDisplayPolysTask2Disabled = 0;

void updateDamageDisplayPolysTask2(sTaskHeader* param_1) {
    // nothing
}

extern u8* shapeTransfertTableCurrentEntry;
extern u8* shapeTransfertTableEnd;

void setupTotalDamageDisplayPoly(sFont& font, int character, int X, int Y, OTTable* pOT) {
    sFontGlyph& pGlyph = font.m4[character];
    if ((shapeTransfertTableCurrentEntry + pGlyph.m0_polyCount * sizeof(POLY_FT4) < shapeTransfertTableEnd)) {
        for (int i = 0; i < pGlyph.m0_polyCount; i++) {
            sFontGlyphPoly& glyphPoly = pGlyph.m4_polys[i];

            POLY_FT4* pPoly = new(shapeTransfertTableCurrentEntry) POLY_FT4;
            shapeTransfertTableCurrentEntry += sizeof(POLY_FT4);

            pPoly->m3_size = 9;
            pPoly->m_colorAndCode.m3_code = 0x2D;

            int textureU;
            if (glyphPoly.m10_colorDepth == 0) {
                textureU = (glyphPoly.m0_textureU << 0x10) >> 0x14;
            }
            else {
                textureU = (glyphPoly.m0_textureU << 0x10) >> 0x12;
            }

            pPoly->clut = GetClut(glyphPoly.m12_clutX, glyphPoly.m14_clutY);
            pPoly->tpage = GetTPage(glyphPoly.m10_colorDepth, 0, (glyphPoly.m16_tpageX & 0xFFC0) + textureU, (glyphPoly.m18_tpageY & 0xFF00) + glyphPoly.m2_textureV);

            pPoly->x0 = X + glyphPoly.m8_width;
            pPoly->v0 = glyphPoly.m2_textureV;
            pPoly->v1 = glyphPoly.m2_textureV;
            pPoly->x1 = X + glyphPoly.m8_width + glyphPoly.m4_textureWidth;
            pPoly->x2 = X + glyphPoly.m8_width;
            pPoly->x3 = X + glyphPoly.m8_width + glyphPoly.m4_textureWidth;
            pPoly->v2 = glyphPoly.m2_textureV + glyphPoly.m6_textureHeight;
            pPoly->v3 = glyphPoly.m2_textureV + glyphPoly.m6_textureHeight;
            pPoly->y0 = glyphPoly.mA_height + Y;
            pPoly->y1 = glyphPoly.mA_height + Y;
            pPoly->y2 = glyphPoly.mA_height + Y + glyphPoly.m6_textureHeight;
            pPoly->y3 = glyphPoly.mA_height + Y + glyphPoly.m6_textureHeight;
            pPoly->u0 = glyphPoly.m0_textureU;
            pPoly->u1 = glyphPoly.m0_textureU + glyphPoly.m4_textureWidth;
            pPoly->u2 = glyphPoly.m0_textureU;
            pPoly->u3 = glyphPoly.m0_textureU + glyphPoly.m4_textureWidth;

            AddPrim(&(*pOT)[0], pPoly);
        }
    }
}

void drawDamageDisplayPolysTask2(sTaskHeader* param_1) {
    sDamageDisplayPoly* pThis = (sDamageDisplayPoly*)param_1->m4;

    int geomOffset[2];
    ReadGeomOffset(&geomOffset[0], &geomOffset[1]);
    setupTotalDamageDisplayPoly(battleFont, 0x81, pThis->m50[0].m0_screenPosition[0], pThis->m50[0].m0_screenPosition[1], characterRenderingOT);

    VECTOR local_28;
    local_28.vx = (0xa0 - geomOffset[0]) * 2;
    local_28.vy = (0x46 - geomOffset[1]) * 2;
    local_28.vz = ReadGeomScreen();
    damageDisplayMatrix.t[2] = ReadGeomScreen();

    MATRIX MStack_50;
    createRotationMatrix(&pThis->m38_transformedPos, &MStack_50);
    TransMatrix(&MStack_50, &local_28);
    CompMatrix(&damageDisplayMatrix, &MStack_50, &MStack_50);
    ScaleMatrix(&MStack_50, &pThis->m40_pos);
    SetRotMatrix(&MStack_50);
    SetTransMatrix(&MStack_50);

    for (int i = 0; i < pThis->m50[0].m14; i++) {
        drawDamageDisplayCharacter(&pThis->m50[i], pThis->m50[0].m10);
    }
}

void deleteDamageDisplayPolysTask2(sTaskHeader* param_1) {
    sDamageDisplayPoly* pThis = (sDamageDisplayPoly*)param_1->m4;

    pDamageDisplayPolysTask2 = nullptr;
    registerSpriteCallback2Sub0(&pThis->m1C);
    allocateSavePointMeshDataSub0_callback(param_1);
}

int createDamageDisplayPolysTask2Var0 = 0;

void createDamageDisplayPolysTask2() {
    if ((pDamageDisplayPolysTask2 == nullptr) && !damageDisplayPolysTask2Disabled) {
        pDamageDisplayPolysTask2 = &damageDisplayPolyTask2;
        allocateSavePointMeshDataSub0(nullptr, &damageDisplayPolyTask2.m0);
        registerSpriteCallback2(&damageDisplayPolyTask2.m0, &damageDisplayPolyTask2.m1C);
        setTaskUpdateFunction(&damageDisplayPolyTask2.m0, updateDamageDisplayPolysTask2);
        setTaskDrawFunction(&damageDisplayPolyTask2.m1C, drawDamageDisplayPolysTask2);
        setTaskDeleteFunction(&damageDisplayPolyTask2.m0, deleteDamageDisplayPolysTask2);
        damageDisplayPolyTask2.m50[0].m0_screenPosition[0] = 0x90;
        damageDisplayPolyTask2.m50[0].m0_screenPosition[1] = 0x2a;
        damageDisplayPolyTask2.m0.m4 = (sSpriteActorCore*)&damageDisplayPolyTask2;
        damageDisplayPolyTask2.m1C.m4 = (sSpriteActorCore*)&damageDisplayPolyTask2;
        damageDisplayPolyTask2.m50[0].m14 = 0;
        createDamageDisplayPolysTask2Var0 = -1;
    }
}

s32 initBattleUnkData0Var1 = 0;

void initBattleUnkData0() {
    pDamageDisplayPolysTask2 = 0;
    initBattleUnkData0Var1 = 0;
}

bool isDamageDisplayPolysTask2Running() {
    return pDamageDisplayPolysTask2 != 0;
}
