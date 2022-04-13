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

sSpriteActorCore* spriteTransfertListHead = nullptr;
sFieldEntitySub4_110 sFieldEntitySub4_110_8005a474;
sFieldEntitySub4_110 sFieldEntitySub4_110_8006be10;

void OP_21_sub(sSpriteActor* param_1, int param_2)
{
	param_1->mAC = param_1->mAC & 0xfff8007f | (param_2 & 0xfff) << 7;
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

void executeSpriteBytecode2Sub0Sub0(sSpriteActorCore* param_1, int param_2, sFieldEntitySub4_110* param_3)
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
			executeSpriteBytecode2Sub0Sub0sub0(param_1, param_2, param_3);
		}
	}
}

void addToSpriteTransferList(sSpriteActorCore* param_1, short param_2)
{
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
					sFieldEntitySub4_110* psVar3 = param_1->m24;
					if (((psVar3 != &sFieldEntitySub4_110_8005a474) && (psVar3 != &sFieldEntitySub4_110_8006be10)) && ((param_1->m40 >> 0x13 & 1) == 0)) {
						executeSpriteBytecode2Sub0Sub0(param_1, param_1->m34, psVar3);
					}
					param_1->m34 = param_2;
					return;
				}
				pCurrentHead = pCurrentHead->m20->getAsSprite()->m38_pNext;
			}
		}
		param_1->m34 = param_2;
		param_1->m40 |= 0x20000;
		param_1->m20->getAsSprite()->m38_pNext = spriteTransfertListHead;
        spriteTransfertListHead = param_1;
	}
	else {
		param_1->m34 = 0;
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
		param_1->mAC &= ~8;
	}
	else {
		param_1->mAC |= 8;
	}
	param_1->m3C = param_1->m3C & ~8 | (param_1->mAC >> 3 & 1 ^ param_1->mAC >> 2 & 1) << 3;
	addToSpriteTransferList(param_1, uVar1 & 0x1ff);
	return;
}

void pushByteOnAnimationStack(sSpriteActorCore* param_1, u8 param)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asU8 = param;
}

u8 spriteBytecode2ExtendedE0_Var0 = 0;

u16 getSavePointCreationMode1(sPS1Pointer param_1)
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

void allocateSavePointMeshDataSub0_callback(sSavePointMesh_1C* param_1)
{
    assert(0);
}

u32 registerSpriteCallback2Counter2 = 0;
u32 allocateSavePointMeshDataSub0_var0 = 0;
u32 allocateSavePointMeshDataSub0_var1 = 0;
void allocateSavePointMeshDataSub0(sSavePointMesh1* param_1, sSavePointMesh_1C* param_2)
{
    param_2->m0_owner = param_1;
    param_2->mC = allocateSavePointMeshDataSub0_callback;
    param_2->m8 = 0;
    param_2->m18_pNext = spriteCallback2Var2;
    spriteCallback2Var2 = param_2;
    param_2->m14 = param_1->m0.m10 & 0x1fffffff;
    param_2->m10 = param_2->m10 & 0xe0000000 | (registerSpriteCallback2Counter2 & 0x1fffffff);
    registerSpriteCallback2Counter2++;

    if (spriteBytecode2ExtendedE0_Var0 == 0) {
        param_2->m14 &= ~0x80000000;
    }
    else {
        allocateSavePointMeshDataSub0_var0++;
        param_2->m14 |= 0x80000000;
    }
    allocateSavePointMeshDataSub0_var1++;
}

sSavePointMesh_1C* spriteCallback2Head = nullptr;
u32 registerSpriteCallback2Counter = 0;

void registerSpriteCallback2Sub0(sSavePointMesh_1C* param_1) {
    MissingCode();
}


void registerSpriteCallback2(sSavePointMeshAbstract* param_1, sSavePointMesh_1C* param_2) {
    param_2->m0_owner = param_1;
    param_2->m10 = param_2->m10 & 0xe0000000 | (registerSpriteCallback2Counter2 & 0x1fffffff);
    registerSpriteCallback2Counter2 = registerSpriteCallback2Counter2 + 1;
    param_2->m18_pNext = spriteCallback2Head;
    spriteCallback2Head = param_2;
    param_2->m8 = nullptr;
    param_2->mC = registerSpriteCallback2Sub0;
    registerSpriteCallback2Counter = registerSpriteCallback2Counter + 1;
    param_2->m14 = param_1->m0.m10 & 0x1fffffff;
}

void regCallback8(sSavePointMesh_1C* param_1, void (*param_2)(sSavePointMesh_1C*))
{
    param_1->m8 = param_2;
}

void regCallbackC(sSavePointMesh_1C* param_1, void (*param_2)(sSavePointMesh_1C*))
{
    param_1->mC = param_2;
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
    assert(0); // this expects the battle overlay to be loaded
}

void savePointCallback8Sub0Sub0(sSpriteActorCore* param_1)
{
    short sVar1;
    int iVar2;
    int iVar3;

    if ((param_1->m3C >> 0x1a & 1) != 0) {
        iVar3 = modulateSpeed(param_1, (param_1->mC_step).vy >> 4);
        iVar2 = param_1->m1C;
        (param_1->m0_position).vy = (param_1->m0_position).vy + iVar3 * 0x10;
        (param_1->mC_step).vy = (param_1->mC_step).vy + iVar2;
        return;
    }
    savePointCallback8Sub0Sub0_battle(param_1);
    iVar3 = (param_1->mC_step).vy;
    iVar2 = iVar3 >> 4;
    if ((iVar3 < 1) || (param_1->m1C < 1)) {
        iVar3 = modulateSpeed(param_1, iVar2);
        iVar3 = (param_1->m0_position).vy + iVar3 * 0x10;
        sVar1 = param_1->m84;
        (param_1->m0_position).vy = iVar3;
        if ((int)sVar1 <= iVar3 >> 0x10) {
            (param_1->m0_position).vy = (int)sVar1 << 0x10;
        }
    }
    else {
        if (*(short*)((int)&(param_1->m0_position).vy + 2) == param_1->m84) {
            return;
        }
        iVar3 = modulateSpeed(param_1, iVar2);
        sVar1 = param_1->m84;
        iVar3 = (param_1->m0_position).vy + iVar3 * 0x10;
        (param_1->m0_position).vy = iVar3;
        if ((int)sVar1 <= iVar3 >> 0x10) {
            iVar3 = -(param_1->mC_step).vy * ((uint)param_1->mA8.mx1 & 0x3ff);
            (param_1->m0_position).vy = (int)sVar1 << 0x10;
            if (iVar3 < 0) {
                iVar3 = iVar3 + 0xff;
            }
            iVar2 = param_1->m1C;
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
    (param_1->mC_step).vy = (param_1->mC_step).vy + param_1->m1C;
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

MATRIX* TransMatrix(MATRIX* m, FP_VEC4* v)
{
    long lVar1;
    long lVar2;

    lVar1 = v->vy;
    lVar2 = v->vz;
    m->t[0] = v->vx;
    m->t[1] = lVar1;
    m->t[2] = lVar2;
    return m;
}

void setupSpriteObjectMatrix(sSpriteActorCore* param_1)

{
    if ((param_1->m40 & 1) == 0) {
        FP_VEC4 scale;
        scale.vx = param_1->m20->m6_scale.vx;
        scale.vy = param_1->m20->m6_scale.vy;
        scale.vz = param_1->m20->m6_scale.vz;
        createRotationMatrix(&param_1->m20->m0_rotation, &param_1->m20->mC_matrix);
        ScaleMatrixL(&param_1->m20->mC_matrix, &scale);
    }
    else {
        MATRIX scaleMatrix;
        FP_VEC4 scale;

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
        FP_VEC4 scale;
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

void spriteCallback_render2(sSavePointMesh_1C* param_1) {
    sSpriteActorCore* pSpriteActor = param_1->m4;

    spriteCallback_render2_updateMatrix(pSpriteActor);

    if (pSpriteActor->m20->getAsObject()->m34_pModelBlock) {
        FP_VEC4 temp;
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
        submitModelForRendering(pSpriteActor->m20->getAsObject()->m34_pModelBlock, pSpriteActor->m20->getAsObject()->m2C[shapeTransfertDoubleBufferIndex], *characterRenderingOT, (pSpriteActor->m40 >> 16) & 4);
    }
}

void savePointCallback8(sSavePointMesh_1C* param_1) {

    // HACK!
    spriteCallback_render2(param_1);


    OP_INIT_ENTITY_SCRIPT_sub0Sub9(param_1->m4);
    savePointCallback8Sub0(param_1->m4);

    if (param_1->m4->m64_spriteByteCode.getPointer()) {
        if (((param_1->m4->mAC >> 6) & 1) == 0) {
            return;
        }

        OP_INIT_ENTITY_SCRIPT_sub0Sub9(param_1->m4);
        savePointCallback8Sub0(param_1->m4);
        if (param_1->m4->m64_spriteByteCode.getPointer()) {
            return;
        }
    }
    param_1->mC(param_1);
}

void savePointCallbackC(sSavePointMesh_1C* param_1) {
    MissingCode();
    registerSpriteCallback2Sub0(param_1 + 1);
}

void initFieldEntitySub4Sub1(sSpriteActorCore* param_1)
{
    param_1->m3C = 0;
    param_1->m28_colorAndCode.m3_code = 0x2d;
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
    param_1->m44_currentAnimationBundle = 0;
    param_1->m68 = 0;
    param_1->m80 = 0;
    param_1->m8C_stackPosition = 0x10;
    param_1->m84 = 0;
    param_1->m6C_pointerToOwnerStructure = nullptr;
    param_1->m50 = 0;
}

sSavePointMeshAbstract* allocateSavePointMeshData(sSavePointMeshAbstract* pThis, sSavePointMesh1* param_1)
{
    sSavePointMeshAbstract* pvVar1 = pThis;
    allocateSavePointMeshDataSub0(param_1, &pvVar1->m0);
    registerSpriteCallback2(pvVar1, &pvVar1->m1C);

    initFieldEntitySub4Sub1(&pvVar1->m38_spriteActorCore);

    pvVar1->m0.m4 = &pvVar1->m38_spriteActorCore;
    pvVar1->m1C.m4 = &pvVar1->m38_spriteActorCore;

    regCallback8(&pvVar1->m0, savePointCallback8);
    regCallbackC(&pvVar1->m0, savePointCallbackC);

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

void createSavePointMeshDataMode1(sSavePointMesh1* param_1)
{
    (param_1->m38_spriteActorCore).m20 = &param_1->mB4;
    initsFieldEntitySub4_B4(&param_1->mB4);
    (param_1->m38_spriteActorCore).m20->getAsObject()->m30 = &param_1->mF4;
    (param_1->m38_spriteActorCore).m20->getAsObject()->m34_pModelBlock = nullptr;
    (param_1->m38_spriteActorCore).m20->getAsObject()->m38 = nullptr;
}

void createSavePointMeshDataMode2(sSavePointMesh2* param_1)
{
    (param_1->m38_spriteActorCore).m20 = &param_1->mB4;
    initsFieldEntitySub4_B4(&param_1->mB4);
    (param_1->m38_spriteActorCore).m20->getAsObject()->m34_pModelBlock = nullptr;
    (param_1->m38_spriteActorCore).m20->getAsObject()->m40 = 0;
}


int initFieldEntitySub4Sub4(const sPS1Pointer& param_1)
{
    return (READ_LE_U16(param_1) >> 9) & 0x3F;
}

sSavePointMeshAbstract* createSavePointMeshData(int mode1, int mode2, sFieldEntitySub4_110* param_3, int param_4, sSavePointMesh1* param_5)
{
    // NOTE: this is kinda different from the original code, that was handcrafting those various structure in a very hackish way
    sSavePointMeshAbstract* pNewSavePoint = nullptr;
    switch (mode2) {
    case 1:
        if (mode1 == 5) {
            assert(0);
        }
        if (mode1 == 6) {
            assert(0);
        }
        {
            sSavePointMesh1* pNewSavePoint1 = new sSavePointMesh1;
            int numF4 = initFieldEntitySub4Sub4(param_3->m0) - 1;
            pNewSavePoint = allocateSavePointMeshData(pNewSavePoint1, param_5);
            pNewSavePoint1->mF4.resize(numF4);
            createSavePointMeshDataMode1(pNewSavePoint1);
            break;
        }
    case 2:
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
    pNewSavePoint->m38_spriteActorCore.m24 = param_3;

    return pNewSavePoint;
}

void spriteBytecode2ExtendedE0_Sub0Sub0Sub0(sSavePointMesh_1C* param_1, void (*param_2)(sSavePointMesh_1C*))
{
    param_1->m8 = param_2;
    return;
}

void spriteCallback_render0(sSavePointMesh_1C* param_1) {
    assert(0);
}

const std::vector<void (*)(sSavePointMesh_1C*)> spriteBytecode2ExtendedE0_Sub0Sub0_callback = { {
        spriteCallback_render0,
        nullptr,
        spriteCallback_render2,
} };

void spriteBytecode2ExtendedE0_Sub0Sub0(sSavePointMesh_1C* param_1, int param_2)
{
    spriteBytecode2ExtendedE0_Sub0Sub0Sub0(param_1, spriteBytecode2ExtendedE0_Sub0Sub0_callback[param_2]);
    return;
}

void spriteBytecode2ExtendedE0_Sub0(sSavePointMeshAbstract* param_1)
{
    u32 mode = param_1->m38_spriteActorCore.m40 >> 0xd & 0xf;
    switch (mode)
    {
    case 0:
    case 2:
        break;
    default:
        assert(0);
        break;
    }
    spriteBytecode2ExtendedE0_Sub0Sub0(&param_1->m1C, mode);
}

void spriteBytecode2ExtendedE0(sSpriteActorCore* param_1, sPS1Pointer param_2, sFieldEntitySub4_110* param_3)
{
    u8 oldSpriteBytecode2ExtendedE0_Var0 = spriteBytecode2ExtendedE0_Var0;
    u32 b0_flag = param_1->mB0;
    param_1->mB0 = b0_flag | 0x800;
    if ((b0_flag & 0x100) != 0) {
        spriteBytecode2ExtendedE0_Var0 = 0;
    }
    u32 savePointCreationMode1 = getSavePointCreationMode1(param_2);
    if (savePointCreationMode1 == 3) {
        savePointCreationMode1 = param_1->m40 >> 0xd & 0xf;
    }
    u32 savePointCreationMode2 = getModeForSavePointMesh(savePointCreationMode1);

    sSavePointMeshAbstract* pSavePointMesh = createSavePointMeshData(savePointCreationMode1, savePointCreationMode2, param_3, 0, (sSavePointMesh1*)param_1->m6C_pointerToOwnerStructure);
    pSavePointMesh->m0.m14 |= 0x20000000;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffe1fff | (savePointCreationMode1 & 0xf) << 0xd;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffffc | savePointCreationMode2 & 3;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffe00ff | (savePointCreationMode1 & 0xf) << 0xd | param_1->m40 & 0x1f00;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffff7 | param_1->m3C & 8;


    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xffffffe7 | param_1->m3C & 8 | param_1->m3C & 0x10;
    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xffff00ff | param_1->m3C & 0xFF00;

    pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xfffbffff | param_1->m40 & 0x40000;

    pSavePointMesh->m38_spriteActorCore.m3C = pSavePointMesh->m38_spriteActorCore.m3C & 0xfffffffb | 0x4000000;
    pSavePointMesh->m38_spriteActorCore.m18_moveSpeed = param_1->m18_moveSpeed;
    pSavePointMesh->m38_spriteActorCore.m32 = param_1->m32;
    pSavePointMesh->m38_spriteActorCore.m2C_scale = param_1->m2C_scale;
    pSavePointMesh->m38_spriteActorCore.m34 = param_1->m34;
    savePointCreationMode1 = (uint)param_1->mB0 >> 9 & 1;
    pSavePointMesh->m38_spriteActorCore.mB0 = pSavePointMesh->m38_spriteActorCore.mB0 & 0xfffffdff | savePointCreationMode1 << 9;

    if (savePointCreationMode1 != 0) {
        pSavePointMesh->m38_spriteActorCore.m3A = param_1->m3A;
        pSavePointMesh->m38_spriteActorCore.m40 = pSavePointMesh->m38_spriteActorCore.m40 & 0xffffe0ff | 0x300;
    }

    pSavePointMesh->m38_spriteActorCore.mA8.mx1E = param_1->mA8.mx1E;
    pSavePointMesh->m38_spriteActorCore.mAC = pSavePointMesh->m38_spriteActorCore.mAC & 0xfffffffc | param_1->mAC & 3;
    pSavePointMesh->m38_spriteActorCore.mB0 = pSavePointMesh->m38_spriteActorCore.mB0 & 0xfffffeffU | param_1->mB0 & 0x100U;
    pSavePointMesh->m38_spriteActorCore.mAC = pSavePointMesh->m38_spriteActorCore.mAC & 0xffffffbf | param_1->mAC & 0x40;
    pSavePointMesh->m38_spriteActorCore.mAC = pSavePointMesh->m38_spriteActorCore.mAC & 0xfff8003f | param_1->mAC & 0x40 | param_1->mAC & 0x7ff80;
    pSavePointMesh->m38_spriteActorCore.mA8.mx0 = 0;
    pSavePointMesh->m38_spriteActorCore.mAC = pSavePointMesh->m38_spriteActorCore.mAC & 0xfff8003b | param_1->mAC & 0x40 | param_1->mAC & 0x7ff80 | param_1->mAC & 4;

    if (((uint)param_1->mA8.mx0) == 0) {
        pSavePointMesh->m38_spriteActorCore.m7C = param_1->m7C;
    }
    else {
        pSavePointMesh->m38_spriteActorCore.m7C = (sFieldEntitySub4_F4*)0x0;
    }

    pSavePointMesh->m38_spriteActorCore.m70 = (int)param_1;
    pSavePointMesh->m38_spriteActorCore.m44_currentAnimationBundle = param_1->m44_currentAnimationBundle;
    pSavePointMesh->m38_spriteActorCore.m48_defaultAnimationbundle = param_1->m48_defaultAnimationbundle;
    pSavePointMesh->m38_spriteActorCore.m74 = param_1->m74;
    pSavePointMesh->m38_spriteActorCore.m82 = param_1->m82;
    pSavePointMesh->m38_spriteActorCore.m50 = param_1->m50;
    pSavePointMesh->m38_spriteActorCore.m8D = (param_1->mAC >> 24) & 0xFF;
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

	iVar2 = ((param1->m18_moveSpeed >> 4) << 8) / (int)(param1->mAC >> 7 & 0xfff);
	iVar1 = getAngleSin((int)param1->m32);
	param1->mC_step.vx = (iVar1 >> 2) * iVar2 >> 6;
	iVar1 = getAngleCos((int)param1->m32);
	param1->mC_step.vz = -((iVar1 >> 2) * iVar2) >> 6;
}

sStackElement* executeSpriteBytecode2Sub3(sSpriteActor* param_1, sPS1Pointer param_2)
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

void setupOverrideTPage(uint x, uint y)
{
    overrideTPageMode = 1;
    overrideTPageValue = GetTPage(0, 0, x & 0xffff, y & 0xffff) & 0x1f;
}

void executeSpriteBytecode2Extended(sSpriteActorCore* param_1, int bytecode, sPS1Pointer param_3)
{
	switch (bytecode & 0xff) {
	case 0x84:
		break;
    case 0x8D:
        setupOverrideTPage(param_1->m24->m4_vramLocation.vx, param_1->m24->m4_vramLocation.vy);
        break;
	case 0x92:
		param_1->m28_colorAndCode.m3_code |= 1; // make transparent
		updateAllSubsprites(param_1);
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
    case 0xBC: // VERY Complicated, used by save points
        if ((READ_LE_U8(param_3) & 0x80) == 0) {
            MissingCode();
        }
        switch (READ_LE_U8(param_3) & 0x3F)
        {
        case 0x16:
            MissingCode();
            break;
        default:
            assert(0);
        }
        break;
	case 0xA0: // set the move speed for the character
		{
			int iVar11 = READ_LE_U8(param_3 )* 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
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
                s32 newValue = READ_LE_U8(param_3) * 0x10 * (fieldDrawEnvsInitialized + 1) * (int)param_1->m82;
                if (newValue < 0) {
                    newValue += 0xfff;
                }
                tempNewAltitude = (newValue >> 0xc) << 8;
            }
            (param_1->mC_step).vy = tempNewAltitude;
            int iVar12 = (param_1->mC_step).vy << 8;
            (param_1->mC_step).vy = iVar12;
            (param_1->mC_step).vy = iVar12 / (int)(param_1->mAC >> 7 & 0xfff);
        }
        break;
    case 0xA3:
        MissingCode(); // save point spinning?
        break;
    case 0xA6:
        MissingCode(); // save point spinning?
        break;
	case 0xC1:
		MissingCode();
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
			param_1->m7C->mC = READ_LE_U16(param_3);
		}
		break;
    case 0xCE:
        MissingCode(); // save point spinning?
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
	case 0xe0:
		spriteBytecode2ExtendedE0(param_1, param_3 + READ_LE_S16(param_3), param_1->m24);
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
    case 0xF5:
    {
        s32 offsetToMesh = READ_LE_U8(param_3) + READ_LE_U8(param_3 + 1) * 0x100 + READ_LE_U8(param_3 + 2) * 0x10000;
        sPS1Pointer meshBlock = param_3 + offsetToMesh;
        sModelBlock* pNewModelBlock = new sModelBlock;
        pNewModelBlock->init(meshBlock.getPointer());
        pNewModelBlock->m_baseItForRelocation = meshBlock.getPointer();
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

u8 popByteFromAnimationStack(sSpriteActorCore* param_1)
{
	return param_1->m8E_stack[param_1->m8C_stackPosition++].asU8;
}

void pushBytecodePointerOnAnimationStack(sSpriteActorCore* param_1, sPS1Pointer param_2)
{
	param_1->m8E_stack[--param_1->m8C_stackPosition].asPs1Pointer = param_2;
}

int spriteCallback2Var0 = 0;

void executeSpriteBytecode2(sSpriteActorCore* param_1)
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
				addToSpriteTransferList(param_1, param_1->m34 + 1);
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
			param_1->mA8.mx1C = 0;
			if (param_1->m68 != nullptr) {
				param_1->m68(param_1);
				return;
			}
			if (-1 < *(char*)&param_1->mB0) {
				spriteActorSetPlayingAnimation(param_1, *(char*)&param_1->mB0);
			}
			param_1->mA8.mx1C = 0;
			return;
		case 0x85:
		case 0x8E:
		case 0x98:
		case 0xBE:
		case 0xC8:
		case 0xD4:
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
		case 0x82: // happen at end of walk cycle animation for player
			if (param_1->m68) {
				param_1->m68(param_1);
			}
			{
				int iVar15 = (param_1->mC_step).vy;
				spriteActorSetPlayingAnimation(param_1, (s8)((param_1->mAC >> 24) & 0xFF));
				(param_1->mC_step).vy = iVar15;
			}
			param_1->m9E_wait = 0;
			executeSpriteBytecode2(param_1);
			return;
        case 0x86: // Happen when Fei jumps in cutscene after lahan battle.
            if (param_1->mC_step.vy < 0)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode = param_1->m64_spriteByteCode + sizePerBytecodeTable[bytecode];
            break;
        case 0x87: // Happen when Fei jumps in cutscene after lahan battle.
            if ((param_1->m0_position.vy >> 16) < param_1->m84)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode = param_1->m64_spriteByteCode + sizePerBytecodeTable[bytecode];
            break;
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
        case 0xE1: // jump in bytecode
            param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
            break;
		case 0xE2: // Looks like a call
			pushBytecodePointerOnAnimationStack(param_1, param_1->m64_spriteByteCode + 3);
			param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
			break;
		case 0xe4: // loop number of time the last byte on the stack is
			{
				s8 cVar4 = popByteFromAnimationStack(param_1);
				if (cVar4 != 0) {
					pushByteOnAnimationStack(param_1, cVar4 - 1);
					param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
				}
				else
				{
					// end of loop
					param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
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

void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActorCore* param_1)
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

void initFieldEntitySub4Sub3(sSpriteActorCore* param_1, int param_2)
{
	sFieldEntitySub4_B4* psVar1;

	psVar1 = param_1->m20->getAsSprite();
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

void setCurrentAnimationPtr(sSpriteActorCore* param_1, const sPS1Pointer startOfAnimation)
{
	uint uVar1;
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
		param_1->mC_step.vz = 0;
		param_1->mC_step.vy = 0;
		param_1->mC_step.vx = 0;
		param_1->m18_moveSpeed = 0;
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
			param_1->m20->getAsSprite()->m3D = 0;
			param_1->m20->getAsSprite()->m3C = 0;
			if (((param_1->m40 >> 0x14 & 1) == 0) && (param_1->m20->getAsSprite()->m34_perSubgroupTransform != nullptr)) {
				resetPerSubgroupTransforms(param_1);
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

void executeSpriteBytecode(sSpriteActorCore* param_1, sPS1Pointer param_2, uint param_3)
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
				addToSpriteTransferList(param_1, param_1->m34 + 1);
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
				addToSpriteTransferList(param_1, param_1->m34 - 1);
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

			param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
		}
	}
}

void setSpriteActorAngle(sSpriteActorCore* param_1, short angle)
{
	short sVar1;
	ushort uVar2;
	uint originalM17;

	param_1->m80 = angle;
	originalM17 = param_1->mA8.mx11;
	if (((int)angle + 0x400U & 1) == 0) {
		param_1->mAC &= ~4;
	}
	else {
		param_1->mAC |= 4;
	}

	if (param_1->m48_defaultAnimationbundle == nullptr) {
		return;
	}

	int newAngle;

	switch (param_1->mA8.mx14)
	{
	case 0:
		if ((angle + 0x400U & 0xfff) < 0x801) {
			param_1->mAC &= ~4;
		}
		else {
			param_1->mAC |= 4;
		}
		param_1->mA8.mx11 = 0;
		param_1->m5C = param_1->m58_startOfCurrentAnimation + 6;
		param_1->m54 = param_1->m58_startOfCurrentAnimation + READ_LE_U16(param_1->m58_startOfCurrentAnimation + 4) + 4;
		break;
	case 2:
	{
		newAngle = angle + 0x500 >> 9 & 7;
		if (newAngle > 4)
		{
			newAngle = newAngle - 5 ^ 3;
			uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + newAngle * 2 + 4);
			param_1->mAC |= 4;
			param_1->m54 = param_1->m58_startOfCurrentAnimation + (uint)uVar2 + newAngle * 2 + 4;
		}
		else
		{
			uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + newAngle * 2 + 4);
			param_1->mAC &= ~4;
			param_1->m54 = param_1->m58_startOfCurrentAnimation + (uint)uVar2 + newAngle * 2 + 4;
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
		uVar2 = READ_LE_U16(param_1->m58_startOfCurrentAnimation + 2);
		sPS1Pointer puVar5 = param_1->m64_spriteByteCode;
		param_1->mA8.mx16 = 0;
		param_1->mA8.mxB = 0x3F;
		param_1->m64_spriteByteCode = param_1->m58_startOfCurrentAnimation + uVar2 + 2;
		executeSpriteBytecode(param_1, puVar5, param_1->mA8.mx16);
		param_1->m9E_wait = sVar1;
	}

	param_1->m3C = param_1->m3C & 0xfffffff7 | (param_1->mAC >> 3 & 1 ^ param_1->mAC >> 2 & 1) << 3;
}

void initFieldEntitySub4Sub5Sub0(sFieldEntitySub4_110* param_1, sSpriteActorAnimationBundle* param_2, sVec2_s16 param_3_vramLocation, sVec2_s16 clut)
{
	param_1->m4_vramLocation = param_3_vramLocation;
	param_1->m8_clut = clut;
	param_1->mC = param_2->mC_pData;
	param_1->m0 = param_2->m8_pData;
	initFieldVar4 = 0;
	param_1->m10_startOfAnimationContainer = &param_2->m4_animations;

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


int initFieldEntitySub4Sub5Sub1(const sPS1Pointer& param_1)
{
	return READ_LE_U8(param_1 + 1) >> 7;
}

void setAnimationBundle(sSpriteActorCore* param_1, sSpriteActorAnimationBundle* pAnimationBundle)
{
	sFieldEntitySub4_110* psVar2 = param_1->m24;
	if (pAnimationBundle != 0) {
		if (pAnimationBundle != param_1->m44_currentAnimationBundle) {
			initFieldEntitySub4Sub5Sub0(psVar2, pAnimationBundle, psVar2->m4_vramLocation, psVar2->m8_clut);
			param_1->m44_currentAnimationBundle = pAnimationBundle;
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

void spriteActorSetPlayingAnimation(sSpriteActorCore* param_1, int animationId)
{
	ushort uVar1;
	uint uVar2;
	int iVar3;

	if (param_1->m48_defaultAnimationbundle == nullptr) {
		param_1->m64_spriteByteCode.makeNull();
	}
	else {
		if (param_1->m44_currentAnimationBundle == param_1->m48_defaultAnimationbundle) {
			uVar2 = param_1->mB0 & ~0x400;
		}
		else {
			uVar2 = param_1->mB0 | 0x400;
		}
		param_1->mB0 = uVar2;
		if (animationId < 0) {
			setAnimationBundle(param_1, param_1->m4C_specialAnimation);
			if ((isBattleOverlayLoaded != 0) &&
				(iVar3 = initFieldEntitySub4Sub5Sub1(param_1->m24->m0), iVar3 == 0)) {
				param_1->m24->m4_vramLocation.vx = 0x100;
				param_1->m24->m4_vramLocation.vy = 0x300;
			}
		}
		else {
			setAnimationBundle(param_1, param_1->m48_defaultAnimationbundle);
			if (isBattleOverlayLoaded != 0) {
				param_1->m24->m4_vramLocation = param_1->m7C->mE_vramLocation;
			}
		}
		param_1->mAC = (param_1->mAC & 0x00FFFFFF) | (((char)animationId) << 24);
		if ((int)animationId < 0) {
			animationId = ~animationId;
		}

		param_1->m40 = param_1->m40 | 0x100000;
		param_1->m58_startOfCurrentAnimation = (*param_1->m24->m10_startOfAnimationContainer)[animationId];
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
	pThis->m24 = &pThis->m110;
	pThis->m20->getAsSprite()->m38_pNext = 0;
}

sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7)
{
	initFieldEntitySub4Sub1(param_1);
	initFieldEntitySub4Sub2(param_1);
	initFieldEntitySub4Sub3(param_1, 0x1000);

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

	param_1->m6C_pointerToOwnerStructure = param_1;
	param_1->m3C = param_1->m3C & 0xff00ffff | (initFieldVar2 & 0xf) << 0x14 | (initFieldVar2 & 0xf) << 0x10;;

	int count = initFieldEntitySub4Sub4(pSetup->m8_pData);
	param_1->m20->getAsSprite()->m2C = param_1->m20->getAsSprite()->m30 = new std::vector<sFieldEntitySub4_B4_sub>;
	param_1->m20->getAsSprite()->m30->resize(count);

	param_1->m24->m4_vramLocation.vx = vramX;
	param_1->m24->m4_vramLocation.vy = vramY;
	param_1->m24->m8_clut.vx = clutX;
	param_1->m24->m8_clut.vy = clutY;

	param_1->m48_defaultAnimationbundle = pSetup;

	setAnimationBundle(param_1, pSetup);
	//param_1->m60 = param_1->m24->m10_startOfAnimationContainer + (READ_LE_U16(param_1->m24->m10_startOfAnimationContainer) & 0x3F) + 1;
	MissingCode();

	spriteActorSetPlayingAnimation(param_1, 0);

	return param_1;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sSpriteActor* param_1, int param_2, int* param_3, int* param_4, int* param_5)
{
	byte bVar1;
	sPS1Pointer pbVar2;
	int iVar3;
	int iVar4;
	uint uVar5;

	pbVar2 = (*param_1->m24->m10_startOfAnimationContainer)[0];
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

void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActor* param1, int param2)
{
	param1->m32 = param2;
	computeStepFromMoveSpeed(param1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActorCore* param1, void(*callback)(sSpriteActorCore*))
{
	param1->m68 = callback;
}

void fieldActorCallback(sSpriteActorCore* pThis)
{
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

void setTransparencyMode(sSpriteActor* sprite, u32 mode)
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