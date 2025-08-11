#pragma once

#include "psx/libgte.h"

typedef MATRIX sMatrix;

MATRIX* RotMatrixX(long r, MATRIX* m);
MATRIX* RotMatrixY(long r, MATRIX* m);
MATRIX* RotMatrixZ(long r, MATRIX* m);

MATRIX* SetMulMatrix(MATRIX* m0, MATRIX* m1);
void createRotationMatrix(SVECTOR*, MATRIX*);
MATRIX* ScaleMatrixL(MATRIX* m, VECTOR* v);
MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v);
MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2);
MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2);
void SetRotMatrix(const MATRIX* m);
void SetTransMatrix(const MATRIX* m);
long RotTransPers(SVECTOR* $2, sVec2_s16* sxy, long* p, long* flag);
long RotTransPers4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag);
void resetMatrixTranslation(MATRIX* m);
MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1);
void RotTrans(SVECTOR* rotation, VECTOR* output, long* flag);
void copyRotationMatrix(MATRIX* param_1, MATRIX* param_2);
VECTOR* ApplyMatrix(MATRIX* m, SVECTOR* inputVector, VECTOR* outputVector);

void GTE_LoadMatrixCoumnToV0(const MATRIX* m1, int column);
void gte_CompMatrix(MATRIX* r1, MATRIX* r2, MATRIX* r3);

void Square0(VECTOR* v0, VECTOR* v1);
s32 SquareRoot0(s32 value);

VECTOR* ApplyMatrixLV(MATRIX* m, VECTOR* v0, VECTOR* v1);

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
void lookAtDivided(MATRIX* pOutputMatrix, VECTOR* param_2, VECTOR* param_3, VECTOR* param_4);
void lookAtNoDivide(MATRIX* param_1, SVECTOR* param_2, SVECTOR* param_3, SVECTOR* param_4);
long RotAverage4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag);
void setIdentityMatrix(MATRIX* param_1);
int VectorNormal(VECTOR* input, VECTOR* output);
int VectorNormalS(VECTOR* input, SVECTOR* output);
void OuterProduct12(VECTOR* a, VECTOR* b, VECTOR* r);
void OuterProduct0(VECTOR* $2, VECTOR* $3, VECTOR* v2);
long NCLIP(sVec2_s16 sxy0, sVec2_s16 sxy1, sVec2_s16 sxy2);
void copyMatrix(MATRIX* param_1, MATRIX* param_2);
MATRIX* TransMatrix(MATRIX* m, VECTOR* v);
MATRIX* RotMatrixZYX(SVECTOR* r, MATRIX* m);
MATRIX* RotMatrixYXZ(SVECTOR* r, MATRIX* m);
VECTOR* ApplyRotMatrixLV(VECTOR* $2, VECTOR* $3);
VECTOR* ApplyRotMatrix(SVECTOR* $2, VECTOR* $3);
void setupVector(VECTOR* param_1, long param_2, long param_3, long param_4);
void setupSVector(SVECTOR* param_1, short param_2, short param_3, short param_4);

s32  RotTransPers3(const SVECTOR* v0, const SVECTOR* v1, const SVECTOR* v2, sVec2_s16* p0, sVec2_s16* p1, sVec2_s16* p2, s32* p, s32* flag);