#pragma once

typedef std::vector<u16> sRamTexture;
extern std::array<sRamTexture*, 10> itemLabelsIds;
std::vector<u8>::iterator getItemLabelString(int param_1);
sRamTexture* allocateTextureRamForText(int param_1);

void battleLoaderTick(s8 param_1);
ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);
extern std::array<u16, 16> bitmaskCharacter;
extern std::array<std::array<u8, 4>, 32> battleSlotLayout;
