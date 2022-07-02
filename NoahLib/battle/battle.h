#pragma once

extern std::vector<u8> battleCharacterConfigFile;
extern std::vector<u8>::iterator battleMechaInitData;

#include "field/mecha/mechaOverlay.h"

extern std::vector<u8>* battleLoadDataVar0_raw;
extern std::vector<u8>::iterator battleLoadDataVar1;
extern std::vector<u8>::iterator battleLoadDataVar2;

void battleEntryPoint(void);