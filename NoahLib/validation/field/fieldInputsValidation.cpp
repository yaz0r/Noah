#include "noahLib.h"
#include "fieldInputsValidation.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"

#include "kernel/inputs.h"
#include "field/fieldInputs.h"
#include "field/dialogWindows.h"

#include "validation/field/validateFieldEntities.h"
#include "validation/kernel/randomSeed.h"
#include "validation/field/validateField.h"

#include "kernel/playTime.h"

void fieldPerFrameReset();
void fieldPerFrameReset_detour();

interceptor<void> fieldPerFrameReset_intercept(fieldPerFrameReset, fieldPerFrameReset_detour);

void fieldPerFrameReset_detour() {
    g_gdbConnection->executeUntilAddress(0x80077dac);

    // Sync the variable that contains the play time, as those are based on vsync
    g_playTimeSeconds = g_gdbConnection->readU8(0x80059418);
    g_playTimeMinutes = g_gdbConnection->readU8(0x80059420);
    g_playTimeHours = g_gdbConnection->readU8(0x80059484);

    fieldPerFrameReset_intercept.callUndetoured();

    g_gdbConnection->executeUntilAddress(0x80077e00);

    // stomp the input with the ones from the PSX
    padButtonForScripts[0].m0_buttons = g_gdbConnection->readU16(0x800afe9c);
    padButtonForScripts[1].m0_buttons = g_gdbConnection->readU16(0x800afea0);
    padButtonForDialogs = g_gdbConnection->readU16(0x800c2694);
    g_padButtonForField = g_gdbConnection->readU16(0x800c3900);
    validateField();
}

void fieldInputsValidation_init() {
    fieldPerFrameReset_intercept.enable();
}
void fieldInputsValidation_shutdown() {
    fieldPerFrameReset_intercept.disable();
}