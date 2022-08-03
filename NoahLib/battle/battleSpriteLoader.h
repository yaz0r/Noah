#pragma once

#include "kernel/filesystem.h"

struct sBattleSpriteConfigs : public sLoadableDataRaw {

};
extern sBattleSpriteConfigs battleConfigFile3;

void createBattleSpriteLoadingTask(sBattleSpriteConfigs* param_1);