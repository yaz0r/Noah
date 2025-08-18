#include "noahLib.h"
#include "fieldGraphicObject.h"
#include "field.h"
#include "sprite/spriteSetup.h"
#include "kernel/TIM.h"
#include "battle/battle.h"
#include "kernel/3dModel_psxRenderer.h"
#include "validation/gdbConnection.h"

int battleDefaultEntityScale = 0x2000;
int fieldDrawEnvsInitialized = 0;
s8 isBattleOverlayLoaded = 0;
int initFieldVar4 = 0;
int initFieldVar5 = 0;
int loadVramSpriteParam = 0;

sSpriteActorCore* spriteTransfertListHead = nullptr;
sFieldEntitySub4_110 sFieldEntitySub4_110_8005a474;
sFieldEntitySub4_110 sFieldEntitySub4_110_8006be10;

void SetTimeScale(sSpriteActor* param_1, int param_2)
{
    param_1->mAC.mx7_timeScale = param_2;
}

void resetPerSubgroupTransforms(sSpriteActorCore* param_1)
{
	for (int i=0; i<8; i++)
	{
		(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i].m0_translateX = 0;
        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i].m1_translateY = 0;
        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i].m2_rotateX = 0;
        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i].m4_rotateY = 0;
        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i].m6_rotateZ = 0;
	}
}

void executeSpriteBytecode2Sub0Sub0sub0(sSpriteActorCore* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer psVar4 = param_3->m0_spriteData->rawPointer + READ_LE_U16(param_3->m0_spriteData->rawPointer + param_2 * 2);
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
					if (param_1->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
						param_1->m20->getAsSprite()->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
						resetPerSubgroupTransforms(param_1);
					}
					if ((bVar2 & 0x20) != 0) {
						(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar6].m0_translateX = READ_LE_U8(pbVar5);
						(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar6].m1_translateY = READ_LE_U8(pbVar4 + 2);
						pbVar5 = pbVar4 + 3;
					}
					pbVar4 = pbVar5;
					if ((bVar2 & 0x10) == 0) {
						(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar6].m6_rotateZ = 0;
					}
					else {
						(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar6].m6_rotateZ = (ushort)READ_LE_U8(pbVar4) << 4;
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

void executeSpriteBytecode2Sub0Sub0(sSpriteActorCore* param_1, int frameId, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer puVar4 = param_3->m0_spriteData->rawPointer;
	if (frameId < param_3->m0_spriteData->m0_header.mx01FF_frameCount + 1) {
		if ((READ_LE_U16(puVar4) & 0x8000) == 0) {
			u8 bVar1 = READ_LE_U8(puVar4 + READ_LE_U16(puVar4 + frameId * 2));
			uint uVar7 = 0;
			sPS1Pointer pbVar5 = puVar4 + READ_LE_U16(puVar4 + frameId * 2) + (bVar1 & 0x3f) * 4 + 6;
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
							if (param_1->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
								param_1->m20->getAsSprite()->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
								resetPerSubgroupTransforms(param_1);
							}
							u32 uVar4 = bVar2 & 7;
							if ((bVar2 & 0x20) != 0) {
								(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar4].m0_translateX = READ_LE_U8(pbVar6);
								(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar4].m1_translateY = READ_LE_U8(pbVar5 + 2);
								pbVar6 = pbVar5 + 3;
							}
							pbVar5 = pbVar6;
							if ((bVar2 & 0x10) == 0) {
								(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar4].m6_rotateZ = 0;
							}
							else {
								bVar2 = READ_LE_U8(pbVar5);
								pbVar5 = pbVar5 + 1;
								(*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar4].m6_rotateZ = (ushort)bVar2 << 4;
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
			executeSpriteBytecode2Sub0Sub0sub0(param_1, frameId, param_3);
		}
	}
}

void addToSpriteTransferList(sSpriteActorCore* param_1, short param_2)
{
    VALIDATE_FIELD(FCT_KernelSpriteVM, 0x8001d2b0);
    VALIDATE_REG(FCT_KernelSpriteVM, A1, param_2);

	if ((param_1->m3C & 3) == 1) {
        if (param_1->m40 & 0x00100000) {
            param_1->m40 &= ~0x00100000;
            if (param_1->m20->getAsSprite()->m34_perSubgroupTransform != nullptr) {
                resetPerSubgroupTransforms(param_1);
            }
        }

        // if already in shape transfer list
		if (param_1->m40 & 0x20000) {
            sSpriteActorCore* pCurrentHead = spriteTransfertListHead;

            // find it in the list
			while (pCurrentHead != nullptr) {
				if (pCurrentHead == param_1) {
					sFieldEntitySub4_110* psVar3 = param_1->m24_vramData;
					if (((psVar3 != &sFieldEntitySub4_110_8005a474) && (psVar3 != &sFieldEntitySub4_110_8006be10)) && ((param_1->m40 >> 0x13 & 1) == 0)) {
						executeSpriteBytecode2Sub0Sub0(param_1, param_1->m34_currentSpriteFrame, psVar3);
					}
					param_1->m34_currentSpriteFrame = param_2;
					return;
				}
				pCurrentHead = pCurrentHead->m20->getAsSprite()->m38_pNext;
			}
		}
		param_1->m34_currentSpriteFrame = param_2;
		param_1->m40 |= 0x20000;
		param_1->m20->getAsSprite()->m38_pNext = spriteTransfertListHead;
        spriteTransfertListHead = param_1;
	}
	else {
		param_1->m34_currentSpriteFrame = 0;
	}
}

void executeSpriteBytecode2Sub1(sSpriteActorCore* param_1)
{
	ushort uVar1;

	int uVar2 = param_1->mA8.mxB;
	if (uVar2 < 0) {
		uVar2 = 0;
	}
	uVar1 = READ_LE_U16(param_1->m54 + uVar2 * 2);
	if ((uVar1 & 0x200) == 0) {
		param_1->mAC.mx3 = 0;
	}
	else {
        param_1->mAC.mx3 = 1;
	}
	param_1->m3C = param_1->m3C & ~8 | (param_1->mAC.mx3 ^ param_1->mAC.mx2_facing) << 3;
	addToSpriteTransferList(param_1, uVar1 & 0x1ff);
	return;
}

void pushByteOnAnimationStack(sSpriteActorCore* param_1, u8 param)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asU8 = param;
}

u16 getSavePointCreationMode1(std::span<u8>::iterator param_1)
{
    u16 uVar1;

    uVar1 = READ_LE_U16(param_1) >> 8 & 7;
    if ((READ_LE_U16(param_1) >> 0xe & 1) != 0) {
        uVar1 = uVar1 + 8;
    }
    return uVar1;
}

u32 getModeForSavePointMesh(u32 param_1)
{
    switch (param_1) {
    default:
        return 1;
        break;
    case 1:
    case 4:
    case 8:
    case 9:
        return 0;
        break;
    case 2:
    case 7:
    case 0xf:
        return 2;
    }
}

int modulateSpeed(sSpriteActorCore* param_1, int param_2)
{
    int iVar1;

    if ((ushort)param_1->m3A != 0) {
        iVar1 = param_2 * (uint)(ushort)param_1->m3A;
        if (iVar1 < 0) {
            iVar1 = iVar1 + 0x3ff;
        }
        param_2 = iVar1 >> 10;
    }
    return param_2;
}

void savePointCallback8Sub0Sub0_battle(sSpriteActorCore* param_1) {
    assert(isBattleOverlayLoaded);

    MissingCode();
    /*
    SVECTOR local_28;
    local_28.vx = param_1->m0_position.vx.getIntegerPart();
    local_28.vy = param_1->m0_position.vy.getIntegerPart();
    local_28.vz = param_1->m0_position.vz.getIntegerPart();
    uVar1 = battleSpriteOp89Sub0(&local_28, param_1->m78, 4, in_a3);
    if ((int)uVar1 < 0) {
        uVar1 = battleSpriteOp89Sub1(&local_28);
    }
    battleSpriteOp89Sub2(&local_28, uVar1, auStack_20);
    param_1->m78 = uVar1;
    param_1->m84_maxY = local_28.vy;
    */
}

void savePointCallback8Sub0Sub0(sSpriteActorCore* param_1)
{
    if ((param_1->m3C >> 0x1a & 1) != 0) {
        (param_1->m0_position).vy += modulateSpeed(param_1, (param_1->mC_step).vy >> 4) * 0x10;
        (param_1->mC_step).vy += param_1->m1C_gravity;
        return;
    }

    assert(isBattleOverlayLoaded);

    short sVar1;
    int iVar2;
    int iVar3;

    savePointCallback8Sub0Sub0_battle(param_1);
    iVar3 = (param_1->mC_step).vy;
    iVar2 = iVar3 >> 4;
    if ((iVar3 < 1) || (param_1->m1C_gravity < 1)) {
        iVar3 = modulateSpeed(param_1, iVar2);
        iVar3 = (param_1->m0_position).vy + iVar3 * 0x10;
        sVar1 = param_1->m84_maxY;
        (param_1->m0_position).vy = iVar3;
        if ((int)sVar1 <= iVar3 >> 0x10) {
            (param_1->m0_position).vy = (int)sVar1 << 0x10;
        }
    }
    else {
        if (param_1->m0_position.vy.getIntegerPart() == param_1->m84_maxY) {
            return;
        }
        iVar3 = modulateSpeed(param_1, iVar2);
        sVar1 = param_1->m84_maxY;
        iVar3 = (param_1->m0_position).vy + iVar3 * 0x10;
        (param_1->m0_position).vy = iVar3;
        if ((int)sVar1 <= iVar3 >> 0x10) {
            iVar3 = -(param_1->mC_step).vy * ((uint)param_1->mA8.mx1 & 0x3ff);
            (param_1->m0_position).vy = (int)sVar1 << 0x10;
            if (iVar3 < 0) {
                iVar3 = iVar3 + 0xff;
            }
            iVar2 = param_1->m1C_gravity;
            iVar3 = iVar3 >> 8;
            (param_1->mC_step).vy = iVar3;
            if (iVar3 < 0) {
                iVar3 = -iVar3;
            }
            if (iVar2 < 0) {
                iVar2 = -iVar2;
            }
            if (iVar2 <= iVar3) {
                return;
            }
            (param_1->mC_step).vy = 0;
            return;
        }
    }
    (param_1->mC_step).vy += param_1->m1C_gravity;
    return;
}

void savePointCallback8Sub0(sSpriteActorCore* param_1)
{
    int iVar1;
    int iVar2;

    iVar1 = modulateSpeed(param_1, (param_1->mC_step).vx >> 4);
    iVar2 = (param_1->mC_step).vz;
    (param_1->m0_position).vx = (param_1->m0_position).vx + iVar1 * 0x10;
    iVar1 = modulateSpeed(param_1, iVar2 >> 4);
    (param_1->m0_position).vz = (param_1->m0_position).vz + iVar1 * 0x10;
    savePointCallback8Sub0Sub0(param_1);
    return;
}

void setupSpriteObjectMatrix(sSpriteActorCore* param_1)

{
    if ((param_1->m40 & 1) == 0) {
        VECTOR scale;
        scale.vx = param_1->m20->m6_scale.vx;
        scale.vy = param_1->m20->m6_scale.vy;
        scale.vz = param_1->m20->m6_scale.vz;
        createRotationMatrix(&param_1->m20->m0_rotation, &param_1->m20->mC_matrix);
        ScaleMatrixL(&param_1->m20->mC_matrix, &scale);
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
        MulMatrix0(&rotationMatrix, &scaleMatrix, &param_1->m20->mC_matrix);
    }

    if (param_1->m3A != 0) {
        VECTOR scale;
        scale.vx = param_1->m3A / 2;
        scale.vy = param_1->m3A / 2;
        scale.vz = param_1->m3A / 2;
        ScaleMatrix(&param_1->m20->mC_matrix, &scale);
    }
}

void spriteCallback_render2_updateMatrix(sSpriteActorCore* param_1)
{
    if ((param_1->m3C >> 0x1c & 1) != 0) {
        setupSpriteObjectMatrix(param_1);
        param_1->m3C = param_1->m3C & 0xefffffff;
    }
    return;
}

void spriteCallback_render8(sTaskHeader* param_1) {
    assert(0);
}

void spriteCallback_render9(sTaskHeader* param_1) {
    MissingCode();
}

void spriteCallback_render2(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteActor = ((sSpriteActorCore*)param_1->m4);

    spriteCallback_render2_updateMatrix(pSpriteActor);

    if (pSpriteActor->m20->getAsObject()->m34_pModelBlock) {
        VECTOR temp;
        temp[0] = pSpriteActor->m0_position.vx.getIntegerPart();
        temp[1] = pSpriteActor->m0_position.vy.getIntegerPart();
        temp[2] = pSpriteActor->m0_position.vz.getIntegerPart();

        TransMatrix(&pSpriteActor->m20->getAsObject()->mC_matrix, &temp);

        MATRIX MStack56;
        if (((pSpriteActor->m3C >> 24) & 1) == 0) {
            CompMatrix(&currentRenderingMatrix, &pSpriteActor->m20->getAsObject()->mC_matrix, &MStack56);
        }

        SetRotMatrix(&MStack56);
        SetTransMatrix(&MStack56);
        submitModelForRendering(pSpriteActor->m20->getAsObject()->m34_pModelBlock, pSpriteActor->m20->getAsObject()->m2C[shapeTransfertDoubleBufferIndex], characterRenderingOT->begin(), (pSpriteActor->m40 >> 16) & 4);
    }
}

void savePointCallback8(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteActor = ((sSpriteActorCore*)param_1->m4);

    OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSpriteActor);
    savePointCallback8Sub0(pSpriteActor);

    if (pSpriteActor->m64_spriteByteCode.has_value()) {
        if (pSpriteActor->mAC.mx6 == 0) {
            return;
        }

        OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSpriteActor);
        savePointCallback8Sub0(pSpriteActor);
        if (pSpriteActor->m64_spriteByteCode.has_value()) {
            return;
        }
    }
    param_1->mC_deleteCallback(param_1);
}

void savePointCallbackC_sub1(sSpriteActorCore* param_1) {
    sSpriteActorCore* psVar1;
    sSpriteActorCore* psVar2;
    sSpriteActorCore* psVar3;

    psVar1 = spriteTransfertListHead;
    psVar2 = (sSpriteActor*)0x0;
    if (spriteTransfertListHead != (sSpriteActor*)0x0) {
        do {
            psVar3 = psVar1;
            if (psVar1 == param_1) {
                psVar3 = psVar2;
                if (psVar2 == (sSpriteActor*)0x0) {
                    spriteTransfertListHead = psVar2->m20->getAsSprite()->m38_pNext;
                }
                else {
                    psVar2->m20->getAsSprite()->m38_pNext = psVar1->m20->getAsSprite()->m38_pNext;
                }
            }
            psVar1 = psVar1->m20->getAsSprite()->m38_pNext;
            psVar2 = psVar3;
        } while (psVar1 != (sSpriteActor*)0x0);
    }
}

void savePointCallbackC(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteActor = ((sSpriteActorCore*)param_1->m4);

    if (pSpriteActor->m20) {
        if (pSpriteActor->m20->getAsSprite()->m2C) {
            //clearShapeTransferEntry(pSpriteActor->m20->getAsSprite()->m2C);
            assert(0);
        }
    }

    if ((pSpriteActor->m3C & 3) == 1) {
        if (pSpriteActor->m20->getAsSprite()->m34_perSubgroupTransform) {
            delete pSpriteActor->m20->getAsSprite()->m34_perSubgroupTransform;
            pSpriteActor->m20->getAsSprite()->m34_perSubgroupTransform = nullptr;
        }
    }

    if (pSpriteActor->mAC.mx5) {
        registerSpriteCallback2_2(param_1);
    }

    if ((pSpriteActor->mB0.mRaw >> 0xB) & 1) {
        assert(0);
    }

    if ((pSpriteActor->m3C & 3) == 1) {
        savePointCallbackC_sub1(pSpriteActor);
    }

    allocateSavePointMeshDataSub0_callback(param_1);
    registerSpriteCallback2Sub0(param_1 + 1);
    delete param_1;
}

void initFieldEntitySub4Sub1(sSpriteActorCore* param_1)
{
    param_1->m3C = 0;
    param_1->m28_colorAndCode.m3_code = 0x2d;
    param_1->m40 = 0;
    param_1->m3A = 0;
    param_1->m30 = 0;
    param_1->m32_direction = 0;
    param_1->m34_currentSpriteFrame = 0;
    param_1->mA8.clear();
    param_1->m3C = param_1->m3C & 0xfe00ffe3;
    param_1->m40 = param_1->m40 & 0xfffe0003;
    param_1->mAC.mRaw = 0;
    param_1->mB0.mRaw = 0;
    int iVar1 = (fieldDrawEnvsInitialized + 1) * (fieldDrawEnvsInitialized + 1) * 0x4000 * param_1->m82;
    param_1->mAC.mx7_timeScale = 0x100;
    param_1->mA8.mx11 = 0;
    if (iVar1 < 0) {
        iVar1 = iVar1 + 0xfff;
    }
    param_1->m1C_gravity = iVar1 >> 0xc;
    param_1->m64_spriteByteCode.reset();
    param_1->m70 = 0;
    param_1->m44_currentAnimationBundle = 0;
    param_1->m68 = 0;
    param_1->m80 = 0;
    param_1->m8C_stackPosition = 0x10;
    param_1->m84_maxY = 0;
    param_1->m6C_pointerToOwnerStructure = nullptr;
    param_1->m50.reset();
}

sSavePointMeshAbstract* allocateSavePointMeshData(sSavePointMeshAbstract* pThis, sSavePointMesh1* param_1)
{
    sSavePointMeshAbstract* pvVar1 = pThis;
    allocateSavePointMeshDataSub0(param_1, pvVar1);
    registerSpriteCallback2(pvVar1, &pvVar1->m1C);

    initFieldEntitySub4Sub1(&pvVar1->m38_spriteActorCore);

    pvVar1->m4 = &pvVar1->m38_spriteActorCore;
    pvVar1->m1C.m4 = &pvVar1->m38_spriteActorCore;

    setTaskUpdateFunction(pvVar1, savePointCallback8);
    setTaskDeleteFunction(pvVar1, savePointCallbackC);

    return pvVar1;
}

void initsFieldEntitySub4_B4(sFieldEntitySub4_B4* pThis)
{

    pThis->m0_rotation.vx = 0;
    pThis->m0_rotation.vy = 0;
    pThis->m0_rotation.vz = 0;
    pThis->m2C = nullptr;
}

void initsFieldEntitySub4_B4(sFieldEntitySub4_B4_alt* pThis)
{

    pThis->m0_rotation.vx = 0;
    pThis->m0_rotation.vy = 0;
    pThis->m0_rotation.vz = 0;
    //pThis->m2C.clear();
}
void createSavePointMeshDataMode0(sSpriteActorCore* param_1) {
    param_1->m20 = nullptr;
}

void createSavePointMeshDataMode1(sSavePointMesh1* param_1)
{
    (param_1->m38_spriteActorCore).m20 = &param_1->mB4;
    initsFieldEntitySub4_B4(&param_1->mB4);
    (param_1->m38_spriteActorCore).m20->getAsSprite()->m30 = &param_1->mF4;
    (param_1->m38_spriteActorCore).m20->getAsSprite()->m34_perSubgroupTransform = nullptr;
    (param_1->m38_spriteActorCore).m20->getAsSprite()->m38_pNext = nullptr;
}

void createSavePointMeshDataMode2(sSavePointMesh2* param_1)
{
    (param_1->m38_spriteActorCore).m20 = &param_1->mB4;
    initsFieldEntitySub4_B4(&param_1->mB4);
    (param_1->m38_spriteActorCore).m20->getAsObject()->m34_pModelBlock = nullptr;
    (param_1->m38_spriteActorCore).m20->getAsObject()->m40 = 0;
}

int isVramPreBacked(const sFieldEntitySub4_110_0* param_1)
{
    return param_1->m0_header.mx8000_isVramPrebacked;
}

int initFieldEntitySub4Sub4(const sFieldEntitySub4_110_0* param_1)
{
    return param_1->m0_header.m_0x7E00;
}

sFieldEntitySub4_110 createSavePointMeshData_mode5;

sSavePointMeshAbstract* createSavePointMeshData(int mode1, int mode2, sFieldEntitySub4_110* param_3, int param_4, sSavePointMesh1* param_5)
{
    // NOTE: this is kinda different from the original code, that was handcrafting those various structure in a very hackish way
    sSavePointMeshAbstract* pNewSavePoint = nullptr;
    switch (mode2) {
    case 0: // used in battle when hitting
    {
        // todo: figure that one out too
        sSavePointMeshAbstract* pNewSavePoint0 = new sSavePointMeshAbstract;
        pNewSavePoint = allocateSavePointMeshData(pNewSavePoint0, param_5);
        createSavePointMeshDataMode0(&pNewSavePoint0->m38_spriteActorCore);
        break;
    }
    break;
    case 1: // this is used for sprite, especially in battle for attack sprite special effects
        if (mode1 == 5) {
            param_3 = &createSavePointMeshData_mode5;
        }
        if (mode1 == 6) {
            param_3 = &sFieldEntitySub4_110_8005a474;
        }
        {
            sSavePointMesh1* pNewSavePoint1 = new sSavePointMesh1;
            int numF4 = initFieldEntitySub4Sub4(param_3->m0_spriteData)/* - 1*/;
            pNewSavePoint = allocateSavePointMeshData(pNewSavePoint1, param_5);
            pNewSavePoint1->mF4.resize(numF4); // TODO: HACK! removed the -1 here. should have been 'iVar2 = (iVar2 + -1) * 0x18 + 0x58;'
            createSavePointMeshDataMode1(pNewSavePoint1);
            break;
        }
    case 2: // this is used by field 3d objects like save points
    {
        // todo: this is a different struct without the last F4 array
        sSavePointMesh2* pNewSavePoint2 = new sSavePointMesh2;
        pNewSavePoint = allocateSavePointMeshData(pNewSavePoint2, param_5);
        createSavePointMeshDataMode2(pNewSavePoint2);
    }
    break;
    default:
        assert(0);
    }

    pNewSavePoint->m38_spriteActorCore.m6C_pointerToOwnerStructure = pNewSavePoint;
    pNewSavePoint->m38_spriteActorCore.m86_thisSize = mode2;
    pNewSavePoint->m38_spriteActorCore.m24_vramData = param_3;

    return pNewSavePoint;
}

void spriteCallback_render0(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteSheet = ((sSpriteActorCore*)param_1->m4);
    if ((!pSpriteSheet->mB0.mx8) || !battleSpritesDisabled) {
        SVECTOR local_30;
        local_30.vx = pSpriteSheet->m0_position.vx.getIntegerPart();
        local_30.vy = pSpriteSheet->m0_position.vy.getIntegerPart();
        local_30.vz = pSpriteSheet->m0_position.vz.getIntegerPart();

        SetRotMatrix(&currentRenderingMatrix);
        SetTransMatrix(&currentRenderingMatrix);
        sVec2_s16 dummy1;
        long dummy2;
        long flags;
        int depth = RotTransPers(&local_30, &dummy1, &dummy2, &flags);
        int OTIndex = (depth >> (gDepthDivider & 0x1f)) + (int)pSpriteSheet->m30;
        if (flags & 0x8000) {
            OTIndex = 0;
        }
        pSpriteSheet->m2E = OTIndex;
        if ((pSpriteSheet->m3C >> 0x18 & 1) == 0) {
            if ((pSpriteSheet->m3C >> 0x1d & 1) != 0) {
                OTIndex = (int)pSpriteSheet->m70->m2E;
            }
            if (OTIndex - 1U < 0xfff) {
                renderSpriteActor(pSpriteSheet, &(*characterRenderingOT)[OTIndex]);
            }
        }
        else {
            assert(0);
        }
    }
}

const std::vector<void (*)(sTaskHeader*)> spriteBytecode2ExtendedE0_Sub0Sub0_callback = { {
        spriteCallback_render0,
        nullptr,
        spriteCallback_render2,
        nullptr,
        nullptr,
        spriteCallback_render0,
        spriteCallback_render0,
        spriteCallback_render2,
        spriteCallback_render8,
        spriteCallback_render9,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
} };

void spriteBytecode2ExtendedE0_Sub0Sub0(sTaskHeader* param_1, int param_2)
{
    setTaskDrawFunction(param_1, spriteBytecode2ExtendedE0_Sub0Sub0_callback[param_2]);
    return;
}

void spriteBytecode2ExtendedE0_Sub0(sSavePointMeshAbstract* param_1)
{
    u32 mode = param_1->m38_spriteActorCore.m40 >> 0xd & 0xf;
    switch (mode)
    {
    case 0:
    case 2:
    case 4:
    case 5:
        break;
    case 9:
        param_1->m38_spriteActorCore.m36 = 3;
        param_1->m38_spriteActorCore.m28_colorAndCode.m3_code = 0x60;
        param_1->m38_spriteActorCore.m34_currentSpriteFrame = 1;
        break;
    case 10:
        param_1->m38_spriteActorCore.m34_currentSpriteFrame = 0;
        spriteBytecode2ExtendedE0_Sub0_10_battle(param_1);
        param_1->m38_spriteActorCore.m0_position.vx = previousCameraEye2.vx;
        param_1->m38_spriteActorCore.m0_position.vy = previousCameraEye2.vy;
        param_1->m38_spriteActorCore.m0_position.vz = previousCameraEye2.vz;
        break;
    case 11:
        param_1->m38_spriteActorCore.m34_currentSpriteFrame = 0;
        spriteBytecode2ExtendedE0_Sub0_10_battle(param_1);
        param_1->m38_spriteActorCore.m0_position.vx = previousCameraAt2.vx;
        param_1->m38_spriteActorCore.m0_position.vy = previousCameraAt2.vy;
        param_1->m38_spriteActorCore.m0_position.vz = previousCameraAt2.vz;
        break;
    default:
        assert(0);
        break;
    }
    spriteBytecode2ExtendedE0_Sub0Sub0(&param_1->m1C, mode);
}

void spriteBytecode2ExtendedE0(sSpriteActorCore* param_1, std::span<u8>::iterator param_2, sFieldEntitySub4_110* param_3)
{
    u8 oldSpriteBytecode2ExtendedE0_Var0 = spriteBytecode2ExtendedE0_Var0;
    auto b0_flag = param_1->mB0;
    param_1->mB0.mx10 = 1;
    if (b0_flag.mx8) {
        spriteBytecode2ExtendedE0_Var0 = 0;
    }
    u32 savePointCreationMode1 = getSavePointCreationMode1(param_2);
    if (savePointCreationMode1 == 3) {
        savePointCreationMode1 = param_1->m40 >> 0xd & 0xf;
    }
    u32 savePointCreationMode2 = getModeForSavePointMesh(savePointCreationMode1);

    sSavePointMeshAbstract* pSavePointMesh = createSavePointMeshData(savePointCreationMode1, savePointCreationMode2, param_3, 0, (sSavePointMesh1*)param_1->m6C_pointerToOwnerStructure);
    pSavePointMesh->m14 |= 0x20000000;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffe1fff | (savePointCreationMode1 & 0xf) << 0xd;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffffc | savePointCreationMode2 & 3;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffe00ff | (savePointCreationMode1 & 0xf) << 0xd | param_1->m40 & 0x1f00;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffff7 | param_1->m3C & 8;


    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xffffffe7 | param_1->m3C & 8 | param_1->m3C & 0x10;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xffff00ff | param_1->m3C & 0xFF00;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffbffff | param_1->m40 & 0x40000;

    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffffb | 0x4000000;
    pSavePointMesh->m38_spriteActorCore.m18_moveSpeed = param_1->m18_moveSpeed;
    pSavePointMesh->m38_spriteActorCore.m32_direction = param_1->m32_direction;
    pSavePointMesh->m38_spriteActorCore.m2C_scale = param_1->m2C_scale;
    pSavePointMesh->m38_spriteActorCore.m34_currentSpriteFrame = param_1->m34_currentSpriteFrame;
    pSavePointMesh->m38_spriteActorCore.mB0.mx9 = param_1->mB0.mx9;

    if (param_1->mB0.mx9) {
        pSavePointMesh->m38_spriteActorCore.m3A = param_1->m3A;
        pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xffffe0ff | 0x300;
    }

    pSavePointMesh->m38_spriteActorCore.mA8.mx1E_entityId_bottom2bit = param_1->mA8.mx1E_entityId_bottom2bit;
    pSavePointMesh->m38_spriteActorCore.mAC.mx0_entityIdUpper2bit = pSavePointMesh->m38_spriteActorCore.mAC.mx0_entityIdUpper2bit;
    pSavePointMesh->m38_spriteActorCore.mB0.mx8 = param_1->mB0.mx8;
    pSavePointMesh->m38_spriteActorCore.mAC.mx6 = pSavePointMesh->m38_spriteActorCore.mAC.mx6;
    pSavePointMesh->m38_spriteActorCore.mAC.mx7_timeScale = pSavePointMesh->m38_spriteActorCore.mAC.mx7_timeScale;
    pSavePointMesh->m38_spriteActorCore.mA8.mx0 = 0;
    pSavePointMesh->m38_spriteActorCore.mAC.mx2_facing = pSavePointMesh->m38_spriteActorCore.mAC.mx2_facing;

    if (param_1->mA8.mx0 == 0) {
        pSavePointMesh->m38_spriteActorCore.m7C = param_1->m7C;
    }
    else {
        pSavePointMesh->m38_spriteActorCore.m7C = nullptr;
    }

    pSavePointMesh->m38_spriteActorCore.m70 = param_1;
    pSavePointMesh->m38_spriteActorCore.m44_currentAnimationBundle = param_1->m44_currentAnimationBundle;
    pSavePointMesh->m38_spriteActorCore.m48_defaultAnimationbundle = param_1->m48_defaultAnimationbundle;
    pSavePointMesh->m38_spriteActorCore.m74_pTargetEntitySprite = param_1->m74_pTargetEntitySprite;
    pSavePointMesh->m38_spriteActorCore.m82 = param_1->m82;
    pSavePointMesh->m38_spriteActorCore.m50 = param_1->m50;
    pSavePointMesh->m38_spriteActorCore.m8D = param_1->mAC.mx18;
    pSavePointMesh->m38_spriteActorCore.m78 = param_1->m78;
    pSavePointMesh->m38_spriteActorCore.m0_position.vx = param_1->m0_position.vx;
    pSavePointMesh->m38_spriteActorCore.m0_position.vy = param_1->m0_position.vy;
    pSavePointMesh->m38_spriteActorCore.m0_position.vz = param_1->m0_position.vz;
    pSavePointMesh->m38_spriteActorCore.mC_step.vx = param_1->mC_step.vx;
    pSavePointMesh->m38_spriteActorCore.mC_step.vy = param_1->mC_step.vy;
    pSavePointMesh->m38_spriteActorCore.mC_step.vz = param_1->mC_step.vz;
    if (savePointCreationMode2 != 0) {
        (pSavePointMesh->m38_spriteActorCore.m20)->m0_rotation[0] = (param_1->m20)->m0_rotation[0];
        (pSavePointMesh->m38_spriteActorCore.m20)->m0_rotation[1] = (param_1->m20)->m0_rotation[1];
        (pSavePointMesh->m38_spriteActorCore.m20)->m0_rotation[2] = (param_1->m20)->m0_rotation[2];
        (pSavePointMesh->m38_spriteActorCore.m20)->m6_scale[0] = (param_1->m20)->m6_scale[0];
        (pSavePointMesh->m38_spriteActorCore.m20)->m6_scale[1] = (param_1->m20)->m6_scale[1];
        (pSavePointMesh->m38_spriteActorCore.m20)->m6_scale[2] = (param_1->m20)->m6_scale[2];
    }

    setCurrentAnimationPtr(&pSavePointMesh->m38_spriteActorCore, param_2);
    spriteBytecode2ExtendedE0_Sub0(pSavePointMesh);
    spriteBytecode2ExtendedE0_Var0 = oldSpriteBytecode2ExtendedE0_Var0;	
}

void updateAllSubsprites(sSpriteActorCore* param_1)
{
	if ((param_1->m3C & 3) == 1) {
		int blendMode = param_1->m3C >> 5 & 7;
		if (blendMode != 0) {
			blendMode = blendMode - 1;
		}
		sColorAndCode colorAndcode = param_1->m28_colorAndCode;
		std::vector<sFieldEntitySub4_B4_sub>::iterator psVar1 = param_1->m20->getAsSprite()->m30->begin();
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

void computeStepFromMoveSpeed(sSpriteActorCore* param1)
{
	int iVar1;
	int iVar2;

	iVar2 = ((param1->m18_moveSpeed >> 4) << 8) / (int)(param1->mAC.mx7_timeScale);
	param1->mC_step.vx = (getAngleSin((int)param1->m32_direction) >> 2) * iVar2 >> 6;
	param1->mC_step.vz = -((getAngleCos((int)param1->m32_direction) >> 2) * iVar2) >> 6;
}

sStackElement* executeSpriteBytecode2Sub3(sSpriteActorCore* param_1, sPS1Pointer param_2)
{
	byte bVar1;

	bVar1 = READ_LE_U8(param_2);
	if ((bVar1 & 0x80) == 0) {
		return &param_1->m8E_stack[(int)param_1->m8C_stackPosition + (int)(char)bVar1];
	}
	else {
		assert(0);// pcVar2 = (char*)(*(int*)&param_1->field_0x88 + (bVar1 & 0x7f));
	}
	return nullptr;
}

u16 customVramUploadX;
u16 customVramUploadY;
std::span<u8>::iterator customVramUploadPtr;

void uploadTextureToVram(std::span<u8>::iterator param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY)
{
    int textureCount = READ_LE_U8(param_1);
    std::span<u8>::iterator psVar2 = param_1 + textureCount * 4 + 4;
    for (int i = 0; i < textureCount; i++) {
        RECT uploadRect[2];
        switch (READ_LE_U32(psVar2)) {
        case 0x1100:
            switch (param_2) {
            case 0:
                uploadRect[0].x = READ_LE_U16(psVar2 + 4) + READ_LE_U16(psVar2 + 8);
                uploadRect[0].y = READ_LE_U16(psVar2 + 6) + READ_LE_U16(psVar2 + 10);
                break;
            case 1:
                uploadRect[0].x = tpageX + READ_LE_U16(psVar2 + 8);
                uploadRect[0].y = tpageY + READ_LE_U16(psVar2 + 10);
                break;
            default:
                assert(0);
                break;
            }
            break;
        case 0x1101:
            switch (param_5)
            {
            case 0:
                uploadRect[0].x = READ_LE_U16(psVar2 + 4) + READ_LE_U16(psVar2 + 8);
                uploadRect[0].y = READ_LE_U16(psVar2 + 6) + READ_LE_U16(psVar2 + 10);
                break;
            case 1:
                uploadRect[0].x = clutX + READ_LE_U16(psVar2 + 8);
                uploadRect[0].y = clutY + READ_LE_U16(psVar2 + 10);
                break;
            default:
                assert(0);
                break;
            }
            break;
        default:
            assert(0);
            break;
        }

        uploadRect[0].w = READ_LE_U16(psVar2 + 0xc);
        uploadRect[0].h = READ_LE_U16(psVar2 + 0xe);
        LoadImage(uploadRect, psVar2 + 0x10);
        psVar2 = psVar2 + 0x10 + (int)uploadRect[0].w * (int)uploadRect[0].h * 2;
    }
}

void customVramUpload()
{
    uploadTextureToVram(customVramUploadPtr, 1, customVramUploadX, customVramUploadY, 0,0,0);
}

void registerSpriteCallback2_2(sTaskHeader* param_1) {
    {
        sTaskHeader* pPrevious = nullptr;
        sTaskHeader* pCurrent = spriteCallbackHead;
        while (pCurrent) {
            if (pCurrent->m0_owner == param_1) {
                if (((pCurrent->m14 >> 0x1E) & 1) == 0) {
                    if ((pCurrent->m14 & 0x1fffffff) == (param_1->m10 & 0x1fffffff)) {
                        if (pPrevious == nullptr) {
                            spriteCallbackHead = pCurrent->m18_pNext;
                        }
                        else {
                            pPrevious->m18_pNext = pCurrent->m18_pNext;
                        }
                        if (spriteCallbacksCurrentEntry == pCurrent) {
                            spriteCallbacksCurrentEntry = pCurrent->m18_pNext;
                        }
                        if (pCurrent->mC_deleteCallback) {
                            pCurrent->mC_deleteCallback(pCurrent);
                        }
                    }
                }
            }

            pPrevious = pCurrent;
            pCurrent = pCurrent->m18_pNext;
        }
    }

    {
        sTaskHeader* pPrevious = nullptr;
        sTaskHeader* pCurrent = spriteCallback2Var2;
        while (pCurrent) {
            if (pCurrent->m0_owner == param_1) {
                if (((pCurrent->m14 >> 0x1E) & 1) == 0) {
                    if ((pCurrent->m14 & 0x1fffffff) == (param_1->m10 & 0x1fffffff)) {
                        if (pPrevious == nullptr) {
                            spriteCallback2Var2 = pCurrent->m18_pNext;
                        }
                        else {
                            pPrevious->m18_pNext = pCurrent->m18_pNext;
                        }
                        if (spriteCallbacksCurrentEntry == pCurrent) {
                            spriteCallbacksCurrentEntry = pCurrent->m18_pNext;
                        }
                        if (pCurrent->mC_deleteCallback) {
                            pCurrent->mC_deleteCallback(pCurrent);
                        }
                    }
                }
            }

            pPrevious = pCurrent;
            pCurrent = pCurrent->m18_pNext;
        }
    }
}

void setGraphicEntityScale(sSpriteActorCore* param_1, int param_2)
{
    sFieldEntitySub4_B4_base* psVar1;

    psVar1 = param_1->m20;
    if (psVar1 != nullptr) {
        param_1->m2C_scale = param_2;
        psVar1->m6_scale.vx = param_2;
        psVar1->m6_scale.vy = param_2;
        psVar1->m6_scale.vz = param_2;
        param_1->m3C = param_1->m3C | 0x10000000;
    }
}

void initPerSubgroupTransforms(sSpriteActorCore* param_1) {
    if (param_1->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
        param_1->m20->getAsSprite()->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
        resetPerSubgroupTransforms(param_1);
    }
}

void executeSpriteBytecode2Extended(sSpriteActorCore* param_1, int bytecode, std::span<u8>::iterator param_3)
{
	switch (bytecode & 0xff) {
	case 0x84:
    case 0x89:
		break;
    case 0x8A:
        (param_1->mC_step).vx = 0;
        (param_1->mC_step).vz = 0;
        param_1->m18_moveSpeed = 0;
        break;
    case 0x8D:
        setupOverrideTPage(param_1->m24_vramData->m4_vramLocation.vx, param_1->m24_vramData->m4_vramLocation.vy);
        break;
    case 0x91:
        param_1->m28_colorAndCode.m3_code &= ~1; // make solid
        updateAllSubsprites(param_1);
        break;
	case 0x92:
		param_1->m28_colorAndCode.m3_code |= 1; // make transparent
		updateAllSubsprites(param_1);
		break;
    case 0x93:
        if (param_1->m70 && (param_1->m3C & 3)) {
            if (!isVramPreBacked(param_1->m24_vramData->m0_spriteData)) {
                param_1->m40 = param_1->m40 & 0xfffe1fff | 0x1c000;
            }
            if (param_1->m20 && param_1->m70->m20->getAsSprite()->m34_perSubgroupTransform) {
                initPerSubgroupTransforms(param_1);
                for (int i = 0; i < 8; i++) {
                    (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[i] = (*param_1->m70->m20->getAsSprite()->m34_perSubgroupTransform)[i];
                }
                param_1->m3C = param_1->m70->m3C;
                param_1->m32_direction = param_1->m70->m32_direction;
            }
            addToSpriteTransferList(param_1, param_1->m34_currentSpriteFrame);
        }
        break;
    case 0x94:
        if ((param_1->m3C & 3) != 2) {
            return;
        }
        param_1->m20->m0_rotation[1] = param_1->m70->m32_direction;
        param_1->m3C = param_1->m3C | 0x10000000;
        break;
	case 0x96:
        // TODO: type-check that
        registerSpriteCallback2_2(param_1->m6C_pointerToOwnerStructure);
		break;
    case 0xA4:
        spriteActorSetPlayingAnimation(param_1->m74_pTargetEntitySprite, READ_LE_S8(param_3));
        break;
    case 0xA5:
    {
        int iVar16 = READ_LE_S8(param_3) * 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
        if (iVar16 < 0) {
            iVar16 = iVar16 + 0xfff;
        }
        param_1->m18_moveSpeed = param_1->m18_moveSpeed + (iVar16 >> 0xc) * 0x100;
        computeStepFromMoveSpeed(param_1); 
    }
    break;
    case 0xA9: // Move X
    {
        s32 magnitude = READ_LE_S8(param_3) * param_1->m2C_scale;
        if (magnitude < 0) {
            magnitude += 0xFFF;
        }
        s32 offset;
        if (param_1->mAC.mx2_facing) { // switch based on facing
            offset = modulateSpeed(param_1, magnitude >> 0xC) * -0x10000;
        }
        else {
            offset = modulateSpeed(param_1, magnitude >> 0xC) * 0x10000;
        }
        param_1->m0_position.vx += offset;
        break;
    }
    case 0xAA: // Move Y
    {
        s32 magnitude = READ_LE_S8(param_3) * param_1->m2C_scale;
        if (magnitude < 0) {
            magnitude += 0xFFF;
        }
        s32 offset = modulateSpeed(param_1, magnitude >> 0xC) * 0x10000;
        param_1->m0_position.vy += offset;
        break;
    }
    case 0xAB: // Move Z
    {
        s32 magnitude = READ_LE_S8(param_3) * param_1->m2C_scale;
        if (magnitude < 0) {
            magnitude += 0xFFF;
        }
        s32 offset = modulateSpeed(param_1, magnitude >> 0xC) * 0x10000;
        param_1->m0_position.vz += offset;
        break;
    }
    case 0xB0:
        MissingCode(); // play sound effect?
        break;
	case 0xB3:
		param_1->mA8.mxB= READ_LE_U8(param_3) & 0x3F;
		break;
	case 0xB4:
		pushByteOnAnimationStack(param_1, READ_LE_U8(param_3));
		break;
    case 0xB5:
        if ((param_1->m3C & 3) != 0) {
            setGraphicEntityScale(param_1, (short)((READ_LE_U8(param_3) << 0x18) >> 0x10));
        }
        break;
    case 0xBA:
        setTransparencyMode(param_1, READ_LE_U8(param_3));
        break;
    case 0xBC: // VERY Complicated, used by save points
        {
            u8 type = READ_LE_U8(param_3);
            if ((type & 0x80) == 0) {
                sSpriteActorCore* psVar25 = param_1->m70;
                if (psVar25 == nullptr) {
                    return;
                }
                if (psVar25->m20 == nullptr) {
                    return;
                }
                if ((psVar25->m3C & 3) != 1) {
                    return;
                }
                s32 X;
                s32 Y;
                if (psVar25->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
                    X = Y = 0;
                }
                else {
                    X = (*psVar25->m20->getAsSprite()->m34_perSubgroupTransform)[type].m0_translateX;
                    Y = (*psVar25->m20->getAsSprite()->m34_perSubgroupTransform)[type].m1_translateY;
                }
                if ((psVar25->m3C >> 3) & 1) {
                    X = -X;
                }
                Y *= psVar25->m2C_scale;
                X *= psVar25->m2C_scale;
                if (Y < 0) {
                    Y += 0xFFF;
                }
                if (X < 0) {
                    X += 0xFFF;
                }
                (param_1->m0_position).vz = (psVar25->m0_position).vz;
                (param_1->m0_position).vx = (psVar25->m0_position).vx + (X >> 0xc) * 0x10000;
                (param_1->m0_position).vy = (psVar25->m0_position).vy + (Y >> 0xc) * 0x10000;
                return;
            }
            bool applyToMatrix = (param_1->m3C >> 24) & 1;
            SVECTOR local_70;
            switch (READ_LE_U8(param_3) & 0x3F)
            {
            case 0:
                local_70.vx = param_1->m74_pTargetEntitySprite->m0_position.vx.getIntegerPart();
                local_70.vy = param_1->m74_pTargetEntitySprite->m0_position.vy.getIntegerPart();
                local_70.vz = param_1->m74_pTargetEntitySprite->m0_position.vz.getIntegerPart();
                break;
            case 0x14:
                local_70.vx = param_1->m74_pTargetEntitySprite->m0_position.vx.getIntegerPart();
                local_70.vz = param_1->m74_pTargetEntitySprite->m0_position.vz.getIntegerPart();
                local_70.vy = param_1->m74_pTargetEntitySprite->m0_position.vy.getIntegerPart() - (param_1->m74_pTargetEntitySprite->m38 - (param_1->m74_pTargetEntitySprite->m38 >> 1));
                break;
            case 0x16:
                local_70.vx = param_1->m70->m0_position.vx.getIntegerPart();
                local_70.vy = param_1->m70->m0_position.vy.getIntegerPart();
                local_70.vz = param_1->m70->m0_position.vz.getIntegerPart();
                applyToMatrix = 0;
                break;
            case 0x18:
                local_70.vx = param_1->m0_position.vx.getIntegerPart();
                local_70.vy = param_1->m0_position.vy.getIntegerPart();
                local_70.vz = param_1->m0_position.vz.getIntegerPart();
                applyToMatrix = 0;
                break;
            case 0x22:
                assert(isBattleOverlayLoaded);
                local_70.vx = processBattleAnimationSub0_var1->m0_position.vx.getIntegerPart();
                local_70.vy = processBattleAnimationSub0_var1->m0_position.vy.getIntegerPart() - processBattleAnimationSub0_var1->m38;
                local_70.vz = processBattleAnimationSub0_var1->m0_position.vz.getIntegerPart();
                break;
            case 0x24:
                // TODO: type-check that
                param_1->m6C_pointerToOwnerStructure->m14 |= 0x40000000;
                local_70.vx = param_1->m70->m0_position.vx.getIntegerPart();
                local_70.vy = param_1->m70->m0_position.vy.getIntegerPart();
                local_70.vz = param_1->m70->m0_position.vz.getIntegerPart();
                applyToMatrix = 0;
                break;
            case 0x25:
                // TODO: type-check that
                param_1->m6C_pointerToOwnerStructure->m14 &= ~0x40000000;
                local_70.vx = param_1->m70->m0_position.vx.getIntegerPart();
                local_70.vy = param_1->m70->m0_position.vy.getIntegerPart();
                local_70.vz = param_1->m70->m0_position.vz.getIntegerPart();
                applyToMatrix = 0;
                break;
            default:
                assert(0);
            }

            if (applyToMatrix != 0) {
                assert(0);
                //ApplyMatrixSV(&currentRenderingMatrix, &local_70, &local_70);
                local_70.vx = local_70.vx + (short)currentRenderingMatrix.t[0];
                local_70.vy = local_70.vy + (short)currentRenderingMatrix.t[1];
                local_70.vz = local_70.vz + (short)currentRenderingMatrix.t[2];
            }

            if ((READ_LE_U8(param_3) & 0x40) != 0) {
                param_1->mA0 = local_70;
                return;
            }
            (param_1->m0_position).vx = (int)local_70.vx << 0x10;
            (param_1->m0_position).vy = (int)local_70.vy << 0x10;
            (param_1->m0_position).vz = (int)local_70.vz << 0x10;
            break;
        }
    case 0xBD:
        spriteBytecode2ExtendedE0(param_1, createSavePointMeshData_mode5.m10_startOfAnimationContainer->getAnimation(READ_LE_U8(param_3)), param_1->m24_vramData);
        break;
	case 0xA0: // set the move speed for the character
		{
			int iVar11 = READ_LE_S8(param_3 )* 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
			if (iVar11 < 0) {
				iVar11 = iVar11 + 0xfff;
			}
			param_1->m18_moveSpeed = (iVar11 >> 0xc) << 8;
			computeStepFromMoveSpeed(param_1);
		}
		break;
    case 0xA1: // fei jumping
        {
            s32 tempNewAltitude = param_1->m7C->m0;

            if ((((uint)param_1->mA8.mx0) != 1) || (tempNewAltitude == 0)) {
                s32 newValue = READ_LE_S8(param_3) * 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
                if (newValue < 0) {
                    newValue += 0xfff;
                }
                tempNewAltitude = (newValue >> 0xc) << 8;
            }
            param_1->mC_step.vy = (tempNewAltitude << 8) / (int)(param_1->mAC.mx7_timeScale);
        }
        break;
    case 0xA3:
        MissingCode(); // save point spinning?
        break;
    case 0xA6: // used by save point (move up and down)
        if ((param_1->mA8.mx0) != 1) {
            int iVar13 = READ_LE_S8(param_3) * 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
            if (iVar13 < 0) {
                iVar13 = iVar13 + 0xfff;
            }
            (param_1->mC_step).vy = (param_1->mC_step).vy + ((iVar13 >> 0xc) << 0x10) / (int)(param_1->mAC.mx7_timeScale);
        }
        break;
    case 0xA8:
        param_1->m32_direction += (READ_LE_S8(param_3) << 4);
        computeStepFromMoveSpeed(param_1);
        break;
    case 0xAD:
        param_1->mA8.mx1 = READ_LE_U8(param_3);
        break;
    case 0xB8: // stack pop
        param_1->m8C_stackPosition -= READ_LE_U8(param_3);
        break;
    case 0xB9: // play sound effect, used when player attacks enemy
        MissingCode();
        break;
    case 0xbb:
        param_1->m30 = param_1->m30 + READ_LE_U8(param_3);
        break;
    case 0xbf:
        param_1->m36 = READ_LE_U8(param_3);
        break;
	case 0xC1:
		MissingCode();
		break;
    case 0xC4:
        MissingCode(); //Set step in a random direction
        break;
	case 0xC5:
		{
			int iVar11 = READ_LE_U8(param_3) / (fieldDrawEnvsInitialized + 1);
			while (iVar11 = iVar11 + -1, iVar11 != -1) {
				//FUN_80022cdc(param_1);
				MissingCode();
			}
		}
		break;
	case 0xc6:
		if (param_1->mA8.mx0) {
			param_1->m7C->mC = READ_LE_U8(param_3);
		}
		break;
    case 0xCC:
        param_1->m88_stack2 = param_1->m64_spriteByteCode.value() + READ_LE_S16(param_3);
        break;
    case 0xCD: // Spin on X axis
    {
        sFieldEntitySub4_B4_base* psVar22 = param_1->m20;
        if (psVar22 == nullptr) {
            return;
        }
        u16 uVar1 = READ_LE_U16(param_3) & 0x1ff;
        u32 sVar14 = uVar1 * 8;
        u16 uVar16 = (int)(READ_LE_U8(param_3 + 1) << 0x18) >> 0x10 & 0xffff;
        s32 uVar15 = uVar16 >> 9 & 7;
        if (param_1->mAC.mx2_facing) {
            sVar14 = uVar1 * -8;
        }
        if ((uVar16 >> 0xc & 1) == 0) {
            if (uVar15 != 0) {
                sModelBlock* pasVar3 = psVar22->getAsObject()->m34_pModelBlock;
                if (pasVar3 == nullptr) {
                    return;
                }
                //(*pasVar3)[uVar15].m2_rotateX = (*pasVar3)[uVar15].m2_rotateY + sVar14;
                assert(0);
                return;
            }
            psVar22->m0_rotation[0] = psVar22->m0_rotation[0] + sVar14;
        }
        else {
            if (uVar15 != 0) {
                assert(0);
                /*
                if (psVar22->m34_perSubgroupTransform == (sFieldEntitySub4_124(*)[8])0x0) {
                    return;
                }
                (*psVar22->m34_perSubgroupTransform)[uVar15].m2_rotateY = sVar14;
                */
                return;
            }
            psVar22->m0_rotation[0] = sVar14;
        }
    }
    param_1->m3C = param_1->m3C | 0x10000000;
    break;
    case 0xCE: // Spin on Y axis (save point spinning)
        {
            sFieldEntitySub4_B4_base* psVar22 = param_1->m20;
            if (psVar22 == nullptr) {
                return;
            }
            u16 uVar1 = READ_LE_U16(param_3) & 0x1ff;
            u32 sVar14 = uVar1 * 8;
            u16 uVar16 = (int)(READ_LE_U8(param_3+1) << 0x18) >> 0x10 & 0xffff;
            s32 uVar15 = uVar16 >> 9 & 7;
            if (param_1->mAC.mx2_facing) {
                sVar14 = uVar1 * -8;
            }
            if ((uVar16 >> 0xc & 1) == 0) {
                if (uVar15 != 0) {
                    sModelBlock* pasVar3 = psVar22->getAsObject()->m34_pModelBlock;
                    if (pasVar3 == nullptr) {
                        return;
                    }
                    //(*pasVar3)[uVar15].m2_rotateY = (*pasVar3)[uVar15].m2_rotateY + sVar14;
                    assert(0);
                    return;
                }
                psVar22->m0_rotation[1] = psVar22->m0_rotation[1] + sVar14;
            }
            else {
                if (uVar15 != 0) {
                    assert(0);
                    /*
                    if (psVar22->m34_perSubgroupTransform == (sFieldEntitySub4_124(*)[8])0x0) {
                        return;
                    }
                    (*psVar22->m34_perSubgroupTransform)[uVar15].m2_rotateY = sVar14;
                    */
                    return;
                }
                psVar22->m0_rotation[1] = sVar14;
            }
        }
        param_1->m3C = param_1->m3C | 0x10000000;
        break;
    case 0xCF: // Spin on Z axis
    {
        if (param_1->m20 == nullptr) {
            return;
        }
        u16 uVar1 = READ_LE_U16(param_3) & 0x1ff;
        u32 sVar14 = uVar1 * 8;
        u16 uVar16 = (int)(READ_LE_U8(param_3 + 1) << 0x18) >> 0x10 & 0xffff;
        s32 uVar15 = uVar16 >> 9 & 7;
        if (param_1->mAC.mx2_facing) {
            sVar14 = uVar1 * -8;
        }
        if ((uVar16 >> 0xc & 1) == 0) {
            if (uVar15 != 0) {
                sFieldEntitySub4_B4_alt* psVar22 = param_1->m20->getAsObject();
                sModelBlock* pasVar3 = psVar22->m34_pModelBlock;
                if (pasVar3 == nullptr) {
                    return;
                }
                //(*pasVar3)[uVar15].m2_rotateZ = (*pasVar3)[uVar15].m2_rotateZ + sVar14;
                assert(0);
                return;
            }
            param_1->m20->m0_rotation[2] += sVar14;
        }
        else {
            if (uVar15 != 0) {
                assert(0);
                /*
                if (psVar22->m34_perSubgroupTransform == (sFieldEntitySub4_124(*)[8])0x0) {
                    return;
                }
                (*psVar22->m34_perSubgroupTransform)[uVar15].m2_rotateZ = sVar14;
                */
                return;
            }
            param_1->m20->m0_rotation[2] = sVar14;
        }
    }
    param_1->m3C = param_1->m3C | 0x10000000;
    break;
	case 0xd1:
		MissingCode();
		//executeSpriteBytecode2Sub3(param_1, param_3)->asU8 *= executeSpriteBytecode2Sub3(param_1, param_3 + 1)->asU8;
		break;
	case 0xd2:
		MissingCode();
		//executeSpriteBytecode2Sub3(param_1, param_3)->asU8 /= executeSpriteBytecode2Sub3(param_1, param_3 + 1)->asU8;
		break;
	case 0xDC:
		MissingCode();
		//executeSpriteBytecode2Sub3(param_1, param_3)->asU16 >>= (READ_LE_U8(param_3 + 1) & 0x1F);
		break;
	case 0xd0:
	case 0xd3:
	case 0xdd:
	case 0xde:
		MissingCode();
		//executeSpriteBytecode2Sub3(param_1, param_3)->asU8 += executeSpriteBytecode2Sub3(param_1, param_3 + 1)->asU8;
		break;
    case 0xDF: // used in battle when attacking
        //executeSpriteBytecode2Sub3(param_1, param_3)->asU8 = READ_LE_U8(param_3 + 1);
        MissingCode();
        break;
    case 0xE5: // set random value in stack
        MissingCode();
        break;
    case 0xAC:
        OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, param_1->m32_direction + ((((xenoRand() & 0xFF) * READ_LE_U32(param_3) >> 8) - READ_LE_U32(param_3) / 2) * 0x10));
        break;
	case 0xe0:
		spriteBytecode2ExtendedE0(param_1, param_3 + READ_LE_S16(param_3), param_1->m24_vramData);
		break;
    case 0xe7:
        setGraphicEntityScale(param_1, param_1->m2C_scale + READ_LE_S16(param_3));
        break;
    case 0xE9:
        if (param_1->m20) {
            param_1->m20->m6_scale[0] += READ_LE_S16(param_3) * 2;
        }
        break;
    case 0xEA:
        if (param_1->m20) {
            param_1->m20->m6_scale[1] += READ_LE_S16(param_3) * 2;
        }
        break;
    case 0xEE:
    {
        int scaledValue = READ_LE_S16(param_3) * param_1->m2C_scale;
        if (scaledValue < 0) {
            scaledValue += 0xfff;
        }
        (param_1->m0_position).vy = (param_1->m84_maxY + (scaledValue >> 0xc)) * 0x10000;
    }
    break;
	case 0xF0:
		return;
	case 0xf1:
		(param_1->m28_colorAndCode).m0_r = READ_LE_U8(param_3);
		(param_1->m28_colorAndCode).m1_g = READ_LE_U8(param_3 + 1);
		(param_1->m28_colorAndCode).m2_b = READ_LE_U8(param_3 + 2);
		if ((param_1->m3C & 3) == 2) {
			MissingCode();
		}
		if ((param_1->m3C & 3) == 1) {
			updateAllSubsprites(param_1);
		}
		break;
    case 0xF2:
        param_1->m28_colorAndCode.m0_r = addAndClamp(param_1->m28_colorAndCode.m0_r, READ_LE_U8(param_3));
        param_1->m28_colorAndCode.m1_g = addAndClamp(param_1->m28_colorAndCode.m1_g, READ_LE_U8(param_3+1));
        param_1->m28_colorAndCode.m2_b = addAndClamp(param_1->m28_colorAndCode.m2_b, READ_LE_U8(param_3+2));
        if ((param_1->m3C & 3) == 2) {
            assert(0);
        }
        if ((param_1->m3C & 3) == 1) {
            updateAllSubsprites(param_1);
        }
        if ((param_1->m40 >> 0xd & 0xf) == 0xf) {
            assert(0);
        }
        break;
    case 0xF5:
    {
        s32 offsetToMesh = READ_LE_U8(param_3) + READ_LE_U8(param_3 + 1) * 0x100 + READ_LE_U8(param_3 + 2) * 0x10000;
        std::span<u8>::iterator meshBlock = param_3 + offsetToMesh;
        sModelBlock* pNewModelBlock = new sModelBlock;
        pNewModelBlock->init(&meshBlock[0]);
        pNewModelBlock->m_baseItForRelocation = &meshBlock[0];
        if (param_1->m20->getAsObject()->m2C[0].size()) {
            param_1->m20->getAsObject()->m2C[0].clear();
            param_1->m20->getAsObject()->m2C[1].clear();
        }
        initModel1(*pNewModelBlock, param_1->m20->getAsObject()->m2C[0], param_1->m20->getAsObject()->m2C[1]);
        initModel2(pNewModelBlock, param_1->m20->getAsObject()->m2C[0], 0);
        param_1->m20->getAsObject()->m2C[1] = param_1->m20->getAsObject()->m2C[0];
        param_1->m20->getAsObject()->m34_pModelBlock = pNewModelBlock;
        break;
    }
	case 0xF6:
		MissingCode();
		break;
	case 0xF7:
		MissingCode();
		break;
	case 0xfc:
        customVramUploadX = (param_1->m24_vramData->m4_vramLocation).vx;
        customVramUploadY = (param_1->m24_vramData->m4_vramLocation).vy;
        customVramUploadPtr = param_3 + (READ_LE_U8(param_3) + READ_LE_U8(param_3 + 1) * 0x100 + READ_LE_U8(param_3 + 2) * 0x10000);
        customVramUpload();
		break;
    //case 0xEC:
    //case 0xF9:
    case 0xFF:
        // BUG?
        break;
	default:
		assert(0);

	// default
	case 0xB1:
	case 0xF8:
        assert(isBattleOverlayLoaded == 0);
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

u8 popByteFromAnimationStack(sSpriteActorCore* param_1)
{
	return param_1->m8E_stack[param_1->m8C_stackPosition++].asU8;
}

void pushBytecodePointerOnAnimationStack(sSpriteActorCore* param_1, std::span<u8>::iterator param_2)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asPs1Pointer = param_2;
}

std::span<u8>::iterator popPointerFromAnimationStack(sSpriteActorCore* param_1)
{
    return param_1->m8E_stack[param_1->m8C_stackPosition++].asPs1Pointer;
}

#define SPRITE_VM_NAME executeSpriteBytecode2_field
#include "kernel/spriteVM.h"
#undef SPRITE_VM_NAME

void executeSpriteBytecode2_battle(sSpriteActorCore* param_1);

void executeSpriteBytecode2(sSpriteActorCore* param_1)
{
    if (isBattleOverlayLoaded != '\0') {
        executeSpriteBytecode2_battle(param_1);
        return;
    }

    executeSpriteBytecode2_field(param_1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActorCore* param_1)
{
	short sVar1;
	int iVar2;

	iVar2 = 0;
	if (fieldDrawEnvsInitialized != -1) {
		do {
            if (param_1->m9E_wait) {
                if (--param_1->m9E_wait == 0) {
                    executeSpriteBytecode2(param_1);
                }
            }
			iVar2 = iVar2 + 1;
		} while (iVar2 != fieldDrawEnvsInitialized + 1);
	}
	return;
}

//header is
//u16 m0: flags
//u16 m2: offset to byte code
//u16 m4: offset to?

void setCurrentAnimationPtr(sSpriteActorCore* param_1, std::span<u8>::iterator startOfAnimation)
{
	sFieldEntitySub4_B4* psVar2;
	sFieldEntitySub4_F4* psVar3;
	int iVar5;

	u16 flags = READ_LE_U16(startOfAnimation);
	u16 offsetToByteCode = READ_LE_U16(startOfAnimation + 2);
	u16 offset2 = READ_LE_U16(startOfAnimation + 4);

	param_1->m58_startOfCurrentAnimation = startOfAnimation;
	param_1->m64_spriteByteCode = startOfAnimation + offsetToByteCode + 2;
	param_1->mA8.mx14 = flags;
	param_1->m54 = startOfAnimation + offset2 + 4;

    s32 gravityFactor = (flags >> 2) & 0x3f;
	if (flags >> 2 & 0x20) {
		gravityFactor |= 0xffffffc0;
	}

	param_1->m1C_gravity = gravityFactor * 0x400;
	int iVar4 = (fieldDrawEnvsInitialized + 1) * (fieldDrawEnvsInitialized + 1) * param_1->m82;
	if (iVar4 < 0) {
		iVar4 = iVar4 + 0xfff;
	}
	iVar4 = gravityFactor * 0x400 * (iVar4 >> 0xc);
	param_1->m1C_gravity = iVar4;

	iVar5 = 0x10000 / (int)(param_1->mAC.mx7_timeScale);
	iVar5 = iVar5 * iVar5;
	if (iVar5 < 0) {
		iVar5 = iVar5 + 0xff;
	}
	iVar4 = iVar4 * (iVar5 >> 8);
	param_1->m1C_gravity = iVar4;
	if (iVar4 < 0) {
		iVar4 = iVar4 + 0xff;
	}
	param_1->m1C_gravity = iVar4 >> 8;


	if ((flags >> 0xb & 1) == 0) {
		param_1->mC_step.vz = 0;
		param_1->mC_step.vy = 0;
		param_1->mC_step.vx = 0;
		param_1->m18_moveSpeed = 0;
	}
	if (param_1->m20)
	{
		if (((flags >> 0xc) & 1) == 0) {
			param_1->m20->m0_rotation.vx = 0;
			param_1->m20->m0_rotation.vy = 0;
			param_1->m20->m0_rotation.vz = 0;
			setupSpriteObjectMatrix(param_1);
		}
		if (((flags >> 0xd) & 1) == 0) {
			if (isBattleOverlayLoaded != '\0') {
				setGraphicEntityScale(param_1, (short)battleDefaultEntityScale);
			}
		}
        if (isBattleOverlayLoaded != '\0') {
            setupSpriteObjectMatrix(param_1);
        }
		if ((param_1->m3C & 3) == 1) {
			param_1->m20->getAsSprite()->m3D = 0;
			param_1->m20->getAsSprite()->m3C = 0;
			if (((param_1->m40 >> 0x14 & 1) == 0) && (param_1->m20->getAsSprite()->m34_perSubgroupTransform != nullptr)) {
				resetPerSubgroupTransforms(param_1);
			}
		}
	}
	param_1->m8C_stackPosition = 0x10;
	param_1->m30 = 0;
	param_1->m9E_wait = 1;

    param_1->mA8.mx1 = 0;
	param_1->mA8.mx16 = 0;
	param_1->mA8.mx1C = 0;
	param_1->mA8.mxB = 0x3F;
	param_1->mA8.mx1C = 2;

	if ((param_1->m7C != nullptr) && (param_1->mA8.mx0)) {
		param_1->m7C->m4 = 0;
		param_1->m7C->m0 = 0;
		param_1->m7C->mC = 0;
	}
	return;
}

void executeSpriteBytecode(sSpriteActorCore* param_1, std::span<u8>::iterator param_2, uint param_3)
{
	int unaff_s3;

    VALIDATE_FIELD(FCT_KernelSpriteVM, 0x80022660);
    VALIDATE_REG(FCT_KernelSpriteVM, A2, param_3);

    while (true)
	{
        std::span<u8>::iterator pBytecode = param_1->m64_spriteByteCode.value();
		if ((pBytecode == param_2) && (param_1->mA8.mx16 == param_3)) {
			return;
		}

		u8 bytecode = READ_LE_U8(pBytecode);

        VALIDATE_FIELD(FCT_KernelSpriteVM, 0x800226bc);
        VALIDATE_REG(FCT_KernelSpriteVM, S2, bytecode);

		if (bytecode < 0x80)
		{
			param_1->m64_spriteByteCode = pBytecode + 1;
			if (bytecode < 0x10)
			{
				addToSpriteTransferList(param_1, param_1->m34_currentSpriteFrame + 1);
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
				addToSpriteTransferList(param_1, param_1->m34_currentSpriteFrame - 1);
				unaff_s3 = (bytecode & 0xf) + 1;
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
			case 0x80:
			case 0x81:
			case 0x82:
				return;
            case 0x86:
            case 0x87:
                if (pBytecode == param_2)
                    return;
                break;
			case 0xB3:
				param_1->mA8.mxB = READ_LE_U8(pBytecode + 1);
				break;
			case 0x84:
			case 0xA0:
            case 0xA1:
			case 0xA7:
			case 0xB4:
            case 0xC6:
			case 0xE4:
				break;
			default:
				assert(0);
			}

			param_1->m64_spriteByteCode.value() += sizePerBytecodeTable[bytecode];
		}
	}
}

void setSpriteActorAngle(sSpriteActorCore* param_1, short angle)
{
    VALIDATE_FIELD(FCT_KernelSpriteVM, 0x800223b0);
    VALIDATE_REG(FCT_KernelSpriteVM, A1, angle);

	short sVar1;
	ushort uVar2;
	uint originalM17;

	param_1->m80 = angle;
	originalM17 = param_1->mA8.mx11;
	if (((int)angle + 0x400U & 1) == 0) {
		param_1->mAC.mx2_facing = 0;
	}
	else {
		param_1->mAC.mx2_facing = 1;
	}

	if (param_1->m48_defaultAnimationbundle == nullptr) {
		return;
	}

	int newAngle;

	switch (param_1->mA8.mx14)
	{
	case 0:
		if ((angle + 0x400U & 0xfff) < 0x801) {
			param_1->mAC.mx2_facing = 0;
		}
		else {
			param_1->mAC.mx2_facing = 1;
		}
		param_1->mA8.mx11 = 0;
		param_1->m5C = param_1->m58_startOfCurrentAnimation + 6;
		param_1->m54 = param_1->m58_startOfCurrentAnimation + READ_LE_U16(param_1->m58_startOfCurrentAnimation + 4) + 4;
		break;
    case 1:
        newAngle = ((angle + 0x600) >> 10) & 3;
        if (newAngle < 3) {
            uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + newAngle * 2 + 4);
            param_1->mAC.mx2_facing = 0;
            param_1->m54 = param_1->m58_startOfCurrentAnimation + (uint)uVar2 + newAngle * 2 + 4;
        }
        else {
            uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + 6);
            param_1->mAC.mx2_facing = 1;
            param_1->m54 = param_1->m58_startOfCurrentAnimation + uVar2 + 6;
        }
        break;
	case 2:
	{
		newAngle = ((angle + 0x500) >> 9) & 7;
		if (newAngle > 4)
		{
			newAngle = newAngle - 5 ^ 3;
			uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + newAngle * 2 + 4);
			param_1->mAC.mx2_facing = 1;
			param_1->m54 = param_1->m58_startOfCurrentAnimation + (uint)uVar2 + newAngle * 2 + 4;
		}
		else
		{
			uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + newAngle * 2 + 4);
			param_1->mAC.mx2_facing = 0;
			param_1->m54 = param_1->m58_startOfCurrentAnimation + (uint)uVar2 + newAngle * 2 + 4;
		}
		param_1->mA8.mx11 = newAngle;
		break;
	}
	default:
		assert(0);
	}

	// direction angle changed
    auto A8Backup = param_1->mA8;
	if (originalM17 != param_1->mA8.mx11) {
		sVar1 = param_1->m9E_wait;
		uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + 2);
        std::span<u8>::iterator puVar5 = param_1->m64_spriteByteCode.value();
		param_1->mA8.mx16 = 0;
		param_1->mA8.mxB = 0x3F;
		param_1->m64_spriteByteCode = param_1->m58_startOfCurrentAnimation + uVar2 + 2;
		executeSpriteBytecode(param_1, puVar5, A8Backup.mx16);
		param_1->m9E_wait = sVar1;
	}

	param_1->m3C = param_1->m3C & 0xfffffff7 | (param_1->mAC.mx3 ^ param_1->mAC.mx2_facing) << 3;
}

void initFieldEntitySub4Sub5Sub0(sFieldEntitySub4_110* param_1, sSpriteActorAnimationBundle* param_2, sVec2_s16 param_3_vramLocation, sVec2_s16 clut)
{
	param_1->m4_vramLocation = param_3_vramLocation;
	param_1->m8_clut = clut;
	param_1->mC = &param_2->mC_pData;
	param_1->m0_spriteData = &param_2->m8_pData;
	initFieldVar4 = 0;
	param_1->m10_startOfAnimationContainer = &param_2->m4_animBundle;

	if (isBattleOverlayLoaded)
	{
		u8 temp = (READ_LE_U16(param_2->m4_entries[0].begin()) >> 6) & 0x3F;
		if (temp)
		{
            assert(0);
			initFieldVar5 = temp;
		}
	}
}


void deleteFieldEntitySub4(sSpriteActor* param_1)
{
	MissingCode();
}


void setAnimationBundle(sSpriteActorCore* param_1, sSpriteActorAnimationBundle* pAnimationBundle)
{
	sFieldEntitySub4_110* psVar2 = param_1->m24_vramData;
	if (pAnimationBundle != 0) {
		if (pAnimationBundle != param_1->m44_currentAnimationBundle) {
			initFieldEntitySub4Sub5Sub0(psVar2, pAnimationBundle, psVar2->m4_vramLocation, psVar2->m8_clut);
			param_1->m44_currentAnimationBundle = pAnimationBundle;
			param_1->m3C = param_1->m3C | 0x40000000;
		}
		if (isBattleOverlayLoaded != '\0') {
			if (isVramPreBacked(psVar2->m0_spriteData) == 0) {
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

void spriteActorSetPlayingAnimation(sSpriteActorCore* param_1, int animationId)
{
	ushort uVar1;

	if (param_1->m48_defaultAnimationbundle == nullptr) {
		param_1->m64_spriteByteCode.reset();
	}
	else {
		if (param_1->m44_currentAnimationBundle == param_1->m48_defaultAnimationbundle) {
            param_1->mB0.mRaw &= ~0x400;
		}
		else {
            param_1->mB0.mRaw |= 0x400;
		}

		if (animationId < 0) {
			setAnimationBundle(param_1, param_1->m4C_specialAnimation);
			if ((isBattleOverlayLoaded != 0) &&
				(isVramPreBacked(param_1->m24_vramData->m0_spriteData) == 0)) {
				param_1->m24_vramData->m4_vramLocation.vx = 0x300;
				param_1->m24_vramData->m4_vramLocation.vy = 0x100;
			}
		}
		else {
			setAnimationBundle(param_1, param_1->m48_defaultAnimationbundle);
			if (isBattleOverlayLoaded != 0) {
				param_1->m24_vramData->m4_vramLocation = param_1->m7C->mE_vramLocation;
			}
		}
        param_1->mAC.mx18 = animationId;
		if ((int)animationId < 0) {
			animationId = ~animationId;
		}

		param_1->m40 = param_1->m40 | 0x100000;
		param_1->m58_startOfCurrentAnimation = param_1->m24_vramData->m10_startOfAnimationContainer->getAnimation(animationId);
		setCurrentAnimationPtr(param_1, param_1->m58_startOfCurrentAnimation);
		setSpriteActorAngle(param_1, param_1->m80);
	}
}

void initFieldEntitySub4Sub2(sSpriteActor* pThis)
{
	pThis->m20 = &pThis->mB4;
	initsFieldEntitySub4_B4(&pThis->mB4);
	pThis->m7C = &pThis->mF4;
	pThis->m20->getAsSprite()->m34_perSubgroupTransform = &pThis->m124;
	pThis->m24_vramData = &pThis->m110;
	pThis->m20->getAsSprite()->m38_pNext = 0;
}

sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7)
{
	initFieldEntitySub4Sub1(param_1);
	initFieldEntitySub4Sub2(param_1);
	setGraphicEntityScale(param_1, 0x1000);

	param_1->m3C = (param_1->m3C & ~0x3) | 1;
	param_1->m40 &= ~0x0001E000;

	if (isBattleOverlayLoaded == 0)
	{
		param_1->mA8.mx0 = 1;
		param_1->m7C->m18.reset();
	}
	else
	{
		param_1->mA8.mx0 = 0;
		param_1->m7C->m8 = 0;
		param_1->m7C->mC = 0;
	}

	param_1->m6C_pointerToOwnerStructureSpriteActor = param_1;
    param_1->m3C &= ~0xFF0000;
    param_1->m3C |= (loadVramSpriteParam & 0xf) << 0x14;
    param_1->m3C |= (loadVramSpriteParam & 0xf) << 0x10;

	int count = initFieldEntitySub4Sub4(&pSetup->m8_pData);
	param_1->m20->getAsSprite()->m2C = param_1->m20->getAsSprite()->m30 = new std::vector<sFieldEntitySub4_B4_sub>;
	param_1->m20->getAsSprite()->m30->resize(count);

	param_1->m24_vramData->m4_vramLocation.vx = vramX;
	param_1->m24_vramData->m4_vramLocation.vy = vramY;
	param_1->m24_vramData->m8_clut.vx = clutX;
	param_1->m24_vramData->m8_clut.vy = clutY;

	param_1->m48_defaultAnimationbundle = pSetup;

	setAnimationBundle(param_1, pSetup);

    sPS1Pointer pAnim = param_1->m24_vramData->m10_startOfAnimationContainer->getAnimation(0);
    param_1->m60_endOfAnimationContainer = pAnim + ((READ_LE_U16(pAnim) & 0x3F) + 1) * 2;
	spriteActorSetPlayingAnimation(param_1, 0);

	return param_1;
}

void getBoundingVolumeFromSprite(sSpriteActor* param_1, int param_2, sFixedPoint* pX, sFixedPoint* pY, sFixedPoint* pZ)
{
    auto startOfAnimation = param_1->m24_vramData->m10_startOfAnimationContainer->bundle.begin();

    uint uVar5 = startOfAnimation[READ_LE_U16(startOfAnimation + READ_LE_U16(startOfAnimation + 2) + 4) + READ_LE_U16(startOfAnimation + 2) + 4];

    sPS1Pointer spriteRawData = param_1->m24_vramData->m0_spriteData->rawPointer;
    int iVar3 = uVar5 << 1;
	if (uVar5 != 0) {
		uVar5 = uVar5 - 1;
		iVar3 = uVar5 * 2;
	}
    int iVar4 = uVar5 << 1;
	if (READ_LE_U16(spriteRawData + iVar3) < (int)uVar5) {
		iVar4 = 0;
	}
	spriteRawData = spriteRawData + READ_LE_U16(spriteRawData + iVar4 + 2);
	iVar3 = READ_LE_U8(spriteRawData + 3) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*pY = iVar3 >> 0xc;
	iVar3 = READ_LE_U8(spriteRawData + 1) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*pZ = iVar3 >> 0xc;
	iVar3 = READ_LE_U8(spriteRawData + 2) * param_1->m2C_scale;
	if (iVar3 < 0) {
		iVar3 = iVar3 + 0xfff;
	}
	*pX = iVar3 >> 0xc;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sSpriteActor* param1, int param2)
{
	param1->m40 = param1->m40 & 0xffffe0ff | (param2 & 0x1f) << 8;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActorCore* param1, int param2)
{
	param1->m32_direction = param2;
	computeStepFromMoveSpeed(param1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActorCore* param1, void(*callback)(sSpriteActorCore*))
{
	param1->m68 = callback;
}

void fieldActorCallback(sSpriteActorCore* pThis)
{
    VALIDATE_FIELD(FCT_Rendering, 0x80076a74);
	actorArray[pThis->m7C->m14_actorId].m4C_scriptEntity->m4_flags.m_rawFlags |= 0x10000;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sSpriteActor* param_1, int param_2)
{
	delete param_1->m20->getAsSprite()->m2C;
	sFieldEntitySub4_B4* psVar2 = param_1->m20->getAsSprite();
	psVar2->m2C = psVar2->m30 = new std::vector<sFieldEntitySub4_B4_sub>;
	psVar2->m30->resize(param_2);

	return;
}

sSpriteActor* createSpriteActor(sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_6)
{
	sSpriteActor* pNewEntry = new sSpriteActor;
	pNewEntry->m86_thisSize = sizeof(sSpriteActor);

	return initializeSpriteActor(pNewEntry, pSetup, clutX, clutY, vramX, vramY, param_6);
}

sSpriteActor* createSpriteActorEX(sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_6, int param_7)
{
	loadVramSpriteParam = param_7;
	sSpriteActor* pVar1 = createSpriteActor(pSetup, clutX, clutY, vramX, vramY, param_6);
	loadVramSpriteParam = 0;
	return pVar1;
}

void setTransparencyMode(sSpriteActorCore* sprite, u32 mode)
{
    byte bVar1;
    uint uVar2;

    sprite->m3C = sprite->m3C & 0xffffff1f | (mode & 7) << 5;
    if ((mode & 7) == 0) {
        bVar1 = (sprite->m28_colorAndCode).m3_code & 0xfd;
    }
    else {
        bVar1 = (sprite->m28_colorAndCode).m3_code | 2;
    }
    (sprite->m28_colorAndCode).m3_code = bVar1;
    if ((sprite->m40 >> 0xd & 0xf) - 8 < 2) {
        uVar2 = sprite->m3C >> 5 & 7;
        if (uVar2 != 0) {
            sprite->m3C = sprite->m3C & 0xffffff1f | (uVar2 - 1 & 7) << 5;
        }
    }
    else {
        updateAllSubsprites(sprite);
    }
}

int addAndClamp(int param_1, int param_2)
{
    int iVar1;

    iVar1 = param_1 + param_2;
    if (iVar1 < 0x100) {
        if (iVar1 < 0) {
            iVar1 = 0;
        }
    }
    else {
        iVar1 = 0xff;
    }
    return iVar1;
}