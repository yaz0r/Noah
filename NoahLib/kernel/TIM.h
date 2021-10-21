#pragma once

void LoadImage(struct RECT* pRect, const u8* data);
void LoadImage(RECT* pRect, sPS1Pointer data);
void loadTimToVram(std::vector<u8>::iterator ptr, short imageX, short imageY, short palX, short palY, short palW, short palH);
