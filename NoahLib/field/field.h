#pragma once

#include "fieldModel.h"
#include "kernel/math.h"
#include "kernel/graphics.h"

enum controllerButtons {
    L1 = 1 << 2, // 0x4
    R1 = 1 << 3, // 0x8
    TRIANGLE = 1 << 4, // 0x10
    INTERACT = 1 << 5, // 0x20
    CROSS = 1 << 6, // 0x40
    JUMP = 1 << 7, // 0x80
    SELECT = 1 << 8, // 0x100
    START = 1 << 11, // 0x800
    UP = 1 << 12, // 0x1000
    RIGHT = 1 << 13, // 0x2000
    DOWN = 1 << 14, // 0x4000
    LEFT = 1 << 15, // 0x8000
};

struct sGameController {
    u16 m0_buttons;
    u16 m2;
};

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
    SFP_VEC4 m0_shadowPolyVertices[4];
    POLY_FT4 m20_shadowPoly[2];
    //size 0x70
};


struct sFieldScriptEntityScriptSlot
{
    u16 m0_scriptPC;
    u8 m2_delay;
    s8 m3_scriptIndex;
    union {
        struct {
            u32 m0 : 16;
            u32 m16_status : 2;
            u32 m18 : 4;
            u32 m22 : 1;
            u32 m23_walkMode : 2;
            u32 m25 : 1;
            u32 m26 : 1;
            u32 m27 : 2;
            u32 m28X : 1;
        };
        u32 raw;
    } m4_flags; // bit 18, size 4
    // size 8
};

struct sFieldScriptEntity_flags0
{
    union {
        struct {
            u32 m0_updateScriptDisabled : 1;
            u32 m1_unk : 6;
            u32 m7 : 1;
            u32 m8 : 3;
            u32 mx800_isJumping : 1; // 0x800
            u32 m12_unk : 1;
            u32 m13_unk : 1;
            u32 m14_isPlayerControlled : 1;
        };
        u32 m_rawFlags = 0;
    };
};


struct sFieldScriptEntity_flags4
{
    union {
        struct {
            u32 m0_unk : 11;
            u32 m_x800 : 1;
        };
        u32 m_rawFlags = 0;
    };
};

struct sFieldScriptEntity
{
    sFieldScriptEntity_flags0 m0_fieldScriptFlags;
    sFieldScriptEntity_flags4 m4_flags;
    std::array<s16, 4> m8_currentWalkMeshTriangle;
    u16 m10_walkmeshId;
    s32 m14_currentTriangleFlag;
    SFP_VEC3 m18_boundingVolume;
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
    s8 m80_dialogPortrait;
    s8 m81_selectedMultichoiceLine;
    std::array<s8, 2> m82;
    s32 m84;
    std::array<s16, 2> m88;
    std::array<sFieldScriptEntityScriptSlot, 8> m8C_scriptSlots;
    u16 mCC_scriptPC;
    s8 mCE_currentScriptSlot;
    s8 mCF_scriptSlotWaitedOn;
    FP_VEC3 mD0_targetPositionOffset;
    s16 mE0_rotationLimit;
    s8 mE2;
    s8 mE3;
    s16 mE4_playableCharacterId;
    s16 mE6;
    s16 mE8_currentAnimationId;
    s16 mEA_forcedAnimation;
    s16 mEC_elevation;
    s32 mF0;
    std::array<s16, 3> mF4_scale3d;
    std::array<u8, 3> mFC;
    std::array<u8, 3> mFF;
    s16 m102_numSteps;
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

extern std::array<s32, 3> partyToFollowStructMapping;

extern bool g_executeScripts;
extern bool g_executeUpdateScripts;

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
extern int fieldExecuteVar1;

extern int asyncPartyCharacterLoadingIndex;
extern std::vector<u8> asyncPartyCharacterLoadingBuffer;
extern int asyncPartyCharacterLoadingCharacterIndex;

extern std::array<sSpriteActorAnimationBundle, 3> partyCharacterBuffers;
extern s16 actorCameraTracked;
extern s32 pcInitVar1;
//extern std::array<int, 11> PCToActorArray;
extern s32 playerControlledActor;
extern const std::array<s8, 12> characterMappingTable;
extern s16 pcInitVar2;
extern std::array<int, 3> unkPartyTable;

extern bool isBackBufferRamCopyEnabled;
extern s32 cameraDeltaTan;

extern s32 opA0_var0;
extern s32 opA0_var1;
extern FP_VEC3 desiredCameraPosition;
extern FP_VEC3 cameraTargetOverride;
extern FP_VEC3 desiredCameraTarget;
extern FP_VEC3 cameraPositionOverride;
extern FP_VEC3 cameraInterpolationTargetStep;
extern FP_VEC3 cameraInterpolationTargetStartPosition;
extern FP_VEC3 cameraInterpolationPositionStep;
extern FP_VEC3 cameraInterpolationStartPosition;
extern FP_VEC4 cameraEye;
extern FP_VEC4 cameraEye2;
extern FP_VEC4 cameraAt;
extern FP_VEC4 cameraAt2;
extern u16 cameraInterpolationFlags;
extern s16 cameraInterpolationTargetNumSteps;
extern s32 cameraInterpolationPositionNumSteps;

extern s16 camera2Tan;

extern u8 menuDisabled;
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
extern s8 pauseDisabled;
extern u8 fieldBackgroundClearColor[3];

extern u16 newPadButtonForDialogs;
extern u16 newPadButtonForDialogs2;
extern u16 newPadButtonForField;
extern u16 newPadButtonForField2;

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
extern const std::array<u16, 8> actorDirectionTable2;
extern const std::array<u16, 8> actorDirectionTable3;

extern s32 load2dAnimVar;
extern s32 loadCompleted;

extern std::array<sGameController, 2> padButtonForScripts;
extern std::array<s16, 4> cameraLimits;

extern bool g_LogOpcodes;
extern bool g_BreakOnMissingOpcode;

extern std::vector<u8> rawFieldDialogBundle;

extern u8 DollyStop;
extern u8 DollySet;

extern s16 playerCanRun;

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

extern u16 padButtonForField;

extern int updateAllEntitiesSub2Var0;

extern s16 cameraDollyVar0;
extern s32 cameraDollyVar1;

extern int onlyUpdateDirector;

struct sFieldRenderContext
{
	DRAWENV m0_drawEnv;
	DRAWENV m5C_backgroundRect;
	DISPENV mB8_displayEnv;
	OTTable mCC_OT;
	OTTable m40D0_secondaryOT;
	std::array<sTag, 8> m80D4_uiOT;
};

extern std::array<sFieldRenderContext, 2> fieldRenderContext;
extern sFieldRenderContext* pCurrentFieldRenderingContext;

extern s16 OP_A4Var0;
extern int OP_A4Var1;
extern int OP_A4Var2;
extern s16 OP_B6SubVar0;
extern s16 OP_B6Var1;
extern s16 OP_B6Var2;

extern struct sCustomRenderableEntityHeader* spriteCallback2Var2;

extern int overrideTPageMode;
extern s32 overrideTPageValue;

extern int initModel2Sub0Var1;
extern s32 primD_initSub0Sub1Var1;

extern MATRIX currentRenderingMatrix;
extern OTTable* characterRenderingOT;
extern int shapeTransfertDoubleBufferIndex;

extern s32 bootModeReady;
extern s32 newBootMode;

extern s32 gDepthDivider;

extern u8 kernelAndFieldStatesSynced;

extern s32 fieldTransitionMode;
extern s32 fieldTransitionFadeInLength;

// Encounter timers and data
extern s32 encounterTimer;
extern s32 fieldTransitionCompleted;
extern s32 encounterDataCountdown;
extern int encounterCount;
extern std::array<s32, 32> encounterTriggerTime;
extern std::array<u8, 16> encounterProbabilityWeight;
extern bool bBattleSequenceInitialized;

// Encounter debug
extern bool debugEncounterTriggerDisabled;
extern int debugForcedEncounter;

extern u32 playTimeInVsync;

extern u32 fullScreenTransitionEffectRenderParam;
extern u32 fullScreenTransitionEffectRenderMode;

extern s16 updateCharacterVar1;

extern int linkOTIndex;
extern int runningOnDTL;

void setCurrentRenderingMatrix(MATRIX* pMatrix);
void renderSpriteActor(sSpriteActor* pSpriteSheet, sTag* pTag);

void allocateShapeTransfert(int param_1);

void resetInputs();
void checkSoftReboot();

void clearShapeTransfertTableEntry(int param_1);
void uploadCharacterSprites();
void shapeTransfert();

void setFieldDrawEnvClip(short x, short y, short w, short h);

void syncKernelAndFieldStates();

void initFontPalettes(short param_1, short param_2);
void initFontSystem();

int loadInputFromVSyncBuffer();

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
int getCharacter(int param_1);
int findCharacterInParty(int param_1);

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

void updateEntityEventCode3Sub1(struct sSpriteActor* param_1, ushort rotation, sFieldEntity* param_3);
void setVisualAnimation(struct sSpriteActor* param_1, int param_2, sFieldEntity* param_3);

void waitForReadFinished(void);
void startLoadingPlayableCharacters();
void finalizeLoadPlayableCharacters();
int getGearForCharacter(int param_1);

void ClearCacheAfterOverlayLoad();

int fieldModelRelocation(std::vector<u8>::iterator pModelData);
void initModel1(sModelBlock& pModelBlock, std::vector<sTag*>& outputBuffer1, std::vector<sTag*>& outputBuffer2);
void initModel2(sModelBlock* pModelBlock, std::vector<sTag*>& outputBuffer, int param_3);

bool submitModelForRendering(sModelBlock* param_1, std::vector<sTag*>& param_2, OTTable& OT, int renderMode);

void doPCCollisionCheckAfterLoading();
void runInitScriptForNewlyLoadedPC(uint param_1);

extern std::array<sGameController, 2> newPadButtonForScripts;

extern SFP_VEC4 computeProjectionMatrixAngles;