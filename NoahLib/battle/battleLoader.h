#pragma once

void battleLoaderTick(s8 param_1);

ushort bitmaskCharacterCheck(ushort bitmask, uint characterId);
extern std::array<u16, 16> bitmaskCharacter;
extern std::array<std::array<u8, 4>, 32> battleSlotLayout;
