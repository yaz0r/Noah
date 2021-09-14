#include "noahLib.h"
#include "kernel/graphics.h"
#include "dialogWindows.h"
#include "fieldScriptSupport.h"

std::array<sDialogWindow, 4> gDialogWindows;
std::array<s32, 4> dialogWindowVar1;

std::array<DR_MODE, 16> gDialogDrModes;
std::array<DR_MODE, 16> gDialogDrModes2;
std::array<RECT, 16> gDialogRects;

std::array<u8, 3> dialogWindowColor;

const std::array<RECT, 8> dialogWindowsRectConfigs3 = {
	{
		{	0x20,	0xF0,	0x10,	0x10,	},
		{	0x10,	0xF8,	0x10,	0x8,	},
		{	0x30,	0xF0,	0x10,	0x10,	},
		{	0x10,	0xF0,	0x10,	0x8,	},
		{	0x40,	0xF0,	0x10,	0x10,	},
		{	0x0,	0xF0,	0x8,	0x10,	},
		{	0x50,	0xF0,	0x10,	0x10,	},
		{	0x8,	0xF0,	0x8,	0x10,	},
	}
};

std::array<RECT, 1> dialogWindowsRectConfigs2 = {
	{	0x20,	0x1C0,	0x8,	0xC	},
};

std::array<RECT, 1> dialogWindowsRectConfigs = {
	{	0x60,	0x1C0,	0xC,	0x8	},
};

void initIndividualDialogWindow(int windowIndex)
{
	SetDrawMode(&gDialogWindows[windowIndex].mC4[0], 0, 0, GetTPage(0, 2, 0x280, 0x1f0), nullptr);
	SetDrawMode(&gDialogWindows[windowIndex].mC4[1], 0, 0, GetTPage(0, 2, 0x280, 0x1f0), nullptr);

	SetTile(&gDialogWindows[windowIndex].mDC_backgroundTile[0]);
	gDialogWindows[windowIndex].mDC_backgroundTile[0].r0 = dialogWindowColor[0];
	gDialogWindows[windowIndex].mDC_backgroundTile[0].g0 = dialogWindowColor[1];
	gDialogWindows[windowIndex].mDC_backgroundTile[0].b0 = dialogWindowColor[2];
	SetSemiTrans(&gDialogWindows[windowIndex].mDC_backgroundTile[0], 1);
	gDialogWindows[windowIndex].mDC_backgroundTile[1] = gDialogWindows[windowIndex].mDC_backgroundTile[0];

	{
		RECT localRect = dialogWindowsRectConfigs[0];
		SetDrawMode(&gDialogWindows[windowIndex].m3C8[0], 0, 0, GetTPage(0, 0, 0x298, 0x1c0), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].m3C8[1], 0, 0, GetTPage(0, 0, 0x298, 0x1c0), &localRect);

		SetSprt(&gDialogWindows[windowIndex].m3E0_Sprt[0]);
		gDialogWindows[windowIndex].m3E0_Sprt[0].r0 = 0x80;
		gDialogWindows[windowIndex].m3E0_Sprt[0].g0 = 0x80;
		gDialogWindows[windowIndex].m3E0_Sprt[0].b0 = 0x80;
		gDialogWindows[windowIndex].m3E0_Sprt[0].w = 0xc;
		gDialogWindows[windowIndex].m3E0_Sprt[0].clut = GetClut(0x100, 0xf6);
		gDialogWindows[windowIndex].m3E0_Sprt[0].u0 = 0x80;
		gDialogWindows[windowIndex].m3E0_Sprt[0].v0 = 0xc0;
		gDialogWindows[windowIndex].m3E0_Sprt[0].h = 8;
		gDialogWindows[windowIndex].m3E0_Sprt[0].x0 = 0;
		gDialogWindows[windowIndex].m3E0_Sprt[0].y0 = 0;
		gDialogWindows[windowIndex].m3E0_Sprt[1] = gDialogWindows[windowIndex].m3E0_Sprt[0];
	}

	{
		RECT localRect = dialogWindowsRectConfigs2[0];
		SetDrawMode(&gDialogWindows[windowIndex].m384[0], 0, 0, GetTPage(0, 0, 0x288, 0x1c0), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].m384[1], 0, 0, GetTPage(0, 0, 0x288, 0x1c0), &localRect);

		SetSprt(&gDialogWindows[windowIndex].m39C_Sprt[0]);
		gDialogWindows[windowIndex].m39C_Sprt[0].r0 = 0x80;
		gDialogWindows[windowIndex].m39C_Sprt[0].g0 = 0x80;
		gDialogWindows[windowIndex].m39C_Sprt[0].b0 = 0x80;
		gDialogWindows[windowIndex].m39C_Sprt[0].w = 0xc;
		gDialogWindows[windowIndex].m39C_Sprt[0].clut = GetClut(0x100, 0xf6);
		gDialogWindows[windowIndex].m39C_Sprt[0].u0 = 0x80;
		gDialogWindows[windowIndex].m39C_Sprt[0].v0 = 0xc0;
		gDialogWindows[windowIndex].m39C_Sprt[0].h = 8;
		gDialogWindows[windowIndex].m39C_Sprt[0].x0 = 0;
		gDialogWindows[windowIndex].m39C_Sprt[0].y0 = 0;
		gDialogWindows[windowIndex].m39C_Sprt[1] = gDialogWindows[windowIndex].m39C_Sprt[0];
	}

	gDialogWindows[windowIndex].m40A = 2;

	for (int i=0; i<8; i++)
	{
		RECT localRect = dialogWindowsRectConfigs3[i];
		SetDrawMode(&gDialogWindows[windowIndex].mFC[0][i], 0, 0, GetTPage(0, 2, 0x280, 0x1f0), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].mFC[1][i], 0, 0, GetTPage(0, 2, 0x280, 0x1f0), &localRect);

		SetSprt(&gDialogWindows[windowIndex].m1EC[0][i]);
		gDialogWindows[windowIndex].m1EC[0][i].r0 = 0x80;
		gDialogWindows[windowIndex].m1EC[0][i].g0 = 0x80;
		gDialogWindows[windowIndex].m1EC[0][i].b0 = 0x80;
		gDialogWindows[windowIndex].m1EC[0][i].clut = GetClut(0x100, 0xf4);
		SetSemiTrans(&gDialogWindows[windowIndex].m1EC[0][i], 1);
		gDialogWindows[windowIndex].m1EC[0][i].u0 = 0x80;
		gDialogWindows[windowIndex].m1EC[0][i].v0 = 0xC0;
		gDialogWindows[windowIndex].m1EC[0][i].x0 = 0;
		gDialogWindows[windowIndex].m1EC[0][i].y0 = 0;
		gDialogWindows[windowIndex].m1EC[0][i].w = dialogWindowsRectConfigs3[i].w;
		gDialogWindows[windowIndex].m1EC[0][i].h = dialogWindowsRectConfigs3[i].h;

		gDialogWindows[windowIndex].m1EC[1][i] = gDialogWindows[windowIndex].m1EC[0][i];
	}

	{
		RECT localRect = { 0,0,0xFF,0xFF };
		SetDrawMode(&gDialogWindows[windowIndex].m42C_FaceDrawMode[0], 0, 0, GetTPage(1, 0, 0x2c0, 0x100), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].m42C_FaceDrawMode[1], 0, 0, GetTPage(1, 0, 0x2c0, 0x100), &localRect);

		SetPolyFT4(&gDialogWindows[windowIndex].m444_FacePoly[0]);
		gDialogWindows[windowIndex].m444_FacePoly[0].r0 = 0x80;
		gDialogWindows[windowIndex].m444_FacePoly[0].g0 = 0x80;
		gDialogWindows[windowIndex].m444_FacePoly[0].b0 = 0x80;
		gDialogWindows[windowIndex].m444_FacePoly[0].clut = GetClut(0, 0xe0);
		gDialogWindows[windowIndex].m444_FacePoly[0].tpage = GetTPage(1, 0, 0x2c0, 0x100);
		gDialogWindows[windowIndex].m444_FacePoly[1] = gDialogWindows[windowIndex].m444_FacePoly[0];
	}
}

void initDialogWindows()
{
	RECT localRect;
	localRect.y = 0;
	localRect.x = 0;
	localRect.h = 0xff;
	localRect.w = 0xff;

	for (int i=0; i<16; i++)
	{
		gDialogRects[i].y = 0;
		gDialogRects[i].x = 0;
		gDialogRects[i].h = 0xff;
		gDialogRects[i].w = 0xff;
		gDialogRects[i].y = 0;

		gDialogRects[i].x = 0;
		gDialogRects[i].h = 0xff;
		gDialogRects[i].w = 0xff;

		SetDrawMode(&gDialogDrModes[i], 0, 0, GetTPage(0, 0, 0x380, 0x100), &gDialogRects[i]);
		SetDrawMode(&gDialogDrModes2[i], 0, 0, GetTPage(0, 0, 0x380, 0x100), &gDialogRects[i]);
	}

	for (int i=0; i<4; i++)
	{
		gDialogWindows[i].m416_fieldActorId = -1;
		gDialogWindows[i].m418_actorId = -1;
		gDialogWindows[i].m37C = -1;
		gDialogWindows[i].m3C4 = -1;
		gDialogWindows[i].m40E= -1;
		gDialogWindows[i].m414 = -1;
		gDialogWindows[i].m410 = -1;
		gDialogWindows[i].m412 = 0;

		initIndividualDialogWindow(i);

		dialogWindowVar1[i] = -1;

		SetDrawMode(&gDialogWindows[i].m0[0], 0, 0, GetTPage(0, 0, 0x300, 0x100), &localRect);
		SetDrawMode(&gDialogWindows[i].m0[1], 0, 0, GetTPage(0, 0, 0x300, 0x100), &localRect);
	}
}

int findDialogWindowForCurrentActor(int* outputId)
{
	for (int i = 0; i < 4; i++)
	{
		if ((gDialogWindows[i].m416_fieldActorId == currentFieldActorId) && (gDialogWindows[i].m40E == 0))
		{
			*outputId = i;
			return 0;
		}
	}
	return -1;
}

int findFreeDialogWindow()
{
	for (int i=0; i<4; i++)
	{
		if (gDialogWindows[i].m410 == -1)
			return 0;
	}
	return -1;
}

int allocateDialogWindow()
{
	for (int i = 0; i < 4; i++)
	{
		if (gDialogWindows[i].m410 != -1)
		{
			gDialogWindows[i].m410++;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (gDialogWindows[i].m410 == -1)
		{
			gDialogWindows[i].m410 = 0;
			return i;
		}
	}

	return -1;
}

s32 createDialogWindowCounter = 0;

void setupDialogWindowFacePoly(int windowIndex, int setupIndex)
{
	static const std::array<sVec2_s16, 4> dialogWindowFacePolySetups = { {
		{ 0x3F, 0 },
		{ 0, 0 },
		{ 0x3F, 0x40 },
		{ 0, 0x40 },
	} };

	gDialogWindows[windowIndex].m444_FacePoly[0].u0 = dialogWindowFacePolySetups[setupIndex].vx;
	gDialogWindows[windowIndex].m444_FacePoly[1].u0 = dialogWindowFacePolySetups[setupIndex].vx;

	gDialogWindows[windowIndex].m444_FacePoly[0].v0 = dialogWindowFacePolySetups[setupIndex].vy;
	gDialogWindows[windowIndex].m444_FacePoly[1].v0 = dialogWindowFacePolySetups[setupIndex].vy;

	gDialogWindows[windowIndex].m444_FacePoly[0].u1 = dialogWindowFacePolySetups[setupIndex].vx + 0x40;
	gDialogWindows[windowIndex].m444_FacePoly[1].u1 = dialogWindowFacePolySetups[setupIndex].vx + 0x40;

	gDialogWindows[windowIndex].m444_FacePoly[0].v1 = dialogWindowFacePolySetups[setupIndex].vy;
	gDialogWindows[windowIndex].m444_FacePoly[1].v1 = dialogWindowFacePolySetups[setupIndex].vy;

	gDialogWindows[windowIndex].m444_FacePoly[0].u2 = dialogWindowFacePolySetups[setupIndex].vx;
	gDialogWindows[windowIndex].m444_FacePoly[1].u2 = dialogWindowFacePolySetups[setupIndex].vx;

	gDialogWindows[windowIndex].m444_FacePoly[0].v2 = dialogWindowFacePolySetups[setupIndex].vy + 0x40;
	gDialogWindows[windowIndex].m444_FacePoly[1].v2 = dialogWindowFacePolySetups[setupIndex].vy + 0x40;

	gDialogWindows[windowIndex].m444_FacePoly[0].u3 = dialogWindowFacePolySetups[setupIndex].vx + 0x40;
	gDialogWindows[windowIndex].m444_FacePoly[1].u3 = dialogWindowFacePolySetups[setupIndex].vx + 0x40;

	gDialogWindows[windowIndex].m444_FacePoly[0].v3 = dialogWindowFacePolySetups[setupIndex].vy + 0x40;
	gDialogWindows[windowIndex].m444_FacePoly[1].v3 = dialogWindowFacePolySetups[setupIndex].vy + 0x40;

	gDialogWindows[windowIndex].m444_FacePoly[0].clut = GetClut(0, setupIndex + 0xe0);
	gDialogWindows[windowIndex].m444_FacePoly[1].clut = GetClut(0, setupIndex + 0xe0);
}

void setupWindowSize1(int windowIndex, short x, short y, short width, short height)
{
	gDialogWindows[windowIndex].mAC_Position.vx = x;
	gDialogWindows[windowIndex].mAC_Position.vy = y;
	gDialogWindows[windowIndex].mB0_Size.vx = width;
	gDialogWindows[windowIndex].mB0_Size.vy = height;
}

s16 textSpriteMode0 = 0;
s16 textSpriteMode1 = 0;

void setupWindowSize2(sDialogWindow18* param_1, int x1, int y1, short x2, short y2, short width, short height)
{
	(param_1->m4).vx = x2;
	param_1->m10_flags = 0;
	param_1->m84 = 0;
	param_1->m8C = nullptr;
	param_1->m82 = 0;
	(param_1->m14).vx = 0xe;
	param_1->m68 = 1;
	param_1->m69 = 1;
	param_1->mA_width1 = width;
	param_1->m6C = 0;
	param_1->m6A = 0;
	param_1->m6D = 0;
	param_1->m6B = 0;
	param_1->m6E_selectedLineForMultiChoice = -1;
	int sVar16 = (short)y1;
	param_1->mE = sVar16;
	(param_1->m4).vy = y2;
	param_1->mC_height = height;
	param_1->mA_width1 |= 1;
	param_1->m8_widthTimes4 = (short)(param_1->mA_width1 << 2);
	param_1->m12_widthPadded = param_1->mA_width1 + 3;

	traceNextAlloc(0x29);
	param_1->m28_perLineBuffer.resize(param_1->mC_height);

	traceNextAlloc(0x28);
	param_1->m2C_inRamDialogTextImage.resize(param_1->m12_widthPadded * 0x1c);

	param_1->m48_textTile[0].tag.m3_size = 3;
	param_1->m48_textTile[0].r0 = 0;
	param_1->m48_textTile[0].g0 = 0;
	param_1->m48_textTile[0].b0 = 0;
	param_1->m48_textTile[0].code = 0x60;
	param_1->m48_textTile[0].x0 = param_1->m4.vx - 7;
	param_1->m48_textTile[0].y0 = param_1->m4.vy - 5;
	param_1->m48_textTile[0].w = param_1->mA_width1 * 0xD;
	param_1->m48_textTile[0].h = param_1->mC_height * (param_1->m14).vx + 10;
	SetSemiTrans(&param_1->m48_textTile[0], 1);
	param_1->m48_textTile[1] = param_1->m48_textTile[0];

	if (0 < param_1->mC_height)
	{
		for (int i=0; i<param_1->mC_height; i++)
		{
			param_1->m28_perLineBuffer[i].m0[0][0].x0 = param_1->m4.vx;
			param_1->m28_perLineBuffer[i].m0[0][0].y0 = param_1->m4.vx * i + param_1->m4.vy;
			param_1->m28_perLineBuffer[i].m0[0][1].x0 = param_1->m4.vx + 0x100;
			param_1->m28_perLineBuffer[i].m0[0][1].y0 = param_1->m4.vx * i + param_1->m4.vy;

			if (param_1->m8_widthTimes4 < 0x101) {
				param_1->m28_perLineBuffer[i].m0[0][0].h = 0xD;
				param_1->m28_perLineBuffer[i].m0[0][0].w = param_1->m8_widthTimes4;
			}
			else
			{
				param_1->m28_perLineBuffer[i].m0[0][0].h = 0xD;
				param_1->m28_perLineBuffer[i].m0[0][0].w = 0x100;
			}

			if (param_1->m8_widthTimes4 < 0x101) {
				param_1->m28_perLineBuffer[i].m0[0][1].h = 0xD;
				param_1->m28_perLineBuffer[i].m0[0][1].w = 0;
			}
			else
			{
				param_1->m28_perLineBuffer[i].m0[0][1].h = 0xD;
				param_1->m28_perLineBuffer[i].m0[0][1].w = param_1->m8_widthTimes4 - 0xF0;
			}

			int yFraq = (y1 + (i / 2) * 0xD);

			param_1->m28_perLineBuffer[i].m0[0][1].u0 = param_1->m28_perLineBuffer[i].m0[0][0].u0 = (x1 & 0x3F) << 2;
			param_1->m28_perLineBuffer[i].m0[0][1].v0 = param_1->m28_perLineBuffer[i].m0[0][0].v0 = (yFraq & 0xFF) << 8;

			param_1->m28_perLineBuffer[i].m0[0][1].tag.m3_size = param_1->m28_perLineBuffer[i].m0[0][0].tag.m3_size = 4;
			param_1->m28_perLineBuffer[i].m0[0][1].code = param_1->m28_perLineBuffer[i].m0[0][0].code = 0x64; // setup as Textured Rectangle, variable size, opaque, texture-blending

			param_1->m28_perLineBuffer[i].m0[0][0].code |= 1; // switch to Textured Rectangle, variable size, opaque, raw-texture
			param_1->m28_perLineBuffer[i].m0[0][1].code |= 1;

			param_1->m28_perLineBuffer[i].m0[1][0] = param_1->m28_perLineBuffer[i].m0[0][0];
			param_1->m28_perLineBuffer[i].m0[1][1] = param_1->m28_perLineBuffer[i].m0[0][1];

			param_1->m28_perLineBuffer[i].m50_x = x1;
			param_1->m28_perLineBuffer[i].m52_y = yFraq;
			param_1->m28_perLineBuffer[i].m54_width = param_1->m12_widthPadded;
			param_1->m28_perLineBuffer[i].m56_height = 0xD;
			param_1->m28_perLineBuffer[i].m58 = 0;
			if ((i & 1) == 0)
			{
				param_1->m28_perLineBuffer[i].m5E = textSpriteMode0;
			}
			else
			{
				param_1->m28_perLineBuffer[i].m5E = textSpriteMode1;
			}
			param_1->m28_perLineBuffer[i].m5C = y1 + (i / 2) * 0xD;
			param_1->m28_perLineBuffer[i].m5A = i & 1;
			param_1->m28_perLineBuffer[i].m5B = i;
		}
		assert(0);
	}

	SetDrawMode(&param_1->m30_textTileDrawMode[0], 0, 0, GetTPage(0, 0, x1, y1), nullptr);
	SetDrawMode(&param_1->m30_textTileDrawMode[1], 0, 0, GetTPage(0, 0, x1 + 0x40, y1), nullptr);
}

const std::vector<sVec2_s16> windowsDefaultPositions = {
	{0x300, 0x100},
	{0x300, 0x180},
	{0x300, 0x134},
	{0x300, 0x1B4},
};

s16 dialogWindowFlag1 = 0;


std::vector<u8>::iterator getDialogParamPointer(std::vector<u8>& buffer, int param_2)
{
	return buffer.begin() + READ_LE_U16(buffer.begin() + param_2 * 2 + 4);
}

void createDialogWindow(short x, short y, int dialogIndex, int windowIndex, int width, int height, int fieldActorId, int actorId, int mode, uint param_10, uint param_11)
{
	int y2 = y - 8;
	u32 flagUpper = actorArray[fieldActorId].m4C_scriptEntity->m84 >> 16;
	if (flagUpper == 0)
	{
		flagUpper = actorArray[fieldActorId].m4C_scriptEntity->m84 & 0xFFFF;
	}
	flagUpper |= param_10;

	for (int i = 0; i < 4; i++)
	{
		int counterEntry = createDialogWindowCounter & 3;
		createDialogWindowCounter++;
		if (dialogWindowVar1[counterEntry] == -1) {
			dialogWindowVar1[counterEntry] = 0;
			break;
		}
	}

	gDialogWindows[windowIndex].m18.m70[0] = getVariable(0x16);
	gDialogWindows[windowIndex].m18.m70[1] = getVariable(0x18);
	gDialogWindows[windowIndex].m18.m70[2] = getVariable(0x1a);
	gDialogWindows[windowIndex].m18.m70[3] = getVariable(0x1c);
	gDialogWindows[windowIndex].m18.m80 = gDialogWindows[windowIndex].m18.m70[3];

	int projectedPosition[2];
	if (mode == 2) {
		projectedPosition[0] = 0xa0;
		projectedPosition[1] = y2 + 0x20;
	}
	else {
		if (mode == 3) {
			projectedPosition[0] = x + 8 + width * 2;
			projectedPosition[1] = y2 + 8 + height * 7;
		}
		else {
			projectActorToScreen(actorId, &projectedPosition[0], &projectedPosition[1], -0x40);
		}
	}

	sFieldScriptEntity* pScriptEntity = actorArray[fieldActorId].m4C_scriptEntity;
	if ((pScriptEntity->m80_DialogAvatarFace == 0xff) || ((flagUpper & 2) != 0)) {
		gDialogWindows[windowIndex].m495_dialogFaceId = 0x80;
		gDialogWindows[windowIndex].m494_hasDialogFace = 0;
	}
	else {
		u32 windowSetupIndex;
		if ((flagUpper & 0x402) == 0) {
			windowSetupIndex = pScriptEntity->m12C_flags >> 1 & 0xe | 1;
		}
		else {
			windowSetupIndex = pScriptEntity->m12C_flags >> 1 & 0xe;
		}
		setupDialogWindowFacePoly(windowIndex, windowSetupIndex);
		gDialogWindows[windowIndex].m494_hasDialogFace = 1;
		gDialogWindows[windowIndex].m495_dialogFaceId = (actorArray[fieldActorId].m4C_scriptEntity)->m80_DialogAvatarFace;
	}

	gDialogWindows[windowIndex].m37C = -1;
	setupWindowSize1(windowIndex, (int)x, (int)y2, width * 4 + 0x10, height * 0xe + 0x10);
	int xOffset = 0;
	if ((actorArray[fieldActorId].m4C_scriptEntity)->m80_DialogAvatarFace != 0xff) {
		xOffset = -(ushort)((flagUpper & 0x402) == 0) & 0x44;
	}
	setupWindowSize2(&gDialogWindows[windowIndex].m18, windowsDefaultPositions[windowIndex].vx, windowsDefaultPositions[windowIndex].vy, x + xOffset + 8, y, width, height);
	if ((flagUpper & 0x400) != 0) {
		gDialogWindows[windowIndex].m40C_flags = gDialogWindows[windowIndex].m40C_flags | 0x20;
	}

	if ((flagUpper & 0x400) != 0) {
		gDialogWindows[windowIndex].m40C_flags = gDialogWindows[windowIndex].m40C_flags | 0x20;
	}

	int bVar3 = 2;
	if (dialogWindowFlag1 == 8) {
		bVar3 = 1;
	}
	gDialogWindows[windowIndex].m18.m68 = bVar3;
	gDialogWindows[windowIndex].m18.m90_dialogPointer = getDialogParamPointer(rawFieldDialogBundle, dialogIndex);
	gDialogWindows[windowIndex].m40E = 0;
	gDialogWindows[windowIndex].m18.m10_flags |= 2;
	gDialogWindows[windowIndex].m416_fieldActorId = fieldActorId;
	gDialogWindows[windowIndex].m418_actorId = actorId;
	gDialogWindows[windowIndex].m408_openAnimationCounter = dialogWindowFlag1;

	if ((param_11 & 0x800) == 0) {
		gDialogWindows[windowIndex].m412 = 0;
	}
	else {
		gDialogWindows[windowIndex].m412 = 1;
	}

	gDialogWindows[windowIndex].m41C[0] = ((projectedPosition[0] - (width * 2 + 8)) - (int)x) * 0x10000;
	gDialogWindows[windowIndex].m41C[1] = ((projectedPosition[1] - (height * 7 + 8)) - (int)y2) * 0x10000;

	if ((flagUpper & 0x100) == 0) {
		gDialogWindows[windowIndex].m424[0] = -(gDialogWindows[windowIndex].m41C[0] / (int)dialogWindowFlag1);
		gDialogWindows[windowIndex].m424[1] = -(gDialogWindows[windowIndex].m41C[1] / (int)dialogWindowFlag1);
	}
	else {
		gDialogWindows[windowIndex].m408_openAnimationCounter = 1;
		gDialogWindows[windowIndex].m424[0] = -gDialogWindows[windowIndex].m41C[0];
		gDialogWindows[windowIndex].m424[1] = -gDialogWindows[windowIndex].m41C[1];
	}
	if ((((actorArray[actorId].m4C_scriptEntity)->m4_flags & 0x200) != 0) && ((flagUpper & 1) == 0)) {
		gDialogWindows[windowIndex].m414 = 0;
	}
}

void releaseDialogWindow(s16 windowIndex)
{
	if (gDialogWindows[windowIndex].m40E == 0) {
		assert(0);
	}
}

void stepDialogWindows()
{
	for (int i=0; i<4; i++)
	{
		if (gDialogWindows[i].m40E == 0)
		{
			if ((gDialogWindows[i].m408_openAnimationCounter == 0) && ((gDialogWindows[i].m18.m10_flags & 4) == 0)) {
				releaseDialogWindow(i);
			}
			if (gDialogWindows[i].m414 == 0) {
				releaseDialogWindow(i);
			}
			if (gDialogWindows[i].m408_openAnimationCounter != 0) {
				gDialogWindows[i].m408_openAnimationCounter--;
			}
		}
	}
}

s32 dialogWindowsFrameCount = 0;
s32 dialogWindowsFrameCount2 = 0;

int getWindowSubStructX(sDialogWindow18* param_1)
{
	return (int)(param_1->m4).vx + (param_1->m0).vx * 4;
}

int getWindowSubStructY(sDialogWindow18* param_1)
{
	int iVar1;

	iVar1 = (int)(param_1->m0).vy - (int)(param_1->m14).vy;
	if (iVar1 < 0) {
		iVar1 = param_1->mC_height + -1;
	}
	return (int)(param_1->m4).vy + iVar1 * (param_1->m14).vx;
}

void setupFacePolyPosition(POLY_FT4* poly, short x, short y, short width, short height, int mode)
{
	short sVar1;

	if (mode == 0) {
		sVar1 = x + -1;
		poly->x0 = sVar1;
		poly->x1 = sVar1 + width;
		poly->x2 = sVar1;
		poly->x3 = sVar1 + width;
	}
	else {
		poly->x1 = x;
		poly->x0 = x + width;
		poly->x3 = x;
		poly->x2 = x + width;
	}
	poly->y0 = y;
	poly->y1 = y;
	poly->y2 = y + height;
	poly->y3 = y + height;
	return;
}

void dialogWindowSetupForRendering(sTag* OT, int oddOrEven, int windowIndex)
{
	if (gDialogWindows[windowIndex].m40E == 0)
	{
		int windowX = (int)gDialogWindows[windowIndex].mAC_Position.vx;
		int windowY = (int)gDialogWindows[windowIndex].mAC_Position.vy;
		int windowWidth2 = (uint)(ushort)gDialogWindows[windowIndex].mB0_Size.vx << 0x10;
		int windowWidth = windowWidth2 >> 0x10;
		int windowHeight2 = (uint)(ushort)gDialogWindows[windowIndex].mB0_Size.vy << 0x10;
		int windowHeight = windowHeight2 >> 0x10;

		// scale window down during opening animation
		if (gDialogWindows[windowIndex].m408_openAnimationCounter != 0) {
			MissingCode();
		}

		if ((((gDialogWindows[windowIndex].m3C4 == 0) && (gDialogWindows[windowIndex].m410 == 0)) && (gDialogWindows[windowIndex].m408_openAnimationCounter == 0)) &&
			(((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0 && (gDialogWindows[windowIndex].m37C != 0)))) {
			if (gDialogWindows[windowIndex].m40A == 0) {
				sDialogWindow18* windowSubStruct = &gDialogWindows[windowIndex].m18;
				int sVar2 = getWindowSubStructX(windowSubStruct);
				int sVar3 = getWindowSubStructY(windowSubStruct);
				RECT local_90;
				local_90.x = dialogWindowsRectConfigs[dialogWindowsFrameCount2].x;
				local_90.y = dialogWindowsRectConfigs[dialogWindowsFrameCount2].y;
				local_90.w = dialogWindowsRectConfigs[dialogWindowsFrameCount2].w;
				local_90.h = dialogWindowsRectConfigs[dialogWindowsFrameCount2].h;
				u16 TPage = GetTPage(0, 0, 0x298, 0x1c0);
				SetDrawMode(&gDialogWindows[windowIndex].m3C8[oddOrEven], 0, 0, (uint)TPage, &local_90);
				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].x0 = sVar2;
				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].y0 = sVar3 + 4;

				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].tag.m0_pNext = OT->m0_pNext;
				OT->m0_pNext = &gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].tag;

				gDialogWindows[windowIndex].m3C8[oddOrEven].tag.m0_pNext = &gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].tag;
				OT->m0_pNext = &gDialogWindows[windowIndex].m3C8[oddOrEven].tag;
			}
			else {
				gDialogWindows[windowIndex].m40A--;
			}
		}
		else {
			gDialogWindows[windowIndex].m40A = 2;
		}

		// setup the sprites
		{
			int sVar8 = windowX + -8;
			gDialogWindows[windowIndex].m1EC[oddOrEven][0].x0 = sVar8;
			int sVar10 = windowY + -7;
			gDialogWindows[windowIndex].m1EC[oddOrEven][0].y0 = sVar10;
			gDialogWindows[windowIndex].m1EC[oddOrEven][2].y0 = sVar10;
			gDialogWindows[windowIndex].m1EC[oddOrEven][4].x0 = sVar8;
			int sVar6 = windowX + windowWidth + -8;
			gDialogWindows[windowIndex].m1EC[oddOrEven][2].x0 = sVar6;
			gDialogWindows[windowIndex].m1EC[oddOrEven][6].x0 = sVar6;
			gDialogWindows[windowIndex].m1EC[oddOrEven][3].x0 = sVar8;
			gDialogWindows[windowIndex].m1EC[oddOrEven][1].x0 = sVar6;
			int sVar7 = windowY + (short)windowHeight + -9;
			gDialogWindows[windowIndex].m1EC[oddOrEven][4].y0 = sVar7;
			gDialogWindows[windowIndex].m1EC[oddOrEven][6].y0 = sVar7;
			gDialogWindows[windowIndex].m1EC[oddOrEven][3].y0 = windowY + 9;
			gDialogWindows[windowIndex].m1EC[oddOrEven][1].y0 = windowY + 9;
			int windowHeight2 = windowHeight + -0x12;
			if (windowHeight2 < 0) {
				windowHeight2 = 0;
			}
			gDialogWindows[windowIndex].m1EC[oddOrEven][3].h = (short)windowHeight2;
			gDialogWindows[windowIndex].m1EC[oddOrEven][1].h = (short)windowHeight2;
			gDialogWindows[windowIndex].m1EC[oddOrEven][5].y0 = sVar10;
			gDialogWindows[windowIndex].m1EC[oddOrEven][5].x0 = windowX + 8;
			gDialogWindows[windowIndex].m1EC[oddOrEven][7].x0 = windowX + 8;
			gDialogWindows[windowIndex].m1EC[oddOrEven][7].y0 = sVar7;
			gDialogWindows[windowIndex].m1EC[oddOrEven][5].w = windowWidth + -0x10;
			gDialogWindows[windowIndex].m1EC[oddOrEven][7].w = windowWidth + -0x10;

			// chain the sprites
			if ((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0) {
				for (int i = 0; i < 8; i++)
				{
					gDialogWindows[windowIndex].m1EC[oddOrEven][i].tag.m0_pNext = OT->m0_pNext;
					OT->m0_pNext = &gDialogWindows[windowIndex].m1EC[oddOrEven][i].tag;

					gDialogWindows[windowIndex].mFC[oddOrEven][i].tag.m0_pNext = &gDialogWindows[windowIndex].m1EC[oddOrEven][i].tag;
					OT->m0_pNext = &gDialogWindows[windowIndex].mFC[oddOrEven][i].tag;
				}
			}
		}

		// Setup the face poly
		{
			int faceWidth = 0x40;
			if (windowWidth + -4 < 0x40) {
				faceWidth = windowWidth + -8;
			}
			int faceHeight = 0x40;
			if (windowHeight + -4 < 0x40) {
				faceHeight = (short)windowHeight + -8;
			}
			int polyX = windowX + 4;
			if ((gDialogWindows[windowIndex].m40C_flags & 0x20U) != 0) {
				polyX = ((windowX + windowWidth) - faceWidth) + -4;
			}

			setupFacePolyPosition(&gDialogWindows[windowIndex].m444_FacePoly[oddOrEven], polyX, windowY + 4, faceWidth, faceHeight, (ushort)gDialogWindows[windowIndex].m40C_flags & 0x20);

			// chain the face poly
			if (gDialogWindows[windowIndex].m494_hasDialogFace == 1) {
				gDialogWindows[windowIndex].m444_FacePoly[oddOrEven].tag.m0_pNext = OT->m0_pNext;
				OT->m0_pNext = &gDialogWindows[windowIndex].m444_FacePoly[oddOrEven].tag;

				gDialogWindows[windowIndex].m42C_FaceDrawMode[oddOrEven].tag.m0_pNext = &gDialogWindows[windowIndex].m444_FacePoly[oddOrEven].tag;
				OT->m0_pNext = &gDialogWindows[windowIndex].m42C_FaceDrawMode[oddOrEven].tag;
			}
		}

		// Sprite to highlight text (selected text in multi-selection)
		if (((gDialogWindows[windowIndex].m37C == 0) && (gDialogWindows[windowIndex].m410 == 0)) && (gDialogWindows[windowIndex].m408_openAnimationCounter == 0)) {
			assert(0);
			MissingCode();
		}

		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].x0 = (short)windowX;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].y0 = (short)windowY + 1;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].w = (short)windowWidth;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].h = (short)windowHeight + -2;

		if ((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0) {
			gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].tag.m0_pNext = OT->m0_pNext;
			OT->m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].tag;

			gDialogWindows[windowIndex].mC4[oddOrEven].tag.m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].tag;
			OT->m0_pNext = &gDialogWindows[windowIndex].mC4[oddOrEven].tag;
		}
	}
}

void clearSelectedLineForMultiChoice(sDialogWindow18* param_1)
{
	param_1->m6E_selectedLineForMultiChoice = -1;
	return;
}

void updateMultiChoice(int param_1)
{
	if ((gDialogWindows[param_1].m37C == 0) && (gDialogWindows[param_1].m408_openAnimationCounter == 0)) {
		if (gDialogWindows[param_1].m410 == 0) {
			assert(0);
		}
		else
		{
			clearSelectedLineForMultiChoice(&gDialogWindows[param_1].m18);
		}
	}
}

s8 addDialogWindowsToOTSub0(sDialogWindow18* param_1)
{
	if ((param_1->m10_flags & 8) != 0) {
		return param_1->m6B;
	}
	return 0;
}

u16 padButtonForDialogs = 0;

void addDialogWindowsToOTSub1(sDialogWindow18* param_1)
{
	ushort uVar1;

	uVar1 = param_1->m10_flags;
	param_1->m10_flags = uVar1 & 0xfff7;
	if ((uVar1 & 0x200) != 0) {
		param_1->m84 = 0;
		param_1->m6C = 0;
		param_1->m10_flags = param_1->m10_flags & 0xfdff;
	}
	return;
}

void updateAndRenderTextForDialogWindow(sDialogWindow18* param_1, sTag* OT, int oddOrEven)
{
	if ((param_1->m10_flags & 4) == 0) {
		assert(0);
	}
	if ((param_1->m10_flags & 0x100) == 0) {
		param_1->m69 = param_1->m68;
	}
	else {
		param_1->m69 = param_1->m68 * 3;
	}

	if (((param_1->m10_flags & 0x40) != 0) && ((param_1->m10_flags & 8) == 0)) {
		param_1->m10_flags = param_1->m10_flags & 0xffbf | 0x20;
	}

	if ((param_1->m10_flags & 0x20) != 0) {
		assert(0);
	}

	if (0 < param_1->mC_height) {
		assert(0);
	}
}

void addDialogWindowsToOT(sTag* OT, int oddOrEven)
{
	dialogWindowsFrameCount = dialogWindowsFrameCount + 1;
	if ((dialogWindowsFrameCount & 3) == 0) {
		dialogWindowsFrameCount2 = dialogWindowsFrameCount2 + 1;
	}
	if (4 < dialogWindowsFrameCount2) {
		dialogWindowsFrameCount2 = 0;
	}

	int lastWindowWith412 = 0xFF;
	for (int i=0; i<4; i++)
	{
		if (gDialogWindows[i].m412)
		{
			lastWindowWith412 = i;
		}
	}

	MissingCode();

	for (int i=0; i<4; i++)
	{
		if ((gDialogWindows[i].m40E == 0) && gDialogWindows[i].m412)
		{
			gDialogWindows[i].m3C4 = -1;
			if (gDialogWindows[i].m408_openAnimationCounter == 0)
			{
				if ((addDialogWindowsToOTSub0(&gDialogWindows[i].m18) != 0) && (gDialogWindows[i].m37C != 0)) {
					gDialogWindows[i].m3C4 = 0;
				}

				if ((padButtonForDialogs & 0x20) != 0) {
					gDialogWindows[i].m37C = 0xffff;
					actorArray[gDialogWindows[i].m416_fieldActorId].m4C_scriptEntity->m81_selectedMultichoiceLine = gDialogWindows[i].m382_CurrentMultiChoiceSelection + gDialogWindows[i].m37E_startOfMultiChoice;
					addDialogWindowsToOTSub1(&gDialogWindows[i].m18);
				}
				if (gDialogWindows[i].m18.m82 == 0) {
					assert(0);
				}
				updateAndRenderTextForDialogWindow(&gDialogWindows[i].m18, OT, oddOrEven);
			}
			gDialogWindows[i].m0[oddOrEven].tag.m0_pNext = OT->m0_pNext;
			OT->m0_pNext = &gDialogWindows[i].m0[oddOrEven].tag;
			dialogWindowSetupForRendering(OT, oddOrEven, i);
			updateMultiChoice(i);
		}
	}

	std::array<s32, 4> processedWindowsArray = { 0,0,0,0 };
	int  counterOfProcessedWindows = 0;

	for (int j = 0; j < 4; j++)
	{
		int secondCounterOfProcessedWindows = 0;
		for (int i = 0; i < 4; i++)
		{
			if (gDialogWindows[i].m410 == j)
			{
				processedWindowsArray[secondCounterOfProcessedWindows] = counterOfProcessedWindows++;

				if ((gDialogWindows[i].m40E == 0) && (i != lastWindowWith412))
				{
					gDialogWindows[i].m3C4 = -1;
					if (gDialogWindows[i].m408_openAnimationCounter == 0)
					{
						if ((addDialogWindowsToOTSub0(&gDialogWindows[i].m18) != 0) && (gDialogWindows[i].m37C != 0)) {
							gDialogWindows[i].m3C4 = 0;
						}

						if (((padButtonForDialogs & 0x20) != 0) && (gDialogWindows[i].m410 == 0)) {
							gDialogWindows[i].m37C = 0xffff;
							actorArray[gDialogWindows[i].m416_fieldActorId].m4C_scriptEntity->m81_selectedMultichoiceLine = gDialogWindows[i].m382_CurrentMultiChoiceSelection + gDialogWindows[i].m37E_startOfMultiChoice;
							addDialogWindowsToOTSub1(&gDialogWindows[i].m18);
						}
						if (gDialogWindows[i].m18.m82 == 0) {
							assert(0);
						}
						updateAndRenderTextForDialogWindow(&gDialogWindows[i].m18, OT, oddOrEven);

						if ((addDialogWindowsToOTSub0(&gDialogWindows[i].m18) != 0) && (gDialogWindows[i].m37C != 0)) {
							gDialogWindows[i].m3C4 = 0;
						}
					}
					gDialogWindows[i].m0[oddOrEven].tag.m0_pNext = OT->m0_pNext;
					OT->m0_pNext = &gDialogWindows[i].m0[oddOrEven].tag;
					dialogWindowSetupForRendering(OT, oddOrEven, i);
					updateMultiChoice(i);
				}
				if (gDialogWindows[i].m410 == -1)
				{
					processedWindowsArray[secondCounterOfProcessedWindows] = -1;
				}
			}
			secondCounterOfProcessedWindows++;
		}
	}

	for (int i=0; i<4; i++)
	{
		gDialogWindows[i].m410 = processedWindowsArray[i];
	}

	gDialogDrModes[g_frameOddOrEven].tag.m0_pNext = OT->m0_pNext;
	OT->m0_pNext = &gDialogDrModes[g_frameOddOrEven].tag;
}
