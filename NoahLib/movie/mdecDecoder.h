#pragma once

#include "noahLib.h"

// PS1 MDEC software decoder
// Decodes Huffman (Huffman) coded MDEC bitstream from STR video frames into RGB pixels

struct sMdecDecoder {
    void init();

    // Decode a complete MDEC frame from Huffman-coded bitstream into RGB output
    bool decodeFrame(const u8* mdecData, int mdecSize, int width, int height,
                     std::vector<u8>& rgbOut);

private:
    // Huffman decode: convert Huffman bitstream to MDEC hardware format
    bool huffmanDecode(const u16* input, int inputHalfwords, std::vector<u16>& output, int qscale);

    // Decode a single 8x8 block from MDEC hardware format words
    bool decodeBlock(const u16*& bs, int& remaining, s16 block[64], const u8 qt[64]);

    // Inverse DCT on an 8x8 block (in-place)
    void idct(s16 block[64]);

    // Convert a 16x16 macroblock from YCbCr to RGB
    void yuv2rgb(const s16 yBlocks[4][64], const s16 crBlock[64], const s16 cbBlock[64],
                 u8* rgbOut, int stride);

    // Zigzag reorder table
    static const u8 sZigzag[64];

    // Default quantization matrix
    static const u8 sDefaultQt[64];

    // IDCT constants
    float m_idctTable[8][8];

    // Huffman lookup tables (loaded from overlay)
    std::vector<u32> m_acFastTable; // 8192 entries * 2 u32 = 16384 u32
    std::vector<u32> m_acSlowTable; // 512 entries * 1 u32
    std::vector<u16> m_dcChromaTable; // 256 entries * 2 u16 (code_len, extra_bits)
    std::vector<u16> m_dcLumaTable;   // 256 entries * 2 u16
    bool m_huffmanTablesLoaded;
};
