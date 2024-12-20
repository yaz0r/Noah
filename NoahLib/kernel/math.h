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
	FP_VEC4 t;
};

typedef MATRIX sMatrix;

MATRIX* RotMatrixX(long r, MATRIX* m);
MATRIX* RotMatrixY(long r, MATRIX* m);
MATRIX* RotMatrixZ(long r, MATRIX* m);

MATRIX* SetMulMatrix(MATRIX* m0, MATRIX* m1);
void createRotationMatrix(SFP_VEC4*, MATRIX*);
MATRIX* ScaleMatrixL(MATRIX* m, FP_VEC4* v);
MATRIX* ScaleMatrix(MATRIX* m, FP_VEC4* v);
MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2);
void SetRotMatrix(const MATRIX* m);
void SetTransMatrix(const MATRIX* m);
long RotTransPers(SFP_VEC4* $2, sVec2_s16* sxy, long* p, long* flag);
long RotTransPers4(SFP_VEC4* v0, SFP_VEC4* v1, SFP_VEC4* v2, SFP_VEC4* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag);
void resetMatrixTranslation(MATRIX* m);
MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1);
void RotTrans(SFP_VEC4* rotation, FP_VEC4* output, long* flag);
void copyRotationMatrix(MATRIX* param_1, MATRIX* param_2);
FP_VEC4* ApplyMatrix(MATRIX* m, SFP_VEC4* inputVector, FP_VEC4* outputVector);

void Square0(VECTOR* v0, VECTOR* v1);
s32 SquareRoot0(s32 value);

FP_VEC4* ApplyMatrixLV(MATRIX* m, FP_VEC4* v0, FP_VEC4* v1);

void gte_MulMatrix0(const MATRIX* m0, const MATRIX* m1, MATRIX* m2);

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
    COP2D_IR0 = 8,
    COP2D_IR1 = 9,
    COP2D_IR2 = 10,
    COP2D_IR3 = 11,
};

s32 length2d(s32 param_1, s32 param_2);
s32 length1d(s32 param_1);
void lookAtDivided(MATRIX* pOutputMatrix, FP_VEC4* param_2, FP_VEC4* param_3, FP_VEC4* param_4);
void lookAtNoDivide(MATRIX* param_1, SVECTOR* param_2, SVECTOR* param_3, SVECTOR* param_4);
long RotAverage4(SFP_VEC4* v0, SFP_VEC4* v1, SFP_VEC4* v2, SFP_VEC4* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag);
void setIdentityMatrix(MATRIX* param_1);
int VectorNormal(FP_VEC4* input, FP_VEC4* output);
int VectorNormalS(FP_VEC4* input, SFP_VEC4* output);
void OuterProduct12(FP_VEC4* a, FP_VEC4* b, FP_VEC4* r);
void OuterProduct0(VECTOR* $2, VECTOR* $3, VECTOR* v2);
long NCLIP(sVec2_s16 sxy0, sVec2_s16 sxy1, sVec2_s16 sxy2);
void copyMatrix(MATRIX* param_1, MATRIX* param_2);
MATRIX* TransMatrix(MATRIX* m, FP_VEC4* v);
MATRIX* RotMatrixZYX(SVECTOR* r, MATRIX* m);
MATRIX* RotMatrixYXZ(SFP_VEC4* r, MATRIX* m);
VECTOR* ApplyRotMatrixLV(VECTOR* $2, VECTOR* $3);
VECTOR* ApplyRotMatrix(SVECTOR* $2, VECTOR* $3);
void setupVector(VECTOR* param_1, long param_2, long param_3, long param_4);
void setupSVector(SVECTOR* param_1, short param_2, short param_3, short param_4);

s32  RotTransPers3(const SVECTOR* v0, const SVECTOR* v1, const SVECTOR* v2, sVec2_s16* p0, sVec2_s16* p1, sVec2_s16* p2, s32* p, s32* flag);