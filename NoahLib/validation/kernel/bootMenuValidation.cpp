#include "noahLib.h"
#include "bootMenuValidation.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"

#include "kernel/inputs.h"
#include "kernel/gameState.h"

void printKernelMenu();
void printKernelMenu_detour();
interceptor<void> printKernelMenu_intercept(printKernelMenu, printKernelMenu_detour);

void printKernelMenu_detour() {
    g_gdbConnection->executeUntilAddress(0x8001a344);

    // stomp the input with the ones from the PSX
    newPadButtonForField = g_gdbConnection->readU16(0x800594a4);
    newPadButtonForDialogs = g_gdbConnection->readU16(0x8005948c);

    printKernelMenu_intercept.callUndetoured();
}

void boolMenuValidation_init()
{
    printKernelMenu_intercept.enable();
}

void boolMenuValidation_shutdown()
{
    printKernelMenu_intercept.disable();
}