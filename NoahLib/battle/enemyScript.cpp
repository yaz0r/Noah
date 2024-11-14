#include "noahLib.h"
#include "battle.h"

#include "enemyScript.h"

std::array<sBattleScriptEntity, 8> monstersScriptsEntities;
std::array<sBattleScriptContext, 32> battleScriptContext;

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);

struct sBattleScriptExecutionContext {
    std::vector<u8>::iterator m0_scriptPtr;
};

int executeMonsterScriptLower(sBattleScriptExecutionContext* pScriptContext, int entityIndex, int numIterations);
int executeMonsterScriptUpper(sBattleScriptExecutionContext* pScriptContext, int entityIndex);
void skipScriptIfFalse(sBattleScriptExecutionContext* param_1);

int executeMonsterScriptWhenAttacked(char param_1) {
    int result = 0;
    int result2 = 0;
    s8 numIterations = 0;;
    if ((battleEntities[param_1].m0_base.m7C & 0x8000) == 0) {
        result = 0;
        if (battleEntities[param_1].m0_base.m34 & 0x800) {
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
    }

    return result;
}

void advanceBattleScript(sBattleScriptExecutionContext* pScriptContext) {
    pScriptContext->m0_scriptPtr += 4;
}

void monsterBattleOpcode_4(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte = param_1->m0_scriptPtr[2];
}

void monsterBattleOpcode_addByte(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte = std::min<u8>(*pByte + param_1->m0_scriptPtr[2], 0xFF);
}

void monsterBattleOpcode_modulo(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_s8Vars.begin() + param_1->m0_scriptPtr[1];
    *pByte %= param_1->m0_scriptPtr[2];
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

void monsterBattleOpcode_40(sBattleScriptExecutionContext* param_1, uint param_2) {
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

void monsterBattleOpcode_41(sBattleScriptExecutionContext* param_1, uint param_2) {
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

void monsterBattleOpcode_43(sBattleScriptExecutionContext* param_1, uint param_2) {
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

void monsterBattleOpcode_4A(sBattleScriptExecutionContext* param_1, uint param_2) {
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

int monsterBattleOpcode_50Sub(u16 bitmask)
{
    for(int i=0; i<11; i++) {
        if (bitmaskCharacterCheck(bitmask, i))
            return i;
    }
    return 10; // always return the last entry if everything fails
}

void monsterBattleOpcode_50(sBattleScriptExecutionContext* param_1, uint param_2) {
    u8 destVarIndex = param_1->m0_scriptPtr[1];
    u8 maskVarIndex = param_1->m0_scriptPtr[2];

    s8 result = 0;
    for (int i = 3; i < 11; i++) {
        if (isMonsterTargetValid(i, 0)) {
            u16 bitMask = monstersScriptsEntities[param_2].m20_s16Vars[maskVarIndex];
            if (battleVisualEntities[i].m0_positionSlot == battleVisualEntities[monsterBattleOpcode_50Sub(bitMask) & 0xff].m0_positionSlot) {
                result++;
            }
        }
    }
    monstersScriptsEntities[param_2].m30_s8Vars[destVarIndex] = result;
}

std::array<s16, 16> gameState_2324_copy;

void monsterBattleOpcode_30(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = gameState_2324_copy[param_1->m0_scriptPtr[2]];
}

void monsterBattleOpcode_3E(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m30_s8Vars[param_1->m0_scriptPtr[1]] = getRandomValueInRange(0, param_1->m0_scriptPtr[2]);
}

void monsterBattleOpcode_52(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, uint param_3, uint param_4) {
    s16 value = monstersScriptsEntities[param_3].m20_s16Vars[param_1->m0_scriptPtr[2]];
    
    switch (param_1->m0_scriptPtr[1]) {
    case 6:
        param_2[param_4 & 0xff].m6_mask = value;
        break;
    default:
        assert(0);
    }
}

void monsterBattleOpcode_64(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20_s16Vars[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(param_2 + 3);
}

byte monsterBattleOpcode_1(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, byte param_3) {
    (&param_2[param_3].m0_attackType)[(byte)param_1->m0_scriptPtr[1]] = param_1->m0_scriptPtr[2];
    if (param_1->m0_scriptPtr[1] == '\0') {
        param_3 = param_3 + 1;
    }
    return param_3;
}

int executeMonsterScriptLower(sBattleScriptExecutionContext* pScriptContext, int entityIndex, int numIterations) {
    u8 bytecode = *pScriptContext->m0_scriptPtr;
    switch (bytecode) {
    case 1:
        numIterations = monsterBattleOpcode_1(pScriptContext, battleScriptContext, numIterations);
        break;
    case 4:
        monsterBattleOpcode_4(pScriptContext, entityIndex);
        break;
    case 8:
        monsterBattleOpcode_addByte(pScriptContext, entityIndex);
        break;
    case 0xC:
        monsterBattleOpcode_modulo(pScriptContext, entityIndex);
        break;
    case 0x30:
        monsterBattleOpcode_30(pScriptContext, entityIndex);
        break;
    case 0x3E:
        monsterBattleOpcode_3E(pScriptContext, entityIndex);
        break;
    case 0x40:
        monsterBattleOpcode_40(pScriptContext, entityIndex);
        break;
    case 0x41:
        monsterBattleOpcode_41(pScriptContext, entityIndex);
        break;
    case 0x43:
        monsterBattleOpcode_43(pScriptContext, entityIndex);
        break;
    case 0x4A:
        monsterBattleOpcode_4A(pScriptContext, entityIndex);
        break;
    case 0x50:
        monsterBattleOpcode_50(pScriptContext, entityIndex);
        break;
    case 0x52:
        monsterBattleOpcode_52(pScriptContext, battleScriptContext, entityIndex, numIterations);
        break;
    case 0x64:
        monsterBattleOpcode_64(pScriptContext, entityIndex);
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
        default:
            assert(0);
            break;
        case 0x90:
            operationResult = monsterBattleOpcode_90(pScriptContext, entityIndex);
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
    while (0x6f < (*param_1->m0_scriptPtr - 0x80)) {
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

void executeBattleCode_attack2Sub(byte param_1, uint param_2) {
    short sVar1;
    sBattleVar2* psVar2;

    sVar1 = battleScriptContext[param_2 & 0xff].m4;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    psVar2 = battleVar2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = sVar1;
    psVar2->m2DA_indexInBattleVar48 = psVar2->m2DA_indexInBattleVar48 + 1;
}

void executeBattleCode_attack2(byte param_1, uint param_2, byte param_3)

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
    executeBattleCode_attack2Sub(param_1, param_2 & 0xff);
    return;
}

extern s8 battleTickMain_var0;

void executeBattleCode_F(byte param_1, uint param_2)
{
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -10;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = battleScriptContext[param_2 & 0xff].m6_mask;
    executeBattleCode_attack2Sub(param_1, param_2);
    return;
}

void executeBattleCode_4(byte param_1, uint param_2)
{
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = battleScriptContext[param_2 & 0xff].m4;
    executeBattleCode_attack2Sub(param_1, param_2 & 0xff);
    return;
}

void executeBattleCode_C(uint param_1, byte param_2)

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

void executeBattleCode_1(byte param_1, uint param_2)
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
    executeBattleCode_attack2Sub(param_1, param_2);
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

void executeBattleCode_E(ushort param_1, s8 param_2) {
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
            executeBattleCode_1(param_1, numIteration & 0xff);
            break;
        case 2:
            executeBattleCode_attack2(param_1, numIteration & 0xff, uVar4 & 0xff);
            break;
        case 0x4:
            executeBattleCode_4(param_1, numIteration & 0xff);
            break;
        case 0xC:
            executeBattleCode_C(numIteration & 0xff, param_1);
            break;
        case 0xe:
            executeBattleCode_E(battleScriptContext[opcodeOffset].m4, param_1);
            break;
        case 0xF:
            executeBattleCode_F(param_1, numIteration & 0xff);
            break;
        }

        numIteration++;
        opcodeOffset++;
        if ((0x1f < (int)numIteration) && (!continueScript)) {
            return;
        }

    } while (true);
}