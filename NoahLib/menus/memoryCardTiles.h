#pragma once

#include "psx/gpuprims.h"

struct sMenuContext_3A8 {
    std::array<LINE_F3, 2> m50;
    std::array<LINE_F3, 2> m80;
    std::array<SVECTOR, 4> m100;
    std::array<SVECTOR, 4> m120;
    //size 0x158
};

void initMenuContext3A8_memoryCardDisplay(void);
void drawMenu_memoycard_tilesLines(void);