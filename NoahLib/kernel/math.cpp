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
