#include "noahLib.h"
#include "battle.h"
#include "field/field.h" // for OT Table TODO: Remove
#include "damageDisplay.h"
#include "damageDisplay2.h"

u16 damageDisplayVar0 = 0;
u16 damageDisplayVar1 = 0;

struct sDamageDisplayTask : public sTaskHeaderPair {
    sDamageDisplayTask* m38_pNext;
    SVECTOR m40;
    VECTOR m48;
    VECTOR m58;
    sSpriteActorCore* m78;
    u8 m7C_direction;
    sColorAndCode m80_colorAndCode;
    s32 m88_timer;
    s32 m8C_damageStringLength;
    std::array<sDamageDisplaySub0, 10> m90;
    // size 0x1B0
};

sDamageDisplayTask* damageTaskDisplayListHead = nullptr;

void updateDamageDisplayTask_FadeOut(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    pTask->m80_colorAndCode.m0_r = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);
    pTask->m80_colorAndCode.m1_g = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);
    pTask->m80_colorAndCode.m2_b = addAndClamp(pTask->m80_colorAndCode.m0_r, -8);

    if ((--pTask->m88_timer < 0) || ((pTask->m80_colorAndCode.m0_r | pTask->m80_colorAndCode.m1_g | pTask->m80_colorAndCode.m2_b) == 0)) {
        param_1->mC_deleteCallback(param_1);
    }
}

void updateDamageDisplayTask2(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    // start fadeout
    if (--pTask->m88_timer < 0) {
        pTask->m88_timer = 0x10;
        pTask->m80_colorAndCode.m3_code = (pTask->m80_colorAndCode.m3_code & 0xfe) | 2;
        setTaskUpdateFunction(param_1, updateDamageDisplayTask_FadeOut);
    }
}

void updateDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    updateDamageDisplayPolys();

    if (pTask->m7C_direction == '\0') {
        pTask->m48.vx -= 0xc0000;
    }
    else {
        pTask->m48.vx += 0xc0000;
    }

    if (--pTask->m88_timer < 0) {
        pTask->m88_timer = 0x10;
        setTaskUpdateFunction(param_1, updateDamageDisplayTask2);
    }
}

void customRenderableEntityGenericDelete(sTaskHeaderPair* param_1) {
    registerSpriteCallback2Sub0(&param_1->m1C);
    allocateSavePointMeshDataSub0_callback(param_1);
    delete param_1;
}

void deleteDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    sDamageDisplayTask* psVar1;
    sDamageDisplayTask* psVar2;
    sDamageDisplayTask* psVar3;

    psVar1 = damageTaskDisplayListHead;
    psVar3 = nullptr;
    if (damageTaskDisplayListHead != nullptr) {
        do {
            psVar2 = psVar1;
            if (psVar2 == pTask) {
                if (psVar3 == nullptr) {
                    damageTaskDisplayListHead = psVar2->m38_pNext;
                }
                else {
                    psVar3->m38_pNext = psVar2->m38_pNext;
                }
                break;
            }
            psVar1 = psVar2->m38_pNext;
            psVar3 = psVar2;
        } while (psVar2->m38_pNext != nullptr);
    }
    customRenderableEntityGenericDelete(pTask);
}

static const std::array<u32, 9> damageDecimals = {
     10,          100,         1000,        10000,
     100000,      1000000,     10000000,    100000000,
     1000000000
};

void formatDamageString(uint param_1, sDamageString& damageString, int param_3, char param_4, char param_5) {
    int damageSign = (int)param_1 < 0;
    auto damageWritePtr = damageString.m1_string.begin();
    if ((bool)damageSign) {
        damageString.m1_string[0] = '-';
        auto damageWritePtr = damageString.m1_string.begin() + 1;
        param_1 = -param_1;
        param_3 = param_3 + -1;
    }
    auto decimalIt = damageDecimals.begin() + param_3;
    while (true) {
        param_1 = param_1 % *decimalIt;
        if (param_3 == 0) break;
        decimalIt = decimalIt + -1;
        param_3 = param_3 + -1;
        if ((param_1 / *decimalIt & 0xff) != 0) {
            param_4 = '\x01';
        }
        if (param_4 != '\0') {
            *damageWritePtr = (char)(param_1 / *decimalIt) + param_5;
            damageWritePtr = damageWritePtr + 1;
            damageSign = damageSign + '\x01';
        }
    }
    *damageWritePtr = (char)param_1 + param_5;
    damageString.m0_length = damageSign + '\x01';
    return;
}

const std::array<s16, 5> damageDysplayOffsetBySize = {
    -4,     -8,    -12,    -16, -20
};

int setupDamagePoly(sFont fontData, int character, sDamageDisplaySub0* param_3, short param_4, short param_5) {
    for (int i = 0; i < fontData.m4[character].m0_polyCount; i++) {
        sFontGlyphPoly& poly = fontData.m4[character].m4_polys[i];
        int textureU = poly.m0_textureU << 0x10;
        if (poly.m10_colorDepth == 0) {
            textureU >>= 0x14;
        }
        else {
            textureU >>= 0x12;
        }
        param_3->mC_clut = GetClut(poly.m12_clutX, poly.m14_clutY);
        param_3->mA_tpage = GetTPage(poly.m10_colorDepth, 1, (poly.m16_tpageX & 0xFFC0) + textureU, (poly.m18_tpageY & 0xFF00) + poly.m2_textureV);
        param_3->m4_U = poly.m0_textureU;
        param_3->m5_V = poly.m2_textureV;
        param_3->m6_width = poly.m4_textureWidth;
        param_3->m7_height = poly.m6_textureHeight;
        param_3->m8 = poly.mC;

        param_3->m0_screenPosition[0] = poly.m8_width + param_4;
        param_3->m0_screenPosition[1] = poly.mA_height + param_5;
    }

    return fontData.m4[character].m0_polyCount;
}

extern u8* shapeTransfertTableCurrentEntry;
extern u8* shapeTransfertTableEnd;
extern SFP_VEC4 currentSpriteCharacterSize[4];

void drawDamageDisplayCharacter(sDamageDisplaySub0* param_1, sColorAndCode colorAndCode) {
    if ((shapeTransfertTableCurrentEntry + sizeof(POLY_FT4) < shapeTransfertTableEnd)) {
        POLY_FT4* pPoly = new(shapeTransfertTableCurrentEntry) POLY_FT4;
        shapeTransfertTableCurrentEntry += sizeof(POLY_FT4);

        pPoly->m3_size = 9;
        pPoly->m_colorAndCode = colorAndCode;
        pPoly->tpage = param_1->mA_tpage;
        pPoly->clut = param_1->mC_clut;

        currentSpriteCharacterSize[0].vx = param_1->m0_screenPosition[0];
        currentSpriteCharacterSize[0].vy = param_1->m0_screenPosition[1];
        currentSpriteCharacterSize[1].vx = currentSpriteCharacterSize[0].vx + (ushort)param_1->m6_width;
        currentSpriteCharacterSize[2].vy = currentSpriteCharacterSize[0].vy + (ushort)param_1->m7_height;
        currentSpriteCharacterSize[1].vy = currentSpriteCharacterSize[0].vy;
        currentSpriteCharacterSize[2].vx = currentSpriteCharacterSize[1].vx;
        currentSpriteCharacterSize[3].vx = currentSpriteCharacterSize[0].vx;
        currentSpriteCharacterSize[3].vy = currentSpriteCharacterSize[2].vy;

        long lStack18;
        long lStack14;
        RotTransPers4(&currentSpriteCharacterSize[0], &currentSpriteCharacterSize[1], &currentSpriteCharacterSize[2], &currentSpriteCharacterSize[3], &pPoly->x0y0, &pPoly->x1y1, &pPoly->x3y3, &pPoly->x2y2, &lStack18, &lStack14);

        pPoly->v1 = pPoly->v0 = param_1->m5_V;
        pPoly->v3 = pPoly->v2 = param_1->m5_V + param_1->m7_height;
        pPoly->u2 = pPoly->u0 = param_1->m4_U;
        pPoly->u3 = pPoly->u1 = param_1->m4_U + param_1->m6_width;

        AddPrim(&(*characterRenderingOT)[0], pPoly);
    }
}

MATRIX damageDisplayMatrix = {
    {{
        {0x1000, 0, 0},
        {0, 0x1000, 0},
        {0, 0, 0x1000}
    }},
    {0,0,0x200}
};

void setupDamageDisplayMatrix(SVECTOR* param_1, VECTOR* param_2) {
    long lVar1;
    sVec2_s16 local_28;
    long lStack_20;
    long lStack_1c;
    int local_18;
    int local_14;

    SetRotMatrix(&battleRenderingMatrix);
    SetTransMatrix(&battleRenderingMatrix);
    RotTransPers(param_1, &local_28, &lStack_20, &lStack_1c);
    ReadGeomOffset(&local_18, &local_14);
    param_2->vx = (local_28.vx - local_18) * 2;
    param_2->vy = (local_28.vy - local_14) * 2;
    lVar1 = ReadGeomScreen();
    param_2->vz = lVar1;
    return;
}

void drawDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    sDamageDisplayTask* pOwner = (sDamageDisplayTask*)pTask->m4;
    damageDisplayMatrix.t[2] = ReadGeomScreen();

    SVECTOR local_30;
    local_30.vx = pOwner->m48.vx.getIntegerPart();
    local_30.vy = pOwner->m48.vy.getIntegerPart();
    local_30.vz = pOwner->m48.vz.getIntegerPart();

    VECTOR VStack_28;
    setupDamageDisplayMatrix(&local_30, &VStack_28);

    MATRIX MStack_50;
    createRotationMatrix(&pOwner->m40, &MStack_50);
    TransMatrix(&MStack_50, &VStack_28);
    CompMatrix(&damageDisplayMatrix, &MStack_50, &MStack_50);
    ScaleMatrix(&MStack_50, &pOwner->m58);
    SetRotMatrix(&MStack_50);
    SetTransMatrix(&MStack_50);

    for (int i = 0; i < pOwner->m8C_damageStringLength; i++) {
        drawDamageDisplayCharacter(&pOwner->m90[i], pOwner->m80_colorAndCode);
    }
}

void createDamageDisplay(sSpriteActorCore* param_1, int damageValue, int damageType) {
    if (jumpAnimationControlStruct == nullptr) {
        return;
    }

    createDamageDisplayPolysTask2();

    MissingCode();

    {
        sDamageDisplayTask* pCurrentDamageTask = damageTaskDisplayListHead;
        while (pCurrentDamageTask) {
            assert(0);
        }
    }

    sDamageDisplayTask* pNewDamageTask = createCustomRenderableEntity<sDamageDisplayTask>(0x1b0, nullptr, updateDamageDisplayTask, drawDamageDisplayTask, deleteDamageDisplayTask);
    pNewDamageTask->m78 = param_1;
    pNewDamageTask->m88_timer = 8;
    pNewDamageTask->m38_pNext = damageTaskDisplayListHead;
    damageTaskDisplayListHead = pNewDamageTask;
    pNewDamageTask->m48.vx = param_1->m0_position.vx;
    pNewDamageTask->m48.vy = param_1->m0_position.vy;
    pNewDamageTask->m48.vz = param_1->m0_position.vz;

    pNewDamageTask->m58[0] = 0x2000;
    pNewDamageTask->m58[1] = 0x2000;
    pNewDamageTask->m58[2] = 0x2000;
    pNewDamageTask->m40[0] = 0;
    pNewDamageTask->m40[1] = 0;
    pNewDamageTask->m40[2] = 0;
    (pNewDamageTask->m80_colorAndCode).m0_r = 0x80;
    (pNewDamageTask->m80_colorAndCode).m1_g = 0x80;
    (pNewDamageTask->m80_colorAndCode).m2_b = 0x80;
    (pNewDamageTask->m80_colorAndCode).m3_code = 0x2d;
    pNewDamageTask->m8C_damageStringLength = 0;
    pNewDamageTask->m7C_direction = (param_1->mAC >> 2) & 1;

    sDamageString damageString;
    int displayOffset;

    if (damageType != 4) {
        formatDamageString(damageValue, damageString, 5, 0, 0);
        displayOffset = damageDysplayOffsetBySize[-1 + damageString.m0_length];
    }

    switch (damageType) {
    case 0:
        break;
    case 5:
        (pNewDamageTask->m80_colorAndCode).m0_r = 0x80;
        (pNewDamageTask->m80_colorAndCode).m1_g = 0;
        (pNewDamageTask->m80_colorAndCode).m2_b = 0;
        (pNewDamageTask->m80_colorAndCode).m3_code &= ~1;
        break;
    default:
        assert(0);
        break;
    }

    if (damageType != 4) {
        for (int i = 0; i < damageString.m0_length; i++) {
            pNewDamageTask->m8C_damageStringLength += setupDamagePoly(battleFont, damageString.m1_string[i] + 0x72, &pNewDamageTask->m90[pNewDamageTask->m8C_damageStringLength], displayOffset, -16);
            displayOffset += 10;
        }
    }
}

void createDamageDisplayTask3(int damageValue) {
    MissingCode();
}


void updateDamageDisplayPolys() {
    if (pDamageDisplayPolysTask2 == nullptr) {
        return;
    }

    if (damageDisplayPolysTask2Disabled != '\0') {
        return;
    }
    if (damageDisplayVar0 == createDamageDisplayPolysTask2Var0) {
        return;
    }
    createDamageDisplayPolysTask2Var0 = damageDisplayVar0;

    createDamageDisplayTask3(damageDisplayVar0);

    sDamageDisplayPoly* pTarget = pDamageDisplayPolysTask2;
    pTarget->m50[0].m10.m3_code = 0x2D;
    (pTarget->m40_pos).vx = 0x2000;
    (pTarget->m40_pos).vy = 0x2000;
    (pTarget->m40_pos).vz = 0x2000;

    pTarget->m50[0].m10.m3_code = 0x2c;
    (pTarget->m38_transformedPos).vx = 0;
    (pTarget->m38_transformedPos).vy = 0;
    (pTarget->m38_transformedPos).vz = 0;

    switch (damageDisplayVar1) {
    case 0:
    case 5:
        pTarget->m50[0].m10.m0_r = 0x80;
        pTarget->m50[0].m10.m1_g = 0x80;
        pTarget->m50[0].m10.m2_b = 0x80;
        break;
    default:
        assert(0);
    }

    sDamageString local_20;
    formatDamageString(damageDisplayVar0, local_20, 5, 0, 0);

    pTarget->m50[0].m14 = 0;

    int X = damageDysplayOffsetBySize[local_20.m0_length];
    int Y = -8;

    for (int i = 0; i < local_20.m0_length; i++) {
        setupDamagePoly(battleFont, 0x72 + local_20.m1_string[i], &pTarget->m50[pTarget->m50[0].m14 + 1], X, Y);
        X += 10;
    }
}