#pragma once

void loadMechaOVerlayAndRegisterMechas();

void freeMechaModule();

extern std::vector<u8> mechaOverlayBuffer;
extern u32 NumMechas;
extern std::array<s16, 4> mechaList;
extern std::array<u8, 4> mechaList2;
extern s16 numMaxMechaOverlayEntries;