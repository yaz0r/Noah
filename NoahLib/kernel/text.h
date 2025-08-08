#pragma once

extern std::vector<u8> dialogSpecialCase9AndA_2Data;
int getDialogFontCharacterWidth(uint param_1, uint param_2);
void printDialogCharacter(ushort characterType, ushort characterId, ushort* outputBuffer, short outputBufferStride, int oddOrEvenLine);
void dialogSpecialCase9AndA(s32 param_1, int param_2, int param_3);
