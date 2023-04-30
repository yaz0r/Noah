#include "noahLib.h"
#include "kernel/graphics.h"
#include "kernel/font.h"
#include "dialogWindows.h"
#include "fieldScriptSupport.h"
#include "kernel/TIM.h"
#include "kernel/gameState.h"

u16 textSpriteMode0 = 0;
u16 textSpriteMode1 = 0;

std::array<sDialogWindow, 4> gDialogWindows;
std::array<s32, 4> dialogWindowVar1;

std::array<std::array<DR_MODE, 16>,2> gDialogDrModes;
std::array<RECT, 16> gDialogRects;

std::array<u8, 3> dialogWindowColor;

u16 padButtonForDialogs = 0;
u16 padButtonForDialogs2 = 0;

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

std::array<RECT, 5> dialogWindowsRectConfigs2 = {
    {
        {	0x20,	0x1C0,	0x8,	0xC	},
        {	0x28,	0x1C0,	0x8,	0xC	},
        {	0x30,	0x1C0,	0x8,	0xC	},
        {	0x38,	0x1C0,	0x8,	0xC	},
        {	0x40,	0x1C0,	0x8,	0xC	},
    }
};

std::array<RECT, 6> dialogWindowsRectConfigs = {
	{
		{	0x60,	0x1C0,	0xC,	0x8	},
		{	0x60,	0x1C8,	0xC,	0x8	},
		{	0x60,	0x1D0,	0xC,	0x8	},
		{	0x60,	0x1D8,	0xC,	0x8	},
		{	0x60,	0x1E0,	0xC,	0x8	},
		{	0x30,	0xF0,	0x10,	0x10	},
	}
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
		gDialogWindows[windowIndex].m39C_Sprt[0].h = 0xc;
		gDialogWindows[windowIndex].m39C_Sprt[0].clut = GetClut(0x100, 0xf6);
		gDialogWindows[windowIndex].m39C_Sprt[0].u0 = 0x80;
		gDialogWindows[windowIndex].m39C_Sprt[0].v0 = 0xc0;
		gDialogWindows[windowIndex].m39C_Sprt[0].w = 8;
		gDialogWindows[windowIndex].m39C_Sprt[0].x0 = 0;
		gDialogWindows[windowIndex].m39C_Sprt[0].y0 = 0;
		gDialogWindows[windowIndex].m39C_Sprt[1] = gDialogWindows[windowIndex].m39C_Sprt[0];
	}

	gDialogWindows[windowIndex].m40A = 2;

	for (int i=0; i<8; i++)
	{
		RECT localRect = dialogWindowsRectConfigs3[i];
		
		int alphaMode = 2;
		alphaMode = 0; // TODO: hack
		SetDrawMode(&gDialogWindows[windowIndex].mFC[0][i], 0, 0, GetTPage(0, alphaMode, 0x280, 0x1f0), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].mFC[1][i], 0, 0, GetTPage(0, alphaMode, 0x280, 0x1f0), &localRect);

		SetSprt(&gDialogWindows[windowIndex].m1EC_windowBorders[0][i]);
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].r0 = 0x80;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].g0 = 0x80;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].b0 = 0x80;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].clut = GetClut(0x100, 0xf4);
		SetSemiTrans(&gDialogWindows[windowIndex].m1EC_windowBorders[0][i], 1);
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].u0 = 0x80;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].v0 = 0xC0;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].x0 = 0;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].y0 = 0;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].w = dialogWindowsRectConfigs3[i].w;
		gDialogWindows[windowIndex].m1EC_windowBorders[0][i].h = dialogWindowsRectConfigs3[i].h;

		gDialogWindows[windowIndex].m1EC_windowBorders[1][i] = gDialogWindows[windowIndex].m1EC_windowBorders[0][i];
	}

	{
		RECT localRect = { 0,0,0xFF,0xFF };
		SetDrawMode(&gDialogWindows[windowIndex].m42C_portraitDrawMode[0], 0, 0, GetTPage(1, 0, 0x2c0, 0x100), &localRect);
		SetDrawMode(&gDialogWindows[windowIndex].m42C_portraitDrawMode[1], 0, 0, GetTPage(1, 0, 0x2c0, 0x100), &localRect);

		SetPolyFT4(&gDialogWindows[windowIndex].m444_portraitPoly[0]);
		gDialogWindows[windowIndex].m444_portraitPoly[0].r0 = 0x80;
		gDialogWindows[windowIndex].m444_portraitPoly[0].g0 = 0x80;
		gDialogWindows[windowIndex].m444_portraitPoly[0].b0 = 0x80;
		gDialogWindows[windowIndex].m444_portraitPoly[0].clut = GetClut(0, 0xe0);
		gDialogWindows[windowIndex].m444_portraitPoly[0].tpage = GetTPage(1, 0, 0x2c0, 0x100);
		gDialogWindows[windowIndex].m444_portraitPoly[1] = gDialogWindows[windowIndex].m444_portraitPoly[0];
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

		SetDrawMode(&gDialogDrModes[0][i], 0, 0, GetTPage(0, 0, 0x380, 0x100), &gDialogRects[i]);
		SetDrawMode(&gDialogDrModes[1][i], 0, 0, GetTPage(0, 0, 0x380, 0x100), &gDialogRects[i]);
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

void setupDialogWindowPortraitPoly(int windowIndex, int setupIndex)
{
	static const std::array<sVec2_s16, 8> dialogWindowPortraitPolySetups = { {
		{ 0x3F, 0 },
		{ 0, 0 },
		{ 0x3F, 0x40 },
		{ 0, 0x40 },
        { 0x3F, 0x80 },
        { 0, 0x80 },
        { 0x3F, 0xBF },
        { 0, 0xBF },
	} };

	gDialogWindows[windowIndex].m444_portraitPoly[0].u0 = dialogWindowPortraitPolySetups[setupIndex].vx;
	gDialogWindows[windowIndex].m444_portraitPoly[1].u0 = dialogWindowPortraitPolySetups[setupIndex].vx;

	gDialogWindows[windowIndex].m444_portraitPoly[0].v0 = dialogWindowPortraitPolySetups[setupIndex].vy;
	gDialogWindows[windowIndex].m444_portraitPoly[1].v0 = dialogWindowPortraitPolySetups[setupIndex].vy;

	gDialogWindows[windowIndex].m444_portraitPoly[0].u1 = dialogWindowPortraitPolySetups[setupIndex].vx + 0x40;
	gDialogWindows[windowIndex].m444_portraitPoly[1].u1 = dialogWindowPortraitPolySetups[setupIndex].vx + 0x40;

	gDialogWindows[windowIndex].m444_portraitPoly[0].v1 = dialogWindowPortraitPolySetups[setupIndex].vy;
	gDialogWindows[windowIndex].m444_portraitPoly[1].v1 = dialogWindowPortraitPolySetups[setupIndex].vy;

	gDialogWindows[windowIndex].m444_portraitPoly[0].u2 = dialogWindowPortraitPolySetups[setupIndex].vx;
	gDialogWindows[windowIndex].m444_portraitPoly[1].u2 = dialogWindowPortraitPolySetups[setupIndex].vx;

	gDialogWindows[windowIndex].m444_portraitPoly[0].v2 = dialogWindowPortraitPolySetups[setupIndex].vy + 0x40;
	gDialogWindows[windowIndex].m444_portraitPoly[1].v2 = dialogWindowPortraitPolySetups[setupIndex].vy + 0x40;

	gDialogWindows[windowIndex].m444_portraitPoly[0].u3 = dialogWindowPortraitPolySetups[setupIndex].vx + 0x40;
	gDialogWindows[windowIndex].m444_portraitPoly[1].u3 = dialogWindowPortraitPolySetups[setupIndex].vx + 0x40;

	gDialogWindows[windowIndex].m444_portraitPoly[0].v3 = dialogWindowPortraitPolySetups[setupIndex].vy + 0x40;
	gDialogWindows[windowIndex].m444_portraitPoly[1].v3 = dialogWindowPortraitPolySetups[setupIndex].vy + 0x40;

	gDialogWindows[windowIndex].m444_portraitPoly[0].clut = GetClut(0, setupIndex + 0xe0);
	gDialogWindows[windowIndex].m444_portraitPoly[1].clut = GetClut(0, setupIndex + 0xe0);
}

void setupWindowSize1(int windowIndex, short x, short y, short width, short height)
{
	gDialogWindows[windowIndex].mAC_Position.vx = x;
	gDialogWindows[windowIndex].mAC_Position.vy = y;
	gDialogWindows[windowIndex].mB0_Size.vx = width;
	gDialogWindows[windowIndex].mB0_Size.vy = height;
}

void setupWindowSize2(sDialogWindow18* param_1, int x1, int y1, short x2, short y2, short width, short height)
{
	(param_1->m4).vx = x2;
	param_1->m10_flags = 0;
	param_1->m84_delay = 0;
	param_1->m8C = nullptr;
	param_1->m82 = 0;
	(param_1->m14).vx = 0xe;
	param_1->m68 = 1;
	param_1->m69 = 1;
	param_1->mA_width1 = width;
	param_1->m6C_autoClose = 0;
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

	param_1->m48_textTile[0].m3_size = 3;
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

			param_1->m28_perLineBuffer[i].m0[0][1].m3_size = param_1->m28_perLineBuffer[i].m0[0][0].m3_size = 4;
			param_1->m28_perLineBuffer[i].m0[0][1].code = param_1->m28_perLineBuffer[i].m0[0][0].code = 0x64; // setup as Textured Rectangle, variable size, opaque, texture-blending

			param_1->m28_perLineBuffer[i].m0[0][0].code |= 1; // switch to Textured Rectangle, variable size, opaque, raw-texture
			param_1->m28_perLineBuffer[i].m0[0][1].code |= 1;

			param_1->m28_perLineBuffer[i].m0[1][0] = param_1->m28_perLineBuffer[i].m0[0][0];
			param_1->m28_perLineBuffer[i].m0[1][1] = param_1->m28_perLineBuffer[i].m0[0][1];

			param_1->m28_perLineBuffer[i].m50.x = x1;
			param_1->m28_perLineBuffer[i].m50.y = yFraq;
			param_1->m28_perLineBuffer[i].m50.w = param_1->m12_widthPadded;
			param_1->m28_perLineBuffer[i].m50.h = 0xD;
			param_1->m28_perLineBuffer[i].m58_widthDiv4 = 0;
			if ((i & 1) == 0)
			{
				param_1->m28_perLineBuffer[i].m5E_clut = textSpriteMode0;
			}
			else
			{
				param_1->m28_perLineBuffer[i].m5E_clut = textSpriteMode1;
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

s16 dialogWindowOpenAnimationNumFrames = 0;


std::vector<u8>::iterator getDialogParamPointer(const std::vector<u8>::iterator& buffer, int param_2)
{
	return buffer + READ_LE_U16(buffer + param_2 * 2 + 4);
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
	if ((pScriptEntity->m80_dialogPortrait == -1) || ((flagUpper & 2) != 0)) {
		gDialogWindows[windowIndex].m495_portrait = 0x80;
		gDialogWindows[windowIndex].m494_hasPortrait = 0;
	}
	else {
		u32 windowSetupIndex;
		if ((flagUpper & 0x402) == 0) {
			windowSetupIndex = pScriptEntity->m12C_flags >> 1 & 0xe | 1;
		}
		else {
			windowSetupIndex = pScriptEntity->m12C_flags >> 1 & 0xe;
		}
		setupDialogWindowPortraitPoly(windowIndex, windowSetupIndex);
		gDialogWindows[windowIndex].m494_hasPortrait = 1;
		gDialogWindows[windowIndex].m495_portrait = (actorArray[fieldActorId].m4C_scriptEntity)->m80_dialogPortrait;
	}

	gDialogWindows[windowIndex].m37C = -1;
	setupWindowSize1(windowIndex, (int)x, (int)y2, width * 4 + 0x10, height * 0xe + 0x10);
	int xOffset = 0;
	if ((actorArray[fieldActorId].m4C_scriptEntity)->m80_dialogPortrait != -1) {
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
	if (dialogWindowOpenAnimationNumFrames == 8) {
		bVar3 = 1;
	}
	gDialogWindows[windowIndex].m18.m68 = bVar3;
	gDialogWindows[windowIndex].m18.m90_dialogPointer = getDialogParamPointer(rawFieldDialogBundle.begin(), dialogIndex);
	gDialogWindows[windowIndex].m40E = 0;
	gDialogWindows[windowIndex].m18.m10_flags |= 2;
	gDialogWindows[windowIndex].m416_fieldActorId = fieldActorId;
	gDialogWindows[windowIndex].m418_actorId = actorId;
	gDialogWindows[windowIndex].m408_openAnimationCounter = dialogWindowOpenAnimationNumFrames;

	if ((param_11 & 0x800) == 0) {
		gDialogWindows[windowIndex].m412 = 0;
	}
	else {
		gDialogWindows[windowIndex].m412 = 1;
	}

	gDialogWindows[windowIndex].m41C[0] = ((projectedPosition[0] - (width * 2 + 8)) - (int)x) * 0x10000;
	gDialogWindows[windowIndex].m41C[1] = ((projectedPosition[1] - (height * 7 + 8)) - (int)y2) * 0x10000;

	if ((flagUpper & 0x100) == 0) {
		gDialogWindows[windowIndex].m424[0] = -(gDialogWindows[windowIndex].m41C[0] / (int)dialogWindowOpenAnimationNumFrames);
		gDialogWindows[windowIndex].m424[1] = -(gDialogWindows[windowIndex].m41C[1] / (int)dialogWindowOpenAnimationNumFrames);
	}
	else {
		gDialogWindows[windowIndex].m408_openAnimationCounter = 1;
		gDialogWindows[windowIndex].m424[0] = -gDialogWindows[windowIndex].m41C[0];
		gDialogWindows[windowIndex].m424[1] = -gDialogWindows[windowIndex].m41C[1];
	}
	if ((((actorArray[actorId].m4C_scriptEntity)->m4_flags.m_rawFlags & 0x200) != 0) && ((flagUpper & 1) == 0)) {
		gDialogWindows[windowIndex].m414 = 0;
	}
}

void resetDialogWindow18(sDialogWindow18* param_1)
{
	if (param_1->m84_delay == 0) {
		param_1->m6C_autoClose = 0;
		param_1->m10_flags = param_1->m10_flags & 2;
	}
	return;
}

u16 windowOpenBF = 0;

void flagWindowOpenBF(uint param_1)
{
	windowOpenBF = windowOpenBF | (ushort)(1 << (param_1 & 0x1f));
	return;
}

void dealocateDialogWindow18MemorySubSub(sDialogWindow18* param_1)
{
	sDialogWindow18_8C* psVar1;
	sDialogWindow18_8C* psVar2;

	if (param_1->m84_delay == 0) {
		psVar1 = param_1->m8C;
		while (psVar1 != nullptr) {
			psVar2 = psVar1->m0_pNext;
			delete psVar1;
			psVar1 = psVar2;
		}
		param_1->m8C = nullptr;
		param_1->m82 = 0;
	}
	return;
}

void dealocateDialogWindow18MemorySub(sDialogWindow18* param_1)
{
	param_1->m6C_autoClose = 0;
	param_1->m84_delay = 0;
	param_1->m10_flags = param_1->m10_flags & 2;
	dealocateDialogWindow18MemorySubSub(param_1);
	return;
}


void dealocateDialogWindow18Memory(sDialogWindow18* param_1)
{
	dealocateDialogWindow18MemorySub(param_1);
	param_1->m28_perLineBuffer.clear();
	param_1->m2C_inRamDialogTextImage.clear();
	return;
}

void addDialogWindowsToOTSub1(sDialogWindow18* param_1)
{
	ushort uVar1;

	uVar1 = param_1->m10_flags;
	param_1->m10_flags = uVar1 & 0xfff7;
	if ((uVar1 & 0x200) != 0) {
		param_1->m84_delay = 0;
		param_1->m6C_autoClose = 0;
		param_1->m10_flags = param_1->m10_flags & 0xfdff;
	}
	return;
}


void releaseDialogWindow(s16 windowIndex)
{
	if (gDialogWindows[windowIndex].m40E == 0) {
		sDialogWindow18* psVar1 = &gDialogWindows[windowIndex].m18;
		resetDialogWindow18(psVar1);
		addDialogWindowsToOTSub1(psVar1);
		dealocateDialogWindow18Memory(psVar1);
		gDialogWindows[windowIndex].m37C = -1;
		gDialogWindows[windowIndex].m40E = -1;
		gDialogWindows[windowIndex].m414 = -1;
		gDialogWindows[windowIndex].m410 = -1;
		dialogWindowVar1[windowIndex] = -1;
		windowOpenBF = windowOpenBF & ((ushort)(1 << (windowIndex & 0x1f)) ^ 0xff);
		gDialogWindows[windowIndex].m416_fieldActorId = 0xff;
		gDialogWindows[windowIndex].m412 = 0;
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
		poly->x0y0.vx = sVar1;
		poly->x1y1.vx = sVar1 + width;
		poly->x2y2.vx = sVar1;
		poly->x3y3.vx = sVar1 + width;
	}
	else {
		poly->x1y1.vx = x;
		poly->x0y0.vx = x + width;
		poly->x3y3.vx = x;
		poly->x2y2.vx = x + width;
	}
	poly->x0y0.vy = y;
	poly->x1y1.vy = y;
	poly->x2y2.vy = y + height;
	poly->x3y3.vy = y + height;
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
            int numFramesTimesTwo = dialogWindowOpenAnimationNumFrames << 1;
            int numFramesPassedInAnimation = dialogWindowOpenAnimationNumFrames - gDialogWindows[windowIndex].m408_openAnimationCounter;
            int XOffset = ((windowWidth << 0x10) / numFramesTimesTwo) * numFramesPassedInAnimation;
            int YOffset = ((windowHeight << 0x10) / numFramesTimesTwo) * numFramesPassedInAnimation;
            windowX = (windowX + ((windowWidth - (windowWidth2 >> 0x1f)) >> 1)) - (XOffset >> 0x10);
            windowWidth = XOffset * 2 >> 0x10;
            windowY = (windowY + ((windowHeight - (windowHeight2 >> 0x1f)) >> 1)) - (YOffset >> 0x10);
            windowHeight = YOffset * 2 >> 0x10;
            if (windowWidth < 0x10) {
                windowX = windowX - (0x10 - windowWidth) / 2;
                windowWidth = 0x10;
            }
            if (windowHeight < 0x10) {
                windowY = windowY - (0x10 - windowHeight) / 2;
                windowHeight = 0x10;
            }
            windowWidth2 = gDialogWindows[windowIndex].m41C[1] + gDialogWindows[windowIndex].m424[1];
            gDialogWindows[windowIndex].m41C[0] = gDialogWindows[windowIndex].m41C[0] + gDialogWindows[windowIndex].m424[0];
            windowY = windowY + (windowWidth2 >> 0x10);
            gDialogWindows[windowIndex].m41C[1] = windowWidth2;
            windowX = windowX + (gDialogWindows[windowIndex].m41C[0] >> 0x10);
		}

		if ((((gDialogWindows[windowIndex].m3C4 == 0) && (gDialogWindows[windowIndex].m410 == 0)) && (gDialogWindows[windowIndex].m408_openAnimationCounter == 0)) &&
			(((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0 && (gDialogWindows[windowIndex].m37C != 0)))) {
			if (gDialogWindows[windowIndex].m40A == 0) {
				sDialogWindow18* windowSubStruct = &gDialogWindows[windowIndex].m18;
				int sVar2 = getWindowSubStructX(windowSubStruct);
				int sVar3 = getWindowSubStructY(windowSubStruct);
				u16 TPage = GetTPage(0, 0, 0x298, 0x1c0);
				SetDrawMode(&gDialogWindows[windowIndex].m3C8[oddOrEven], 0, 0, (uint)TPage, &dialogWindowsRectConfigs[dialogWindowsFrameCount2]);
				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].x0 = sVar2;
				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].y0 = sVar3 + 4;

				gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven].m0_pNext = OT->m0_pNext;
				OT->m0_pNext = &gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven];

				gDialogWindows[windowIndex].m3C8[oddOrEven].m0_pNext = &gDialogWindows[windowIndex].m3E0_Sprt[oddOrEven];
				OT->m0_pNext = &gDialogWindows[windowIndex].m3C8[oddOrEven];
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
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][0].x0 = windowX - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][0].y0 = windowY - 7;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][2].y0 = windowY - 7;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][4].x0 = windowX - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][2].x0 = windowX + windowWidth - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][6].x0 = windowX + windowWidth - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][3].x0 = windowX - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][1].x0 = windowX + windowWidth - 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][4].y0 = windowY + windowHeight - 9;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][6].y0 = windowY + windowHeight - 9;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][3].y0 = windowY + 9;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][1].y0 = windowY + 9;
			int windowHeight2 = windowHeight + -0x12;
			if (windowHeight2 < 0) {
				windowHeight2 = 0;
			}
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][3].h = (short)windowHeight2;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][1].h = (short)windowHeight2;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][5].y0 = windowY - 7;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][5].x0 = windowX + 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][7].x0 = windowX + 8;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][7].y0 = windowY + windowHeight - 9;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][5].w = windowWidth + -0x10;
			gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][7].w = windowWidth + -0x10;

			// chain the sprites
			if ((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0) {
				for (int i = 0; i < 8; i++)
				{
					gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][i].m0_pNext = OT->m0_pNext;
					OT->m0_pNext = &gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][i];

					gDialogWindows[windowIndex].mFC[oddOrEven][i].m0_pNext = &gDialogWindows[windowIndex].m1EC_windowBorders[oddOrEven][i];
					OT->m0_pNext = &gDialogWindows[windowIndex].mFC[oddOrEven][i];
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

			setupFacePolyPosition(&gDialogWindows[windowIndex].m444_portraitPoly[oddOrEven], polyX, windowY + 4, faceWidth, faceHeight, (ushort)gDialogWindows[windowIndex].m40C_flags & 0x20);

			// chain the face poly
			if (gDialogWindows[windowIndex].m494_hasPortrait == 1) {
				gDialogWindows[windowIndex].m444_portraitPoly[oddOrEven].m0_pNext = OT->m0_pNext;
				OT->m0_pNext = &gDialogWindows[windowIndex].m444_portraitPoly[oddOrEven];

				gDialogWindows[windowIndex].m42C_portraitDrawMode[oddOrEven].m0_pNext = &gDialogWindows[windowIndex].m444_portraitPoly[oddOrEven];
				OT->m0_pNext = &gDialogWindows[windowIndex].m42C_portraitDrawMode[oddOrEven];
			}
		}

		// Selector sprite for multi-choice (selected text in multi-selection)
		if (((gDialogWindows[windowIndex].m37C == 0) && (gDialogWindows[windowIndex].m410 == 0)) && (gDialogWindows[windowIndex].m408_openAnimationCounter == 0)) {
            if ((gDialogWindows[windowIndex].m494_hasPortrait == 1) && ((gDialogWindows[windowIndex].m40C_flags & 0x20U) == 0)) {
                gDialogWindows[windowIndex].m39C_Sprt[oddOrEven].x0 = windowX + 0x5A;
            }
            else {
                gDialogWindows[windowIndex].m39C_Sprt[oddOrEven].x0 = windowX + 0x16;
            }

            gDialogWindows[windowIndex].m39C_Sprt[oddOrEven].y0 = (gDialogWindows[windowIndex].m382_CurrentMultiChoiceSelection + gDialogWindows[windowIndex].m37E_startOfMultiChoice) * 0xe + (short)windowY + 8;

            RECT local_90;
            local_90.x = dialogWindowsRectConfigs2[dialogWindowsFrameCount2].x;
            local_90.y = dialogWindowsRectConfigs2[dialogWindowsFrameCount2].y;
            local_90.w = dialogWindowsRectConfigs2[dialogWindowsFrameCount2].w;
            local_90.h = dialogWindowsRectConfigs2[dialogWindowsFrameCount2].h;

            SetDrawMode(&gDialogWindows[windowIndex].m384[oddOrEven], 0, 0, GetTPage(0, 0, 0x288, 0x1c0), &local_90);

            gDialogWindows[windowIndex].m39C_Sprt[oddOrEven].m0_pNext = OT->m0_pNext;
            OT->m0_pNext = &gDialogWindows[windowIndex].m39C_Sprt[oddOrEven];

            gDialogWindows[windowIndex].m384[oddOrEven].m0_pNext = &gDialogWindows[windowIndex].m39C_Sprt[oddOrEven];
            OT->m0_pNext = &gDialogWindows[windowIndex].m384[oddOrEven];
		}

		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].x0 = (short)windowX;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].y0 = (short)windowY + 1;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].w = (short)windowWidth;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].h = (short)windowHeight + -2;

		if ((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0) {
			gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].m0_pNext = OT->m0_pNext;
			OT->m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven];

			gDialogWindows[windowIndex].mC4[oddOrEven].m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven];
			OT->m0_pNext = &gDialogWindows[windowIndex].mC4[oddOrEven];
		}
	}
}

void clearSelectedLineForMultiChoice(sDialogWindow18* param_1)
{
	param_1->m6E_selectedLineForMultiChoice = -1;
	return;
}

void setSelectedLineForMultiChoice(sDialogWindow18* param_1, char param_2)
{
    param_1->m6E_selectedLineForMultiChoice = param_2;
    return;
}

void updateMultiChoice(int param_1)
{
	if ((gDialogWindows[param_1].m37C == 0) && (gDialogWindows[param_1].m408_openAnimationCounter == 0)) {
		if (gDialogWindows[param_1].m410 == 0) {
            if ((padButtonForField & 0x4000) != 0) {
                gDialogWindows[param_1].m382_CurrentMultiChoiceSelection++;
                if(gDialogWindows[param_1].m380_numMaxMultiChoiceSelection + -1 < (int)gDialogWindows[param_1].m382_CurrentMultiChoiceSelection)
                    gDialogWindows[param_1].m382_CurrentMultiChoiceSelection = 0;
            }
            if ((padButtonForField & 0x1000) != 0) {
                gDialogWindows[param_1].m382_CurrentMultiChoiceSelection = gDialogWindows[param_1].m382_CurrentMultiChoiceSelection + -1;
                if (gDialogWindows[param_1].m382_CurrentMultiChoiceSelection < 0) {
                    gDialogWindows[param_1].m382_CurrentMultiChoiceSelection = gDialogWindows[param_1].m380_numMaxMultiChoiceSelection + -1;
                }
            }
            setSelectedLineForMultiChoice(&gDialogWindows[param_1].m18, (char)gDialogWindows[param_1].m382_CurrentMultiChoiceSelection + (char)gDialogWindows[param_1].m37E_startOfMultiChoice);
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

const std::array<u8, 8> dpadButtonMapping = { {
    0,1,2,3,4,5,6,7
} };

int printDialogTextVar = 0;

std::array<s16, 10> dialogSpecialCase9AndATable;
s16 dialogSpecialCase9AndAVar0;
s16 dialogSpecialCase9AndAVar1;

void dialogSpecialCase9AndASub0(s16* param_1)
{
    ushort uVar1;
    char cVar2;
    char* pcVar3;
    char* pcVar4;
    int iVar5;

    assert(0);
    /*
    pcVar4 = &dialogSpecialCase9AndA_2Data;
    iVar5 = *(int*)(printDialogTextVar + 0x6c);
    uVar1 = *param_1;
    while (uVar1 != 0xffff) {
        pcVar3 = (char*)((uint)uVar1 * 2 + iVar5);
        param_1 = param_1 + 1;
        if (*pcVar3 == '\0') {
            cVar2 = pcVar3[1];
        }
        else {
            *pcVar4 = *pcVar3;
            cVar2 = pcVar3[1];
            pcVar4 = pcVar4 + 1;
        }
        *pcVar4 = cVar2;
        pcVar4 = pcVar4 + 1;
        uVar1 = *param_1;
    }
    *pcVar4 = '\0';
    * */
    return;
}

void dialogSpecialCase9AndA_2(sDialogWindow18* param_1, std::vector<u8>::iterator string) {
    param_1->m20 = param_1->m1C_currentStringToPrint;
    param_1->m1C_currentStringToPrint = string;
    param_1->m10_flags |= 0x80;
}

void dialogSpecialCase9AndA(s32 param_1, int param_2, int param_3) {
    
    uint uVar3;
    uint uVar4;
    int iVar5;
    uint uVar6; 
    
    uVar4 = 1000000000;
    uVar3 = param_2 * 0x10;
    if ((param_3 != 0) && (param_3 = 0xb, (int)param_1 < 0)) {
        param_1 = -param_1;
        param_3 = 10;
    }
    iVar5 = 0;
    std::array<s16, 10>::iterator psVar1 = dialogSpecialCase9AndATable.begin();
    do {
        uVar6 = param_1 / uVar4;
        param_1 = param_1 % uVar4;
        iVar5 = iVar5 + 1;
        uVar4 = uVar4 / 10;
        dialogSpecialCase9AndAVar0 = (short)uVar3;
        *psVar1 = (short)uVar6 + dialogSpecialCase9AndAVar0;
        psVar1 = psVar1 + 1;
    } while (iVar5 < 10);
    s16* puVar2 = &dialogSpecialCase9AndAVar0;
    dialogSpecialCase9AndAVar1 = -1;
    uVar4 = uVar3 & 0xfff0;
    while ((uVar4 == uVar3 && (puVar2 != (&dialogSpecialCase9AndATable[9])))) {
        puVar2 = puVar2 + 1;
        uVar4 = (uint)*puVar2;
    }
    if (param_3 != 0) {
        puVar2 = puVar2 + -1;
        *puVar2 = (short)param_3 + dialogSpecialCase9AndAVar0;
    }
    dialogSpecialCase9AndASub0(puVar2);
}

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
			param_1->m28_perLineBuffer[(param_1->m14).vy].m58_widthDiv4 = 0;
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
			param_1->m28_perLineBuffer[iVar13].m5E_clut = textSpriteMode0;
		}
		else {
			param_1->m28_perLineBuffer[iVar13].m5E_clut = textSpriteMode1;
		}
		param_1->m28_perLineBuffer[iVar13].m5A = (byte)characterToPrint & 1;
		param_1->m28_perLineBuffer[iVar13].m5B = (byte)characterToPrint;
		param_1->m28_perLineBuffer[iVar13].m50.y = param_1->mE + (short)((int)characterToPrint / 2) * 0xd;
	}

	if (param_1->m6C_autoClose != 0) {
		param_1->m6C_autoClose = 0;
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

		u8* pCharacterToPrint = &(*param_1->m1C_currentStringToPrint);
		u8 characterToPrint = *pCharacterToPrint;
		printDialogTextVar = iVar14;

		switch (characterToPrint)
		{
		case 0:
			if ((param_1->m10_flags & 0x80) == 0) {
				param_1->m10_flags |= 8;
				param_1->m6B = 1;
				param_1->m6C_autoClose = 1;
				return;
			}
			param_1->m10_flags &= ~0x80;
			param_1->m1C_currentStringToPrint = param_1->m20 + 1;
			iVar13 = iVar13 + -1;
			iVar14 = printDialogTextVar;
			break;
		case 1: // end of line
			(param_1->m0).vx = 100;
			param_1->m1C_currentStringToPrint++;
			return;
		case 2:
			param_1->m6B = '\x02';
			param_1->m10_flags = param_1->m10_flags | 0x48;
			param_1->m1C_currentStringToPrint++;
			if (*param_1->m1C_currentStringToPrint != '\x01') {
				return;
			}
			param_1->m1C_currentStringToPrint++;
			return;
		case 3:
			assert(0);
			break;
		case 0xF:
			// escape character;
			switch (pCharacterToPrint[1])
			{
			case 0:
				param_1->m84_delay = param_1->m1C_currentStringToPrint[2];
				param_1->m1C_currentStringToPrint += 3;
				return;
			case 1:
				if (param_1->m1C_currentStringToPrint[2] == 0) {
					int bVar1 = param_1->m6A;
					int bVar2 = param_1->m6A;
					param_1->m6A = 0;
					param_1->m68 = bVar1;
					param_1->m69 = bVar2;
				}
				else {
					int bVar2 = param_1->m68;
					iVar13 = iVar13 + param_1->m1C_currentStringToPrint[2];
					param_1->m68 = bVar1;
					param_1->m69 = bVar1;
					param_1->m6A = bVar2;
				}
				param_1->m1C_currentStringToPrint = param_1->m1C_currentStringToPrint + 3;
				break;
			case 2:
				param_1->m84_delay = param_1->m1C_currentStringToPrint[2];
				param_1->m1C_currentStringToPrint += 3;
				param_1->m6C_autoClose = 1;
				return;
            case 5: // print gear name
            {
                u8 characterToPrint = param_1->m1C_currentStringToPrint[2];
                param_1->m1C_currentStringToPrint += 2;
                if ((characterToPrint < 0x80) || (characterToPrint = gameState.m1D34_currentParty[characterToPrint - 0x80], characterToPrint != 0xff)) {
                    dialogSpecialCase9AndA_2(param_1, gameState.m0_names[characterToPrint].begin());
                }
                else {
                    assert(0);
                    //dialogSpecialCase9AndA_2(param_1, getDialogParamPointer(printDialogTextVar->m68), 0);
                }
                iVar14 = printDialogTextVar;
                continue;
            }
            case 0xA:
                param_1->m1C_currentStringToPrint = param_1->m1C_currentStringToPrint + 2;
                dialogSpecialCase9AndA(param_1->m70[param_1->m1C_currentStringToPrint[2]], 1, 0);
                //dialogSpecialCase9AndA_2(param_1, &dialogSpecialCase9AndA_2Data);
                MissingCode();
                continue;
			case 0xE:
				{
					int bVar1 = param_1->m68;
					param_1->m68 = 1;
					param_1->m69 = 1;
					param_1->m6A = bVar1;
					bVar1 = param_1->m1C_currentStringToPrint[2];
					param_1->m1C_currentStringToPrint = param_1->m1C_currentStringToPrint + 3;
					int uVar6 = (ushort)bVar1;
					param_1->m88_delayBetweenCharacters = uVar6;
					param_1->m86_currentDelayForNextCharacter = uVar6;
				}
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
			param_1->m1C_currentStringToPrint += iVar13;
			(param_1->m0).vx = sVar7;
			param_1->m28_perLineBuffer[sVar3].m58_widthDiv4 = sVar7;
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
		param_1->m1C_currentStringToPrint = psVar8->m4_dialogPointer;
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
		param_1->m10_flags &= ~0x40;
		param_1->m10_flags |= 0x20;
	}

	if ((param_1->m10_flags & 0x20) != 0) {
		sDialogWindow18PerLineBufferEntry* psVar5 = &param_1->m28_perLineBuffer[0];
		short sVar2 = param_1->mE;
		(param_1->m14).vy = 0;
		param_1->m18 = 0;
		(param_1->m0).vy = 0;
		(param_1->m0).vx = 0;
		psVar5->m5C = (byte)sVar2;
		param_1->m28_perLineBuffer[0].m5E_clut = textSpriteMode0;
		param_1->m28_perLineBuffer[0].m5A = 0;
		param_1->m28_perLineBuffer[0].m50.y = param_1->mE;
		if (0 < param_1->mC_height) {
			for (int i=0; i< param_1->mC_height; i++)
			{
				param_1->m28_perLineBuffer[i].m58_widthDiv4 = 0;
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

			if (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4 > 0x40)
			{
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].v0 = param_1->m28_perLineBuffer[iVar6].m5C;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].clut = param_1->m28_perLineBuffer[iVar6].m5E_clut;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].y0 = (param_1->m4).vy + (param_1->m14).vx * i;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1].w = (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4 - 0x40) * 4;
				addSPRT(OT, &param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][1]);
			}
			iVar6++;
		}
	}

	AddPrim(OT, &param_1->m30_textTileDrawMode[1]);

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
			if (param_1->m28_perLineBuffer[iVar6].m58_widthDiv4)
			{
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].v0 = param_1->m28_perLineBuffer[iVar6].m5C;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].clut = param_1->m28_perLineBuffer[iVar6].m5E_clut;
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].y0 = (param_1->m4).vy + (param_1->m14).vx * i;
				int uVar4 = param_1->m28_perLineBuffer[iVar6].m58_widthDiv4;
				if (0x40 < uVar4) {
					uVar4 = 0x40;
				}
				param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0].w = uVar4 * 4;
				addSPRT(OT, &param_1->m28_perLineBuffer[iVar6].m0[oddOrEven][0]);
			}
			iVar6++;
		}
	}

	if (param_1->m84_delay == 0) {
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
		param_1->m84_delay = param_1->m84_delay + -1;
	}

	if (param_1->m84_delay != 0)
	{
		if (--param_1->m84_delay == -1)
		{
			param_1->m10_flags = param_1->m10_flags & 0xffef;
		}
	}

	if ((param_1->m10_flags & 2) == 0) {
		assert(0);
	}

	param_1->m10_flags = param_1->m10_flags & 0xfeff;
	AddPrim(OT, &param_1->m30_textTileDrawMode[0]);
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

				if ((padButtonForDialogs & controllerButtons::INTERACT) != 0) {
					gDialogWindows[i].m37C = 0xffff;
					actorArray[gDialogWindows[i].m416_fieldActorId].m4C_scriptEntity->m81_selectedMultichoiceLine = gDialogWindows[i].m382_CurrentMultiChoiceSelection + gDialogWindows[i].m37E_startOfMultiChoice;
					addDialogWindowsToOTSub1(&gDialogWindows[i].m18);
				}
				if (gDialogWindows[i].m18.m82 == 0) {
					assert(0);
				}
				updateAndRenderTextForDialogWindow(&gDialogWindows[i].m18, OT, oddOrEven);
			}
			gDialogWindows[i].m0[oddOrEven].m0_pNext = OT->m0_pNext;
			OT->m0_pNext = &gDialogWindows[i].m0[oddOrEven];
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

						if (((padButtonForDialogs & controllerButtons::INTERACT) != 0) && (gDialogWindows[i].m410 == 0)) {
							gDialogWindows[i].m37C = -1;
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
					gDialogWindows[i].m0[oddOrEven].m0_pNext = OT->m0_pNext;
					OT->m0_pNext = &gDialogWindows[i].m0[oddOrEven];
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

	gDialogDrModes[g_frameOddOrEven][0].m0_pNext = OT->m0_pNext;
	OT->m0_pNext = &gDialogDrModes[g_frameOddOrEven][0];
}

void setupMultiChoice(sDialogWindow18* param_1, u8 R, u8 G, u8 B)
{
    for (int i=0; i<param_1->mC_height; i++)
    {
        param_1->m28_perLineBuffer[i].m0[1][1].r0 = R;
        param_1->m28_perLineBuffer[i].m0[1][0].r0 = R;
        param_1->m28_perLineBuffer[i].m0[0][1].r0 = R;
        param_1->m28_perLineBuffer[i].m0[0][0].r0 = R;

        param_1->m28_perLineBuffer[i].m0[1][1].g0 = G;
        param_1->m28_perLineBuffer[i].m0[1][0].g0 = G;
        param_1->m28_perLineBuffer[i].m0[0][1].g0 = G;
        param_1->m28_perLineBuffer[i].m0[0][0].g0 = G;

        param_1->m28_perLineBuffer[i].m0[1][1].b0 = B;
        param_1->m28_perLineBuffer[i].m0[1][0].b0 = B;
        param_1->m28_perLineBuffer[i].m0[0][1].b0 = B;
        param_1->m28_perLineBuffer[i].m0[0][0].b0 = B;
    }
}

void releaseAllDialogWindows(void)
{
    for (int i = 0; i < 4; i++) {
        if (!gDialogWindows[i].m40E) {
            releaseDialogWindow(i);
        }
    }
}