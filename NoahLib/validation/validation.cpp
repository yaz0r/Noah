#include "noahLib.h"
#include "validation.h"
#include "gdbConnection.h"

GDBConnection* g_gdbConnection = nullptr;

bool validationInit() {
    g_gdbConnection = new GDBConnection();
    if (!g_gdbConnection->openConnection("127.0.0.1", 3333)) {
        return false;
    }
    g_gdbConnection->pauseExecution();
    g_gdbConnection->setBreakpoint(0x80019578); // start of main
    g_gdbConnection->resetTarget();

    u32 PC = g_gdbConnection->resumeExecution();
    assert(PC == 0x80019578);
}