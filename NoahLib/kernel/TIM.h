#pragma once

struct TIM_IMAGE
{
    u32 mode;
    struct RECT* crect;
    const void* caddr;
    struct RECT* prect;
    const void* paddr;
};
int OpenTIM(std::vector<u8>::const_iterator ptr);
TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);

void StoreImage(struct RECT* rect, std::vector<u16>& output);
void LoadImage(struct RECT* pRect, const u8* data);
void LoadImage(struct RECT* pRect, std::span<u8>::iterator data);
void LoadImage(struct RECT* pRect, std::span<u16>::iterator data);
void loadTimToVram(std::vector<u8>::iterator ptr, short imageX, short imageY, short palX, short palY, short palW, short palH);

void loadMenuImageBundle(const std::vector<u8>& imageBundle);