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

void zeroInitStack(size_t size)
{
    std::vector<u8> tempStack;
    tempStack.resize(size, 0);
    g_gdbConnection->writeMemory(g_gdbConnection->getRegister(GDBConnection::REG_Names::SP), tempStack.data(), tempStack.size());
}

#define DECLARE_HOOK(name, returnType, ...) \
returnType name(__VA_ARGS__);\
returnType name##_detour (__VA_ARGS__);\
interceptor<returnType, __VA_ARGS__> name##_intercept(name, name##_detour);

#define DECLARE_HOOK_VOID(name, returnType) \
returnType name();\
returnType name##_detour ();\
interceptor<returnType> name##_intercept(name, name##_detour);

DECLARE_HOOK(EntityMoveCheck0, void, uint, sFieldEntity*, sFieldScriptEntity*);
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

DECLARE_HOOK_VOID(updateScriptAndMoveEntities, void);
void updateScriptAndMoveEntities_detour() {
    // go to start of function and validate input state
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8008110c);
        validateField();
    }
    updateScriptAndMoveEntities_intercept.callUndetoured();

    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        // go to end of function and validate state
        g_gdbConnection->executeUntilAddress(0x800815e8);
        validateField();
    }
}

DECLARE_HOOK_VOID(initFieldData, void);
void initFieldData_detour() {

    initFieldData_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x80071a5c);
    validateField();
}

DECLARE_HOOK_VOID(startAllEntityScripts, void);
void startAllEntityScripts_detour() {
    // go to start of function and validate input state
    g_gdbConnection->executeUntilAddress(0x800a28d4);
    validateField();

    startAllEntityScripts_intercept.callUndetoured();

    // go to end of function and validate state
    g_gdbConnection->executeUntilAddress(0x800a2fb8);
    validateField();
}

DECLARE_HOOK_VOID(isLoadCompleted, int);
int isLoadCompleted_detour() {
    g_gdbConnection->executeUntilAddress(0x8008a598);
    isLoadCompleted_intercept.callUndetoured();
    return g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
}

DECLARE_HOOK(updateEntityEventCode3, void, int, sFieldEntity*, sFieldScriptEntity*);
void updateEntityEventCode3_detour(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity) {
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x80082bbc);
        zeroInitStack(0x58);
        validateField();
        assert(index == g_gdbConnection->getRegister(GDBConnection::REG_Names::A0));
    }
    updateEntityEventCode3_intercept.callUndetoured(index, pFieldEntity, pFieldScriptEntity);

    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x80083170);
        validateField();
    }
}

// Need to sync bootField because of transition effect
DECLARE_HOOK_VOID(bootField, void);
void bootField_detour() {
    bootField_intercept.callUndetoured();
    g_gdbConnection->executeUntilAddress(0x80079280);
    validateField();
}

// Need to 0 initialize memory
DECLARE_HOOK(createSpriteActor, sSpriteActor*, sSpriteActorAnimationBundle*, int, int, int, int, int);
sSpriteActor* createSpriteActor_detour(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6) {
    g_gdbConnection->executeUntilAddress(0x80024568);
    u32 allocationBase = g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
    for (int i = 0; i < 0x164; i += 4) {
        g_gdbConnection->writeU32(allocationBase + i, 0);
    }
    return createSpriteActor_intercept.callUndetoured(pSetup, param_2, param_3, vramX, vramY, param_6);
}

DECLARE_HOOK(updateEntityEventCode3Sub3, int, FP_VEC3*, sFieldScriptEntity*, std::array<SVECTOR, 2>&, short);
int updateEntityEventCode3Sub3_detour(FP_VEC3* param_1, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, short angle) {
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007bac4);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A0), *param_1);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A1), *param_2);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::A3) == angle);
        zeroInitStack(0xA8);
    }
    int result = updateEntityEventCode3Sub3_intercept.callUndetoured(param_1, param_2, param_3, angle);
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007bec0);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::V0) == result);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::S1), *param_1);
    }

    return result;
}

DECLARE_HOOK(updateEntityEventCode3Sub4, int, FP_VEC3*, sFieldScriptEntity*, std::array<SVECTOR, 2>&, short);
int updateEntityEventCode3Sub4_detour(FP_VEC3* param_1, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, short angle) {
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007b814);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A0), *param_1);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A1), *param_2);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::A3) == angle);
    }
    int result = updateEntityEventCode3Sub4_intercept.callUndetoured(param_1, param_2, param_3, angle);
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007ba90);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::V0) == result);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::S1), *param_1);
    }

    return result;
}

DECLARE_HOOK(updateEntityEventCode3Sub4Sub1, int, FP_VEC3*, VECTOR*, sFieldScriptEntity*, std::array<SVECTOR, 2>&, SVECTOR*, int);
int updateEntityEventCode3Sub4Sub1_detour(FP_VEC3* deltaStep, VECTOR* position, sFieldScriptEntity* pFieldScriptEntity, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6) {
    int result = updateEntityEventCode3Sub4Sub1_intercept.callUndetoured(deltaStep, position, pFieldScriptEntity, param_4, param_5, param_6);
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007cd08);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::V0) == result);
    }
    return result;
}

DECLARE_HOOK(updateEntityEventCode3Sub3Sub1, int, FP_VEC3*, VECTOR*, sFieldScriptEntity*, std::array<SVECTOR, 2>&, SVECTOR*, int, uint*);
int updateEntityEventCode3Sub3Sub1_detour(FP_VEC3* deltaStep, VECTOR* position, sFieldScriptEntity* pFieldScriptEntity, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6, uint* param_7) {
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007bef8);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A0), *deltaStep);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A1), *position);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A2), *pFieldScriptEntity);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::A3), param_4);
        validate(g_gdbConnection->readU32(g_gdbConnection->getRegister(GDBConnection::REG_Names::SP) + 0xA0), *param_5);
    }
    int result = updateEntityEventCode3Sub3Sub1_intercept.callUndetoured(deltaStep, position, pFieldScriptEntity, param_4, param_5, param_6, param_7);
    if (isFieldValidationContextEnabled(FCT_MoveCheck)) {
        g_gdbConnection->executeUntilAddress(0x8007c63c);
        assert(g_gdbConnection->getRegister(GDBConnection::REG_Names::V0) == result);
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::S7), param_4);
        validate(g_gdbConnection->readU32(g_gdbConnection->getRegister(GDBConnection::REG_Names::SP) + 0xA0), *param_5);
    }
    return result;
}


void validateField_init() {
    enableFieldValidationContext(FCT_Base);
    enableFieldValidationContext(FCT_Init);
    //enableFieldValidationContext(FCT_Script);
    //enableFieldValidationContext(FCT_MoveCheck);
    //enableFieldValidationContext(FCT_Rendering);

    bootField_intercept.enable();
    createSpriteActor_intercept.enable();
    isLoadCompleted_intercept.enable();
    initFieldData_intercept.enable();
    startAllEntityScripts_intercept.enable();

    updateScriptAndMoveEntities_intercept.enable();
    EntityMoveCheck0_intercept.enable();
    updateEntityEventCode3_intercept.enable();
    updateEntityEventCode3Sub3_intercept.enable();
    updateEntityEventCode3Sub4_intercept.enable();
    updateEntityEventCode3Sub4Sub1_intercept.enable();
    updateEntityEventCode3Sub3Sub1_intercept.enable();
}

void validateField_shutdown() {
    updateEntityEventCode3Sub3Sub1_intercept.disable();
    updateEntityEventCode3Sub4Sub1_intercept.disable();
    updateEntityEventCode3Sub4_intercept.disable();
    updateEntityEventCode3Sub3_intercept.disable();
    updateEntityEventCode3_intercept.disable();
    EntityMoveCheck0_intercept.disable();
    updateScriptAndMoveEntities_intercept.disable();

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
    validateWalkMeshBundle();
    assert(fieldTransitionInProgress == g_gdbConnection->readU32(0x800afd04));
}