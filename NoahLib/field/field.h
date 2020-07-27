#pragma once

#include "fieldModel.h"

typedef s32 sFP1616;

struct MATRIX
{
    std::array<std::array<s16, 3>, 3> m;
    std::array<s32, 3> t;
};

struct VECTOR
{
    s32 vx;
    s32 vy;
    s32 vz;
};

typedef std::array<s16, 3> SVECTOR;

typedef MATRIX sMatrix;


typedef std::array<s16, 2> sVec2_s16;

struct sFieldActorSetupParams
{
    void init(std::vector<u8>::iterator inputData)
    {
        rawData = &inputData[0];

        m4_offset = READ_LE_U32(inputData + 4);
        m4_pData = &(*(inputData + m4_offset));

        m8_offset = READ_LE_U32(inputData + 8);
        m8_pData = &(*(inputData + m8_offset));

        mC_offset = READ_LE_U32(inputData + 0xC);
        mC_pData = &(*(inputData + mC_offset));

        u32 end = READ_LE_U32(inputData + 0x10);
    }

    u8* rawData;

    u32 m4_offset;
    u8* m4_pData;
    u32 m8_offset;
    u8* m8_pData;
    u32 mC_offset;
    u8* mC_pData;

};

struct sFieldEntitySub0
{
    std::vector<sModelBlock>::iterator m4_pModelBlock;
    std::vector<s16> m8;
    std::vector<s16>::iterator mC_end;
    //size 0x24
};

struct sFieldEntity2dSprite
{
    //size 0x70
};


struct sFieldScriptEntityScriptSlot
{
    u16 m0_scriptPC;
    u8 m2_delay;
    u8 m3;
    u32 m4_flags;
    // size 8
};

struct sFieldScriptEntity
{
    u32 m0_flags;
    u32 m4_flags;
    s16 m18[4];
    VECTOR m20_position;
    s16 m5A;
    s16 m60;
    s16 m64;
    s16 m72_elevation;
    s8 m80_DialogAvatarFace;
    std::array<sFieldScriptEntityScriptSlot, 8> m8C_scriptSlots;
    u16 mCC_scriptPC;
    s8 mCE_currentScriptSlot;
    s16 mE4_playableCharacterId;
    s16 mEC_elevation;
    s16 m104_rotation;
    s16 m106_currentRotation;
    s16 m108_rotation3;
    u8 m126;
    u8 m127;
    struct
    {
        u32 m28 : 2;
    }m130;

    struct
    {
        u32 m0 : 4;
        u32 m4 : 1;
    }m134;

    // size 0x138
};

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
    sFieldEntitySub4_124* m34;
    s32 m38;
    s8 m3C;
    s8 m3D;
    //size ???
};

struct sFieldEntitySub4_110
{
    u8* m0;
    sVec2_s16 m4_vramLocation;
    sFP1616 m8;
    u8* mC;
    u8* m10;
    // size ???
};

struct sFieldEntitySub4
{
    VECTOR m0_position;
    s32 mC;
    s32 m10;
    s32 m14;
    s32 m18;
    s32 m1C;
    sFieldEntitySub4_B4* m20;
    sFieldEntitySub4_110* m24;
    s8 m2B;
    s16 m2C;
    s16 m30;
    s16 m32;
    s16 m34;
    s16 m3A;
    u32 m3C;
    u32 m40;
    sFieldActorSetupParams* m44;
    sFieldActorSetupParams* m48;
    sFieldActorSetupParams* m4C;
    s32 m50;
    u8* m54;
    u16* m58;
    void* m60;
    u8* m64_spriteByteCode;
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
    sFieldEntitySub4_124 m124;
    //size 0x164
};

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    sFieldEntitySub4* m4_pSub4;
    sFieldEntity2dSprite* m8_2dSprite;
    sMatrix mC_matrix;
    sMatrix m2C_matrixBackup;
    sFieldScriptEntity* m4C_scriptEntity;
    s16 m50_modelRotation[3];
    u16 m58_flags;
    u16 m5A;
    //size 0x5C
};
extern std::vector<sFieldEntity> fieldEntityArray;

extern s32 requestFieldId0;
extern s32 fieldChangePrevented;
extern s32 fieldChangePrevented2;

void setVar(int varIndex, s16 value);

void fieldEntryPoint();
