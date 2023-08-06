#include "noahLib.h"
#include "battle.h"

#include "damageDisplay.h"
#include "damageDisplay2.h"

u16 damageDisplayVar0 = 0;
u16 damageDisplayVar1 = 0;

struct sDamageDisplaySub0 {
    std::array<s16, 2> m0_screenPosition;
    u8 m4_U;
    u8 m5_V;
    u8 m6_width;
    u8 m7_height;
    u8 m8;
    u16 mA_tpage;
    u16 mC_clut;
    // 0x18
};

struct sDamageDisplayTask {
    sTaskHeader m0;
    sTaskHeader m1C;

    sDamageDisplayTask* m38_pNext;
    SVECTOR m40;
    VECTOR m48;
    std::array<s32, 3> m58;
    sSpriteActorCore* m78;
    u8 m7C_direction;
    sColorAndCode m80_colorAndCode;
    s32 m88_timer;
    s32 m8C_damageStringLength;
    std::array<sDamageDisplaySub0, 10> m90;
    // size 0x1B0
};

sDamageDisplayTask* damageTaskDisplayListHead = nullptr;

void updateDamageDisplayPolys() {
    MissingCode();
}

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

void deleteDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;
    MissingCode();
}

static const std::array<u32, 9> damageDecimals = {
     10,          100,         1000,        10000,
     100000,      1000000,     10000000,    100000000,
     1000000000
};

struct sDamageString {
    char m0_length;
    std::array<char, 7> m1_string;
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

int setupDamagePoly(std::vector<u8>& fontData, int character, sDamageDisplaySub0* param_3, short param_4, short param_5) {
    std::vector<u8>::iterator characterData = fontData.begin() + READ_LE_U16(fontData.begin() + character * 2 + 4);
    u16 count = READ_LE_U16(characterData);
    std::vector<u8>::iterator characterData3 = characterData + 2 * 2;
    for (int i = 0; i < count; i++) {
        characterData = characterData + 7 * 2;
        int tpageX;
        if (READ_LE_U16(characterData + 3) == 0) {
            tpageX = (READ_LE_U16(characterData3) << 0x10) >> 0x14;
        }
        else {
            tpageX = (READ_LE_U16(characterData3) << 0x10) >> 0x12;
        }
        u16 XInTpage = READ_LE_U16(characterData + 6 * 2);
        u16 YInTpage = READ_LE_U16(characterData + 7 * 2);
        u16 tp = READ_LE_U16(characterData + 3 * 2);
        u16 tpageY = READ_LE_U16(characterData -1 * 2);
        param_3->mC_clut = GetClut(READ_LE_U16(characterData + 4 * 2), READ_LE_U16(characterData + 5 * 2));
        param_3->mA_tpage = GetTPage(tp, 1, (XInTpage & 0xFFC0) + tpageX, (YInTpage & 0xFF00) + tpageY);
        param_3->m4_U = READ_LE_U8(characterData3);
        param_3->m5_V = READ_LE_U8(characterData - 4 * 2);
        param_3->m6_width = READ_LE_U8(characterData - 4 * 2);
        param_3->m7_height = READ_LE_U8(characterData - 3 * 2);
        param_3->m8 = READ_LE_U8(characterData - 2 * 2);

        characterData3 += 0xE * 2;
        param_3->m0_screenPosition[0] = READ_LE_U16(characterData - 1 * 2) + param_4;
        param_3->m0_screenPosition[1] = READ_LE_U16(characterData) + param_5;
        characterData += 0xE * 2;
    }

    return count;
}

extern u8* shapeTransfertTableCurrentEntry;
extern u8* shapeTransfertTableEnd;

void drawDamageDisplayCharacter(sDamageDisplaySub0* param_1, sColorAndCode colorAndCode) {
    if ((shapeTransfertTableCurrentEntry + sizeof(POLY_FT4) < shapeTransfertTableEnd)) {
        POLY_FT4* pPoly = (POLY_FT4*)shapeTransfertTableCurrentEntry;
        shapeTransfertTableCurrentEntry += sizeof(POLY_FT4);

        pPoly->m3_size = 9;
        pPoly->m_colorAndCode = colorAndCode;
        assert(0);
    }
}

void drawDamageDisplayTask(sTaskHeader* param_1) {
    sDamageDisplayTask* pTask = (sDamageDisplayTask*)param_1;

    sDamageDisplayTask* pOwner = (sDamageDisplayTask*)pTask->m0.m4;

    MissingCode();
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
        }
    }
}

