#pragma once

struct sFieldCompassVar2
{
	SVECTOR m0;
	SVECTOR m8;
	SVECTOR m10;
	SVECTOR m18;
	std::array<POLY_FT4, 2> m20;
	// size 0x70
};
extern std::array<sFieldCompassVar2, 25> fieldCompassVar2;

void renderCompass();
void initCompassData();
void initCompassPoly(sFieldCompassVar2* prim, int XIndex, int ZIndex, int paletteIndex);
