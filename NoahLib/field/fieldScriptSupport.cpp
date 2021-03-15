#include "noahLib.h"
#include "fieldScriptSupport.h"

int currentFieldActorId;
std::vector<u8>::iterator pCurrentFieldScriptFile;
sFieldScriptEntity* pCurrentFieldScriptActor;
std::array<int, 3> partyToFieldEntityArrayMapping;

s32 op99Var1 = 0;
s32 sceneSCRZ = 0;
s32 op99Var5 = 0;
s32 op99Var6 = 0;
s32 op99Var7 = 0;
s16 cameraInterpolationMode = 0;
s16 op99Var2 = 0;
s16 op99Var3 = 0;
s16 op99Var4 = 0;
s16 sceneScale = 0;
s16 sceneDIP = 0;
s16 op99VarB = 0;

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
            fieldChangePrevented2 = -1;
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
        if ((musicVar2 == 1) || (fieldChangePrevented2 == -1)) {
            breakCurrentScript = 1;
            return;
        }
        if (iVar2 != currentlyPlayingMusic) {
            clearMusic();
            fieldChangePrevented2 = -1;
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
