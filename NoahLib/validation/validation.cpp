#include "noahLib.h"
#include "validation.h"
#include "gdbConnection.h"

#include "validation/kernel/bootMenuValidation.h"
#include "validation/kernel/validateKernel.h"
#include "validation/field/fieldInputsValidation.h"
#include "validation/field/validateField.h"


#include "kernel/gameState.h"

bool validationInit() {
    g_gdbConnection = new GDBConnection();
    if (!g_gdbConnection->openConnection("127.0.0.1", 3333)) {
        delete g_gdbConnection;
        g_gdbConnection = nullptr;
        return false;
    }
    g_gdbConnection->pauseExecution();
    g_gdbConnection->resetTarget();
    g_gdbConnection->executeUntilAddress(0x80019578); // start of main

    // disable the EA logo to speed boot
    g_gdbConnection->writeU32(0x8001991c, 0x00000000);

    g_gdbConnection->executeUntilAddress(0x8001991c); // just before booting the game

    // patch boot mode to kernel
    g_gdbConnection->writeU32(0x80019930, 0x34040000); 
    
    // Patch game state to enter debug room from kernel
    gameState.m231A_fieldID = 0;
    g_gdbConnection->writeU16(0x8006f94e, gameState.m231A_fieldID);

    // Sync random seed
    randSeed = g_gdbConnection->readU32(0x8005a1fc);

    validateKernel_init();
    boolMenuValidation_init();
    validateField_init();
    fieldInputsValidation_init();

    return true;
}