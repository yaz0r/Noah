#pragma once

struct TIM_IMAGE
{
    u32 mode;
    struct RECT* crect;
    void* caddr;
    struct RECT* prect;
    void* paddr;
};
int OpenTIM(std::vector<u8>::iterator ptr);
TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);

void LoadImage(struct RECT* pRect, const u8* data);
void LoadImage(RECT* pRect, sPS1Pointer data);
void loadTimToVram(std::vector<u8>::iterator ptr, short imageX, short imageY, short palX, short palY, short palW, short palH);
