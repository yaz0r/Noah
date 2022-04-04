#pragma once
#include "kernel/math.h"

extern int spriteCallback2Var0;
extern s8 isBattleOverlayLoaded;
extern s8 isOtherOverlayLoaded;

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

struct sFieldEntitySub4_124
{
	s8 m0_translateX;
	s8 m1_translateY;
	s16 m2_rotateX;
	s16 m4_rotateY;
	s16 m6_rotateZ;
	//size 8
};

struct sColorAndCode
{
	u8 m0_r;
	u8 m1_g;
	u8 m2_b;
	u8 m3_code;
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
	u32 m14;
	//size 0x18
};

struct sFieldEntitySub4_B4
{
	SFP_VEC4 m0_rotation;
	SFP_VEC4 m6_scale;
	MATRIX mC_spriteMatrix;
	std::vector<sFieldEntitySub4_B4_sub>* m2C;
	std::vector<sFieldEntitySub4_B4_sub>* m30;
	std::array<sFieldEntitySub4_124, 8>* m34_perSubgroupTransform;
	struct sSpriteActor* m38;
	s8 m3C;
	s8 m3D;
	//size ???
};

struct sFieldEntitySub4_110
{
	sPS1Pointer m0;
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

struct sSpriteActor
{
	FP_VEC3 m0_position;
	FP_VEC3 mC_step;
	s32 m18_moveSpeed;
	s32 m1C;
	sFieldEntitySub4_B4* m20;
	sFieldEntitySub4_110* m24;
	sColorAndCode m28_colorAndCode;
	s16 m2C_scale;
	s16 m30;
	s16 m32;
	s16 m34;
	s16 m36;
	s16 m38;
	s16 m3A;
	u32 m3C;
	u32 m40;
	struct sSpriteActorAnimationBundle* m44_currentAnimationBundle;
	struct sSpriteActorAnimationBundle* m48_defaultAnimationbundle;
	struct sSpriteActorAnimationBundle* m4C_specialAnimation;
	s32 m50;
	sPS1Pointer m54;
	sPS1Pointer m58_startOfCurrentAnimation;
	sPS1Pointer m5C;
	sPS1Pointer m60;
	sPS1Pointer m64_spriteByteCode;
	void(*m68)(sSpriteActor*);
	sSpriteActor* m6C;
	s32 m70;
	sFieldEntitySub4_F4* m7C;
	s16 m80;
	s16 m82;
	s16 m84;
	u16 m86_thisSize;
	s8 m8C_stackPosition;
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
	sFieldEntitySub4_B4 mB4;
	sFieldEntitySub4_F4 mF4;
	sFieldEntitySub4_110 m110;
	std::array<sFieldEntitySub4_124, 8> m124;
	//size 0x164
};

extern sSpriteActor* spriteTransfertListHead;

void OP_21_sub(sSpriteActor* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sSpriteActor* param_1);
sSpriteActor* initializeSpriteActor(sSpriteActor* param_1, sSpriteActorAnimationBundle* pSetup, int clutX, int clutY, int vramX, int vramY, int param_7);
void spriteActorSetPlayingAnimation(sSpriteActor* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sSpriteActor* param1, int param2);

void deleteFieldEntitySub4(sSpriteActor* param_1);
sSpriteActor* createSpriteActor(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
sSpriteActor* createSpriteActorEX(sSpriteActorAnimationBundle* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6, int param_7);

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sSpriteActor* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sSpriteActor* param_1, int param_2, int* param_3, int* param_4, int* param_5);
void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sSpriteActor* param1, int param2);

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sSpriteActor* param1, void(*callback)(sSpriteActor*));
void fieldActorCallback(sSpriteActor* pThis);

void setSpriteActorAngle(sSpriteActor* param_1, short param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(sSpriteActor* param_1);

void setTransparencyMode(sSpriteActor* param_1, u32 mode);