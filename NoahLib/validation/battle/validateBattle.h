#pragma once

#include "validation/gdbConnection.h"

enum battleValidationContexts {
    BCT_Base = 0,
    BCT_Tick,
    BCT_Damage,
    BCT_KernelSpriteVM,

    BCT_Undefined,
};

bool isBattleValidationContextEnabled(battleValidationContexts context);
void enableBattleValidationContext(battleValidationContexts context);
void disableBattleValidationContext(battleValidationContexts context);

void validateBattle_init();
void validateBattle_shutdown();

void validateBattleEntities();
void validateBattleSpriteActorCores();
void validateBattleState();

#define VALIDATE_BATTLE(context, x) \
    if (g_gdbConnection && isBattleValidationContextEnabled(context)) \
    { \
        g_gdbConnection->executeUntilAddress(x); \
        validateBattleState(); \
    }

#define VALIDATE_BATTLE_REG(context, reg, value) \
    if (g_gdbConnection && isBattleValidationContextEnabled(context)) \
        assert(g_gdbConnection->getRegister(GDBConnection::reg) == value);
