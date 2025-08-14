#include "noahLib.h"
#include "validateField.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"
#include "validation/field/validateFieldEntities.h"
#include "validation/kernel/randomSeed.h"
#include "field/field.h"
#include "field/fieldScriptSupport.h"
#include "kernel/playTime.h"

static u64 fieldValidationContextStatus = 0;
bool isFieldValidationContextEnabled(fieldValidationContexts context) {
    return fieldValidationContextStatus & (1 << (int)context);
}

void enableFieldValidationContext(fieldValidationContexts context) {
    fieldValidationContextStatus |= (1 << (int)context);
}
void disableFieldValidationContext(fieldValidationContexts context) {
    fieldValidationContextStatus &= ~(1 << (int)context);
}

void EntityMoveCheck0(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity);
void EntityMoveCheck0_detour(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity);
interceptor<void, uint, sFieldEntity*, sFieldScriptEntity*> EntityMoveCheck0_intercept(EntityMoveCheck0, EntityMoveCheck0_detour);

void EntityMoveCheck0_detour(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity) {
    // go to start of function and validate input state
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x80084158);
        validateField();
    }

    EntityMoveCheck0_intercept.callUndetoured(playerEntityIndex, pPlayerEntity, pPlayerScriptEntity);

    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        // go to end of function and validate state
        g_gdbConnection->executeUntilAddress(0x80084924);
        validateField();
    }
}

void updateScriptAndMoveEntities();
void updateScriptAndMoveEntities_detour();
interceptor<void> updateScriptAndMoveEntities_intercept(updateScriptAndMoveEntities, updateScriptAndMoveEntities_detour);

void updateScriptAndMoveEntities_detour() {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x8008110c);
    validateField();

    updateScriptAndMoveEntities_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x800815e8);
    validateField();
}

void initFieldData();
void initFieldData_detour();
interceptor<void> initFieldData_intercept(initFieldData, initFieldData_detour);

void initFieldData_detour() {

    initFieldData_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x80071a5c);
    validateField();
}

void startAllEntityScripts();
void startAllEntityScripts_detour();
interceptor<void> startAllEntityScripts_intercept(startAllEntityScripts, startAllEntityScripts_detour);

void startAllEntityScripts_detour() {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x800a28d4);
    validateField();

    startAllEntityScripts_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x800a2fb8);
    validateField();
}

int isLoadCompleted();
int isLoadCompleted_detour();
interceptor<int> isLoadCompleted_intercept(isLoadCompleted, isLoadCompleted_detour);

int isLoadCompleted_detour() {
    g_gdbConnection->executeUntilAddress(0x8008a598);
    isLoadCompleted_intercept.callUndetoured();
    return g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
}

// Need to sync bootField because of transition effect
void bootField();
void bootField_detour();
interceptor<void> bootField_intercept(bootField, bootField_detour);

void bootField_detour() {
    bootField_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x80079280);
    validateField();
}

// Need to 0 initialize memory
sSpriteActor* createSpriteActor(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
sSpriteActor* createSpriteActor_detour(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
interceptor<sSpriteActor*, sSpriteActorAnimationBundle*, int, int, int, int, int> createSpriteActor_intercept(createSpriteActor, createSpriteActor_detour);

sSpriteActor* createSpriteActor_detour(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6) {
    g_gdbConnection->executeUntilAddress(0x80024568);
    u32 allocationBase = g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
    for (int i = 0; i < 0x164; i += 4) {
        g_gdbConnection->writeU32(allocationBase + i, 0);
    }
    return createSpriteActor_intercept.callUndetoured(pSetup, param_2, param_3, vramX, vramY, param_6);
}


bool bDebugEntityMoves = true;

void validateField_init() {
    enableFieldValidationContext(FCT_Base);
    enableFieldValidationContext(FCT_MoveCheck);

    bootField_intercept.enable();
    createSpriteActor_intercept.enable();
    isLoadCompleted_intercept.enable();
    initFieldData_intercept.enable();
    startAllEntityScripts_intercept.enable();
    if (bDebugEntityMoves) {
        updateScriptAndMoveEntities_intercept.enable();
        EntityMoveCheck0_intercept.enable();
    }
}

void validateField_shutdown() {
    if (bDebugEntityMoves) {
        EntityMoveCheck0_intercept.disable(); 
        updateScriptAndMoveEntities_intercept.disable();
    }
    startAllEntityScripts_intercept.disable();
    initFieldData_intercept.disable();
    isLoadCompleted_intercept.disable();
    createSpriteActor_intercept.disable();
    bootField_intercept.disable();
}

void validateField() {
    validateRandomSeed();
    validateFieldVars();
    validateFieldEntities();
    assert(fieldTransitionInProgress == g_gdbConnection->readU32(0x800afd04));
}