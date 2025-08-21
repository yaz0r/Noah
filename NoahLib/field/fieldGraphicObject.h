#pragma once
#include "kernel/math.h"
#include "psx/libgpu.h"
#include <optional>

#include "kernel/taskLists.h"

#include "sprite/spriteSetup.h"

extern s8 isBattleOverlayLoaded;
extern int battleDefaultEntityScale;

struct sFieldEntitySub4_124
{
	s8 m0_translateX;
	s8 m1_translateY;
	s16 m2_rotateX;
	s16 m4_rotateY;
	s16 m6_rotateZ;
	//size 8
};

struct sFieldEntitySub4_F4
{
    s32 m0;
    s32 m4;
    s32 m8;
    s16 mC;
    sVec2_s16 mE_vramLocation;
    s16 m14_actorId;
    std::optional<std::span<u8>::iterator> m18;
    //size ???
};

struct sFieldEntitySub4_B4_sub
{
	s16 m0;
	s16 m2;
	u8 m4_texcoordX;
	u8 m5_texcoordY;
	u8 m6_width;
	u8 m7_height;
	s8 m8;
	s8 m9;
	s16 mA_tpage;
	u16 mC_clut;
	sColorAndCode m10_colorAndCode;

    union {
        struct {
            u32 m0x0 : 3;
            u32 m0x8 : 1;
            u32 m0x10_tileFlipX : 1;
            u32 m0x20_tileFlipY : 1;
        };
        u32 raw;
    } m14;
	//size 0x18
};

struct sFieldEntitySub4_B4_base {
    SVECTOR m0_rotation;
    SVECTOR m6_scale;
    MATRIX mC_matrix;

    virtual ~sFieldEntitySub4_B4_base() = default;
    virtual struct sFieldEntitySub4_B4* getAsSprite() = 0;
    virtual struct sFieldEntitySub4_B4_alt* getAsObject() = 0;
};

struct sFieldEntitySub4_B4 : public sFieldEntitySub4_B4_base
{
    virtual struct sFieldEntitySub4_B4* getAsSprite() { return this; }
    virtual struct sFieldEntitySub4_B4_alt* getAsObject() { assert(0); return nullptr; }

    std::vector<sFieldEntitySub4_B4_sub>* m2C;
    std::vector<sFieldEntitySub4_B4_sub>* m30;
	std::array<sFieldEntitySub4_124, 8>* m34_perSubgroupTransform;
	struct sSpriteActorCore* m38_pNext;
	s8 m3C;
	s8 m3D;
	//size ???
};

// size would be 0x54 or 0x58? Or there is 2 versions of it
struct sFieldEntitySub4_B4_alt : public sFieldEntitySub4_B4_base
{
    virtual struct sFieldEntitySub4_B4* getAsSprite() { assert(0); return nullptr; }
    virtual struct sFieldEntitySub4_B4_alt* getAsObject() { return this; }

    std::array<std::vector<struct sTag*>, 2> m2C;
    std::vector<sFieldEntitySub4_B4_sub>* m30;
    struct sModelBlock* m34_pModelBlock;
    void* m38;
    u32 m40;
};

struct sFieldEntitySub4_110
{
    struct sFieldEntitySub4_110_0* m0_spriteData = nullptr;
	sVec2_s16 m4_vramLocation = sVec2_s16(0,0);
	sVec2_s16 m8_clut = sVec2_s16(0,0);
	std::span<u8>* mC = nullptr;
    sSpriteActorAnimationBundle::sAnimationBundle* m10_startOfAnimationContainer = nullptr;
	// size 0x14 (guessed, but seems to make sense)
};

struct sStackElement
{
	u8 asU8 = 0;
	u16 asU16 = 0;
    std::span<u8>::iterator asPs1Pointer;
};

struct sSpriteActorCore {
    FP_VEC3 m0_position = { 0,0,0 };
    FP_VEC3 mC_step = { 0,0,0 };
    s32 m18_moveSpeed = 0;
    sFixedPoint m1C_gravity = sFixedPoint::fromValue(0);
    sFieldEntitySub4_B4_base* m20 = nullptr;
    sFieldEntitySub4_110* m24_vramData = nullptr;
    sColorAndCode m28_colorAndCode = sColorAndCode::zero();
    s16 m2C_scale = 0;
    s16 m2E = 0;
    s16 m30 = 0;
    s16 m32_direction = 0;
    s16 m34_currentSpriteFrame = 0;
    s16 m36 = 0;
    s16 m38 = 0;
    s16 m3A = 0;
    u32 m3C = 0;

    /*
    * 0x00100000: related to having sub group transforms array
    * 0x00020000 = added to shape transfer list
    */
    u32 m40 = 0;

    struct sSpriteActorAnimationBundle* m44_currentAnimationBundle = nullptr;
    struct sSpriteActorAnimationBundle* m48_defaultAnimationbundle = nullptr;
    struct sSpriteActorAnimationBundle* m4C_specialAnimation = nullptr;
    std::optional<std::vector<u8>::const_iterator> m50;
    std::span<u8>::iterator m54;
    std::span<u8>::iterator m58_startOfCurrentAnimation;
    std::span<u8>::iterator m5C;
    std::optional<std::span<u8>::iterator> m60_endOfAnimationContainer;
    std::optional<std::span<u8>::iterator> m64_spriteByteCode;

    void(*m68)(sSpriteActorCore*) = nullptr;
    union {
        struct sTaskHeader* m6C_pointerToOwnerStructure = nullptr;
        struct sTaskHeaderPair* m6C_pointerToOwnerStructurePair;
        struct sSpriteActor* m6C_pointerToOwnerStructureSpriteActor;
    };
    sSpriteActorCore* m70 = nullptr;
    sSpriteActorCore* m74_pTargetEntitySprite = nullptr;
    u32 m78 = 0;
    sFieldEntitySub4_F4* m7C = nullptr;
    s16 m80 = 0;
    s16 m82 = 0;
    s16 m84_maxY = 0;
    u16 m86_thisSize = 0;
    std::span<u8>::iterator m88_stack2;
    s8 m8C_stackPosition = 0;
    s8 m8D = 0;
    std::array<sStackElement, 16> m8E_stack;
    u16 m9E_wait = 0;
    SVECTOR mA0;
    struct sMA8{
        u32 mx0 : 1;
        u32 mx1 : 10;
        u32 mxB : 6;
        u32 mx11 : 3;
        u32 mx14 : 2;
        u32 mx16 : 6;
        u32 mx1C : 2;
        u32 mx1E_entityId_bottom2bit : 2;

        sMA8() {
            clear();
        }

        void clear()
        {
            memset(this, 0, sizeof(*this));
        }
    } mA8;
    union sMAC {
        struct {
            u32 mx0_entityIdUpper2bit : 2;
            u32 mx2_facing : 1;
            u32 mx3 : 1;
            u32 mx4_dummy : 1;
            u32 mx5 : 1;
            u32 mx6 : 1;
            u32 mx7_timeScale : 12;
            u32 mx13_dummy : 5;
            s32 mx18 : 8;
        };
        u32 mRaw;

        sMAC() {
            mRaw = 0;
        }
    } mAC;
    union sMB0 {
        struct {
            s32 mx0_animationId : 8;
            u32 mx8 : 1;
            u32 mx9 : 1;
            u32 mx10 : 1;
        };
        u32 mRaw;

        sMB0() {
            mRaw = 0;
        }
    } mB0;
};

struct sSpriteActor : public sSpriteActorCore
{
    sSpriteActor() : m0_spriteActorCore(*this) {

    }
    sSpriteActorCore& m0_spriteActorCore;
	sFieldEntitySub4_B4 mB4;
	sFieldEntitySub4_F4 mF4;
	sFieldEntitySub4_110 m110;
	std::array<sFieldEntitySub4_124, 8> m124;
	//size 0x164
};

struct sSavePointMesh_data1
{
    sSpriteActorCore m0_spriteActorCore;
    sFieldEntitySub4_B4_alt mB4;
    std::vector<sFieldEntitySub4_B4_sub> mF4;
};

struct sSavePointMesh_data2
{
    sSpriteActorCore m0_spriteActorCore;
    sFieldEntitySub4_B4_alt mB4;
};


// base size of one of those is expected to be 0xEC
struct sSavePointMeshAbstract : public sTaskHeaderPair {
    sSpriteActorCore m38_spriteActorCore;
};


// size of this one is 0xEC(base size) + 0x58(size of sFieldEntitySub4_B4?) and 0x18*count (size of sFieldEntitySub4_B4_sub)
struct sSavePointMesh1 : public sSavePointMeshAbstract
{
    sFieldEntitySub4_B4 mB4;
    std::vector<sFieldEntitySub4_B4_sub> mF4;
};

// size of this one is 0xEC(base size) + 0x54(size of sFieldEntitySub4_B4_alt?)
struct sSavePointMesh2 : public sSavePointMeshAbstract
{
    sFieldEntitySub4_B4_alt mB4;
};

extern sSpriteActorCore* spriteTransfertListHead;

void savePointCallback8Sub0(sSpriteActorCore* param_1);
void registerSpriteCallback2(sTaskHeader* param_1, sTaskHeader* param_2);

void SetTimeScale(sSpriteActor* param_1, int param_2); // 0x80021BCC
void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActorCore* param_1);
sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7);
void spriteActorSetPlayingAnimation(sSpriteActorCore* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActorCore* param1, int param2);

void deleteFieldEntitySub4(sSpriteActor* param_1);
sSpriteActor* createSpriteActor(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
sSpriteActor* createSpriteActorEX(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6, int param_7);

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sSpriteActor* param_1, int param_2);
void getBoundingVolumeFromSprite(sSpriteActor* param_1, int param_2, sFixedPoint* pX, sFixedPoint* pY, sFixedPoint* pZ);
void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sSpriteActor* param1, int param2);

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActorCore* param1, void(*callback)(sSpriteActorCore*));
void fieldActorCallback(sSpriteActorCore* pThis);

void setSpriteActorAngle(sSpriteActorCore* param_1, short param_2);
void resetPerSubgroupTransforms(sSpriteActorCore* param_1);

void setTransparencyMode(sSpriteActorCore* param_1, u32 mode);
void setCurrentAnimationPtr(sSpriteActorCore* param_1, std::span<u8>::iterator startOfAnimation);

void uploadTextureToVram(std::span<u8>::iterator param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY);

void spriteCallback_render2_updateMatrix(sSpriteActorCore* param_1);

void setGraphicEntityScale(sSpriteActorCore* param_1, int param_2);

void resetSpriteCallbacks(void);

int isVramPreBacked(const sFieldEntitySub4_110_0* param_1);
void addToSpriteTransferList(sSpriteActorCore* param_1, short param_2);
void executeSpriteBytecode2Sub1(sSpriteActorCore* param_1);
std::span<u8>::iterator popPointerFromAnimationStack(sSpriteActorCore* param_1);
void pushBytecodePointerOnAnimationStack(sSpriteActorCore* param_1, std::span<u8>::iterator param_2);
void executeSpriteBytecode2Extended(sSpriteActorCore* param_1, int bytecode, std::span<u8>::iterator param_3);
u8 popByteFromAnimationStack(sSpriteActorCore* param_1);
void pushByteOnAnimationStack(sSpriteActorCore* param_1, u8 param);
void savePointCallback8Sub0Sub0(sSpriteActorCore* param_1);
void savePointCallback8Sub0Sub0_battle(sSpriteActorCore* param_1);
void updateAllSubsprites(sSpriteActorCore* param_1);
void registerSpriteCallback2_2(sTaskHeader* param_1);
void spriteBytecode2ExtendedE0(sSpriteActorCore* param_1, std::span<u8>::iterator param_2, sFieldEntitySub4_110* param_3);

extern const std::array<u8, 256> sizePerBytecodeTable;
extern u32 allocateSavePointMeshDataSub0_var0;
extern u8 spriteBytecode2ExtendedE0_Var0;


template <typename T>
T* createCustomRenderableEntity(size_t param_1, sTaskHeader* param_2, void(*updateCallback)(sTaskHeader*), void(*drawCallback)(sTaskHeader*), void(*deleteCallback)(sTaskHeader*)) {
    T* pNewEntity = new T;
    allocateSavePointMeshDataSub0(param_2, pNewEntity);
    registerSpriteCallback2(pNewEntity, &pNewEntity->m1C);
    setTaskUpdateFunction(pNewEntity, updateCallback);
    setTaskDrawFunction(&pNewEntity->m1C, drawCallback);
    if (deleteCallback == nullptr) {
        assert(0);
    }
    else {
        setTaskDeleteFunction(pNewEntity, deleteCallback);
    }

    // TODO: Gross!
    pNewEntity->m4 = (sSpriteActorCore*)pNewEntity;
    pNewEntity->m1C.m4 = (sSpriteActorCore*)pNewEntity;

    return pNewEntity;
}

extern sFieldEntitySub4_110 sFieldEntitySub4_110_8005a474;
void initFieldEntitySub4Sub5Sub0(sFieldEntitySub4_110* param_1, sSpriteActorAnimationBundle* param_2, sVec2_s16 param_3_vramLocation, sVec2_s16 clut);

extern sFieldEntitySub4_110 createSavePointMeshData_mode5;
int addAndClamp(int param_1, int param_2);
int modulateSpeed(sSpriteActorCore* param_1, int param_2);

extern int initFieldVar5;
extern int initFieldVar4;

void initFieldEntitySub4Sub1(sSpriteActorCore* param_1);
void initFieldEntitySub4Sub2(sSpriteActor* pThis);
void setAnimationBundle(sSpriteActorCore* param_1, sSpriteActorAnimationBundle* pAnimationBundle);