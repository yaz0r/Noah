#pragma once

#include "psx/gpuprims.h"

struct sMenuContext_3A8 {
    std::array<POLY_FT4, 2> m0_memoryCardTransparentPolys;
    std::array<LINE_F3, 2> m50;
    std::array<LINE_F3, 2> m80;
    std::array<SVECTOR, 4> mE0_vertices;
    std::array<POLY_F4, 2> mB0_memoryCardOpaquePolys;
    std::array<SVECTOR, 4> m100;
    std::array<SVECTOR, 4> m120;
    std::array<DR_MODE, 2> m140;
    //size 0x158
};

void initMenuContext3A8_memoryCardDisplay(void);
void drawMenu_memoycard_tilesLines(void);
void drawMenu_4D8_text(void);