#pragma once

struct sVec2_s16
{
	s16 vx;
	s16 vy;

	void set(s16 x, s16 y)
	{
		vx = x;
		vy = y;
	}

	void get(s16& x, s16& y)
	{
		x = vx;
		y = vy;
	}

	static sVec2_s16 fromValue(s16 x, s16 y)
	{
		sVec2_s16 newValue;
		newValue.set(x, y);
		return newValue;
	}

	s32 asS32()
	{
		u32 temp = 0;
		temp = vy;
		temp <<= 16;
		temp |= ((u32)vx) & 0xFFFF;
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

typedef MATRIX sMatrix;

void createRotationMatrix(SVECTOR*, MATRIX*);
MATRIX* ScaleMatrixL(MATRIX* m, VECTOR* v);
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v);
MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2);
void SetRotMatrix(const MATRIX* m);
void SetTransMatrix(const MATRIX* m);
long RotTransPers(SVECTOR* $2, sVec2_s16* sxy, long* p, long* flag);
void resetMatrixTranslation(MATRIX* m);
MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1);
void RotTrans(SVECTOR* rotation, VECTOR* output, long* flag);

void PopMatrix();
void PushMatrix();

enum eCOP2D
{
	COP2D_XY0 = 0,
	COP2D_Z0 = 1,
	COP2D_XY1 = 2,
	COP2D_Z1 = 3,
	COP2D_XY2 = 4,
	COP2D_Z2 = 5,
};

s32 length2d(s32 param_1, s32 param_2);
s32 length1d(s32 param_1);
void computeMatrix(MATRIX* pOutputMatrix, VECTOR* param_2, VECTOR* param_3, VECTOR* param_4);
long RotAverage4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag);
void setIdentityMatrix(MATRIX* param_1);
int VectorNormal(VECTOR* v0, VECTOR* v1);
void OuterProduct12(VECTOR* a, VECTOR* b, VECTOR* r);
long NCLIP(sVec2_s16 sxy0, sVec2_s16 sxy1, sVec2_s16 sxy2);
void copyMatrix(MATRIX* param_1, MATRIX* param_2);
