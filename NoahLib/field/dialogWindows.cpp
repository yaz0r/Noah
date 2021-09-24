#include "noahLib.h"
#include "kernel/graphics.h"
#include "kernel/font.h"
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

int getWindowWithLowestPriority()
{
	int bestValue = 0;
	s16 index =-1;
	for (int i = 0; i < 4; i++)
	{
		s16 currentValue = gDialogWindows[0].m410;
		if ((currentValue != -1) && (bestValue <= currentValue)) {
			index = i;
			bestValue = currentValue;
		}
	}
	return index;
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
	param_1->m2C_inRamDialogTextImage.resize(param_1->m12_widthPadded * 14);

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

			param_1->m28_perLineBuffer[i].m50.x = x1;
			param_1->m28_perLineBuffer[i].m50.y = yFraq;
			param_1->m28_perLineBuffer[i].m50.w = param_1->m12_widthPadded;
			param_1->m28_perLineBuffer[i].m50.h = 0xD;
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
	if ((pScriptEntity->m80_DialogAvatarFace == -1) || ((flagUpper & 2) != 0)) {
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

void addDialogWindowsToOTSub2(sDialogWindow18* param_1, const std::vector<u8>::iterator& param_2)
{
	sDialogWindow18_8C* psVar1;
	sDialogWindow18_8C* psVar2;

	psVar2 = param_1->m8C;
	param_1->m82 = param_1->m82 + 1;
	traceNextAlloc(0x2a);
	psVar1 = new sDialogWindow18_8C;
	psVar1->m4_dialogPointer = param_2;
	psVar1->m0_pNext = nullptr;
	if (psVar2 == nullptr) {
		param_1->m8C = psVar1;
	}
	else {
		for (; psVar2->m0_pNext != nullptr; psVar2 = psVar2->m0_pNext) {
		}
		psVar2->m0_pNext = psVar1;
	}
}

void AddPrim(sTag* ot, sTag* p)
{
	p->m0_pNext = ot->m0_pNext;
	ot->m0_pNext = p;
}

void addSPRT(sTag* ot, sTag* p)
{
	p->m0_pNext = ot->m0_pNext;
	p->m3_size |= 4;
	ot->m0_pNext = p;
}

int getDialogFontCharacterWidth(uint param_1, uint param_2)
{
	bool bVar1;
	int iVar2;

	if ((param_1 & 0xffff) == 0) {
		bVar1 = (int)((param_2 & 0xffff) - dialogFontVar1) < dialogFontVar2;
	}
	else {
		if ((param_1 & 0xffff) != dialogFontVar0) {
			return 3;
		}
		bVar1 = (int)(param_2 & 0xffff) < dialogFontVar3;
	}
	iVar2 = 3;
	if (bVar1) {
		iVar2 = 2;
	}
	return iVar2;
}

void printDialogCharacter(ushort characterType, ushort characterId, ushort* outputBuffer, short outputBufferStride, int oddOrEvenLine)
{
	{
		std::string decodedString;
		if (characterId == 0x10)
		{
			decodedString += ' ';
		}
		else if ((characterId >= 0x16) && (characterId <= 0x1F))
		{
			decodedString += (char)(characterId - 0x16 + '0');
		}
		else if ((characterId >= 0x20) && (characterId <= 0x39))
		{
			decodedString += (char)(characterId - 0x20 + 'A');
		}
		else if ((characterId >= 0x3D) && (characterId <= 0x56))
		{
			decodedString += (char)(characterId - 0x3D + 'a');
		}
		printf("%s", decodedString.c_str());
	}

	u16* pCharacterData = nullptr;
	if (characterType == 0) {
		pCharacterData = (ushort*)(dialogFontVarPtr2 + ((uint)characterId - dialogFontVar1) * 0x16);
	}
	else {
		assert(0);
	}

	ushort uVar1;
	ushort uVar2;
	int iVar3;
	ushort uVar4;
	ushort* puVar5;
	ushort* puVar6;
	ushort* puVar7;
	ushort* puVar8;
	int iVar9;

	iVar9 = 0;
	if (oddOrEvenLine == 0) {
		outputBuffer[2] = outputBuffer[2] & 0xcccc;
		*outputBuffer = *outputBuffer & 0xcccc;
		iVar3 = (int)outputBufferStride;
		outputBuffer[1] = outputBuffer[1] & 0xcccc;
		puVar5 = outputBuffer + iVar3;
		puVar8 = puVar5 + 2;
		puVar7 = puVar5 + -iVar3;
		*puVar5 = *puVar5 & 0xcccc;
		puVar5[2] = puVar5[2] & 0xcccc;
		puVar5[1] = puVar5[1] & 0xcccc;
		puVar6 = puVar5;
		do {
			puVar6 = puVar6 + iVar3;
			*puVar6 = *puVar6 & 0xcccc;
			puVar6[2] = puVar6[2] & 0xcccc;
			puVar6[1] = puVar6[1] & 0xcccc;
			uVar1 = *pCharacterData;
			uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x222;
			pCharacterData = pCharacterData + 1;
			if ((uVar1 & 0x40) != 0) {
				uVar2 = uVar2 | 0x2220;
			}
			if ((uVar1 & 0x20) != 0) {
				uVar2 = uVar2 | 0x2200;
			}
			uVar4 = uVar2 | 0x2000;
			if ((uVar1 & 0x10) == 0) {
				uVar4 = uVar2;
			}
			*puVar7 = *puVar7 | uVar4;
			uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x212;
			*puVar6 = *puVar6 | uVar4;
			if ((uVar1 & 0x40) != 0) {
				uVar2 = uVar2 | 0x2120;
			}
			if ((uVar1 & 0x20) != 0) {
				uVar2 = uVar2 | 0x1200;
			}
			uVar4 = *puVar5;
			if ((uVar1 & 0x10) != 0) {
				uVar4 = uVar4 | 0x2000;
			}
			*puVar5 = uVar4 | uVar2;
			uVar2 = 0x222;
			if ((uVar1 & 8) == 0) {
				if ((uVar1 & 0x10) == 0) {
					uVar2 = uVar1 >> 4 & 2;
				}
				else {
					uVar2 = 0x22;
				}
			}
			if ((uVar1 & 4) != 0) {
				uVar2 = uVar2 | 0x2220;
			}
			if ((uVar1 & 2) != 0) {
				uVar2 = uVar2 | 0x2200;
			}
			uVar4 = uVar2 | 0x2000;
			if ((uVar1 & 1) == 0) {
				uVar4 = uVar2;
			}
			puVar7[1] = puVar7[1] | uVar4;
			puVar6[1] = puVar6[1] | uVar4;
			uVar2 = uVar1 >> 4 & 2;
			if ((uVar1 & 0x10) != 0) {
				uVar2 = uVar2 | 0x21;
			}
			if ((uVar1 & 8) != 0) {
				uVar2 = uVar2 | 0x212;
			}
			if ((uVar1 & 4) != 0) {
				uVar2 = uVar2 | 0x2120;
			}
			if ((uVar1 & 2) != 0) {
				uVar2 = uVar2 | 0x1200;
			}
			uVar4 = puVar8[-1];
			if ((uVar1 & 1) != 0) {
				uVar4 = uVar4 | 0x2000;
			}
			puVar8[-1] = uVar4 | uVar2;
			uVar2 = 0x222;
			if (((uVar1 & 0x8000) == 0) && (uVar2 = 0x22, (uVar1 & 1) == 0)) {
				uVar2 = uVar1 & 2;
			}
			if ((uVar1 & 0x4000) != 0) {
				uVar2 = uVar2 | 0x2220;
			}
			if ((uVar1 & 0x2000) != 0) {
				uVar2 = uVar2 | 0x2200;
			}
			uVar4 = uVar2 | 0x2000;
			if ((uVar1 & 0x1000) == 0) {
				uVar4 = uVar2;
			}
			puVar7[2] = puVar7[2] | uVar4;
			puVar6[2] = puVar6[2] | uVar4;
			uVar2 = uVar1 & 2;
			if ((uVar1 & 1) != 0) {
				uVar2 = uVar2 | 0x21;
			}
			if ((uVar1 & 0x8000) != 0) {
				uVar2 = uVar2 | 0x212;
			}
			if ((uVar1 & 0x4000) != 0) {
				uVar2 = uVar2 | 0x2120;
			}
			if ((uVar1 & 0x2000) != 0) {
				uVar2 = uVar2 | 0x1200;
			}
			uVar4 = *puVar8;
			if ((uVar1 & 0x1000) != 0) {
				uVar4 = uVar4 | 0x2000;
			}
			*puVar8 = uVar4 | uVar2;
			puVar8 = puVar8 + iVar3;
			puVar7 = puVar7 + iVar3;
			iVar9 = iVar9 + 1;
			puVar5 = puVar5 + iVar3;
		} while (iVar9 < 0xb);
	}
	else {
		outputBuffer[2] = outputBuffer[2] & 0x3333;
		*outputBuffer = *outputBuffer & 0x3333;
		iVar3 = (int)outputBufferStride;
		outputBuffer[1] = outputBuffer[1] & 0x3333;
		puVar5 = outputBuffer + iVar3;
		puVar8 = puVar5 + 2;
		puVar7 = puVar5 + -iVar3;
		*puVar5 = *puVar5 & 0x3333;
		puVar5[2] = puVar5[2] & 0x3333;
		puVar5[1] = puVar5[1] & 0x3333;
		puVar6 = puVar5;
		do {
			puVar6 = puVar6 + iVar3;
			*puVar6 = *puVar6 & 0x3333;
			puVar6[2] = puVar6[2] & 0x3333;
			puVar6[1] = puVar6[1] & 0x3333;
			uVar1 = *pCharacterData;
			uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x888;
			pCharacterData = pCharacterData + 1;
			if ((uVar1 & 0x40) != 0) {
				uVar2 = uVar2 | 0x8880;
			}
			if ((uVar1 & 0x20) != 0) {
				uVar2 = uVar2 | 0x8800;
			}
			uVar4 = uVar2 | 0x8000;
			if ((uVar1 & 0x10) == 0) {
				uVar4 = uVar2;
			}
			*puVar7 = *puVar7 | uVar4;
			uVar2 = -(ushort)((uVar1 & 0x80) != 0) & 0x848;
			*puVar6 = *puVar6 | uVar4;
			if ((uVar1 & 0x40) != 0) {
				uVar2 = uVar2 | 0x8480;
			}
			if ((uVar1 & 0x20) != 0) {
				uVar2 = uVar2 | 0x4800;
			}
			uVar4 = *puVar5;
			if ((uVar1 & 0x10) != 0) {
				uVar4 = uVar4 | 0x8000;
			}
			*puVar5 = uVar4 | uVar2;
			uVar2 = 0x888;
			if ((uVar1 & 8) == 0) {
				if ((uVar1 & 0x10) == 0) {
					uVar2 = uVar1 >> 2 & 8;
				}
				else {
					uVar2 = 0x88;
				}
			}
			if ((uVar1 & 4) != 0) {
				uVar2 = uVar2 | 0x8880;
			}
			if ((uVar1 & 2) != 0) {
				uVar2 = uVar2 | 0x8800;
			}
			uVar4 = uVar2 | 0x8000;
			if ((uVar1 & 1) == 0) {
				uVar4 = uVar2;
			}
			puVar7[1] = puVar7[1] | uVar4;
			puVar6[1] = puVar6[1] | uVar4;
			uVar2 = uVar1 >> 2 & 8;
			if ((uVar1 & 0x10) != 0) {
				uVar2 = uVar2 | 0x84;
			}
			if ((uVar1 & 8) != 0) {
				uVar2 = uVar2 | 0x848;
			}
			if ((uVar1 & 4) != 0) {
				uVar2 = uVar2 | 0x8480;
			}
			if ((uVar1 & 2) != 0) {
				uVar2 = uVar2 | 0x4800;
			}
			uVar4 = puVar8[-1];
			if ((uVar1 & 1) != 0) {
				uVar4 = uVar4 | 0x8000;
			}
			puVar8[-1] = uVar4 | uVar2;
			uVar2 = 0x888;
			if ((uVar1 & 0x8000) == 0) {
				if ((uVar1 & 1) == 0) {
					uVar2 = (ushort)((uVar1 & 2) << 2);
				}
				else {
					uVar2 = 0x88;
				}
			}
			if ((uVar1 & 0x4000) != 0) {
				uVar2 = uVar2 | 0x8880;
			}
			if ((uVar1 & 0x2000) != 0) {
				uVar2 = uVar2 | 0x8800;
			}
			uVar4 = uVar2 | 0x8000;
			if ((uVar1 & 0x1000) == 0) {
				uVar4 = uVar2;
			}
			puVar7[2] = puVar7[2] | uVar4;
			puVar6[2] = puVar6[2] | uVar4;
			uVar2 = (ushort)((uVar1 & 2) << 2);
			if ((uVar1 & 1) != 0) {
				uVar2 = uVar2 | 0x84;
			}
			if ((uVar1 & 0x8000) != 0) {
				uVar2 = uVar2 | 0x848;
			}
			if ((uVar1 & 0x4000) != 0) {
				uVar2 = uVar2 | 0x8480;
			}
			if ((uVar1 & 0x2000) != 0) {
				uVar2 = uVar2 | 0x4800;
			}
			uVar4 = *puVar8;
			if ((uVar1 & 0x1000) != 0) {
				uVar4 = uVar4 | 0x8000;
			}
			*puVar8 = uVar4 | uVar2;
			puVar8 = puVar8 + iVar3;
			puVar7 = puVar7 + iVar3;
			iVar9 = iVar9 + 1;
			puVar5 = puVar5 + iVar3;
		} while (iVar9 < 0xb);
	}
}

int printDialogTextVar = 0;
void updateDialogTextImage(sDialogWindow18* param_1)
{
	if (param_1->mA_width1 < (param_1->m0).vx) // go to next line?
	{
		int sVar3 = param_1->mC_height;
		(param_1->m0).vx = 0;
		(param_1->m0).vy = (param_1->m0).vy + 1;
		int sVar8 = (param_1->m0).vy;
		param_1->m18 = param_1->m18 + 1;
		if (sVar3 <= sVar8) {
			int uVar5 = param_1->m10_flags;
			(param_1->m0).vy = 0;
			param_1->m10_flags = uVar5 | 1;
		}
		if ((param_1->m10_flags & 1) != 0) {
			param_1->m28_perLineBuffer[(param_1->m14).vy].m58 = 0;
			sVar3 = param_1->mC_height;
			sVar8 = (param_1->m14).vy + 1;
			(param_1->m14).vy = sVar8;
			if (sVar3 <= sVar8) {
				(param_1->m14).vy = 0;
			}
		}
		int characterToPrint = (int)param_1->m18 % (param_1->mC_height + 1);
		int iVar13 = (int)(param_1->m0).vy;
		param_1->m28_perLineBuffer[iVar13].m5C = (char)((int)characterToPrint / 2) * '\r' + *(char*)&param_1->mE;
		if ((characterToPrint & 1) == 0) {
			param_1->m28_perLineBuffer[iVar13].m5E = textSpriteMode0;
		}
		else {
			param_1->m28_perLineBuffer[iVar13].m5E = textSpriteMode1;
		}
		param_1->m28_perLineBuffer[iVar13].m5A = (byte)characterToPrint & 1;
		param_1->m28_perLineBuffer[iVar13].m5B = (byte)characterToPrint;
		param_1->m28_perLineBuffer[iVar13].m50.y = param_1->mE + (short)((int)characterToPrint / 2) * 0xd;
	}

	if (param_1->m6C != 0) {
		param_1->m6C = 0;
		param_1->m10_flags = param_1->m10_flags & 0xfffb;
		return;
	}

	int bVar1 = param_1->m69;
	int iVar13 = bVar1 - 1;
	int iVar14 = printDialogTextVar;

	while (1)
	{
		if (iVar13 == -1)
		{
			printDialogTextVar = iVar14;
			return;
		}

		u8* pCharacterToPrint = &(*param_1->m1C);
		u8 characterToPrint = *pCharacterToPrint;
		printDialogTextVar = iVar14;

		switch (characterToPrint)
		{
		case 0:
			if ((param_1->m10_flags & 0x80) == 0) {
				param_1->m10_flags |= 8;
				param_1->m6B = '\x01';
				param_1->m6C = 1;
				return;
			}
			param_1->m10_flags &= ~0x80;
			param_1->m1C = param_1->m20 + 1;
			iVar13 = iVar13 + -1;
			iVar14 = printDialogTextVar;
			break;
		case 1: // end of line
			printf("\n");
			(param_1->m0).vx = 100;
			param_1->m1C++;
			return;
		case 2:
			assert(0);
			break;
		case 3:
			assert(0);
			break;
		case 0xF:
			// escape character;
			switch (pCharacterToPrint[1])
			{
			case 0:
				param_1->m84 = param_1->m1C[2];
				param_1->m1C += 3;
				return;
			default:
				assert(0);
			}
			break;
		default:
		{
			// normal character
			int iVar13 = 1;
			int uVar10;
			if ((int)characterToPrint < dialogFontVar0) {
				uVar10 = characterToPrint;
				characterToPrint = 0;
			}
			else {
				uVar10 = (uint)pCharacterToPrint[1];
				iVar13 = 2;
			}
			int uVar4 = getDialogFontCharacterWidth(characterToPrint, uVar10);
			int sVar3 = (param_1->m0).vx;
			if ((int)param_1->mA_width1 < (int)((int)sVar3 + (uint)uVar4)) {
				(param_1->m0).vx = sVar3 + uVar4;
				return;
			}
			printDialogCharacter(characterToPrint, uVar10, &param_1->m2C_inRamDialogTextImage[sVar3], (int)param_1->m12_widthPadded, param_1->m28_perLineBuffer[(param_1->m0).vy].m5A);
			sVar3 = (param_1->m0).vy;
			int sVar7 = (param_1->m0).vx + uVar4;
			param_1->m1C += iVar13;
			(param_1->m0).vx = sVar7;
			param_1->m28_perLineBuffer[sVar3].m58 = sVar7;
			break;
		}
		}

		iVar13--;
		iVar14 = printDialogTextVar;
	}

	MissingCode();
}

void updateAndRenderTextForDialogWindow(sDialogWindow18* param_1, sTag* OT, int oddOrEven)
{
	if ((param_1->m10_flags & 4) == 0) {
		if (param_1->m82 == 0) {
			return;
		}
		sDialogWindow18_8C* psVar8 = param_1->m8C;
		param_1->m1C = psVar8->m4_dialogPointer;
		param_1->m8C = param_1->m8C->m0_pNext;
		delete psVar8;
		param_1->m82 = param_1->m82 + -1;
		param_1->m10_flags = param_1->m10_flags & 2 | 0x24;
		if (param_1->m6A != 0) {
			u8 bVar3 = param_1->m6A;
			param_1->m68 = param_1->m6A;
			param_1->m6A = 0;
			param_1->m69 = bVar3;
		}
		param_1->m88_delayBetweenCharacters = 0;
		param_1->m86_currentDelayForNextCharacter = 0;
		param_1->m69 = param_1->m68;
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
		sDialogWindow18PerLineBufferEntry* psVar5 = &param_1->m28_perLineBuffer[0];
		short sVar2 = param_1->mE;
		(param_1->m14).vy = 0;
		param_1->m18 = 0;
		(param_1->m0).vy = 0;
		(param_1->m0).vx = 0;
		psVar5->m5C = (byte)sVar2;
		param_1->m28_perLineBuffer[0].m5E = textSpriteMode0;
		param_1->m28_perLineBuffer[0].m5A = 0;
		param_1->m28_perLineBuffer[0].m50.y = param_1->mE;
		if (0 < param_1->mC_height) {
			for (int i=0; i< param_1->mC_height; i++)
			{
				param_1->m28_perLineBuffer[i].m58 = 0;
			}
		}
		param_1->m10_flags &= ~0x20;
	}

	if (0 < param_1->mC_height) {
		int iVar11 = param_1->m14.vy;
		int iVar6 = param_1->m14.vy;
		for (int i=0; i< param_1->mC_height; i++)
		{
			if (iVar11 >= param_1->mC_height)
			{
				iVar6 = 0;
				iVar11 = 0;
			}

			if (param_1->m6E_selectedLineForMultiChoice == i) {
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code &= ~1;
			}
			else {
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code |= 1;
			}

			if (param_1->m28_perLineBuffer[iVar6].m58 > 0x40)
			{
				MissingCode();
				//assert(0);
			}
		}
	}

	AddPrim(OT, &param_1->m30_textTileDrawMode[1].tag);

	if (0 < param_1->mC_height) {
		int iVar11 = param_1->m14.vy;
		int iVar6 = param_1->m14.vy;
		for (int i = 0; i < param_1->mC_height; i++)
		{
			if (iVar11 >= param_1->mC_height)
			{
				iVar6 = 0;
				iVar11 = 0;
			}

			if (param_1->m6E_selectedLineForMultiChoice == i) {
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code &= ~1;
			}
			else {
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].code |= 1;
			}

			// do we overflow in the second sprite?
			if (param_1->m28_perLineBuffer[iVar6].m58)
			{
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].v0 = param_1->m28_perLineBuffer[iVar6].m5C;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].clut = param_1->m28_perLineBuffer[iVar6].m5E;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].y0 = (param_1->m4).vy + (param_1->m14).vx * i;
				int uVar4 = param_1->m28_perLineBuffer[iVar6].m58;
				if (0x40 < uVar4) {
					uVar4 = 0x40;
				}
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].w = uVar4 * 4;
				addSPRT(OT, &param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].tag);
			}
			iVar6++;
		}
	}

	if (param_1->m84 == 0) {
		if (param_1->m86_currentDelayForNextCharacter == 0) {
			param_1->m86_currentDelayForNextCharacter = param_1->m88_delayBetweenCharacters;
			if ((param_1->m10_flags & 0x58) == 0) {
				updateDialogTextImage(param_1);
				LoadImage(&param_1->m28_perLineBuffer[(param_1->m0).vy].m50, (u8*)&param_1->m2C_inRamDialogTextImage[0]);
			}
		}
		else {
			param_1->m86_currentDelayForNextCharacter = param_1->m86_currentDelayForNextCharacter + -1;
		}
	}
	else {
		param_1->m84 = param_1->m84 + -1;
	}

	if (param_1->m84 != 0)
	{
		--param_1->m84;
		if (--param_1->m84 == -1)
		{
			param_1->m10_flags = param_1->m10_flags & 0xffef;
		}
	}

	if ((param_1->m10_flags & 2) == 0) {
		assert(0);
	}

	param_1->m10_flags = param_1->m10_flags & 0xfeff;
	AddPrim(OT, &param_1->m30_textTileDrawMode->tag);
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

	std::array<s32, 4> processedWindowsArray;
	processedWindowsArray.fill(-1);

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
							addDialogWindowsToOTSub2(&gDialogWindows[i].m18, gDialogWindows[i].m18.m90_dialogPointer);
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