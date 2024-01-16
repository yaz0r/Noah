#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "walkMesh.h"
#include "fieldModel.h"
#include "kernel/isoManager.h"
#include "fieldDebugger/fieldInspector.h"
#include "fieldScriptSupport.h"
#include "opcodeTables.h"
#include "fieldGraphicObject.h"
#include "sprite/spriteSetup.h"
#include "kernel/trigo.h"
#include "dialogWindows.h"
#include "kernel/gte.h"
#include "compass.h"
#include "kernel/TIM.h"
#include "screenDistortion.h"
#include "mecha/mechaOverlay.h"
#include "menus/menuHandler.h"
#include "kernel/gte.h"
#include "field/particles/particles.h"
#include "kernel/memory.h"
#include "kernel/gameMode.h"
#include "worldmap/worldmap.h"
#include "battle/battleConfig.h"

#include "SDL_gamecontroller.h"
#include "SDL_keyboard.h"

MATRIX computeProjectionMatrixTempMatrix2;

u32 playTimeInVsync = 0;
s32 bootModeReady = 0;
s32 newBootMode = 0;
s32 menuFadeState = 0;
u8 kernelAndFieldStatesSynced = 1;
s16 updateCharacterVar1 = 0;

bool g_executeScripts = true;
bool g_executeUpdateScripts = true;

struct sFieldVramMapping
{
    s16 m0_vramX;
    s16 m2_vramY;
    s16 m4;
    s16 m6;
};
std::array<sFieldVramMapping, 32> fieldVramMapping;

std::vector<u8> rawFieldBundleForDebug;
std::vector<u8> rawFieldBundle;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldDialogBundle;
std::vector<sFieldTrigger> fieldTriggerData;

// this is malloc/free in original code, I keep it around for debugger
std::vector<u8> rawFieldImageBundle;
std::vector<u8> rawFieldImageBundle2;


std::array<s16, 4> cameraLimits;

s32 totalObjects;

s32 fieldMapNumber = -1;
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;

s32 totalActors;

std::array<s32, 3> currentParty;

std::vector<sFieldEntity> actorArray;

std::vector<sSpriteActorAnimationBundle> fieldActorSetupParams;
std::vector<u8> rawFieldActorSetupParams;

s16 numWalkMesh = 0;
std::array<s32, 5> walkMeshNumTrianglePerBlock;
std::vector<u32>* walkMeshVar1;
std::array<std::vector<SFP_VEC4>*, 5> walkMeshVertices;
std::array<std::vector<sWalkMesh::sTriangleData>*, 5> walkMeshTriangle;
s32 walkMeshVar4 = 0;

s16 isFogSetup = 0;
s16 cameraTan = 0;

s16 deltaMoveVar0 = 0;

s16 entityMoveSpeedFactor = 0;

int g_frameOddOrEven = 0;

std::array<sLoadableDataRaw, 3> partyCharacterBuffersRaw;
std::array<sLoadableDataRaw, 3> partyCharacterBuffersCompressed;
std::array<sSpriteActorAnimationBundle, 3> partyCharacterBuffers;
s16 actorCameraTracked = 0;
s32 pcInitVar1 = 0;
s32 playerControlledActor = 0;
//std::array<int, 11> PCToActorArray;

s32 fieldPolyCount;
s32 fieldPolyCount2;
s32 worldScaleFactor;
MATRIX worldScaleMatrix;
MATRIX cameraMatrix;
MATRIX cameraMatrix2;
s32 objectClippingMask = 1;
FP_VEC4 fieldObjectRenderingVar1 = { 0,0,0 };
SFP_VEC4 fieldObjectRenderingVar2 = { 0,0,0 };
s32 sceneSCRZ = 0;
int fieldUseGroundOT = 0;

u8 DollyStop = 0;
u8 DollySet = 0;

s16 OP_A4Var0 = 0;
int OP_A4Var1 = 0;
int OP_A4Var2 = 0;
s16 OP_B6SubVar0 = 0;
s16 OP_B6Var1 = 0;
s16 OP_B6Var2 = 0;

u32 fullScreenTransitionEffectRenderMode = 0;
u32 fullScreenTransitionEffectRenderParam = 0;

const std::array<s8, 12> characterMappingTable = {
    0,1,2,3,4,5,6,7,8,2,6,0
};

s16 pcInitVar2 = 0;
std::array<int, 3> unkPartyTable;

u8 fieldBackgroundClearColor[3];

SFP_VEC4 renderModelRotationAngles;
MATRIX renderModelRotationMatrix;
s32 cameraDeltaTan;

MATRIX currentProjectionMatrix;

s32 updateCameraInterpolationVar0 = 0;
s32 updateCameraInterpolationVar1 = 0;



int fieldModelRelocation(std::vector<u8>::iterator pModelData)
{
    int count = READ_LE_U32(pModelData);
    MissingCode();
    return count;
}

s32 fieldChangePrevented = -1;
s32 fieldMusicLoadPending = -1;
s32 fieldTransitionMode = 0;
s32 fieldTransitionFadeInLength = 0;
s32 encounterTimer = 0;
s32 fieldTransitionCompleted = 0;
int encounterDataCountdown = 0;
int encounterCount = 0;
std::array<s32, 32> encounterTriggerTime;
std::array<u8, 16> encounterProbabilityWeight;
bool bBattleSequenceInitialized = false;

bool debugEncounterTriggerDisabled = false;
int debugForcedEncounter = -1;

u16 inputAllowedMask = 0xFFFF;
u16 padButtonForField;
u16 padButtonForField2;

std::array<sGameController, 2> newPadButtonForScripts;
u16 inputAllowedMask2;
u16 newPadButtonForDialogs;
u16 newPadButtonForDialogs2;
u16 newPadButtonForField;
u16 newPadButtonForField2;

struct sInputReplayEntry
{
    u16 m0_newPadButtonForScripts0[16];
    u16 m20_newPadButtonForScripts1[16];
    u16 m40_newPadButtonForDialogs[16];
    u16 m60_newPadButtonForDialogs2[16];
    u16 m80_newPadButtonForField[16];
    u16 mA0_newPadButtonForField2[16];
} inputReplayBuffer;

int inputReplayPosition = 0;
int inputReplayPosition2 = 0;
int inputOverflowed = 0;
int inputReplayOffset = 0;

void resetInputs()
{
    MissingCode();

    inputOverflowed = 0;
    inputReplayPosition = 0;
    inputReplayPosition2 = 0;
    inputReplayOffset = 0;
    newPadButtonForDialogs = 0;
    newPadButtonForDialogs2 = 0;
    newPadButtonForField = 0;
    newPadButtonForField2 = 0;

    MissingCode();

    newPadButtonForScripts[0].m0_buttons = 0;
    newPadButtonForScripts[1].m0_buttons = 0;

    MissingCode();
}

std::array<sFieldRenderContext, 2> fieldRenderContext;
sFieldRenderContext* pCurrentFieldRenderingContext = nullptr;

SFP_VEC4 cameraProjectionAngles;
SFP_VEC4 cameraRotation;
FP_VEC4 cameraUp = { 0,0,0,0 };

s16 cameraDollyVar0 = 0;
s32 cameraDollyVar1 = 0;

void resetCameraData()
{
    op99Var5 = 8;
    op99Var6 = 8;
    cameraDollyVar1 = 0x400000;
    opA0_var1 = 0x8000000;

    MissingCode();

    op99Var7 = 0;
    cameraTan = 0;
    camera2Tan = 0;
    DollySet = 0;
    DollyStop = 0;
    cameraDollyVar0 = 0;

    MissingCode();

    cameraEye[0] = 0;
    cameraEye[1] = 0;
    cameraEye[2] = 0;
    cameraAt[0] = 0;
    cameraAt[1] = 0;
    cameraAt[2] = 0;
    cameraUp.vx = 0;
    cameraUp.vy = 0x10000000;
    cameraUp.vz = 0;
    cameraEye2.vx = 0;
    cameraEye2.vy = 0;
    cameraEye2.vz = 0;
    cameraAt2.vx = 0;
    cameraAt2.vy = 0;
    cameraAt2.vz = 0;

    MissingCode();

    cameraInterpolationMode = 0;
    cameraInterpolationFlags = 0;
    cameraInterpolationTargetNumSteps = 0;
    cameraInterpolationPositionNumSteps = 0;
    sceneSCRZ = 0x200;
    sceneDIP = 0x1e;
    sceneScale = 0x1000;

    MissingCode();
}

int linkOTIndex;
s8 EntityMoveCheck1Var1 = 0;
char fieldObjectRenderingVar3 = 0;
char noUpdatesToPartyMemebers = 0;
int onlyUpdateDirector = 0;
s16 playerCanRun = 1;
s32 updateEntityEventCode3Var0 = 0;
s32 updateEntityEventCode3Var1 = 0;
s32 updateEntityEventCode3Var2 = 0;
s16 updateEntityEventCode4Var0 = 0;
s16 jumpAnimationId = 0;
int cameraCollisionState = 0;
enum class eCameraTrackingMode : s16
{
    e0_followPlayer = 0,
};
eCameraTrackingMode cameraTrackingMode = eCameraTrackingMode::e0_followPlayer;
s32 EntityMoveCheck0Var0 = 0;

void resetFieldDefault()
{
    MissingCode();

    setFieldDrawEnvClip(0, 0, 0x140, 0xe0);
    resetInputs();
    inputAllowedMask = 0xffff;
    padButtonForScripts[0].m0_buttons = 0;
    padButtonForScripts[1].m0_buttons = 0;
    padButtonForDialogs = 0;
    padButtonForDialogs2 = 0;
    padButtonForField = 0;
    padButtonForField2 = 0;

    MissingCode();
    jumpAnimationId = 3;
    numMaxMechaOverlayEntries = 0x40;

    MissingCode();

    NumMechas = 0;

    MissingCode();

    compassDisabled = 0;
    menuDisabled = 0;
    MissingCode();
    pcInitVar1 = 0;
    updateCharacterVar0 = 0;
    fieldObjectRenderingVar2.vy = 0;
    fieldObjectRenderingVar2.vz = 0;
    fieldObjectRenderingVar2.vx = 0;
    screenDistortionRunning = 0;
    MissingCode();
    fieldBackgroundClearColor[2] = 0;
    fieldBackgroundClearColor[1] = 0;
    fieldBackgroundClearColor[0] = 0;
    noUpdatesToPartyMemebers = 0;
    EntityMoveCheck1Var1 = 0;
    fieldObjectRenderingVar3 = 0;
    onlyUpdateDirector = 0;
    fieldUseGroundOT = 0;
    load2dAnimVar = 0;
    screenDistrotionAllocated = 0;
    updateEntityEventCode3Var1 = 0;
    EntityMoveCheck0Var0 = 0;
    MissingCode();
    loadCompleted = 0;
    playerCanRun = 0;
    MissingCode();
    cameraCollisionState = 0;
    newBootMode = 0;
    //partyPortraitPointers[2] = (undefined*)0x0;
    menuFadeState = 0;
    MissingCode();
    kernelAndFieldStatesSynced = 0;
    MissingCode();
    cameraTrackingMode = eCameraTrackingMode::e0_followPlayer;
    MissingCode();
    fullScreenTransitionEffectRenderMode = 0;
    fullScreenTransitionEffectRenderParam = 0;
    fieldTransitionFadeInLength = 0x20;
    fieldTransitionMode = 2;
    MissingCode();
    updateEntityEventCode3Var1 = 0;
    MissingCode();
    currentParticleCreatorId = 0;
    updateCharacterVar1 = 0;
    actorCameraTracked = 0;
    MissingCode();
    disableParticles = 0;
    menuIdToOpen = 0xff;
    MissingCode();

    op99Var4 = 0x1000;
    linkOTIndex = 0x720;
    MissingCode();
    fieldObjectRenderingVar2.pad = 0x80;
    asyncLoadingVar1 = 0xFF;
    entityMoveSpeedFactor = 0x1000;
    renderModelRotationAngles.vx = 0;
    renderModelRotationAngles.vy = 0;
    renderModelRotationAngles.vz = 0;
    MissingCode();

    fieldChangePrevented = -1;
    op9DVar1 = 2;
    inputAllowedMask2 = 0xFFFF;

    MissingCode();
    g_frameOddOrEven = 0;
    MissingCode();
    updateEntityEventCode3Var2 = 0;
    MissingCode();
    playerControlledActor = 0;
    deltaMoveVar0 = 0;
    windowOpenBF = 0;
    fieldRandomBattleVar = 0;

    MissingCode();

    dialogWindowOpenAnimationNumFrames = 8;

    MissingCode();


    for (int i = 0; i < 0x200; i++) {
        fieldVars[i] = pKernelGameState->m1930_fieldVarsBackup[i];
        fieldVars[i + 0x200] = 0;
    }

    SetGeomScreen(0x200);
    setIdentityMatrix(&cameraMatrix);
    setIdentityMatrix(&cameraMatrix2);
    setIdentityMatrix(&currentProjectionMatrix);
    setIdentityMatrix(&computeProjectionMatrixTempMatrix2);
    cameraProjectionAngles.vx = 0;
    cameraProjectionAngles.vy = 0;
    cameraProjectionAngles.vz = 0;
    cameraRotation.vx = 0;
    cameraRotation.vy = 0;
    cameraRotation.vz = 0;
    worldScaleFactor = 0x3000;

    createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
    pCurrentFieldRenderingContext = &fieldRenderContext[0];
    resetCameraData();
    resetPortraitLoadingStatus();

    MissingCode();
    resetParticleEffectsTable();
    MissingCode();
}

bool g_LogOpcodes = false;
bool g_BreakOnMissingOpcode = false;

void LogOpcode(int actorId, u16 scriptOffset, u16 opcode)
{
    if (g_LogOpcodes)
    {
        printf("Actor0x%02X: offset 0x%04X OP_%02X\n", actorId, scriptOffset, opcode);
    }
}

void LogMissingOpcode(int actorId, u16 scriptOffset, u16 opcode)
{
    if (g_BreakOnMissingOpcode)
    {
#ifdef WIN32
        __debugbreak();
#endif
    }

    Noah_Log("Missing opcode 0x%04X (Actor %d, offset %d)", opcode, actorId, scriptOffset);
}

void uploadFieldImages(std::vector<u8>::iterator pImageData)
{
    MissingCode();
}

RECT* currentNpcSpriteUploadRect = nullptr;
std::vector<u8>::iterator currentNpcSpriteUploadDataPtr;

void transfertNpcSpriteSheetElement()
{
    LoadImage(currentNpcSpriteUploadRect, &currentNpcSpriteUploadDataPtr[0]);
}

void uploadNpcSpriteSheet(std::vector<u8>::iterator pImageData, int x, int y)
{
    int count = READ_LE_U32(pImageData);

    int xOffset = 0;

    for (int i = 0; i < count; i++)
    {
        int offset = READ_LE_U32(pImageData + 4 + i * 4);
        std::vector<u8>::iterator data = pImageData + offset;

        RECT rect;
        rect.x = x + xOffset;
        rect.y = y;
        rect.w = READ_LE_U16(data);
        rect.h = READ_LE_U16(data + 2);

        xOffset += 0x40;

        currentNpcSpriteUploadDataPtr = data + 4;
        currentNpcSpriteUploadRect = &rect;

        transfertNpcSpriteSheetElement();
    }

    MissingCode();
}

void setupField3d(std::vector<u8>::iterator input)
{
    lookAtDivided(&cameraMatrix, &cameraEye, &cameraAt, &cameraUp);
    createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
    MulRotationMatrix(&cameraMatrix, &currentProjectionMatrix);

    MissingCode();
}


void getTriangleNormalAndAdjustY(const SFP_VEC4& vec0, const SFP_VEC4& vec1, const SFP_VEC4& vec2, SFP_VEC4* outputPosition, FP_VEC4* pOutputTriangleNormal)
{
    FP_VEC4 normalizedVec0To1;
    FP_VEC4 normalizedVec0To2;

    {
        FP_VEC4 vec0To1;
        vec0To1.vx = vec1.vx - vec0.vx;
        vec0To1.vy = vec1.vy - vec0.vy;
        vec0To1.vz = vec1.vz - vec0.vz;
        VectorNormal(&vec0To1, &normalizedVec0To1);
    }

    {
        FP_VEC4 vec0To2;
        vec0To2.vx = vec2.vx - vec0.vx;
        vec0To2.vy = vec2.vy - vec0.vy;
        vec0To2.vz = vec2.vz - vec0.vz;
        VectorNormal(&vec0To2, &normalizedVec0To2);
    }

    OuterProduct12(&normalizedVec0To1, &normalizedVec0To2, pOutputTriangleNormal);
    if (pOutputTriangleNormal->vy == 0) {
        outputPosition->vy = 0;
    }
    else {
        outputPosition->vy = vec0.vy + (short)((-(pOutputTriangleNormal->vz * (outputPosition->vz - vec0.vz)) - pOutputTriangleNormal->vx * (outputPosition->vx - vec0.vx)) / pOutputTriangleNormal->vy);
    }
}

s16 findTriangleInWalkMesh(int posX, int posZ, int walkmeshId, SFP_VEC4* param_4, FP_VEC4* triangleNormal)
{
    SFP_VEC4 resultPosition;

    resultPosition.vx = (short)posX;
    resultPosition.vy = 0;
    resultPosition.vz = (short)posZ;

    std::vector<sWalkMesh::sTriangleData>::iterator pTriangle = walkMeshTriangle[walkmeshId]->begin();
    std::vector<SFP_VEC4>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

    sVec2_s16 refPos;
    refPos.set(posX, posZ);

    for (int i = 0; i < walkMeshNumTrianglePerBlock[walkmeshId]; i++)
    {
        sVec2_s16 pos0;
        sVec2_s16 pos1;
        sVec2_s16 pos2;

        pos0.set(pVertices[pTriangle->m0_verticeIndex[0]].vx, pVertices[pTriangle->m0_verticeIndex[0]].vz);
        pos1.set(pVertices[pTriangle->m0_verticeIndex[1]].vx, pVertices[pTriangle->m0_verticeIndex[1]].vz);
        pos2.set(pVertices[pTriangle->m0_verticeIndex[2]].vx, pVertices[pTriangle->m0_verticeIndex[2]].vz);

        if ((NCLIP(pos0, pos1, refPos) > -1) && (NCLIP(pos1, pos2, refPos) > -1) && (NCLIP(pos2, pos0, refPos) > -1))
        {
            SFP_VEC4 vec0 = pVertices[pTriangle->m0_verticeIndex[0]];
            SFP_VEC4 vec1 = pVertices[pTriangle->m0_verticeIndex[1]];
            SFP_VEC4 vec2 = pVertices[pTriangle->m0_verticeIndex[2]];

            getTriangleNormalAndAdjustY(vec0, vec1, vec2, &resultPosition, triangleNormal);
            param_4->vx = resultPosition.vx;
            param_4->vy = resultPosition.vy;
            param_4->vz = resultPosition.vz;

            return i;
        }

        pTriangle++;
    }

    param_4->vx = 0;
    param_4->vy = 0;
    param_4->vz = 0;

    triangleNormal->vx = 0;
    triangleNormal->vy = 0;
    triangleNormal->vz = 0;
    return 0;
}

int getWalkmeshTriangleFlag(sFieldScriptEntity* param_1)
{
    if ((param_1->m4_flags.m_rawFlags >> (param_1->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
        int triangleId = param_1->m8_currentWalkMeshTriangle[param_1->m10_walkmeshId];
        int materialId = (*walkMeshTriangle[param_1->m10_walkmeshId])[triangleId].mC_indexInWalkmeshData1;
        return (*walkMeshVar1)[materialId];
    }
    return 0;
}

void initFieldScriptEntityValues(int index)
{
    sFieldScriptEntity* pFieldScriptEntity = actorArray[index].m4C_scriptEntity;

    pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags = 0xB0;
    pFieldScriptEntity->m4_flags.m_rawFlags = 0x800;
    pFieldScriptEntity->m18_boundingVolume.vx = 0x10;
    pFieldScriptEntity->m18_boundingVolume.vz = 0x10;
    pFieldScriptEntity->m18_boundingVolume.vy = 0x60;
    pFieldScriptEntity->m74 = -1;
    pFieldScriptEntity->m75 = -1;

    pFieldScriptEntity->m40.vx = 0;
    pFieldScriptEntity->m40.vy = 0;
    pFieldScriptEntity->m40.vz = 0;

    pFieldScriptEntity->m30_stepVector.vx = 0;
    pFieldScriptEntity->m30_stepVector.vy = 0;
    pFieldScriptEntity->m30_stepVector.vz = 0;

    pFieldScriptEntity->m60[2] = 0;
    pFieldScriptEntity->m60[0] = 0;
    pFieldScriptEntity->m60[1] = 0;

    pFieldScriptEntity->mD0_targetPositionOffset[0] = 0;
    pFieldScriptEntity->mD0_targetPositionOffset[1] = 0;
    pFieldScriptEntity->mD0_targetPositionOffset[2] = 0;
    pFieldScriptEntity->mE6 = 0;
    pFieldScriptEntity->mEA_forcedAnimation = 0xff;

    pFieldScriptEntity->mE2 = 0;
    pFieldScriptEntity->mCC_scriptPC = 0;
    pFieldScriptEntity->m6E = 0;
    pFieldScriptEntity->m12C_flags &= 0xffffffdf;
    pFieldScriptEntity->m11E = 0x200;
    pFieldScriptEntity->m1E_collisionRadius= pFieldScriptEntity->m18_boundingVolume.vx;
    pFieldScriptEntity->m12C_flags &= 0xfffffffc;
    pFieldScriptEntity->mFF[2] = 0x80;
    pFieldScriptEntity->mFF[1] = 0x80;
    pFieldScriptEntity->mFF[0] = 0x80;
    pFieldScriptEntity->mFC[2] = 0x80;
    pFieldScriptEntity->mFC[1] = 0x80;
    pFieldScriptEntity->mFC[0] = 0x80;
    pFieldScriptEntity->m128 = -1;
    pFieldScriptEntity->m12C_flags &= 0xfffcffff;
    pFieldScriptEntity->m130.m0 = 0;
    pFieldScriptEntity->m12C_flags &= 0xf003ffff;

    for (int i = 0; i < 8; i++)
    {
        pFieldScriptEntity->m8C_scriptSlots[i].m2_delay = 0;
        pFieldScriptEntity->m8C_scriptSlots[i].m0_scriptPC = -1;
        pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m16_status = 0;
        pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m18 = 0xF;
        pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m22 = 0;
        pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m23_walkMode = 0;
        pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m0 = -1;
    }

    delete pFieldScriptEntity->m120_special2dAnimation;
    pFieldScriptEntity->m120_special2dAnimation = nullptr;
    pFieldScriptEntity->m120_special2dAnimationRaw.clear();
    pFieldScriptEntity->mE4_playableCharacterId = 0xFF;
    pFieldScriptEntity->m76 = 0x100;

    pFieldScriptEntity->m82[1] = 0;
    pFieldScriptEntity->m82[0] = 0;
    pFieldScriptEntity->m88[1] = 0;
    pFieldScriptEntity->m88[0] = 0;
    pFieldScriptEntity->m84 = 0;
    pFieldScriptEntity->mCF_scriptSlotWaitedOn = 0;
    pFieldScriptEntity->mCE_currentScriptSlot = 0;
    pFieldScriptEntity->mE8_currentAnimationId = 0;
    pFieldScriptEntity->m10_walkmeshId = 0;
    pFieldScriptEntity->mEC_elevation = 0;

    pFieldScriptEntity->m134.m7_isModelRotation2Allocated = 0;

    pFieldScriptEntity->m12C_flags &= 0xffffe03f;

    pFieldScriptEntity->m134.m5 = 0;

    pFieldScriptEntity->m102_numSteps = xenoRand();

    pFieldScriptEntity->mF4_scale3d[0] = 0x1000;
    pFieldScriptEntity->mF4_scale3d[1] = 0x1000;
    pFieldScriptEntity->mF4_scale3d[2] = 0x1000;
    pFieldScriptEntity->m10D = 0xff;
    pFieldScriptEntity->m80_dialogPortrait = -1;
    pFieldScriptEntity->m106_currentRotation = -0x8000;
    pFieldScriptEntity->m104_rotation = -0x8000;
    pFieldScriptEntity->m108_rotation3 = -0x8000;
    pFieldScriptEntity->m124_special2dAnimationId = -1;
    pFieldScriptEntity->mE3 = 0;
    pFieldScriptEntity->m8_currentWalkMeshTriangle[3] = 0;
    pFieldScriptEntity->m8_currentWalkMeshTriangle[2] = 0;
    pFieldScriptEntity->m8_currentWalkMeshTriangle[1] = 0;
    pFieldScriptEntity->m8_currentWalkMeshTriangle[0] = 0;
    pFieldScriptEntity->m12C_flags &= 0xffffffe3;

    SFP_VEC4 local_48[4];
    FP_VEC4 local_88[4];

    for (int i = 0; i < numWalkMesh; i++)
    {
        pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(actorArray[index].mC_matrix.t[0], actorArray[index].mC_matrix.t[2], i, &local_48[i], &local_88[i]);
        if ((pFieldScriptEntity->m8_currentWalkMeshTriangle[i] != -1) && (walkMeshNumTrianglePerBlock[i] <= pFieldScriptEntity->m8_currentWalkMeshTriangle[i]))
        {
            // seems like this would be a bug
            assert(0);

            walkMeshNumTrianglePerBlock[i] = 0;
            local_88[i].vx = 0;
            local_88[i].vy = 0;
            local_88[i].vz = 0;
            local_48[i].vx = 0;
            local_48[i].vy = 0;
            local_48[i].vz = 0;
        }
    }

    pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);

    pFieldScriptEntity->m50_surfaceNormal.vx = local_88[pFieldScriptEntity->m10_walkmeshId].vx;
    pFieldScriptEntity->m50_surfaceNormal.vy = local_88[pFieldScriptEntity->m10_walkmeshId].vy;
    pFieldScriptEntity->m50_surfaceNormal.vz = local_88[pFieldScriptEntity->m10_walkmeshId].vz;

    if ((actorArray[index].m58_flags & 0x80) == 0) {
        actorArray[index].mC_matrix.t[1] = local_48[pFieldScriptEntity->m10_walkmeshId].vy;
    }

    (pFieldScriptEntity->m20_position).vx = (int)actorArray[index].mC_matrix.t[0] << 0x10;
    (pFieldScriptEntity->m20_position).vy = (int)actorArray[index].mC_matrix.t[1] << 0x10;
    (pFieldScriptEntity->m20_position).vz = (int)actorArray[index].mC_matrix.t[2] << 0x10;

    pFieldScriptEntity->m72_elevation = actorArray[index].mC_matrix.t[1];
}

void initCharacterShadowPoly(sFieldEntity2dSprite* pSprite)
{
    POLY_FT4* p = &pSprite->m20_shadowPoly[0];
    SetPolyFT4(p);
    pSprite->m0_shadowPolyVertices[3].vx = -0x18;
    pSprite->m0_shadowPolyVertices[3].vz = -0x18;
    pSprite->m0_shadowPolyVertices[2].vz = -0x18;
    pSprite->m0_shadowPolyVertices[1].vx = -0x18;
    pSprite->m0_shadowPolyVertices[3].vy = 0;
    pSprite->m0_shadowPolyVertices[2].vx = 0x18;
    pSprite->m0_shadowPolyVertices[2].vy = 0;
    pSprite->m0_shadowPolyVertices[1].vy = 0;
    pSprite->m0_shadowPolyVertices[1].vz = 0x18;
    pSprite->m0_shadowPolyVertices[0].vx = 0x18;
    pSprite->m0_shadowPolyVertices[0].vy = 0;
    pSprite->m0_shadowPolyVertices[0].vz = 0x18;
    pSprite->m20_shadowPoly[0].r0 = 0x80;
    pSprite->m20_shadowPoly[0].g0 = 0x80;
    pSprite->m20_shadowPoly[0].b0 = 0x80;
    pSprite->m20_shadowPoly[0].tpage = GetTPage(0, 2, 0x280, 0x1e0);
    pSprite->m20_shadowPoly[0].clut = GetClut(0x100, 0xf3);
    SetSemiTrans(p, 1);
    pSprite->m20_shadowPoly[0].v0 = 0xe0;
    pSprite->m20_shadowPoly[0].v1 = 0xe0;
    pSprite->m20_shadowPoly[0].u0 = 0;
    pSprite->m20_shadowPoly[0].u1 = 0xF;
    pSprite->m20_shadowPoly[0].u2 = 0;
    pSprite->m20_shadowPoly[0].v2 = 0xef;
    pSprite->m20_shadowPoly[0].u3 = 0xF;
    pSprite->m20_shadowPoly[0].v3 = 0xef;

    pSprite->m20_shadowPoly[1] = pSprite->m20_shadowPoly[0];
}

int numInitializedFieldScriptEntities = 0;

void initFieldScriptEntity(int index)
{
    if (index < totalActors)
    {
        numInitializedFieldScriptEntities++;
        sFieldScriptEntity* pNewFieldScriptEntity = new sFieldScriptEntity;
        actorArray[index].m4C_scriptEntity = pNewFieldScriptEntity;
        memset(pNewFieldScriptEntity, 0, sizeof(sFieldScriptEntity));
        new(pNewFieldScriptEntity) sFieldScriptEntity;

        actorArray[index].m5A = 0;

        if (actorArray[index].m58_flags & 0x2000)
        {
            pNewFieldScriptEntity->m118.resize(32);
            MissingCode();
        }

        initFieldScriptEntityValues(index);
        actorArray[index].m8_2dSprite = new sFieldEntity2dSprite;
        initCharacterShadowPoly(actorArray[index].m8_2dSprite);
    }
}

void traceNextAlloc(int state)
{
    MissingCode();
}

void initModel1(sModelBlock& pModelBlock, std::vector<sTag*>& outputBuffer1, std::vector<sTag*>& outputBuffer2)
{
    traceNextAlloc(0x25);

    outputBuffer1.resize(pModelBlock.m34_instanceBufferSize, nullptr);
    outputBuffer2.resize(pModelBlock.m34_instanceBufferSize, nullptr);
}

int overrideTPageMode = 0;
int initModel2Sub0Var1 = 1;

void initModel2Sub0(void)
{
    overrideTPageMode = 0;
    initModel2Sub0Var1 = 1;
    return;
}

s32 overrideTPageValue = 0;
s16 primD_initSub0Sub0Var0 = 0;

void setupPrimTexturePage(ushort param_1)

{
    primD_initSub0Sub0Var0 = (ushort)overrideTPageValue;
    if (overrideTPageMode == 1) {
        primD_initSub0Sub0Var0 = param_1 & 0xffe0 | primD_initSub0Sub0Var0;
    }
    else {
        if (overrideTPageMode != 2) {
            primD_initSub0Sub0Var0 = param_1;
            return;
        }
    }
    return;
}

s16 primD_initSub0Sub1Var0 = 0;
s32 primD_initSub0Sub1Var1 = 0;

void setupPrimClut(ushort param_1)
{
    primD_initSub0Sub1Var0 = param_1;
    if (initModel2Sub0Var1 == 0) {
        primD_initSub0Sub1Var0 = primD_initSub0Sub1Var0 & 0xf | (ushort)primD_initSub0Sub1Var1;
    }
    return;
}

int primD_isValid(u8* displayList)
{
    byte bVar1;
    int iVar2;

    bVar1 = READ_LE_U8(displayList + 3);
    iVar2 = 1;
    if ((bVar1 & 0xf0) == 0xc0) {
        if (bVar1 == 0xc4) {
            setupPrimTexturePage(READ_LE_U16(displayList));
            iVar2 = 0;
        }
        else {
            iVar2 = 1;
            if (bVar1 == 0xc8) {
                setupPrimClut(READ_LE_U16(displayList));
                iVar2 = 0;
            }
        }
    }
    return iVar2;
}

std::vector<sTag*>::iterator currentModelInstanceDrawPrims;

u8* currentModelBlockDisplayLists;
u8* g_currentModelBlockSubBlocks;
u8* currentModelBlockNormals;
u8* currentModelBlockVertices;
u8* currentModeBlock18;

void computeFaceNormal(const SFP_VEC4& v0, const SFP_VEC4& v1, const SFP_VEC4& v2, SFP_VEC4* pNormal)
{
    MissingCode();
}

void NormalColorCol(SFP_VEC4* faceNormal, u8* displayList, POLY_FT3* pNewPoly)
{
    MissingCode();
}

void NormalColor(SFP_VEC4* $2, std::array<u8,4> $3)
{
/*    gte_ldv0((SVECTOR*)$2);
    gte_ncs_b();
    gte_strgb($3);
    return;*/
    MissingCode();
}

int prim0_init(u8* displayList, u8* meshBlock, int initParam)
{
    POLY_FT3* pNewPoly = new POLY_FT3;
    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;

    pNewPoly->m3_size = 4;

    if ((initParam & 1) == 0) {
        if ((initParam & 4) == 0) {
            pNewPoly->r0 = READ_LE_U8(displayList + 0);
            pNewPoly->g0 = READ_LE_U8(displayList + 1);
            pNewPoly->b0 = READ_LE_U8(displayList + 2);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }
        currentModeBlock18 = currentModeBlock18 + 4;
    }
    else {
        if ((initParam & 2) == 0) {
            SFP_VEC4 faceNormal;
            computeFaceNormal(
                SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock) * 8),
                SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 2) * 8),
                SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 4) * 8),
                &faceNormal
            );
            NormalColorCol(&faceNormal, displayList, pNewPoly);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }

        *(u32*)&(*currentModeBlock18) = READ_LE_U32(displayList);
        currentModeBlock18 = currentModeBlock18 + 4;

        computeFaceNormal(
            SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock) * 8),
            SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 2) * 8),
            SFP_VEC4::FromIt(currentModelBlockVertices + READ_LE_U16(meshBlock + 4) * 8),
            (SFP_VEC4*)&(*currentModeBlock18)
        );
    }
    NormalColorCol((SFP_VEC4*)&(*currentModeBlock18), displayList, pNewPoly);
    currentModeBlock18 = currentModeBlock18 + 4;
    pNewPoly->code = READ_LE_U8(displayList + 3);
    return 1;
}

int prim1_init(u8* displayList, u8* meshBlock, int initParam)
{
    if (primD_isValid(displayList))
    {
        //assert(READ_LE_U8(displayList + 3) == 0x24); // triangle with texture

        POLY_FT3* pNewPoly = new POLY_FT3;

        pNewPoly->m3_size = 7;
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        if ((initParam & 1) == 0) {
            if (initParam & 4) {
                //NormalColor(currentModeBlock18, pNewPoly);
                MissingCode();
            }
        }
        else
        {
            MissingCode();
        }
        currentModeBlock18 += 8;

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim2_init(u8* displayList, u8* meshBlock, int initParam) {
    MissingCode();
    return 1;
}

struct CVECTOR {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
};

void NormalColor3(SFP_VEC4* $2, SFP_VEC4* $3, SFP_VEC4* v2, CVECTOR* v3, CVECTOR* v4, CVECTOR* v5)
{
    MissingCode();
}

int prim3_init(u8* displayList, u8* meshBlock, int initParam)
{
    if (primD_isValid(displayList))
    {
        //assert((READ_LE_U8(displayList + 3) & ~(16 | 2 | 1)) == 0x34);

        POLY_GT3* pNewPoly = new POLY_GT3;

        pNewPoly->m3_size = 9;

        SFP_VEC4 n0 = SFP_VEC4::FromIt(currentModelBlockNormals + READ_LE_U16(meshBlock) * 8);
        SFP_VEC4 n1 = SFP_VEC4::FromIt(currentModelBlockNormals + READ_LE_U16(meshBlock + 2) * 8);
        SFP_VEC4 n2 = SFP_VEC4::FromIt(currentModelBlockNormals + READ_LE_U16(meshBlock + 4) * 8);

        NormalColor3(&n0, &n1, &n2, (CVECTOR*)&pNewPoly->r0, (CVECTOR*)&pNewPoly->r1, (CVECTOR*)&pNewPoly->r2);

        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim4_init(u8* displayList, u8* meshBlock, int initParam)
{
    //assert(READ_LE_U8(displayList + 3) == 0x20); // colored triangle

    POLY_F3* pNewPoly = new POLY_F3;

    pNewPoly->m3_size = 4;
    pNewPoly->r0 = READ_LE_U8(displayList + 0);
    pNewPoly->g0 = READ_LE_U8(displayList + 1);
    pNewPoly->b0 = READ_LE_U8(displayList + 2);
    pNewPoly->code = READ_LE_U8(displayList + 3);

    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;
    return 1;
}

int prim5_init(u8* displayList, u8* meshBlock, int initParam)
{
    if (primD_isValid(displayList))
    {
        assert((READ_LE_U8(displayList + 3) & ~(16 | 2 | 1)) == 0x24);

        POLY_FT3* pNewPoly = new POLY_FT3;

        pNewPoly->m3_size = 7;
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 0);
        pNewPoly->v2 = READ_LE_U8(displayList + 1);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

int prim8_init(u8* displayList, u8* meshBlock, int initParam)
{
    assert(READ_LE_U8(displayList + 3) == 0x28); // quad with color

    POLY_F4* pNewPoly = new POLY_F4;

    pNewPoly->m3_size = 4;
    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;

    if ((initParam & 1) == 0) {
        if ((initParam & 4) == 0) {
            pNewPoly->r0 = READ_LE_U8(displayList + 0);
            pNewPoly->g0 = READ_LE_U8(displayList + 1);
            pNewPoly->b0 = READ_LE_U8(displayList + 2);
            pNewPoly->code = READ_LE_U8(displayList + 3);
            return 1;
        }
        currentModeBlock18 = currentModeBlock18 + 4;
    }
    else
    {
        assert(0);
    }
    pNewPoly->code = READ_LE_U8(displayList + 3);
    return 1;
}

int prim9_init(u8* displayList, u8* meshBlock, int initParam)
{
    if (primD_isValid(displayList))
    {
        assert(READ_LE_U8(displayList + 3) == 0x2C); // quad with texture

        POLY_FT4* pNewPoly = new POLY_FT4;

        pNewPoly->m3_size = 9;
        pNewPoly->r0 = READ_LE_U8(displayList + 0);
        pNewPoly->g0 = READ_LE_U8(displayList + 1);
        pNewPoly->b0 = READ_LE_U8(displayList + 2);
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 8);
        pNewPoly->v2 = READ_LE_U8(displayList + 9);
        pNewPoly->u3 = READ_LE_U8(displayList + 10);
        pNewPoly->v3 = READ_LE_U8(displayList + 11);

        currentModeBlock18 += 8;
        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}


int primC_init(u8* displayList, u8* meshBlock, int initParam)
{
    //assert(READ_LE_U8(displayList + 3) == 0x28); // quad with color

    POLY_F4* pNewPoly = new POLY_F4;

    pNewPoly->m3_size = 5;
    pNewPoly->r0 = READ_LE_U8(displayList + 0);
    pNewPoly->g0 = READ_LE_U8(displayList + 1);
    pNewPoly->b0 = READ_LE_U8(displayList + 2);
    pNewPoly->code = READ_LE_U8(displayList + 3);

    *currentModelInstanceDrawPrims = pNewPoly;
    currentModelInstanceDrawPrims++;
    return 1;
}

s32 screenClippingX = 319;
s32 screenClippingY = 238 << 0x10;

OTTable::iterator currentOTEntry;

void genericTrianglePrim_14(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_F3* pOutputPrim = (POLY_F3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x14);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 4);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 4);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void genericTrianglePrim_28(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_GT3* pOutputPrim = (POLY_GT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 0xC);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void F4_FAKE(u8* meshSubBlock, int count) {
    MissingCode();
    Hack("TODO: this is not correct and implemented as a shortcut");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_F4* pOutputPrim = (POLY_F4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    pOutputPrim->m3_size = 7;
                }
            }
        }
    }
}

void FT3_FAKE(u8* meshSubBlock, int count) {
    MissingCode();
    Hack("TODO: this is not correct and implemented as a shortcut");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    pOutputPrim->m3_size = 7;
                }
            }
        }
    }
}

void genericTrianglePrim_20(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x20);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 7);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim5_0(u8* meshSubBlock, int count)
{
    genericTrianglePrim_20(meshSubBlock, count, 0x20, 0x7, 0x8);
}

void prim5_4(u8* meshSubBlock, int count) {
    Noah_MissingCode("Incorrect prim5_4");
    genericTrianglePrim_20(meshSubBlock, count, 0x20, 0x7, 0x8);
}

void prim5_2generic(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT3* pOutputPrim = (POLY_FT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x20);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                assert(outputStride == 0x8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 7);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim3_0(u8* meshSubBlock, int count)
{
    genericTrianglePrim_28(meshSubBlock, count, 0x28, 0x9, 0xc);
}

void GT3_FAKE(u8* meshSubBlock, int count)
{
    Hack("Not how it's supposed to work");

    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_GT3* pOutputPrim = (POLY_GT3*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy) < 0)
                    continue;

                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;

                int sz0, sz1, sz2;
                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 && sz1 && sz2) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    //pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void prim4_0(u8* meshSubBlock, int count)
{
    genericTrianglePrim_14(meshSubBlock, count, 0x14, 0x4, 4);
}

void prim5_2(u8* meshSubBlock, int count)
{
    prim5_2generic(meshSubBlock, count, 0x20, 0x7, 0x8);
}

void prim6_2(u8* meshSubBlock, int count)
{
    prim5_2generic(meshSubBlock, count, 0x28, 0x9, 0xC);
}

void prim6_3(u8* meshSubBlock, int count)
{
    assert(0);
}

void primD_2generic(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT4* pOutputPrim = (POLY_FT4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        u8* pVertices4 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 6) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                gte_ldv0(pVertices4); // load last vertex and compute 3rd point
                gte_rtps();

                int flagsTriangle1;
                gte_stlzc(&flagsTriangle1);

                sVec2_s16 xy3;
                gte_stsxy2(&xy3);

                if (flagsTriangle1 < 0)
                {
                    continue;
                }

                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;
                pOutputPrim->x3y3 = xy3;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void primD_2(u8* meshSubBlock, int count)
{
    primD_2generic(meshSubBlock, count, 0x28, 0x9, 0x8);
}

void primD_0generic(u8* meshSubBlock, int count, int outputPrimSize, uint primSize, int outputStride)
{
    OTTable::iterator pOT = currentOTEntry;
    int depthGranularity = gDepthDivider + 2;

    for (int i = 0; i < count; i++)
    {
        POLY_FT4* pOutputPrim = (POLY_FT4*)*currentModelInstanceDrawPrims;
        currentModelInstanceDrawPrims++;
        assert(outputPrimSize == 0x28);

        u8* pVertices1 = currentModelBlockVertices + READ_LE_U16(meshSubBlock) * 8;
        u8* pVertices2 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 2) * 8;
        u8* pVertices3 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 4) * 8;
        u8* pVertices4 = currentModelBlockVertices + READ_LE_U16(meshSubBlock + 6) * 8;
        meshSubBlock += 8;

        gte_ldv3(pVertices1, pVertices2, pVertices3);
        gte_rtpt();

        int flagsTriangle0;
        gte_stlzc(&flagsTriangle0);

        sVec2_s16 xy0;
        sVec2_s16 xy1;
        sVec2_s16 xy2;
        gte_stsxy3(&xy0, &xy1, &xy2);

        if (flagsTriangle0 > 0)
        {
            gte_nclip(); // write the normal to mac0

            int normalMagnitude;
            gte_getMAC0(&normalMagnitude);

            if (normalMagnitude > 0) // is first triangle facing screen
            {
                gte_ldv0(pVertices4); // load last vertex and compute 3rd point
                gte_rtps();

                int flagsTriangle1;
                gte_stlzc(&flagsTriangle1);

                sVec2_s16 xy3;
                gte_stsxy2(&xy3);

                if (flagsTriangle1 < 0)
                {
                    continue;
                }

                // TODO: this is not how the original code works, but this works for now

                // X clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) > screenClippingX)
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vx, xy1.vx), xy2.vx), xy3.vx) < 0)
                    continue;

                // Y clip
                if (std::min<int>(std::min<int>(std::min<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) > (screenClippingY >> 16))
                    continue;
                if (std::max<int>(std::max<int>(std::max<int>(xy0.vy, xy1.vy), xy2.vy), xy3.vy) < 0)
                    continue;

                assert(outputStride == 8);
                pOutputPrim->x0y0 = xy0;
                pOutputPrim->x1y1 = xy1;
                pOutputPrim->x2y2 = xy2;
                pOutputPrim->x3y3 = xy3;

                int sz0, sz1, sz2, sz3;
                gte_stsz4(&sz0, &sz1, &sz2, &sz3);

                if (sz0 && sz1 && sz2 && sz3) {
                    int polyz = std::max<int>(sz0, sz1);
                    polyz = std::max<int>(sz2, polyz);
                    polyz = std::max<int>(sz3, polyz);
                    fieldPolyCount = fieldPolyCount + 1;
                    assert(polyz);

                    sTag* sDestTag = pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext;
                    pOT[polyz >> (depthGranularity & 0x1f)].m0_pNext = pOutputPrim;
                    pOutputPrim->m0_pNext = sDestTag;
                    assert(primSize == 9);
                    pOutputPrim->m3_size = primSize;
                }
            }
        }
    }
}

void primD_0(u8* meshSubBlock, int count)
{
    primD_0generic(meshSubBlock, count, 0x28, 0x9, 0x8);
}

void primD_1(u8* meshSubBlock, int count)
{
    Hack("hacked primD_1");
    primD_0generic(meshSubBlock, count, 0x28, 0x9, 0x8);
}


void primD_4(u8* meshSubBlock, int count)
{
    Noah_MissingCode("primD_4");
    primD_0generic(meshSubBlock, count, 0x28, 0x9, 0x8);
}

int primD_init(u8* displayList, u8* meshBlock, int initParam)
{
    if (primD_isValid(displayList))
    {
        POLY_FT4* pNewPoly = new POLY_FT4;

        pNewPoly->m3_size = 9;
        pNewPoly->r0 = READ_LE_U8(displayList + 0);
        pNewPoly->g0 = READ_LE_U8(displayList + 1);
        pNewPoly->b0 = READ_LE_U8(displayList + 2);
        pNewPoly->code = READ_LE_U8(displayList + 3);
        pNewPoly->u0 = READ_LE_U8(displayList + 4);
        pNewPoly->v0 = READ_LE_U8(displayList + 5);
        pNewPoly->clut = primD_initSub0Sub1Var0;
        pNewPoly->u1 = READ_LE_U8(displayList + 6);
        pNewPoly->v1 = READ_LE_U8(displayList + 7);
        pNewPoly->tpage = primD_initSub0Sub0Var0;
        pNewPoly->u2 = READ_LE_U8(displayList + 8);
        pNewPoly->v2 = READ_LE_U8(displayList + 9);
        pNewPoly->u3 = READ_LE_U8(displayList + 10);
        pNewPoly->v3 = READ_LE_U8(displayList + 11);

        *currentModelInstanceDrawPrims = pNewPoly;
        currentModelInstanceDrawPrims++;
        return 1;
    }
    return 0;
}

typedef void(*t_primRenderFunc)(u8* meshSubBlock, int count);
typedef int(*t_primInitFunc)(u8* displayList, u8* meshBlock, int initParam);

struct sPolyTypeRenderDefinition
{
    t_primRenderFunc m0_type0;
    t_primRenderFunc m4_type1;
    t_primRenderFunc m8_type2;
    t_primRenderFunc mC_type3;
    t_primRenderFunc m10_type4;
    t_primRenderFunc m14_type5;
    t_primInitFunc m18_init;
    int m1C_size;
    int m20_sizeDisplayList;
    int m24_sizeInstanceArray;
};

const std::array<sPolyTypeRenderDefinition, 17> polyRenderDefs = { {
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim0_init,	8,	0x4,	0x04}, // 0x0 POLY_FT3 with light
    {	prim5_0,	FT3_FAKE,	prim5_2,	nullptr,	nullptr,	nullptr,	prim1_init,	8,	0x8,	0x20}, // 0x1 POLY_FT3
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim2_init,	8,	0x4,	0x1C}, // 0x2
    {	prim3_0,	GT3_FAKE,	nullptr,	nullptr,	prim3_0,	prim3_0,	prim3_init,	8,	0x8,	0x28}, // 0x3 POLY_GT3
    {	prim4_0,	prim4_0,	nullptr,	nullptr,	prim4_0,	prim4_0,	prim4_init,	8,	0x4,	0x14}, // 0x4 POLY_F3
    {	prim5_0,	nullptr,	prim5_2,	nullptr,	prim5_4,	nullptr,	prim5_init,	8,	0x8,	0x20}, // 0x5 POLY_FT3
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x1C}, // 0x6
    {	prim3_0,	prim3_0,	prim6_2,	prim6_3,	prim3_0,	prim3_0,	prim3_init,	8,	0x8,	0x28}, // 0x7
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	prim8_init,	8,	0x4,	0x18}, // 0x8 POLY_F4
    {	nullptr,	primD_1,	nullptr,	nullptr,	nullptr,	nullptr,	prim9_init,	8,	0xC,	0x28}, // 0x9 POLY_FT4
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xA
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xB
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	primC_init,	8,	0x4,	0x18}, // 0xC POLY_F4
    {	primD_0,	nullptr,	primD_2,	nullptr,	primD_4,	nullptr,	primD_init,	8,	0xC,	0x28}, // 0xD POLY_FT4
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x24}, // 0xE
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0xC,	0x34}, // 0xF
    {	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	8,	0x4,	0x20}, // 0x10
} };

void initModel5(sModelBlock* pModelBlock)
{
    MissingCode();
}

void* initModelDynamicVertices(sModelBlock* pModelBlock)
{
    MissingCode();
    return nullptr;
}

void initModel2(sModelBlock* pModelBlock, std::vector<sTag*>& outputBuffer, int param_3)
{
    currentModelInstanceDrawPrims = outputBuffer.begin();

    if ((((pModelBlock->m0_flags & 1) == 0) && (pModelBlock->m30 != 0)) && (param_3 != 0)) {
        traceNextAlloc(0x26);
        pModelBlock->m18.resize(pModelBlock->m30);
        pModelBlock->m0_flags |= 1;
    }

    currentModelBlockDisplayLists = pModelBlock->m_baseItForRelocation + pModelBlock->m14_offsetDisplayList;
    g_currentModelBlockSubBlocks = pModelBlock->m_baseItForRelocation + pModelBlock->m10_offsetMeshBlocks;
    currentModelBlockNormals = pModelBlock->m_baseItForRelocation + pModelBlock->mC_offsetNormals;
    currentModelBlockVertices = pModelBlock->m_baseItForRelocation + pModelBlock->m8_offsetVertices;
    currentModeBlock18 = nullptr;
    if(pModelBlock->m18.size())
    {
        currentModeBlock18 = &pModelBlock->m18[0];
    }

    int initParam;
    switch (param_3)
    {
    case 0:
        initParam = 0;
        break;
    case 1:
        initParam = 1;
        break;
    case 2:
        if (pModelBlock->m0_flags & 2)
        {
            initParam = 1;
            if (pModelBlock->m0_flags & 1)
            {
                initParam = 4;
            }
        }
        else
        {
            if (pModelBlock->m0_flags & 1)
            {
                initParam = 1;
            }
            else
            {
                initParam = 3;
                pModelBlock->m0_flags |= 2;
            }
        }
        break;
    case 3:
        initParam = 3;
        pModelBlock->m0_flags |= 2;
        break;
    default:
        assert(0);
    }

    fieldPolyCount2 += pModelBlock->m4_numPrims;

    int currentMeshBlockCount = pModelBlock->m6_numMeshBlock;
    while (currentMeshBlockCount = currentMeshBlockCount - 1, currentMeshBlockCount != 0xffffffff) {
        int primType = READ_LE_U8(g_currentModelBlockSubBlocks);
        int numPrims = READ_LE_U16(g_currentModelBlockSubBlocks + 2) - 1;

        t_primInitFunc pInitFunction = polyRenderDefs[primType].m18_init;
        g_currentModelBlockSubBlocks += 4;
        while (numPrims != -1)
        {
            if (pInitFunction(currentModelBlockDisplayLists, g_currentModelBlockSubBlocks, (int)initParam) == 0)
            {
                currentModelBlockDisplayLists += 4;
            }
            else
            {
                g_currentModelBlockSubBlocks += polyRenderDefs[primType].m1C_size;
                //currentModelInstanceArray8 += polyRenderDefs[primType].m24_sizeInstanceArray;
                currentModelBlockDisplayLists += polyRenderDefs[primType].m20_sizeDisplayList;
                numPrims = numPrims + -1;

            }
        }
    }

    initModel2Sub0();
}

int fieldScriptEntityAlreadyInitialized = 0;

// 0: scenarioFlag
std::array<s16, 1024> fieldVars;

void setVar(int varIndex, s16 value)
{
    fieldVars[varIndex / 2] = value;
}

void setVarsForCurrentParty()
{
    setVar(0x3E, currentParty[0]);
    setVar(0x40, currentParty[1]);
    setVar(0x42, currentParty[2]);
}

sFieldEntity* pCurrentFieldEntity;
int fieldScriptInitVar0;
int currentScriptFinished;

u16 getScriptEntryPoint(int entityId, int scriptIndex)
{
    return READ_LE_U16(rawFieldScriptData.begin() + (entityId * 0x20 + scriptIndex) * 2 + 0x84);
}

int breakCurrentScript;
int fieldExectuteMaxCycles;

int fieldDebugDisable = 0;
int renderCharsDisabled = 0;

void sprintf_screen(const char* format, ...)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int clutYEntry, sSpriteActorAnimationBundle* pSetup, int param_4, int clutXEntry, int param_6, int param_7)
{
    resetMemoryAllocStats(8, 0);
    actorArray[actorId].m4C_scriptEntity->m127 = clutYEntry;
    actorArray[actorId].m4C_scriptEntity->m126 = param_6;
    actorArray[actorId].m4C_scriptEntity->m134.m0 = clutXEntry;
    actorArray[actorId].m4C_scriptEntity->m130.m28 = param_4;
    actorArray[actorId].m4C_scriptEntity->m134.m4 = param_7;

    if (param_4 == 0)
    {
        s16 vramX = fieldVramMapping[clutYEntry].m0_vramX;
        s16 vramY = fieldVramMapping[clutYEntry].m2_vramY;

        if (clutXEntry == 0)
        {
            if (actorArray[actorId].m5A & 1) {
                deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
            }

            actorArray[actorId].m4_pVramSpriteSheet = createSpriteActor(pSetup, 0x100, ((clutYEntry + 480) * 0x10000) >> 0x10, vramX, vramY, 0x40);
        }
        else
        {
            if ((actorArray[actorId].m5A & 1U) != 0) {
                deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
            }
            actorArray[actorId].m4_pVramSpriteSheet = createSpriteActorEX(pSetup, (short)((clutXEntry * 0x10 + 0x100) * 0x10000 >> 0x10), (short)((uint)((clutYEntry + 480) * 0x10000) >> 0x10), vramX, vramY, 0x40, clutXEntry);
        }
    }
    else
    {
        int iVar4;
        short sVar5;

        if ((actorArray[actorId].m5A & 1U) != 0) {
            deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
        }
        if (param_4 == 1) {
            iVar4 = clutYEntry + 0xe0;
            sVar5 = 0x280;
        }
        else {
            iVar4 = clutYEntry + 0xe3;
            sVar5 = 0x2a0;
        }
        actorArray[actorId].m4_pVramSpriteSheet = createSpriteActor(pSetup, 0x100, (short)((uint)(iVar4 * 0x10000) >> 0x10), sVar5, (short)clutYEntry * 0x40 + 0x100, 8);
        OP_INIT_ENTITY_SCRIPT_sub0Sub3(actorArray[actorId].m4_pVramSpriteSheet, 0x20);
    }

    actorArray[actorId].m5A |= 1;

    sSpriteActor* pFieldEntitySub4 = actorArray[actorId].m4_pVramSpriteSheet;

    int temp0;
    int temp1;
    int temp2;
    OP_INIT_ENTITY_SCRIPT_sub0Sub4(pFieldEntitySub4, 0, &temp0, &temp1, &temp2);
    OP_INIT_ENTITY_SCRIPT_sub0Sub5(pFieldEntitySub4, 3);

    pFieldEntitySub4->m2C_scale = 0xC00;
    pFieldEntitySub4->m82 = 0x2000;

    if (!fieldScriptEntityAlreadyInitialized)
    {
        pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
        pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
        pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
        pFieldEntitySub4->mC_step.vy = 0;
        pFieldEntitySub4->mC_step.vx = 0;
        pFieldEntitySub4->mC_step.vy = 0;
        pFieldEntitySub4->mC_step.vz = 0;
        pFieldEntitySub4->m1C_gravity = 0x10000;
        pFieldEntitySub4->m84_maxY = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;
        if (!param_4)
        {
            actorArray[actorId].m4C_scriptEntity->m18_boundingVolume.vx = temp1 * 2;
        }
        else
        {
            actorArray[actorId].m4C_scriptEntity->m18_boundingVolume.vx = 0x40;
        }
    }
    else
    {
        assert(0);
    }

    if (isFogSetup)
    {
        pFieldEntitySub4->m40 |= 0x40000;
    }

    spriteActorSetPlayingAnimation(pFieldEntitySub4, 0);
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(pFieldEntitySub4, 0);
    resetMemoryAllocStats(8, 0);

    pFieldEntitySub4->m7C->m14_actorId = actorId;
    OP_INIT_ENTITY_SCRIPT_sub0Sub8(pFieldEntitySub4, &fieldActorCallback);

    if (!param_7)
    {
        OP_INIT_ENTITY_SCRIPT_sub0Sub9(pFieldEntitySub4);
        execSpritesCallbacksList2();
        if (pFieldEntitySub4->m7C->mC == 0xff) {
            (actorArray[actorId].m4C_scriptEntity)->mEA_forcedAnimation = 0xff;
            (actorArray[actorId].m4C_scriptEntity)->m4_flags.m_rawFlags |= 0x1000000;
            pFieldEntitySub4->m0_position.vx = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vx;
            pFieldEntitySub4->m0_position.vy = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vy;
            pFieldEntitySub4->m0_position.vz = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vz;
        }
    }

    actorArray[actorId].mC_matrix.t[0] = actorArray[actorId].m4C_scriptEntity->m20_position.vx >> 16;
    actorArray[actorId].mC_matrix.t[1] = actorArray[actorId].m4C_scriptEntity->m20_position.vy >> 16;
    actorArray[actorId].mC_matrix.t[2] = actorArray[actorId].m4C_scriptEntity->m20_position.vz >> 16;

    actorArray[actorId].m2C_matrixBackup.t = actorArray[actorId].mC_matrix.t;

    pFieldEntitySub4->m84_maxY = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;

    pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
    pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
    pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
    fieldScriptInitVar0++;
}

void OP_INIT_ENTITY_SCRIPT_sub1()
{
    actorArray[currentFieldActorId].mC_matrix.t[0] = actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = pCurrentFieldScriptActor->m20_position.vx >> 16;
    actorArray[currentFieldActorId].mC_matrix.t[1] = actorArray[currentFieldActorId].m2C_matrixBackup.t[1] = pCurrentFieldScriptActor->m20_position.vy >> 16;
    actorArray[currentFieldActorId].mC_matrix.t[2] = actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = pCurrentFieldScriptActor->m20_position.vz >> 16;

    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = pCurrentFieldScriptActor->m20_position.vx;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = pCurrentFieldScriptActor->m20_position.vy;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = pCurrentFieldScriptActor->m20_position.vz;

    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vy = 0;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m84_maxY = pCurrentFieldScriptActor->m20_position.vy >> 16;
    pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy >> 16;
}

int asyncLoadingVar1 = 0xFF;

int findFreePartySlot(int param_1, int* param_2)
{
    int iVar1;

    iVar1 = 0;
    while (true) {
        if (currentParty[iVar1] == param_1) {
            return -1;
        }
        if (currentParty[iVar1] == 0xFF)
            break;
        iVar1 = iVar1 + 1;

        if (2 < iVar1) {
            return -1;
        }
    }
    *param_2 = iVar1;
    return 0;
}

int asyncPartyCharacterLoadingCharacterIndex = 0;
int asyncPartyCharacterLoadingIndex = 0;
int fieldExecuteVar1 = 0;
std::array<int, 3> asyncPartyCharacterLoadingTable;
std::vector<u8> asyncPartyCharacterLoadingBuffer;

void copyPartySlotFromNext(uint param_1)
{
    s32* piVar7 = &partyToFieldEntityArrayMapping[param_1 + 1];
    int iVar13 = *piVar7;
    if (iVar13 == 0xff) {
        asyncPartyCharacterLoadingTable[param_1] = asyncPartyCharacterLoadingTable[param_1 + 1];
        currentParty[param_1] = currentParty[param_1 + 1];
        partyToFieldEntityArrayMapping[param_1] = *piVar7;
        currentParty[param_1 + 1] = 0xff;
        asyncPartyCharacterLoadingTable[param_1 + 1] = 0xff;
        *piVar7 = 0xff;
    }
    else {
        partyCharacterBuffers[param_1] = partyCharacterBuffers[param_1 + 1];
        asyncPartyCharacterLoadingTable[param_1] = asyncPartyCharacterLoadingTable[param_1 + 1];
        currentParty[param_1] = currentParty[param_1 + 1];
        partyToFieldEntityArrayMapping[param_1] = partyToFieldEntityArrayMapping[param_1 + 1];
        sFieldScriptEntity* psVar3 = actorArray[iVar13].m4C_scriptEntity;
        if ((psVar3->m126 & 0x80) == 0) {
            OP_INIT_ENTITY_SCRIPT_sub0(iVar13, param_1, &partyCharacterBuffers[param_1], 1, 0, param_1, 1);
        }
        else {
            OP_INIT_ENTITY_SCRIPT_sub0(iVar13, psVar3->m127, &fieldActorSetupParams[psVar3->m126 & 0x7f], psVar3->m130.m28, psVar3->m134.m0, actorArray[iVar13].m4C_scriptEntity->m126, actorArray[iVar13].m4C_scriptEntity->m134.m4);
        }
        currentParty[param_1 + 1] = 0xff;
        asyncPartyCharacterLoadingTable[param_1 + 1] = 0xff;
        partyToFieldEntityArrayMapping[param_1 + 1] = 0xff;
    }
    return;
}

void emptyPartySlot(int param_1)
{
    uint* puVar1;
    ushort* puVar2;
    ushort uVar3;
    int iVar4;
    sFieldScriptEntity* psVar5;
    sFieldScriptEntity* psVar6;
    sFieldEntity* psVar7;
    int iVar8;
    int* piVar9;

    psVar7 = pCurrentFieldEntity;
    psVar5 = pCurrentFieldScriptActor;
    iVar4 = currentFieldActorId;
    piVar9 = &partyToFieldEntityArrayMapping[param_1];
    if (*piVar9 != 0xff) {
        pCurrentFieldEntity = &actorArray[*piVar9];
        uVar3 = pCurrentFieldScriptActor->mCC_scriptPC;
        pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
        initFieldScriptEntityValues(*piVar9);
        iVar8 = *piVar9;
        currentFieldActorId = iVar8;
        actorArray[iVar8].m58_flags = actorArray[iVar8].m58_flags & 0xf07f | 0x200;
        OP_INIT_ENTITY_SCRIPT_sub0(iVar8, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
        psVar6 = pCurrentFieldScriptActor;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m0_updateScriptDisabled = 1;
        puVar1 = &pCurrentFieldScriptActor->m4_flags.m_rawFlags;
        breakCurrentScript = 0;
        puVar2 = &pCurrentFieldScriptActor->mCC_scriptPC;
        currentFieldActorId = iVar4;
        pCurrentFieldScriptActor = psVar5;
        pCurrentFieldEntity = psVar7;
        *puVar2 = uVar3;
        psVar6->m4_flags.m_rawFlags = *puVar1 | 0x100000;
        psVar6->m0_fieldScriptFlags.m_rawFlags |= 0x20000;
        psVar6->m4_flags.m_rawFlags = psVar6->m4_flags.m_rawFlags | 0x400;
        *piVar9 = 0xff;
    }
    currentParty[param_1] = 0xff;
    asyncPartyCharacterLoadingTable[param_1] = 0xff;
    return;
}

int getGearForCharacter(int param_1)
{
    return pKernelGameState->m26C_party[param_1].mA0_partyData_gearNum;
}

void startPartyCharacterASyncLoading(int partyCharacter, int partySlot)
{
    size_t sVar1;
    void* pvVar2;
    int iVar3;
    int iVar4;

    asyncPartyCharacterLoadingCharacterIndex = partyCharacter;
    asyncPartyCharacterLoadingIndex = partySlot;
    setCurrentDirectory(4, 0);
    if (fieldExecuteVar1 == 0) {
        waitReadCompletion(0);
    }
    iVar3 = partyCharacter + 5;
    if ((fieldMapNumber & 0xc000) == 0) {
        sVar1 = getFileSizeAligned(iVar3);
        asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = partyCharacter;

        asyncPartyCharacterLoadingBuffer.resize(sVar1);
    }
    else {
        iVar3 = getGearForCharacter(partyCharacter);
        iVar4 = iVar3 + 0x10;
        if (iVar3 == 0xff) {
            iVar4 = 0x10;
        }
        iVar3 = iVar4 + 5;
        sVar1 = getFileSizeAligned(iVar3);
        asyncPartyCharacterLoadingBuffer.resize(sVar1);
        asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = iVar4;
    }
    readFile(iVar3, asyncPartyCharacterLoadingBuffer, 0, 0x80);
    if (fieldExecuteVar1 == 0) {
        waitReadCompletion(0);
    }
    asyncLoadingVar1 = 1;
    return;
}

void setCurrentActor2DPosition(int posX, int posZ)
{
    std::array<FP_VEC4, 4> alStack136;
    std::array<SFP_VEC4, 4> auStack72;
    for (int i = 0; i < numWalkMesh; i++)
    {
        pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(posX, posZ, i, &auStack72[i], &alStack136[i]);
    }

    pCurrentFieldScriptActor->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pCurrentFieldScriptActor);
    pCurrentFieldScriptActor->m50_surfaceNormal = alStack136[pCurrentFieldScriptActor->m10_walkmeshId];


    actorArray[currentFieldActorId].mC_matrix.t[0] = posX;
    actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = posX;

    actorArray[currentFieldActorId].mC_matrix.t[1] = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;
    actorArray[currentFieldActorId].m2C_matrixBackup.t[1] = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

    actorArray[currentFieldActorId].mC_matrix.t[2] = posZ;
    actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = posZ;

    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m84_maxY = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

    pCurrentFieldScriptActor->m20_position.vx = posX << 16;
    pCurrentFieldScriptActor->m20_position.vz = posZ << 16;
    pCurrentFieldScriptActor->m20_position.vy = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy << 16;
    pCurrentFieldScriptActor->m72_elevation = auStack72[pCurrentFieldScriptActor->m10_walkmeshId].vy;

    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = pCurrentFieldScriptActor->m20_position.vx;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = pCurrentFieldScriptActor->m20_position.vy;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = pCurrentFieldScriptActor->m20_position.vz;

    pCurrentFieldScriptActor->m40.vx = 0;
    pCurrentFieldScriptActor->m40.vy = 0;
    pCurrentFieldScriptActor->m40.vz = 0;

    pCurrentFieldScriptActor->m30_stepVector.vx = 0;
    pCurrentFieldScriptActor->m30_stepVector.vy = 0;
    pCurrentFieldScriptActor->m30_stepVector.vz = 0;

    pCurrentFieldScriptActor->mD0_targetPositionOffset.vx = 0;
    pCurrentFieldScriptActor->mD0_targetPositionOffset.vy = 0;
    pCurrentFieldScriptActor->mD0_targetPositionOffset.vz = 0;

    pCurrentFieldScriptActor->m60[0] = 0;
    pCurrentFieldScriptActor->m60[1] = 0;
    pCurrentFieldScriptActor->m60[2] = 0;

    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vx = 0;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vy = 0;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vz = 0;

    pCurrentFieldScriptActor->mF0 = 0;
    pCurrentFieldScriptActor->mEC_elevation = 0;
    pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy >> 0x10;
    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags = (pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & ~0x40001) | 0x400000;
}


const std::array<u16, 8> actorDirectionTable = {
    0x8C00,  0x8E00,  0x8000,  0x8200,
    0x8400,  0x8600,  0x8800,  0x8A00
};

const std::array<u16, 8> actorDirectionTable2 = {
    0x8C00,  0x8E00,  0x8000,  0x8200,
    0x8400,  0x8600,  0x8800,  0x8A00
};

const std::array<u16, 8> actorDirectionTable3 = {
    0x8C00,  0x8400,  0x8800,  0x8000,
    0x8A00,  0x8E00,  0x8600,  0x8200
};

int getCharacter(int param_1)
{
    int iVar1;

    iVar1 = currentParty[2];
    if ((((param_1 != 0xff) && (iVar1 = currentParty[1], param_1 != 0xfe)) && (iVar1 = currentParty[0], param_1 != 0xfd)) && (iVar1 = 0xff, param_1 != 0xfc)) {
        iVar1 = param_1;
    }
    return iVar1;
}

s32 findCharacterInParty(int param_1)
{
    int iVar1;
    int* piVar2;

    iVar1 = 0;
    if (param_1 != 0xff) {
        do {
            if (currentParty[iVar1] == 0xff) {
                return -1;
            }
            if (currentParty[iVar1] == param_1) {
                return iVar1;
            }
            iVar1 = iVar1 + 1;
        } while (iVar1 < 3);
    }
    return -1;
}

void updateScriptActor3dRotation(int index)
{
    sFieldEntity* psVar1;
    FP_VEC4 local_20;

    psVar1 = &actorArray[index];
    local_20.vx = (long)psVar1->m4C_scriptEntity->mF4_scale3d[0];
    local_20.vy = (long)psVar1->m4C_scriptEntity->mF4_scale3d[1];
    local_20.vz = (long)psVar1->m4C_scriptEntity->mF4_scale3d[2];
    createRotationMatrix(&actorArray[index].m50_modelRotation, &actorArray[index].mC_matrix);
    ScaleMatrix(&actorArray[index].mC_matrix, &local_20);
}

int spriteWalkToPositionOrActor(int param_1)
{
    if ((actorArray[currentFieldActorId].m4C_scriptEntity->m4_flags.m_rawFlags & 0x2000) == 0)
    {
        if (actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18_moveSpeed == 0)
        {
            actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18_moveSpeed = 0x4000000 / pCurrentFieldScriptActor->m76;
        }
    }
    else
    {
        actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18_moveSpeed = 0x8000000 / pCurrentFieldScriptActor->m76;
    }

    int destinationX;
    int destinationZ;

    int lVar13 = 0;
    int lVar14 = 0;

    s32 stepLength = length1d(actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18_moveSpeed >> 15);

    s32 currentX = (pCurrentFieldScriptActor->m20_position.vx >> 16);
    s32 currentZ = (pCurrentFieldScriptActor->m20_position.vz >> 16);

    switch (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode)
    {
    case 0:
        destinationX = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
        destinationZ = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
        break;
    case 1:
        destinationX = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
        destinationZ = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[2];
        break;
    case 2:
        if (readCharacter(1) == 0xFF)
        {
            return 0;
        }
        lVar14 = length1d(actorArray[readCharacter(1)].m4C_scriptEntity->m1E_collisionRadius + pCurrentFieldScriptActor->m1E_collisionRadius);
        destinationX = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vx >> 16;
        destinationZ = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vz >> 16;
        if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == playerControlledActor)
        {
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x200000;
        }
        break;
    case 3:
        destinationX = pCurrentFieldScriptActor->mD0_targetPositionOffset[0] + ((getAngleSin(getImmediateOrVariableUnsigned(1) & 0xfff) << 0xc) >> 0xc);
        destinationZ = pCurrentFieldScriptActor->mD0_targetPositionOffset[2] + (getAngleCos(getImmediateOrVariableUnsigned(1) & 0xfff) * -0x1000 >> 0xc);
        break;
    default:
        assert(0);
    }

    FP_VEC3 travelDelta;
    travelDelta[0] = destinationX - currentX;
    travelDelta[1] = 0;
    travelDelta[2] = destinationZ - currentZ;

    pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags |= 0x400000;

    // Reached destination yet?
    s32 distanceToDestination = length2d(travelDelta[0], travelDelta[2]);

    if ((pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0) ||
        (distanceToDestination <= (stepLength + 1 + lVar14)))
    {
        //reached location
        if (param_1 == 0)
        {
            pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(travelDelta);
        }
        else
        {
            if ((pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags & 0x8000) == 0)
            {
                pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation | 0x8000;
            }
            else
            {
                pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m11C | 0x8000;
            }
        }

        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xFFFF;
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.mx800_isJumping = 0;
        pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x2200000;
        return 0;
    }
    else
    {
        pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0--;
        pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(travelDelta);
        breakCurrentScript = 1;
        return -1;
    }
}

void resetFieldScriptActor()
{
    breakCurrentScript = 1;
    pCurrentFieldScriptActor->m30_stepVector.vx = 0;
    pCurrentFieldScriptActor->m30_stepVector.vy = 0;
    pCurrentFieldScriptActor->m30_stepVector.vz = 0;
    pCurrentFieldScriptActor->m40.vx = 0;
    pCurrentFieldScriptActor->m40.vy = 0;
    pCurrentFieldScriptActor->m40.vz = 0;
    pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation | 0x8000;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vx = 0;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC_step.vz = 0;
    actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18_moveSpeed = 0;
}

s32 opA0_var0 = 0;
s32 opA0_var1 = 0;

FP_VEC3 desiredCameraPosition = { 0,0,0 };
FP_VEC3 cameraTargetOverride = { 0,0,0 };
FP_VEC3 desiredCameraTarget = { 0,0,0 };
FP_VEC3 cameraPositionOverride = { 0,0,0 };
FP_VEC3 cameraInterpolationTargetStep = { 0,0,0 };
FP_VEC3 cameraInterpolationTargetStartPosition = { 0,0,0 };
FP_VEC3 cameraInterpolationPositionStep = { 0,0,0 };
FP_VEC3 cameraInterpolationStartPosition = { 0,0,0 };
FP_VEC4 cameraEye;
FP_VEC4 cameraAt;
FP_VEC4 cameraEye2;
FP_VEC4 cameraAt2;
u16 cameraInterpolationFlags = 0;
s16 cameraInterpolationTargetNumSteps = 0;
s32 cameraInterpolationPositionNumSteps = 0;

s16 camera2Tan = 0;
void setCurrentActorRotation2(s16 param_1)
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;

    psVar2 = pCurrentFieldScriptActor;
    iVar1 = fieldExecuteVar1;
    uVar3 = param_1 - camera2Tan & 0xfffU | 0x8000;
    pCurrentFieldScriptActor->m104_rotation = uVar3;
    psVar2->m106_currentRotation = uVar3;
    if (iVar1 == 0) {
        psVar2->m108_rotation3 = uVar3;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int getCurrentActorRotationIn8Cardinal()
{
    return ((pCurrentFieldScriptActor->m106_currentRotation + 0x100) >> 9) + 2U & 7;
}

void setCurrentActorTargetRotation(s16 param_1)
{
    s16 angle = param_1 | 0x8000;
    if (fieldExecuteVar1 == 0) {
        pCurrentFieldScriptActor->m104_rotation = angle;
        pCurrentFieldScriptActor->m106_currentRotation = angle;
        pCurrentFieldScriptActor->m108_rotation3 = angle;
    }
    pCurrentFieldScriptActor->m104_rotation = angle;
    pCurrentFieldScriptActor->m106_currentRotation = angle;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int playMusicAuthorized = 0;
int musicVar1 = -1;
int musicVar2 = 0;
int currentlyPlayingMusic = 255;

void clearMusic()
{
    MissingCode();
}

void clearMusic2()
{
    MissingCode();
}

int isLoadCompleted()
{
    MissingCode();
    return 0;
}

void playMusic(int musicId, int)
{
    MissingCode();
}

s32 fieldExecuteVar2 = 0;
s32 fieldExecuteVar3 = 0;
s16 fieldRandomBattleVar = 0;


void setCurrentActorElevation(short param_1)
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    (pCurrentFieldScriptActor->m20_position).vy = (uint)param_1 << 0x10;
    psVar1->mEC_elevation = param_1;
    psVar1->m72_elevation = param_1;
    return;
}

void OPX_45()
{
    byte bVar1;
    sFieldScriptEntity* psVar2;

    psVar2 = pCurrentFieldScriptActor;
    bVar1 = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1];
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    psVar2->mE6 = (ushort)bVar1;
}

void OPX_47()
{
    fieldObjectRenderingVar2.pad = getImmediateOrVariableUnsigned(1);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

s32 load2dAnimVar = 0;
s32 loadCompleted = 0;

void setupSpecialAnimation(sSpriteActorCore* param_1, sSpriteActorAnimationBundle* param_2)
{
    param_1->m4C_specialAnimation = param_2;
    return;
}

void updateGearState(int param_1)
{
    ushort uVar2;
    int uVar1;

    uVar2 = (ushort)fieldMapNumber;
    if (param_1 == 1) {
        setVar(0x30, uVar2 & 0xfff);
        setVar(0x32, 0);
        uVar1 = 0x34;
    }
    else {
        if (param_1 < 2) {
            if (param_1 != 0) {
                return;
            }
            setVar(0x2a, uVar2 & 0xfff);
            setVar(0x2c, 0);
            uVar1 = 0x2e;
        }
        else {
            if (param_1 != 2) {
                return;
            }
            setVar(0x36, uVar2 & 0xfff);
            setVar(0x38, 0);
            uVar1 = 0x3a;
        }
    }
    setVar(uVar1, 0);
    return;
}

u8 menuDisabled = 0;
u8 compassDisabled = 0;
u16 OPX_80Params[8] = { 0,0,0,0,0,0,0,0 };
s32 OPX_81Params[3] = { 0,0,0 };
std::array<sColor, 3> OPX_82Param0 = { {{0,0,0},{0,0,0},{0,0,0} } };
s16 OPX_82Param3[3] = { 0,0,0 };
s16 OPX_82Param4 = 0;
s32 OPX_86Param = 0;
s32 OPX8E_param0 = 0;
s32 OPX8E_param1 = 0;

int getCurrentDiscNumber()
{
    MissingCode();
    return 0;
}



s8 pauseDisabled = 0;

void OPX_13Sub(int)
{
    MissingCode();
}


std::array<sGameController, 2> padButtonForScripts;


int isScriptAlreadyRunning(sFieldScriptEntity* pEntity, int scriptIndex)
{
    for (int i = 0; i < 8; i++)
    {
        if (pEntity->m8C_scriptSlots[i].m3_scriptIndex == scriptIndex)
        {
            return -1;
        }
    }
    return 0;
}


void setupRGBCalcMode(int, int, int, int, int, int)
{
    MissingCode();
}

void executeFieldScript(int param)
{
    if (!g_executeScripts)
    {
        return;
    }
    breakCurrentScript = 0;
    fieldExectuteMaxCycles = param;
    int cycles = 0;
    if (param > 0)
    {
        while (cycles <= 0x400)
        {
            //printf("PC: 0x%04X ", pCurrentFieldScriptActor->mCC_scriptPC);
            u8 opcodeId = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC];

            LogOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, opcodeId);

            if (fieldScriptOpcodes[opcodeId] == nullptr)
            {
                LogMissingOpcode(currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, opcodeId);
                breakCurrentScript = 1;
                return;
            }

            //_CrtCheckMemory();
            fieldScriptOpcodes[opcodeId]();
            //_CrtCheckMemory();

            if (currentScriptFinished == 0) {
                fieldExectuteMaxCycles = 0xffff;
            }
            if (fieldExecuteVar1 != 0) {
                if (fieldExecuteVar2 == 0) {
                    return;
                }
                if (fieldExecuteVar3 == 0) {
                    return;
                }
                if (fieldChangePrevented == 0) {
                    return;
                }
            }
            if ((breakCurrentScript == 1) && (currentScriptFinished == 1)) {
                return;
            }

            cycles++;
            if (cycles >= fieldExectuteMaxCycles)
            {
                return;
            }
        }

        if (fieldDebugDisable == 0) {
            sprintf_screen("EVENTLOOP ERROR ACT=%d\n", currentFieldActorId);
        }
    }
}

void startAllEntityScripts()
{
    if (fieldScriptEntityAlreadyInitialized == 0)
    {
        setVar(0x10, 0);
        setVarsForCurrentParty();

        for (int i = 0; i < totalActors; i++)
        {
            // that was probably some macro
            pCurrentFieldEntity = &actorArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            currentFieldActorId = i;
            pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 2); // the update script

            // Does the entry point have any code?
            if ((READ_LE_U16(pCurrentFieldScriptFile + pCurrentFieldScriptActor->mCC_scriptPC) == 0))
            {
                pCurrentFieldScriptActor->m4_flags.m_rawFlags |= 0x4000000;
            }

            // again, this time for the init script
            pCurrentFieldEntity = &actorArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            currentFieldActorId = i;
            pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 0); // the init script

        }

        // execute the init script
        for (int i = 0; i < totalActors; i++)
        {
            pCurrentFieldEntity = &actorArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            fieldScriptInitVar0 = 0;
            currentScriptFinished = 0;
            currentFieldActorId = i;

            executeFieldScript(0xFFFF);

            if (fieldScriptInitVar0 == 0)
            {
                OP_INIT_ENTITY_SCRIPT_sub0(i, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 0);
                pCurrentFieldScriptActor->m4_flags.m_x800 = 1;
            }
        }
    }
    else
    {
        assert(0);
    }
}

struct sShapeTransfert
{
    RECT m0_rect;
    std::optional<std::span<u8>::iterator> m8_pData;
    sShapeTransfert* mC_pNext;
    // size 0x10
};

int shapeTransfertTableSize = 0;
u8* shapeTransfertBuffer[2] = { nullptr, nullptr };
u8* shapeTransfertTemporaryBuffersLinkedLists[2] = { nullptr, nullptr };

u8* shapeTransfertTableCurrentEntry;
int shapeTransfertDoubleBufferIndex = 0;
u8* shapeTransfertTableStart;
u8* shapeTransfertTableEnd;
std::array<u8*, 2>shapeTransfertTable;

void resetSpriteTransfertList()
{
    spriteTransfertListHead = nullptr;
}

void allocateShapeTransfert(int param_1)
{
    shapeTransfertTableSize = param_1;
    shapeTransfertBuffer[0] = new u8[param_1 * 2];
    shapeTransfertBuffer[1] = shapeTransfertBuffer[0] + param_1;
    shapeTransfertTemporaryBuffersLinkedLists[1] = nullptr;
    shapeTransfertTemporaryBuffersLinkedLists[0] = nullptr;
    shapeTransfertTable[0] = nullptr;
    resetSpriteTransfertList();
    return;
}

void addToShapeTransfertTable(std::optional<std::span<u8>::iterator> pData, short x, short y, short w, short h)
{
    sShapeTransfert** ppsVar1;
    int iVar2;
    sShapeTransfert* psVar3;

    psVar3 = (sShapeTransfert*)shapeTransfertTableCurrentEntry;
    iVar2 = shapeTransfertDoubleBufferIndex;
    if ((sShapeTransfert*)shapeTransfertTableCurrentEntry + 1 < (sShapeTransfert*)shapeTransfertTableEnd) {
        psVar3->m0_rect.x = x;
        psVar3->m0_rect.y = y;
        psVar3->m0_rect.w = w;
        psVar3->m0_rect.h = h;
        psVar3->m8_pData = pData;
        ppsVar1 = &psVar3->mC_pNext;
        shapeTransfertTableCurrentEntry += sizeof(sShapeTransfert);
        *ppsVar1 = (sShapeTransfert*)shapeTransfertTable[iVar2];
        shapeTransfertTable[iVar2] = (u8*)psVar3;
    }
    return;
}

struct sRGB32
{
    s32 R;
    s32 G;
    s32 B;
};

struct sScreenEffectSlot
{
    TILE m18_tile[2];
    sRGB32 m38_Current;
    sRGB32 m44_step;
    s16 m50_mode;
    s16 m52_isActive;
    s16 m54_duration;
};

std::array<sScreenEffectSlot, 2> screenEffects;

void setupRGBFaderSlot0_fadeIn(int)
{
    MissingCode();
}

void setupRGBCalcSlot0_fadeToBlack(int)
{
    MissingCode();
}

void resetRGBFaderToBlack(int index)
{
    TILE* p = &screenEffects[index].m18_tile[0];
    SetTile(p);
    SetSemiTrans(p, 1);
    screenEffects[index].m18_tile[0].wh.vx = 0x140;
    screenEffects[index].m18_tile[0].wh.vy = 0xe0;
    screenEffects[index].m18_tile[0].x0y0.vx = 0;
    screenEffects[index].m18_tile[0].x0y0.vy = 0;
    screenEffects[index].m18_tile[1] = screenEffects[index].m18_tile[0];

    screenEffects[index].m52_isActive = 0;
    screenEffects[index].m54_duration = 0;
    screenEffects[index].m38_Current.B = 0;
    screenEffects[index].m38_Current.G = 0;
    screenEffects[index].m38_Current.R = 0;
    screenEffects[index].m50_mode = 2;
}

void resetAllRGBFadersToBlack()
{
    resetRGBFaderToBlack(0);
    resetRGBFaderToBlack(1);
}

void flagAllocation(struct sLoadableData&) {
    MissingCode();
}

void unflagAllocation(struct sLoadableData&) {
    MissingCode();
}

void flagAllocation(std::vector<u8>&)
{
    MissingCode();
}

void unflagAllocation(std::vector<u8>&)
{
    MissingCode();
}

void setupObjectRenderModesSub0(sFieldEntitySub0* param_1)
{
    short sVar1;
    short sVar2;
    sModelBlock* psVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar7;

    psVar3 = &(*param_1->m4_pModelBlock);
    sVar1 = psVar3->m22;
    sVar2 = psVar3->m24;
    iVar6 = (int)psVar3->m28 - (int)psVar3->m20;
    iVar5 = (int)psVar3->m2A - (int)sVar1;
    iVar4 = (int)psVar3->m2C - (int)sVar2;
    iVar7 = iVar6;
    if (iVar6 < iVar5) {
        iVar7 = iVar5;
    }
    if (iVar7 < iVar4) {
        iVar7 = iVar4;
    }
    param_1->m18.vx = (short)(iVar6 / 2) + psVar3->m20;
    param_1->m18.vy = (short)(iVar5 / 2) + sVar1;
    param_1->m18.vz = (short)(iVar4 / 2) + sVar2;
    param_1->m20 = (short)iVar7 * 2 + 1;
}

void setupObjectRenderModes()
{
    for (int i = 0; i < totalObjects; i++)
    {
        sFieldEntity* pFieldEntity = &actorArray[i];
        if ((pFieldEntity->m58_flags & 0x40) == 0) {
            setupObjectRenderModesSub0(pFieldEntity->m0);
            if (isFogSetup == 0) {
                if ((pFieldEntity->m58_flags & 0xc) == 0) {
                    if ((pFieldEntity->m58_flags & 0x4000) == 0) {
                        if ((pFieldEntity->m58_flags & 0x10) == 0) {
                            pFieldEntity->m0->m12_renderMode = 0;
                        }
                        else {
                            pFieldEntity->m0->m12_renderMode = 2;
                        }
                    }
                    else {
                        pFieldEntity->m0->m12_renderMode = 3;
                    }
                }
                else {
                    pFieldEntity->m0->m12_renderMode = 1;
                }
            }
            else {
                if ((pFieldEntity->m58_flags & 0x10) == 0) {
                    pFieldEntity->m0->m12_renderMode = 4;
                }
                else {
                    pFieldEntity->m0->m12_renderMode = 5;
                }
            }
        }
    }
}

int shoudlGroundOTBeEnabled()
{
    int iVar1;
    std::vector<sFieldEntity>::iterator psVar2;

    iVar1 = 0;
    psVar2 = actorArray.begin();
    if (0 < totalObjects) {
        do {
            if (((psVar2->m58_flags & 0x40) == 0) && ((psVar2->m58_flags & 0x8000) != 0)) {
                return 1;
            }
            iVar1 = iVar1 + 1;
            psVar2 = psVar2 + 1;
        } while (iVar1 < totalObjects);
    }
    return 0;
}

struct sBackgroundPoly {
    std::array<std::array<POLY_FT4, 8>, 2> m0_FT4;
    std::array<POLY_F4, 4> m280_F4;
    std::array<POLY_G4, 2> m2E0_G4;
    s32 m328;
    s32 m32C;
    s32 m330;
    s16 m334;
    s16 m336;
    s16 m338;
    s16 m33A;
    s16 m33C;
    s16 m33E;
    s16 m340;
    s16 m342;
    s16 m344;
    s16 m346;
    s16 m348;
    s16 m34A;
} *backgroundPoly = nullptr;

sBackgroundPoly* createBackgroundPoly(s32 param_1, int param_2, s32 param_3, s32 param_4, int clutX, int clutY, short param_7, int param_8, s32* param_9, std::array<sColor, 3>* param_10, u16 param_11, s16 param_12, s16 param_13)
{
    sBackgroundPoly* pBackgroundPoly = new sBackgroundPoly;

    pBackgroundPoly->m328 = param_3;
    pBackgroundPoly->m32C = param_4;
    pBackgroundPoly->m33C = param_9[0];
    pBackgroundPoly->m33E = param_9[1];
    pBackgroundPoly->m346 = param_11;
    pBackgroundPoly->m348 = param_12;
    pBackgroundPoly->m34A = param_13;
    pBackgroundPoly->m340 = param_9[2];
    if (param_9[2] < 0) {
        pBackgroundPoly->m330 = -param_8;
    }
    else {
        pBackgroundPoly->m330 = param_8;
    }
    pBackgroundPoly->m336 = param_2;
    pBackgroundPoly->m334 = param_1;
    pBackgroundPoly->m338 = param_7;
    if (param_2 < 0) {
        param_2 = param_2 + 0xff;
    }
    pBackgroundPoly->m33A = param_2 + (short)((uint)param_2 >> 8) * -0x100;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            SetPolyFT4(&pBackgroundPoly->m0_FT4[i][j]);
            SetShadeTex(&pBackgroundPoly->m0_FT4[i][j], 1);
            pBackgroundPoly->m0_FT4[i][j].clut = GetClut(clutX, clutY);
        }
    }

    if (param_10 == nullptr) {
        pBackgroundPoly->m344 = 0;
    }
    else {
        pBackgroundPoly->m344 = 1;
        for (int i = 0; i < 2; i++) {
            SetPolyF4(&pBackgroundPoly->m280_F4[i]);
            pBackgroundPoly->m280_F4[i].r0 = (*param_10)[0].m0_R;
            pBackgroundPoly->m280_F4[i].g0 = (*param_10)[0].m1_G;
            pBackgroundPoly->m280_F4[i].b0 = (*param_10)[0].m2_B;

            pBackgroundPoly->m280_F4[i].x0y0.set(0, 0);
            pBackgroundPoly->m280_F4[i].x1y1.set(320, 0);
            pBackgroundPoly->m280_F4[i].x2y2.set(0, 0);
            pBackgroundPoly->m280_F4[i].x3y3.set(320, 0);
        }

        for (int i = 0; i < 2; i++) {
            SetPolyG4(&pBackgroundPoly->m2E0_G4[i]);
            pBackgroundPoly->m2E0_G4[i].r0 = (*param_10)[1].m0_R;
            pBackgroundPoly->m2E0_G4[i].g0 = (*param_10)[1].m1_G;
            pBackgroundPoly->m2E0_G4[i].b0 = (*param_10)[1].m2_B;
            pBackgroundPoly->m2E0_G4[i].r1 = (*param_10)[1].m0_R;
            pBackgroundPoly->m2E0_G4[i].g1 = (*param_10)[1].m1_G;
            pBackgroundPoly->m2E0_G4[i].b1 = (*param_10)[1].m2_B;
            pBackgroundPoly->m2E0_G4[i].r2 = (*param_10)[2].m0_R;
            pBackgroundPoly->m2E0_G4[i].g2 = (*param_10)[2].m1_G;
            pBackgroundPoly->m2E0_G4[i].b2 = (*param_10)[2].m2_B;
            pBackgroundPoly->m2E0_G4[i].r3 = (*param_10)[2].m0_R;
            pBackgroundPoly->m2E0_G4[i].g3 = (*param_10)[2].m1_G;
            pBackgroundPoly->m2E0_G4[i].b3 = (*param_10)[2].m2_B;
            pBackgroundPoly->m2E0_G4[i].x0y0.set(0, 0);
            pBackgroundPoly->m2E0_G4[i].x1y1.set(320, 0);
            pBackgroundPoly->m2E0_G4[i].x2y2.set(0, 0);
            pBackgroundPoly->m2E0_G4[i].x3y3.set(320, 0);
        }

        for (int i = 2; i < 4; i++) {
            SetPolyF4(&pBackgroundPoly->m280_F4[i]);
            pBackgroundPoly->m280_F4[i].r0 = (*param_10)[2].m0_R;
            pBackgroundPoly->m280_F4[i].g0 = (*param_10)[2].m1_G;
            pBackgroundPoly->m280_F4[i].b0 = (*param_10)[2].m2_B;

            pBackgroundPoly->m280_F4[i].x0y0.set(0, 0);
            pBackgroundPoly->m280_F4[i].x1y1.set(320, 0);
            pBackgroundPoly->m280_F4[i].x2y2.set(0, 240);
            pBackgroundPoly->m280_F4[i].x3y3.set(320, 240);
        }
    }

    return pBackgroundPoly;
}

void computeBackgroundPoly(sBackgroundPoly* pBackgroundPoly, int param_2, int param_3, int param_4, sTag* pTag, uint oddOrEven)
{
    s32 iVar15 = pBackgroundPoly->m328;
    s32 iVar17 = param_4 + 0x100;
    if (iVar17 == 0) {
        assert(0);
    }
    if ((iVar17 == -1) && (iVar15 << 8 == -(s32)0x80000000)) {
        assert(0);
    }
    s32 iVar10 = (0x140 - (iVar15 << 8) / iVar17) / 2;
    s32 iVar23 = iVar10 * param_4;
    if (iVar23 < 0) {
        iVar23 = iVar23 + 0xff;
    }
    iVar23 = (param_2 - (iVar10 + (iVar23 >> 8))) * 0x10000 >> 0x10;
    iVar10 = iVar23 % iVar15;
    if (iVar15 == 0) {
        assert(0);
    }
    if ((iVar15 == -1) && (iVar23 == -(s32)0x80000000)) {
        assert(0);
    }
    if (iVar10 << 0x10 < 0) {
        iVar10 = (uint) * (ushort*)&pBackgroundPoly->m328 + iVar10;
    }
    iVar15 = 0;
    if ((-1 < param_3) && (iVar23 = pBackgroundPoly->m32C << 8, param_3 <= pBackgroundPoly->m32C + 0xf0)) {
        iVar15 = iVar23 / iVar17;
        if (iVar17 == 0) {
            assert(0);
        }
        if ((iVar17 == -1) && (iVar23 == -(s32)0x80000000)) {
            assert(0);
        }
    }

    s32 currentX = 0;
    std::array<POLY_FT4, 8>& currentArray = pBackgroundPoly->m0_FT4[oddOrEven & 1];
    if (0 < iVar15 << 0x10) {
        s32 iVar11 = (int)pBackgroundPoly->m334;
        s32 iVar21 = iVar11;
        if (iVar11 < 0) {
            iVar21 = iVar11 + 0x3f;
        }
        s32 iVar20 = param_4 + 0x100;
        s32 sVar4 = pBackgroundPoly->m338;

        for (int i = 0; i < 8; i++) {

            POLY_FT4& p = currentArray[i];

            u16* puVar19 = &(currentArray)[i].tpage;

            s32 sVar16 = (short)iVar10;
            s32 sVar5 = pBackgroundPoly->m338;
            s32 uVar6 = pBackgroundPoly->m334;
            s32 uVar12 = iVar10 + ((iVar11 + (iVar21 >> 6) * -0x40) << (2U - (int)sVar4 & 0x1f) & 0xffffU) & (0x100 >> ((int)sVar5 & 0x1fU)) - 1U;
            s32 iVar8 = 0x100 - uVar12;
            if (pBackgroundPoly->m328 < (int)sVar16 + (iVar8 * 0x10000 >> 0x10)) {
                iVar8 = (uint) * (ushort*)&pBackgroundPoly->m328 - iVar10;
            }
            s32 sVar7 = (short)iVar8;
            iVar10 = (iVar8 << 0x10) >> 8;
            iVar8 = iVar10 / iVar20;
            if (iVar20 == 0) {
                assert(0);
            }
            if ((iVar20 == -1) && (iVar10 == -(s32)0x80000000)) {
                assert(0);
            }
            s32 uVar9 = currentX;
            if (0x140 < (int)(short)uVar9 + (int)(short)iVar8) {
                iVar8 = 320 - currentX;
                iVar10 = (iVar8 * 0x10000 >> 0x10) * iVar20;
                if (iVar10 < 0) {
                    iVar10 = iVar10 + 0xff;
                }
                sVar7 = (short)((uint)iVar10 >> 8);
            }
            s32 iVar13 = pBackgroundPoly->m328;
            iVar10 = (int)(short)(sVar16 + sVar7) % iVar13;
            if (iVar13 == 0) {
                assert(0);
            }
            if ((iVar13 == -1) && ((short)(sVar16 + sVar7) == -(s32)0x80000000)) {
                assert(0);
            }
            currentX = currentX + iVar8;


            p.x0y0.vx = uVar9;
            p.x1y1.vx = currentX;
            p.x2y2.vx = uVar9;
            uVar9 = (s16)param_3;
            p.x2y2.vy = uVar9;
            p.x3y3.vx = (s16)currentX;
            p.x3y3.vy = uVar9;
            s32 uVar18 = (u8)uVar12;
            p.u0 = uVar18;
            uVar9 = uVar9 - (short)iVar15;
            p.x0y0.vy = uVar9;
            p.x1y1.vy = uVar9;
            u8 uVar1 = *(u8*)&pBackgroundPoly->m33A;
            s32 uVar14 = (char)sVar7 + uVar18 + 0xff;
            p.u1 = uVar14;
            p.v0 = uVar1;
            uVar1 = *(u8*)&pBackgroundPoly->m33A;
            p.u2 = uVar18;
            p.v1 = uVar1;
            s8 cVar2 = *(char*)&pBackgroundPoly->m33A;
            s8 cVar3 = *(char*)&pBackgroundPoly->m32C;
            *(u8*)(puVar19 + 7) = uVar14;
            p.v2 = cVar2 + cVar3;
            iVar8 = (int)(((uint)uVar6 + ((int)sVar16 >> (2U - (int)sVar5 & 0x1f))) * 0x10000) >> 0x10;
            p.v3 = *(char*)&pBackgroundPoly->m33A + *(char*)&pBackgroundPoly->m32C;
            if (iVar8 < 0) {
                iVar8 = iVar8 + 0x3f;
            }
            iVar13 = (int)pBackgroundPoly->m336;
            if (iVar13 < 0) {
                iVar13 = iVar13 + 0xff;
            }
            p.tpage = GetTPage(pBackgroundPoly->m338, 0, (iVar8 >> 6) << 6, (iVar13 >> 8) << 8);

            p.m0_pNext = pTag->m0_pNext;
            pTag->m0_pNext = &p;

            if (319 < currentX * 0x10000 >> 0x10) {
                return;
            }
        }
    }
}

void renderBackgroundPoly(sBackgroundPoly* pBackgroundPoly, SVECTOR* eye, SVECTOR* at, MATRIX* projectionMatrix, sTag* pTag, int oddOrEven)
{
    if (pBackgroundPoly == nullptr) {
        return;
    }

    FP_VEC4 viewDir;
    viewDir.vy = 0;
    viewDir.vx = (int)at->vx - (int)eye->vx;
    viewDir.vz = (int)at->vz - (int)eye->vz;

    SFP_VEC4 viewDirNormalized;
    VectorNormalS(&viewDir, &viewDirNormalized);

    SVECTOR local_48;
    {
        s32 iVar5 = viewDirNormalized.vx * pBackgroundPoly->m340;
        if (iVar5 < 0) iVar5 += 0xFFF;
        local_48.vx = (iVar5 >> 0xC) + at->vx;
        local_48.vy = pBackgroundPoly->m33E;
        iVar5 = viewDirNormalized.vz * pBackgroundPoly->m340;
        if (iVar5 < 0) iVar5 += 0xFFF;
        local_48.vz = (iVar5 >> 0xC) + at->vz;
    }

    SetRotMatrix(projectionMatrix);
    SetTransMatrix(projectionMatrix);
    gte_ldv0(&local_48);
    gte_rtps();

    sVec2_s16 projectedPosition;
    gte_stsxy(&projectedPosition);

    s32 sVar4 = 0;
    if (pBackgroundPoly->m348) {
        assert(0);
    }

    s32 iVar5 = pBackgroundPoly->m328 * pBackgroundPoly->m330 * (ratan2(viewDir.vx, viewDir.vz) & 0xfff);
    if (iVar5 < 0) {
        iVar5 = iVar5 + 0xfff;
    }
    computeBackgroundPoly(pBackgroundPoly, iVar5 >> 0xc, (int)projectedPosition.vy, (int)sVar4, pTag, oddOrEven);

    if (pBackgroundPoly->m344 < 1) {
        return;
    }

    projectedPosition.vx = projectedPosition.vy - pBackgroundPoly->m32C;
    if (0xf0 < projectedPosition.vx * 0x10000 >> 0x10) {
        projectedPosition.vx = 0xf0;
    }
    if (0 < projectedPosition.vx) {
        pBackgroundPoly->m280_F4[oddOrEven].x2y2.vy = projectedPosition.vx;
        pBackgroundPoly->m280_F4[oddOrEven].x3y3.vy = projectedPosition.vx;

        pBackgroundPoly->m280_F4[oddOrEven].m0_pNext = pTag->m0_pNext;
        pTag->m0_pNext = &pBackgroundPoly->m280_F4[oddOrEven];
    }
}

void renderBackgroundPolyIfEnabled() {
    if ((OPX_82Param4 != 0) && (menuFadeState == 0)) {
        SVECTOR eyePos;
        SVECTOR eyeAt;

        eyePos.vx = cameraEye[0].getIntegerPart();
        eyePos.vy = cameraEye[1].getIntegerPart();
        eyePos.vz = cameraEye[2].getIntegerPart();
        eyeAt.vx = cameraAt[0].getIntegerPart();
        eyeAt.vy = cameraAt[1].getIntegerPart();
        eyeAt.vz = cameraAt[2].getIntegerPart();
        renderBackgroundPoly(backgroundPoly, &eyePos, &eyeAt, &currentProjectionMatrix, &pCurrentFieldRenderingContext->m40D0_secondaryOT[linkOTIndex-1], g_frameOddOrEven);
    }
}

void initFieldData()
{
    resetFieldDefault();

    for (int i = 0; i < 32; i++)
    {
        fieldVramMapping[i].m0_vramX = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 0);
        fieldVramMapping[i].m2_vramY = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 2);
        fieldVramMapping[i].m4 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 4);
        fieldVramMapping[i].m6 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 6);
    }

    MissingCode();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            initCompassPoly(&fieldCompassVar2[i * 4 + j], j, i, 0); // main compass
        }
    }

    initCompassPoly(&fieldCompassVar2[0x10], 4, 4, 1); // letters
    initCompassPoly(&fieldCompassVar2[0x11], 5, 5, 1);
    initCompassPoly(&fieldCompassVar2[0x12], 6, 6, 1);
    initCompassPoly(&fieldCompassVar2[0x13], 7, 7, 1);

    initCompassPoly(&fieldCompassVar2[0x14], 8, 8, 1); // arrow

    initCompassShadowPoly();

    {
        {

            int rawFieldImageBundleSize = READ_LE_U32(&rawFieldBundle[0x10C]);
            rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
            fieldDecompress(rawFieldImageBundleSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x130]), rawFieldImageBundle);

            int numImages = READ_LE_U32(rawFieldImageBundle.begin());
            for (int i = 0; i < numImages; i++)
            {
                int offset = READ_LE_U32(rawFieldImageBundle.begin() + i * 4);
                uploadFieldImages(rawFieldImageBundle.begin() + offset);
            }
        }

        {
            int rawFieldImageBundle2Size = READ_LE_U32(&rawFieldBundle[0x11C]);
            rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
            fieldDecompress(rawFieldImageBundle2Size + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x140]), rawFieldImageBundle2);

            int numImages = READ_LE_U32(rawFieldImageBundle2.begin());
            for (int i = 0; i < numImages; i++)
            {
                int offset = READ_LE_U32(rawFieldImageBundle2.begin() + 4 + i * 4);

                if (fieldVramMapping[i].m6 == 0)
                {
                    int x = fieldVramMapping[i].m0_vramX;
                    int y = fieldVramMapping[i].m2_vramY;

                    uploadNpcSpriteSheet(rawFieldImageBundle2.begin() + offset, x, y);
                }

            }
        }

        DrawSync(0); // needs to be done to transfert to vram before the bundles get deallocated
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x114]);
        rawFieldModels.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x138]), rawFieldModels);

        int numModels = READ_LE_U32(rawFieldModels.begin());
        gCurrentFieldModels.clear();
        gCurrentFieldModels.resize(numModels);
        for (int i = 0; i < numModels; i++)
        {
            int offset = READ_LE_U32(rawFieldModels.begin() + 4 + i * 4);
            int nextOffset;

            if (i < numModels - 1)
            {
                nextOffset = READ_LE_U32(rawFieldModels.begin() + 4 + (i + 1) * 4);
            }
            else
            {
                nextOffset = rawFieldModels.size();
            }
            fieldModelRelocation(rawFieldModels.begin() + offset);
            gCurrentFieldModels[i].init(rawFieldModels.begin() + offset, nextOffset - offset);
        }
    }

    {
        std::vector<u8> rawEncounterData;
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x124]);
        rawEncounterData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x148]), rawEncounterData);
        int numEntries = rawFieldSize / 0x20;
        assert(numEntries == 16);

        // 16 entries of 32 bytes (battleConfigs) for every encounter at 0x800658DC
        // then 16 entries of 1 byte
        for (int i = 0; i < 0x10; i++) {            
            battleConfigs[i].init(rawEncounterData.begin() + 0x20 * i);
            encounterProbabilityWeight[i] = READ_LE_U8(rawEncounterData.begin() + i + 0x200);
        }
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x120]);
        rawFieldScriptData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x144]), rawFieldScriptData);
        totalActors = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
        pCurrentFieldScriptFile = rawFieldScriptData.begin() + 0x84 + totalActors * 0x40;
    }

    {
        std::vector<u8> rawFieldTriggerData;
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x12C]);
        rawFieldTriggerData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x150]), rawFieldTriggerData);

        fieldTriggerData.resize(rawFieldTriggerData.size() / 0x18);
        for (int i = 0; i < fieldTriggerData.size(); i++)
        {
            fieldTriggerData[i].m0[0] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x0);
            fieldTriggerData[i].m0[1] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x2);
            fieldTriggerData[i].m0[2] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x4);
            fieldTriggerData[i].m6[0] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x6);
            fieldTriggerData[i].m6[1] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x8);
            fieldTriggerData[i].m6[2] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0xA);
            fieldTriggerData[i].mC[0] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0xC);
            fieldTriggerData[i].mC[1] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0xE);
            fieldTriggerData[i].mC[2] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x10);
            fieldTriggerData[i].m12[0] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x12);
            fieldTriggerData[i].m12[1] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x14);
            fieldTriggerData[i].m12[2] = READ_LE_S16(rawFieldTriggerData.begin() + i * 0x18 + 0x16);
        }
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x128]);
        rawFieldDialogBundle.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x14C]), rawFieldDialogBundle);
    }

    {
        // TODO: clean that up!

        std::vector<u8> rawFieldWalkMesh;
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x110]);
        rawFieldWalkMesh.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x134]), rawFieldWalkMesh);
        walkMesh.init(rawFieldWalkMesh);

        std::vector<u8>::iterator walkMeshIterator = rawFieldWalkMesh.begin();

        numWalkMesh = walkMesh.m0_count;
        assert(numWalkMesh < 5);

        assert(numWalkMesh == READ_LE_U32(walkMeshIterator));
        walkMeshIterator += 4;

        walkMeshNumTrianglePerBlock.fill(0);
        for (int i = 0; i < walkMesh.m0_count; i++)
        {
            walkMeshNumTrianglePerBlock[i] = walkMesh.m_blocks[i].m_triangles.size();
        }

        walkMeshIterator += 4 * 4;

        int iVar4 = 0;
        u8* walkMeshVar1Raw = &(rawFieldWalkMesh[0]) + READ_LE_U32(walkMeshIterator);
        walkMeshIterator += 4;

        std::array<u8*, 5> walkMeshVerticesRaw;
        std::array<u8*, 5> walkMeshTriangleRaw;

        if (numWalkMesh > 0) {
            std::array<u8*, 5>::iterator piVar11 = walkMeshVerticesRaw.begin();
            std::array<u8*, 5>::iterator ppuVar8 = walkMeshTriangleRaw.begin();
            do {
                iVar4 = iVar4 + 1;
                *ppuVar8 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator);
                *piVar11 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator + 4);
                ppuVar8++;
                piVar11++;
                walkMeshIterator = walkMeshIterator + 2 * 4;
            } while (iVar4 < numWalkMesh);
        }
        walkMeshVar4 = (walkMeshTriangleRaw[0] - walkMeshVar1Raw) / 4;

        //////////////////////////////////////////////
        // Not in disassembly

        for (int i = 0; i < numWalkMesh; i++)
        {
            walkMeshTriangle[i] = &walkMesh.m_blocks[i].m_triangles;
            walkMeshVertices[i] = &walkMesh.m_blocks[i].m_vertices;
        }
        walkMeshVar1 = &walkMesh.m_materials;
    }

    {
        // TODO: rawFieldActorSetupParams should be local, but fieldActorSetupParams still points inside of it. Figure that out.

        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x118]);
        rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x13C]), rawFieldActorSetupParams);

        u32 count = READ_LE_U32(rawFieldActorSetupParams.begin());
        fieldActorSetupParams.resize(count);
        for (int i = 0; i < count; i++)
        {
            std::span<u8> tempSpan(rawFieldActorSetupParams.begin(), rawFieldActorSetupParams.size());
            fieldActorSetupParams[i].init(tempSpan.begin() + READ_LE_U32(4 + rawFieldActorSetupParams.begin() + 4 * i));
        }
    }

    cameraLimits[0] = 1;
    cameraLimits[1] = 1;
    cameraLimits[2] = 1;
    cameraLimits[3] = 1;

    setupField3d(rawFieldBundle.begin() + 0x154);

    totalObjects = READ_LE_U16(&rawFieldBundle[0x18C]);
    actorArray.resize(totalObjects);

    for (int i = 0; i < totalObjects; i++)
    {
        memset(&actorArray[i], 0, sizeof(sFieldEntity));
    }

    std::vector<u8>::iterator fieldEntitySetup = rawFieldBundle.begin() + 0x190;
    for (int i = 0; i < totalObjects; i++)
    {
        actorArray[i].m58_flags = READ_LE_U16(fieldEntitySetup);
        actorArray[i].m50_modelRotation.vx = READ_LE_S16(fieldEntitySetup + 2);
        actorArray[i].m50_modelRotation.vy = READ_LE_S16(fieldEntitySetup + 4);
        actorArray[i].m50_modelRotation.vz = READ_LE_S16(fieldEntitySetup + 6);

        actorArray[i].m2C_matrixBackup.t[0] = actorArray[i].mC_matrix.t[0] = READ_LE_S16(fieldEntitySetup + 8);
        actorArray[i].m2C_matrixBackup.t[1] = actorArray[i].mC_matrix.t[1] = READ_LE_S16(fieldEntitySetup + 10);
        actorArray[i].m2C_matrixBackup.t[2] = actorArray[i].mC_matrix.t[2] = READ_LE_S16(fieldEntitySetup + 12);

        // Is this a 3d model?
        if (!(actorArray[i].m58_flags & 0x40))
        {
            u16 modelIndex = READ_LE_U16(fieldEntitySetup + 14);

            actorArray[i].m0 = new sFieldEntitySub0;
            sModel& pModel = gCurrentFieldModels[modelIndex];
            std::vector<sModelBlock>::iterator pModelBlock = pModel.m10_blocks.begin();

            actorArray[i].m0->m4_pModelBlock = pModelBlock;

            initModel1(*pModelBlock, actorArray[i].m0->m8[0], actorArray[i].m0->m8[1]);
            initModel2(&(*actorArray[i].m0->m4_pModelBlock), actorArray[i].m0->m8[0], (actorArray[i].m58_flags & 0xC) >> 2);
            actorArray[i].m0->m8[1] = actorArray[i].m0->m8[0];
            if (actorArray[i].m58_flags & 0x2000)
            {
                resetMemoryAllocStats(3, 0);
                actorArray[i].m0->m14 = initModelDynamicVertices(&(*actorArray[i].m0->m4_pModelBlock));
                resetMemoryAllocStats(8, 0);
            }
            initModel5(&(*actorArray[i].m0->m4_pModelBlock));
        }
        else
        {
            actorArray[i].m58_flags |= 0x20;
            actorArray[i].m50_modelRotation.vx = 0;
            actorArray[i].m50_modelRotation.vy = 0;
            actorArray[i].m50_modelRotation.vz = 0;

        }

        initFieldScriptEntity(i);

        fieldEntitySetup += 16;
    }

    MissingCode();

    initDialogWindows();
    resetAllRGBFadersToBlack();

    unflagAllocation(rawFieldBundle);
    rawFieldBundleForDebug = rawFieldBundle;
    rawFieldBundle.clear();

    resetMemoryAllocStats(5, 0);

    allocateShapeTransfert(0x3c00);
    resetSpriteCallbacks();
    resetMemoryAllocStats(8, 0);
    initMechaFieldArgs2(mechaFieldArgs2[1], 0x800, 0, 0, 0x800, 0, 0, 0x800, 0, 0);
    initMechaFieldArgs2(mechaFieldArgs2[0], 0x1f8, -0xfc1, -0x1f8, 0, 0, 0, 0, 0, 0);
    mechaBackColor[2] = 0x1e;
    mechaBackColor[1] = 0x1e;
    mechaBackColor[0] = 0x1e;

    OPX_80Params[2] = 0x140;
    OPX_80Params[7] = 0;
    OPX_82Param0[2].m1_G = 0;
    OPX_82Param0[2].m0_R = 0;
    OPX_82Param0[1].m2_B = 0;
    OPX_82Param0[1].m1_G = 0;
    OPX_82Param0[1].m0_R = 0;
    OPX_82Param0[0].m2_B = 0;
    OPX_82Param0[0].m1_G = 0;
    OPX_82Param0[0].m0_R = 0;
    OPX_81Params[0] = 0;
    OPX_81Params[2] = 0x1000;
    OPX_82Param3[2] = 0;
    OPX_82Param3[1] = 0;
    OPX_82Param3[0] = 0;
    OPX_80Params[6] = 0;
    OPX_80Params[5] = 0;
    OPX_80Params[4] = 0;
    OPX_80Params[3] = 0;
    OPX_80Params[1] = 0;
    OPX_80Params[0] = 0;
    OPX_82Param4 = 0;
    OPX_81Params[1] = 0;
    OPX_82Param0[2].m2_B = 0x20;
    fieldExecuteVar1 = 0;
    startAllEntityScripts();
    fieldExecuteVar1 = 1;
    createRotationMatrix(&renderModelRotationAngles, &renderModelRotationMatrix);
    renderModelRotationMatrix.t[2] = 0;
    renderModelRotationMatrix.t[1] = 0;
    renderModelRotationMatrix.t[0] = 0;

    if (OPX_82Param4 != 0)
    {
        backgroundPoly = createBackgroundPoly
        ((int)(short)OPX_80Params[0], (int)(short)OPX_80Params[1], (int)(short)OPX_80Params[2], (int)(short)OPX_80Params[3], (int)(short)OPX_80Params[4], (int)(short)OPX_80Params[5],
            (int)(short)OPX_80Params[6], (int)(short)OPX_80Params[7], OPX_81Params, &OPX_82Param0, (int)OPX_82Param3[0], (int)OPX_82Param3[1], (int)OPX_82Param3[2]);
    }
    resetMemoryAllocStats(8, 0);

    cameraAt2.vx = actorArray[actorCameraTracked].mC_matrix.t[0] << 16;
    cameraAt2.vy = actorArray[actorCameraTracked].mC_matrix.t[1] << 16;
    cameraAt2.vz = actorArray[actorCameraTracked].mC_matrix.t[2] << 16;

    for (int i = 0; i < totalObjects; i++)
    {
        createRotationMatrix(&actorArray[i].m50_modelRotation, &actorArray[i].mC_matrix);
        actorArray[i].m2C_matrixBackup = actorArray[i].mC_matrix;
    }
    MissingCode();

    fieldIdForDebugger = currentFieldId0 / 2; // hack!

    MissingCode();
    loadMechaOVerlayAndRegisterMechas();


    MissingCode();

    currentFieldId1 = -1;
    currentFieldId0 = -1;
    setupObjectRenderModes();
    doPCCollisionCheckAfterLoading();
    int enableGroundOT = 1;
    if (OPX_82Param4 == 0) {
        enableGroundOT = shoudlGroundOTBeEnabled();
    }
    fieldUseGroundOT = enableGroundOT;

    MissingCode();

}


void loadRawFieldBundle(int fieldId)
{
    rawFieldBundle.resize(getFileSizeAligned(184 + fieldId));
    flagAllocation(rawFieldBundle);

    readFile(184 + fieldId, rawFieldBundle, 0, 0x80);
}

int loadNewField(int fieldId, int param)
{
    if ((currentFieldId1 == param) && (currentFieldId0 == fieldId))
    {
        return 0;
    }

    if (isCDBusy())
    {
        return -1;
    }

    waitReadCompletion(0);
    if (currentFieldId1 != -1)
    {
        unflagAllocation(rawFieldBundle);
        rawFieldBundleForDebug = rawFieldBundle;
        rawFieldBundle.clear();
    }
    loadRawFieldBundle(fieldId);

    currentFieldId0 = fieldId;
    currentFieldId1 = param;

    return -1;
}

void loadInitialField()
{
    waitReadCompletion(0);

    while (loadNewField((fieldMapNumber & 0xFFF) * 2, 0));
}


void* allocateBufferForVramUpload(int)
{
    MissingCode();
    return nullptr;
}

std::array<u8, 2048 * 512> gVram;
void clearModelTextureCache();

void loadImageFileToVram(int fileId, void* allocation, int, int, int, int, int, int, int, int)
{
    // this is completely different from the original code that was done async and using the cd callbacks
    MissingCode();

    // make sure we recreate all texture after loading this
    clearModelTextureCache();

    std::vector<u8> data;
    c_isoManager::getCurrentDisc()->readData(getFileStartSector(fileId), getFileSize(fileId), data);

    int offset = 0;
    while (offset < data.size() - 0x14)
    {
        std::vector<u8>::iterator texture = data.begin() + offset;
        u32 type = READ_LE_U16(texture + 0x00);
        u16 pos_x = READ_LE_U16(texture + 0x04);
        u16 pos_y = READ_LE_U16(texture + 0x06);
        u16 move_x = READ_LE_U16(texture + 0x08);
        u16 move_y = READ_LE_U16(texture + 0x0a);
        u16 width = READ_LE_U16(texture + 0x0c);
        u16 height = READ_LE_U16(texture + 0x0e);
        u16 chunks = READ_LE_U16(texture + 0x12);

        if (width > 2048 || height > 512 || width == 0 || height == 0) {
            return;
        }
        int blockSize = 0x1C + chunks * 2;
        offset += (blockSize + 2047) & ~2047;
        for (int i = 0; i < chunks; i++) {
            height = READ_LE_U16(texture + 0x1C + i * 2);
            for (int j = 0; j < height; j++) {
                memcpy(&gVram[0] + (pos_y + move_y + j) * 2048 + (pos_x + move_x) * 2, ((u8*)&data[0]) + offset + j * width * 2, width * 2);
            }
            pos_y += height;
            blockSize = width * height * 2;
            offset += (blockSize + 2047) & ~2047;
        }
    }
}

int fieldGraphicsUploaded = 0;
void loadFieldGraphics()
{
    if (!fieldGraphicsUploaded)
    {
        fieldGraphicsUploaded = 1;
        void* fieldGraphicsAllocation = allocateBufferForVramUpload(4);
        loadImageFileToVram((fieldMapNumber & 0xfff) * 2 + 0xb9, fieldGraphicsAllocation, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void setFieldDrawEnvClip(short x, short y, short w, short h)
{
    fieldRenderContext[0].m0_drawEnv.clip.x = x;
    fieldRenderContext[0].m0_drawEnv.clip.y = y;
    fieldRenderContext[0].m0_drawEnv.clip.w = w;
    fieldRenderContext[0].m0_drawEnv.clip.h = h;
    fieldRenderContext[1].m0_drawEnv.clip.x = x;
    fieldRenderContext[1].m0_drawEnv.clip.y = y + 0x100;
    fieldRenderContext[1].m0_drawEnv.clip.w = w;
    fieldRenderContext[1].m0_drawEnv.clip.h = h;
}

void setupFieldDisplayEnv(void)
{
    fieldRenderContext[0].mB8_displayEnv.screen.x = 0;
    fieldRenderContext[0].mB8_displayEnv.screen.y = 10;
    fieldRenderContext[0].mB8_displayEnv.screen.w = 0x100;
    fieldRenderContext[0].mB8_displayEnv.screen.h = 0xd8;
    fieldRenderContext[1].mB8_displayEnv.screen.x = 0;
    fieldRenderContext[1].mB8_displayEnv.screen.y = 10;
    fieldRenderContext[1].mB8_displayEnv.screen.w = 0x100;
    fieldRenderContext[1].mB8_displayEnv.screen.h = 0xd8;
}

void initFieldDrawEnvsSub0(int param_1, int param_2)
{
    screenClippingX = param_1;
    screenClippingY = (param_2 + -1) * 0x10000;
    return;
}

void initFieldDrawEnvs()
{
    fieldDrawEnvsInitialized = 1;
    DrawSync(0);
    VSync(0);
    InitGeom();
    SetGeomOffset(0xa0, 0x70);
    SetDefDrawEnv(&fieldRenderContext[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&fieldRenderContext[1].m0_drawEnv, 0, 0x100, 0x140, 0xe0);
    SetDefDrawEnv(&fieldRenderContext[0].m5C_backgroundRect, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&fieldRenderContext[1].m5C_backgroundRect, 0, 0x100, 0x140, 0xe0);
    SetDefDispEnv(&fieldRenderContext[0].mB8_displayEnv, 0, 0x100, 0x140, 0xe0);
    SetDefDispEnv(&fieldRenderContext[1].mB8_displayEnv, 0, 0, 0x140, 0xe0);
    setFieldDrawEnvClip(0, 0, 0x140, 0xe0);
    setupFieldDisplayEnv();
    fieldRenderContext[0].m0_drawEnv.r0 = 0;
    fieldRenderContext[0].m0_drawEnv.g0 = 0;
    fieldRenderContext[0].m0_drawEnv.b0 = 0;
    fieldRenderContext[1].m0_drawEnv.r0 = 0;
    fieldRenderContext[1].m0_drawEnv.g0 = 0;
    fieldRenderContext[1].m0_drawEnv.b0 = 0;
    fieldRenderContext[0].m0_drawEnv.dtd = 1;
    fieldRenderContext[1].m0_drawEnv.dtd = 1;
    VSync(0);
    PutDispEnv(&fieldRenderContext[1].mB8_displayEnv);
    PutDrawEnv(&fieldRenderContext[1].m0_drawEnv);
    initFieldDrawEnvsSub0(0x140, 0xf0);
}

void setupFieldRenderingContext(void)
{
    if (fieldDebugDisable == 0) {
        assert(0);
    }
    g_frameOddOrEven = (g_frameOddOrEven + 1) % 2;
    pCurrentFieldRenderingContext = &fieldRenderContext[g_frameOddOrEven];
    ClearOTagR(&fieldRenderContext[g_frameOddOrEven].m80D4_uiOT[0], 8);
    return;
}

void clearFieldOrderingTable()
{
    setupFieldRenderingContext();
    pCurrentFieldRenderingContext->mCC_OT.resize(4096);
    ClearOTagR(&pCurrentFieldRenderingContext->mCC_OT[0], 0x1000);
    if (fieldUseGroundOT != 0) {
        pCurrentFieldRenderingContext->m40D0_secondaryOT.resize(4096);
        ClearOTagR(&pCurrentFieldRenderingContext->m40D0_secondaryOT[0], 0x1000);
    }
}

const std::vector<u8> fontPalettes = {
    {
         0x0,  0x0, 0xBD, 0xF7,
        0x86, 0xC0, 0xBD, 0xF7,
         0x0,  0x0, 0xBD, 0xF7,
        0x86, 0xC0, 0xBD, 0xF7,
         0x0,  0x0, 0xBD, 0xF7,
        0x86, 0xC0, 0xBD, 0xF7,
         0x0,  0x0, 0xBD, 0xF7,
        0x86, 0xC0, 0xBD, 0xF7,
         0x0,  0x0,  0x0,  0x0,
         0x0,  0x0,  0x0,  0x0,
        0xBD, 0xF7, 0xBD, 0xF7,
        0xBD, 0xF7, 0xBD, 0xF7,
        0x86, 0xC0, 0x86, 0xC0,
        0x86, 0xC0, 0x86, 0xC0,
        0xBD, 0xF7, 0xBD, 0xF7,
        0xBD, 0xF7, 0xBD, 0xF7,
    }
};

void initFontPalettes(short param_1, short param_2)
{
    RECT local_18;

    local_18.w = 0x20;
    local_18.h = 1;
    local_18.x = param_1;
    local_18.y = param_2;
    LoadImage(&local_18, &fontPalettes[0]);
    textSpriteMode0 = GetClut((int)param_1, (int)param_2);
    textSpriteMode1 = GetClut(param_1 + 0x10, (int)param_2);
    return;
}

void initFontSystem()
{
    if (fieldDebugDisable == 0) {
        assert(0);
    }

    initFontPalettes(0x100, 0xf0);
}

void bootField()
{
    initFontSystem();

    MissingCode();

    setCurrentDirectory(4, 0);
    loadInitialField();

    MissingCode();

    initFieldDrawEnvs();

    MissingCode();

    g_frameOddOrEven = 1;

    MissingCode();

    clearFieldOrderingTable();

    MissingCode();

    waitReadCompletion(0);
    setCurrentDirectory(4, 0);
    initFieldData();
    loadFieldGraphics();

    MissingCode();

    if (fieldGraphicsUploaded)
    {
        MissingCode();
        fieldGraphicsUploaded = 0;
        MissingCode();
    }

    MissingCode();

    resetInputs();
    fieldMusicLoadPending = 0;

    MissingCode();
}

void allocatePartyCharacterBuffers()
{
    resetMemoryAllocStats(8, 0);

    partyCharacterBuffersRaw[0].resize(0x14000);
    partyCharacterBuffersRaw[1].resize(0x14000);
    partyCharacterBuffersRaw[2].resize(0x14000);

    flagAllocation(partyCharacterBuffersRaw[0]);
    flagAllocation(partyCharacterBuffersRaw[1]);
    flagAllocation(partyCharacterBuffersRaw[2]);
}

void waitForReadFinished(void)
{
    int iVar1;

    do {
        iVar1 = isCDBusy();
    } while (iVar1 != 0);
    waitReadCompletion(0);
    return;
}

int characterOrGearsLoadingInProgress = 0;
int fieldRequestedGears = 0;
int typeOfPlayableCharacterLoaded = 0;

void finalizeLoadPlayableCharacters()
{
    if (characterOrGearsLoadingInProgress != 0) {
        waitForReadFinished();

        for (int i = 0; i < 3; i++)
        {
            unflagAllocation(partyCharacterBuffersRaw[i]);
            if (currentParty[i] != 0xFF)
            {
                unflagAllocation(partyCharacterBuffersCompressed[i]);
                decompress(partyCharacterBuffersCompressed[i].mData.begin(), partyCharacterBuffersRaw[i].mData);
                partyCharacterBuffersCompressed[i].mData.clear();

                std::span<u8> tempSpan(partyCharacterBuffersRaw[i].mData.begin(), partyCharacterBuffersRaw[i].mData.size());
                partyCharacterBuffers[i].init(tempSpan.begin());
            }
        }

        characterOrGearsLoadingInProgress = 0;
    }
}

void refinalizePlayableCharacters(int)
{
    MissingCode();
}

sLoadingBatchCommands playableCharacterLoadingBatchCommands[11];

void setupFieldCurrentPartyFromKernelAsGears()
{
    int pCurrentPartyId = 0;
    for (int i = 0; i < 3; i++)
    {
        if (gameState.m1D34_currentParty[i] != 0xff) {
            currentParty[pCurrentPartyId++] = (uint)gameState.m1D34_currentParty[i];
        }
    }

    int indexInLoadingBatch = 0;
    for (int i = 0; i < 3; i++)
    {
        if (currentParty[i] != 0xFF)
        {
            int gearId = getGearForCharacter(currentParty[i]);
            int gearFileId = gearId + 0x10;
            if (gearId == 0xff) {
                gearFileId = 0x10;
            }
            playableCharacterLoadingBatchCommands[indexInLoadingBatch].m0_fileIndex = gearFileId;
            asyncPartyCharacterLoadingTable[indexInLoadingBatch] = gearFileId;
            partyCharacterBuffersCompressed[indexInLoadingBatch].resize(getFileSizeAligned(gearFileId + 5));
            playableCharacterLoadingBatchCommands[indexInLoadingBatch].m4_loadPtr = &partyCharacterBuffersCompressed[indexInLoadingBatch];
            flagAllocation(partyCharacterBuffersCompressed[indexInLoadingBatch]);
            indexInLoadingBatch++;
        }
    }
    playableCharacterLoadingBatchCommands[indexInLoadingBatch].m0_fileIndex = 0;
    playableCharacterLoadingBatchCommands[indexInLoadingBatch].m4_loadPtr = nullptr;

    batchStartLoadingFiles(playableCharacterLoadingBatchCommands, 0);
    typeOfPlayableCharacterLoaded = 2;

}

void setupFieldCurrentPartyFromKernelAsCharacters()
{
    pKernelGameState = &gameState;

    for (int i = 0; i < 3; i++)
    {
        currentParty[i] = 0xFF;
        if (gameState.m1D34_currentParty[i] != 0xFF)
        {
            currentParty[i] = gameState.m1D34_currentParty[i];
        }
    }

    int loadSlot = 0;
    for (int i = 0; i < 3; i++)
    {
        if (currentParty[i] != -1)
        {
            playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = currentParty[i] + 5;
            asyncPartyCharacterLoadingTable[i] = currentParty[i];
            partyCharacterBuffersCompressed[i].resize(getFileSizeAligned(currentParty[i] + 5));
            playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = &partyCharacterBuffersCompressed[i];
            flagAllocation(partyCharacterBuffersCompressed[i]);
            loadSlot++;
        }
    }

    playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = 0;
    playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = nullptr;

    batchStartLoadingFiles(playableCharacterLoadingBatchCommands, 0);
    typeOfPlayableCharacterLoaded = 1;
}

void startLoadingPlayableCharacters()
{
    waitForReadFinished();
    if (characterOrGearsLoadingInProgress == 1) {
        finalizeLoadPlayableCharacters();
        if (fieldScriptEntityAlreadyInitialized != 0) {
            return;
        }
    }
    else {
        if (fieldScriptEntityAlreadyInitialized != 0) {
            refinalizePlayableCharacters(0);
            return;
        }
    }
    fieldRequestedGears = (fieldMapNumber & 0xc000) != 0;
    characterOrGearsLoadingInProgress = 0;
    if (fieldRequestedGears) {
        if (typeOfPlayableCharacterLoaded != 2) {
            setupFieldCurrentPartyFromKernelAsGears();
            characterOrGearsLoadingInProgress = 1;
        }
    }
    else {
        if (typeOfPlayableCharacterLoaded != 1) {
            setupFieldCurrentPartyFromKernelAsCharacters();
            characterOrGearsLoadingInProgress = 1;
        }
    }
}

void ClearCacheAfterOverlayLoad()
{
    MissingCode();
}

void freeFieldData()
{
    MissingCode();

    actorArray.clear();
    fieldTriggerData.clear();
    rawFieldDialogBundle.clear();
    rawFieldScriptData.clear();
    walkMesh.clear();
    rawFieldModels.clear();
    gCurrentFieldModels.clear();

    fieldActorSetupParams.clear();
    rawFieldActorSetupParams.clear();

    MissingCode();

    if (NumMechas != 0) {
        freeMechaModule();
        mechaOverlayBuffer.clear();
        ClearCacheAfterOverlayLoad();
    }
    NumMechas = 0;

    MissingCode();
}

void setupScreenTransitionPoly(int param_1, int param_2) {
    MissingCode();
}

void updateScreenEffectSlots(sTag*,int)
{
    MissingCode();
}
void renderScreenTransitionPoly(void) {
    MissingCode();
}

void renderScreenTransition() {
    MissingCode();
}

void transitionFields()
{
    MissingCode();

    freeFieldData();

    MissingCode();

    switch (fieldTransitionMode)
    {
    case 0:
        setupScreenTransitionPoly(0, 0);
        setupRGBCalcSlot0_fadeToBlack(fieldTransitionFadeInLength);
        for(int i=0; i< fieldTransitionFadeInLength; i++) {
            clearFieldOrderingTable();
            updateScreenEffectSlots(&pCurrentFieldRenderingContext->m80D4_uiOT[0], g_frameOddOrEven);
            renderScreenTransitionPoly();
            renderScreenTransition();
        }
        clearFieldOrderingTable();
        MissingCode();
        startLoadingPlayableCharacters();
        finalizeLoadPlayableCharacters();
        {
            s32 fieldTransitionModeBackup = fieldTransitionMode;
            s32 fieldTransitionFadeInLengthBackup = fieldTransitionFadeInLength;
            initFieldData();
            loadFieldGraphics();
            MissingCode();
            fieldTransitionFadeInLength = fieldTransitionFadeInLengthBackup;
            fieldTransitionMode = fieldTransitionModeBackup;
        }
        if (fieldMusicLoadPending == -1) {
            playMusic(currentlyPlayingMusic, 0);
        }
        setupRGBFaderSlot0_fadeIn(fieldTransitionFadeInLength);
        break;
    case 2:
        MissingCode();
        startLoadingPlayableCharacters();
        finalizeLoadPlayableCharacters();
        initFieldData();
        loadFieldGraphics();
        if (fieldGraphicsUploaded)
        {
            MissingCode();
            fieldGraphicsUploaded = 0;
            MissingCode();
        }
        MissingCode();
        break;
    case 6:
        setupRGBCalcSlot0_fadeToBlack(fieldTransitionFadeInLength);
        for (int i = 0; i < fieldTransitionFadeInLength; i++) {
            clearFieldOrderingTable();
            MissingCode();
        }
        clearFieldOrderingTable();
        MissingCode();
        startLoadingPlayableCharacters();
        finalizeLoadPlayableCharacters();
        {
            s32 fieldTransitionModeBackup = fieldTransitionMode;
            s32 fieldTransitionFadeInLengthBackup = fieldTransitionFadeInLength;
            initFieldData();
            loadFieldGraphics();
            MissingCode();
            fieldTransitionFadeInLength = fieldTransitionFadeInLengthBackup;
            fieldTransitionMode = fieldTransitionModeBackup;
        }
        if (fieldMusicLoadPending == -1) {
            playMusic(currentlyPlayingMusic, 0);
        }
        setupRGBFaderSlot0_fadeIn(fieldTransitionFadeInLength);
        break;
    default:
        assert(0);
        break;
    }
    if (fieldTransitionMode != 6) {
        //restoreBackBufferRamCopy();
        MissingCode();
    }
    fieldTransitionMode = 2;
    MissingCode();

}

void exectueEntitiesUpdateFunction()
{
    int numEntitiesToUpdate = totalActors;
    if (onlyUpdateDirector == 1)
    {
        numEntitiesToUpdate = 1;
    }

    playerCanRun = 0;

    numDialogWindowsCreatedThisFrame = 0;
    if (!g_executeUpdateScripts)
    {
        numEntitiesToUpdate = 0;
    }
    for (int i = 0; i < numEntitiesToUpdate; i++)
    {
        sFieldEntity* pFieldEntity = &actorArray[i];
        if (((pFieldEntity->m58_flags & 0xf00) != 0) && ((pFieldEntity->m4C_scriptEntity->m4_flags.m_rawFlags & 0x100000) == 0))
        {
            if (fieldExecuteVar1 != 0) {
                if (fieldExecuteVar2 == 0) {
                    return;
                }
                if (fieldExecuteVar3 == 0) {
                    return;
                }
                if (fieldChangePrevented == 0) {
                    return;
                }
            }
            pCurrentFieldScriptActor = pFieldEntity->m4C_scriptEntity;
            pCurrentFieldScriptActor->m0_fieldScriptFlags.m_rawFlags &= ~0x1000000;
            currentFieldActorId = i;
            pCurrentFieldEntity = pFieldEntity;

            int maxScriptIndex = 0xF;
            if (noUpdatesToPartyMemebers)
            {
                assert(0);
            }
            for (int j = 0; j < 8; j++)
            {
                if (pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18 <= maxScriptIndex)
                {
                    pCurrentFieldScriptActor->mCE_currentScriptSlot = j;
                    maxScriptIndex = pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18;
                }
            }
            if (maxScriptIndex == 0xF)
            {
                pCurrentFieldScriptActor->m8C_scriptSlots[0].m0_scriptPC = getScriptEntryPoint(i, 1);
                pCurrentFieldScriptActor->mCE_currentScriptSlot = 0;
                pCurrentFieldScriptActor->m8C_scriptSlots[0].m4_flags.m18 = 7;
            }
            currentScriptFinished = 1;
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC;
            if (!(pCurrentFieldScriptActor->m0_fieldScriptFlags.m0_updateScriptDisabled))
            {
                executeFieldScript(8);
            }
            pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC;
        }
    }
}

int entityUpdateVar0 = 0;
int entityUpdateVar1 = 0;

std::array<s16, 4> deltaMoveTable0 = {
    0x4, 0x8, 0x10, 0x20
};

std::array<s16, 8> deltaMoveTable1 = {
    0xC00, 0xE00, 0x0, 0x200,
    0x400, 0x600, 0x800, 0xA00
};

void computeDeltaMove(FP_VEC3* param_1, int param_2, uint param_3)
{
    int iVar1;
    int iVar2;

    iVar2 = param_2 * 0x10 * entityMoveSpeedFactor >> 0xc;
    iVar1 = getAngleSin(param_3 & 0xfff);
    param_1->vx = iVar1 * iVar2;
    iVar1 = getAngleCos(param_3 & 0xfff);
    param_1->vy = 0;
    param_1->vz = -(iVar1 * iVar2);
    return;
}

void updateEntityEventCode2(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
    int triangleFlags = 0;
    if (((pFieldScriptEntity->m4_flags.m_rawFlags >> ((int)(short)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) && (noUpdatesToPartyMemebers == '\0')) {
        triangleFlags = pFieldScriptEntity->m14_currentTriangleFlag;
    }

    FP_VEC3 deltaMove;
    computeDeltaMove(&deltaMove, deltaMoveTable0[(triangleFlags >> 9) & 3], deltaMoveTable1[(triangleFlags >> 11) & 0x7] + deltaMoveVar0 & 0xfff);

    int iVar9 = 0;
    int iVar12 = 0;
    s32 touchedActor;
    bool bApplyDelta = triangleFlags & 0x4000;
    if (!pFieldScriptEntity->m0_fieldScriptFlags.mx800_isJumping && ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x41000) == 0))
    {
        if ((triangleFlags & 0x420000) != 0) {
            FP_VEC4 local_60;
            local_60.vx = -((pFieldScriptEntity->m50_surfaceNormal).vx * (pFieldScriptEntity->m50_surfaceNormal).vy) >> 0xf;
            local_60.vz = -((pFieldScriptEntity->m50_surfaceNormal).vz * (pFieldScriptEntity->m50_surfaceNormal).vy) >> 0xf;
            if (local_60.vx == 0) {
                local_60.vx = 1;
            }
            local_60.vy = 1;
            if (local_60.vz == 0) {
                local_60.vz = 1;
            }
            FP_VEC4 local_50;
            VectorNormal(&local_60, &local_50);
            if (local_50.vx == 0) {
                local_50.vx = 1;
            }
            if (local_50.vy == 0) {
                local_50.vy = 1;
            }
            if (local_50.vz == 0) {
                local_50.vz = 1;
            }
            int iVar13 = pFieldScriptEntity->mF0 >> 0x11;
            int iVar8 = 0xc;
            iVar9 = local_50.vx * iVar13 * 0x10;
            iVar12 = local_50.vz * iVar13 * 0x10;
            if ((triangleFlags & 0x400000) != 0) {
                iVar8 = 0x18;
            }
            iVar13 = iVar8 << 0x10;
            if (pFieldScriptEntity->mF0 >> 0x10 < iVar8) {
                iVar13 = pFieldScriptEntity->mF0 + pFieldEntity->m4_pVramSpriteSheet->m1C_gravity;
            }
            pFieldScriptEntity->mF0 = iVar13;
            pFieldEntity->m4_pVramSpriteSheet->mC_step.vy = pFieldScriptEntity->mF0 >> 1;
        }
        if ((triangleFlags & 0x400000) != 0) {
            pFieldScriptEntity->m40.vx += iVar9;
            pFieldScriptEntity->m40.vz += iVar12;
            pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;
        }
        touchedActor = pFieldScriptEntity->m74;
        if (touchedActor == -1)
        {
            if ((triangleFlags & 0x20000) != 0) {
                (pFieldScriptEntity->m40).vx = (pFieldScriptEntity->m40).vx + iVar9;
                (pFieldScriptEntity->m40).vz = (pFieldScriptEntity->m40).vz + iVar12;
            }
            bApplyDelta = triangleFlags & 0x8000;
            goto LAB_Field__8008288c;
        }
    }
    else
    {
LAB_Field__8008288c:
        if (bApplyDelta)
        {
            pFieldScriptEntity->m40.vx += deltaMove.vx;
            pFieldScriptEntity->m40.vy += deltaMove.vy;
            pFieldScriptEntity->m40.vz += deltaMove.vz;
        }
        touchedActor = pFieldScriptEntity->m74;
        if (touchedActor == -1)
        {
            goto LAB_Field__80082af4;
        }
    }

    if (((actorArray[touchedActor].m4C_scriptEntity)->m4_flags.m_rawFlags & 0xc0) == 0xc0) {
        assert(0);
    }

LAB_Field__80082af4:
    if ((pFieldScriptEntity->m4_flags.m_rawFlags & 0x22000) == 0x22000) {
        assert(0);
    }
}

int updateEntityEventCode3Sub0(sFieldScriptEntity* param_1)
{
    int iVar1;

    iVar1 = -1;
    if (((((((param_1->m14_currentTriangleFlag & 0x420000U) == 0) && (iVar1 = -1, updateEntityEventCode3Var1 == 0)) && (iVar1 = -1, param_1->m30_stepVector.vx == 0)) &&
        ((iVar1 = -1, param_1->m30_stepVector.vy == 0 && (iVar1 = -1, param_1->m30_stepVector.vz == 0)))) &&
        ((iVar1 = -1, updateEntityEventCode3Var2 == 1 && ((iVar1 = -1, param_1->m74 == -1 && (iVar1 = -1, (param_1->m0_fieldScriptFlags.m_rawFlags & 0x401800) == 0)))))) &&
        (((param_1->m4_flags.m_rawFlags & 1) == 0 || (iVar1 = -1, param_1->m10_walkmeshId != 0)))) {
        if (((param_1->m4_flags.m_rawFlags & 2) == 0) || (param_1->m10_walkmeshId != 1)) {
            iVar1 = 0;
            if ((param_1->m4_flags.m_rawFlags & 4) != 0) {
                iVar1 = -(s32)(param_1->m10_walkmeshId == 2);
            }
        }
        else {
            iVar1 = -1;
        }
    }
    return iVar1;
}

void updateEntityEventCode3Sub1(sSpriteActor* param_1, ushort rotation, sFieldEntity* param_3)
{
    sFieldScriptEntity* psVar2;
    int iVar3;
    uint uVar4;

    if ((param_3->m58_flags & 0x40) == 0) {
        uVar4 = 0x40000 / (ushort)param_3->m4C_scriptEntity->m76;
        if ((rotation & 0x8000) != 0) {
        LAB_Field__800821b0:
            (param_1->mC_step).vx = 0;
            (param_1->mC_step).vz = 0;
            goto LAB_Field__800821bc;
        }
    }
    else {
        if (((int)(short)rotation & 0x8000U) != 0) goto LAB_Field__800821b0;
        uVar4 = param_3->m4C_scriptEntity->m4_flags.m_rawFlags;
        if ((uVar4 & 0x2000) == 0) {
            if ((uVar4 & 0x80000) == 0) {
                OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, (int)(short)rotation);
            }
            else {
                iVar3 = ((int)(0x40000 / (ushort)param_3->m4C_scriptEntity->m76) >> 8) * 0x20;
                (param_1->mC_step).vx = (getAngleSin(rotation & 0xfff) * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
                (param_1->mC_step).vz = (-(getAngleCos(rotation & 0xfff) * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];
                param_1->m18_moveSpeed = 0x4000000 / (ushort)param_3->m4C_scriptEntity->m76;
            }
            goto LAB_Field__800821bc;
        }
        if ((uVar4 & 0x20000) != 0) {
            uVar4 = param_3->m4C_scriptEntity->m12C_flags;
            assert(0);
            //(param_1->mC).vx = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x128) * -0x10000;
            //(param_1->mC).vz = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x130) * -0x10000;
            goto LAB_Field__800821bc;
        }
        uVar4 = 0x80000 / (ushort)param_3->m4C_scriptEntity->m76;
    }

    iVar3 = ((int)uVar4 >> 8) * 0x20;
    (param_1->mC_step).vx = (getAngleSin(rotation & 0xfff) * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
    (param_1->mC_step).vz = (-(getAngleCos(rotation & 0xfff) * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];

LAB_Field__800821bc:
    (param_1->mC_step).vx = (param_1->mC_step).vx & 0xfffff000;
    (param_1->mC_step).vz = (param_1->mC_step).vz & 0xfffff000;
}

int updateEntityEventCode3Sub2(FP_VEC3* param_1, sFieldScriptEntity* param_2)
{
    long lVar2;
    std::vector<u16>* pasVar3;

    if ((param_2->m12C_flags & 0x1000) != 0) {
        sVec2_s16 position;
        position.set((((param_2->m20_position).vx + param_1->vx) >> 0x10), (((param_2->m20_position).vz + param_1->vz) >> 0x10));

        std::vector<sVec2_s16>& pasVar3 = param_2->m114_movementBoundingZone;
        std::array<sVec2_s16, 4> boundingZone;
        boundingZone[0].set(pasVar3[0].vx, pasVar3[0].vy);
        boundingZone[1].set(pasVar3[1].vx, pasVar3[1].vy);
        boundingZone[2].set(pasVar3[2].vx, pasVar3[2].vy);
        boundingZone[3].set(pasVar3[3].vx, pasVar3[3].vy);

        if ((NCLIP(boundingZone[0], boundingZone[1], position) < 0) || (NCLIP(boundingZone[1], boundingZone[2], position) < 0) || (NCLIP(boundingZone[2], boundingZone[3], position) < 0) || (NCLIP(boundingZone[3], boundingZone[0], position) < 0)) {
            return -1;
        }
    }
    return 0;
}

int updateEntityEventCode3Sub3Sub1(FP_VEC3* param_1, FP_VEC4* param_2, sFieldScriptEntity* pFieldScriptEntity, std::array<SFP_VEC4, 2>& param_4, SFP_VEC4* param_5, int param_6, uint* param_7)
{
    int triangleId = pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
    int collisionFlag;

    int lastTriangle;
    std::vector<sWalkMesh::sTriangleData>::iterator pWalkMeshTriangles = walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId]->begin();
    std::vector<SFP_VEC4>::iterator pVertices = walkMeshVertices[pFieldScriptEntity->m10_walkmeshId]->begin();

    if (triangleId != -1)
    {
        param_5->vx = (param_2->vx + param_1->vx) >> 16;
        param_5->vy = 0;
        param_5->vz = (param_2->vz + param_1->vz) >> 16;

        sVec2_s16 refPos = sVec2_s16::fromValue(param_5->vx, param_5->vz);
        sVec2_s16 refPos2 = sVec2_s16::fromValue(param_2->vx, param_2->vz);

        u32 mask = 0;
        if ((pFieldScriptEntity->m4_flags.m_rawFlags >> ((int)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
            mask = -(s32)(noUpdatesToPartyMemebers == '\0');
        }

        int local_38;
        if (((((*walkMeshVar1)[pWalkMeshTriangles[triangleId].mC_indexInWalkmeshData1]) & mask & 0x400000) == 0) && (param_6 != 0x80)) {
            local_38 = 0;
        }
        else {
            local_38 = 1;
        }

        int iterationCount = 0;
        for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
        {
            sVec2_s16 vert0;
            sVec2_s16 vert1;
            sVec2_s16 vert2;

            sWalkMesh::sTriangleData& pTriangle = pWalkMeshTriangles[triangleId];
            vert0.set(pVertices[pTriangle.m0_verticeIndex[0]].vx, pVertices[pTriangle.m0_verticeIndex[0]].vz); // s4
            vert1.set(pVertices[pTriangle.m0_verticeIndex[1]].vx, pVertices[pTriangle.m0_verticeIndex[1]].vz); // s3
            vert2.set(pVertices[pTriangle.m0_verticeIndex[2]].vx, pVertices[pTriangle.m0_verticeIndex[2]].vz); // s1

            lastTriangle = triangleId;

            collisionFlag = 0;
            if (NCLIP(vert0, vert1, refPos) < 0) {
                collisionFlag |= 1;
            }

            if (NCLIP(vert1, vert2, refPos) < 0) {
                collisionFlag |= 2;
            }

            if (NCLIP(vert2, vert0, refPos) < 0) {
                collisionFlag |= 4;
            }

            if (collisionFlag < 8) {
                switch (collisionFlag)
                {
                case 0:
                    iterationCount = 0xFF;
                    break;
                case 1:
                    triangleId = pTriangle.m6_connectivity[0];
                    break;
                case 2:
                    triangleId = pTriangle.m6_connectivity[1];
                    break;
                case 3:
                    if (NCLIP(vert1, refPos, refPos2) < 0)
                    {
                        triangleId = pTriangle.m6_connectivity[0];
                    }
                    else {
                        triangleId = pTriangle.m6_connectivity[1];
                    }
                    break;
                case 4:
                    triangleId = pTriangle.m6_connectivity[2];
                    break;
                case 5:
                    if (NCLIP(vert0, refPos, refPos2) < 0)
                    {
                        triangleId = pTriangle.m6_connectivity[2];
                    }
                    else {
                        triangleId = pTriangle.m6_connectivity[0];
                    }
                    break;
                case 6:
                    if (NCLIP(vert2, refPos, refPos2) < 0)
                    {
                        triangleId = pTriangle.m6_connectivity[1];
                    }
                    else {
                        triangleId = pTriangle.m6_connectivity[2];
                    }
                    break;
                case 7:
                    triangleId = -1;
                    break;
                default:
                    assert(0);
                }
            }

            u32 uVar3 = (*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask;

            *param_7 = uVar3;

            FP_VEC4 VStack120;

            if ((((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags >> 8 & 7 & uVar3 >> 5) != 0) || (((uVar3 & 0x800000) != 0 && (pFieldScriptEntity->m10_walkmeshId == 0)))) ||
                (((uVar3 & 0x400000) != 0 &&
                    ((local_38 == 0 &&
                        (getTriangleNormalAndAdjustY(pVertices[pTriangle.m0_verticeIndex[0]], pVertices[pTriangle.m0_verticeIndex[1]], pVertices[pTriangle.m0_verticeIndex[2]], param_5, &VStack120),
                            param_5->vy < param_2->vy.getIntegerPart())))))) {
                triangleId = -1;
                break;
            }

            // reached the edge of the walkmesh
            if (triangleId == -1)
            {
                goto LAB_Field__8007c3e4;
            }
        }
        if ((triangleId != -1) && (iterationCount != 0x20)) {
            if (param_6 == -1) {
                return 0;
            }
            sWalkMesh::sTriangleData& pTriangle = pWalkMeshTriangles[triangleId];
            FP_VEC4 VStack120;
            getTriangleNormalAndAdjustY(pVertices[pTriangle.m0_verticeIndex[0]], pVertices[pTriangle.m0_verticeIndex[1]], pVertices[pTriangle.m0_verticeIndex[2]], param_5, &VStack120);
            return 0;
        }

    LAB_Field__8007c3e4:
        if (collisionFlag == 2) {
            sWalkMesh::sTriangleData& pTriangle = pWalkMeshTriangles[lastTriangle];
            param_4[0].vx = pVertices[pTriangle.m0_verticeIndex[1]].vx;
            param_4[0].vy = pVertices[pTriangle.m0_verticeIndex[1]].vy;
            param_4[0].vz = pVertices[pTriangle.m0_verticeIndex[1]].vz;
            param_4[1].vx = pVertices[pTriangle.m0_verticeIndex[2]].vx;
            param_4[1].vy = pVertices[pTriangle.m0_verticeIndex[2]].vy;
            param_4[1].vz = pVertices[pTriangle.m0_verticeIndex[2]].vz;
        }
        else {
            if (collisionFlag < 3) {
                if (collisionFlag != 1) {
                    return 0xffffffff;
                }
                sWalkMesh::sTriangleData& pTriangle = pWalkMeshTriangles[lastTriangle];
                param_4[0].vx = pVertices[pTriangle.m0_verticeIndex[0]].vx;
                param_4[0].vy = pVertices[pTriangle.m0_verticeIndex[0]].vy;
                param_4[0].vz = pVertices[pTriangle.m0_verticeIndex[0]].vz;
                param_4[1].vx = pVertices[pTriangle.m0_verticeIndex[1]].vx;
                param_4[1].vy = pVertices[pTriangle.m0_verticeIndex[1]].vy;
                param_4[1].vz = pVertices[pTriangle.m0_verticeIndex[1]].vz;
            }
            else {
                if (collisionFlag != 4) {
                    return -1;
                }
                sWalkMesh::sTriangleData& pTriangle = pWalkMeshTriangles[lastTriangle];
                param_4[0].vx = pVertices[pTriangle.m0_verticeIndex[2]].vx;
                param_4[0].vy = pVertices[pTriangle.m0_verticeIndex[2]].vy;
                param_4[0].vz = pVertices[pTriangle.m0_verticeIndex[2]].vz;
                param_4[1].vx = pVertices[pTriangle.m0_verticeIndex[0]].vx;
                param_4[1].vy = pVertices[pTriangle.m0_verticeIndex[0]].vy;
                param_4[1].vz = pVertices[pTriangle.m0_verticeIndex[0]].vz;
            }
        }
    }

    return -1;
}

uint updateEntityEventCode3Sub3Sub2(short param_1, std::array<SFP_VEC4, 2>& param_2, FP_VEC3* param_3)

{
    short sVar1;
    short sVar2;
    long lVar3;
    uint uVar4;
    uint uVar5;
    FP_VEC4 local_38;
    FP_VEC4 local_28;

    lVar3 = ratan2(param_2[1].vz - param_2[0].vz, param_2[1].vx - param_2[0].vx);
    uVar4 = -lVar3 & 0xfff;
    uVar5 = (0xc00U - (int)param_1 & 0xfff) + uVar4 & 0xfff;
    if (uVar5 - 0x80 < 0xf01) {
        if (uVar5 < 0x800) {
            local_38.vx = param_2[0].vx - param_2[1].vx;
            sVar1 = param_2[0].vz;
            sVar2 = param_2[1].vz;
            uVar4 = uVar4 + 0x800 & 0xfff;
        }
        else {
            local_38.vx = param_2[1].vx - param_2[0].vx;
            sVar1 = param_2[1].vz;
            sVar2 = param_2[0].vz;
        }
        local_38.vy = 0;
        local_38.vz = (int)sVar1 - (int)sVar2;
        VectorNormal(&local_38, &local_28);
        lVar3 = length2d(param_3->vx >> 0xc, param_3->vz >> 0xc);

        param_3->vx = local_28.vx * lVar3;
        param_3->vy = 0;
        param_3->vz = local_28.vz * lVar3;
    }
    else {
        param_3->vx = 0;
        param_3->vy = 0;
        param_3->vz = 0;
    }
    return uVar4;
}

int updateEntityEventCode3Sub4Sub1(FP_VEC3* deltaStep, FP_VEC4* position, sFieldScriptEntity* pFieldScriptEntity, std::array<SFP_VEC4, 2>& param_4, SFP_VEC4* param_5, int param_6)
{
    int collisionFlag;
    int uVar2;

    int triangleId = (int)pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
    std::vector<sWalkMesh::sTriangleData>& pWalkMeshTriangles = *walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
    std::vector<SFP_VEC4>& pWalkMeshVertices = *walkMeshVertices[pFieldScriptEntity->m10_walkmeshId];

    if (triangleId == -1) {
        uVar2 = -1;
    }
    else {
        param_5->vx = (position->vx + deltaStep->vx) >> 0x10;
        param_5->vy = 0;
        param_5->vz = (position->vz + deltaStep->vz) >> 0x10;

        sVec2_s16 startPosition;
        sVec2_s16 endPosition;
        startPosition.set((position->vx >> 0x10), (position->vz >> 0x10));
        endPosition.set((position->vx + deltaStep->vx) >> 0x10, (position->vz + deltaStep->vz) >> 0x10);

        u32 mask = 0;

        if ((pFieldScriptEntity->m4_flags.m_rawFlags >> ((int)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
            mask = -(s32)(noUpdatesToPartyMemebers == '\0');
        }

        int tempTriangleId;

        sFieldScriptEntity* local_58 = pFieldScriptEntity;
        int iterationCount = 0;
        for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
        {
            sWalkMesh::sTriangleData* pTriangle = &pWalkMeshTriangles[triangleId];

            sVec2_s16 vert0;
            sVec2_s16 vert1;
            sVec2_s16 vert2;

            vert0.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vz);
            vert1.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vz);
            vert2.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vz);

            tempTriangleId = triangleId;

            collisionFlag = 0;
            if (NCLIP(vert0, vert1, endPosition) < 0) {
                collisionFlag |= 1;
            }

            if (NCLIP(vert1, vert2, endPosition) < 0) {
                collisionFlag |= 2;
            }

            if (NCLIP(vert2, vert0, endPosition) < 0) {
                collisionFlag |= 4;
            }

            if (collisionFlag < 8) {
                switch (collisionFlag)
                {
                case 0:
                    mask = 0xFF;
                    break;
                case 1:
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                    break;
                case 2:
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                    break;
                case 3:
                    if (NCLIP(vert1, endPosition, startPosition) < 0)
                    {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                        collisionFlag = 1;
                    }
                    else {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                        collisionFlag = 2;
                    }
                    break;
                case 4:
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                    break;
                case 5:
                    if (NCLIP(vert0, endPosition, startPosition) < 0)
                    {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                        collisionFlag = 4;
                    }
                    else {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                        collisionFlag = 1;
                    }
                    break;
                case 6:
                    if (NCLIP(vert2, endPosition, startPosition) < 0)
                    {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                        collisionFlag = 2;
                    }
                    else {
                        triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                        collisionFlag = 4;
                    }
                    break;
                case 7:
                    triangleId = -1;
                    break;
                default:
                    assert(0);
                }
            }

            // todo: does this really happen to be -1?
            if (triangleId == -1)
            {
                return -1;
            }
            u32 uVar4 = ((*walkMeshVar1)[pWalkMeshTriangles[triangleId].mC_indexInWalkmeshData1]) & mask;
            if ((((local_58->m0_fieldScriptFlags.m_rawFlags >> 9 & 3 & uVar4 >> 3) != 0) || ((local_58->m0_fieldScriptFlags.m_rawFlags >> 8 & 7 & uVar4 >> 5) != 0)) || (((uVar4 & 0x800000) != 0 && (local_58->m10_walkmeshId == 0)))) {
                triangleId = -1;
                break;
            }

            if (triangleId == -1)
                break;
        }

        if ((triangleId == -1) || (iterationCount == 0x20)) {

            switch (collisionFlag)
            {
            case 0:
                return -1;
            case 1:
                param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
                param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
                break;
            case 2:
                param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
                param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
                break;
            case 4:
                param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
                param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
                break;
            default:
                assert(0);
                return -1;
            }
            uVar2 = -1;
        }
        else {
            if (param_6 == -1) {
                uVar2 = 0;
            }
            else {
                //psVar6 = (short*)(puVar9 + iVar8 * 0xe);
                assert(0);
                //FUN_Field__8007b07c((short*)(puVar10 + *psVar6 * 8), (short*)(puVar10 + psVar6[1] * 8), (short*)(puVar10 + psVar6[2] * 8), param_5, &sStack104);
                uVar2 = 0;
            }
        }
    }
    return uVar2;
}

int updateEntityEventCode3Sub4(FP_VEC3* position, sFieldScriptEntity* param_2, std::array<SFP_VEC4, 2>& param_3, s16 angle)
{
    FP_VEC4* psVar4;
    FP_VEC3 local_60;
    SFP_VEC4 auStack80;

    local_60.vx = position->vx + getAngleSin(angle & 0xfff) * 0x40;
    local_60.vz = position->vz + getAngleCos(angle & 0xfff) * -0x40;

    psVar4 = &param_2->m20_position;

    if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1) {
        local_60.vx = position->vx;
        local_60.vy = position->vy;
        local_60.vz = position->vz;
        updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
    }
    else {
        int newAngle = angle - 0x100U & 0xfff;
        local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
        local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
        if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
        {
            local_60.vx = position->vx;
            local_60.vy = position->vy;
            local_60.vz = position->vz;
            updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
        }
        else
        {
            int newAngle = angle + 0x100U & 0xfff;
            local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
            local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
            if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
            {
                local_60.vx = position->vx;
                local_60.vy = position->vy;
                local_60.vz = position->vz;
                updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
            }
            else
            {
                local_60.vx = position->vx;
                local_60.vy = position->vy;
                local_60.vz = position->vz;
            }

        }
    }

    if (updateEntityEventCode3Sub4Sub1(&local_60, &param_2->m20_position, param_2, param_3, &auStack80, 0) == -1) {
        return -1;
    }
    else {
        if ((param_2->m0_fieldScriptFlags.m_rawFlags & 0x40000) == 0) {
            if (((int)auStack80.vy << 0x10 < (param_2->m20_position).vy) && (updateEntityEventCode3Var1 == 0))
                return -1;
        }
        else {
            auStack80.vy = param_2->mEC_elevation;
        }

        position->vx = local_60.vx;
        position->vy = auStack80.vy * 0x10000 - param_2->m20_position.vy;
        position->vz = local_60.vz;

        param_2->m72_elevation = (param_2->m20_position.vy + position->vy) >> 0x10;
        return 0;
    }
}

int updateEntityEventCode3Sub3(FP_VEC3* param_1, sFieldScriptEntity* param_2, std::array<SFP_VEC4, 2>& param_3, short angle)
{
    long lVar2;
    FP_VEC3 local_88;
    SFP_VEC4 local_68;
    FP_VEC4 local_58;
    FP_VEC4 local_48;
    uint local_38;

    local_88.vx = param_1->vx + getAngleSin(angle - 0x100U & 0xfff) * 0x40;
    local_88.vz = param_1->vz + getAngleCos(angle - 0x100U & 0xfff) * -0x40;
    if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1) {
        local_88.vx = param_1->vx;
        local_88.vy = param_1->vy;
        local_88.vz = param_1->vz;
        updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
    }
    else {
        local_88.vx = param_1->vx + getAngleSin(angle + 0x100U & 0xfff) * 0x40;
        local_88.vz = param_1->vz + getAngleCos(angle + 0x100U & 0xfff) * -0x40;
        if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
        {
            local_88.vx = param_1->vx;
            local_88.vy = param_1->vy;
            local_88.vz = param_1->vz;
            updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
        }
        else
        {
            local_88.vx = param_1->vx + getAngleSin(angle & 0xfff) * 0x40;
            local_88.vz = param_1->vz + getAngleCos(angle & 0xfff) * -0x40;
            if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
            {
                local_88.vx = param_1->vx;
                local_88.vy = param_1->vy;
                local_88.vz = param_1->vz;
                updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
            }
            else
            {
                local_88.vx = param_1->vx;
                local_88.vy = param_1->vy;
                local_88.vz = param_1->vz;
            }
        }
    }
    if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
        return -1;
    }
    if ((((param_2->m20_position.vy) >> 16) <= local_68.vy) && ((local_38 & 0x200000) == 0)) {
        if ((local_38 & 0x420000) == 0) {
            if (((param_2->m20_position.vy) >> 16) + 0x40 <= (int)local_68.vy)
            {
                param_1->vx = local_88.vx;
                param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
                param_1->vz = local_88.vz;
                param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
                return 0;
            }
        }
        else {
            if ((param_2->m14_currentTriangleFlag & 0x420000U) == 0)
            {
                param_1->vx = local_88.vx;
                param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
                param_1->vz = local_88.vz;
                param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
                return 0;
            }
        }
    }
    local_58.vx = -local_88.vx >> 8;
    local_58.vz = -local_88.vz >> 8;
    local_58.vy = (local_68.vy * 0x10000 - (param_2->m20_position).vy) >> 8;
    VectorNormal(&local_58, &local_48);
    lVar2 = length2d(local_88.vx >> 8, local_88.vz >> 8);

    local_88.vx = -(lVar2 * local_48.vx) >> 4;
    local_88.vy = lVar2 * local_48.vy >> 4;
    local_88.vz = -(lVar2 * local_48.vz) >> 4;
    if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
        return -1;
    }
    param_2->m0_fieldScriptFlags.m_rawFlags |= 0x4000000;

    param_1->vx = local_88.vx;
    param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
    param_1->vz = local_88.vz;
    param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
    return 0;
}

void setVisualAnimation(sSpriteActor* param_1, int animationId, sFieldEntity* param_3)
{
    if ((param_3->m58_flags & 0x40) != 0) {
        if ((animationId != 3) && (updateEntityEventCode4Var0 == 0)) {
            param_3->m4C_scriptEntity->m0_fieldScriptFlags.mx800_isJumping = 0;
        }
        if (animationId == 0xff) {
            animationId = 0;
        }
        if (animationId != jumpAnimationId) {
            param_3->m4C_scriptEntity->m0_fieldScriptFlags.mx800_isJumping = 0;
        }

        if ((param_3->m4C_scriptEntity->m4_flags.m_rawFlags & 0x2000) == 0) {
            if ((param_3->m4C_scriptEntity->m4_flags.m_rawFlags & 0x1000000) == 0) {
                spriteActorSetPlayingAnimation(param_1, animationId);
            }
        }
        else{
            static const std::array<u8, 12> updateEntityEventCode4Table0 = { {
                    1,2,2,2,
                    0,0,0,6,
                    0,0,0,0
            } };

            if (animationId < 0x10) {
                animationId = updateEntityEventCode4Table0[animationId];
            }
            else {
                animationId -= 0x10;
            }
            mechaPlayAnimation(param_3->m4C_scriptEntity->m12C_flags >> 0xd & 7, 0, animationId);
            mechaCurrentAnimation[param_3->m4C_scriptEntity->m12C_flags >> 0xd & 0x7] = animationId;

        }
    }
    return;
}

void updateEntityEventCode3(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
    int rotation = pFieldScriptEntity->m104_rotation;
    std::array<SFP_VEC4, 2> auStack56;

    if ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1000000) != 0) {
        updateEntityEventCode3Var0 = index;
        return;
    }

    int animationId = 1;
    if (((pFieldScriptEntity->m0_fieldScriptFlags.m14_isPlayerControlled) && ((padButtonForScripts[0].m0_buttons & controllerButtons::CROSS) != 0)) && (playerCanRun == 1)) {
        animationId = 2; // run
    }
    if (((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1800) != 0) && (pFieldScriptEntity->mE8_currentAnimationId != animationId)) {
        if (pFieldScriptEntity->mE8_currentAnimationId == 1) {
            animationId = 1;
        }
        else {
            if (pFieldScriptEntity->mE8_currentAnimationId == 2) {
                animationId = 2;
            }
        }
    }

    updateEntityEventCode3Var0 = index;

    if (8 < pFieldScriptEntity->mE3) {
        pFieldScriptEntity->mE3--;
    }

    int moveMask = (pFieldScriptEntity->m40).vx | (pFieldScriptEntity->m40).vy | (pFieldScriptEntity->m40).vz;
    if (updateEntityEventCode3Sub0(pFieldScriptEntity) == -1)
    {
        moveMask = 1;
    }

    FP_VEC3 stepVector;

    bool bMoved = true;

    if (((((int)(short)rotation & 0x8000U) == 0) || (moveMask != 0)) || ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x40800) != 0)) {
        if (((int)(short)rotation & 0x8000U) == 0) {
            updateEntityEventCode3Sub1(pFieldEntity->m4_pVramSpriteSheet, (int)(short)rotation, pFieldEntity);
            stepVector.vx = pFieldEntity->m4_pVramSpriteSheet->mC_step.vx + pFieldScriptEntity->m40.vx;
            stepVector.vy = pFieldEntity->m4_pVramSpriteSheet->mC_step.vy + pFieldScriptEntity->m40.vy;
            stepVector.vz = pFieldEntity->m4_pVramSpriteSheet->mC_step.vz + pFieldScriptEntity->m40.vz;
            pFieldScriptEntity->m106_currentRotation = rotation;
        }
        else {
            stepVector.vx = (pFieldScriptEntity->m40).vx;
            stepVector.vy = (pFieldScriptEntity->m40).vy;
            stepVector.vz = (pFieldScriptEntity->m40).vz;
            rotation = pFieldScriptEntity->m106_currentRotation & 0xfff;
        }

        if (updateEntityEventCode3Sub2(&stepVector, pFieldScriptEntity) != 0)
        {
            bMoved = false;
        }
        else
        {
            if ((stepVector.vx != 0) || (stepVector.vz != 0)) {
                int lVar2 = ratan2(stepVector.vz, stepVector.vx);
                rotation = -(short)lVar2 & 0xfff;
            }

            int iVar1 = -1;
            if (pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId] != -1) {
                int backupFlags = pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags;
                if (index == playerControlledActor) {
                    if (partyToFieldEntityArrayMapping[1] != 0xff) {
                        pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags |= (actorArray[partyToFieldEntityArrayMapping[1]].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags & 0x600;
                    }
                    if (partyToFieldEntityArrayMapping[2] != 0xff) {
                        pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags |= (actorArray[partyToFieldEntityArrayMapping[2]].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags & 0x600;
                    }
                }
                if ((((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x41800) == 0) && (pFieldScriptEntity->m74 == -1)) && (updateEntityEventCode3Var1 == 0)) {
                    iVar1 = updateEntityEventCode3Sub3(&stepVector, pFieldScriptEntity, auStack56, rotation);
                }
                else {
                    iVar1 = updateEntityEventCode3Sub4(&stepVector, pFieldScriptEntity, auStack56, rotation);
                }
                pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags = (pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & ~0x600) | (backupFlags & 0x600);
            }
            if (iVar1 == -1)
            {
                bMoved = false;
            }
        }
    }
    else
    {
        animationId = pFieldScriptEntity->mE6;
        pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;

        bMoved = false;
    }

    if (!bMoved)
    {
        pFieldScriptEntity->mF0 = 0x10000;
        (pFieldScriptEntity->m40).vx = 0;
        (pFieldScriptEntity->m40).vy = 0;
        (pFieldScriptEntity->m40).vz = 0;
        stepVector.vx = 0;
        stepVector.vy = 0;
        stepVector.vz = 0;
        (pFieldEntity->m4_pVramSpriteSheet->mC_step).vx = 0;
        (pFieldEntity->m4_pVramSpriteSheet->mC_step).vz = 0;
        pFieldScriptEntity->m106_currentRotation = pFieldScriptEntity->m106_currentRotation | 0x8000;
    }

    pFieldScriptEntity->m4_flags.m_rawFlags &= ~0x1000;

    if (!pFieldScriptEntity->m0_fieldScriptFlags.mx800_isJumping) {
        if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
            animationId = pFieldScriptEntity->mE6;
        }
        moveMask = getWalkmeshTriangleFlag(pFieldScriptEntity);
        if ((moveMask & 0x200000) != 0) {
            if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
                animationId = 6;
                if (pFieldScriptEntity->mE8_currentAnimationId != 6) goto LAB_Field__800830ac;
                pFieldScriptEntity->m4_flags.m_rawFlags = pFieldScriptEntity->m4_flags.m_rawFlags | 0x1000;
            }
            animationId = 6;
        }
    }
    else
    {
        animationId = jumpAnimationId;
        if (updateEntityEventCode4Var0 == 0) {
            if (((pFieldEntity->m4_pVramSpriteSheet->m0_position).vy.getIntegerPart()) == pFieldEntity->m4_pVramSpriteSheet->m84_maxY) {
                pFieldEntity->m4_pVramSpriteSheet->m18_moveSpeed = 0;
                animationId = jumpAnimationId;
            }
            else {
                if (animationId == 2) {
                    pFieldEntity->m4_pVramSpriteSheet->m18_moveSpeed = pFieldEntity->m4_pVramSpriteSheet->m82 * 0x60;
                    animationId = jumpAnimationId;
                }
                else {
                    pFieldEntity->m4_pVramSpriteSheet->m18_moveSpeed = pFieldEntity->m4_pVramSpriteSheet->m82 * 0x30;
                    animationId = jumpAnimationId;
                }
            }
        }
    }

LAB_Field__800830ac:
    if (pFieldScriptEntity->mEA_forcedAnimation != 0xff) {
        animationId = pFieldScriptEntity->mEA_forcedAnimation;
    }
    if (((int)pFieldScriptEntity->mE8_currentAnimationId != (int)animationId) && ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x2000000) == 0)) {
        pFieldScriptEntity->mE8_currentAnimationId = animationId;
        setVisualAnimation(pFieldEntity->m4_pVramSpriteSheet, animationId, pFieldEntity);
    }
    if ((pFieldScriptEntity->m14_currentTriangleFlag & 0x100U) != 0) {
        stepVector.vx = stepVector.vx >> 1;
        stepVector.vz = stepVector.vz >> 1;
    }
    pFieldScriptEntity->m30_stepVector.vx = stepVector.vx;
    pFieldScriptEntity->m30_stepVector.vy = stepVector.vy;
    pFieldScriptEntity->m30_stepVector.vz = stepVector.vz;
    pFieldScriptEntity->m40.vx = 0;
    pFieldScriptEntity->m40.vy = 0;
    pFieldScriptEntity->m40.vz = 0;
}

int EntityMoveCheck1(int entityIndex, int mask, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity, uint param_5);

void Vec3ToSVec3(SFP_VEC3* param_1, FP_VEC3* param_2)
{
    param_1->vx = param_2->vx.getIntegerPart();
    param_1->vy = param_2->vy.getIntegerPart();
    param_1->vz = param_2->vz.getIntegerPart();
    return;
}

s32 computeDistanceBetweenActors(int param_1, int param_2)
{
    return length2d(
        actorArray[param_2].m4C_scriptEntity->m20_position.vx.getIntegerPart() - actorArray[param_1].m4C_scriptEntity->m20_position.vx.getIntegerPart(),
        actorArray[param_2].m4C_scriptEntity->m20_position.vz.getIntegerPart() - actorArray[param_1].m4C_scriptEntity->m20_position.vz.getIntegerPart());
}

int isPositionInEntityScriptBoundingVolume(int testedX, int testedZ, sFieldScriptEntity* testedScriptEntity, int extraRadius)
{
    s32 minX = testedScriptEntity->m20_position.vx.getIntegerPart() - testedScriptEntity->m18_boundingVolume.vx - extraRadius;
    s32 maxX = testedScriptEntity->m20_position.vx.getIntegerPart() + testedScriptEntity->m18_boundingVolume.vx + extraRadius;

    s32 minZ = testedScriptEntity->m20_position.vz.getIntegerPart() - testedScriptEntity->m18_boundingVolume.vz - extraRadius;
    s32 maxZ = testedScriptEntity->m20_position.vz.getIntegerPart() + testedScriptEntity->m18_boundingVolume.vz + extraRadius;

    sVec2_s16 minXmaxZ = sVec2_s16::fromValue(minX, maxZ);
    sVec2_s16 maxXmaxZ = sVec2_s16::fromValue(maxX, maxZ);
    sVec2_s16 maxXminZ = sVec2_s16::fromValue(maxX, minZ);
    sVec2_s16 minXminZ = sVec2_s16::fromValue(minX, minZ);
    sVec2_s16 testedPosition = sVec2_s16::fromValue(testedX, testedZ);

    if (NCLIP(minXmaxZ, maxXmaxZ, testedPosition) < 0)
        return -1;
    if (NCLIP(maxXmaxZ, maxXminZ, testedPosition) < 0)
        return -1;
    if (NCLIP(maxXminZ, minXminZ, testedPosition) < 0)
        return -1;
    if (NCLIP(minXminZ, minXmaxZ, testedPosition) < 0)
        return -1;

    if (fieldDebugDisable == 0) {
        assert(0);
    }
    return 0;
}

int startScriptsForCollisionsVar0 = 0;

void  startScriptsForCollisions(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity)
{
    bool bTrigger = false;
    u32 newValueInFlags = 7;
    for (int actorId =0; actorId <totalActors; actorId++)
    {
        sFieldScriptEntity* pTestedScriptEntity = actorArray[actorId].m4C_scriptEntity;
        s8 scriptIndexToStart = -1;

        if (((pTestedScriptEntity->m0_fieldScriptFlags.m0_updateScriptDisabled) == 0) && (pPlayerScriptEntity->m74 != actorId)) {
            s32 yDiff = pTestedScriptEntity->m20_position.vy.getIntegerPart() + pTestedScriptEntity->m60[1];
            if ((pTestedScriptEntity->m4_flags.m_rawFlags & 0x180) != 0) {
                if ((pTestedScriptEntity->m4_flags.m_rawFlags & 0x100) == 0) {
                    startScriptsForCollisionsVar0 = 0;
                }
                else {
                    if ((((padButtonForDialogs & controllerButtons::INTERACT) == 0) || (bTrigger)) || ((pTestedScriptEntity->m4_flags.m_rawFlags & 0x4000000) != 0)) {
                        if ((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0xa20000) == 0) {
                            scriptIndexToStart = '\x03';
                            newValueInFlags = 4;
                            FP_VEC4 tempVec1;
                            tempVec1.vx = (((int)(pTestedScriptEntity->m20_position).vx.getIntegerPart()) - pPlayerScriptEntity->m20_position.vx.getIntegerPart()) + (int)pTestedScriptEntity->m60[0];
                            tempVec1.vz = (((int)(pTestedScriptEntity->m20_position).vz.getIntegerPart()) - pPlayerScriptEntity->m20_position.vz.getIntegerPart()) + (int)pTestedScriptEntity->m60[2];
                            int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                            int bVar1 = startScriptsForCollisionsVar0 == 0;
                            pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | (-(lVar5 >> 9) & 7U) << 9;
                            if ((bVar1) && ((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x8000000) != 0)) {
                                startScriptsForCollisionsVar0 = 1;
                                (pPlayerEntity->m4_pVramSpriteSheet->mC_step).vy = 0;
                            }
                        }
                    }
                    else {
                        if (((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x220000) == 0) && (windowOpenBF == 0)) {
                            bTrigger = true;
                            scriptIndexToStart = '\x02';
                            newValueInFlags = 3;
                            FP_VEC4 tempVec1;
                            tempVec1.vx = (((int)(pTestedScriptEntity->m20_position).vx.getIntegerPart()) - pPlayerScriptEntity->m20_position.vx.getIntegerPart()) + (int)pTestedScriptEntity->m60[0];
                            tempVec1.vz = (((int)(pTestedScriptEntity->m20_position).vz.getIntegerPart()) - pPlayerScriptEntity->m20_position.vz.getIntegerPart()) + (int)pTestedScriptEntity->m60[2];
                            int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                            pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | -lVar5 & 0xe00U;
                        }
                    }
                }
            }
            if ((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x2000) == 0) {
                FP_VEC4 tempVec1;
                tempVec1.vx = pTestedScriptEntity->m20_position.vx.getIntegerPart() - pPlayerScriptEntity->m20_position.vx.getIntegerPart() + pTestedScriptEntity->m60[0];
                tempVec1.vz = pTestedScriptEntity->m20_position.vz.getIntegerPart() - pPlayerScriptEntity->m20_position.vz.getIntegerPart() + pTestedScriptEntity->m60[2];
                tempVec1.vy = pPlayerScriptEntity->m1E_collisionRadius + 0x20 + pTestedScriptEntity->m1E_collisionRadius;

                FP_VEC4 tempVec2;
                Square0(&tempVec1, &tempVec2);

                if ((((tempVec2.vx + tempVec2.vz < tempVec2.vy) && (pPlayerScriptEntity->m20_position.vy.getIntegerPart() - pPlayerScriptEntity->m18_boundingVolume.vy <= yDiff)) && ((int)(yDiff - (uint)(ushort)pTestedScriptEntity->m18_boundingVolume.vy) <= pPlayerScriptEntity->m20_position.vy.getIntegerPart())) && (actorId != playerEntityIndex)) {
                    tempVec1.vx = pTestedScriptEntity->m20_position.vx.getIntegerPart() - pPlayerScriptEntity->m20_position.vx.getIntegerPart() + pTestedScriptEntity->m60[0];
                    tempVec1.vz = pTestedScriptEntity->m20_position.vz.getIntegerPart() - pPlayerScriptEntity->m20_position.vz.getIntegerPart() + pTestedScriptEntity->m60[2];

                    Square0(&tempVec1, &tempVec2);

                    FP_VEC4 tempVec3;
                    FP_VEC4 tempVec4;
                    tempVec3.vx = pPlayerScriptEntity->m1E_collisionRadius + 8 + (uint)(ushort)pTestedScriptEntity->m1E_collisionRadius;
                    tempVec3.vz = pPlayerScriptEntity->m1E_collisionRadius + 0x20 + (uint)(ushort)pTestedScriptEntity->m1E_collisionRadius;
                    Square0(&tempVec3, &tempVec4);

                    s32 distance = tempVec2.vx + tempVec2.vz;
                    if (((tempVec4.vz <= distance) || ((padButtonForDialogs & controllerButtons::INTERACT) == 0)) || ((bTrigger || ((pTestedScriptEntity->m4_flags.m_rawFlags & 0x4000000) != 0)))) {
                        if (((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0xa20000) == 0) && (distance < tempVec4.vx)) {
                            scriptIndexToStart =  3;
                            newValueInFlags = 4;
                            int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                            int uVar7 = -(lVar5 >> 9);
                            pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | (uVar7 & 7) << 9;
                        }
                    }
                    else
                    {
                        int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                        int uVar7 = -lVar5 >> 9;
                        if ((0xa88 < ((pPlayerScriptEntity->m106_currentRotation & 0xfff) - (-lVar5 & 0xfffU) & 0xfff) - 700) && (windowOpenBF == 0)) {
                            bTrigger = true;
                            scriptIndexToStart = 2;
                            newValueInFlags = 3;
                            pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | (uVar7 & 7) << 9;
                        }
                    }
                }
            }
            else
            {
                if ((((pPlayerScriptEntity->m20_position.vy.getIntegerPart() - pPlayerScriptEntity->m18_boundingVolume.vy <= yDiff) && ((int)(yDiff - (uint)(ushort)pTestedScriptEntity->m18_boundingVolume.vy) <= pPlayerScriptEntity->m20_position.vy.getIntegerPart())) && (actorId != playerEntityIndex)) &&
                    (isPositionInEntityScriptBoundingVolume(pPlayerScriptEntity->m20_position.vx.getIntegerPart(), pPlayerScriptEntity->m20_position.vz.getIntegerPart(), pTestedScriptEntity, 0x10) == 0)) {
                    if ((((padButtonForDialogs & controllerButtons::INTERACT) == 0) || (bTrigger)) || ((pTestedScriptEntity->m4_flags.m_rawFlags & 0x4000000) != 0)) {
                        if ((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0xa20000) == 0) {
                            scriptIndexToStart = 3;
                            newValueInFlags = 4;
                            FP_VEC4 tempVec1;
                            tempVec1.vx = (pTestedScriptEntity->m20_position.vx.getIntegerPart() - pPlayerScriptEntity->m20_position.vx.getIntegerPart()) + (int)pTestedScriptEntity->m60[0];
                            tempVec1.vz = (pTestedScriptEntity->m20_position.vz.getIntegerPart() - pPlayerScriptEntity->m20_position.vz.getIntegerPart()) + (int)pTestedScriptEntity->m60[2];
                            int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                            pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | (-(lVar5 >> 9) & 7U) << 9;
                            if (fieldDebugDisable == 0) {
                                assert(0);
                            }
                        }
                    }
                    else {
                        if (((pTestedScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x220000) == 0) && (windowOpenBF == 0)) {
                            FP_VEC4 tempVec1;
                            tempVec1.vx = (pTestedScriptEntity->m20_position.vx.getIntegerPart() - pPlayerScriptEntity->m20_position.vx.getIntegerPart()) + (int)pTestedScriptEntity->m60[0];
                            tempVec1.vz = (pTestedScriptEntity->m20_position.vz.getIntegerPart() - pPlayerScriptEntity->m20_position.vz.getIntegerPart()) + (int)pTestedScriptEntity->m60[2];
                            int lVar5 = ratan2(tempVec1.vz, tempVec1.vx);
                            if (((pTestedScriptEntity->m4_flags.m_rawFlags & 0x40000) == 0) || (0xa88 < ((pPlayerScriptEntity->m106_currentRotation & 0xfff) - (-lVar5 & 0xfffU) & 0xfff) - 700)) {
                                bTrigger = true;
                                scriptIndexToStart = 2;
                                newValueInFlags = 3;
                                pTestedScriptEntity->m12C_flags = pTestedScriptEntity->m12C_flags & 0xfffff1ff | (-lVar5 >> 9 & 7U) << 9;
                                if (fieldDebugDisable == 0) {
                                    assert(0);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (scriptIndexToStart != -1) {
            int foundSlot = 0;
            for (foundSlot = 0; foundSlot < 8; foundSlot++)
            {
                if (pTestedScriptEntity->m8C_scriptSlots[foundSlot].m3_scriptIndex == scriptIndexToStart)
                    break;
            }

            if (foundSlot == 8) // script wasn't running already, start it
            {
                for (int slotId = 0; slotId < 8; slotId++)
                {
                    if (((pTestedScriptEntity->m8C_scriptSlots[slotId].m4_flags.m18) == 0xf) && ((pTestedScriptEntity->m8C_scriptSlots[slotId].m4_flags.m22) == 0)) {
                        pTestedScriptEntity->m8C_scriptSlots[slotId].m0_scriptPC = getScriptEntryPoint(actorId, scriptIndexToStart);
                        pTestedScriptEntity->m8C_scriptSlots[slotId].m3_scriptIndex = scriptIndexToStart;
                        pTestedScriptEntity->m8C_scriptSlots[slotId].m4_flags.m18 = newValueInFlags;
                        pTestedScriptEntity->m106_currentRotation |= 0x8000;
                        pTestedScriptEntity->m104_rotation = pTestedScriptEntity->m106_currentRotation;
                        break;
                    }
                }
            }
        }
    }
}

int EntityMoveCheck0Sub1(int actorId, sSpriteActor* param_2, int stepX, int stepZ, int* param_5, FP_VEC4* outputNormal)
{
    MissingCode();
    return -1;
}

void EntityMoveCheck0(uint playerEntityIndex, sFieldEntity* pPlayerEntity, sFieldScriptEntity* pPlayerScriptEntity)
{
    std::array<FP_VEC4, 8> scratchBuffer; // this would normally be allocated in the cpu scratch buffer
    std::array<FP_VEC4, 8>::iterator pScratchBuffer = scratchBuffer.begin();

    bool finalState = false;

    FP_VEC3 postStepPosition;
    postStepPosition.vx = (pPlayerScriptEntity->m20_position).vx + (pPlayerScriptEntity->m30_stepVector).vx;
    postStepPosition.vy = (pPlayerScriptEntity->m20_position).vy + (pPlayerScriptEntity->m30_stepVector).vy;
    postStepPosition.vz = (pPlayerScriptEntity->m20_position).vz + (pPlayerScriptEntity->m30_stepVector).vz;

    SFP_VEC3 SFP_postStepPosition;
    Vec3ToSVec3(&SFP_postStepPosition, &postStepPosition);

    s32 SFPStepAsInts[3];
    SFPStepAsInts[0] = (int)SFP_postStepPosition.vx;
    SFPStepAsInts[2] = (int)SFP_postStepPosition.vz;

    sFieldScriptEntity_flags0 playerFlags = pPlayerScriptEntity->m0_fieldScriptFlags;

    u32 bestDistance = 0x7fffffff;
    s32 testedEntityYWithOffset;

    u32 playerFlags40800 = playerFlags.m_rawFlags & 0x40800;

    s32 playerY = pPlayerScriptEntity->m20_position.vy.getIntegerPart();
    s32 testedEntityY;
    s16 initialActor18Y = pPlayerScriptEntity->m18_boundingVolume.vy;
    s32 finalCount;
    s8 playerVar74 = pPlayerScriptEntity->m74;

    for (int actorId=0; actorId < totalActors; actorId++)
    {
        if (actorId == playerEntityIndex)
            continue;

        sFieldScriptEntity* pCurrentFieldScriptEntity = actorArray[actorId].m4C_scriptEntity;
        sFieldScriptEntity_flags0 entityFlags = pCurrentFieldScriptEntity->m0_fieldScriptFlags;

        if (entityFlags.m0_updateScriptDisabled != 0)
            continue;

        pCurrentFieldScriptEntity->m4_flags.m_rawFlags &= ~0xC100;
        if ((pCurrentFieldScriptEntity->m4_flags.m_rawFlags & 0x80) == 0)
        {
            if ((entityFlags.m_rawFlags & 0x2000) == 0)
            {
                pScratchBuffer->vx = (((pCurrentFieldScriptEntity->m20_position).vx + (pCurrentFieldScriptEntity->m30_stepVector).vx) >> 0x10) - SFPStepAsInts[0];
                pScratchBuffer->vz = (((pCurrentFieldScriptEntity->m20_position).vz + (pCurrentFieldScriptEntity->m30_stepVector).vz) >> 0x10) - SFPStepAsInts[2];
                pScratchBuffer->vy = pPlayerScriptEntity->m1E_collisionRadius + pCurrentFieldScriptEntity->m1E_collisionRadius;
                Square0(&pScratchBuffer[0], &pScratchBuffer[1]);
                if (pScratchBuffer[1].vx + pScratchBuffer[1].vz < pScratchBuffer[1].vy) // player is in collision radius
                    goto LAB_Field__80084438;
                pCurrentFieldScriptEntity->m4_flags.m_rawFlags &= ~0xC00000;
            }
            else
            {
                testedEntityY = isPositionInEntityScriptBoundingVolume(SFPStepAsInts[0], SFPStepAsInts[2], pCurrentFieldScriptEntity, 0);
                if (testedEntityY == 0) {
                LAB_Field__80084438:
                    if ((pPlayerScriptEntity->m14_currentTriangleFlag & 0x400000U) == 0) {
                        if ((((pCurrentFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags | playerFlags.m_rawFlags) & 0x80) == 0) && (noUpdatesToPartyMemebers == 0)) {
                            testedEntityY = pCurrentFieldScriptEntity->m20_position.vy.getIntegerPart();
                            testedEntityYWithOffset = testedEntityY - pCurrentFieldScriptEntity->m18_boundingVolume.vy;
                            goto LAB_Field__800844b8;
                        }
                    }
                    else {
                        if (fieldDebugDisable == 0) {
                            assert(0);
                        }
                    }
                    continue;
                }
                pCurrentFieldScriptEntity->m4_flags.m_rawFlags &= ~0xC00000;
            }
        }
        else
        {
            FP_VEC4 local_98;
            testedEntityY = EntityMoveCheck0Sub1(actorId, actorArray[actorId].m4_pVramSpriteSheet, SFPStepAsInts[0], SFPStepAsInts[2], &testedEntityYWithOffset, &local_98);
            if (testedEntityY != 0) {
                pCurrentFieldScriptEntity->m4_flags.m_rawFlags = pCurrentFieldScriptEntity->m4_flags.m_rawFlags & 0xff3fffff;
                continue;
            }
            if (fieldDebugDisable == 0) {
                assert(0);
            }
            pCurrentFieldScriptEntity->m4_flags.m_rawFlags = pCurrentFieldScriptEntity->m4_flags.m_rawFlags | 0x100;
            testedEntityY = testedEntityYWithOffset + (uint)(ushort)pCurrentFieldScriptEntity->m18_boundingVolume.vy;
            if ((byte)pPlayerScriptEntity->m74 != actorId) {
                testedEntityY = testedEntityYWithOffset + testedEntityY;
            }
            (pPlayerScriptEntity->m50_surfaceNormal).vx = local_98[0];
            (pPlayerScriptEntity->m50_surfaceNormal).vy = local_98[1];
            (pPlayerScriptEntity->m50_surfaceNormal).vz = local_98[2];
            pCurrentFieldScriptEntity->m4_flags.m_rawFlags |= 0x4000;

LAB_Field__800844b8:
            if ((pPlayerScriptEntity->m74 == actorId) && (playerFlags40800 == 0)) {
LAB_Field__80084520:
                pCurrentFieldScriptEntity->m4_flags.m_rawFlags = pCurrentFieldScriptEntity->m4_flags.m_rawFlags | 0x800000;
                (pPlayerScriptEntity->m40).vx = (pCurrentFieldScriptEntity->m30_stepVector).vx;
                (pPlayerScriptEntity->m40).vy = (pCurrentFieldScriptEntity->m30_stepVector).vy;
                finalCount = 2;
                (pPlayerScriptEntity->m40).vz = (pCurrentFieldScriptEntity->m30_stepVector).vz;
                bestDistance = testedEntityYWithOffset;
                if (playerFlags40800 == 0) {
                    pPlayerScriptEntity->m74 = (byte)actorId;
                    finalState = true;
                }
            }
            else
            {
                if ((testedEntityY < (int)(playerY - (uint)(ushort)initialActor18Y)) || (playerY < testedEntityYWithOffset)) {
                    u32 tempFlags = pCurrentFieldScriptEntity->m4_flags.m_rawFlags;
                    pCurrentFieldScriptEntity->m4_flags.m_rawFlags = tempFlags & 0xfffffeff;
                    if (playerY < testedEntityYWithOffset) {
                        pCurrentFieldScriptEntity->m4_flags.m_rawFlags = tempFlags & 0xfffffeff | 0x800000;
                        if (testedEntityYWithOffset < bestDistance) {
                            bestDistance = testedEntityYWithOffset;
                        }
                    }
                    else {
                        pCurrentFieldScriptEntity->m4_flags.m_rawFlags = tempFlags & 0xff7ffeff;
                    }
                }
                else
                {
                    if ((playerY < testedEntityYWithOffset + 0x10) || ((pCurrentFieldScriptEntity->m4_flags.m_rawFlags & 0x800000) != 0))
                        goto LAB_Field__80084520;
                    if ((entityFlags.m_rawFlags & 0x10) == 0) {
                        assert(0);
                    }
                    (pCurrentFieldScriptEntity->m40).vx = 0;
                    (pCurrentFieldScriptEntity->m40).vy = 0;
                    (pCurrentFieldScriptEntity->m40).vz = 0;
                    (pCurrentFieldScriptEntity->m30_stepVector).vx = 0;
                    (pCurrentFieldScriptEntity->m30_stepVector).vy = 0;
                    (pCurrentFieldScriptEntity->m30_stepVector).vz = 0;
                    (pPlayerScriptEntity->m30_stepVector).vx = 0;
                    (pPlayerScriptEntity->m30_stepVector).vy = 0;
                    (pPlayerScriptEntity->m30_stepVector).vz = 0;
                    (pPlayerScriptEntity->m40).vx = 0;
                    (pPlayerScriptEntity->m40).vy = 0;
                    (pPlayerScriptEntity->m40).vz = 0;
                }
            }
            pCurrentFieldScriptEntity->m4_flags.m_rawFlags |= 0x400000;
        }
    }

    if (updateEntityEventCode3Var1 != 0) {
        finalState = false;
        finalCount = finalCount + 1;
        bestDistance = EntityMoveCheck0Var0;
    }
    if (finalState) {
        (actorArray[pPlayerScriptEntity->m74].m4C_scriptEntity)->m4_flags.m_rawFlags = (actorArray[pPlayerScriptEntity->m74].m4C_scriptEntity)->m4_flags.m_rawFlags | 0x8000;
        if (playerVar74 == -1) {
            if ((pPlayerScriptEntity->m134.m7_isModelRotation2Allocated) == 0) {
                pPlayerScriptEntity->m110_modelRotation2.resize(6);
                pPlayerScriptEntity->m134.m7_isModelRotation2Allocated = 1;
            }
            pPlayerScriptEntity->m110_modelRotation2[0] = actorArray[pPlayerScriptEntity->m74].m50_modelRotation[0];
            pPlayerScriptEntity->m110_modelRotation2[1] = actorArray[pPlayerScriptEntity->m74].m50_modelRotation[1];
            pPlayerScriptEntity->m110_modelRotation2[2] = actorArray[pPlayerScriptEntity->m74].m50_modelRotation[2];
            pPlayerScriptEntity->m110_modelRotation2[4] = computeDistanceBetweenActors(playerEntityIndex, pPlayerScriptEntity->m74);
        }
    }
    else {
        pPlayerScriptEntity->m74 = -1;
    }

    MissingCode();
    if (((pPlayerScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10000) == 0) && ((pPlayerScriptEntity->m4_flags.m_rawFlags & 0x200000) == 0)) {
        EntityMoveCheck1(playerEntityIndex, bestDistance, pPlayerEntity, pPlayerScriptEntity, 0);
    }
    if ((actorArray[playerEntityIndex].m4_pVramSpriteSheet)->m7C->mC == 1) {
        resetInputs();
        pPlayerScriptEntity->m0_fieldScriptFlags.mx800_isJumping = 0;
    }

    MissingCode();
}

struct sEntityMoveCheck1StackStruct
{
    std::array<s32, 4> m0_altitude;
    std::array<s32, 4> m10_altitude2;
    std::array<s32, 4> m20_walkmeshId;
    std::array<s16, 4> m30_triangleId;
    std::array<FP_VEC4, 4> m38_triangleNormal;
    std::array<s32, 4> m78_position;
    std::array<s16, 4> m88;
};

void EntityMoveCheck1Sub1Sub1(int* param_1, int* param_2, int param_3)

{
    int iVar1;

    iVar1 = *param_1;
    if (param_3 < 0) {
        *param_1 = iVar1;
    }
    else {
        *param_1 = iVar1;
        iVar1 = iVar1 + param_3;
    }
    *param_2 = iVar1;
    return;
}

s32 EntityMoveCheck1Sub1(sFieldScriptEntity* pFieldScriptEntity, int walkmeshId, s32* pOutputAltitude, FP_VEC4* triangleNormal, s16* pOutputTriangleId, s32* pOutputAltitude2)
{
    if (pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId] == -1) {
        return -1;
    }

    int triangleId = pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId];

    sVec2_s16 refPos;
    sVec2_s16 refPos2;

    SFP_VEC4 testedPosition;
    testedPosition.vx = (pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16;
    testedPosition.vy = 0;
    testedPosition.vz = (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16;

    refPos.set((pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16, (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16);
    refPos2.set(pFieldScriptEntity->m20_position.vx >> 16, pFieldScriptEntity->m20_position.vz >> 16);

    int mask = 0;
    if ((pFieldScriptEntity->m4_flags.m_rawFlags >> (walkmeshId + 3U & 0x1f) & 1) == 0) {
        mask = -(s32)(noUpdatesToPartyMemebers == '\0');
    }

    int oldWalkmeshId = walkmeshId;

    int iterationCount = 0;
    for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
    {
        sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];
        std::vector<sWalkMesh::sTriangleData>::iterator pWalkMeshTriangles = walkMeshTriangle[walkmeshId]->begin();
        std::vector<SFP_VEC4>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

        sVec2_s16 vert0;
        sVec2_s16 vert1;
        sVec2_s16 vert2;

        vert0.set(pVertices[pTriangle.m0_verticeIndex[0]].vx, pVertices[pTriangle.m0_verticeIndex[0]].vz);
        vert1.set(pVertices[pTriangle.m0_verticeIndex[1]].vx, pVertices[pTriangle.m0_verticeIndex[1]].vz);
        vert2.set(pVertices[pTriangle.m0_verticeIndex[2]].vx, pVertices[pTriangle.m0_verticeIndex[2]].vz);

        int collisionFlag = 0;
        if (NCLIP(vert0, vert1, refPos) < 0) {
            collisionFlag |= 1;
        }

        if (NCLIP(vert1, vert2, refPos) < 0) {
            collisionFlag |= 2;
        }

        if (NCLIP(vert2, vert0, refPos) < 0) {
            collisionFlag |= 4;
        }

        if (collisionFlag < 8) {
            switch (collisionFlag)
            {
            case 0:
                iterationCount = 0xFF;
                break;
            case 1:
                triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                break;
            case 2:
                triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                break;
            case 3:
                if (NCLIP(vert1, refPos, refPos2) < 0)
                {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                }
                else {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                }
                break;
            case 4:
                triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                break;
            case 5:
                if (NCLIP(vert0, refPos, refPos2) < 0)
                {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                }
                else {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
                }
                break;
            case 6:
                if (NCLIP(vert2, refPos, refPos2) < 0)
                {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
                }
                else {
                    triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
                }
                break;
            case 7:
                triangleId = -1;
                break;
            default:
                assert(0);
            }
        }

        if (triangleId == -1)
        {
            return -1;
        }
    }
    if (iterationCount == 0x20)
    {
        return -1;
    }

    std::vector<SFP_VEC4>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();
    sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];

    getTriangleNormalAndAdjustY(pVertices[pTriangle.m0_verticeIndex[0]], pVertices[pTriangle.m0_verticeIndex[1]], pVertices[pTriangle.m0_verticeIndex[2]], &testedPosition, triangleNormal);

    *pOutputTriangleId = triangleId;

    int triangleParamD = pTriangle.mD * 4;
    if (triangleParamD < 0)
    {
        triangleParamD = 0;
    }

    if (pFieldScriptEntity->m10_walkmeshId == oldWalkmeshId)
    {
        if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
        {
            *pOutputAltitude = 0x7fffffff;
            *pOutputAltitude2 = 0x7fffffff;
            return 0;
        }

        if (((pFieldScriptEntity->m30_stepVector.vx == 0) && (pFieldScriptEntity->m30_stepVector.vy == 0)) && (pFieldScriptEntity->m30_stepVector.vz == 0)) {
            *pOutputAltitude = testedPosition.vy;
        }
        else
        {
            *pOutputAltitude = pFieldScriptEntity->m72_elevation;
        }
    }
    else {
        if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
        {
            *pOutputAltitude = 0x7fffffff;
            *pOutputAltitude2 = 0x7fffffff;
            return 0;
        }

        *pOutputAltitude = testedPosition.vy;
    }

    EntityMoveCheck1Sub1Sub1(pOutputAltitude, pOutputAltitude2, triangleParamD);
    return 0;
}


struct sFollowStruct {
    sFieldScriptEntity_flags0 m0_flags;
    sFieldScriptEntity_flags4 m4_flags;
    SFP_VEC4 m8_pos;
    s16 m10;
    s16 m12_animationId;
    u16 m14_rotation;
    std::array<s16, 4> m16_walkMeshTriangles;
    FP_VEC3 m20;
    FP_VEC4 m30;
    s32 m40_currentTriangleFlag;
    s8 m44_walkmeshId;
};
std::array<sFollowStruct, 32> followDataTable;

s16 playerIsntPartyLeader = 0;
std::array<s32, 3> partyToFollowStructMapping = { {0,0,0} };

void initFollowStructForPlayer(s32 entityIndex) {
    sFieldScriptEntity* pScriptEntity = actorArray[entityIndex].m4C_scriptEntity;
    sSpriteActor* pSpriteActor = actorArray[entityIndex].m4_pVramSpriteSheet;
    if (entityIndex == playerControlledActor) {
        if (!noUpdatesToPartyMemebers) {
            followDataTable[partyToFollowStructMapping[0]].m20.vx = (pSpriteActor->m0_spriteActorCore).mC_step.vx;
            followDataTable[partyToFollowStructMapping[0]].m20.vy = (pSpriteActor->m0_spriteActorCore).mC_step.vy;
            followDataTable[partyToFollowStructMapping[0]].m20.vz = (pSpriteActor->m0_spriteActorCore).mC_step.vz;
            followDataTable[partyToFollowStructMapping[0]].m30.vx = (pScriptEntity->m50_surfaceNormal).vx;
            followDataTable[partyToFollowStructMapping[0]].m30.vy = (pScriptEntity->m50_surfaceNormal).vy;
            followDataTable[partyToFollowStructMapping[0]].m30.vz = (pScriptEntity->m50_surfaceNormal).vz;
            followDataTable[partyToFollowStructMapping[0]].m14_rotation = pScriptEntity->m106_currentRotation & 0xfff;
            followDataTable[partyToFollowStructMapping[0]].m10 = (pSpriteActor->m0_spriteActorCore).m84_maxY;
            followDataTable[partyToFollowStructMapping[0]].m8_pos[0] = (pScriptEntity->m20_position).vx.getIntegerPart();
            followDataTable[partyToFollowStructMapping[0]].m8_pos[1] = (pScriptEntity->m20_position).vy.getIntegerPart();
            followDataTable[partyToFollowStructMapping[0]].m8_pos[2] = (pScriptEntity->m20_position).vz.getIntegerPart();
            followDataTable[partyToFollowStructMapping[0]].m12_animationId = pScriptEntity->mE8_currentAnimationId;
            followDataTable[partyToFollowStructMapping[0]].m40_currentTriangleFlag = pScriptEntity->m14_currentTriangleFlag;
            followDataTable[partyToFollowStructMapping[0]].m0_flags = pScriptEntity->m0_fieldScriptFlags;
            followDataTable[partyToFollowStructMapping[0]].m4_flags = pScriptEntity->m4_flags;

            for (int i = 0; i < 4; i++) {
                followDataTable[partyToFollowStructMapping[0]].m16_walkMeshTriangles[i] = pScriptEntity->m8_currentWalkMeshTriangle[i];
            }

            followDataTable[partyToFollowStructMapping[0]].m44_walkmeshId = (char)pScriptEntity->m10_walkmeshId;
            entityUpdateVar0 = 0;
            partyToFollowStructMapping[0] = partyToFollowStructMapping[0] - 1U & 0x1f;
        }
    }
}

int EntityMoveCheck1(int entityIndex, int maxAltitude, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity, uint param_5)
{
    sSpriteActor* pSpriteActor = actorArray[entityIndex].m4_pVramSpriteSheet;
    if (entityIndex == playerControlledActor) {
        inputAllowedMask = 0xFFFF;
    }
    if ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1000000) != 0) {
        return -1;
    }
    if ((pFieldScriptEntity->m4_flags.m_rawFlags & 0x200000) != 0) {
        return -1;
    }
    if (((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10000) != 0) ||
        ((((entityIndex != playerControlledActor || (EntityMoveCheck1Var1 != '\x01')) && ((pSpriteActor->mC_step).vy == 0)) &&
            (!updateEntityEventCode3Sub0(pFieldScriptEntity) && (pSpriteActor->m84_maxY == (pFieldScriptEntity->m20_position.vy >> 16)))))) {
        return -1;
    }

    sEntityMoveCheck1StackStruct stackVar;

    stackVar.m78_position[0] = pFieldScriptEntity->m20_position.vx;
    stackVar.m78_position[1] = pFieldScriptEntity->m20_position.vy;
    stackVar.m78_position[2] = pFieldScriptEntity->m20_position.vz;

    int sVar2 = pFieldScriptEntity->m10_walkmeshId;

    for (int i = 0; i < 4; i++)
    {
        stackVar.m88[i] = pFieldScriptEntity->m8_currentWalkMeshTriangle[i];
    }

    for (int i = 0; i < 4; i++)
    {
        stackVar.m20_walkmeshId[i] = i;
        stackVar.m0_altitude[i] = 0x7fffffff;
        stackVar.m10_altitude2[i] = 0x7fffffff;
    }

    int evaluatedWalkMeshes = 0;
    for (evaluatedWalkMeshes = 0; evaluatedWalkMeshes < numWalkMesh - 1; evaluatedWalkMeshes++)
    {
        if (EntityMoveCheck1Sub1(pFieldScriptEntity, evaluatedWalkMeshes, &stackVar.m0_altitude[evaluatedWalkMeshes], &stackVar.m38_triangleNormal[evaluatedWalkMeshes], &stackVar.m30_triangleId[evaluatedWalkMeshes], &stackVar.m10_altitude2[evaluatedWalkMeshes]))
        {
            break;
        }
    }

    if ((pFieldScriptEntity->m4_flags.m_rawFlags & 1) != 0) {
        stackVar.m0_altitude[0] = 0x7fffffff;
        stackVar.m10_altitude2[0] = 0x7fffffff;
    }
    if ((pFieldScriptEntity->m4_flags.m_rawFlags & 2) != 0) {
        stackVar.m0_altitude[1] = 0x7fffffff;
        stackVar.m10_altitude2[1] = 0x7fffffff;
    }
    if ((pFieldScriptEntity->m4_flags.m_rawFlags & 4) != 0) {
        stackVar.m0_altitude[2] = 0x7fffffff;
        stackVar.m10_altitude2[2] = 0x7fffffff;
    }

    int altitudeOfCurrentWalkMesh = stackVar.m0_altitude[pFieldScriptEntity->m10_walkmeshId];
    {
        // sort by altitude
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (stackVar.m0_altitude[j + 1] < stackVar.m0_altitude[j + 0]) {
                    std::swap<s32>(stackVar.m0_altitude[j + 1], stackVar.m0_altitude[j + 0]);
                    std::swap<s32>(stackVar.m10_altitude2[j + 1], stackVar.m10_altitude2[j + 0]);
                    std::swap<s32>(stackVar.m20_walkmeshId[j + 1], stackVar.m20_walkmeshId[j + 0]);
                }
            }
        }
    }

    if (evaluatedWalkMeshes == numWalkMesh - 1)
    {
        if (evaluatedWalkMeshes > 0)
        {
            for (int i = 0; i < numWalkMesh - 1; i++)
            {
                pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = stackVar.m30_triangleId[i];
            }
        }

        int foundEntry;
        if (((pFieldScriptEntity->m20_position.vy.getIntegerPart()) < altitudeOfCurrentWalkMesh) || ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1800) != 0)) //0x402443
        {
            for (foundEntry = 0; foundEntry < numWalkMesh - 1; foundEntry++)
            {
                if ((pFieldScriptEntity->m20_position.vy.getIntegerPart()) <= stackVar.m0_altitude[foundEntry]) {
                    if (foundEntry != 0)
                    {
                        pFieldScriptEntity->m10_walkmeshId = stackVar.m20_walkmeshId[foundEntry];
                    }
                    break;
                }
            }
        }
        else
        {
            for (foundEntry = 0; foundEntry < numWalkMesh - 1; foundEntry++)
            {
                if (pFieldScriptEntity->m10_walkmeshId == stackVar.m20_walkmeshId[foundEntry])
                    break;
            }
        }

        if ((((getWalkmeshTriangleFlag(pFieldScriptEntity) & 4) != 0) && (foundEntry != 0)) && ((int)pFieldScriptEntity->m10_walkmeshId <= numWalkMesh + -1)) {
            pFieldScriptEntity->m10_walkmeshId = stackVar.m20_walkmeshId[0];
        }

        if ((pFieldScriptEntity->m0_fieldScriptFlags.m8 & getWalkmeshTriangleFlag(pFieldScriptEntity) >> 5) == 0) {
            if ((getWalkmeshTriangleFlag(pFieldScriptEntity) & 0x800000) == 0) {
                (pFieldScriptEntity->m20_position).vx = (pFieldScriptEntity->m20_position).vx + (pFieldScriptEntity->m30_stepVector).vx;
                (pFieldScriptEntity->m20_position).vz = (pFieldScriptEntity->m20_position).vz + (pFieldScriptEntity->m30_stepVector).vz;

                for (int i = 0; i < numWalkMesh + -1; i++)
                {
                    if (pFieldScriptEntity->m10_walkmeshId == stackVar.m20_walkmeshId[i]) {
                        pSpriteActor->m84_maxY = stackVar.m0_altitude[i];
                        break;
                    }
                }

                VectorNormal(&stackVar.m38_triangleNormal[pFieldScriptEntity->m10_walkmeshId], &pFieldScriptEntity->m50_surfaceNormal);
                goto LAB_Field__80084fd0;
            }
            if (fieldDebugDisable == 0) {
                assert(0);
            }
        }
        else {
            if (fieldDebugDisable == 0) {
                assert(0);
            }
        }

        // disable the player's directional arrow for movement
        if (entityIndex == playerControlledActor) {
            inputAllowedMask = 0xfff;
        }

        (pFieldScriptEntity->m20_position).vx = stackVar.m78_position[0];
        (pFieldScriptEntity->m20_position).vy += pSpriteActor->mC_step.vy;
        (pFieldScriptEntity->m20_position).vz = stackVar.m78_position[2];

        pFieldScriptEntity->m10_walkmeshId = sVar2;
        pFieldScriptEntity->mF0 = 0;

        for (int i = 0; i < 4; i++)
        {
            pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = stackVar.m88[i];
        }

        if ((pFieldScriptEntity->m20_position.vy >> 16) < pSpriteActor->m84_maxY) {
            if (pSpriteActor->m84_maxY != sVar2) {
                (pSpriteActor->mC_step).vy += pSpriteActor->m1C_gravity;
            }
        }
        else {
            if (0 < (pSpriteActor->mC_step).vy) {
                (pSpriteActor->mC_step).vy = 0;
            }
            pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags &= ~0x401000;
            (pFieldScriptEntity->m20_position).vy = (int)pSpriteActor->m84_maxY << 0x10;
        }
        (pSpriteActor->m0_position).vx = (pFieldScriptEntity->m20_position).vx;
        (pSpriteActor->m0_position).vy = (pFieldScriptEntity->m20_position).vy;
        (pSpriteActor->m0_position).vz = (pFieldScriptEntity->m20_position).vz;

        actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
    }
    else {
        pFieldScriptEntity->mF0 = 0;

    LAB_Field__80084fd0:
        if (updateEntityEventCode3Var1 == 0) {
            if (param_5 != 0) {
                if (actorArray[entityIndex].m4_pVramSpriteSheet->m84_maxY < maxAltitude + 10) {
                    pFieldScriptEntity->m74 = 0xff;
                }
                actorArray[entityIndex].m4_pVramSpriteSheet->m84_maxY = maxAltitude;
                (pFieldScriptEntity->m20_position).vy = maxAltitude << 0x10;
            }
        }
        else {
            if (param_5 < 2) {
                actorArray[entityIndex].m4_pVramSpriteSheet->m84_maxY = maxAltitude;
            }
        }

        auto psVar18 = actorArray[entityIndex].m4_pVramSpriteSheet;
        if ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x40000) != 0) {
            (pFieldScriptEntity->m20_position).vy = (int)pFieldScriptEntity->mEC_elevation << 0x10;
            (psVar18->mC_step).vy = 0;
        }
        (pFieldScriptEntity->m20_position).vy = (pFieldScriptEntity->m20_position).vy + (psVar18->mC_step).vy;
        int uVar6 = getWalkmeshTriangleFlag(pFieldScriptEntity);
        if (pFieldScriptEntity->m10_walkmeshId != sVar2) {
            pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags &= ~0x4000000;
        }
        if ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x4000000) == 0) {
            int sVar1 = (pFieldScriptEntity->m20_position).vy >> 16;
            if (psVar18->m84_maxY <= sVar1) goto LAB_Field__80085104;
            if (psVar18->m84_maxY != sVar1) {
                (psVar18->mC_step).vy += psVar18->m1C_gravity; // this apply gravity when falling
            }
            pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags |= 0x1000;
            pFieldScriptEntity->mF0 = (psVar18->mC_step).vy;
        }
        else {
        LAB_Field__80085104:
            if ((uVar6 & 0x420000) == 0) {
                pFieldScriptEntity->mF0 = 0;
            }
            if (0 < (psVar18->mC_step).vy) {
                (psVar18->mC_step).vy = 0;
            }
            pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags &= ~0x401000;
            (pFieldScriptEntity->m20_position).vy = (int)psVar18->m84_maxY << 0x10;
        }
        pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags &= ~0x4000000;
        int foundEntry = 0;
        if (0 < numWalkMesh + -1) {
            altitudeOfCurrentWalkMesh = pFieldScriptEntity->m20_position.vy >> 16;
            int psVar8 = 0;
            do {
                if (((stackVar.m0_altitude[psVar8] < altitudeOfCurrentWalkMesh) && ((int)(altitudeOfCurrentWalkMesh - (uint)(ushort)pFieldScriptEntity->m18_boundingVolume.vy) < stackVar.m10_altitude2[psVar8])) && (stackVar.m0_altitude[psVar8] != stackVar.m10_altitude2[0])) break;
                foundEntry = foundEntry + 1;
                psVar8++;
            } while (foundEntry < numWalkMesh + -1);
        }
        int foundEntryTriangle;
        if ((foundEntry == numWalkMesh + -1) &&
            ((foundEntryTriangle = (*walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId])[pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId]].mD * 4, -1 < foundEntryTriangle ||
                (foundEntryTriangle + psVar18->m84_maxY <= ((pFieldScriptEntity->m20_position.vy >> 16) - (uint)(ushort)pFieldScriptEntity->m18_boundingVolume.vy))))) {
            psVar18->m0_position.vx = (pFieldScriptEntity->m20_position).vx;
            psVar18->m0_position.vy = (pFieldScriptEntity->m20_position).vy;
            psVar18->m0_position.vz = (pFieldScriptEntity->m20_position).vz;
            actorArray[entityIndex].mC_matrix.t[0] = pFieldScriptEntity->m20_position.vx >> 16;
            actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
            actorArray[entityIndex].mC_matrix.t[2] = pFieldScriptEntity->m20_position.vz >> 16;
            pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);
        }
        else {
            (pFieldScriptEntity->m20_position).vx = stackVar.m78_position[0];
            (pFieldScriptEntity->m20_position).vz = stackVar.m78_position[2];

            pFieldScriptEntity->m10_walkmeshId = sVar2;
            pFieldScriptEntity->mF0 = 0;

            for (int i = 0; i < 4; i++)
            {
                pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = stackVar.m88[i];
            }

            if (pSpriteActor->m84_maxY != (pFieldScriptEntity->m20_position.vy.getIntegerPart())) {
                (pSpriteActor->mC_step).vy += pSpriteActor->m1C_gravity;
            }

            if ((pSpriteActor->mC_step).vy < 0) {
                (pSpriteActor->mC_step).vy = 0;
                (pFieldScriptEntity->m20_position).vy = stackVar.m78_position[1];
            }

            (pSpriteActor->m0_position).vx = (pFieldScriptEntity->m20_position).vx;
            (pSpriteActor->m0_position).vy = (pFieldScriptEntity->m20_position).vy;
            (pSpriteActor->m0_position).vz = (pFieldScriptEntity->m20_position).vz;
            actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
        }
    }

    initFollowStructForPlayer(entityIndex);
    return 0;
}

void updatePartyFollowLeader() {
    if (!playerIsntPartyLeader) {
        for (int i = 0; i < totalActors; i++) {
            sFieldEntity* pActor = &actorArray[i];
            sFieldScriptEntity* pScriptEntity = pActor->m4C_scriptEntity;
            if ((((pScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1000000) != 0) && (i != playerControlledActor)) && ((pActor->m58_flags & 0x20) == 0)) {
                s32 partyPosition = findCharacterInParty(pScriptEntity->mE4_playableCharacterId);
                if (partyPosition != -1) {
                    std::array<s32, 3>::iterator puVar9 = partyToFollowStructMapping.begin() + partyPosition;
                    updateEntityEventCode3Sub1(pActor->m4_pVramSpriteSheet, followDataTable[*puVar9].m14_rotation, pActor);
                    sFieldScriptEntity_flags0 uVar7 = followDataTable[*puVar9].m0_flags;
                    if (EntityMoveCheck1Var1 == 1) {
                        *puVar9 = partyToFollowStructMapping[0] + 1U & 0x1f;
                    LAB_Field__80081920:

                        if ((uVar7.m_rawFlags & 0x800) == 0) {
                            uVar7.m_rawFlags = pScriptEntity->m0_fieldScriptFlags.m_rawFlags & ~0x800;
                        }
                        else {
                            uVar7.m_rawFlags = pScriptEntity->m0_fieldScriptFlags.m_rawFlags | 0x800;
                        }
                        pScriptEntity->m0_fieldScriptFlags = uVar7;

                        s32 sVar1 = followDataTable[partyToFollowStructMapping[partyPosition]].m12_animationId;
                        if (pScriptEntity->mE8_currentAnimationId != sVar1) {
                            pScriptEntity->mE8_currentAnimationId = sVar1;
                            if (sVar1 < 0) {
                                pScriptEntity->mE8_currentAnimationId = 0;
                            }
                            setVisualAnimation(pActor->m4_pVramSpriteSheet, (int)pScriptEntity->mE8_currentAnimationId, pActor);
                        }

                        for (int i = 0; i < 4; i++) {
                            pScriptEntity->m8_currentWalkMeshTriangle[i] = followDataTable[partyToFollowStructMapping[partyPosition]].m16_walkMeshTriangles[i];
                        }

                        pScriptEntity->m10_walkmeshId = followDataTable[partyToFollowStructMapping[partyPosition]].m44_walkmeshId;
                        (pScriptEntity->m50_surfaceNormal).vx = followDataTable[partyToFollowStructMapping[partyPosition]].m30.vx;
                        (pScriptEntity->m50_surfaceNormal).vy = followDataTable[partyToFollowStructMapping[partyPosition]].m30.vy;
                        (pScriptEntity->m50_surfaceNormal).vz = followDataTable[partyToFollowStructMapping[partyPosition]].m30.vz;
                        (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).mC_step.vx = followDataTable[partyToFollowStructMapping[partyPosition]].m20.vx;
                        (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).mC_step.vy = followDataTable[partyToFollowStructMapping[partyPosition]].m20.vy;
                        (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).mC_step.vz = followDataTable[partyToFollowStructMapping[partyPosition]].m20.vz;
                        (pActor->mC_matrix).t[0] = followDataTable[partyToFollowStructMapping[partyPosition]].m8_pos.vx;
                        (pActor->mC_matrix).t[1] = followDataTable[partyToFollowStructMapping[partyPosition]].m8_pos.vy;
                        (pActor->mC_matrix).t[2] = followDataTable[partyToFollowStructMapping[partyPosition]].m8_pos.vz;
                        (pScriptEntity->m20_position).vx = (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).m0_position.vx = (pActor->mC_matrix).t[0] << 0x10;
                        (pScriptEntity->m20_position).vy = (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).m0_position.vy = (pActor->mC_matrix).t[1] << 0x10;
                        (pScriptEntity->m20_position).vz = (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).m0_position.vz = (pActor->mC_matrix).t[2] << 0x10;
                        (pActor->m4_pVramSpriteSheet->m0_spriteActorCore).m84_maxY = followDataTable[partyToFollowStructMapping[partyPosition]].m10;
                        pScriptEntity->m106_currentRotation = followDataTable[partyToFollowStructMapping[partyPosition]].m14_rotation;
                        pScriptEntity->m104_rotation = followDataTable[partyToFollowStructMapping[partyPosition]].m14_rotation;
                        partyToFollowStructMapping[partyPosition] = partyToFollowStructMapping[partyPosition] - 1 & 0x1f;
                    }
                    else {
                        if (!uVar7.mx800_isJumping) {
                            pScriptEntity->m4_flags.m_rawFlags &= ~0x1000;
                            if ((pScriptEntity->m14_currentTriangleFlag & 0x420000U) != 0)
                                assert(0);
                            if (entityUpdateVar0 != -1) {
                                int iVar8 = 0x14;
                                if (partyPosition == 1) {
                                    iVar8 = 10;
                                }
                                if ((partyToFollowStructMapping[0] + iVar8 & 0x1fU) != *puVar9)
                                    continue;
                                goto LAB_Field__800818c8;
                            }
                            if ((pActor->m4_pVramSpriteSheet->m0_spriteActorCore).m84_maxY != pScriptEntity->m20_position.vy.getIntegerPart())
                                goto LAB_Field__800818c8;
                            if (pScriptEntity->mE8_currentAnimationId == 6) {
                                pScriptEntity->m4_flags.m_rawFlags |= 0x1000;
                                continue;
                            }
                            if (pScriptEntity->mE8_currentAnimationId == pScriptEntity->mE6)
                                continue;
                            pScriptEntity->mE8_currentAnimationId = pScriptEntity->mE6;
                            if (pScriptEntity->mE8_currentAnimationId < 0) {
                                pScriptEntity->mE8_currentAnimationId = 0;
                            }
                        }
                        else {
LAB_Field__800818c8:
                            if (partyToFollowStructMapping[partyPosition] != partyToFollowStructMapping[0])
                                goto LAB_Field__80081920;
                            pScriptEntity->mE8_currentAnimationId = pScriptEntity->mE6;
                            pScriptEntity->m0_fieldScriptFlags.m_rawFlags &= ~0x800;
                            if ((int)((uint)(ushort)pScriptEntity->mE6 << 0x10) < 0) {
                                pScriptEntity->mE8_currentAnimationId = 0;
                            }

                        }
                        setVisualAnimation(pActor->m4_pVramSpriteSheet, (int)pScriptEntity->mE8_currentAnimationId, pActor);
                    }
                }
            }
        }
    }
    else {
        for (int i = 0; i < totalActors; i++) {
            sFieldEntity* pActor = &actorArray[i];
            sFieldScriptEntity* pScriptEntity = pActor->m4C_scriptEntity;
            if ((((pScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x1000000) != 0) && (i != playerControlledActor)) && ((pActor->m58_flags & 0x20) == 0)) {
                if (pScriptEntity->mE8_currentAnimationId != pScriptEntity->mE6) {
                    pScriptEntity->mE8_currentAnimationId = pScriptEntity->mE6;
                    if (pScriptEntity->mE8_currentAnimationId < 0) {
                        pScriptEntity->mE8_currentAnimationId = 0;
                    }
                    setVisualAnimation(pActor->m4_pVramSpriteSheet, pScriptEntity->mE8_currentAnimationId, pActor);
                }
            }
        }
    }
}

void updateScriptAndMoveEntities()
{
    entityUpdateVar0 = -1;

    exectueEntitiesUpdateFunction();

    for (int i = 0; i < totalActors; i++)
    {
        actorArray[i].m4C_scriptEntity->m68_oldPosition[0] = actorArray[i].m4C_scriptEntity->m20_position.vx >> 16;
        actorArray[i].m4C_scriptEntity->m68_oldPosition[1] = actorArray[i].m4C_scriptEntity->m20_position.vy >> 16;
        actorArray[i].m4C_scriptEntity->m68_oldPosition[2] = actorArray[i].m4C_scriptEntity->m20_position.vz >> 16;
    }
    if (fieldDebugDisable == 0)
    {
        assert(0); // "EVENT_CODE"
    }

    entityUpdateVar1 = 0;

    for (int i = 0; i < totalActors; i++)
    {
        sFieldEntity* pFieldEntity = &actorArray[i];
        sFieldScriptEntity* pFieldScriptEntity = pFieldEntity->m4C_scriptEntity;

        if ((actorArray[i].m58_flags & 0xF80) == 0x200)
        {
            if ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10001) == 0)
            {
                if ((pFieldScriptEntity->m4_flags.m_rawFlags & 0x600) != 0x200)
                {
                    pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);
                    updateEntityEventCode2(i, pFieldEntity, pFieldScriptEntity);
                    updateEntityEventCode3(i, pFieldEntity, pFieldScriptEntity);
                }
            }
            else if(((pFieldScriptEntity->m4_flags.m_rawFlags & 0x1000000) == 0) || ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10000) != 0)) {
                if ((pFieldScriptEntity->m4_flags.m_rawFlags & 0x200000) != 0) {
                    if (pFieldScriptEntity->mE8_currentAnimationId != pFieldScriptEntity->mEA_forcedAnimation) {
                        pFieldScriptEntity->mE8_currentAnimationId = pFieldScriptEntity->mEA_forcedAnimation;
                        setVisualAnimation(pFieldEntity->m4_pVramSpriteSheet, pFieldEntity->m4C_scriptEntity->mEA_forcedAnimation, pFieldEntity);
                    }
                }
            }
            else if (pFieldScriptEntity->mE8_currentAnimationId != pFieldScriptEntity->mEA_forcedAnimation) {
                pFieldScriptEntity->mEA_forcedAnimation = 2;
                pFieldScriptEntity->mE8_currentAnimationId = pFieldScriptEntity->mEA_forcedAnimation;
                spriteActorSetPlayingAnimation(pFieldEntity->m4_pVramSpriteSheet, pFieldScriptEntity->mEA_forcedAnimation);
            }
        }
    }

    if (fieldDebugDisable == 0)
    {
        assert(0); // "MOV CHECK0"
    }

    EntityMoveCheck0(playerControlledActor, &actorArray[playerControlledActor], actorArray[playerControlledActor].m4C_scriptEntity);

    if (fieldDebugDisable == 0)
    {
        assert(0); // "MOV CHECK1"
    }

    for (int i = 0; i < totalActors; i++)
    {
        if (actorArray[i].m58_flags & 0xF00)
        {
            sFieldScriptEntity* pFieldScriptEntity = actorArray[i].m4C_scriptEntity;
            if (((((pFieldScriptEntity->m4_flags.m_rawFlags & 0x600) != 0x200) && ((actorArray[i].m58_flags & 0xf80) == 0x200)) && ((pFieldScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10001) == 0)) &&
                ((i != playerControlledActor &&
                    EntityMoveCheck1(i, 0x7fffffff, &actorArray[i], pFieldScriptEntity, 0) && (actorArray[i].m4_pVramSpriteSheet->m7C->mC == 1)))) {
                pFieldScriptEntity->m0_fieldScriptFlags.mx800_isJumping = 0;
            }
        }
    }

    if (fieldDebugDisable == 0) {
        assert(0);
    }
    if ((fieldRandomBattleVar == 0) && (noUpdatesToPartyMemebers == '\0')) {
        startScriptsForCollisions(playerControlledActor, &actorArray[playerControlledActor], actorArray[playerControlledActor].m4C_scriptEntity);
    }

    updateEntityEventCode3Var2 = 1;
    updatePartyFollowLeader();

    if (fieldDebugDisable == 0)
    {
        assert(0); // "MOV CHECK3"
    }
}

int updateAllEntitiesSub2Var0 = 0;

uint stepInterpolateDirectionSub(int currentDirection, int targetDirection, int step)
{
    uint uVar1;

    if ((currentDirection - targetDirection & 0xfff) < 0x800) {
        uVar1 = currentDirection - step;
        if (0x7ff < ((currentDirection - step) - targetDirection & 0xfff)) {
            uVar1 = targetDirection;
        }
    }
    else {
        uVar1 = currentDirection + step;
        if (((currentDirection + step) - targetDirection & 0xfff) < 0x800) {
            uVar1 = targetDirection;
        }
    }
    return uVar1 & 0xfff;
}

uint stepInterpolateDirection(int currentDirection, int targetDirection, int step)
{
    if (updateAllEntitiesSub2Var0 == 0) {
        return stepInterpolateDirectionSub(currentDirection, targetDirection, step);
    }
    else {
        return targetDirection & 0xfff;
    }
}

short cameraShakeEnabled = 0;
FP_VEC4 fieldCameraOffset = { 0,0,0,0 };

void updateCameraInterpolationSub2()
{
    if ((op99Var7 & 0x10) != 0) {
        if (OP_B6SubVar0 != 0) {
            OP_B6Var2 = OP_B6Var2 + OP_B6Var1;
            sceneSCRZ = OP_B6Var2 >> 0x10;
        }
        int iVar1 = OP_B6SubVar0 + -1;
        OP_B6SubVar0 = (short)iVar1;
        if (iVar1 * 0x10000 < 0) {
            OP_B6SubVar0 = 0;
            op99Var7 = op99Var7 & 0xffef;
        }
    }
    if (op9DVar1 != 0) {
        op99Var5 = 1;
        op99Var6 = 1;
        op9DVar1--;
    }

    s32 iVar2;
    s32 iVar1 = op99Var5 * op99Var5;
    s32 iVar3 = op99Var6 * op99Var6;
    if ((cameraEye[0] >> 0x10 != cameraEye2.vx >> 0x10) && (iVar2 = (cameraEye2.vx - cameraEye[0]) >> 0x10, iVar3 <= iVar2 * iVar2)) {
        cameraEye[0] = cameraEye[0] + (cameraEye2.vx - cameraEye[0]) / op99Var6;
    }
    if ((cameraEye[2] >> 0x10 != cameraEye2.vz >> 0x10) && (iVar2 = (cameraEye2.vz - cameraEye[2]) >> 0x10, iVar3 <= iVar2 * iVar2)) {
        cameraEye[2] = cameraEye[2] + (cameraEye2.vz - cameraEye[2]) / op99Var6;
    }
    if ((cameraEye[1] >> 0x10 != cameraEye2.vy >> 0x10) && (iVar2 = (cameraEye2.vy - cameraEye[1]) >> 0x10, iVar3 <= iVar2 * iVar2)) {
        cameraEye[1] = cameraEye[1] + (cameraEye2.vy - cameraEye[1]) / op99Var6;
    }
    if ((cameraAt[0] >> 0x10 != cameraAt2.vx >> 0x10) && (iVar3 = (cameraAt2.vx - cameraAt[0]) >> 0x10, iVar1 <= iVar3 * iVar3)) {
        cameraAt[0] = cameraAt[0] + (cameraAt2.vx - cameraAt[0]) / op99Var5;
    }
    if ((cameraAt[2] >> 0x10 != cameraAt2.vz >> 0x10) && (iVar3 = (cameraAt2.vz - cameraAt[2]) >> 0x10, iVar1 <= iVar3 * iVar3)) {
        cameraAt[2] = cameraAt[2] + (cameraAt2.vz - cameraAt[2]) / op99Var5;
    }
    if ((cameraAt[1] >> 0x10 != cameraAt2.vy >> 0x10) && (iVar3 = (cameraAt2.vy - cameraAt[1]) >> 0x10, iVar1 <= iVar3 * iVar3)) {
        cameraAt[1] = cameraAt[1] + (cameraAt2.vy - cameraAt[1]) / op99Var5;
    }

    fieldCameraOffset.vx = 0;
    fieldCameraOffset.vy = 0;
    fieldCameraOffset.vz = 0;

    if (cameraShakeEnabled != 0) {
        assert(0);
    }
}


std::array<u8, 8> cameraDirectionToDollyBitmask = {
    0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08l
};

int updateCameraDollySub0(byte param_1, uint param_2)
{
    uint uVar1;
    int iVar2;
    int iVar3;

    iVar2 = 0;
    iVar3 = 0;
    do {
        uVar1 = param_2 & 7;
        param_2 = param_2 + 1;
        if ((param_1 & (cameraDirectionToDollyBitmask)[uVar1]) == 0) {
            return iVar3;
        }
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 1;
    } while (iVar2 < 8);
    return 0;
}

int updateCameraDollySub1(byte param_1, uint param_2)
{
    uint uVar1;
    int iVar2;
    int iVar3;

    iVar2 = 0;
    iVar3 = 0;
    do {
        uVar1 = param_2 & 7;
        param_2 = param_2 - 1;
        if ((param_1 & (cameraDirectionToDollyBitmask)[uVar1]) == 0) {
            return iVar3;
        }
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 1;
    } while (iVar2 < 8);
    return 0;
}

void updateCameraDolly()
{
    if ((DollySet != 0xff) && (DollyStop != 0xff)) {
        if (cameraDollyVar0 == 0) {
            if (((cameraDirectionToDollyBitmask)[((ushort)cameraRotationBetweenEyeAndAt.vy & 0xfff) >> 9] & DollySet) != 0) {
                if ((cameraDollyVar1 != -0x400000) && (cameraDollyVar1 != 0x400000)) {
                    cameraDollyVar1 = 0x400000;
                    opA0_var0 = opA0_var0 + 0x200;
                }
                cameraDollyVar0 = 8;
            }
            int uVar5 = ((ushort)cameraRotationBetweenEyeAndAt.vy & 0xfff) >> 9;
            if (((cameraDirectionToDollyBitmask)[uVar5] & DollyStop) != 0) {
                int iVar2 = updateCameraDollySub0(DollyStop, uVar5);
                int iVar3 = updateCameraDollySub1(DollyStop, ((ushort)cameraRotationBetweenEyeAndAt.vy & 0xfff) >> 9);
                if (iVar3 < iVar2) {
                    cameraDollyVar1 = -0x400000;
                    opA0_var0 = opA0_var0 + -0x200;
                }
                else {
                    cameraDollyVar1 = 0x400000;
                    opA0_var0 = opA0_var0 + 0x200;
                }
                cameraDollyVar0 = 8;
            }
        }

        // Rotate camera left
        if (((((padButtonForScripts[0].m0_buttons & controllerButtons::L1) != 0) && ((op99Var7 & 0x8000) == 0)) && (cameraDollyVar0 == 0)) &&
            (((cameraDirectionToDollyBitmask)[(int)((int)cameraRotationBetweenEyeAndAt.vy - 0x200U & 0xfff) >> 9] & DollyStop) == 0)) {
            cameraDollyVar1 = -0x400000;
            cameraDollyVar0 = 8;
            opA0_var0 = opA0_var0 + -0x200;
        }

        // Rotate camera right
        if (((((padButtonForScripts[0].m0_buttons & controllerButtons::R1) != 0) && ((op99Var7 & 0x8000) == 0)) && (cameraDollyVar0 == 0)) &&
            (((cameraDirectionToDollyBitmask)[(int)((int)cameraRotationBetweenEyeAndAt.vy + 0x200U & 0xfff) >> 9] & DollyStop) == 0)) {
            cameraDollyVar1 = 0x400000;
            cameraDollyVar0 = 8;
            opA0_var0 = opA0_var0 + 0x200;
        }
    }

    if (cameraDollyVar0 != 0)
    {
        opA0_var1 = opA0_var1 + cameraDollyVar1;
        cameraRotationBetweenEyeAndAt.vy = (short)((uint)opA0_var1 >> 0x10);
        int sVar4 = cameraDollyVar0 + -1;
        int bVar1 = cameraDollyVar0 != 1;
        cameraDollyVar0 = sVar4;
        if (!bVar1)
        {
            cameraRotationBetweenEyeAndAt.vy = (short)opA0_var0;
        }
    }
    else
    {
        cameraRotationBetweenEyeAndAt.vy = (short)opA0_var0;
    }

    if (fieldDebugDisable == 0) {
        assert(0);
    }
}

int checkCameraCollision(FP_VEC4* param_1, std::array<SFP_VEC4, 2>& param_2, std::array<s16, 4>& output)
{
    s32 X = param_1->vx >> 16;
    s32 Z = param_1->vz >> 16;

    sVec2_s16 refPos = sVec2_s16::fromValue(X, Z);

    s32 posX = cameraLimits[0];
    if (posX <= X) {
        s32 temp = posX + cameraLimits[2];
        posX = X;
        if (temp < X) {
            posX = temp;
        }
    }

    s32 posZ = cameraLimits[1];
    if (Z <= posZ) {
        s32 temp = posZ + cameraLimits[3];
        posZ = Z;
        if (Z < temp) {
            posZ = temp;
        }
    }

    sVec2_s16 refPos2 = sVec2_s16::fromValue(posX, posZ);

    SFP_VEC4 asStack80;
    FP_VEC4 FStack72;
    s32 triangleId = findTriangleInWalkMesh(posX, posZ, numWalkMesh -1, &asStack80, &FStack72);

    std::vector<sWalkMesh::sTriangleData>::iterator pCollisionTriangles = walkMeshTriangle[numWalkMesh - 1]->begin();
    std::vector<SFP_VEC4>::iterator pVertices = walkMeshVertices[numWalkMesh - 1]->begin();

    int lastTriangle;
    int collisionFlag;
    int iterationCount = 0;
    for (iterationCount = 0; iterationCount < 0xF0; iterationCount++)
    {
        sVec2_s16 vert0;
        sVec2_s16 vert1;
        sVec2_s16 vert2;

        sWalkMesh::sTriangleData& pTriangle = pCollisionTriangles[triangleId];
        vert0.set(pVertices[pTriangle.m0_verticeIndex[0]].vx, pVertices[pTriangle.m0_verticeIndex[0]].vz); // s4
        vert1.set(pVertices[pTriangle.m0_verticeIndex[1]].vx, pVertices[pTriangle.m0_verticeIndex[1]].vz); // s3
        vert2.set(pVertices[pTriangle.m0_verticeIndex[2]].vx, pVertices[pTriangle.m0_verticeIndex[2]].vz); // s1

        lastTriangle = triangleId;

        collisionFlag = 0;
        if (NCLIP(vert0, vert1, refPos) < 0) {
            collisionFlag |= 1;
        }

        if (NCLIP(vert1, vert2, refPos) < 0) {
            collisionFlag |= 2;
        }

        if (NCLIP(vert2, vert0, refPos) < 0) {
            collisionFlag |= 4;
        }
        if (collisionFlag < 8) {
            switch (collisionFlag)
            {
            case 0:
                iterationCount = 0xFF;
                break;
            case 1:
                triangleId = pTriangle.m6_connectivity[0];
                break;
            case 2:
                triangleId = pTriangle.m6_connectivity[1];
                break;
            case 3:
                if (NCLIP(vert1, refPos, refPos2) < 0) // a0 s4 s3
                {
                    triangleId = pTriangle.m6_connectivity[0];
                }
                else {
                    triangleId = pTriangle.m6_connectivity[1];
                }
                break;
            case 4:
                triangleId = pTriangle.m6_connectivity[2];
                break;
            case 5:
                if (NCLIP(vert0, refPos, refPos2) < 0) // a2,s4,s3
                {
                    triangleId = pTriangle.m6_connectivity[2];
                }
                else {
                    triangleId = pTriangle.m6_connectivity[0];
                }
                break;
            case 6:
                if (NCLIP(vert2, refPos, refPos2) < 0) // v1,s4,s3
                {
                    triangleId = pTriangle.m6_connectivity[1];
                }
                else {
                    triangleId = pTriangle.m6_connectivity[2];
                }
                break;
            case 7:
                triangleId = -1;
                break;
            default:
                assert(0);
            }
        }

        if (((*walkMeshVar1)[pCollisionTriangles[triangleId].mC_indexInWalkmeshData1] & 0x800000) == 0) {
            triangleId = -1;
            break;
        }

        // reached the edge of the mesh
        if (triangleId == -1)
        {
            break;
        }
    }

    if ((triangleId != -1) && (iterationCount != 0xF0)) {
        return 0;
    }

    switch (collisionFlag)
    {
    case 1:
        param_2[0] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[1]];
        param_2[1] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[2]];
        break;
    case 2:
        param_2[0] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[1]];
        param_2[1] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[2]];
        break;
    case 4:
        param_2[0] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[1]];
        param_2[1] = pVertices[pCollisionTriangles[lastTriangle].m0_verticeIndex[2]];
        break;
    default:
        assert(0);
        return -1;
    }


    output[0] = X;
    output[1] = Z;
    output[2] = posX;
    output[3] = posZ;

    return -1;
}

void computeCameraEyeFromAt(FP_VEC4* param_1, FP_VEC4* param_2)
{
    MATRIX MStack80;
    FP_VEC4 local_30;
    FP_VEC4 local_20;

    PushMatrix();
    createRotationMatrix(&cameraRotationBetweenEyeAndAt, &MStack80);
    local_30.vx = param_2->vx - param_1->vx;
    local_30.vy = param_2->vy - param_1->vy;
    local_30.vz = param_2->vz - param_1->vz;
    ApplyMatrixLV(&MStack80, &local_30, &local_20);
    param_1->vx = local_20.vx + param_2->vx;
    param_1->vz = local_20.vz + param_2->vz;
    PopMatrix();
    return;
}

s8 cameraCollisionVar0 = 0;

void updateCameraInterpolationSub1Sub0(std::array<s16, 4>& param_1, std::array<s16, 4>& param_2, std::array<s16, 2>& param_3)
{
    int iVar1;
    VECTOR local_48;
    VECTOR local_38;
    VECTOR local_28;

    local_28.vy = 0;
    local_28.vx = (int)param_1[2] - (int)param_1[0];
    local_28.vz = (int)param_1[3] - (int)param_1[1];
    VectorNormal(&local_28, &local_48);
    local_28.vy = 0;
    local_28.vx = (int)param_2[2] - (int)param_2[0];
    local_28.vz = (int)param_2[3] - (int)param_2[1];
    VectorNormal(&local_28, &local_38);

    iVar1 = (local_38.vx * local_48.vz - local_38.vz * local_48.vx) >> 0xc;
    if (iVar1 == 0) {
        iVar1 = 0;
    }
    else {
        iVar1 = (((int)param_2[1] - (int)param_1[1]) * local_48.vx - ((int)param_2[0] - (int)param_1[0]) * local_48.vz) / iVar1;
    }
    param_3[0] = param_2[0] + (short)(iVar1 * local_38.vx >> 0xc);
    param_3[1] = param_2[1] + (short)(iVar1 * local_38.vz >> 0xc);
}

void updateCameraInterpolationSub1(FP_VEC4* param_1, int elevation)
{
    FP_VEC4 local_48;
    local_48.vx = param_1->vx;
    local_48.vy = 0;
    local_48.vz = param_1->vz;
    std::array<SFP_VEC4, 2> collisionResult;
    std::array<s16, 4> collisionResult2;
    if (checkCameraCollision(&local_48, collisionResult, collisionResult2) == -1) {
        std::array<s16, 4> local_60;
        local_60[0] = collisionResult[0].vx;
        local_60[1] = collisionResult[0].vz;
        local_60[2] = collisionResult[1].vx;
        local_60[3] = collisionResult[1].vz;

        std::array<s16, 2> local_50;
        updateCameraInterpolationSub1Sub0(local_60, collisionResult2, local_50);
        cameraAt2.vx = (int)local_50[0] << 0x10;
        cameraAt2.vz = (int)local_50[1] << 0x10;
        if (cameraCollisionVar0 == '\0') {
            if (cameraCollisionState == 0) {
                cameraCollisionState = 1;
                cameraAt2.vy = elevation << 0x10;
            }
        }
        else {
            cameraAt2.vy = param_1->vy + -0x200000;
        }
    }
    else {
        cameraAt2.vx = param_1->vx;
        cameraCollisionState = 0;
        cameraAt2.vz = param_1->vz;
        cameraAt2.vy = param_1->vy + -0x200000;
    }

    cameraEye2.vy = (getAngleSin((sceneDIP * 0x5b >> 3) + 0xc00) * sceneSCRZ * -0x20 >> 0x10) * (int)sceneScale * 0x10 + cameraAt2.vy;
    cameraEye2.vx = cameraAt2.vx;
    cameraEye2.vz = (getAngleCos((sceneDIP * 0x5b >> 3) + 0xc00) * sceneSCRZ * 0x20 >> 0x10) * (int)sceneScale * 0x10 + cameraAt2.vz;

    computeCameraEyeFromAt(&cameraEye2, &cameraAt2);

    if ((op99Var7 & 1) != 0) {
        if (op9DVar0 != 0) {
            op9DVar3 = op9DVar3 + op9DVar2;
            sceneScale = (short)((uint)op9DVar3 >> 0x10);
        }
        int uVar3 = (int)op9DVar0 - 1;
        op9DVar0 = (short)uVar3;
        if ((uVar3 & 0xffff) == 0) {
            op99Var7 = op99Var7 & 0xfffe;
        }
    }
    if ((op99Var7 & 8) != 0) {
        OP_A4Var1 = OP_A4Var1 + OP_A4Var2;
        sceneDIP = (short)((uint)OP_A4Var1 >> 0x10);
        int sVar4 = OP_A4Var0 + -1;
        int bVar1 = OP_A4Var0 == 1;
        OP_A4Var0 = sVar4;
        if (bVar1) {
            op99Var7 = op99Var7 & 0xfff7;
        }
    }
}

void updateCameraInterpolation(void)
{
    switch (cameraInterpolationMode)
    {
    case 0:
        updateCameraInterpolationVar1 = 0;
        {
            int iVar1 = op99Var6;
            if ((updateCameraInterpolationVar0 & 3) == 0) {
                iVar1 = 8;
                if (8 < op99Var5) {
                    iVar1 = op99Var5 + -2;
                }
                op99Var5 = iVar1;
                iVar1 = 8;
                if (8 < op99Var6) {
                    iVar1 = op99Var6 + -2;
                }
            }
            op99Var6 = iVar1;
            updateCameraInterpolationVar0 = updateCameraInterpolationVar0 + 1;
        }
        break;
    case 1:
        updateCameraInterpolationVar0 = 0;
        updateCameraInterpolationVar1 = 0;
        if ((cameraInterpolationFlags & 1) != 0) {
            if (cameraInterpolationTargetNumSteps != 0) {
                cameraInterpolationTargetStartPosition[0] = cameraInterpolationTargetStartPosition[0] + cameraInterpolationTargetStep[0];
                cameraInterpolationTargetStartPosition[1] = cameraInterpolationTargetStartPosition[1] + cameraInterpolationTargetStep[1];
                cameraInterpolationTargetStartPosition[2] = cameraInterpolationTargetStartPosition[2] + cameraInterpolationTargetStep[2];
            }
            if ((--cameraInterpolationTargetNumSteps) == 0) {
                cameraInterpolationFlags &= ~1;
            }
            cameraAt2.vx = cameraInterpolationTargetStartPosition[0];
            cameraAt2.vy = cameraInterpolationTargetStartPosition[1];
            cameraAt2.vz = cameraInterpolationTargetStartPosition[2];
        }
        if ((cameraInterpolationFlags & 2) != 0) {
            if (cameraInterpolationPositionNumSteps != 0) {
                cameraInterpolationStartPosition[0] = cameraInterpolationStartPosition[0] + cameraInterpolationPositionStep[0];
                cameraInterpolationStartPosition[1] = cameraInterpolationStartPosition[1] + cameraInterpolationPositionStep[1];
                cameraInterpolationStartPosition[2] = cameraInterpolationStartPosition[2] + cameraInterpolationPositionStep[2];
            }
            if ((--cameraInterpolationPositionNumSteps) == 0) {
                cameraInterpolationFlags &= ~2;
            }
            cameraEye2.vx = cameraInterpolationStartPosition[0];
            cameraEye2.vy = cameraInterpolationStartPosition[1];
            cameraEye2.vz = cameraInterpolationStartPosition[2];
        }
        updateCameraInterpolationSub2();
        cameraRotationBetweenEyeAndAt.vy &= 0xfff;
        return;
    case 2:
        updateCameraInterpolationVar0 = 0;
        updateCameraInterpolationVar1 = updateCameraInterpolationVar1 + 1;
        if (0x40 < updateCameraInterpolationVar1) {
            cameraInterpolationMode = 0;
        }
        break;
    default:
        assert(0);
    }

    updateCameraDolly();
    sFieldEntity* psVar2 = &actorArray[actorCameraTracked];
    FP_VEC4 localPosition;
    localPosition.vx = (psVar2->m4C_scriptEntity->m20_position).vx;
    localPosition.vy = (psVar2->m4C_scriptEntity->m20_position).vy;
    localPosition.vz = (psVar2->m4C_scriptEntity->m20_position).vz;

    updateCameraInterpolationSub1(&localPosition, (int)psVar2->m4C_scriptEntity->m72_elevation);
    if ((op99Var7 & 0x4000) == 0) {
        SFP_VEC4 walkmeshOutput1;
        FP_VEC4 walkmeshOutput2;
        findTriangleInWalkMesh((cameraEye2.vx >> 16), (cameraEye2.vz >> 16), numWalkMesh + -1, &walkmeshOutput1, &walkmeshOutput2);
        if ((int)walkmeshOutput1.vy < (int)(cameraEye2.vy >> 16)) {
            cameraEye2.vy = (int)walkmeshOutput1.vy << 0x10;
        }
    }
    if (cameraInterpolationMode == 2) {
        int lVar3 = length2d((cameraAt2.vx >> 16) - (cameraAt[0] >> 16), (cameraAt2.vz >> 16) - (cameraAt[2] >> 16));
        int lVar4 = length2d((cameraEye2.vx >> 16) - (cameraEye[0] >> 16), (cameraEye2.vz >> 16) - (cameraEye[2] >> 16));
        if ((lVar3 < 0x80) && (lVar4 < 0x80)) {
            cameraInterpolationMode = 0;
        }
    }

    updateCameraInterpolationSub2();
    cameraRotationBetweenEyeAndAt.vy &= 0xfff;
}

struct sCurrentCameraVectors
{
    FP_VEC4 m10_eye;
    FP_VEC4 m20_at;
};

sCurrentCameraVectors* pCurrentCameraVectors = nullptr;

SFP_VEC4 computeProjectionMatrixAngles = { 0,0,0 };
MATRIX computeProjectionMatrixTempMatrix;

void computeProjectionMatrix(void)
{
    FP_VEC4 local_70;
    MATRIX MStack64;
    long alStack32[2];

    createRotationMatrix(&computeProjectionMatrixAngles, &computeProjectionMatrixTempMatrix);
    resetMatrixTranslation(&computeProjectionMatrixTempMatrix);
    CompMatrix(&computeProjectionMatrixTempMatrix, &cameraMatrix, &MStack64);
    copyMatrix(&cameraMatrix, &MStack64);
    createRotationMatrix(&cameraProjectionAngles, &computeProjectionMatrixTempMatrix2);
    resetMatrixTranslation(&computeProjectionMatrixTempMatrix2);
    createRotationMatrix(&cameraProjectionAngles, &currentProjectionMatrix);
    MulRotationMatrix(&cameraMatrix, &currentProjectionMatrix);
    SetRotMatrix(&cameraMatrix);
    SetTransMatrix(&cameraMatrix);
    RotTrans(&cameraRotation, &currentProjectionMatrix.t, alStack32);
    local_70.vx = worldScaleFactor;
    local_70.vy = worldScaleFactor;
    local_70.vz = worldScaleFactor;
    ScaleMatrix(&currentProjectionMatrix, &local_70);
    SetRotMatrix(&currentProjectionMatrix);
    SetTransMatrix(&currentProjectionMatrix);
    return;
}

void setProjectionMatrixForField(void)
{
    computeProjectionMatrix();
    SetRotMatrix(&currentProjectionMatrix);
    SetTransMatrix(&currentProjectionMatrix);
    if (fieldDebugDisable == 0) {
        assert(0);
    }
    return;
}

void updateAllEntities()
{
    updateScriptAndMoveEntities();

    createRotationMatrix(&renderModelRotationAngles, &renderModelRotationMatrix);
    renderModelRotationMatrix.t[2] = 0;
    renderModelRotationMatrix.t[1] = 0;
    renderModelRotationMatrix.t[0] = 0;

    cameraTan = ratan2(cameraAt[2] - cameraEye[2], cameraAt[0] - cameraEye[0]) + -0x400;
    camera2Tan = ratan2(cameraAt2.vz - cameraEye2.vz, cameraAt2.vx - cameraEye2.vx) + -0x400;
    cameraDeltaTan = ratan2(length2d((cameraAt[0] - cameraEye[0]) >> 0x10, (cameraAt[2] - cameraEye[2]) >> 0x10), (cameraAt[1] - cameraEye[1]) >> 0x10);

    sCurrentCameraVectors cameraVectors;
    pCurrentCameraVectors = &cameraVectors;

    updateCameraInterpolation();

    int lVar6 = fieldCameraOffset.vz;
    int lVar5 = fieldCameraOffset.vy;
    int lVar7 = fieldCameraOffset.vx;
    int iVar9 = cameraEye[2];
    int iVar11 = cameraEye[1];
    int iVar12 = cameraEye[0];
    (pCurrentCameraVectors->m10_eye).vx = cameraEye[0];
    (pCurrentCameraVectors->m10_eye).vx = iVar12 + lVar7;
    iVar12 = cameraAt[0];
    (pCurrentCameraVectors->m10_eye).vy = iVar11;
    (pCurrentCameraVectors->m10_eye).vy = iVar11 + lVar5;
    iVar11 = cameraAt[1];
    (pCurrentCameraVectors->m10_eye).vz = iVar9;
    int iVar4 = cameraAt[2];
    (pCurrentCameraVectors->m20_at).vx = iVar12;
    (pCurrentCameraVectors->m20_at).vx = iVar12 + lVar7;
    (pCurrentCameraVectors->m10_eye).vz = iVar9 + lVar6;
    (pCurrentCameraVectors->m20_at).vy = iVar11;
    (pCurrentCameraVectors->m20_at).vz = iVar4;
    (pCurrentCameraVectors->m20_at).vy = iVar11 + lVar5;
    (pCurrentCameraVectors->m20_at).vz = iVar4 + lVar6;
    if (updateAllEntitiesSub2Var0 == 0) {
        cameraMatrix = cameraMatrix2;
        lookAtDivided(&cameraMatrix2, &pCurrentCameraVectors->m10_eye, &pCurrentCameraVectors->m20_at, &cameraUp);
    }
    else {
        lookAtDivided(&cameraMatrix, &pCurrentCameraVectors->m10_eye, &pCurrentCameraVectors->m20_at, &cameraUp);
        cameraMatrix2 = cameraMatrix;
    }

    setProjectionMatrixForField();

    for (int i = 0; i < totalActors; i++)
    {
        sFieldEntity* pActor = &actorArray[i];
        if (((pActor->m58_flags & 0xf40) != 0) && ((pActor->m58_flags & 0x20) == 0)) {
            sFieldScriptEntity* psVar10 = pActor->m4C_scriptEntity;
            u32 uVar7 = psVar10->m4_flags.m_rawFlags;
            if (((uVar7 & 0x100000) == 0) && ((uVar7 & 0x600) != 0x200)) {
                if ((psVar10->m0_fieldScriptFlags.m_rawFlags & 0x8000) == 0) {
                    s32 iVar9;
                    s32 iVar8;
                    s32 sVar5;
                    if (((psVar10->m14_currentTriangleFlag & 0x200000U) == 0) || (iVar9 = 0x200, (psVar10->m0_fieldScriptFlags.m_rawFlags & 0x1800) != 0)) {
                        sVar5 = fieldObjectRenderingVar2.pad;
                        if ((uVar7 & 0x2000) == 0) {
                            sVar5 = psVar10->m11E;
                        }
                        iVar9 = (int)sVar5;
                        sVar5 = psVar10->m108_rotation3;
                        iVar8 = (int)psVar10->m106_currentRotation;
                    }
                    else {
                        iVar8 = (((uint)psVar10->m14_currentTriangleFlag >> 0xb) - 2 & 7) << 9;
                        sVar5 = psVar10->m108_rotation3;
                    }
                    sVar5 = stepInterpolateDirection(sVar5, iVar8, iVar9);
                    psVar10->m108_rotation3 = sVar5;
                }
                if (renderCharsDisabled == '\0') {
                    if ((psVar10->m4_flags.m_rawFlags & 0x1000000) == 0) {
                        setSpriteActorAngle(pActor->m4_pVramSpriteSheet, cameraTan + pActor->m4C_scriptEntity->m108_rotation3);
                    }
                    else {
                        OP_INIT_ENTITY_SCRIPT_sub0Sub7(pActor->m4_pVramSpriteSheet, pActor->m4C_scriptEntity->m108_rotation3);
                    }
                }
            }
        }
    }

    if (fieldDebugDisable == 0) {
        assert(0);
    }
}

struct sUpdateCameraAtStruct
{
    s32 m0[4];
    s32 m10[4];
    s32 m20[4];
    s32 m30[4];
    s32 m40[4];
    s32 m50[4];
};

void updateCameraAt(FP_VEC4* pCameraAt)
{
    sUpdateCameraAtStruct local_98;
    for (int i = 0; i < 3; i++)
    {
        local_98.m0[i] = 0xFFFF;
        local_98.m10[i] = -1;
        local_98.m40[i] = 0;
        local_98.m30[i] = 0;
        local_98.m50[i] = 0;
        local_98.m20[i] = 0;
    }

    for (int i = 0; i < totalActors; i++)
    {
        MissingCode();
    }

    MissingCode();
}

void updateCamera()
{
    switch (cameraTrackingMode)
    {
    case eCameraTrackingMode::e0_followPlayer: // follow player
        updateCameraAt(&actorArray[playerControlledActor].m4C_scriptEntity->m20_position);
        break;
    default:
        assert(0);
    }
}


MATRIX occlusionTestMatrix;

bool isObjectOcluded(sFieldEntitySub0* param_1)
{
    short sVar1;
    short sVar2;
    int iVar3;
    int iVar4;
    FP_VEC4 local_48;
    SFP_VEC4 local_38;
    long lStack48;
    sVec2_s16 local_2c;
    long alStack40[2];

    RotTrans(&param_1->m18, &local_48, &lStack48);
    occlusionTestMatrix.t[0] = local_48.vx;
    occlusionTestMatrix.t[1] = local_48.vy;
    occlusionTestMatrix.t[2] = local_48.vz;
    SetRotMatrix(&occlusionTestMatrix);
    SetTransMatrix(&occlusionTestMatrix);
    sVar1 = param_1->m20;
    local_38.vz = 0;
    local_38.vx = -sVar1;
    local_38.vy = local_38.vx;
    RotTransPers(&local_38, &local_2c, alStack40, &lStack48);
    local_38.vz = 0;
    iVar4 = local_2c.vy;
    sVar2 = local_2c.vx;
    local_38.vx = sVar1;
    local_38.vy = sVar1;
    RotTransPers(&local_38, &local_2c, alStack40, &lStack48);
    iVar3 = -1;
    if (((iVar4 < OPX8E_param1 + 0xe0) && (-OPX8E_param1 < local_2c.vy)) && ((int)sVar2 < OPX8E_param0 + 0x140)) {
        iVar3 = -(-(OPX8E_param0 < local_2c.vx) ^ 1);
    }

    Hack("Force isObjectOcluded to return false");
    return 0;
    return iVar3;
}

int isObjectClipped(sModelBlock* param_1, uint param_2)
{
    MissingCode();
    return 0;
}

bool submitModelForRendering(sModelBlock* param_1, std::vector<sTag*>& param_2, OTTable::iterator OT, int renderMode)
{
    if ((objectClippingMask != 0) && (isObjectClipped(param_1, objectClippingMask))) {
        return 0;
    }

    if(param_1->m18.size())
        currentModeBlock18 = &param_1->m18[0];
    currentModelBlockNormals = param_1->m_baseItForRelocation + param_1->mC_offsetNormals;
    currentModelBlockVertices = param_1->m_baseItForRelocation + param_1->m8_offsetVertices;
    fieldPolyCount2 += param_1->m4_numPrims;
    currentModelInstanceDrawPrims = param_2.begin();
    currentOTEntry = OT;

    int numMeshBlockLeft = param_1->m6_numMeshBlock;
    u8* currentModelBlockSubBlocks = param_1->m_baseItForRelocation + param_1->m10_offsetMeshBlocks;
    while (numMeshBlockLeft = numMeshBlockLeft - 1, numMeshBlockLeft != 0xffffffff) {
        int primType = READ_LE_U8(currentModelBlockSubBlocks);
        t_primRenderFunc primRenderFunc = nullptr;
        switch (renderMode) {
        case 0:
            primRenderFunc = polyRenderDefs[primType].m0_type0;
            break;
        case 1:
            primRenderFunc = polyRenderDefs[primType].m4_type1;
            break;
        case 2:
            primRenderFunc = polyRenderDefs[primType].m8_type2;
            break;
        case 3:
            primRenderFunc = polyRenderDefs[primType].mC_type3;
            break;
        case 4:
            primRenderFunc = polyRenderDefs[primType].m10_type4;
            break;
        case 5:
            primRenderFunc = polyRenderDefs[primType].m14_type5;
            break;
        default:
            assert(0);
        }
        g_currentModelBlockSubBlocks = currentModelBlockSubBlocks + 4;
        if (primRenderFunc)
        {
            primRenderFunc(g_currentModelBlockSubBlocks, READ_LE_U16(currentModelBlockSubBlocks + 2));
        }
        else
        {
            currentModelInstanceDrawPrims += READ_LE_U16(currentModelBlockSubBlocks + 2);
        }
        currentModelBlockSubBlocks = g_currentModelBlockSubBlocks + READ_LE_U16(currentModelBlockSubBlocks + 2) * polyRenderDefs[primType].m1C_size;
    }
    g_currentModelBlockSubBlocks = currentModelBlockSubBlocks;
    return 1;
}

void renderObjects()
{
    fieldPolyCount = 0;
    fieldPolyCount2 = 0;

    if (isFogSetup != 0) {
        MissingCode();
    }

    FP_VEC4 scale1 = { 0x800, 0x800, 0x800 };
    FP_VEC4 scale2 = { worldScaleFactor , worldScaleFactor , worldScaleFactor };

    ScaleMatrix(&worldScaleMatrix, &scale2);

    MATRIX fieldLocalWorldMatrix;
    CompMatrix(&currentProjectionMatrix, &renderModelRotationMatrix, &fieldLocalWorldMatrix);

    objectClippingMask = 0;
    fieldObjectRenderingVar1.vy = fieldObjectRenderingVar1.vy + fieldObjectRenderingVar2.vz;
    fieldObjectRenderingVar1.vx = fieldObjectRenderingVar1.vx + fieldObjectRenderingVar2.vx;
    fieldObjectRenderingVar1.vz = fieldObjectRenderingVar1.vz + fieldObjectRenderingVar2.vy;

    for (int i = 0; i < totalObjects; i++)
    {
        MATRIX rotationMatrix;
        MATRIX projectedMatrix;

        sFieldEntity* pFieldEntity = &actorArray[i];

        pFieldEntity->m2C_matrixBackup = pFieldEntity->mC_matrix;
        bool forceVisibility = false;

        if (!(pFieldEntity->m58_flags & 0x40))
        {
            if (i < totalActors)
            {
                SFP_VEC4 rotationAxis;
                switch (pFieldEntity->m4C_scriptEntity->m12C_flags & 3)
                {
                case 0:
                    if (pFieldEntity->m4C_scriptEntity->m128 == -1)
                    {
                        if (pFieldEntity->m4C_scriptEntity->m75 == -1)
                        {
                            goto LAB_Field__80074d80;
                        }
                        else
                        {
                            CompMatrix(&currentProjectionMatrix, &actorArray[pFieldEntity->m4C_scriptEntity->m75].m2C_matrixBackup, &rotationMatrix);
                            CompMatrix(&rotationMatrix, &actorArray[i].mC_matrix, &projectedMatrix);
                            CompMatrix(&actorArray[pFieldEntity->m4C_scriptEntity->m75].m2C_matrixBackup, &actorArray[i].mC_matrix, &actorArray[i].m2C_matrixBackup);
                        }
                    }
                    else
                    {
                        assert(0);
                    }
                    break;
                case 1:
                    rotationAxis = { pFieldEntity->m4C_scriptEntity->m70_rotationForRendering, 0,0 };
                    createRotationMatrix(&rotationAxis, &rotationMatrix);
                    MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
                    rotationMatrix.t = actorArray[i].mC_matrix.t;
                    CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
                    break;
                case 2:
                    rotationAxis = { 0, pFieldEntity->m4C_scriptEntity->m70_rotationForRendering,0 };
                    createRotationMatrix(&rotationAxis, &rotationMatrix);
                    MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
                    rotationMatrix.t = actorArray[i].mC_matrix.t;
                    CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
                    break;
                case 3:
                    rotationAxis = { 0, 0, pFieldEntity->m4C_scriptEntity->m70_rotationForRendering };
                    createRotationMatrix(&rotationAxis, &rotationMatrix);
                    MulRotationMatrix(&actorArray[i].mC_matrix, &rotationMatrix);
                    rotationMatrix.t = actorArray[i].mC_matrix.t;
                    CompMatrix(&currentProjectionMatrix, &rotationMatrix, &projectedMatrix);
                    break;
                default:
                    assert(0); // else projectedMatrix is uninitialized
                }
            }
            else
            {
                if ((fieldObjectRenderingVar3 & 0x7f) == 0) {
                    (pFieldEntity->mC_matrix).t[0] = ((int)(pFieldEntity->mC_matrix).t[0] + (int)fieldObjectRenderingVar2.vx);
                    (pFieldEntity->mC_matrix).t[1] = ((int)(pFieldEntity->mC_matrix).t[1] + (int)fieldObjectRenderingVar2.vz);
                    (pFieldEntity->mC_matrix).t[2] = ((int)(pFieldEntity->mC_matrix).t[2] + (int)fieldObjectRenderingVar2.vy);
                }

            LAB_Field__80074d80:
                if ((fieldObjectRenderingVar3 & 0x7f) == 1) {
                    (pFieldEntity->mC_matrix).t[0] = ((int)(pFieldEntity->mC_matrix).t[0] + (int)fieldObjectRenderingVar2.vx);
                    (pFieldEntity->mC_matrix).t[1] = ((int)(pFieldEntity->mC_matrix).t[1] + (int)fieldObjectRenderingVar2.vz);
                    (pFieldEntity->mC_matrix).t[2] = ((int)(pFieldEntity->mC_matrix).t[2] + (int)fieldObjectRenderingVar2.vy);
                }

                forceVisibility = (fieldObjectRenderingVar3 & 0x80) != 0;

                MulMatrix0(&fieldLocalWorldMatrix, &pFieldEntity->mC_matrix, &projectedMatrix);
                setCopControlWord(2, 0x2800, fieldLocalWorldMatrix.t[0]);
                setCopControlWord(2, 0x3000, fieldLocalWorldMatrix.t[1]);
                setCopControlWord(2, 0x3800, fieldLocalWorldMatrix.t[2]);

                setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[0], pFieldEntity->mC_matrix.t[1]));
                setCopReg(2, 1, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[2], 0));
                copFunction(2, 0x480012);
                projectedMatrix.t[0] = getCopReg(2, 0x19);
                projectedMatrix.t[1] = getCopReg(2, 0x1a);
                projectedMatrix.t[2] = getCopReg(2, 0x1b);

                if ((actorArray[i].m58_flags & 3) != 0) {
                    if ((actorArray[i].m58_flags & 3) == 1) {
                        MulMatrix0(&worldScaleMatrix, &actorArray[i].mC_matrix, &projectedMatrix);
                    }
                    else {
                        copyRotationMatrix(&projectedMatrix, &actorArray[i].mC_matrix);
                        ScaleMatrix(&projectedMatrix, &scale2);
                    }
                    MulRotationMatrix(&computeProjectionMatrixTempMatrix, &projectedMatrix);
                }
            }

            if (pFieldEntity->m0->m12_renderMode == 1)
            {
                MissingCode();
            }
            objectClippingMask = 0;

            if (!(actorArray[i].m58_flags & 0x20))
            {
                if ((actorArray[i].m58_flags & 0x2000) && (pFieldEntity->m0->m14))
                {
                    MissingCode();
                }

                SetRotMatrix(&projectedMatrix);
                SetTransMatrix(&projectedMatrix);

                if (!isObjectOcluded(pFieldEntity->m0) || forceVisibility) {
                    SetRotMatrix(&projectedMatrix);
                    SetTransMatrix(&projectedMatrix);

                    if ((pFieldEntity->m58_flags & 0x8000) == 0) {
                        submitModelForRendering(&pFieldEntity->m0->m4_pModelBlock[0], pFieldEntity->m0->m8[g_frameOddOrEven], pCurrentFieldRenderingContext->mCC_OT.begin(), pFieldEntity->m0->m12_renderMode);
                    }
                    else {
                        submitModelForRendering(&pFieldEntity->m0->m4_pModelBlock[0], pFieldEntity->m0->m8[g_frameOddOrEven], pCurrentFieldRenderingContext->m40D0_secondaryOT.begin(), pFieldEntity->m0->m12_renderMode);
                    }
                }
            }
        }
    }

    if (fieldDebugDisable == 0) {
        assert(0);
    }
}

const std::array<sVec2_s16, 8> tpageLocationTable = { {
    {0x300, 0x0},
    {0x340, 0x0},
    {0x380, 0x0},
    {0x3C0, 0x0},

    {0x300, 0x100},
    {0x340, 0x100},
    {0x380, 0x100},
    {0x3C0, 0x100},
} };

std::array<SFP_VEC4, 4> shadowProjectionWorkArea;

void uploadCharacterSpriteSub1(sSpriteActorCore* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
    sPS1Pointer puVar16 = param_3->m0_spriteData->rawPointer;
    sPS1Pointer pbVar13 = puVar16 + READ_LE_U16(puVar16 + param_2 * 2);
    int iVar23 = READ_LE_U8(pbVar13 + 3) * param_1->m2C_scale;
    int bVar2 = READ_LE_U8(pbVar13);
    sPS1Pointer local_54 = pbVar13 + 4;
    sPS1Pointer pbVar18 = pbVar13 + (bVar2 & 0x3f) * 2 + 4;
    std::vector<sFieldEntitySub4_B4_sub>::iterator psVar13 = param_1->m20->getAsSprite()->m30->begin();
    short sVar8 = (short)(iVar23 >> 0xc);
    if (iVar23 < 0) {
        sVar8 = (short)((iVar23 + 0xfff) >> 0xc);
    }
    param_1->m36 = sVar8;
    iVar23 = READ_LE_U8(pbVar13 + 1) * param_1->m2C_scale;
    if (iVar23 < 0) {
        iVar23 = iVar23 + 0xfff;
    }
    param_1->m38 = (short)(iVar23 >> 0xc);
    u8 bVar3 = param_1->m3C & 0xFF;
    u16 texcoordStartY = param_3->m4_vramLocation.vy & 0xFF;
    sColorAndCode colorAndCode = param_1->m28_colorAndCode;
    u32 transformId = 4;
    u32 uVar22 = 0;

    if ((bVar2 & 0x3f) != 0) {
        do {
            psVar13->m9 = 0;
            psVar13->m8 = 0;
            psVar13->m14.m0x20_tileFlipY = 0;
            while (true) {
                u8 transformData = READ_LE_U8(pbVar18);
                if ((transformData & 0x80) == 0)
                    break;
                sPS1Pointer pbVar19 = pbVar18 + 1;
                if ((transformData & 0x40) == 0) {
                    if ((transformData & 4) != 0) {
                        psVar13->m14.m0x20_tileFlipY = 1;
                    }
                    if ((transformData & 1) != 0) {
                        psVar13->m8 = READ_LE_U8(pbVar19);
                        pbVar19 = pbVar18 + 2;
                    }
                    pbVar18 = pbVar19;
                    if ((transformData & 2) != 0) {
                        transformData = READ_LE_U8(pbVar19);
                        pbVar18 = pbVar18 + 1;
                        psVar13->m9 = transformData;
                    }
                }
                else {
                    transformId = transformData & 7;
                    if (param_1->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
                        param_1->m20->getAsSprite()->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
                        resetPerSubgroupTransforms(param_1);
                    }
                    if ((transformData & 0x20) != 0) {
                        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[transformId].m0_translateX = READ_LE_U8(pbVar18 + 1);
                        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[transformId].m1_translateY = READ_LE_U8(pbVar18 + 2);
                        pbVar19 = pbVar18 + 3;
                    }
                    pbVar18 = pbVar19;
                    if ((transformData & 0x10) == 0) {
                        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[transformId].m6_rotateZ = 0;
                    }
                    else {
                        transformData = READ_LE_U8(pbVar18);
                        pbVar18 = pbVar18 + 1;
                        (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[transformId].m6_rotateZ = (ushort)transformData << 4;
                    }
                }
            }
            sPS1Pointer pbVar19 = puVar16 + READ_LE_U16(local_54);
            u8 bVar4 = READ_LE_U8(pbVar19);
            u32 uVar17 = (uint)bVar4;
            local_54 = local_54 + 2;

            s8 texcoordStartX;
            if ((bVar4 & 1) == 0) {
                psVar13->m14.m0x8 = 0;
                texcoordStartX = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 2);
            }
            else {
                psVar13->m14.m0x8 = 1;
                texcoordStartX = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 1);
            }

            u8 bVar5 = READ_LE_U8(pbVar18);
            u8 bVar7 = bVar5 >> 4;
            u8 uVar15 = bVar7 & 3;
            psVar13->m10_colorAndCode = colorAndCode;
            if (((bVar7 & 3) != 0) || (uVar15 = (uint)(bVar3 >> 5), uVar15 != 0)) {
                uVar15 = uVar15 - 1;
                psVar13->m10_colorAndCode.m3_code |= 2;
            }
            if (((int)uVar17 >> 4 & 1U) == 0) {
                std::optional<sPS1Pointer> iVar12;
                if (((param_1->mA8.mx0) == 1) && (iVar12 = param_1->m7C->m18, iVar12.has_value() != 0)) {
                    sPS1Pointer puVar11 = iVar12.value() + ((uVar17 & 0xe) * 2);
                    u16 uVar6 = READ_LE_U16(puVar11 + 2);
                    u16 uVar14 = READ_LE_U16(puVar11);
                    texcoordStartY = uVar6 & 0xff;
                    texcoordStartX = (char)((uVar14 & 0x3f) >> 2);
                    psVar13->mA_tpage = (ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((uVar14 & 0x3ff) >> 6) | (ushort)((uVar6 & 0x200) << 2);
                }
                else {
                    u16 uVar6 = (param_3->m4_vramLocation).vy;
                    psVar13->mA_tpage =
                        (ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((int)((int)(param_3->m4_vramLocation).vx + (uVar17 & 0xe) * 0x20 & 0x3ff) >> 6) |
                        (ushort)((uVar6 & 0x200) << 2);
                }
                psVar13->mC_clut = (param_3->m8_clut).vy << 6 | (ushort)((int)((int)(param_3->m8_clut).vx + (bVar5 & 0xf) * 0x10) >> 4) & 0x3f;
            }
            else {
                pbVar19 = pbVar19 + 1;
                uVar17 = (READ_LE_U8(pbVar19) << 8) | bVar4;
                int tpageLocationIndex = (uVar17 >> 1) & 7;
                psVar13->mA_tpage = GetTPage(uVar17 & 1, (int)(short)uVar15, (int)tpageLocationTable[tpageLocationIndex].vx, (int)tpageLocationTable[tpageLocationIndex].vy);
                psVar13->mC_clut = GetClut((int)uVar17 >> 1 & 0xf0, ((int)uVar17 >> 9 & 0xfU) + 0x1cc);
                                
            }
            psVar13->m14.m0x0 = transformId;
            psVar13->m4_texcoordX = texcoordStartX + READ_LE_U8(pbVar19 + 1);
            psVar13->m5_texcoordY = texcoordStartY + READ_LE_U8(pbVar19 + 2);
            psVar13->m6_width = READ_LE_U8(pbVar19 + 3);
            psVar13->m7_height = READ_LE_U8(pbVar19 + 4);
            psVar13->m14.m0x10_tileFlipX = READ_LE_U8(pbVar18) >> 6;
            if ((bVar2 & 0x80) == 0) {
                psVar13->m0 = READ_LE_S8(pbVar18 + 1);
                psVar13->m2 = READ_LE_S8(pbVar18 + 2);
            }
            else {
                psVar13->m0 = (s16)READ_LE_U8(pbVar18 + 1) | (ushort)(((uint)READ_LE_S8(pbVar18 + 2) << 0x18) >> 0x10);
                pbVar19 = pbVar18 + 3;
                sPS1Pointer pbVar1 = pbVar18 + 4;
                pbVar18 = pbVar18 + 2;
                psVar13->m2 = (ushort)READ_LE_U8(pbVar19) | (ushort)(((uint)READ_LE_U8(pbVar1) << 0x18) >> 0x10);
            }
            psVar13 = psVar13 + 1;
            uVar22 = uVar22 + 1;
            pbVar18 = pbVar18 + 3;
        } while (uVar22 != (bVar2 & 0x3f));
    }
    param_1->m40 = param_1->m40 & 0xffffff03 | (uVar22 & 0x3f) << 2;
}

s16 specialEffectSpriteX = 0;
s16 specialEffectSpriteY = 0;

sVec2_s16* allocateSpecialEffectSpriteVram(sVec2_s16* param_1, int param_2) {
    if (0x40 < specialEffectSpriteX + param_2) {
        specialEffectSpriteX = 0;
        specialEffectSpriteY = specialEffectSpriteY + 1;
        if (2 < specialEffectSpriteY) {
            specialEffectSpriteY = 0;
        }
    }
    
    param_1->set(specialEffectSpriteX + 0x300, 0x140 + specialEffectSpriteY * 0x40);
    specialEffectSpriteX += param_2;

    return param_1;
}

void uploadCharacterSprite(sSpriteActorCore* param_1, int spriteFrame, sFieldEntitySub4_110* param_3)
{
    param_1->m40 &= ~0x000a0000;
    sPS1Pointer dataPtr = param_3->m0_spriteData->rawPointer;
    if (spriteFrame < param_3->m0_spriteData->m0_header.mx01FF_frameCount + 1) {
        if ((param_1->m3C >> 0x1e & 1) != 0) {
            param_1->m3C &= ~0x40000000;
            std::span<u8>::iterator puVar10 = param_3->mC->begin();
            if (READ_LE_U16(puVar10) != 0) {
                // Transfer the clut
                auto clutIt = puVar10 + (uint)READ_LE_U16(puVar10) * ((param_1->m3C >> 16) & 0xf0) * 2 + 4;
                int clutSize = READ_LE_U16(puVar10) * 16;
                addToShapeTransfertTable(clutIt, param_3->m8_clut.vx, param_3->m8_clut.vy, clutSize, 1);
            }
        }
        if (!param_3->m0_spriteData->m0_header.mx8000_isVramPrebacked) {
            // character sprite needs to be uploaded every frame (playable characters)
            sVec2_s16 vramLocation = param_1->m24_vramData->m4_vramLocation;
            sPS1Pointer framePtr = param_3->m0_spriteData->m4_frames[spriteFrame].rawPointer;
            sFieldEntitySub4_110_0_frame& frame = param_3->m0_spriteData->m4_frames[spriteFrame];

            if ((param_1->m40 >> 0xd & 0xf) == 0xe) {
                allocateSpecialEffectSpriteVram(&vramLocation, READ_LE_U8(framePtr + 4));
            }

            u8 frameHeader = READ_LE_U8(framePtr);
            u8 numSubSprites = (frameHeader & 0x3f);
            sPS1Pointer puVar19 = framePtr + 6;
            sPS1Pointer pSubSprite = framePtr + 6 + numSubSprites * 4;
            int height = READ_LE_U8(framePtr + 3) * param_1->m2C_scale;
            if (height < 0) {
                height += 0xfff;
            }
            param_1->m36 = (short)(height >> 0xc);

            int width = READ_LE_U8(framePtr + 1) * param_1->m2C_scale;
            if (width < 0) {
                width += 0xfff;
            }
            param_1->m38 = (short)(width >> 0xc);

            int bVar7 = *(byte*)&param_1->m3C >> 5;
            int baseTexcoordX = (uint)vramLocation.vx & 0x3f;
            sColorAndCode colorAndCode = param_1->m28_colorAndCode;
            int uVar18 = 4;
            int uVar20 = 0;

            std::vector<sFieldEntitySub4_B4_sub>::iterator pFieldEntitySub4_B4_sub = param_1->m20->getAsSprite()->m30->begin();

            if ((frameHeader & 0x3f) != 0) {
                do {
                    pFieldEntitySub4_B4_sub->m9 = 0;
                    pFieldEntitySub4_B4_sub->m8 = 0;
                    pFieldEntitySub4_B4_sub->m14.m0x20_tileFlipY = 0;
                    while (true) {
                        int bVar3 = READ_LE_U8(pSubSprite);
                        if ((bVar3 & 0x80) == 0)
                            break;

                        if ((bVar3 & 0x40) == 0) {
                            auto pbVar17 = pSubSprite + 1;
                            if ((bVar3 & 4) != 0) {
                                pFieldEntitySub4_B4_sub->m14.m0x20_tileFlipY = 1;
                            }
                            if ((bVar3 & 1) != 0) {
                                pFieldEntitySub4_B4_sub->m8 = READ_LE_U8(pbVar17);
                                pbVar17 = pSubSprite + 2;
                            }
                            pSubSprite = pbVar17;
                            if ((bVar3 & 2) != 0) {
                                bVar3 = READ_LE_U8(pSubSprite);
                                pSubSprite = pSubSprite + 1;
                                pFieldEntitySub4_B4_sub->m9 = bVar3;
                            }
                        }
                        else {
                            sPS1Pointer pbVar17 = pSubSprite + 1;
                            if (param_1->m20->getAsSprite()->m34_perSubgroupTransform == nullptr) {
                                param_1->m20->getAsSprite()->m34_perSubgroupTransform = new std::array<sFieldEntitySub4_124, 8>;
                                resetPerSubgroupTransforms(param_1);
                            }
                            uVar18 = bVar3 & 7;
                            if ((bVar3 & 0x20) != 0) {
                                (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar18].m0_translateX = READ_LE_U8(pSubSprite + 1);
                                (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar18].m1_translateY = READ_LE_U8(pSubSprite + 2);
                                pbVar17 = pSubSprite + 3;
                            }
                            pSubSprite = pbVar17;
                            if ((bVar3 & 0x10) == 0) {
                                (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar18].m6_rotateZ = 0;
                            }
                            else {
                                bVar3 = READ_LE_U8(pSubSprite);
                                pSubSprite = pSubSprite + 1;
                                (*param_1->m20->getAsSprite()->m34_perSubgroupTransform)[uVar18].m6_rotateZ = (ushort)bVar3 << 4;
                            }
                        }
                    }
                    sPS1Pointer pbVar17 = dataPtr + READ_LE_U16(puVar19) * 4;

                    int uVar5 = READ_LE_U16(puVar19 + 2);
                    int uVar6 = READ_LE_U16(pbVar17 + 2);
                    puVar19 = puVar19 + 4;
                    int tileOffsetInTexcoordX = uVar5 & 0x1f;
                    int tileOffsetInTexcoordY = uVar5 >> 5 & 0x3f;
                    int width;

                    int colorDepth = uVar6 & 1; // 0: 4bit, 1: 8bit

                    if (colorDepth == 0) {
                        // 4bit
                        pFieldEntitySub4_B4_sub->m4_texcoordX = (baseTexcoordX + tileOffsetInTexcoordX) * 4;
                        width = READ_LE_U8(pbVar17) >> 2;
                        pFieldEntitySub4_B4_sub->m14.m0x8 = 0;
                    }
                    else {
                        // 8bit
                        pFieldEntitySub4_B4_sub->m4_texcoordX = (baseTexcoordX + tileOffsetInTexcoordX) * 2;
                        width = READ_LE_U8(pbVar17) >> 1;
                        pFieldEntitySub4_B4_sub->m14.m0x8 = 1;
                    }
                    pFieldEntitySub4_B4_sub->m14.m0x0 = uVar18;
                    pFieldEntitySub4_B4_sub->m5_texcoordY = vramLocation.vy + tileOffsetInTexcoordY;
                    pFieldEntitySub4_B4_sub->m6_width = READ_LE_U8(pbVar17);
                    pFieldEntitySub4_B4_sub->m7_height = READ_LE_U8(pbVar17 + 1);

                    int bVar4 = READ_LE_U8(pSubSprite);

                    int unkBit = (bVar4 >> 6) & 1;
                    int blending = (bVar4 >> 4) & 3;
                    int clutXOffset = (bVar4 >> 0) & 0xf;

                    pFieldEntitySub4_B4_sub->m14.m0x10_tileFlipX = unkBit;

                    pFieldEntitySub4_B4_sub->m10_colorAndCode = colorAndCode;

                    if (blending || (blending = (uint)bVar7, bVar7 != 0)) {
                        blending = blending - 1;
                        (pFieldEntitySub4_B4_sub->m10_colorAndCode).m3_code |= 2; // enable ABE (semi-transparent)
                    }

                    pFieldEntitySub4_B4_sub->mA_tpage = GetTPage(colorDepth, blending, vramLocation.vx, vramLocation.vy);
                    pFieldEntitySub4_B4_sub->mC_clut = GetClut(param_3->m8_clut.vx + clutXOffset * 0x10, param_3->m8_clut.vy);

                    addToShapeTransfertTable(pbVar17 + 4, vramLocation.vx + tileOffsetInTexcoordX, vramLocation.vy + tileOffsetInTexcoordY, (ushort)width, (ushort)READ_LE_U8(pbVar17 + 1));
                    if ((frameHeader & 0x80) == 0) {
                        pFieldEntitySub4_B4_sub->m0 = (short)(char)READ_LE_U8(pSubSprite + 1);
                        pFieldEntitySub4_B4_sub->m2 = (short)(char)READ_LE_U8(pSubSprite + 2);
                    }
                    else {
                        pFieldEntitySub4_B4_sub->m0 = (ushort)READ_LE_U8(pSubSprite + 1) | (ushort)(((uint)READ_LE_U8(pSubSprite + 2) << 0x18) >> 0x10);
                        pbVar17 = pSubSprite + 3;
                        sPS1Pointer pbVar1 = pSubSprite + 4;
                        pSubSprite = pSubSprite + 2;
                        pFieldEntitySub4_B4_sub->m2 = (ushort)READ_LE_U8(pbVar17) | (ushort)(((uint)READ_LE_U8(pbVar1) << 0x18) >> 0x10);
                    }
                    pFieldEntitySub4_B4_sub = pFieldEntitySub4_B4_sub + 1;
                    uVar20 = uVar20 + 1;
                    pSubSprite = pSubSprite + 3;
                } while (uVar20 != (frameHeader & 0x3f));
            }
            param_1->m40 = param_1->m40 & 0xffffff03 | (uVar20 & 0x3f) << 2;
            std::optional<std::span<u8>::iterator> pointer;
            addToShapeTransfertTable(pointer, vramLocation.vx, vramLocation.vy, READ_LE_U8(framePtr + 4), READ_LE_U8(framePtr + 5));
        }
        else
        {
            uploadCharacterSpriteSub1(param_1, spriteFrame, param_3);
        }
    }
}

void uploadCharacterSprites()
{
    sSpriteActorCore* psVar1 = spriteTransfertListHead;
    if (spriteTransfertListHead != (sSpriteActor*)0x0) {
        do {
            if ((ushort)psVar1->m34_currentSpriteFrame == 0) {
                psVar1->m40 &= ~0x000000fc;
            }
            else {
                uploadCharacterSprite(psVar1, psVar1->m34_currentSpriteFrame, psVar1->m24_vramData);
            }
            psVar1 = psVar1->m20->getAsSprite()->m38_pNext;
        } while (psVar1 != (sSpriteActor*)0x0);
    }
    spriteTransfertListHead = (sSpriteActor*)0x0;
}

int frameOddOrEven = 0;

void clearShapeTransfertTableEntry(int param_1)
{
    void* ppvVar1 = shapeTransfertTemporaryBuffersLinkedLists[param_1];
    shapeTransfertTableStart = shapeTransfertBuffer[param_1];
    shapeTransfertTableEnd = shapeTransfertTableStart + shapeTransfertTableSize;
    shapeTransfertTableCurrentEntry = shapeTransfertTableStart;
    shapeTransfertDoubleBufferIndex = param_1;
    while (ppvVar1 != nullptr) {
        assert(0);
        /*free(*ppvVar1);
        ppvVar1 = (void**)ppvVar1[1];*/
    }
    shapeTransfertTemporaryBuffersLinkedLists[param_1] = nullptr;
    return;
}

MATRIX currentRenderingMatrix;

void setCurrentRenderingMatrix(MATRIX* pMatrix)
{
    currentRenderingMatrix = *pMatrix;
}

OTTable* characterRenderingOT = nullptr;

void setCharacterRenderingOT(OTTable& OT)
{
    characterRenderingOT = &OT;
}

int disableFogForCharacters = 0;
bool disableCharacterShadowsRendering = 0;



void setupSpriteActorTransform(sSpriteActorCore* pSpriteSheet)
{
    if ((isBattleOverlayLoaded != '\0') || (isWorldMapOverlayLoaded != '\0')) {
        spriteCallback_render2_updateMatrix(pSpriteSheet);
    }

    SFP_VEC4 local_30;
    FP_VEC4 local_28;
    uint uVar1;
    
    int iVar3;
    int iVar4;

    uVar1 = pSpriteSheet->m40 >> 8 & 0x1f;
    iVar3 = (int)(char)pSpriteSheet->m20->getAsSprite()->m3C << uVar1;
    if ((pSpriteSheet->mAC.mx2_facing) != 0) {
        iVar3 = -iVar3;
    }
    iVar4 = ((int)(char)pSpriteSheet->m20->getAsSprite()->m3D << uVar1) * (int)pSpriteSheet->m2C_scale;
    iVar3 = iVar3 * pSpriteSheet->m2C_scale;
    if (iVar4 < 0) {
        iVar4 = iVar4 + 0xfff;
    }
    if (iVar3 < 0) {
        iVar3 = iVar3 + 0xfff;
    }
    local_30.vx = (pSpriteSheet->m0_position).vx >> 0x10;
    local_30.vy = (pSpriteSheet->m0_position).vy >> 0x10;
    local_30.vz = (pSpriteSheet->m0_position).vz >> 0x10;
    ApplyMatrix(&currentRenderingMatrix, &local_30, &local_28);
    sFieldEntitySub4_B4_base* psVar2 = pSpriteSheet->m20;
    (psVar2->mC_matrix).t[0] = currentRenderingMatrix.t[0] + local_28.vx + (iVar3 >> 0xc);
    (psVar2->mC_matrix).t[1] = currentRenderingMatrix.t[1] + local_28.vy + (iVar4 >> 0xc);
    (psVar2->mC_matrix).t[2] = currentRenderingMatrix.t[2] + local_28.vz;
    SetRotMatrix(&psVar2->mC_matrix);
    SetTransMatrix(&psVar2->mC_matrix);
}

SFP_VEC4 currentSpriteCharacterSize[4];

std::array<s16, 8> spriteMatrixTable = {
    1,2,4,8,0x10,0x20,0x40,0x80,
};

void submitSpriteActorToRendering(sSpriteActorCore* pSpriteSheet, sTag* pTag)
{
    sFieldEntitySub4_B4* psVar7 = pSpriteSheet->m20->getAsSprite();
    u32 uVar9 = pSpriteSheet->m40 >> 8 & 0x1f;
    s8 bVar1 = psVar7->m3D;
    s16 spriteWidth = (short)((int)(char)psVar7->m3C << uVar9);
    if ((pSpriteSheet->mAC.mx2_facing) != 0) {
        spriteWidth = -spriteWidth;
    }
    u32 numPolyInSprite = pSpriteSheet->m40 >> 2 & 0x3f;
    int currentBoundMatrixId = -1;

    int isVisible = 0;

    // enough space in the shape transfer buffer to write the poly?
    if ((shapeTransfertTableCurrentEntry + numPolyInSprite * sizeof(POLY_FT4) < shapeTransfertTableEnd)) {
        for (int i = 0; i < numPolyInSprite; i++)
        {
            sFieldEntitySub4_B4_sub* pSpriteDefinition = &(*psVar7->m30)[i];
            int matrixIdNeeded = pSpriteDefinition->m14.m0x0;
            if (currentBoundMatrixId != matrixIdNeeded) { // need to change the matrix?
                isVisible = (spriteMatrixTable[matrixIdNeeded] & (pSpriteSheet->m3C >> 8) & 0xFF) == 0;
                if (psVar7->m34_perSubgroupTransform == nullptr)
                {
                    SetRotMatrix(&pSpriteSheet->m20->mC_matrix);
                    SetTransMatrix(&pSpriteSheet->m20->mC_matrix);
                }
                else
                {
                    sFieldEntitySub4_124* psVar8 = &(*psVar7->m34_perSubgroupTransform)[matrixIdNeeded];
                    if ((psVar8->m0_translateX == 0) && (psVar8->m1_translateY == 0) && (psVar8->m6_rotateZ == 0))
                    {
                        SetRotMatrix(&pSpriteSheet->m20->mC_matrix);
                        SetTransMatrix(&pSpriteSheet->m20->mC_matrix);
                    }
                    else
                    {
                        int spriteScale = pSpriteSheet->m40 >> 8 & 0x1f;

                        int spriteTranslateX = (int)(char)psVar8->m0_translateX << spriteScale;
                        if ((pSpriteSheet->m3C >> 3 & 1) != 0) {
                            spriteTranslateX = -spriteTranslateX;
                        }
                        spriteTranslateX *= pSpriteSheet->m2C_scale;
                        if (spriteTranslateX < 0) {
                            spriteTranslateX = spriteTranslateX + 0xfff;
                        }

                        int spriteTranslateY = ((int)(char)psVar8->m1_translateY << spriteScale) * (int)pSpriteSheet->m2C_scale;
                        if (spriteTranslateY < 0) {
                            spriteTranslateY = spriteTranslateY + 0xfff;
                        }

                        SFP_VEC4 localRotationVector;
                        localRotationVector.vx = psVar8->m2_rotateX;
                        localRotationVector.vy = psVar8->m4_rotateY;
                        localRotationVector.vz = psVar8->m6_rotateZ;
                        if ((pSpriteSheet->m3C >> 3 & 1) != 0) {
                            localRotationVector.vz = -localRotationVector.vz;
                        }
                        MATRIX MStack112;
                        createRotationMatrix(&localRotationVector, &MStack112);
                        MStack112.t[0] = (pSpriteSheet->m20->mC_matrix).t[0] + (spriteTranslateX >> 0xc);
                        MStack112.t[1] = (pSpriteSheet->m20->mC_matrix).t[1] + (spriteTranslateY >> 0xc);
                        MStack112.t[2] = (pSpriteSheet->m20->mC_matrix).t[2];
                        SetMulMatrix(&pSpriteSheet->m20->mC_matrix, &MStack112);
                        SetTransMatrix(&MStack112);
                    }
                }
                currentBoundMatrixId = matrixIdNeeded;
            }
            if (isVisible)
            {
                POLY_FT4* p = new(shapeTransfertTableCurrentEntry) POLY_FT4;

                shapeTransfertTableCurrentEntry += sizeof(POLY_FT4);

                p->m3_size = 9;
                p->r0 = pSpriteDefinition->m10_colorAndCode.m0_r;
                p->g0 = pSpriteDefinition->m10_colorAndCode.m1_g;
                p->b0 = pSpriteDefinition->m10_colorAndCode.m2_b;
                p->code = pSpriteDefinition->m10_colorAndCode.m3_code;
                p->tpage = pSpriteDefinition->mA_tpage;
                p->clut = pSpriteDefinition->mC_clut;

                int spriteScale = pSpriteSheet->m40 >> 8 & 0x1f;
                int spriteHeight = (short)(((int)(((uint)pSpriteDefinition->m6_width + (int)(char)pSpriteDefinition->m8) * 0x10000) >> 0x10) << spriteScale);
                int sVar13 = (short)(((int)(((uint)pSpriteDefinition->m7_height + (int)(char)pSpriteDefinition->m9) * 0x10000) >> 0x10) << spriteScale);
                int sVar11 = (short)((int)pSpriteDefinition->m0 << spriteScale);

                currentSpriteCharacterSize[3].vy = (short)((int)pSpriteDefinition->m2 << spriteScale);
                if ((pSpriteSheet->m3C >> 3 & 1) != 0) {
                    spriteHeight = -spriteHeight;
                    sVar11 = -sVar11;
                }
                if ((pSpriteSheet->m3C >> 4 & 1) != 0) {
                    sVar13 = -sVar13;
                    currentSpriteCharacterSize[3].vy = -currentSpriteCharacterSize[3].vy;
                }
                currentSpriteCharacterSize[3].vx = sVar11 + spriteHeight;
                currentSpriteCharacterSize[2].vx = sVar11;
                if (!pSpriteDefinition->m14.m0x10_tileFlipX) {
                    currentSpriteCharacterSize[2].vx = currentSpriteCharacterSize[3].vx;
                    currentSpriteCharacterSize[3].vx = sVar11;
                }
                currentSpriteCharacterSize[1].vy = currentSpriteCharacterSize[3].vy + sVar13;
                if (!pSpriteDefinition->m14.m0x20_tileFlipY) {
                    currentSpriteCharacterSize[1].vy = currentSpriteCharacterSize[3].vy;
                    currentSpriteCharacterSize[3].vy = currentSpriteCharacterSize[3].vy + sVar13;
                }

                {
                    int spriteHeight = (short)((int)(char)psVar7->m3D << uVar9);
                    currentSpriteCharacterSize[0].vy = currentSpriteCharacterSize[1].vy - spriteHeight;
                    currentSpriteCharacterSize[1].vy = currentSpriteCharacterSize[1].vy - spriteHeight;
                    currentSpriteCharacterSize[2].vy = currentSpriteCharacterSize[3].vy - spriteHeight;
                    currentSpriteCharacterSize[3].vy = currentSpriteCharacterSize[3].vy - spriteHeight;
                    currentSpriteCharacterSize[0].vx = currentSpriteCharacterSize[3].vx - spriteWidth;
                    currentSpriteCharacterSize[1].vx = currentSpriteCharacterSize[2].vx - spriteWidth;
                    currentSpriteCharacterSize[2].vx = currentSpriteCharacterSize[2].vx - spriteWidth;
                    currentSpriteCharacterSize[3].vx = currentSpriteCharacterSize[3].vx - spriteWidth;
                }

                long lStack60;
                long lStack56;
                RotTransPers4(&currentSpriteCharacterSize[0], &currentSpriteCharacterSize[1], &currentSpriteCharacterSize[2], &currentSpriteCharacterSize[3], &p->x0y0, &p->x1y1, &p->x3y3, &p->x2y2, &lStack60, &lStack56);

                int UV_xStart = pSpriteDefinition->m4_texcoordX;
                int UV_yStart = pSpriteDefinition->m5_texcoordY;
                int UV_Width = pSpriteDefinition->m6_width - 1;
                int UV_Height = pSpriteDefinition->m7_height - 1;

                if (((p->x3y3.vx) < (p->x0y0.vx)) && (UV_xStart = UV_xStart - 1, (int)UV_xStart < 0)) {
                    UV_xStart = 0;
                    UV_Width = pSpriteDefinition->m6_width - 2;
                }

                p->u0 = UV_xStart;
                p->v0 = UV_yStart;

                p->u1 = UV_xStart + UV_Width;
                p->v1 = UV_yStart;

                p->u2 = UV_xStart;
                p->v2 = UV_yStart + UV_Height;

                p->u3 = UV_xStart + UV_Width;
                p->v3 = UV_yStart + UV_Height;

                if ((pSpriteSheet->m3C >> 0x1b & 1) == 0) {
                    p->m0_pNext = pTag->m0_pNext;
                    pTag->m0_pNext = p;
                }
                else {
                    sTag* puVar9 = pTag + -currentBoundMatrixId;
                    p->m0_pNext = puVar9->m0_pNext;
                    puVar9->m0_pNext = p;
                }
            }
        }
    }
}

void renderSpriteSquishedShadow(sSpriteActorCore* pSpriteSheet, sTag* pTag)
{
    MATRIX tempMatrix = currentRenderingMatrix;
    SVECTOR tempSVector1;
    tempSVector1.vx = pSpriteSheet->m0_position.vx.getIntegerPart();
    tempSVector1.vy = pSpriteSheet->m0_position.vy.getIntegerPart();
    tempSVector1.vz = pSpriteSheet->m0_position.vz.getIntegerPart();
    VECTOR tempVector2;
    tempVector2.vx = pSpriteSheet->m2C_scale;
    tempVector2.vz = 0;
    s32 scale = pSpriteSheet->m2C_scale << 0x10;
    tempVector2.vy = (scale >> 0x10) - (scale >> 0x1f) >> 1;
    ScaleMatrixL(&tempMatrix, &tempVector2);
    tempSVector1.vy = pSpriteSheet->m84_maxY;
    VECTOR tempVector3;
    ApplyMatrix(&currentRenderingMatrix, &tempSVector1, &tempVector3);
    tempMatrix.t[0] = tempMatrix.t[0] + tempVector3.vx;
    tempMatrix.t[1] = tempMatrix.t[1] + tempVector3.vy;
    tempMatrix.t[2] = tempMatrix.t[2] + tempVector3.vz;
    SetRotMatrix(&tempMatrix);
    SetTransMatrix(&tempMatrix);

    auto uVar5 = (pSpriteSheet->m40 & 0xFF) >> 2;
    auto* sprite = pSpriteSheet->m20->getAsSprite();
    auto psVar4 = sprite->m30->begin();
    s32 uVar11 = -1;
    s32 uVar10;
    s32 unaff_s6;
    if ((shapeTransfertTableCurrentEntry + uVar5 * sizeof(POLY_FT4) < shapeTransfertTableEnd) &&
        (uVar10 = 0, uVar5 != 0)) {
        do {
            POLY_FT4* pbVar2 = (POLY_FT4*)shapeTransfertTableCurrentEntry;
            auto uVar9 = psVar4->m14.raw;
            uVar5 = uVar9 & 7;
            if (uVar11 != uVar5) {
                unaff_s6 = (ushort)(spriteMatrixTable[uVar5] &
                    (ushort) ((int)&(*pSpriteSheet).m3C>>8)) == 0;
                uVar11 = uVar5;
            }
            if (unaff_s6 != '\0') {
                uVar5 = (*pSpriteSheet).m40 >> 8 & 0x1f;
                auto sVar7 = (short)(((int)(((uint)psVar4->m6_width + (int)(char)psVar4->m8) * 0x10000) >> 0x10) << uVar5);
                auto sVar8 = (short)(((int)(((uint)psVar4->m7_height + (int)(char)psVar4->m9) * 0x10000) >> 0x10) << uVar5);
                auto sVar6 = (short)((int)psVar4->m0 << uVar5);
                shadowProjectionWorkArea[3].vz = (short)((int)psVar4->m2 << uVar5);
                uVar5 = (*pSpriteSheet).m3C;
                if ((uVar5 >> 3 & 1) != 0) {
                    sVar7 = -sVar7;
                    sVar6 = -sVar6;
                }
                if ((uVar5 >> 4 & 1) != (uVar9 >> 5 & 1)) {
                    sVar8 = -sVar8;
                    shadowProjectionWorkArea[3].vz = -shadowProjectionWorkArea[3].vz;
                }
                shadowProjectionWorkArea[3].vx = sVar6 + sVar7;
                shadowProjectionWorkArea[2].vx = sVar6;
                if ((uVar9 >> 4 & 1) == 0) {
                    shadowProjectionWorkArea[2].vx = shadowProjectionWorkArea[3].vx;
                    shadowProjectionWorkArea[3].vx = sVar6;
                }
                shadowProjectionWorkArea[1].vz = shadowProjectionWorkArea[3].vz + sVar8;
                if (psVar4->m14.m0x10_tileFlipX) {
                    shadowProjectionWorkArea[1].vz = shadowProjectionWorkArea[3].vz;
                    shadowProjectionWorkArea[3].vz = shadowProjectionWorkArea[3].vz + sVar8;
                }
                shadowProjectionWorkArea[0].vx = shadowProjectionWorkArea[3].vx;
                shadowProjectionWorkArea[0].vz = shadowProjectionWorkArea[1].vz;
                shadowProjectionWorkArea[1].vx = shadowProjectionWorkArea[2].vx;
                shadowProjectionWorkArea[2].vz = shadowProjectionWorkArea[3].vz;
                shapeTransfertTableCurrentEntry = shapeTransfertTableCurrentEntry + sizeof(POLY_FT4);
                pbVar2->m3_size = 9;
                pbVar2->code = 0x2C;
                pbVar2->r0 = 0;
                pbVar2->g0 = 0;
                pbVar2->b0 = 0;
                long lStack_30;
                long lStack_2c;

                RotAverage4(&shadowProjectionWorkArea[0], &shadowProjectionWorkArea[1], &shadowProjectionWorkArea[2], &shadowProjectionWorkArea[3],
                    &pbVar2->x0y0, &pbVar2->x1y1, &pbVar2->x2y2, &pbVar2->x3y3, &lStack_30, &lStack_2c);

                scale = ((uint)pbVar2->x0y0.vy + (uint)pbVar2->x1y1.vy) * 0x10000;
                auto uVar3 = (s16)((scale >> 0x10) - (scale >> 0x1f) >> 1);

                pbVar2->x1y1.vy = uVar3;
                pbVar2->x0y0.vy = uVar3;

                scale = ((uint)pbVar2->x2y2.vy + (uint)pbVar2->x3y3.vy) * 0x10000;
                uVar3 = (s16)((scale >> 0x10) - (scale >> 0x1f) >> 1);

                pbVar2->x3y3.vy = uVar3;
                pbVar2->x2y2.vy = uVar3;

                pbVar2->tpage = psVar4->mA_tpage;
                pbVar2->clut = psVar4->mC_clut;

                pbVar2->u0 = psVar4->m4_texcoordX;
                pbVar2->v0 = psVar4->m5_texcoordY;
                pbVar2->u1 = psVar4->m6_width + psVar4->m4_texcoordX + -1;
                pbVar2->v1 = psVar4->m5_texcoordY;
                pbVar2->u2 = psVar4->m4_texcoordX;
                pbVar2->v2 = psVar4->m7_height + psVar4->m5_texcoordY + -1;
                pbVar2->u0 = psVar4->m6_width + psVar4->m4_texcoordX + -1;
                pbVar2->v0 = psVar4->m7_height + psVar4->m5_texcoordY + -1;

                pbVar2->m0_pNext = pTag->m0_pNext;
                pTag->m0_pNext = pbVar2;
            }
            uVar10 = uVar10 + 1;
            psVar4 = psVar4 + 1;
        } while (uVar10 != (pSpriteSheet->m40 & 0xFF) >> 2);
    }
}

void renderSpriteActor(sSpriteActorCore* pSpriteSheet, sTag* pTag)
{
    setupSpriteActorTransform(pSpriteSheet);
    submitSpriteActorToRendering(pSpriteSheet, pTag);
    if ((pSpriteSheet->m3C >> 2 & 1) != 0) {
        renderSpriteSquishedShadow(pSpriteSheet, pTag);
    }
}

void renderFieldCharacterSprites(OTTable& OT, int oddOrEven)
{
    int cameraDirection = getCameraDirection() & 0xFFFF;
    MATRIX localRotationMatrix;
    copyRotationMatrix(&localRotationMatrix, &computeProjectionMatrixTempMatrix);

    SFP_VEC4 tempVector;
    tempVector.vx = 0;
    tempVector.vy = (sceneDIP / 3) * -2;
    tempVector.vz = 0;
    if (totalActors)
    {
        std::array<sLoadedMechas*, 10>::iterator mechaIt = loadedMechas.begin();
        std::array<int, 10>::iterator mechaList3It = mechaList3.begin();

        for (int currentActorIndex = 0; currentActorIndex < totalActors; currentActorIndex++)
        {
            sFieldEntity* pFieldEntity = &actorArray[currentActorIndex];
            if ((pFieldEntity->m58_flags & 0x40) != 0) {
                sFieldScriptEntity* pScriptEntity = pFieldEntity->m4C_scriptEntity;
                sSpriteActor* pSpriteSheet = pFieldEntity->m4_pVramSpriteSheet;
                int flags = pScriptEntity->m4_flags.m_rawFlags;

                pFieldEntity->m2C_matrixBackup = pFieldEntity->mC_matrix;

                if ((flags & 0x2000) == 0) {
                    MATRIX tempMatrix;
                    SetRotMatrix(&currentProjectionMatrix);

                    setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[0][0], pFieldEntity->mC_matrix.m[1][0]));
                    setCopReg(2, 0x800, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[2][0], 0));
                    copFunction(2, 0x49e012);
                    tempMatrix.m[0][0] = getCopReg(2, 0x4800);
                    tempMatrix.m[1][0] = getCopReg(2, 0x5000);
                    tempMatrix.m[2][0] = getCopReg(2, 0x5800);

                    setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[0][1], pFieldEntity->mC_matrix.m[1][1]));
                    setCopReg(2, 0x800, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[2][1], 0));
                    copFunction(2, 0x49e012);
                    tempMatrix.m[0][1] = getCopReg(2, 0x4800);
                    tempMatrix.m[1][1] = getCopReg(2, 0x5000);
                    tempMatrix.m[2][1] = getCopReg(2, 0x5800);

                    setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[0][2], pFieldEntity->mC_matrix.m[1][2]));
                    setCopReg(2, 0x800, sVec2_s16::fromValue(pFieldEntity->mC_matrix.m[2][2], 0));
                    copFunction(2, 0x49e012);
                    tempMatrix.m[0][2] = getCopReg(2, 0x4800);
                    tempMatrix.m[1][2] = getCopReg(2, 0x5000);
                    tempMatrix.m[2][2] = getCopReg(2, 0x5800);

                    setCopControlWord(2, 0x2800, currentProjectionMatrix.t[0]);
                    setCopControlWord(2, 0x3000, currentProjectionMatrix.t[1]);
                    setCopControlWord(2, 0x3800, currentProjectionMatrix.t[2]);
                    setCopReg(2, 0, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[0], pFieldEntity->mC_matrix.t[1]));
                    setCopReg(2, 1, sVec2_s16::fromValue(pFieldEntity->mC_matrix.t[2], 0));
                    copFunction(2, 0x480012);
                    tempMatrix.t[0] = getCopReg(2, 0x19);
                    tempMatrix.t[1] = getCopReg(2, 0x1a);
                    tempMatrix.t[2] = getCopReg(2, 0x1b);

                    SetRotMatrix(&tempMatrix);
                    SetTransMatrix(&tempMatrix);

                    setCopReg(2, 0, sVec2_s16::fromValue(tempVector.vx, tempVector.vy));
                    setCopReg(2, 0x800, sVec2_s16::fromValue(tempVector.vz, 0));
                    copFunction(2, 0x180001);
                    sVec2_s16 local_50 = sVec2_s16::fromS32(getCopReg(2, 0xe));
                    s32 p = getCopReg(2, 8);
                    s32 mathFlag = getCopControlWord(2, 0xf800);
                    s32 spriteDepth = getCopReg(2, 0x9800) >> 2;
                    if (((local_50.vy) + 9U < 323) && (local_50.vx + 0x27U < 399)) {
                        flags = pScriptEntity->m4_flags.m_rawFlags & ~0x200;
                    }
                    else {
                        flags = pScriptEntity->m4_flags.m_rawFlags | 0x200;
                    }
                    pScriptEntity->m4_flags.m_rawFlags = flags;

                    if (((disableCharacterShadowsRendering == 0) && ((pFieldEntity->m58_flags & 0x20) == 0)) && (-1 < mathFlag)) {
                        FP_VEC4 VStack200;
                        VStack200.vx = pScriptEntity->mF4_scale3d[0] * 3 >> 2;
                        VStack200.vy = pScriptEntity->mF4_scale3d[1] * 3 >> 2;
                        VStack200.vz = pScriptEntity->mF4_scale3d[2] * 3 >> 2;
                        if ((pScriptEntity->mE4_playableCharacterId == 7) && (pcInitVar1 != 0)) {
                            VStack200.vx = VStack200.vx * 5 >> 2;
                            VStack200.vy = VStack200.vy * 5 >> 2;
                            VStack200.vz = VStack200.vz * 5 >> 2;
                        }
                        pSpriteSheet->m20->mC_matrix = localRotationMatrix;
                        ScaleMatrix(&pSpriteSheet->m20->mC_matrix, &VStack200);

                        if (pFieldEntity->m4C_scriptEntity->m14_currentTriangleFlag & 0x200000)
                        {
                            MissingCode();
                        }

                        if ((disableFogForCharacters == '\0') && (isFogSetup != 0)) {
                            assert(0);
                        }

                        spriteDepth = spriteDepth >> (gDepthDivider & 0x1f);
                        if (1 < spriteDepth) {
                            spriteDepth = spriteDepth + -2;
                        }

                        if ((ushort)(pScriptEntity->mE8_currentAnimationId + 0x22U) < 2) {
                            assert(0);
                        }
                        else
                        {
                            pSpriteSheet->m3C &= 0xFFFF00FF;
                            if ((pScriptEntity->m4_flags.m_rawFlags & 0x2000000) == 0) {
                                if ((pScriptEntity->m134.m5) == 0) {
                                    MissingCode();
                                    renderSpriteActor(pSpriteSheet, &OT[spriteDepth]);
                                }
                                else
                                {
                                    assert(0);
                                }
                            }
                        }
                    }
                }
                else if(disableMechaRendering == 0) {
                    // Synchronize actor and mecha
                    sLoadedMechas* pMecha = *mechaIt;
                    if ((((pScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10000) == 0) && ((pScriptEntity->m14_currentTriangleFlag & 0x200002U) == 0)) && ((pScriptEntity->m4_flags.m_rawFlags & 0x800) == 0)) {
                        pMecha->m4A &= ~1;
                    }
                    else {
                        pMecha->m4A |= 1;
                    }

                    if ((pFieldEntity->m58_flags & 0x20) == 0) {
                        pMecha->m34 = 1;
                    }
                    else {
                        pMecha->m34 = 0;
                    }

                    if ((pScriptEntity->m4_flags.m_rawFlags & 0x20000) == 0) {
                        (*pMecha->m4_bones)[0].m54_rotationAngles.vy = pScriptEntity->m108_rotation3 + 0xc00;
                    }
                    else {
                        pScriptEntity->m106_currentRotation = pScriptEntity->m108_rotation3 = (*pMecha->m4_bones)[0].m54_rotationAngles.vy - 0xc00;
                    }

                    pMecha->m1C_scale = (short)((int)pScriptEntity->mF4_scale3d[0] * *mechaList3It >> 0xc);
                    pMecha->m60 = pScriptEntity->m20_position.vy.getIntegerPart();
                    (*pMecha->m4_bones)[0].m5C_translation[0] = pScriptEntity->m20_position.vx.getIntegerPart();
                    (*pMecha->m4_bones)[0].m5C_translation[2] = pScriptEntity->m20_position.vz.getIntegerPart();
                    pScriptEntity->m4_flags.m_rawFlags &= ~0x200;

                    mechaIt++;
                    mechaList3It++;
                }
            }
        }
    }
}

void renderCharShadows(OTTable& OT, int oddOrEven)
{
    MATRIX localProjectionMatrix = currentProjectionMatrix;

    if (!disableCharacterShadowsRendering)
    {
        for (int currentActorId = 0; currentActorId < totalActors; currentActorId++)
        {
            sFieldEntity* pCurrentFieldEntity = &actorArray[currentActorId];
            if ((pCurrentFieldEntity->m58_flags & 0x60) == 0x40) {
                sFieldScriptEntity* pScriptEntity = pCurrentFieldEntity->m4C_scriptEntity;
                if (((((pScriptEntity->m4_flags.m_rawFlags & 0x102200) == 0) && ((pScriptEntity->m4_flags.m_rawFlags & 0x800) == 0)) && ((pScriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x10000) == 0)) &&
                    ((pScriptEntity->m14_currentTriangleFlag & 0x200002U) == 0)) {

                    FP_VEC4 local_c8;
                    FP_VEC4 local_d8;
                    FP_VEC4 local_e8;
                    local_e8.vx = 0;
                    local_e8.vy = 0;
                    local_e8.vz = 0x1000;

                    // inlined version of OuterProduct12 without register saving
                    setCopControlWord(2, 0, 0);
                    setCopControlWord(2, 0x1000, 0);
                    setCopControlWord(2, 0x2000, 0x1000);
                    pScriptEntity = pCurrentFieldEntity->m4C_scriptEntity;
                    setCopReg(2, 11, (pScriptEntity->m50_surfaceNormal).vz);
                    setCopReg(2, 9, (pScriptEntity->m50_surfaceNormal).vx);
                    setCopReg(2, 10, (pScriptEntity->m50_surfaceNormal).vy);
                    copFunction(2, 0x178000c);
                    local_c8.vx = getCopReg(2, 0x19);
                    local_c8.vy = getCopReg(2, 0x1a);
                    local_c8.vz = getCopReg(2, 0x1b);
                    VectorNormal(&local_c8, &local_d8);
                    setCopControlWord(2, 0, local_d8.vx);
                    setCopControlWord(2, 0x1000, local_d8.vy);
                    setCopControlWord(2, 0x2000, local_d8.vz);
                    pScriptEntity = pCurrentFieldEntity->m4C_scriptEntity;
                    setCopReg(2, 11, (pScriptEntity->m50_surfaceNormal).vz);
                    setCopReg(2, 9, (pScriptEntity->m50_surfaceNormal).vx);
                    setCopReg(2, 10, (pScriptEntity->m50_surfaceNormal).vy);
                    copFunction(2, 0x178000c);
                    local_c8.vx = getCopReg(2, 0x19);
                    local_c8.vy = getCopReg(2, 0x1a);
                    local_c8.vz = getCopReg(2, 0x1b);
                    VectorNormal(&local_c8, &local_e8);

                    short local_b8[9];
                    local_b8[0] = (short)local_d8.vx;
                    local_b8[1] = (short)local_d8.vy;
                    local_b8[2] = (short)local_d8.vz;
                    int uVar1 = (pCurrentFieldEntity->m4C_scriptEntity->m50_surfaceNormal).vx;
                    local_b8[3] = (short)uVar1;
                    int uVar2 = (pCurrentFieldEntity->m4C_scriptEntity->m50_surfaceNormal).vy;
                    local_b8[4] = (short)uVar2;
                    int uVar4 = (pCurrentFieldEntity->m4C_scriptEntity->m50_surfaceNormal).vz;
                    local_b8[6] = (short)local_e8.vx;
                    local_b8[8] = (short)local_e8.vz;
                    local_b8[7] = (short)local_e8.vy;
                    local_b8[5] = (short)uVar4;

                    SetRotMatrix(&currentProjectionMatrix);

                    setCopReg(2, 0x4800, local_d8.vx & 0xffff);
                    setCopReg(2, 0x5000, uVar1 & 0xffff);
                    setCopReg(2, 0x5800, local_e8.vx & 0xffff);
                    copFunction(2, 0x49e012);
                    uVar1 = getCopReg(2, 0x4800);
                    int iVar8 = getCopReg(2, 0x5000);
                    int uVar9 = getCopReg(2, 0x5800);
                    setCopReg(2, 0x4800, local_d8.vy & 0xffff);
                    setCopReg(2, 0x5000, uVar2 & 0xffff);
                    setCopReg(2, 0x5800, local_e8.vy & 0xffff);
                    copFunction(2, 0x49e012);
                    int iVar7 = getCopReg(2, 0x4800);
                    uVar2 = getCopReg(2, 0x5000);
                    int iVar10 = getCopReg(2, 0x5800);

                    MATRIX local_98;

                    local_98.m[0][0] = uVar1;
                    local_98.m[0][1] = iVar7;
                    local_98.m[2][0] = uVar9;
                    local_98.m[2][1] = iVar10;
                    setCopReg(2, 0x4800, local_d8.vz & 0xffff);
                    setCopReg(2, 0x5000, uVar4 & 0xffff);
                    setCopReg(2, 0x5800, local_e8.vz & 0xffff);
                    copFunction(2, 0x49e012);
                    uVar1 = getCopReg(2, 0x4800);
                    iVar7 = getCopReg(2, 0x5000);
                    uVar4 = getCopReg(2, 0x5800);

                    local_98.m[0][2] = uVar1;
                    local_98.m[1][0] = iVar8;
                    local_98.m[1][1] = uVar2;
                    local_98.m[1][2] = iVar7;
                    local_98.m[2][2] = uVar4;
                    setCopControlWord(2, 0x2800, currentProjectionMatrix.t[0]);
                    setCopControlWord(2, 0x3000, currentProjectionMatrix.t[1]);
                    setCopControlWord(2, 0x3800, currentProjectionMatrix.t[2]);
                    setCopReg(2, 0, (uint)(pCurrentFieldEntity->mC_matrix).t[0] & 0xffff | (uint)(ushort)pCurrentFieldEntity->m4_pVramSpriteSheet->m84_maxY << 0x10);
                    setCopReg(2, 1, (pCurrentFieldEntity->mC_matrix).t[2]);
                    copFunction(2, 0x480012);
                    local_98.t[0] = getCopReg(2, 0x19);
                    local_98.t[1] = getCopReg(2, 0x1a);
                    local_98.t[2] = getCopReg(2, 0x1b);
                    iVar10 = pCurrentFieldEntity->m4C_scriptEntity->mF4_scale3d[0] * 0xc00;
                    FP_VEC4 local_58;
                    local_58.vx = iVar10 >> 0xc;
                    iVar8 = pCurrentFieldEntity->m4C_scriptEntity->mF4_scale3d[1] * 0xc00;
                    local_58.vy = iVar8 >> 0xc;
                    iVar7 = pCurrentFieldEntity->m4C_scriptEntity->mF4_scale3d[2] * 0xc00;
                    local_58.vz = iVar7 >> 0xc;

                    if ((pcInitVar1 != 0) && ((pCurrentFieldEntity->m4C_scriptEntity->m0_fieldScriptFlags.m_rawFlags & 0x400) != 0)) {
                        local_58.vy = iVar8 >> 0xe;
                        local_58.vz = iVar7 >> 0xe;
                        local_58.vx = iVar10 >> 0xe;
                    }

                    ScaleMatrix(&local_98, &local_58);
                    SetRotMatrix(&local_98);
                    SetTransMatrix(&local_98);

                    sFieldEntity2dSprite* psVar6 = pCurrentFieldEntity->m8_2dSprite;

                    long lStack72;
                    long lStack68;
                    int lVar3 = RotAverage4(&psVar6->m0_shadowPolyVertices[0], &psVar6->m0_shadowPolyVertices[1], &psVar6->m0_shadowPolyVertices[2], &psVar6->m0_shadowPolyVertices[3],
                        &psVar6->m20_shadowPoly[oddOrEven].x0y0, &psVar6->m20_shadowPoly[oddOrEven].x1y1, &psVar6->m20_shadowPoly[oddOrEven].x2y2, &psVar6->m20_shadowPoly[oddOrEven].x3y3, &lStack72, &lStack68);

                    sTag* pDestOT = &OT[lVar3 >> (gDepthDivider & 0x1F)];

                    psVar6->m20_shadowPoly[oddOrEven].m0_pNext = pDestOT->m0_pNext;
                    pDestOT->m0_pNext = &psVar6->m20_shadowPoly[oddOrEven];
                }
            }

        }
    }
}

void renderChars()
{
    if (renderCharsDisabled != 1) {
        clearShapeTransfertTableEntry(frameOddOrEven);
        setCharacterRenderingOT(pCurrentFieldRenderingContext->mCC_OT);
        setCurrentRenderingMatrix(&currentProjectionMatrix);
        uploadCharacterSprites();
        execSpritesCallbacksList1();
        execSpritesCallbacksList2();
        renderFieldCharacterSprites(pCurrentFieldRenderingContext->mCC_OT, g_frameOddOrEven);

        for (int i = 0; i < totalActors; i++)
        {
            sFieldEntity* pFieldEntity = &actorArray[i];
            if ((pFieldEntity->m58_flags & 0x60) == 0x40)
            {
                if ((((pFieldEntity->m4C_scriptEntity->m4_flags.m_rawFlags & 0x600) != 0x200) && ((pFieldEntity->m4C_scriptEntity->m4_flags.m_rawFlags & 0x1000) == 0)) && ((pFieldEntity->m4C_scriptEntity->m0_fieldScriptFlags.m_rawFlags & 1) == 0)) {
                    OP_INIT_ENTITY_SCRIPT_sub0Sub9(pFieldEntity->m4_pVramSpriteSheet);
                }
            }
            else {
                if ((pFieldEntity->m4C_scriptEntity->m4_flags.m_rawFlags & 0x1000000) != 0)
                {
                    OP_INIT_ENTITY_SCRIPT_sub0Sub9(pFieldEntity->m4_pVramSpriteSheet);
                }
            }
        }

        renderCharShadows(pCurrentFieldRenderingContext->mCC_OT, g_frameOddOrEven);
        if (fieldDebugDisable == 0) {
            assert(0);
        }
    }
}

void shapeTransfert()
{
    sShapeTransfert* rect;

    rect = (sShapeTransfert*)shapeTransfertTable[shapeTransfertDoubleBufferIndex];
    while (rect != (sShapeTransfert*)0x0) {
        if (!rect->m8_pData.has_value()) {
            ClearImage(&rect->m0_rect, 0, 0, 0);
        }
        else {
            LoadImage(&rect->m0_rect, rect->m8_pData.value());
        }
        rect = rect->mC_pNext;
    }
    shapeTransfertTable[shapeTransfertDoubleBufferIndex] = nullptr;
}

// Add prims from p0 to p1 to ordering table element ot
void AddPrims(sTag* ot, sTag* p0, sTag* p1)
{
    p1->m0_pNext = ot->m0_pNext;
    ot->m0_pNext = p0;
}

// Add OT b (from 0 to index) to ordering table element a
void linkOT(sTag* a, OTTable& b, int index)
{
    AddPrims(a, &b[index], &b[0]);
}

void updateAndRenderField()
{
    MissingCode();
    updateAllEntities();
    updateCamera();
    if (fieldDebugDisable == 0) {
        assert(0);
    }
    MissingCode(); //updateScreenEffectSlots(_DAT_800c426c[0x166].dr_env.code + 3, frameOddOrEven);
    if (fieldDebugDisable == 0) {
        assert(0);
    }
    renderCompass();
    MissingCode();
    renderObjects();
    renderChars();
    updateParticles();
    MissingCode();
    updateAndRenderScreenDistortion();
    MissingCode();
    renderBackgroundPolyIfEnabled();
    renderMechasInField();
    MissingCode();
    DrawSync(0);
    stepDialogWindows();
    addDialogWindowsToOT(&pCurrentFieldRenderingContext->m80D4_uiOT[0], g_frameOddOrEven);
    VSync(0);
    MissingCode();

    u8 clearR = fieldBackgroundClearColor[0];
    u8 clearG = fieldBackgroundClearColor[1];
    u8 clearB = fieldBackgroundClearColor[2];
    if (updateAllEntitiesSub2Var0 != 0) {
        if (fieldTransitionMode == 3) {
            assert(0);
        }
        clearR = clearG = clearB = 0;
    }
    ClearImage(&pCurrentFieldRenderingContext->m5C_backgroundRect.clip, clearR, clearG, clearB);

    PutDispEnv(&pCurrentFieldRenderingContext->mB8_displayEnv);
    PutDrawEnv(&pCurrentFieldRenderingContext->m0_drawEnv);

    MissingCode();

    shapeTransfert();
    MissingCode();

    if (updateAllEntitiesSub2Var0 == 0) {
        if (fieldUseGroundOT != 0) {
            // Add secondaryOT[0 - linkOTIndex] to mCC_OT[linkOTIndex]
            linkOT(&pCurrentFieldRenderingContext->mCC_OT[linkOTIndex], pCurrentFieldRenderingContext->m40D0_secondaryOT, linkOTIndex);
        }
        // Add mCC_OT[0 - linkOTIndex] to mCC_OT[linkOTIndex]
        linkOT(&pCurrentFieldRenderingContext->m80D4_uiOT[7], pCurrentFieldRenderingContext->mCC_OT, linkOTIndex);
    }
    DrawOTag(&pCurrentFieldRenderingContext->m80D4_uiOT[7]);

    MissingCode();
}

int runningOnDTL = -1;
int* pRunningOnDTL = &runningOnDTL;
int startOfUpdateFieldTime = 0;

void logFieldRenderingEvent(const char* param_1)
{
    MissingCode();
}

int loadInputFromVSyncBuffer()
{
    int iVar1;
    uint uVar2;

    if (inputReplayPosition == 0) {
        iVar1 = 0;
    }
    else {
        uVar2 = inputReplayOffset & 0xf;
        newPadButtonForScripts[0].m0_buttons = inputReplayBuffer.m0_newPadButtonForScripts0[uVar2];
        newPadButtonForScripts[1].m0_buttons = inputReplayBuffer.m20_newPadButtonForScripts1[uVar2];
        newPadButtonForDialogs = inputReplayBuffer.m40_newPadButtonForDialogs[uVar2];
        newPadButtonForDialogs2 = inputReplayBuffer.m60_newPadButtonForDialogs2[uVar2];
        newPadButtonForField = inputReplayBuffer.m80_newPadButtonForField[uVar2];
        newPadButtonForField2 = inputReplayBuffer.mA0_newPadButtonForField2[uVar2];
        inputReplayOffset = inputReplayOffset + 1;
        iVar1 = inputReplayPosition;
        inputReplayPosition = inputReplayPosition + -1;
    }
    return iVar1;
}

/*

// Pad terminal type masks. First four bits of padData[1]
#define PADTT_DIG 0x40 // 16 button controller (no analog)
#define PADTT_ANA 0x70 // Analog controller

// All 16 16bit button bit masks. Button state bit is 0 if pressed.
#define PADLeft 0x8000
#define PADDown 0x4000
#define PADRight 0x2000
#define PADUp 0x1000
#define PADStart 0x800
#define PADR3 0x400
#define PADL3 0x200
#define PADSelect 0x100
#define PADSquare 0x80
#define PADCross 0x40
#define PADCircle 0x20
#define PADTriangle 0x10
#define PADRB1 0x8
#define PADLB1 0x4
#define PADRB2 0x2
#define PADLB2 0x1

*/

bool isGameInFocus();

void getInputDuringVsync(void)
{
    MissingCode();

    if (!isGameInFocus())
        return;

    static SDL_GameController* controller = nullptr;
    if (controller == nullptr)
    {
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    break;
                }
                else {
                    fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
                }
            }
        }
    }

    u16 buttonMask = 0;
    if (controller)
    {
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ? controllerButtons::L1 : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ? controllerButtons::R1 : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) ? controllerButtons::TRIANGLE : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) ? controllerButtons::INTERACT : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) ? controllerButtons::CROSS : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) ? controllerButtons::JUMP : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK) ? controllerButtons::SELECT : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) ? controllerButtons::START : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? controllerButtons::UP : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? controllerButtons::RIGHT : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? controllerButtons::DOWN : 0;
        buttonMask |= SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? controllerButtons::LEFT : 0;


        /*
                static bool pressRight = false;
                if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                {
                    pressRight = true;
                }
                if (pressRight)
                {
                    buttonMask |= 0x2000;
                }
                */
    }
    //else
    {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        for (int i = 0; i < SDL_NUM_SCANCODES; i++)
        {
            if (keyState[i])
            {
                switch (i)
                {
                case SDL_SCANCODE_Z:
                    buttonMask |= controllerButtons::CROSS; // CROSS
                    break;
                case SDL_SCANCODE_X:
                    buttonMask |= controllerButtons::INTERACT; // CIRCLE
                    break;
                case SDL_SCANCODE_A:
                    buttonMask |= controllerButtons::JUMP; // SQUARE
                    break;
                case SDL_SCANCODE_S:
                    buttonMask |= controllerButtons::TRIANGLE; // TRIANGLE
                    break;
                case SDL_SCANCODE_UP:
                    buttonMask |= controllerButtons::UP;
                    break;
                case SDL_SCANCODE_RIGHT:
                    buttonMask |= controllerButtons::RIGHT;
                    break;
                case SDL_SCANCODE_DOWN:
                    buttonMask |= controllerButtons::DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    buttonMask |= controllerButtons::LEFT;
                    break;
                case SDL_SCANCODE_RETURN:
                    buttonMask |= controllerButtons::START;
                    break;
                default:
                    break;
                }
            }
        }
    }

    static u32 previousDownButtons = 0;
    static u32 numVsyncButtonHeld = 0;

    newPadButtonForScripts[0].m0_buttons = buttonMask;
    newPadButtonForDialogs = (newPadButtonForScripts[0].m0_buttons ^ previousDownButtons) & newPadButtonForScripts[0].m0_buttons;

    if (newPadButtonForDialogs != 0) {
        numVsyncButtonHeld = 0;
    }
    newPadButtonForField = newPadButtonForScripts[0].m0_buttons;

    if (numVsyncButtonHeld < 0x20)
    {
        previousDownButtons++;
        newPadButtonForField = newPadButtonForDialogs;
    }
    else if(playTimeInVsync & 3)
    {
        newPadButtonForField = newPadButtonForDialogs;
    }

    MissingCode();

    previousDownButtons = newPadButtonForScripts[0].m0_buttons;

}

int getInputOverflowed() {
    return inputOverflowed;
}

void saveInputs()
{
    ushort uVar1;
    ushort uVar2;
    ushort uVar3;
    ushort uVar4;
    short sVar5;
    uint uVar6;

    sVar5 = newPadButtonForScripts[1].m0_buttons;
    uVar4 = newPadButtonForField2;
    uVar3 = newPadButtonForField;
    uVar2 = newPadButtonForDialogs2;
    uVar1 = newPadButtonForDialogs;
    if (inputReplayPosition < 0x10) {
        uVar6 = inputReplayPosition2 & 0xf;
        inputReplayPosition2 = inputReplayPosition2 + 1;
        inputReplayPosition = inputReplayPosition + 1;
        inputReplayBuffer.m0_newPadButtonForScripts0[uVar6] = newPadButtonForScripts[0].m0_buttons;
        inputReplayBuffer.m20_newPadButtonForScripts1[uVar6] = sVar5;
        inputReplayBuffer.m40_newPadButtonForDialogs[uVar6] = uVar1;
        inputReplayBuffer.m60_newPadButtonForDialogs2[uVar6] = uVar2;
        inputReplayBuffer.m80_newPadButtonForField[uVar6] = uVar3;
        inputReplayBuffer.mA0_newPadButtonForField2[uVar6] = uVar4;
    }
    else {
        inputOverflowed = 1;
    }
}

s32 timeOverflow = 0;
s32 timeVSyncCount = 0;
extern u32 timeSeconds;
extern u32 timeMinutes;
extern u32 timeHours;
void incrementTime()
{
    if (timeOverflow == 0) {
        timeVSyncCount += 1;
        if (timeVSyncCount == 0x3c) {
            timeVSyncCount = 0;
            timeSeconds += 1;
        }
        if (timeSeconds == 0x3c) {
            timeSeconds = 0;
            timeMinutes += 1;
        }
        if (timeMinutes == 0x3c) {
            timeMinutes = 0;
            timeHours += 1;
        }
        if (timeHours == 100) {
            timeOverflow = 1;
        }
    }
}

void vsyncCallback(void)
{
    playTimeInVsync++;
    getInputDuringVsync();
    saveInputs();
    incrementTime();
    MissingCode();
}

void updateFieldInputs()
{
    padButtonForScripts[0].m0_buttons = 0;
    padButtonForScripts[1].m0_buttons = 0;
    padButtonForDialogs = 0;
    padButtonForDialogs2 = 0;
    padButtonForField = 0;
    padButtonForField2 = 0;
    while (loadInputFromVSyncBuffer()) {
        padButtonForScripts[0].m0_buttons |= newPadButtonForScripts[0].m0_buttons & inputAllowedMask2;
        padButtonForScripts[1].m0_buttons |= newPadButtonForScripts[1].m0_buttons;
        padButtonForDialogs |= newPadButtonForDialogs & inputAllowedMask2;
        padButtonForDialogs2 |= newPadButtonForDialogs2;
        padButtonForField |= newPadButtonForField & inputAllowedMask2;
        padButtonForField2 |= newPadButtonForField2;
    }
    padButtonForScripts[0].m0_buttons &= inputAllowedMask;
    padButtonForField &= inputAllowedMask;
    padButtonForDialogs &= inputAllowedMask;
    resetInputs();
    //FUN_Field__8007ae78(1, &DAT_80065848);
    MissingCode();
    if (updateAllEntitiesSub2Var0 != 0) {
        padButtonForScripts[0].m0_buttons = 0;
        padButtonForScripts[1].m0_buttons = 0;
        padButtonForDialogs = 0;
        padButtonForDialogs2 = 0;
        padButtonForField = 0;
        padButtonForField2 = 0;
    }
    if (playMusicAuthorized == 0) {
        padButtonForDialogs &= ~controllerButtons::JUMP;
    }
}

void saveStateToKernel(void)
{
    pKernelGameState->m231A_fieldID = fieldMapNumber;

    MissingCode();

    for (int i = 0; i < 0x200; i++) {
        pKernelGameState->m1930_fieldVarsBackup[i] = fieldVars[i];
    }

    return;
}

void syncKernelAndFieldStates()
{
    if (kernelAndFieldStatesSynced != 1) {
        MissingCode();
        for (int i = 0; i < 3; i++) {
            pKernelGameState->m1D34_currentParty[i] = currentParty[i];
        }
        MissingCode();
        saveStateToKernel();
        MissingCode();

        setVar(0xc, (timeMinutes << 8) | timeSeconds);
        setVar(0xe, timeHours);
        setVar(0x1e, actorArray[playerControlledActor].m4C_scriptEntity->m20_position.vx.getIntegerPart());
        setVar(0x20, actorArray[playerControlledActor].m4C_scriptEntity->m20_position.vz.getIntegerPart());
        setVar(0x22, actorArray[playerControlledActor].m4C_scriptEntity->m20_position.vy.getIntegerPart());
    }
}

void fieldPerFrameReset()
{
    startOfUpdateFieldTime = VSync(1);
    clearFieldOrderingTable();
    updateFieldInputs();
    if (fieldDebugDisable == 0) {
        logFieldRenderingEvent("Clear OTAG");
    }
    syncKernelAndFieldStates();
}

int updateMusicState2(int param_1)
{
    MissingCode();
    return 0;
}

void updateMusicState()
{
    xenoRand();
    if (fieldMusicLoadPending == -1) {
        fieldMusicLoadPending = updateMusicState2(currentlyPlayingMusic);
    }
    if (updateAllEntitiesSub2Var0 != 0) {
        updateAllEntitiesSub2Var0 = updateAllEntitiesSub2Var0 + -1;
    }
}

void fieldChangeGameMode(int mode) {
    MissingCode();

    switch (mode) {
    case 0: // battle
        saveStateToKernel();
        MissingCode();
        /*
        currentlyPlayingMusic = currentlyPlayingMusic2;
        pKernelGameState->previouslyPlayingMusic = (short)currentlyPlayingMusic2;*/
        pKernelGameState->m2320_worldmapMode = pKernelGameState->m1930_fieldVarsBackup[1];
        MissingCode();
        /*if (mechaOverlayLoadingMode != 0) {
            return;
        }*/
        setGameMode(2);
        bootGame(0);
        break;
    case 1: // worldmap
        Noah_MissingCode("Clear music when going to worldmap");
        Noah_MissingCode("MechaOverlayLoading check");
        setGameMode(3);
        bootGame(0);
        break;
    case 3:
        MissingCode();
        fieldScriptEntityAlreadyInitialized = 0;
        MissingCode();
        if ((newBootMode & 0x80) != 0) {
            initGameState();
        }
        setGameMode(newBootMode & 0x7f);
        bootGame(0);
        break;
    default:
        assert(0);
    }
}


bool isControllerConnected(int port) {
    return true;
}

void decompressPauseSignToVram(short param_1, short param_2) {
    MissingCode();
}

void pauseMusic() {
    MissingCode();
}

void resumeMusic() {
    MissingCode();
}

void checkSoftReboot() {
    if (newPadButtonForScripts[0].m0_buttons == 0x90c) {
        assert(0);
    }
}

void renderFullScreenTransitionEffect() {
    MissingCode();
}

bool isFieldTransitionPermittedByLoading() {
    MissingCode();
    return false;
}

void flushFrame() {
    DrawSync(0);
    VSync(0);
}

void startFieldTransition(void) {
    MissingCode();

    if (NumMechas != 0) {
        freeMechaModule();
        ClearCacheAfterOverlayLoad();
        flushFrame();
        mechaOverlayBuffer.clear();
    }

    MissingCode();
}

int isFieldBattlePrevented(void)
{
    int iVar1;

    iVar1 = 0;
    if ((bBattleSequenceInitialized == 1) && (updateEntityEventCode4Var0 == 0)) {
        iVar1 = -(s32)(((actorArray[playerControlledActor].m4C_scriptEntity)->m0_fieldScriptFlags.mx800_isJumping) != 0);
    }
    return iVar1;
}

void fieldEntryPoint()
{
    fieldDebugDisable = (runningOnDTL == -1);
    ClearCacheAfterOverlayLoad();
    if (fieldDebugDisable == 0) {
        assert(0);
        //DrawSyncCallback(fieldDebuggerDrawSyncCallback);
    }

    MissingCode();

    setCurrentDirectory(4, 0); // TODO: this is not explicitly called at this level

    allocatePartyCharacterBuffers();

    MissingCode();

    fieldExecuteVar3 = -1;
    fieldExecuteVar2 = -1;
    playMusicAuthorized = -1;
    bootModeReady = -1;

    MissingCode();

    fieldMapNumber = gameState.m231A_fieldID;
    pKernelGameState = &gameState;
    pKernelGameState = &gameState;
    gameState.m1930_fieldVarsBackup[4] = (ushort)gameState.m231C_CameraYaw >> 9;
    gameState.m1930_fieldVarsBackup[1] = gameState.m2320_worldmapMode;
    currentlyPlayingMusic = 0xff;

    MissingCode();

    if (fieldDebugDisable == 1) {
        gameState.m1930_fieldVarsBackup[41] = 1;
        setVar(0x50, 1);
        MissingCode();
    }
    initCompassData();
    fieldRequestedGears = 0;
    startLoadingPlayableCharacters();
    finalizeLoadPlayableCharacters();

    MissingCode();

    bootField();

    MissingCode();

    bool bVar2 = false;

    do
    {
        MissingCode();
        ////
        u32 playTimeBeginningOfLoop = playTimeInVsync;
        if (!isControllerConnected(0)) {
            pauseMusic();
            decompressPauseSignToVram(0x88, (g_frameOddOrEven + 1U & 1) << 8 | 100);
            do {
                DrawSync(0);
                VSync(2);
                updateFieldInputs();
                checkSoftReboot();
            } while (!isControllerConnected(0));
            resumeMusic();
        }

        /* pause when player press start */
        if ((((padButtonForField & 0x800) != 0) && ((padButtonForScripts[0].m0_buttons & 0x40U) == 0)) && (pauseDisabled == '\0')) {
            playTimeInVsync = playTimeBeginningOfLoop;
            pauseMusic();
            decompressPauseSignToVram(0x88, (g_frameOddOrEven + 1U & 1) << 8 | 100);
            do {
                DrawSync(0);
                VSync(2);
                updateFieldInputs();
                checkSoftReboot();
            } while ((padButtonForField & 0x800) == 0);
            resumeMusic();
        }

        fieldPerFrameReset();
        updateAndRenderField();
        renderFullScreenTransitionEffect();

        if ((g_frameOddOrEven == 1) && (playMusicAuthorized == 0) && (isFieldTransitionPermittedByLoading() == 0) && (isFieldBattlePrevented() == 0)) {
            MissingCode();
            syncKernelAndFieldStates();
            MissingCode();
            releaseAllDialogWindows();
            DrawSync(0);
            VSync(0);
            freeFieldData();
            MissingCode();
            typeOfPlayableCharacterLoaded = 0;
            MissingCode();
            fieldChangeGameMode(0);//enter battle
            return;
        }

        MissingCode();
        ////
        if ((fieldChangePrevented == 0) && (fieldMusicLoadPending == 0))
        {
            if ((asyncLoadingVar1 == 0xff) && (load2dAnimVar == 0))
            {
                setCurrentDirectory(4, 0); //TODO: shouldn't be necessary!
                if (loadNewField((fieldMapNumber & 0xfff) * 2, 0) == 0)
                {
                    if (isCDBusy() == 0)
                    {
                        //if (screenEffects[0].m3C_duration == 0)
                        {
                            //fieldTransitionCompleted = 0;
                            //saveStateToKernel();
                            waitReadCompletion(0);
                            transitionFields();
                            //resetInputs();
                            //fieldTransitionCompleted = 1;
                        }
                    }
                }
            }
        }
        if (g_frameOddOrEven == 1) {
            if ((fieldExecuteVar3 == 0) && !isFieldTransitionPermittedByLoading()) {
                waitReadCompletion(0);
                if (currentFieldId1 != -1) {
                    unflagAllocation(rawFieldBundle);
                    rawFieldBundle.clear();
                }
                MissingCode();
                syncKernelAndFieldStates();
                MissingCode();
                releaseAllDialogWindows();
                DrawSync(0);
                VSync(0);
                freeFieldData();
                MissingCode();
                typeOfPlayableCharacterLoaded = 0;
                MissingCode();
                fieldChangeGameMode(1);
                return;
            }
            if (g_frameOddOrEven == 1) {
                MissingCode();
                if (((g_frameOddOrEven == 1) && (bootModeReady == 0))) {
                    MissingCode();
                    waitReadCompletion(0);
                    if (currentFieldId1 != -1) {
                        unflagAllocation(rawFieldBundle);
                        rawFieldBundle.clear();
                    }
                    MissingCode();
                    syncKernelAndFieldStates();
                    MissingCode();
                    fieldChangeGameMode(3);
                    return;
                }
            }
        }
        ////

        // TODO: this is within a larger test
        {
            if (((menuIdToOpen != 0xff) && (g_frameOddOrEven == 0)) && (((actorArray[playerControlledActor].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags & 0x1800) == 0)) {
                //releaseAllDialogWindows();
                MissingCode();
                loadAndOpenMenu();
                menuIdToOpen = 0xff;
            }

            if ((((padButtonForField & 0x10) != 0) && (menuDisabled == '\0')) && ((menuIdToOpen == 0xff && (playerCanRun == 1)))) {
                menuIdToOpen = 0x80;
                //menuOpenArg = (byte)DAT_Field__800b236c;
            }
        }

        bool iVar3;
        if ((((bootModeReady == -1) && (playMusicAuthorized == -1)) && (fieldExecuteVar3 == -1)) && ((iVar3 = isFieldTransitionPermittedByLoading(), iVar3 == 0 && (fieldChangePrevented == -1)))) {
            if ((padButtonForScripts[0].m0_buttons & 3U) == 0) {
                bVar2 = false;
            }
            /*
            if ((((padButtonForScripts[0].m0_buttons & 1U) != 0) && (playerCanRun == 1)) && (((padButtonForScripts[0].m0_buttons & 2U) != 0 && (!bVar2)))) {
                bVar2 = true;
                FUN_Field__800798bc();
                if (((menuIdToOpen == 0xff) && (((actorArray[playerControlledActor].m4C_scriptEntity)->m0_fieldScriptFlags & 0x1800) == 0)) && (DAT_80059179 == '\0')) {
                    FUN_Field__800ace90();
                }
            }
            if ((((padButtonForField & 0x100) != 0) && (fieldChangePrevented == -1)) && (playerCanRun == 1)) {
                openFieldMenu();
            }*/
            MissingCode();
            if ((iRam800adb70 != 0) && (g_frameOddOrEven == 1)) {
                startFieldTransition();
                iRam800adb70 = 0;
            }
            if (((menuIdToOpen != 0xff) && (g_frameOddOrEven == 0)) && (((actorArray[playerControlledActor].m4C_scriptEntity)->m0_fieldScriptFlags.m_rawFlags & 0x1800) == 0)) {
                releaseAllDialogWindows();
                loadAndOpenMenu();
                menuIdToOpen = 0xff;
            }
            if ((((padButtonForField & 0x10) != 0) && (menuDisabled == '\0')) && ((menuIdToOpen == 0xff && (playerCanRun == 1)))) {
                menuIdToOpen = 0x80;
                //menuOpenArg = (byte)DAT_Field__800b236c;
            }
        }
        updateMusicState();
    }while (1);
}

void copyActorPositions(int dest, int source) {
    sFieldScriptEntity* pSourceScriptEntity = actorArray[source].m4C_scriptEntity;
    sFieldScriptEntity* pDestScriptEntity = actorArray[dest].m4C_scriptEntity;

    for (int i = 0; i < 4; i++) {
        pDestScriptEntity->m8_currentWalkMeshTriangle[i] = pSourceScriptEntity->m8_currentWalkMeshTriangle[i];
    }

    pDestScriptEntity->m10_walkmeshId = pSourceScriptEntity->m10_walkmeshId;
    pDestScriptEntity->m50_surfaceNormal = pSourceScriptEntity->m50_surfaceNormal;
    pDestScriptEntity->m20_position = pSourceScriptEntity->m20_position;
    pDestScriptEntity->mEC_elevation = pSourceScriptEntity->mEC_elevation;
    pDestScriptEntity->m72_elevation = pSourceScriptEntity->m72_elevation;
    pDestScriptEntity->m14_currentTriangleFlag = pSourceScriptEntity->m14_currentTriangleFlag;

    actorArray[dest].m4_pVramSpriteSheet->m0_spriteActorCore.m84_maxY = actorArray[source].m4_pVramSpriteSheet->m0_spriteActorCore.m84_maxY;
    actorArray[dest].m4_pVramSpriteSheet->m0_spriteActorCore.m0_position = actorArray[source].m4_pVramSpriteSheet->m0_spriteActorCore.m0_position;
    actorArray[dest].mC_matrix.t = actorArray[source].mC_matrix.t;
}

void doPCCollisionCheckAfterLoading() {
    EntityMoveCheck1(playerControlledActor, ((actorArray[playerControlledActor].m4C_scriptEntity)->m20_position).vy.getIntegerPart(), &actorArray[playerControlledActor],
        actorArray[playerControlledActor].m4C_scriptEntity, 0); // TODO: what is the last argument supposed to be?

    for (int i = 0; i < totalActors; i++) {
        sFieldEntity* pFieldEntity = &actorArray[i];
        sFieldScriptEntity* pFieldScriptEntity = pFieldEntity->m4C_scriptEntity;
        if ((pFieldEntity->m58_flags & 0xF80) == 0x200) {
            int partySlot = findCharacterInParty(pFieldScriptEntity->mE4_playableCharacterId);
            if ((partySlot != -1) && (partySlot != 0)) {
                EntityMoveCheck1(i, actorArray[i].m4C_scriptEntity->m20_position.vy.getIntegerPart(), pFieldEntity, pFieldScriptEntity, 0);// TODO: what is the last argument supposed to be?

                actorArray[i].m4_pVramSpriteSheet->m0_position = actorArray[i].m4_pVramSpriteSheet->m0_spriteActorCore.m0_position;
                pFieldEntity->mC_matrix.t = actorArray[playerControlledActor].mC_matrix.t;
            }
        }
    }

    partyToFollowStructMapping.fill(0);

    for (int i = 0; i < 0x20; i++) {
        initFollowStructForPlayer(playerControlledActor);
    }
}

void runInitScriptForNewlyLoadedPC(uint param_1) {
    sFieldEntity* backupCurrentFieldEntity = pCurrentFieldEntity;
    sFieldScriptEntity* backupCurrentFieldScriptActor = pCurrentFieldScriptActor;

    pKernelGameState->m1D30_partyMemberBitField |= 1 << (asyncPartyCharacterLoadingCharacterIndex & 0x1f);

    int backupBreakCurrentScript = breakCurrentScript;
    int backupCurrentFieldActorId = currentFieldActorId;
    int backupFieldExecuteMaxCycles = fieldExectuteMaxCycles;

    if (fieldExecuteVar1) {
        u16 backScriptPC = backupCurrentFieldScriptActor->mCC_scriptPC;
        for (int i = 0; i < totalActors; i++) {
            u16 scriptEntryPoint = getScriptEntryPoint(i, 0);
            // 0x16 is OP_INIT_ENTITY_PC
            if ((pCurrentFieldScriptFile[scriptEntryPoint] == 0x16) && ((pCurrentFieldScriptFile + scriptEntryPoint)[1] == param_1)) {
                pCurrentFieldEntity = &actorArray[i];
                pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
                initFieldScriptEntityValues(i);
                currentFieldActorId = i;
                actorArray[i].m4C_scriptEntity->mCC_scriptPC = scriptEntryPoint;
                currentScriptFinished = 0;
                pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 0);
                copyActorPositions(i, playerControlledActor);
                executeFieldScript(0xffff);
                doPCCollisionCheckAfterLoading();
                
                pKernelGameState->m1D30_partyMemberBitField |= 1 << (asyncPartyCharacterLoadingCharacterIndex & 0x1f);

                if (fieldMapNumber & 0xC000) {
                    assert(0);
                }
            }
        }

        fieldExectuteMaxCycles = backupFieldExecuteMaxCycles;
        currentFieldActorId = backupCurrentFieldActorId;
        pCurrentFieldScriptActor = backupCurrentFieldScriptActor;
        breakCurrentScript = backupBreakCurrentScript;
        pCurrentFieldEntity = backupCurrentFieldEntity;
        backupCurrentFieldScriptActor->mCC_scriptPC = backScriptPC;
    }
}
