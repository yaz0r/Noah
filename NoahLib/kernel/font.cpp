#include "noahLib.h"
#include "font.h"
#include "field/field.h"

int dialogFontVar0 = 0;
int dialogFontVar1 = 0;
int dialogFontVar2 = 0;
int dialogFontVar3 = 0;
int dialogFontVar4 = 0;

std::vector<u8> dialogFontVarPtr;
u8* dialogFontVarPtr2;

std::vector<std::vector<u8>::iterator>::iterator printDialogTextVar;

void initFont(sLoadableDataRaw& fontData)
{
	assert(fontData.getRawData().size());

    dialogFontVar0 = READ_LE_U16(fontData.getRawData().begin() + 4);
    dialogFontVar4 = READ_LE_U16(fontData.getRawData().begin() + 6);
    dialogFontVar2 = READ_LE_U16(fontData.getRawData().begin() + 8);
    dialogFontVar3 = READ_LE_U16(fontData.getRawData().begin() + 10);
    dialogFontVar1 = READ_LE_U16(fontData.getRawData().begin() + 0xC);
	dialogFontVarPtr = fontData.getRawData();
	dialogFontVarPtr2 = &dialogFontVarPtr[0] + READ_LE_U16(dialogFontVarPtr.begin() + 2);

}

struct sBattleDialogBundle {
    u32 m0_numEntries;
    std::vector<std::vector<u8>::iterator> m4_entries;

    void init(const std::vector<u8>& rawData) {
        m_rawData = rawData;
        m0_numEntries = READ_LE_U32(m_rawData.begin());
        m4_entries.resize(m0_numEntries);
        for (int i = 0; i < m0_numEntries; i++) {
            m4_entries[i] = m_rawData.begin() + READ_LE_U32(m_rawData.begin() + 4 + i * 4);
        }
    }

    std::vector<u8> m_rawData;
};

void setupPrintDialogTextVar(sLoadableDataRaw& fontData)
{
    assert(fontData.getRawData().size());
    static sBattleDialogBundle persistantBuffer;
    persistantBuffer.init(fontData.getRawData());
    
    flagAllocation(fontData);
    //printDialogTextVar = persistantBuffer.begin();
    //printDialogTextVar2 = fontData;
    MissingCode();
    //doPointerRelocation(persistantBuffer);
    printDialogTextVar = persistantBuffer.m4_entries.begin();
}