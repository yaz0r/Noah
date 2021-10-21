#include "noahLib.h"
#include "TIM.h"
#include "graphics.h"

extern std::array<u8, 2048 * 512> gVram;

RECT TIM_crect;
RECT TIM_prect;

struct TIM_IMAGE
{
	u32 mode;
	RECT* crect;
	void* caddr;
	RECT* prect;
	void* paddr;
};

std::vector<u8>::iterator currentTIM;

int OpenTIM(std::vector<u8>::iterator ptr)
{
	currentTIM = ptr;
	return 0;
}

int get_tim_addr(std::vector<u8>::iterator input, TIM_IMAGE* timimg)
{
	if (READ_LE_U32(input) == 0x10)
	{
		std::vector<u8>::iterator timaddr = input + 8;
		timimg->mode = READ_LE_U32(input + 4);
		int uVar3 = 0;
		if ((timimg->mode & 8) == 0)
		{
			timimg->crect = nullptr;
			timimg->caddr = nullptr;
		}
		else
		{
			TIM_crect.x = READ_LE_S16(input + 0xC);
			TIM_crect.y = READ_LE_S16(input + 0xC + 2);
			TIM_crect.w = READ_LE_S16(input + 0xC + 4);
			TIM_crect.h = READ_LE_S16(input + 0xC + 6);
			timimg->crect = &TIM_crect;
			timimg->caddr = &(*(input + 0x14));
			uVar3 = READ_LE_U32(timaddr) >> 2;
			timaddr += READ_LE_U32(timaddr) & ~3;

		}
		TIM_prect.x = READ_LE_S16(timaddr + 4);
		TIM_prect.y = READ_LE_S16(timaddr + 4 + 2);
		TIM_prect.w = READ_LE_S16(timaddr + 4 + 4);
		TIM_prect.h = READ_LE_S16(timaddr + 4 + 6);
		timimg->prect = &TIM_prect;
		timimg->paddr = &(*(timaddr + 0xC));
		return uVar3 + (READ_LE_U32(timaddr) / 4) + 2;
	}
	else
	{
		return -1;
	}
}

TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg)
{
	int timOffset = get_tim_addr(currentTIM, timimg);
	if (timOffset == -1)
	{
		return nullptr;
	}
	else
	{
		currentTIM += timOffset * 4;
	}
	return timimg;
}

void loadTimToVram(std::vector<u8>::iterator ptr, short imageX, short imageY, short palX, short palY, short palW, short palH)
{
	OpenTIM(ptr);

	TIM_IMAGE timImageData;
	if (ReadTIM(&timImageData))
	{
		if (timImageData.caddr)
		{
			if (palY != -1)
			{
				timImageData.crect->x = palX;
				timImageData.crect->y = palY;
			}
			if (palW != 0)
			{
				timImageData.crect->w = palW;
			}
			if (palH != 0)
			{
				timImageData.crect->h = palH;
			}
			LoadImage(timImageData.crect, (u8*)timImageData.caddr);
		}
		if (timImageData.paddr)
		{
			timImageData.prect->x = imageX;
		}
		// huh? this seems buggy in the original game, as this should have been in the above test
		timImageData.prect->y = imageY;
		LoadImage(timImageData.prect, (u8*)timImageData.paddr);
	}
}

void LoadImage(RECT* pRect, const u8* data)
{
	int vramOffset = pRect->y * 2048 + pRect->x * 2;
	for (int y = 0; y < pRect->h; y++)
	{
		auto vramIterator = gVram.begin() + vramOffset;
		for (int x = 0; x < pRect->w * 2; x++)
		{
			*(vramIterator++) = *(data++);
			vramOffset++;
		}
		vramOffset += 2048 - pRect->w * 2;
	}
}

void LoadImage(RECT* pRect, sPS1Pointer data)
{
	auto vramIterator = gVram.begin() + pRect->y * 2048 + pRect->x * 2;
	for (int y = 0; y < pRect->h; y++)
	{
		for (int x = 0; x < pRect->w * 2; x++)
		{
			*vramIterator = READ_LE_U8(data);
			vramIterator++;
			data = data + 1;
		}

		vramIterator += 2048 - pRect->w * 2;
	}
}
