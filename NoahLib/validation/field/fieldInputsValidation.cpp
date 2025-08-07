#include "noahLib.h"
#include "fieldInputsValidation.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"

#include "field/field.h"
#include "field/dialogWindows.h"

void updateFieldInputs();
void updateFieldInputs_detour();

interceptor<void> updateFieldInputs_intercept(updateFieldInputs, updateFieldInputs_detour);


void updateFieldInputs_detour() {
    updateFieldInputs_intercept.callUndetoured();

    g_gdbConnection->executeUntilAddress(0x800748e4);
    // stomp the input with the ones from the PSX

    padButtonForScripts[0].m0_buttons = g_gdbConnection->readU16(0x800afe9c);
    padButtonForScripts[1].m0_buttons = g_gdbConnection->readU16(0x800afea0);
    padButtonForDialogs = g_gdbConnection->readU16(0x800c2694);
}

void fieldInputsValidation_init() {
    updateFieldInputs_intercept.enable();
}
void fieldInputsValidation_shutdown() {
    updateFieldInputs_intercept.disable();
}