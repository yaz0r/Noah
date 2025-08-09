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
#include "battleRenderContext.h"
#include "menus/menuHandler.h"
#include "battle/enemyScript.h"
#include "kernel/memory.h"
#include "field/dialogWindows.h"

void dummpyLoaderEnd() {
    // empty, probably an old logging code
}

s8 battleForceOnGear;
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

ushort bitmaskCharacterCheckReverse(ushort bitmask, uint characterId)
{
    static std::array<u16, 16> bitmaskCharacterReverse = { {
        0x8000, 0x4000, 0x2000, 0x1000,
        0x800, 0x400, 0x200, 0x100,
        0x80, 0x40, 0x20, 0x10,
        0x8, 0x4, 0x2, 0x1,
    } };
    ushort result;

    if ((characterId & 0xff) < 0x10) {
        result = bitmaskCharacterReverse[characterId & 0xff] & bitmask;
    }
    else {
        result = 0;
    }
    return result;
}

void computeMenuBorder(sFont& param_1, int param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7, int* param_8);

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

struct sBattleEnemyInitialStats : public sLoadableDataRaw {
    std::array<std::array<s16, 4>, 4> m0_scriptsOffsets;
    std::vector<u8>::const_iterator m30_strings;
    std::array<sBattleEntity,8> m32;

    virtual void init(const std::vector<u8>& data) override {
        sLoadableDataRaw::init(data);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m0_scriptsOffsets[i][j] = READ_LE_S16(data.begin() + 0 + 4 * 2 * i + 2 * j);
            }
        }
        m30_strings = getRawData().begin() + READ_LE_U16(getRawData().begin() + 0x30);
        for (int i = 0; i < 8; i++) {
            m32[i].deserialize(data.begin() + 0x32 + 0x170 * i);
        }
    }

    std::vector<u8>::const_iterator getPointerForScript(int setupId, int scriptType) {
        return getRawData().begin() + m0_scriptsOffsets[setupId][scriptType];
    }

    std::vector<u8>::const_iterator getPointerForScriptSub(int setupId, int scriptType, int subEntry) {
        auto it = getPointerForScript(setupId, scriptType);
        s16 offset = READ_LE_S16(it);
        assert(offset >= 0);
        return it + offset;
    }
};

sBattleEnemyInitialStats battleConfigFile2;

void loadPartyMembers() {
    battleForceOnGear = (battleVar8006f9dd & 0x10) != 0;
    u16 partyMembersBitmask = gameState.m1D30_partyMemberBitField & gameState.m1D32_partyFrameMask;
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

    std::vector<std::vector<u8>::const_iterator> relocatedPointer = doPointerRelocation(battleCharacterConfigFile.getRawData());

    for (int i = 0; i < 3; i++) {
        if (battleCharacters[i] != 0x7F) {
            battleEntities[i].m0_base = gameState.m26C_party[battleCharacters[i]];

            if (battleForceOnGear && (i - 1U < 2)) {
                battleEntities[i].m0_base.mA0_partyData_gearNum = 0x11;
            }

            int gearId = battleEntities[i].m0_base.mA0_partyData_gearNum;
            if (gearId == -1) {
                gearId = 0;
            }

            battleEntities[i].mA4_gear = gameState.m978_gears[gearId];

            partyBattleStats[i].init(mallocAndDecompress(relocatedPointer[4 + battleCharacters[i]]));
            mechaBattleStats[i].init(mallocAndDecompress(relocatedPointer[16 + gearId]));
        }
    }

    enemiesBattleStats.init(mallocAndDecompress(relocatedPointer[3]));

    Noah_MissingCode("battle800D2200");

    // battle UI graphic elements
    auto sprites = mallocAndDecompress(relocatedPointer[1]);
    std::span<u8> spritesSpan(sprites.begin(), sprites.end());
    uploadTextureToVram(spritesSpan.begin(), 0,0,0,0,0,0);
    battleFont.deserialize(mallocAndDecompress(relocatedPointer[0]));
    initFontPalettes(0, 0x1f0);

    Noah_MissingCode("battle800d329c");

    loadBattleCharacterPortraits(mallocAndDecompress(relocatedPointer[35]), 0x61);

    Noah_MissingCode("battle800D2500");

    setCurrentDirectory(0xc, 1);

    //battleConfigFile2.resize(getFileSizeAligned(currentBattleConfig.m0 * 2 + 2));
    playerLoadingCommands[0].m0_fileIndex = currentBattleConfig.m0 * 2 + 2;
    playerLoadingCommands[0].m4_loadPtr = &battleConfigFile2;

    battleConfigFile3.resize(getFileSizeAligned(currentBattleConfig.m0 * 2 + 3));
    playerLoadingCommands[1].m0_fileIndex = currentBattleConfig.m0 * 2 + 3;
    playerLoadingCommands[1].m4_loadPtr = &battleConfigFile3;

    playerLoadingCommands[2].m0_fileIndex = 0;
    playerLoadingCommands[2].m4_loadPtr = nullptr;

    batchStartLoadingFiles(&playerLoadingCommands[0], 0);
}

void batteLoaderPhase1_0() {
    MissingCode();
    drawBattleMode1Disabled = 0;
    MissingCode();
    for (int i = 0; i < 3; i++) {
        MissingCode();
        battleVisualEntities[i].m2 = battleCharacters[i];
    }
    for (int i = 0; i < 11; i++) {
        battleVisualEntities[i].m3 = 0;
        if (battleForceOnGear == 0) {
            battleVisualEntities[i].m4_isGear = gameState.m22B1_isOnGear[i];
        }
        else {
            battleVisualEntities[i].m4_isGear = 1;
        }
        battleVisualEntities[i].m5 = 0;
    }
    MissingCode();
    bBattleTickMode1 = 0;
}

std::array<std::array<u8, 4>, 32> battleSlotLayout;

u16 characterIdToTargetBitmask(uint param_1)
{
    if (param_1 == 0xFF) {
        Hack("Prevented out of bound access");
        return 0;
    }
    else {
        return bitmaskCharacter[param_1 & 0xff];
    }
}

void batteLoaderPhase1_1() {
    battlePartyLayoutType = 0;  
    battleLoadDataVar2Bis = battleLoadDataVar2;
    battleLoadDataVar2Ter = battleLoadDataVar2;

    //party
    for (int i = 0; i < 3; i++) {
        if (battleVisualEntities[i].m2 != 0x7F) {
            isBattleSlotFilled[i] = 1;
            battlePartyLayoutType++;
        }
        else {
            isBattleSlotFilled[i] = 0;
        }

        if (battleVisualEntities[i].m4_isGear == 0) {
            battleVisualEntities[i].m0_positionSlot = currentBattleConfig.m4[i] & 0x7F;
        }
        else {
            battleVisualEntities[i].m0_positionSlot = i;
        }
    }

    battlePartyLayoutType--; // 1 PC->layout 0, 2 PC->layout1, 3PC->layout2

    // monsters slots
    for (int i = 3; i < 11; i++) {
        battleVisualEntities[i].m2 = currentBattleConfig.m4[i + 1] & 0x7F;
        if (battleVisualEntities[i].m2 == 0x7F) {
            battleEntities[i].m0_base.m4E_MaxHP = 0;
            battleEntities[i].m0_base.m4C_HP = 0;
            battleVisualEntities[i].m3 = 0;
            battleVisualEntities[i].m4_isGear = 0;
            isBattleSlotFilled[i] = 0;
        }
        else {
            battleVisualEntities[i].m3 = currentBattleConfig.m10[i - 3] & 0x80;
            battleVisualEntities[i].m4_isGear = currentBattleConfig.m4[i + 1] & 0x80;
            battleVisualEntities[i].m5 = currentBattleConfig.m10[i - 3] & 0x1;
            isBattleSlotFilled[i] = 1;
            battleVisualEntities[i].m0_positionSlot = currentBattleConfig.m18[i - 3] & 0x7F;
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
                if (battleVisualEntities[i].m4_isGear == 0) {
                    battleVisualEntities[i].m1 = battleSlotLayout[battleVisualEntities[i].m0_positionSlot][0];
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot][1] |= characterIdToTargetBitmask(battleVisualEntities[i].m1);
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot][0]++;
                }
                else {
                    battleVisualEntities[i].m1 = 0;
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 0x10][1] = 1;
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 0x10][0] = 1;
                }
            }
        }

        // monsters
        for (int i = 3; i < 11; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4_isGear == 0) {
                    battleVisualEntities[i].m1 = battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 8][0];
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 8][1] |= characterIdToTargetBitmask(battleVisualEntities[i].m1);
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 8][0]++;
                }
                else {
                    battleVisualEntities[i].m1 = 0;
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 0x18][1] = 1;
                    battleSlotLayout[battleVisualEntities[i].m0_positionSlot + 0x18][0] = 1;
                }
            }
        }
    }

    // setup locations on field
    {
        // party
        for (int i = 0; i < 3; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4_isGear == 0) {
                    battleVisualEntities[i].mA_X = battleLoadDataVar2Bis->m4[battleVisualEntities[i].m0_positionSlot][battleVisualEntities[i].m1].vx;
                    battleVisualEntities[i].mC_Z = battleLoadDataVar2Bis->m4[battleVisualEntities[i].m0_positionSlot][battleVisualEntities[i].m1].vy;
                }
                else {
                    battleVisualEntities[i].mA_X = battleLoadDataVar2Bis->m100[battleVisualEntities[i].m0_positionSlot][0].vx;
                    battleVisualEntities[i].mC_Z = battleLoadDataVar2Bis->m100[battleVisualEntities[i].m0_positionSlot][0].vy;
                }
            }
        }

        // monsters
        for (int i = 3; i < 11; i++) {
            if (battleVisualEntities[i].m2 != 0x7F) {
                if (battleVisualEntities[i].m4_isGear == 0) {
                    battleVisualEntities[i].mA_X = battleLoadDataVar2Bis->m4[battleVisualEntities[i].m0_positionSlot][battleVisualEntities[i].m1 + 3].vx;
                    battleVisualEntities[i].mC_Z = battleLoadDataVar2Bis->m4[battleVisualEntities[i].m0_positionSlot][battleVisualEntities[i].m1 + 3].vy;
                }
                else {
                    battleVisualEntities[i].mA_X = battleLoadDataVar2Bis->m100[battleVisualEntities[i].m0_positionSlot][1].vx;
                    battleVisualEntities[i].mC_Z = battleLoadDataVar2Bis->m100[battleVisualEntities[i].m0_positionSlot][1].vy;
                }
            }
        }
    }
}

void initBattleInventory(void) {
    MissingCode();
}

std::array<std::array<u8, 8>, 16> battleCommandsSetups = { {
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x24, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x26, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x24, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x27, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x28, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x24, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x2A, 0x2C, 0x1C, 0x1A, 0x20},
    {0x1E, 0x1C, 0x22, 0x29, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x26, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x24, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
    {0x1E, 0x1C, 0x22, 0x25, 0x2C, 0x1C, 0x1A, 0x2E},
} };

bool needBattleEventOverlay = false;

void batteLoaderPhase2_2() {
    if (currentBattleConfig.m1_flags & 0x20) {
        needBattleEventOverlay = 1;
    }

    for (int i = 0; i < 3; i++) {
        if (battleCharacters[i] == 0x7F) // FIX: this wasn't in the original code and would read past the end of the array
            continue;

        apConfigArray[i].m4_currentAP = gameState.m16C0[battleCharacters[i]].m17_energyPerTurn;
        for (int j = 0; j < 8; j++) {
            battleVar2->m0[i].m0_circleMenuBattleCommandsMapping[j] = battleCommandsSetups[battleEntities[i].m0_base.m56_battleCommandLoadout][j];
        }
        for (int j = 0; j < 4; j++) {

            if (battleCharacters[i] == 7) {
                static const std::array<u8, 8> batteLoaderPhase2_2_arrayFor7 = { {
                        0x1F, 0x1D, 0x22, 0x2A,
                        0x1F, 0x1D, 0x22, 0x2B
                } };
                battleVar2->m0[i].m8[j] = batteLoaderPhase2_2_arrayFor7[j];
            }
            else {
                static const std::array<u8, 8> batteLoaderPhase2_2_array = { {
                        0x1F, 0x1D, 0x23, 0x2B,
                        0x1F, 0x1D, 0x23, 0x2B
                } };
                battleVar2->m0[i].m8[j] = batteLoaderPhase2_2_array[j];
            }
            static const std::array<u8, 8> batteLoaderPhase2_2_array3 = { {
                    0x2D, 0x1D, 0x1B, 0x2F,
                    0x2D, 0x1D, 0x1B, 0x2F
            } };
            battleVar2->m0[i].mC[j] = batteLoaderPhase2_2_array3[j];
        }
        battleVar2->m0[i].m3C_currentTarget = getEntityToFace(i);
        battleVisualEntities[i].m6_direction = getDirectionBetween2BattleEntities(i, battleVar2->m0[i].m3C_currentTarget);
        for (int j = 0; j < 0x10; j++) {
            battleVar2->m0[i].m1C_isCommandEnabled[j] = battleEntities[i].m0_base.m7A_commandEnabledBF & party1C_InitialValues[j];
        }
        // Disable call gear command if no gear or battle disallow gears
        if ((battleEntities[i].m0_base.mA0_partyData_gearNum == -1) || (currentBattleConfig.m1_flags & 0x40)) {
            battleVar2->m0[i].m1C_isCommandEnabled[7] = party1C_InitialValues[7];
            battleEntities[i].m0_base.m7A_commandEnabledBF |= party1C_InitialValues[7];
        }

        // Disable escape
        if (currentBattleConfig.m1_flags & 0x80) {
            battleVar2->m0[i].m1C_isCommandEnabled[8] = party1C_InitialValues[8];
            battleEntities[i].m0_base.m7A_commandEnabledBF |= party1C_InitialValues[8];
        }
    }

    for (int i = 3; i < 11; i++) {
        // figure out who each enemy is facing
        battleVar2->m0[i].m3C_currentTarget = getEntityToFace(i);
        battleVisualEntities[i].m6_direction = getDirectionBetween2BattleEntities(i, battleVar2->m0[i].m3C_currentTarget);
    }
}

u32 getRandomValueInRange(u32 param_1, u32 param_2)
{
    u32 uVar1;
    u32 uVar2;

    uVar2 = param_1 & 0xff;
    uVar1 = 0xff;
    if (uVar2 != 0xff) {
        param_2 = param_2 & 0xff;
        if (param_2 == 0) {
            uVar1 = 0;
        }
        else {
            uVar1 = uVar2;
            if (uVar2 != param_2) {
                if ((int)(param_2 - uVar2) < 0xff) {
                    uVar1 = xenoRand();
                    uVar1 = param_1 + (int)(uVar1 & 0xff) % (int)((param_2 - uVar2) + 1) & 0xff;
                }
                else {
                    uVar1 = xenoRand();
                    uVar1 = uVar1 & 0xff;
                }
            }
        }
    }
    return uVar1;
}

void updatePlayerWithStatusC000And2() {
    MissingCode();
}

u32 battleGetSlotStatusSub(u32 param_1) {
    u32 entityIndex = param_1 & 0xFF;
    battleGetSlotStatusSub_currentBattleEntity = &battleEntities[entityIndex].m0_base;
    battleGetSlotStatusSub_currentBattleEntityGear = &battleEntities[entityIndex].mA4_gear;

    u32 result2;

    if (entityIndex < 3) {
        if ((battleEntities[entityIndex].m15A_flags & 0x80) == 0) {
            currentEntityBattleStats = &partyBattleStats[entityIndex].m0[battleGetSlotStatusSub_current28Index];
            s32 index2 = battleEntities[entityIndex].m0_base.m5A_agility;
            s32 result = index2 - currentEntityBattleStats->m27;
            if ((int)(char)currentEntityBattleStats->m27 < (int)index2) {
                s32 randVar2 = result * 8;
                result2 = randVar2 + result;
            }
            else {
                result2 = 9;
            }
        }
        else {
            currentEntityBattleStats = &mechaBattleStats[entityIndex].m0[battleGetSlotStatusSub_current28Index];
            s32 index3 = (uint)(byte)battleEntities[entityIndex].mA4_gear.m98;
            s32 result = index3 - (int)(char)currentEntityBattleStats->m27;
            if ((int)(char)currentEntityBattleStats->m27 < (int)index3) {
                s32 randVar2 = result * 8;
                result2 = randVar2 + result;
            }
            else {
                result2 = 9;
            }
        }
        
    }
    else {
        s32 result = (uint)(byte)battleEntities[entityIndex].m0_base.m5A_agility;
        s32 randVar2 = result << 3;
        result2 = randVar2 + result;
    }

    if (0xa5 < (result2 & 0xffff)) {
        result2 = 0xa0;
    }
    result2 = 0xa5 - result2;
    u32 randVar1 = xenoRand();
    u32 randVar2 = randVar1;
    if (randVar1 < 0) {
        randVar2 = randVar1 + 7;
    }
    updatePlayerWithStatusC000And2();
    return result2 - (randVar1 + (randVar2 >> 3) * -8 + -4) & 0xff;
}

void battleGetSlotStatus(std::array<u8, 0xB>& param1) {
    for (int i = 0; i < 0xB; i++) {
        if (isBattleSlotFilled[i] == 0) {
            numTicksBeforeReady[i] = 0xFF;
            battleSlotStatusVar0[i] = 0xFF;
        }
        else {
            numTicksBeforeReady[i] = battleSlotStatusVar0[i] = battleGetSlotStatusSub(i);
        }
        isEntityReadyForBattle[i] = 0;
        battleSlotStatusVar1[i] = 0;
        param1[i] = 0;
    }
}

void batteLoaderPhase2_1(void) {
    battleGetSlotStatusSub_current28Index = 0;

    std::array<u8, 0xB> slotStatus;
    battleGetSlotStatus(slotStatus);

    for (int i = 0; i < 11;) {
        s32 randomValue = getRandomValueInRange(0, 10);
        if (slotStatus[randomValue] == 0) {
            slotStatus[randomValue] = 1;
            randomTurnOrder[i] = randomValue;
            i++;
        }
    }

    currentEntryInRandomTurnOrder = 0;

    for (int i = 3; i < 11; i++) {
        if (slotStatus[i] && (battleEntities[i].m0_base.m34 & 0x200)) {
            numTicksBeforeReady[i] = 1;
            battleSlotStatusVar0[i] = 1;
        }
    }

    // Figure out smallest value, so we can remove that from each initiative and start turn immediately
    u16 iVar5 = 0xFFFF;
    for (int i = 0; i < 11; i++) {
        if (isBattleSlotFilled[i]) {
            if (numTicksBeforeReady[i] < iVar5) {
                iVar5 = numTicksBeforeReady[i];
            }
        }
    }

    for (int i = 0; i < 11; i++) {
        if (isBattleSlotFilled[i]) {
            numTicksBeforeReady[i] -= iVar5;
        }
    }
}

void batteLoaderPhase3_0_sub0() {
    battleVar1->m7C[0] = 1;
    battleVar1->m7C[1] = 1;
    battleVar1->m7C[2] = 1;

    computeMenuBorder(battleFont, 0x5C, &battleVar0->mA234[0].m0, &battleVar0->mA234[0].m4_tpage_tp, &battleVar0->mA234[0].m8_clutX, &battleVar0->mA234[0].mC_clutY, &battleVar0->mA234[0].m10_tpage_X, &battleVar0->mA234[0].m14_tpage_Y);
    battleVar0->mA2AE = GetClut(battleVar0->mA234[0].m8_clutX, battleVar0->mA234[0].mC_clutY);
    battleVar0->mA2AC = GetClut(battleVar0->mA234[0].m8_clutX, battleVar0->mA234[0].mC_clutY - 1);
    battleVar0->mA2B2 = GetClut(battleVar0->mA234[0].m8_clutX, battleVar0->mA234[0].mC_clutY - 2);
    battleVar0->mA2B0 = GetClut(battleVar0->mA234[0].m8_clutX, battleVar0->mA234[0].mC_clutY - 3);

    // Original code might overflow m740_APOrFuelPoly by initializing 8 entries (while there is only 6)
    // Split in 2 loops to initialize at proper size
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            POLY_GT4& pGT4 = battleVar0->m5A0[i][j];
            SetPolyGT4(&pGT4);
            pGT4.r0 = 0x80;
            pGT4.g0 = 0x80;
            pGT4.b0 = 0x80;
            pGT4.r1 = 0x80;
            pGT4.g1 = 0x80;
            pGT4.b1 = 0x80;
            pGT4.r2 = 0x00;
            pGT4.g2 = 0x00;
            pGT4.b2 = 0x00;
            pGT4.r3 = 0x00;
            pGT4.g3 = 0x00;
            pGT4.b3 = 0x00;
            pGT4.tpage = GetTPage(battleVar0->mA234[0].m4_tpage_tp, 0, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y);
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            POLY_G4& pG4 = battleVar0->m740_APOrFuelPoly[i][j];
            SetPolyG4(&pG4);
            pG4.r2 = 0x4F;
            pG4.g2 = 0x4F;
            pG4.b2 = 0x4F;
            pG4.r3 = 0x4F;
            pG4.g3 = 0x4F;
            pG4.b3 = 0x4F;
        }
    }

    for (int i = 0; i < 6; i++) {
        for( int j=0; j<2; j++) {
            LINE_F2& pF2 = battleVar0->m908[i][j];
            SetLineF2(&pF2);
            pF2.r0 = 0xFF;
            pF2.g0 = 0xFF;
            pF2.b0 = 0xFF;
        }
    }
}

int battleSetupStringInPolyFT4Small(int character, std::array<POLY_FT4, 2>* polyArray, short x, short y) {
    return setupStringInPolyFT4(battleFont, character, polyArray, battleOddOrEven, x, y, 0x800);
}

int battleSetupStringInPolyFT4Large(int character, std::array<POLY_FT4, 2>* polyArray, short x, short y) {
    return setupStringInPolyFT4(battleFont, character, polyArray, battleOddOrEven, x, y, 0x1000);
}

void battleSetupTextPoly(POLY_FT4* param_1) {
    battleSetupTextPolySub(param_1);
    param_1->r0 = 0x40;
    param_1->g0 = 0x40;
    param_1->b0 = 0x40;
    param_1->tpage = param_1->tpage | 0x40;
}

void batteLoaderPhase3_0_sub1() {
    for (int i = 0; i < 3; i++) {
        if (battleVisualEntities[i].m2 != 0x7F) {
            battleVar1->m78[i] += battleSetupStringInPolyFT4Small(0x52, &battleVar0->m0[i][battleVar1->m78[i]], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x44) * 0x10000) >> 16, 0x24);
            int length = battleVar1->m78[i];
            battleVar1->m78[i] += battleSetupStringInPolyFT4Small(0x53, &battleVar0->m0[i][battleVar1->m78[i]], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x44) * 0x10000) >> 16, 0x24);

            for (int j = length; j < battleVar1->m78[i]; j++) {
                battleSetupTextPoly(&battleVar0->m0[i][j][battleOddOrEven]);
            }

            battleSetupStringInPolyFT4Large(i + 0x61, &battleVar0->m818[i], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x1C) * 0x10000) >> 16, 0x14);
            battleVar0->m835C[i].m1E2 = battleSetupStringInPolyFT4Large(0x90, &battleVar0->m835C[i].m0[0], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x38) * 0x10000) >> 16, 0x27);
            battleVar0->m835C[i].m1E3 = battleSetupStringInPolyFT4Large(0x91, &battleVar0->m835C[i].mA0[0], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x3C) * 0x10000) >> 16, 0x27);
            battleVar0->m835C[i].m1E0 = battleOddOrEven;
        }
    }

    battleVar1->mA2 = battleOddOrEven;
    battleVar1->m83 = battleOddOrEven;
}

void batteLoaderPhase3_0() {
    batteLoaderPhase3_0_sub0();
    batteLoaderPhase3_0_sub1();
}

std::vector<u16>* allocateTextureRamForText(int param_1)
{
    resetMemoryAllocStats(2, 0);
    std::vector<u16>* newRaw = new std::vector<u16>;
    newRaw->resize(13 * (param_1 + 3), 0);
    return newRaw;
}

std::array<sRamTexture*, 10> itemLabelsIds;

std::vector<u8>::const_iterator getItemLabelString(int param_1)
{
    return getDialogParamPointer(printDialogTextVar[18], param_1);
}

void renderItemTargetsLabels() {
    for (int i = 0; i < 10; i++) {
        itemLabelsIds[i] = allocateTextureRamForText(4);
        renderString(getItemLabelString(i), *itemLabelsIds[i], 2, 0);
    }
}

void setupBattleVar0Polys() {

    RECT localRect;
    localRect.x = 0;
    localRect.y = 0;
    localRect.h = 0x100;
    localRect.w = 0x100;

    for (int i = 0; i < 2; i++) {
        SetPolyF4(&battleVar0->m63C8[i]);
        battleVar0->m63C8[i].r0 = 0xFF;
        battleVar0->m63C8[i].g0 = 0xFF;
        battleVar0->m63C8[i].b0 = 0xFF;
        SetSemiTrans(&battleVar0->m63C8[i], 1);
        SetDrawMode(&battleVar0->m63F8[i], 0, 0, GetTPage(0, 2, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y), &localRect);
    }

    battleVar0->m6415 = 0;
    battleVar0->m6410 = 0xFF;
    battleVar0->m6416 = 0;
}

void batteLoaderPhase3_1() {
    setupBattleVar0Polys();
    renderItemTargetsLabels();
}

std::array<sApStruct, 11> apConfigArray;

void batteLoaderPhase1_3() {
    MissingCode();

    for (int i = 0; i < 3; i++) {
        if (battleVisualEntities[i].m2 == 0x7F) {
            apConfigArray[i].m1 = 0;
            battleEntities[i].m15A_flags &= 0x7F;
            battleVar0->m835C[i].m1E1 = 0;
        }
        else {
            battleVar0->m835C[i].m1E1 = 1; // display AP
            if (battleVisualEntities[i].m4_isGear == 0) {
                apConfigArray[i].m1 = 0;
                battleEntities[i].m15A_flags &= 0x7F;
            }
            else {
                apConfigArray[i].m1 = 0;
                battleEntities[i].m15A_flags |= 0x80;
                if (battleVisualEntities[i].m2 != 0xA) { // chuchu?
                    battleVar0->m835C[i].m1E1 = 2; // display Fuel
                }
            }
        }
    }

    MissingCode();
}

std::vector<u8>::const_iterator currentBattleSpecificStrings;

void loadEnemies() {
    performAttackSub3_var0 = 0;
    currentBattleSpecificStrings = battleConfigFile2.m30_strings;
    for (int i = 3; i < 11; i++) {
        unknownMonsterStatus0[i - 3].m3 = 0;
        if (battleVisualEntities[3].m2 == 0x7F) {
            memset(&battleEntities[i], 0, sizeof(battleEntities[i]));
            unknownMonsterStatus0[i - 3].m0 = 0;
            unknownMonsterStatus0[i - 3].m1 = 0;
        }
        else {
            // If that happens, we are using scripts from previous battle, those need to be cleared!
            assert(!monstersScriptsEntities[i - 3].m0_scripts[0].has_value());
            assert(!monstersScriptsEntities[i - 3].m0_scripts[1].has_value());
            assert(!monstersScriptsEntities[i - 3].m0_scripts[2].has_value());
            assert(!monstersScriptsEntities[i - 3].m0_scripts[3].has_value());

            battleEntities[i] = battleConfigFile2.m32[battleVisualEntities[3].m2];
            monstersScriptsEntities[i - 3].m0_scripts[0] = battleConfigFile2.getPointerForScriptSub(battleVisualEntities[3].m2, 0, 0);
            monstersScriptsEntities[i - 3].m0_scripts[1] = battleConfigFile2.getPointerForScriptSub(battleVisualEntities[3].m2, 1, 0);
            if (battleConfigFile2.m0_scriptsOffsets[battleVisualEntities[3].m2][2] == -1) {
                unknownMonsterStatus0[i - 3].m0 = 0;
            }
            else {
                monstersScriptsEntities[i - 3].m0_scripts[2] = battleConfigFile2.getPointerForScriptSub(battleVisualEntities[3].m2, 2, 0);
                unknownMonsterStatus0[i - 3].m0 = 1;
            }
            if (battleConfigFile2.m0_scriptsOffsets[battleVisualEntities[3].m2][3] == -1) {
                unknownMonsterStatus0[i - 3].m1 = 0;
            }
            else {
                monstersScriptsEntities[i - 3].m0_scripts[3] = battleConfigFile2.getPointerForScriptSub(battleVisualEntities[3].m2, 3, 0);
                unknownMonsterStatus0[i - 3].m1 = 1;
            }
            for (int j = 0; j < 4; j++) {
                monstersScriptsEntities[i - 3].m10_s32Vars[j] = 0;
            }
            for (int j = 0; j < 8; j++) {
                monstersScriptsEntities[i - 3].m20_s16Vars[j] = 0;
            }
            for (int j = 0; j < 16; j++) {
                monstersScriptsEntities[i - 3].m30_s8Vars[j] = 0;
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
        loadEnemies();
        batteLoaderPhase1_3();
        break;
    case 2:
        initBattleInventory();
        batteLoaderPhase2_1();
        batteLoaderPhase2_2();
        battleG3 = new sBattle800c3e24();
        dummpyLoaderEnd();
        break;
    case 3:
        batteLoaderPhase3_0();
        batteLoaderPhase3_1();
        break;
    default:
        assert(0);
    }
}
