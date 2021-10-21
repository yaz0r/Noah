#include "noahLib.h"
#include "math.h"
#include "trigo.h"
#include "gte.h"

void createRotationMatrix(SVECTOR* param_1, MATRIX* param_2)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    short sVar7;
    int iVar8;

    sVar7 = rcossin_tbl[(param_1->vx & 0xFFF) * 2];
    iVar4 = rcossin_tbl[(param_1->vx & 0xFFF) * 2 + 1];
    iVar8 = rcossin_tbl[(param_1->vz & 0xFFF) * 2];
    iVar5 = rcossin_tbl[(param_1->vz & 0xFFF) * 2 + 1];
    iVar2 = rcossin_tbl[(param_1->vy & 0xFFF) * 2];
    iVar3 = rcossin_tbl[(param_1->vy & 0xFFF) * 2 + 1];
    iVar1 = -(int)sVar7;
    param_2->m[0][0] = (short)(iVar5 * iVar4 >> 0xc);
    iVar6 = (int)(short)iVar2;
    iVar2 = iVar5 * iVar1 >> 0xc;
    param_2->m[1][0] = (short)(iVar8 * iVar3 >> 0xc) - (short)(iVar2 * iVar6 >> 0xc);
    param_2->m[2][0] = (short)(iVar2 * iVar3 >> 0xc) + (short)(iVar8 * iVar6 >> 0xc);
    param_2->m[0][1] = (short)(-(iVar8 * iVar4) >> 0xc);
    iVar1 = iVar8 * iVar1 >> 0xc;
    param_2->m[1][1] = (short)(iVar5 * iVar3 >> 0xc) + (short)(iVar1 * iVar6 >> 0xc);
    param_2->m[2][1] = (short)(iVar5 * iVar6 >> 0xc) - (short)(iVar1 * iVar3 >> 0xc);
    param_2->m[0][2] = sVar7;
    param_2->m[1][2] = (short)(-(iVar4 * iVar6) >> 0xc);
    param_2->m[2][2] = (short)(iVar4 * iVar3 >> 0xc);
}

MATRIX* ScaleMatrixL(MATRIX* m, VECTOR* v)
{
    m->m[0][0] = m->m[0][0] * v->vx >> 0xc;
    m->m[0][1] = m->m[0][1] * v->vx >> 0xc;
    m->m[0][2] = m->m[0][2] * v->vx >> 0xc;

    m->m[1][0] = m->m[1][0] * v->vy >> 0xc;
    m->m[1][1] = m->m[1][1] * v->vy >> 0xc;
    m->m[1][2] = m->m[1][2] * v->vy >> 0xc;

    m->m[2][0] = m->m[2][0] * v->vz >> 0xc;
    m->m[2][1] = m->m[2][1] * v->vz >> 0xc;
    m->m[2][2] = m->m[2][2] * v->vz >> 0xc;

    return m;
}

MATRIX* ScaleMatrix(MATRIX* m, VECTOR* v)
{
    m->m[0][0] = m->m[0][0] * v->vx >> 0xc;
    m->m[0][1] = m->m[0][1] * v->vx >> 0xc;
    m->m[0][2] = m->m[0][2] * v->vx >> 0xc;

    m->m[1][0] = m->m[1][0] * v->vy >> 0xc;
    m->m[1][1] = m->m[1][1] * v->vy >> 0xc;
    m->m[1][2] = m->m[1][2] * v->vy >> 0xc;

    m->m[2][0] = m->m[2][0] * v->vz >> 0xc;
    m->m[2][1] = m->m[2][1] * v->vz >> 0xc;
    m->m[2][2] = m->m[2][2] * v->vz >> 0xc;

    return m;
}

void MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j=0; j<3; j++)
        {
            m2->m[i][j] = m0->m[i][j] * m1->m[j][i] >> 0xC;
        }
    }
}

long RotTransPers(SVECTOR* v0, sVec2_s16* sxy, long* p, long* flag)
{
    MissingCode();
    return 0;
}

void resetMatrixTranslation(MATRIX* m)
{
	m->t[0] = 0;
	m->t[1] = 0;
	m->t[2] = 0;
}

MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	s32 Var2 = getCopReg(2, 0x4800);
	s32 Var3 = getCopReg(2, 0x5000);
	s32 Var4 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	s32 Var5 = getCopReg(2, 0x4800);
	s32 Var6 = getCopReg(2, 0x5000);
	s32 Var7 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);

	m1->m[0][0] = Var2;
	m1->m[0][1] = Var5;
	m1->m[2][0] = Var4;
	m1->m[2][1] = Var7;

	Var2 = getCopReg(2, 0x4800);
	Var5 = getCopReg(2, 0x5000);
	m1->m[0][2] = Var2;
	m1->m[1][0] = Var3;
	m1->m[1][1] = Var6;
	m1->m[1][2] = Var5;

	m1->m[2][2] = getCopReg(2, 0xb);

	return m1;
}

std::array<MATRIX, 20> matrixStack;
int currentMatrixStackOffset = 0;

void PushMatrix()
{
	matrixStack[currentMatrixStackOffset].m[0][0] = sVec2_s16::fromS32(getCopControlWord(2, 0x0000)).vx;
	matrixStack[currentMatrixStackOffset].m[0][1] = sVec2_s16::fromS32(getCopControlWord(2, 0x0000)).vy;
	matrixStack[currentMatrixStackOffset].m[0][2] = sVec2_s16::fromS32(getCopControlWord(2, 0x0800)).vx;
	matrixStack[currentMatrixStackOffset].m[1][0] = sVec2_s16::fromS32(getCopControlWord(2, 0x0800)).vy;
	matrixStack[currentMatrixStackOffset].m[1][1] = sVec2_s16::fromS32(getCopControlWord(2, 0x1000)).vx;
	matrixStack[currentMatrixStackOffset].m[1][2] = sVec2_s16::fromS32(getCopControlWord(2, 0x1000)).vy;
	matrixStack[currentMatrixStackOffset].m[2][0] = sVec2_s16::fromS32(getCopControlWord(2, 0x1800)).vx;
	matrixStack[currentMatrixStackOffset].m[2][1] = sVec2_s16::fromS32(getCopControlWord(2, 0x1800)).vy;
	matrixStack[currentMatrixStackOffset].m[2][2] = sVec2_s16::fromS32(getCopControlWord(2, 0x2000)).vx;

	matrixStack[currentMatrixStackOffset].t[0] = getCopControlWord(2, 0x2800);
	matrixStack[currentMatrixStackOffset].t[1] = getCopControlWord(2, 0x3000);
	matrixStack[currentMatrixStackOffset].t[2] = getCopControlWord(2, 0x3800);

	currentMatrixStackOffset++;
}

void PopMatrix()
{
	const MATRIX* m = &matrixStack[currentMatrixStackOffset];
	setCopControlWord(2, 0, sVec2_s16::fromValue(m->m[0][0], m->m[0][1]));
	setCopControlWord(2, 0x800, sVec2_s16::fromValue(m->m[0][2], m->m[1][0]));
	setCopControlWord(2, 0x1000, sVec2_s16::fromValue(m->m[1][1], m->m[1][2]));
	setCopControlWord(2, 0x1800, sVec2_s16::fromValue(m->m[2][0], m->m[2][1]));
	setCopControlWord(2, 0x2000, sVec2_s16::fromValue(m->m[2][2], 0));
	setCopControlWord(2, 0x2800, m->t[0]);
	setCopControlWord(2, 0x3000, m->t[1]);
	setCopControlWord(2, 0x3800, m->t[2]);
	currentMatrixStackOffset--;
}

void SetRotMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0, sVec2_s16::fromValue(m->m[0][0], m->m[0][1]));
	setCopControlWord(2, 0x800, sVec2_s16::fromValue(m->m[0][2], m->m[1][0]));
	setCopControlWord(2, 0x1000, sVec2_s16::fromValue(m->m[1][1], m->m[1][2]));
	setCopControlWord(2, 0x1800, sVec2_s16::fromValue(m->m[2][0], m->m[2][1]));
	setCopControlWord(2, 0x2000, sVec2_s16::fromValue(m->m[2][2], 0));
}

void SetTransMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0x2800, m->t[0]);
	setCopControlWord(2, 0x3000, m->t[1]);
	setCopControlWord(2, 0x3800, m->t[2]);
}

void GET_LoadMatrixCoumnToV0(const MATRIX* m1, int column)
{
	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][column], m1->m[1][column]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][column], 0));
}

void GTE_StoreMatrixColumnFromV0(MATRIX* m2, int column)
{
	m2->m[0][column] = getCopReg(2, 0x4800);
	m2->m[1][column] = getCopReg(2, 0x5000);
	m2->m[2][column] = getCopReg(2, 0x5800);
}

void gte_MulMatrix0(const MATRIX* m0, const MATRIX* m1, MATRIX* m2)
{
	SetRotMatrix(m0);

	// this uses the register v0, original code uses generic vector reg
	GET_LoadMatrixCoumnToV0(m1, 0);
	gte_rtv0();
	GTE_StoreMatrixColumnFromV0(m2, 0);

	GET_LoadMatrixCoumnToV0(m1, 1);
	gte_rtv0();
	GTE_StoreMatrixColumnFromV0(m2, 1);

	GET_LoadMatrixCoumnToV0(m1, 2);
	gte_rtv0();
	GTE_StoreMatrixColumnFromV0(m2, 2);
}

MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2)
{
	gte_MulMatrix0(m0, m1, m2);

	// translation
	setCopReg(2, 0, sVec2_s16::fromValue(m1->t[0], m1->t[1]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->t[2], 0));
	gte_rtv0();

	m2->t[0] = getCopReg(2, 0xc800) + m0->t[0];
	m2->t[1] = getCopReg(2, 0xd000) + m0->t[1];
	m2->t[2] = getCopReg(2, 0xd800) + m0->t[2];

	return m2;
}

void RotTrans(SVECTOR* rotation, VECTOR* output, long* flag)
{
	setCopReg(2, 0, sVec2_s16::fromValue(rotation->vx, rotation->vy));
	setCopReg(2, 1, sVec2_s16::fromValue(rotation->vz, 0));
	gte_rtv0();
	output->vx = getCopReg(2, 0x19);
	output->vy = getCopReg(2, 0x1a);
	output->vz = getCopReg(2, 0x1b);
	*flag = getCopControlWord(2, 0xF800);
}

void Square0(sVec3* v0, sVec3* v1)
{
	v1->vx = powl(v0->vx, 2);
	v1->vy = powl(v0->vy, 2);
	v1->vz = powl(v0->vz, 2);
}

s32 SquareRoot0(s32 value)
{
	return sqrtl(value);
}

s32 length1d(s32 param_1)
{
	sVec3 local_28;
	sVec3 local_18;
	local_18.vx = param_1;

	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx);
}

s32 length2d(s32 param_1, s32 param_2)
{
	sVec3 local_28;
	sVec3 local_18;
	local_18.vx = param_1;
	local_18.vy = param_2;
	local_18.vz = 0;
	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx + local_18.vy);
}

VECTOR* rotateVectorByMatrix(MATRIX* m, SVECTOR* inputVector, VECTOR* outputVector)
{
	outputVector->vx = m->m[0][0] * inputVector->vx + m->m[1][0] * inputVector->vy + m->m[2][0] * inputVector->vz;
	outputVector->vx = m->m[0][1] * inputVector->vx + m->m[1][1] * inputVector->vy + m->m[2][1] * inputVector->vz;
	outputVector->vx = m->m[0][2] * inputVector->vx + m->m[1][2] * inputVector->vy + m->m[2][2] * inputVector->vz;

	return outputVector;
}

void computeMatrix(MATRIX* pOutputMatrix, VECTOR* param_2, VECTOR* param_3, VECTOR* param_4)
{
	VECTOR local_60 = {
		(param_3->vx - param_2->vx) >> 16,
		(param_3->vy - param_2->vy) >> 16,
		(param_3->vz - param_2->vz) >> 16,
	};
	VECTOR local_30 = {
		(param_4->vx) >> 16,
		(param_4->vy) >> 16,
		(param_4->vz) >> 16,
	};

	VECTOR local_50;
	VectorNormal(&local_60, &local_50);
	OuterProduct12(&local_30, &local_50, &local_60);

	VECTOR local_40;
	VectorNormal(&local_60, &local_40);
	OuterProduct12(&local_50, &local_40, &local_60);

	VectorNormal(&local_60, &local_30);

	pOutputMatrix->m[0][0] = local_40.vx;
	pOutputMatrix->m[0][1] = local_40.vy;
	pOutputMatrix->m[0][2] = local_40.vz;

	pOutputMatrix->m[1][0] = local_30.vx;
	pOutputMatrix->m[1][1] = local_30.vy;
	pOutputMatrix->m[1][2] = local_30.vz;

	pOutputMatrix->m[2][0] = local_50.vx;
	pOutputMatrix->m[2][1] = local_50.vy;
	pOutputMatrix->m[2][2] = local_50.vz;

	SVECTOR local_20 = {
		(param_2->vx >> 16) * 3,
		(param_2->vy >> 16) * 3,
		(param_2->vz >> 16) * 3,
	};

	rotateVectorByMatrix(pOutputMatrix, &local_20, &local_60);

	pOutputMatrix->t[0] = -local_60.vx;
	pOutputMatrix->t[1] = -local_60.vy;
	pOutputMatrix->t[2] = -local_60.vz;
}

void setIdentityMatrix(MATRIX* param_1)
{
	SVECTOR rotation = { 0,0,0 };
	createRotationMatrix(&rotation, param_1);
	param_1->t[0] = 0;
	param_1->t[1] = 0;
	param_1->t[2] = 0;
}

long RotAverage4(SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag)
{
	gte_ldv3(v0, v1, v2);
	gte_rtpt();
	gte_stsxy3(sxy0, sxy1, sxy2);

	int flag0;
	gte_stlzc(&flag0);

	gte_ldv0(v3);
	gte_rtps();
	gte_stsxy2(sxy3);
	int flag1;
	gte_stlzc(&flag1);

	*p = getCopReg(2, 8);
	*flag = flag0 | flag1;

	copFunction(2, 0x168002e);
	return getCopReg(2, 0x3800);
}

long NCLIP(sVec2_s16 sxy0, sVec2_s16 sxy1, sVec2_s16 sxy2)
{
	s16 SX0;
	s16 SY0;
	s16 SX1;
	s16 SY1;
	s16 SX2;
	s16 SY2;

	sxy0.get(SY0, SX0);
	sxy1.get(SY1, SX1);
	sxy2.get(SY2, SX2);

	return ((int64_t)(SX0 * SY1) + (SX1 * SY2) + (SX2 * SY0) - (SX0 * SY2) - (SX1 * SY0) - (SX2 * SY1));
}

void OuterProduct12(VECTOR* a, VECTOR* b, VECTOR* r)
{
	r->vx = a->vy * b->vz - a->vz * b->vy;
	r->vy = a->vz * b->vx - a->vx * b->vz;
	r->vz = a->vx * b->vy - a->vy * b->vx;
}

void toFloat(VECTOR* v0, std::array<float, 3>& output)
{
	output[0] = v0->vx / (float)0x10000;
	output[1] = v0->vy / (float)0x10000;
	output[2] = v0->vz / (float)0x10000;
}

void fromFloat(VECTOR* v0, std::array<float, 3>& asFloat)
{
	v0->vx = asFloat[0] * (float)0x10000;
	v0->vy = asFloat[1] * (float)0x10000;
	v0->vz = asFloat[2] * (float)0x10000;
}

int VectorNormal(VECTOR* v0, VECTOR* v1)
{
	std::array<float, 3> fb0;

	toFloat(v0, fb0);

	float sum = (fb0[0] * fb0[0]) + (fb0[1] * fb0[1]) + (fb0[2] * fb0[2]);
	float lenght = sqrtf(sum);

	fb0[0] /= lenght;
	fb0[1] /= lenght;
	fb0[2] /= lenght;

	fromFloat(v1, fb0);

	return (v0->vx * v0->vx) + (v0->vy * v0->vy) + (v0->vz * v0->vz);
}
