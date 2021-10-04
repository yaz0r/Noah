#include "noahLib.h"
#include "math.h"
#include "trigo.h"

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

long RotTransPers(SVECTOR* $2, sVec2_s16* sxy, long* p, long* flag)
{
    MissingCode();
    return 0;
}

void setCopControlWord(int, int, s32)
{
	MissingCode();
}

void setCopControlWord(int, int, sFP1616)
{
	MissingCode();
}

void resetMatrixTranslation(MATRIX* m)
{
	m->t[0] = 0;
	m->t[1] = 0;
	m->t[2] = 0;
}

void setCopReg(int, int, sFP1616)
{
	MissingCode();
}

void copFunction(int, u32)
{
	MissingCode();
}

s32 getCopReg(int, int)
{
	MissingCode();
	return 0;
}

s32 getCopControlWord(int, int)
{
	MissingCode();
	return 0;
}

MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	s32 Var2 = getCopReg(2, 0x4800);
	s32 Var3 = getCopReg(2, 0x5000);
	s32 Var4 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	s32 Var5 = getCopReg(2, 0x4800);
	s32 Var6 = getCopReg(2, 0x5000);
	s32 Var7 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][2], 0));
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

void SetRotMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0, sFP1616::fromValue(m->m[0][0], m->m[0][1]));
	setCopControlWord(2, 0x800, sFP1616::fromValue(m->m[0][2], m->m[1][0]));
	setCopControlWord(2, 0x1000, sFP1616::fromValue(m->m[1][1], m->m[1][2]));
	setCopControlWord(2, 0x1800, sFP1616::fromValue(m->m[2][0], m->m[2][1]));
	setCopControlWord(2, 0x2000, sFP1616::fromValue(m->m[2][2], 0));
}

void SetTransMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0x2800, m->t[0]);
	setCopControlWord(2, 0x3000, m->t[1]);
	setCopControlWord(2, 0x3800, m->t[2]);
}

MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	s32 Var2 = getCopReg(2, 0x4800);
	s32 Var3 = getCopReg(2, 0x5000);
	s32 Var5 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	s32 Var7 = getCopReg(2, 0x4800);
	s32 Var8 = getCopReg(2, 0x5000);
	s32 Var9 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);

	m2->m[0][0] = Var2;
	m2->m[0][1] = Var7;
	m2->m[2][0] = Var5;
	m2->m[2][1] = Var9;

	Var2 = getCopReg(2, 0x4800);
	Var7 = getCopReg(2, 0x5000);
	m2->m[0][2] = Var2;
	m2->m[1][0] = Var3;
	m2->m[1][1] = Var8;
	m2->m[1][2] = Var7;

	m2->m[2][2] = getCopReg(2, 0xb);

	// translation
	setCopReg(2, 0, sFP1616::fromValue(m1->t[0], m1->t[1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->t[2], 0));
	copFunction(2, 0x486012);

	Var7 = getCopReg(2, 0xc800);
	Var3 = getCopReg(2, 0xd000);
	Var9 = getCopReg(2, 0xd800);
	s32 Var4 = m0->t[1];
	s32 Var6 = m0->t[2];
	m2->t[0] = Var7 + m0->t[0];
	m2->t[1] = Var3 + Var4;
	m2->t[2] = Var9 + Var6;

	return m2;
}

void RotTrans(SVECTOR* rotation, VECTOR* output, long* flag)
{
	setCopReg(2, 0, sFP1616::fromValue(rotation->vx, rotation->vy));
	setCopReg(2, 1, sFP1616::fromValue(rotation->vz, 0));
	copFunction(2, 0x480012);
	output->vx = getCopReg(2, 0x19);
	output->vy = getCopReg(2, 0x1a);
	output->vz = getCopReg(2, 0x1b);
	*flag = getCopControlWord(2, 0xF800);
	MissingCode();
}
