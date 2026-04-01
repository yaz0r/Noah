#include "noahLib.h"
#include "mdecDecoder.h"
#include <cmath>
#include <algorithm>

// Zigzag scan order: maps zigzag position k -> row-major index in 8x8 block
const u8 sMdecDecoder::sZigzag[64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

// PS1 default quantization matrix
const u8 sMdecDecoder::sDefaultQt[64] = {
     2, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83
};

//////////////////////////////////////////////////////////////////////////////
// Init — load Huffman tables from the overlay binary
//////////////////////////////////////////////////////////////////////////////

void sMdecDecoder::init()
{
    // Precompute IDCT cosine table
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            m_idctTable[i][j] = cosf((float)(2 * i + 1) * (float)j * 3.14159265358979f / 16.0f);
        }
    }

    // Load Huffman tables from the STR library overlay
    // Fast table: 8192 entries * 8 bytes at overlay offset 0x502C
    // Slow table: 512 entries * 4 bytes at overlay offset 0x1502C
    m_huffmanTablesLoaded = false;

    FILE* f = fopen("movie_str_lib.ovl", "rb");
    if (!f) {
        Noah_Log("MDEC: failed to open movie_str_lib.ovl for Huffman tables\n", 0);
        return;
    }

    // Fast table: 8192 entries, each 2 x u32 = 8 bytes, total 65536 bytes
    m_acFastTable.resize(8192 * 2);
    fseek(f, 0x502C, SEEK_SET);
    fread(m_acFastTable.data(), 4, 8192 * 2, f);

    // Slow table: 512 entries, each 1 x u32 = 4 bytes, total 2048 bytes
    m_acSlowTable.resize(512);
    fseek(f, 0x1502C, SEEK_SET);
    fread(m_acSlowTable.data(), 4, 512, f);

    // DC Huffman tables: 256 entries each, 4 bytes per entry (2 x u16: code_len, extra_bits)
    // Chroma DC table at overlay offset 0x4C2C (address 801d7c2c)
    m_dcChromaTable.resize(256 * 2);
    fseek(f, 0x4C2C, SEEK_SET);
    fread(m_dcChromaTable.data(), 2, 256 * 2, f);

    // Luma DC table at overlay offset 0x482C (address 801d782c)
    m_dcLumaTable.resize(256 * 2);
    fseek(f, 0x482C, SEEK_SET);
    fread(m_dcLumaTable.data(), 2, 256 * 2, f);

    fclose(f);
    m_huffmanTablesLoaded = true;
    Noah_Log("MDEC: Huffman tables loaded OK\n", 0);
}

//////////////////////////////////////////////////////////////////////////////
// Huffman decode — convert compressed bitstream to MDEC hardware format
// Matches Ghidra strVlcDecode (FUN_movie_str_lib__801d4cc8)
//////////////////////////////////////////////////////////////////////////////

bool sMdecDecoder::huffmanDecode(const u16* input, int inputHalfwords, std::vector<u16>& output, int qscale)
{
    if (!m_huffmanTablesLoaded) return false;

    output.clear();
    output.reserve(inputHalfwords * 2);

    // Bit accumulator (32 bits, MSB-first)
    u32 bits = 0;
    int bitPos = 0;
    const u16* src = input;
    const u16* srcEnd = input + inputHalfwords;

    // Macro: consume N bits and refill if needed
    #define CONSUME_BITS(n) do { \
        bits <<= (n); \
        bitPos += (n); \
        if (bitPos >= 16 && src < srcEnd) { \
            bitPos -= 16; \
            bits |= (u32)(*src++) << bitPos; \
        } \
    } while(0)

    // Initial load: word0 HIGH, word1 LOW (MSB-first)
    if (src < srcEnd) bits = (u32)(*src++) << 16;
    if (src < srcEnd) bits |= (u32)(*src++);

    u32 qscaleShifted = (u32)(qscale & 0x3F) << 10;

    // For version 2: uVar19 stays 0, meaning ALL blocks use raw 10-bit absolute DC
    // (Differential Huffman DC is only used in version 3)
    bool inAC = false;
    int maxIter = inputHalfwords * 32;

    while (maxIter-- > 0) {
        if (!inAC) {
            // DC coefficient: raw 10-bit absolute value
            u32 dcBits = bits >> 22; // top 10 bits

            if (dcBits == 0x1FF) {
                // End-of-frame signal (DC=0x1FF means end of data)
                break;
            }

            CONSUME_BITS(10);

            // Output DC word: qscale in bits 15-10, DC value in bits 9-0
            output.push_back((u16)(qscaleShifted | (dcBits & 0x3FF)));
            inAC = true;
        } else {
            // AC coefficient: use Huffman lookup table
            u32 tableIdx = bits >> 19; // top 13 bits
            u32 entry0 = m_acFastTable[tableIdx * 2];
            u32 extraWord;

            if (entry0 == 0) {
                // Fast table miss — shift 8 bits, use slow table
                CONSUME_BITS(8);
                u32 slowIdx = bits >> 23; // top 9 bits
                entry0 = m_acSlowTable[slowIdx];
                extraWord = 0;
            } else {
                extraWord = m_acFastTable[tableIdx * 2 + 1];
            }

            int codeBits = entry0 & 0xFF;
            u16 outputWord = (u16)(entry0 >> 16);

            if (codeBits == 0 && outputWord == 0) {
                // Invalid table entry — skip a bit to avoid infinite loop
                CONSUME_BITS(1);
                continue;
            }

            CONSUME_BITS(codeBits);

            // Handle the output word and extra words from the fast table
            auto emitWord = [&](u16 word) -> bool {
                if (word == 0x7C1F) {
                    // Escape: read raw 16 bits from bitstream
                    u16 raw = (u16)(bits >> 16);
                    CONSUME_BITS(16);
                    output.push_back(raw);
                    return false; // escape processed, don't continue to more extras
                } else {
                    output.push_back(word);
                    if (word == 0xFE00) {
                        inAC = false;
                        return false; // EOB, stop processing extras
                    }
                    return true; // continue to next extra if available
                }
            };

            if (!emitWord(outputWord)) {
                // main word was escape or EOB, done
            } else if (extraWord != 0) {
                u16 extra_lo = (u16)(extraWord & 0xFFFF);
                if (emitWord(extra_lo)) {
                    u16 extra_hi = (u16)(extraWord >> 16);
                    if (extra_hi != 0) {
                        emitWord(extra_hi);
                    }
                }
            }
        }
    }

    #undef CONSUME_BITS
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// Block decode from MDEC hardware format (after Huffman decode)
//////////////////////////////////////////////////////////////////////////////

bool sMdecDecoder::decodeBlock(const u16*& bs, int& remaining, s16 block[64], const u8 qt[64])
{
    memset(block, 0, sizeof(s16) * 64);

    if (remaining < 1) return false;

    u16 val = *bs++; remaining--;

    // First word: bits 15-10 = quantScale, bits 9-0 = DC (signed 10-bit)
    s16 dc = (s16)(val << 6) >> 6;
    int quantScale = (val >> 10) & 0x3F;

    // Dequantize DC
    if (quantScale == 0) {
        block[0] = dc * 2;
    } else {
        block[0] = dc * qt[0];
    }

    // AC coefficients
    int k = 0;
    while (remaining > 0) {
        val = *bs++; remaining--;

        if (val == 0xFE00) break; // End of block

        int run = (val >> 10) & 0x3F;
        s16 level = (s16)(val << 6) >> 6;

        k += run + 1;
        if (k >= 64) break;

        int zigzagIdx = sZigzag[k];

        if (quantScale == 0) {
            block[zigzagIdx] = level * 2;
        } else {
            block[zigzagIdx] = (level * qt[zigzagIdx] * quantScale + 4) >> 3;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// IDCT
//////////////////////////////////////////////////////////////////////////////

void sMdecDecoder::idct(s16 block[64])
{
    float temp[64];

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            float sum = 0;
            for (int u = 0; u < 8; u++) {
                float cu = (u == 0) ? 0.353553390593274f : 0.5f;
                sum += cu * (float)block[y * 8 + u] * m_idctTable[x][u];
            }
            temp[y * 8 + x] = sum;
        }
    }

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            float sum = 0;
            for (int v = 0; v < 8; v++) {
                float cv = (v == 0) ? 0.353553390593274f : 0.5f;
                sum += cv * temp[v * 8 + x] * m_idctTable[y][v];
            }
            block[y * 8 + x] = (s16)std::clamp((int)roundf(sum), -512, 511);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// YCbCr -> RGB
//////////////////////////////////////////////////////////////////////////////

static u8 clampU8(int v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (u8)v;
}

void sMdecDecoder::yuv2rgb(const s16 yBlocks[4][64], const s16 crBlock[64], const s16 cbBlock[64],
                           u8* rgbOut, int stride)
{
    for (int by = 0; by < 2; by++) {
        for (int bx = 0; bx < 2; bx++) {
            int blockIdx = by * 2 + bx;
            const s16* yBlock = yBlocks[blockIdx];

            for (int py = 0; py < 8; py++) {
                for (int px = 0; px < 8; px++) {
                    int x = bx * 8 + px;
                    int y = by * 8 + py;
                    int cx = x / 2;
                    int cy = y / 2;

                    s16 yVal = yBlock[py * 8 + px];
                    s16 cr = crBlock[cy * 8 + cx];
                    s16 cb = cbBlock[cy * 8 + cx];

                    int r = yVal + ((1435 * cr) >> 10) + 128;
                    int g = yVal - ((351 * cb) >> 10) - ((731 * cr) >> 10) + 128;
                    int b = yVal + ((1814 * cb) >> 10) + 128;

                    u8* pixel = rgbOut + (y * stride + x) * 3;
                    pixel[0] = clampU8(r);
                    pixel[1] = clampU8(g);
                    pixel[2] = clampU8(b);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Frame decode
//////////////////////////////////////////////////////////////////////////////

bool sMdecDecoder::decodeFrame(const u8* mdecData, int mdecSize, int width, int height,
                               std::vector<u8>& rgbOut)
{
    rgbOut.resize(width * height * 3);

    if (mdecSize < 8) return false;

    const u16* headerWords = (const u16*)mdecData;
    u16 wordCount = headerWords[0];
    u16 magic = headerWords[1];
    u16 frameQuantScale = headerWords[2];
    u16 version = headerWords[3];

    if (magic != 0x3800) {
        Noah_Log("MDEC: bad frame magic 0x%04x (expected 0x3800)\n", magic);
        return false;
    }

    // Step 1: Huffman decode the compressed bitstream into MDEC hardware format
    std::vector<u16> mdecWords;
    if (!huffmanDecode((const u16*)(mdecData + 8), (mdecSize - 8) / 2, mdecWords, frameQuantScale)) {
        return false;
    }

    // Step 2: Decode MDEC hardware format into pixel blocks
    const u16* bs = mdecWords.data();
    int remaining = (int)mdecWords.size();

    const u8* qt = sDefaultQt;
    int mbCountX = width / 16;
    int mbCountY = height / 16;

    // PS1 STR player processes macroblocks in column-major order
    // (one full column of macroblocks at a time, left to right)
    bool decodeError = false;
    for (int mbx = 0; mbx < mbCountX; mbx++) {
        for (int mby = 0; mby < mbCountY; mby++) {
            s16 crBlock[64] = {}, cbBlock[64] = {};
            s16 yBlocks[4][64] = {};

            if (!decodeError) {
                if (!decodeBlock(bs, remaining, crBlock, qt)) { decodeError = true; }
                else idct(crBlock);

                if (!decodeError && !decodeBlock(bs, remaining, cbBlock, qt)) { decodeError = true; }
                else if (!decodeError) idct(cbBlock);

                for (int i = 0; i < 4 && !decodeError; i++) {
                    if (!decodeBlock(bs, remaining, yBlocks[i], qt)) { decodeError = true; }
                    else idct(yBlocks[i]);
                }
            }

            u8* mbOut = rgbOut.data() + (mby * 16 * width + mbx * 16) * 3;
            yuv2rgb(yBlocks, crBlock, cbBlock, mbOut, width);
        }
    }

    return true;
}
