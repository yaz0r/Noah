#include "noahLib.h"
#include "validation.h"
#include "gdbConnection.h"

#include "kernel/bootMenuValidation.h"
#include "field/fieldInputsValidation.h"

#include "kernel/gameState.h"

bool validationInit() {
    g_gdbConnection = new GDBConnection();
    if (!g_gdbConnection->openConnection("127.0.0.1", 3333)) {
        return false;
    }
    g_gdbConnection->pauseExecution();
    g_gdbConnection->resetTarget();
    g_gdbConnection->executeUntilAddress(0x80019578); // start of main

    // disable the EA logo to speed boot
    g_gdbConnection->writeU32(0x8001991c, 0x00000000);

    g_gdbConnection->executeUntilAddress(0x8001991c); // just before booting the game

    // patch boot mode to kernel
    g_gdbConnection->writeU32(0x80019930, 0x00000434); 
    
    // Patch game state to enter debug room from kernel
    gameState.m231A_fieldID = 0;
    g_gdbConnection->writeU16(0x8006f94e, 0);

    boolMenuValidation_init();
    fieldInputsValidation_init();

    return true;
}