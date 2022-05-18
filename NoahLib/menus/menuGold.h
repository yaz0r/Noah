#pragma once

#include <array>
#include "kernel/graphics.h"

struct sMenuContext_340_goldOwned {
    std::array<std::array<POLY_FT4, 2>, 9> m0_goldPolys;
    std::array<POLY_FT4, 2> m2D0;
    u32 m320_goldPolyLength;
    u32 m324_oddOrEven;
};

void updateGoldDisplay();
