#pragma once

#include "fieldModel.h"
#include "kernel/math.h"
#include "kernel/graphics.h"

struct sFieldEntitySub0
{
    std::vector<sModelBlock>::iterator m4_pModelBlock;
    std::array<std::vector<sTag*>,2> m8;
    s16 m12_renderMode;
    void* m14;
    SFP_VEC4 m18;
    s16 m20;
    //size 0x24
};

struct sFieldEntity2dSprite
{
    SFP_VEC4 m0_screenVertices[4];
    POLY_FT4 m20_Poly[2];
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

struct sFieldScriptEntity_flags0
{
    union {
        struct {
            u32 m0 : 1;
            u32 m1_unk : 6;
            u32 m7 : 1;
            u32 m8 : 3;
            u32 m11 : 1;
        };
        u32 m_rawFlags = 0;
    };
};

struct sFieldScriptEntity
{
    sFieldScriptEntity_flags0 m0_fieldScriptFlags;
    u32 m4_flags;
    std::array<s16, 4> m8_currentWalkMeshTriangle;
    u16 m10_walkmeshId;
    s32 m14_currentTriangleFlag;
    s16 m18;
    s16 m1A;
    s16 m1C;
    s16 m1E_collisionRadius;
    FP_VEC4 m20_position;
    FP_VEC4 m30_stepVector;
    FP_VEC4 m40;
    FP_VEC4 m50_surfaceNormal;
    std::array<s16, 3> m60;
    std::array<s16, 3> m68_oldPosition;
    s16 m6E;
    s16 m70_rotationForRendering;
    s16 m72_elevation;
    s8 m74;
    s8 m75;
    s16 m76;
    std::array<u16, 4> m78_stack;
    s8 m80_DialogAvatarFace;
    s8 m81_selectedMultichoiceLine;
    std::array<s8, 2> m82;
    s32 m84;
    std::array<s16, 2> m88;
    std::array<sFieldScriptEntityScriptSlot, 8> m8C_scriptSlots;
    u16 mCC_scriptPC;
    s8 mCE_currentScriptSlot;
    s8 mCF;
    FP_VEC3 mD0_targetPositionOffset;
    s16 mE0_rotationLimit;
    s8 mE2;
    s8 mE3;
    s16 mE4_playableCharacterId;
    s16 mE6;
    s16 mE8_currentWalkSpeed;
    s16 mEA_forcedAnimation;
    s16 mEC_elevation;
    s32 mF0;
    std::array<s16, 3> mF4_scale3d;
    std::array<s8, 6> mFC;
    s16 m102_randomSeed;
    s16 m104_rotation;
    s16 m106_currentRotation;
    s16 m108_rotation3;
    s16 m10A;
    s8 m10C;
    s8 m10D;
    std::vector<s16> m110_modelRotation2;
    std::vector<sVec2_s16> m114_movementBoundingZone;
    std::vector<s32> m118;
    s16 m11C;
    s16 m11E;
    struct sSpriteActorAnimationBundle* m120_special2dAnimation;
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
        u32 m7_isModelRotation2Allocated : 1;
    }m134;

    // size 0x138
};

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    struct sSpriteActor* m4_pVramSpriteSheet;
    sFieldEntity2dSprite* m8_2dSprite;
    sMatrix mC_matrix;
    sMatrix m2C_matrixBackup;
    sFieldScriptEntity* m4C_scriptEntity;
    SFP_VEC4 m50_modelRotation;
    u16 m58_flags;
    u16 m5A;
    //size 0x5C
};
extern std::vector<sFieldEntity> actorArray;
extern std::array<s32, 3> currentParty;

extern s32 fieldMapNumber;
extern s32 fieldChangePrevented;
extern s32 fieldMusicLoadPending;
extern s32 currentFieldId0;

extern std::array<s16, 1024> fieldVars;
extern std::vector<u8> rawFieldScriptData;

extern int breakCurrentScript;
extern int fieldExectuteMaxCycles;
extern int fieldDebugDisable;
extern int renderCharsDisabled;

extern sFieldEntity* pCurrentFieldEntity;
extern int fieldScriptInitVar0;
extern int currentScriptFinished;

extern int asyncLoadingVar1;
extern int asyncLoadingVar2;
extern int fieldExecuteVar1;

extern const std::array<u16, 8> actorDirectionTable3;
extern std::array<sSpriteActorAnimationBundle, 3> partyCharacterBuffers;
extern s16 actorCameraTracked;
extern s32 pcInitVar1;
//extern std::array<int, 11> PCToActorArray;
extern s32 playerControlledActor;
extern const std::array<s8, 12> characterMappingTable;
extern s16 pcInitVar2;
extern std::array<int, 3> unkPartyTable;

extern s32 opA0_var0;
extern s32 opA0_var1;
extern s32 desiredCameraPosition[3];
extern s32 cameraTargetOverride[3];
extern s32 desiredCameraTarget[3];
extern s32 cameraPositionOverride[3];
extern s32 cameraInterpolationTargetStep[3];
extern s32 cameraInterpolationTargetStartPosition[3];
extern s32 cameraInterpolationPositionStep[3];
extern s32 cameraInterpolationStartPosition[3];
extern s32 cameraEye[3];
extern FP_VEC4 cameraEye2;
extern s32 cameraAt[3];
extern FP_VEC4 cameraAt2;
extern u16 cameraInterpolationFlags;
extern s16 cameraInterpolationTargetNumSteps;
extern s32 cameraInterpolationPositionNumSteps;

extern s16 camera2Tan;

extern u8 OPX_50Param;
extern u8 compassDisabled;
extern u16 OPX_80Params[8];
extern s32 OPX_81Params[3];
extern s8 OPX_82Param0[4];
extern s8 OPX_82Param1[4];
extern s8 OPX_82Param2[4];
extern s16 OPX_82Param3[3];
extern s16 OPX_82Param4;
extern s32 OPX_86Param;
extern s32 OPX8E_param0;
extern s32 OPX8E_param1;
extern s8 OPX_E0Param;
extern u8 OPE7_param[3];

extern int playMusicAuthorized;
extern int musicVar1;
extern int musicVar2;
extern int currentlyPlayingMusic;

extern int g_frameOddOrEven;

extern int fieldDrawEnvsInitialized;
extern s32 fieldExecuteVar2;
extern s32 fieldExecuteVar3;
extern s16 fieldRandomBattleVar;

extern const std::array<u16, 8> actorDirectionTable;
extern const std::array<u16, 8> actorDirectionTable3;

extern s32 load2dAnimVar;
extern s32 loadCompleted;

extern sVec2_s16 padButtonForScripts[2];
extern std::array<s16, 4> fieldInitVar1;

extern bool g_LogOpcodes;
extern bool g_BreakOnMissingOpcode;

extern std::vector<u8> rawFieldDialogBundle;

extern u8 DollyStop;
extern u8 DollySet;


struct sFieldTrigger
{
	std::array<s16, 3> m0;
    std::array<s16, 3> m6;
    std::array<s16, 3> mC;
    std::array<s16, 3> m12;
};
extern std::vector<sFieldTrigger> fieldTriggerData;

extern MATRIX currentProjectionMatrix;

extern std::array<std::vector<u8>, 3> partyCharacterBuffersRaw;
extern std::array<int, 3> asyncPartyCharacterLoadingTable;

extern SFP_VEC4 cameraProjectionAngles;
extern SFP_VEC4 cameraRotation;
extern FP_VEC4 cameraUp;
extern s16 cameraTan;

extern MATRIX worldScaleMatrix;
extern MATRIX cameraMatrix;
extern s32 sceneSCRZ;

extern int updateAllEntitiesSub2Var0;

extern short screenDistortionConfigured;
extern short screenDistortionAvailable;

struct sFieldRenderContext
{
	DRAWENV m0_drawEnv;
	DRAWENV m5C_backgroundRect;
	DISPENV mB8_displayEnv;
	std::array<sTag, 4096> mCC_OT;
	std::array<sTag, 4096> m40D0_secondaryOT;
	std::array<sTag, 8> m80D4_uiOT;
};

extern std::array<sFieldRenderContext, 2> fieldRenderContext;
extern sFieldRenderContext* pCurrentFieldRenderingContext;

void flagAllocation(std::vector<u8>&);
void unflagAllocation(std::vector<u8>&);

uint stepInterpolateDirection(int currentDirection, int targetDirection, int step);

void emptyPartySlot(int param_1);
void copyPartySlotFromNext(uint param_1);

void vsyncCallback(void);

void traceNextAlloc(int state);

void LogOpcode(int actorId, u16 scriptOffset, u16 opcode);
void LogMissingOpcode(int actorId, u16 scriptOffset, u16 opcode);

void initFieldScriptEntityValues(int index);

void playMusic(int musicId, int);
void setupRGBCalcMode(int, int, int, int, int, int);
u16 getScriptEntryPoint(int entityId, int scriptIndex);
int isScriptAlreadyRunning(sFieldScriptEntity* pEntity, int scriptIndex);
void clearMusic();
void clearMusic2();
void OPX_13Sub(int);
void updateGearState(int param_1);
int getCurrentDiscNumber();
int isLoadCompleted();
void SetGeomScreen(s32);
void SetGeomOffset(s32, s32);
int getCharacter(int param_1);
int findCharacterInParty(int param_1);

int isAsyncLoadingProhibited();
int findFreePartySlot(int param_1, int* param_2);
void setVar(int varIndex, s16 value);
void startPartyCharacterASyncLoading(int partyCharacter, int partySlot);
void fieldEntryPoint();

void setupSpecialAnimation(sSpriteActor* param_1, struct sSpriteActorAnimationBundle* param_2);
void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int param_2, struct sSpriteActorAnimationBundle* pSetup, int param_4, int param_5, int param_6, int param_7);
void OP_INIT_ENTITY_SCRIPT_sub1();
void setCurrentActor2DPosition(int posX, int posZ);
void updateScriptActor3dRotation(int index);
int getAngleSin(u32 param_1);
int getAngleCos(u32 param_1);
int spriteWalkToPositionOrActor(int param_1);
void setCurrentActorRotation2(s16 param_1);
int getCurrentActorRotationIn8Cardinal();
void setCurrentActorTargetRotation(s16 param_1);
void setCurrentActorElevation(short param_1);

void resetRGBFaderToBlack(int index);
void setupRGBFaderSlot0_fadeIn(int);
void setupRGBCalcSlot0_fadeToBlack(int);

void setPolyUV(POLY_FT4* poly, ushort u0, ushort v0, ushort u1, ushort v1, ushort u2, ushort v2, ushort u3, ushort v3);
