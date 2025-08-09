#pragma once
#include "field.h"

extern int currentFieldActorId;
extern std::vector<u8>::iterator pCurrentFieldScriptFile;
extern sFieldScriptEntity* pCurrentFieldScriptActor;

extern s32 op99Var1;
extern s32 op99Var5;
extern s32 op99Var6;
extern s32 op99Var7;
extern s16 cameraInterpolationMode;
extern s16 op99Var2;
extern s16 op99Var3;
extern s16 op99Var4;
extern s16 sceneScale;
extern s16 sceneDIP;
extern SVECTOR cameraRotationBetweenEyeAndAt;
extern s16 op9DVar0;
extern s32 op9DVar1;
extern s32 op9DVar2;
extern s32 op9DVar3;

extern s32 iRam800adb70;

extern s32 dialogWindowFlag2;

int getVar80(int param_1, uint param_2);
int getVar40(int param_1, uint param_2);
int getVar20(int param_1, uint param_2);
int getVar10(int param_1, uint param_2);
int getVar08(int param_1, uint param_2);
int getVar04(int param_1, uint param_2);
int getVar02(int param_1, uint param_2);
int getVar01(int param_1, uint param_2);

u32 getCameraDirection(void);

uint getVariable(int param_1);

s16 readS16FromScript(int param_1);
u16 readU16FromScript(int param_1);

int getBytesSign(int param_1);

uint readCharacter(int param_1);
uint getImmediateOrVariableUnsigned(int param_1);
void playMusicFromScript();
void jumpIfMask(u16 mask);

void resetPortraitLoadingStatus();
int isDialogAvatarLoaded(int faceId);
int showDialogWindowForActor(int, int);

void projectActorToScreen(int param_1, int* param_2, int* param_3, short param_4);

void SaveFieldAndDirections(void);
void OP_CHANGE_FIELD_WHEN_READY_Sub();
s32 distance3d(long param_1, long param_2, long param_3);

extern s32 numDialogWindowsCreatedThisFrame;
extern u8 updateCharacterVar0;
