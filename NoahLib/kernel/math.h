#pragma once

struct sFP1616
{
	u32 value;

	static sFP1616 fromValue(s16 x, s16 y)
	{
		sFP1616 newValue;
		newValue.set(x, y);
		return newValue;
	}

	void set(s16 x, s16 y)
	{
		value = ((u16)x) * 0x10000 + (u16)y;
	}

	void get(s16& x, s16& y)
	{
		x = (s16)(value >> 16);
		y = (s16)(value & 0xFFFF);
	}
};

struct MATRIX
{
	std::array<std::array<s16, 3>, 3> m;
	std::array<s32, 3> t;
};

struct sVec3
{
	s32 vx;
	s32 vy;
	s32 vz;

	s32 operator[](size_t idx) const
	{
		switch (idx)
		{
		case 0:
			return vx;
		case 1:
			return vy;
		case 2:
			return vz;
		default:
			assert(0);
			return 0;
		}
	}

	s32& operator[](size_t idx)
	{
		switch (idx)
		{
		case 0:
			return vx;
		case 1:
			return vy;
		case 2:
			return vz;
		default:
			assert(0);
			return vx;
		}
	}
};

struct sVec2_s16
{
	s16 vx;
	s16 vy;

	s32 asS32()
	{
		s32 temp = 0;
		temp = vy;
		temp <<= 16;
		temp | vx;
		return temp;
	}

	static sVec2_s16 fromS32(s32 input)
	{
		sVec2_s16 temp;
		temp.vx = input & 0xFFFF;
		input >>= 16;
		temp.vy = input & 0xFFFF;
		return temp;
	}
};

typedef MATRIX sMatrix;

void createRotationMatrix(SVECTOR*, MATRIX*);
MATRIX* ScaleMatrixL(MATRIX* m, VECTOR* v);
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v);
void MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2);
void SetRotMatrix(const MATRIX* m);
void SetTransMatrix(const MATRIX* m);
long RotTransPers(SVECTOR* $2, sVec2_s16* sxy, long* p, long* flag);
void resetMatrixTranslation(MATRIX* m);
MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1);
void RotTrans(SVECTOR* rotation, VECTOR* output, long* flag);
void setCopReg(int, int, sFP1616);
void copFunction(int, u32);
s32 getCopReg(int, int);

#define gte_rtpt() copFunction(2,0x280030)
#define gte_nclip() copFunction(2,0x1400006)
#define gte_rtps() copFunction(2,0x180001)


enum eCOP2D
{
	COP2D_XY0 = 0,
	COP2D_Z0 = 1,
	COP2D_XY1 = 2,
	COP2D_Z1 = 3,
	COP2D_XY2 = 4,
	COP2D_Z2 = 5,
};
