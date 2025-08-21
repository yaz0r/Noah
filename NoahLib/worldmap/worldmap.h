#pragma once

#include "kernel/filesystem.h"

struct sWorldmapStateEntry {
    s16 m0_state;
    s16 m2;
    s16 m4;
    s16 m6;
    s32(*m18_init)(s32 index);
    s32(*m1C_update)(s32 index);
    s16 m20;
    s16 m22;
    s16 m24;
    VECTOR m28_position;
    VECTOR m38_step;
    s16 m48;
    s16 m4A;
    struct sSpriteActor* m4C;
    s32 m50;
    s32 m54;
    s32 m58; // for gears this is how far back you go in the follow data
    s32 m5C;
    s32 m60;    
    const std::array<s16, 4>* m64_array;// those two are sometimes aliased to u32
    s32 m64;
    const std::array<s16, 4>* m68_array;
    s32 m68;
    s32 m6C;
    s32 m70;
    s32 m74; // for yggdrasil, this is the number of gears onboard
    s32 m78;
    s32 m7C;
};

struct sWorldmapState {
    std::array<sWorldmapStateEntry, 64> m0;
};

bool changeWorldmapEntityState(int param_1, short param_2);

extern sWorldmapState* gWorldmapState;

#include "psx/libgpu.h"

struct sWorldmapRenderingStruct {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    std::vector<sTag> m70_OT;
    std::vector<POLY_FT3> m74_polyFT3;
};

extern sWorldmapRenderingStruct* pCurrentWorldmapRenderingStruct;


extern s8 isWorldMapOverlayLoaded;
extern s32 worldmapOddOrEven;

extern VECTOR worldmapPosition;
extern SVECTOR worldmapRotation;
extern VECTOR worldmapRadarPosition;
extern s32 worldmapGeometryOffsetY;
extern s32 worldMapGearMode;
extern u32 worldmapGamestate1824;
extern s16 worldmapVar_8009d52c;

extern MATRIX worldmapMainMatrix2;
extern s32 worldmapMatrixMode;

struct sWorldmapCameraState {
    SVECTOR m0;
    SVECTOR m8;
    VECTOR m10;
};

extern sWorldmapCameraState worldmapCameraVector;

extern VECTOR worldmapRadarPosition3;
extern VECTOR worldmapGridInputPosition;

extern s32 worldmapSizeX;
extern s32 worldmapSizeY;
extern s32 continueWorldmapLoop;
extern s32 exitWorldMapMode;

extern u16 worldmapInput1_0;
extern u16 worldmapInput1_1;
extern u16 worldmapInput2_0;
extern u16 worldmapInput2_1;
extern u16 worldmapInput3_0;
extern u16 worldmapInput3_1;

extern s16 worldmapExitVar2;

extern s32 worldmapFile9;
extern s32 worldmapFile10;

extern MATRIX worldMapIdentityMatrix;
extern SVECTOR worldmapGridPosition;

extern s32 worldmapNumModels;

struct sWorldmapModel {
    u16 m0_hidden;
    u16 m2_modelBlockIndex;
    s16 m4_flags;
    VECTOR m8;
    SVECTOR m18_rotation;
    MATRIX m20_rotationMatrix;
    struct sModelBlock* m40_modelBlock;
    std::vector<u8>::const_iterator m44_collisionMesh;
    std::array<std::vector<sTag*>, 2> m48;
    sWorldmapModel* m50_parentModel;
    // size 0x54;
};

extern s32 worldmapModelVar0;
extern s32 worldmapModelVar1;

extern std::vector<sWorldmapModel> worldmapModels;

extern std::array<u16, 0x40> USHORT_ARRAY_8009ccb4;
extern std::array<u16, 8> USHORT_ARRAY_8009cd54;

extern std::array<struct sSpriteActorAnimationBundle, 3 > worldmapPartySprites;
extern std::array<sSpriteActorAnimationBundle, 3 > worldmapPartyGearSprites;

extern s32 worldmapNumActivePartyMembers;
extern u8 adjustLocationAfterCollisionVar0;
extern u8 targetVehicleEntityIndex;

extern s32 currentWorldmapMode;

s32 worldmapGetAltitudeFor2dPoint(s32 x, s32 y);
void worldmapEntryPoint(void);
void getGamestate182C(VECTOR* param_1);
void setGamestate182C(VECTOR* param_1);