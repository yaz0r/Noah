#include "noahLib.h"
#include "validateBattle.h"
#include "validation/validation.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"
#include "validation/field/validateField.h"
#include "validation/field/validateFieldEntities.h"
#include "validation/kernel/randomSeed.h"
#include "battle/battle.h"
#include "battle/battleRenderContext.h"
#include "kernel/inputs.h"
#include "psx/libgte/gte.h"

#include "battle/battleSpriteLoader.h"
#include "battle/battleLoader.h"

extern u16 allPlayerCharacterBitmask;
extern SVECTOR battleCameraEye;
extern SVECTOR battleCameraEyeTarget;
extern SVECTOR battleCameraAt;
extern SVECTOR battleCameraAtTarget;
extern MATRIX battleRenderingMatrix;

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
static const u32 PSX_mainBattleSpriteCallback_phase5Var = 0x800c35d8; // int

// PSX addresses for visual entities and camera
static const u32 PSX_battleSpriteActorCores = 0x800CCB3C; // std::array<sSpriteActorCore*, 11>
static const u32 PSX_battleVisualEntities = 0x800c3eb4;  // std::array<sBattleVisualEntity, 11>, each 0x1c
static const u32 PSX_battleRenderingMatrix = 0x800d30bc; // MATRIX
static const u32 PSX_battleCameraEyeTarget = 0x800d30a0; // SVECTOR
static const u32 PSX_battleCameraAtTarget = 0x800d30a8;  // SVECTOR
static const u32 PSX_battleCameraEye = 0x800d3354;       // SVECTOR
static const u32 PSX_battleCameraAt = 0x800d335c;        // SVECTOR

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

void validateBattleVisualEntities() {
    for (int i = 0; i < 11; i++) {
        u32 base = PSX_battleVisualEntities + 0x1c * i;
        validate(base + 0x0, battleVisualEntities[i].m0_positionSlot);
        validate(base + 0x1, battleVisualEntities[i].m1);
        validate(base + 0x2, battleVisualEntities[i].m2);
        validate(base + 0x3, battleVisualEntities[i].m3);
        validate(base + 0x4, battleVisualEntities[i].m4_isGear);
        validate(base + 0x5, battleVisualEntities[i].m5);
        validate(base + 0x6, battleVisualEntities[i].m6_direction);
        validate(base + 0xA, battleVisualEntities[i].mA_X);
        validate(base + 0xC, battleVisualEntities[i].mC_Z);
    }
}

void validateBattleSpriteActorCore(u32 psxBase, const sSpriteActorCore* pCore) {
    validate(psxBase + 0x00, pCore->m0_position);
    validate(psxBase + 0x0C, pCore->mC_step);
    validate(psxBase + 0x18, pCore->m18_moveSpeed);
    validate(psxBase + 0x1C, pCore->m1C_gravity);
    validate(psxBase + 0x2C, pCore->m2C_scale);
    validate(psxBase + 0x2E, pCore->m2E);
    validate(psxBase + 0x30, pCore->m30);
    validate(psxBase + 0x32, pCore->m32_direction);
    validate(psxBase + 0x34, pCore->m34_currentSpriteFrame);
    validate(psxBase + 0x36, pCore->m36);
    validate(psxBase + 0x38, pCore->m38);
    validate(psxBase + 0x3A, pCore->m3A);
    validate(psxBase + 0x3C, pCore->m3C);
    validate(psxBase + 0x40, pCore->m40);
    validate(psxBase + 0x78, pCore->m78);
    validate(psxBase + 0x80, pCore->m80);
    validate(psxBase + 0x82, pCore->m82);
    validate(psxBase + 0x84, pCore->m84_maxY);
    validate(psxBase + 0x9E, pCore->m9E_wait);
    validate(psxBase + 0xA0, pCore->mA0);
    validate(psxBase + 0xA8, pCore->mA8.mRaw);
    validate(psxBase + 0xAC, pCore->mAC.mRaw);
    validate(psxBase + 0xB0, pCore->mB0.mRaw);
}

void validateBattleSpriteActorCores() {
    for (int i = 0; i < 11; i++) {
        u32 psxPtr = g_gdbConnection->readU32(PSX_battleSpriteActorCores + i * 4);
        if (battleSpriteActorCores[i] && psxPtr) {
            validateBattleSpriteActorCore(psxPtr, battleSpriteActorCores[i]);
        } else {
            validateAssert((battleSpriteActorCores[i] == nullptr) == (psxPtr == 0));
        }
    }
}

void validateBattleCamera() {
    validate(PSX_battleCameraEye, battleCameraEye);
    validate(PSX_battleCameraEyeTarget, battleCameraEyeTarget);
    validate(PSX_battleCameraAt, battleCameraAt);
    validate(PSX_battleCameraAtTarget, battleCameraAtTarget);
    validate(PSX_battleRenderingMatrix, battleRenderingMatrix);
}

void validateBattleState() {
    validateRandomSeed();
    validate(0x800ccc58, battleTimeEnabled);
    validate(PSX_mainBattleSpriteCallback_phase5Var, mainBattleSpriteCallback_phase5Var);
    validate(PSX_currentBattleMode, currentBattleMode);
    validate(PSX_battleRunningVar1, battleRunningVar1);
    validate(PSX_allPlayerCharacterBitmask, allPlayerCharacterBitmask);
    validate(PSX_currentEntryInRandomTurnOrder, currentEntryInRandomTurnOrder);

    for (int i = 0; i < 11; i++) {
        validateAssert(g_gdbConnection->readS8(PSX_isBattleSlotFilled + i) == isBattleSlotFilled[i]);
        validateAssert(g_gdbConnection->readS8(PSX_isEntityReadyForBattle + i) == isEntityReadyForBattle[i]);
        validateAssert(g_gdbConnection->readS8(PSX_randomTurnOrder + i) == randomTurnOrder[i]);
        validate(PSX_battleSlotStatusVar0 + i * 2, battleSlotStatusVar0[i]);
    }

    validateBattleVisualEntities();
    validateBattleSpriteActorCores();
    validateBattleCamera();
    validateBattleEntities();
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

    g_gdbConnection->executeUntilAddress(0x80071b94); // PSX battleTickMain entry
    validateBattleState();

    battleTickMain_intercept.callUndetoured(arg);

    g_gdbConnection->executeUntilAddress(0x80072268);

    validateBattleState();
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

DECLARE_HOOK(battleLoaderTick, void, s8);
void battleLoaderTick_detour(s8 arg) {
    g_gdbConnection->executeUntilAddress(0x801e5840);
    assert(arg == (s8)g_gdbConnection->getRegister(GDBConnection::A0));

    validateBattleState();

    battleLoaderTick_intercept.callUndetoured(arg);
}

DECLARE_HOOK_VOID(battleRender, void);
void battleRender_detour() {
    g_gdbConnection->executeUntilAddress(0x800be790);

    validateBattleState();

    battleRender_intercept.callUndetoured();
}

DECLARE_HOOK_VOID(updateBattleCamera, void);
void updateBattleCamera_detour() {
    g_gdbConnection->executeUntilAddress(0x800bbab8);

    validateBattleState();

    updateBattleCamera_intercept.callUndetoured();
}

DECLARE_HOOK(VSync, int, int);
int VSync_detour(int arg) {
    if (arg == -1) {
        g_gdbConnection->executeUntilAddress(0x8004b54c);

        VSync_intercept.callUndetoured(arg);

        g_gdbConnection->executeUntilAddress(0x8004b68c);

        g_gdbConnection->removeBreakpoint(0x8004b54c); // else we trap in internal calls to vsync
        g_gdbConnection->removeBreakpoint(0x8004b68c); // else we trap in internal calls to vsync

        return g_gdbConnection->getRegister(GDBConnection::V0);
    }
    else {
        return VSync_intercept.callUndetoured(arg);
    }
}

DECLARE_HOOK(mainBattleSpriteCallback_phase1, void, sTaskHeader*);
void mainBattleSpriteCallback_phase1_detour(sTaskHeader* arg) {
    g_gdbConnection->executeUntilAddress(0x801e7004);
    int isCDBusy = g_gdbConnection->getRegister(GDBConnection::V0);
    if (isCDBusy == 0) {
        mainBattleSpriteCallback_phase1_intercept.callUndetoured(arg);
        g_gdbConnection->executeUntilAddress(0x801e7078);
        validateBattleState();
    }
}

DECLARE_HOOK(mainBattleSpriteCallback_phase2, void, sTaskHeader*);
void mainBattleSpriteCallback_phase2_detour(sTaskHeader* arg) {
    g_gdbConnection->executeUntilAddress(0x801e6f18);
    int isCDBusy = g_gdbConnection->getRegister(GDBConnection::V0);
    if (isCDBusy == 0) {
        mainBattleSpriteCallback_phase2_intercept.callUndetoured(arg);
        g_gdbConnection->executeUntilAddress(0x801e6fd0);
        validateBattleState();
    }
}

DECLARE_HOOK(computeBattleCameraParams, void, uint);
void computeBattleCameraParams_detour(uint bitmask) {
    g_gdbConnection->executeUntilAddress(0x800bc460);

    validateBattleState();

    computeBattleCameraParams_intercept.callUndetoured(bitmask);

    g_gdbConnection->executeUntilAddress(0x800bca9c);

    validateBattleState();
}

DECLARE_HOOK_VOID(batteLoaderPhase1_0, void);
void batteLoaderPhase1_0_detour() {
    g_gdbConnection->executeUntilAddress(0x801e4048);
    validateBattleState();
    batteLoaderPhase1_0_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x801e4158);
    validateBattleState();
}

DECLARE_HOOK_VOID(batteLoaderPhase1_1, void);
void batteLoaderPhase1_1_detour() {
    g_gdbConnection->executeUntilAddress(0x801e4160);
    validateBattleState();
    batteLoaderPhase1_1_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x801e4868);
    validateBattleState();
}

DECLARE_HOOK_VOID(loadEnemies, void);
void loadEnemies_detour() {
    g_gdbConnection->executeUntilAddress(0x801e4870);
    validateBattleState();
    loadEnemies_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x801e4ab8);
    validateBattleState();
}

DECLARE_HOOK_VOID(batteLoaderPhase1_3, void);
void batteLoaderPhase1_3_detour() {
    g_gdbConnection->executeUntilAddress(0x801e4ac0);
    validateBattleState();
    batteLoaderPhase1_3_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x801e4cc8);
    validateBattleState();
}

DECLARE_HOOK_VOID(battleRenderDebugAndMain, int);
int battleRenderDebugAndMain_detour() {
    g_gdbConnection->executeUntilAddress(0x800716d8);

    validateBattleState();

    int value = battleRenderDebugAndMain_intercept.callUndetoured();

    g_gdbConnection->executeUntilAddress(0x80071714);

    validateBattleState();

    return value;
}

void validateBattle_init() {
    battleHandleInput_intercept.enable();
    battleDebugSelectorMainLoop_intercept.enable();
    battleTickMain_intercept.enable();
    battleLoaderTick_intercept.enable();
    battleRender_intercept.enable();
    updateBattleCamera_intercept.enable();
    computeBattleCameraParams_intercept.enable();
    VSync_intercept.enable();
    mainBattleSpriteCallback_phase1_intercept.enable();
    mainBattleSpriteCallback_phase2_intercept.enable();
    battleRenderDebugAndMain_intercept.enable();
    batteLoaderPhase1_0_intercept.enable();
    batteLoaderPhase1_1_intercept.enable();
    loadEnemies_intercept.enable();
    batteLoaderPhase1_3_intercept.enable();

    //enableBattleValidationContext(BCT_Base);
    enableBattleValidationContext(BCT_Tick);
    //enableBattleValidationContext(BCT_Damage);
    enableBattleValidationContext(BCT_KernelSpriteVM);

    //checkWinConditions_intercept.enable();
    battleTickGameplay_intercept.enable();
    //applyChangeToHpOrMp_intercept.enable();
}

void validateBattle_shutdown() {
    applyChangeToHpOrMp_intercept.disable();
    battleTickGameplay_intercept.disable();
    checkWinConditions_intercept.disable();
}
