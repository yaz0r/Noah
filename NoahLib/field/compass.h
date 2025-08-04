#pragma once

struct sFieldCompassVar2
{
	std::array<SVECTOR, 4> m0;
	std::array<POLY_FT4, 2> m20_poly;
	// size 0x70
};
extern std::array<sFieldCompassVar2, 25> fieldCompassVar2;

void renderCompass();
void initCompassData();
void initCompassPoly(sFieldCompassVar2* prim, int XIndex, int ZIndex, int paletteIndex);
void initCompassShadowPoly();
