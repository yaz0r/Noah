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
	s32 vx;
	s32 vy;
};

typedef MATRIX sMatrix;

void createRotationMatrix(SVECTOR*, MATRIX*);
MATRIX* ScaleMatrixL(MATRIX* m, VECTOR* v);
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v);
void MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);

