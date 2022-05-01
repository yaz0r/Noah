#pragma once

extern int dialogFontVar0;
extern int dialogFontVar1;
extern int dialogFontVar2;
extern int dialogFontVar3;
extern int dialogFontVar4;

extern std::vector<u8> dialogFontVarPtr;
extern u8* dialogFontVarPtr2;

void initFont(std::vector<u8>& fontData);
void setupPrintDialogTextVar(std::vector<u8>& fontData);
