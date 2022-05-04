#include "noahLib.h"
#include "font.h"

int dialogFontVar0 = 0;
int dialogFontVar1 = 0;
int dialogFontVar2 = 0;
int dialogFontVar3 = 0;
int dialogFontVar4 = 0;

std::vector<u8> dialogFontVarPtr;
u8* dialogFontVarPtr2;

void initFont(std::vector<u8>& fontData)
{
	assert(fontData.size());

	dialogFontVar0 = READ_LE_U16(fontData.begin() + 4);
	dialogFontVar4 = READ_LE_U16(fontData.begin() + 6);
	dialogFontVar2 = READ_LE_U16(fontData.begin() + 8);
	dialogFontVar3 = READ_LE_U16(fontData.begin() + 10);
	dialogFontVar1 = READ_LE_U16(fontData.begin() + 0xC);
	dialogFontVarPtr = fontData;
	dialogFontVarPtr2 = &dialogFontVarPtr[0] + READ_LE_U16(dialogFontVarPtr.begin() + 2);

}

void setupPrintDialogTextVar(std::vector<u8>& fontData)
{
    assert(fontData.size());
    /*
    flagAllocation(fontData);
    printDialogTextVar = fontData;
    printDialogTextVar2 = fontData;
    doPointerRelocation(fontData);
    printDialogTextVar = printDialogTextVar + 4;
    */
}