#pragma once

extern int dialogFontVar0;
extern int dialogFontVar1;
extern int dialogFontVar2;
extern int dialogFontVar3;
extern int dialogFontVar4;

extern std::vector<u8> dialogFontVarPtr;
extern u8* dialogFontVarPtr2;

extern std::vector<std::vector<u8>::iterator>::iterator printDialogTextVar;

void initFont(std::vector<u8>& fontData);
void setupPrintDialogTextVar(std::vector<u8>& fontData);

struct sFontGlyphPoly {
    s16 m0_textureU;
    s16 m2_textureV;
    s16 m4_textureWidth;
    s16 m6_textureHeight;
    s16 m8_width;
    s16 mA_height;
    u16 mC;
    u16 mE;
    u16 m10_colorDepth;
    u16 m12_clutX;
    u16 m14_clutY;
    u16 m16_tpageX;
    u16 m18_tpageY;
    u8 m1A_flipX;
    u8 m1B_flipY;

    void deserialize(const std::vector<u8>::const_iterator input) {
        m0_textureU = READ_LE_S16(input + 0x00);
        m2_textureV = READ_LE_S16(input + 0x02);
        m4_textureWidth = READ_LE_S16(input + 0x04);
        m6_textureHeight = READ_LE_S16(input + 0x06);
        m8_width = READ_LE_S16(input + 0x08);
        mA_height = READ_LE_S16(input + 0x0A);
        mC = READ_LE_U16(input + 0x0C);
        mE = READ_LE_U16(input + 0x0E);
        m10_colorDepth = READ_LE_U16(input + 0x10);
        m12_clutX = READ_LE_U16(input + 0x12);
        m14_clutY = READ_LE_U16(input + 0x14);
        m16_tpageX = READ_LE_U16(input + 0x16);
        m18_tpageY = READ_LE_U16(input + 0x18);
        m1A_flipX = READ_LE_U8(input + 0x1A);
        m1B_flipY = READ_LE_U8(input + 0x1B);
    }

    // size 0x1C
};

struct sFontGlyph {
    u16 m0_polyCount;
    u16 m2;
    std::vector<sFontGlyphPoly> m4_polys;

    void deserialize(const std::vector<u8>::const_iterator input) {
        m0_polyCount = READ_LE_U16(input + 0);
        m2 = READ_LE_U16(input + 2);
        m4_polys.resize(m0_polyCount);
        for (int i = 0; i < m0_polyCount; i++) {
            m4_polys[i].deserialize(input + 4 + i * 0x1C);
        }
    }
};

struct sFont {
    u32 m0_numGlyphs;
    std::vector<sFontGlyph> m4;

    void deserialize(const std::vector<u8>& input) {
        m0_numGlyphs = READ_LE_U32(input.begin() + 0);
        assert(READ_LE_U16(input.begin() + 4) == m0_numGlyphs * 2 + 4);
        m4.resize(m0_numGlyphs);
        for (u32 i = 0; i < m0_numGlyphs; i++) {
            m4[i].deserialize(input.cbegin() + READ_LE_U16(input.begin() + 4 + 2 * i));
        }
    }
};