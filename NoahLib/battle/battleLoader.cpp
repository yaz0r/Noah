#include "noahLib.h"
#include "battleLoader.h"
#include "kernel/gameState.h"
#include "battle.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "field/fieldGraphicObject.h"
#include "field/field.h"
#include "kernel/TIM.h"
#include "battleConfig.h"
#include "battleSpriteLoader.h"

void dummpyLoaderEnd() {
    // empty, probably an old logging code
}

s8 battleForceOnGear;
s8 battleCharacters[3];
s8 battleVar8006f9dd;

std::array<u16, 16> bitmaskCharacter = { {
        0x1, 0x2, 0x4, 0x8,
        0x10, 0x20, 0x40, 0x80,
        0x100, 0x200, 0x400, 0x800,
        0x1000, 0x2000, 0x4000, 0x8000
} };

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId)
{
    ushort result;

    if ((characterId & 0xff) < 0x10) {
        result = bitmaskCharacter[characterId & 0xff] & bitmask;
    }
    else {
        result = 0;
    }
    return result;
}

struct sBattleEntity {
    sGameStateA4 m0_base;
    sGameStateA42 mA4_gear;
};

std::array<sBattleEntity, 11> battleEntities;
std::vector<u8> battleFont;
void computeMenuBorder(std::vector<u8>& param_1, int param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7, int* param_8);

void loadBattleCharacterPortraits(const std::vector<u8>& input, s8 param_2) {

    int menuBorderData[3][6];

    for (int i = 0; i < 3; i++) {
        int characterId = battleCharacters[i];
        if (characterId != 0x7F) {
            if ((battleForceOnGear) && (i < 1)) {
                characterId = 11;
            }

            OpenTIM(input.begin() + characterId * 0x460);
            TIM_IMAGE tim;
            ReadTIM(&tim);
            computeMenuBorder(battleFont, param_2 + i, &menuBorderData[i][0], &menuBorderData[i][1], &menuBorderData[i][2], &menuBorderData[i][3], &menuBorderData[i][4], &menuBorderData[i][5]);
            tim.crect->x = menuBorderData[i][2];
            tim.crect->y = menuBorderData[i][3];
            tim.prect->x = menuBorderData[i][4] + 6 * i;
            tim.prect->y = menuBorderData[i][5];
            LoadImage(tim.crect, (u8*)tim.caddr);
            LoadImage(tim.prect, (u8*)tim.paddr);
            DrawSync(0);
        }
    }
}

std::array<sLoadingBatchCommands, 3> playerLoadingCommands;
sLoadableDataRaw battleConfigFile2;

void loadPartyMembers() {
    battleForceOnGear = (battleVar8006f9dd & 0x10) != 0;
    u16 partyMembersBitmask = gameState.m1D30_partyMemberBitField & gameState.m1D32_partyFrameMask;
    Hack("force battle team");
    partyMembersBitmask = 0x7FF;
    if (battleForceOnGear) {
        battleCharacters[1] = 10;
        battleCharacters[2] = 10;
        battleCharacters[0] = gameState.m1D34_currentParty[0] & 0x7F;
    }
    else {
        int currentSlotUsed = 0;
        for (int i = 0; i < 3; i++) {
            if (bitmaskCharacterCheck(partyMembersBitmask & 0x7FF, gameState.m1D34_currentParty[i])) {
                battleCharacters[i] = gameState.m1D34_currentParty[i] & 0x7F;
                currentSlotUsed++;
            }
        }
        // fill remaining slots
        for (int i = currentSlotUsed; i < 3; i++) {
            battleCharacters[i] = 0x7F;
        }
    }

    std::vector<std::vector<u8>::iterator> relocatedPointer = doPointerRelocation(battleCharacterConfigFile.mData);

    for (int i = 0; i < 3; i++) {
        if (battleCharacters[i] != 0x7F) {
            battleEntities[i].m0_base = gameState.m26C_party[battleCharacters[i]];

            if (battleForceOnGear && (i - 1U < 2)) {
                battleEntities[i].m0_base.m78_partyData_gearNum = 0x11;
            }

            int gearId = battleEntities[i].m0_base.m78_partyData_gearNum;
            if (gearId == -1) {
                gearId = 0;
            }

            battleEntities[i].mA4_gear = gameState.m978_gears[gearId];

            mallocAndDecompress(relocatedPointer[5 + battleCharacters[i]]);
            Noah_MissingCode("battle800CDD40");

            mallocAndDecompress(relocatedPointer[17 + gearId]);
            Noah_MissingCode("battle800CEF10");
        }
    }

    Noah_MissingCode("battle800D02C0");
    Noah_MissingCode("battle800D2200");

    // battle UI graphic elements
    uploadTextureToVram(sPS1Pointer(& mallocAndDecompress(relocatedPointer[1])[0]), 0,0,0,0,0,0);
    battleFont = mallocAndDecompress(relocatedPointer[0]);
    initFontPalettes(0, 0x1f0);

    Noah_MissingCode("battle800d329c");

    loadBattleCharacterPortraits(mallocAndDecompress(relocatedPointer[35]), 0x61);

    Noah_MissingCode("battle800D2500");

    setCurrentDirectory(0xc, 1);

    battleConfigFile2.resize(getFileSizeAligned(currentBattleConfig.m0 * 2 + 2));
    playerLoadingCommands[0].m0_fileIndex = currentBattleConfig.m0 * 2 + 2;
    playerLoadingCommands[0].m4_loadPtr = &battleConfigFile2;

    battleConfigFile3.resize(getFileSizeAligned(currentBattleConfig.m0 * 2 + 3));
    playerLoadingCommands[1].m0_fileIndex = currentBattleConfig.m0 * 2 + 3;
    playerLoadingCommands[1].m4_loadPtr = &battleConfigFile3;

    playerLoadingCommands[2].m0_fileIndex = 0;
    playerLoadingCommands[2].m4_loadPtr = nullptr;

    batchStartLoadingFiles(&playerLoadingCommands[0], 0);
}

u8 bBattleTickMode1 = 0;

void batteLoaderPhase1_0() {
    MissingCode();
    for (int i = 0; i < 3; i++) {
        MissingCode();
        battleVisualEntities[i].m2 = battleCharacters[i];
    }
    for (int i = 0; i < 11; i++) {
        battleVisualEntities[i].m3 = 0;
        if (battleForceOnGear == 0) {
            battleVisualEntities[i].m4 = gameState.m22B1_isOnGear[i];
        }
        else {
            battleVisualEntities[i].m4 = 1;
        }
        battleVisualEntities[i].m5 = 0;
    }
    MissingCode();
    bBattleTickMode1 = 0;
}

std::array<s8, 11> isBattleSlotFilled;
s8 battleNumPartyMembers = 0;

std::array<std::array<u8, 4>, 32> battleSlotLayout;

short characterIdToTargetBitmask(uint param_1)
{
    return bitmaskCharacter[param_1 & 0xff];
}

std::array<s8, 11> battleMonsterMapping;

void batteLoaderPhase1_1() {
    MissingCode();
    battleLoadDataVar2Bis = battleLoadDataVar2;
    MissingCode();

    //party
    for (int i = 0; i < 3; i++) {
        if (battleVisualEntities[i].m2 != 0x7F) {
            isBattleSlotFilled[i] = 1;
            battleNumPartyMembers++;
        }
        else {
            isBattleSlotFilled[i] = 0;
        }

        if (battleVisualEntities[i].m4 == 0) {
            battleVisualEntities[i].m0 = currentBattleConfig.m4[i] & 0x7F;
        }
        else {
            battleVisualEntities[i].m0 = i;
        }
    }

    battleNumPartyMembers--; // why?

    // monsters slots
    for (int i = 3; i < 11; i++) {
        battleVisualEntities[i].m2 = currentBattleConfig.m4[i + 1] & 0x7F;
        if (battleVisualEntities[i].m2 == 0x7F) {
            battleEntities[i].m0_base.m26_MaxHP = 0;
            battleEntities[i].m0_base.m24_HP = 0;
            battleVisualEntities[i].m3 = 0;
            battleVisualEntities[i].m4 = 0;
            isBattleSlotFilled[i] = 0;
        }
        else {
            battleVisualEntities[i].m3 = currentBattleConfig.m10[i - 3] & 0x80;
            battleVisualEntities[i].m4 = currentBattleConfig.m4[i + 1] & 0x80;
            battleVisualEntities[i].m5 = currentBattleConfig.m10[i - 3] & 0x1;
            isBattleSlotFilled[i] = 1;
            battleVisualEntities[i].m0 = currentBattleConfig.m18[i - 3] & 0x7F;
        }
        battleMonsterMapping[i] = battleVisualEntities[i].m2 + 1;
    }

    // init the battleSlotLayout
    {
        // reset
        for (int i = 0; i < 32; i++) {
            battleSlotLayout[i][0] = 0;
            battleSlotLayout[i][1] = 0;
        }

        // party members
        for (int i = 0; i < 3; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4 == 0) {
                    battleVisualEntities[i].m1 = battleSlotLayout[battleVisualEntities[i].m0][0];
                    battleSlotLayout[battleVisualEntities[i].m0][1] |= characterIdToTargetBitmask(battleVisualEntities[i].m1);
                    battleSlotLayout[battleVisualEntities[i].m0][0]++;
                }
                else {
                    battleVisualEntities[i].m1 = 0;
                    battleSlotLayout[battleVisualEntities[i].m0 + 0x10][1] = 1;
                    battleSlotLayout[battleVisualEntities[i].m0 + 0x10][0] = 1;
                }
            }
        }

        // monsters
        for (int i = 3; i < 11; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4 == 0) {
                    battleVisualEntities[i].m1 = battleSlotLayout[battleVisualEntities[i].m0 + 8][0];
                    battleSlotLayout[battleVisualEntities[i].m0][1] |= characterIdToTargetBitmask(battleVisualEntities[i].m1);
                    battleSlotLayout[battleVisualEntities[i].m0][0]++;
                }
                else {
                    battleVisualEntities[i].m1 = 0;
                    battleSlotLayout[battleVisualEntities[i].m0 + 0x18][1] = 1;
                    battleSlotLayout[battleVisualEntities[i].m0 + 0x18][0] = 1;
                }
            }
        }
    }

    // setup locations on field
    {
        // party
        for (int i = 0; i < 3; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4 == 0) {
                    battleVisualEntities[i].mA_X = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m1 * 4 + battleVisualEntities[i].m0 * 0x20 + 4);
                    battleVisualEntities[i].mC_Z = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m1 * 4 + battleVisualEntities[i].m0 * 0x20 + 6);
                }
                else {
                    battleVisualEntities[i].mA_X = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m0 * 8 + 0x100);
                    battleVisualEntities[i].mC_Z = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m0 * 8 + 0x102);
                }
            }
        }

        // monsters
        for (int i = 3; i < 11; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4 == 0) {
                    battleVisualEntities[i].mA_X = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m1 * 4 + battleVisualEntities[i].m0 * 0x20 + 0x10);
                    battleVisualEntities[i].mC_Z = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m1 * 4 + battleVisualEntities[i].m0 * 0x20 + 0x12);
                }
                else {
                    battleVisualEntities[i].mA_X = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m0 * 8 + 0x104);
                    battleVisualEntities[i].mC_Z = READ_LE_S16(battleLoadDataVar2Bis + battleVisualEntities[i].m0 * 8 + 0x106);
                }
            }
        }
    }
}

void battleLoaderTick(s8 param_1) {
    switch (param_1) {
    case 0:
        loadPartyMembers();
        dummpyLoaderEnd();
        break;
    case 1:
        batteLoaderPhase1_0();
        batteLoaderPhase1_1();
        MissingCode();
        break;
    case 2:
        MissingCode();
        break;
    case 3:
        MissingCode();
        break;
    default:
        assert(0);
    }
}
