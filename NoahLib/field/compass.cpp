#include "noahLib.h"
#include "field.h"
#include "compass.h"
#include "kernel/filesystem.h"
#include "kernel/TIM.h"
#include "dialogWindows.h"

std::array<sFieldCompassVar2, 25> fieldCompassVar2;

const std::array<std::array<s16, 4>, 9> compassInitDataX = { {
	{0x400, 0x200, 0x400, 0x200},
	{0x200, 0x0, 0x200, 0x0},
	{0, -0x200, 0x0, -0x200},
	{-0x200, -0x400, -0x200, -0x400},
	{-0x100, 0x100, -0x100, 0x100},
	{-0x100, 0x100, -0x100, 0x100},
	{-0x100, 0x100, -0x100, 0x100},
	{-0x100, 0x100, -0x100, 0x100},
	{-0x100, 0x100, -0x100, 0x100},
} };

const std::array<std::array<s16, 4>, 9> compassInitDataZ = { {
	{0x400, 0x400, 0x200, 0x200},
	{0x200, 0x200, 0x0, 0},
	{0, 0, -0x200, -0x200},
	{-0x200, -0x200, -0x400, -0x400},
	{0xA0, 0xA0, -0xA0, -0xA0},
	{0xA0, 0xA0, -0xA0, -0xA0},
	{0xA0, 0xA0, -0xA0, -0xA0},
	{0xA0, 0xA0, -0xA0, -0xA0},
	{0x300, 0x300, 0x0, 0},
} };

const std::array<std::array<s16, 6>, 4> compassInitDataTPage = { {
	{0x1, 0x0, 0x2A0, 0x1C0, 0x0, 0xFB},
	{0x0, 0x0, 0x29C, 0x1C0, 0x100, 0xF5},
	{0x0, 0x0, 0x280, 0x1C0, 0x100, 0xF2},
	{0x0, 0x0, 0x2A0, 0x1C0, 0x100, 0xFB},
} };

const std::array<std::array<s16, 4>, 9> compassInitDataUV = { {
	{0x40, 0x50, 0x40, 0x50},
	{0x50, 0x60, 0x50, 0x60},
	{0x60, 0x70, 0x60, 0x70},
	{0x70, 0x7F, 0x70, 0x7F},
	{0x70, 0x80, 0x70, 0x80},
	{0x70, 0x80, 0x70, 0x80},
	{0x70, 0x80, 0x70, 0x80},
	{0x70, 0x80, 0x70, 0x80},
	{0x70, 0x80, 0x70, 0x80},
} };

const std::array<std::array<s16, 4>, 9> compassInitDataUV2 = { {
	{0xC0, 0xC0, 0xD0, 0xD0},
	{0xD0, 0xD0, 0xE0, 0xE0},
	{0xE0, 0xE0, 0xF0, 0xF0},
	{0xF0, 0xF0, 0xFF, 0xFF},
	{0xC0, 0xC0, 0xCA, 0xCA},
	{0xCA, 0xCA, 0xD4, 0xD4},
	{0xD4, 0xD4, 0xDE, 0xDE},
	{0xDE, 0xDE, 0xE8, 0xE8},
	{0xE8, 0xE8, 0x100, 0x100},
} };


std::array<u16, 8> compassData0 = {
	0x81, 0xC0, 0x60, 0x30,
	0x18, 0xC, 0x6, 0x3,
};

std::array<std::array<u16, 16>, 8> compassData1;

std::vector<u16> compassData2 = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

RECT compassDataRect = { 0,0,0,0 };

s16 compassArrowTargetDirection = 0;
s16 compassArrowCurrentDirection = 0;

int fieldCompassVar = 0;

void drawCompassLetters(sTag* param_1, sFieldCompassVar2* param_2, MATRIX* param_3, int frameOddOrEven)
{
	long lStack32;
	long lStack28;

	POLY_FT4* pPrim = &param_2->m20_poly[frameOddOrEven];
	PushMatrix();
	SetRotMatrix(param_3);
	SetTransMatrix(param_3);
	RotAverage4(&param_2->m0[0], &param_2->m0[1], &param_2->m0[2], &param_2->m0[3], &pPrim->x0y0, &pPrim->x1y1, &pPrim->x2y2, &pPrim->x3y3, &lStack32, &lStack28);

	int sVar2 = (short)(((int)pPrim->x3y3.vx + (int)pPrim->x2y2.vx) / 2);
	int sVar3 = sVar2 + 8;
	int sVar1 = pPrim->x3y3.vy;
	sVar2 = sVar2 + -8;
	pPrim->x0y0.vx = sVar2;
	pPrim->x1y1.vx = sVar3;
	pPrim->x2y2.vx = sVar2;
	pPrim->x3y3.vx = sVar3;
	pPrim->x2y2.vy = sVar1;
	pPrim->x3y3.vy = sVar1;
	pPrim->x0y0.vy = sVar1 + -10;
	pPrim->x1y1.vy = sVar1 + -10;

	pPrim->m0_pNext = param_1[1].m0_pNext;
	param_1[1].m0_pNext = pPrim;

	PopMatrix();
}

void drawCompassArrowSegment(sTag* param_1, sFieldCompassVar2* param_2, MATRIX* param_3, int frameOddOrEven)
{
	long lStack32;
	long lStack28;

	POLY_FT4* pPrim = &param_2->m20_poly[frameOddOrEven];
	PushMatrix();
	SetRotMatrix(param_3);
	SetTransMatrix(param_3);
	RotAverage4(&param_2->m0[0], &param_2->m0[1], &param_2->m0[2], &param_2->m0[3], &pPrim->x0y0, &pPrim->x1y1, &pPrim->x2y2, &pPrim->x3y3, &lStack32, &lStack28);

	pPrim->m0_pNext = param_1[1].m0_pNext;
	param_1[1].m0_pNext = pPrim;

	PopMatrix();
}

short compassCardinalPointsLocation[4][2] = {
	{0x0, 0x500},
	{0x500, 0x0},
	{0x0, -0x500},
	{-0x500, 0x0},
};

void renderCompass()
{
	for (int i = 0; i < 8; i++)
	{
		if ((compassData0[i] & DollyStop) == 0)
		{
			for (int j = 0; j < 16; j++)
			{
				compassData1[i][j] = compassData2[j];
			}
		}
		else
		{
			for (int j = 0; j < 16; j++)
			{
				compassData1[i][j] = 0;
			}
		}
	}
	
	compassDataRect.w = 0x80;
	LoadImage(&compassDataRect, (u8*)&compassData1[0][0]);
	SetGeomScreen(0x80);
	SetGeomOffset(0x10a, 0xa6);

	FP_VEC4 local_30 = { 0,0,0 };
	FP_VEC4 local_40 = {
		0,
		cameraEye[1] - cameraAt[1],
		length2d((cameraEye[0] - cameraAt[0]) >> 0x10,(cameraEye[2] - cameraAt[2]) >> 0x10) * -0x10000,
	};

	MATRIX MStack200;
	computeMatrix(&MStack200, &local_40, &local_30, &cameraUp);

	MATRIX MStack232;
	setIdentityMatrix(&MStack232);
	MStack232.t[2] = 0x80;

	SetRotMatrix(&MStack232);
	SetTransMatrix(&MStack232);

	compassArrowTargetDirection = actorArray[actorCameraTracked].m4C_scriptEntity->m106_currentRotation + cameraTan + 0x400;
	compassArrowCurrentDirection = stepInterpolateDirection(compassArrowCurrentDirection, compassArrowTargetDirection, 0x40);;

	SFP_VEC4 local_48;
	local_48.vx = 0;
	local_48.vy = compassArrowCurrentDirection;
	local_48.vz = 0;

	MATRIX MStack168;
	resetMatrixTranslation(&MStack168);
	createRotationMatrix(&local_48, &MStack168);

	MulRotationMatrix(&MStack200, &MStack168);

	MStack168.t[2] = 0x1000;

	MATRIX MStack136;
	CompMatrix(&MStack232, &MStack168, &MStack136);

	if (((compassDisabled == '\0') && (updateAllEntitiesSub2Var0 == 0))) {
		if (fieldCompassVar == 0)
		{
			for (int i = 0x14; i < 0x15; i++)
			{
				// actual arrow
				drawCompassArrowSegment(&pCurrentFieldRenderingContext->m80D4_uiOT[0], &fieldCompassVar2[i], &MStack136, g_frameOddOrEven);
			}
		}
	}

	setIdentityMatrix(&MStack168);
	MulRotationMatrix(&MStack200, &MStack168);
	MStack168.t[2] = 0x1000;
	CompMatrix(&MStack232, &MStack168, &MStack136);
	MulMatrix0(&MStack232, &MStack168, &worldScaleMatrix);
	SetRotMatrix(&MStack232);
	SetTransMatrix(&MStack232);
	setIdentityMatrix(&MStack168);
	MulRotationMatrix(&cameraMatrix, &MStack168);
	MStack168.t[2] = 0x1000;
	CompMatrix(&MStack232, &MStack168, &MStack136);
	copyMatrix(&MStack232, &MStack136);
	local_48.vx = 0x400;
	local_48.vy = 0;
	local_48.vz = 0;
	MATRIX MStack104;
	createRotationMatrix(&local_48, &MStack104);

	if (((compassDisabled == '\0') && (updateAllEntitiesSub2Var0 == 0))) {
		//compass cardinal letters
		for (int i=0; i<4; i++)
		{
			setIdentityMatrix(&MStack168);
			MStack168.t[0] = compassCardinalPointsLocation[i][0];
			MStack168.t[2] = compassCardinalPointsLocation[i][1];
			CompMatrix(&MStack232, &MStack168, &MStack136);
			copyRotationMatrix(&MStack136, &MStack104);
			drawCompassLetters(&pCurrentFieldRenderingContext->m80D4_uiOT[0], &fieldCompassVar2[i + 0x10], &MStack136, g_frameOddOrEven);
		}

		// main compass
		for (int i=0; i<0x10; i++)
		{
			drawCompassArrowSegment(&pCurrentFieldRenderingContext->m80D4_uiOT[0], &fieldCompassVar2[i], &MStack232, g_frameOddOrEven);
		}

		// compass shadow
		for (int i = 0x15; i < 0x19; i++)
		{
			drawCompassArrowSegment(&pCurrentFieldRenderingContext->m80D4_uiOT[0], &fieldCompassVar2[i], &MStack232, g_frameOddOrEven);
		}
	}

	
	gDialogDrModes[g_frameOddOrEven][1].m0_pNext = pCurrentFieldRenderingContext->m80D4_uiOT[0].m0_pNext;
	pCurrentFieldRenderingContext->m80D4_uiOT[0].m0_pNext = &gDialogDrModes[g_frameOddOrEven][1];

	SetGeomOffset(0xa0, 0x70);
	SetGeomScreen(sceneSCRZ);
}

int compassGraphicDataLoaded = 0;
std::vector<u8> compassGraphicDataStaging;

struct RECT2
{
	s16 imageX;
	s16 imageY;
	s16 palX;
	s16 palY;
	s16 palW;
	s16 palH;
	//Size 0xC
};

static const std::array<RECT2, 8> compassImageRects = {
	{
		{0x2A0, 0x1C0, 0, 0xFB, 0x0, 0x0},
		{0x280, 0x1E0, 0x100, 0xF3, 0x10, 0x1},
		{0x29C, 0x1C0, 0x100, 0xF5, 0x0, 0x0},
		{0x280, 0x1C0, 0x100, 0xF2, 0x0, 0x0},
		{0x280, 0x1F0, 0x100, 0xF4, 0x10, 0x1},
		{0x3C0, 0x140, 0x100, 0xF7, 0x10, 0x1},
		{0x298, 0x1C0, 0x100, 0xF6, 0x10, 0x1},
		{0x288, 0x1C0, 0x100, 0xF6, 0x10, 0x1},
	}
};

void initCompassData()
{
	if (compassGraphicDataLoaded == 0)
	{
		compassGraphicDataStaging.resize(getFileSizeAligned(0xA7));
		flagAllocation(compassGraphicDataStaging);
		readFile(0xA7, compassGraphicDataStaging, 0, 0x80);
		waitReadCompletion(0);
	}

	unflagAllocation(compassGraphicDataStaging);
	compassGraphicDataLoaded = 0;

	MissingCode();

	std::vector<std::vector<u8>::iterator> relocatedPointers = doPointerRelocation(compassGraphicDataStaging);

	for (int i = 0; i < 8; i++)
	{
		loadTimToVram(relocatedPointers[i], compassImageRects[i].imageX, compassImageRects[i].imageY, compassImageRects[i].palX, compassImageRects[i].palY, compassImageRects[i].palW, compassImageRects[i].palH);
		DrawSync(0);
	}

	compassDataRect.x = 0;
	compassDataRect.y = 0xfb;
	compassDataRect.w = 0x10;
	compassDataRect.h = 1;
	StoreImage(&compassDataRect, compassData2);
	DrawSync(0);
	compassGraphicDataStaging.clear();
}

static const short compassShadowPositions[4][4][2] = {
	{
		{-0x600, -0x600},
		{0, -0x600},
		{-0x600, 0},
		{0, 0},
	},
	{
		{0x600, -0x600},
		{0, -0x600},
		{0x600, 0},
		{0, 0},
	},
	{
		{-0x600, 0x600},
		{0, 0x600},
		{-0x600, 0},
		{0, 0},
	},
	{
		{0x600, 0x600},
		{0, 0x600},
		{0x600, 0},
		{0, 0},
	},
};

s8 compassShadowUVs[4][8] = {
	{0,0,0x1F, 0, 0, 0x1F, 0x1F, 0x1F},
	{0,0,0x1F, 0, 0, 0x1F, 0x1F, 0x1F},
	{0,0,0x1F, 0, 0, 0x1F, 0x1F, 0x1F},
	{0,0,0x1F, 0, 0, 0x1F, 0x1F, 0x1F},
};

void initCompassShadowPoly()
{
	for (int i=0; i<4; i++)
	{
		sFieldCompassVar2& polyEntry = fieldCompassVar2[i + 0x15];
		SetPolyFT4(&polyEntry.m20_poly[0]);

		for (int j=0; j<4; j++)
		{
			polyEntry.m0[j].vx = compassShadowPositions[i][j][0];
			polyEntry.m0[j].vy = 0;
			polyEntry.m0[j].vz = compassShadowPositions[i][j][1];
		}

		polyEntry.m20_poly[0].r0 = 0x80;
		polyEntry.m20_poly[0].g0 = 0x80;
		polyEntry.m20_poly[0].b0 = 0x80;

		setPolyUV(&polyEntry.m20_poly[0], compassShadowUVs[i][0], compassShadowUVs[i][1] + 0xc0, compassShadowUVs[i][2], compassShadowUVs[i][3] + 0xc0, compassShadowUVs[i][4],
			compassShadowUVs[i][5] + 0xc0, compassShadowUVs[i][6], compassShadowUVs[i][7] + 0xc0);
		SetSemiTrans(&polyEntry.m20_poly[0], 1);

		polyEntry.m20_poly[0].tpage = GetTPage(0, 2, 0x280, 0x1c0);
		polyEntry.m20_poly[0].clut = GetClut(0x100, 0xf2);

		polyEntry.m20_poly[1] = polyEntry.m20_poly[0];
	}
}

void initCompassPoly(sFieldCompassVar2* prim, int XIndex, int ZIndex, int paletteIndex)
{
	POLY_FT4* p = &prim->m20_poly[0];
	SetPolyFT4(p);

	prim->m0[0].vx = compassInitDataX[XIndex][0];
	prim->m0[0].vy = 0;
	prim->m0[0].vz = compassInitDataZ[ZIndex][0];

	prim->m0[1].vx = compassInitDataX[XIndex][1];
	prim->m0[1].vy = 0;
	prim->m0[1].vz = compassInitDataZ[ZIndex][1];

	prim->m0[2].vx = compassInitDataX[XIndex][2];
	prim->m0[2].vy = 0;
	prim->m0[2].vz = compassInitDataZ[ZIndex][2];

	prim->m0[3].vx = compassInitDataX[XIndex][3];
	prim->m0[3].vy = 0;
	prim->m0[3].vz = compassInitDataZ[ZIndex][3];

	p->r0 = 0x80;
	p->g0 = 0x80;
	p->b0 = 0x80;

	p->tpage = GetTPage((int)compassInitDataTPage[paletteIndex][0], (int)compassInitDataTPage[paletteIndex][1], (int)compassInitDataTPage[paletteIndex][2], (int)compassInitDataTPage[paletteIndex][3]);
	p->clut = GetClut((int)compassInitDataTPage[paletteIndex][4], (int)compassInitDataTPage[paletteIndex][5]);

	setPolyUV(p,
		compassInitDataUV[XIndex][0], compassInitDataUV2[ZIndex][0],
		compassInitDataUV[XIndex][1], compassInitDataUV2[ZIndex][1],
		compassInitDataUV[XIndex][2], compassInitDataUV2[ZIndex][2],
		compassInitDataUV[XIndex][3], compassInitDataUV2[ZIndex][3]
	);

	prim->m20_poly[1] = prim->m20_poly[0];
}
