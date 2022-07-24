#include "noahLib.h"
#include "math.h"
#include "trigo.h"
#include "gte.h"

void createRotationMatrix(SFP_VEC4* param_1, MATRIX* param_2)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    short sVar7;
    int iVar8;

	sVar7 = rcossin_tbl[(param_1->vy & 0xFFF) * 2];
	iVar4 = rcossin_tbl[(param_1->vy & 0xFFF) * 2 + 1];
    iVar8 = rcossin_tbl[(param_1->vz & 0xFFF) * 2];
    iVar5 = rcossin_tbl[(param_1->vz & 0xFFF) * 2 + 1];
    iVar2 = rcossin_tbl[(param_1->vx & 0xFFF) * 2];
    iVar3 = rcossin_tbl[(param_1->vx & 0xFFF) * 2 + 1];
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

MATRIX* ScaleMatrixL(MATRIX* m, FP_VEC4* v)
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

MATRIX* ScaleMatrix(MATRIX* m, FP_VEC4* v)
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

MATRIX* MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2)
{
	/*
    for (int i = 0; i < 3; i++)
    {
        for (int j=0; j<3; j++)
        {
            m2->m[i][j] = m0->m[i][j] * m1->m[j][i] >> 0xC;
        }
    }*/

	SetRotMatrix(m0);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	m2->m[0][0] = getCopReg(2, 0x4800);
	m2->m[1][0] = getCopReg(2, 0x5000);
	m2->m[2][0] = getCopReg(2, 0x5800);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	m2->m[0][1] = getCopReg(2, 0x4800);
	m2->m[1][1] = getCopReg(2, 0x5000);
	m2->m[2][1] = getCopReg(2, 0x5800);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);
	m2->m[0][2] = getCopReg(2, 0x4800);
	m2->m[1][2] = getCopReg(2, 0x5000);
	m2->m[2][2] = getCopReg(2, 0xb);

	return m2;
}

long RotTransPers(SFP_VEC4* v0, sVec2_s16* sxy, long* p, long* flag)
{
	setCopReg(2, 0, sVec2_s16::fromValue(v0->vx, v0->vy));
	setCopReg(2, 0x800, sVec2_s16::fromValue(v0->vz, 0));
	copFunction(2, 0x180001);
	*sxy = sVec2_s16::fromS32(getCopReg(2, 0xe));
	*p = getCopReg(2, 8);
	*flag = getCopControlWord(2, 0xf800);;
	return getCopReg(2, 0x9800) >> 2;
}

long RotTransPers4(SFP_VEC4* v0, SFP_VEC4* v1, SFP_VEC4* v2, SFP_VEC4* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag)
{
	setCopReg(2, 0, sVec2_s16::fromValue(v0->vx, v0->vy));
	setCopReg(2, 1, sVec2_s16::fromValue(v0->vz, 0));
	setCopReg(2, 2, sVec2_s16::fromValue(v1->vx, v1->vy));
	setCopReg(2, 3, sVec2_s16::fromValue(v1->vz, 0));
	setCopReg(2, 4, sVec2_s16::fromValue(v2->vx, v2->vy));
	setCopReg(2, 5, sVec2_s16::fromValue(v2->vz, 0));
	copFunction(2, 0x280030);
	*sxy0 = sVec2_s16::fromS32(getCopReg(2, 0xc));
	*sxy1 = sVec2_s16::fromS32(getCopReg(2, 0xd));
	*sxy2 = sVec2_s16::fromS32(getCopReg(2, 0xe));

	setCopReg(2, 0, sVec2_s16::fromValue(v3->vx, v3->vy));
	setCopReg(2, 1, sVec2_s16::fromValue(v3->vz, 0));
	copFunction(2, 0x180001);
	*sxy3 = sVec2_s16::fromS32(getCopReg(2, 0xe));

	*p = getCopReg(2, 8);
	*flag = getCopControlWord(2, 0xf800);;
	return getCopReg(2, 0x9800) >> 2;
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

void RotTrans(SFP_VEC4* rotation, FP_VEC4* output, long* flag)
{
	setCopReg(2, 0, sVec2_s16::fromValue(rotation->vx, rotation->vy));
	setCopReg(2, 1, sVec2_s16::fromValue(rotation->vz, 0));
	copFunction(2, 0x480012);
	output->vx = getCopReg(2, 0x19);
	output->vy = getCopReg(2, 0x1a);
	output->vz = getCopReg(2, 0x1b);
    if (flag) {
        *flag = getCopControlWord(2, 0xF800);
    }
}

void Square0(VECTOR* v0, VECTOR* v1)
{
	/*v1->vx = powl(v0->vx, 2);
	v1->vy = powl(v0->vy, 2);
	v1->vz = powl(v0->vz, 2);*/

	setCopReg(2, 9, v0->vx);
	setCopReg(2, 10, v0->vy);
	setCopReg(2, 11, v0->vz);
	copFunction(2, 0xa00428);
	v1->vx = getCopReg(2, 0x19);
	v1->vy = getCopReg(2, 0x1a);
	v1->vz = getCopReg(2, 0x1b);
}

struct sSQRTables
{
	short SQRT[202];
	short ISQRT[192];
};

sSQRTables SQRTables = {
	{
		0x1000, 0x101F, 0x103F, 0x105E, 0x107E, 0x109C, 0x10BB, 0x10DA,
		0x10F8, 0x1116, 0x1134, 0x1152, 0x116F, 0x118C, 0x11A9, 0x11C6,
		0x11E3, 0x1200, 0x121C, 0x1238, 0x1254, 0x1270, 0x128C, 0x12A7,
		0x12C2, 0x12DE, 0x12F9, 0x1314, 0x132E, 0x1349, 0x1364, 0x137E,
		0x1398, 0x13B2, 0x13CC, 0x13E6, 0x1400, 0x1419, 0x1432, 0x144C,
		0x1465, 0x147E, 0x1497, 0x14B0, 0x14C8, 0x14E1, 0x14F9, 0x1512,
		0x152A, 0x1542, 0x155A, 0x1572, 0x158A, 0x15A2, 0x15B9, 0x15D1,
		0x15E8, 0x1600, 0x1617, 0x162E, 0x1645, 0x165C, 0x1673, 0x1689,
		0x16A0, 0x16B7, 0x16CD, 0x16E4, 0x16FA, 0x1710, 0x1726, 0x173C,
		0x1752, 0x1768, 0x177E, 0x1794, 0x17AA, 0x17BF, 0x17D5, 0x17EA,
		0x1800, 0x1815, 0x182A, 0x183F, 0x1854, 0x1869, 0x187E, 0x1893,
		0x18A8, 0x18BD, 0x18D1, 0x18E6, 0x18FA, 0x190F, 0x1923, 0x1938,
		0x194C, 0x1960, 0x1974, 0x1988, 0x199C, 0x19B0, 0x19C4, 0x19D8,
		0x19EC, 0x1A00, 0x1A13, 0x1A27, 0x1A3A, 0x1A4E, 0x1A61, 0x1A75,
		0x1A88, 0x1A9B, 0x1AAE, 0x1AC2, 0x1AD5, 0x1AE8, 0x1AFB, 0x1B0E,
		0x1B21, 0x1B33, 0x1B46, 0x1B59, 0x1B6C, 0x1B7E, 0x1B91, 0x1BA3,
		0x1BB6, 0x1BC8, 0x1BDB, 0x1BED, 0x1C00, 0x1C12, 0x1C24, 0x1C36,
		0x1C48, 0x1C5A, 0x1C6C, 0x1C7E, 0x1C90, 0x1CA2, 0x1CB4, 0x1CC6,
		0x1CD8, 0x1CE9, 0x1CFB, 0x1D0D, 0x1D1E, 0x1D30, 0x1D41, 0x1D53,
		0x1D64, 0x1D76, 0x1D87, 0x1D98, 0x1DAA, 0x1DBB, 0x1DCC, 0x1DDD,
		0x1DEE, 0x1E00, 0x1E11, 0x1E22, 0x1E33, 0x1E43, 0x1E54, 0x1E65,
		0x1E76, 0x1E87, 0x1E98, 0x1EA8, 0x1EB9, 0x1ECA, 0x1EDA, 0x1EEB,
		0x1EFB, 0x1F0C, 0x1F1C, 0x1F2D, 0x1F3D, 0x1F4E, 0x1F5E, 0x1F6E,
		0x1F7E, 0x1F8F, 0x1F9F, 0x1FAF, 0x1FBF, 0x1FCF, 0x1FDF, 0x1FEF,
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0x1000, 0xFE0, 0xFC1, 0xFA3, 0xF85, 0xF68, 0xF4C, 0xF30,
		0xF15, 0xEFB, 0xEE1, 0xEC7, 0xEAE, 0xE96, 0xE7E, 0xE66,
		0xE4F, 0xE38, 0xE22, 0xE0C, 0xDF7, 0xDE2, 0xDCD, 0xDB9,
		0xDA5, 0xD91, 0xD7E, 0xD6B, 0xD58, 0xD45, 0xD33, 0xD21,
		0xD10, 0xCFF, 0xCEE, 0xCDD, 0xCCC, 0xCBC, 0xCAC, 0xC9C,
		0xC8D, 0xC7D, 0xC6E, 0xC5F, 0xC51, 0xC42, 0xC34, 0xC26,
		0xC18, 0xC0A, 0xBFD, 0xBEF, 0xBE2, 0xBD5, 0xBC8, 0xBBB,
		0xBAF, 0xBA2, 0xB96, 0xB8A, 0xB7E, 0xB72, 0xB67, 0xB5B,
		0xB50, 0xB45, 0xB39, 0xB2E, 0xB24, 0xB19, 0xB0E, 0xB04,
		0xAF9, 0xAEF, 0xAE5, 0xADB, 0xAD1, 0xAC7, 0xABD, 0xAB4,
		0xAAA, 0xAA1, 0xA97, 0xA8E, 0xA85, 0xA7C, 0xA73, 0xA6A,
		0xA61, 0xA59, 0xA50, 0xA47, 0xA3F, 0xA37, 0xA2E, 0xA26,
		0xA1E, 0xA16, 0xA0E, 0xA06, 0x9FE, 0x9F6, 0x9EF, 0x9E7,
		0x9E0, 0x9D8, 0x9D1, 0x9C9, 0x9C2, 0x9BB, 0x9B4, 0x9AD,
		0x9A5, 0x99E, 0x998, 0x991, 0x98A, 0x983, 0x97C, 0x976,
		0x96F, 0x969, 0x962, 0x95C, 0x955, 0x94F, 0x949, 0x943,
		0x93C, 0x936, 0x930, 0x92A, 0x924, 0x91E, 0x918, 0x912,
		0x90D, 0x907, 0x901, 0x8FB, 0x8F6, 0x8F0, 0x8EB, 0x8E5,
		0x8E0, 0x8DA, 0x8D5, 0x8CF, 0x8CA, 0x8C5, 0x8BF, 0x8BA,
		0x8B5, 0x8B0, 0x8AB, 0x8A6, 0x8A1, 0x89C, 0x897, 0x892,
		0x88D, 0x888, 0x883, 0x87E, 0x87A, 0x875, 0x870, 0x86B,
		0x867, 0x862, 0x85E, 0x859, 0x855, 0x850, 0x84C, 0x847,
		0x843, 0x83E, 0x83A, 0x836, 0x831, 0x82D, 0x829, 0x824,
		0x820, 0x81C, 0x818, 0x814, 0x810, 0x80C, 0x808, 0x804,
	}
};

FP_VEC4* ApplyMatrixLV(MATRIX* m, FP_VEC4* v0, FP_VEC4* v1)
{
	uint uVar1;
	int iVar2;
	uint uVar3;
	int iVar4;
	uint uVar5;
	int iVar6;
	int iVar7;
	int iVar8;
	int iVar9;

	SetRotMatrix(m);
	uVar1 = v0->vx;
	uVar3 = v0->vy;
	uVar5 = v0->vz;
	iVar7 = (int)uVar1 >> 0xf;
	if ((int)uVar1 < 0) {
		iVar7 = -((int)-uVar1 >> 0xf);
		uVar1 = -(-uVar1 & 0x7fff);
	}
	else {
		uVar1 = uVar1 & 0x7fff;
	}
	iVar8 = (int)uVar3 >> 0xf;
	if ((int)uVar3 < 0) {
		iVar8 = -((int)-uVar3 >> 0xf);
		uVar3 = -(-uVar3 & 0x7fff);
	}
	else {
		uVar3 = uVar3 & 0x7fff;
	}
	iVar9 = (int)uVar5 >> 0xf;
	if ((int)uVar5 < 0) {
		iVar9 = -((int)-uVar5 >> 0xf);
		uVar5 = -(-uVar5 & 0x7fff);
	}
	else {
		uVar5 = uVar5 & 0x7fff;
	}
	setCopReg(2, 0x4800, iVar7);
	setCopReg(2, 0x5000, iVar8);
	setCopReg(2, 0x5800, iVar9);
	copFunction(2, 0x41e012);
	iVar7 = getCopReg(2, 0xc800);
	iVar8 = getCopReg(2, 0xd000);
	iVar9 = getCopReg(2, 0xd800);
	setCopReg(2, 0x4800, uVar1);
	setCopReg(2, 0x5000, uVar3);
	setCopReg(2, 0x5800, uVar5);
	copFunction(2, 0x49e012);
	if (iVar7 < 0) {
		iVar7 = iVar7 * 8;
	}
	else {
		iVar7 = iVar7 << 3;
	}
	if (iVar8 < 0) {
		iVar8 = iVar8 * 8;
	}
	else {
		iVar8 = iVar8 << 3;
	}
	if (iVar9 < 0) {
		iVar9 = iVar9 * 8;
	}
	else {
		iVar9 = iVar9 << 3;
	}
	iVar2 = getCopReg(2, 0xc800);
	iVar4 = getCopReg(2, 0xd000);
	iVar6 = getCopReg(2, 0xd800);
	v1->vx = iVar2 + iVar7;
	v1->vy = iVar4 + iVar8;
	v1->vz = iVar6 + iVar9;
	return v1;
}

s32 SquareRoot0(s32 value)
{
	setCopReg(2, 0xf000, value);
	int uVar1 = getCopReg(2, 0xf800);
	int iVar2;
	if (uVar1 != 0x20) {
		uVar1 = uVar1 & 0xfffffffe;
		if ((int)(uVar1 - 0x18) < 0) {
			iVar2 = value >> (0x18 - uVar1 & 0x1f);
		}
		else {
			iVar2 = value << (uVar1 - 0x18 & 0x1f);
		}
		return (uint)((int)SQRTables.SQRT[iVar2 + -0x40] << ((int)(0x1f - uVar1) >> 1 & 0x1fU)) >> 0xc;
	}
	return 0;
}

s32 length1d(s32 param_1)
{
	VECTOR local_28;
	VECTOR local_18;
	local_28.vx = param_1;

	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx);
}

s32 length2d(s32 param_1, s32 param_2)
{
	VECTOR local_28;
	VECTOR local_18;
	local_28.vx = param_1;
	local_28.vy = param_2;
	local_28.vz = 0;
	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx + local_18.vy);
}

FP_VEC4* ApplyMatrix(MATRIX* m, SFP_VEC4* inputVector, FP_VEC4* outputVector)
{
#if 0
	outputVector->vx = m->m[0][0] * inputVector->vx + m->m[1][0] * inputVector->vy + m->m[2][0] * inputVector->vz;
	outputVector->vx = m->m[0][1] * inputVector->vx + m->m[1][1] * inputVector->vy + m->m[2][1] * inputVector->vz;
	outputVector->vx = m->m[0][2] * inputVector->vx + m->m[1][2] * inputVector->vy + m->m[2][2] * inputVector->vz;

	return outputVector;
#endif

	SetRotMatrix(m);
	setCopReg(2, 0, sVec2_s16::fromValue(inputVector->vx, inputVector->vy));
	setCopReg(2, 0x800, sVec2_s16::fromValue(inputVector->vz, 0));
	copFunction(2, 0x486012);
	outputVector->vx = getCopReg(2, 0x19);;
	outputVector->vy = getCopReg(2, 0x1a);;
	outputVector->vz = getCopReg(2, 0x1b);;
	return outputVector;
}

void setupVector(VECTOR* param_1, long param_2, long param_3, long param_4)
{
    param_1->vx = param_2;
    param_1->vy = param_3;
    param_1->vz = param_4;
    return;
}

void lookAtNoDivide(MATRIX* param_1, SVECTOR* param_2, SVECTOR* param_3, SVECTOR* param_4)
{
    VECTOR local_58;
    VECTOR local_48;
    VECTOR local_38;
    VECTOR local_28;

    setupVector(&local_58, (int)param_3->vx - (int)param_2->vx, (int)param_3->vy - (int)param_2->vy, (int)param_3->vz - (int)param_2->vz);
    local_28.vx = (long)param_4->vx;
    local_28.vy = (long)param_4->vy;
    local_28.vz = (long)param_4->vz;
    VectorNormal(&local_58, &local_48);
    OuterProduct12(&local_28, &local_48, &local_58);
    VectorNormal(&local_58, &local_38);
    OuterProduct12(&local_48, &local_38, &local_58);
    VectorNormal(&local_58, &local_28);
    param_1->m[0][0] = (short)local_38.vx;
    param_1->m[0][1] = (short)local_38.vy;
    param_1->m[0][2] = (short)local_38.vz;
    param_1->m[1][0] = (short)local_28.vx;
    param_1->m[1][1] = (short)local_28.vy;
    param_1->m[1][2] = (short)local_28.vz;
    param_1->m[2][0] = (short)local_48.vx;
    param_1->m[2][1] = (short)local_48.vy;
    param_1->m[2][2] = (short)local_48.vz;
    PushMatrix();
    ApplyMatrix(param_1, param_2, &local_58);
    param_1->t[0] = -local_58.vx;
    param_1->t[1] = -local_58.vy;
    param_1->t[2] = -local_58.vz;
    PopMatrix();
    return;
}

void lookAtDivided(MATRIX* pOutputMatrix, FP_VEC4* vEye, FP_VEC4* vAt, FP_VEC4* vUp)
{
	FP_VEC4 local_60 = {
		(vAt->vx - vEye->vx) >> 16,
		(vAt->vy - vEye->vy) >> 16,
		(vAt->vz - vEye->vz) >> 16,
	};
	FP_VEC4 local_30 = {
		(vUp->vx) >> 16,
		(vUp->vy) >> 16,
		(vUp->vz) >> 16,
	};

	FP_VEC4 local_50;
	VectorNormal(&local_60, &local_50);
	OuterProduct12(&local_30, &local_50, &local_60);

	FP_VEC4 local_40;
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

    SFP_VEC4 local_20 = {
        (s16)((vEye->vx >> 16) * 3),
        (s16)((vEye->vy >> 16) * 3),
        (s16)((vEye->vz >> 16) * 3),
    };

	ApplyMatrix(pOutputMatrix, &local_20, &local_60);

	pOutputMatrix->t[0] = -local_60.vx;
	pOutputMatrix->t[1] = -local_60.vy;
	pOutputMatrix->t[2] = -local_60.vz;
}

void setIdentityMatrix(MATRIX* param_1)
{
	SFP_VEC4 rotation = { 0,0,0 };
	createRotationMatrix(&rotation, param_1);
	param_1->t[0] = 0;
	param_1->t[1] = 0;
	param_1->t[2] = 0;
}

long RotAverage4(SFP_VEC4* v0, SFP_VEC4* v1, SFP_VEC4* v2, SFP_VEC4* v3, sVec2_s16* sxy0, sVec2_s16* sxy1, sVec2_s16* sxy2, sVec2_s16* sxy3, long* p, long* flag)
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

    sxy0.get(SX0, SY0);
    sxy1.get(SX1, SY1);
    sxy2.get(SX2, SY2);

    return -((int64_t)(SX0 * SY1) + (SX1 * SY2) + (SX2 * SY0) - (SX0 * SY2) - (SX1 * SY0) - (SX2 * SY1));
}

void OuterProduct12(FP_VEC4* a, FP_VEC4* b, FP_VEC4* r)
{
#if 0
	r->vx = ((s64)a->vy * b->vz - (s64)a->vz * b->vy) >> 12;
	r->vy = ((s64)a->vz * b->vx - (s64)a->vx * b->vz) >> 12;
	r->vz = ((s64)a->vx * b->vy - (s64)a->vy * b->vx) >> 12;
#else
	int uVar2 = getCopControlWord(2, 0);
	int uVar3 = getCopControlWord(2, 0x1000);
	int uVar4 = getCopControlWord(2, 0x2000);
	setCopControlWord(2, 0, a->vx);
	setCopControlWord(2, 0x1000, a->vy);
	setCopControlWord(2, 0x2000, a->vz);
	setCopReg(2, 11, b->vz);
	setCopReg(2, 9, b->vx);
	setCopReg(2, 10, b->vy);
	copFunction(2, 0x178000c);
	r->vx = getCopReg(2, 0x19);
	r->vy = getCopReg(2, 0x1a);
	r->vz = getCopReg(2, 0x1b);	
	setCopControlWord(2, 0, uVar2);
	setCopControlWord(2, 0x1000, uVar3);
	setCopControlWord(2, 0x2000, uVar4);
#endif
}

s32 gte_stR11R12() {
    return getCopControlWord(2, 0);
}

s32 gte_stR22R23() {
    return getCopControlWord(2, 0x1000);
}

s32 gte_stR33() {
    return getCopControlWord(2, 0x2000);
}

void gte_ldR11R12(s32 value) {
    setCopControlWord(2, 0, value);
}

void gte_ldR22R23(s32 value) {
    setCopControlWord(2, 0x1000, value);
}

void gte_ldR33(s32 value) {
    setCopControlWord(2, 0x2000, value);
}

void gte_ldopv2(VECTOR* v) {
    setCopReg(2, 11, v->vz);
    setCopReg(2, 9, v->vx);
    setCopReg(2, 10, v->vy);
}

void OuterProduct0(VECTOR* $2, VECTOR* $3, VECTOR* v2)
{
    s32 uVar1 = gte_stR11R12();
    s32 uVar2 = gte_stR22R23();
    s32 uVar3 = gte_stR33();
    gte_ldR11R12($2->vx);
    gte_ldR22R23($2->vy);
    gte_ldR33($2->vz);
    gte_ldopv2($3);
    gte_op0();
    gte_stlvnl(v2);
    gte_ldR11R12(uVar1);
    gte_ldR22R23(uVar2);
    gte_ldR33(uVar3);
    return;
}

void toFloat(FP_VEC4* v0, std::array<float, 3>& output)
{
	output[0] = v0->vx / (float)0x10000;
	output[1] = v0->vy / (float)0x10000;
	output[2] = v0->vz / (float)0x10000;
}

void fromFloat(FP_VEC4* v0, std::array<float, 3>& asFloat)
{
	v0->vx = asFloat[0] * (float)0x10000;
	v0->vy = asFloat[1] * (float)0x10000;
	v0->vz = asFloat[2] * (float)0x10000;
}

int VectorNormalInner(FP_VEC4* input, FP_VEC4* output)
{
	setCopReg(2, 0x4800, input->vx);
	setCopReg(2, 0x5000, input->vy);
	setCopReg(2, 0x5800, input->vz);
	copFunction(2, 0xa00428);
	int iVar2 = getCopReg(2, 0xc800);
	int iVar3 = getCopReg(2, 0xd000);
	int iVar4 = getCopReg(2, 0xd800);
	iVar4 = iVar2 + iVar3 + iVar4;
	setCopReg(2, 0xf000, iVar4);
	int uVar1 = getCopReg(2, 0xf800);
	uVar1 = uVar1 & 0xfffffffe;
	if ((int)(uVar1 - 0x18) < 0) {
		iVar2 = iVar4 >> (0x18 - uVar1 & 0x1f);
	}
	else {
		iVar2 = iVar4 << (uVar1 - 0x18 & 0x1f);
	}
	setCopReg(2, 0x4000, (int)SQRTables.ISQRT[iVar2 + -0x40]); // this can overflow back into SQRT
	setCopReg(2, 0x4800, input->vx);
	setCopReg(2, 0x5000, input->vy);
	setCopReg(2, 0x5800, input->vz);
	copFunction(2, 0x190003d);
	output->vx = getCopReg(2, 0xc800) >> ((int)(0x1f - uVar1) >> 1 & 0x1fU);
	output->vy = getCopReg(2, 0xd000) >> ((int)(0x1f - uVar1) >> 1 & 0x1fU);
	output->vz = getCopReg(2, 0xd800) >> ((int)(0x1f - uVar1) >> 1 & 0x1fU);
	return iVar4;
}

int VectorNormal(FP_VEC4* input, FP_VEC4* output)
{
    return VectorNormalInner(input, output);
}

int VectorNormalS(FP_VEC4* input, SFP_VEC4* output)
{
    FP_VEC4 tempOutput;
    int result = VectorNormalInner(input, &tempOutput);
    output->vx = tempOutput.vx;
    output->vy = tempOutput.vy;
    output->vz = tempOutput.vz;
    return result;
}

void copyRotationMatrix(MATRIX* param_1, MATRIX* param_2)
{
	param_1->m[0] = param_2->m[0];
	param_1->m[1] = param_2->m[1];
	param_1->m[2] = param_2->m[2];
	param_1->m[1][0] = param_2->m[1][0];
	param_1->m[1][1] = param_2->m[1][1];
	param_1->m[1][2] = param_2->m[1][2];
	param_1->m[2][0] = param_2->m[2][0];
	param_1->m[2][1] = param_2->m[2][1];
	param_1->m[2][2] = param_2->m[2][2];
	return;
}

void copyTranslationMatrix(MATRIX* param_1, MATRIX* param_2)
{
	param_1->t[0] = param_2->t[0];
	param_1->t[1] = param_2->t[1];
	param_1->t[2] = param_2->t[2];
	return;
}

void copyMatrix(MATRIX* param_1, MATRIX* param_2)
{
	copyRotationMatrix(param_1, param_2);
	copyTranslationMatrix(param_1, param_2);
}

MATRIX* SetMulMatrix(MATRIX* m0, MATRIX* m1)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	int uVar4 = getCopReg(2, 0x4800);
	int iVar5 = getCopReg(2, 0x5000);
	int uVar6 = getCopReg(2, 0x5800);
	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	int iVar7 = getCopReg(2, 0x4800);
	int uVar8 = getCopReg(2, 0x5000);
	int iVar9 = getCopReg(2, 0x5800);
	setCopReg(2, 0, sVec2_s16::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sVec2_s16::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);
	int uVar1 = getCopReg(2, 0x4800);
	int iVar2 = getCopReg(2, 0x5000);
	int uVar3 = getCopReg(2, 0x5800);
	setCopControlWord(2, 0, iVar7 << 0x10 | uVar4 & 0xffff);
	setCopControlWord(2, 0x800, uVar1 & 0xffff | iVar5 << 0x10);
	setCopControlWord(2, 0x1000, iVar2 << 0x10 | uVar8 & 0xffff);
	setCopControlWord(2, 0x1800, iVar9 << 0x10 | uVar6 & 0xffff);
	setCopControlWord(2, 0x2000, uVar3);
	return m0;
}

MATRIX* TransMatrix(MATRIX* m, FP_VEC4* v)
{
    long lVar1;
    long lVar2;

    lVar1 = v->vy;
    lVar2 = v->vz;
    m->t[0] = v->vx;
    m->t[1] = lVar1;
    m->t[2] = lVar2;
    return m;
}

int rsin(int a)
{
    //if (a < 0) // really not needed; bit mask does it all
    //	return -rcossin_tbl[(-a & 0xfffU) * 2];

    return rcossin_tbl[(a & 0xfffU) * 2];
}

int rcos(int a)
{
    //if (a < 0) // really not needed; bit mask does it all
    //	return rcossin_tbl[(-a & 0xfffU) * 2 + 1];

    return rcossin_tbl[(a & 0xfffU) * 2 + 1];
}

#define	FIXED(a)			((a) >> 12)
MATRIX* RotMatrixX(long r, MATRIX* m)
{
    // correct Psy-Q implementation
    int s0 = rsin(r);
    int c0 = rcos(r);
    int t1, t2;
    t1 = m->m[1][0];
    t2 = m->m[2][0];
    m->m[1][0] = FIXED(t1 * c0 - t2 * s0);
    m->m[2][0] = FIXED(t1 * s0 + t2 * c0);
    t1 = m->m[1][1];
    t2 = m->m[2][1];
    m->m[1][1] = FIXED(t1 * c0 - t2 * s0);
    m->m[2][1] = FIXED(t1 * s0 + t2 * c0);
    t1 = m->m[1][2];
    t2 = m->m[2][2];
    m->m[1][2] = FIXED(t1 * c0 - t2 * s0);
    m->m[2][2] = FIXED(t1 * s0 + t2 * c0);

    return m;
}

MATRIX* RotMatrixY(long r, MATRIX* m)
{
    // correct Psy-Q implementation
    int s0 = rsin(r);
    int c0 = rcos(r);
    int t1, t2;
    t1 = m->m[0][0];
    t2 = m->m[2][0];
    m->m[0][0] = FIXED(t1 * c0 + t2 * s0);
    m->m[2][0] = FIXED(-t1 * s0 + t2 * c0);
    t1 = m->m[0][1];
    t2 = m->m[2][1];
    m->m[0][1] = FIXED(t1 * c0 + t2 * s0);
    m->m[2][1] = FIXED(-t1 * s0 + t2 * c0);
    t1 = m->m[0][2];
    t2 = m->m[2][2];
    m->m[0][2] = FIXED(t1 * c0 + t2 * s0);
    m->m[2][2] = FIXED(-t1 * s0 + t2 * c0);

    return m;
}

MATRIX* RotMatrixZ(long r, MATRIX* m)
{
    // correct Psy-Q implementation
    int s0 = rsin(r);
    int c0 = rcos(r);
    int t1, t2;
    t1 = m->m[0][0];
    t2 = m->m[1][0];
    m->m[0][0] = FIXED(t1 * c0 - t2 * s0);
    m->m[1][0] = FIXED(t1 * s0 + t2 * c0);
    t1 = m->m[0][1];
    t2 = m->m[1][1];
    m->m[0][1] = FIXED(t1 * c0 - t2 * s0);
    m->m[1][1] = FIXED(t1 * s0 + t2 * c0);
    t1 = m->m[0][2];
    t2 = m->m[1][2];
    m->m[0][2] = FIXED(t1 * c0 - t2 * s0);
    m->m[1][2] = FIXED(t1 * s0 + t2 * c0);

    return m;
}

MATRIX* RotMatrixYXZ(SFP_VEC4* r, MATRIX* m)
{
    // correct Psy-Q implementation
    int c0, c1, c2;
    int s0, s1, s2;

    c0 = rcos(r->vx);
    c1 = rcos(r->vy);
    c2 = rcos(r->vz);
    s0 = rsin(r->vx);
    s1 = rsin(r->vy);
    s2 = rsin(r->vz);

    // Y-axis
    m->m[1][0] = FIXED(s2 * c0);
    m->m[1][1] = FIXED(c2 * c0);
    m->m[1][2] = -s0;

    // X-axis
    int x0 = FIXED(s1 * s0);
    m->m[0][0] = FIXED(c1 * c2) + FIXED(x0 * s2);
    m->m[0][1] = FIXED(x0 * c2) - FIXED(c1 * s2);
    m->m[0][2] = FIXED(s1 * c0);

    // Z-axis
    int z0 = FIXED(c1 * s0);
    m->m[2][1] = FIXED(s1 * s2) + FIXED(z0 * c2);
    m->m[2][0] = FIXED(z0 * s2) - FIXED(s1 * c2);
    m->m[2][2] = FIXED(c1 * c0);

    return m;
}

MATRIX* RotMatrixZYX(SVECTOR* r, MATRIX* m)
{
#if 0
    // TODO: correct Psy-Q implementation
#else
    m->m[0][0] = 0x1000;
    m->m[0][1] = 0;
    m->m[0][2] = 0;

    m->m[1][0] = 0;
    m->m[1][1] = 0x1000;
    m->m[1][2] = 0;

    m->m[2][0] = 0;
    m->m[2][1] = 0;
    m->m[2][2] = 0x1000;

    RotMatrixX(r->vx, m);
    RotMatrixY(r->vy, m);
    RotMatrixZ(r->vz, m);
#endif
    return m;
}

VECTOR* ApplyRotMatrix(SVECTOR* v0, VECTOR* v1)
{
#if 1
    // correct Psy-Q implementation
    gte_ldv0(v0);
    gte_rtv0();
    gte_stlvnl(v1);
#else
    MATRIX temp;
    gte_ReadRotMatrix(&temp);
    MATRIX* m = &temp;
    APPLYMATRIX(m, v0, v1);
#endif
    return v1;
}


VECTOR* ApplyRotMatrixLV(VECTOR* v0, VECTOR* v1)
{
#if 1
    // correct Psy-Q implementation
    VECTOR tmpHI;
    VECTOR tmpLO;

    tmpHI.vx = v0->vx;
    tmpHI.vy = v0->vy;
    tmpHI.vz = v0->vz;

    if (tmpHI.vx < 0)
    {
        tmpLO.vx = -(-tmpHI.vx >> 0xf);
        tmpHI.vx = -(-tmpHI.vx & 0x7fff);
    }
    else
    {
        tmpLO.vx = tmpHI.vx >> 0xf;
        tmpHI.vx = tmpHI.vx & 0x7fff;
    }

    if (tmpHI.vy < 0)
    {
        tmpLO.vy = -(-tmpHI.vy >> 0xf);
        tmpHI.vy = -(-tmpHI.vy & 0x7fff);
    }
    else
    {
        tmpLO.vy = tmpHI.vy >> 0xf;
        tmpHI.vy = tmpHI.vy & 0x7fff;
    }

    if (tmpHI.vz < 0)
    {
        tmpLO.vz = -(-tmpHI.vz >> 0xf);
        tmpHI.vz = -(-tmpHI.vz & 0x7fff);
    }
    else
    {
        tmpLO.vz = tmpHI.vz >> 0xf;
        tmpHI.vz = tmpHI.vz & 0x7fff;
    }

    gte_ldlvl(&tmpLO);
    gte_rtir_sf0();
    gte_stlvnl(&tmpLO);

    gte_ldlvl(&tmpHI);
    gte_rtir();

    if (tmpLO.vx < 0)
        tmpLO.vx *= 8;
    else
        tmpLO.vx <<= 3;

    if (tmpLO.vy < 0)
        tmpLO.vy *= 8;
    else
        tmpLO.vy <<= 3;

    if (tmpLO.vz < 0)
        tmpLO.vz *= 8;
    else
        tmpLO.vz <<= 3;

    gte_stlvnl(&tmpHI);

    v1->vx = tmpHI.vx + tmpLO.vx;
    v1->vy = tmpHI.vy + tmpLO.vy;
    v1->vz = tmpHI.vz + tmpLO.vz;
#else
    MATRIX temp;
    gte_ReadRotMatrix(&temp);
    MATRIX* m = &temp;
    APPLYMATRIX(m, v0, v1);
#endif
    return v1;
}
