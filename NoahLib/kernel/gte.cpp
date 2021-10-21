#include "noahLib.h"
#include "gte.h"

struct sGTEControlRegisters
{
	union
	{
		struct
		{
			sVec2_s16 r0_R11R12;
			sVec2_s16 r1_R13R21;
			sVec2_s16 r2_R22R23;
			sVec2_s16 r3_R31R32;
			s16 r4_R33;
			s16 r4_padding;
			s32 r5_TRX;
			s32 r6_TRY;
			s32 r7_TRZ;
			s32 r8;
			s32 r9;
			s32 r10;
			s32 r11;
			s32 r12;
			s32 r13;
			s32 r14;
			s32 r15;
			s32 r16;
			s32 r17;
			s32 r18;
			s32 r19;
			s32 r20;
			s32 r21;
			s32 r22;
			s32 r23;
			s32 r24_OFX;
			s32 r25_OFY;
			s16 r26_H;
			s16 r26_padding;
			s16 r27_DQA;
			s16 r27_padding;
			s32 r28_DQB;
			s32 r29;
			s16 r30_ZSF4;
			s16 r30_padding;
			u32 r31_FLAG;
		};

		s32 r_s32[32];
		sVec2_s16 r_s16[32];
	};

} GTEControlRegisters;


struct sGTEDataRegisters
{
	std::array<SVECTOR, 3> r0_VECTORS;
	std::array<u8, 4> r6;
	s16 r7_OTZ;
	std::array<s32, 4> r8_IR;
	sVec2_s16 r12_SXY0;
	sVec2_s16 r13_SXY1;
	sVec2_s16 r14_SXY2;
	sVec2_s16 r15_SXYP;
	std::array<s32, 4> r16_SZ;
	std::array<s32, 4> r24_MAC;
	s32 r31_LZCR;
} GTEDataRegisters;

int normalizeGTERegIndex(int reg)
{
	if (reg >= 32)
	{
		reg >>= 8;
		reg /= 0x8;
	}
	return reg;
}

void setCopControlWord(int cop, int reg, s32 value)
{
	switch (normalizeGTERegIndex(reg))
	{
	case 5:
		GTEControlRegisters.r5_TRX = value;
		break;
	case 6:
		GTEControlRegisters.r6_TRY = value;
		break;
	case 7:
		GTEControlRegisters.r7_TRZ = value;
		break;
	case 24:
		GTEControlRegisters.r24_OFX = value;
		break;
	case 25:
		GTEControlRegisters.r25_OFY = value;
		break;
	case 26:
		GTEControlRegisters.r26_H = value;
		break;
	default:
		assert(0);
	}
}

void setCopControlWord(int cop, int reg, sVec2_s16 value)
{
	switch (normalizeGTERegIndex(reg))
	{
	case 0:
		GTEControlRegisters.r0_R11R12 = value;
		break;
	case 1:
		GTEControlRegisters.r1_R13R21 = value;
		break;
	case 2:
		GTEControlRegisters.r2_R22R23 = value;
		break;
	case 3:
		GTEControlRegisters.r3_R31R32 = value;
		break;
	case 4:
		GTEControlRegisters.r4_R33 = value.vx;
		break;
	default:
		assert(0);
	}
}

void setCopReg(int cop, int reg, sVec2_s16 value)
{
	reg = normalizeGTERegIndex(reg);
	switch (reg)
	{
	case 0:
	case 2:
	case 4:
		GTEDataRegisters.r0_VECTORS[reg/2].vx = value.vx;
		GTEDataRegisters.r0_VECTORS[reg/2].vy = value.vy;
		break;
	case 1:
	case 3:
	case 5:
		GTEDataRegisters.r0_VECTORS[reg/2].vz = value.vx;
		break;
	default:
		assert(0);
	}

}

s32 getCopControlWord(int cop, int reg)
{
	switch (normalizeGTERegIndex(reg))
	{
	case 0:
		return GTEControlRegisters.r0_R11R12.asS32();
		break;
	case 1:
		return GTEControlRegisters.r1_R13R21.asS32();
		break;
	case 2:
		return GTEControlRegisters.r2_R22R23.asS32();
		break;
	case 3:
		return GTEControlRegisters.r3_R31R32.asS32();
		break;
	case 4:
		return GTEControlRegisters.r4_R33;
		break;
	case 5:
		return GTEControlRegisters.r5_TRX;
	case 6:
		return GTEControlRegisters.r6_TRY;
	case 7:
		return GTEControlRegisters.r7_TRZ;
	case 31:
		return GTEControlRegisters.r31_FLAG;
	default:
		assert(0);
		return 0;
	}
}

s32 getCopReg(int cop, int reg)
{
	switch (normalizeGTERegIndex(reg))
	{
	case 7:
		return GTEDataRegisters.r7_OTZ;
	case 8:
		return GTEDataRegisters.r8_IR[0];
	case 9:
		return GTEDataRegisters.r8_IR[1];
	case 10:
		return GTEDataRegisters.r8_IR[2];
	case 11:
		return GTEDataRegisters.r8_IR[3];
	case 12:
		return GTEDataRegisters.r12_SXY0.asS32();
	case 13:
		return GTEDataRegisters.r13_SXY1.asS32();
	case 14:
		return GTEDataRegisters.r14_SXY2.asS32();
	case 24:
		return GTEDataRegisters.r24_MAC[0];
	case 25:
		return GTEDataRegisters.r24_MAC[1];
	case 26:
		return GTEDataRegisters.r24_MAC[2];
	case 27:
		return GTEDataRegisters.r24_MAC[3];
	case 31:
		return GTEDataRegisters.r31_LZCR;
	default:
		assert(0);
	}
}

#define GTE_SF(op) ((op >> 19) & 1)
#define GTE_MX(op) ((op >> 17) & 3)
#define GTE_V(op) ((op >> 15) & 3)
#define GTE_CV(op) ((op >> 13) & 3)
#define GTE_LM(op) ((op >> 10) & 1)
#define GTE_FUNCT(op) (op & 63)

#define VX0 (GTEDataRegisters.r0_VECTORS[0].vx)
#define VY0 (GTEDataRegisters.r0_VECTORS[0].vy)
#define VZ0 (GTEDataRegisters.r0_VECTORS[0].vz)
#define VX1 (GTEDataRegisters.r0_VECTORS[1].vx)
#define VY1 (GTEDataRegisters.r0_VECTORS[1].vy)
#define VZ1 (GTEDataRegisters.r0_VECTORS[1].vz)
#define VX2 (GTEDataRegisters.r0_VECTORS[2].vx)
#define VY2 (GTEDataRegisters.r0_VECTORS[2].vy)
#define VZ2 (GTEDataRegisters.r0_VECTORS[2].vz)
#define R (GTEDataRegisters.r6[0])

#define OTZ (GTEDataRegisters.r7_OTZ)
#define IR0 (GTEDataRegisters.r8_IR[0])
#define IR1 (GTEDataRegisters.r8_IR[1])
#define IR2 (GTEDataRegisters.r8_IR[2])
#define IR3 (GTEDataRegisters.r8_IR[3])
#define SXY0 (GTEDataRegisters.r12_SXY0)
#define SXY1 (GTEDataRegisters.r13_SXY1)
#define SXY2 (GTEDataRegisters.r14_SXY2)
#define SX2 (GTEDataRegisters.r14_SXY2.vx)
#define SY2 (GTEDataRegisters.r14_SXY2.vy)
#define SZ0 (GTEDataRegisters.r16_SZ[0])
#define SZ1 (GTEDataRegisters.r16_SZ[1])
#define SZ2 (GTEDataRegisters.r16_SZ[2])
#define SZ3 (GTEDataRegisters.r16_SZ[3])
#define MAC0 (GTEDataRegisters.r24_MAC[0])
#define MAC1 (GTEDataRegisters.r24_MAC[1])
#define MAC2 (GTEDataRegisters.r24_MAC[2])
#define MAC3 (GTEDataRegisters.r24_MAC[3])

#define R11 (GTEControlRegisters.r0_R11R12.vx)
#define R12 (GTEControlRegisters.r0_R11R12.vy)
#define R13 (GTEControlRegisters.r1_R13R21.vx)
#define R21 (GTEControlRegisters.r1_R13R21.vy)
#define R22 (GTEControlRegisters.r2_R22R23.vx)
#define R23 (GTEControlRegisters.r2_R22R23.vy)
#define R31 (GTEControlRegisters.r3_R31R32.vx)
#define R32 (GTEControlRegisters.r3_R31R32.vy)
#define R33 (GTEControlRegisters.r4_R33)
#define TRX (GTEControlRegisters.r5_TRX)
#define TRY (GTEControlRegisters.r6_TRY)
#define TRZ (GTEControlRegisters.r7_TRZ)
#define OFX (GTEControlRegisters.r24_OFX)
#define OFY (GTEControlRegisters.r25_OFY)
#define H (GTEControlRegisters.r26_H)
#define DQA (GTEControlRegisters.r27_DQA)
#define DQB (GTEControlRegisters.r28_DQB)
#define ZSF4 (GTEControlRegisters.r30_ZSF4)
#define FLAG (GTEControlRegisters.r31_FLAG)

#define VX(n) (n < 3 ? GTEDataRegisters.r0_VECTORS[n].vx : IR1)
#define VY(n) (n < 3 ? GTEDataRegisters.r0_VECTORS[n].vy : IR2)
#define VZ(n) (n < 3 ? GTEDataRegisters.r0_VECTORS[n].vz : IR3)
#define MX11(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3)].vx : -R << 4)
#define MX12(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3)].vy : R << 4)
#define MX13(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 1].vx : IR0)
#define MX21(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 1].vy : R13)
#define MX22(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 2].vx : R13)
#define MX23(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 2].vy : R13)
#define MX31(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 3].vx : R22)
#define MX32(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 3].vy : R22)
#define MX33(n) (n < 3 ? GTEControlRegisters.r_s16[(n << 3) + 4].vx : R22)
#define CV1(n) (n < 3 ?  GTEControlRegisters.r_s32[(n << 3) + 5] : 0)
#define CV2(n) (n < 3 ?  GTEControlRegisters.r_s32[(n << 3) + 6] : 0)
#define CV3(n) (n < 3 ?  GTEControlRegisters.r_s32[(n << 3) + 7] : 0)


#define U64(val) val##ULL
#define S64(val) val##LL

int s_sf;
int64_t s_mac0;
int64_t s_mac3;

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

static uint32_t gte_leadingzerocount(uint32_t lzcs) {
	uint32_t lzcr = 0;

	if ((lzcs & 0x80000000) == 0) lzcs = ~lzcs;

	while ((lzcs & 0x80000000) != 0) {
		lzcr++;
		lzcs <<= 1;
	}

	return lzcr;
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

void AVSZ4(u32 function)
{
	MAC0 = F((int64_t)(ZSF4 * SZ0) + (ZSF4 * SZ1) + (ZSF4 * SZ2) + (ZSF4 * SZ3));
	OTZ = Lm_D(s_mac0, 1);
}

void copFunction(int cop, u32 function)
{
	int getFunctionCode = GTE_FUNCT(function);
	switch (getFunctionCode)
	{
	case 0x1:
		RTPS(function);
		return;
	/*case 0x6:
		gte_nclip();
		return;*/
	case 0x12:
		MVMVA(function);
		return;
	case 0x2E:
		AVSZ4(function);
		return;
	case 0x30:
		RTPT(function);
		return;
	default:
		assert(0);
	}
}

void gte_rtpt()
{
	copFunction(2, 0x280030);
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

void gte_ldv0(const SVECTOR* pVertices0)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(pVertices0->vz, 0));
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

void gte_ldv3(const SVECTOR* pVertices0, const SVECTOR* pVertices1, const SVECTOR* pVertices2)
{
	setCopReg(2, COP2D_XY0, sVec2_s16::fromValue(pVertices0->vx, pVertices0->vy));
	setCopReg(2, COP2D_Z0, sVec2_s16::fromValue(pVertices0->vz, 0));

	setCopReg(2, COP2D_XY1, sVec2_s16::fromValue(pVertices1->vx, pVertices1->vy));
	setCopReg(2, COP2D_Z1, sVec2_s16::fromValue(pVertices1->vz, 0));

	setCopReg(2, COP2D_XY2, sVec2_s16::fromValue(pVertices2->vx, pVertices2->vy));
	setCopReg(2, COP2D_Z2, sVec2_s16::fromValue(pVertices2->vz, 0));
}


void gte_stlzc(int* pOutput)
{
	*pOutput = getCopReg(2, 0xf800);
}

void gte_stopz(int* pOutput)
{
	*pOutput = getCopReg(2, 0xC000);
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

void gte_stsz4(int* sz0, int* sz1, int* sz2, int* sz3)
{
	*sz0 = getCopReg(2, 0x8000);
	*sz1 = getCopReg(2, 0x8800);
	*sz2 = getCopReg(2, 0x9000);
	*sz3 = getCopReg(2, 0x9800);
}


