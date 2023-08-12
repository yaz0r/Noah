#include "noahLib.h"
#include "battle.h"

#include "enemyScript.h"

std::array<sBattleScriptEntity, 8> monstersScriptsEntities;

struct sBattleScriptContext {
    u8 m0_attackType;
    u16 m6_mask;
};

std::array<sBattleScriptContext, 32> battleScriptContext;

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

int executeMonsterScriptLower(sBattleScriptExecutionContext* pScriptContext, int entityIndex, int numIterations) {
    u8 bytecode = *pScriptContext->m0_scriptPtr;
    switch (bytecode) {
    case 8:
        monsterBattleOpcode_addByte(pScriptContext, entityIndex);
        break;
    default:
        assert(0);
    }

    advanceBattleScript(pScriptContext);
    return numIterations;
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
            assert(0);
        }
    }
}

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);

void executeBattleCode(byte param_1) {
    int numIteration = 0;
    bool continueScript = true;
    int opcodeOffset = 0;

    do {
        for (int i = 0; i < 11; i++) {
            if (bitmaskCharacterCheck(battleScriptContext[opcodeOffset].m6_mask, i)) {
                break;
            }
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
        }

        numIteration++;
        opcodeOffset++;
        if ((0x1f < (int)numIteration) && (!continueScript)) {
            return;
        }

    } while (true);
}