#include "noahLib.h"
#include "memoryCardTiles.h"
#include "menuHandler.h"

void initMenuContext3A8_memoryCardDisplay(void)
{
    for(int i=0; i<0x20; i++) {
        gMenuContext->m3A8_memoryCardTiles[i] = new sMenuContext_3A8;
        memset(gMenuContext->m3A8_memoryCardTiles[i], 0, sizeof(sMenuContext_3A8));
        //initMenuContext3A8_MemoryCardPolys(i);
        //initMenuContext3A8_Lines(i);
        MissingCode();
    }
}