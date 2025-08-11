#include "noahLib.h"
#include "validateField.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"
#include "validation/field/validateFieldEntities.h"
#include "validation/kernel/randomSeed.h"
#include "field/field.h"
#include "field/fieldScriptSupport.h"
#include "kernel/playTime.h"

void EntityMoveCheck0(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity);
void EntityMoveCheck0_detour(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity);
interceptor<void, uint, sFieldEntity*, sFieldScriptEntity*> EntityMoveCheck0_intercept(EntityMoveCheck0, EntityMoveCheck0_detour);

void EntityMoveCheck0_detour(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity) {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x80084158);
    validateFieldEntities();

    EntityMoveCheck0_intercept.callUndetoured(playerEntityIndex, pPlayerEntity, pPlayerScriptEntity);

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x80084924);
    validateFieldEntities();
}

void updateScriptAndMoveEntities();
void updateScriptAndMoveEntities_detour();
interceptor<void> updateScriptAndMoveEntities_intercept(updateScriptAndMoveEntities, updateScriptAndMoveEntities_detour);

void updateScriptAndMoveEntities_detour() {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x8008110c);
    validateFieldEntities();

    updateScriptAndMoveEntities_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x800815e8);
    validateFieldEntities();
}

void initFieldData();
void initFieldData_detour();
interceptor<void> initFieldData_intercept(initFieldData, initFieldData_detour);

void initFieldData_detour() {

    initFieldData_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x80071a5c);
    validateFieldEntities();
}

void startAllEntityScripts();
void startAllEntityScripts_detour();
interceptor<void> startAllEntityScripts_intercept(startAllEntityScripts, startAllEntityScripts_detour);

void startAllEntityScripts_detour() {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x800a28d4);
    validateFieldEntities();

    startAllEntityScripts_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x800a2fb8);
    validateFieldEntities();
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

bool bDebugEntityMoves = true;

void validateField_init() {
    bootField_intercept.enable();
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
    bootField_intercept.disable();
}

void validateField() {
    validateRandomSeed();
    validateFieldVars();
    validateFieldEntities();
    assert(fieldTransitionInProgress == g_gdbConnection->readU32(0x800afd04));
}