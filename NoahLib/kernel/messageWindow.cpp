#include "noahLib.h"
#include "kernel/messageWindow.h"
#include "kernel/text.h"
#include "kernel/memory.h"
#include "kernel/font.h"
#include "kernel/gameState.h"
#include "kernel/TIM.h"

u16 textSpriteMode0 = 0;
u16 textSpriteMode1 = 0;

void addSPRT(sTag* ot, sTag* p)
{
    // TODO: I think this is wrong
    p->m0_pNext = ot->m0_pNext;
    p->m3_size |= 4;
    ot->m0_pNext = p;
}

void clearSelectedLineForMultiChoice(sDialogWindow18* param_1)
{
    param_1->m6E_selectedLineForMultiChoice = -1;
    return;
}

void setSelectedLineForMultiChoice(sDialogWindow18* param_1, char param_2)
{
    param_1->m6E_selectedLineForMultiChoice = param_2;
    return;
}

std::vector<u8>::iterator getDialogParamPointer(const std::vector<u8>::iterator& buffer, int param_2)
{
    u32 numEntries = READ_LE_U32(buffer);
    assert(param_2 <= numEntries);
    return buffer + READ_LE_U16(buffer + param_2 * 2 + 4);
}

void resetDialogWindow18(sDialogWindow18* param_1)
{
    if (param_1->m84_delay == 0) {
        param_1->m6C_autoClose = 0;
        param_1->m10_flags = param_1->m10_flags & 2;
    }
    return;
}

void setupWindowSize2(sDialogWindow18* param_1, int x1, int y1, short x2, short y2, short width, short height)
{
    (param_1->m4).vx = x2;
    param_1->m10_flags = 0;
    param_1->m84_delay = 0;
    param_1->m8C = nullptr;
    param_1->m82 = 0;
    (param_1->m14).vx = 0xe;
    param_1->m68 = 1;
    param_1->m69 = 1;
    param_1->mA_width1 = width;
    param_1->m6C_autoClose = 0;
    param_1->m6A = 0;
    param_1->m6D = 0;
    param_1->m6B = 0;
    param_1->m6E_selectedLineForMultiChoice = -1;
    int sVar16 = (short)y1;
    param_1->mE = sVar16;
    (param_1->m4).vy = y2;
    param_1->mC_height = height;
    param_1->mA_width1 |= 1;
    param_1->m8_widthTimes4 = (short)(param_1->mA_width1 << 2);
    param_1->m12_widthPadded = param_1->mA_width1 + 3;

    traceNextAlloc(0x29);
    param_1->m28_perLineBuffer.resize(param_1->mC_height);

    traceNextAlloc(0x28);
    param_1->m2C_inRamDialogTextImage.resize(param_1->m12_widthPadded * 14);

    param_1->m48_textTile[0].m3_size = 3;
    param_1->m48_textTile[0].r0 = 0;
    param_1->m48_textTile[0].g0 = 0;
    param_1->m48_textTile[0].b0 = 0;
    param_1->m48_textTile[0].code = 0x60;
    param_1->m48_textTile[0].x0y0.vx = param_1->m4.vx - 7;
    param_1->m48_textTile[0].x0y0.vy = param_1->m4.vy - 5;
    param_1->m48_textTile[0].wh.vx = param_1->mA_width1 * 0xD;
    param_1->m48_textTile[0].wh.vy = param_1->mC_height * (param_1->m14).vx + 10;
    SetSemiTrans(&param_1->m48_textTile[0], 1);
    param_1->m48_textTile[1] = param_1->m48_textTile[0];

    if (0 < param_1->mC_height)
    {
        for (int i = 0; i < param_1->mC_height; i++)
        {
            param_1->m28_perLineBuffer[i].m0[0][0].x0 = param_1->m4.vx;
            param_1->m28_perLineBuffer[i].m0[0][0].y0 = param_1->m4.vx * i + param_1->m4.vy;
            param_1->m28_perLineBuffer[i].m0[0][1].x0 = param_1->m4.vx + 0x100;
            param_1->m28_perLineBuffer[i].m0[0][1].y0 = param_1->m4.vx * i + param_1->m4.vy;

            if (param_1->m8_widthTimes4 < 0x101) {
                param_1->m28_perLineBuffer[i].m0[0][0].h = 0xD;
                param_1->m28_perLineBuffer[i].m0[0][0].w = param_1->m8_widthTimes4;
            }
            else
            {
                param_1->m28_perLineBuffer[i].m0[0][0].h = 0xD;
                param_1->m28_perLineBuffer[i].m0[0][0].w = 0x100;
            }

            if (param_1->m8_widthTimes4 < 0x101) {
                param_1->m28_perLineBuffer[i].m0[0][1].h = 0xD;
                param_1->m28_perLineBuffer[i].m0[0][1].w = 0;
            }
            else
            {
                param_1->m28_perLineBuffer[i].m0[0][1].h = 0xD;
                param_1->m28_perLineBuffer[i].m0[0][1].w = param_1->m8_widthTimes4 - 0xF0;
            }

            int yFraq = (y1 + (i / 2) * 0xD);

            param_1->m28_perLineBuffer[i].m0[0][1].u0 = param_1->m28_perLineBuffer[i].m0[0][0].u0 = (x1 & 0x3F) << 2;
            param_1->m28_perLineBuffer[i].m0[0][1].v0 = param_1->m28_perLineBuffer[i].m0[0][0].v0 = (yFraq & 0xFF) << 8;

            param_1->m28_perLineBuffer[i].m0[0][1].m3_size = param_1->m28_perLineBuffer[i].m0[0][0].m3_size = 4;
            param_1->m28_perLineBuffer[i].m0[0][1].code = param_1->m28_perLineBuffer[i].m0[0][0].code = 0x64; // setup as Textured Rectangle, variable size, opaque, texture-blending

            param_1->m28_perLineBuffer[i].m0[0][0].code |= 1; // switch to Textured Rectangle, variable size, opaque, raw-texture
            param_1->m28_perLineBuffer[i].m0[0][1].code |= 1;

            param_1->m28_perLineBuffer[i].m0[1][0] = param_1->m28_perLineBuffer[i].m0[0][0];
            param_1->m28_perLineBuffer[i].m0[1][1] = param_1->m28_perLineBuffer[i].m0[0][1];

            param_1->m28_perLineBuffer[i].m50.x = x1;
            param_1->m28_perLineBuffer[i].m50.y = yFraq;
            param_1->m28_perLineBuffer[i].m50.w = param_1->m12_widthPadded;
            param_1->m28_perLineBuffer[i].m50.h = 0xD;
            param_1->m28_perLineBuffer[i].m58_widthDiv4 = 0;
            if ((i & 1) == 0)
            {
                param_1->m28_perLineBuffer[i].m5E_clut = textSpriteMode0;
            }
            else
            {
                param_1->m28_perLineBuffer[i].m5E_clut = textSpriteMode1;
            }
            param_1->m28_perLineBuffer[i].m5C = y1 + (i / 2) * 0xD;
            param_1->m28_perLineBuffer[i].m5A = i & 1;
            param_1->m28_perLineBuffer[i].m5B = i;
        }
    }

    SetDrawMode(&param_1->m30_textTileDrawMode[0], 0, 0, GetTPage(0, 0, x1, y1), nullptr);
    SetDrawMode(&param_1->m30_textTileDrawMode[1], 0, 0, GetTPage(0, 0, x1 + 0x40, y1), nullptr);
}

void dealocateDialogWindow18MemorySubSub(sDialogWindow18* param_1)
{
    sDialogWindow18_8C* psVar1;
    sDialogWindow18_8C* psVar2;

    if (param_1->m84_delay == 0) {
        psVar1 = param_1->m8C;
        while (psVar1 != nullptr) {
            psVar2 = psVar1->m0_pNext;
            delete psVar1;
            psVar1 = psVar2;
        }
        param_1->m8C = nullptr;
        param_1->m82 = 0;
    }
    return;
}

void dealocateDialogWindow18MemorySub(sDialogWindow18* param_1)
{
    param_1->m6C_autoClose = 0;
    param_1->m84_delay = 0;
    param_1->m10_flags = param_1->m10_flags & 2;
    dealocateDialogWindow18MemorySubSub(param_1);
    return;
}


void dealocateDialogWindow18Memory(sDialogWindow18* param_1)
{
    dealocateDialogWindow18MemorySub(param_1);
    param_1->m28_perLineBuffer.clear();
    param_1->m2C_inRamDialogTextImage.clear();
    return;
}

void addDialogWindowsToOTSub1(sDialogWindow18* param_1)
{
    ushort uVar1;

    uVar1 = param_1->m10_flags;
    param_1->m10_flags = uVar1 & 0xfff7;
    if ((uVar1 & 0x200) != 0) {
        param_1->m84_delay = 0;
        param_1->m6C_autoClose = 0;
        param_1->m10_flags = param_1->m10_flags & 0xfdff;
    }
    return;
}

int getWindowSubStructX(sDialogWindow18* param_1)
{
    return (int)(param_1->m4).vx + (param_1->m0).vx * 4;
}

int getWindowSubStructY(sDialogWindow18* param_1)
{
    int iVar1;

    iVar1 = (int)(param_1->m0).vy - (int)(param_1->m14).vy;
    if (iVar1 < 0) {
        iVar1 = param_1->mC_height + -1;
    }
    return (int)(param_1->m4).vy + iVar1 * (param_1->m14).vx;
}

s8 addDialogWindowsToOTSub0(sDialogWindow18* param_1)
{
    if ((param_1->m10_flags & 8) != 0) {
        return param_1->m6B;
    }
    return 0;
}

void addDialogWindowsToOTSub2(sDialogWindow18* param_1, const std::vector<u8>::iterator& param_2)
{
    sDialogWindow18_8C* psVar1;
    sDialogWindow18_8C* psVar2;

    psVar2 = param_1->m8C;
    param_1->m82 = param_1->m82 + 1;
    traceNextAlloc(0x2a);
    psVar1 = new sDialogWindow18_8C;
    psVar1->m4_dialogPointer = param_2;
    psVar1->m0_pNext = nullptr;
    if (psVar2 == nullptr) {
        param_1->m8C = psVar1;
    }
    else {
        for (; psVar2->m0_pNext != nullptr; psVar2 = psVar2->m0_pNext) {
        }
        psVar2->m0_pNext = psVar1;
    }
}

void dialogSpecialCase9AndA_2(sDialogWindow18* param_1, std::vector<u8>::iterator string) {
    param_1->m20 = param_1->m1C_currentStringToPrint;
    param_1->m1C_currentStringToPrint = string;
    param_1->m10_flags |= 0x80;
}

void updateDialogTextImage(sDialogWindow18* param_1)
{
    if (param_1->mA_width1 < (param_1->m0).vx) // go to next line?
    {
        int sVar3 = param_1->mC_height;
        (param_1->m0).vx = 0;
        (param_1->m0).vy = (param_1->m0).vy + 1;
        int sVar8 = (param_1->m0).vy;
        param_1->m18 = param_1->m18 + 1;
        if (sVar3 <= sVar8) {
            int uVar5 = param_1->m10_flags;
            (param_1->m0).vy = 0;
            param_1->m10_flags = uVar5 | 1;
        }
        if ((param_1->m10_flags & 1) != 0) {
            param_1->m28_perLineBuffer[(param_1->m14).vy].m58_widthDiv4 = 0;
            sVar3 = param_1->mC_height;
            sVar8 = (param_1->m14).vy + 1;
            (param_1->m14).vy = sVar8;
            if (sVar3 <= sVar8) {
                (param_1->m14).vy = 0;
            }
        }
        int characterToPrint = (int)param_1->m18 % (param_1->mC_height + 1);
        int iVar13 = (int)(param_1->m0).vy;
        param_1->m28_perLineBuffer[iVar13].m5C = (char)((int)characterToPrint / 2) * '\r' + *(char*)&param_1->mE;
        if ((characterToPrint & 1) == 0) {
            param_1->m28_perLineBuffer[iVar13].m5E_clut = textSpriteMode0;
        }
        else {
            param_1->m28_perLineBuffer[iVar13].m5E_clut = textSpriteMode1;
        }
        param_1->m28_perLineBuffer[iVar13].m5A = (byte)characterToPrint & 1;
        param_1->m28_perLineBuffer[iVar13].m5B = (byte)characterToPrint;
        param_1->m28_perLineBuffer[iVar13].m50.y = param_1->mE + (short)((int)characterToPrint / 2) * 0xd;
    }

    int bVar1 = param_1->m69;
    int iVar13 = bVar1 - 1;
    auto printDialogTextVarBackup = printDialogTextVar;

    if (param_1->m6C_autoClose != 0) {
        param_1->m6C_autoClose = 0;
        param_1->m10_flags = param_1->m10_flags & 0xfffb;
        return;
    }

    while (1)
    {
        if (iVar13 == -1)
        {
            printDialogTextVar = printDialogTextVarBackup;
            return;
        }

        u8* pCharacterToPrint = &(*param_1->m1C_currentStringToPrint);
        u8 characterToPrint = *pCharacterToPrint;

        switch (characterToPrint)
        {
        case 0:
            if ((param_1->m10_flags & 0x80) == 0) {
                param_1->m10_flags |= 8;
                param_1->m6B = 1;
                param_1->m6C_autoClose = 1;
                return;
            }
            param_1->m10_flags &= ~0x80;
            param_1->m1C_currentStringToPrint = param_1->m20 + 1;
            iVar13 = iVar13 + -1;
            break;
        case 1: // end of line
            (param_1->m0).vx = 100;
            param_1->m1C_currentStringToPrint++;
            return;
        case 2:
            param_1->m6B = '\x02';
            param_1->m10_flags = param_1->m10_flags | 0x48;
            param_1->m1C_currentStringToPrint++;
            if (*param_1->m1C_currentStringToPrint != '\x01') {
                return;
            }
            param_1->m1C_currentStringToPrint++;
            return;
        case 3:
            assert(0);
            break;
        case 0xF:
            // escape character;
            switch (pCharacterToPrint[1])
            {
            case 0:
                param_1->m84_delay = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 3;
                return;
            case 1:
                if (param_1->m1C_currentStringToPrint[2] == 0) {
                    int bVar1 = param_1->m6A;
                    int bVar2 = param_1->m6A;
                    param_1->m6A = 0;
                    param_1->m68 = bVar1;
                    param_1->m69 = bVar2;
                }
                else {
                    int bVar2 = param_1->m68;
                    iVar13 = iVar13 + param_1->m1C_currentStringToPrint[2];
                    param_1->m68 = bVar1;
                    param_1->m69 = bVar1;
                    param_1->m6A = bVar2;
                }
                param_1->m1C_currentStringToPrint = param_1->m1C_currentStringToPrint + 3;
                break;
            case 2:
                param_1->m84_delay = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 3;
                param_1->m6C_autoClose = 1;
                return;
            case 5: // print gear name
            {
                u8 characterToPrint = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 2;
                if ((characterToPrint < 0x80) || (characterToPrint = gameState.m1D34_currentParty[characterToPrint - 0x80], characterToPrint != 0xff)) {
                    dialogSpecialCase9AndA_2(param_1, gameState.m0_names[characterToPrint].begin());
                }
                else {
                    assert(0);
                    //dialogSpecialCase9AndA_2(param_1, getDialogParamPointer(printDialogTextVar->m68), 0);
                }
                continue;
            }
            case 0xA:
            {
                u8 varNumberToPrint = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 2;
                dialogSpecialCase9AndA(param_1->m70[varNumberToPrint], 1, 0);
                dialogSpecialCase9AndA_2(param_1, dialogSpecialCase9AndA_2Data.begin());
                printDialogTextVarBackup = printDialogTextVar;
                continue;
            }
            case 0xC:
            {
                u8 varNumberToPrint = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 2;
                dialogSpecialCase9AndA(param_1->m70[varNumberToPrint], 1, 1);
                dialogSpecialCase9AndA_2(param_1, dialogSpecialCase9AndA_2Data.begin());
                printDialogTextVarBackup = printDialogTextVar;
                continue;
            }
            case 0xE:
            {
                int bVar1 = param_1->m68;
                param_1->m68 = 1;
                param_1->m69 = 1;
                param_1->m6A = bVar1;
                bVar1 = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint = param_1->m1C_currentStringToPrint + 3;
                int uVar6 = (ushort)bVar1;
                param_1->m88_delayBetweenCharacters = uVar6;
                param_1->m86_currentDelayForNextCharacter = uVar6;
            }
            return;
            /*
        case 0xF:
        {
            u8 varNumberToPrint = param_1->m1C_currentStringToPrint[2];
            param_1->m1C_currentStringToPrint += 2;
            characterToPrint = dpadButtonMapping[varNumberToPrint];
            dialogSpecialCase9AndA_2(param_1, getDialogParamPointer(printDialogTextVarBackup[51], characterToPrint));
            printDialogTextVarBackup = printDialogTextVar;
            continue;
        }*/
            default:
                assert(0);
            }
            break;
        default:
        {
            // normal character
            int iVar13 = 1;
            int uVar10;
            if ((int)characterToPrint < dialogFontVar0) {
                uVar10 = characterToPrint;
                characterToPrint = 0;
            }
            else {
                uVar10 = (uint)pCharacterToPrint[1];
                iVar13 = 2;
            }
            int uVar4 = getDialogFontCharacterWidth(characterToPrint, uVar10);
            int sVar3 = (param_1->m0).vx;
            if ((int)param_1->mA_width1 < (int)((int)sVar3 + (uint)uVar4)) {
                (param_1->m0).vx = sVar3 + uVar4;
                return;
            }
            printDialogCharacter(characterToPrint, uVar10, &param_1->m2C_inRamDialogTextImage[sVar3], (int)param_1->m12_widthPadded, param_1->m28_perLineBuffer[(param_1->m0).vy].m5A);
            sVar3 = (param_1->m0).vy;
            int sVar7 = (param_1->m0).vx + uVar4;
            param_1->m1C_currentStringToPrint += iVar13;
            (param_1->m0).vx = sVar7;
            param_1->m28_perLineBuffer[sVar3].m58_widthDiv4 = sVar7;
            break;
        }
        }

        iVar13--;
    }

    MissingCode();
}

void updateAndRenderTextForDialogWindow(sDialogWindow18* param_1, sTag* OT, int oddOrEven)
{
    if ((param_1->m10_flags & 4) == 0) {
        if (param_1->m82 == 0) {
            return;
        }
        sDialogWindow18_8C* psVar8 = param_1->m8C;
        param_1->m1C_currentStringToPrint = psVar8->m4_dialogPointer;
        param_1->m8C = param_1->m8C->m0_pNext;
        delete psVar8;
        param_1->m82 = param_1->m82 + -1;
        param_1->m10_flags = param_1->m10_flags & 2 | 0x24;
        if (param_1->m6A != 0) {
            u8 bVar3 = param_1->m6A;
            param_1->m68 = param_1->m6A;
            param_1->m6A = 0;
            param_1->m69 = bVar3;
        }
        param_1->m88_delayBetweenCharacters = 0;
        param_1->m86_currentDelayForNextCharacter = 0;
        param_1->m69 = param_1->m68;
    }
    if ((param_1->m10_flags & 0x100) == 0) {
        param_1->m69 = param_1->m68;
    }
    else {
        param_1->m69 = param_1->m68 * 3;
    }

    if (((param_1->m10_flags & 0x40) != 0) && ((param_1->m10_flags & 8) == 0)) {
        param_1->m10_flags &= ~0x40;
        param_1->m10_flags |= 0x20;
    }

    if ((param_1->m10_flags & 0x20) != 0) {
        sDialogWindow18PerLineBufferEntry* psVar5 = &param_1->m28_perLineBuffer[0];
        short sVar2 = param_1->mE;
        (param_1->m14).vy = 0;
        param_1->m18 = 0;
        (param_1->m0).vy = 0;
        (param_1->m0).vx = 0;
        psVar5->m5C = (byte)sVar2;
        param_1->m28_perLineBuffer[0].m5E_clut = textSpriteMode0;
        param_1->m28_perLineBuffer[0].m5A = 0;
        param_1->m28_perLineBuffer[0].m50.y = param_1->mE;
        if (0 < param_1->mC_height) {
            for (int i = 0; i < param_1->mC_height; i++)
            {
                param_1->m28_perLineBuffer[i].m58_widthDiv4 = 0;
            }
        }
        param_1->m10_flags &= ~0x20;
    }

    if (0 < param_1->mC_height) {
        int iVar11 = param_1->m14.vy;
        int iVar6 = param_1->m14.vy;
        for (int i = 0; i < param_1->mC_height; i++)
        {
            if (iVar11 >= param_1->mC_height)
            {
                iVar6 = 0;
                iVar11 = 0;
            }

            if (param_1->m6E_selectedLineForMultiChoice == i) {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code &= ~1;
            }
            else {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code |= 1;
            }

            if (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4 > 0x40)
            {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].v0 = param_1->m28_perLineBuffer[iVar6].m5C;
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].clut = param_1->m28_perLineBuffer[iVar6].m5E_clut;
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].y0 = (param_1->m4).vy + (param_1->m14).vx * i;
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].w = (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4 - 0x40) * 4;
                addSPRT(OT, &param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1]);
            }
            iVar6++;
        }
    }

    AddPrim(OT, &param_1->m30_textTileDrawMode[1]);

    if (0 < param_1->mC_height) {
        int iVar11 = param_1->m14.vy;
        int iVar6 = param_1->m14.vy;
        for (int i = 0; i < param_1->mC_height; i++)
        {
            if (iVar11 >= param_1->mC_height)
            {
                iVar6 = 0;
                iVar11 = 0;
            }

            if (param_1->m6E_selectedLineForMultiChoice == i) {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code &= ~1;
            }
            else {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code |= 1;
            }

            // do we overflow in the second sprite?
            if (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4)
            {
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].v0 = param_1->m28_perLineBuffer[iVar6].m5C;
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].clut = param_1->m28_perLineBuffer[iVar6].m5E_clut;
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].y0 = (param_1->m4).vy + (param_1->m14).vx * i;
                int uVar4 = param_1->m28_perLineBuffer[iVar6].m58_widthDiv4;
                if (0x40 < uVar4) {
                    uVar4 = 0x40;
                }
                param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].w = uVar4 * 4;
                addSPRT(OT, &param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0]);
            }
            iVar6++;
        }
    }

    if (param_1->m84_delay == 0) {
        if (param_1->m86_currentDelayForNextCharacter == 0) {
            param_1->m86_currentDelayForNextCharacter = param_1->m88_delayBetweenCharacters;
            if ((param_1->m10_flags & 0x58) == 0) {
                updateDialogTextImage(param_1);
                LoadImage(&param_1->m28_perLineBuffer[(param_1->m0).vy].m50, (u8*)&param_1->m2C_inRamDialogTextImage[0]);
            }
        }
        else {
            param_1->m86_currentDelayForNextCharacter = param_1->m86_currentDelayForNextCharacter + -1;
        }
    }
    else {
        param_1->m84_delay = param_1->m84_delay + -1;
    }

    if (param_1->m84_delay != 0)
    {
        if (--param_1->m84_delay == -1)
        {
            param_1->m10_flags = param_1->m10_flags & 0xffef;
        }
    }

    if ((param_1->m10_flags & 2) == 0) {
        assert(0);
    }

    param_1->m10_flags = param_1->m10_flags & 0xfeff;
    AddPrim(OT, &param_1->m30_textTileDrawMode[0]);
}

void setupMultiChoiceColor(sDialogWindow18* param_1, u8 R, u8 G, u8 B)
{
    for (int i = 0; i < param_1->mC_height; i++)
    {
        param_1->m28_perLineBuffer[i].m0[1][1].r0 = R;
        param_1->m28_perLineBuffer[i].m0[1][0].r0 = R;
        param_1->m28_perLineBuffer[i].m0[0][1].r0 = R;
        param_1->m28_perLineBuffer[i].m0[0][0].r0 = R;

        param_1->m28_perLineBuffer[i].m0[1][1].g0 = G;
        param_1->m28_perLineBuffer[i].m0[1][0].g0 = G;
        param_1->m28_perLineBuffer[i].m0[0][1].g0 = G;
        param_1->m28_perLineBuffer[i].m0[0][0].g0 = G;

        param_1->m28_perLineBuffer[i].m0[1][1].b0 = B;
        param_1->m28_perLineBuffer[i].m0[1][0].b0 = B;
        param_1->m28_perLineBuffer[i].m0[0][1].b0 = B;
        param_1->m28_perLineBuffer[i].m0[0][0].b0 = B;
    }
}
