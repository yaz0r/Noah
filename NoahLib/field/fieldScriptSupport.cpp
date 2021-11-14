#include "noahLib.h"
#include "fieldScriptSupport.h"
#include "dialogWindows.h"

int currentFieldActorId;
std::vector<u8>::iterator pCurrentFieldScriptFile;
sFieldScriptEntity* pCurrentFieldScriptActor;
std::array<int, 3> partyToFieldEntityArrayMapping;

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
            playMusic(iVar2, 0);
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

int isDialogAvatarLoaded(int faceId)
{
	MissingCode();
	return 0;
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
	return 7U - (camera2Tan + -0x100 >> 9) & 7;
}

uint getPlayerCharacterDir(void)
{
	return ((actorArray[playerControlledEntity].m4C_scriptEntity)->m106_currentRotation + 0x100 >> 9) + 2U & 7;
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
			((pCurrentFieldScriptActor->m80_DialogAvatarFace != -1 && (iVar6 = isDialogAvatarLoaded((uint)pCurrentFieldScriptActor->m80_DialogAvatarFace), iVar6 == -1)))))) {
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

    pCurrentFieldScriptActor->m84 &= 0xFFFF; // clear upper part
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] != 0) {
        u32 newUpperPart = (pCurrentFieldScriptActor->m84 & 0xFF00) | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3];
        pCurrentFieldScriptActor->m84 |= (newUpperPart << 16);
	}

    int windowDisplayMode = (pCurrentFieldScriptActor->m84 >> (16 + 4)) & 3;

    MissingCode(); // some weird handling of camera direction (select top or bottom of the screen depending where the character is?)

    gDialogWindows[windowIndex].m40C_flags = 1;

    int boxY;
    int projectedPosition[2];

	if ((mode == 0) || (mode == 3)) {
        projectActorToScreen(actorId, &projectedPosition[0], &projectedPosition[1], -0x40);
        boxY = 0x14;
		if (mode == 0) {
            boxY = projectedPosition[1] + dialogBoxHeight * -0xe + -0x24;
		}
		else {
            projectedPosition[0] = 0xa0;
		}
		if ((pCurrentFieldScriptActor->m80_DialogAvatarFace != -1) && (((pCurrentFieldScriptActor->m84>>16) & 2) == 0)) {
			dialogBoxHeight = 4;
			if ((int)dialogBoxWidth < 0x18) {
				dialogBoxWidth = 0x18;
			}
			dialogBoxWidth = dialogBoxWidth + 0x11;
            boxY = 0x10;
		}
	}

    int boxX = projectedPosition[0] + -8 + dialogBoxWidth * -2;
    int boxX2 = boxX + 0x10;
	if (boxX < 0xc) {
        boxX = 0xc;
        boxX2 = 0x1c;
	}
	if (0x134 < (int)(boxX2 + dialogBoxWidth * 4)) {
		boxX = (short)dialogBoxWidth * -4 + 0x124;
	}
    int boxY2 = boxY + 8;
	if (boxY < 0x10) {
        boxY = 0x10;
        boxY2 = 0x18;
	}
	if (0xd4 < (int)(boxY2 + dialogBoxHeight * 0xe)) {
        boxY2 = (short)dialogBoxHeight * -0xe + 0xcc;
	}
	if ((mode == 0) || (mode == 3)) {
		if (pCurrentFieldScriptActor->m88[0] != 0) {
			boxX = pCurrentFieldScriptActor->m88[0];
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
		if ((pCurrentFieldScriptActor->m80_DialogAvatarFace != -1) && (((pCurrentFieldScriptActor->m84 >> 16) & 2) == 0)) {
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
	createDialogWindow((int)boxX, (int)boxY, dialogIndex, windowIndex, dialogBoxWidth, dialogBoxHeight, currentFieldActorId, actorId, mode, flags, (pCurrentFieldScriptActor->m84 >> 16));
	flagWindowOpenBF(windowIndex);
	pCurrentFieldScriptActor->m104_rotation |= 0x8000;
    pCurrentFieldScriptActor->mCC_scriptPC += 4;
	return 0;
}

s32 distance3d(long param_1, long param_2, long param_3)
{
	sVec3 local_28;
    sVec3 local_18;

	local_28.vx = param_1;
	local_28.vy = param_2;
	local_28.vz = param_3;
	Square0(&local_28, &local_18);
    return SquareRoot0(local_18.vx + local_18.vy + local_18.vz);
}
