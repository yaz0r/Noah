#include "NoahLib.h"
#include "screenDistortion.h"
#include "kernel/graphics.h"
#include "fieldScriptSupport.h"

short screenDistortionRunning = 0;
short screenDistortionFadeOut = 0;
u32 screenDistrotionAllocated = 0;

std::vector<DR_MOVE> screenDistortionDrawMove[2];
std::vector<POLY_FT4> screenDistortionPolys[2];

short distortionParamDivider = 0;
int distortionParam0 = 0;
int distortionParam1 = 0;
int distortionParam2 = 0;
int distortionParam3 = 0;
int distortionParam4 = 0;
int distortionParam5 = 0;
int distortionParamDiff0 = 0;
int distortionParamDiff1 = 0;
int distortionParamDiff2 = 0;
int distortionParamDiff3 = 0;
int distortionParamDiff4 = 0;
int distortionParamDiff5 = 0;

void setScreenDistortionParams(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
    if (param_7 == 0) {
        param_7 = 1;
    }
    distortionParamDivider = (short)param_7;
    distortionParam0 = (param_1 * 0x10000 - distortionParamDiff0) / param_7;
    distortionParam1 = (param_2 * 0x10000 - distortionParamDiff1) / param_7;
    distortionParam2 = (param_3 * 0x10000 - distortionParamDiff2) / param_7;
    distortionParam3 = (param_4 * 0x10000 - distortionParamDiff3) / param_7;
    distortionParam4 = (param_5 * 0x10000 - distortionParamDiff4) / param_7;
    distortionParam5 = (param_6 * 0x10000 - distortionParamDiff5) / param_7;
    return;
}

void freeScreenDistortion()
{
    screenDistortionRunning = 0;
    if (screenDistrotionAllocated != 0) {
        screenDistortionDrawMove[0].clear();
        screenDistortionDrawMove[1].clear();
        screenDistortionPolys[0].clear();
        screenDistortionPolys[1].clear();
        screenDistrotionAllocated = 0;
    }
}

void setupScreenDistortion(int param_1)
{
    screenDistortionRunning = 1;
    if (screenDistrotionAllocated == 0) {
        screenDistortionDrawMove[0].resize(16);
        screenDistortionDrawMove[1].resize(16);
        screenDistortionPolys[0].resize(360);
        screenDistortionPolys[1].resize(360);
        screenDistrotionAllocated = 1;

        int x2 = 0;
        int y2 = 0x10;

        for (int i=0; i<17; i++)
        {
            int tempy2 = y2;
            int x = 0;
            int y = 0x10;

            for (int j=0; j<20; j++)
            {
                POLY_FT4* p = &screenDistortionPolys[0][j + i];
                POLY_FT4* p2 = &screenDistortionPolys[1][j + i];

                SetPolyFT4(p);
                SetSemiTrans(p, 0);

                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->x0y0.vx = x;
                p->x0y0.vy = i << 4;
                p->x1y1.vx = y;
                p->x1y1.vy = i << 4;
                p->x2y2.vx = x;
                p->x2y2.vy = tempy2;
                p->x3y3.vx = y;
                p->x3y3.vy = tempy2;

                if (i < 0xe) {
                    p->u0 = x & 0x3f;
                    p->v0 = i << 4;
                    p->u1 = p->u0 + 0x10;
                    p->v1 = p->v0;
                    p->u2 = p->u0;
                    p->v2 = tempy2;
                    p->u3 = p->u0 + 0x10;
                    p->v3 = p->v2;
                    p->tpage = GetTPage(2, 0, x & 0xffc0, 0);

                    *p2 = *p;
                    p2->tpage = GetTPage(2, 0, x & 0xffc0, 0x100);
                }
                else
                {
                    p->u0 = x & 0x3f;
                    p->v0 = ((j >> 2) * 16) + (i + -0xe) * 0x50;
                    p->u1 = p->u0 + 0x10;
                    p->v1 = p->v0;
                    p->u2 = p->u0;
                    p->v2 = p->v0 + 0x10;
                    p->u3 = p->u0 + 0x10;
                    p->v3 = p->v0 + 0x10;
                    p->tpage = GetTPage(2, 0, 0x3c0, 0);

                    *p2 = *p;
                    p2->tpage = GetTPage(2, 0, x & 0xffc0, 0x100);

                }
                x = x + 0x10;
                y = y + 0x10;
            }
            x2 = x2 + 0x14;
            y2 = y2 + 0x10;
        }

        RECT tempRect;
        tempRect.y = 0x20;
        tempRect.w = 0x140;
        tempRect.x = 0;
        tempRect.h = 0xc0;
        SetDrawMove(&screenDistortionDrawMove[0][0], &tempRect, 0, 0);
        tempRect.y = 0x120;
        SetDrawMove(&screenDistortionDrawMove[1][0], &tempRect, 0, 0x100);

        tempRect.w = 0x40;
        tempRect.h = 0x10;

        const std::array<std::array<u16, 2>, 0xF> distortionDrawMoves = {
            {
                {0x000, 0xB0},
                {0x040, 0xB0},
                {0x080, 0xB0},
                {0x0C0, 0xB0},
                {0x100, 0xB0},
                {0x000, 0xC0},
                {0x040, 0xC0},
                {0x080, 0xC0},
                {0x0C0, 0xC0},
                {0x100, 0xC0},
                {0x000, 0xD0},
                {0x040, 0xD0},
                {0x080, 0xD0},
                {0x0C0, 0xD0},
                {0x100, 0xD0},
            }
        };

        for (int i=0; i<0xF; i++)
        {
            tempRect.x = distortionDrawMoves[i][0];
            tempRect.y = distortionDrawMoves[i][1];
            SetDrawMove(&screenDistortionDrawMove[0][i+1], &tempRect, 0x3C0, i << 4);

            tempRect.y = distortionDrawMoves[i][1] + 0x100;
            SetDrawMove(&screenDistortionDrawMove[1][i+1], &tempRect, 0x3C0, i << 4);
        }
    }

    if (param_1 == 0) {
        int uVar11 = getImmediateOrVariableUnsigned(1);
        int uVar12 = getImmediateOrVariableUnsigned(3);
        int uVar13 = getImmediateOrVariableUnsigned(5);
        int uVar6 = getImmediateOrVariableUnsigned(7);
        int uVar7 = getImmediateOrVariableUnsigned(9);
        int uVar8 = getImmediateOrVariableUnsigned(0xb);
        int uVar9 = getImmediateOrVariableUnsigned(0xd);
        setScreenDistortionParams(uVar11, uVar12, uVar13, uVar6, uVar7, uVar8, uVar9);
    }
    screenDistortionFadeOut = 0;
}

void updateAndRenderScreenDistortion(void)
{
    if (screenDistortionRunning == 0) {
        return;
    }
    if (screenDistortionFadeOut == 0) {
        if (distortionParamDivider < 1) goto LAB_Field__800a4f24;
    }
    else {
        if (distortionParamDivider < 1) {
            distortionParamDiff5 = 0;
            distortionParamDiff4 = 0;
            distortionParamDiff3 = 0;
            distortionParamDiff2 = 0;
            distortionParamDiff1 = 0;
            distortionParamDiff0 = 0;
            screenDistortionRunning = 0;
            goto LAB_Field__800a4f24;
        }
    }
    distortionParamDiff0 = distortionParamDiff0 + distortionParam0;
    distortionParamDiff1 = distortionParamDiff1 + distortionParam1;
    distortionParamDivider = distortionParamDivider + -1;
    distortionParamDiff2 = distortionParamDiff2 + distortionParam2;
    distortionParamDiff3 = distortionParamDiff3 + distortionParam3;
    distortionParamDiff4 = distortionParamDiff4 + distortionParam4;
    distortionParamDiff5 = distortionParamDiff5 + distortionParam5;
LAB_Field__800a4f24:

    MissingCode();
}
