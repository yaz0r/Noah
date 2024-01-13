#pragma once

struct sBattleScriptEntity {
    std::array<std::optional<std::vector<u8>::iterator>, 4> m0_scripts;
    std::array<s32, 4> m10;
    std::array<s16, 8> m20;
    std::array<u8, 16> m30_varArray;
};
extern std::array<sBattleScriptEntity, 8> monstersScriptsEntities;

struct sBattleScriptContext {
    u8 m0_attackType;
    u8 m1;
    s8 m2;
    s16 m4;
    u16 m6_mask;
};
extern std::array<sBattleScriptContext, 32> battleScriptContext;

void executeMonsterScript(char param_1, int param_2);
int executeMonsterScriptWhenAttacked(char param_1);
void executeBattleCode(byte param_1);