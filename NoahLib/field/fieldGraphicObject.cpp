#include "noahLib.h"
#include "fieldGraphicObject.h"
#include "field.h"
#include "sprite/spriteSetup.h"

int OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1_var = 0x2000;
int fieldDrawEnvsInitialized = 0;
s8 isBattleOverlayLoaded = 0;
s8 isOtherOverlayLoaded = 0;
s32 initFieldVar2 = 0;
int initFieldVar4 = 0;
int initFieldVar5 = 0;
int loadVramSpriteParam = 0;

sSpriteActor* spriteTransfertListHead = nullptr;
sFieldEntitySub4_110 sFieldEntitySub4_110_8005a474;
sFieldEntitySub4_110 sFieldEntitySub4_110_8006be10;

void OP_21_sub(sSpriteActor* param_1, int param_2)
{
	param_1->mAC = param_1->mAC & 0xfff8007f | (param_2 & 0xfff) << 7;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(sSpriteActor* param_1)
{
	for (int i=0; i<8; i++)
	{
		(*param_1->m20->m34_perSubgroupTransform)[i].m0_translateX = 0;
		(*param_1->m20->m34_perSubgroupTransform)[i].m1_translateY = 0;
		(*param_1->m20->m34_perSubgroupTransform)[i].m2_rotateX = 0;
		(*param_1->m20->m34_perSubgroupTransform)[i].m4_rotateY = 0;
		(*param_1->m20->m34_perSubgroupTransform)[i].m6_rotateZ = 0;
	}
}

void executeSpriteBytecode2Sub0Sub0sub0(sSpriteActor* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer psVar4 = param_3->m0 + READ_LE_U16(param_3->m0 + param_2 * 2);
	u32 bVar1 = READ_LE_U8(psVar4);
	u32 uVar7 = 0;
	sPS1Pointer pbVar4 = psVar4 + (bVar1 & 0x3f) * 2 + 4;
	if ((bVar1 & 0x3f) != 0) {
		do {
			while (true) {
				u8 bVar2 = READ_LE_U8(psVar4);
				if ((bVar2 & 0x80) == 0) break;
				sPS1Pointer pbVar5 = pbVar4 + 1;
				if ((bVar2 & 0x40) == 0) {
					if ((bVar2 & 1) != 0) {
						pbVar5 = pbVar4 + 2;
					}
					pbVar4 = pbVar5;
					if ((bVar2 & 2) != 0) {
						pbVar4 = pbVar4 + 1;
					}
				}
				else {
					u32 uVar6 = bVar2 & 7;
					if (param_1->m20->m34_perSubgroupTransform == nullptr) {
						param_1->m20->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
						OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
					}
					if ((bVar2 & 0x20) != 0) {
						(*param_1->m20->m34_perSubgroupTransform)[uVar6].m0_translateX = READ_LE_U8(pbVar5);
						(*param_1->m20->m34_perSubgroupTransform)[uVar6].m1_translateY = READ_LE_U8(pbVar4 + 2);
						pbVar5 = pbVar4 + 3;
					}
					pbVar4 = pbVar5;
					if ((bVar2 & 0x10) == 0) {
						(*param_1->m20->m34_perSubgroupTransform)[uVar6].m6_rotateZ = 0;
					}
					else {
						(*param_1->m20->m34_perSubgroupTransform)[uVar6].m6_rotateZ = (ushort)READ_LE_U8(pbVar4) << 4;
						pbVar4 += 1;
					}
				}
			}
			if ((bVar1 & 0x80) != 0) {
				pbVar4 = pbVar4 + 2;
			}
			uVar7 = uVar7 + 1;
			pbVar4 = pbVar4 + 3;
		} while (uVar7 != (bVar1 & 0x3f));
	}
}

void executeSpriteBytecode2Sub0Sub0(sSpriteActor* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer puVar4 = param_3->m0;
	if (param_2 < (int)((READ_LE_U16(puVar4) & 0x1ff) + 1)) {
		if ((READ_LE_U16(puVar4) & 0x8000) == 0) {
			u8 bVar1 = READ_LE_U8(puVar4 + READ_LE_U16(puVar4 + param_2 * 2));
			uint uVar7 = 0;
			sPS1Pointer pbVar5 = puVar4 + READ_LE_U16(puVar4 + param_2 * 2) + (bVar1 & 0x3f) * 4 + 6;
			if ((bVar1 & 0x3f) != 0) {
				do {
					while (true) {
						u8 bVar2 = READ_LE_U8(pbVar5);
						if ((bVar2 & 0x80) == 0) break;
						sPS1Pointer pbVar6 = pbVar5 + 1;
						if ((bVar2 & 0x40) == 0) {
							if ((bVar2 & 1) != 0) {
								pbVar6 = pbVar5 + 2;
							}
							pbVar5 = pbVar6;
							if ((bVar2 & 2) != 0) {
								pbVar5 = pbVar5 + 1;
							}
						}
						else {
							if (param_1->m20->m34_perSubgroupTransform == nullptr) {
								param_1->m20->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
								OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
							}
							u32 uVar4 = bVar2 & 7;
							if ((bVar2 & 0x20) != 0) {
								(*param_1->m20->m34_perSubgroupTransform)[uVar4].m0_translateX = READ_LE_U8(pbVar6);
								(*param_1->m20->m34_perSubgroupTransform)[uVar4].m1_translateY = READ_LE_U8(pbVar5 + 2);
								pbVar6 = pbVar5 + 3;
							}
							pbVar5 = pbVar6;
							if ((bVar2 & 0x10) == 0) {
								(*param_1->m20->m34_perSubgroupTransform)[uVar4].m6_rotateZ = 0;
							}
							else {
								bVar2 = READ_LE_U8(pbVar5);
								pbVar5 = pbVar5 + 1;
								(*param_1->m20->m34_perSubgroupTransform)[uVar4].m6_rotateZ = (ushort)bVar2 << 4;
							}
						}
					}
					if ((bVar1 & 0x80) != 0) {
						pbVar5 = pbVar5 + 2;
					}
					uVar7 = uVar7 + 1;
					pbVar5 = pbVar5 + 3;
				} while (uVar7 != (bVar1 & 0x3f));
			}
		}
		else {
			executeSpriteBytecode2Sub0Sub0sub0(param_1, param_2, param_3);
		}
	}
}

void executeSpriteBytecode2Sub0(sSpriteActor* param_1, short param_2)
{
	if ((param_1->m3C & 3) == 1) {
		if (((param_1->m40 >> 0x14 & 1) != 0) && (param_1->m40 = param_1->m40 & 0xffefffff, param_1->m20->m34_perSubgroupTransform != nullptr)) {
			OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
		}
		sSpriteActor* psVar2 = spriteTransfertListHead;
		sSpriteActor* psVar1 = spriteTransfertListHead;
		if ((param_1->m40 >> 0x11 & 1) != 0) {
			while (psVar1 != nullptr) {
				if (psVar1 == param_1) {
					sFieldEntitySub4_110* psVar3 = param_1->m24;
					if (((psVar3 != &sFieldEntitySub4_110_8005a474) && (psVar3 != &sFieldEntitySub4_110_8006be10)) && ((param_1->m40 >> 0x13 & 1) == 0)) {
						executeSpriteBytecode2Sub0Sub0(param_1, param_1->m34, psVar3);
					}
					param_1->m34 = param_2;
					return;
				}
				psVar1 = psVar1->m20->m38;
			}
		}
		param_1->m34 = param_2;
		spriteTransfertListHead = param_1;
		param_1->m40 = param_1->m40 | 0x20000;
		param_1->m20->m38 = psVar2;
	}
	else {
		param_1->m34 = 0;
	}
}

void executeSpriteBytecode2Sub1(sSpriteActor* param_1)
{
	ushort uVar1;

	int uVar2 = param_1->mA8.mxB;
	if (uVar2 < 0) {
		uVar2 = 0;
	}
	uVar1 = READ_LE_U16(param_1->m54 + uVar2 * 2);
	if ((uVar1 & 0x200) == 0) {
		param_1->mAC &= ~8;
	}
	else {
		param_1->mAC |= 8;
	}
	param_1->m3C = param_1->m3C & ~8 | (param_1->mAC >> 3 & 1 ^ param_1->mAC >> 2 & 1) << 3;
	executeSpriteBytecode2Sub0(param_1, uVar1 & 0x1ff);
	return;
}

void pushByteOnAnimationStack(sSpriteActor* param_1, u8 param)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asU8 = param;
}

sSpriteActor* spriteBytecode2ExtendedE0(sSpriteActor* param_1, sPS1Pointer param_2, sFieldEntitySub4_110* param_3)
{
	//assert(0);
	MissingCode();
	return nullptr;
}

void executeSpriteBytecode2Extended92(sSpriteActor* param_1)
{
	if ((param_1->m3C & 3) == 1) {
		int blendMode = param_1->m3C >> 5 & 7;
		if (blendMode != 0) {
			blendMode = blendMode - 1;
		}
		sColorAndCode colorAndcode = param_1->m28_colorAndCode;
		std::vector<sFieldEntitySub4_B4_sub>::iterator psVar1 = param_1->m20->m30->begin();
		int uVar4 = 0;
		if ((param_1->m40 & 0xFF) >> 2 != 0) {
			do {
				psVar1->m10_colorAndCode = colorAndcode;
				psVar1->mA_tpage = psVar1->mA_tpage & 0xff9fU | (ushort)(blendMode << 5);
				uVar4 = uVar4 + 1;
				psVar1 = psVar1 + 1;
			} while (uVar4 != (param_1->m40 & 0xFF) >> 2);
		}
	}
}

void executeSpriteBytecode2Extended(sSpriteActor* param_1, int bytecode, sPS1Pointer param_3)
{
	switch (bytecode & 0xff) {
	case 0x92:
		param_1->m2B |= 1;
		executeSpriteBytecode2Extended92(param_1);
		break;
	case 0x96:
		MissingCode();
		break;
	case 0xB3:
		param_1->mA8.mxB= READ_LE_U8(param_3) & 0x3F;
		break;
	case 0xB4:
		pushByteOnAnimationStack(param_1, READ_LE_U8(param_3));
		break;
	case 0xc6:
		if (param_1->mA8.mx0) {
			param_1->m7C->mC = READ_LE_U16(param_3);
		}
		break;
	case 0xd0:
	case 0xd3:
	case 0xdd:
	case 0xde:
		MissingCode();
		/*pcVar11 = (char*)executeSpriteBytecode2Sub3(param_1, param_3);
		pcVar12 = (char*)executeSpriteBytecode2Sub3(param_1, param_3 + 1);
		*pcVar11 = *pcVar11 + *pcVar12;
		* */
		break;
	case 0xe0:
		spriteBytecode2ExtendedE0(param_1, param_3 + READ_LE_U16(param_3), param_1->m24);
		break;
	case 0xF7:
		MissingCode();
		break;
	case 0xfc:
		MissingCode();
		break;
	default:
		assert(0);

	// default
	case 0xB1:
	case 0xF8:
		break;
	}
}

const std::array<u8, 256> sizePerBytecodeTable = {
	0x10, 0xE7, 0x2E, 0x70,
	0x50, 0x10, 0x3C, 0x70,
	0xFF, 0x7F, 0x50, 0x90,
	0xA0, 0x10, 0x58, 0x10,
	0xFB, 0x50, 0x90, 0x9C,
	0x10, 0x16, 0x50, 0xA0,
	0xE, 0x30, 0xA0, 0x30,
	0x52, 0x30, 0x86, 0x30,
	0x1F, 0xA2, 0xB0, 0x50,
	0xD0, 0x9C, 0x30, 0x90,
	0x30, 0x50, 0x30, 0x19,
	0x0, 0x19, 0x3F, 0x50,
	0x80, 0xBC, 0x30, 0xF2,
	0x30, 0xA0, 0x90, 0x50,
	0x90, 0xF0, 0x90, 0xFF,
	0x7F, 0xFB, 0x3E, 0x31,
	0x50, 0xF0, 0x18, 0x44,
	0x41, 0x22, 0x31, 0x50,
	0x50, 0xA0, 0xF0, 0xF0,
	0x30, 0xFF, 0x8E, 0x31,
	0x12, 0x91, 0xE2, 0x50,
	0xF0, 0xF0, 0xF0, 0xF0,
	0x50, 0xF0, 0x50, 0x70,
	0xA0, 0x50, 0xBF, 0xA2,
	0x70, 0x4E, 0x30, 0xE6,
	0x11, 0x90, 0xF1, 0x90,
	0xD1, 0x30, 0x92, 0xE,
	0x30, 0x22, 0xFE, 0x11,
	0x30, 0x62, 0x2E, 0x52,
	0x4, 0x30, 0x30, 0x12,
	0x32, 0x32, 0x30, 0xF2,
	0x7C, 0x32, 0x0, 0x0,

	// 0x80

	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x2, 0x2, 0x2, 0x2,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3,
	0x3, 0x4, 0x4, 0x4,
	0x4, 0x4, 0x4, 0x4,
	0x4, 0x4, 0x4, 0x4,
	0x4, 0x4, 0x4, 0x4,
};

u8 popByteFromAnimationStack(sSpriteActor* param_1)
{
	return param_1->m8E_stack[param_1->m8C_stackPosition++].asU8;
}

void pushBytecodePointerOnAnimationStack(sSpriteActor* param_1, sPS1Pointer param_2)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asPs1Pointer = param_2;
}

int spriteCallback2Var0 = 0;

void executeSpriteBytecode2(sSpriteActor* param_1)
{
	if (isBattleOverlayLoaded != '\0') {
		assert(0);
		return;
	}

	do
	{
		if (param_1->m9E_wait != 0)
		{
			return;
		}

		sPS1Pointer startOfOpcode = param_1->m64_spriteByteCode;
		u8 bytecode = READ_LE_U8(param_1->m64_spriteByteCode);
		sPS1Pointer pEndOfOpcode = param_1->m64_spriteByteCode + 1;
		if (bytecode < 0x80)
		{
			param_1->m64_spriteByteCode = pEndOfOpcode;

			int waitDelay = 0;

			if (bytecode < 0x10)
			{
				executeSpriteBytecode2Sub0(param_1, param_1->m34 + 1);
				waitDelay = (bytecode & 0xf) + 1;
			}
			else if (bytecode < 0x20)
			{
				param_1->mA8.mxB++;
				executeSpriteBytecode2Sub1(param_1);
				waitDelay = (bytecode & 0xf) + 1;
			}
			else if (bytecode < 0x30)
			{
				param_1->m34--;
				executeSpriteBytecode2Sub1(param_1);
				waitDelay = (bytecode & 0xf) + 1;
			}

			if (bytecode < 0x40)
			{
				waitDelay = (bytecode & 0xf) + 1;
			}

			int iVar12 = waitDelay * (param_1->mAC >> 7 & 0xfff);
			if (iVar12 < 0) {
				iVar12 = iVar12 + 0xff;
			}
			iVar12 = iVar12 >> 8;
			if (iVar12 == 0) {
				iVar12 = 1;
			}

			param_1->m9E_wait += iVar12;
			param_1->mA8.mx16++;

			if (param_1->mA8.mx16 != 0)
			{
				return;
			}

			param_1->mA8.mx16 = 0x3F;
			return;
		}

		switch (bytecode)
		{
		case 0x80:
		case 0x85:
		case 0x87:
		case 0x8E:
		case 0x98:
		case 0xBE:
		case 0xC8:
		case 0xD4:
		case 0xE1:
		case 0xFA:
			assert(0);
			break;
		case 0x81:
			if (((param_1->mAC >> 24) & 0xFF) == 0x3F)
			{
				assert(0);
			}
			param_1->m9E_wait = 0;
			if (param_1->m68) {
				param_1->m68(param_1);
			}
			param_1->mA8.mx1C = 1;
			return;
		case 0x82:
			if (param_1->m68) {
				param_1->m68(param_1);
			}
			{
				int iVar15 = (param_1->mC).vy;
				OP_INIT_ENTITY_SCRIPT_sub0Sub6(param_1, (param_1->mAC >> 24) & 0xFF);
				(param_1->mC).vy = iVar15;
			}
			param_1->m9E_wait = 0;
			executeSpriteBytecode2(param_1);
			return;
		case 0xA7:
			param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
			if ((READ_LE_U8(pEndOfOpcode) & 0x80) != 0) {
				spriteCallback2Var0 = (READ_LE_U8(pEndOfOpcode) & 0x7f) + 1;
				param_1->m9E_wait = param_1->m9E_wait + 1;
				return;
			}
			{
				int iVar13 = READ_LE_U8(pEndOfOpcode + 2) * (param_1->mAC >> 7 & 0xfff);
				if (false) {
					iVar13 = iVar13 + 0xff;
				}
				int sVar11 = (short)((uint)iVar13 >> 8);
				if (iVar13 >> 8 == 0) {
					sVar11 = 1;
				}
				param_1->m9E_wait = param_1->m9E_wait + sVar11;
			}
			return;
		case 0xE2: // Looks like a call
			pushBytecodePointerOnAnimationStack(param_1, param_1->m64_spriteByteCode + 3);
			param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
			break;
		case 0xe4:
			{
				u8 cVar4 = popByteFromAnimationStack(param_1);
				if (cVar4 != 0) {
					pushByteOnAnimationStack(param_1, cVar4 + 0xff);
					param_1->m64_spriteByteCode += (u32)READ_LE_U8(pEndOfOpcode) | (((u32)READ_LE_U8(startOfOpcode + 2) << 0x18) >> 0x10);
				}
			}
			break;
		default:
			executeSpriteBytecode2Extended(param_1, bytecode, pEndOfOpcode);
			param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
			break;
		}
	} while (1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActor* param_1)
{
	short sVar1;
	int iVar2;

	iVar2 = 0;
	if (fieldDrawEnvsInitialized != -1) {
		do {
			sVar1 = param_1->m9E_wait;
			if ((param_1->m9E_wait != 0) && (param_1->m9E_wait = sVar1 + -1, sVar1 == 1)) {
				executeSpriteBytecode2(param_1);
			}
			iVar2 = iVar2 + 1;
		} while (iVar2 != fieldDrawEnvsInitialized + 1);
	}
	return;
}

void setupSpriteObjectMatrix(sSpriteActor* param_1)

{
	if ((param_1->m40 & 1) == 0) {
		VECTOR scale;
		scale.vx = param_1->m20->m6_scale.vx;
		scale.vy = param_1->m20->m6_scale.vy;
		scale.vz = param_1->m20->m6_scale.vz;
		createRotationMatrix(&param_1->m20->m0_rotation, &param_1->m20->mC_spriteMatrix);
		ScaleMatrixL(&param_1->m20->mC_spriteMatrix, &scale);
	}
	else {
		MATRIX scaleMatrix;
		VECTOR scale;

		// make identity
		scaleMatrix.m[0][0] = 0x1000;
		scaleMatrix.m[0][1] = 0;
		scaleMatrix.m[0][2] = 0;
		scaleMatrix.m[1][0] = 0;
		scaleMatrix.m[1][1] = 0x1000;
		scaleMatrix.m[1][0] = 0;
		scaleMatrix.m[2][0] = 0;
		scaleMatrix.m[2][1] = 0;
		scaleMatrix.m[2][2] = 0x1000;
		scaleMatrix.t[0] = 0;
		scaleMatrix.t[1] = 0;
		scaleMatrix.t[2] = 0;

		scale.vx = param_1->m20->m6_scale.vx;
		scale.vy = param_1->m20->m6_scale.vy;
		scale.vz = param_1->m20->m6_scale.vz;
		ScaleMatrixL(&scaleMatrix, &scale);

		MATRIX rotationMatrix;
		createRotationMatrix(&param_1->m20->m0_rotation, &rotationMatrix);
		MulMatrix0(&rotationMatrix, &scaleMatrix, &param_1->m20->mC_spriteMatrix);
	}

	if (param_1->m3A != 0) {
		VECTOR scale;
		scale.vx = param_1->m3A / 2;
		scale.vy = param_1->m3A / 2;
		scale.vz = param_1->m3A / 2;
		ScaleMatrix(&param_1->m20->mC_spriteMatrix, &scale);
	}
}

void initFieldEntitySub4Sub3(sSpriteActor* param_1, int param_2)
{
	sFieldEntitySub4_B4* psVar1;

	psVar1 = param_1->m20;
	if (psVar1 != nullptr) {
		param_1->m2C_scale = param_2;
		psVar1->m6_scale.vx = param_2;
		psVar1->m6_scale.vy = param_2;
		psVar1->m6_scale.vz = param_2;
		param_1->m3C = param_1->m3C | 0x10000000;
	}
}

//header is
//u16 m0: flags
//u16 m2: offset to byte code
//u16 m4: offset to?

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1(sSpriteActor* param_1, const sPS1Pointer param_2)
{
	uint uVar1;
	sFieldEntitySub4_B4* psVar2;
	sFieldEntitySub4_F4* psVar3;
	int iVar5;

	u16 flags = READ_LE_U16(param_2);
	u16 offsetToByteCode = READ_LE_U16(param_2 + 2);
	u16 offset2 = READ_LE_U16(param_2 + 4);

	param_1->m58 = param_2;
	param_1->m64_spriteByteCode = param_2 + offsetToByteCode + 2;
	param_1->mA8.mx14 = flags;
	param_1->m54 = param_2 + offset2 + 4;

	uVar1 = (uint)(flags >> 2) & 0x3f;
	if ((flags >> 2 & 0x20) != 0) {
		uVar1 = uVar1 | 0xffffffc0;
	}

	param_1->m1C = uVar1 * 0x400;
	int iVar4 = (fieldDrawEnvsInitialized + 1) * (fieldDrawEnvsInitialized + 1) * param_1->m82;
	if (iVar4 < 0) {
		iVar4 = iVar4 + 0xfff;
	}
	iVar4 = uVar1 * 0x400 * (iVar4 >> 0xc);
	param_1->m1C = iVar4;

	iVar5 = 0x10000 / (int)(param_1->mAC >> 7 & 0xfff);
	iVar5 = iVar5 * iVar5;
	if (iVar5 < 0) {
		iVar5 = iVar5 + 0xff;
	}
	iVar4 = iVar4 * (iVar5 >> 8);
	param_1->m1C = iVar4;
	if (iVar4 < 0) {
		iVar4 = iVar4 + 0xff;
	}
	param_1->m1C = iVar4 >> 8;


	if ((flags >> 0xb & 1) == 0) {
		param_1->mC.vz = 0;
		param_1->mC.vy = 0;
		param_1->mC.vx = 0;
		param_1->m18 = 0;
	}
	if (param_1->m20)
	{
		if ((flags >> 0xc & 1) == 0) {
			param_1->m20->m0_rotation.vx = 0;
			param_1->m20->m0_rotation.vy = 0;
			param_1->m20->m0_rotation.vz = 0;
			setupSpriteObjectMatrix(param_1);
		}
		if ((flags >> 0xd & 1) == 0) {
			if (isBattleOverlayLoaded != '\0') {
				initFieldEntitySub4Sub3(param_1, (short)OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1_var);
				goto LAB_800236f4;
			}
		}
		else {
		LAB_800236f4:
			if (isBattleOverlayLoaded != '\0') {
				setupSpriteObjectMatrix(param_1);
			}
		}
		if ((param_1->m3C & 3) == 1) {
			param_1->m20->m3D = 0;
			param_1->m20->m3C = 0;
			if (((param_1->m40 >> 0x14 & 1) == 0) && (param_1->m20->m34_perSubgroupTransform != nullptr)) {
				OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
			}
		}
	}
	param_1->m8C_stackPosition = 0x10;
	param_1->m30 = 0;
	param_1->mA8.mx1 = 0;
	param_1->m9E_wait = 1;
	param_1->mA8.mx16 = 0;
	param_1->mA8.mx1C = 0;
	param_1->mA8.mxB = 0x3F;
	param_1->mA8.mx1E = 2;
	if ((param_1->m7C != nullptr) && (param_1->mA8.mx0)) {
		param_1->m7C->m4 = 0;
		param_1->m7C->m0 = 0;
		param_1->m7C->mC = 0;
	}
	return;
}

void executeSpriteBytecode(sSpriteActor* param_1, sPS1Pointer param_2, uint param_3)
{
	int unaff_s3;

	while (true)
	{
		sPS1Pointer pBytecode = param_1->m64_spriteByteCode;
		if ((pBytecode == param_2) && (param_1->mA8.mx16 == param_3)) {
			return;
		}

		u8 bytecode = READ_LE_U8(pBytecode);
		if (bytecode < 0x80)
		{
			param_1->m64_spriteByteCode = pBytecode + 1;
			if (bytecode < 0x10)
			{
				executeSpriteBytecode2Sub0(param_1, param_1->m34 + 1);
				unaff_s3 = (bytecode & 0xf) + 1;
			}
			else if (bytecode < 0x20)
			{
				param_1->mA8.mxB++;
				executeSpriteBytecode2Sub1(param_1);
				unaff_s3 = (bytecode & 0xf) + 1;
			}
			else if (bytecode < 0x30)
			{
				assert(0);
			}

			if (bytecode < 0x40) {
				unaff_s3 = (bytecode & 0xf) + 1;
			}

			param_1->m9E_wait += unaff_s3;
			param_1->mA8.mx16++;
			if (param_1->mA8.mx16 == 0) {
				param_1->mA8.mx16 = 0x3F;
			}
		}
		else
		{
			switch (bytecode)
			{
			case 0x81:
			case 0x82:
				return;
			case 0xB3:
				param_1->mA8.mxB = READ_LE_U8(pBytecode + 1);
				break;
			case 0xA7:
			case 0xB4:
			case 0xE4:
				break;
			default:
				assert(0);
			}

			param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
		}
	}
}

void setSpriteActorAngle(sSpriteActor* param_1, short param_2)
{
	short sVar1;
	ushort uVar2;
	uint originalM17;

	param_1->m80 = param_2;
	originalM17 = param_1->mA8.mx11;
	if (((int)param_2 + 0x400U & 1) == 0) {
		param_1->mAC &= ~4;
	}
	else {
		param_1->mAC |= 4;
	}

	if (param_1->m48 == nullptr) {
		return;
	}

	int newAngle;

	switch (param_1->mA8.mx14)
	{
	case 0:
		if ((param_2 + 0x400U & 0xfff) < 0x801) {
			param_1->mAC &= ~4;
		}
		else {
			param_1->mAC |= 4;
		}
		param_1->mA8.mx11 = 0;
		param_1->m5C = param_1->m58 + 6;
		param_1->m54 = param_1->m58 + READ_LE_U16(param_1->m58 + 4) + 4;
		break;
	case 2:
	{
		newAngle = param_2 + 0x500 >> 9 & 7;
		if (newAngle > 4)
		{
			newAngle = newAngle - 5 ^ 3;
			uVar2 = READ_LE_U16(param_1->m58 + newAngle * 2 + 4);
			param_1->mAC |= 4;
			param_1->m54 = param_1->m58 + (uint)uVar2 + newAngle * 2 + 4;
		}
		else
		{
			uVar2 = READ_LE_U16(param_1->m58 + newAngle * 2 + 4);
			param_1->mAC &= ~4;
			param_1->m54 = param_1->m58 + (uint)uVar2 + newAngle * 2 + 4;
		}
		param_1->mA8.mx11 = newAngle;
		break;
	}
	default:
		assert(0);
	}

	// direction angle changed
	if (originalM17 != param_1->mA8.mx11) {
		sVar1 = param_1->m9E_wait;
		uVar2 = READ_LE_U16(param_1->m58 + 2);
		sPS1Pointer puVar5 = param_1->m64_spriteByteCode;
		param_1->mA8.mx16 = 0;
		param_1->mA8.mxB = 0x3F;
		param_1->m64_spriteByteCode = param_1->m58 + uVar2 + 2;
		executeSpriteBytecode(param_1, puVar5, param_1->mA8.mx16);
		param_1->m9E_wait = sVar1;
	}

	param_1->m3C = param_1->m3C & 0xfffffff7 | (param_1->mAC >> 3 & 1 ^ param_1->mAC >> 2 & 1) << 3;
}

void initFieldEntitySub4Sub5Sub0(sFieldEntitySub4_110* param_1, sFieldActorSetupParams* param_2, sVec2_s16 param_3_vramLocation, sVec2_s16 clut)
{
	param_1->m4_vramLocation = param_3_vramLocation;
	param_1->m8_clut = clut;
	param_1->mC = param_2->mC_pData;
	param_1->m0 = param_2->m8_pData;
	initFieldVar4 = 0;
	param_1->m10 = param_2->m4_pData;

	if (isBattleOverlayLoaded)
	{
		u8 temp = (READ_LE_U16(param_2->m4_pData) >> 6) & 0x3F;
		if (temp)
		{
			initFieldVar5 = temp;
		}
	}
}


void deleteFieldEntitySub4(sSpriteActor* param_1)
{
	MissingCode();
}

void initsFieldEntitySub4_B4(sFieldEntitySub4_B4* pThis)
{
	pThis->m0_rotation.vx = 0;
	pThis->m0_rotation.vy = 0;
	pThis->m0_rotation.vz = 0;
	pThis->m2C = nullptr;
}

int initFieldEntitySub4Sub4(const sPS1Pointer& param_1)
{
	return (READ_LE_U16(param_1) >> 9) & 0x3F;
}

int initFieldEntitySub4Sub5Sub1(const sPS1Pointer& param_1)
{
	return READ_LE_U8(param_1 + 1) >> 7;
}

void initFieldEntitySub4Sub5(sSpriteActor* param_1, sFieldActorSetupParams* param_2)
{
	sFieldEntitySub4_110* psVar2 = param_1->m24;
	if (param_2 != 0) {
		if (param_2 != param_1->m44) {
			initFieldEntitySub4Sub5Sub0(psVar2, param_2, psVar2->m4_vramLocation, psVar2->m8_clut);
			param_1->m44 = param_2;
			param_1->m3C = param_1->m3C | 0x40000000;
		}
		if (isBattleOverlayLoaded != '\0') {
			if (initFieldEntitySub4Sub5Sub1(psVar2->m0) == 0) {
				psVar2->m4_vramLocation.vx = 0x300;
				psVar2->m4_vramLocation.vy = 0x100;
			}
			else {
				psVar2->m4_vramLocation.vx = param_1->m7C->mE_vramLocation.vx;
				psVar2->m4_vramLocation.vy = param_1->m7C->mE_vramLocation.vy;
			}
		}
	}
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6(sSpriteActor* param_1, int param_2)
{
	ushort uVar1;
	uint uVar2;
	int iVar3;

	if (param_1->m48 == nullptr) {
		param_1->m64_spriteByteCode.makeNull();
	}
	else {
		if (param_1->m44 == param_1->m48) {
			uVar2 = param_1->mB0 & 0xfffffbff;
		}
		else {
			uVar2 = param_1->mB0 | 0x400;
		}
		param_1->mB0 = uVar2;
		if (param_2 < 0) {
			initFieldEntitySub4Sub5(param_1, param_1->m4C_specialAnimation);
			if ((isBattleOverlayLoaded != '\0') &&
				(iVar3 = initFieldEntitySub4Sub5Sub1(param_1->m24->m0), iVar3 == 0)) {
				param_1->m24->m4_vramLocation.vx = 0x100;
				param_1->m24->m4_vramLocation.vy = 0x300;
			}
		}
		else {
			initFieldEntitySub4Sub5(param_1, param_1->m48);
			if (isBattleOverlayLoaded != '\0') {
				param_1->m24->m4_vramLocation = param_1->m7C->mE_vramLocation;
			}
		}
		param_1->mAC = (param_1->mAC & 0xFF00FFFF) | (((char)param_2) << 16);
		if ((int)param_2 < 0) {
			param_2 = ~param_2;
		}

		int uVar1 = READ_LE_U16(param_1->m24->m10 + param_2 * 2 + 2);
		param_1->m40 = param_1->m40 | 0x100000;
		param_1->m58 = param_1->m24->m10 + uVar1;
		OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1(param_1, param_1->m58);
		setSpriteActorAngle(param_1, param_1->m80);
	}
}

void initFieldEntitySub4Sub1(sSpriteActor* param_1)
{
	param_1->m3C = 0;
	param_1->m2B = 0x2d;
	param_1->m40 = 0;
	param_1->m3A = 0;
	param_1->m30 = 0;
	param_1->m32 = 0;
	param_1->m34 = 0;
	param_1->mA8.clear();
	param_1->m3C = param_1->m3C & 0xfe00ffe3;
	param_1->m40 = param_1->m40 & 0xfffe0003;
	param_1->mAC = 0;
	param_1->mB0 = 0;
	int iVar1 = (fieldDrawEnvsInitialized + 1) * (fieldDrawEnvsInitialized + 1) * 0x4000 * param_1->m82;
	param_1->mAC = param_1->mAC & 0xfff8007f | 0x8000;
	param_1->mA8.mx11 = 0;
	if (iVar1 < 0) {
		iVar1 = iVar1 + 0xfff;
	}
	param_1->m1C = iVar1 >> 0xc;
	param_1->m64_spriteByteCode.makeNull();
	param_1->m70 = 0;
	param_1->m44 = 0;
	param_1->m68 = 0;
	param_1->m80 = 0;
	param_1->m8C_stackPosition = 0x10;
	param_1->m84 = 0;
	param_1->m6C = (sSpriteActor*)0x0;
	param_1->m50 = 0;
}

void initFieldEntitySub4Sub2(sSpriteActor* pThis)
{
	pThis->m20 = &pThis->mB4;
	initsFieldEntitySub4_B4(&pThis->mB4);
	pThis->m7C = &pThis->mF4;
	pThis->m20->m34_perSubgroupTransform = &pThis->m124;
	pThis->m24 = &pThis->m110;
	pThis->m20->m38 = 0;
}

sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sFieldActorSetupParams* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7)
{
	initFieldEntitySub4Sub1(param_1);
	initFieldEntitySub4Sub2(param_1);
	initFieldEntitySub4Sub3(param_1, 0x10000);

	param_1->m3C = (param_1->m3C & ~0x3) | 1;
	param_1->m40 &= ~0x0001E000;

	if (isBattleOverlayLoaded == 0)
	{
		param_1->mA8.mx0 = 1;
		param_1->m7C->m18.makeNull();
	}
	else
	{
		param_1->mA8.mx0 = 0;
		param_1->m7C->m8 = 0;
		param_1->m7C->mC = 0;
	}

	param_1->m6C = param_1;
	param_1->m3C = param_1->m3C & 0xff00ffff | (initFieldVar2 & 0xf) << 0x14 | (initFieldVar2 & 0xf) << 0x10;;

	int count = initFieldEntitySub4Sub4(pSetup->m8_pData);
	param_1->m20->m2C = param_1->m20->m30 = new std::vector<sFieldEntitySub4_B4_sub>;
	param_1->m20->m30->resize(count);

	param_1->m24->m4_vramLocation.vx = vramX;
	param_1->m24->m4_vramLocation.vy = vramY;
	param_1->m24->m8_clut.vx = clutX;
	param_1->m24->m8_clut.vy = clutY;

	param_1->m48 = pSetup;

	initFieldEntitySub4Sub5(param_1, pSetup);
	param_1->m60 = param_1->m24->m10 + (READ_LE_U16(param_1->m24->m10) & 0x3F) + 1;

	OP_INIT_ENTITY_SCRIPT_sub0Sub6(param_1, 0);

	return param_1;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sSpriteActor* param_1, int param_2, int* param_3, int* param_4, int* param_5)
{
	byte bVar1;
	sPS1Pointer pbVar2;
	int iVar3;
	int iVar4;
	uint uVar5;

	pbVar2 = param_1->m24->m10;
	pbVar2 = pbVar2 + READ_LE_U16(pbVar2 + 2);
	bVar1 = READ_LE_U8(pbVar2 + READ_LE_U16(pbVar2 + 4) + 4);
	uVar5 = (uint)bVar1;
	pbVar2 = param_1->m24->m0;
	iVar3 = uVar5 << 1;
	if (bVar1 != 0) {
		uVar5 = uVar5 - 1;
		iVar3 = uVar5 * 2;
	}
	iVar4 = uVar5 << 1;
	if (READ_LE_U16(pbVar2 + iVar3) < (int)uVar5) {
		iVar4 = 0;
	}
	pbVar2 = pbVar2 + READ_LE_U16(pbVar2 + iVar4 + 2);
	iVar3 = READ_LE_U8(pbVar2 + 3) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*param_4 = iVar3 >> 0xc;
	iVar3 = READ_LE_U8(pbVar2 + 1) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*param_5 = iVar3 >> 0xc;
	iVar3 = READ_LE_U8(pbVar2 + 2) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*param_3 = iVar3 >> 0xc;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sSpriteActor* param1, int param2)
{
	param1->m40 = param1->m40 & 0xffffe0ff | (param2 & 0x1f) << 8;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7Sub0(sSpriteActor* param1)
{
	int iVar1;
	int iVar2;

	iVar2 = ((param1->m18 >> 4) << 8) / (int)(param1->mAC >> 7 & 0xfff);
	iVar1 = getAngleSin((int)param1->m32);
	param1->mC.vx = (iVar1 >> 2) * iVar2 >> 6;
	iVar1 = getAngleCos((int)param1->m32);
	param1->mC.vz = -((iVar1 >> 2) * iVar2) >> 6;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActor* param1, int param2)
{
	param1->m32 = param2;
	OP_INIT_ENTITY_SCRIPT_sub0Sub7Sub0(param1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActor* param1, void(*callback)(sSpriteActor*))
{
	param1->m68 = callback;
}

void fieldActorCallback(sSpriteActor* pThis)
{
	actorArray[pThis->m7C->m14_actorId].m4C_scriptEntity->m4_flags |= 0x10000;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sSpriteActor* param_1, int param_2)
{
	delete param_1->m20->m2C;
	sFieldEntitySub4_B4* psVar2 = param_1->m20;
	psVar2->m2C = psVar2->m30 = new std::vector<sFieldEntitySub4_B4_sub>;
	psVar2->m30->resize(param_2);

	return;
}

sSpriteActor* createSpriteActor(sFieldActorSetupParams* pSetup, int clutX, int clutY, int vramX, int vramY, int param_6)
{
	sSpriteActor* pNewEntry = new sSpriteActor;
	pNewEntry->m86_thisSize = sizeof(sSpriteActor);

	return initializeSpriteActor(pNewEntry, pSetup, clutX, clutY, vramX, vramY, param_6);
}

sSpriteActor* createSpriteActorEX(sFieldActorSetupParams* pSetup, int clutX, int clutY, int vramX, int vramY, int param_6, int param_7)
{
	loadVramSpriteParam = param_7;
	sSpriteActor* pVar1 = createSpriteActor(pSetup, clutX, clutY, vramX, vramY, param_6);
	loadVramSpriteParam = 0;
	return pVar1;
}
