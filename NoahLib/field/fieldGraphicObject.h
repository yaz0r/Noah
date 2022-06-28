#pragma once
#include "kernel/math.h"
#include "psx/gpuprims.h"

extern int spriteCallback2Var0;
extern s8 isBattleOverlayLoaded;

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
    sPS1Pointer m18;
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
    SFP_VEC4 m0_rotation;
    SFP_VEC4 m6_scale;
    MATRIX mC_matrix;

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
    struct sFieldEntitySub4_110_0* m0_spriteData;
	sVec2_s16 m4_vramLocation;
	sVec2_s16 m8_clut;
	sPS1Pointer mC;
	std::vector<sPS1Pointer>* m10_startOfAnimationContainer;
	// size 0x14 (guessed, but seems to make sense)
};

struct sStackElement
{
	u8 asU8;
	u16 asU16;
	sPS1Pointer asPs1Pointer;
};

struct sCustomRenderable {
    virtual struct sSpriteActorCore* getAsSpriteActorCore() { assert(0); return nullptr; }
};

struct sSpriteActorCore : public sCustomRenderable {
    virtual struct sSpriteActorCore* getAsSpriteActorCore() override { return this; }

    FP_VEC3 m0_position;
    FP_VEC3 mC_step;
    s32 m18_moveSpeed;
    s32 m1C_gravity;
    sFieldEntitySub4_B4_base* m20 = nullptr;
    sFieldEntitySub4_110* m24_vramData;
    sColorAndCode m28_colorAndCode;
    s16 m2C_scale;
    s16 m30;
    s16 m32;
    s16 m34_currentSpriteFrame;
    s16 m36;
    s16 m38;
    s16 m3A;
    u32 m3C;

    /*
    * 0x00100000: related to having sub group transforms array
    * 0x00020000 = added to shape transfer list
    */
    u32 m40;

    struct sSpriteActorAnimationBundle* m44_currentAnimationBundle;
    struct sSpriteActorAnimationBundle* m48_defaultAnimationbundle;
    struct sSpriteActorAnimationBundle* m4C_specialAnimation;
    s32 m50;
    sPS1Pointer m54;
    sPS1Pointer m58_startOfCurrentAnimation;
    sPS1Pointer m5C;
    //sPS1Pointer m60_endOfAnimationContainer; weird thing, skipped its init for now
    sPS1Pointer m64_spriteByteCode;
    void(*m68)(sSpriteActorCore*);
    void* m6C_pointerToOwnerStructure;
    sSpriteActorCore* m70;
    u32 m74;
    u32 m78;
    sFieldEntitySub4_F4* m7C;
    s16 m80;
    s16 m82;
    s16 m84_maxY;
    u16 m86_thisSize;
    s8 m8C_stackPosition;
    s8 m8D;
    std::array<sStackElement, 16> m8E_stack;
    u16 m9E_wait;
    struct {
        u32 mx0 : 1;
        u32 mx1 : 10;
        u32 mxB : 6;
        u32 mx11 : 3;
        u32 mx14 : 2;
        u32 mx16 : 6;
        u32 mx1C : 2;
        u32 mx1E : 2;

        void clear()
        {
            memset(this, 0, sizeof(*this));
        }
    } mA8;
    u32 mAC; // animation speed >> 7 & 0xFFF
    u32 mB0;
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


struct sCustomRenderableEntityHeader
{
    void* m0_owner;
    sCustomRenderable* m4;
    void (*m8)(sCustomRenderableEntityHeader*);
    void (*mC)(sCustomRenderableEntityHeader*);
    u32 m10;
    u32 m14;
    sCustomRenderableEntityHeader* m18_pNext;
};

// base size of one of those is expected to be 0xEC
struct sSavePointMeshAbstract {
    sCustomRenderableEntityHeader m0;
    sCustomRenderableEntityHeader m1C;
    sSpriteActorCore m38_spriteActorCore;
};


// size of this one is 0xEC(base size) + 0x58(size of sFieldEntitySub4_B4_alt? but that is 4 off) and 0x18*count (size of sFieldEntitySub4_B4_sub)
struct sSavePointMesh1 : public sSavePointMeshAbstract
{
    sFieldEntitySub4_B4_alt mB4;
    std::vector<sFieldEntitySub4_B4_sub> mF4;
};

// size of this one is 0xEC(base size) + 0x54(size of sFieldEntitySub4_B4_alt?)
struct sSavePointMesh2 : public sSavePointMeshAbstract
{
    sFieldEntitySub4_B4_alt mB4;
};

extern sSpriteActorCore* spriteTransfertListHead;
extern sCustomRenderableEntityHeader* spriteCallback2Head;

void SetTimeScale(sSpriteActor* param_1, int param_2); // 0x80021BCC
void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActorCore* param_1);
sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7);
void spriteActorSetPlayingAnimation(sSpriteActorCore* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActor* param1, int param2);

void deleteFieldEntitySub4(sSpriteActor* param_1);
sSpriteActor* createSpriteActor(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
sSpriteActor* createSpriteActorEX(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6, int param_7);

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sSpriteActor* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sSpriteActor* param_1, int param_2, int* param_3, int* param_4, int* param_5);
void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sSpriteActor* param1, int param2);

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActorCore* param1, void(*callback)(sSpriteActorCore*));
void fieldActorCallback(sSpriteActorCore* pThis);

void setSpriteActorAngle(sSpriteActorCore* param_1, short param_2);
void resetPerSubgroupTransforms(sSpriteActorCore* param_1);

void setTransparencyMode(sSpriteActor* param_1, u32 mode);
void setCurrentAnimationPtr(sSpriteActorCore* param_1, const sPS1Pointer startOfAnimation);

void uploadTextureToVram(sPS1Pointer param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY);
void setupOverrideTPage(uint x, uint y);
void setupOverrideClut(uint x, uint y);

void spriteCallback_render2_updateMatrix(sSpriteActorCore* param_1);

void setGraphicEntityScale(sSpriteActorCore* param_1, int param_2);

void resetSpriteCallbacks(void);