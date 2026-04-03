#include "noahLib.h"
#include "validateBattle.h"
#include "validation/validation.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"
#include "validation/field/validateField.h"
#include "validation/field/validateFieldEntities.h"
#include "validation/kernel/randomSeed.h"
#include "battle/battle.h"
#include "kernel/inputs.h"

extern u16 allPlayerCharacterBitmask;

// PSX addresses for battle globals
static const u32 PSX_battleEntities = 0x800CCCE8;       // std::array<sBattleEntity, 11>, each 0x170
static const u32 PSX_battleVar2_ptr = 0x800c3eac;       // pointer to sBattleVar2
static const u32 PSX_isEntityReadyForBattle = 0x800d2de4;// std::array<s8, 11>
static const u32 PSX_randomTurnOrder = 0x800d2dd8;       // std::array<s8, 11>
static const u32 PSX_currentEntryInRandomTurnOrder = 0x800d2dd7; // s8
static const u32 PSX_isBattleSlotFilled = 0x800d2dcc;   // std::array<s8, 11>
static const u32 PSX_allPlayerCharacterBitmask = 0x800d39dc; // u16
static const u32 PSX_battleRunningVar1 = 0x800c48ea;    // s8
static const u32 PSX_currentBattleMode = 0x800c3e4c;    // s8
static const u32 PSX_battleSlotStatusVar0 = 0x800d2df0;  // std::array<s16, 11>

// PSX function addresses
static const u32 PSX_checkWinConditions_start = 0x8007252c;
static const u32 PSX_checkWinConditions_end = 0x8007288c;
static const u32 PSX_battleTickGameplay_start = 0x800723e0;
static const u32 PSX_battleTickGameplay_end = 0x8007251c;
static const u32 PSX_applyChangeToHpOrMp_start = 0x80085618;
static const u32 PSX_applyChangeToHpOrMp_end = 0x80085a90;

static u64 battleValidationContextStatus = 0;

bool isBattleValidationContextEnabled(battleValidationContexts context) {
    return battleValidationContextStatus & (1 << (int)context);
}

void enableBattleValidationContext(battleValidationContexts context) {
    battleValidationContextStatus |= (1 << (int)context);
}

void disableBattleValidationContext(battleValidationContexts context) {
    battleValidationContextStatus &= ~(1 << (int)context);
}

#define DECLARE_HOOK(name, returnType, ...) \
returnType name(__VA_ARGS__);\
returnType name##_detour (__VA_ARGS__);\
interceptor<returnType, __VA_ARGS__> name##_intercept(name, name##_detour);

#define DECLARE_HOOK_VOID(name, returnType) \
returnType name();\
returnType name##_detour ();\
interceptor<returnType> name##_intercept(name, name##_detour);

// Validate a single sBattleEntity against PSX memory
static void validateBattleEntity(u32 psxBase, const sBattleEntity& entity) {
    // sGameStateA4 m0_base (key combat fields)
    validate(psxBase + 0x4C, entity.m0_base.m4C_HP);
    validate(psxBase + 0x4E, entity.m0_base.m4E_MaxHP);
    validate(psxBase + 0x50, entity.m0_base.m50_MP);
    validate(psxBase + 0x52, entity.m0_base.m52_MaxMP);
    validate(psxBase + 0x7C, entity.m0_base.m7C);
    validate(psxBase + 0x7E, entity.m0_base.m7E);
    validate(psxBase + 0x80, entity.m0_base.m80);
    validate(psxBase + 0x82, entity.m0_base.m82);
    validate(psxBase + 0x84, entity.m0_base.m84);
    validate(psxBase + 0x86, entity.m0_base.m86);
    validate(psxBase + 0x88, entity.m0_base.m88);

    // sGameStateA42 mA4_gear (key gear fields)
    validate(psxBase + 0xA4 + 0x60, entity.mA4_gear.m60_hp);
    validate(psxBase + 0xA4 + 0x7C, entity.mA4_gear.m7C);
}

void validateBattleEntities() {
    for (int i = 0; i < 11; i++) {
        if (isBattleSlotFilled[i]) {
            u32 entityBase = PSX_battleEntities + 0x170 * i;
            validateBattleEntity(entityBase, battleEntities[i]);
        }
    }
}

void validateBattleState() {
    validateRandomSeed();

    validateAssert(g_gdbConnection->readS8(PSX_currentBattleMode) == currentBattleMode);
    validateAssert(g_gdbConnection->readS8(PSX_battleRunningVar1) == battleRunningVar1);
    validate(PSX_allPlayerCharacterBitmask, allPlayerCharacterBitmask);
    validateAssert(g_gdbConnection->readS8(PSX_currentEntryInRandomTurnOrder) == currentEntryInRandomTurnOrder);

    for (int i = 0; i < 11; i++) {
        validateAssert(g_gdbConnection->readS8(PSX_isBattleSlotFilled + i) == isBattleSlotFilled[i]);
        validateAssert(g_gdbConnection->readS8(PSX_isEntityReadyForBattle + i) == isEntityReadyForBattle[i]);
        validateAssert(g_gdbConnection->readS8(PSX_randomTurnOrder + i) == randomTurnOrder[i]);
        validate(PSX_battleSlotStatusVar0 + i * 2, battleSlotStatusVar0[i]);
    }
}

// Hook: checkWinConditions
// Validates battle entity state after win/loss condition evaluation
DECLARE_HOOK_VOID(checkWinConditions, void);
void checkWinConditions_detour() {
    if (isBattleValidationContextEnabled(BCT_Base)) {
        g_gdbConnection->executeUntilAddress(PSX_checkWinConditions_start);
        validateBattleEntities();
        validateBattleState();
    }

    checkWinConditions_intercept.callUndetoured();

    if (isBattleValidationContextEnabled(BCT_Base)) {
        g_gdbConnection->executeUntilAddress(PSX_checkWinConditions_end);
        validateBattleEntities();
        validateBattleState();
    }
}

// Hook: battleTickGameplay
// Validates turn order state at the start of each gameplay tick
DECLARE_HOOK_VOID(battleTickGameplay, void);
void battleTickGameplay_detour() {
    if (isBattleValidationContextEnabled(BCT_Tick)) {
        g_gdbConnection->executeUntilAddress(PSX_battleTickGameplay_start);
        validateBattleState();
    }

    battleTickGameplay_intercept.callUndetoured();

    if (isBattleValidationContextEnabled(BCT_Tick)) {
        g_gdbConnection->executeUntilAddress(PSX_battleTickGameplay_end);
        validateBattleState();
    }
}

// Hook: applyChangeToHpOrMp
// Validates HP/MP changes are applied identically
DECLARE_HOOK(applyChangeToHpOrMp, void, uint);
void applyChangeToHpOrMp_detour(uint damageIndex) {
    if (isBattleValidationContextEnabled(BCT_Damage)) {
        g_gdbConnection->executeUntilAddress(PSX_applyChangeToHpOrMp_start);
        validateBattleEntities();
    }

    applyChangeToHpOrMp_intercept.callUndetoured(damageIndex);

    if (isBattleValidationContextEnabled(BCT_Damage)) {
        g_gdbConnection->executeUntilAddress(PSX_applyChangeToHpOrMp_end);
        validateBattleEntities();
    }
}

DECLARE_HOOK_VOID(battleHandleInput, void);
void battleHandleInput_detour() {

    g_gdbConnection->executeUntilAddress(0x8008a13c);
    
    // stomp the input with the ones from the PSX
    newPadButtonForField = g_gdbConnection->readU16(0x800594a4);
    newPadButtonForDialogs = g_gdbConnection->readU16(0x8005948c);

    u8 tempBattleInputButton = g_gdbConnection->readU8(0x800d3014);

    battleHandleInput_intercept.callUndetoured();

    battleInputButton = tempBattleInputButton;

}

DECLARE_HOOK(battleTickMain, void, s8);
void battleTickMain_detour(s8 arg) {
    battleTickMain_intercept.callUndetoured(arg);

    g_gdbConnection->executeUntilAddress(0x80072268);
}

DECLARE_HOOK_VOID(battleDebugSelectorMainLoop, void);
void battleDebugSelectorMainLoop_detour() {
    g_gdbConnection->executeUntilAddress(0x801e0238);

    g_gdbConnection->writeU32(0x801e09a8, 0); // hack to not crash on retail us

    /* Sync the debug battle selector */
    for (int i = 0; i < 3; i++) {
        g_gdbConnection->writeU32(0x8006d634 + 0x1D34 + 4 * i, gameState.m1D34_currentParty[i]);
    }

    extern std::array<std::array<int, 3>, 4> battleDebugSelectorTableValues;
    extern std::array<std::array<u8, 3>, 4> battleDebugSelectorIsCellEnabled;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            g_gdbConnection->writeU32(0x801e1da0 + (i * 3 + j) * 4, battleDebugSelectorTableValues[i][j]);
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            g_gdbConnection->writeU8(0x801e1dd0 + (i * 3 + j) * 1, battleDebugSelectorIsCellEnabled[i][j]);
        }
    }

    battleDebugSelectorMainLoop_intercept.callUndetoured();
}

void validateBattle_init() {
    battleHandleInput_intercept.enable();
    battleDebugSelectorMainLoop_intercept.enable();
    battleTickMain_intercept.enable();

    //enableBattleValidationContext(BCT_Base);
    //enableBattleValidationContext(BCT_Tick);
    //enableBattleValidationContext(BCT_Damage);

    //checkWinConditions_intercept.enable();
    //battleTickGameplay_intercept.enable();
    //applyChangeToHpOrMp_intercept.enable();
}

void validateBattle_shutdown() {
    applyChangeToHpOrMp_intercept.disable();
    battleTickGameplay_intercept.disable();
    checkWinConditions_intercept.disable();
}
