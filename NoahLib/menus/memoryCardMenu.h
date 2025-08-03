#pragma once

#include "psx/libgpu.h"

struct sMenuContext_34C {
    std::array<std::array<POLY_FT4, 2>, 32> m0_polys;
    std::array<POLY_FT4, 2> mA00_polys2;
    std::array<POLY_G4, 2> mA50_polyG4;

    // size 0x2dc0
};

void initMenuContext34C();