#include "noahLib.h"
#include "gte.h"
#include <math.h>

typedef union {
#if defined(__BIGENDIAN__)
	struct {
		uint8_t h3, h2, h, l;
	} b;
	struct {
		int8_t h3, h2, h, l;
	} sb;
	struct {
		uint16_t h, l;
	} w;
	struct {
		int16_t h, l;
	} sw;
#else
	struct {
		uint8_t l, h, h2, h3;
	} b;
	struct {
		uint16_t l, h;
	} w;
	struct {
		int8_t l, h, h2, h3;
	} sb;
	struct {
		int16_t l, h;
	} sw;
#endif
	uint32_t d;
	int32_t sd;
} PAIR;

struct
{
	std::array<PAIR, 32> p;
} CP2D;

struct
{
	std::array<PAIR, 32> p;
} CP2C;

#define GTE_SF(op) ((op >> 19) & 1)
#define GTE_MX(op) ((op >> 17) & 3)
#define GTE_V(op) ((op >> 15) & 3)
#define GTE_CV(op) ((op >> 13) & 3)
#define GTE_LM(op) ((op >> 10) & 1)
#define GTE_FUNCT(op) (op & 63)

s16& VX0 = (CP2D.p[0].sw.l);
s16& VY0 = (CP2D.p[0].sw.h);
s16& VZ0 = (CP2D.p[1].sw.l);
u16& VX1 = (CP2D.p[2].w.l);
u16& VY1 = (CP2D.p[2].w.h);
u16& VZ1 = (CP2D.p[3].w.l);
u16& VX2 = (CP2D.p[4].w.l);
u16& VY2 = (CP2D.p[4].w.h);
u16& VZ2 = (CP2D.p[5].w.l);
u8& R = (CP2D.p[6].b.l);
u8& G = (CP2D.p[6].b.h);
u8& B = (CP2D.p[6].b.h2);
u8& CODE = (CP2D.p[6].b.h3);
u16& OTZ = (CP2D.p[7].w.l);
s16& IR0 = (CP2D.p[8].sw.l);
s16& IR1 = (CP2D.p[9].sw.l);
s16& IR2 = (CP2D.p[10].sw.l);
s16& IR3 = (CP2D.p[11].sw.l);
u32& SXY0 = (CP2D.p[12].d);
s16& SX0 = (CP2D.p[12].sw.l);
s16& SY0 = (CP2D.p[12].sw.h);
u32& SXY1 = (CP2D.p[13].d);
#define SX1 (CP2D.p[13].sw.l)
#define SY1 (CP2D.p[13].sw.h)
u32& SXY2 = (CP2D.p[14].d);
#define SX2 (CP2D.p[14].sw.l)
#define SY2 (CP2D.p[14].sw.h)
u32& SXYP = (CP2D.p[15].d);
#define SXP (CP2D.p[15].sw.l)
#define SYP (CP2D.p[15].sw.h)
u16& SZ0 = (CP2D.p[16].w.l);
u16& SZ1 = (CP2D.p[17].w.l);
u16& SZ2 = (CP2D.p[18].w.l);
u16& SZ3 = (CP2D.p[19].w.l);
#define RGB0 (CP2D.p[20].d)
#define R0 (CP2D.p[20].b.l)
#define G0 (CP2D.p[20].b.h)
#define B0 (CP2D.p[20].b.h2)
#define CD0 (CP2D.p[20].b.h3)
#define RGB1 (CP2D.p[21].d)
#define R1 (CP2D.p[21].b.l)
#define G1 (CP2D.p[21].b.h)
#define B1 (CP2D.p[21].b.h2)
#define CD1 (CP2D.p[21].b.h3)
#define RGB2 (CP2D.p[22].d)
#define R2 (CP2D.p[22].b.l)
#define G2 (CP2D.p[22].b.h)
#define B2 (CP2D.p[22].b.h2)
#define CD2 (CP2D.p[22].b.h3)
#define RES1 (CP2D.p[23].d)
s32& MAC0 = (CP2D.p[24].sd);
s32& MAC1 = (CP2D.p[25].sd);
s32& MAC2 = (CP2D.p[26].sd);
s32& MAC3 = (CP2D.p[27].sd);
#define IRGB (CP2D.p[28].d)
#define ORGB (CP2D.p[29].d)
#define LZCS (CP2D.p[30].d)
#define LZCR (CP2D.p[31].d)

s16& R11 = (CP2C.p[0].sw.l);
s16& R12 = (CP2C.p[0].sw.h);
s16& R13 = (CP2C.p[1].sw.l);
s16& R21 = (CP2C.p[1].sw.h);
s16& R22 = (CP2C.p[2].sw.l);
s16& R23 = (CP2C.p[2].sw.h);
s16& R31 = (CP2C.p[3].sw.l);
s16& R32 = (CP2C.p[3].sw.h);
s16& R33 = (CP2C.p[4].sw.l);
s32& TRX = (CP2C.p[5].sd);
s32& TRY = (CP2C.p[6].sd);
s32& TRZ = (CP2C.p[7].sd);
#define L11 (CP2C.p[8].sw.l)
#define L12 (CP2C.p[8].sw.h)
#define L13 (CP2C.p[9].sw.l)
#define L21 (CP2C.p[9].sw.h)
#define L22 (CP2C.p[10].sw.l)
#define L23 (CP2C.p[10].sw.h)
#define L31 (CP2C.p[11].sw.l)
#define L32 (CP2C.p[11].sw.h)
#define L33 (CP2C.p[12].sw.l)
#define RBK (CP2C.p[13].sd)
#define GBK (CP2C.p[14].sd)
#define BBK (CP2C.p[15].sd)
#define LR1 (CP2C.p[16].sw.l)
#define LR2 (CP2C.p[16].sw.h)
#define LR3 (CP2C.p[17].sw.l)
#define LG1 (CP2C.p[17].sw.h)
#define LG2 (CP2C.p[18].sw.l)
#define LG3 (CP2C.p[18].sw.h)
#define LB1 (CP2C.p[19].sw.l)
#define LB2 (CP2C.p[19].sw.h)
#define LB3 (CP2C.p[20].sw.l)
#define RFC (CP2C.p[21].sd)
#define GFC (CP2C.p[22].sd)
#define BFC (CP2C.p[23].sd)
#define OFX (CP2C.p[24].sd)
#define OFY (CP2C.p[25].sd)
#define H (CP2C.p[26].sw.l)
#define DQA (CP2C.p[27].sw.l)
#define DQB (CP2C.p[28].sd)
#define ZSF3 (CP2C.p[29].sw.l)
#define ZSF4 (CP2C.p[30].sw.l)
#define FLAG (CP2C.p[31].d)

#define VX(n) (n < 3 ? CP2D.p[n << 1].sw.l : IR1)
#define VY(n) (n < 3 ? CP2D.p[n << 1].sw.h : IR2)
#define VZ(n) (n < 3 ? CP2D.p[(n << 1) + 1].sw.l : IR3)
#define MX11(n) (n < 3 ? CP2C.p[(n << 3)].sw.l : -R << 4)
#define MX12(n) (n < 3 ? CP2C.p[(n << 3)].sw.h : R << 4)
#define MX13(n) (n < 3 ? CP2C.p[(n << 3) + 1].sw.l : IR0)
#define MX21(n) (n < 3 ? CP2C.p[(n << 3) + 1].sw.h : R13)
#define MX22(n) (n < 3 ? CP2C.p[(n << 3) + 2].sw.l : R13)
#define MX23(n) (n < 3 ? CP2C.p[(n << 3) + 2].sw.h : R13)
#define MX31(n) (n < 3 ? CP2C.p[(n << 3) + 3].sw.l : R22)
#define MX32(n) (n < 3 ? CP2C.p[(n << 3) + 3].sw.h : R22)
#define MX33(n) (n < 3 ? CP2C.p[(n << 3) + 4].sw.l : R22)
#define CV1(n) (n < 3 ? CP2C.p[(n << 3) + 5].sd : 0)
#define CV2(n) (n < 3 ? CP2C.p[(n << 3) + 6].sd : 0)
#define CV3(n) (n < 3 ? CP2C.p[(n << 3) + 7].sd : 0)


#define U64(val) val##ULL
#define S64(val) val##LL

int s_sf;
int64_t s_mac0;
int64_t s_mac3;


int normalizeGTERegIndex(int reg)
{
	if (reg >= 32)
	{
		reg >>= 8;
		reg /= 0x8;
	}
	return reg;
}

void setCopControlWord(int cop, int reg, u32 value)
{
	reg = normalizeGTERegIndex(reg);

	switch (reg) {
	case 4:
	case 12:
	case 20:
	case 26:
	case 27:
	case 29:
	case 30:
		value = (int32_t)(int16_t)value;
		break;

	case 31:
		value = value & 0x7ffff000;
		if ((value & 0x7f87e000) != 0) value |= 0x80000000;
		break;
	}

	CP2C.p[reg].d = value;
}

void setCopControlWord(int cop, int reg, sVec2_s16 value)
{
	setCopControlWord(cop, reg, value.asS32());
}

static uint32_t gte_leadingzerocount(uint32_t lzcs) {
	uint32_t lzcr = 0;

	if ((lzcs & 0x80000000) == 0) lzcs = ~lzcs;

	while ((lzcs & 0x80000000) != 0) {
		lzcr++;
		lzcs <<= 1;
	}

	return lzcr;
}

void setCopReg(int cop, int reg, u32 value)
{
	reg = normalizeGTERegIndex(reg);

	switch (reg) {
	case 15:
		SXY0 = SXY1;
		SXY1 = SXY2;
		SXY2 = value;
		break;

	case 28:
		IR1 = (value & 0x1f) << 7;
		IR2 = (value & 0x3e0) << 2;
		IR3 = (value & 0x7c00) >> 3;
		break;

	case 30:
		LZCR = gte_leadingzerocount(value);
		break;

	case 31:
		return;
	}
	CP2D.p[reg].d = value;
}

void setCopReg(int cop, int reg, sVec2_s16 value)
{
	setCopReg(cop, reg, value.asS32());
}

s32 getCopControlWord(int cop, int reg)
{
	return CP2C.p[normalizeGTERegIndex(reg)].sd;
}

static int32_t LIM(int32_t value, int32_t max, int32_t min, uint32_t flag) {
	if (value > max) {
		FLAG |= flag;
		return max;
	}
	else if (value < min) {
		FLAG |= flag;
		return min;
	}

	return value;
}

s32 getCopReg(int cop, int reg)
{
	reg = normalizeGTERegIndex(reg);

	switch (reg) {
	case 1:
	case 3:
	case 5:
	case 8:
	case 9:
	case 10:
	case 11:
		CP2D.p[reg].d = (int32_t)CP2D.p[reg].sw.l;
		break;

	case 7:
	case 16:
	case 17:
	case 18:
	case 19:
		CP2D.p[reg].d = (uint32_t)CP2D.p[reg].w.l;
		break;

	case 15:
		CP2D.p[reg].d = SXY2;
		break;

	case 28:
	case 29:
		CP2D.p[reg].d = LIM(IR1 >> 7, 0x1f, 0, 0) | (LIM(IR2 >> 7, 0x1f, 0, 0) << 5) | (LIM(IR3 >> 7, 0x1f, 0, 0) << 10);
		break;
	}

	return CP2D.p[reg].sd;
}

static inline int64_t gte_shift(int64_t a, int sf) {
	if (sf > 0)
		return a >> 12;
	else if (sf < 0)
		return a << 12;

	return a;
}

int32_t BOUNDS(/*int44*/ int64_t value, int max_flag, int min_flag) {
	if (value /*.positive_overflow()*/ > S64(0x7ffffffffff)) FLAG |= max_flag;

	if (value /*.negative_overflow()*/ < S64(-0x80000000000)) FLAG |= min_flag;

	return gte_shift(value /*.value()*/, s_sf);
}

static uint32_t gte_divide(uint16_t numerator, uint16_t denominator) {
	if (numerator < (denominator * 2)) {
		static uint8_t table[] = {
			0xff, 0xfd, 0xfb, 0xf9, 0xf7, 0xf5, 0xf3, 0xf1, 0xef, 0xee, 0xec, 0xea, 0xe8, 0xe6, 0xe4, 0xe3, 0xe1, 0xdf,
			0xdd, 0xdc, 0xda, 0xd8, 0xd6, 0xd5, 0xd3, 0xd1, 0xd0, 0xce, 0xcd, 0xcb, 0xc9, 0xc8, 0xc6, 0xc5, 0xc3, 0xc1,
			0xc0, 0xbe, 0xbd, 0xbb, 0xba, 0xb8, 0xb7, 0xb5, 0xb4, 0xb2, 0xb1, 0xb0, 0xae, 0xad, 0xab, 0xaa, 0xa9, 0xa7,
			0xa6, 0xa4, 0xa3, 0xa2, 0xa0, 0x9f, 0x9e, 0x9c, 0x9b, 0x9a, 0x99, 0x97, 0x96, 0x95, 0x94, 0x92, 0x91, 0x90,
			0x8f, 0x8d, 0x8c, 0x8b, 0x8a, 0x89, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81, 0x7f, 0x7e, 0x7d, 0x7c, 0x7b,
			0x7a, 0x79, 0x78, 0x77, 0x75, 0x74, 0x73, 0x72, 0x71, 0x70, 0x6f, 0x6e, 0x6d, 0x6c, 0x6b, 0x6a, 0x69, 0x68,
			0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61, 0x60, 0x5f, 0x5e, 0x5d, 0x5d, 0x5c, 0x5b, 0x5a, 0x59, 0x58, 0x57,
			0x56, 0x55, 0x54, 0x53, 0x53, 0x52, 0x51, 0x50, 0x4f, 0x4e, 0x4d, 0x4d, 0x4c, 0x4b, 0x4a, 0x49, 0x48, 0x48,
			0x47, 0x46, 0x45, 0x44, 0x43, 0x43, 0x42, 0x41, 0x40, 0x3f, 0x3f, 0x3e, 0x3d, 0x3c, 0x3c, 0x3b, 0x3a, 0x39,
			0x39, 0x38, 0x37, 0x36, 0x36, 0x35, 0x34, 0x33, 0x33, 0x32, 0x31, 0x31, 0x30, 0x2f, 0x2e, 0x2e, 0x2d, 0x2c,
			0x2c, 0x2b, 0x2a, 0x2a, 0x29, 0x28, 0x28, 0x27, 0x26, 0x26, 0x25, 0x24, 0x24, 0x23, 0x22, 0x22, 0x21, 0x20,
			0x20, 0x1f, 0x1e, 0x1e, 0x1d, 0x1d, 0x1c, 0x1b, 0x1b, 0x1a, 0x19, 0x19, 0x18, 0x18, 0x17, 0x16, 0x16, 0x15,
			0x15, 0x14, 0x14, 0x13, 0x12, 0x12, 0x11, 0x11, 0x10, 0x0f, 0x0f, 0x0e, 0x0e, 0x0d, 0x0d, 0x0c, 0x0c, 0x0b,
			0x0a, 0x0a, 0x09, 0x09, 0x08, 0x08, 0x07, 0x07, 0x06, 0x06, 0x05, 0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02,
			0x01, 0x01, 0x00, 0x00, 0x00 };

		int shift = gte_leadingzerocount(denominator) - 16;

		int r1 = (denominator << shift) & 0x7fff;
		int r2 = table[((r1 + 0x40) >> 7)] + 0x101;
		int r3 = ((0x80 - (r2 * (r1 + 0x8000))) >> 8) & 0x1ffff;
		uint32_t reciprocal = ((r2 * r3) + 0x80) >> 8;

		return (uint32_t)((((uint64_t)reciprocal * (numerator << shift)) + 0x8000) >> 16);
	}

	return 0xffffffff;
}


int32_t A1(/*int44*/ int64_t a) { return BOUNDS(a, (1 << 31) | (1 << 30), (1 << 31) | (1 << 27)); }
int32_t A2(/*int44*/ int64_t a) { return BOUNDS(a, (1 << 31) | (1 << 29), (1 << 31) | (1 << 26)); }
int32_t A3(/*int44*/ int64_t a) {
	s_mac3 = a;
	return BOUNDS(a, (1 << 31) | (1 << 28), (1 << 31) | (1 << 25));
}

static int32_t Lm_B1(int32_t a, int lm) { return LIM(a, 0x7fff, -0x8000 * !lm, (1 << 31) | (1 << 24)); }
static int32_t Lm_B2(int32_t a, int lm) { return LIM(a, 0x7fff, -0x8000 * !lm, (1 << 31) | (1 << 23)); }
static int32_t Lm_B3(int32_t a, int lm) { return LIM(a, 0x7fff, -0x8000 * !lm, (1 << 22)); }

static int32_t Lm_B3_sf(int64_t value, int sf, int lm) {
	int32_t value_sf = gte_shift(value, sf);
	int32_t value_12 = gte_shift(value, 1);
	int max = 0x7fff;
	int min = 0;
	if (lm == 0) min = -0x8000;

	if (value_12 < -0x8000 || value_12 > 0x7fff) FLAG |= (1 << 22);

	if (value_sf > max)
		return max;
	else if (value_sf < min)
		return min;

	return value_sf;
}

static int32_t Lm_C1(int32_t a) { return LIM(a, 0x00ff, 0x0000, (1 << 21)); }
static int32_t Lm_C2(int32_t a) { return LIM(a, 0x00ff, 0x0000, (1 << 20)); }
static int32_t Lm_C3(int32_t a) { return LIM(a, 0x00ff, 0x0000, (1 << 19)); }
static int32_t Lm_D(int64_t a, int sf) { return LIM(gte_shift(a, sf), 0xffff, 0x0000, (1 << 31) | (1 << 18)); }

static uint32_t Lm_E(uint32_t result) {
	if (result == 0xffffffff) {
		FLAG |= (1 << 31) | (1 << 17);
		return 0x1ffff;
	}

	if (result > 0x1ffff) return 0x1ffff;

	return result;
}

int64_t F(int64_t a) {
	s_mac0 = a;

	if (a > S64(0x7fffffff)) FLAG |= (1 << 31) | (1 << 16);

	if (a < S64(-0x80000000)) FLAG |= (1 << 31) | (1 << 15);

	return a;
}

static int32_t Lm_G1(int64_t a) {
	if (a > 0x3ff) {
		FLAG |= (1 << 31) | (1 << 14);
		return 0x3ff;
	}
	if (a < -0x400) {
		FLAG |= (1 << 31) | (1 << 14);
		return -0x400;
	}

	return a;
}

static int32_t Lm_G2(int64_t a) {
	if (a > 0x3ff) {
		FLAG |= (1 << 31) | (1 << 13);
		return 0x3ff;
	}

	if (a < -0x400) {
		FLAG |= (1 << 31) | (1 << 13);
		return -0x400;
	}

	return a;
}

static int32_t Lm_G1_ia(int64_t a) {
	if (a > 0x3ffffff) return 0x3ffffff;

	if (a < -0x4000000) return -0x4000000;

	return a;
}

static int32_t Lm_G2_ia(int64_t a) {
	if (a > 0x3ffffff) return 0x3ffffff;

	if (a < -0x4000000) return -0x4000000;

	return a;
}

static int32_t Lm_H(int64_t value, int sf) {
	int64_t value_sf = gte_shift(value, sf);
	int32_t value_12 = gte_shift(value, 1);
	int max = 0x1000;
	int min = 0x0000;

	if (value_sf < min || value_sf > max) FLAG |= (1 << 12);

	if (value_12 > max) return max;

	if (value_12 < min) return min;

	return value_12;
}

bool gWidescreen = false;

void RTPS(u32 function)
{
	int lm = GTE_LM(function);
	int32_t h_over_sz3 = 0;

	MAC1 = A1(/*int44*/ (int64_t)((int64_t)TRX << 12) + (R11 * VX0) + (R12 * VY0) + (R13 * VZ0));
	MAC2 = A2(/*int44*/ (int64_t)((int64_t)TRY << 12) + (R21 * VX0) + (R22 * VY0) + (R23 * VZ0));
	MAC3 = A3(/*int44*/ (int64_t)((int64_t)TRZ << 12) + (R31 * VX0) + (R32 * VY0) + (R33 * VZ0));
	IR1 = Lm_B1(MAC1, lm);
	IR2 = Lm_B2(MAC2, lm);
	IR3 = Lm_B3_sf(s_mac3, s_sf, lm);
	SZ0 = SZ1;
	SZ1 = SZ2;
	SZ2 = SZ3;
	SZ3 = Lm_D(s_mac3, 1);
	h_over_sz3 = Lm_E(gte_divide(H, SZ3));
	SXY0 = SXY1;
	SXY1 = SXY2;
	SX2 = Lm_G1(
		F((int64_t)OFX + ((int64_t)IR1 * h_over_sz3) * (gWidescreen ? 0.75 : 1)) >> 16);
	SY2 = Lm_G2(F((int64_t)OFY + ((int64_t)IR2 * h_over_sz3)) >> 16);

#if 0
	PGXP_pushSXYZ2s(Lm_G1_ia((int64_t)OFX +
		(int64_t)(IR1 * h_over_sz3) * (PCSX::g_emulator->config().Widescreen ? 0.75 : 1)),
		Lm_G2_ia((int64_t)OFY + (int64_t)(IR2 * h_over_sz3)), std::max((int)SZ3, H / 2), SXY2);
#endif
	// PGXP_RTPS(0, SXY2);

	MAC0 = F((int64_t)DQB + ((int64_t)DQA * h_over_sz3));
	IR0 = Lm_H(s_mac0, 1);
}

void RTPT(u32 function)
{
	int lm = GTE_LM(function);
	int32_t h_over_sz3 = 0;

	for (int v = 0; v < 3; v++) {
		MAC1 = A1(/*int44*/ (int64_t)((int64_t)TRX << 12) + (R11 * VX(v)) + (R12 * VY(v)) + (R13 * VZ(v)));
		MAC2 = A2(/*int44*/ (int64_t)((int64_t)TRY << 12) + (R21 * VX(v)) + (R22 * VY(v)) + (R23 * VZ(v)));
		MAC3 = A3(/*int44*/ (int64_t)((int64_t)TRZ << 12) + (R31 * VX(v)) + (R32 * VY(v)) + (R33 * VZ(v)));
		IR1 = Lm_B1(MAC1, lm);
		IR2 = Lm_B2(MAC2, lm);
		IR3 = Lm_B3_sf(s_mac3, s_sf, lm);
		SZ0 = SZ1;
		SZ1 = SZ2;
		SZ2 = SZ3;
		SZ3 = Lm_D(s_mac3, 1);
		h_over_sz3 = Lm_E(gte_divide(H, SZ3));
		SXY0 = SXY1;
		SXY1 = SXY2;
		SX2 = Lm_G1(F((int64_t)OFX +
			((int64_t)IR1 * h_over_sz3) * (gWidescreen ? 0.75 : 1)) >>
			16);
		SY2 = Lm_G2(F((int64_t)OFY + ((int64_t)IR2 * h_over_sz3)) >> 16);

		// float tempMx = MAC1;
		// float tempx = IR1;
		// float temphow = (float)h_over_sz3 / (float)(1 << 16);

		// float tempMz = MAC3;
		// float tempZ = SZ3;
		//
		/*
		PGXP_pushSXYZ2s(Lm_G1_ia((int64_t)OFX + (int64_t)(IR1 * h_over_sz3) *
			(PCSX::g_emulator->config().Widescreen ? 0.75 : 1)),
			Lm_G2_ia((int64_t)OFY + (int64_t)(IR2 * h_over_sz3)), std::max((int)SZ3, H / 2), SXY2);
			*/
		// PGXP_RTPS(v, SXY2);
	}

	MAC0 = F((int64_t)DQB + ((int64_t)DQA * h_over_sz3));
	IR0 = Lm_H(s_mac0, 1);
}

void MVMVA(u32 function)
{
	int mx = GTE_MX(function);
	int v = GTE_V(function);
	int cv = GTE_CV(function);
	int lm = GTE_LM(function);

	switch (cv) {
	case 2:
		MAC1 = A1((int64_t)(MX12(mx) * VY(v)) + (MX13(mx) * VZ(v)));
		MAC2 = A2((int64_t)(MX22(mx) * VY(v)) + (MX23(mx) * VZ(v)));
		MAC3 = A3((int64_t)(MX32(mx) * VY(v)) + (MX33(mx) * VZ(v)));
		Lm_B1(A1(((int64_t)CV1(cv) << 12) + (MX11(mx) * VX(v))), 0);
		Lm_B2(A2(((int64_t)CV2(cv) << 12) + (MX21(mx) * VX(v))), 0);
		Lm_B3(A3(((int64_t)CV3(cv) << 12) + (MX31(mx) * VX(v))), 0);
		break;

	default:
		MAC1 = A1(/*int44*/ (int64_t)((int64_t)CV1(cv) << 12) + (MX11(mx) * VX(v)) + (MX12(mx) * VY(v)) + (MX13(mx) * VZ(v)));
		MAC2 = A2(/*int44*/ (int64_t)((int64_t)CV2(cv) << 12) + (MX21(mx) * VX(v)) + (MX22(mx) * VY(v)) + (MX23(mx) * VZ(v)));
		MAC3 = A3(/*int44*/ (int64_t)((int64_t)CV3(cv) << 12) + (MX31(mx) * VX(v)) + (MX32(mx) * VY(v)) + (MX33(mx) * VZ(v)));
		break;
	}

	IR1 = Lm_B1(MAC1, lm);
	IR2 = Lm_B2(MAC2, lm);
	IR3 = Lm_B3(MAC3, lm);

}

void OP(u32 function)
{
	int lm = GTE_LM(function);

	MAC1 = A1((int64_t)(R22 * IR3) - (R33 * IR2));
	MAC2 = A2((int64_t)(R33 * IR1) - (R11 * IR3));
	MAC3 = A3((int64_t)(R11 * IR2) - (R22 * IR1));
	IR1 = Lm_B1(MAC1, lm);
	IR2 = Lm_B2(MAC2, lm);
	IR3 = Lm_B3(MAC3, lm);
}

void AVSZ4(u32 function)
{
	MAC0 = F((int64_t)(ZSF4 * SZ0) + (ZSF4 * SZ1) + (ZSF4 * SZ2) + (ZSF4 * SZ3));
	OTZ = Lm_D(s_mac0, 1);
}

void SQR(u32 function)
{
	int lm = GTE_LM(function);
	MAC1 = A1(IR1 * IR1);
	MAC2 = A2(IR2 * IR2);
	MAC3 = A3(IR3 * IR3);
	IR1 = Lm_B1(MAC1, lm);
	IR2 = Lm_B2(MAC2, lm);
	IR3 = Lm_B3(MAC3, lm);
}

float PGXP_NCLIP() {
	float nclip = ((SX0 * SY1) + (SX1 * SY2) + (SX2 * SY0) - (SX0 * SY2) - (SX1 * SY0) - (SX2 * SY1));

	// ensure fractional values are not incorrectly rounded to 0
	float nclipAbs = fabs(nclip);
	if ((0.1f < nclipAbs) && (nclipAbs < 1.f)) nclip += (nclip < 0.f ? -1 : 1);

	// float AX = SX1 - SX0;
	// float AY = SY1 - SY0;

	// float BX = SX2 - SX0;
	// float BY = SY2 - SY0;

	//// normalise A and B
	// float mA = sqrt((AX*AX) + (AY*AY));
	// float mB = sqrt((BX*BX) + (BY*BY));

	//// calculate AxB to get Z component of C
	// float CZ = ((AX * BY) - (AY * BX)) * (1 << 12);

	return nclip;
}

void NCLIP(u32 function)
{
	/*if (PGXP_NLCIP_valid(SXY0, SXY1, SXY2))
		MAC0 = F(PGXP_NCLIP());
	else*/
		MAC0 = F((int64_t)(SX0 * SY1) + (SX1 * SY2) + (SX2 * SY0) - (SX0 * SY2) - (SX1 * SY0) - (SX2 * SY1));
}


void GPF(u32 function)
{
	int lm = GTE_LM(function);
	MAC1 = A1(IR0 * IR1);
	MAC2 = A2(IR0 * IR2);
	MAC3 = A3(IR0 * IR3);
	IR1 = Lm_B1(MAC1, lm);
	IR2 = Lm_B2(MAC2, lm);
	IR3 = Lm_B3(MAC3, lm);
	RGB0 = RGB1;
	RGB1 = RGB2;
	CD2 = CODE;
	R2 = Lm_C1(MAC1 >> 4);
	G2 = Lm_C2(MAC2 >> 4);
	B2 = Lm_C3(MAC3 >> 4);
}

void copFunction(int cop, u32 function)
{
	int getFunctionCode = GTE_FUNCT(function);

	s_sf = GTE_SF(function);
	FLAG = 0;

	switch (getFunctionCode)
	{
	case 0x1:
		RTPS(function);
		return;
	case 0x6:
		NCLIP(function);
		return;
	case 0xC:
		OP(function);
		return;
	case 0x12:
		MVMVA(function);
		return;
	case 0x28:
		SQR(function);
		return;
	case 0x2E:
		AVSZ4(function);
		return;
	case 0x30:
		RTPT(function);
		return;
	case 0x3D:
		GPF(function);
		return;
	default:
		assert(0);
	}
}

void gte_rtpt()
{
	copFunction(2, 0x280030);
}

void gte_op0() {
    copFunction(2, 0x0170000C);
}

void gte_rt()
{
    copFunction(2, 0x0480012);
}

void gte_nclip()
{
	copFunction(2, 0x1400006);
}

void gte_rtps()
{
	copFunction(2, 0x180001);
}

void gte_rtv0()
{
	copFunction(2, 0x486012);
}

void gte_rtir()
{
	copFunction(2, 0x49E012);
}

void gte_rtir_sf0()
{
    copFunction(2, 0x41E012);
}

void gte_ldlv0(const VECTOR* pVertices0)
{
    setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
    setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(pVertices0->vz, 0));
}

void gte_ldv0(const SFP_VEC4* pVertices0)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(pVertices0->vz, 0));
}

void gte_ldv2(SVECTOR* pVertices0) {
    setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
    setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(pVertices0->vz, 0));
}

void gte_ldv0(u8* pVertices)
{
    setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(READ_LE_S16(pVertices), READ_LE_S16(pVertices + 2)));
    setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(READ_LE_S16(pVertices + 4), 0));
}

void gte_ldVXY0(struct SFP_VEC4* pInput) {
    setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pInput->vx, pInput->vy));
}

void gte_ldVZ0(s16* pInput) {
    setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(*pInput, 0));
}

void gte_ldVXY1(struct SFP_VEC4* pInput) {
    setCopReg(2, COP2D_XY1, sVec2_s16::fromValue(pInput->vx, pInput->vy));
}

void gte_ldVZ1(s16* pInput) {
    setCopReg(2, COP2D_Z1, sVec2_s16::fromValue(*pInput, 0));
}

void gte_ldVXY2(struct SFP_VEC4* pInput) {
    setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(pInput->vx, pInput->vy));
}

void gte_ldVZ2(s16* pInput) {
    setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(*pInput, 0));
}

void gte_ldv0(std::vector<u8>::iterator& pVertices)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(READ_LE_S16(pVertices), READ_LE_S16(pVertices + 2)));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(READ_LE_S16(pVertices + 4), 0));
}

void gte_ldv3(std::vector<u8>::iterator& pVertices0, std::vector<u8>::iterator& pVertices1, std::vector<u8>::iterator& pVertices2)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(READ_LE_S16(pVertices0), READ_LE_S16(pVertices0 + 2)));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(READ_LE_S16(pVertices0 + 4), 0));

	setCopReg(2, COP2D_XY1, sVec2_s16::fromValue(READ_LE_S16(pVertices1), READ_LE_S16(pVertices1 + 2)));
	setCopReg(2, COP2D_Z1, sVec2_s16::fromValue(READ_LE_S16(pVertices1 + 4), 0));

	setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(READ_LE_S16(pVertices2), READ_LE_S16(pVertices2 + 2)));
	setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(READ_LE_S16(pVertices2 + 4), 0));
}

void gte_ldv3(u8* pVertices0, u8* pVertices1, u8* pVertices2)
{
    setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(READ_LE_S16(pVertices0), READ_LE_S16(pVertices0 + 2)));
    setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(READ_LE_S16(pVertices0 + 4), 0));

    setCopReg(2, COP2D_XY1, sVec2_s16::fromValue(READ_LE_S16(pVertices1), READ_LE_S16(pVertices1 + 2)));
    setCopReg(2, COP2D_Z1, sVec2_s16::fromValue(READ_LE_S16(pVertices1 + 4), 0));

    setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(READ_LE_S16(pVertices2), READ_LE_S16(pVertices2 + 2)));
    setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(READ_LE_S16(pVertices2 + 4), 0));
}

void gte_ldv3(const SFP_VEC4* pVertices0, const SFP_VEC4* pVertices1, const SFP_VEC4* pVertices2)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(pVertices0->vz, 0));

	setCopReg(2, COP2D_XY1, sVec2_s16::fromValue(pVertices1->vx, pVertices1->vy));
	setCopReg(2, COP2D_Z1, sVec2_s16::fromValue(pVertices1->vz, 0));

	setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(pVertices2->vx, pVertices2->vy));
	setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(pVertices2->vz, 0));
}

void gte_ldlvl(VECTOR* pInput) {
    setCopReg(2, 9, sVec2_s16::fromValue(pInput->vx, pInput->vy));
    setCopReg(2, 10, sVec2_s16::fromValue(pInput->vz, 0));
}

void gte_stlvnl(VECTOR* pOutput)
{
    pOutput->vx = getCopReg(2, 0xC800);
    pOutput->vy = getCopReg(2, 0xD000);
    pOutput->vz = getCopReg(2, 0xD800);
}

void read_sz_fifo3(short* z0, short* z1, short* z2) {
    *z0 = getCopReg(2, 0x800 * 17);
    *z1 = getCopReg(2, 0x800 * 18);
    *z2 = getCopReg(2, 0x800 * 19);
}

s32 gte_stFLAG()
{
    return getCopControlWord(2, 0x800 * 31);
}

s32 gte_stMAC0()
{
    return getCopReg(2, 0x800 * 24);
}

s32 gte_stIR0()
{
    return getCopReg(2, 0x800 * 8);
}

s32 gte_stSXY0()
{
    return getCopReg(2, 0x800 * 12);
}

s32 gte_stSXY1()
{
    return getCopReg(2, 0x800 * 13);
}

s32 gte_stSXY2()
{
    return getCopReg(2, 0x800 * 14);
}


void gte_stlzc(int* pOutput)
{
	*pOutput = getCopReg(2, 0x800 * 31);
}

void gte_getMAC0(int* pOutput)
{
	*pOutput = getCopReg(2, 0x800 * 24);
}

void gte_stsxy3(sVec2_s16* xy0, sVec2_s16* xy1, sVec2_s16* xy2)
{
	*xy0 = sVec2_s16::fromS32(getCopReg(2, 0x6000));
	*xy1 = sVec2_s16::fromS32(getCopReg(2, 0x6800));
	*xy2 = sVec2_s16::fromS32(getCopReg(2, 0x7000));
}

void gte_stsxy2(sVec2_s16* xy)
{
	*xy = sVec2_s16::fromS32(getCopReg(2, 0x7000));
}

void gte_stsxy(sVec2_s16* xy)
{
    *xy = sVec2_s16::fromS32(getCopReg(2, 0x7000));
}


void gte_stsz3(int* sz0, int* sz1, int* sz2)
{
	*sz0 = getCopReg(2, 0x8800);
	*sz1 = getCopReg(2, 0x9000);
	*sz2 = getCopReg(2, 0x9800);
}

void gte_stsz(s32* output) {
    *output = getCopReg(2, 0x9800);
}

void gte_stsz4(int* sz0, int* sz1, int* sz2, int* sz3)
{
	*sz0 = getCopReg(2, 0x8000);
	*sz1 = getCopReg(2, 0x8800);
	*sz2 = getCopReg(2, 0x9000);
	*sz3 = getCopReg(2, 0x9800);
}

void gte_stdp(s32* output) {
    *output = getCopReg(2, 8 * 0x800);
}

s32 gte_stSZ3() {
    return getCopReg(2, 0x9800);
}


void gte_SetRotMatrix(const MATRIX* m) {
    setCopControlWord(2, 0, sVec2_s16::fromValue(m->m[0][0], m->m[0][1]));
    setCopControlWord(2, 0x800, sVec2_s16::fromValue(m->m[0][2], m->m[1][0]));
    setCopControlWord(2, 0x1000, sVec2_s16::fromValue(m->m[1][1], m->m[1][2]));
    setCopControlWord(2, 0x1800, sVec2_s16::fromValue(m->m[2][0], m->m[2][1]));
    setCopControlWord(2, 0x2000, sVec2_s16::fromValue(m->m[2][2], 0));
}

void gte_SetTransMatrix(const MATRIX* m) {
    setCopControlWord(2, 0x2800, m->t[0]);
    setCopControlWord(2, 0x3000, m->t[1]);
    setCopControlWord(2, 0x3800, m->t[2]);
}

void gte_ldDQA(s32 value) {
    setCopControlWord(2, 27 * 0x800, value);
}

void gte_ldDQB(s32 value) {
    setCopControlWord(2, 28 * 0x800, value);
}

void SetDQA(s32 value) {
    gte_ldDQA(value);
}

void SetDQB(s32 value) {
    gte_ldDQB(value);
}
