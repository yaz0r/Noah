#pragma once
#include "kernel/math.h"

void loadMechaOVerlayAndRegisterMechas();

void freeMechaModule();

void renderMechasInField();
void initMechaFieldArgs2(MATRIX& param_1, short param_2, short param_3, short param_4, short param_5, short param_6, short param_7, short param_8, short param_9, short param_10);

void renderMechasForDebugFieldRenderer(int viewId);

extern std::vector<u8> mechaOverlayBuffer;
extern u32 NumMechas;
extern std::array<s16, 4> mechaList;
extern std::array<u8, 4> mechaList2;
extern s16 numMaxMechaOverlayEntries;
extern std::array<u8, 3> mechaBackColor;
extern std::array<MATRIX, 2> mechaFieldArgs2;
