#include "noahLib.h"
#include "fieldScriptSupport.h"
#include "dialogWindows.h"
#include "kernel/filesystem.h"
#include "kernel/TIM.h"
#include "kernel/kernelVariables.h"

int currentFieldActorId;
std::vector<u8>::iterator pCurrentFieldScriptFile;
sFieldScriptEntity* pCurrentFieldScriptActor;

s32 op99Var1 = 0;
s32 op99Var5 = 0;
s32 op99Var6 = 0;
s32 op99Var7 = 0;
s16 cameraInterpolationMode = 0;
s16 op99Var2 = 0;
s16 op99Var3 = 0;
s16 op99Var4 = 0;
s16 sceneScale = 0;
s16 sceneDIP = 0;

SVECTOR cameraRotationBetweenEyeAndAt;

s16 op9DVar0 = 0;
s32 op9DVar1 = 0;
s32 op9DVar2 = 0;
s32 op9DVar3 = 0;

u8 updateCharacterVar0 = 0;

int getVar80(int param_1, uint param_2)

{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x80) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar40(int param_1, uint param_2)

{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x40) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}


int getVar20(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x20) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar10(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x10) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar08(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x08) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar04(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x04) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar02(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x02) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar01(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x01) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

uint readCharacter(int param_1)
{
    uint uVar1;
    uint uVar2;

    uVar1 = (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1];
    uVar2 = partyToFieldEntityArrayMapping[0];
    if ((((uVar1 != 0xff) && (uVar2 = partyToFieldEntityArrayMapping[1], uVar1 != 0xfe)) && (uVar2 = partyToFieldEntityArrayMapping[2], uVar1 != 0xfd)) && (uVar2 = uVar1, uVar1 == 0xfb)) {
        uVar2 = currentFieldActorId;
    }
    return uVar2;
}

uint getImmediateOrVariableUnsigned(int param_1)
{
    ushort uVar1;
    uint uVar2;

    uVar1 = readU16FromScript(param_1);
    uVar2 = (uint)uVar1 & 0x7fff;
    if ((uVar1 & 0x8000) == 0) {
        uVar2 = getVariable((uint)uVar1);
    }
    return uVar2;
}

/*
GameProgress:

0x3: Prolog text done
0x6: Fei painting intro done
0x7: Talked to Dan (before leaving Fei's house)
0x8: Talked to Alice
0xA: Looking for Citan around his house
0xC: Citan told Fei to look in storage room (music box)
0xD: At citan, night time, Fei leaves
0xF: At citan, night time, heading back to Lahan, Citan waiting outside after Fei's departure

0x15: if > 0x15, Yui is not in Citan's kitchen
*/

/*
0x0: GameProgress
0xA: event time
0xC/0xE: system time
*/
uint getVariable(int param_1)
{
    uint uVar1;

    uVar1 = param_1 >> 1;
    if ((*(uint*)(&rawFieldScriptData[0] + (param_1 >> 6) * 4) & 1 << (uVar1 & 0x1f)) == 0) {
        uVar1 = fieldVars[uVar1];
    }
    else {
        uVar1 = (uint)(ushort)fieldVars[uVar1];
    }
    return uVar1;
}

u16 readU16FromScript(int param_1)
{
    return *(ushort*)
        (&pCurrentFieldScriptFile[0] + (uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1);
}

s16 readS16FromScript(int param_1)
{
    return (int)(((uint)pCurrentFieldScriptFile
        [(uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1] +
        (uint)(pCurrentFieldScriptFile +
            (uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1)[1] * 0x100) * 0x10000)
        >> 0x10;
}


int getBytesSign(int param_1)
{
    return -(uint)((*(uint*)(&rawFieldScriptData[0] + (param_1 >> 6) * 4) & 1 << (param_1 >> 1 & 0x1fU)) != 0);
}

void playMusicFromScript()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    iVar2 = getImmediateOrVariableUnsigned(1);
    if (fieldExecuteVar1 == 0) {
        clearMusic2();
        if (iVar2 != currentlyPlayingMusic) {
            clearMusic();
            fieldMusicLoadPending = -1;
        }
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC;
        currentlyPlayingMusic = iVar2;
    }
    else {
        iVar3 = isLoadCompleted();
        if ((iVar3 != 0) || (playMusicAuthorized == 0)) {
            breakCurrentScript = 1;
            return;
        }
        if ((musicVar2 == 1) || (fieldMusicLoadPending == -1)) {
            breakCurrentScript = 1;
            return;
        }
        if (iVar2 != currentlyPlayingMusic) {
            clearMusic();
            fieldMusicLoadPending = -1;
            currentlyPlayingMusic = iVar2;
            playMusic(iVar2);
        }
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1 + 3;
}

void jumpIfMask(u16 mask)
{
    u16 uVar1 = readU16FromScript(1);
    if ((uVar1 & mask) == 0) {
        uVar1 = readU16FromScript(3);
    }
    else {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

struct sPortraitLoadingStatus {
    s16 m0_id;
    s16 m2_status;
    s16 m4_isAsymmetric;
};

std::array<sPortraitLoadingStatus, 3> portraitLoadingStatus;
std::array<sLoadableDataRaw, 2> partyPortraitPointers;
int currentPortraitLoadingSlot = 0;

void resetPortraitLoadingStatus() {
    for (int i = 0; i < 3; i++) {
        portraitLoadingStatus[i].m0_id = -1;
        portraitLoadingStatus[i].m2_status = -1;
    }
    currentPortraitLoadingSlot = 0;
}

std::array<std::array<RECT, 2>, 3> partyPortraitsRects = { {
    {{{0x2C0, 0x100, 0x0, 0xE1}, {0x2E0, 0x100, 0x0, 0xE0}}},
    {{{0x2C0, 0x140, 0x0, 0xE3}, {0x2E0, 0x140, 0x0, 0xE2}}},
    {{{0x2C0, 0x180, 0x0, 0xE5}, {0x2E0, 0x180, 0x0, 0xE4}}},
}};

std::array<sLoadingBatchCommands, 3> portraitsLoadingCommands;

std::array<std::array<u8, 2>, 90> portraitMapping = { {
    {0x0, 0x0},
    {0x6, 0x6},
    {0x11, 0x11},
    {0x13, 0x14},
    {0x15, 0x15},
    {0x17, 0x17},
    {0x18, 0x18},
    {0x1C, 0x1C},
    {0x1B, 0x1B},
    {0x11, 0x11},
    {0x19, 0x19},
    {0x22, 0x22},
    {0x23, 0x23},
    {0x24, 0x24},
    {0x25, 0x25},
    {0x4F, 0x4F},
    {0x52, 0x52},
    {0x53, 0x53},
    {0x1A, 0x1A},
    {0x34, 0x34},
    {0x51, 0x51},
    {0x4D, 0x4D},
    {0x4E, 0x4E},
    {0x21, 0x21},
    {0x29, 0x29},
    {0x1D, 0x1D},
    {0x2B, 0x2B},
    {0x32, 0x33},
    {0x2A, 0x2A},
    {0x35, 0x35},
    {0x38, 0x38},
    {0x26, 0x26},
    {0x1, 0x1},
    {0x2, 0x2},
    {0x3, 0x3},
    {0x4, 0x4},
    {0x5, 0x5},
    {0x7, 0x7},
    {0x8, 0x8},
    {0x9, 0x9},
    {0xA, 0xA},
    {0xB, 0xB},
    {0xC, 0xC},
    {0xD, 0xD},
    {0xE, 0xE},
    {0xF, 0xF},
    {0x10, 0x10},
    {0x12, 0x12},
    {0x1E, 0x1E},
    {0x1F, 0x1F},
    {0x20, 0x20},
    {0x27, 0x27},
    {0x28, 0x28},
    {0x2C, 0x2C},
    {0x2D, 0x2D},
    {0x2E, 0x2E},
    {0x2F, 0x2F},
    {0x30, 0x30},
    {0x31, 0x31},
    {0x36, 0x36},
    {0x16, 0x16},
    {0x39, 0x39},
    {0x3A, 0x3A},
    {0x3B, 0x3B},
    {0x3C, 0x3C},
    {0x3D, 0x3D},
    {0x3E, 0x3E},
    {0x3F, 0x3F},
    {0x40, 0x40},
    {0x41, 0x41},
    {0x42, 0x42},
    {0x43, 0x43},
    {0x44, 0x44},
    {0x45, 0x45},
    {0x46, 0x46},
    {0x47, 0x47},
    {0x48, 0x48},
    {0x49, 0x49},
    {0x4A, 0x4A},
    {0x4B, 0x4B},
    {0x4C, 0x4C},
    {0x50, 0x50},
    {0x37, 0x37},
    {0x54, 0x54},
    {0x55, 0x55},
    {0x56, 0x56},
    {0x57, 0x57},
    {0x58, 0x58},
    {0x59, 0x59},
    {0x5A, 0x5A},
} };

int isPortraitUsed(uint param_1)
{
    int i = 0;
    while ((((gDialogWindows[i].m40E) != 0 || (((int)(gDialogWindows[i].m494_hasPortrait)) != 1)) ||
        (((int)(gDialogWindows[i].m495_portrait)) != param_1))) {
        i = i + 1;
        if (3 < i) {
            return 0;
        }
    }
    return -1;
}

int isDialogAvatarLoaded(int portraitId)
{
    for (int i = 0; i < 3; i++) {
        switch (portraitLoadingStatus[i].m2_status) {
        case 0: // idle
            break;
        case 1: // wait loading
            if(waitReadCompletion(1) == 0) {
                portraitLoadingStatus[i].m2_status = 2;
                loadTimToVram(partyPortraitPointers[0].mData.begin(), partyPortraitsRects[i][0].x, partyPortraitsRects[i][0].y, partyPortraitsRects[i][0].w, partyPortraitsRects[i][0].h, 0x100, 1);

                // handle portrait asymmetry
                if (portraitLoadingStatus[i].m4_isAsymmetric == 0) {
                    loadTimToVram(partyPortraitPointers[0].mData.begin(), partyPortraitsRects[i][1].x, partyPortraitsRects[i][1].y, partyPortraitsRects[i][1].w, partyPortraitsRects[i][1].h, 0x100, 1);
                }
                else {
                    loadTimToVram(partyPortraitPointers[1].mData.begin(), partyPortraitsRects[i][1].x, partyPortraitsRects[i][1].y, partyPortraitsRects[i][1].w, partyPortraitsRects[i][1].h, 0x100, 1);
                }
            }
            return -1;
        case 2: // cleanup
            portraitLoadingStatus[i].m2_status = 0;
            break;
        }

    }

    for (int i = 0; i < 3; i++) {
        if (portraitLoadingStatus[i].m0_id == portraitId) {
            pCurrentFieldScriptActor->m12C_flags &= ~(7 << 2);
            pCurrentFieldScriptActor->m12C_flags |= (i & 7) << 2;
            return 0;
        }
    }

    {
        bool bVar1 = false;
        int i = 0;
        while (true) {
            currentPortraitLoadingSlot = currentPortraitLoadingSlot + 1;
            if (2 < (int)currentPortraitLoadingSlot) {
                currentPortraitLoadingSlot = 0;
            }

            int iVar2 = isPortraitUsed((int)portraitLoadingStatus[currentPortraitLoadingSlot].m0_id);
            i = i + 1;
            if (iVar2 == 0) {
                bVar1 = true;
                goto LAB_Field__8009c364;
            }
            if (2 < i) {
            LAB_Field__8009c364:
                if (bVar1) {
                    pCurrentFieldScriptActor->m12C_flags = pCurrentFieldScriptActor->m12C_flags & 0xffffffe3 | (currentPortraitLoadingSlot & 7) << 2;
                    setCurrentDirectory(4, 0);
                    portraitLoadingStatus[currentPortraitLoadingSlot].m0_id = (short)portraitId;
                    portraitLoadingStatus[currentPortraitLoadingSlot].m2_status = 1;
                    portraitLoadingStatus[currentPortraitLoadingSlot].m4_isAsymmetric = 0;
                    portraitsLoadingCommands[0].m0_fileIndex = portraitMapping[portraitId][0] + 0x46;
                    partyPortraitPointers[0].resize(getFileSizeAligned(portraitsLoadingCommands[0].m0_fileIndex));
                    i = 1;
                    portraitsLoadingCommands[0].m4_loadPtr = &partyPortraitPointers[0];
                    if (portraitMapping[portraitId][1] != portraitMapping[portraitId][0]) {
                        portraitLoadingStatus[currentPortraitLoadingSlot].m4_isAsymmetric = 1;
                        portraitsLoadingCommands[1].m0_fileIndex = portraitMapping[portraitId][1] + 0x46;
                        i = 2;
                        partyPortraitPointers[1].resize(getFileSizeAligned(portraitsLoadingCommands[1].m0_fileIndex));
                        portraitsLoadingCommands[1].m4_loadPtr = &partyPortraitPointers[1];
                    }
                    portraitsLoadingCommands[i].m0_fileIndex = 0;
                    portraitsLoadingCommands[i].m4_loadPtr = nullptr;
                    batchStartLoadingFiles(&portraitsLoadingCommands[0], 0);
                }
                return -1;
            }
        }
    }
}

int getDialogParam0(const std::vector<u8>& bundle, int index)
{
    return bundle[index * 2 + READ_LE_U16(&bundle[0]) * 2 + 6];
}

int getDialogParam1(const std::vector<u8>& bundle, int index)
{
	return bundle[index * 2 + READ_LE_U16(&bundle[0]) * 2 + 7];
}

s32 iRam800afd04 = 0;
s32 numDialogWindowsCreatedThisFrame = 0;
s32 iRam800adb64 = 0xFF;
s32 iRam800adb70 = 0;
s32 dialogWindowFlag2 = 0;

u32 getCameraDirection(void)
{
	return 7U - ((camera2Tan + -0x100) >> 9) & 7;
}

uint getPlayerCharacterDir(void)
{
	return (((actorArray[playerControlledActor].m4C_scriptEntity)->m106_currentRotation + 0x100) >> 9) + 2U & 7;
}

void SaveFieldAndDirections(void)
{
	short sVar1;

	setVar(4, (ushort)fieldMapNumber & 0x3fff);
	sVar1 = getPlayerCharacterDir();
	setVar(6, sVar1);
	sVar1 = getCameraDirection();
	setVar(8, sVar1);
	sVar1 = getVariable(0x12);
	setVar(0x12, sVar1 + 1);
	return;
}

void OP_CHANGE_FIELD_WHEN_READY_Sub()
{
	// Nothing!
}

void projectActorToScreen(int param_1, int* param_2, int* param_3, short param_4)
{
	SVECTOR local_50;
	MATRIX MStack72;
    sVec2_s16 local_28;
	long lStack36;
	long alStack32[2];

	CompMatrix(&currentProjectionMatrix, &actorArray[param_1].mC_matrix, &MStack72);
	SetRotMatrix(&MStack72);
	SetTransMatrix(&MStack72);
	local_50.vx = 0;
	local_50.vz = 0;
	local_50.vy = param_4;
	RotTransPers(&local_50, &local_28, &lStack36, alStack32);
	*param_3 = (int)local_28.vy;
	*param_2 = (int)local_28.vx;
}

int showDialogWindowForActor(int actorId, int mode)
{
	fieldExectuteMaxCycles += 0x20;

	int iVar6;
	if (((((loadCompleted != 0) || (iRam800afd04 != 0)) || (numDialogWindowsCreatedThisFrame != 0)) || (iRam800adb64 != 0xff)) ||
		(((iRam800adb70 == 0 && (iVar6 = isLoadCompleted(), iVar6 != 0)) ||
			((pCurrentFieldScriptActor->m80_dialogPortrait != -1 && (iVar6 = isDialogAvatarLoaded((uint)pCurrentFieldScriptActor->m80_dialogPortrait), iVar6 == -1)))))) {
		breakCurrentScript = 1;
		return -1;
	}
	numDialogWindowsCreatedThisFrame++;

    int dialogWindow;
	if (findDialogWindowForCurrentActor(&dialogWindow) != -1) {
		breakCurrentScript = 1;
        gDialogWindows[dialogWindow].m414 = 0;
		return -1;
	}
	fieldExectuteMaxCycles += 8;

    u16 dialogIndex = readU16FromScript(1);
    int windowIndex = findFreeDialogWindow();
	if (windowIndex == 0) {
		windowIndex = allocateDialogWindow();
	}
    else
    {
		windowIndex = getWindowWithLowestPriority();
		if (windowIndex != -1) {
			gDialogWindows[windowIndex].m414 = 0;
			breakCurrentScript = 1;
			return -1;
		}
    }

    // get the combined flags of all active windows
    int numActiveWindows = 0;
    int activeWindowsFlags = 0;
    for (int i=0; i<4; i++)
    {
        if (gDialogWindows[i].m40E == 0)
        {
            numActiveWindows++;
            activeWindowsFlags |= gDialogWindows[i].m40C_flags;
        }
    }

	int dialogBoxWidth = getDialogParam0(rawFieldDialogBundle, dialogIndex);
	int dialogBoxHeight = getDialogParam1(rawFieldDialogBundle, dialogIndex);

	if ((mode == 0) || (mode == 3)) {
		if (pCurrentFieldScriptActor->m82[0] != 0) {
			dialogBoxWidth = pCurrentFieldScriptActor->m82[0];
		}
		if (pCurrentFieldScriptActor->m82[1] != 0) {
			dialogBoxHeight = pCurrentFieldScriptActor->m82[1];
		}
	}

    int old84 = pCurrentFieldScriptActor->m84;
    int windowDisplayMode = old84 & 0xffff;
    pCurrentFieldScriptActor->m84 = windowDisplayMode;
    int flagFromScript = windowDisplayMode;
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] != 0) {
        flagFromScript = old84 & 0xff00 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3];
        pCurrentFieldScriptActor->m84 = windowDisplayMode | flagFromScript << 0x10;
	}

    windowDisplayMode = (flagFromScript >> 4) & 3;

    int boxY = 0x10;
    int projectedPosition[2];

    if (windowDisplayMode == 1) {
    LAB_Field__8009c8e0:
        boxY = windowIndex << 3;
    LAB_Field__8009c8e4:
        gDialogWindows[windowIndex].m40C_flags = 1;
        if ((mode == 0) || (mode == 3)) {
            projectActorToScreen(actorId, projectedPosition, projectedPosition + 1, -0x40);
            boxY = 0x14;
            if (mode == 0) {
                boxY = projectedPosition[1] + dialogBoxHeight * -0xe + -0x24;
            }
            else {
                projectedPosition[0] = 0xa0;
            }
            if ((pCurrentFieldScriptActor->m80_dialogPortrait != -1) && ((flagFromScript & 2) == 0)) {
                dialogBoxHeight = 4;
                if ((int)dialogBoxWidth < 0x18) {
                    dialogBoxWidth = 0x18;
                }
                dialogBoxWidth = dialogBoxWidth + 0x11;
                boxY = 0x10;
            }
            goto LAB_Field__8009ca64;
        }
        boxY = 0x10;
    }
    else {
        if (windowDisplayMode == 0) {
            if (((pCurrentFieldScriptActor->m12C_flags >> 9 & 7) - (getCameraDirection() & 0xffff) & 7) < 5) {
                if ((activeWindowsFlags & 0x80) != 0) goto LAB_Field__8009c8e0;
            }
            else {
                if (((activeWindowsFlags & 0x80) == 0) && (boxY = windowIndex * 8, numActiveWindows == 0)) goto LAB_Field__8009c8e4;
            }
        }
        else {
            if (windowDisplayMode != 2) goto LAB_Field__8009ca64;
        }
        gDialogWindows[windowIndex].m40C_flags = 0x81;
        if ((mode == 0) || (boxY = 0x94, mode == 3)) {
            projectActorToScreen(actorId, projectedPosition, projectedPosition + 1, -0x40);
            boxY = 0x94;
            if (mode == 0) {
                boxY = projectedPosition[1] + 0x30;
            }
            else {
                projectedPosition[0] = 0xa0;
            }
            int bVar2;
            if ((pCurrentFieldScriptActor->m80_dialogPortrait != -1) && (bVar2 = (int)dialogBoxWidth < 0x18, (flagFromScript & 2) == 0)) {
                dialogBoxWidth = dialogBoxWidth + 0x11;
                if (bVar2) {
                    dialogBoxWidth = 0x29;
                }
                dialogBoxHeight = 4;
                boxY = 0x94;
            }
            goto LAB_Field__8009ca64;
        }
    }
    dialogBoxHeight = 4;
    dialogBoxWidth = 0x48;
    projectedPosition[0] = 0xa0;

LAB_Field__8009ca64:
    int boxX3 = projectedPosition[0] + -8 + dialogBoxWidth * -2;
    int boxX2 = boxX3 + 0x10;
	if (boxX3 < 0xc) {
        boxX3 = 0xc;
        boxX2 = 0x1c;
	}
	if (0x134 < (int)(boxX2 + dialogBoxWidth * 4)) {
		boxX3 = (short)dialogBoxWidth * -4 + 0x124;
	}
    int boxY22 = boxY + 8;
	if (boxY < 0x10) {
        boxY = 0x10;
        boxY22 = 0x18;
	}
    int boxY2 = (short)boxY;
	if (0xd4 < (int)(boxY22 + dialogBoxHeight * 0xe)) {
        boxY2 = (short)dialogBoxHeight * -0xe + 0xcc;
	}
	if ((mode == 0) || (mode == 3)) {
		if (pCurrentFieldScriptActor->m88[0] != 0) {
			boxX3 = pCurrentFieldScriptActor->m88[0];
		}
		if (pCurrentFieldScriptActor->m88[1] != 0) {
            boxY2 = pCurrentFieldScriptActor->m88[1];
		}
		if (pCurrentFieldScriptActor->m82[0] != 0) {
			dialogBoxWidth = (uint)pCurrentFieldScriptActor->m82[0];
		}
		if (pCurrentFieldScriptActor->m82[1] != 0) {
			dialogBoxHeight = (uint)pCurrentFieldScriptActor->m82[1];
		}
		if ((pCurrentFieldScriptActor->m80_dialogPortrait != -1) && (((pCurrentFieldScriptActor->m84 >> 16) & 2) == 0)) {
			dialogBoxHeight = 4;
		}
	}
	if (((pCurrentFieldScriptActor->m84 >> 16) & 0x40) != 0) {
		gDialogWindows[windowIndex].m40C_flags = gDialogWindows[windowIndex].m40C_flags | 0x40;
	}

	int flags = 0;
	if (((pCurrentFieldScriptActor->m84 >> 16) & 0xc) == 0) {
		windowDisplayMode = getCameraDirection();
		flags = (((((int)((uint)(ushort)(actorArray[actorId].m4C_scriptEntity)->m106_currentRotation << 0x10) >> 0x19) - (windowDisplayMode & 0xffff)) + 1 & 7) < 4 ^ 1) << 10;
	}
	else {
		if (((pCurrentFieldScriptActor->m84 >> 16) & 4) != 0) {
			flags = 0x400;
		}
	}
	createDialogWindow((int)boxX3, (int)boxY2, dialogIndex, windowIndex, dialogBoxWidth, dialogBoxHeight, currentFieldActorId, actorId, mode, flags, (pCurrentFieldScriptActor->m84 >> 16));
	flagWindowOpenBF(windowIndex);
	pCurrentFieldScriptActor->m104_rotation |= 0x8000;
    pCurrentFieldScriptActor->mCC_scriptPC += 4;
	return 0;
}

s32 distance3d(long param_1, long param_2, long param_3)
{
	VECTOR local_28;
    VECTOR local_18;

	local_28.vx = param_1;
	local_28.vy = param_2;
	local_28.vz = param_3;
	Square0(&local_28, &local_18);
    return SquareRoot0(local_18.vx + local_18.vy + local_18.vz);
}
