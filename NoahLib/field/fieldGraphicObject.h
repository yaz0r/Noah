#pragma once
#include "kernel/math.h"

struct sFieldEntitySub4_F4
{
    s32 m0;
    s32 m4;
    s32 m8;
    s16 mC;
    sVec2_s16 mE_vramLocation;
    s16 m14_actorId;
    s32 m18;
    //size ???
};

struct sFieldEntitySub4_124
{
    s8 m0;
    s8 m1;
    s16 m2;
    s16 m4;
    s16 m6;
    //size 8
};

struct sFieldEntitySub4_B4
{
    SVECTOR m0_rotation;
    SVECTOR m6_scale;
    MATRIX m20;
    u8* m2C;
    u8* m30;
    std::array<sFieldEntitySub4_124, 8>* m34;
    s32 m38;
    s8 m3C;
    s8 m3D;
    //size ???
};

struct sFieldEntitySub4_110
{
    sPS1Pointer m0;
    sVec2_s16 m4_vramLocation;
    sFP1616 m8;
    sPS1Pointer mC;
    sPS1Pointer m10;
    // size ???
};

struct sFieldEntitySub4
{
    sVec3 m0_position;
    sVec3 mC;
    s32 m18;
    s32 m1C;
    sFieldEntitySub4_B4* m20;
    sFieldEntitySub4_110* m24;
    s8 m2B;
    s16 m2C_scale;
    s16 m30;
    s16 m32;
    s16 m34;
    s16 m3A;
    u32 m3C;
    u32 m40;
    struct sFieldActorSetupParams* m44;
    struct sFieldActorSetupParams* m48;
    struct sFieldActorSetupParams* m4C_specialAnimation;
    s32 m50;
    sPS1Pointer m54;
    sPS1Pointer m58;
    sPS1Pointer m60;
    sPS1Pointer m64_spriteByteCode;
    void(*m68)(sFieldEntitySub4*);
    sFieldEntitySub4* m6C;
    s32 m70;
    sFieldEntitySub4_F4* m7C;
    s16 m80;
    s16 m82;
    s16 m84;
    u16 m86_thisSize;
    s8 m8C;
    u16 m9E;
    u32 mAC;
    u32 mA8;
    u32 mB0;
    sFieldEntitySub4_B4 mB4;
    sFieldEntitySub4_F4 mF4;
    sFieldEntitySub4_110 m110;
    std::array<sFieldEntitySub4_124, 8> m124;
    //size 0x164
};

void OP_21_sub(sFieldEntitySub4* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub9(sFieldEntitySub4* param_1);
sFieldEntitySub4* initFieldEntitySub4(sFieldEntitySub4* param_1, sFieldActorSetupParams* pSetup, int param_3, int param_4, int vramX, int vramY, int param_7);
void OP_INIT_ENTITY_SCRIPT_sub0Sub6(sFieldEntitySub4* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sFieldEntitySub4* param1, int param2);

void deleteFieldEntitySub4(sFieldEntitySub4* param_1);
sFieldEntitySub4* createFieldEntitySub4(sFieldActorSetupParams* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6);
sFieldEntitySub4* createFieldEntitySub4Ext(sFieldActorSetupParams* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6, int param_7);

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sFieldEntitySub4* param_1, int param_2);
void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sFieldEntitySub4* param_1, int param_2, int* param_3, int* param_4, int* param_5);
void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sFieldEntitySub4* param1, int param2);

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sFieldEntitySub4* param1, void(*callback)(sFieldEntitySub4*));
void fieldActorCallback(sFieldEntitySub4* pThis);

