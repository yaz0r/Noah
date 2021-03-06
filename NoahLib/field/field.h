#pragma once

#include "fieldModel.h"

struct sFP1616
{
    u32 value;

    void set(s16 x, s16 y)
    {
        value = ((u16)x) * 0x10000 + (u16)y;
    }

    void get(s16& x, s16& y)
    {
        x = (s16)(value >> 16);
        y = (s16)(value & 0xFFFF);
    }
};

struct MATRIX
{
    std::array<std::array<s16, 3>, 3> m;
    std::array<s32, 3> t;
};

struct sVec3
{
    s32 vx;
    s32 vy;
    s32 vz;
};

struct sVec2_s16
{
    s32 vx;
    s32 vy;
};

typedef MATRIX sMatrix;

struct sFieldActorSetupParams
{
    void init(std::vector<u8>::iterator& inputData)
    {
        rawData = &inputData[0];

        m4_offset = READ_LE_U32(inputData + 4);
        m4_pData.setPointer(&(*(inputData + m4_offset)));

        m8_offset = READ_LE_U32(inputData + 8);
        m8_pData.setPointer(&(*(inputData + m8_offset)));

        mC_offset = READ_LE_U32(inputData + 0xC);
        mC_pData.setPointer(&(*(inputData + mC_offset)));

        u32 end = READ_LE_U32(inputData + 0x10);
    }

    u8* rawData;

    u32 m4_offset;
    sPS1Pointer m4_pData;
    u32 m8_offset;
    sPS1Pointer m8_pData;
    u32 mC_offset;
    sPS1Pointer mC_pData;

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
    u8 m3_scriptIndex;
    struct {
        u32 m0 : 16;
        u32 m16 : 2;
        u32 m18 : 4;
        u32 m22 : 1;
        u32 m23_walkMode : 2;
        u32 m25X : 5;
    } m4_flags; // bit 18, size 4
    // size 8
};

struct sFieldScriptEntity
{
    u32 m0_flags;
    u32 m4_flags;
    std::array<s16, 4> m8_currentWalkMeshTriangle;
    u16 m10_walkmeshId;
    s32 m14_currentTriangleFlag;
    std::array<s16, 4> m18;
    VECTOR m20_position;
    VECTOR m30_stepVector;
    VECTOR m40;
    VECTOR m50;
    std::array<s16, 3> m60;
    std::array<s16, 3> m68_oldPosition;
    s16 m6E;
    s16 m72_elevation;
    s8 m74;
    s16 m76;
    std::array<u16, 4> m78_stack;
    s8 m80_DialogAvatarFace;
    std::array<s8, 2> m82;
    s32 m84;
    std::array<s16, 2> m88;
    std::array<sFieldScriptEntityScriptSlot, 8> m8C_scriptSlots;
    u16 mCC_scriptPC;
    s8 mCE_currentScriptSlot;
    s8 mCF;
    std::array<s32, 3> mD0_targetPositionOffset;
    s8 mE2;
    s8 mE3;
    s16 mE4_playableCharacterId;
    s16 mE6;
    s16 mE8;
    s16 mEA_currentAnimationPlaying;
    s16 mEC_elevation;
    s32 mF0;
    std::array<s16, 3> mF4_scale3d;
    std::array<s8, 6> mFC;
    s16 m102_rotationCount;
    s16 m104_rotation;
    s16 m106_currentRotation;
    s16 m108_rotation3;
    s16 m10A;
    s8 m10C;
    s8 m10D;
    std::vector<sVec2_s16> m114_movementBoundingZone;
    std::vector<s32> m118;
    s16 m11C;
    s16 m11E;
    sFieldActorSetupParams* m120_special2dAnimation;
    std::vector<u8> m120_special2dAnimationRaw;
    s16 m124_special2dAnimationId;
    u8 m126;
    u8 m127;
    s16 m128;
    u32 m12C_flags;
    struct
    {
        u32 m0 : 28;
        u32 m28 : 2;
        u32 m30_stepVector : 2;
    }m130;

    struct
    {
        u32 m0 : 4;
        u32 m4 : 1;
        u32 m5 : 2;
        u32 m7 : 1;
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
    std::array<sFieldEntitySub4_124,8>* m34;
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
    sFieldActorSetupParams* m44;
    sFieldActorSetupParams* m48;
    sFieldActorSetupParams* m4C_specialAnimation;
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
    std::array<sFieldEntitySub4_124,8> m124;
    //size 0x164
};

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    sFieldEntitySub4* m4_pVramSpriteSheet;
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

extern s32 fieldMapNumber;
extern s32 fieldChangePrevented;
extern s32 fieldChangePrevented2;
extern s32 currentFieldId0;

void setVar(int varIndex, s16 value);

void fieldEntryPoint();
