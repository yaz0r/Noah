#include "noahLib.h"
#include "opcodes.h"
#include "fieldScriptSupport.h"
#include "kernel/trigo.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "opcodeTables.h"
#include "fieldGraphicObject.h"
#include "sprite/spriteSetup.h"
#include "dialogWindows.h"

// TODO: Cleanup
s16 findTriangleInWalkMesh(int posX, int posZ, int walkmeshId, SVECTOR* param_4, VECTOR* param_5);
extern s16 numWalkMesh;
/////////////////////////

void OP_JUMP_IF()
{
	bool bVar1;
	ushort uVar2;
	int iVar3;
	byte bVar4;
	uint uVar5;
	uint uVar6;

	uVar5 = 0;
	bVar4 = pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf0;
	uVar6 = 0;
	if (bVar4 == 0x40) {
		uVar2 = readU16FromScript(1);
		uVar6 = getVariable((uint)uVar2);
		iVar3 = readS16FromScript(3);
		uVar5 = iVar3;
		uVar2 = readU16FromScript(1);
		iVar3 = getBytesSign((uint)uVar2);
		if (iVar3 != 0) {
		LAB_Field__800a1cd0:
			uVar5 = uVar5 & 0xffff;
		}
	}
	else {
		if (bVar4 < 0x41) {
			if ((pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf0) == 0) {
				uVar2 = readU16FromScript(1);
				uVar6 = getVariable((uint)uVar2);
				uVar2 = readU16FromScript(3);
				uVar5 = getVariable((uint)uVar2);
				uVar2 = readU16FromScript(1);
				iVar3 = getBytesSign((uint)uVar2);
				if (iVar3 != 0) goto LAB_Field__800a1cd0;
				uVar5 = uVar5;
			}
		}
		else {
			if (bVar4 == 0x80) {
				iVar3 = readS16FromScript(1);
				uVar6 = iVar3;
				uVar2 = readU16FromScript(3);
				uVar5 = getVariable((uint)uVar2);
				uVar2 = readU16FromScript(3);
				iVar3 = getBytesSign((uint)uVar2);
				if (iVar3 != 0) {
					uVar6 = uVar6 & 0xffff;
				}
			}
			else {
				if (bVar4 == 0xc0) {
					iVar3 = readS16FromScript(1);
					uVar6 = iVar3;
					iVar3 = readS16FromScript(3);
					uVar5 = iVar3;
				}
			}
		}
	}
	bVar1 = false;
	switch ((uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf) {
	case 0:
		if (uVar6 == uVar5) {
			bVar1 = true;
		}
		goto switchD_Field_800a1d84_caseD_b;
	case 1:
		if (uVar6 != uVar5) {
			bVar1 = true;
		}
		goto switchD_Field_800a1d84_caseD_b;
	case 2:
		uVar5 = (uint)((int)uVar5 < (int)uVar6);
		break;
	case 3:
		uVar5 = (uint)((int)uVar6 < (int)uVar5);
		break;
	case 4:
		if ((int)uVar5 <= (int)uVar6) {
			bVar1 = true;
		}
		goto switchD_Field_800a1d84_caseD_b;
	case 5:
		if ((int)uVar6 <= (int)uVar5) {
			bVar1 = true;
		}
		goto switchD_Field_800a1d84_caseD_b;
	case 6:
		uVar5 = uVar6 & uVar5;
		break;
	case 7:
		if (uVar6 != uVar5) {
			bVar1 = true;
		}
		goto switchD_Field_800a1d84_caseD_b;
	case 8:
		uVar5 = uVar6 | uVar5;
		break;
	case 9:
		uVar5 = uVar6 & uVar5;
		break;
	case 10:
		uVar5 = ~uVar6 & uVar5;
		break;
	default:
		goto switchD_Field_800a1d84_caseD_b;
	}
	if (uVar5 != 0) {
		bVar1 = true;
	}
switchD_Field_800a1d84_caseD_b:
	if (bVar1) {
		uVar2 = pCurrentFieldScriptActor->mCC_scriptPC + 8;
	}
	else {
		uVar2 = readU16FromScript(6);
	}
	pCurrentFieldScriptActor->mCC_scriptPC = uVar2;
	return;
}

void OP_4()
{
	for (int i=0; i<8; i++)
	{
		if (pCurrentFieldScriptActor->m8C_scriptSlots[i].m4_flags.m18 == 7)
		{
			pCurrentFieldScriptActor->m8C_scriptSlots[i].m0_scriptPC = getScriptEntryPoint(currentFieldActorId, 1);
		}
	}

	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18 = 7;
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = 0xFF;
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
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = 0xff;
		currentScriptFinished = 1;
		breakCurrentScript = 1;
	}
	else {
		uVar3 = ((uVar2 >> 6 & 7) - 1 & 7) << 6;
		pCurrentFieldScriptActor->m12C_flags = uVar2 & 0xfffffe3f | uVar3;
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->m78_stack[uVar3 >> 5];
	}
}

void OP_ADD_TO_CURRENT_PARTY()
{
	if (((asyncLoadingVar1 == 0xff) && (asyncLoadingVar2 == 0)) && (!isAsyncLoadingProhibited()))
	{
		waitReadCompletion(0);

		int freePartySlot;
		int partyIsFullOrCharacterAlreadyInParty = findFreePartySlot((uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1], &freePartySlot);

		sFieldScriptEntity* psVar2 = pCurrentFieldScriptActor;
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
			psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 4;
		}
	}
	else
	{
		breakCurrentScript = 1;
		pCurrentFieldScriptActor->mCC_scriptPC--;
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
		switch(iVar3)
		{
		case 0:
			if (currentParty[1] == 0xff) {
				currentParty[0] = iVar1;
				asyncPartyCharacterLoadingTable[0] = iVar1;
				pKernelGameState->m22B1_isOnGear[2] = 0;
			}
			else {
				partyCharacterBuffers[0] = partyCharacterBuffers[1];
				asyncPartyCharacterLoadingTable[0] = asyncPartyCharacterLoadingTable[1];
				currentParty[0] = currentParty[1];
				currentParty[1] = 0xff;
				asyncPartyCharacterLoadingTable[1] = 0xff;
				pKernelGameState->m22B1_isOnGear[2] = pKernelGameState->m22B1_isOnGear[3];
				if (currentParty[2] != 0xff) {
					partyCharacterBuffers[1] = partyCharacterBuffers[2];
					asyncPartyCharacterLoadingTable[1] = asyncPartyCharacterLoadingTable[2];
					currentParty[1] = currentParty[2];
					currentParty[2] = 0xff;
					asyncPartyCharacterLoadingTable[2] = 0xff;
					pKernelGameState->m22B1_isOnGear[3] = pKernelGameState->m22B1_isOnGear[4];
				}
			}
			break;
		case 1:
			if (currentParty[2] == 0xff) {
				currentParty[1] = iVar1;
				asyncPartyCharacterLoadingTable[1] = iVar1;
				pKernelGameState->m22B1_isOnGear[3] = 0;
			}
			else {
				partyCharacterBuffers[1] = partyCharacterBuffers[2];
				asyncPartyCharacterLoadingTable[1] = asyncPartyCharacterLoadingTable[2];
				currentParty[1] = currentParty[2];
				currentParty[2] = 0xff;
				asyncPartyCharacterLoadingTable[2] = 0xff;
				pKernelGameState->m22B1_isOnGear[3] = pKernelGameState->m22B1_isOnGear[4];
				pKernelGameState->m22B1_isOnGear[4] = 0;
			}
			break;
		case 2:
			currentParty[2] = iVar1;
			asyncPartyCharacterLoadingTable[2] = iVar1;
			pKernelGameState->m22B1_isOnGear[4] = 0;
		}
	}
	else
	{
		switch (iVar3)
		{
		case 0:
			pKernelGameState->m22B1_isOnGear[2] = pKernelGameState->m22B1_isOnGear[3];
			pKernelGameState->m22B1_isOnGear[3] = pKernelGameState->m22B1_isOnGear[4];
			pKernelGameState->m22B1_isOnGear[4] = 0;
			emptyPartySlot(0);
			copyPartySlotFromNext(0);
			copyPartySlotFromNext(1);
			break;
		case 1:
			pKernelGameState->m22B1_isOnGear[3] = pKernelGameState->m22B1_isOnGear[4];
			pKernelGameState->m22B1_isOnGear[4] = 0;
			emptyPartySlot(1);
			copyPartySlotFromNext(1);
			break;
		case 2:
			pKernelGameState->m22B1_isOnGear[4] = 0;
			emptyPartySlot(2);
			break;
		default:
			assert(0);
		}

		if ((currentParty[0] == 0xff) || (partyToFieldEntityArrayMapping[0] == 0xff)) {
			playerControlledEntity = 0;
		}
		else {
			playerControlledEntity = partyToFieldEntityArrayMapping[0];
			(actorArray[partyToFieldEntityArrayMapping[0]].m4C_scriptEntity)->m0_flags = (actorArray[partyToFieldEntityArrayMapping[0]].m4C_scriptEntity)->m0_flags & 0xffffff7f | 0x4400;
		}
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OP_2A(void)
{
	ushort* puVar1;
	sFieldScriptEntity* psVar2;

	psVar2 = pCurrentFieldScriptActor;
	puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x20000;
	psVar2->mCC_scriptPC = *puVar1 + 1;
	return;
}

void OP_PLAY_ANIMATION()
{
	ushort* puVar1;
	byte bVar2;

	pCurrentFieldScriptActor = pCurrentFieldScriptActor;
	puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
	pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xfeffffff;
	bVar2 = pCurrentFieldScriptFile[*puVar1 + 1];
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
	pCurrentFieldScriptActor->mEA_currentAnimationPlaying = (ushort)bVar2;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
	return;
}

void OP_GET_ACTOR_FACING_ANGLE()
{
	setVar(readU16FromScript(1), (pCurrentFieldScriptActor->m106_currentRotation + 0x100 >> 9) + 2U & 7);
	pCurrentFieldScriptActor->mCC_scriptPC += 3;
}

void OP_STOP(void)
{
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m18 = 0xF;
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3_scriptIndex = 0xff;
	currentScriptFinished = 1;
	breakCurrentScript = 1;
	return;
}

void OPX_15()
{
	uint* puVar1;
	int iVar2;
	sFieldScriptEntity* psVar3;
	uint uVar4;
	int uVar5;

	actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
	uVar4 = getImmediateOrVariableUnsigned(1);
	uVar5 = getImmediateOrVariableUnsigned(3);
	OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, uVar4, &fieldActorSetupParams[uVar4], 0, uVar5, uVar4 | 0x80, 1);
	OP_INIT_ENTITY_SCRIPT_sub1();
	psVar3 = pCurrentFieldScriptActor;
	puVar1 = &pCurrentFieldScriptActor->m0_flags;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
	psVar3->m0_flags = *puVar1 & 0xffffff7f | 0x100;
	iVar2 = currentFieldActorId;
	psVar3->m4_flags = psVar3->m4_flags & 0xfffff7ff;
	actorArray[iVar2].m58_flags = actorArray[iVar2].m58_flags & 0xffdf;
}

void OP_INIT_ENTITY_NPC(void)

{
	uint* puVar1;
	int iVar2;
	sFieldScriptEntity* psVar3;
	uint uVar4;

	actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
	uVar4 = getImmediateOrVariableUnsigned(1);
	OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, uVar4, &fieldActorSetupParams[uVar4], 0, 0, uVar4 | 0x80, 0);
	OP_INIT_ENTITY_SCRIPT_sub1();
	psVar3 = pCurrentFieldScriptActor;
	puVar1 = &pCurrentFieldScriptActor->m0_flags;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	psVar3->m0_flags = *puVar1 & 0xffffff7f | 0x100;
	iVar2 = currentFieldActorId;
	psVar3->m4_flags = psVar3->m4_flags & 0xfffff7ff;
	actorArray[iVar2].m58_flags = actorArray[iVar2].m58_flags & 0xffdf;
	return;
}

void OP_UPDATE_CHARACTER()
{
	MissingCode();
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
	sFieldScriptEntity* psVar1;
	int posX;
	int posZ;

	posX = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
	posZ = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
	setCurrentActor2DPosition(posX, posZ);
	psVar1 = pCurrentFieldScriptActor;
	pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xffdfffff;
	psVar1->m0_flags = psVar1->m0_flags & 0xfffeffff;
	psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 6;
	return;
}

void OP_1A()
{
	pCurrentFieldScriptActor->m10_walkmeshId = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
	setCurrentActor2DPosition((int)(actorArray[currentFieldActorId].m4C_scriptEntity)->m20_position.vx, (int)(actorArray[currentFieldActorId].m4C_scriptEntity)->m20_position.vz);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
	return;
}

int readS16FromScriptFile(int param_1)
{
	return (int)(((uint)pCurrentFieldScriptFile[param_1] + (uint)(pCurrentFieldScriptFile + param_1)[1] * 0x100) * 0x10000) >> 0x10;
}

void OP_INIT_ENTITY_PC_Sub0(int param_1)
{
	if (*pCurrentFieldScriptFile == 0xff) {
		param_1 = param_1 * 7;
		pCurrentFieldScriptActor->m10_walkmeshId = (ushort)pCurrentFieldScriptFile[param_1 + 5];
		int sVar2 = readS16FromScriptFile(param_1 + 1);
		int sVar3 = readS16FromScriptFile(param_1 + 3);
		setCurrentActor2DPosition((int)sVar2, (int)sVar3);
		int uVar5 = (uint)pCurrentFieldScriptFile[param_1 + 6];
		if (uVar5 == 0xff) {
			uVar5 = getVariable(8);
		}
		uVar5 = uVar5 + 4 & 7;
		opA0_var1 = uVar5 << 0x19;
		opA0_var0 = opA0_var1 >> 0x10;
		op99VarB = (s16)(uVar5 << 9);
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
	sFieldEntitySub4* psVar6;
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
		OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
		psVar7 = pCurrentFieldScriptActor;
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 1;
		currentScriptFinished = 1;
		breakCurrentScript = 1;
		uVar5 = psVar7->m4_flags | 0x100000;
	}
	else {
		if (iVar4 == 0) {
			PCToActorArray[0] = currentFieldActorId;
			actorCameraTracked = currentFieldActorId;
			psVar7->m0_flags = psVar7->m0_flags & 0xffffff7f | 0x4400;
		}
		iVar2 = currentFieldActorId;
		partyToFieldEntityArrayMapping[iVar4] = currentFieldActorId;
		if (pcInitVar1 == 0) {
			OP_INIT_ENTITY_SCRIPT_sub0(iVar2, iVar4, &partyCharacterBuffers[iVar4], 1, 0, iVar4, 1);
			pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffffcff | 0x400;
		}
		else {
			OP_INIT_ENTITY_SCRIPT_sub0
			(iVar2, (uint)characterMappingTable[iVar3] + pcInitVar1, &fieldActorSetupParams[pcInitVar1 + characterMappingTable[iVar3]], 0, 0,
				(uint)characterMappingTable[iVar3] + pcInitVar1 | 0x80, 1);
			psVar7 = pCurrentFieldScriptActor;
			psVar1 = pKernelGameState;
			pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffffcff | 0x400;
			if (psVar1->m22B1_isOnGear[iVar4] != 0) {
				psVar6 = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
				actorArray[currentFieldActorId].m4_pVramSpriteSheet = actorArray[unkPartyTable[iVar4]].m4_pVramSpriteSheet;
				actorArray[unkPartyTable[iVar4]].m4_pVramSpriteSheet = psVar6;
				psVar7->m0_flags = psVar7->m0_flags & 0xfffffaff | 0x200;
			}
		}
		pcInitVar2 = -0xc0;
		actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xffdf;
		local_v0_684 = getVariable(2);
		OP_INIT_ENTITY_PC_Sub0(local_v0_684);
		OP_INIT_ENTITY_SCRIPT_sub1();
		uVar5 = pCurrentFieldScriptActor->m4_flags & 0xfffff7ff;
		psVar7 = pCurrentFieldScriptActor;
	}
	psVar7->m4_flags = uVar5;
	psVar7 = pCurrentFieldScriptActor;
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x20000;
	psVar7->m4_flags = psVar7->m4_flags | 0x400;
	psVar7->mCC_scriptPC = psVar7->mCC_scriptPC + 3;
	return;
}

void OP_18()
{
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] != 0) {
		pCurrentFieldScriptActor->m18[0] = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] << 1;
	}
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] != 0) {
		pCurrentFieldScriptActor->m18[2] = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 2] << 1;
	}
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] != 0) {
		pCurrentFieldScriptActor->m18[1] = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 3] << 1;
	}
	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 4] != 0) {
		pCurrentFieldScriptActor->m18[3] = (ushort)pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 4] << 1;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_SET_DIALOG_AVATAR(void)
{
	pCurrentFieldScriptActor->m80_DialogAvatarFace = getCharacter(getImmediateOrVariableUnsigned(1));
	pCurrentFieldScriptActor->mCC_scriptPC += 3;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_RAND_ROTATION()
{
	ushort* puVar1;
	sFieldScriptEntity* psVar2;
	ushort uVar3;
	uint uVar4;
	ushort uVar5;

	uVar5 = pCurrentFieldScriptActor->m106_currentRotation;
	pCurrentFieldScriptActor->m102_rotationCount++;
	if ((pCurrentFieldScriptActor->m102_rotationCount & 0xf) == 0) {
		uVar4 = rand();
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
	pCurrentFieldScriptActor->mCC_scriptPC++;
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
		pCurrentFieldScriptActor->m82[0] = 0;
		psVar2 = pCurrentFieldScriptActor;
		psVar5->m88[0] = 0;
		psVar5->m88[1] = 0;
		psVar2->m82[1] = 0;
		psVar5 = pCurrentFieldScriptActor;
		puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
		pCurrentFieldScriptActor->m84 = 0;
		uVar3 = *puVar1 + 2;
	}
	psVar5->mCC_scriptPC = uVar3;
	breakCurrentScript = 1;
}

void OP_SHOW_DIALOG_WINDOW_FOR_CURRENT_ACTOR_MODE3()
{
	showDialogWindowForActor(currentFieldActorId, 3);
}

void OP_SET_FLAGS_DYNAMIC()
{
	u32 uVar1 = readU16FromScript(2);
	switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1]) {
	case 0:
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | uVar1;
		break;
	case 1:
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | uVar1 << 0x10;
		break;
	case 2:
		pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags | uVar1;
		break;
	case 3:
		pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags | uVar1 << 0x10;
		break;
	case 4:
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & ~uVar1;
		break;
	case 5:
		pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & ~(uVar1 << 0x10);
		break;
	case 6:
		pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & ~uVar1;
		break;
	case 7:
		pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & ~(uVar1 << 0x10);
		break;
	default:
		assert(0);
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
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
		pCurrentFieldScriptActor->mCC_scriptPC += 5;
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
	psVar1->m4_flags = psVar1->m4_flags | 0x800;
	psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
	return;
}


void OP_4A()
{
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xffff;
	if (spriteWalkToPositionOrActor(-1) == 0) {
		pCurrentFieldScriptActor->mCC_scriptPC += 6;
	}
}

void OP_4B()
{
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
	if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0xFFFF) {
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = getImmediateOrVariableUnsigned(6);
	}
	if (spriteWalkToPositionOrActor(getImmediateOrVariableUnsigned(6)) == 0) {
		pCurrentFieldScriptActor->mCC_scriptPC += 8;
	}
}

void OP_52(void)
{
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 2;
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xffff;
	if (spriteWalkToPositionOrActor(0xffff) == 0) {
		pCurrentFieldScriptActor->mCC_scriptPC += 2;
	}
	return;
}

void OP_53(void)
{
	pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 2;
	if (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0xFFFF)
	{
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = getImmediateOrVariableUnsigned(2);
	}
	if (spriteWalkToPositionOrActor(getImmediateOrVariableUnsigned(2)) == 0)
	{
		pCurrentFieldScriptActor->mCC_scriptPC += 4;
	}
}

void OP_57(void)
{
	std::array<VECTOR, 4> auStack168;
	std::array<SVECTOR, 4> auStack104;

	pCurrentFieldScriptActor->m0_flags |= 0x10000;
	int iVar6;
	switch (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 3)
	{
	case 0:
		iVar6 = getVar10(8, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 10]);
		break;
	case 3:
		if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0xF)
		{
			for (int i=0; i<numWalkMesh -1; i++)
			{
				pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(pCurrentFieldScriptActor->m20_position.vx >> 0x10, pCurrentFieldScriptActor->m20_position.vz >> 0x10, i, &auStack104[i], &auStack168[i]);
			}
			pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffeffff;
			pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xffdfffff;
			pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
			breakCurrentScript = 1;
			return;
		}

		if (pCurrentFieldScriptActor->m102_rotationCount < pCurrentFieldScriptActor->mE0_rotationLimit)
		{
			(pCurrentFieldScriptActor->m20_position).vx = (pCurrentFieldScriptActor->m20_position).vx + pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
			(pCurrentFieldScriptActor->m20_position).vz = (pCurrentFieldScriptActor->m20_position).vz + pCurrentFieldScriptActor->mD0_targetPositionOffset[2];
			(pCurrentFieldScriptActor->m20_position).vy = (pCurrentFieldScriptActor->m20_position).vy + (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy;
			(actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy = (actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy + actorArray[currentFieldActorId].m4_pVramSpriteSheet->m1C;
			if (((pCurrentFieldScriptActor->mD0_targetPositionOffset[0] != 0) || (pCurrentFieldScriptActor->mD0_targetPositionOffset[2] != 0)) && ((pCurrentFieldScriptActor->m0_flags & 0x8000) == 0)) {
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
			(actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy = 0;
			(pCurrentFieldScriptActor->m20_position).vx = iVar6 << 0x10;
			(pCurrentFieldScriptActor->m20_position).vy = iVar8 << 0x10;
			(pCurrentFieldScriptActor->m20_position).vz = iVar7 << 0x10;
			pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffeffff;
			pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xd;
			pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xffdfffff;
		}

		actorArray[currentFieldActorId].mC_matrix.t[0] = (pCurrentFieldScriptActor->m20_position).vx >> 0x10;
		actorArray[currentFieldActorId].mC_matrix.t[1] = (pCurrentFieldScriptActor->m20_position).vy >> 0x10;
		actorArray[currentFieldActorId].mC_matrix.t[2] = (pCurrentFieldScriptActor->m20_position).vz >> 0x10;
		actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = (pCurrentFieldScriptActor->m20_position).vx;
		actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = (pCurrentFieldScriptActor->m20_position).vy;
		actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = (pCurrentFieldScriptActor->m20_position).vz;
		pCurrentFieldScriptActor->m102_rotationCount = pCurrentFieldScriptActor->m102_rotationCount + 1;
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

		pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[iVar8] = findTriangleInWalkMesh(iVar7, iVar8, iVar15, &auStack104[iVar8], &auStack168[iVar8]);

		iVar9 = (int)auStack104[iVar15].vy;
		pCurrentFieldScriptActor->m10_walkmeshId = (short)iVar15;
	}
	int iVar15 = -((actorArray[currentFieldActorId].m4_pVramSpriteSheet->m1C * iVar6) / 2);
	(actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy = iVar15;
	(actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC).vy = iVar15 + (iVar9 * 0x10000 - (pCurrentFieldScriptActor->m20_position).vy) / iVar6;
	iVar9 = (pCurrentFieldScriptActor->m20_position).vx;
	iVar15 = (pCurrentFieldScriptActor->m20_position).vz;
	pCurrentFieldScriptActor->mD0_targetPositionOffset[1] = 0;
	pCurrentFieldScriptActor->mE0_rotationLimit = (short)iVar6;
	pCurrentFieldScriptActor->m102_rotationCount = 0;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xb;
	pCurrentFieldScriptActor->mD0_targetPositionOffset[0] = (iVar7 * 0x10000 - iVar9) / (iVar6 + 1);
	pCurrentFieldScriptActor->mD0_targetPositionOffset[2] = (iVar8 * 0x10000 - iVar15) / (iVar6 + 1);
	breakCurrentScript = 1;
}

void OP_59()
{
	s16 uVar5 = pCurrentFieldScriptActor->m106_currentRotation;
	s16 uVar3 = pCurrentFieldScriptActor->m102_rotationCount + 1;
	pCurrentFieldScriptActor->m102_rotationCount = uVar3;
	if ((uVar3 & 0xf) == 0) {
		u32 uVar4 = rand();
		if ((uVar4 & 0x30) == 0) {
			if ((uVar4 & 1) == 0) {
				uVar5 = pCurrentFieldScriptActor->m106_currentRotation + 0x200;
			}
			else {
				uVar5 = pCurrentFieldScriptActor->m106_currentRotation - 0x200;
			}
			uVar5 = uVar5 & 0xfff;
		}
		else {
			uVar5 = pCurrentFieldScriptActor->m106_currentRotation | 0x8000;
			pCurrentFieldScriptActor->m106_currentRotation = uVar5;
		}
	}
	breakCurrentScript = 1;
	pCurrentFieldScriptActor->m104_rotation = uVar5;
	pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_5A()
{
	resetFieldScriptActor();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_99(void)
{
	cameraInterpolationMode = 1;
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
	op99Var1 = sceneSCRZ * sceneScale >> 0xc;
	op99Var2 = sceneDIP;
	op99Var3 = op99VarB;
	op99Var4 = 0x1000;
	op99Var5 = 0xc;
	op99Var6 = 0xc;
	op99Var7 = op99Var7 | 0x8000;
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

void OP_SET_GAMEPROGRESS()
{
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 0x20;
	setVar(0, getImmediateOrVariableUnsigned(1));
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_ADD_ENDITY_TO_FIELD1721_LIST()
{
	actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
	int iVar3 = getImmediateOrVariableUnsigned(1);
	OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 1);
	OP_INIT_ENTITY_SCRIPT_sub1();
	sFieldScriptEntity* psVar2 = pCurrentFieldScriptActor;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	int iVar1 = currentFieldActorId;
	psVar2->m0_flags = psVar2->m0_flags | 0x100;
	actorArray[iVar1].m58_flags = actorArray[iVar1].m58_flags & 0xffdf;
	psVar2->m4_flags = psVar2->m4_flags & 0xfffff7ff | 0x2000;
	//(&field1721EntityList)[NumEntityInField1721List] = (short)(iVar3 << 1);
	//(&DAT_Field__800b225f)[NumEntityInField1721List] = 0;
	//pCurrentFieldScriptActor->m12C_flags = pCurrentFieldScriptActor->m12C_flags & 0xffff1fff | (NumEntityInField1721List & 7) << 0xd;
	//NumEntityInField1721List = NumEntityInField1721List + 1;
	MissingCode();
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
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
		if (((actorArray[gDialogWindows[windowIndex].m418_actorId].m4C_scriptEntity)->m4_flags & 0x200) != 0) {
			int uVar2 = (uint)pCurrentFieldScriptActor->m84 >> 0x10;
			if (uVar2 == 0) {
				uVar2 = pCurrentFieldScriptActor->m84 & 0xffff;
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
	op99VarB = (short)(uVar2 << 9);
	sceneSCRZ = getImmediateOrVariableUnsigned(5);
	SetGeomScreen(sceneSCRZ);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
}

void OP_A8()
{
	ushort uVar1;
	int iVar2;
	int iVar3;

	iVar2 = rand();
	iVar3 = getImmediateOrVariableUnsigned(3);
	uVar1 = readU16FromScript(1);
	setVar(uVar1, iVar2 * (iVar3 + 1) >> 0xf);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_AC()
{
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
			cameraAt[0] = cameraTargetOverride[0];
			cameraAt[1] = cameraTargetOverride[1];
			cameraAt[2] = cameraTargetOverride[2];
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
			cameraEye[0] = cameraPositionOverride[0];
			cameraEye[1] = cameraPositionOverride[1];
			cameraEye[2] = cameraPositionOverride[2];
		}
		break;
	default:
		assert(0);
		break;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_60()
{
	MissingCode();
	/*    cameraTargetOverride[2] = DAT_Field__800af8c8;
		cameraTargetOverride[0] = DAT_Field__800af8c0;
		cameraTargetOverride[1] = DAT_Field__800af8c4;*/
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_64()
{
	MissingCode();
	/*  cameraPositionOverride[2] = DAT_Field__800af8b8;
  cameraPositionOverride[0] = DAT_Field__800af8b0;
  cameraPositionOverride[1] = DAT_Field__800af8b4;*/
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 1;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
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
	iVar2 = getCurrentActorRotation();
	setCurrentActorTargetRotation(actorDirectionTable[iVar1 + iVar2 & 7]);
}

void OP_ROTATE_ACTOR_ANTICLOCKWISE()
{
	int iVar1;
	int iVar2;

	iVar1 = getImmediateOrVariableUnsigned(1);
	iVar2 = getCurrentActorRotation();
	setCurrentActorTargetRotation(actorDirectionTable[iVar2 - iVar1 & 7]);
}

void OP_SET_CURRENT_ACTOR_ROTATION(void)
{
	setCurrentActorTargetRotation(getImmediateOrVariableUnsigned(1));
}

void OP_15()
{
	if ((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) {
		breakCurrentScript = 1;
	}
	else {
		fieldRandomBattleVar = 0xffff;
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
	}
}

void OP_A()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_1F()
{
	pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xfffffff8 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 7;
	pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xffffffc0 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] & 7 | pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] >> 1 & 0x38;
	pCurrentFieldScriptActor->mCC_scriptPC += 2;
}

void OP_21()
{
	int sVar2 = getImmediateOrVariableUnsigned(1);
	pCurrentFieldScriptActor->m76 = sVar2;
	OP_21_sub(actorArray[currentFieldActorId].m4_pVramSpriteSheet, sVar2);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_74()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
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
		pKernelGameState->m294[i].m24_HP = pKernelGameState->m294[i].m26_MaxHP;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_RESTORE_MP()
{
	for (int i = 0; i < 11; i++)
	{
		pKernelGameState->m294[i].m28_MP = pKernelGameState->m294[i].m2A_MaxMP;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_SET_CURRENT_ACTOR_ELEVATION(void)
{
	int elevation = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 3]);
	setCurrentActorElevation(elevation);
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x40000;
	pCurrentFieldScriptActor->mCC_scriptPC += 4;
	return;
}

void OP_SET_CURRENT_ACTOR_POSITION_3D()
{
	setCurrentActor2DPosition(readS16FromScript(1), readS16FromScript(3));
	setCurrentActorElevation(readS16FromScript(5));
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x40000;

	pCurrentFieldScriptActor->mCC_scriptPC += 7;
}

void OP_JUMP(void)
{
	ushort uVar1;

	uVar1 = readU16FromScript(1);
	pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
	return;
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
		pCurrentFieldScriptActor->m120_special2dAnimation = new sFieldActorSetupParams;

		pCurrentFieldScriptActor->m120_special2dAnimationRaw.resize(iVar2 + 8);
		readFile(iVar1, pCurrentFieldScriptActor->m120_special2dAnimationRaw, 0, 0x80);

		pCurrentFieldScriptActor->m120_special2dAnimation->init(pCurrentFieldScriptActor->m120_special2dAnimationRaw.begin());

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
	psVar2->mEA_currentAnimationPlaying = ~(ushort)bVar1;
	return;
}

void OP_ASSIGN_GEAR()
{
	pKernelGameState->m294[getImmediateOrVariableUnsigned(1)].m78_partyData_gearNum = getImmediateOrVariableUnsigned(3);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_SET_OFF_GEAR()
{
	int iVar1;

	iVar1 = getImmediateOrVariableUnsigned(1);
	if (2 < iVar1) {
		iVar1 = 2;
	}
	pKernelGameState->m22B1_isOnGear[iVar1 + 2] = 0;
	updateGearState(iVar1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OPX_50(void)
{
	OPX_50Param = 1;
	pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_DISABLE_COMPASS(void)
{
	compassDisabled = 1;
	pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OPX_54(void)
{
	fieldRandomBattleVar = 0xffff;
	OPX_50Param = 1;
	compassDisabled = 1;
	op99Var7 = op99Var7 | 0xc000;
	if ((playMusicAuthorized == 0) || (fieldExecuteVar3 == 0)) {
		breakCurrentScript = 1;
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC - 1;
	}
	else {
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
	}
	return;
}

void OPX_66(void)
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 9;
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

	pCurrentFieldScriptActor->mCC_scriptPC += 0xF;
}

void OPX_81(void)
{
	OPX_81Params[0] = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
	OPX_81Params[2] = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);
	OPX_81Params[1] = getVar20(5, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 7]);

	pCurrentFieldScriptActor->mCC_scriptPC += 8;
}

void OPX_82(void)
{
	OPX_82Param0[0] = getImmediateOrVariableUnsigned(1);
	OPX_82Param0[1] = getImmediateOrVariableUnsigned(3);
	OPX_82Param0[2] = getImmediateOrVariableUnsigned(5);
	OPX_82Param1[0] = getImmediateOrVariableUnsigned(7);
	OPX_82Param1[1] = getImmediateOrVariableUnsigned(9);
	OPX_82Param1[2] = getImmediateOrVariableUnsigned(0xb);
	OPX_82Param2[0] = getImmediateOrVariableUnsigned(0xd);
	OPX_82Param2[1] = getImmediateOrVariableUnsigned(0xf);
	OPX_82Param2[2] = getImmediateOrVariableUnsigned(0x11);
	OPX_82Param3[0] = getImmediateOrVariableUnsigned(0x13);
	OPX_82Param3[1] = getImmediateOrVariableUnsigned(0x15);
	OPX_82Param3[2] = getImmediateOrVariableUnsigned(0x17);
	pCurrentFieldScriptActor->mCC_scriptPC += 0x19;
	OPX_82Param4 = 1;
}

void OPX_86(void)
{
	OPX_86Param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OPX_8E()
{
	OPX8E_param0 = getImmediateOrVariableUnsigned(1);
	OPX8E_param1 = getImmediateOrVariableUnsigned(3);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OPX_8F()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
	MissingCode();
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
}

void OPX_90()
{
	MissingCode();
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 9;
}

void OPX_97()
{
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 4;
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OPX_98()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_RESTORE_GEAR()
{
	for (int i=0; i<20; i++)
	{
		pKernelGameState->m9A0_gears[i].m38_HP = pKernelGameState->m9A0_gears[i].m3C_maxHP;
		pKernelGameState->m9A0_gears[i].m10_ether = pKernelGameState->m9A0_gears[i].m12_maxEther;
	}
	pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_GET_CURRENT_DISC_NUMBER()
{
	setVar(readU16FromScript(1), getCurrentDiscNumber());
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OPX_E0(void)
{
	OPX_E0Param = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
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
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xf;
}

void OPX_3C()
{
	if (fieldExecuteVar1 != 0) {
		short uVar1 = getImmediateOrVariableUnsigned(1);
		int uVar2 = getImmediateOrVariableUnsigned(3);
		assert(0);
		int iVar3 = getImmediateOrVariableUnsigned(1);
		uVar1 = getImmediateOrVariableUnsigned(3);
		assert(0);
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OPX_1C(void)
{
	uint* puVar1;
	ushort uVar2;
	byte* pbVar3;
	sFieldScriptEntity* psVar4;
	int iVar5;
	int iVar6;
	sFieldEntitySub4* psVar7;

	psVar4 = pCurrentFieldScriptActor;
	psVar7 = actorArray[currentFieldActorId].m4_pVramSpriteSheet;
	puVar1 = &pCurrentFieldScriptActor->m4_flags;
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x10000;
	pbVar3 = &pCurrentFieldScriptFile[0];
	psVar4->m4_flags = *puVar1 | 0x200000;
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

void OP_23(void)
{
	sFieldScriptEntity* psVar1;

	psVar1 = pCurrentFieldScriptActor;
	actorArray[currentFieldActorId].m58_flags = actorArray[currentFieldActorId].m58_flags | 0x20;
	psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
	return;
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

void OP_INIT_ENTITY_SCRIPT()
{
	OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 1);
	OP_INIT_ENTITY_SCRIPT_sub1();

	pCurrentFieldScriptActor->m0_flags |= 0x100;
	pCurrentFieldScriptActor->m4_flags |= 0x800;
	pCurrentFieldScriptActor->mCC_scriptPC++;
}

void OP_INCREASE_FIELD_EXECUTION_MAX_CYCLES()
{
	fieldExectuteMaxCycles = fieldExectuteMaxCycles + 0x20;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 1;
}

void OP_IF_PLAYER_IN_TRIGGER2()
{
	s32 sxy2 = (actorArray[playerControlledEntity].m4C_scriptEntity->m20_position.vz >> 16) * 0x10000 + (actorArray[playerControlledEntity].m4C_scriptEntity->m20_position.vx >> 16);
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_IF_PLAYER_IN_TRIGGER()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 4;
}

void OP_E7()
{
	OPE7_param[0] = getImmediateOrVariableUnsigned(1);
	OPE7_param[1] = getImmediateOrVariableUnsigned(3);
	OPE7_param[2] = getImmediateOrVariableUnsigned(5);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 7;
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
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	}
}

void OP_17()
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
	pCurrentFieldScriptActor->m114_movementBoundingZone[3].vx = getVar04(0xD, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
	pCurrentFieldScriptActor->m114_movementBoundingZone[3].vy = getVar04(0xF, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 0x11]);
	pCurrentFieldScriptActor->mCC_scriptPC += 0x12;
}

void OP_SET_VAR_FROM_S16(void)
{
	ushort uVar1;
	int uVar2;

	uVar1 = readU16FromScript(1);
	uVar2 = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
	setVar((uint)uVar1, uVar2);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 6;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 6;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 6;
}

void OP_SET_VAR_INC(void)
{
	setVar(readU16FromScript(1), getVariable(readU16FromScript(1)) + 1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	return;
}

void OP_SET_VAR_DEC(void)
{
	setVar(readU16FromScript(1), getVariable(readU16FromScript(1)) - 1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	return;
}

void OP_SET_VAR_RANDOM()
{
	setVar(readU16FromScript(1), rand());
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_SET_VAR_FALSE(void)
{
	ushort uVar1;

	uVar1 = readU16FromScript(1);
	setVar(uVar1, 0);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	return;
}

void OP_SET_VAR_TRUE(void)
{
	ushort uVar1;

	uVar1 = readU16FromScript(1);
	setVar(uVar1, 1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	return;
}

void OP_JUMP_IF_PAD_MASK()
{
	jumpIfMask(padButtonForScripts[0].vx);
}

void OP_SET_CURRENT_ACTOR_FLAGS()
{
	ushort* puVar1;
	sFieldScriptEntity* psVar2;
	uint uVar3;
	uint uVar4;

	uVar3 = getImmediateOrVariableUnsigned(1);
	psVar2 = pCurrentFieldScriptActor;
	uVar4 = (uVar3 & 1) << 7;
	if ((uVar3 & 4) != 0) {
		uVar4 = uVar4 | 0x20;
	}
	if ((uVar3 & 8) != 0) {
		uVar4 = uVar4 | 0x10;
	}
	if ((uVar3 & 0x10) != 0) {
		uVar4 = uVar4 | 8;
	}
	if ((uVar3 & 0x20) != 0) {
		uVar4 = uVar4 | 4;
	}
	if ((uVar3 & 0x40) != 0) {
		uVar4 = uVar4 | 0x8000000;
	}
	puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
	pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xf7ffff43 | uVar4;
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
		pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	}
	breakCurrentScript = 1;
}

void OP_RESET_CHARACTER()
{
	int iVar1;
	uint uVar2;
	sFieldScriptEntity* psVar3;
	ushort uVar4;

	iVar1 = readCharacter(1);
	if (iVar1 != 0xff) {
		iVar1 = readCharacter(1);
		psVar3 = actorArray[iVar1].m4C_scriptEntity;
		uVar4 = psVar3->m104_rotation;
		uVar2 = psVar3->m0_flags;
		psVar3->m30_stepVector.vx = 0;
		psVar3->m30_stepVector.vy = 0;
		psVar3->m30_stepVector.vz = 0;
		psVar3->m40.vx = 0;
		psVar3->m40.vy = 0;
		psVar3->m40.vz = 0;
		uVar4 = uVar4 | 0x8000;
		psVar3->m0_flags = uVar2 | 1;
		psVar3->m106_currentRotation = uVar4;
		psVar3->m104_rotation = uVar4;

		int dialogWindow;
		if (findDialogWindowForCurrentActor(&dialogWindow) == 0) {
			gDialogWindows[dialogWindow].m414 = 0;
		}
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OP_SET_DIALOG_WINDOW_PARAM()
{
	pCurrentFieldScriptActor->m88[0] = getImmediateOrVariableUnsigned(1);
	pCurrentFieldScriptActor->m88[1] = getImmediateOrVariableUnsigned(3);
	pCurrentFieldScriptActor->m82[0] = getImmediateOrVariableUnsigned(5) * 3;
	pCurrentFieldScriptActor->m82[1] = getImmediateOrVariableUnsigned(7);
	pCurrentFieldScriptActor->m84 = getImmediateOrVariableUnsigned(9);
	pCurrentFieldScriptActor->mCC_scriptPC += 0xB;
}

void OP_SHOW_DIALOG_WINDOW_MODE0()
{
	showDialogWindowForActor(currentFieldActorId, 0);
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 5;
}

void OP_E6(void)
{
	fieldInitVar1[0] = readS16FromScript(1);
	fieldInitVar1[1] = readS16FromScript(3);
	fieldInitVar1[2] = readS16FromScript(5);
	fieldInitVar1[3] = -readS16FromScript(7);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 9;
}

void OP_ROTATION_3D_YAW_SUBSTRACT()
{
	int iVar1;
	sFieldScriptEntity* psVar2;
	short sVar3;

	sVar3 = getImmediateOrVariableUnsigned(1);
	psVar2 = pCurrentFieldScriptActor;
	iVar1 = currentFieldActorId;
	actorArray[currentFieldActorId].m50_modelRotation.vx = actorArray[currentFieldActorId].m50_modelRotation.vx - sVar3;
	psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 3;
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
	psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 3;
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
	psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 3;
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
	psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 3;
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
	psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 3;
	updateScriptActor3dRotation(iVar1);
}

void OP_RUN_ENTITY_SCRIPT_ASYNC()
{
	if (readCharacter(1) != 0xFF)
	{
		int entityId = readCharacter(1);
		sFieldScriptEntity* pEntity = actorArray[entityId].m4C_scriptEntity;

		if ((pEntity->m4_flags & 0x100000) == 0)
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
			pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 0;
			pEntity->m8C_scriptSlots[pEntity->mCF].m4_flags.m22 = 0;
		}
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

void OP_RUN_ENTITY_SCRIPT_BLOCKING()
{
	if (readCharacter(1) != 0xFF)
	{
		int entityId = readCharacter(1);
		sFieldScriptEntity* pEntity = actorArray[entityId].m4C_scriptEntity;
		if ((pEntity->m4_flags & 0x100000) == 0)
		{
			int scriptStatus = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16;
			switch (scriptStatus)
			{
			case 0:
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
					pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m22 = 1;
					pEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex = scriptId;
					pCurrentFieldScriptActor->mCF = foundSlot;
					pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 1;
					return;
				}
				assert(0);// does this ever happen?
				pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
				return;
			case 1:
				if ((pEntity->mCE_currentScriptSlot != pCurrentFieldScriptActor->mCF) && (pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m18 != 0xf)) {
					pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 2;
				}
				breakCurrentScript = 1;
				return;
			case 2:
				if (pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m18 != 0xF)
				{
					breakCurrentScript = 1;
					return;
				}
				pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 0;
				pEntity->m8C_scriptSlots[pEntity->mCF].m4_flags.m22 = 0;
				break;
			default:
				assert(0);
				break;
			}
		}
		else
		{
			pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 0;
			pEntity->m8C_scriptSlots[pEntity->mCF].m4_flags.m22 = 0;
		}
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
	return;
}

void OP_RUN_ENTITY_SCRIPT_UNKMODE()
{
	if (readCharacter(1) != 0xFF)
	{
		int entityId = readCharacter(1);
		sFieldScriptEntity* pEntity = actorArray[entityId].m4C_scriptEntity;

		if ((pEntity->m4_flags & 0x100000) == 0)
		{
			int index = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16;
			if (index)
			{
				if (index != 1)
				{
					return;
				}
				if ((pEntity->mCE_currentScriptSlot != pCurrentFieldScriptActor->mCF) && (pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m18 != 0xf))
				{
					breakCurrentScript = 1;
					return;
				}

				pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
				pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 0;
				pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m22 = 0;
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
				pEntity->m8C_scriptSlots[pCurrentFieldScriptActor->mCF].m4_flags.m22 = 1;
				pEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex = scriptId;
				pCurrentFieldScriptActor->mCF = foundSlot;
				pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 1;
				return;
			}
		}
		else
		{
			pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m16 = 0;
			pEntity->m8C_scriptSlots[pEntity->mCF].m4_flags.m22 = 0;
		}
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
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
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 8;
}

void OP_SETUP_SCREEN_EFFECT1()
{
	int uVar1 = getImmediateOrVariableUnsigned(9);
	int uVar2 = getImmediateOrVariableUnsigned(3);
	int uVar3 = getImmediateOrVariableUnsigned(5);
	int uVar4 = getImmediateOrVariableUnsigned(7);
	int uVar5 = getImmediateOrVariableUnsigned(1);
	setupRGBCalcMode(1, uVar1, uVar2, uVar3, uVar4, uVar5);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0xb;
}

void OP_SETUP_FOG()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 0x11;
}

void OPX_07()
{
	uint uVar1;

	if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == 0) {
		uVar1 = pCurrentFieldScriptActor->m4_flags & 0xfffffbff;
	}
	else {
		if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] != 1) goto LAB_Field__8008d664;
		uVar1 = pCurrentFieldScriptActor->m4_flags | 0x400;
	}
	pCurrentFieldScriptActor->m4_flags = uVar1;
LAB_Field__8008d664:
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
}

void OP_SET_MUSIC_PARAMS()
{
	MissingCode();
	pCurrentFieldScriptActor->mCC_scriptPC += 5;
	breakCurrentScript = 1;
}

void OP_SHOW_DIALOG_WINDOW_FOR_OTHER_ACTOR_MODE0()
{
	if (readCharacter(1) == 0xFF)
	{
		pCurrentFieldScriptActor->mCC_scriptPC += 6;
	}
	else
	{
		int characterId = readCharacter(1);
		pCurrentFieldScriptActor->mCC_scriptPC += 1;
		if (showDialogWindowForActor(characterId, 0) != -1)
		{
			return;
		}
		pCurrentFieldScriptActor->mCC_scriptPC -= 1;
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
		pCurrentFieldScriptActor->m80_DialogAvatarFace = (actorArray[characterId].m4C_scriptEntity)->m80_DialogAvatarFace;
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
