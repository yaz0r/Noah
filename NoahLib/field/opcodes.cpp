#include "noahLib.h"
#include "opcodes.h"
#include "fieldScriptSupport.h"
#include "kernel/trigo.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "kernel/decompress.h"
#include "opcodeTables.h"
#include "fieldGraphicObject.h"
#include "sprite/spriteSetup.h"
#include "dialogWindows.h"
#include "screenDistortion.h"
#include "field.h"
#include "kernel/inputs.h"
#include "mecha/mechaOverlay.h"
#include "menus/menuHandler.h"
#include "field/particles/particles.h"
#include "battle/battle.h"
#include "field/fieldInputs.h"
#include "kernel/kernelVariables.h"
#include "validation/gdbConnection.h"
#include "walkMesh.h"

#include "kernel/audio/soundInstance.h"

s32 particleCreationSlot = 0;

void OP_JUMP_IF()
{
    
    
    int iVar3;

    uint valueB = 0;
    uint valueA = 0;

    u8 testControlByte = pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5];

    switch (testControlByte & 0xf0) {
    case 0x00:
        valueA = getVariable(readU16FromScript(1));
        valueB = getVariable(readU16FromScript(3));
        if (getBytesSign(readU16FromScript(1)) != 0) {
            valueB &= 0xffff;
        }
        else {
            valueB = (s16)valueB;
        }
        break;
    case 0x40:
        valueA = getVariable(readU16FromScript(1));
        valueB = readS16FromScript(3);
        if (getBytesSign(readU16FromScript(1)) != 0) {
            valueB &= 0xffff;
        }
        else {
            valueB = (s16)valueB;
        }
        break;
    case 0x80:
        valueA = readS16FromScript(1);
        valueB = getVariable(readU16FromScript(3));
        if (getBytesSign(readU16FromScript(3))) {
            valueA &= 0xffff;
        }
        else {
            valueA = (s16)valueA;
        }
        break;
    case 0xC0:
        valueA = readS16FromScript(1);
        valueB = readS16FromScript(3);
        break;
    }

    bool testResult = false;
    switch (testControlByte & 0xf) {
    case 0:
        if (valueA == valueB) {
            testResult = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 1:
        if (valueA != valueB) {
            testResult = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 2:
        valueB = (uint)((int)valueB < (int)valueA);
        break;
    case 3:
        valueB = (uint)((int)valueA < (int)valueB);
        break;
    case 4:
        if ((int)valueB <= (int)valueA) {
            testResult = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 5:
        if ((int)valueA <= (int)valueB) {
            testResult = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 6:
        valueB = valueA & valueB;
        break;
    case 7:
        if (valueA != valueB) {
            testResult = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 8:
        valueB = valueA | valueB;
        break;
    case 9:
        valueB = valueA & valueB;
        break;
    case 10:
        valueB = ~valueA & valueB;
        break;
    default:
        goto switchD_Field_800a1d84_caseD_b;
    }
    if (valueB != 0) {
        testResult = true;
    }

switchD_Field_800a1d84_caseD_b:
    if (testResult) {
        pCurrentFieldScriptActor->mCC_scriptPC += 8;
    }
    else {
        pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(6);
    }
}

void OP_4()
{
    for (int i = 0; i < 8; i++)
    {
        if (pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m18 == 7)
        {
            pCurrentFieldScriptActor->m8C_scriptSlots[i].m0_scriptPC = getScriptEntryPoint(currentFieldActorId, 1);
        }
    }

    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18 = 0xF;
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = -1;
    breakCurrentScript = 1;
}

void OP_CALL()
{
    sFieldScriptEntity* psVar1;
    ushort uVar2;
    uint uVar3;

    if ((pCurrentFieldScriptActor->m12C_flags & 0x1c0) == 0x100) {
        if (fieldDebugDisable == 0) {
            //sprintf_screen(s_STACKERR_ACT = % d_Field__8006fd44, currentFieldActorId);
            assert(0);
        }
        breakCurrentScript = 1;
    }
    else {
        pCurrentFieldScriptActor->m78_stack[pCurrentFieldScriptActor->m12C_flags >> 5 & 0xe] = pCurrentFieldScriptActor->mCC_scriptPC + 3;
        uVar2 = readU16FromScript(1);
        psVar1 = pCurrentFieldScriptActor;
        pCurrentFieldScriptActor->mCC_scriptPC = uVar2;
        uVar3 = psVar1->m12C_flags;
        psVar1->m12C_flags = uVar3 & 0xfffffe3f | ((uVar3 >> 6 & 7) + 1 & 7) << 6;
    }
}

void OP_RETURN()
{
    uint uVar2;
    uint uVar3;

    uVar2 = pCurrentFieldScriptActor->m12C_flags;
    if ((uVar2 & 0x1c0) == 0) {
        if (fieldDebugDisable == 0) {
            //sprintf_screen(s_STACKERR_ACT = % d_Field__8006fd44, currentFieldActorId);
            assert(0);
        }
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18 = 0xF;
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = -1;
        currentScriptFinished = 1;
        breakCurrentScript = 1;
    }
    else {
        uVar3 = ((uVar2 >> 6 & 7) - 1 & 7) << 6;
        pCurrentFieldScriptActor->m12C_flags = uVar2 & 0xfffffe3f | uVar3;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->m78_stack[uVar3 >> 5];
    }
}

void OP_10Sub(int param_1)
{
    sSpriteActor* pSpriteActor = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
    s16 pField76;
    s32 valueFromFlags;

    int walkSpeed;
    if (((actorArray[currentFieldActorId].m4C_scriptEntity)->m4_flags.m_rawFlags & 0x2000) == 0) {
        walkSpeed = (0x4000000 / pCurrentFieldScriptActor->m76) >> 0x10;
    }
    else {
        walkSpeed = (0x8000000 / pCurrentFieldScriptActor->m76) >> 0x10;
    }

    if (walkSpeed == 0) {
        walkSpeed = 1;
    }

    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x10000;

    int animation = 1;

    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1])
    {
    case 0:
    {
        int X = getVar80(2, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) * 0x10000;
        int Z = getVar40(4, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) * 0x10000;
        int Y = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) * 0x10000;
        int distance = distance3d((X - (pCurrentFieldScriptActor->m20_position).vx) >> 0x10, (Y - (pCurrentFieldScriptActor->m20_position).vy) >> 0x10, (Z - (pCurrentFieldScriptActor->m20_position).vz) >> 0x10);

        pCurrentFieldScriptActor->m102_numSteps = distance / walkSpeed;
        if (pCurrentFieldScriptActor->m102_numSteps == 0) {
            pCurrentFieldScriptActor->m102_numSteps = 1;
        }

        pCurrentFieldScriptActor->mD0_targetPositionOffset[0] = (X - (pCurrentFieldScriptActor->m20_position).vx) / pCurrentFieldScriptActor->m102_numSteps;
        pCurrentFieldScriptActor->mD0_targetPositionOffset[1] = (Y - (pCurrentFieldScriptActor->m20_position).vy) / pCurrentFieldScriptActor->m102_numSteps;
        pCurrentFieldScriptActor->mD0_targetPositionOffset[2] = (Z - (pCurrentFieldScriptActor->m20_position).vz) / pCurrentFieldScriptActor->m102_numSteps;

        if ((X >> 0x10 != pCurrentFieldScriptActor->m20_position.vx.getIntegerPart()) || (Z >> 0x10 != pCurrentFieldScriptActor->m20_position.vz.getIntegerPart())) {
            pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = -ratan2(pCurrentFieldScriptActor->mD0_targetPositionOffset[2] >> 0x10, pCurrentFieldScriptActor->mD0_targetPositionOffset.vx.getIntegerPart());
        }
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 9;
        break;
    }
    case 1:
        // reached target?
        if ((pCurrentFieldScriptActor->m102_numSteps < 1) || (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0)) {
            // go back to previous opcode to retrieve target location
            pCurrentFieldScriptActor->mCC_scriptPC -= 9;
            assert(pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0] == 0x10);
            assert(pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0);
            if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 != 0) {
                VECTOR currentPosition = pCurrentFieldScriptActor->m20_position;

                (pCurrentFieldScriptActor->m20_position).vx = getVar80(2, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) << 0x10;
                (pCurrentFieldScriptActor->m20_position).vz = getVar40(4, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) << 0x10;
                (pCurrentFieldScriptActor->m20_position).vy = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]) << 0x10;

                (pCurrentFieldScriptActor->m30_stepVector).vx = (pCurrentFieldScriptActor->m20_position).vx - currentPosition.vx;
                (pCurrentFieldScriptActor->m30_stepVector).vy = (pCurrentFieldScriptActor->m20_position).vy - currentPosition.vy;
                (pCurrentFieldScriptActor->m30_stepVector).vz = (pCurrentFieldScriptActor->m20_position).vz - currentPosition.vz;
            }
            animation = (int)pCurrentFieldScriptActor->mE6;
            if (param_1 == 0) {
                ADVANCE_VM(0xB);
            }
            else {
                ADVANCE_VM(0xD);
            }
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = -1;

        }
        else
        {
            pCurrentFieldScriptActor->m20_position.vx += pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
            pCurrentFieldScriptActor->m20_position.vy += pCurrentFieldScriptActor->mD0_targetPositionOffset[1];
            pCurrentFieldScriptActor->m20_position.vz += pCurrentFieldScriptActor->mD0_targetPositionOffset[2];

            pCurrentFieldScriptActor->m30_stepVector.vx = pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
            pCurrentFieldScriptActor->m30_stepVector.vy = pCurrentFieldScriptActor->mD0_targetPositionOffset[1];
            pCurrentFieldScriptActor->m30_stepVector.vz = pCurrentFieldScriptActor->mD0_targetPositionOffset[2];

            breakCurrentScript = 1;
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0--;
        }

        pCurrentFieldScriptActor->m102_numSteps--;
        actorArray[currentFieldActorId].mC_matrix.t[0] = pCurrentFieldScriptActor->m20_position.vx.getIntegerPart();
        actorArray[currentFieldActorId].mC_matrix.t[1] = pCurrentFieldScriptActor->m20_position.vy.getIntegerPart();
        actorArray[currentFieldActorId].mC_matrix.t[2] = pCurrentFieldScriptActor->m20_position.vz.getIntegerPart();
        (pSpriteActor->m0_position).vx = (pCurrentFieldScriptActor->m20_position).vx;
        (pSpriteActor->m0_position).vy = (pCurrentFieldScriptActor->m20_position).vy;
        (pSpriteActor->m0_position).vz = (pCurrentFieldScriptActor->m20_position).vz;
        break;
    default:
        assert(0);
    }


    if (pCurrentFieldScriptActor->mEA_forcedAnimation != 0xff) {
        animation = (int)pCurrentFieldScriptActor->mEA_forcedAnimation;
    }
    if ((pCurrentFieldScriptActor->mE8_currentAnimationId != animation) && ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x2000000) == 0)) {
        pCurrentFieldScriptActor->mE8_currentAnimationId = (short)animation;
        setVisualAnimation(pSpriteActor, animation, pCurrentFieldEntity);
    }
    updateEntityEventCode3Sub1(pSpriteActor, pCurrentFieldScriptActor->m104_rotation, pCurrentFieldEntity);
}

void OP_10()
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = -1;
    OP_10Sub(0);
}

void OP_CUSTOM_FIELD_TRANSITION()
{
    if ((((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) || (loadCompleted != 0)) || ((fieldMusicLoadPending == -1 || (load2dAnimVar != 0)))) {
        breakCurrentScript = 1;
    }
    else {
        OP_CHANGE_FIELD_WHEN_READY2();
        OP_CHANGE_FIELD_WHEN_READY_Sub();
        fieldTransitionMode = getImmediateOrVariableUnsigned(0);
        fieldTransitionFadeInLength = getImmediateOrVariableUnsigned(2);
        ADVANCE_VM(0x4);
    }
}

void OP_DISABLE_RANDOM_BATTLES()
{
    fieldRandomBattleVar = 0;
    ADVANCE_VM(0x1);
}

void OP_ADD_TO_CURRENT_PARTY()
{
    if (((asyncLoadingVar1 == 0xff) && (loadCompleted == 0)) && (!isLoadCompleted()))
    {
        waitReadCompletion(0);

        int freePartySlot;
        int partyIsFullOrCharacterAlreadyInParty = findFreePartySlot((uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1], &freePartySlot);

        if (partyIsFullOrCharacterAlreadyInParty == 0) {
            pKernelGameState->m22B1_isOnGear[freePartySlot] = 0;

            s8 bVar1 = pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1];
            currentParty[freePartySlot] = (uint)bVar1;

            startPartyCharacterASyncLoading((uint)bVar1, freePartySlot);
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        }
        else {
            pKernelGameState->m1D30_partyMemberBitField =
                pKernelGameState->m1D30_partyMemberBitField |
                (ushort)(1 << ((uint)pCurrentFieldScriptFile
                    [(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x1f));
            ADVANCE_VM(0x4);
        }
    }
    else
    {
        breakCurrentScript = 1;
        ADVANCE_VM(-0x1);
    }
}

void  OP_REMOVE_FROM_CURRENT_PARTY()
{
    int iVar1 = asyncLoadingVar1;
    if (asyncLoadingVar1 != 0xff) {
        breakCurrentScript = 1;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
        return;
    }
    DrawSync(0);
    int uVar2 = getCharacter(pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]);
    int iVar3 = findCharacterInParty(uVar2);

    if (iVar3 == -1)
    {
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        return;
    }

    if (fieldExecuteVar1 == 0) {
        switch (iVar3)
        {
        case 0:
            if (currentParty[1] == 0xff) {
                currentParty[0] = iVar1;
                asyncPartyCharacterLoadingTable[0] = iVar1;
                pKernelGameState->m22B1_isOnGear[0] = 0;
            }
            else {
                partyCharacterBuffers[0] = partyCharacterBuffers[1];
                asyncPartyCharacterLoadingTable[0] = asyncPartyCharacterLoadingTable[1];
                currentParty[0] = currentParty[1];
                currentParty[1] = 0xff;
                asyncPartyCharacterLoadingTable[1] = 0xff;
                pKernelGameState->m22B1_isOnGear[0] = pKernelGameState->m22B1_isOnGear[1];
                if (currentParty[2] != 0xff) {
                    partyCharacterBuffers[1] = partyCharacterBuffers[2];
                    asyncPartyCharacterLoadingTable[1] = asyncPartyCharacterLoadingTable[2];
                    currentParty[1] = currentParty[2];
                    currentParty[2] = 0xff;
                    asyncPartyCharacterLoadingTable[2] = 0xff;
                    pKernelGameState->m22B1_isOnGear[1] = pKernelGameState->m22B1_isOnGear[2];
                }
            }
            break;
        case 1:
            if (currentParty[2] == 0xff) {
                currentParty[1] = iVar1;
                asyncPartyCharacterLoadingTable[1] = iVar1;
                pKernelGameState->m22B1_isOnGear[1] = 0;
            }
            else {
                partyCharacterBuffers[1] = partyCharacterBuffers[2];
                asyncPartyCharacterLoadingTable[1] = asyncPartyCharacterLoadingTable[2];
                currentParty[1] = currentParty[2];
                currentParty[2] = 0xff;
                asyncPartyCharacterLoadingTable[2] = 0xff;
                pKernelGameState->m22B1_isOnGear[1] = pKernelGameState->m22B1_isOnGear[2];
                pKernelGameState->m22B1_isOnGear[2] = 0;
            }
            break;
        case 2:
            currentParty[2] = iVar1;
            asyncPartyCharacterLoadingTable[2] = iVar1;
            pKernelGameState->m22B1_isOnGear[2] = 0;
        }
    }
    else
    {
        switch (iVar3)
        {
        case 0:
            pKernelGameState->m22B1_isOnGear[0] = pKernelGameState->m22B1_isOnGear[1];
            pKernelGameState->m22B1_isOnGear[1] = pKernelGameState->m22B1_isOnGear[2];
            pKernelGameState->m22B1_isOnGear[2] = 0;
            emptyPartySlot(0);
            copyPartySlotFromNext(0);
            copyPartySlotFromNext(1);
            break;
        case 1:
            pKernelGameState->m22B1_isOnGear[1] = pKernelGameState->m22B1_isOnGear[2];
            pKernelGameState->m22B1_isOnGear[2] = 0;
            emptyPartySlot(1);
            copyPartySlotFromNext(1);
            break;
        case 2:
            pKernelGameState->m22B1_isOnGear[2] = 0;
            emptyPartySlot(2);
            break;
        default:
            assert(0);
        }

        if ((currentParty[0] == 0xff) || (partyToFieldEntityArrayMapping[0] == 0xff)) {
            g_playerControlledActor = 0;
        }
        else {
            g_playerControlledActor = partyToFieldEntityArrayMapping[0];
            (actorArray[partyToFieldEntityArrayMapping[0]].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags &= ~0x80;
            (actorArray[partyToFieldEntityArrayMapping[0]].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags |= 0x0400;
            (actorArray[partyToFieldEntityArrayMapping[0]].m4C_scriptEntity)->m0_fieldScriptFlags.m14_isPlayerControlled = 1;
        }
    }
    ADVANCE_VM(0x2);
}

void OP_FINALIZE_PARTY_CHARACTER_LOADING()
{
    if ((asyncLoadingVar1 == 0xff) || isCDBusy()) {
        breakCurrentScript = 1;
        ADVANCE_VM(-0x1);
    }
    else {
        waitReadCompletion(0);
        decompress(asyncPartyCharacterLoadingBuffer.getRawData().begin(), partyCharacterBuffersRaw[asyncPartyCharacterLoadingIndex]);
        asyncPartyCharacterLoadingBuffer.clear();
        partyCharacterBuffers[asyncPartyCharacterLoadingIndex].init(partyCharacterBuffersRaw[asyncPartyCharacterLoadingIndex].beginSpan());
        runInitScriptForNewlyLoadedPC(asyncPartyCharacterLoadingCharacterIndex);
        asyncLoadingVar1 = 0xff;
        breakCurrentScript = 1;
        ADVANCE_VM(0x1);
    }
}

void OP_ENABLE_VM_FOR_CHARACTER(void)
{
    int iVar1;

    iVar1 = readCharacter(1);
    if (iVar1 != 0xff) {
        iVar1 = readCharacter(1);
        actorArray[iVar1].m4C_scriptEntity->m0_fieldScriptFlags.m0_updateScriptDisabled = 0;
    }
    ADVANCE_VM(2);
}

void OP_DELETE_ENTITY()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    int local_10;

    iVar1 = readCharacter(1);
    if (iVar1 != 0xff) {
        iVar1 = readCharacter(1);
        psVar2 = actorArray[iVar1].m4C_scriptEntity;
        psVar2->m0_fieldScriptFlags.m0_updateScriptDisabled = 1;
        psVar2->m4_flags.m_rawFlags = psVar2->m4_flags.m_rawFlags | 0x100000;
        iVar1 = readCharacter(1);
        actorArray[iVar1].m58_flags = actorArray[iVar1].m58_flags | 0x20;
        iVar1 = findDialogWindowForCurrentActor(&local_10);
        if (iVar1 == 0) {
            gDialogWindows[local_10].m414 = 0;
        }
    }
    ADVANCE_VM(0x2);
}

void DISABLE_DIALOG_ACTIVATION(void)
{
    sFieldScriptEntity* psVar2;
    psVar2 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x20000;
    ADVANCE_VM(0x1);
    return;
}

void OP_PLAY_ANIMATION()
{
    ushort* puVar1;
    byte bVar2;

    pCurrentFieldScriptActor = pCurrentFieldScriptActor;
    puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
    pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xfeffffff;
    bVar2 = pCurrentFieldScriptFile[*puVar1 + 1];
    ADVANCE_VM(0x2);
    pCurrentFieldScriptActor->mEA_forcedAnimation = (ushort)bVar2;
}

void OP_GET_ACTOR_POSITION(void)
{
    ushort uVar1;
    int iVar2;

    iVar2 = readCharacter(1);
    if (iVar2 != 0xff) {
        uVar1 = readU16FromScript(2);
        setVar((uint)uVar1, actorArray[iVar2].m2C_matrixBackup.t[0]);
        uVar1 = readU16FromScript(4);
        setVar((uint)uVar1, actorArray[iVar2].m2C_matrixBackup.t[2]);
        uVar1 = readU16FromScript(6);
        setVar((uint)uVar1, actorArray[iVar2].m2C_matrixBackup.t[1]);
    }
    ADVANCE_VM(0x8);
    return;
}

void OP_GET_ACTOR_FACING_ANGLE()
{
    setVar(readU16FromScript(1), ((pCurrentFieldScriptActor->m106_currentRotation + 0x100) >> 9) + 2U & 7);
    ADVANCE_VM(0x3);
}

void OP_STOP(void)
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18 = 0xF;
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = -1;
    currentScriptFinished = 1;
    breakCurrentScript = 1;
    return;
}

void OPX_15()
{
    actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
    uint uVar4 = getImmediateOrVariableUnsigned(1);
    int uVar5 = getImmediateOrVariableUnsigned(3);
    resetFieldScriptEntityGraphicEntity(currentFieldActorId, uVar4, &fieldActorSetupParams[uVar4], 0, uVar5, uVar4 | 0x80, 1);
    OP_INIT_ENTITY_SCRIPT_sub1();
    ADVANCE_VM(0x5);
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x80;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x100;
    pCurrentFieldScriptActor->m4_flags.m_x800 = 0;
    actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
}

void OPX_ROTATE_CHARACTER_TO_CHARACTER(void)
{
    ushort uVar1;
    uint uVar2;
    long lVar3;
    sFieldScriptEntity* psVar4;
    sFieldScriptEntity* psVar5;

    uVar2 = readCharacter(1);
    if ((uVar2 != 0xff) && (uVar2 = readCharacter(2), uVar2 != 0xff)) {
        uVar2 = readCharacter(2);
        psVar4 = actorArray[uVar2].m4C_scriptEntity;
        uVar2 = readCharacter(1);
        psVar5 = actorArray[uVar2].m4C_scriptEntity;
        lVar3 = ratan2((psVar4->m20_position).vz - (psVar5->m20_position).vz, (psVar4->m20_position).vx - (psVar5->m20_position).vx);
        uVar1 = -(short)lVar3 | 0x8000;
        if (fieldExecuteVar1 == 0) {
            psVar5->m104_rotation = uVar1;
            psVar5->m106_currentRotation = uVar1;
            psVar5->m108_rotation3 = uVar1;
        }
        psVar5->m104_rotation = uVar1;
        psVar5->m106_currentRotation = uVar1;
    }
    ADVANCE_VM(3);
    return;
}

void OP_INIT_ENTITY_NPC(void)
{
    actorArray[currentFieldActorId].m58_flags &= ~0xF80;
    actorArray[currentFieldActorId].m58_flags |= 0x200;

    u32 index = getImmediateOrVariableUnsigned(1);
    resetFieldScriptEntityGraphicEntity(currentFieldActorId, index, &fieldActorSetupParams[index], 0, 0, index | 0x80, 0);
    OP_INIT_ENTITY_SCRIPT_sub1();
    ADVANCE_VM(0x3);
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x80;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x100;
    pCurrentFieldScriptActor->m4_flags.m_x800 = 0;
    actorArray[currentFieldActorId].m58_flags &= ~0x20;
}

s16 updateCharacterVar2 = 0;
s16 updateCharacterVar2ResetValue = 0;
s8 updateCharacterVar4 = 0;
int numFollowStruct2 = 0;
extern s32 menuIdToOpen;
extern s16 updateEntityEventCode4Var0;

std::array<u16, 16> updateCharacterRotationOrder1 = {
    0x8000,   0x400,   0x800,   0x600,
     0xC00,  0x8000,   0xA00,   0x800,
       0x0,   0x200,  0x8000,   0x400,
     0xE00,     0x0,   0xC00,  0x8000,
};

std::array<u16, 16> updateCharacterRotationOrder2 = {
    0x8000,   0xC00,     0x0,   0xE00,
     0x400,  0x8000,   0x200,     0x0,
     0x800,   0xA00,  0x8000,   0xC00,
     0x600,   0x800,   0x400,  0x8000,
};

int OP_UPDATE_CHARACTER_Sub0(sFieldScriptEntity* param_1)
{
    return -(int)((param_1->m0_fieldScriptFlags.m_rawFlags >> 9 & 3 & (uint)param_1->m14_currentTriangleFlag >> 3) != 0);
}

// Initialize `encounterCount` timers
// Every timers is an unique value in range [0, encouterTimer + 1] 
void setInitialEncounterData()
{
    encounterDataCountdown = encounterTimer;
    if (encounterCount == 0) {
        encounterTimer = 0;
        return;
    }

    for (int i = 0; i < 32; i++)
    {
        encounterTriggerTime[i] = -1;
    }

    if (encounterCount > 0) {
        // Ensure that encounterTickData contains unique random numbers in range [0, encouterTimer + 1]
        for (int encounterId = 0; encounterId < encounterCount; encounterId++)
        {
        LAB_Field__8008e798:

            int s1 = 0;
            int r = xenoRand() * (encounterTimer + 1);
            int r2 = (r >> 0xf) & 0xffff;

            for (int i = 0; i < 32; i++)
            {
                if (encounterTriggerTime[i] == r2) goto LAB_Field__8008e798;
            }
            encounterTriggerTime[s1] = r2;
            s1++;
        }
    }

    if (encounterCount > 0) {
        for (int encounterId = 0; encounterId < encounterCount; encounterId++)
        {
            encounterTriggerTime[encounterId]++;
        }
    }
}

void checkForRandomEncounter()
{
    // BEGIN: Extra debug

    if (debugForcedEncounter != -1) {        
        //SHORT_Field__800b2290 = SHORT_ARRAY_Field__800b2270[iVar6];
        //battleInitVar2 = debugForcedEncounter;
        //battleInitVar1 = 0;
        //if (mechaOverlayLoadingMode == 0) {
        //    readGameOverlay(2);
        // }
        playMusicAuthorized = 0;
        bBattleSequenceInitialized = true;
        if (fieldDebugDisable != 0) {
            playMusicAuthorized = 0;
            return;
        }
        debugForcedEncounter = -1;
    }

    if (debugEncounterTriggerDisabled) {
        // Supressed battle start from the debug menu.
        return;
    }

    // END: Extra debug

    if (((((playMusicAuthorized != 0) && (fieldExecuteVar3 != 0)) && (fieldChangePrevented != 0)) &&
        ((fieldMusicLoadPending != -1 && (encounterTimer != 0)))) &&
        ((fieldRandomBattleVar != -1 && ((loadCompleted != 1 && (/*fieldTransitionCompleted != 0*/ -1 != 0))))))
    {
        encounterDataCountdown--;
        if (encounterDataCountdown == 0) {
            setInitialEncounterData();
        }

        // Decrease every encounter counter by -1 unless already -1
        if (encounterCount > 0) {
            for (int s0 = 0; s0 < encounterCount; s0++) {
                if (encounterTriggerTime[s0] != -1) {
                    encounterTriggerTime[s0]--;
                }
            }
        }
        
        // Exit unless we have at least one encounter entry
        if (encounterCount <= 0) {
            return;
        }

        // Find last entry and set it to -1
        int encounterId = 0;
        while (encounterTriggerTime[encounterId] != 0) {
            encounterId++;            
            if (encounterId == encounterCount) {
                // Exit if no entries have any data
                return;
            }
        }
        encounterTriggerTime[encounterId] = -1;

        // Select random encounter using weighted encounter chance:
        //   Get total sum of encounter variables
        //   Find a random value between 0 and total sum
        //   Build a sequence where every element N is sum of previous ones
        //   For every element from end if sum value is less than rand, set encounter start flag
        //   If flag is set - break and activate battle

        int iTotalEncounterSum = 0;
        for (int v1 = 0; v1 < 16; v1++) {
            iTotalEncounterSum += encounterProbabilityWeight[v1];
        }

        std::array<int, 16> encounterProbabilitySum;
        int iPreviousSum = 0;
        for (int v1 = 0; v1 < 16; v1++) {
            encounterProbabilitySum[v1] = iPreviousSum;
            iPreviousSum += encounterProbabilityWeight[v1];
        }

        int iVar5 = (xenoRand() * (iTotalEncounterSum + 1)) >> 0xf;
        bool bShouldTriggerEncounter = false;
        int selectedEncounter = 0;

        for (int v1 = 15; v1 >= 0; v1--) {
            if (encounterProbabilityWeight[v1] != 0 && encounterProbabilitySum[v1] < iVar5) {
                bShouldTriggerEncounter = true;
                selectedEncounter = v1;
                break; 
            }
        }

        if (bShouldTriggerEncounter) {
            //SHORT_Field__800b2290 = SHORT_ARRAY_Field__800b2270[iVar6];
            //battleInitVar2 = selectedEncounter;
            //battleInitVar1 = 0;
            //if (mechaOverlayLoadingMode == 0) {
            //    readGameOverlay(2);
            // }
            playMusicAuthorized = 0;
            bBattleSequenceInitialized = true;
            if (fieldDebugDisable != 0) {
                playMusicAuthorized = 0;
                return;
            }
            //recordEncounterDebugData(selectedEncounter);
        }
    }
}


void OP_UPDATE_CHARACTER()
{
    if (!pCurrentFieldScriptActor->m0_fieldScriptFlags.m14_isPlayerControlled) {
        if (updateCharacterVar0 == 0) {
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x1000000;
        }
        ADVANCE_VM(0x1);
        return;
    }

    int windowIndex = 0;
    for (windowIndex = 0; windowIndex < 4; windowIndex++)
    {
        if (gDialogWindows[windowIndex].m37C == 0)
            break;
    }
    if ((windowIndex != 4) || (fieldRandomBattleVar != 0))
    {
        pCurrentFieldScriptActor->m104_rotation = -0x8000;
        ADVANCE_VM(0x1);
        return;
    }

    // this is a shortcut to test if any dpad buttons are pressed
    if ((padButtonForScripts[0].m0_buttons >> 12) != 0)
    {
        checkForRandomEncounter();
    }

    playerCanRun = 1;
    if ((pCurrentFieldScriptActor->m14_currentTriangleFlag & 0x400000U) == 0) {
        updateCharacterVar1 = 0;
    }
    else {
        if (((pCurrentFieldScriptActor->m68_oldPosition[0] == (pCurrentFieldScriptActor->m20_position.vx.getIntegerPart())) &&
            (pCurrentFieldScriptActor->m68_oldPosition[1] == (pCurrentFieldScriptActor->m20_position.vy.getIntegerPart()))) &&
            (pCurrentFieldScriptActor->m68_oldPosition[2] == (pCurrentFieldScriptActor->m20_position.vz.getIntegerPart()))) {
            updateCharacterVar1 = updateCharacterVar1 + 1;
        }
    }

    // handle jump
    if (((updateCharacterVar1 < 0x21) || (updateCharacterVar1 = 0x20, (padButtonForScripts[0].m0_buttons & controllerButtons::JUMP) == 0)) || (((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x1800) != 0 || (menuIdToOpen != 0xff)))) {
        if (updateEntityEventCode4Var0 == 0) {
            if (padButtonForDialogs & controllerButtons::JUMP) {
                if ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x1800) == 0) {
                    if ((pCurrentFieldScriptActor->m14_currentTriangleFlag & 0x400000U) == 0) {
                        if (menuIdToOpen == 0xff) {
                            if (!OP_UPDATE_CHARACTER_Sub0(pCurrentFieldScriptActor)) {
                                pCurrentFieldScriptActor->m0_fieldScriptFlags.mx800_isJumping = 1;
                                numFollowStruct2 = partyToFollowStructMapping[0];
                            }
                        }
                    }
                }
            }
        }
        else {
            if ((padButtonForDialogs & controllerButtons::JUMP) == 0) {
                if (updateCharacterVar2 != 0) {
                    updateCharacterVar2--;
                }
            }
            else {
                if (updateCharacterVar2 == 0) {
                    if ((menuIdToOpen == 0xff) && !OP_UPDATE_CHARACTER_Sub0(pCurrentFieldScriptActor)) {
                        pCurrentFieldScriptActor->m0_fieldScriptFlags.mx800_isJumping = 1;
                        numFollowStruct2 = partyToFollowStructMapping[0];
                        pCurrentFieldScriptActor->mE8_currentAnimationId = 0xff;
                        updateCharacterVar2 = updateCharacterVar2ResetValue;
                    }
                    if (updateCharacterVar2 != 0) {
                        updateCharacterVar2--;
                    }
                }
            }
        }
    }
    else {
        if (!OP_UPDATE_CHARACTER_Sub0(pCurrentFieldScriptActor)) {
            pCurrentFieldScriptActor->m0_fieldScriptFlags.mx800_isJumping = 1;
            numFollowStruct2 = partyToFollowStructMapping[0];
        }
    }

    // Update direction based on dpad
    int uVar4;
    if (updateCharacterVar4 == '\0') {
        uVar4 = updateCharacterRotationOrder1[(ushort)padButtonForScripts[0].m0_buttons >> 0xc ^ 0xf];
    }
    else {
        uVar4 = updateCharacterRotationOrder2[(ushort)padButtonForScripts[0].m0_buttons >> 0xc ^ 0xf];
    }
    if ((uVar4 & 0x8000) == 0) {
        uVar4 = uVar4 - camera2Tan & 0xfff;
    }
    pCurrentFieldScriptActor->m104_rotation = uVar4;
    ADVANCE_VM(0x1);
}

void OP_UPDATE_CHARACTER_INFINITLY()
{
    ushort uVar1;

    uVar1 = pCurrentFieldScriptActor->mCC_scriptPC;
    OP_UPDATE_CHARACTER();
    breakCurrentScript = 1;
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

void OP_SET_ACTOR_POSITION_2D(void)
{
    int posX = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    int posZ = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    setCurrentActor2DPosition(posX, posZ);
    pCurrentFieldScriptActor->m4_flags.m_rawFlags &= ~0x200000;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x10000;
    ADVANCE_VM(0x6);
}

void OP_SET_ENTITY_WALKMESH_LAYER()
{
    pCurrentFieldScriptActor->m10_walkmeshId = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    setCurrentActor2DPosition((int)(actorArray[currentFieldActorId].m4C_scriptEntity)->m20_position.vx.getIntegerPart(), (int)(actorArray[currentFieldActorId].m4C_scriptEntity)->m20_position.vz.getIntegerPart());
    ADVANCE_VM(0x2);
}

void OP_1B()
{
    MissingCode();
    ADVANCE_VM(0x7);
}

void OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION(void)
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;

    psVar2 = pCurrentFieldScriptActor;
    iVar1 = fieldExecuteVar1;
    uVar3 = actorDirectionTable3[pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1]] | 0x8000;
    pCurrentFieldScriptActor->m104_rotation = uVar3;
    psVar2->m106_currentRotation = uVar3;
    if (iVar1 == 0) {
        psVar2->m108_rotation3 = uVar3;
    }
    ADVANCE_VM(0x2);
}

int readS16FromScriptFile(int param_1)
{
    return (int)(((uint)pCurrentFieldScriptFile[param_1] + (uint)(pCurrentFieldScriptFile + param_1)[1] * 0x100) * 0x10000) >> 0x10;
}

void setupActorToFieldEntryID(int param_1)
{
    if (*pCurrentFieldScriptFile == 0xff) {
        param_1 = param_1 * 7;
        pCurrentFieldScriptActor->m10_walkmeshId = (ushort)pCurrentFieldScriptFile[param_1 + 5];
        setCurrentActor2DPosition(readS16FromScriptFile(param_1 + 1), readS16FromScriptFile(param_1 + 3));
        int uVar5 = (uint)pCurrentFieldScriptFile[param_1 + 6];
        if (uVar5 == 0xff) {
            uVar5 = getVariable(8);
        }
        uVar5 = uVar5 + 4 & 7;
        opA0_var1 = uVar5 << 0x19;
        opA0_var0 = opA0_var1 >> 0x10;
        cameraRotationBetweenEyeAndAt.vy = (s16)(uVar5 << 9);
        uVar5 = (uint)pCurrentFieldScriptFile[param_1 + 7];
        if (uVar5 == 0xff) {
            uVar5 = getVariable(6);
        }
        sFieldScriptEntity* psVar1 = pCurrentFieldScriptActor;
        uint uVar4 = (ushort)((uVar5 - 2 & 7) << 9) | 0x8000;
        pCurrentFieldScriptActor->m104_rotation = uVar4;
        psVar1->m106_currentRotation = uVar4;
        psVar1->m108_rotation3 = uVar4;
    }
}

void OP_INIT_ENTITY_PC(void)
{
    sGameState* psVar1;
    int iVar2;
    int local_v0_60;
    int iVar3;
    int iVar4;
    int local_v0_684;
    uint uVar5;
    sSpriteActor* psVar6;
    sFieldScriptEntity* psVar7;
    sFieldEntity* psVar8;

    psVar8 = &actorArray[currentFieldActorId];
    local_v0_60 = getImmediateOrVariableUnsigned(1);
    iVar3 = getCharacter(local_v0_60);
    iVar4 = findCharacterInParty(iVar3);
    psVar7 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->mE4_playableCharacterId = (short)iVar3;
    psVar8->m58_flags = psVar8->m58_flags & 0xf07f | 0x200;
    if (iVar4 == -1) {
        resetFieldScriptEntityGraphicEntity(currentFieldActorId, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
        psVar7 = pCurrentFieldScriptActor;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m0_updateScriptDisabled = 1;
        currentScriptFinished = 1;
        breakCurrentScript = 1;
        uVar5 = psVar7->m4_flags.m_rawFlags | 0x100000;
    }
    else {
        if (iVar4 == 0) {
            g_playerControlledActor = currentFieldActorId;
            actorCameraTracked = currentFieldActorId;
            psVar7->m0_fieldScriptFlags.m_rawFlags &= ~0x80;
            psVar7->m0_fieldScriptFlags.m_rawFlags |= 0x0400;
            psVar7->m0_fieldScriptFlags.m14_isPlayerControlled = 1;
        }
        iVar2 = currentFieldActorId;
        partyToFieldEntityArrayMapping[iVar4] = currentFieldActorId;
        if (pcInitVar1 == 0) {
            resetFieldScriptEntityGraphicEntity(iVar2, iVar4, &partyCharacterBuffers[iVar4], 1, 0, iVar4, 1);
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x300;
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x400;
        }
        else {
            resetFieldScriptEntityGraphicEntity
            (iVar2, (uint)characterMappingTable[iVar3] + pcInitVar1, &fieldActorSetupParams[pcInitVar1 + characterMappingTable[iVar3]], 0, 0,
                (uint)characterMappingTable[iVar3] + pcInitVar1 | 0x80, 1);
            psVar7 = pCurrentFieldScriptActor;
            psVar1 = pKernelGameState;
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x300;
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x400;
            if (psVar1->m22B1_isOnGear[iVar4] != 0) {
                psVar6 = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
                actorArray[currentFieldActorId].m4_pVramSpriteSheet = actorArray[unkPartyTable[iVar4]].m4_pVramSpriteSheet;
                actorArray[unkPartyTable[iVar4]].m4_pVramSpriteSheet = psVar6;
                psVar7->m0_fieldScriptFlags.m_rawFlags &= ~0x500;
                psVar7->m0_fieldScriptFlags.m_rawFlags |= 0x200;
            }
        }
        pcInitVar2 = -0xc0;
        actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
        setupActorToFieldEntryID(getVariable(2)); // set the player position based on the field param
        OP_INIT_ENTITY_SCRIPT_sub1();
        uVar5 = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xfffff7ff;
        psVar7 = pCurrentFieldScriptActor;
    }
    psVar7->m4_flags.m_rawFlags = uVar5;
    psVar7 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x20000;
    psVar7->m4_flags.m_rawFlags = psVar7->m4_flags.m_rawFlags | 0x400;
    ADVANCE_VM(0x3);
    return;
}

void OP_SET_BOUNDING_VOLUME()
{
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] != 0) {
        pCurrentFieldScriptActor->m18_boundingVolume.vx = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] << 1;
    }
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] != 0) {
        pCurrentFieldScriptActor->m18_boundingVolume.vz = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] << 1;
    }
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] != 0) {
        pCurrentFieldScriptActor->m18_boundingVolume.vy = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] << 1;
    }
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 4] != 0) {
        pCurrentFieldScriptActor->m1E_collisionRadius = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 4] << 1;
    }
    ADVANCE_VM(0x5);
}

void OP_SET_DIALOG_PORTRAIT(void)
{
    pCurrentFieldScriptActor->m80_dialogPortrait = getCharacter(getImmediateOrVariableUnsigned(1));
    ADVANCE_VM(0x3);
}

void OP_SET_CURRENT_ACTOR_SCALE()
{
    int iVar3 = getImmediateOrVariableUnsigned(1);
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m2C_scale = (short)((uint)(iVar3 * 3) >> 2);
    s16 sVar2 = (short)iVar3;
    pCurrentFieldScriptActor->mF4_scale3d[0] = sVar2;
    pCurrentFieldScriptActor->mF4_scale3d[1] = sVar2;
    pCurrentFieldScriptActor->mF4_scale3d[2] = sVar2;
    updateScriptActor3dRotation(currentFieldActorId);
    ADVANCE_VM(0x3);
}

void OPX_04()
{
    int iVar2;

    iVar2 = getImmediateOrVariableUnsigned(1);
    (actorArray[currentFieldActorId].m4_pVramSpriteSheet)->m82 = (short)(iVar2 << 1);
    ADVANCE_VM(0x3);
}

void OP_RAND_ROTATION()
{
    ushort* puVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;
    uint uVar4;
    ushort uVar5;

    uVar5 = pCurrentFieldScriptActor->m106_currentRotation;
    pCurrentFieldScriptActor->m102_numSteps++;
    if ((pCurrentFieldScriptActor->m102_numSteps & 0xf) == 0) {
        uVar4 = xenoRand();
        if ((uVar4 & 1) == 0) {
            uVar5 = pCurrentFieldScriptActor->m106_currentRotation + 0x200;
        }
        else {
            uVar5 = pCurrentFieldScriptActor->m106_currentRotation - 0x200;
        }
        uVar5 = uVar5 & 0xfff;
    }

    breakCurrentScript = 1;
    pCurrentFieldScriptActor->m104_rotation = uVar5;
    ADVANCE_VM(0x1);
}

uint getCharacterToEntity(int param_1)
{
    uint uVar1;

    uVar1 = readCharacter(param_1);
    if (uVar1 == 0xff) {
        uVar1 = partyToFieldEntityArrayMapping[0];
    }
    return uVar1;
}


void projectCharacterToScreen(int* param_1, int* param_2)
{
    int iVar1;
    SVECTOR local_48;
    MATRIX MStack64;
    sVec2_s16 local_20;
    long lStack28;
    long alStack24[2];

    iVar1 = getCharacterToEntity(1);
    CompMatrix(&g_currentProjectionMatrix, (MATRIX*)&actorArray[iVar1].m2C_matrixBackup, &MStack64);
    local_48.vx = 0;
    local_48.vy = 0;
    local_48.vz = 0;
    SetRotMatrix(&MStack64);
    SetTransMatrix(&MStack64);
    RotTransPers(&local_48, &local_20, &lStack28, alStack24);
    *param_2 = (int)local_20.vy;
    *param_1 = (int)local_20.vx;
}

void OP_IF_CHARACTER_IN_VIEW() {
    s32 x, y;

    projectCharacterToScreen(&x, &y);
    if (g_updateAllEntitiesSub2Var0 == 0) {
        if ((y - 0x21U < 0x9f) && (x - 0x21U < 0xff)) {
            ADVANCE_VM(0x4);
        }
        else {
            pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);
        }
        breakCurrentScript = 1;
    }
    else {
        ADVANCE_VM(0x4);
    }
}

void OP_F3()
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    ushort uVar7;
    int iVar8;
    long lVar9;
    long lVar10;

    iVar6 = desiredCameraPosition[2];
    iVar5 = desiredCameraPosition[1];
    iVar4 = desiredCameraPosition[0];
    iVar3 = desiredCameraTarget[2];
    iVar2 = desiredCameraTarget[1];
    iVar1 = desiredCameraTarget[0];
    iVar8 = distance3d((desiredCameraPosition[0] - desiredCameraTarget[0]) >> 0x10, (desiredCameraPosition[1] - desiredCameraTarget[1]) >> 0x10, (desiredCameraPosition[2] - desiredCameraTarget[2]) >> 0x10);
    op99Var4 = 0x1000;
    lVar9 = ratan2(iVar3 - iVar6, iVar1 - iVar4);
    lVar10 = length2d((iVar4 - iVar1) >> 0x10, (iVar6 - iVar3) >> 0x10);
    lVar10 = ratan2(lVar10, (iVar2 - iVar5) >> 0x10);
    uVar7 = readU16FromScript(1);
    setVar((uint)uVar7, 0xfc00U - (short)lVar9 & 0xfff);
    uVar7 = readU16FromScript(3);
    setVar((uint)uVar7, (short)(lVar10 * -0x168 >> 0xc) + 0x5b);
    uVar7 = readU16FromScript(5);
    setVar((uint)uVar7, (short)(iVar8 / 2));
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x7);
}

void OP_CLOSE_CURRENT_ACTOR_DIALOG()
{
    ushort* puVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;
    int iVar4;
    sFieldScriptEntity* psVar5;
    int local_10[2];

    psVar5 = pCurrentFieldScriptActor;
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0) {
        iVar4 = findDialogWindowForCurrentActor(local_10);
        psVar5 = pCurrentFieldScriptActor;
        if (iVar4 == 0) {
            gDialogWindows[local_10[0]].m414 = 0;
            uVar3 = psVar5->mCC_scriptPC + 2;
        }
        else {
            uVar3 = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        }
    }
    else {
        pCurrentFieldScriptActor->m82_dialogWindowSize[0] = 0;
        psVar2 = pCurrentFieldScriptActor;
        psVar5->m88_dialogWindowPosition[0] = 0;
        psVar5->m88_dialogWindowPosition[1] = 0;
        psVar2->m82_dialogWindowSize[1] = 0;
        psVar5 = pCurrentFieldScriptActor;
        puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
        pCurrentFieldScriptActor->m84_dialogWindowFlags = 0;
        uVar3 = *puVar1 + 2;
    }
    psVar5->mCC_scriptPC = uVar3;
    breakCurrentScript = 1;
}

void OP_SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE3()
{
    showDialogWindowForActor(currentFieldActorId, 3);
}

void OP_F6()
{
    ushort* puVar1;
    byte bVar2;
    sSpriteActor* psVar3;
    uint uVar4;
    sFieldScriptEntity* psVar5;

    psVar5 = pCurrentFieldScriptActor;
    bVar2 = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    if (bVar2 == 1) {
        puVar1 = (ushort*)&pCurrentFieldScriptActor->m106_currentRotation;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x8000;
        psVar5->m11C = *puVar1;
        goto LAB_Field__8008e9d8;
    }
    if (bVar2 < 2) {
        if (bVar2 != 0) goto LAB_Field__8008e9d8;
        if ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x8000) != 0) {
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x8000;
        }
        psVar5 = pCurrentFieldScriptActor;
        if ((pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0x80000) == 0) goto LAB_Field__8008e9d8;
        psVar3 = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
        psVar3->m18_moveSpeed = 0;
        (psVar3->mC_step).vz = 0;
        (psVar3->mC_step).vx = 0;
        uVar4 = psVar5->m4_flags.m_rawFlags & 0xfff7ffff;
    }
    else {
        if (bVar2 != 2) goto LAB_Field__8008e9d8;
        uVar4 = pCurrentFieldScriptActor->m4_flags.m_rawFlags | 0x80000;
    }
    psVar5->m4_flags.m_rawFlags = uVar4;
LAB_Field__8008e9d8:
    ADVANCE_VM(0x2);
}

void OP_SET_FLAGS_DYNAMIC()
{
    u32 uVar1 = readU16FromScript(2);
    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]) {
    case 0:
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= uVar1;
        break;
    case 1:
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= uVar1 << 0x10;
        break;
    case 2:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags | uVar1;
        break;
    case 3:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags | uVar1 << 0x10;
        break;
    case 4:
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~uVar1;
        break;
    case 5:
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~(uVar1 << 0x10);
        break;
    case 6:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & ~uVar1;
        break;
    case 7:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & ~(uVar1 << 0x10);
        break;
    default:
        assert(0);
    }
    ADVANCE_VM(0x4);
}

void OP_SET_ENCOUNTER_DATA() 
{
    encounterTimer = getImmediateOrVariableUnsigned(1);
    encounterCount = std::min<uint>(getImmediateOrVariableUnsigned(3), 0x20);
    setInitialEncounterData();
    ADVANCE_VM(0x5);
}

void OP_44()
{
    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode == 0)
    {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 3;

        pCurrentFieldScriptActor->mD0_targetPositionOffset[0] = pCurrentFieldScriptActor->m20_position.vx >> 0x10;
        pCurrentFieldScriptActor->mD0_targetPositionOffset[1] = pCurrentFieldScriptActor->m20_position.vy >> 0x10;
        pCurrentFieldScriptActor->mD0_targetPositionOffset[2] = pCurrentFieldScriptActor->m20_position.vz >> 0x10;
    }

    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == -1)
    {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = getImmediateOrVariableUnsigned(3);
    }

    if (spriteWalkToPositionOrActor(getImmediateOrVariableUnsigned(3)) == 0)
    {
        ADVANCE_VM(0x5);
    }
}

void OP_46(void)
{
    sFieldScriptEntity* psVar1;
    sFieldEntity* psVar2;
    int iVar3;

    iVar3 = getAngleSin((int)pCurrentFieldEntity->m50_modelRotation.vy);
    psVar2 = pCurrentFieldEntity;
    pCurrentFieldScriptActor->m60[0] = (short)((uint)(iVar3 * 9) >> 10);
    iVar3 = getAngleCos((int)psVar2->m50_modelRotation.vy);
    psVar1 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m60[2] = (short)(iVar3 * -0x24 >> 0xc);
    psVar1->m4_flags.m_rawFlags = psVar1->m4_flags.m_rawFlags | 0x800;
    ADVANCE_VM(0x1);
    return;
}

void enablePlayerFlag80(void)
{
    (actorArray[g_playerControlledActor].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags |= 0x80;
    return;
}

int walkAndChangeField(int param_1, int param_2, int param_3, int param_4)
{
    fieldRandomBattleVar = -1;

    sFieldScriptEntity* psVar11 = actorArray[g_playerControlledActor].m4C_scriptEntity;
    sSpriteActor* psVar12 = actorArray[g_playerControlledActor].m4_pVramSpriteSheet;
    psVar11->m4_flags.m_rawFlags = psVar11->m4_flags.m_rawFlags | 0x38;
    psVar12->m18_moveSpeed = 0x80000;
    int lVar6 = length1d(8);
    int sVar5 = (psVar11->m20_position).vx.getIntegerPart();
    int sVar1 = (psVar11->m20_position).vz.getIntegerPart();

    if (param_2 == 0) {
        if ((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) {
            breakCurrentScript = 1;
        }
        if (fieldChangePrevented != 0) {
            int sVar3 = getImmediateOrVariableUnsigned(4);
            int uVar7 = getImmediateOrVariableUnsigned(2);
            SaveFieldAndDirections();
            fieldChangePrevented = 0;
            setVar(2, sVar3);
            fieldMapNumber = uVar7;
        }
        int iVar10 = pCurrentFieldEntity->m50_modelRotation[1] + param_1 + -0x400;
        int iVar8 = getAngleSin(iVar10);
        param_3 = (int)pCurrentFieldScriptActor->m60[0] + (int)((int)(pCurrentFieldScriptActor->m20_position).vx.getIntegerPart()) + (iVar8 * 0x28 >> 0xc);
        iVar8 = getAngleCos(iVar10);
        param_4 = (int)pCurrentFieldScriptActor->m60[2] + (int)((int)(pCurrentFieldScriptActor->m20_position).vz.getIntegerPart()) + (iVar8 * -0x28 >> 0xc);
    }

    FP_VEC3 local_38;
    local_38.vx = param_3 - sVar5;
    local_38.vz = param_4 - sVar1;
    local_38.vy = 0;
    int lVar9 = length2d(local_38.vx, local_38.vz);
    if (lVar6 << 1 < lVar9) {
        if (((psVar11->m68_oldPosition[0] == ((int)(psVar11->m20_position).vx.getIntegerPart())) && (psVar11->m68_oldPosition[1] == ((int)(psVar11->m20_position).vy.getIntegerPart()))) &&
            (psVar11->m68_oldPosition[2] == ((int)(psVar11->m20_position).vz.getIntegerPart()))) {
            psVar11->m6E = psVar11->m6E + 1;
        }
        else {
            psVar11->m6E = 0;
        }
        if (psVar11->m6E < 0x41) {
            sVar5 = fp_atan2(local_38);
            psVar11->m104_rotation = sVar5;
            psVar11->m106_currentRotation = sVar5;
            breakCurrentScript = 1;
            if (param_2 == 0) {
                breakCurrentScript = 1;
                return -1;
            }
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
            return -1;
        }
    }

    int uVar4 = psVar11->m106_currentRotation | 0x8000;
    psVar11->m104_rotation = uVar4;
    psVar11->m106_currentRotation = uVar4;
    psVar12->m18_moveSpeed = 0;
    psVar11->mE8_currentAnimationId = 0;
    setVisualAnimation(psVar12, 0, &actorArray[g_playerControlledActor]);
    breakCurrentScript = 1;
    psVar11->m8C_scriptSlots[psVar11->mCE_currentScriptSlot].m4_flags.raw = 0xffff;
    psVar11->m8C_scriptSlots[psVar11->mCE_currentScriptSlot].m4_flags.raw &= 0xfe7fffff;
    psVar11->m0_fieldScriptFlags.m_rawFlags = psVar11->m0_fieldScriptFlags.m_rawFlags & 0xffdfffff;
    if (param_2 == 1) {
        psVar11->m4_flags.m_rawFlags = psVar11->m4_flags.m_rawFlags & 0xffffffc7;
    }
    psVar11->m6E = 0;
    ADVANCE_VM(0x6);
    return 0;
}

void OP_WALK_AND_CHANGE_FIELD()
{
    if ((((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) || (loadCompleted != 0)) || ((fieldMusicLoadPending == -1 || (load2dAnimVar != 0)))) {
        breakCurrentScript = 1;
    }
    else {
        enablePlayerFlag80();
        walkAndChangeField(0x3e0, 0, 0, 0);
    }
}

void  OP_SPRITE_WALK_TO_POSITION_AND_WAIT()
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xffff;
    if (spriteWalkToPositionOrActor(-1) == 0) {
        ADVANCE_VM(0x6);
    }
}

void OP_4B()
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0xFFFF) {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = getImmediateOrVariableUnsigned(6);
    }
    if (spriteWalkToPositionOrActor(getImmediateOrVariableUnsigned(6)) == 0) {
        ADVANCE_VM(0x8);
    }
}

void OP_4C()
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xFFFF;
    MissingCode();
    ADVANCE_VM(0x8);
}

void OP_52(void)
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 2;
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xffff;
    if (spriteWalkToPositionOrActor(0xffff) == 0) {
        ADVANCE_VM(0x2);
    }
    return;
}

void OP_FOLLOW_CHARACTER(void)
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 2;
    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0xFFFF)
    {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = getImmediateOrVariableUnsigned(2);
    }
    if (spriteWalkToPositionOrActor(getImmediateOrVariableUnsigned(2)) == 0)
    {
        ADVANCE_VM(0x4);
    }
}

void OP_55(void)
{
    MissingCode();
    ADVANCE_VM(0x7);
}

void OP_WORLDMAP(void) {
    if ((((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) || (loadCompleted != 0)) || ((fieldMusicLoadPending == -1 || (load2dAnimVar != 0)))) {
        breakCurrentScript = 1;
    }
    else {
        fieldPerFrameReset();
        fieldRandomBattleVar = -1;
        fieldExecuteVar3 = 0;
        pKernelGameState->m231A_fieldID = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
        pKernelGameState->m231E_worldmapInitialPositionIndex = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
        s32 uVar4 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
        if ((uVar4 & 0xffff) == 0xffff || uVar4 == 0xffffffff) {
            uVar4 = (uint)(ushort)cameraRotationBetweenEyeAndAt.vy;
        }
        pKernelGameState->m231C_CameraYaw = (short)uVar4 + 0x800U & 0xfff;
        pKernelGameState->m2320_worldmapMode = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
        OP_CHANGE_FIELD_WHEN_READY_Sub();
        kernelAndFieldStatesSynced = 1;
        breakCurrentScript = 1;
        ADVANCE_VM(0xA);
    }
}

void OP_57(void)
{
    std::array<VECTOR, 4> auStack168;
    std::array<SVECTOR, 4> auStack104;

    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x10000;
    int iVar6;
    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 3)
    {
    case 0:
        iVar6 = getVar10(8, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
        break;
    case 3:
        if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0xF)
        {
            for (int i = 0; i < g_numWalkMesh - 1; i++)
            {
                pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(pCurrentFieldScriptActor->m20_position.vx >> 0x10, pCurrentFieldScriptActor->m20_position.vz >> 0x10, i, &auStack104[i], &auStack168[i]);
            }
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x10000;
            pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xffdfffff;
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
            breakCurrentScript = 1;
            return;
        }

        if (pCurrentFieldScriptActor->m102_numSteps < pCurrentFieldScriptActor->mE0_rotationLimit)
        {
            (pCurrentFieldScriptActor->m20_position).vx += pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
            (pCurrentFieldScriptActor->m20_position).vz += pCurrentFieldScriptActor->mD0_targetPositionOffset[2];
            (pCurrentFieldScriptActor->m20_position).vy += (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step).vy;
            (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step).vy += actorArray[currentFieldActorId].m4_pVramSpriteSheet->m1C_gravity;
            if (((pCurrentFieldScriptActor->mD0_targetPositionOffset[0] != 0) || (pCurrentFieldScriptActor->mD0_targetPositionOffset[2] != 0)) && ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x8000) == 0)) {
                s32 uVar5 = fp_atan2(pCurrentFieldScriptActor->mD0_targetPositionOffset);
                pCurrentFieldScriptActor->m104_rotation = uVar5 | 0x8000;
                pCurrentFieldScriptActor->m106_currentRotation = uVar5 | 0x8000;
            }
        }
        else
        {
            pCurrentFieldScriptActor->mCC_scriptPC -= 0xB;
            int iVar6 = getVar80(2, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
            int iVar7 = getVar40(4, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
            int iVar8;
            if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x80) == 0) {
                iVar8 = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
            }
            else {
                iVar8 = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);

                pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[iVar8] = findTriangleInWalkMesh(iVar6, iVar7, iVar8, &auStack104[iVar8], &auStack168[iVar8]);
                iVar8 = auStack104[iVar8].vy;
            }
            (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step).vy = 0;
            (pCurrentFieldScriptActor->m20_position).vx = iVar6 << 0x10;
            (pCurrentFieldScriptActor->m20_position).vy = iVar8 << 0x10;
            (pCurrentFieldScriptActor->m20_position).vz = iVar7 << 0x10;
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x10000;
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xd;
            pCurrentFieldScriptActor->m4_flags.m_rawFlags &= ~0x00200000;
        }

        actorArray[currentFieldActorId].mC_matrix.t[0] = (pCurrentFieldScriptActor->m20_position).vx >> 0x10;
        actorArray[currentFieldActorId].mC_matrix.t[1] = (pCurrentFieldScriptActor->m20_position).vy >> 0x10;
        actorArray[currentFieldActorId].mC_matrix.t[2] = (pCurrentFieldScriptActor->m20_position).vz >> 0x10;
        actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = (pCurrentFieldScriptActor->m20_position).vx;
        actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = (pCurrentFieldScriptActor->m20_position).vy;
        actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = (pCurrentFieldScriptActor->m20_position).vz;
        pCurrentFieldScriptActor->m102_numSteps = pCurrentFieldScriptActor->m102_numSteps + 1;
        breakCurrentScript = 1;
        return;
    default:
        assert(0);
    }

    if (iVar6 == 0) {
        iVar6 = 1;
    }
    int iVar7 = getVar80(2, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
    int iVar8 = getVar40(4, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
    int iVar9;
    if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x80) == 0) {
        iVar9 = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
    }
    else {
        int iVar15 = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);

        findTriangleInWalkMesh(iVar7, iVar8, iVar15, &auStack104[iVar15], &auStack168[iVar15]);

        iVar9 = (int)auStack104[iVar15].vy;
        pCurrentFieldScriptActor->m10_walkmeshId = (short)iVar15;
    }
    int iVar15 = -((actorArray[currentFieldActorId].m4_pVramSpriteSheet->m1C_gravity * iVar6) / 2);
    (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step).vy = iVar15;
    (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step).vy = iVar15 + (iVar9 * 0x10000 - (pCurrentFieldScriptActor->m20_position).vy) / iVar6;
    iVar9 = (pCurrentFieldScriptActor->m20_position).vx;
    iVar15 = (pCurrentFieldScriptActor->m20_position).vz;
    pCurrentFieldScriptActor->mD0_targetPositionOffset[1] = 0;
    pCurrentFieldScriptActor->mE0_rotationLimit = (short)iVar6;
    pCurrentFieldScriptActor->m102_numSteps = 0;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xb;
    pCurrentFieldScriptActor->mD0_targetPositionOffset[0] = (iVar7 * 0x10000 - iVar9) / (iVar6 + 1);
    pCurrentFieldScriptActor->mD0_targetPositionOffset[2] = (iVar8 * 0x10000 - iVar15) / (iVar6 + 1);
    breakCurrentScript = 1;
}

void OP_SET_ANGLE_AXIS_ROTATION()
{
    byte bVar1;
    int iVar2;
    short sVar3;

    bVar1 = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3];
    if (bVar1 == 1) {
        sVar3 = getImmediateOrVariableUnsigned(1);
        actorArray[currentFieldActorId].m50_modelRotation[1] = sVar3;
    }
    else if (bVar1 < 2) {
        if (bVar1 == 0) {
            sVar3 = getImmediateOrVariableUnsigned(1);
            actorArray[currentFieldActorId].m50_modelRotation[0] = sVar3;
        }
    }
    else if (bVar1 == 2) {
        sVar3 = getImmediateOrVariableUnsigned(1);
        actorArray[currentFieldActorId].m50_modelRotation[2] = sVar3;
    }
    iVar2 = currentFieldActorId;
    ADVANCE_VM(0x4);
    updateScriptActor3dRotation(iVar2);
}


void OP_WALK_RANDOM_DIRECTION()
{
    s16 newRotation = pCurrentFieldScriptActor->m106_currentRotation;
    pCurrentFieldScriptActor->m102_numSteps++;

    if ((pCurrentFieldScriptActor->m102_numSteps & 0xf) == 0) {
        u32 uVar4 = xenoRand();
        if ((uVar4 & 0x30) == 0) {
            if ((uVar4 & 1) == 0) {
                newRotation = pCurrentFieldScriptActor->m106_currentRotation + 0x200;
            }
            else {
                newRotation = pCurrentFieldScriptActor->m106_currentRotation - 0x200;
            }
            newRotation = newRotation & 0xfff;
        }
        else {
            newRotation = pCurrentFieldScriptActor->m106_currentRotation | 0x8000;
            pCurrentFieldScriptActor->m106_currentRotation = newRotation;
        }
    }
    breakCurrentScript = 1;
    pCurrentFieldScriptActor->m104_rotation = newRotation;
    ADVANCE_VM(0x1);
}

void OP_5A()
{
    resetFieldScriptActor();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_5CSub0(int param_1, int* param_2, int* param_3, int* param_4)
{
    int uVar1;
    int uVar2;
    int iVar1;
    int uVar4;
    int uVar3;

    if (param_1 == 1) {
        iVar1 = getVariable(0x30);
        *param_2 = iVar1;
        uVar4 = getVariable(0x32);
        uVar3 = 0x34;
    }
    else {
        if (param_1 < 2) {
            if (param_1 != 0) {
                return;
            }
            uVar1 = getVariable(0x2a);
            *param_2 = uVar1;
            uVar2 = getVariable(0x2c);
            uVar3 = 0x2e;
            uVar4 = uVar2;
        }
        else {
            if (param_1 != 2) {
                return;
            }
            iVar1 = getVariable(0x36);
            *param_2 = iVar1;
            uVar4 = getVariable(0x38);
            uVar3 = 0x3a;
        }
    }
    *param_3 = uVar4;
    uVar4 = getVariable(uVar3);
    *param_4 = uVar4;
    return;
}

void OP_5C()
{
    sFieldEntity* pFieldEntity = &actorArray[currentFieldActorId];
    int param0 = getImmediateOrVariableUnsigned(1);
    if (2 < param0) {
        param0 = 2;
    }
    unkPartyTable[param0] = currentFieldActorId;
    int iVar5 = currentParty[param0];
    int bVar4;
    if ((iVar5 == 0xff) || (bVar4 = getGearForCharacter(iVar5), iVar5 = param0, bVar4 == 0xff)) {
        OP_INIT_ENTITY_SCRIPT();
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0x800;
    }
    else {
        int entityParam;
        int position[2];
        OP_5CSub0(param0, &entityParam, &position[0], &position[1]);

        bool bFieldDifferent = (fieldMapNumber & 0xfff) == (entityParam & 0x3fffU);
        pCurrentFieldScriptActor->m10_walkmeshId = (ushort)(entityParam >> 0xe) & 3;

        if (!bFieldDifferent) {
            position[0] = 0;
            position[1] = 0;
            pCurrentFieldScriptActor->m10_walkmeshId = 0;
        }

        if ((pKernelGameState->m22B1_isOnGear[param0] != 0) || (currentParty[param0] == 7)) {
            bFieldDifferent = false;
        }

        pFieldEntity->m58_flags = pFieldEntity->m58_flags & 0xf07f | 0x200;

        resetFieldScriptEntityGraphicEntity(currentFieldActorId, param0, &partyCharacterBuffers[param0], 1, 0, param0, 1);
        actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
        if ((fieldMapNumber & 0xfff) != (entityParam & 0x3fffU)) {
            pCurrentFieldScriptActor->m10_walkmeshId = 0;
        }

        setCurrentActor2DPosition(position[0], position[1]);
        OP_INIT_ENTITY_SCRIPT_sub1();
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0xfffffcff | 0x400;
        if (!bFieldDifferent) {
            actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags | 0x20;
        }
        if (g_numWalkMesh + -1 < (int)pCurrentFieldScriptActor->m10_walkmeshId) {
            pCurrentFieldScriptActor->m10_walkmeshId = 0;
        }
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x20000;
        pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0xc00;
        ADVANCE_VM(0x3);
    }
}

void OP_PLAY_ANIMATION_EX()
{
    OP_PLAY_ANIMATION();
    pCurrentFieldScriptActor->m4_flags.m_rawFlags &= ~0x10000;
}

void OP_WAIT_ANIMATION_END()
{
    if ((pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0x10000) != 0) {
        pCurrentFieldScriptActor->mEA_forcedAnimation = 0xff;
        ADVANCE_VM(0x1);
    }
}

void OP_CHANGE_FIELD_WHEN_READY2()
{
    short sVar1;
    int uVar2;

    if ((((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) || (loadCompleted != 0)) || (((fieldMusicLoadPending == -1 || (load2dAnimVar != 0)) || (iRam800adb70 != 0)))) {
        breakCurrentScript = 1;
    }
    else {
        fieldRandomBattleVar = -1;
        if (fieldChangePrevented != 0) {
            sVar1 = getImmediateOrVariableUnsigned(3);
            uVar2 = getImmediateOrVariableUnsigned(1);
            SaveFieldAndDirections();
            fieldChangePrevented = 0;
            setVar(2, sVar1);
            fieldMapNumber = uVar2;
            OP_CHANGE_FIELD_WHEN_READY_Sub();
        }
        breakCurrentScript = 1;
        ADVANCE_VM(0x5);
    }
}

void OP_99(void)
{
    cameraInterpolationMode = 1;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    op99Var1 = sceneSCRZ * sceneScale >> 0xc;
    op99Var2 = sceneDIP;
    op99Var3 = cameraRotationBetweenEyeAndAt.vy;
    op99Var4 = 0x1000;
    op99Var5 = 0xc;
    op99Var6 = 0xc;
    op99Var7 = op99Var7 | 0x8000;
}

void OP_9A(void)
{
    int iVar1;

    if (cameraInterpolationMode == 1) {
        iVar1 = getImmediateOrVariableUnsigned(1);
        if (iVar1 == 0) {
            cameraInterpolationMode = 0;
            op99Var7 = op99Var7 & 0x7fff;
            ADVANCE_VM(0x3);
            op9DVar1 = 2;
        }
        else {
            cameraInterpolationMode = 2;
            op99Var5 = iVar1;
            op99Var6 = iVar1;
        }
    }
    else {
        if (1 < cameraInterpolationMode) {
            return;
        }
        if (cameraInterpolationMode != 0) {
            return;
        }
        op99Var7 = op99Var7 & 0x7fff;
    }
    ADVANCE_VM(0x3);
    return;
}

void OP_IF_GAMEPROGRESS_LESS()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    iVar2 = getImmediateOrVariableUnsigned(1);
    iVar3 = getVariable(0);
    if (iVar3 < iVar2) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    }
    else {
        uVar1 = readU16FromScript(3);
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

void OP_IF_GAMEPROGRESS_GREATER()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    iVar2 = getImmediateOrVariableUnsigned(1);
    iVar3 = getVariable(0);
    if (iVar2 < iVar3) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    }
    else {
        uVar1 = readU16FromScript(3);
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

void OP_IF_GAMEPROGRESS_EQUAL()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    iVar2 = getImmediateOrVariableUnsigned(1);
    iVar3 = getVariable(0);
    if (iVar3 == iVar2) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    }
    else {
        uVar1 = readU16FromScript(3);
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

void OP_SET_GAMEPROGRESS()
{
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 0x20;
    setVar(0, getImmediateOrVariableUnsigned(1));
    ADVANCE_VM(0x3);
}

void OP_ADD_GOLD() {
    sFieldScriptEntity* psVar1;
    int iVar2;

    iVar2 = getImmediateOrVariableUnsigned(1);
    psVar1 = pCurrentFieldScriptActor;
    iVar2 = pKernelGameState->m1924_Gold + iVar2;
    if (9999999 < iVar2) {
        iVar2 = 9999999;
    }
    pKernelGameState->m1924_Gold = iVar2;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 3;
}

void OP_IF_CHARACTER_IN_PARTY()
{
    for (int i = 0; i < 3; i++) {
        if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == currentParty[i]) {
            pCurrentFieldScriptActor->mCC_scriptPC += 4;
            return;
        }
    }

    pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);
}

void OP_92()
{
    for (int i=0; i<8; i++)
    {
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m2_delay = 0;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m0 = -1;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m0_scriptPC = -1;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m16_status = 0;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m18 = 0xF;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m26 = 0;
        pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m27 = 0;
    }

    pCurrentFieldScriptActor->mCE_currentScriptSlot = 0;
    pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn = 0;
    pCurrentFieldScriptActor->m84_dialogWindowFlags = 0;
    pCurrentFieldScriptActor->m12C_flags &=  0xfffffe3f;
    breakCurrentScript = 1;
}

void OP_ADD_ENDITY_TO_MECHA_LIST()
{
    actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
    int iVar3 = getImmediateOrVariableUnsigned(1);
    resetFieldScriptEntityGraphicEntity(currentFieldActorId, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 1);
    OP_INIT_ENTITY_SCRIPT_sub1();
    sFieldScriptEntity* psVar2 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
    int iVar1 = currentFieldActorId;
    psVar2->m0_fieldScriptFlags.m_rawFlags = psVar2->m0_fieldScriptFlags.m_rawFlags | 0x100;
    actorArray[iVar1].m58_flags = actorArray[iVar1].m58_flags & 0xffdf;
    psVar2->m4_flags.m_rawFlags = psVar2->m4_flags.m_rawFlags & 0xfffff7ff | 0x2000;
    mechaList[NumMechas] = (short)(iVar3 << 1);
    mechaList2[NumMechas] = 0;
    pCurrentFieldScriptActor->m12C_flags = pCurrentFieldScriptActor->m12C_flags & 0xffff1fff | (NumMechas & 7) << 0xd;
    NumMechas = NumMechas + 1;
}

void OP_SET_CAMERA_INTERPOLATION_RATE(void)
{
    op99Var5 = getImmediateOrVariableUnsigned(1);
    op99Var6 = getImmediateOrVariableUnsigned(3);
    if (op99Var5 == 0) {
        op99Var5 = 1;
    }
    if (op99Var6 == 0) {
        op99Var6 = 1;
    }
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x5);
}

void OP_WAIT_DIALOG()
{
    int windowIndex;
    if (findDialogWindowForCurrentActor(&windowIndex) == -1) {
        fieldExectuteMaxCycles = fieldExectuteMaxCycles + 8;
        setVar(0x14, pCurrentFieldScriptActor->m81_selectedMultichoiceLine);
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    }
    else {
        if (((actorArray[gDialogWindows[windowIndex].m418_actorId].m4C_scriptEntity)->m4_flags.m_rawFlags & 0x200) != 0) {
            int uVar2 = (uint)pCurrentFieldScriptActor->m84_dialogWindowFlags >> 0x10;
            if (uVar2 == 0) {
                uVar2 = pCurrentFieldScriptActor->m84_dialogWindowFlags & 0xffff;
            }
            if ((uVar2 & 1) == 0) {
                if ((pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18) != 7) {
                    OP_STOP();
                }
                gDialogWindows[windowIndex].m414 = 0;
            }
        }
        breakCurrentScript = 1;
    }
}

void OP_9D()
{
    op9DVar0 = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3];
    if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] == 0) {
        op9DVar0 = 1;
        op9DVar1 = op9DVar1 + 2;
    }
    s32 iVar1 = getImmediateOrVariableUnsigned(1);
    op9DVar2 = ((sceneScale - iVar1) * -0x10000) / (int)(short)op9DVar0;
    op9DVar3 = (int)sceneScale << 0x10;
    op99Var7 = op99Var7 | 1;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_SET_SCREEN_GEOMETRY()
{
    sceneDIP = getImmediateOrVariableUnsigned(3);
    int iVar1 = getImmediateOrVariableUnsigned(1);
    int uVar2 = iVar1 + 4U & 7;
    opA0_var0 = uVar2 << 9;
    opA0_var1 = uVar2 << 0x19;
    cameraRotationBetweenEyeAndAt.vy = (short)(uVar2 << 9);
    sceneSCRZ = getImmediateOrVariableUnsigned(5);
    SetGeomScreen(sceneSCRZ);
    ADVANCE_VM(0x7);
}

void OP_SET_DOLLY_STOP()
{
    DollyStop = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OP_WAIT_FOR_CAMERA_ANIMATION_FLAG_CLEAR()
{
    if ((op99Var7 & pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]) == 0) {
        ADVANCE_VM(0x2);
    }
    else {
        breakCurrentScript = 1;
    }
}

void OP_SET_DESIRED_CAMERA_POSITION()
{
    int iVar1 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraPosition[0] = iVar1 << 0x10;
    iVar1 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraPosition[2] = iVar1 << 0x10;
    iVar1 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraPosition[1] = iVar1 << 0x10;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x8);
}

void OP_A4Sub(int param_1, int param_2)
{
    if (param_2 == 0) {
        op9DVar1 = 2;
        param_2 = 1;
    }
    op99Var7 = op99Var7 | 8;
    OP_A4Var0 = (short)param_2;
    OP_A4Var1 = (int)sceneDIP << 0x10;
    OP_A4Var2 = ((sceneDIP - param_1) * -0x10000) / param_2;
    return;
}

void OP_A4()
{
    int uVar1;

    uVar1 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3]);
    OP_A4Sub(uVar1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] & 0x7f);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_SETUP_MULTICHOICE()
{
    byte bVar1;
    byte bVar2;
    sFieldScriptEntity* psVar4;
    int iVar5;
    int dialogWindowId;

    iVar5 = findDialogWindowForCurrentActor(&dialogWindowId);
    if (iVar5 == 0) {
        fieldExectuteMaxCycles = fieldExectuteMaxCycles + 8;
        iVar5 = addDialogWindowsToOTSub0(&gDialogWindows[dialogWindowId].m18);
        psVar4 = pCurrentFieldScriptActor;
        if (iVar5 != 1) {
            if (gDialogWindows[dialogWindowId].m18.m84_delay == 0) {
                breakCurrentScript = 1;
                return;
            }
            if (gDialogWindows[dialogWindowId].m18.m6C_autoClose == 0) {
                breakCurrentScript = 1;
                return;
            }
        }
        gDialogWindows[dialogWindowId].m37C = 0;
        psVar4->m81_selectedMultichoiceLine = 0xff;
        psVar4 = pCurrentFieldScriptActor;
        bVar1 = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
        gDialogWindows[dialogWindowId].m37E_startOfMultiChoice = (ushort)(bVar1 >> 4);
        bVar2 = pCurrentFieldScriptFile[psVar4->mCC_scriptPC + 1];
        gDialogWindows[dialogWindowId].m382_CurrentMultiChoiceSelection = 0;
        gDialogWindows[dialogWindowId].m380_numMaxMultiChoiceSelection = ((bVar2 & 0xf) - (ushort)(bVar1 >> 4)) + 1;
        setupMultiChoiceColor(&gDialogWindows[dialogWindowId].m18, 0xef, 0x1e, 0xf0);
    }
    ADVANCE_VM(0x2);
    breakCurrentScript = 1;
}

void OP_A8()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    iVar2 = xenoRand();
    iVar3 = getImmediateOrVariableUnsigned(3);
    uVar1 = readU16FromScript(1);
    setVar(uVar1, iVar2 * (iVar3 + 1) >> 0xf);
    ADVANCE_VM(0x5);
}

void OP_ROTATE_CAMERA_RELATIVE(void)
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;

    psVar2 = pCurrentFieldScriptActor;
    iVar1 = fieldExecuteVar1;
    uVar3 = actorDirectionTable2[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]] - camera2Tan & 0xfffU | 0x8000;
    pCurrentFieldScriptActor->m104_rotation = uVar3;
    psVar2->m106_currentRotation = uVar3;
    if (iVar1 == 0) {
        psVar2->m108_rotation3 = uVar3;
    }
    ADVANCE_VM(0x2);
}

void OP_AB(void)
{
    cameraTargetOverride[2] = g_cameraAt2.vz;
    desiredCameraTarget[2] = g_cameraAt2.vz;
    cameraTargetOverride[0] = g_cameraAt2.vx;
    desiredCameraTarget[0] = g_cameraAt2.vx;
    cameraTargetOverride[1] = g_cameraAt2.vy;
    desiredCameraTarget[1] = g_cameraAt2.vy;
    cameraPositionOverride[0] = g_cameraEye2.vx;
    cameraPositionOverride[1] = g_cameraEye2.vy;
    cameraPositionOverride[2] = g_cameraEye2.vz;
    desiredCameraPosition[0] = g_cameraEye2.vx;
    desiredCameraPosition[1] = g_cameraEye2.vy;
    desiredCameraPosition[2] = g_cameraEye2.vz;
    fieldExectuteMaxCycles++;
    ADVANCE_VM(0x1);
    return;
}

void OP_SETUP_CAMERA_INTERPOLATION_STEPS()
{
    VALIDATE_FIELD(FCT_Script, 0x800903bc);
    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0xf)
    {
    case 0:
        cameraInterpolationTargetNumSteps = getImmediateOrVariableUnsigned(2);
        if (cameraInterpolationTargetNumSteps == 0) {
            cameraInterpolationTargetNumSteps = 1;
            op99Var5 = 1;
        }
        cameraInterpolationTargetStep[0] = (desiredCameraTarget[0] - cameraTargetOverride[0]) / cameraInterpolationTargetNumSteps;
        cameraInterpolationTargetStep[1] = (desiredCameraTarget[1] - cameraTargetOverride[1]) / cameraInterpolationTargetNumSteps;
        cameraInterpolationTargetStep[2] = (desiredCameraTarget[2] - cameraTargetOverride[2]) / cameraInterpolationTargetNumSteps;
        cameraInterpolationTargetStartPosition[0] = cameraTargetOverride[0];
        cameraInterpolationTargetStartPosition[1] = cameraTargetOverride[1];
        cameraInterpolationTargetStartPosition[2] = cameraTargetOverride[2];
        cameraInterpolationFlags = cameraInterpolationFlags | 1;
        if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x80) != 0) {
            g_cameraAt[0] = cameraTargetOverride[0];
            g_cameraAt[1] = cameraTargetOverride[1];
            g_cameraAt[2] = cameraTargetOverride[2];
        }
        break;
    case 1:
        cameraInterpolationPositionNumSteps = getImmediateOrVariableUnsigned(2);
        if (cameraInterpolationPositionNumSteps == 0) {
            cameraInterpolationPositionNumSteps = 1;
            op99Var6 = 1;
        }
        cameraInterpolationPositionStep[0] = (desiredCameraPosition[0] - cameraPositionOverride[0]) / cameraInterpolationPositionNumSteps;
        cameraInterpolationPositionStep[1] = (desiredCameraPosition[1] - cameraPositionOverride[1]) / cameraInterpolationPositionNumSteps;
        cameraInterpolationPositionStep[2] = (desiredCameraPosition[2] - cameraPositionOverride[2]) / cameraInterpolationPositionNumSteps;
        cameraInterpolationStartPosition[0] = cameraPositionOverride[0];
        cameraInterpolationStartPosition[1] = cameraPositionOverride[1];
        cameraInterpolationStartPosition[2] = cameraPositionOverride[2];
        cameraInterpolationFlags = cameraInterpolationFlags | 2;
        if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x80) != 0) {
            g_cameraEye[0] = cameraPositionOverride[0];
            g_cameraEye[1] = cameraPositionOverride[1];
            g_cameraEye[2] = cameraPositionOverride[2];
        }
        break;
    default:
        assert(0);
        break;
    }
    VALIDATE_FIELD(FCT_Script, 0x80090a08);
    ADVANCE_VM(0x4);
}

void OP_60()
{
    cameraTargetOverride[2] = g_cameraAt2.vz;
    cameraTargetOverride[0] = g_cameraAt2.vx;
    cameraTargetOverride[1] = g_cameraAt2.vy;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x1);
}

void OP_64()
{
    cameraPositionOverride[2] = g_cameraEye2.vz;
    cameraPositionOverride[0] = g_cameraEye2.vx;
    cameraPositionOverride[1] = g_cameraEye2.vy;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x1);
}


void OP_SET_CAMERA_TARGET_OVERRIDE()
{
    int iVar1 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraTargetOverride[0] = iVar1 << 0x10;
    iVar1 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraTargetOverride[2] = iVar1 << 0x10;
    iVar1 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraTargetOverride[1] = iVar1 << 0x10;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x8);
}

void OP_SET_DESIRED_CAMERA_TARGET_TO_ENTITY()
{
    int iVar1;
    sFieldScriptEntity* psVar2;

    iVar1 = getCharacterToEntity(1);
    psVar2 = actorArray[iVar1].m4C_scriptEntity;
    desiredCameraTarget[0] = (psVar2->m20_position).vx;
    desiredCameraTarget[1] = (psVar2->m20_position).vy;
    desiredCameraTarget[2] = (psVar2->m20_position).vz;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x2);
}

void OP_SET_DESIRED_CAMERA_TARGET()
{
    int iVar1 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraTarget[0] = iVar1 << 0x10;
    iVar1 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraTarget[2] = iVar1 << 0x10;
    iVar1 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    desiredCameraTarget[1] = iVar1 << 0x10;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x8);
}

void OP_SET_CAMERA_POSITION_OVERRIDE()
{
    int iVar1 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraPositionOverride[0] = iVar1 << 0x10;
    iVar1 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraPositionOverride[2] = iVar1 << 0x10;
    iVar1 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    cameraPositionOverride[1] = iVar1 << 0x10;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x8);
}

void OP_67Sub(ushort param_1)
{
    ushort uVar1;
    uint uVar2;
    sFieldScriptEntity* psVar3;

    uVar2 = readCharacter(1);
    if (uVar2 != 0xff) {
        uVar2 = readCharacter(1);
        psVar3 = actorArray[uVar2].m4C_scriptEntity;
        uVar1 = param_1 | 0x8000;
        if (fieldExecuteVar1 == 0) {
            psVar3->m104_rotation = uVar1;
            psVar3->m106_currentRotation = uVar1;
            psVar3->m108_rotation3 = uVar1;
        }
        psVar3->m104_rotation = param_1 | 0x8000;
        psVar3->m106_currentRotation = param_1 | 0x8000;
    }
    ADVANCE_VM(0x4);
    return;
}

void OP_SET_CURRENT_ACTOR_ROTATION_WORLD_RELATIVE()
{
    int iVar1;

    iVar1 = getImmediateOrVariableUnsigned(2);
    OP_67Sub(actorDirectionTable[iVar1]);
}

void OP_SET_ACTOR_ROTATION2()
{
    setCurrentActorRotation2(actorDirectionTable[getImmediateOrVariableUnsigned(1)]);
}

void OP_ROTATE_ACTOR_CLOCKWISE()
{
    int iVar1;
    int iVar2;

    iVar1 = getImmediateOrVariableUnsigned(1);
    iVar2 = getCurrentActorRotationIn8Cardinal();
    setCurrentActorTargetRotation(actorDirectionTable[iVar1 + iVar2 & 7]);
}

void OP_ROTATE_ACTOR_ANTICLOCKWISE()
{
    int iVar1;
    int iVar2;

    iVar1 = getImmediateOrVariableUnsigned(1);
    iVar2 = getCurrentActorRotationIn8Cardinal();
    setCurrentActorTargetRotation(actorDirectionTable[iVar2 - iVar1 & 7]);
}

void OP_SET_CURRENT_ACTOR_ROTATION(void)
{
    setCurrentActorTargetRotation(actorDirectionTable[getImmediateOrVariableUnsigned(1)]);
}

void OP_15()
{
    if ((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) {
        breakCurrentScript = 1;
    }
    else {
        fieldRandomBattleVar = -1;
        ADVANCE_VM(0x1);
    }
}

void OP_CALL_IF_IN_TRIGGER()
{
    ushort uVar2;
    long lVar3;
    uint uVar4;

    // Note: this is done with Z/X and not X/Z coordinates

    sGTE_XY characterPosition(
        actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vz >> 16,
        actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vx >> 16
    );

    sFieldTrigger* psVar5 = &fieldTriggerData[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]];

    std::array<sGTE_XY, 4> triggerCorners;
    triggerCorners[0].set(psVar5->m0[2], psVar5->m0[0]);
    triggerCorners[1].set(psVar5->m6[2], psVar5->m6[0]);
    triggerCorners[2].set(psVar5->mC[2], psVar5->mC[0]);
    triggerCorners[3].set(psVar5->m12[2], psVar5->m12[0]);

    if ((NormalClip(triggerCorners[0], triggerCorners[1], characterPosition) > -1) &&
        (NormalClip(triggerCorners[1], triggerCorners[2], characterPosition) > -1) &&
        (NormalClip(triggerCorners[2], triggerCorners[3], characterPosition) > -1) &&
        (NormalClip(triggerCorners[3], triggerCorners[0], characterPosition) > -1))
    {
        if ((pCurrentFieldScriptActor->m12C_flags & 0x1c0) != 0x100)
        {
            pCurrentFieldScriptActor->m78_stack[pCurrentFieldScriptActor->m12C_flags >> 5 & 0xe] = pCurrentFieldScriptActor->mCC_scriptPC + 4;
            pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);
            uVar4 = pCurrentFieldScriptActor->m12C_flags;
            pCurrentFieldScriptActor->m12C_flags = uVar4 & 0xfffffe3f | ((uVar4 >> 6 & 7) + 1 & 7) << 6;
            return;
        }
    }
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x4);
}

void OP_UPDATE_ELEVATION(void)
{
    pCurrentFieldScriptActor->mEC_elevation = 0;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0xfffbffff | 0x400000;
    ADVANCE_VM(0x1);
    pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy.getIntegerPart();
}

void OP_1F()
{
    pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xfffffff8 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 7;
    pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xffffffc0 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 7 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] >> 1 & 0x38;
    ADVANCE_VM(0x2);
}

void OP_SET_TIME_SCALE()
{
    int sVar2 = getImmediateOrVariableUnsigned(1);
    pCurrentFieldScriptActor->m76 = sVar2;
    SetTimeScale(actorArray[currentFieldActorId].m4_pVramSpriteSheet, sVar2);
    ADVANCE_VM(0x3);
}

void OP_ROTATE_AWAY_FROM_ACTOR()
{
    sFieldScriptEntity* psVar1;
    ushort uVar2;
    long lVar3;

    if (partyToFieldEntityArrayMapping[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]] != 0xff) {
        lVar3 = ratan2(((actorArray[partyToFieldEntityArrayMapping[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]]].m4C_scriptEntity)->m20_position).vz -
            (pCurrentFieldScriptActor->m20_position).vz,
            ((actorArray[partyToFieldEntityArrayMapping[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]]].m4C_scriptEntity)->m20_position).vx -
            (pCurrentFieldScriptActor->m20_position).vx);
        psVar1 = pCurrentFieldScriptActor;
        uVar2 = -(short)lVar3 | 0x8000;
        pCurrentFieldScriptActor->m104_rotation = uVar2;
        psVar1->m106_currentRotation = uVar2;
    }
    ADVANCE_VM(0x2);
}

void OP_72()
{
    if (playMusicAuthorized == 0) {
        breakCurrentScript = 1;
    }
    else {
        musicVar1 = 0;
        playMusicFromScript();
    }
}

void OP_C4SubSub1(int param_1, short param_2, short param_3, uint param_4)
{
    StopSoundEffect((param_4 & 7) << 1);
    PlaySoundEffect2(param_1, (param_4 & 7) << 1, (int)param_2, (int)param_3);
}

int OP_C4SubVar0 = 0;
void playFieldSoundEffect(int param_1, uint param_2)
{
    if (param_1 == 0) {
        StopSoundEffect((param_2 & 7) << 1);
    }
    else {
        OP_C4SubVar0 = param_1;
        OP_C4SubSub1(param_1, 0x7f, 0x40, param_2 & 7);
    }
}

void OP_PLAY_SOUND_EFFECT()
{
    playFieldSoundEffect(getImmediateOrVariableUnsigned(1), 3);
    ADVANCE_VM(0x3);
}

void OP_PLAY_MUSIC()
{
    if (playMusicAuthorized == 0) {
        breakCurrentScript = 1;
    }
    else {
        musicVar1 = -1;
        playMusicFromScript();
    }
}

void OP_76()
{
    op99Var7 &= 0x7fff;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_RESTORE_HP()
{
    for (int i = 0; i < 11; i++)
    {
        pKernelGameState->m26C_party[i].m4C_HP = pKernelGameState->m26C_party[i].m4E_MaxHP;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_RESTORE_MP()
{
    for (int i = 0; i < 11; i++)
    {
        pKernelGameState->m26C_party[i].m50_MP = pKernelGameState->m26C_party[i].m52_MaxMP;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_SET_CURRENT_ACTOR_ELEVATION(void)
{
    int elevation = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 3]);
    setCurrentActorElevation(elevation);
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x40000;
    pCurrentFieldScriptActor->mCC_scriptPC += 4;
    return;
}

void OP_SET_CURRENT_ACTOR_POSITION_3D()
{
    setCurrentActor2DPosition(readS16FromScript(1), readS16FromScript(3));
    setCurrentActorElevation(readS16FromScript(5));
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x40000;

    pCurrentFieldScriptActor->mCC_scriptPC += 7;
}

void OP_JUMP(void)
{
    ushort uVar1;

    uVar1 = readU16FromScript(1);
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
}

void OPX_SET_CAMERA_ANGLE()
{
    computeProjectionMatrixAngles.vx =
        getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    computeProjectionMatrixAngles.vz =
        getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    computeProjectionMatrixAngles.vy =
        getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    ADVANCE_VM(0x8);
}

void OP_LOAD_SPECIAL_2D_ANIMATION()
{
    int iVar1;
    int iVar2;

    if ((load2dAnimVar == 0) && (loadCompleted == 0)) {
        iVar1 = isLoadCompleted();
        if (iVar1 != 0) {
            breakCurrentScript = 1;
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
            return;
        }
        if (pCurrentFieldScriptActor->m124_special2dAnimationId != -1) {
            delete pCurrentFieldScriptActor->m120_special2dAnimation;
            pCurrentFieldScriptActor->m120_special2dAnimation = nullptr;
            pCurrentFieldScriptActor->m120_special2dAnimationRaw.clear();
            pCurrentFieldScriptActor->m124_special2dAnimationId = -1;
        }
        iVar1 = getImmediateOrVariableUnsigned(1);
        setCurrentDirectory(4, 0);
        iVar1 = iVar1 + 0x77a;
        iVar2 = getFileSizeAligned(iVar1);
        pCurrentFieldScriptActor->m124_special2dAnimationId = (short)iVar1;
        pCurrentFieldScriptActor->m120_special2dAnimation = new sSpriteActorAnimationBundle;

        pCurrentFieldScriptActor->m120_special2dAnimationRaw.resize(iVar2 + 8);
        readFile(iVar1, pCurrentFieldScriptActor->m120_special2dAnimationRaw, 0, 0x80);

        std::span<u8>::iterator it = pCurrentFieldScriptActor->m120_special2dAnimationRaw.beginSpan();
        pCurrentFieldScriptActor->m120_special2dAnimation->init(it);

        if (fieldExecuteVar1 == 0) {
            waitReadCompletion(0);
        }
        load2dAnimVar = 1;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
    }
    else {
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }
    breakCurrentScript = 1;
}

void OP_WAIT_LOADING_SPECIAL_2D_ANIMATION()
{
    ushort uVar1;
    int iVar2;

    iVar2 = isLoadCompleted();
    if (iVar2 == 0) {
        load2dAnimVar = 0;
        setupSpecialAnimation(actorArray[currentFieldActorId].m4_pVramSpriteSheet, pCurrentFieldScriptActor->m120_special2dAnimation);
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    }
    else {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
    breakCurrentScript = 1;
}

void OPX_4D(void)
{
    byte bVar1;
    sFieldScriptEntity* psVar2;

    psVar2 = pCurrentFieldScriptActor;
    bVar1 = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    psVar2->mEA_forcedAnimation = ~(ushort)bVar1;
    return;
}

void OP_ENABLE_MENU()
{
    g_menuDisabled = 0;
    pCurrentFieldScriptActor->mCC_scriptPC += 1;
}

void OP_FREE_SPECIAL_2D_ANIMATION()
{
    if (pCurrentFieldScriptActor->m124_special2dAnimationId != -1) {
        delete pCurrentFieldScriptActor->m120_special2dAnimation;
        pCurrentFieldScriptActor->m120_special2dAnimation = nullptr; // not in original
        pCurrentFieldScriptActor->m124_special2dAnimationId = -1;
    }
    breakCurrentScript = 1;
    ADVANCE_VM(0x1);
}

void OP_ASSIGN_GEAR()
{
    pKernelGameState->m26C_party[getImmediateOrVariableUnsigned(1)].mA0_partyData_gearNum = getImmediateOrVariableUnsigned(3);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_SET_ON_GEAR()
{
    int iVar1;

    iVar1 = getImmediateOrVariableUnsigned(1);
    if (2 < iVar1) {
        iVar1 = 2;
    }
    pKernelGameState->m22B1_isOnGear[iVar1] = 1;
    updateGearState(iVar1);
    ADVANCE_VM(0x3);
}

void OP_SET_OFF_GEAR()
{
    int iVar1;

    iVar1 = getImmediateOrVariableUnsigned(1);
    if (2 < iVar1) {
        iVar1 = 2;
    }
    pKernelGameState->m22B1_isOnGear[iVar1] = 0;
    updateGearState(iVar1);
    ADVANCE_VM(0x3);
}

void OP_DISABLE_MENU(void)
{
    g_menuDisabled = 1;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_DISABLE_COMPASS(void)
{
    g_compassDisabled = 1;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_ENABLE_PLAYER_CONTROLS()
{
    fieldRandomBattleVar = 0;
    g_menuDisabled = 0;
    g_compassDisabled = 0;
    op99Var7 = op99Var7 & 0x3fff;
    ADVANCE_VM(0x1);
}

void OPX_START_CINEMATIC(void)
{
    fieldRandomBattleVar = -1;
    g_menuDisabled = 1;
    g_compassDisabled = 1;
    op99Var7 = op99Var7 | 0xc000;
    if ((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) {
        breakCurrentScript = 1;
        ADVANCE_VM(-0x1);
    }
    else {
        ADVANCE_VM(0x1);
    }
    return;
}

void OP_OPEN_LOAD_GAME_MENU()
{
    menuIdToOpen = 2;
    breakCurrentScript = 1;
    menuOpenCount = menuOpenCount + 1;
    ADVANCE_VM(0x1);
}

void OPX_5B() {
    pCurrentFieldScriptActor->m11E = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OP_LOAD_NEW_MECHA(void)
{
    int iVar5;
    if ((loadCompleted != 0) || (iVar5 = isLoadCompleted(), iVar5 != 0)) {
        breakCurrentScript = 1;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
        return;
    }

    pCurrentFieldScriptActor->m4_flags.m_rawFlags = pCurrentFieldScriptActor->m4_flags.m_rawFlags & 0xffffdfff;
    setCurrentDirectory(4, 0);
    int mechaIndex = pCurrentFieldScriptActor->m12C_flags >> 0xd & 7;

    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1])
    {
    case 0:
        loadedMechas[mechaIndex]->m34 = 0;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        break;
    case 1:
        freeMecha(mechaIndex);
        {
            int newMechaIndex = getImmediateOrVariableUnsigned(5);

            mechaOverlayBatchLoadingTable[0].m0_fileIndex = newMechaIndex * 2 + 0x6BA;
            mechaOverlayBatchLoadingTable[0].m4_loadPtr = &mechaDataTable2_raw[mechaIndex];
            mechaDataTable2_raw[mechaIndex].resize(getFileSizeAligned(newMechaIndex * 2 + 0x6BA));

            mechaOverlayBatchLoadingTable[1].m0_fileIndex = newMechaIndex * 2 + 0x6BB;
            mechaOverlayBatchLoadingTable[1].m4_loadPtr = &mechaDataTable1_raw[mechaIndex];
            mechaDataTable1_raw[mechaIndex].resize(getFileSizeAligned(newMechaIndex * 2 + 0x6BB));

            mechaOverlayBatchLoadingTable[2].m0_fileIndex = 0;
            mechaOverlayBatchLoadingTable[2].m4_loadPtr = nullptr;

            batchStartLoadingFiles(&mechaOverlayBatchLoadingTable[0], 0);
        }
        ADVANCE_VM(0x2);
        break;
    case 2:
        if (waitReadCompletion(1) == 0) {
            mechaDataTable2[mechaIndex] = new sMechaDataTable2;
            mechaDataTable2[mechaIndex]->init(mechaDataTable2_raw[mechaIndex].getRawData().begin());
            mechaDataTable1[mechaIndex] = new sMechaDataTable1;
            mechaDataTable1[mechaIndex]->init(mechaDataTable1_raw[mechaIndex].getRawData().begin());

            mechaInitNewMecha(mechaIndex, 0, mechaDataTable2[mechaIndex], mechaDataTable1[mechaIndex], (ushort)(((mechaIndex + mechaList2[mechaIndex]) * -0x40 + 0x240) * 0x10000 >> 0x10), 0x100, 0,
                (short)((mechaIndex + 0xfc) * 0x10000 >> 0x10), &initMechaTempVar[mechaIndex]);

            mechaList3[mechaIndex] = loadedMechas[mechaIndex]->m1C_scale;

            delete mechaDataTable1[mechaIndex];
            mechaDataTable1[mechaIndex] = nullptr;
            mechaDataTable1_raw[mechaIndex].clear();

            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;

            pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0x2000;

            loadedMechas[mechaIndex]->m1C_scale = pCurrentFieldScriptActor->mF4_scale3d[0] * 5 >> 6;

            loadedMechas[mechaIndex]->m60 = pCurrentFieldScriptActor->m20_position[1].getIntegerPart();

            (*loadedMechas[mechaIndex]->m4_bones)[0].m5C_translation[0] = pCurrentFieldScriptActor->m20_position[0].getIntegerPart();
            (*loadedMechas[mechaIndex]->m4_bones)[0].m5C_translation[2] = pCurrentFieldScriptActor->m20_position[2].getIntegerPart();
        }
        else {
            ADVANCE_VM(-0x1);
        }
        break;
    default:
        assert(0);
    }

    breakCurrentScript = 1;
}

void OPX_5D()
{
    int uVar1;
    int uVar2;
    int uVar3;

    uVar1 = getImmediateOrVariableUnsigned(1);
    uVar2 = getImmediateOrVariableUnsigned(5);
    uVar3 = getImmediateOrVariableUnsigned(3);
    MissingCode();
    ADVANCE_VM(0x7);
}

void OP_SET_TRANSPARENCY_MODE()
{
    setTransparencyMode(actorArray[currentFieldActorId].m4_pVramSpriteSheet, getImmediateOrVariableUnsigned(1));
    ADVANCE_VM(0x3);
}

void OPX_5F()
{
    if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 1) != 0) {
        pCurrentFieldScriptActor->mFC[0] = getImmediateOrVariableUnsigned(2);
        pCurrentFieldScriptActor->mFC[1] = getImmediateOrVariableUnsigned(4);
        pCurrentFieldScriptActor->mFC[2] = getImmediateOrVariableUnsigned(6);
    }
    if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 2) != 0) {
        pCurrentFieldScriptActor->mFF[0] = getImmediateOrVariableUnsigned(2);
        pCurrentFieldScriptActor->mFF[1] = getImmediateOrVariableUnsigned(4);
        pCurrentFieldScriptActor->mFF[2] = getImmediateOrVariableUnsigned(6);
    }
    ADVANCE_VM(0x8);
}

void OP_SET_2D_BACKGROUND()
{
    MissingCode();
    ADVANCE_VM(9);
}

void OPX_61()
{
    MissingCode();
    ADVANCE_VM(0x1);
    breakCurrentScript = 1;
}

void OPX_62(void)
{
    MissingCode();
    ADVANCE_VM(0x5);
}

void OPX_65(void)
{
    MissingCode();
    ADVANCE_VM(0x5);
}

void OPX_66(void)
{
    MissingCode();
    ADVANCE_VM(0x9);
}

void OP_SET_LINK_OT(void)
{
    linkOTIndex = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OPX_6B(void)
{
    MissingCode();
    ADVANCE_VM(0x5);
}

void OPX_DEBUG_PRINT_FLAG_VALUE(void) {
    uint uVar1 = readU16FromScript(1);
    int uVar2 = getVariable((uint)uVar1);
    if (fieldDebugDisable == 0) {
        //sprintf_screen("DEB=%xh %d \n", uVar2, uVar2);
    }
    ADVANCE_VM(3);
}

void OPX_7F()
{
    MissingCode();
/*    if (DAT_Field__800adb88 == 0) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    }
    else {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }*/
    ADVANCE_VM(0x1);
    breakCurrentScript = 1;
}

void OPX_80(void)
{
    OPX_80Params[0] = readU16FromScript(1);
    OPX_80Params[1] = readU16FromScript(3);
    OPX_80Params[2] = readU16FromScript(5);
    if (OPX_80Params[2] == 0)
    {
        OPX_80Params[2]++;
    }
    OPX_80Params[3] = readU16FromScript(7);
    OPX_80Params[4] = 0;
    OPX_80Params[5] = readU16FromScript(9);
    OPX_80Params[6] = readU16FromScript(0xB);
    OPX_80Params[7] = readU16FromScript(0xD);

    ADVANCE_VM(0xF);
}

void OPX_81(void)
{
    OPX_81Params[0] = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    OPX_81Params[2] = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    OPX_81Params[1] = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);

    ADVANCE_VM(0x8);
}

void OPX_82(void)
{
    OPX_82Param0[0].m0_R = getImmediateOrVariableUnsigned(1);
    OPX_82Param0[0].m1_G = getImmediateOrVariableUnsigned(3);
    OPX_82Param0[0].m2_B = getImmediateOrVariableUnsigned(5);
    OPX_82Param0[1].m0_R = getImmediateOrVariableUnsigned(7);
    OPX_82Param0[1].m1_G = getImmediateOrVariableUnsigned(9);
    OPX_82Param0[1].m2_B = getImmediateOrVariableUnsigned(0xb);
    OPX_82Param0[2].m0_R = getImmediateOrVariableUnsigned(0xd);
    OPX_82Param0[2].m1_G = getImmediateOrVariableUnsigned(0xf);
    OPX_82Param0[2].m2_B = getImmediateOrVariableUnsigned(0x11);
    OPX_82Param3[0] = getImmediateOrVariableUnsigned(0x13);
    OPX_82Param3[1] = getImmediateOrVariableUnsigned(0x15);
    OPX_82Param3[2] = getImmediateOrVariableUnsigned(0x17);
    ADVANCE_VM(0x19);
    OPX_82Param4 = 1;
}

void OP_SET_BOOTMODE()
{
    if (bootModeReady != 0) {
        fieldRandomBattleVar = -1;
        bootModeReady = 0;
        newBootMode = getImmediateOrVariableUnsigned(1);
    }
    ADVANCE_VM(0x3);
}

s8 fieldBattleTransitionEffect = 0;
s32 battleStartSaveWritePending = 0;

void OPX_84()
{
    if ((((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) || (fieldChangePrevented == 0)) || (((loadCompleted != 0 || (fieldMusicLoadPending == -1)) || (load2dAnimVar != 0)))) {
        breakCurrentScript = 1;
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }
    else {
        battleTransitionEffect = fieldBattleTransitionEffect;
        requestedBattleConfig = getImmediateOrVariableUnsigned(1);
        battleInitVar1 = 0;
        playMusicAuthorized = 0;
        fieldExecuteVar2 = 0;
        MissingCode();
        s16 nextField = getImmediateOrVariableUnsigned(5);
        if (nextField != 0x7FFF)
        {
            SaveFieldAndDirections();
            setVar(2, getImmediateOrVariableUnsigned(7));
            battleStartSaveWritePending = 1;
            fieldMapNumber = nextField;
        }
        breakCurrentScript = 1;
        ADVANCE_VM(0x9);
    }
}

void OPX_85()
{
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
}

void OPX_86(void)
{
    OPX_86Param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    ADVANCE_VM(0x2);
}

void OP_WAIT_MENU()
{
    if (menuOpenCount == 0) {
        ADVANCE_VM(0x1);
    }
    else {
        ADVANCE_VM(-0x1);
    }
    breakCurrentScript = 1;
}

void OP_SETUP_MECHA_FIELD_ARGS()
{
    MissingCode();
    ADVANCE_VM(0x12);
}

void OPX_89()
{
    MissingCode();
    ADVANCE_VM(0xB);
}

void OPX_8C()
{
    MissingCode();
    ADVANCE_VM(0x7);
}

void OPX_SET_SCREEN_BOUNDS_PADDING()
{
    OPX8E_param0 = getImmediateOrVariableUnsigned(1);
    OPX8E_param1 = getImmediateOrVariableUnsigned(3);
    ADVANCE_VM(0x5);
}

s32 particleCreationCharacter = 0;
s32 particleCreationCharacterOption = 0;
s32 particleCreationCharacterOption2[2] = { 0,0 };

void OP_RESET_PARTICLE_CONFIGS()
{
    u32 uVar2 = readCharacter(1);
    if (uVar2 == 0xff) {
        uVar2 = 0;
    }
    particleCreationCharacter = uVar2;
    particleCreationCharacterOption = getImmediateOrVariableUnsigned(2);
    particleCreationCharacterOption2[0] = getImmediateOrVariableUnsigned(4);
    particleCreationCharacterOption2[1] = getImmediateOrVariableUnsigned(6);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
    resetParticleCreationSetup((short)uVar2);

    switch (particleCreationCharacterOption)
    {
    case 0:
        particleCreationCharacterOption = 0x00;
        break;
    case 1:
        particleCreationCharacterOption = 0x10;
        break;
    case 2:
        particleCreationCharacterOption = 0x20;
        break;
    case 3:
        particleCreationCharacterOption = 0x30;
        break;
    default:
        assert(0);
        break;
    }

    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
}

void resetParticleXY() {
    for (int i = 0; i < 8; i++) {
        particleCreationSetup.m0[particleCreationSlot].m30[2].vy = 0;
        particleCreationSetup.m0[particleCreationSlot].m30[2].vx = 0;
    }
}

s32 particleVar1 = 0;

void OP_SETUP_PARTICLE()
{
    particleCreationSlot = getImmediateOrVariableUnsigned(1);
    particleCreationSetup.m0[particleCreationSlot].m24 = 1;
    particleCreationSetup.m0[particleCreationSlot].m52 = particleCreationCharacter;
    particleCreationSetup.m0[particleCreationSlot].m0 = 0;
    particleVar1 = particleCreationSetup.m0[particleCreationSlot].m52;
    particleCreationSetup.m0[particleCreationSlot].m76_rotationAngle = 0;
    particleCreationSetup.m0[particleCreationSlot].m6_maxParticles = getImmediateOrVariableUnsigned(3);
    particleCreationSetup.m0[particleCreationSlot].m2_sWait = getImmediateOrVariableUnsigned(5);
    particleCreationSetup.m0[particleCreationSlot].m4_eWait = getImmediateOrVariableUnsigned(7);
    resetParticleXY();

    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    ADVANCE_VM(0x9);
}

void OP_SETUP_PARTICLE_POSITION()
{
    particleCreationSetup.m0[particleCreationSlot].mC_sPos.vx = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].mC_sPos.vy = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].mC_sPos.vz = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m14_ePos.vx = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m14_ePos.vy = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m14_ePos.vz = getVar04(0xb, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    ADVANCE_VM(0xE);
}

void OP_SETUP_LIFE1()
{
    particleCreationSetup.m0[particleCreationSlot].m8_speed = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m1C_gravity.vx = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m1C_gravity.vy = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m1C_gravity.vz = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m26_sRange = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m28_eRange = getVar04(0xb, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    ADVANCE_VM(0xE);
}

void OP_SETUP_LIFE2()
{
    particleCreationSetup.m0[particleCreationSlot].m56_psWait = getImmediateOrVariableUnsigned(1);
    particleCreationSetup.m0[particleCreationSlot].m58_peWait = getImmediateOrVariableUnsigned(3);
    particleCreationSetup.m0[particleCreationSlot].m54_shape = getImmediateOrVariableUnsigned(5);
    particleCreationSetup.m0[particleCreationSlot].m2A_flags = getImmediateOrVariableUnsigned(7) | (ushort)(getImmediateOrVariableUnsigned(9) << 1) | (ushort)particleCreationCharacterOption;
    particleCreationSetup.m0[particleCreationSlot].m72 = particleCreationCharacterOption2[0];
    particleCreationSetup.m0[particleCreationSlot].m74 = particleCreationCharacterOption2[1];
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    ADVANCE_VM(0xB);
}

void OP_SETUP_SCALE()
{
    particleCreationSetup.m0[particleCreationSlot].m5A_scale.vx = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    particleCreationSetup.m0[particleCreationSlot].m5A_scale.vy = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    particleCreationSetup.m0[particleCreationSlot].m5A_scale.vz = 0;
    particleCreationSetup.m0[particleCreationSlot].m62_scaleOffset.vx = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    particleCreationSetup.m0[particleCreationSlot].m62_scaleOffset.vy = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    particleCreationSetup.m0[particleCreationSlot].m62_scaleOffset.vz = 0;
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    ADVANCE_VM(0xA);
}

void OP_SETUP_PARTICLE_COLOR()
{
    particleCreationSetup.m0[particleCreationSlot].m6A_color[0] = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m6A_color[1] = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m6A_color[2] = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m6E_colorOffset[0] = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m6E_colorOffset[1] = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    particleCreationSetup.m0[particleCreationSlot].m6E_colorOffset[2] = getVar04(0xb, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xE;
}

void OP_CREATE_PARTICLE_EFFECT_FOR_CURRENT_ACTOR()
{
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    if (disableParticles == 0) {
        createParticleEffect(currentFieldActorId);
    }
    ADVANCE_VM(0x1);
}

void OPX_97()
{
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
    stopParticleEffect(currentFieldActorId, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]);
    ADVANCE_VM(0x2);
}

void OPX_98()
{
    MissingCode();
    ADVANCE_VM(0x3);
}

void OPX_99()
{
    MissingCode();
    ADVANCE_VM(0x3);
}

void OPX_9A()
{
    byte bVar1;
    int iVar2;
    sFieldScriptEntity* psVar3;

    iVar2 = readCharacter(2);
    if (iVar2 != 0xff) {
        psVar3 = actorArray[iVar2].m4C_scriptEntity;
        if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 1) != 0) {
            psVar3->mFC[0] = getImmediateOrVariableUnsigned(3);
            psVar3->mFC[1] = getImmediateOrVariableUnsigned(5);
            psVar3->mFC[2] = getImmediateOrVariableUnsigned(7);
        }
        if ((pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 2) != 0) {
            psVar3->mFF[0] = getImmediateOrVariableUnsigned(3);
            psVar3->mFF[1] = getImmediateOrVariableUnsigned(5);
            psVar3->mFF[2] = getImmediateOrVariableUnsigned(7);
        }
    }
    ADVANCE_VM(0x9);
}

void OP_TRANSITION_EFFECT_1()
{
    fullScreenTransitionEffectRenderParam = getImmediateOrVariableUnsigned(1);
    fullScreenTransitionEffectRenderMode = 1;
    ADVANCE_VM(0x3);
}

void OP_TRANSITION_EFFECT_2()
{
    fullScreenTransitionEffectRenderParam = getImmediateOrVariableUnsigned(1);
    fullScreenTransitionEffectRenderMode = 2;
    ADVANCE_VM(0x3);
}

void OP_TRANSITION_EFFECT_3()
{
    fullScreenTransitionEffectRenderParam = getImmediateOrVariableUnsigned(1);
    fullScreenTransitionEffectRenderMode = 3;
    ADVANCE_VM(0x3);
}

void OP_TOGGLE_FRAME_LOCK()
{
    int uVar1;
    uint uVar2;

    uVar1 = getImmediateOrVariableUnsigned(2);
    uVar2 = getCharacter(uVar1);
    if (uVar2 != 0xff) {
        if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0) {
            pKernelGameState->m2318_partyFrameLock = pKernelGameState->m2318_partyFrameLock | (ushort)(1 << (uVar2 & 0x1f));
        }
        else {
            pKernelGameState->m2318_partyFrameLock = pKernelGameState->m2318_partyFrameLock & ~(ushort)(1 << (uVar2 & 0x1f));
        }
    }
    ADVANCE_VM(0x4);
}

void OP_RESTORE_GEAR()
{
    for (int i = 0; i < 20; i++)
    {
        pKernelGameState->m978_gears[i].m38_fuel = pKernelGameState->m978_gears[i].m3C_maxFuel;
        pKernelGameState->m978_gears[i].m10_ether = pKernelGameState->m978_gears[i].m12_maxEther;
    }
    ADVANCE_VM(0x1);
}

void OP_GET_CURRENT_DISC_NUMBER()
{
    setVar(readU16FromScript(1), getCurrentDiscNumber());
    ADVANCE_VM(0x3);
}

void OP_SET_MAX_MECHA() {
    numMaxMechaOverlayEntries = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OP_SET_PAUSE_DISABLED(void)
{
    pauseDisabled = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OP_SETUP_PARTICLE_ANGLE()
{
    particleCreationSetup.m0[particleCreationSlot].m24 = getImmediateOrVariableUnsigned(1);
    particleCreationSetup.m0[particleCreationSlot].m2A_flags |= getImmediateOrVariableUnsigned(3) << 8;
    particleCreationSetup.m0[particleCreationSlot].m76_rotationAngle = getImmediateOrVariableUnsigned(5);
    fieldExectuteMaxCycles += 4;
    ADVANCE_VM(0x7);
}

void OPX_AF()
{
    MissingCode();
    ADVANCE_VM(0x12);
}

void OPX_B5()
{
    MissingCode();
    ADVANCE_VM(0x1);
}

void OPX_B8()
{
    MissingCode();
    ADVANCE_VM(0x4);
}

void OPX_BD()
{
    sFieldScriptEntity* psVar1;
    int iVar2;
    ushort uVar3;

    uVar3 = 0;
    iVar2 = getImmediateOrVariableUnsigned(1);
    psVar1 = pCurrentFieldScriptActor;
    if (iVar2 == 1) {
        uVar3 = 0x80;
    }
    else if (iVar2 == 2) {
        uVar3 = 0x40;
    }
    particleCreationSetup.m0[particleCreationSlot].m2A_flags = particleCreationSetup.m0[particleCreationSlot].m2A_flags | uVar3;
    fieldExectuteMaxCycles += 4;
    ADVANCE_VM(0x7);
}

void OPX_C8Sub(long slot)
{
    particleCreationSetup.m0[particleCreationSlot].m30[slot].vx = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot].vy = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 1].vx = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 1].vy = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 2].vx = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 2].vy = getVar04(0xb, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 3].vx = getVar02(0xd, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    particleCreationSetup.m0[particleCreationSlot].m30[slot + 3].vy = getVar01(0xf, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    fieldExectuteMaxCycles += 4;
    ADVANCE_VM(0x12);
}

void OPX_C8()
{
    OPX_C8Sub(0);
}

void OPX_CB()
{
    breakCurrentScript = 1;
    MissingCode();
    ADVANCE_VM(0x1);
}

void OPX_CC(void)
{
    ushort uVar1;

    if (onlyUpdateDirector == 0) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    }
    else {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
    breakCurrentScript = 1;
    return;
}

void OPX_A2(void)
{
    ushort uVar1;

    if (fieldMusicLoadPending == -1) {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC - 1;
    }
    else {
        uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
    breakCurrentScript = 1;
}

void OPX_13()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    byte bVar3;
    short sVar4;

    sVar4 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m10A = sVar4;
    psVar2->m10D = 0;
    bVar3 = getImmediateOrVariableUnsigned(3);
    pCurrentFieldScriptActor->m10C = bVar3;
    iVar1 = currentFieldActorId;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    OPX_13Sub(iVar1);
    if (pCurrentFieldScriptActor->m10A == 0) {
        pCurrentFieldScriptActor->m10D = 0xff;
    }
}

void OP_SETUP_SCREEN_DISTORTION()
{
    setupScreenDistortion(0);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xf;
}

void OP_SCREEN_DISTORTION_FADE_OUT()
{
    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1])
    {
    case 0:
        setScreenDistortionParams(0, 0, 0, 0, 0, 0, getImmediateOrVariableUnsigned(2));
        screenDistortionFadeOut = 1;
        ADVANCE_VM(0x4);
        break;
    case 1:
        if (screenDistortionRunning == 0) {
            ADVANCE_VM(0x2);
        }
        else
        {
            ADVANCE_VM(-0x1);
        }
        break;
    case 2:
        screenDistortionRunning = 0;
        ADVANCE_VM(0x2);
        break;
    case 3:
        freeScreenDistortion();
        ADVANCE_VM(0x2);
        break;
    default:
        assert(0);
    }

    breakCurrentScript = 1;
}

void OP_SET_PARTY_FRAME_MASK()
{
    int uVar1;
    uint uVar2;

    uVar1 = getImmediateOrVariableUnsigned(1);
    uVar2 = getCharacter(uVar1);
    if (uVar2 != 0xff) {
        pKernelGameState->m1D32_partyFrameMask = pKernelGameState->m1D32_partyFrameMask | (ushort)(1 << (uVar2 & 0x1f));
    }
    ADVANCE_VM(0x3);
}

void OP_CLEAR_PARTY_FRAME_MASK()
{
    int uVar1;
    uint uVar2;

    uVar1 = getImmediateOrVariableUnsigned(1);
    uVar2 = getCharacter(uVar1);
    if (uVar2 != 0xff) {
        pKernelGameState->m1D32_partyFrameMask = pKernelGameState->m1D32_partyFrameMask & ~(1 << (uVar2 & 0x1f));
    }
    ADVANCE_VM(0x3);
}

void OP_MECHA_PLAY_ANIMATION()
{
    if (fieldExecuteVar1 != 0) {
        int mechaId = getImmediateOrVariableUnsigned(1);
        int animationId = getImmediateOrVariableUnsigned(3);
        mechaPlayAnimation(mechaId, 0, animationId);
        mechaCurrentAnimation[mechaId] = animationId;
    }
    ADVANCE_VM(0x5);
}

void OPX_3D()
{
    sFieldScriptEntity* psVar1;
    short sVar2;
    int iVar3;

    iVar3 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    sVar2 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 9]);
    psVar1 = pCurrentFieldScriptActor;
    mechaFieldArgs2[0].m[iVar3][0] = sVar2;
    sVar2 = getVar20(5, pCurrentFieldScriptFile[psVar1->mCC_scriptPC + 9]);
    psVar1 = pCurrentFieldScriptActor;
    mechaFieldArgs2[0].m[iVar3][1] = sVar2;
    sVar2 = getVar10(7, pCurrentFieldScriptFile[psVar1->mCC_scriptPC + 9]);
    mechaFieldArgs2[0].m[iVar3][2] = sVar2;
    ADVANCE_VM(0xA);
}

void OPX_3E()
{
    MissingCode();
    ADVANCE_VM(0xA);
}

void OPX_3F()
{
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
}

void OPX_40()
{
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
}

void OPX_1C(void)
{
    uint* puVar1;
    ushort uVar2;
    byte* pbVar3;
    sFieldScriptEntity* psVar4;
    int iVar5;
    int iVar6;
    sSpriteActor* psVar7;

    psVar4 = pCurrentFieldScriptActor;
    psVar7 = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
    puVar1 = &pCurrentFieldScriptActor->m4_flags.m_rawFlags;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags | 0x10000;
    pbVar3 = &pCurrentFieldScriptFile[0];
    psVar4->m4_flags.m_rawFlags = *puVar1 | 0x200000;
    iVar5 = getVar80(1, (uint)pbVar3[(uint)psVar4->mCC_scriptPC + 7]);
    pbVar3 = &pCurrentFieldScriptFile[0];
    uVar2 = pCurrentFieldScriptActor->mCC_scriptPC;
    (pCurrentFieldScriptActor->m20_position).vx = iVar5 << 0x10;
    iVar5 = getVar40(3, (uint)pbVar3[(uint)uVar2 + 7]);
    pbVar3 = &pCurrentFieldScriptFile[0];
    uVar2 = pCurrentFieldScriptActor->mCC_scriptPC;
    (pCurrentFieldScriptActor->m20_position).vz = iVar5 << 0x10;
    iVar6 = getVar20(5, (uint)pbVar3[(uint)uVar2 + 7]);
    psVar4 = pCurrentFieldScriptActor;
    iVar5 = currentFieldActorId;
    (pCurrentFieldScriptActor->m20_position).vy = iVar6 << 0x10;
    actorArray[iVar5].mC_matrix.t[0] = (psVar4->m20_position.vx >> 0x10);
    actorArray[iVar5].mC_matrix.t[1] = (psVar4->m20_position.vy >> 0x10);
    actorArray[iVar5].mC_matrix.t[2] = (psVar4->m20_position.vz >> 0x10);
    psVar7->m0_position.vx = (psVar4->m20_position).vx;
    psVar7->m0_position.vy = (psVar4->m20_position).vy;
    psVar7->m0_position.vz = (psVar4->m20_position).vz;
    psVar4->mCC_scriptPC = psVar4->mCC_scriptPC + 8;
    return;
}

void OP_SWITCH_MAP_TO_GEARS(void)
{
    fieldMapNumber = fieldMapNumber | 0xc000;
    waitForReadFinished();
    startLoadingPlayableCharacters();
    finalizeLoadPlayableCharacters();
    pcInitVar1 = (uint)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    return;
}

short OPX_21_Sub0 = 0;
void OPX_21()
{
    sFieldScriptEntity* psVar1;
    int uVar2;
    int uVar3;
    int iVar2;
    sFieldEntity* psVar3;

    psVar3 = &actorArray[currentFieldActorId];
    uVar2 = getImmediateOrVariableUnsigned(1);
    uVar3 = getCharacter(uVar2);
    iVar2 = findCharacterInParty(uVar3);
    psVar3->m58_flags = psVar3->m58_flags & 0xf07f | 0x200;
    if (iVar2 == -1) {
        resetFieldScriptEntityGraphicEntity(currentFieldActorId, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
        psVar1 = pCurrentFieldScriptActor;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m0_updateScriptDisabled = 1;
        currentScriptFinished = 1;
        breakCurrentScript = 1;
        psVar1->m4_flags.m_rawFlags = psVar1->m4_flags.m_rawFlags | 0x100000;
    }
    else {
        resetFieldScriptEntityGraphicEntity(currentFieldActorId, iVar2, &partyCharacterBuffers[iVar2], 2, 0, iVar2, 1);
        OPX_21_Sub0 = -0xc0;
        actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
        OP_INIT_ENTITY_SCRIPT_sub1();
        iVar2 = currentFieldActorId;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x80;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x100;
        actorArray[iVar2].m58_flags &= ~0x20;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OPX_23() {
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0x14;
}

void OP_MAKE_VISIBLE_ACTIVE()
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
    psVar1->mE8_currentAnimationId = 0xff;
    psVar1->m4_flags.m_rawFlags = psVar1->m4_flags.m_rawFlags & 0xfdffffff;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
}

void OP_MAKE_INVISIBLE_ACTIVE(void)
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags | 0x20;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
    return;
}

void OP_MAKE_VISIBLE_BY_ID(void)
{
    int iVar1;
    sFieldEntity* psVar2;

    iVar1 = readCharacter(1);
    if (iVar1 != 0xff) {
        iVar1 = readCharacter(1);
        psVar2 = &actorArray[iVar1];
        if ((psVar2->m4C_scriptEntity->m4_flags.m_rawFlags & 0x100000) == 0) {
            psVar2->m58_flags = psVar2->m58_flags & 0xffdf;
            psVar2->m4C_scriptEntity->m4_flags.m_rawFlags = psVar2->m4C_scriptEntity->m4_flags.m_rawFlags & 0xfdffffff;
        }
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    return;
}

void OP_MAKE_INVISIBLE_BY_ID()
{
    int iVar1;

    iVar1 = readCharacter(1);
    if (iVar1 != 0xff) {
        iVar1 = readCharacter(1);
        actorArray[iVar1].m58_flags = actorArray[iVar1].m58_flags | 0x20;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OP_START_FADE_IN()
{
    resetRGBFaderToBlack(0);
    setupRGBFaderSlot0_fadeIn(getImmediateOrVariableUnsigned(1));
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_START_FADE_TO_BLACK()
{
    setupRGBCalcSlot0_fadeToBlack(getImmediateOrVariableUnsigned(1));
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_B6Sub(int param_1, int param_2)
{
    if (param_2 == 0) {
        OP_B6SubVar0 = 0;
        op9DVar1 = op9DVar1 + 2;
        sceneSCRZ = param_1;
    }
    else {
        OP_B6Var1 = ((sceneSCRZ - param_1) * -0x10000) / param_2;
        OP_B6Var2 = sceneSCRZ << 0x10;
        OP_B6SubVar0 = param_2;
        op99Var7 = op99Var7 | 0x10;
    }
    op99Var7 = op99Var7 & 0xdfff;
    return;
}

std::array<u16, 64> OP_B5SubTable = { {
   0x0, 0x1, 0x2, 0x3,
   0x4, 0xFFFD, 0xFFFE, 0xFFFF,
0xFFFF, 0x0, 0x1, 0x2,
   0x3, 0x4, 0xFFFD, 0xFFFE,
0xFFFE, 0xFFFF, 0x0, 0x1,
   0x2, 0x3, 0x4, 0xFFFD,
0xFFFD, 0xFFFE, 0xFFFF, 0x0,
   0x1, 0x2, 0x3, 0x4,
   0x4, 0xFFFD, 0xFFFE, 0xFFFF,
   0x0, 0x1, 0x2, 0x3,
   0x3, 0x4, 0xFFFD, 0xFFFE,
0xFFFF, 0x0, 0x1, 0x2,
   0x2, 0x3, 0x4, 0xFFFD,
0xFFFE, 0xFFFF, 0x0, 0x1,
   0x1, 0x2, 0x3, 0x4,
0xFFFD, 0xFFFE, 0xFFFF, 0x0,
} };


void OP_B5Sub(int param_1, int param_2)
{
    uint uVar1;

    uVar1 = getCameraDirection();
    if (param_2 == 0) {
        param_2 = 1;
        op9DVar1 = op9DVar1 + 2;
    }
    cameraDollyVar1 = ((int)OP_B5SubTable[(uVar1 & 0xffff) * 8 + param_1] << 0x19) / param_2;
    cameraDollyVar0 = (short)param_2;
    opA0_var0 = (param_1 + 4U & 7) << 9;
    return;
}

void OP_B5()
{
    int iVar1;
    int uVar2;
    uint uVar3;

    iVar1 = getImmediateOrVariableUnsigned(1);
    uVar2 = getImmediateOrVariableUnsigned(3);
    if (fieldExecuteVar1 == 0) {
        uVar3 = iVar1 + 4U & 7;
        cameraRotationBetweenEyeAndAt.vy = (short)(uVar3 << 9);
        opA0_var0 = uVar3 << 9;
    }
    else {
        if (cameraDollyVar0 != 0) {
            breakCurrentScript = 1;
            return;
        }
        OP_B5Sub(iVar1, uVar2);
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
    breakCurrentScript = 1;
}

void OP_B6()
{
    int uVar1;
    int uVar2;

    uVar1 = getImmediateOrVariableUnsigned(1);
    uVar2 = getImmediateOrVariableUnsigned(3);
    OP_B6Sub(uVar1, uVar2);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_B7()
{
    op99Var7 |= 0x4000;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_ENABLE_CAMERA_HEIGHT_CHECK()
{
    op99Var7 &= ~0x4000;
    ADVANCE_VM(1);
}

void OP_IF_CHARACTER_IN_AVAILABLE_PARTY()
{
    if ((pKernelGameState->m1D30_partyMemberBitField >> (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x1f) & 1U) == 0) {
        pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);
    }
    else {
        pCurrentFieldScriptActor->mCC_scriptPC += 4;
    }
}

void OP_ADD_TO_AVAILABLE_PARTY()
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    pKernelGameState->m1D30_partyMemberBitField = pKernelGameState->m1D30_partyMemberBitField | (ushort)(1 << (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x1f));
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 2;
}

void OP_REMOVE_FROM_AVAILABLE_PARTY()
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    pKernelGameState->m1D30_partyMemberBitField = pKernelGameState->m1D30_partyMemberBitField & ~(ushort)(1 << (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x1f));
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 2;
}

void OP_INIT_ENTITY_SCRIPT()
{
    resetFieldScriptEntityGraphicEntity(currentFieldActorId, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 1);
    OP_INIT_ENTITY_SCRIPT_sub1();

    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x100;
    pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0x800;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_INCREASE_FIELD_EXECUTION_MAX_CYCLES()
{
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 0x20;
    ADVANCE_VM(0x1);
}

void OP_C8()
{
    MissingCode();
    ADVANCE_VM(0x3);
    breakCurrentScript = 1;
    return;
}

void OP_IF_PLAYER_IN_TRIGGER2()
{
    sFieldScriptEntity* psVar1;
    ushort uVar2;
    long lVar3;
    uint uVar4;

    sGTE_XY caracterPosition(
        actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vz >> 16,
        actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vx >> 16
    );

    sFieldTrigger* psVar5 = &fieldTriggerData[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]];

    std::array<sGTE_XY, 4> triggerCorners;
    triggerCorners[0].set(psVar5->m0[2], psVar5->m0[0]);
    triggerCorners[1].set(psVar5->m6[2], psVar5->m6[0]);
    triggerCorners[2].set(psVar5->mC[2], psVar5->mC[0]);
    triggerCorners[3].set(psVar5->m12[2], psVar5->m12[0]);


    if ((NormalClip(triggerCorners[0], triggerCorners[1], caracterPosition) < 0) ||
        (NormalClip(triggerCorners[1], triggerCorners[2], caracterPosition) < 0) ||
        (NormalClip(triggerCorners[2], triggerCorners[3], caracterPosition) < 0) ||
        (NormalClip(triggerCorners[3], triggerCorners[0], caracterPosition) < 0))
    {
        fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
        pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);
    }
    else
    {
        ADVANCE_VM(0x4);
    }

}

void OP_IF_PLAYER_IN_TRIGGER()
{
    sFieldScriptEntity* psVar1;
    ushort uVar2;
    long lVar3;
    uint uVar4;

    sGTE_XY playerPosition(actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vz.getIntegerPart(), actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vx.getIntegerPart());

    sFieldTrigger* psVar5 = &fieldTriggerData[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]];

    int playerY = actorArray[g_playerControlledActor].m4C_scriptEntity->m20_position.vy.getIntegerPart();

    if ((psVar5->m0[1] < playerY) && ((int)(playerY - (uint)(ushort)actorArray[g_playerControlledActor].m4C_scriptEntity->m18_boundingVolume.vy) < (int)psVar5->m0[1]))
    {
        std::array<sGTE_XY, 4> triggerCorners;
        triggerCorners[0].set(psVar5->m0[2], psVar5->m0[0]);
        triggerCorners[1].set(psVar5->m6[2], psVar5->m6[0]);
        triggerCorners[2].set(psVar5->mC[2], psVar5->mC[0]);
        triggerCorners[3].set(psVar5->m12[2], psVar5->m12[0]);

        if ((NormalClip(triggerCorners[0], triggerCorners[1], playerPosition) > -1) &&
            (NormalClip(triggerCorners[1], triggerCorners[2], playerPosition) > -1) &&
            (NormalClip(triggerCorners[2], triggerCorners[3], playerPosition) > -1) &&
            (NormalClip(triggerCorners[3], triggerCorners[0], playerPosition) > -1))
        {
            ADVANCE_VM(0x4);
            return;
        }
    }
    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(2);

}

void OP_SET_BACKGROUND_CLEAR_COLOR()
{
    fieldBackgroundClearColor[0] = getImmediateOrVariableUnsigned(1);
    fieldBackgroundClearColor[1] = getImmediateOrVariableUnsigned(3);
    fieldBackgroundClearColor[2] = getImmediateOrVariableUnsigned(5);
    ADVANCE_VM(0x7);
}

void computeOrbit(FP_VEC3* param_1, FP_VEC3* param_2, s16 orbitRotation)
{
    MATRIX MStack88;
    VECTOR local_38;
    VECTOR local_28;
    SVECTOR local_18;

    local_18.vx = 0;
    local_18.vz = 0;
    local_18.vy = orbitRotation;
    PushMatrix();
    createRotationMatrix(&local_18, &MStack88);
    local_38.vx = param_2->vx - param_1->vx;
    local_38.vy = param_2->vy - param_1->vy;
    local_38.vz = param_2->vz - param_1->vz;
    ApplyMatrixLV(&MStack88, &local_38, &local_28);
    param_1->vx = local_28.vx + param_2->vx;
    param_1->vz = local_28.vz + param_2->vz;
    PopMatrix();
    return;
}

const std::array<FP_VEC3*, 4> cameraMapping = {
    {
        &cameraTargetOverride,
        &desiredCameraTarget,
        &cameraPositionOverride,
        &desiredCameraPosition
    }
};

void OP_EB()
{
    ushort uVar1;
    int iVar2;
    int iVar3;
    uint uVar5;
    FP_VEC3 local_38;
    FP_VEC3 local_28;
    int uVar4;

    iVar2 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    local_38.vx = iVar2 << 0x10;
    iVar2 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    local_38.vz = iVar2 << 0x10;
    iVar2 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    local_38.vy = iVar2 << 0x10;
    uVar4 = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    iVar2 = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    iVar3 = getVar04(0xb, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0xd]);
    uVar5 = (iVar2 * 0xb60 >> 8) + 0xc00;
    iVar2 = getAngleSin(uVar5);
    local_28.vy = (iVar2 * iVar3 * -0x20 >> 0x10) * (int)op99Var4 * 0x10 + local_38.vy;
    iVar2 = getAngleCos(uVar5);
    local_28.vx = local_38.vx;
    local_28.vz = (iVar2 * iVar3 * 0x20 >> 0x10) * (int)op99Var4 * 0x10 + local_38.vz;
    computeOrbit(&local_28, &local_38, uVar4);
    uVar1 = readU16FromScript(0xe);
    setVar((uint)uVar1, local_28.vx);
    uVar1 = readU16FromScript(0x10);
    setVar((uint)uVar1, local_28.vz);
    uVar1 = readU16FromScript(0x12);
    setVar((uint)uVar1, local_28.vy);

    fieldExectuteMaxCycles++;
    ADVANCE_VM(0x14);
}

void OP_ORBIT(void)
{
    FP_VEC3 vec = *cameraMapping[pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]];

    int orbitRotation = getVar80(2, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]);
    int param2 = getVar40(4, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]);
    int param3 = getVar20(6, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 8]);

    int angle = (param2 * 0xb60 >> 8) + 0xc00;

    FP_VEC3 vec2;
    vec2.vy = (getAngleSin(angle) * param3 * -0x20 >> 0x10) * (int)op99Var4 * 0x10 + vec.vy;
    vec2.vx = vec.vx;
    vec2.vz = (getAngleCos(angle) * param3 * 0x20 >> 0x10) * (int)op99Var4 * 0x10 + vec.vz;

    computeOrbit(&vec2, &vec, orbitRotation);
    setVar(readU16FromScript(9), vec2.vx.getIntegerPart());
    setVar(readU16FromScript(0xb), vec2.vz.getIntegerPart());
    setVar(readU16FromScript(0xd), vec2.vy.getIntegerPart());

    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0xF);
    return;
}

void OP_EE()
{
    int sourceIndex = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    int destIndex = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2];

    *cameraMapping[destIndex] = *cameraMapping[sourceIndex];

    fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
    ADVANCE_VM(0x3);
}

void OP_WAIT_CAMERA()
{
    int uVar1 = getImmediateOrVariableUnsigned(1);
    int uVar2 = 3;

    if (cameraInterpolationTargetNumSteps == 0) {
        uVar2 = 2;
    }
    if (cameraInterpolationPositionNumSteps == 0) {
        uVar2 = uVar2 & 1;
    }
    breakCurrentScript = 1;
    if ((uVar2 & uVar1) == 0) {
        ADVANCE_VM(0x3);;
    }
}

void OP_SETUP_MOVEMENT_BOUNDING_ZONE()
{
    if ((pCurrentFieldScriptActor->m12C_flags & 0x1000) == 0) {
        pCurrentFieldScriptActor->m114_movementBoundingZone.resize(4);
    }
    pCurrentFieldScriptActor->m12C_flags |= 0x1000;

    pCurrentFieldScriptActor->m114_movementBoundingZone[0].vx = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[0].vy = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[1].vx = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[1].vy = getVar10(7, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[2].vx = getVar08(9, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[2].vy = getVar04(0xB, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[3].vx = getVar02(0xD, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    pCurrentFieldScriptActor->m114_movementBoundingZone[3].vy = getVar01(0xF, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
    ADVANCE_VM(0x12);
}

void OP_SET_VAR_FROM_S16(void)
{
    ushort uVar1;
    int uVar2;

    uVar1 = readU16FromScript(1);
    uVar2 = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    setVar((uint)uVar1, uVar2);
    ADVANCE_VM(0x6);
    return;
}

void OP_SET_VAR_ADD(void)
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    uVar1 = readU16FromScript(1);
    iVar2 = getVariable(uVar1);
    iVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    uVar1 = readU16FromScript(1);
    setVar(uVar1, iVar2 + iVar3);
    ADVANCE_VM(0x6);
    return;
}

void OP_SET_VAR_SUBSTRACT()
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    uVar1 = readU16FromScript(1);
    iVar2 = getVariable(uVar1);
    iVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    uVar1 = readU16FromScript(1);
    setVar(uVar1, iVar2 - iVar3);
    ADVANCE_VM(0x6);
}

void OP_SET_VARBIT_TRUE()
{
    ushort uVar1;
    ushort uVar2;
    uint uVar3;

    uVar1 = readU16FromScript(1);
    uVar1 = getVariable(uVar1);
    uVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    uVar2 = readU16FromScript(1);
    setVar((uint)uVar2, uVar1 | (ushort)(1 << (uVar3 & 0x1f)));
    ADVANCE_VM(0x6);
}

void OP_SET_VARBIT_FALSE()
{
    ushort uVar1;
    ushort uVar2;
    uint uVar3;

    uVar1 = readU16FromScript(1);
    uVar1 = getVariable(uVar1);
    uVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    uVar2 = readU16FromScript(1);
    setVar((uint)uVar2, uVar1 & ~(ushort)(1 << (uVar3 & 0x1f)));
    ADVANCE_VM(0x6);
}

void OP_SET_VAR_INC(void)
{
    setVar(readU16FromScript(1), getVariable(readU16FromScript(1)) + 1);
    ADVANCE_VM(0x3);
    return;
}

void OP_SET_VAR_DEC(void)
{
    setVar(readU16FromScript(1), getVariable(readU16FromScript(1)) - 1);
    ADVANCE_VM(0x3);
    return;
}

void OP_SET_VAR_RANDOM()
{
    setVar(readU16FromScript(1), xenoRand());
    ADVANCE_VM(0x3);
}

void OP_SET_VAR_FALSE(void)
{
    ushort uVar1;

    uVar1 = readU16FromScript(1);
    setVar(uVar1, 0);
    ADVANCE_VM(0x3);
    return;
}

void OP_SET_VAR_TRUE(void)
{
    ushort uVar1;

    uVar1 = readU16FromScript(1);
    setVar(uVar1, 1);
    ADVANCE_VM(0x3);
    return;
}

void OP_JUMP_IF_PAD_MASK()
{
    jumpIfMask(padButtonForScripts[0].m0_buttons);
}

void OP_SET_CURRENT_ACTOR_FLAGS()
{
    ushort* puVar1;
    sFieldScriptEntity* psVar2;
    uint uVar3;

    uVar3 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    sFieldScriptEntity_flags0 newFlags;
    newFlags.m7 = uVar3 & 1;
    if ((uVar3 & 4) != 0) {
        newFlags.m_rawFlags |= 0x20;
    }
    if ((uVar3 & 8) != 0) {
        newFlags.m_rawFlags |= 0x10;
    }
    if ((uVar3 & 0x10) != 0) {
        newFlags.m_rawFlags |= 8;
    }
    if ((uVar3 & 0x20) != 0) {
        newFlags.m_rawFlags |= 4;
    }
    if ((uVar3 & 0x40) != 0) {
        newFlags.m_rawFlags |= 0x8000000;
    }
    puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x80000BC;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= newFlags.m_rawFlags;
    psVar2->mCC_scriptPC = *puVar1 + 3;
}

void OP_WAIT()
{
    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m2_delay == 0) {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m2_delay = getImmediateOrVariableUnsigned(1);
    }
    else {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m2_delay--;
    }
    if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m2_delay == 0) {
        ADVANCE_VM(0x3);
    }
    breakCurrentScript = 1;
}

void OP_RESET_CHARACTER()
{
    int iVar1;
    sFieldScriptEntity* psVar3;

    iVar1 = readCharacter(1);
    if (iVar1 != 0xff) {
        iVar1 = readCharacter(1);
        psVar3 = actorArray[iVar1].m4C_scriptEntity;
        psVar3->m30_stepVector.vx = 0;
        psVar3->m30_stepVector.vy = 0;
        psVar3->m30_stepVector.vz = 0;
        psVar3->m40.vx = 0;
        psVar3->m40.vy = 0;
        psVar3->m40.vz = 0;
        psVar3->m0_fieldScriptFlags.m0_updateScriptDisabled = 1;
        psVar3->m104_rotation = psVar3->m106_currentRotation = psVar3->m104_rotation | 0x8000;

        int dialogWindow;
        if (findDialogWindowForCurrentActor(&dialogWindow) == 0) {
            gDialogWindows[dialogWindow].m414 = 0;
        }
    }
    ADVANCE_VM(0x2);
}

void OP_CD()
{
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x800000;
    ADVANCE_VM(0x1);
}

void OP_CE()
{
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x800000;
    ADVANCE_VM(0x1);
}

void OP_CF()
{
    pCurrentFieldScriptActor->m88_dialogWindowPosition[0] = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] * 2;
    pCurrentFieldScriptActor->m88_dialogWindowPosition[1] = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2];
    pCurrentFieldScriptActor->m82_dialogWindowSize[0] = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] * 3;
    pCurrentFieldScriptActor->m82_dialogWindowSize[1] = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 4];
    ADVANCE_VM(0x5);
}

void OP_SET_DIALOG_WINDOW_PARAM()
{
    pCurrentFieldScriptActor->m88_dialogWindowPosition[0] = getImmediateOrVariableUnsigned(1);
    pCurrentFieldScriptActor->m88_dialogWindowPosition[1] = getImmediateOrVariableUnsigned(3);
    pCurrentFieldScriptActor->m82_dialogWindowSize[0] = getImmediateOrVariableUnsigned(5) * 3;
    pCurrentFieldScriptActor->m82_dialogWindowSize[1] = getImmediateOrVariableUnsigned(7);
    pCurrentFieldScriptActor->m84_dialogWindowFlags = getImmediateOrVariableUnsigned(9);
    ADVANCE_VM(0xB);
}

void OP_SHOW_DIALOG_WINDOW_MODE0()
{
    showDialogWindowForActor(currentFieldActorId, 0);
}

void OP_SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0()
{
    if (readCharacter(1) == 0xff) {
        pCurrentFieldScriptActor->mCC_scriptPC += 6;
    }
    else {
        int characterId = readCharacter(1);
        pCurrentFieldScriptActor->mCC_scriptPC++;
        if (showDialogWindowForActor(characterId, 0) != -1) {
            return;
        }
        pCurrentFieldScriptActor->mCC_scriptPC--;
    }
}

void OP_D6()
{
    short uVar1;

    dialogWindowFlag2 = getImmediateOrVariableUnsigned(1);
    if (dialogWindowFlag2 == 1) {
        uVar1 = 6;
    }
    else {
        if (dialogWindowFlag2 < 2) {
            uVar1 = 8;
            if (dialogWindowFlag2 != 0) goto LAB_Field__80092600;
        }
        else {
            uVar1 = 4;
            if (dialogWindowFlag2 != 2) goto LAB_Field__80092600;
        }
    }
    dialogWindowOpenAnimationNumFrames = uVar1;
LAB_Field__80092600:
    ADVANCE_VM(0x3);
}

void OP_SET_OBJECT_SWIVEL_ON_X_AXIS(void)
{
    pCurrentFieldScriptActor->m12C_flags &= ~3;
    pCurrentFieldScriptActor->m12C_flags |= 1;
    pCurrentFieldScriptActor->m70_rotationForRendering = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OP_SET_OBJECT_SWIVEL_ON_Z_AXIS(void)
{
    pCurrentFieldScriptActor->m12C_flags &= ~3;
    pCurrentFieldScriptActor->m12C_flags |= 3;
    pCurrentFieldScriptActor->m70_rotationForRendering = getImmediateOrVariableUnsigned(1);
    ADVANCE_VM(0x3);
}

void OP_DA() {
    MissingCode();
    ADVANCE_VM(0x11);
}

void OP_DB()
{
    int iVar1;
    int iVar2;

    iVar1 = getImmediateOrVariableUnsigned(1);
    iVar2 = getImmediateOrVariableUnsigned(3);
    if (0xfff < iVar2) {
        iVar2 = 0xfff;
    }
    if ((actorArray[currentFieldActorId].m58_flags & 0x2000) != 0) {
        pCurrentFieldScriptActor->m118[iVar1] = iVar2;
    }
    ADVANCE_VM(0x5);
}

void OP_DC(void)
{
    ushort uVar1;
    short sVar2;
    short sVar3;

    uVar1 = readU16FromScript(1);
    sVar2 = getVariable(uVar1);
    uVar1 = readU16FromScript(3);
    sVar3 = getVariable(uVar1);
    uVar1 = readU16FromScript(3);
    setVar((uint)uVar1, sVar2);
    uVar1 = readU16FromScript(1);
    setVar((uint)uVar1, sVar3);
    ADVANCE_VM(0x5);
    return;
}

void OP_DE()
{
    ushort uVar1;
    short sVar2;
    short sVar3;

    uVar1 = readU16FromScript(1);
    sVar2 = getVariable(uVar1);
    sVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    uVar1 = readU16FromScript(1);
    setVar((uint)uVar1, sVar2 * sVar3);
    ADVANCE_VM(0x6);
}

void OP_DF(void)
{
    ushort uVar1;
    int iVar2;
    int iVar3;

    uVar1 = readU16FromScript(1);
    iVar2 = getVariable(uVar1);
    iVar3 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    if (iVar3 == 0) {
        iVar3 = 1;
    }
    uVar1 = readU16FromScript(1);
    setVar((uint)uVar1, (short)(iVar2 / iVar3));
    ADVANCE_VM(0x6);
    return;
}

void OP_SET_CAMERA_LIMITS(void)
{
    cameraLimits[0] = readS16FromScript(1);
    cameraLimits[1] = readS16FromScript(3);
    cameraLimits[2] = readS16FromScript(5);
    cameraLimits[3] = -readS16FromScript(7);
    ADVANCE_VM(0x9);
}

void OP_ROTATION_3D_YAW_SUBSTRACT()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    short sVar3;

    sVar3 = getImmediateOrVariableUnsigned(1);
    iVar1 = currentFieldActorId;
    actorArray[currentFieldActorId].m50_modelRotation.vx = actorArray[currentFieldActorId].m50_modelRotation.vx - sVar3;
    ADVANCE_VM(0x3);
    updateScriptActor3dRotation(iVar1);
}

void OP_ROTATION_3D_PITCH_ADD()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    short sVar3;

    sVar3 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    iVar1 = currentFieldActorId;
    actorArray[currentFieldActorId].m50_modelRotation.vy = actorArray[currentFieldActorId].m50_modelRotation.vy + sVar3;
    ADVANCE_VM(0x3);
    updateScriptActor3dRotation(iVar1);
}

void OP_ROTATION_3D_PITCH_SUBSTRACT()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    short sVar3;

    sVar3 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    iVar1 = currentFieldActorId;
    actorArray[currentFieldActorId].m50_modelRotation.vy = actorArray[currentFieldActorId].m50_modelRotation.vy - sVar3;
    ADVANCE_VM(0x3);
    updateScriptActor3dRotation(iVar1);
}

void OP_ROTATION_3D_ROLL_ADD()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    short sVar3;

    sVar3 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    iVar1 = currentFieldActorId;
    actorArray[currentFieldActorId].m50_modelRotation.vz = actorArray[currentFieldActorId].m50_modelRotation.vz + sVar3;
    ADVANCE_VM(0x3);
    updateScriptActor3dRotation(iVar1);
}

void OP_ROTATION_3D_ROLL_SUBSTRACT()
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    short sVar3;

    sVar3 = getImmediateOrVariableUnsigned(1);
    psVar2 = pCurrentFieldScriptActor;
    iVar1 = currentFieldActorId;
    actorArray[currentFieldActorId].m50_modelRotation.vz = actorArray[currentFieldActorId].m50_modelRotation.vz - sVar3;
    ADVANCE_VM(0x3);
    updateScriptActor3dRotation(iVar1);
}

void OP_OPEN_DOOR()
{
    uint* puVar1;
    sFieldScriptEntity* psVar2;
    sFieldScriptEntity* psVar3;

    psVar2 = pCurrentFieldScriptActor;
    if ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x100000) == 0) {
        if ((pCurrentFieldScriptActor->m12C_flags & 0x20) == 0) {
            pCurrentFieldScriptActor->m12C_flags = pCurrentFieldScriptActor->m12C_flags | 0x20;
            psVar2->mE2 = 0;
            playFieldSoundEffect(8, 3);
        }
        else {
            pCurrentFieldScriptActor->mE2 = pCurrentFieldScriptActor->mE2 + 1;
            psVar2 = pCurrentFieldScriptActor;
            if (pCurrentFieldScriptActor->mE2 < 0x1f) {
                if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0) {
                    actorArray[currentFieldActorId].m50_modelRotation[1] = actorArray[currentFieldActorId].m50_modelRotation[1] + 0x20;
                }
                else {
                    actorArray[currentFieldActorId].m50_modelRotation[1] = actorArray[currentFieldActorId].m50_modelRotation[1] + -0x20;
                }
            }
            else {
                pCurrentFieldScriptActor->mE2 = 0;
                psVar2->m0_fieldScriptFlags.m_rawFlags |= 0x100000;
                psVar3 = pCurrentFieldScriptActor;
                psVar2->m12C_flags = psVar2->m12C_flags & 0xffffffdf;
                psVar3->mCC_scriptPC = psVar3->mCC_scriptPC + 2;
            }
        }
    }
    else {
        ADVANCE_VM(0x2);
    }
    updateScriptActor3dRotation(currentFieldActorId);
}

void OP_CLOSE_DOOR(void)
{
    uint* puVar1;
    sFieldScriptEntity* psVar2;
    sFieldScriptEntity* psVar3;

    psVar2 = pCurrentFieldScriptActor;
    if ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x100000) == 0) {
        pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    }
    else {
        if ((pCurrentFieldScriptActor->m12C_flags & 0x20) == 0) {
            pCurrentFieldScriptActor->m12C_flags = pCurrentFieldScriptActor->m12C_flags | 0x20;
            psVar2->mE2 = 0;
            playFieldSoundEffect(8, 3);
        }
        else {
            pCurrentFieldScriptActor->mE2 = pCurrentFieldScriptActor->mE2 + 1;
            psVar2 = pCurrentFieldScriptActor;
            if (pCurrentFieldScriptActor->mE2 < 0x1f) {
                if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0) {
                    actorArray[currentFieldActorId].m50_modelRotation[1] = actorArray[currentFieldActorId].m50_modelRotation[1] + -0x20;
                }
                else {
                    actorArray[currentFieldActorId].m50_modelRotation[1] = actorArray[currentFieldActorId].m50_modelRotation[1] + 0x20;
                }
            }
            else {
                pCurrentFieldScriptActor->mE2 = 0;
                psVar3 = pCurrentFieldScriptActor;
                psVar2->m0_fieldScriptFlags.m_rawFlags &= 0xffefffff;
                psVar2->m12C_flags = psVar2->m12C_flags & 0xffffffdf;
                psVar3->mCC_scriptPC = psVar3->mCC_scriptPC + 2;
            }
        }
    }
    updateScriptActor3dRotation(currentFieldActorId);
    return;
}


void OP_RUN_ENTITY_SCRIPT_ASYNC()
{
    if (readCharacter(1) != 0xFF)
    {
        int entityId = readCharacter(1);
        sFieldScriptEntity* pEntity = actorArray[entityId].m4C_scriptEntity;

        if ((pEntity->m4_flags.m_rawFlags & 0x100000) == 0)
        {
            if (isScriptAlreadyRunning(pEntity, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f) != -1)
            {
                // find a free slot
                int foundSlot = -1;
                for (int i = 0; i < 8; i++)
                {
                    if ((pEntity->m8C_scriptSlots[i].m4_flags.m18 == 0xf) && !pEntity->m8C_scriptSlots[i].m4_flags.m22)
                    {
                        foundSlot = i;
                        break;
                    }
                }
                if (foundSlot == -1)
                {
                    return;
                }

                u8 param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f;
                u8 scriptId = param & 0x1F;

                pEntity->m8C_scriptSlots[foundSlot].m0_scriptPC = getScriptEntryPoint(entityId, scriptId);
                pEntity->m8C_scriptSlots[foundSlot].m4_flags.m18 = param >> 5;
                pEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex = scriptId;
            }
        }
        else
        {
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 0;
            pEntity->m8C_scriptSlots[pEntity->mCF_scriptSlotWaitedOn].m4_flags.m22 = 0;
        }
    }
    ADVANCE_VM(0x3);
}

void OP_RUN_ENTITY_SCRIPT_BLOCKING()
{
    if (readCharacter(1) != 0xFF)
    {
        int targetEntityId = readCharacter(1);
        sFieldScriptEntity* pTargetEntity = actorArray[targetEntityId].m4C_scriptEntity;
        if ((pTargetEntity->m4_flags.m_rawFlags & 0x100000) == 0)
        {
            int scriptStatus = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status;
            switch (scriptStatus)
            {
            case 0: //init
                if (isScriptAlreadyRunning(pTargetEntity, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f) != -1)
                {
                    // find a free slot
                    int foundSlot = -1;
                    for (int i = 0; i < 8; i++)
                    {
                        if ((pTargetEntity->m8C_scriptSlots[i].m4_flags.m18 == 0xf) && !pTargetEntity->m8C_scriptSlots[i].m4_flags.m22)
                        {
                            foundSlot = i;
                            break;
                        }
                    }
                    if (foundSlot == -1)
                    {
                        return;
                    }

                    u8 param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f;
                    u8 scriptId = param & 0x1F;

                    pTargetEntity->m8C_scriptSlots[foundSlot].m0_scriptPC = getScriptEntryPoint(targetEntityId, scriptId);
                    pTargetEntity->m8C_scriptSlots[foundSlot].m4_flags.m18 = param >> 5;
                    pTargetEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m22 = 1;
                    pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn = foundSlot;
                    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 1;
                    pTargetEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex = scriptId;
                    return;
                }
                // TODO: does this ever happen?
                pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
                return;
            case 1: // waiting for script completion
                if ((pTargetEntity->mCE_currentScriptSlot == pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn) || (pTargetEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m18 == 0xf)) {
                    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 2;
                    return;
                }
                breakCurrentScript = 1;
                return;
            case 2: // cleanup
                if (pTargetEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m18 != 0xF)
                {
                    breakCurrentScript = 1;
                    return;
                }
                pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 0;
                pTargetEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m22 = 0;
                break;
            default:
                assert(0);
                break;
            }
        }
        else
        {
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 0;
            pTargetEntity->m8C_scriptSlots[pTargetEntity->mCF_scriptSlotWaitedOn].m4_flags.m22 = 0;
        }
    }
    ADVANCE_VM(0x3);
    return;
}

void OP_RUN_ENTITY_SCRIPT_UNKMODE()
{
    if (readCharacter(1) != 0xFF)
    {
        int entityId = readCharacter(1);
        sFieldScriptEntity* pEntity = actorArray[entityId].m4C_scriptEntity;

        if ((pEntity->m4_flags.m_rawFlags & 0x100000) == 0)
        {
            int index = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status;
            if (index)
            {
                if (index != 1)
                {
                    return;
                }
                if ((pEntity->mCE_currentScriptSlot != pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn) && (pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m18 != 0xf))
                {
                    breakCurrentScript = 1;
                    return;
                }

                pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
                pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 0;
                pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m22 = 0;
                return;
            }

            if (isScriptAlreadyRunning(pEntity, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f) != -1)
            {
                // find a free slot
                int foundSlot = -1;
                for (int i = 0; i < 8; i++)
                {
                    if ((pEntity->m8C_scriptSlots[i].m4_flags.m18 == 0xf) && !pEntity->m8C_scriptSlots[i].m4_flags.m22)
                    {
                        foundSlot = i;
                        break;
                    }
                }
                if (foundSlot == -1)
                {
                    return;
                }

                u8 param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] & 0x1f;
                u8 scriptId = param & 0x1F;

                pEntity->m8C_scriptSlots[foundSlot].m0_scriptPC = getScriptEntryPoint(entityId, scriptId);
                pEntity->m8C_scriptSlots[foundSlot].m4_flags.m18 = param >> 5;
                pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn].m4_flags.m22 = 1;
                pEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex = scriptId;
                pCurrentFieldScriptActor->mCF_scriptSlotWaitedOn = foundSlot;
                pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 1;
                return;
            }
        }
        else
        {
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16_status = 0;
            pEntity->m8C_scriptSlots[pEntity->mCF_scriptSlotWaitedOn].m4_flags.m22 = 0;
        }
    }
    ADVANCE_VM(0x3);
}

void OP_GET_ANGLE_COS()
{
    u16 uVar1;
    int uVar2;
    int iVar3;
    int iVar4;

    uVar1 = readU16FromScript(1);
    uVar2 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    iVar3 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    iVar4 = getAngleCos(uVar2);
    setVar(uVar1, iVar4 * iVar3 >> 0xc);
    ADVANCE_VM(0x8);
}

void OP_GET_ANGLE_SIN()
{
    u16 uVar1;
    int uVar2;
    int iVar3;
    int iVar4;

    uVar1 = readU16FromScript(1);
    uVar2 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    iVar3 = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
    iVar4 = getAngleSin(uVar2);
    setVar(uVar1, iVar4 * iVar3 >> 0xc);
    ADVANCE_VM(0x8);
}

void OP_ROTATE_TO_ACTOR()
{
    sFieldScriptEntity* psVar1;
    ushort uVar2;
    int iVar3;
    long lVar4;

    iVar3 = readCharacter(1);
    if (iVar3 != 0xff) {
        lVar4 = ratan2(((actorArray[iVar3].m4C_scriptEntity)->m20_position).vz - (pCurrentFieldScriptActor->m20_position).vz,
            ((actorArray[iVar3].m4C_scriptEntity)->m20_position).vx - (pCurrentFieldScriptActor->m20_position).vx);
        psVar1 = pCurrentFieldScriptActor;
        uVar2 = -(short)lVar4 | 0x8000;
        pCurrentFieldScriptActor->m104_rotation = uVar2;
        psVar1->m106_currentRotation = uVar2;
    }
    ADVANCE_VM(0x2);
}

void OP_F0()
{
    ushort uVar1;
    uVar1 = readU16FromScript(1);
    setVar((uint)uVar1, op99Var3);
    uVar1 = readU16FromScript(3);
    setVar((uint)uVar1, op99Var2);
    uVar1 = readU16FromScript(5);
    setVar((uint)uVar1, (short)op99Var1);
    fieldExectuteMaxCycles++;
    ADVANCE_VM(0x7);
}

void OP_SETUP_SCREEN_EFFECT1()
{
    int uVar1 = getImmediateOrVariableUnsigned(9);
    int uVar2 = getImmediateOrVariableUnsigned(3);
    int uVar3 = getImmediateOrVariableUnsigned(5);
    int uVar4 = getImmediateOrVariableUnsigned(7);
    int uVar5 = getImmediateOrVariableUnsigned(1);
    setupRGBCalcMode(1, uVar1, uVar2, uVar3, uVar4, uVar5);
    ADVANCE_VM(0xB);
}

void OP_SETUP_CAMERA_SHAKE()
{
    MissingCode();
    ADVANCE_VM(0x9);
}

void OP_VRAM_OPERATION()
{
    MissingCode();
    ADVANCE_VM(0xE);
}

void OP_SETUP_FOG()
{
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0x11;
}

void OPX_07()
{
    switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]) {
    case 0:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags &= ~0x400;
        break;
    case 1:
        pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0x400;
        break;
    default:
        assert(0);
    }

    pCurrentFieldScriptActor->mCC_scriptPC += 2;
}

void OP_SET_3D_SCALE() {
    ((actorArray[currentFieldActorId].m4_pVramSpriteSheet)->m0_spriteActorCore).m2C_scale = 0xc00;
    pCurrentFieldScriptActor->mF4_scale3d[0] = getImmediateOrVariableUnsigned(1);
    pCurrentFieldScriptActor->mF4_scale3d[1] = getImmediateOrVariableUnsigned(3);
    pCurrentFieldScriptActor->mF4_scale3d[2] = getImmediateOrVariableUnsigned(5);
    updateScriptActor3dRotation(currentFieldActorId);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
}

void OPX_09()
{
    if (getImmediateOrVariableUnsigned(1) == 0) {
        pCurrentFieldScriptActor->m4_flags.m_x800 = 0;
    }
    else {
        pCurrentFieldScriptActor->m4_flags.m_x800 = 1;
    }
    ADVANCE_VM(0x3);
}

void OPX_SET_FLAG_BIT_FIELD()
{
    ushort uVar1;
    ushort uVar2;
    ushort uVar3;

    uVar1 = readU16FromScript(1);
    uVar2 = readU16FromScript(1);
    uVar3 = getVariable((uint)(uVar1 >> 4));
    setVar((uint)(uVar1 >> 4), uVar3 | (ushort)(1 << (uVar2 & 0xf)));
    ADVANCE_VM(3);
}

void OPX_0C()
{
    MissingCode();
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xd;
}

void OP_SET_MUSIC_PARAMS()
{
    MissingCode();
    ADVANCE_VM(0x5);
    breakCurrentScript = 1;
}

void OPX_SET_TEMPO()
{
    MissingCode();
    ADVANCE_VM(0x5);
    breakCurrentScript = 1;
}

void OP_F9()
{
    uint uVar1;

    uVar1 = readCharacter(1);
    if (uVar1 != 0xff) {
        pCurrentFieldScriptActor->m75_parentActorId = (char)uVar1;
    }
    ADVANCE_VM(0x2);
}

void OP_FB()
{
    ushort uVar1;
    ushort uVar2;
    uint uVar3;

    uVar1 = readU16FromScript(1);
    uVar2 = readU16FromScript(1);
    uVar3 = getVariable(uVar1 >> 4);
    if ((uVar3 & 1 << (uVar2 & 0xf)) == 0) {
        pCurrentFieldScriptActor->mCC_scriptPC = readU16FromScript(3);
    }
    else {
        ADVANCE_VM(0x5);
    }
}

void OP_FC()
{
    if (readCharacter(1) == 0xFF)
    {
        pCurrentFieldScriptActor->mCC_scriptPC += 6;
    }
    else
    {
        int characterId = readCharacter(1);
        pCurrentFieldScriptActor->m80_dialogPortrait = (actorArray[characterId].m4C_scriptEntity)->m80_dialogPortrait;
        OP_SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0();
    }
}

void OP_EXTENDED_OPCODE()
{
    u8 opcodeEX = pCurrentFieldScriptFile[++pCurrentFieldScriptActor->mCC_scriptPC];

    LogOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, 0xFE00 | opcodeEX);

    if (fieldScriptOpcodes_EX[opcodeEX] == nullptr)
    {
        LogMissingOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, 0xFE00 | opcodeEX);

        pCurrentFieldScriptActor->mCC_scriptPC--;
        breakCurrentScript = 1;
        return;
    }

    fieldScriptOpcodes_EX[opcodeEX]();
}

void OP_FF()
{
    ADVANCE_VM(0x1);
}
