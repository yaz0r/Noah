#include "noahLib.h"
#include "validation/kernel/validateKernel.h"
#include "validation/interception.h"
#include "validation/gdbConnection.h"

int isCDBusy();
int isCDBusy_detour();

interceptor<int> isCDBusy_intercept(isCDBusy, isCDBusy_detour);

int isCDBusy_detour() {
    g_gdbConnection->executeUntilAddress(0x80028724);
    isCDBusy_intercept.callUndetoured();
    return g_gdbConnection->getRegister(GDBConnection::REG_Names::V0);
}

void validateKernel_init() {
    //isCDBusy_intercept.enable(); doing this is way too slow
}
