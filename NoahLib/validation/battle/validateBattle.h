#pragma once

enum battleValidationContexts {
    BCT_Base = 0,
    BCT_Tick,
    BCT_Damage,

    BCT_Undefined,
};

bool isBattleValidationContextEnabled(battleValidationContexts context);
void enableBattleValidationContext(battleValidationContexts context);
void disableBattleValidationContext(battleValidationContexts context);

void validateBattle_init();
void validateBattle_shutdown();

void validateBattleEntities();
void validateBattleState();
