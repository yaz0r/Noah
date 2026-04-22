#include "noahLib.h"
#include "battle.h"
#include "battleLoader.h"

#include "enemyScript.h"

std::array<sBattleScriptEntity, 8> monstersScriptsEntities;
std::array<sBattleScriptContext, 32> battleScriptContext;

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);


int executeMonsterScriptWhenAttacked(char param_1) {
    int result = 0;
    int result2 = 0;
    s8 numIterations = 0;;
    if (((battleEntities[param_1].m0_base.m7C & 0x8000) == 0) ||
        (result = 0, (battleEntities[param_1].m0_base.m34 & 0x800) != 0)) {
        battleScriptContext[0].m0_attackType = 0;
        result = result2;
        if (unknownMonsterStatus0[param_1 - 3].m0 != 0) {
            sBattleScriptExecutionContext localContext;
            localContext.m0_scriptPtr = monstersScriptsEntities[param_1 - 3].m0_scripts[2].value();
            memset(battleScriptContext.data(), 0, sizeof(battleScriptContext));
            while (true) {
                result = result2;
                if ((*localContext.m0_scriptPtr == 0xFD) || (*localContext.m0_scriptPtr == 0xFF))
                    break;
                if (*localContext.m0_scriptPtr < 0x80) {
                    if (*localContext.m0_scriptPtr == 0x62) {
                        result2 = 1;
                    }
                    numIterations = executeMonsterScriptLower(&localContext, param_1 - 3, numIterations);
                }
                else if(executeMonsterScriptUpper(&localContext, param_1 + -3) == 0) {
                    skipScriptIfFalse(&localContext);
                }
            }
        }
        if (battleScriptContext[0].m0_attackType) {
            executeBattleCode(param_1);
        }
    }

    return result;
}

void advanceBattleScript(sBattleScriptExecutionContext* pScriptContext) {
    pScriptContext->m0_scriptPtr += 4;
}

void monsterBattleOpcode_setByteVar(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte = param_1->m0_scriptPtr[2];
}

void monsterBattleOpcode_addByteVar(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte = std::min<u8>(*pByte + param_1->m0_scriptPtr[2], 0xFF);
}

void monsterBattleOpcode_moduloByteVar(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte %= param_1->m0_scriptPtr[2];
}

bool isEnemySlotTargetable(uint param_1, char param_2) {
    param_1 &= 0xFF;
    if (isBattleSlotFilled[param_1] && (battleEntities[param_1].m0_base.m7C & 0xc002) == 0) {
        if (param_2 != 0) {
            return true;
        }
        return (battleEntities[param_1].m0_base.m84 & 0x20) == 0;
    }
    return false;
}

bool isMonsterTargetValid(uint param_1, char param_2) {
    param_1 &= 0xFF;

    if (isBattleSlotFilled[param_1]) {
        if (battleVisualEntities[param_1].m3 == 0) {
            if ((battleEntities[param_1].m0_base.m7C & 0xc002) == 0) {
                if (param_2 == 0) {
                    return (battleEntities[param_1].m0_base.m84 & 0x20) == 0;
                }
                return true;
            }
        }
    }
    return false;
}

u32 getRandomValueInRange(u32 param_1, u32 param_2);

void monsterBattleOpcode_selectRandomPC(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<bool, 3> wasTargetTestedYet;
    wasTargetTestedYet.fill(false);

    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 targetId = getRandomValueInRange(0, 2) & 0xff; // choose a target
        if (!wasTargetTestedYet[targetId]) {
            if (isMonsterTargetValid(targetId, param_1->m0_scriptPtr[2])) {
                if (apConfigArray[targetId].m1 == 0) {
                    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(targetId);
                    return;
                }
            }
            wasTargetTestedYet[targetId] = true;
        }
    } while ((wasTargetTestedYet[2] & wasTargetTestedYet[0] & wasTargetTestedYet[1]) == 0);
}

void monsterBattleOpcode_selectRandomPCSameRow(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<bool, 3> wasTargetTestedYet;
    wasTargetTestedYet.fill(false);

    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 targetId = getRandomValueInRange(0, 2) & 0xff; // choose a target
        if (!wasTargetTestedYet[targetId]) {
            if (isMonsterTargetValid(targetId, param_1->m0_scriptPtr[2])) {
                if (battleVisualEntities[(param_2 & 0xff) + 3].m0_positionSlot == battleVisualEntities[targetId].m0_positionSlot) {
                    if (apConfigArray[targetId].m1 == 0) {
                        monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(targetId);
                        return;
                    }
                }
            }
            wasTargetTestedYet[targetId] = true;
        }
    } while ((wasTargetTestedYet[2] & wasTargetTestedYet[0] & wasTargetTestedYet[1]) == 0);
}

void monsterBattleOpcode_selectRandomPCDiffRow(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<bool, 3> wasTargetTestedYet;
    wasTargetTestedYet.fill(false);

    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 targetId = getRandomValueInRange(0, 2) & 0xff; // choose a target
        if (!wasTargetTestedYet[targetId]) {
            if (isMonsterTargetValid(targetId, param_1->m0_scriptPtr[2])) {
                if (battleVisualEntities[(param_2 & 0xff) + 3].m0_positionSlot != battleVisualEntities[targetId].m0_positionSlot) { // this differs from opcode 0x41
                    if (apConfigArray[targetId].m1 == 0) {
                        monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(targetId);
                        return;
                    }
                }
            }
            wasTargetTestedYet[targetId] = true;
        }
    } while ((wasTargetTestedYet[2] & wasTargetTestedYet[0] & wasTargetTestedYet[1]) == 0);
}

void monsterBattleOpcode_selectRandomGearPC(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<bool, 3> wasTargetTestedYet;
    wasTargetTestedYet.fill(false);

    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 targetId = getRandomValueInRange(0, 2);
        if (!wasTargetTestedYet[targetId]) {
            if (isMonsterTargetValid(targetId, param_1->m0_scriptPtr[2])) {
                if (apConfigArray[targetId].m1 != 0) { // this differs from opcode 0x40
                    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(targetId);
                    return;
                }
            }
            wasTargetTestedYet[targetId] = true;
        }
    } while ((wasTargetTestedYet[2] & wasTargetTestedYet[0] & wasTargetTestedYet[1]) == 0);
}

int monsterBattleOpcode_countEnemiesSameRow_findFirstInBitmask(u16 bitmask)
{
    int i = 0;
    for (; i < 11; i++) {
        if (bitmaskCharacterCheck(bitmask, i))
            break;
    }
    return i & 0xff; // returns 11 if no match (PSX behavior: loop exits with i=11)
}

void monsterBattleOpcode_countEnemiesSameRow(sBattleScriptExecutionContext* param_1, uint param_2) {
    u8 destVarIndex = param_1->m0_scriptPtr[1];
    u8 maskVarIndex = param_1->m0_scriptPtr[2];

    s8 result = 0;
    for (int i = 3; i < 11; i++) {
        if (isMonsterTargetValid(i, 0)) {
            u16 bitMask = monstersScriptsEntities[param_2].m20_s16Vars[maskVarIndex];
            if (battleVisualEntities[i].m0_positionSlot == battleVisualEntities[monsterBattleOpcode_countEnemiesSameRow_findFirstInBitmask(bitMask) & 0xff].m0_positionSlot) {
                result++;
            }
        }
    }
    monstersScriptsEntities[param_2].m30_s8Vars[destVarIndex] = result;
}

std::array<s16, 16> gameState_2324_copy;

void monsterBattleOpcode_loadGameStateVar(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = gameState_2324_copy[param_1->m0_scriptPtr[2]];
}

void monsterBattleOpcode_randomByteVar(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m30_s8Vars[param_1->m0_scriptPtr[1]] = getRandomValueInRange(0, param_1->m0_scriptPtr[2]);
}

void monsterBattleOpcode_copyVarToContext(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, uint param_3, uint param_4) {
    s16 value = monstersScriptsEntities[param_3].m20_s16Vars[param_1->m0_scriptPtr[2]];
    
    switch (param_1->m0_scriptPtr[1]) {
    case 6:
        param_2[param_4 & 0xff].m6_mask = value;
        break;
    default:
        assert(0);
    }
}

void monsterBattleOpcode_selectRandomHiddenEnemy(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = 0;
    std::array<u8, 8> candidates;
    int numCandidates = 0;
    for (u32 i = 3; i < 11; i++) {
        if (isEnemySlotTargetable(i, param_1->m0_scriptPtr[2])) {
            if (battleVisualEntities[i].m3 & 0x80) {
                candidates[numCandidates] = (u8)i;
                numCandidates++;
            }
        }
    }
    if (numCandidates != 0) {
        u32 pick = getRandomValueInRange(0, (numCandidates - 1) & 0xff);
        monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(candidates[pick & 0xff]);
    }
}

void monsterBattleOpcode_storeSelfBitmask(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(param_2 + 3);
}

void monsterBattleOpcode_forceNextTurn(sBattleScriptExecutionContext* param_1, uint param_2) {
    u16 bitmask = monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]];
    assert(bitmask != 0); // empty bitmask → findFirstInBitmask returns 11 → out of range forced turn
    int entityIndex = monsterBattleOpcode_countEnemiesSameRow_findFirstInBitmask(bitmask);
    assert(entityIndex < 11); // no matching entity in bitmask
    bBattleTickMode1 = (u8)(entityIndex + 1);
}

byte monsterBattleOpcode_writeContextByte(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, byte param_3) {
    (&param_2[param_3].m0_attackType)[(byte)param_1->m0_scriptPtr[1]] = param_1->m0_scriptPtr[2];
    if (param_1->m0_scriptPtr[1] == '\0') {
        param_3 = param_3 + 1;
    }
    return param_3;
}

void monsterBattleOpcode_writeContextWord(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, byte param_3) {
    u8* base = &param_2[param_3].m0_attackType;
    u8 offset = (u8)param_1->m0_scriptPtr[1];
    base[offset] = param_1->m0_scriptPtr[2];
    base[offset + 1] = param_1->m0_scriptPtr[3];
}

int executeMonsterScriptLower(sBattleScriptExecutionContext* pScriptContext, int entityIndex, int numIterations) {
    u8 bytecode = *pScriptContext->m0_scriptPtr;
    switch (bytecode) {
    case 1:
        numIterations = monsterBattleOpcode_writeContextByte(pScriptContext, battleScriptContext, numIterations);
        break;
    case 4:
        monsterBattleOpcode_setByteVar(pScriptContext, entityIndex);
        break;
    case 8:
        monsterBattleOpcode_addByteVar(pScriptContext, entityIndex);
        break;
    case 0xC:
        monsterBattleOpcode_moduloByteVar(pScriptContext, entityIndex);
        break;
    case 0x30:
        monsterBattleOpcode_loadGameStateVar(pScriptContext, entityIndex);
        break;
    case 0x3D:
        monsterBattleOpcode_writeContextWord(pScriptContext, battleScriptContext, numIterations);
        break;
    case 0x3E:
        monsterBattleOpcode_randomByteVar(pScriptContext, entityIndex);
        break;
    case 0x40:
        monsterBattleOpcode_selectRandomPC(pScriptContext, entityIndex);
        break;
    case 0x41:
        monsterBattleOpcode_selectRandomPCSameRow(pScriptContext, entityIndex);
        break;
    case 0x43:
        monsterBattleOpcode_selectRandomPCDiffRow(pScriptContext, entityIndex);
        break;
    case 0x4A:
        monsterBattleOpcode_selectRandomGearPC(pScriptContext, entityIndex);
        break;
    case 0x50:
        monsterBattleOpcode_countEnemiesSameRow(pScriptContext, entityIndex);
        break;
    case 0x52:
        monsterBattleOpcode_copyVarToContext(pScriptContext, battleScriptContext, entityIndex, numIterations);
        break;
    case 0x56:
        monsterBattleOpcode_selectRandomHiddenEnemy(pScriptContext, entityIndex);
        break;
    case 0x64:
        monsterBattleOpcode_storeSelfBitmask(pScriptContext, entityIndex);
        break;
    case 0x73:
        monsterBattleOpcode_forceNextTurn(pScriptContext, entityIndex);
        break;

    default:
        assert(0);
    }

    advanceBattleScript(pScriptContext);
    return numIterations;
}

bool executeMonsterScriptUpper_81(sBattleScriptExecutionContext* param_1, uint param_2) {
    return monstersScriptsEntities[param_2].m30_s8Vars[param_1->m0_scriptPtr[1]] == param_1->m0_scriptPtr[2];
}

bool executeMonsterScriptUpper_82(sBattleScriptExecutionContext* param_1, uint param_2) {
    return monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] == READ_LE_S16(param_1->m0_scriptPtr + 2);
}

bool monsterBattleOpcode_90(sBattleScriptExecutionContext* param_1, uint param_2) {
    return monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] != READ_LE_S16(param_1->m0_scriptPtr + 2);
}

int executeMonsterScriptUpper(sBattleScriptExecutionContext* pScriptContext, int entityIndex) {
    byte operationResult;
    byte currentResult;
    byte unk;
    bool exitScript;

    operationResult = 0;
    exitScript = false;
    unk = 0;
    do {
        switch (*pScriptContext->m0_scriptPtr) {
        case 0x80:
        case 0x9a:
            operationResult = 1;
            break;
        case 0x81:
            operationResult = executeMonsterScriptUpper_81(pScriptContext, entityIndex);
            break;
        case 0x82:
            operationResult = executeMonsterScriptUpper_82(pScriptContext, entityIndex);
            break;
        case 0x83: // m30[arg1] <= imm8
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] <= pScriptContext->m0_scriptPtr[2];
            break;
        case 0x84: // m20[arg1] <= imm16 (unsigned)
            operationResult = (u16)monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] <= READ_LE_U16(pScriptContext->m0_scriptPtr + 2);
            break;
        case 0x85: // m30[arg1] >= imm8
            operationResult = pScriptContext->m0_scriptPtr[2] <= monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]];
            break;
        case 0x86: // m20[arg1] >= imm16 (unsigned)
            operationResult = READ_LE_U16(pScriptContext->m0_scriptPtr + 2) <= (u16)monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]];
            break;
        case 0x87: // m30[arg1] == m30[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] == monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x88: // m20[arg1] == m20[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] == monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x89: // m30[arg1] <= m30[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] <= monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x8a: // m20[arg1] <= m20[arg2] (unsigned)
            operationResult = (u16)monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] <= (u16)monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x8b: // m30[arg1] & imm8
            operationResult = (monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] & pScriptContext->m0_scriptPtr[2]) != 0;
            break;
        case 0x8c: // m20[arg1] & imm16
            operationResult = (monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] & READ_LE_U16(pScriptContext->m0_scriptPtr + 2)) != 0;
            break;
        case 0x8d: // m30[arg1] & m30[arg2]
            operationResult = (monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] & monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[2]]) != 0;
            break;
        case 0x8e: // m20[arg1] & m20[arg2]
            operationResult = (monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] & monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[2]]) != 0;
            break;
        case 0x8f: // m30[arg1] != imm8
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] != pScriptContext->m0_scriptPtr[2];
            break;
        case 0x90:
            operationResult = monsterBattleOpcode_90(pScriptContext, entityIndex);
            break;
        case 0x91: // m30[arg1] != m30[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[1]] != monstersScriptsEntities[entityIndex & 0xff].m30_s8Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x92: // m20[arg1] != m20[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[1]] != monstersScriptsEntities[entityIndex & 0xff].m20_s16Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x93: // m10[arg1] == m10[arg2]
            operationResult = monstersScriptsEntities[entityIndex & 0xff].m10_s32Vars[pScriptContext->m0_scriptPtr[1]] == monstersScriptsEntities[entityIndex & 0xff].m10_s32Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x94: // m10[arg1] <= m10[arg2] (unsigned)
            operationResult = (u32)monstersScriptsEntities[entityIndex & 0xff].m10_s32Vars[pScriptContext->m0_scriptPtr[1]] <= (u32)monstersScriptsEntities[entityIndex & 0xff].m10_s32Vars[pScriptContext->m0_scriptPtr[2]];
            break;
        case 0x95: // entity m7C high bit
            operationResult = battleEntities[pScriptContext->m0_scriptPtr[1]].m0_base.m7C >> 0xf;
            break;
        case 0x96: // slot layout empty check
            operationResult = battleSlotLayout[pScriptContext->m0_scriptPtr[1]][0] == 0;
            break;
        case 0x97: // no player characters in slots 0-1
            operationResult = (allPlayerCharacterBitmask & 3) == 0;
            break;
        case 0x98: { // all monsters hidden
            operationResult = 1;
            for (int i = 0; i < 8; i++) {
                if ((allPlayerCharacterBitmask >> 5) != 0 && (battleVisualEntities[i + 3].m3 & 0x80) == 0) {
                    operationResult = 0;
                    break;
                }
            }
            break;
        }
        case 0x99:
            exitScript = true;
            break;
        case 0x9B:
            operationResult = battleVisualEntities[(entityIndex & 0xff) + 3].m3 >> 7;
            break;
        default:
            assert(0);
            break;
        }
        advanceBattleScript(pScriptContext);
        currentResult = operationResult;
    } while ((exitScript) &&
        (currentResult = unk | operationResult, unk = currentResult,
            0x7f < *pScriptContext->m0_scriptPtr));
    return currentResult;
}

void skipScriptIfFalse(sBattleScriptExecutionContext* param_1)
{
    while (0x7f < *param_1->m0_scriptPtr) {
        advanceBattleScript(param_1);
    }
    while (0x6f < (u8)(*param_1->m0_scriptPtr + 0x80)) {
        advanceBattleScript(param_1);
    }
}

void executeMonsterScript(char param_1, int param_2) {
    memset(&battleScriptContext, 0, sizeof(battleScriptContext));

    for (int i = 0; i < 32; i++) {
        battleCurrentDamages[i].m47_battleAnimationToPlay = -1;
    }
    
    int numIterations = 0;
    sBattleScriptExecutionContext local_18;
    assert(monstersScriptsEntities[param_1 - 3].m0_scripts[0].has_value()); // PSX doesn't check — uninitialized enemy script?
    local_18.m0_scriptPtr = monstersScriptsEntities[param_1 - 3].m0_scripts[0].value();
    while (1) {
        u8 bytecode = *local_18.m0_scriptPtr;
        if (bytecode == 0xFD)
            break;
        if (bytecode == 0xFF)
            break;

        if (bytecode < 0x80) {
            numIterations = executeMonsterScriptLower(&local_18, param_1 - 3, numIterations);
        }
        else {
            if (executeMonsterScriptUpper(&local_18, param_1 + -3) == 0) {
                skipScriptIfFalse(&local_18);
            }
        }
    }
}

void moveEntityToOtherEntity(byte param_1, uint param_2);
void moveEntityToShootEntity(uint param_1, uint param_2);
bool initJumpData(uint entity, uint target);
void setCameraVisibleEntities(uint playerBitmask);

void executeBattleCode_finalizeDamageEntry(byte param_1, uint param_2) {
    short sVar1;
    sBattleVar2* psVar2;

    sVar1 = battleScriptContext[param_2 & 0xff].m4;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    psVar2 = battleVar2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = sVar1;
    psVar2->m2DA_indexInBattleVar48 = psVar2->m2DA_indexInBattleVar48 + 1;
}

void executeBattleCode_copyMonsterToSlot(byte param_1, uint param_2) {
    // Find the first entity in the target bitmask
    int sourceEntity = 0;
    for (int i = 0; i < 11; i++) {
        if (bitmaskCharacterCheck(battleScriptContext[param_2 & 0xff].m6_mask, i)) {
            sourceEntity = i;
            break;
        }
    }

    uint destSlot = (uint)param_1;
    uint srcSlot = sourceEntity & 0xff;
    int destMonsterIdx = destSlot - 3;
    int srcMonsterIdx = srcSlot - 3;

    battleMonsterMapping[destSlot] = battleMonsterMapping[srcSlot];
    monstersScriptsEntities[destMonsterIdx].m0_scripts[0] = monstersScriptsEntities[srcMonsterIdx].m0_scripts[0];
    monstersScriptsEntities[destMonsterIdx].m0_scripts[1] = monstersScriptsEntities[srcMonsterIdx].m0_scripts[1];
    monstersScriptsEntities[destMonsterIdx].m0_scripts[2] = monstersScriptsEntities[srcMonsterIdx].m0_scripts[2];
    monstersScriptsEntities[destMonsterIdx].m0_scripts[3] = monstersScriptsEntities[srcMonsterIdx].m0_scripts[3];
    unknownMonsterStatus0[destMonsterIdx].m0 = unknownMonsterStatus0[srcMonsterIdx].m0;
    unknownMonsterStatus0[destMonsterIdx].m1 = unknownMonsterStatus0[srcMonsterIdx].m1;
    memmove(&battleEntities[destSlot], &battleEntities[srcSlot], 0x170);

    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = (s16)sourceEntity;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -5;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    battleVar2->m2DA_indexInBattleVar48 = battleVar2->m2DA_indexInBattleVar48 + 1;
}

void executeBattleCode_moveAndAttack(byte param_1, uint param_2, byte param_3)

{
    sBattleVar2* psVar1;
    uint uVar2;

    uVar2 = (uint)param_1;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -3;
    psVar1 = battleVar2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = 0;
    battleCurrentDamages[psVar1->m2DA_indexInBattleVar48].m16_targetBitMask =
        battleScriptContext[param_2 & 0xff].m6_mask;
    initJumpData(uVar2, param_3);
    if (battleScriptContext[param_2 & 0xff].m4 != '\x01') {
        if (apConfigArray[uVar2].m1 == 0) {
            moveEntityToOtherEntity(uVar2, param_3);
        }
        else {
            moveEntityToShootEntity(uVar2, param_3);
        }
    }
    uVar2 = characterIdToTargetBitmask(param_1);
    setCameraVisibleEntities((uint)battleScriptContext[param_2 & 0xff].m6_mask | uVar2 & 0xffff);
    executeBattleCode_finalizeDamageEntry(param_1, param_2 & 0xff);
    return;
}

extern s8 battleTickMain_var0;

void executeBattleCode_specialAnimation(byte param_1, uint param_2)
{
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -10;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = battleScriptContext[param_2 & 0xff].m6_mask;
    executeBattleCode_finalizeDamageEntry(param_1, param_2);
    return;
}

void executeBattleCode_playAnimation(byte param_1, uint param_2)
{
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = battleScriptContext[param_2 & 0xff].m4;
    executeBattleCode_finalizeDamageEntry(param_1, param_2 & 0xff);
    return;
}

void executeBattleCode_showMonsterName(uint param_1, byte param_2)

{
    sBattleVar2* psVar1;
    char* pcVar2;
    int iVar3;
    ushort uVar4;

    if (battleTickMain_var0 < 9) {
        MissingCode();
        /*
        pcVar2 = getDialogParamPointer(battleStrings, (uint)(byte)battleScriptContext[param_1 & 0xff].field_0x3);
        iVar3 = renderString(pcVar2, (ushort*)
            (&ramBufferFromMonsterName)[(uint)battleTickMain_var0 * 0x18], 0x39,
            battleTickMain_var0 & 1);
        (&currentMonsterNameStringWidth)[(uint)battleTickMain_var0 * 0x60] = (char)iVar3;
        loadImageSync(&DAT_Battle__800d3718 + (uint)battleTickMain_var0 * 0x30,
            (&ramBufferFromMonsterName)[(uint)battleTickMain_var0 * 0x18]);*/
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_2;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -6;
        psVar1 = battleVar2;
        uVar4 = (ushort)battleTickMain_var0;
        battleTickMain_var0 = battleTickMain_var0 + 1;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = uVar4;
        psVar1->m2DA_indexInBattleVar48 = psVar1->m2DA_indexInBattleVar48 + 1;
    }
    return;
}

void executeBattleCode_performAttack(byte param_1, uint param_2)
{
    int iVar1;
    
    param_2 = param_2 & 0xff;
    //FUN_Battle__8007893c(param_2, param_1);
    MissingCode();
    battleVar2->m2DC = battleScriptContext[param_2].m1 + 1;
    performAttackSub3(param_1, battleScriptContext[param_2].m6_mask, battleScriptContext[param_2].m2);
    performAttackSub6(battleVar2->m2DA_indexInBattleVar48);
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = battleScriptContext[param_2].m2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = entitiesHitInCurrentAttackBF;
    executeBattleCode_finalizeDamageEntry(param_1, param_2);
    if (battleScriptContext[param_2].m6_mask != entitiesHitInCurrentAttackBF)
    {
        for (int i = 0; i < battleVar2->m2DA_indexInBattleVar48; i++) {
            if (battleCurrentDamages[i].m47_battleAnimationToPlay == -3) {
                battleCurrentDamages[i].m16_targetBitMask = entitiesHitInCurrentAttackBF;
                return;
            }
        }
    }
    return;
}

void executeBattleCode_hideMonsterName(byte param_1) {
    if (battleTickMain_var0 != 0) {
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -8;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = battleTickMain_var0 - 1;
        battleVar2->m2DA_indexInBattleVar48 = battleVar2->m2DA_indexInBattleVar48 + 1;
    }
}

void executeBattleCode_statusEffect(ushort param_1, s8 param_2) {
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -9;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = param_1 & 0xff;
    battleVar2->m2DA_indexInBattleVar48++;
}

void executeBattleCode(byte param_1) {
    int numIteration = 0;
    bool continueScript = true;
    int opcodeOffset = 0;

    do {
        int uVar4;
        for (int i = 0; i < 11; i++) {
            uVar4 = i;
            if (bitmaskCharacterCheck(battleScriptContext[opcodeOffset].m6_mask, i)) {
                break;
            }
            uVar4 = 0;
        }
        performAttackSub2();
        switch (battleScriptContext[opcodeOffset].m0_attackType) {
        default:
            assert(0);
            continueScript = false;
            break;
        case 0:
            continueScript = false;
            break;
        case 1:
            executeBattleCode_performAttack(param_1, numIteration & 0xff);
            break;
        case 2:
            executeBattleCode_moveAndAttack(param_1, numIteration & 0xff, uVar4 & 0xff);
            break;
        case 0x4:
            executeBattleCode_playAnimation(param_1, numIteration & 0xff);
            break;
        case 0x7:
            executeBattleCode_copyMonsterToSlot(param_1, numIteration & 0xff);
            break;
        case 0xC:
            executeBattleCode_showMonsterName(numIteration & 0xff, param_1);
            break;
        case 0xe:
            executeBattleCode_statusEffect(battleScriptContext[opcodeOffset].m4, param_1);
            break;
        case 0xF:
            executeBattleCode_specialAnimation(param_1, numIteration & 0xff);
            break;
        case 0x10:
            executeBattleCode_hideMonsterName(param_1);
            break;
        }

        numIteration++;
        opcodeOffset++;
        if ((0x1f < (int)numIteration) && (!continueScript)) {
            return;
        }

    } while (true);
}