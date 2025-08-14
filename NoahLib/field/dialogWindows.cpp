#include "noahLib.h"
#include "kernel/graphics.h"
#include "kernel/font.h"
#include "dialogWindows.h"
#include "fieldScriptSupport.h"
#include "kernel/TIM.h"
#include "kernel/gameState.h"
#include "kernel/inputs.h"
#include "field/fieldInputs.h"

std::array<sDialogWindow, 4> gDialogWindows;
std::array<s32, 4> dialogWindowVar1;

std::array<std::array<DR_MODE, 16>,2> gDialogDrModes;
std::array<RECT, 16> gDialogRects;

std::array<u8, 3> dialogWindowColor;
s32 dialogBattleWindowAbr = 0;

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

const std::vector<sVec2_s16> windowsDefaultPositions = {
	{0x300, 0x100},
	{0x300, 0x180},
	{0x300, 0x134},
	{0x300, 0x1B4},
};

s16 dialogWindowOpenAnimationNumFrames = 0;


void createDialogWindow(short x, short y, int dialogIndex, int windowIndex, int width, int height, int fieldActorId, int actorId, int mode, uint param_10, uint param_11)
{
	int y2 = y - 8;
	u32 flagUpper = actorArray[fieldActorId].m4C_scriptEntity->m84_dialogWindowFlags >> 16;
	if (flagUpper == 0)
	{
		flagUpper = actorArray[fieldActorId].m4C_scriptEntity->m84_dialogWindowFlags & 0xFFFF;
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
	gDialogWindows[windowIndex].m18.m90_dialogPointer = getDialogParamPointer(rawFieldDialogBundle.getRawData().begin(), dialogIndex);
	gDialogWindows[windowIndex].m40E = 0;
	gDialogWindows[windowIndex].m18.m10_flags.m02 = 1;
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



u16 windowOpenBF = 0;

void flagWindowOpenBF(uint param_1)
{
	windowOpenBF = windowOpenBF | (ushort)(1 << (param_1 & 0x1f));
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
			if ((gDialogWindows[i].m408_openAnimationCounter == 0) && !gDialogWindows[i].m18.m10_flags.m04) {
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

		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].x0y0.vx = (short)windowX;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].x0y0.vy = (short)windowY + 1;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].wh.vx = (short)windowWidth;
		gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].wh.vy = (short)windowHeight + -2;

		if ((gDialogWindows[windowIndex].m40C_flags & 0x40U) == 0) {
			gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven].m0_pNext = OT->m0_pNext;
			OT->m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven];

			gDialogWindows[windowIndex].mC4[oddOrEven].m0_pNext = &gDialogWindows[windowIndex].mDC_backgroundTile[oddOrEven];
			OT->m0_pNext = &gDialogWindows[windowIndex].mC4[oddOrEven];
		}
	}
}

void updateMultiChoice(int param_1)
{
	if ((gDialogWindows[param_1].m37C == 0) && (gDialogWindows[param_1].m408_openAnimationCounter == 0)) {
		if (gDialogWindows[param_1].m410 == 0) {
            if ((g_padButtonForField & 0x4000) != 0) {
                gDialogWindows[param_1].m382_CurrentMultiChoiceSelection++;
                if(gDialogWindows[param_1].m380_numMaxMultiChoiceSelection + -1 < (int)gDialogWindows[param_1].m382_CurrentMultiChoiceSelection)
                    gDialogWindows[param_1].m382_CurrentMultiChoiceSelection = 0;
            }
            if ((g_padButtonForField & 0x1000) != 0) {
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

void releaseAllDialogWindows(void)
{
    for (int i = 0; i < 4; i++) {
        if (!gDialogWindows[i].m40E) {
            releaseDialogWindow(i);
        }
    }
}