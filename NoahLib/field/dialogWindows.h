#pragma once

#include "kernel/graphics.h"
#include "kernel/math.h"
#include "kernel/messageWindow.h"

struct sDialogWindow
{
	DR_MODE m0[2];
	sDialogWindow18 m18;
	sVec2_s16 mAC_Position;
	sVec2_s16 mB0_Size;
	DR_MODE mC4[2];
	TILE mDC_backgroundTile[2];
	DR_MODE mFC[2][10];
	SPRT m1EC_windowBorders[2][10];
	s16 m37C;
	s8 m37E_startOfMultiChoice;
    s16 m380_numMaxMultiChoiceSelection;
    s16 m382_CurrentMultiChoiceSelection;
	DR_MODE m384[2];
	SPRT m39C_Sprt[2];
	s16 m3C4;
	DR_MODE m3C8[2];
	SPRT m3E0_Sprt[2];
	s16 m408_openAnimationCounter;
	s16 m40A;
	s16 m40E;
	u16 m40C_flags;
	s16 m410;
	s16 m412;
	s16 m414;
	s16 m416_fieldActorId;
	s16 m418_actorId;
	s32 m41C[2];
	s32 m424[2];
	DR_MODE m42C_portraitDrawMode[2];
	POLY_FT4 m444_portraitPoly[2];
	u8 m494_hasPortrait;
	u8 m495_portrait;
	// size 0x498
};

extern std::array<sDialogWindow, 4> gDialogWindows;
extern s16 dialogWindowOpenAnimationNumFrames;

extern u16 padButtonForDialogs;
extern u16 padButtonForDialogs2;

extern std::array<std::array<DR_MODE, 16>, 2> gDialogDrModes;

extern std::array<u8, 3> dialogWindowColor;
extern s32 dialogBattleWindowAbr;

extern u16 windowOpenBF;

void updateDialogTextImage(sDialogWindow18* param_1);
void flagWindowOpenBF(uint param_1);
void createDialogWindow(short x, short y, int dialogIndex, int windowIndex, int width, int height, int fieldActorId, int actorId, int mode, uint param_10, uint param_11);
void initDialogWindows();
int findDialogWindowForCurrentActor(int* outputId);
int findFreeDialogWindow();
int allocateDialogWindow();
int getWindowWithLowestPriority();
void stepDialogWindows();
void addDialogWindowsToOT(sTag* OT, int oddOrEven);
s8 addDialogWindowsToOTSub0(sDialogWindow18* param_1);
void setupMultiChoiceColor(sDialogWindow18* param_1, u8 R, u8 G, u8 B);
std::vector<u8>::iterator getDialogParamPointer(const std::vector<u8>::iterator& buffer, int param_2);
void releaseAllDialogWindows(void);
void setupWindowSize2(sDialogWindow18* param_1, int x1, int y1, short x2, short y2, short width, short height);
void resetDialogWindow18(sDialogWindow18* param_1);
void addDialogWindowsToOTSub2(sDialogWindow18* param_1, const std::vector<u8>::iterator& param_2);
void updateAndRenderTextForDialogWindow(sDialogWindow18* param_1, sTag* OT, int oddOrEven);
void dealocateDialogWindow18Memory(sDialogWindow18* param_1);
void addDialogWindowsToOTSub1(sDialogWindow18* param_1);
