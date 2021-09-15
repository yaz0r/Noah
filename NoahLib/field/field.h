#pragma once

#include "fieldModel.h"
#include "kernel/math.h"

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
    s8 m81_selectedMultichoiceLine;
    std::array<s8, 2> m82;
    s32 m84;
    std::array<s16, 2> m88;
    std::array<sFieldScriptEntityScriptSlot, 8> m8C_scriptSlots;
    u16 mCC_scriptPC;
    s8 mCE_currentScriptSlot;
    s8 mCF;
    sVec3 mD0_targetPositionOffset;
    s16 mE0_rotationLimit;
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
    struct sFieldActorSetupParams* m120_special2dAnimation;
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

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    struct sFieldEntitySub4* m4_pVramSpriteSheet;
    sFieldEntity2dSprite* m8_2dSprite;
    sMatrix mC_matrix;
    sMatrix m2C_matrixBackup;
    sFieldScriptEntity* m4C_scriptEntity;
    s16 m50_modelRotation[3];
    u16 m58_flags;
    u16 m5A;
    //size 0x5C
};
extern std::vector<sFieldEntity> actorArray;
extern std::array<s32, 3> currentParty;

extern s32 fieldMapNumber;
extern s32 fieldChangePrevented;
extern s32 fieldChangePrevented2;
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
extern std::array<sFieldActorSetupParams, 3> partyCharacterBuffers;
extern s16 actorCameraTracked;
extern s32 pcInitVar1;
extern std::array<int, 11> PCToActorArray;
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
extern VECTOR cameraEye2;
extern s32 cameraAt[3];
extern VECTOR cameraAt2;
extern u16 cameraInterpolationFlags;
extern s16 cameraInterpolationTargetNumSteps;
extern s32 cameraInterpolationPositionNumSteps;

extern s16 camera2Tan;

extern u8 OPX_50Param;
extern u8 OPX_52Param;
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

extern u16 padButtonForScripts;
extern std::array<s16, 4> fieldInitVar1;

extern bool g_LogOpcodes;
extern bool g_BreakOnMissingOpcode;

extern std::vector<u8> rawFieldDialogBundle;

extern MATRIX currentProjectionMatrix;

void LoadImage(struct RECT* pRect, u8* data);

void traceNextAlloc(int state);

void LogOpcode(int actorId, u16 scriptOffset, u16 opcode);
void LogMissingOpcode(int actorId, u16 scriptOffset, u16 opcode);

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

void setupSpecialAnimation(sFieldEntitySub4* param_1, struct sFieldActorSetupParams* param_2);
void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int param_2, struct sFieldActorSetupParams* pSetup, int param_4, int param_5, int param_6, int param_7);
void OP_INIT_ENTITY_SCRIPT_sub1();
void setCurrentActor2DPosition(int posX, int posZ);
void updateScriptActor3dRotation(int index);
int getAngleSin(u32 param_1);
int getAngleCos(u32 param_1);
int spriteWalkToPositionOrActor(int param_1);
void setCurrentActorRotation2(s16 param_1);
int getCurrentActorRotation();
void setCurrentActorTargetRotation(s16 param_1);
void setCurrentActorElevation(short param_1);

