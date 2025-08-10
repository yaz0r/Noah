#include "noahLib.h"
#include "validation/gdbConnection.h"

void validateRandomSeed() {
    // Sync random seed
    assert(randSeed == g_gdbConnection->readU32(0x8005a1fc));
}