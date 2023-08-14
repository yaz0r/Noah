#include "noahLib.h"
#include "battle.h"

#include "enemyScript.h"

std::array<sBattleScriptEntity, 8> monstersScriptsEntities;

struct sBattleScriptContext {
    u8 m0_attackType;
    u8 m1;
    s8 m2;
    s16 m4;
    u16 m6_mask;
};

std::array<sBattleScriptContext, 32> battleScriptContext;

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);

int executeMonsterScriptWhenAttacked(char param_1) {
    int result = 0;
    int result2 = 0;
    if ((battleEntities[param_1].m0_base.m7C & 0x8000) == 0) {
        result = 0;
        if (battleEntities[param_1].m0_base.m34 & 0x800) {
            battleScriptContext[0].m0_attackType = 0;
            result = result2;
            if (unknownMonsterStatus0[param_1 - 3].m0 != 0) {
                assert(0);
            }
            if (battleScriptContext[0].m0_attackType) {
                assert(0);
            }
        }
    }

    return result;
}

struct sBattleScriptExecutionContext {
    std::vector<u8>::iterator m0_scriptPtr;
};

void advanceBattleScript(sBattleScriptExecutionContext* pScriptContext) {
    pScriptContext->m0_scriptPtr += 4;
}

void monsterBattleOpcode_addByte(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_varArray.begin() + param_1->m0_scriptPtr[1];
    *pByte = std::min<u8>(*pByte + param_1->m0_scriptPtr[2], 0xFF);
}

void monsterBattleOpcode_modulo(sBattleScriptExecutionContext* param_1, uint param_2) {
    auto pByte = monstersScriptsEntities[param_2].m30_varArray.begin() + param_1->m0_scriptPtr[1];
    *pByte %= param_1->m0_scriptPtr[2];
}

bool isMonsterTargetValid(uint param_1, char param_2) {
    bool bVar1;

    param_1 = param_1 & 0xff;
    bVar1 = false;
    if ((((isBattleSlotFilled[param_1] != 0) && (battleVisualEntities[param_1].m3 == 0)) &&
        ((battleEntities[param_1].m0_base.m7C & 0xc002) == 0)) && (bVar1 = true, param_2 == '\0')) {
        bVar1 = (battleEntities[param_1].m0_base.m84 & 0x20) == 0;
    }
    return bVar1;
}

u32 getRandomValueInRange(u32 param_1, u32 param_2);

void monsterBattleOpcode_40(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<u8, 3> local_28 = { 0,0,0 };
    monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 uVar3 = getRandomValueInRange(0, 2);
        u32 uVar5 = uVar3 & 0xff;
        if (local_28[uVar5] == 0) {
            bool cVar1 = isMonsterTargetValid(uVar5, param_1->m0_scriptPtr[2]);
            if ((cVar1 != '\0') && (apConfigArray[uVar5].m1 == 0)) {
                monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(uVar5);
                return;
            }
            local_28[uVar3 & 0xff] = 1;
        }
    } while ((local_28[2] & local_28[0] & local_28[1]) == 0);
}

void monsterBattleOpcode_41(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<u8, 3> local_28 = { 0,0,0 };
    monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 uVar3 = getRandomValueInRange(0, 2);
        u32 uVar5 = uVar3 & 0xff;
        if (local_28[uVar5] == 0) {
            bool cVar1 = isMonsterTargetValid(uVar5, param_1->m0_scriptPtr[2]);
            if (((cVar1 != '\0') &&
                (battleVisualEntities[(param_2 & 0xff) + 3].m0_positionSlot ==
                    battleVisualEntities[uVar5].m0_positionSlot)) && (apConfigArray[uVar5].m1 == 0)) {
                monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(uVar5);
                return;
            }
            local_28[uVar3 & 0xff] = 1;
        }
    } while ((local_28[2] & local_28[0] & local_28[1]) == 0);
}

void monsterBattleOpcode_43(sBattleScriptExecutionContext* param_1, uint param_2) {
    std::array<u8, 3> local_28 = { 0,0,0 };
    monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = 0;
    do {
        u32 uVar3 = getRandomValueInRange(0, 2);
        u32 uVar5 = uVar3 & 0xff;
        if (local_28[uVar5] == 0) {
            bool cVar1 = isMonsterTargetValid(uVar5, param_1->m0_scriptPtr[2]);
            if (((cVar1 != '\0') &&
                (battleVisualEntities[(param_2 & 0xff) + 3].m0_positionSlot !=
                    battleVisualEntities[uVar5].m0_positionSlot)) && (apConfigArray[uVar5].m1 == 0)) {
                monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(uVar5);
                return;
            }
            local_28[uVar3 & 0xff] = 1;
        }
    } while ((local_28[2] & local_28[0] & local_28[1]) == 0);
}

int monsterBattleOpcode_50Sub(int param_1)
{
    short sVar1;
    uint uVar2;

    uVar2 = 0;
    do {
        sVar1 = bitmaskCharacterCheck(param_1, uVar2 & 0xff);
        if (sVar1 != 0) break;
        uVar2 = uVar2 + 1;
    } while ((int)uVar2 < 0xb);
    return uVar2 & 0xff;
}

void monsterBattleOpcode_50(sBattleScriptExecutionContext* param_1, uint param_2) {
    s8 result = 0;
    for (int i = 3; i < 11; i++) {
        if (isMonsterTargetValid(i, 0)) {
            s32 uVar2 = monsterBattleOpcode_50Sub(monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]]);
            if (battleVisualEntities[i].m0_positionSlot == battleVisualEntities[uVar2 & 0xff].m0_positionSlot) {
                result++;
            }
        }
    }
    monstersScriptsEntities[param_2].m30_varArray[param_1->m0_scriptPtr[1]] = result;
}

void monsterBattleOpcode_3E(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m30_varArray[param_1->m0_scriptPtr[1]] = getRandomValueInRange(0, param_1->m0_scriptPtr[2]);
}

void monsterBattleOpcode_52(sBattleScriptExecutionContext* param_1, std::array<sBattleScriptContext, 32>& param_2, uint param_3, uint param_4) {
    s16 value = monstersScriptsEntities[param_3].m20[param_1->m0_scriptPtr[2]];
    
    switch (param_1->m0_scriptPtr[1]) {
    case 6:
        param_2[param_4 & 0xff].m6_mask = value;
        break;
    default:
        assert(0);
    }
}

void monsterBattleOpcode_64(sBattleScriptExecutionContext* param_1, uint param_2) {
    monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] = characterIdToTargetBitmask(param_2 + 3);
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
    case 8:
        monsterBattleOpcode_addByte(pScriptContext, entityIndex);
        break;
    case 0xC:
        monsterBattleOpcode_modulo(pScriptContext, entityIndex);
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
    return monstersScriptsEntities[param_2].m30_varArray[param_1->m0_scriptPtr[1]] == param_1->m0_scriptPtr[2];
}

bool executeMonsterScriptUpper_82(sBattleScriptExecutionContext* param_1, uint param_2) {
    return monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] == READ_LE_S16(param_1->m0_scriptPtr + 2);
}

bool monsterBattleOpcode_90(sBattleScriptExecutionContext* param_1, uint param_2) {
    return monstersScriptsEntities[param_2].m20[param_1->m0_scriptPtr[1]] != READ_LE_S16(param_1->m0_scriptPtr + 2);
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

void performAttackSub3(s8 param_1, s16 param_2, s16 param_3);
void performAttackSub6(s8 param_1);
extern s16 entitiesHitInCurrentAttackBF;;

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
        case 0xC:
            executeBattleCode_C(numIteration & 0xff, param_1);
            break;
        }

        numIteration++;
        opcodeOffset++;
        if ((0x1f < (int)numIteration) && (!continueScript)) {
            return;
        }

    } while (true);
}