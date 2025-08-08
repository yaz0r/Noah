#include "noahLib.h"
#include "kernel/text.h"
#include "kernel/font.h"
#include "psx/libgpu.h"

int getDialogFontCharacterWidth(uint param_1, uint param_2)
{
    bool bVar1;
    int iVar2;

    if ((param_1 & 0xffff) == 0) {
        bVar1 = (int)((param_2 & 0xffff) - dialogFontVar1) < dialogFontVar2;
    }
    else {
        if ((param_1 & 0xffff) != dialogFontVar0) {
            return 3;
        }
        bVar1 = (int)(param_2 & 0xffff) < dialogFontVar3;
    }
    iVar2 = 3;
    if (bVar1) {
        iVar2 = 2;
    }
    return iVar2;
}

void printDialogCharacter(ushort characterType, ushort characterId, ushort* outputBuffer, short outputBufferStride, int oddOrEvenLine)
{
    {
        std::string decodedString;
        if (characterId == 0x10)
        {
            decodedString += ' ';
        }
        else if ((characterId >= 0x16) && (characterId <= 0x1F))
        {
            decodedString += (char)(characterId - 0x16 + '0');
        }
        else if ((characterId >= 0x20) && (characterId <= 0x39))
        {
            decodedString += (char)(characterId - 0x20 + 'A');
        }
        else if ((characterId >= 0x3D) && (characterId <= 0x56))
        {
            decodedString += (char)(characterId - 0x3D + 'a');
        }
    }

    u16* pCharacterData = nullptr;
    if (characterType == 0) {
        assert(characterId >= dialogFontVar1);
        pCharacterData = (ushort*)(dialogFontVarPtr2 + ((uint)characterId - dialogFontVar1) * 0x16);
    }
    else {
        assert(0);
    }

    ushort uVar1;
    ushort uVar2;
    int iVar3;
    ushort uVar4;
    ushort* puVar5;
    ushort* puVar6;
    ushort* puVar7;
    ushort* puVar8;
    int iVar9;

    iVar9 = 0;
    if (oddOrEvenLine == 0) {
        outputBuffer[2] = outputBuffer[2] & 0xcccc;
        *outputBuffer = *outputBuffer & 0xcccc;
        iVar3 = (int)outputBufferStride;
        outputBuffer[1] = outputBuffer[1] & 0xcccc;
        puVar5 = outputBuffer + iVar3;
        puVar8 = puVar5 + 2;
        puVar7 = puVar5 + -iVar3;
        *puVar5 = *puVar5 & 0xcccc;
        puVar5[2] = puVar5[2] & 0xcccc;
        puVar5[1] = puVar5[1] & 0xcccc;
        puVar6 = puVar5;
        do {
            puVar6 = puVar6 + iVar3;
            *puVar6 = *puVar6 & 0xcccc;
            puVar6[2] = puVar6[2] & 0xcccc;
            puVar6[1] = puVar6[1] & 0xcccc;
            uVar1 = *pCharacterData;
            uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x222;
            pCharacterData = pCharacterData + 1;
            if ((uVar1 & 0x40) != 0) {
                uVar2 = uVar2 | 0x2220;
            }
            if ((uVar1 & 0x20) != 0) {
                uVar2 = uVar2 | 0x2200;
            }
            uVar4 = uVar2 | 0x2000;
            if ((uVar1 & 0x10) == 0) {
                uVar4 = uVar2;
            }
            *puVar7 = *puVar7 | uVar4;
            uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x212;
            *puVar6 = *puVar6 | uVar4;
            if ((uVar1 & 0x40) != 0) {
                uVar2 = uVar2 | 0x2120;
            }
            if ((uVar1 & 0x20) != 0) {
                uVar2 = uVar2 | 0x1200;
            }
            uVar4 = *puVar5;
            if ((uVar1 & 0x10) != 0) {
                uVar4 = uVar4 | 0x2000;
            }
            *puVar5 = uVar4 | uVar2;
            uVar2 = 0x222;
            if ((uVar1 & 8) == 0) {
                if ((uVar1 & 0x10) == 0) {
                    uVar2 = uVar1 >> 4 & 2;
                }
                else {
                    uVar2 = 0x22;
                }
            }
            if ((uVar1 & 4) != 0) {
                uVar2 = uVar2 | 0x2220;
            }
            if ((uVar1 & 2) != 0) {
                uVar2 = uVar2 | 0x2200;
            }
            uVar4 = uVar2 | 0x2000;
            if ((uVar1 & 1) == 0) {
                uVar4 = uVar2;
            }
            puVar7[1] = puVar7[1] | uVar4;
            puVar6[1] = puVar6[1] | uVar4;
            uVar2 = uVar1 >> 4 & 2;
            if ((uVar1 & 0x10) != 0) {
                uVar2 = uVar2 | 0x21;
            }
            if ((uVar1 & 8) != 0) {
                uVar2 = uVar2 | 0x212;
            }
            if ((uVar1 & 4) != 0) {
                uVar2 = uVar2 | 0x2120;
            }
            if ((uVar1 & 2) != 0) {
                uVar2 = uVar2 | 0x1200;
            }
            uVar4 = puVar8[-1];
            if ((uVar1 & 1) != 0) {
                uVar4 = uVar4 | 0x2000;
            }
            puVar8[-1] = uVar4 | uVar2;
            uVar2 = 0x222;
            if (((uVar1 & 0x8000) == 0) && (uVar2 = 0x22, (uVar1 & 1) == 0)) {
                uVar2 = uVar1 & 2;
            }
            if ((uVar1 & 0x4000) != 0) {
                uVar2 = uVar2 | 0x2220;
            }
            if ((uVar1 & 0x2000) != 0) {
                uVar2 = uVar2 | 0x2200;
            }
            uVar4 = uVar2 | 0x2000;
            if ((uVar1 & 0x1000) == 0) {
                uVar4 = uVar2;
            }
            puVar7[2] = puVar7[2] | uVar4;
            puVar6[2] = puVar6[2] | uVar4;
            uVar2 = uVar1 & 2;
            if ((uVar1 & 1) != 0) {
                uVar2 = uVar2 | 0x21;
            }
            if ((uVar1 & 0x8000) != 0) {
                uVar2 = uVar2 | 0x212;
            }
            if ((uVar1 & 0x4000) != 0) {
                uVar2 = uVar2 | 0x2120;
            }
            if ((uVar1 & 0x2000) != 0) {
                uVar2 = uVar2 | 0x1200;
            }
            uVar4 = *puVar8;
            if ((uVar1 & 0x1000) != 0) {
                uVar4 = uVar4 | 0x2000;
            }
            *puVar8 = uVar4 | uVar2;
            puVar8 = puVar8 + iVar3;
            puVar7 = puVar7 + iVar3;
            iVar9 = iVar9 + 1;
            puVar5 = puVar5 + iVar3;
        } while (iVar9 < 0xb);
    }
    else {
        outputBuffer[2] = outputBuffer[2] & 0x3333;
        *outputBuffer = *outputBuffer & 0x3333;
        iVar3 = (int)outputBufferStride;
        outputBuffer[1] = outputBuffer[1] & 0x3333;
        puVar5 = outputBuffer + iVar3;
        puVar8 = puVar5 + 2;
        puVar7 = puVar5 + -iVar3;
        *puVar5 = *puVar5 & 0x3333;
        puVar5[2] = puVar5[2] & 0x3333;
        puVar5[1] = puVar5[1] & 0x3333;
        puVar6 = puVar5;
        do {
            puVar6 = puVar6 + iVar3;
            *puVar6 = *puVar6 & 0x3333;
            puVar6[2] = puVar6[2] & 0x3333;
            puVar6[1] = puVar6[1] & 0x3333;
            uVar1 = *pCharacterData;
            uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x888;
            pCharacterData = pCharacterData + 1;
            if ((uVar1 & 0x40) != 0) {
                uVar2 = uVar2 | 0x8880;
            }
            if ((uVar1 & 0x20) != 0) {
                uVar2 = uVar2 | 0x8800;
            }
            uVar4 = uVar2 | 0x8000;
            if ((uVar1 & 0x10) == 0) {
                uVar4 = uVar2;
            }
            *puVar7 = *puVar7 | uVar4;
            uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x848;
            *puVar6 = *puVar6 | uVar4;
            if ((uVar1 & 0x40) != 0) {
                uVar2 = uVar2 | 0x8480;
            }
            if ((uVar1 & 0x20) != 0) {
                uVar2 = uVar2 | 0x4800;
            }
            uVar4 = *puVar5;
            if ((uVar1 & 0x10) != 0) {
                uVar4 = uVar4 | 0x8000;
            }
            *puVar5 = uVar4 | uVar2;
            uVar2 = 0x888;
            if ((uVar1 & 8) == 0) {
                if ((uVar1 & 0x10) == 0) {
                    uVar2 = uVar1 >> 2 & 8;
                }
                else {
                    uVar2 = 0x88;
                }
            }
            if ((uVar1 & 4) != 0) {
                uVar2 = uVar2 | 0x8880;
            }
            if ((uVar1 & 2) != 0) {
                uVar2 = uVar2 | 0x8800;
            }
            uVar4 = uVar2 | 0x8000;
            if ((uVar1 & 1) == 0) {
                uVar4 = uVar2;
            }
            puVar7[1] = puVar7[1] | uVar4;
            puVar6[1] = puVar6[1] | uVar4;
            uVar2 = uVar1 >> 2 & 8;
            if ((uVar1 & 0x10) != 0) {
                uVar2 = uVar2 | 0x84;
            }
            if ((uVar1 & 8) != 0) {
                uVar2 = uVar2 | 0x848;
            }
            if ((uVar1 & 4) != 0) {
                uVar2 = uVar2 | 0x8480;
            }
            if ((uVar1 & 2) != 0) {
                uVar2 = uVar2 | 0x4800;
            }
            uVar4 = puVar8[-1];
            if ((uVar1 & 1) != 0) {
                uVar4 = uVar4 | 0x8000;
            }
            puVar8[-1] = uVar4 | uVar2;
            uVar2 = 0x888;
            if ((uVar1 & 0x8000) == 0) {
                if ((uVar1 & 1) == 0) {
                    uVar2 = (ushort)((uVar1 & 2) << 2);
                }
                else {
                    uVar2 = 0x88;
                }
            }
            if ((uVar1 & 0x4000) != 0) {
                uVar2 = uVar2 | 0x8880;
            }
            if ((uVar1 & 0x2000) != 0) {
                uVar2 = uVar2 | 0x8800;
            }
            uVar4 = uVar2 | 0x8000;
            if ((uVar1 & 0x1000) == 0) {
                uVar4 = uVar2;
            }
            puVar7[2] = puVar7[2] | uVar4;
            puVar6[2] = puVar6[2] | uVar4;
            uVar2 = (ushort)((uVar1 & 2) << 2);
            if ((uVar1 & 1) != 0) {
                uVar2 = uVar2 | 0x84;
            }
            if ((uVar1 & 0x8000) != 0) {
                uVar2 = uVar2 | 0x848;
            }
            if ((uVar1 & 0x4000) != 0) {
                uVar2 = uVar2 | 0x8480;
            }
            if ((uVar1 & 0x2000) != 0) {
                uVar2 = uVar2 | 0x4800;
            }
            uVar4 = *puVar8;
            if ((uVar1 & 0x1000) != 0) {
                uVar4 = uVar4 | 0x8000;
            }
            *puVar8 = uVar4 | uVar2;
            puVar8 = puVar8 + iVar3;
            puVar7 = puVar7 + iVar3;
            iVar9 = iVar9 + 1;
            puVar5 = puVar5 + iVar3;
        } while (iVar9 < 0xb);
    }
}

const std::array<u8, 8> dpadButtonMapping = { {
    0,1,2,3,4,5,6,7
} };

std::array<s16, 11> dialogSpecialCase9AndATable;
s16 dialogSpecialCase9AndAVar0;

std::vector<u8> dialogSpecialCase9AndA_2Data = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void dialogSpecialCase9AndASub0(s16* param_1)
{
    auto pcVar4 = dialogSpecialCase9AndA_2Data.begin();
    auto iVar5 = printDialogTextVar[27];

    ushort uVar1 = *(param_1++);
    while (uVar1 != 0xffff) {
        auto pcVar3 = ((uint)uVar1 * 2 + iVar5);
        if (*pcVar3 == '\0') {
            *pcVar4 = pcVar3[1];
        }
        else {
            *pcVar4 = *pcVar3;
            pcVar4 = pcVar4 + 1;
            *pcVar4 = pcVar3[1];

        }
        pcVar4 = pcVar4 + 1;
        uVar1 = *(param_1++);
    }
    *pcVar4 = 0;
    return;
}

void dialogSpecialCase9AndA(s32 param_1, int param_2, int param_3) {

    uint uVar3;
    uint uVar4;
    int iVar5;
    uint uVar6;

    uVar4 = 1000000000;
    uVar3 = param_2 * 0x10;
    if ((param_3 != 0) && (param_3 = 0xb, (int)param_1 < 0)) {
        param_1 = -param_1;
        param_3 = 10;
    }
    iVar5 = 0;
    std::array<s16, 11>::iterator psVar1 = dialogSpecialCase9AndATable.begin();
    do {
        uVar6 = param_1 / uVar4;
        param_1 = param_1 % uVar4;
        iVar5 = iVar5 + 1;
        uVar4 = uVar4 / 10;
        dialogSpecialCase9AndAVar0 = (short)uVar3;
        *psVar1 = (short)uVar6 + dialogSpecialCase9AndAVar0;
        psVar1 = psVar1 + 1;
    } while (iVar5 < 10);
    s16* puVar2 = dialogSpecialCase9AndATable.data() - 1;
    dialogSpecialCase9AndATable[10] = -1;
    uVar4 = uVar3 & 0xfff0;
    while ((uVar4 == uVar3 && (puVar2 != (&dialogSpecialCase9AndATable[9])))) {
        puVar2 = puVar2 + 1;
        uVar4 = (uint)*puVar2;
    }
    if (param_3 != 0) {
        puVar2 = puVar2 + -1;
        *puVar2 = (short)param_3 + dialogSpecialCase9AndAVar0;
    }
    dialogSpecialCase9AndASub0(puVar2);
}