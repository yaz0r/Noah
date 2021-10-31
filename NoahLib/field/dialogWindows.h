#pragma once

#include "kernel/graphics.h"
#include "kernel/math.h"

struct sDialogWindow18PerLineBufferEntry
{
	SPRT m0[2][2];
	RECT m50;
	s16 m58_widthDiv4;
	s8 m5A;
	s8 m5B;
	s16 m5C;
	u16 m5E_clut;
	// size 0x60
};

struct sDialogWindow18_8C
{
	sDialogWindow18_8C* m0_pNext;
	std::vector<u8>::iterator m4_dialogPointer;
	// size 0x8
};

struct sDialogWindow18
{
	sVec2_s16 m0;
	sVec2_s16 m4;
	s16 m8_widthTimes4;
	s16 mA_width1;
	s16 mC_height;
	s16 mE;
	u16 m10_flags;
	s16 m12_widthPadded;
	sVec2_s16 m14;
	s16 m18;
	std::vector<u8>::iterator m1C;
	std::vector<u8>::iterator m20;
	std::vector<sDialogWindow18PerLineBufferEntry> m28_perLineBuffer;
	std::vector<u16> m2C_inRamDialogTextImage;
	DR_MODE m30_textTileDrawMode[2];
	TILE m48_textTile[2];
	s8 m68;
	s8 m69;
	s8 m6A;
	s8 m6B;
	s8 m6C_autoClose;
	s8 m6D;
	s8 m6E_selectedLineForMultiChoice;
	s32 m70[4];
	s16 m80;
	s16 m82;
	s16 m84_delay;
	s16 m86_currentDelayForNextCharacter;
	s16 m88_delayBetweenCharacters;
	sDialogWindow18_8C* m8C;
	std::vector<u8>::iterator m90_dialogPointer;
};

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
	s8 m382_CurrentMultiChoiceSelection;
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
	DR_MODE m42C_FaceDrawMode[2];
	POLY_FT4 m444_FacePoly[2];
	u8 m494_hasDialogFace;
	u8 m495_dialogFaceId;
	// size 0x498
};

extern std::array<sDialogWindow, 4> gDialogWindows;
extern s16 dialogWindowFlag1;

extern u16 textSpriteMode0;
extern u16 textSpriteMode1;

extern u16 padButtonForDialogs;
extern u16 padButtonForDialogs2;

extern std::array<std::array<DR_MODE, 16>, 2> gDialogDrModes;

extern std::array<u8, 3> dialogWindowColor;

void flagWindowOpenBF(uint param_1);
void createDialogWindow(short x, short y, int dialogIndex, int windowIndex, int width, int height, int fieldActorId, int actorId, int mode, uint param_10, uint param_11);
void initDialogWindows();
int findDialogWindowForCurrentActor(int* outputId);
int findFreeDialogWindow();
int allocateDialogWindow();
int getWindowWithLowestPriority();
void stepDialogWindows();
void addDialogWindowsToOT(sTag* OT, int oddOrEven);
