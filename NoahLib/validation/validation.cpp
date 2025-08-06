#include "noahLib.h"
#include "validation.h"
#include "gdbConnection.h"


GDBConnection* g_gdbConnection = nullptr;

#include "kernel/filesystem.h"
#include "field/field.h"

namespace windows {
#include <windows.h>
}
using namespace windows;

#include "interception.h"

int getCurrentDirectory_detour(int* param_1, int* param_2);

void updateFieldInputs();
void updateFieldInputs_detour();

interceptor<int, int*, int*> getCurrentDirectory_intercept(getCurrentDirectory, getCurrentDirectory_detour);
interceptor<void> updateFieldInputs_intercept(updateFieldInputs, updateFieldInputs_detour);

int getCurrentDirectory_detour(int* param_1, int* param_2) {
    int result = getCurrentDirectory_intercept.callUndetoured(param_1, param_2);
    g_gdbConnection->executeUntilAddress(0x8002852C);
    int expectedResult = g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
    assert(result == expectedResult);
    return result;
}

void updateFieldInputs_detour() {
    updateFieldInputs_intercept.callUndetoured();

    g_gdbConnection->executeUntilAddress(0x800748e4);
    // stomp the input with the ones from the PSX

    padButtonForScripts[0].m0_buttons = g_gdbConnection->readU16(0x800afe9c);
    padButtonForScripts[1].m0_buttons = g_gdbConnection->readU16(0x800afea0);
}

bool validationInit() {
    g_gdbConnection = new GDBConnection();
    if (!g_gdbConnection->openConnection("127.0.0.1", 3333)) {
        return false;
    }
    g_gdbConnection->pauseExecution();
    g_gdbConnection->resetTarget();
    g_gdbConnection->executeUntilAddress(0x80019578);

    getCurrentDirectory_intercept.enable();
    updateFieldInputs_intercept.enable();

    //u32 PC = g_gdbConnection->resumeExecution();
    //assert(PC == 0x80019578);
}