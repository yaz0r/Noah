#include "noahLib.h"
#include "battle.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battleRenderContext.h"
#include "battleDebugSelector.h"
#include "kernel/memory.h"
#include "battleStartEffect.h"
#include "battleConfig.h"
#include "field/mecha/mechaOverlay.h"
#include "field/field.h"
#include "kernel/gameMode.h"
#include "field/fieldGraphicObject.h"
#include "battleSpriteLoader.h"
#include "kernel/trigo.h"
#include "kernel/gte.h"
#include "battleCursor.generated.h"

u16 allPlayerCharacterBitmask = 0;
u8 battleInitVar0 = 0;
u8 battleInitVar1 = 0;
s32 battleRunningVar0 = 0;
s8 battleRunningVar1 = 0;
s8 isBattleAnEvent = 0;
s8 currentBattleMode = 0;
s8 makeBattleTimeProgress = 0;
s8 battlePartyLayoutType = 0;
s8 drawBattleMode1Disabled = 0;
s8 newBattleInputButton2;
s8 previousNewBattleInputButton;
u8 startCharacterJumpToEnemyVar0;
u8 startCharacterJumpToEnemyVar1;
u8 numValidTarget = 0;
std::array<u8, 12> targetsPerPriority;

std::array<sBattleSpriteActor*, 11> battleSpriteActors;
std::array<sSpriteActorCore*, 11> battleSpriteActorCores;

std::vector<u8> battleFont;

std::array<s16, 0xB> battleSlotStatusVar0;
std::array<s16, 0xB> battleSlotStatusVar1;
std::array<s16, 0xB> numTicksBeforeReady;

std::array<std::array<s16, 2>, 3> battleCharactersCopyGameState;
s16 bBattleTickMode0 = 0;
u8 bBattleTickMode1 = 0;


s8 battleDebugDisplay = 0;
s8 requestedBattleConfig = 0;
bool battleIsPaused = false;
u8 battleInputButton = 0;
s8 battleCharacters[3];
std::array<s8, 11> isBattleSlotFilled;

sMechaDataTable1* battleLoadDataVar0;
sLoadableDataRaw* battleLoadDataVar0_raw;
std::vector<u8>::iterator battleLoadDataVar1;
sBattleMechaInitData* battleLoadDataVar2;
sBattleMechaInitData* battleLoadDataVar2Bis;
sBattleMechaInitData* battleLoadDataVar2Ter;

std::array<s8, 0xB> isEntityReadyForBattle;
std::array<s8, 0xB> randomTurnOrder;
s8 currentEntryInRandomTurnOrder;

std::array<sBattleVisualBuffer, 0x11> battleVisualBuffers;
std::array<sBattleVisualEntity, 0x11> battleVisualEntities;
std::array<sBattleEntity, 11> battleEntities;

sGameStateA4* startBattleAttackAnimationVar5;
sGameStateA42* startBattleAttackAnimationVar6;

bool battleSpritesDisabled = false;

void jumpUpdatePositionSub0(sSpriteActorCore* param_1, sSpriteActorCore* param_2);

void initGraphicsForBattle(void) {
    ResetGraph(1);
    Noah_MissingCode("Some dummy function calls");
    InitGeom();
    SetGeomOffset(0xa0, 0xb4);
    SetGeomScreen(0x200);
    SetDefDispEnv(&battleRenderStructs[0].m5C_dispEnv, 0, 0xe0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
    SetDefDispEnv(&battleRenderStructs[1].m5C_dispEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[1].m0_drawEnv, 0, 0xe0, 0x140, 0xe0);
    initBattleRenderStructs(&battleRenderStructs[0]);
    initBattleRenderStructs(&battleRenderStructs[1]);
}

sBattleVar0* battleVar0 = nullptr;
sBattleVar1* battleVar1 = nullptr;
sBattleVar2* battleVar2 = nullptr;

s8 battleTransitionEffect = 0;
s8 currentBattleLoaderTransitionEffect;
s8 setupTurnRenderLoop_menuVar;

u8 startJumpAnimationVar1;
u16 jumpAnimationActiveActorBF = 0;
s16 startJumpAnimationVar2;
void* loadWdsDataNeeded = nullptr;
u8 startJumpAnimationVar3 = 0;
u32 startReadingBattleJumpAnimationVar0 = 0;

void loadWdsDataIfNeeded(void)
{
    if (loadWdsDataNeeded != 0) {
        assert(0);
    }
    loadWdsDataNeeded = 0;
    return;
}



void initBattleDefDrawContext() {
    SetGeomOffset(0xa0, 0xa4);
    SetDefDispEnv(&battleRenderStructs[0].m5C_dispEnv, 0, 0xe0, 0x140, 0xe0);
    SetDefDispEnv(&battleRenderStructs[1].m5C_dispEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[0].m0_drawEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&battleRenderStructs[1].m0_drawEnv, 0, 0xe0, 0x140, 0xe0);
    battleRenderStructs[0].m5C_dispEnv.screen.x = 0;
    battleRenderStructs[0].m5C_dispEnv.screen.y = 10;
    battleRenderStructs[0].m5C_dispEnv.screen.w = 0x100;
    battleRenderStructs[0].m5C_dispEnv.screen.h = 0xd8;
    battleRenderStructs[1].m5C_dispEnv.screen.x = 0;
    battleRenderStructs[1].m5C_dispEnv.screen.y = 10;
    battleRenderStructs[1].m5C_dispEnv.screen.w = 0x100;
    battleRenderStructs[1].m5C_dispEnv.screen.h = 0xd8;
}

sLoadingBatchCommands battleLoadingCommands[4];

sLoadableDataRaw battleStartSeq;
sLoadableDataRaw battleCharacterConfigFile;
sLoadableDataRaw battleLoaderBinary;

s32 battleMechaVar0;
s16 battleMechaVar1;
s8 battleMechaVar2;
s8 battleMechaVar3;
s16 battleMechaVar4;
s8 battleMechaVar5;

sMechaInitVar2 battleMechaInitVar2;
sMechaInitVar3 battleMechaInitVar3;
sBattleMechaInitData* battleMechaInitData = nullptr;

void mechaInitForBattle() {
    battleMechaVar0 = 0;
    battleMechaVar1 = 0;
    battleMechaVar2 = 0;
    battleMechaVar3 = 0;
    battleMechaVar4 = 0;
    battleMechaVar5 = 1;
    initMechaInitVar2(&battleMechaInitVar2, battleMechaInitData->m348);
    initMechaInitVar3(&battleMechaInitVar3, battleMechaInitData->m34A);

    MissingCode();
}

void loadBattleLoader() {
    resetMemoryAllocStats(2, 0);
    setCurrentDirectory(0xc, 0);

    Noah_MissingCode("Code to get the amount of free memory before battle loader");

    battleStartSeq.resize(getFileSizeAligned(2));
    battleCharacterConfigFile.resize(getFileSizeAligned(3));

    battleLoadingCommands[0].m0_fileIndex = 2;
    battleLoadingCommands[0].m4_loadPtr = &battleStartSeq;
    battleLoadingCommands[1].m0_fileIndex = 3;
    battleLoadingCommands[1].m4_loadPtr = &battleCharacterConfigFile;
    battleLoadingCommands[2].m0_fileIndex = 4;
    battleLoadingCommands[2].m4_loadPtr = &battleLoaderBinary;
    battleLoadingCommands[3].m0_fileIndex = 0;
    battleLoadingCommands[3].m4_loadPtr = nullptr;
    batchStartLoadingFiles(battleLoadingCommands, 0);

    while (isCDBusy() == 3) {};

    Noah_MissingCode("Start battle transition seq");
}

MATRIX battleMatrix800CCB94;
MATRIX battleMatrix800CCBB4;

std::vector<sMechaBone>* environmentModelConfigs;
sMechaInitVar4* environmentModelBlocks = nullptr;
u8 battleAnimationSoundLoaded;

std::array<struct sBackgroundPoly*, 2> battleEnvBackgroundPolys;

std::array<sLoadedMechas*, 32> battleMechas;

void uploadTextureToVram(sPS1Pointer param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY);

std::array<sMechaInitVar4, 0x14> battleMechaInitVar4;
void sMechaModel_init(sModel& modelBlock, sMechaInitVar4* param_2);
std::vector<sMechaBone>* processMechaMesh(sMechaInitVar4* param_1, std::vector<sMechaDataTable1_C>& param_2, u32 param_3, int useTpageAndClut, short tpageX, short tpageY, short clutX, short clutY);
void initMechSub110(sLoadedMechas* param_1);

void* sendWds(std::vector<u8>::iterator buffer, u32 param_2) {
    return nullptr;
}

int waitForMusic(uint param_1) {
    return 0;
}

void mechaInitEnvironmentMechaMesh(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9)
{
    resetMemoryAllocStats(4, 0);
    if (0x1F < entryId) {
        return;
    }
    if (battleMechas[entryId] != nullptr) {
        return;
    }

    sLoadedMechas* pLoadedMecha = new sLoadedMechas;
    if ((flags & 1) == 0) {
        /*
        doPointerRelocation((uint*)pData1);
        doPointerRelocation((uint*)pData1->m10);
        */
    }
    pLoadedMecha->m62_isSeqLoaded = 0;
    pLoadedMecha->m63 = 0;
    if ((flags & 4) == 0) {
        /*
                doPointerRelocation((uint*)pData2);
                doPointerRelocation(*(uint**)(pData2 + 8));
                puVar12 = *(uint**)(pData2 + 4);
                doPointerRelocation(puVar12);
                doPointerRelocation((uint*)puVar12[1]);
                */

#if 0
        MissingCode();
        psVar9 = pData2->m8;
        if ((psVar9->mC != psVar9->m8_seq) && (iVar3 = isSeqValid(psVar9->m8_seq, 0), iVar3 == 0)) {
            startSeq(psVar9->m8_seq);
            pLoadedMecha->m62 = 1;
        }
#endif
    }

    battleMechas[entryId] = pLoadedMecha;
    pLoadedMecha->m24[0] = pData1->m10->m4->m2[0];
    pLoadedMecha->m24[1] = pData1->m10->m4->m2[1];
    pLoadedMecha->m24[2] = pData1->m10->m4->m2[2];

    pLoadedMecha->m2A[0] = pData1->m10->m4->mA;
    pLoadedMecha->m4A = pData1->m10->m4->mC;

    if (pLoadedMecha->m4A & 0x200) {
        MissingCode();
        //patchSelfModifyingCode(2, 2, 0x40, 0x40);
    }

    sModel battleMechaModelBlocksBufferForLoading;

    int battleMechaInitVar4Counter = 0;
    if ((flags & 1) == 0) {
        int uVar29 = 0;
        if ((flags & 0x40) == 0) {
            uVar29 = ((pLoadedMecha->m4A & 4) == 0);
        }

        // TODO: convert that properly
        sPS1Pointer temp;
        temp.setPointer(&pData1->m4_textures.m_raw[0]);
        uploadTextureToVram(temp, uVar29, tpageX, tpageY, uVar29, clutX, clutY);
        battleMechaModelBlocksBufferForLoading = pData1->m8_modelBlocks;

        int iVar2;
        do {
            iVar2 = battleMechaInitVar4Counter + 1;
            if (battleMechaInitVar4[battleMechaInitVar4Counter].m0.size() == 0)
                break;
            battleMechaInitVar4Counter = iVar2;
        } while (iVar2 < 8);

        sMechaModel_init(battleMechaModelBlocksBufferForLoading, &battleMechaInitVar4[battleMechaInitVar4Counter]);
    }

    pLoadedMecha->m0 = &battleMechaInitVar4[battleMechaInitVar4Counter];


    if ((flags & 0x40) == 0) {
        if ((pLoadedMecha->m4A & 4) == 0) {
            pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 1, tpageX, tpageY, clutX, clutY);

        }
        else
        {
            pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 0, 0, 0, 0, 0);
        }
    }
    else
    {
        pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 0, 0, 0, 0, 0, 0);
    }

    if (param_9 != nullptr) {
        (*pLoadedMecha->m4_bones)[0].m5C_translation[0] = param_9->vx;
        (*pLoadedMecha->m4_bones)[0].m5C_translation[1] = param_9->vy;
        (*pLoadedMecha->m4_bones)[0].m5C_translation[2] = param_9->vz;
    }

    if ((flags & 1) == 0) {
        pLoadedMecha->m90[0] = tpageX;
        pLoadedMecha->m90[1] = tpageY;
        pLoadedMecha->m90[2] = clutX;
        pLoadedMecha->m96 = clutY;
    }
    else {
        pLoadedMecha->m90[0] = -1;
    }

    if (((flags & 4) == 0) && ((flags & 0x80) == 0)) {
        pLoadedMecha->mAC = pData2;
    }
    else {
        pLoadedMecha->mAC = nullptr;
    }

    for (int i = 0; i < 2; i++)
    {
        POLY_FT4* p = &pLoadedMecha->mB8_Polys[i];
        SetPolyFT4(p);
        SetSemiTrans(p, 1);
        p->r0 = 0x40;
        p->g0 = 0x40;
        p->b0 = 0x40;
        p->clut = GetClut(0x100, 0xF3);
        p->tpage = GetTPage(0, 2, 0x280, 0x100);
        p->u0 = 0x00;
        p->v0 = 0xE0;
        p->u1 = 0x0F;
        p->v1 = 0xE0;
        p->u2 = 0x00;
        p->v2 = 0xEF;
        p->u1 = 0x0F;
        p->v1 = 0xEF;
    }

    pLoadedMecha->m1C_scale = pData1->m10->m4->m8;
    pLoadedMecha->m10C = pData1->m10->m4->mE;
    initMechSub110(pLoadedMecha);
    pLoadedMecha->m10E = pData1->m10->m4->m10;

    if (pLoadedMecha->m10E != 0) {
        pLoadedMecha->m118.resize(pLoadedMecha->m10E);
        for (int i = 0; i < pLoadedMecha->m10E; i++) {
            pLoadedMecha->m118[i].m1A = 0;
            pLoadedMecha->m118[i].m4 = 0;
            pLoadedMecha->m118[i].m8 = 0;
            pLoadedMecha->m118[i].mC = 0;
        }
    }

    pLoadedMecha->m10D = pData1->m10->m4->m12;
    if (pLoadedMecha->m10D != 0) {
        assert(0);
    }

    pLoadedMecha->m22 = 0;
    pLoadedMecha->m34 = 1;
    pLoadedMecha->m20_mechaEntryId = entryId;

    if ((flags & 0x40) == 0) {
     //   pLoadedMecha->mB0 = &pData2->m8;
     //   initMechaTransforms1(pLoadedMecha, &mechaInitVar2, &pData2->m4.m8, &pData2->m4.m4);
     //   initMechaAnimation(pLoadedMecha, pLoadedMecha, &mechaInitVar2, 0);
        assert(0);
    }

    if ((flags & 2) == 0) {
        pLoadedMecha->mA8 = new sModel;
        *pLoadedMecha->mA8 = battleMechaModelBlocksBufferForLoading;
        // Fixup
        for (int i = 0; i < pLoadedMecha->mA8->m0_numBlocks; i++) {
            pLoadedMecha->mA8->m10_blocks[i].m_baseItForRelocation = &pLoadedMecha->mA8->mRawData[0];
        }

        sMechaModel_init(*pLoadedMecha->mA8, pLoadedMecha->m0);
        MissingCode();
    }
    else
    {
        pLoadedMecha->mA8 = nullptr;
    }
}

int loadBattleEnvironment(std::vector<u8>::iterator param_1, std::vector<u8>::iterator param_2, sMechaDataTable1* param_3, MATRIX* param_4, MATRIX* param_5, sColor* param_6) {
    if (param_3) {
        Noah_MissingCode("Check that param_1 is not a singular iterator");
        if (READ_LE_U32(param_1)) {
            resetMemoryAllocStats(4, 0);

            environmentModelConfigs = nullptr;
            MissingCode();
            for (int i = 0; i < 2; i++) {
                battleEnvBackgroundPolys[i] = nullptr;
            }

            Noah_MissingCode("battle move image commands");
            Noah_MissingCode("800d361a");

            if (param_3) {
                Hack("Original code alias the animation bundle and asset bundle");
                mechaInitEnvironmentMechaMesh(0x1f, 0xc4, nullptr, param_3, 0, 0, 0, 0, 0);
                Noah_MissingCode("processBattleEnvTextures");
                environmentModelConfigs = battleMechas[0x1F]->m4_bones;
                environmentModelBlocks = battleMechas[0x1F]->m0;
                Noah_MissingCode("Environment bones init");
            }
        }
    }

    return 0;
}

void startBattleLoader(int param_1)
{
    currentBattleLoaderTransitionEffect = (byte)param_1;
    initBattleDefDrawContext();
    loadBattleLoader();

    switch (param_1) {
    case 0:
        battleStartEffect();
        break;
    default:
        Hack("Unimplemented battle start effect, using default");
        battleStartEffect();
        break;
    }

    waitReadCompletion(0);
    mechaInitForBattle();

    Hack("Deserialize mecha data for battle env");
    battleLoadDataVar0 = new sMechaDataTable1;
    battleLoadDataVar0->init(battleLoadDataVar0_raw->mData.begin());

    battleRenderStructs[0].m0_drawEnv.isbg = loadBattleEnvironment(battleLoadDataVar2->mData.begin(), battleLoadDataVar1, battleLoadDataVar0, &battleMatrix800CCB94, &battleMatrix800CCBB4, &battleRenderStructs[0].m0_drawEnv.color);

    MissingCode();
}

s32 battleRenderCount = 0;
s32 startOfBattleTime;
MATRIX battleRenderingMatrix;

SVECTOR battleCameraEye;
SVECTOR battleCameraEyeTarget;
SVECTOR battleCameraAt;
SVECTOR battleCameraAtTarget;
SVECTOR battleCameraUp = { 0x0, 0x1000, 0x0 };
SVECTOR battleSceneRenderingMatrixOffset = { 0,0,0 };
s8 renderBattleSceneDisabled = 0;

void renderBattleScenePolys(sMechaInitVar4* envBlocks, std::vector<sMechaBone>* envHierarchy, MATRIX* matrix, MATRIX* lightMatrix, s32 param_5, std::vector<sTag>* inOT, int param_7, int param_8)
{
    s32 backupDepthDivider = gDepthDivider;

    for (int i = 1; i < (*envHierarchy)[0].mA_numBones; i++) {
        sMechaBone* pBone = &(*envHierarchy)[i];

        if ((pBone->m8_geometryId != -1) && pBone->m7_isEnabled) {
            MATRIX m1F800040;
            CompMatrix(matrix, &pBone->m2C_boneFinalMatrix, &m1F800040);

            // billboard
            if ((pBone->m52 - 1) < 2) {
                m1F800040.m[0][0] = 0x1000;
                m1F800040.m[0][2] = 0;
                m1F800040.m[1][0] = 0;
                m1F800040.m[1][2] = 0;
                m1F800040.m[2][0] = 0;
                m1F800040.m[2][2] = 0x1000;
                if (pBone->m52 == 1) {
                    m1F800040.m[0][1] = matrix->m[0][1];
                    m1F800040.m[1][1] = matrix->m[1][1];
                    m1F800040.m[2][1] = matrix->m[2][1];
                }
                else {
                    m1F800040.m[0][1] = 0;
                    m1F800040.m[1][1] = 0x1000;
                    m1F800040.m[2][1] = 0;
                }
            }
            SetRotMatrix(&m1F800040);
            SetTransMatrix(&m1F800040);

            s32 renderMode;

            switch (pBone->m52) {
            case 4:
                renderMode = 2;
                break;
            case 5:
                renderMode = 3;
                break;
            case 6:
                renderMode = 4;
                break;
            case 7:
                renderMode = 5;
                break;
            default:
                renderMode = 0;
                break;
            }

            std::vector<sTag*>* psVar4;
            std::vector<sTag>::iterator OT;
            sModelBlock* psVar3;

            if (pBone->m52 == 0) {
                psVar4 = &pBone->m68[param_7];
                psVar3 = envBlocks->m0[pBone->m8_geometryId];
                gDepthDivider = 0x10;
                OT = inOT->begin() + (param_8 - 1);
            }
            else {
                psVar4 = &pBone->m68[param_7];
                psVar3 = envBlocks->m0[pBone->m8_geometryId];
                gDepthDivider = backupDepthDivider;
                OT = inOT->begin();
            }

            submitModelForRendering(psVar3, *psVar4, OT, renderMode);
        }
    }

    gDepthDivider = backupDepthDivider;
}

void battleDrawEnv(MATRIX* param_1, MATRIX* lightMatrix, s32 param_3, std::vector<sTag>* OT, s32 oddOrEven, SVECTOR* param_6, SVECTOR* param_7, int param_8)
{
    if (lightMatrix != nullptr) {
        SetLightMatrix(lightMatrix);
    }
    MissingCode();
    if (environmentModelConfigs) {
        // hack param 5 is not 0, but it's probably garbage (uninitialized) and not used anyway
        renderBattleScenePolys(environmentModelBlocks, environmentModelConfigs, param_1, lightMatrix, 0, OT, oddOrEven, param_8);
    }
    MissingCode();
}

void renderBattleScene() {
    lookAtNoDivide(&battleRenderingMatrix, &battleCameraEye, &battleCameraAt, &battleCameraUp);
    battleRenderingMatrix.t[0] = battleRenderingMatrix.t[0] + battleSceneRenderingMatrixOffset.vx;
    battleRenderingMatrix.t[1] = battleRenderingMatrix.t[1] + battleSceneRenderingMatrixOffset.vy;
    battleRenderingMatrix.t[2] = battleRenderingMatrix.t[2] + battleSceneRenderingMatrixOffset.vz;
    if (renderBattleSceneDisabled == '\0') {
        battleDrawEnv(&battleRenderingMatrix, 0, 0, pCurrentBattleOT, battleOddOrEven, &battleCameraEye, &battleCameraAt, 0x1000);
    }
}

s16 gMechaAngleBattle;
void submitMechaForRendering(sLoadedMechas* pMecha, MATRIX* pMatrix, MATRIX* param_3, int param_4, int param_5, OTTable& OT, int oddOrEven);
void updateMechAnim(sLoadedMechas* param_1, sMechaInitVar2* param_2, int iterations, int oddOrEven);

void renderMechasBattle(MATRIX* pMatrix, MATRIX* param_2, OTTable& OT, int oddOrEven)
{
    MissingCode();
    //battleMechaVar0 += 1 + DAT_Battle__800ccc5c;
    if (battleMechaVar0 > 6) {
        battleMechaVar0 = 6;
    }
    int mechaIterationCount = 0;
    int mechaIterationCount2 = 0;
    if (battleMechaVar0 > 1) {
        do {
            battleMechaVar0 = battleMechaVar0 + -2;
            mechaIterationCount = mechaIterationCount + 1;
        } while (1 < battleMechaVar0);
        mechaIterationCount2 = mechaIterationCount * 8;
    }
    mechaIterationCount2 = (uint)battleMechaVar1 + (mechaIterationCount2 - mechaIterationCount) * 8;
    battleMechaVar1 = (ushort)mechaIterationCount2;
    battleMechaVar4 = battleMechaVar4 + 0x80; // TODO: this doesn't exists in field version?
    int angleForMecha = getAngleSin(mechaIterationCount2 * 0x10000 >> 0x10);
    gMechaAngleBattle = ((short)(((angleForMecha + 0x1000) * 0x51eb851f) >> 0x28) - (short)(angleForMecha + 0x1000 >> 0x1f)) + 4;

    MissingCode();

    for (int i = 0; i < 32; i++) {
        if (battleMechas[i]) {
            battleMechas[i]->m11C_previousTranslation = (*battleMechas[i]->m4_bones)[0].m5C_translation;
            Hack("Using field codepath for battle mecha!");
            updateMechAnim(battleMechas[i], &battleMechaInitVar2, mechaIterationCount, oddOrEven);
        }
    }

    MissingCode();

    for (int i = 0; i < 32; i++) {
        if (battleMechas[i] && battleMechas[i]->m5C != -1) {
            //updateMechasBaseMatrixIsAttached(battleMechas[i]);
        }
    }

    MissingCode();

    //SetColorMatrix(mechaFinalizeVar0);

    // draw everything but the last (the environment that is drawn separately)
    for (int i = 0; i < 31; i++) {
        if (battleMechas[i]) {
            battleMechas[i]->m128_deltaTranslation[0] = battleMechas[i]->m11C_previousTranslation[0] - (*battleMechas[i]->m4_bones)[0].m5C_translation[0];
            battleMechas[i]->m128_deltaTranslation[1] = battleMechas[i]->m11C_previousTranslation[1] - (*battleMechas[i]->m4_bones)[0].m5C_translation[1];
            battleMechas[i]->m128_deltaTranslation[2] = battleMechas[i]->m11C_previousTranslation[2] - (*battleMechas[i]->m4_bones)[0].m5C_translation[2];
            Hack("Using field codepath for battle mecha!");
            submitMechaForRendering(battleMechas[i], pMatrix, param_2, 1, 1, OT, oddOrEven);
        }
    }
    MissingCode();
}

s32 battleCameraVar2;
u32 allEntitiesToFitInView = 0xFFFFFFFF;
SVECTOR battleCameraParamsVar0;

// Compute camera to fit all characters from bitmask in view
void computeBattleCameraParams(uint bitmask) {
    VECTOR positionSum = { 0,0,0,0 };

    s32 bestDistance = 0;
    u32 numEntries = 0;
    {
        u32 currentBitmask = bitmask;
        for (int i = 0; i < 11; i++) {
            if ((currentBitmask & 1) && (battleVisualEntities[i].m3 == 0) && (battleSpriteActorCores[i])) {
                positionSum.vx += battleSpriteActorCores[i]->m0_position.vx / 2;
                positionSum.vy += battleSpriteActorCores[i]->m0_position.vy / 2;
                positionSum.vz += battleSpriteActorCores[i]->m0_position.vz / 2;
                numEntries++;
            }
            currentBitmask >>= 1;
        }
    }
    allEntitiesToFitInView = bitmask;
    if (numEntries) {
        int minX = (positionSum.vx / numEntries) * 2;
        int minY = (positionSum.vy / numEntries) * 2;
        int minZ = (positionSum.vz / numEntries) * 2;
        int maxX = minX;
        int maxY = minY;
        int maxZ = minZ;

        u32 currentBitmask = bitmask;
        for (int i = 0; i < 11; i++) {
            if ((currentBitmask & 1) && (battleVisualEntities[i].m3 == 0) && (battleSpriteActorCores[i])) {
                s32 X = (battleSpriteActorCores[i]->m0_position).vx;
                if (maxX < X) {
                    maxX = X;
                }
                if (X < minX) {
                    minX = X;
                }
                s32 Z = (battleSpriteActorCores[i]->m0_position).vz;
                if (maxZ < Z) {
                    maxZ = Z;
                }
                if (Z < minZ) {
                    minZ = Z;
                }
                s32 Y = (battleSpriteActorCores[i]->m0_position).vy;
                if (maxY < Y) {
                    maxY = Y;
                }
                if (Y < minY) {
                    minY = Y;
                }
            }
            currentBitmask >>= 1;
        }

        positionSum.vx = (minX + maxX) - (minX + maxX >> 0x1f) >> 0x11;
        positionSum.vy = (maxY + minY) - (maxY + minY >> 0x1f) >> 0x11;
        positionSum.vz = (minZ + maxZ) - (minZ + maxZ >> 0x1f) >> 0x11;

        MATRIX MStack_f0;
        RotMatrixZYX(&battleCameraParamsVar0, &MStack_f0);
        SVECTOR local_b0;
        local_b0.vx = 0;
        local_b0.vy = 0;
        local_b0.vz = (ReadGeomScreen() << 3);

        VECTOR local_d0;
        ApplyMatrix(&MStack_f0, &local_b0, &local_d0);

        SVECTOR local_f8;
        SVECTOR local_100;
        local_f8.vx = (short)positionSum.vx;
        local_100.vx = (short)positionSum.vx - (short)local_d0.vx;
        local_f8.vy = (short)positionSum.vy;
        local_f8.vz = (short)positionSum.vz;
        local_100.vy = (short)positionSum.vy + (short)local_d0.vy;
        local_100.vz = (short)positionSum.vz - (short)local_d0.vz;
        lookAtNoDivide(&MStack_f0, &local_100, &local_f8, &battleCameraUp);
        SetRotMatrix(&MStack_f0);
        SetTransMatrix(&MStack_f0);

        for (int i = 0; i < 11; i++) {
            if ((bitmask & 1) && (battleVisualEntities[i].m3 == 0) && (battleSpriteActorCores[i])) {
                SVECTOR local_c0;
                local_c0.vx = (battleSpriteActorCores[i]->m0_position).vx.getIntegerPart();
                local_c0.vy = (battleSpriteActorCores[i]->m0_position).vy.getIntegerPart();
                local_c0.vz = (battleSpriteActorCores[i]->m0_position).vz.getIntegerPart();

                sVec2_s16 transformed;
                long dummy1;
                long dummy2;
                RotTransPers(&local_c0, &transformed, &dummy1, &dummy2);

                s32 X = ((transformed.vx - 0xa0) * 0x40000) >> 0x10;
                s32 Y = ((transformed.vy - 0xa4) * 0x40000) >> 0x10;

                transformed.vx = (transformed.vx - 0xa0) * 4;
                transformed.vy = (transformed.vy - 0xa4) * 4;

                s32 distance = X * X + Y * Y;
                if (bestDistance < distance) {
                    bestDistance = distance;
                }
                if (battleVisualEntities[i].m4_isGear) {
                    assert(0);
                }
            }

            bitmask >>= 1;
        }

        s32 realDistance = SquareRoot0(bestDistance);
        VECTOR local_c0;
        s32 finalZ;
        if (realDistance < 0x78) {
            MATRIX MStack_a0;
            RotMatrixZYX(&battleCameraParamsVar0, &MStack_a0);
            SVECTOR local_70;
            local_70.vx = 0;
            local_70.vy = 0;
            realDistance = ReadGeomScreen();
            local_70.vz = (short)(realDistance << 1);
            realDistance = ReadGeomScreen();
            battleCameraVar2 = realDistance << 1;
            ApplyMatrix(&MStack_a0, &local_70, (VECTOR*)&local_c0);
        }
        else {
            s32 lVar1 = ReadGeomScreen();
            bestDistance = ((realDistance * 0x4000) / 0x78) * 2 * lVar1 >> 0xe;
            battleCameraVar2 = bestDistance;
            MATRIX MStack_68;
            RotMatrixZYX(&battleCameraParamsVar0, &MStack_68);
            SVECTOR local_38;
            local_38.vx = 0;
            local_38.vy = 0;
            local_38.vz = (short)bestDistance;
            VECTOR local_80;
            ApplyMatrix(&MStack_68, &local_38, (VECTOR*)&local_80);
            local_c0.vy = local_80.vy;
            local_c0.vz = local_80.vz;
            local_c0.vx = local_80.vx;
        }
 
        battleCameraEyeTarget.vx = (short)positionSum.vx - local_c0.vx;
        battleCameraEyeTarget.vy = (short)positionSum.vy + local_c0.vy;
        battleCameraEyeTarget.vz = (short)positionSum.vz - local_c0.vz;
        battleCameraAtTarget.vx = (short)positionSum.vx;
        battleCameraAtTarget.vy = (short)positionSum.vy;
        battleCameraAtTarget.vz = (short)positionSum.vz;
    }
}

s32 battleCameraMode = 0;
s32 battleCameraModeInterpolation;
s32 battleIR0 = 0x200;
s32 battleCameraSpriteDistance;
SVECTOR battleCameraSpriteOrientation;

void updateBattleCamera() {
    switch (battleCameraMode) {
    case 1:
        computeBattleCameraParams(allEntitiesToFitInView);
        break;
    default:
        assert(0);
    }

    if (battleCameraModeInterpolation == 1) {
        gte_ldIR0(battleIR0);

        {
            VECTOR difference;
            difference.vx = battleCameraEyeTarget.vx - battleCameraEye.vx;
            difference.vy = battleCameraEyeTarget.vy - battleCameraEye.vy;
            difference.vz = battleCameraEyeTarget.vz - battleCameraEye.vz;
            gte_ldlvl(&difference);
            gte_gpf12();
            gte_stlvl(&difference);

            if ((difference.vx | difference.vy | difference.vz) == 0) {
                battleCameraEye = battleCameraEyeTarget;
            }
            else {
                battleCameraEye.vx += difference.vx;
                battleCameraEye.vy += difference.vy;
                battleCameraEye.vz += difference.vz;
            }
        }

        {
            VECTOR difference;
            difference.vx = battleCameraAtTarget.vx - battleCameraAt.vx;
            difference.vy = battleCameraAtTarget.vy - battleCameraAt.vy;
            difference.vz = battleCameraAtTarget.vz - battleCameraAt.vz;
            gte_ldlvl(&difference);
            gte_gpf12();
            gte_stlvl(&difference);

            if ((difference.vx | difference.vy | difference.vz) == 0) {
                battleCameraAt = battleCameraAtTarget;
            }
            else {
                battleCameraAt.vx += difference.vx;
                battleCameraAt.vy += difference.vy;
                battleCameraAt.vz += difference.vz;
            }
        }
    }

    // update battleCameraSpriteDistance/battleCameraSpriteOrientation
    VECTOR temp40;
    temp40.vx = battleCameraAt.vx - battleCameraEye.vx;
    temp40.vy = battleCameraAt.vy - battleCameraEye.vy;
    temp40.vz = battleCameraAt.vz - battleCameraEye.vz;

    VECTOR squareVector;
    Square0(&temp40, &squareVector);
    battleCameraSpriteDistance = SquareRoot0(squareVector.vx + squareVector.vy + squareVector.vz);
    battleCameraSpriteOrientation.vy = -ratan2(temp40.vz,  temp40.vx);
    battleCameraSpriteOrientation.vx = -ratan2(temp40.vy, SquareRoot0(squareVector.vx + squareVector.vz));
    battleCameraSpriteOrientation.vz = 0;
}

void execSpritesCallback(void);
void execSpritesCallbacks2(void);

void battleTimeProgress() {
    if (makeBattleTimeProgress) {
        for (int i = 0; i < 11; i++) {
            if (isBattleSlotFilled[i] && !isEntityReadyForBattle[i]) {
                s8 iVar5 = 1;
                if ((battleEntities[i].m0_base.m84 | battleEntities[i].m0_base.m86) & 0x8000) {
                    iVar5 = 2;
                }
                if (((battleEntities[i].m0_base.m7C & 0x1000) == 0) || ((battleSlotStatusVar1[i] ^= 1) == 0)) {
                    if ((battleEntities[i].m0_base.m7C & 0x2000) == 0) {
                        if (((battleEntities[i].m0_base.m7C & 0x80) == 0) && ((battleEntities[i].m0_base.m80 & 0x1000) == 0)) {
                            numTicksBeforeReady[i] -= iVar5;
                            if ((numTicksBeforeReady[i] * 0x10000) < 1) {
                                isEntityReadyForBattle[i] = 1;
                                numTicksBeforeReady[i] = 0;
                            }
                        }
                    }
                    else {
                        assert(0);
                    }
                }
            }
        }
    }
}

void battleUpdateInputs_mode1(int mode) {
    if (battleTimeEnabled != '\0') {
        battleTimeProgress();
    }

    if (mode == 0) {
        battleHandleInput();
    }

    battleVar1->mA9_timeBarNumMoveSteps = battleVar1->mA9_timeBarNumMoveSteps + '\x06';
    battleVar1->mAB_timeBarNumAnimationSteps = battleVar1->mAB_timeBarNumAnimationSteps + 1;

    if (battleVar0->m6415 != 0) {
        if (battleVar0->m6416 == 0) {
            battleVar0->m6410 = battleVar0->m6410 + 4;
            if (0x80 < battleVar0->m6410) {
                battleVar0->m6410 = 0x7c;
                battleVar0->m6416 = 1;
            }
        }
        else {
            battleVar0->m6410 = battleVar0->m6410 + -4;
            if (battleVar0->m6410 < 0) {
                battleVar0->m6410 = 4;
                battleVar0->m6416 = 0;
            }
        }
    }

    MissingCode();
}

void battleUpdateInputs_mode0(int mode) {
    bool bVar1 = true;
    bool wasPaused = false;
    do 
    {
        if (!isControllerConnected(0)) {
            assert(0);
        }
        else {
            bVar1 = false;
            if (wasPaused) {
                assert(0);
            }
        }

    } while (bVar1);

    if (battleVar1->mCF == 0) {
        battleInputButton = 0xff;
    }
    do 
    {
        if (!getInputOverflowed()) {
            do {
                if (!loadInputFromVSyncBuffer()) {
                    goto LAB_Battle__8008a804;
                }
                if (newPadButtonForDialogs & 0x20) {
                    battleInputButton = 4;
                    goto LAB_Battle__8008a804;
                }
            } while ((newPadButtonForDialogs & 0x800) == 0);
            if (battleIsPaused == '\0') {
                pauseMusic();
                decompressPauseSignToVram(0x88, 100);
                decompressPauseSignToVram(0x88, 0x144);
                battleIsPaused = '\x01';
                //unaff_s1 = playTimeInVsync;
                assert(0);
            }
            else {
                resumeMusic();
                battleIsPaused = '\0';
                //playTimeInVsync = unaff_s1;
                assert(0);
            }
        }
        else {
            resetInputs();
        }
LAB_Battle__8008a804:
        if (battleIsPaused == 0) {
            MissingCode();

            return;
        }

    } while (1);
}

void battleUpdateInputs(int mode) {
    switch (currentBattleMode) {
    case 0:
        battleUpdateInputs_mode0(mode);
        break;
    case 1:
        battleUpdateInputs_mode1(mode);
        break;
    case 2: // cutscene
        MissingCode();
        break;
    default:
        assert(0);
    }
}

void drawBattleMode2() {
    MissingCode();
}

void battleRenderPortraitSelection() {
    if (battleVar0->m6415) {
        battleVar0->m63C8[battleVar0->m6414].r0 = battleVar0->m6410;
        battleVar0->m63C8[battleVar0->m6414].g0 = battleVar0->m6410;
        battleVar0->m63C8[battleVar0->m6414].b0 = battleVar0->m6410;
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m63C8[battleVar0->m6414]);
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m63F8[battleVar0->m6414]);
    }
}

void battleRenderPolyArray(std::array<POLY_FT4, 2>* p, int param2, int param3) {
    for (int i = 0; i < param2; i++) {
        AddPrim(&(*pCurrentBattleOT)[1], &p[i][param3]);
    }
}

void battleRenderPlayerPortraits() {
    if (battleVar1->m97) {
        assert(0);
    }

    for (int i = 0; i < 4; i++) {
        battleRenderPolyArray(&battleVar0->m2E08_timeBars[i][0], battleVar1->m74_timeBarsLengths[i], battleVar1->m84_timeBarsOddOrEven[i]);
    }

    for (int i = 0; i < 3; i++) {
        if (battleVar1->mCC[i]) {
            switch (battleVar0->m835C[i].m1E1) {
            case 0: // nothing
                break;
            case 1: // AP
                battleRenderPolyArray(&battleVar0->m835C[i].m0[0], battleVar0->m835C[i].m1E2, battleVar0->m835C[i].m1E0);
                break;
            case 2: // Fuel
                battleRenderPolyArray(&battleVar0->m835C[i].mA0[0], battleVar0->m835C[i].m1E3, battleVar0->m835C[i].m1E0);
                break;
            default:
                assert(0);
            }
        }
    }

    if (battleVar1->mAF) {
        MissingCode();
    }

    for (int j = 0; j < 3; j++) {
        battleRenderPolyArray(&battleVar0->m3A88[j][0], battleVar1->mE0[j], battleVar1->m93_oddOrEven[j]);
    }
    for (int j = 0; j < 3; j++) {
        if (battleVisualEntities[j].m2 != 0x7F) {
            battleRenderPolyArray(&battleVar0->m818[j], 1, battleVar1->m83); // render the portrait
            if (battleVar1->mCC[j]) {
                battleRenderPolyArray(&battleVar0->m0[j][0], battleVar1->m78[j], battleVar1->mA2); // display the AP bar
                AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m740_APOrFuelPoly[j][battleOddOrEven]);
            }
        }
    }
}

MATRIX identityMatrix = {
    {{
        {0x1000, 0, 0},
        {0, 0x1000, 0},
        {0, 0, 0x1000}
    }},
    {0,0,0}
};

int battleSetupStringInPolyFTRot(std::vector<u8>& param_1, int param_2, std::array<POLY_FT4,2>* param_3, int param_4, short posX, short posY, short scaleX, short scaleY, ushort rotZ)
{
    MATRIX localMatrix = identityMatrix;
    VECTOR scale = { scaleX, scaleY, 0x1000 };

    PushMatrix();
    ScaleMatrixL(&localMatrix, &scale);
    RotMatrixZ(rotZ, &localMatrix);

    // Save
    s32 geoX;
    s32 geoY;  
    ReadGeomOffset(&geoX, &geoY);
    s32 geoScreen = ReadGeomScreen();

    SetGeomOffset(posX, posY);
    SetGeomScreen(0x1000);
    SetRotMatrix(&localMatrix);
    SetTransMatrix(&localMatrix);

    std::vector<u8>::iterator fontData = param_1.begin() + READ_LE_U16(param_1.begin() + 4 + param_2 * 2);
    int count = READ_LE_U16(fontData);
    for (int i = 0; i < count; i++) {
        POLY_FT4* p = &param_3[i][param_4];
        SetPolyFT4(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 1);

        std::vector<u8>::iterator characterData = fontData + 4 + 0x1C * i;
        p->tpage = GetTPage(READ_LE_U16(characterData + 0x10), 0, READ_LE_U16(characterData + 0x16), READ_LE_U16(characterData + 0x18));
        p->clut = GetClut(READ_LE_U16(characterData + 0x12), READ_LE_U16(characterData + 0x14));

        std::array<SVECTOR, 4> tempVectorArray;
        tempVectorArray.fill({ 0,0,0x1000,0x0 });

        tempVectorArray[3].vy = READ_LE_S16(characterData + 10);
        tempVectorArray[3].vx = READ_LE_S16(characterData + 8) + READ_LE_S16(characterData + 4);
        tempVectorArray[2].vx = READ_LE_S16(characterData + 8);
        if (READ_LE_U8(characterData + 0x1A) == 0) {
            tempVectorArray[2].vx = tempVectorArray[3].vx;
            tempVectorArray[3].vx = READ_LE_S16(characterData + 8);
        }
        tempVectorArray[1].vy = tempVectorArray[3].vy + READ_LE_S16(characterData + 6);
        if (READ_LE_U8(characterData + 0x1B) == 0) {
            tempVectorArray[1].vy = tempVectorArray[3].vy;
            tempVectorArray[3].vy = tempVectorArray[3].vy + READ_LE_S16(characterData + 6);
        }
        tempVectorArray[0].vx = tempVectorArray[3].vx;
        tempVectorArray[0].vy = tempVectorArray[1].vy;
        tempVectorArray[1].vx = tempVectorArray[2].vx;
        tempVectorArray[2].vy = tempVectorArray[3].vy;

        long lStack_40;
        long lStack_3c;
        RotTransPers4(&tempVectorArray[0], &tempVectorArray[1], &tempVectorArray[2], &tempVectorArray[3],
            &p->x0y0, &p->x1y1, &p->x3y3, &p->x2y2,
            &lStack_40, &lStack_3c);

        s16 uVar6 = READ_LE_S16(characterData + 0);
        s16 uVar7 = READ_LE_S16(characterData + 2);
        s16 cVar8 = READ_LE_S16(characterData + 4);
        s16 cVar9 = READ_LE_S16(characterData + 6);
        if ((rotZ & 0xfff) == 0xc00) {
            uVar6 = uVar6 - 1;
        }
        if ((rotZ & 0xfff) == 0) {
            int uVar5 = uVar6 - 1;
            if ((p->x3 < p->x0) && (uVar6 = uVar5, (int)((uint)uVar5 << 0x10) < 0)) {
                uVar6 = 0;
                cVar8 = cVar8 + -1;
            }
            if ((p->y3 < p->y0) && (uVar7 = uVar7 - 1, (int)((uint)(ushort)(uVar7 - 1) << 0x10) < 0)) {
                uVar7 = 0;
                cVar9 = cVar9 + -1;
            }
        }
        u8 uVar2 = (u8)uVar6;
        u8 uVar3 = (u8)uVar7;
        p->u0 = uVar2;
        p->v0 = uVar3;
        p->u1 = uVar2 + cVar8;
        p->v1 = uVar3;
        p->u2 = uVar2;
        p->v2 = uVar3 + cVar9;
        p->u3 = uVar2 + cVar8;
        p->v3 = uVar3 + cVar9;
    }

    SetGeomOffset(geoX, geoY);
    SetGeomScreen(geoScreen);
    PopMatrix();
    return count;
}

void timerBarUpdateStartOfTurnAnimation(int param_1) {
    short barX = 0; // was un-intialized
    short barY = 0; // was un-intialized

    assert(battleVar1->mA9_timeBarNumMoveSteps);
    for (int i = 0; i < battleVar1->mA9_timeBarNumMoveSteps; i++) {
        battleVar1->m64_timeBarAnimationX -= battleVar1->m54_timeBarAnimationXStep;
        battleVar1->m6C_timeBarAnimationY += battleVar1->m5C_timeBarAnimationYStep;
        barX = (battleVar1->m64_timeBarAnimationX >> 8) + (s16)battleVar1->m34_timeBarScreenPositionX;
        barY = (battleVar1->m6C_timeBarAnimationY >> 8) + (s16)battleVar1->m44_timeBarScreenPositionY;
    }

    battleVar1->mA9_timeBarNumMoveSteps = 0;
    battleVar1->m74_timeBarsLengths[3] = 0;
    battleVar1->m74_timeBarsLengths[3] += battleSetupStringInPolyFTRot(battleFont, 0x52, &battleVar0->m2E08_timeBars[3][battleVar1->m74_timeBarsLengths[3]], battleOddOrEven, barX, barY, battleVar1->m104_timeBarAnimationScale, battleVar1->m104_timeBarAnimationScale, battleVar1->m106_timeBarAnimationRotation);
    int startOfShadowSprite = battleVar1->m74_timeBarsLengths[3];
    battleVar1->m74_timeBarsLengths[3] += battleSetupStringInPolyFTRot(battleFont, 0x53, &battleVar0->m2E08_timeBars[3][battleVar1->m74_timeBarsLengths[3]], battleOddOrEven, barX, barY, battleVar1->m104_timeBarAnimationScale, battleVar1->m104_timeBarAnimationScale, battleVar1->m106_timeBarAnimationRotation);

    for (int i = startOfShadowSprite; i < battleVar1->m74_timeBarsLengths[3]; i++) {
        SetSemiTrans(&battleVar0->m2E08_timeBars[3][i][battleOddOrEven], 1);
    }

    battleVar1->m84_timeBarsOddOrEven[3] = battleOddOrEven;

    for (int i = 0; i < battleVar1->mAB_timeBarNumAnimationSteps; i++) {
        battleVar1->m104_timeBarAnimationScale += 0x66;
        battleVar1->m106_timeBarAnimationRotation += 0x80;
    }

    battleVar1->mAB_timeBarNumAnimationSteps = 0;
    if (barX <= battleVar1->m3C_timeBarAnimationTargetPositionX) {
        battleVar1->m90_perPCTimeBarStatus[param_1] = 1;
    }
    if (battleVar1->m4C_timeBarAnimationTargetPositionY <= barY) {
        battleVar1->m90_perPCTimeBarStatus[param_1] = 1;
    }
}

void timerBarInitStartOfTurnAnimation(int param_1) {
    battleVar1->m44_timeBarScreenPositionY = 0x1c;
    battleVar1->m34_timeBarScreenPositionX = param_1 * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][param_1] + 0x48;
    if ((apConfigArray[param_1].m1 != 0) && (battleCharacters[param_1] != 7)) {
        battleVar1->m44_timeBarScreenPositionY = 0x24;
        battleVar1->m34_timeBarScreenPositionX = param_1 * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][param_1] + 0x44;
    }
    battleVar1->m3C_timeBarAnimationTargetPositionX = 0x10;
    battleVar1->m4C_timeBarAnimationTargetPositionY = 0x98;
    battleVar1->m54_timeBarAnimationXStep = (battleVar1->m34_timeBarScreenPositionX + -5) - battleVar1->m3C_timeBarAnimationTargetPositionX;
    battleVar1->m104_timeBarAnimationScale = 0x800;
    battleVar1->mA9_timeBarNumMoveSteps = 6;
    battleVar1->m5C_timeBarAnimationYStep = battleVar1->m4C_timeBarAnimationTargetPositionY - (battleVar1->m44_timeBarScreenPositionY + 5);
    battleVar1->m5C_timeBarAnimationYStep = 0x100;
    battleVar1->m64_timeBarAnimationX = 0;
    battleVar1->m6C_timeBarAnimationY = 0;
    battleVar1->m106_timeBarAnimationRotation = 0;
    battleVar1->m54_timeBarAnimationXStep = (battleVar1->m54_timeBarAnimationXStep << 8) / battleVar1->m5C_timeBarAnimationYStep;
    battleVar1->mAB_timeBarNumAnimationSteps = 1;
    battleVar1->m90_perPCTimeBarStatus[param_1]--; // Go into animation mode
}

void updatePortraits() {
    battleVar1->m74_timeBarsLengths[3] = 0;

    for (int i = 0; i < 3; i++) {
        if (battleVisualEntities[i].m2 != 0x7F) {
            switch (battleVar1->m90_perPCTimeBarStatus[i]) {
            case 0: // draw the time bar normally
                battleVar1->m74_timeBarsLengths[i] = 0;
                //"Time"
                battleVar1->m74_timeBarsLengths[i] += battleSetupStringInPolyFT4Large(0x8F, &battleVar0->m2E08_timeBars[i][battleVar1->m74_timeBarsLengths[i]], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x44) * 0x10000) >> 16, 0x1F);
                //Bar itself
                battleVar1->m74_timeBarsLengths[i] += battleSetupStringInPolyFT4Small(0x52, &battleVar0->m2E08_timeBars[i][battleVar1->m74_timeBarsLengths[i]], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x48) * 0x10000) >> 16, 0x1C);
                {
                    int length = battleVar1->m74_timeBarsLengths[i];
                    //Bar shadow
                    battleVar1->m74_timeBarsLengths[i] += battleSetupStringInPolyFT4Small(0x53, &battleVar0->m2E08_timeBars[i][battleVar1->m74_timeBarsLengths[i]], ((i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x48) * 0x10000) >> 16, 0x1C);
                    while (length < battleVar1->m74_timeBarsLengths[i]) {
                        battleSetupTextPoly(&battleVar0->m2E08_timeBars[i][length][battleOddOrEven]);
                        length++;
                    }
                }
                battleVar1->m84_timeBarsOddOrEven[i] = battleOddOrEven;
                battleVar1->mCC[i] = 1;
                break;
            case 1: // Time bar visible on left screen
                battleVar1->m74_timeBarsLengths[3] = 0;
                battleVar1->m74_timeBarsLengths[3] += battleSetupStringInPolyFTRot(battleFont, 0x52, &battleVar0->m2E08_timeBars[3][battleVar1->m74_timeBarsLengths[3]], battleOddOrEven, 0x10, 0x98, 0x1000, 0x1000, 0xc00);
                {
                    int length = battleVar1->m74_timeBarsLengths[3];
                    battleVar1->m74_timeBarsLengths[3] += battleSetupStringInPolyFTRot(battleFont, 0x53, &battleVar0->m2E08_timeBars[3][battleVar1->m74_timeBarsLengths[3]], battleOddOrEven, 0x10, 0x98, 0x1000, 0x1000, 0xc00);
                    while (length < battleVar1->m74_timeBarsLengths[3]) {
                        battleSetupTextPoly(&battleVar0->m2E08_timeBars[3][length][battleOddOrEven]);
                        length++;
                    }
                }
                battleVar1->m84_timeBarsOddOrEven[3] = battleOddOrEven;
                break;
            case 3: // Time bar start of turn animation init
                if((apConfigArray[i].m1 == 0) || (battleCharacters[i] == 7)) {
                    battleVar1->m74_timeBarsLengths[i] = 0;
                }
                else {
                    battleVar1->mCC[i] = 0;
                }
                timerBarInitStartOfTurnAnimation(i);
                [[fallthrough]];
            case 2: // Time bar start of turn animation update
                timerBarUpdateStartOfTurnAnimation(i);
                if ((apConfigArray[i].m1 == 0) || (battleCharacters[i] == 7)) {
                    battleVar1->m7F[i] = 0;
                }
                break;
            default:
                assert(0);
            }
        }
    }
}

void updateApFuelPolyBar() {
    for (int i = 0; i < 3; i++) {
        switch (battleVar0->m835C[i].m1E1) {
        case 0: // nothing
            break;
        case 1: // AP
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x0 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y0 = 0x22;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x1 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28 + apConfigArray[i].m0 * 2;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y1 = 0x22;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x2 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y2 = 0x26;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x3 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28 + apConfigArray[i].m0 * 2;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y3 = 0x26;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r0 = 0;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g0 = 0xFF;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b0 = 0;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r1 = 0;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g1 = 0xFF;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b1 = 0;
            break;
        default:
            assert(0);
        }
    }
}

void renderGearHP() {
    updateApFuelPolyBar();
    MissingCode();
}

void battleDrawAPBar() {
    AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m8920[battleOddOrEven]);
    for (int i = 0; i < 4; i++) {
        if ((battleVar1->m7F[i]) && (currentBattleMode == 1)) {
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m5A0[i][battleVar1->m84_timeBarsOddOrEven[i]]);
        }
    }
    AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m8908_drMode[battleOddOrEven]);

    for (int i = 0; i < 7; i++) {
        if (battleVar1->mB0_isDialogWindowInitialized[i]) {
            assert(0);
        }
    }
}

void battleRenderCommandRing() {
    if (battleVar1->mCB) {
        for (int i = 0; i < 2; i++) {
            battleRenderPolyArray(&battleVar0->m641C[i][0], battleVar1->mD0_battleCommandLength[i], battleVar1->mA3);
        }
    }
}

void renderTargetSelectionCursor() {
    if (battleVar1->mC6_isTargetSelectionCursorVisible) {
        if (battleG3->mE5_fadeDirection == 0) {
            battleG3->mE0_fadeValue += 4;
            if (battleG3->mE0_fadeValue >= 0x100) {
                battleG3->mE5_fadeDirection = 1;
                battleG3->mE0_fadeValue = 0xfc;
            }
        }
        else {
            battleG3->mE0_fadeValue -= 4;
            if (battleG3->mE0_fadeValue < 0x40) {
                battleG3->mE5_fadeDirection = 0;
                battleG3->mE0_fadeValue = 0x40;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (battleG3->mE6_isDirectionEnabled[i]) {
                battleG3->m0_poly[i][battleG3->mE4_oddOrEven].r0 = battleG3->mE0_fadeValue & 0xFF;
                battleG3->m0_poly[i][battleG3->mE4_oddOrEven].g0 = 0;
                battleG3->m0_poly[i][battleG3->mE4_oddOrEven].b0 = 0;
                AddPrim(&(*pCurrentBattleOT)[1], &battleG3->m0_poly[i][battleG3->mE4_oddOrEven]);
            }
        }
    }
}

std::array<u8, 9> printIntegerToStringBattleBuffer;

void printIntegerToStringBattle(uint param_1)
{
    int iVar2;
    uint uVar3;
    uint uVar4;

    uVar3 = 100000000;
    iVar2 = 0;
    do {
        uVar4 = param_1 / uVar3;
        param_1 = param_1 % uVar3;
        uVar3 = uVar3 / 10;
        printIntegerToStringBattleBuffer[iVar2] = (byte)uVar4;
        iVar2 = iVar2 + 1;
    } while (iVar2 < 9);
    iVar2 = 1;
    auto pbVar1 = printIntegerToStringBattleBuffer.begin();
    do {
        if (printIntegerToStringBattleBuffer[iVar2] != 0) {
            if (*pbVar1 != 0) {
                return;
            }
            *pbVar1 = 0xff;
            return;
        }
        *pbVar1 = 0xff;
        iVar2 = iVar2 + 1;
        pbVar1 = pbVar1 + 1;
    } while (iVar2 < 9);
    return;
}

void setupPolyArray3A88_HP(int param_1) {
    short currentHP;
    if (battleVar0->m835C[param_1].m1E1 == 1) {
        currentHP = apConfigArray[param_1].m0;
    }
    else {
        currentHP = battleEntities[param_1].mA4_gear.m38_HP;
    }
    printIntegerToStringBattle(currentHP);
    
    for (int i = 0; i < 4; i++) {
        int characterIndex = i + 5;
        if (printIntegerToStringBattleBuffer[characterIndex] != 0xFF) {
            battleVar1->mEC[param_1] += battleSetupStringInPolyFT4Large(
                0x83 + printIntegerToStringBattleBuffer[characterIndex],
                &battleVar0->m6008[param_1][battleVar1->mEC[param_1]],
                partyMemberSpritesOffset[battlePartyLayoutType][param_1] + 0x60 * param_1 + 0x4A + i * 6,
                0x25);
        }
    }

    battleVar1->m99_oddOrEven[param_1] = battleOddOrEven;
}

int currentEntityMaxHpForDisplay;
int currentEntityHpForDisplay;
int currentEntityGearMaxHpForDisplay;
int currentEntityGearHpForDisplay;

std::array<u8, 3> currentEntityHpForDisplayBuffer;
std::array<u8, 3> currentEntityMaxHpForDisplayBuffer;
std::array<u8, 5> currentEntityGearMaxHpForDisplayBuffer;
s16 currentEntityHpForDisplayLastDigit;
s16 currentEntityMaxHpForDisplayLastDigit;
s16 currentEntityGearMaxHpForDisplayLastDigit;

int setupPolyArray3A88_HP2(int param_1, char param_2) {

    int returnValue = 0;

    currentEntityMaxHpForDisplay = battleEntities[param_1].m0_base.m4E_MaxHP;
    currentEntityHpForDisplay = battleEntities[param_1].m0_base.m4C_HP;
    currentEntityGearMaxHpForDisplay = battleEntities[param_1].mA4_gear.m60;
    currentEntityGearHpForDisplay = battleEntities[param_1].mA4_gear.m64;

    int bVar1;
    int iVar2;
    if (param_2 == '\0') {
        int iVar4 = (int)(short)currentEntityMaxHpForDisplay;
        int iVar2 = iVar4;
        if (iVar4 < 0) {
            iVar2 = iVar4 + 7;
        }
        if (iVar2 >> 3 < (int)(short)currentEntityHpForDisplay) {
            if (iVar4 < 0) {
                iVar4 = iVar4 + 3;
            }
            bVar1 = iVar4 >> 2 < (int)(short)currentEntityHpForDisplay;
            goto LAB_Battle__8007304c;
        }
    }
    else {
        iVar2 = currentEntityGearHpForDisplay;
        if (currentEntityGearHpForDisplay < 0) {
            iVar2 = currentEntityGearHpForDisplay + 7;
        }
        if (iVar2 >> 3 < currentEntityGearMaxHpForDisplay) {
            iVar2 = currentEntityGearHpForDisplay;
            if (currentEntityGearHpForDisplay < 0) {
                iVar2 = currentEntityGearHpForDisplay + 3;
            }
            bVar1 = iVar2 >> 2 < currentEntityGearMaxHpForDisplay;
        LAB_Battle__8007304c:
            if (!bVar1) {
                returnValue = 1;
            }
            goto LAB_Battle__8007305c;
        }
    }
    returnValue = 2;
LAB_Battle__8007305c:

    int iVar5 = (uint)currentEntityHpForDisplay +
        (((int)(short)currentEntityHpForDisplay / 100) * 0x10000 >> 0x10) * -100;
    int iVar7 = (uint)currentEntityMaxHpForDisplay +
        (((int)(short)currentEntityMaxHpForDisplay / 100) * 0x10000 >> 0x10) * -100;
    iVar2 = (iVar5 * 0x10000 >> 0x10) / 10;
    int iVar6 = (currentEntityGearMaxHpForDisplay % 10000) % 1000;
    int iVar4 = (iVar7 * 0x10000 >> 0x10) / 10;
    currentEntityHpForDisplayLastDigit = (short)iVar5 + (short)((uint)(iVar2 * 0x10000) >> 0x10) * -10
        ;
    iVar5 = iVar6 % 100;
    currentEntityMaxHpForDisplayLastDigit =
        (short)iVar7 + (short)((uint)(iVar4 * 0x10000) >> 0x10) * -10;
    currentEntityGearMaxHpForDisplayLastDigit = iVar5 % 10;
    currentEntityHpForDisplayBuffer[0] = (byte)((int)(short)currentEntityHpForDisplay / 100);
    currentEntityHpForDisplayBuffer[1] = (byte)iVar2;
    currentEntityMaxHpForDisplayBuffer[0] = (byte)((int)(short)currentEntityMaxHpForDisplay / 100);
    currentEntityMaxHpForDisplayBuffer[1] = (byte)iVar4;
    currentEntityGearMaxHpForDisplayBuffer[0] = (byte)(currentEntityGearMaxHpForDisplay / 10000);
    currentEntityGearMaxHpForDisplayBuffer[1] =
        (byte)((currentEntityGearMaxHpForDisplay % 10000) / 1000);
    currentEntityGearMaxHpForDisplayBuffer[2] = (byte)(iVar6 / 100);
    currentEntityGearMaxHpForDisplayBuffer[3] = (byte)(iVar5 / 10);
    currentEntityHpForDisplayBuffer[2] = (byte)currentEntityHpForDisplayLastDigit;
    currentEntityMaxHpForDisplayBuffer[2] = (byte)currentEntityMaxHpForDisplayLastDigit;
    currentEntityGearMaxHpForDisplayBuffer[4] = (byte)currentEntityGearMaxHpForDisplayLastDigit;

    // Replace all leading 0 by 0xFF
    for (auto it = currentEntityHpForDisplayBuffer.begin(); it < currentEntityHpForDisplayBuffer.end() - 1; it++) {
        if (*it)
            break;
        *it = 0xFF;
    }
    for (auto it = currentEntityMaxHpForDisplayBuffer.begin(); it < currentEntityMaxHpForDisplayBuffer.end() - 1; it++) {
        if (*it)
            break;
        *it = 0xFF;
    }
    for (auto it = currentEntityGearMaxHpForDisplayBuffer.begin(); it < currentEntityGearMaxHpForDisplayBuffer.end() - 1; it++) {
        if (*it)
            break;
        *it = 0xFF;
    }

    return returnValue;
}

struct sHPPosition {
    std::array<s16, 7> m0;
};

std::array<sHPPosition, 3> HP_positionTable = { {
    {
        {0x2c, 0x34, 0x3C, 0x44, 0x4C, 0x54, 0x5C},
    },
    {
        {0x8c, 0x94, 0x9C, 0xA4, 0xAC, 0xB4, 0xBC},
    },
    {
        {0xEC, 0xF4, 0xFC, 0x104, 0x10C, 0x114, 0x11C},
    },
} };

void tintFT4(std::array<POLY_FT4, 2>* poly, int from, int to, u8 param_4) {
    for (int i = from; i < to; i++) {
        SetShadeTex(&poly[i][battleOddOrEven], 0);
        if (param_4 == 1) {
            poly[i][battleOddOrEven].r0 = 0x80;
            poly[i][battleOddOrEven].g0 = 0x80;
            poly[i][battleOddOrEven].b0 = 0x00;
        }
        else {
            poly[i][battleOddOrEven].r0 = 0x80;
            poly[i][battleOddOrEven].g0 = 0x00;
            poly[i][battleOddOrEven].b0 = 0x00;
        }
    }
}

void tintHP(int param_1, char param_2) {
    int start = battleVar1->mE0[param_1];
    for (int i = 0; i < 3; i++) {
        if (currentEntityHpForDisplayBuffer[i] != 0xFF) {
            battleVar1->mE0[param_1] += battleSetupStringInPolyFT4Large(
                0x67 + currentEntityHpForDisplayBuffer[i],
                &battleVar0->m3A88[param_1][battleVar1->mE0[param_1]],
                HP_positionTable[param_1].m0[i] + partyMemberSpritesOffset[battlePartyLayoutType][param_1],
                0x10
            );
        }
    }
    // separator
    battleVar1->mE0[param_1] += battleSetupStringInPolyFT4Large(
        0x71,
        &battleVar0->m3A88[param_1][battleVar1->mE0[param_1]],
        HP_positionTable[param_1].m0[3] + partyMemberSpritesOffset[battlePartyLayoutType][param_1], 0x10
    );

    for (int i = 4; i < 7; i++) {
        if (currentEntityMaxHpForDisplayBuffer[i-4] != 0xFF) {
            battleVar1->mE0[param_1] += battleSetupStringInPolyFT4Large(
                0x67 + currentEntityMaxHpForDisplayBuffer[i-4],
                &battleVar0->m3A88[param_1][battleVar1->mE0[param_1]],
                HP_positionTable[param_1].m0[i] + partyMemberSpritesOffset[battlePartyLayoutType][param_1],
                0x10
            );
        }
    }

    if (param_2 != '\0') {
        tintFT4(&battleVar0->m3A88[param_1][0], start, battleVar1->mE0[param_1], param_2);
    }
}

void tintHpGear(int param_1, char param_2) {
    assert(0);
}

void updatePolyArray3A88() {
    for (int i = 0; i < 3; i++) {
        if (battleVar1->m7C[i]) {
            battleVar1->mE0[i] = 0;
            battleVar1->mEC[i] = 0;
            if (battleVisualEntities[i].m2 != 0x7F) {
                setupPolyArray3A88_HP(i);
                int tint = setupPolyArray3A88_HP2(i, apConfigArray[i].m1);
                if (apConfigArray[i].m1 == 0) {
                    tintHP(i, tint & 0xff);
                }
                else {
                    tintHpGear(i, tint & 0xff);
                }
                battleVar1->m93_oddOrEven[i] = battleOddOrEven;
                battleVar1->m7C[i] = 0;
            }
        }
    }
}

void drawBattleMode1() {
    if (!drawBattleMode1Disabled) {
        MissingCode();
        updatePolyArray3A88();
        renderGearHP();
        updatePortraits();
        battleRenderPortraitSelection();
        battleRenderPlayerPortraits();
        battleRenderCommandRing();
        MissingCode();
        renderTargetSelectionCursor();
        battleDrawAPBar();
        MissingCode();
    }
}

void drawBattleDialogWindows() {
    MissingCode();
}

void battleResultDraw(void) {
    MissingCode();
}

void drawBattleMode0(void)
{
    battleDebugDisplay = 0;
    drawBattleDialogWindows();
    battleResultDraw();
    battleDrawAPBar();
    return;
}

void drawBattleFromModes() {
    switch (currentBattleMode)
    {
    case 0:
        drawBattleMode0();
        break;
    case 1:
        drawBattleMode1();
        break;
    case 2:
        drawBattleMode2();
        break;
    default:
        assert(0);
        break;
    }
}

struct sJumpAnimationControlStruct {
    sSpriteActorCore* m4;
    void (*m8)(sJumpAnimationControlStruct* param_1);
    u32 m1C;
    u32 m20_actor;
    u32 m24;
    u32 m28;
    u32 m2C;
    u32 m30;
    u32 m34;
    u32 m40_facing;
    s32 m44_distanceToTarget;
    u8 m48;
    u8 m49;
    u8 m4A;
    sSpriteActorCore* m4C;
    // size 0x50
};

sJumpAnimationControlStruct* jumpAnimationControlStruct = nullptr;

u8 isFxFragmentsSetup = 0;

void fxFragmentSetup(u8 value) {
    assert(0);
}

void battleRender() {
    battleRenderCount++;
    checkSoftReboot();
    startOfBattleTime = VSync(-1);

    // double buffer the ordering table
    if (pCurrentBattleRenderStruct == &battleRenderStructs[0]) {
        pCurrentBattleOT = &battleRenderStructs[1].m70_OT;
        pCurrentBattleRenderStruct = &battleRenderStructs[1];
    }
    else {
        pCurrentBattleOT = &battleRenderStructs[0].m70_OT;
        pCurrentBattleRenderStruct = &battleRenderStructs[0];
    }

    ClearOTagR(*pCurrentBattleOT, 0x1000);
    battleOddOrEven = 1 - battleOddOrEven;
    MissingCode("DTL stuff");

    clearShapeTransfertTableEntry(battleOddOrEven);
    updateBattleCamera();
    renderBattleScene();
    setCurrentRenderingMatrix(&battleRenderingMatrix);
    setCharacterRenderingOT(*pCurrentBattleOT);

    MissingCode();
    renderMechasBattle(&battleRenderingMatrix, &battleMatrix800CCB94, *characterRenderingOT, battleOddOrEven);
    uploadCharacterSprites();
    execSpritesCallback();
    execSpritesCallbacks2();
    MissingCode();

    drawBattleFromModes();
    battleUpdateInputs(0);
    MissingCode();

    PutDispEnv(&pCurrentBattleRenderStruct->m5C_dispEnv);
    PutDrawEnv(&pCurrentBattleRenderStruct->m0_drawEnv);
    shapeTransfert();
    DrawOTag(&pCurrentBattleRenderStruct->m70_OT[0xfff]);

    if (battleRenderCount == 1) {
        if (jumpAnimationControlStruct != nullptr) {
            jumpAnimationControlStruct->m8(jumpAnimationControlStruct);
        }
        if (isFxFragmentsSetup != 0) {
            u8 oldValue = isFxFragmentsSetup;
            isFxFragmentsSetup = 0;
            fxFragmentSetup(oldValue);
        }
        MissingCode();
    }

    battleRenderCount = battleRenderCount + -1;
}

s8 battleTimeEnabled = 0;

int battleRenderDebugAndMain(void) {
    MissingCode("battle debug update");
    battleRender();
    return 0;
}

void checkWinConditions() {
    allPlayerCharacterBitmask = 0;
    for (int i = 0; i < 3; i++) {
        if (isBattleSlotFilled[i]) {
            if ((battleEntities[i].m0_base.m7C & 0xC000) == 0) {
                if (battleVisualEntities[i].m3 == 0) {
                    allPlayerCharacterBitmask |= characterIdToTargetBitmask(i & 0xff);
                }
            }
            else {
                if ((battleEntities[i].m0_base.m7C & 0x8000) == 0) {
                    battleEntities[i].m0_base.m4C_HP = 0;
                }
                isEntityReadyForBattle[i] = 0xFF;
            }
        }
    }

    MissingCode();
}

void initBattle3dRendering(void) {
    MissingCode();
}

void (*battleCameraModeCallback1)() = nullptr;
void (*battleCameraModeCallback2)() = nullptr;

void setBattleCameraMode(int param_1) {
    battleCameraModeInterpolation = 1;
    battleCameraMode = param_1;

    switch (param_1)
    {
    case 2:
    case 4:
        assert(0);
    default:
        if (battleCameraModeCallback1) {
            assert(0);
        }
        if (battleCameraModeCallback2) {
            assert(0);
        }
        break;
    }
}

void resetBattleCameraMode() {
    battleIR0 = 0x200;
    allEntitiesToFitInView = 0xffffffff;
    MissingCode();
    battleCameraModeInterpolation = 1;
    setBattleCameraMode(0);
}

s32 initBattleUnkData0Var0;
s32 initBattleUnkData0Var1;

void initBattleUnkData0() {
    initBattleUnkData0Var0 = 0;
    initBattleUnkData0Var1 = 0;
}

void initBattleSpriteSystem() {
    isBattleOverlayLoaded = 1;
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 0;
    MissingCode();
    initBattleUnkData0();
    resetSpriteCallbacks();
    resetBattleCameraMode();
    allocateShapeTransfert(0x5000);
    MissingCode();
    objectClippingMask = 0;
}

void setupSVector(SVECTOR* param_1, short param_2, short param_3, short param_4)
{
    param_1->vx = param_2;
    param_1->vy = param_3;
    param_1->vz = param_4;
    return;
}

void initBattleGraphics(sBattleSpriteConfigs* param_1) {
    initBattle3dRendering();
    initBattleSpriteSystem();
    createBattleSpriteLoadingTask(param_1);
    setupSVector(&battleCameraEyeTarget, battleMechaInitData->m482_eye.vx, battleMechaInitData->m482_eye.vy, battleMechaInitData->m482_eye.vz);
    setupSVector(&battleCameraEye, battleMechaInitData->m482_eye.vx, battleMechaInitData->m482_eye.vy, battleMechaInitData->m482_eye.vz);
    setupSVector(&battleCameraAtTarget, battleMechaInitData->m47C_at.vx, battleMechaInitData->m47C_at.vy, battleMechaInitData->m47C_at.vz);
    setupSVector(&battleCameraAt, battleMechaInitData->m47C_at.vx, battleMechaInitData->m47C_at.vy, battleMechaInitData->m47C_at.vz);
    SetDispMask(1);
}

s8 battleCameraVar0 = 0;
s16 battleCameraVar1 = 0;

void setCameraVisibleEntities(uint playerBitmask) {
    if (battleCameraVar0 == '\0') {
        setBattleCameraMode(1);
        computeBattleCameraParams(playerBitmask);
    }
    battleCameraVar1 = battleCameraVar2;
}

s8 battleTickMain_var0 = 1;
s8 battleTickMain_var1 = 0;
s8 battleTickMain_var2;

std::array<s8, 5> battleTickMain_var3;
std::array<s8, 11> battleTickMainSub0Var1;
std::array<s16, 11> battleTickMainSub0Var0;

void battleTickMainSub0() {
    for (int i = 0; i < 11; i++) {
        battleTickMainSub0Var1[i] = -1;
        battleTickMainSub0Var0[i] = 0;
    }
}

void resetBattleVar2_m2EB() {
    for (int i = 0; i < 3; i++) {
        battleVar2->m2EB[i] = 0;
    }
}

s16 battleTickMain_var4;

void updateBattleEntityStatus(int index) {
    MissingCode();
}

struct sUnkMonsterStatus {
    s8 m2;
    // size 4
};

std::array<sUnkMonsterStatus, 8> unknownMonsterStatus0;

std::array<std::array<s16, 3>, 3> partyMemberSpritesOffset = { {
    {0x60, 0x0, 0x0}, // 1 PC
    {0x20, 0x40, 0x0}, // 2 PC
    {0x0, 0x0, 0x0} // 3 PC
} };

void battleSoundEffect(u32) {
    MissingCode();
}

void battleSoundEffect2(u32) {
    MissingCode();
}

void drawCircleMenuChi(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 7;
            }
            else {
                battleSoundEffect2(0x4f);
            }
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 2;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        battleVar2->m2DD_currentActiveBattleMenu = 3;
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 10;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 8:
        break;
    default:
        assert(0);
    }
}

u8 updateTargetSelectionMarkerVar0 = 0;
u8 battleGetSlotStatusSub_current28Index = 0;

void characterPerformDefendAction(uint param_1)
{
    ushort uVar1;

    param_1 = param_1 & 0xff;
    battleGetSlotStatusSub_current28Index = 0;
    battleEntities[param_1].m15A_flags = battleEntities[param_1].m15A_flags | 1;
    if (((battleEntities[param_1].m15A_flags & 0x80) != 0) &&
        ((battleEntities[param_1].mA4_gear.m82 & 0x10) != 0)) {
        uVar1 = battleEntities[param_1].m0_base.m7C;
        battleEntities[param_1].mA4_gear.m7C = battleEntities[param_1].mA4_gear.m7C & 0xfe4f;
        battleEntities[param_1].m0_base.m7C = uVar1 & 0xefff;
    }
    if (updateTargetSelectionMarkerVar0 == '\x04') {
        battleTickMain_var1 = 0x3d;
    }
    return;
}

void drawCircleMenuDefend(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 7;
                battleVar2->m2F6 = 0;
                return;
            }
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 2;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 9;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 10;
                battleVar2->m2F6 = 0;
                return;
            }
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 4:
    case 6:
    case 7:
        characterPerformDefendAction(param_1);
        battleVar2->m2EA = 0;
        battleVar2->m2DE = 1;
        return;
    case 8:
        break;
    default:
        assert(0);
    }
}

void updateAPCounterDisplay() {
    MissingCode();
}

struct sBattleVar48 {
    std::array<u16, 11> m0;
    u16 m16_targetBitMask;
    std::array<u8, 11> m18_operationType;
    u8 m23_battleEntityIndex;
    std::array<s16, 11> m24;
    std::array<u8,11> m3C;
    s8 m47_battleAnimationToPlay;

    //size 0x48
};

std::array<sBattleVar48, 32> battleVar48;

void initAnimSeqFromCharacterToCharacter(u8 entity, u8 target) {
    for (int i = 0; i < 0x20; i++) {
        battleVar48[i].m47_battleAnimationToPlay = -1;
        battleVar48[i].m23_battleEntityIndex = entity;
        battleVar48[i].m16_targetBitMask = characterIdToTargetBitmask(target);
    }
}

struct sBattleJump {
    s16 m0_X;
    s16 m2_Z;
    u8 m4;
};

std::array<sBattleJump, 9> battleJumpData;

bool initJumpData(uint entity, uint target) {
    for (int i = 0; i < 9; i++) {
        battleJumpData[i].m0_X = -1;
        battleJumpData[i].m2_Z = -1;
    }

    battleJumpData[0].m0_X = battleVisualEntities[entity].mA_X;
    battleJumpData[0].m2_Z = battleVisualEntities[entity].mC_Z;
    battleJumpData[0].m4 = 0;

    for (int i = 1; i < 8; i++) {
        u8 var2 = battleLoadDataVar2Bis->m140[battleVisualEntities[entity].m0_positionSlot][battleVisualEntities[target].m0_positionSlot][i];
        if (var2 == 0xFF)
            break;

        battleJumpData[i].m0_X = battleLoadDataVar2Bis->m4[(var2 & 7) - 1][7].vx;
        battleJumpData[i].m2_Z = battleLoadDataVar2Bis->m4[(var2 & 7) - 1][7].vy;
        battleJumpData[i].m4 = var2 & 0x80;
    }

    return battleCharacters[entity] == 4;
}

u32 computeFacingForJump(int) {
    MissingCode();
    return 0;
}

u32 allocateJumpAnimationStructVar0;

void setupJumpAnimationStruct1C(uint param_1)
{
    jumpAnimationControlStruct->m1C = param_1;
    return;
}

s32 computerJumpDistanceToTarget(sVec2_s16 param_1, sVec2_s16 param_2) {
    VECTOR tempVec;
    tempVec.vx = param_1.vx - param_2.vx;
    tempVec.vz = param_1.vy - param_2.vy;

    Square0(&tempVec, &tempVec);
    return SquareRoot0(tempVec.vx + tempVec.vz);
}

uint vec2dRatan2(sVec2_s16 param_1, sVec2_s16 param_2)
{
    long lVar1;
    short local_res0;
    short sStackX_2;
    short local_res4;
    short sStackX_6;

    local_res0 = param_1.vx;
    local_res4 = param_2.vx;
    sStackX_2 = param_1.vy;
    sStackX_6 = param_2.vy;
    lVar1 = ratan2((int)sStackX_2 - (int)sStackX_6, (int)local_res0 - (int)local_res4);
    return -lVar1 & 0xfff;
}

int spriteActorCoreRatan2(sSpriteActorCore* param_1, sSpriteActorCore* param_2) {
    return vec2dRatan2(sVec2_s16::fromValue(param_2->m0_position.vx.getIntegerPart(), param_2->m0_position.vz.getIntegerPart()),
        sVec2_s16::fromValue(param_1->m0_position.vx.getIntegerPart(), param_1->m0_position.vz.getIntegerPart()));
}

void battleAnimationCallback_2Sub0(sSpriteActorCore* param_1, sSpriteActorCore* param_2)
{
    short sVar1;

    if (param_1 != param_2) {
        sVar1 = spriteActorCoreRatan2(param_1, param_2);
        setSpriteActorAngle(param_1, sVar1);
        sVar1 = spriteActorCoreRatan2(param_1, param_2);
        OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, sVar1);
        if ((param_2->mAC >> 24) != '\x15') {
            sVar1 = spriteActorCoreRatan2(param_2, param_1);
            setSpriteActorAngle(param_2, sVar1);
            sVar1 = spriteActorCoreRatan2(param_2, param_1);
            OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_2, sVar1);
        }
    }
    return;
}

void battleAnimationCallback_2(sSpriteActorCore* param_1, sSpriteActorCore* param_2)
{
    short sVar1;

    sVar1 = (param_1->mA0).vz;
    (param_1->m0_position).vx = (int)(param_1->mA0).vx << 0x10;
    (param_1->m0_position).vz = (int)sVar1 << 0x10;
    jumpAnimationControlStruct->m48 = 1;
    setupJumpAnimationStruct1C(4);
    spriteActorSetPlayingAnimation(param_1, param_1->mB0.mx0_animationId);
    battleAnimationCallback_2Sub0(param_1, param_2);
    return;
}

int needFxFragments = 0;
int isInJumpAnimationCallbackCode = 0;
std::array<sSpriteActorCore*, 11> pSpriteCoreListHead;

void setRenderingInfoForItemUser(sSpriteActorCore*) {
    MissingCode();
}

sSpriteActorCore* getSpriteCoreForAnimation(int param_1) {
    if (jumpAnimationControlStruct->m4 && (jumpAnimationControlStruct->m24 != param_1) && (battleVisualEntities[(jumpAnimationControlStruct->m4->mAC & 3) << 2 | jumpAnimationControlStruct->m4->mA8.mx1E].m3 == 0)) {
        spriteActorSetPlayingAnimation(jumpAnimationControlStruct->m4, jumpAnimationControlStruct->m4->mB0.mx0_animationId);
    }
    jumpAnimationControlStruct->m24 = param_1;
    jumpAnimationControlStruct->m4 = battleSpriteActorCores[param_1];
    return jumpAnimationControlStruct->m4;
}

int countBattleActorCoresActiveForBitmask(uint bitmask, std::array<sSpriteActorCore*, 11>& param_2, sSpriteActorCore* param_3) {
    sSpriteActorCore* psVar1;
    sBattleMechaInitData** ppsVar2;
    int iVar4;

    iVar4 = 0;
    ppsVar2 = &battleLoadDataVar2Ter;
    std::array<sSpriteActorCore*, 11>::iterator ppsVar1 = param_2.begin();
    for(int i=0; i<11; i++) {
        if (((bitmask & 1) != 0) &&
            (psVar1 = battleSpriteActorCores[0], psVar1 != (sSpriteActorCore*)0x0)) {
            psVar1->m74_pNextSpriteCore = param_3;
            *ppsVar1 = psVar1;
            ppsVar1 = ppsVar1 + 1;
            iVar4 = iVar4 + 1;
        }
        ppsVar2 = ppsVar2 + 1;
        bitmask = (bitmask & 0xffff) >> 1;
    }
    param_2[iVar4] = nullptr;
    return iVar4;
}

s16 performAttackSub3_var0 = 0;
s16 processBattleAnimationSub0_var0 = 0;
sSpriteActorCore* processBattleAnimationSub0_var1 = 0;
u8 needBattleAnimationDataLoading = 0;

void updateBattleAnimationDataLoading(void) {
    if (needBattleAnimationDataLoading) {
        assert(0);
    }
}

int processBattleAnimationSub0(void) {
    std::array< sSpriteActorCore*, 11> local_50;
    bool hasPlayedAnimationOnMecha = false;

    int count = 0;
    int slot = countBattleActorCoresActiveForBitmask(performAttackSub3_var0 & (performAttackSub3_var0 ^ processBattleAnimationSub0_var0), local_50, processBattleAnimationSub0_var1);
    if (slot != 0) {
        for (int i = slot - 1; i > -1; i--) {
            sSpriteActorCore* pSlotEntry = local_50[i];
            if ((jumpAnimationActiveActorBF >> ((pSlotEntry->mAC & 3) << 2 | (uint)pSlotEntry->mA8.mx1E) & 1U) == 0) {
                pSlotEntry->m9E_wait = 0;
                int entityId = (pSlotEntry->mAC & 3) << 2 | (uint)pSlotEntry->mA8.mx1E;
                if (battleVisualEntities[entityId].m4_isGear == 0) {
                    if (((int)pSlotEntry->mAC>>24) != '\x15') {
                        spriteActorSetPlayingAnimation(pSlotEntry, 0x15);
                    }
                }
                else {
                    hasPlayedAnimationOnMecha = true;
                    battleMechas[entityId]->m38 = 1;
                    entityId = (pSlotEntry->mAC & 3) << 2 | (uint)pSlotEntry->mA8.mx1E;
                    performMechaPlayAnimation(entityId, entityId, 0x15);
                }
                count++;
                processBattleAnimationSub0_var0 |= 1 << ((pSlotEntry->mAC & 3) << 2 | (uint)pSlotEntry->mA8.mx1E);
            }
            else {
                local_50[i] = nullptr;
            }
        }
    }

    while (true) {
        bool bVar3 = false;
        int iVar1 = 0;
        auto ppsVar2 = local_50.begin();
        if (count != 0) {
            do {
                sSpriteActorCore* pSlotEntry = *ppsVar2;
                if (((pSlotEntry != nullptr) &&
                    (pSlotEntry->m48_defaultAnimationbundle != nullptr)) &&
                    (pSlotEntry->m9E_wait != 0)) {
                    bVar3 = true;
                }
                iVar1 = iVar1 + 1;
                ppsVar2 = ppsVar2 + 1;
            } while (iVar1 != count);
        }
        if (!bVar3) break;
        if ((count != 0) && (updateBattleAnimationDataLoading(), count != 0)) {
            battleRender();
        }
    }

    if (hasPlayedAnimationOnMecha) {
        assert(0);
    }
    return 0;
}

void startJumpAnimationCallback(sSpriteActorCore*) {
    jumpAnimationControlStruct->m48 = 1;
    jumpAnimationControlStruct->m49 = jumpAnimationControlStruct->m1C & 0xFF;
}

void battleIdleDuringLoading(void)
{
    int iVar1;

    while (iVar1 = isCDBusy(), iVar1 != 0) {
        battleRender();
    }
    return;
}

struct sBattleAnimationSpecial : public sSpriteActorAnimationBundle {
    void init(std::vector<u8> inputData) {
        m_rawData = inputData;
        sSpriteActorAnimationBundle::init(m_rawData);
    }

    std::vector<u8> m_rawData;
};

sBattleAnimationSpecial battleAnimationLoadingDest;

s8 fxFragmentLoaded = 0;
s16 attackInProgress = 0;

s16 delayBattleAnimationCounter = 0;

void startSeq(std::vector<u8>::iterator param_1) {
    MissingCode();
}

void loadEffectFragmentsAndAudioModel(sBattleAnimationSpecial& buffer, u32 offset) {
    MissingCode();
}

std::optional<std::vector<u8>::iterator> loadEffectFragmentsAndAudio(sBattleAnimationSpecial& buffer) {
    std::optional<std::vector<u8>::iterator> pWds;
    int numEntries = READ_LE_U32(buffer.m_rawData.begin());
    for (int i = 0; i< numEntries - 3; i++) {
        u32 offset = READ_LE_U32(buffer.m_rawData.begin() + 0x10 + 4 * i);
        std::vector<u8>::iterator entry = buffer.m_rawData.begin() + offset;
        u32 magic = READ_LE_U32(entry);
        if (magic == ' sdw') {
            loadWdsDataIfNeeded();
            battleAnimationSoundLoaded = 0;
            startJumpAnimationVar3 = 0;
            loadWdsDataNeeded = sendWds(entry, 0);
            while (waitForMusic(0)) {
                if (runningOnDTL != -1) {
                    trap(0x400);
                }
            }
            if (i == 1) {
                loadEffectFragmentsAndAudioModel(buffer, offset);
            }
        }
        else if (magic == 'sdes') {
            startSeq(entry);
            pWds = entry;
        }
        else {
            assert(0);
            initFieldEntitySub4Sub5Sub0(&sFieldEntitySub4_110_8005a474, nullptr, sVec2_s16::fromValue(0x380, 0x100), sVec2_s16::fromValue(0x0, 0x1F4));
        }
    }
    return pWds;
}

std::optional<std::vector<u8>::iterator> loadFragmentIsNotLoadedYet() {
    std::optional<std::vector<u8>::iterator> fragmentPtr;
    if (fxFragmentLoaded == 0) {
        fragmentPtr = loadEffectFragmentsAndAudio(battleAnimationLoadingDest);
        fxFragmentLoaded = 1;
    }
    return fragmentPtr;
}

void processBattleAnimation(sSpriteActorCore* param_1) {
    if (delayBattleAnimationCounter != 0) {
        delayBattleAnimationCounter = delayBattleAnimationCounter + -1;
        return;
    }

    switch (battleVar48[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay) {
    case -1: // waiting for attack prompt
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1->getAsSpriteActorCore(), 0);
        jumpAnimationControlStruct->m48 = 1;
        if (processBattleAnimationSub0() == 0) {
            if (param_1->getAsSpriteActorCore()->mA8.mx1C != 0) {
                return;
            }
            if (battleVisualEntities[(param_1->getAsSpriteActorCore()->mAC & 3) << 2 | param_1->getAsSpriteActorCore()->mA8.mx1E].
                m3 != 0) {
                return;
            }
            spriteActorSetPlayingAnimation(param_1->getAsSpriteActorCore(), param_1->getAsSpriteActorCore()->mB0.mx0_animationId);
            return;
        }
        assert(0);
    case -2:
    {
        jumpAnimationControlStruct->m48 = 0;
        sSpriteActorCore* psVar10 = getSpriteCoreForAnimation(jumpAnimationControlStruct->m20_actor);
        setupBattleAnimationSpriteCore(psVar10);
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(psVar10, startJumpAnimationCallback);
        setupJumpAnimationStruct1C(5);
        sJumpAnimationControlStruct* psVar2 = jumpAnimationControlStruct;
        if ((psVar10->m0_position.vx.getIntegerPart() == battleVisualEntities[jumpAnimationControlStruct->m24].mA_X) &&
            (psVar10->m0_position.vz.getIntegerPart() == battleVisualEntities[jumpAnimationControlStruct->m24].mC_Z)) {
            startJumpAnimationCallback(psVar10);
            return;
        }
        (psVar10->mA0).vx = battleVisualEntities[jumpAnimationControlStruct->m24].mA_X;
        (psVar10->mA0).vy = 0;
        (psVar10->mA0).vz = battleVisualEntities[psVar2->m24].mC_Z;
        spriteActorSetPlayingAnimation(psVar10, 4);
        return;
    }
    //start attack animation
    case 4: // 1ap
    case 5: // 2ap
    case 6: // 3ap
        attackInProgress = 0;
        setRenderingInfoForItemUser(param_1->getAsSpriteActorCore());
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1->getAsSpriteActorCore(), startJumpAnimationCallback);
        (param_1->getAsSpriteActorCore()->m7C)->m8 =
            (uint)battleVar48[allocateJumpAnimationStructVar0].m18_operationType
            [(param_1->getAsSpriteActorCore()->m74_pNextSpriteCore->mAC & 3) << 2 | param_1->getAsSpriteActorCore()->m74_pNextSpriteCore->mA8.mx1E];
        if (!isVramPreBacked((param_1->getAsSpriteActorCore()->m24_vramData)->m0_spriteData)) {
            if (battleVar48[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay < 0x10) {
                if ((fxFragmentLoaded == '\0') && (battleAnimationLoadingDest.m_rawData.size() != 0)) {
                    battleIdleDuringLoading();
                    (param_1->getAsSpriteActorCore())->m50 = loadFragmentIsNotLoadedYet();
                    setupSpecialAnimation(param_1, &battleAnimationLoadingDest);
                }
                int expectActorId = (((param_1->getAsSpriteActorCore())->mAC & 3) << 2 | (uint)(param_1->getAsSpriteActorCore())->mA8.mx1E);
                if (jumpAnimationControlStruct->m20_actor == expectActorId) {
                    spriteActorSetPlayingAnimation(param_1->getAsSpriteActorCore(), ~battleVar48[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay);
                }
                else {
                    spriteActorSetPlayingAnimation(param_1->getAsSpriteActorCore(), battleVar48[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay);
                }
            }
            else {
                assert(0);
            }
        }
        else {
            spriteActorSetPlayingAnimation(param_1->getAsSpriteActorCore(), battleVar48[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay);
            sSpriteActorCore* psVar10 = param_1->getAsSpriteActorCore()->m74_pNextSpriteCore;
            computeBattleCameraParams
            (1 << ((param_1->getAsSpriteActorCore()->mAC & 3) << 2 |
                (uint)param_1->getAsSpriteActorCore()->mA8.mx1E) |
                1 << ((psVar10->mAC & 3) << 2 | (uint)psVar10->mA8.mx1E));
        }
        jumpAnimationControlStruct->m48 = 0;
        break;
        default:
            assert(0);
    }

    allocateJumpAnimationStructVar0 = allocateJumpAnimationStructVar0 + 1;
}

s8 startPlayingAttackAnimationVar0 = 0;
void startPlayingAttackAnimation() {
    getSpriteCoreForAnimation
    ((uint)(byte)battleVar48[allocateJumpAnimationStructVar0].m23_battleEntityIndex);
    setupBattleAnimationSpriteCore(jumpAnimationControlStruct->m4);

    if (startPlayingAttackAnimationVar0) {
        assert(0);
    }
    MissingCode();
    processBattleAnimation(jumpAnimationControlStruct->m4);
}

void battleAnimationCallback_5(int param_1) {
    short sVar1;
    sSpriteActorCore* psVar2;

    psVar2 = battleSpriteActorCores[param_1];
    if ((psVar2->mAC >> 24) != '\x15') {
        sVar1 = (ushort)(battleVisualEntities[param_1].m6_direction != 0) << 0xb;
        setSpriteActorAngle(psVar2, sVar1);
        OP_INIT_ENTITY_SCRIPT_sub0Sub7(psVar2, sVar1);
    }
}

bool getInitBattleUnkData0Var0Set() {
    return initBattleUnkData0Var0 != 0;
}

void allocateJumpAnimationStructCallback(sJumpAnimationControlStruct* param_1) {
    if (isInJumpAnimationCallbackCode) {
        return;
    }

    isInJumpAnimationCallbackCode = 1;
    jumpAnimationControlStruct = param_1;
    if (needFxFragments != 0) {
        assert(0);
    }

    if (jumpAnimationControlStruct->m34 != 0) {
        assert(0);
        jumpAnimationControlStruct->m34 = jumpAnimationControlStruct->m34 - 1;
    }

    // if we have a pending change
    if(jumpAnimationControlStruct->m49)
    {
        switch (jumpAnimationControlStruct->m49) {
        case 2: // happens when jump animation completes
            battleAnimationCallback_2(param_1->m4, param_1->m4->m74_pNextSpriteCore);
            break;
        case 4:
        {
            int index = (pSpriteCoreListHead[0]->mAC & 3) << 2 | (uint)pSpriteCoreListHead[0]->mA8.mx1E;
            if ((index < 3) && (battleVisualEntities[index].m4_isGear == 0) && (battleVar48[allocateJumpAnimationStructVar0 + -1].m18_operationType[index] == 7)) {
                setRenderingInfoForItemUser(pSpriteCoreListHead[0]);
                spriteActorSetPlayingAnimation(pSpriteCoreListHead[0], 0x1B);
                while (pSpriteCoreListHead[0]->mAC >> 24 == 0x1B) {
                    battleRender();
                }
            }
            break;
        }
        case 5: // end of attack
            if (battleVisualEntities[(param_1->m4->mAC & 3) << 2 | (uint)param_1->m4->mA8.mx1E].m3 == 0) {
                spriteActorSetPlayingAnimation(param_1->m4, param_1->m4->mB0.mx0_animationId);
            }
            battleAnimationCallback_5(jumpAnimationControlStruct->m24);
            setupJumpAnimationStruct1C(10);
            break;
        case 6:
            jumpUpdatePositionSub0(param_1->m4, param_1->m4->m74_pNextSpriteCore);
            break;
        case 10:
            break;
        default:
            assert(0);
        }
        jumpAnimationControlStruct->m49 = 0;
    }

    switch (jumpAnimationControlStruct->m1C) {
    case 2: // happens first when jump finishes
    {
        s32 distance = computerJumpDistanceToTarget(
            sVec2_s16::fromValue(param_1->m4->m0_position.vx.getIntegerPart(), param_1->m4->m0_position.vz.getIntegerPart()),
            sVec2_s16::fromValue(param_1->m4->mA0.vx, param_1->m4->mA0.vz)
        );
        if (jumpAnimationControlStruct->m44_distanceToTarget < distance) {
            assert(0);
        }
        else {
            jumpAnimationControlStruct->m44_distanceToTarget = distance;
        }
        break;
    }
    case 4: // wait for attack
        if (jumpAnimationControlStruct->m48 != '\0') {
            startPlayingAttackAnimation();
        }
        break;
    case 5:
        break;
    case 6:
    {
        s32 distance = computerJumpDistanceToTarget(
            sVec2_s16::fromValue(param_1->m4->m0_position.vx.getIntegerPart(), param_1->m4->m0_position.vz.getIntegerPart()),
            sVec2_s16::fromValue(param_1->m4->mA0.vx, param_1->m4->mA0.vz)
        );
        if (jumpAnimationControlStruct->m44_distanceToTarget < distance) {
            jumpUpdatePositionSub0(param_1->m4, param_1->m4C);
            isInJumpAnimationCallbackCode = 0;
            return;
        }
        jumpAnimationControlStruct->m44_distanceToTarget = distance;
        break;
    }
    case 10: // during attack
        if (allocateSavePointMeshDataSub0_var0 == getInitBattleUnkData0Var0Set()) {
            assert(0);
        }
        break;
    default:
        assert(0);
    }

    isInJumpAnimationCallbackCode = 0;
}

sJumpAnimationControlStruct* allocateJumpAnimationStruct() {
    jumpAnimationControlStruct = new sJumpAnimationControlStruct;
    jumpAnimationControlStruct->m8 = &allocateJumpAnimationStructCallback;
    allocateJumpAnimationStructVar0 = 0;
    jumpAnimationControlStruct->m4A = 0;
    jumpAnimationControlStruct->m48 = 1;
    jumpAnimationControlStruct->m30 = 0;
    jumpAnimationControlStruct->m49 = 0;
    jumpAnimationControlStruct->m2C = 1;
    jumpAnimationControlStruct->m4 = nullptr;
    jumpAnimationControlStruct->m34 = 0;
    setupJumpAnimationStruct1C(0);
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 1;
    return jumpAnimationControlStruct;
}

u16 currentJumpAnimationBitMask;
s16 numActiveSpriteCores;

void setupBattleAnimationSpriteCore(sSpriteActorCore* param_1) {
    currentJumpAnimationBitMask = battleVar48[allocateJumpAnimationStructVar0].m16_targetBitMask;
    numActiveSpriteCores = countBattleActorCoresActiveForBitmask(battleVar48[allocateJumpAnimationStructVar0].m16_targetBitMask, pSpriteCoreListHead, param_1);

    if (numActiveSpriteCores == 0) {
        pSpriteCoreListHead[0] = param_1;
    }

    param_1->m74_pNextSpriteCore = pSpriteCoreListHead[0];
    pSpriteCoreListHead[0]->m74_pNextSpriteCore = param_1;
    jumpAnimationControlStruct->m4C = pSpriteCoreListHead[0];
    jumpAnimationControlStruct->m28 = (pSpriteCoreListHead[0]->mAC & 3) << 2 | pSpriteCoreListHead[0]->mA8.mx1E;
    setSpriteActorAngle(param_1, spriteActorCoreRatan2(param_1, param_1->m74_pNextSpriteCore));
    if ((pSpriteCoreListHead[0]->mAC >> 24) != 0x15) {
        setSpriteActorAngle(pSpriteCoreListHead[0], spriteActorCoreRatan2(param_1->m74_pNextSpriteCore, param_1));
    }
}

void setBattleCameraParamX(short param_1)
{
    battleCameraParamsVar0.vx = param_1;
    return;
}

void startReadingBattleJumpAnimation(sSpriteActorCore* param_1) {
    MissingCode();
    setCurrentDirectory(0x2c, 1);
    battleAnimationLoadingDest.m_rawData.resize(getFileSizeAligned(param_1->m7C->m0));
    readFile(param_1->m7C->m0, battleAnimationLoadingDest.m_rawData, 0, 0x80);
    battleAnimationLoadingDest.init(battleAnimationLoadingDest.m_rawData);
    startReadingBattleJumpAnimationVar0 = (param_1->mAC & 3) << 2 | param_1->mA8.mx1E;
}
void jumpUpdatePosition(sSpriteActorCore* param_1);

int getRatan2ToDest(sSpriteActorCore* param_1) {
    return vec2dRatan2(sVec2_s16::fromValue(param_1->mA0.vx, param_1->mA0.vz),
        sVec2_s16::fromValue(param_1->m0_position.vx.getIntegerPart(), param_1->m0_position.vz.getIntegerPart()));
}

void jumpUpdatePositionSub1(sSpriteActorCore* param_1, uint param_2) {
    jumpAnimationControlStruct->m44_distanceToTarget = computerJumpDistanceToTarget(
        sVec2_s16::fromValue(param_1->m0_position.vx.getIntegerPart(), param_1->m0_position.vz.getIntegerPart()),
        sVec2_s16::fromValue(param_1->mA0.vx, param_1->mA0.vz)
    );
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, getRatan2ToDest(param_1));
    setSpriteActorAngle(param_1, getRatan2ToDest(param_1));
    spriteActorSetPlayingAnimation(param_1, param_2);
    setupJumpAnimationStruct1C(6);
}

void jumpUpdatePositionSub0(sSpriteActorCore* param_1, sSpriteActorCore* param_2) {
    if ((battleJumpData[jumpAnimationControlStruct->m2C].m0_X == -1) &&
        (battleJumpData[jumpAnimationControlStruct->m2C].m2_Z == -1)) {

        (param_1->m0_position).vx = (int)(param_1->mA0).vx << 0x10;
        (param_1->m0_position).vz = (int)(param_1->mA0).vz << 0x10;

        int sVar2 = -0x50;
        if ((param_2->m0_position).vx.getIntegerPart() <= (param_1->m0_position).vx.getIntegerPart()) {
            sVar2 = 0x50;
        }
        (param_1->mA0).vx = (param_2->m0_position).vx.getIntegerPart() + sVar2;
        (param_1->mA0).vy = 0;
        (param_1->mA0).vz = (param_2->m0_position).vz.getIntegerPart();

        if (((param_1->mA0).vx == (param_1->m0_position).vx.getIntegerPart()) &&
            ((param_1->mA0).vz == (param_1->m0_position).vz.getIntegerPart())) {
            assert(0);
        }
        else {
            jumpUpdatePositionSub1(param_1, 3);
            setupJumpAnimationStruct1C(2);
        }
    }
    else {
        jumpUpdatePosition(param_1);
    }
}

void jumpUpdatePosition(sSpriteActorCore* param_1) {
    if ((battleJumpData[jumpAnimationControlStruct->m2C].m0_X == -1) &&
        (battleJumpData[jumpAnimationControlStruct->m2C].m2_Z == -1)) {

        (param_1->mA0).vx = param_1->m0_position.vx.getIntegerPart();
        (param_1->mA0).vy = 0;
        (param_1->mA0).vz = param_1->m0_position.vz.getIntegerPart();

        jumpUpdatePositionSub0(param_1, param_1->m74_pNextSpriteCore);
    }
    else {
        (param_1->mA0).vx = battleJumpData[jumpAnimationControlStruct->m2C].m0_X;
        (param_1->mA0).vy = 0;
        (param_1->mA0).vz = battleJumpData[jumpAnimationControlStruct->m2C].m2_Z;

        int uVar5 = 2;
        if (battleJumpData[jumpAnimationControlStruct->m2C].m4 != 0) {
            uVar5 = 3;
        }

        jumpUpdatePositionSub1(param_1, uVar5);
        jumpAnimationControlStruct->m2C++;;
    }

}

void startJumpAnimation(int isBilly, uint actor, uint jumpTarget, uint facing) {
    MissingCode();
    assert(jumpAnimationControlStruct == nullptr);
    battleRender();
    battleRender();
    startJumpAnimationVar1 = 0;
    jumpAnimationActiveActorBF = jumpAnimationActiveActorBF & ~(ushort)(1 << (actor & 0x1f));
    jumpAnimationControlStruct = allocateJumpAnimationStruct();
    jumpAnimationControlStruct->m40_facing = facing;
    jumpAnimationControlStruct->m20_actor = actor;
    setupBattleAnimationSpriteCore(getSpriteCoreForAnimation(actor));
    startJumpAnimationVar2 = 0;

    if (battleVisualEntities[actor].m4_isGear == 0) {
        loadWdsDataIfNeeded();
        battleAnimationSoundLoaded = 0;
        setBattleCameraParamX(0xc0);
        int isPrebacked = isVramPreBacked(jumpAnimationControlStruct->m4->m24_vramData->m0_spriteData);
        if (isPrebacked == 0) {
            startJumpAnimationVar3 = 0;
            if ((battleAnimationLoadingDest.m_rawData.size() == 0) ||
                (((jumpAnimationControlStruct->m4->mAC & 3) << 2 | jumpAnimationControlStruct->m4->mA8.mx1E ) != startReadingBattleJumpAnimationVar0)) {
                startReadingBattleJumpAnimation(jumpAnimationControlStruct->m4);
            }
        }
        else {
            if (startJumpAnimationVar3 == 0) {
                battleIdleDuringLoading();
                setCurrentDirectory(0x2c, 0);
                std::vector<u8> buffer;
                buffer.resize(getFileSizeAligned(7));
                readFile(7, buffer, 0, 0x80);
                battleIdleDuringLoading();
                loadWdsDataIfNeeded();
                loadWdsDataNeeded = sendWds(buffer.begin(), 0);
                while (waitForMusic(0)) {
                    battleRender();
                }
            }
            startJumpAnimationVar3 = 1;
        }
    }
    else {
        assert(0);
    }

    if (isBilly == 0) {
        if (battleVisualEntities[actor].m4_isGear == 0) {
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(jumpAnimationControlStruct->m4, &startJumpAnimationCallback);
            jumpUpdatePosition(jumpAnimationControlStruct->m4);
        }
        else {
            assert(0);
        }
    }
    else {
        int uVar6 = 4;
        if (battleVisualEntities[actor].m4_isGear != 0) {
            uVar6 = 9;
        }
        setupJumpAnimationStruct1C(uVar6);
    }

    MissingCode();
}

void startCharacterJumpToEnemy(int param_1) {
    param_1 &= 0xFF;
    initAnimSeqFromCharacterToCharacter(param_1, battleVar2->m0[param_1].m3C_currentTarget);
    startCharacterJumpToEnemyVar0 = 0;
    bool isBilly = initJumpData(param_1, battleVar2->m0[param_1].m3C_currentTarget);
    startJumpAnimation(isBilly, param_1, battleVar2->m0[param_1].m3C_currentTarget, computeFacingForJump(param_1));
    startCharacterJumpToEnemyVar0 = 1;
    startCharacterJumpToEnemyVar1 = 0;

}

void initBattleG3Array(void) {
    battleG3->reset();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            SetPolyG3(&battleG3->m0_poly[i][j]);
            switch (i) {
            case 0:
                battleG3->m0_poly[i][j].x0 = 0xc0;
                battleG3->m0_poly[i][j].y0 = 0x70;
                battleG3->m0_poly[i][j].x1 = 0xb0;
                battleG3->m0_poly[i][j].y1 = 0x78;
                battleG3->m0_poly[i][j].x2 = 0xb0;
                battleG3->m0_poly[i][j].y2 = 0x68;
                break;
            case 1:
                battleG3->m0_poly[i][j].x0 = 0xA0;
                battleG3->m0_poly[i][j].y0 = 0x90;
                battleG3->m0_poly[i][j].x1 = 0x98;
                battleG3->m0_poly[i][j].y1 = 0x80;
                battleG3->m0_poly[i][j].x2 = 0xA8;
                battleG3->m0_poly[i][j].y2 = 0x80;
                break;
            case 2:
                battleG3->m0_poly[i][j].x0 = 0x80;
                battleG3->m0_poly[i][j].y0 = 0x70;
                battleG3->m0_poly[i][j].x1 = 0x90;
                battleG3->m0_poly[i][j].y1 = 0x68;
                battleG3->m0_poly[i][j].x2 = 0x90;
                battleG3->m0_poly[i][j].y2 = 0x78;
                break;
            case 3:
                battleG3->m0_poly[i][j].x0 = 0xA0;
                battleG3->m0_poly[i][j].y0 = 0x50;
                battleG3->m0_poly[i][j].x1 = 0x98;
                battleG3->m0_poly[i][j].y1 = 0x60;
                battleG3->m0_poly[i][j].x2 = 0xA8;
                battleG3->m0_poly[i][j].y2 = 0x60;
                break;
            default:
                assert(0);
            }

            battleG3->m0_poly[i][j].r0 = 0xff;
            battleG3->m0_poly[i][j].g0 = 0;
            battleG3->m0_poly[i][j].b0 = 0;
            battleG3->m0_poly[i][j].r1 = 0x40;
            battleG3->m0_poly[i][j].g1 = 0;
            battleG3->m0_poly[i][j].b1 = 0;
            battleG3->m0_poly[i][j].r2 = 0x40;
            battleG3->m0_poly[i][j].g2 = 0;
            battleG3->m0_poly[i][j].b2 = 0;
        }
        
    }

    battleG3->mE0_fadeValue = 0xff;
    battleG3->mE5_fadeDirection = 1;
    battleG3->mE4_oddOrEven = (byte)battleOddOrEven;
    battleVar1->mC6_isTargetSelectionCursorVisible = 1;
}

void selectDefaultTarget(int param_1) {
    battleVar2->m2E8_currentTarget = battleVar2->m0[param_1].m3C_currentTarget;
    getEntityToFace(param_1);
    bool bTargetIsValid = false;
    for (int i = 0; i < numValidTarget; i++) {
        if (targetsPerPriority[i] == battleVar2->m0[param_1].m3C_currentTarget) {
            bTargetIsValid = true;
        }
    }

    if (!bTargetIsValid) {
        battleVar2->m2E8_currentTarget = targetsPerPriority[0];
    }
}

void drawCircleMenuAttackActive(int param_1) {
    switch (battleInputButton) {
    case 0: // right
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 7;
            return;
        }
        break;
    case 1: // down
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 2;
            return;
        }
        break;
    case 2: // left
        battleVar2->m2DD_currentActiveBattleMenu = 3;
        return;
    case 3: // up
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 10;
            }
            else {
                battleSoundEffect2(0x4f);
            }
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        break;
        break;
    case 4: // attack!
    case 6:
    case 7:
        param_1 &= 0xFF;
        if (battleVar2->m0[param_1].m3C_currentTarget != 0xFF) {
            startCharacterJumpToEnemyVar0 = 0;
            startCharacterJumpToEnemy(param_1);
            selectDefaultTarget(param_1);
            initBattleG3Array();
            battleVar2->m2DD_currentActiveBattleMenu = 5;
            return;
        }
        break;
    case 5:
        return;
    case 8:
    case 0xE:
        return;
    default:
        assert(0);
    }
    MissingCode();
}

void drawCircleMenuItemActive(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            return;
        }
        break;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 8;
            return;
        }
        break;
    case 2:
        battleVar2->m2DD_currentActiveBattleMenu = 3;
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 10;
                battleVar2->m2F6 = 0;
                return;
            }
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 8:
    case 0xE:
        return;
    default:
        assert(0);
    }
    MissingCode();
}

void drawCircleMenuItemSecondaryActive(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 7;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 1;
                battleVar2->m2F6 = 0;
                return;
            }
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        break;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 2;
            return;
        }
        break;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 9;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = 3;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        break;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 10;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 4;
                battleVar2->m2F6 = 0;
                return;
            }
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        break;
    case 8:
        return;
    default:
        assert(0);
    }
}

void drawBattleMenu7(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            return;
        }
        break;
    case 1:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 8;
            return;
        }
        break;
    case 2:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 9;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = 3;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 10;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] != 0) {
                battleSoundEffect2(0x4f);
                battleVar2->m2F6 = 0;
                return;
            }
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 8:
    case 0xE:
        return;
    default:
        assert(0);
    }
    MissingCode();
}


void drawBattleMenuEscape(int param_1) {
    switch (battleInputButton) {
    case 0: // right
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 7;
            return;
        }
        if ((battleVar2->m2F6 == 0) || (newBattleInputButton2 != '\0')) {
            battleVar2->m2F6 = 1;
            battleSoundEffect2(0x4f);
            return;
        }
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] != 0) {
            battleSoundEffect2(0x4f);
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2DD_currentActiveBattleMenu = 1;
        battleVar2->m2F6 = 0;
        break;
    case 1:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] != 0) {
            battleSoundEffect2(0x4f);
            return;
        }
        battleVar2->m2DD_currentActiveBattleMenu = 8;
        return;
    case 2: // left
        battleVar2->m2DD_currentActiveBattleMenu = 3;
        return;
    case 3:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 10;
            return;
        }
        if ((battleVar2->m2F6 == 0) || (newBattleInputButton2 != '\x03')) {
            battleVar2->m2F6 = 1;
            battleSoundEffect2(0x4f);
            return;
        }
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
        }
        else {
            battleSoundEffect2(0x4f);
        }
        battleVar2->m2F6 = 0;
        return;
    case 8:
    case 0xE:
        return;
    default:
        assert(0);
    }
    MissingCode();
}

const std::array<const std::array<u8, 4>, 26> battleMenuGraphicConfigs = { {
    { {0x00, 0xFF, 0x00, 0xFF} }, //0
    { {0x00, 0xFF, 0x00, 0xFF} }, //1
    { {0x00, 0xFF, 0x01, 0xFF} }, //2
    { {0x00, 0xFF, 0x02, 0xFF} }, //3
    { {0x00, 0xFF, 0x03, 0xFF} }, //4
    { {0x01, 0x00, 0x00, 0x04} }, //5
    { {0x00, 0xFF, 0x05, 0xFF} }, //6
    { {0x00, 0xFF, 0x05, 0xFF} }, //7
    { {0x00, 0xFF, 0x06, 0xFF} }, //8
    { {0x00, 0xFF, 0x07, 0xFF} }, //9
    { {0x00, 0xFF, 0x08, 0xFF} }, //10
    { {0x00, 0x01, 0x01, 0x08} }, //11
    { {0x00, 0x01, 0x01, 0x08} }, //12
    { {0x00, 0x01, 0x01, 0x08} }, //13
    { {0x00, 0xFF, 0x09, 0xFF} }, //14
    { {0x00, 0xFF, 0x0A, 0xFF} }, //15
    { {0x00, 0xFF, 0x0A, 0xFF} }, //16
    { {0x00, 0xFF, 0x0B, 0xFF} }, //17
    { {0x00, 0xFF, 0x0C, 0xFF} }, //18
    { {0x00, 0xFF, 0x0D, 0xFF} }, //19
    { {0x00, 0xFF, 0x0E, 0xFF} }, //20
    { {0x00, 0xFF, 0x0E, 0xFF} }, //21
    { {0x00, 0xFF, 0x0F, 0xFF} }, //22
    { {0x00, 0xFF, 0x10, 0xFF} }, //23
    { {0x00, 0xFF, 0x11, 0xFF} }, //24
    { {0x01, 0x00, 0x0A, 0x12} }, //25
} };

struct sBattleMenuGraphicConfigs2 {
    s16 m0;
    s16 m2_X;
    s16 m4_Y;
    s16 m6;
    // size 0x8
};

const std::array<const std::vector<sBattleMenuGraphicConfigs2>, 9> battleMenuGraphicConfigs2 = { {
    { // 0 Attack
        {0x4000, 0x80, 0x48, 0x2009}, //Attack
        {0x4001, 0x60, 0x60, 0x208B}, //Item
        {0x4002, 0x40, 0x48, 0x0080}, //Defend
        {0x4003, 0x60, 0x30, 0x208A}, //Chi
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x54, 0x68, 0x48, 0x0},
        {-1}
    },
    { // 1 Item
        {0x4000, 0x80, 0x48, 0x2089}, //Attack
        {0x4001, 0x60, 0x60, 0x200B}, //Item
        {0x4002, 0x40, 0x48, 0x0080}, //Defend
        {0x4003, 0x60, 0x30, 0x208A}, //Chi
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 2 Defend
        {0x4000, 0x80, 0x48, 0x2089}, //Attack
        {0x4001, 0x60, 0x60, 0x208B}, //Item
        {0x4002, 0x40, 0x48, 0x0000}, //Defend
        {0x4003, 0x60, 0x30, 0x208A}, //Chi
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 3 Chi menu
        {0x4000, 0x80, 0x48, 0x2089}, //Attack
        {0x4001, 0x60, 0x60, 0x208B}, //Item
        {0x4002, 0x40, 0x48, 0x0080}, //Defend
        {0x4003, 0x60, 0x30, 0x200A}, //Chi
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 4
        {0x0003, 0xD6, 0x6E, 0x0000},
        {0x0000, 0xB0, 0x90, 0x2002},
        {0x0001, 0x8C, 0x70, 0x2001},
        {0x0002, 0xB0, 0x48, 0x2000},
        {0x000B, 0xC0, 0x70, 0x0000},
        {0x000C, 0xB0, 0x80, 0x2002},
        {0x000D, 0xA0, 0x70, 0x2001},
        {0x000E, 0xB0, 0x60, 0x2000},
        {-1}
    },
    { // 5
        {0x4004, 0x80, 0x48, 0x2005},
        {0x4005, 0x60, 0x60, 0x0080},
        {0x4006, 0x40, 0x48, 0x2088},
        {0x4007, 0x60, 0x30, 0x2087},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 6
        {0x4004, 0x80, 0x48, 0x2085},
        {0x4005, 0x60, 0x60, 0x0000},
        {0x4006, 0x40, 0x48, 0x2088},
        {0x4007, 0x60, 0x30, 0x2087},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 7
        {0x4004, 0x80, 0x48, 0x2085},
        {0x4005, 0x60, 0x60, 0x0080},
        {0x4006, 0x40, 0x48, 0x2008},
        {0x4007, 0x60, 0x30, 0x2087},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 8
        {0x4004, 0x80, 0x48, 0x2085},
        {0x4005, 0x60, 0x60, 0x0080},
        {0x4006, 0x40, 0x48, 0x2088},
        {0x4007, 0x60, 0x30, 0x2007},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
} };

void makeAllBattleCommandsDisabled() {
    MissingCode();
}

uint updateBattleMenuSpriteForMenu(uint param_1, uint menuId, char param_3) {
    if (param_3 == 0) {
        std::array<u8, 2> menuConfig1;
        std::array<u8, 2> menuConfig2;
        for (int i = 0; i < 2; i++) {
            menuConfig1[i] = battleMenuGraphicConfigs[menuId & 0xFF][i];
            menuConfig2[i] = battleMenuGraphicConfigs[menuId & 0xFF][i + 2];
            battleVar1->mD0_battleCommandLength[i] = 0;
        }

        for (int i = 0; i < 2; i++) {
            if (menuConfig1[i] == 0xFF)
                return battleOddOrEven;

            for (auto it = battleMenuGraphicConfigs2[menuConfig2[i]].begin(); it->m0 != -1; it++) {
                s16 value = it->m0;
                if (value >= 0x4000) {
                    value = battleVar2->m2C0_circleMenuCommandsNames[it->m0 & 3] = battleVar2->m0[param_1].m0_circleMenuBattleCommandsMapping[it->m0 & 0xFF];
                    if (value == 0xFF) {
                        continue;
                    }
                }
                int startOfCurrentBattleCommand = battleVar1->mD0_battleCommandLength[menuConfig1[i]];
                battleVar1->mD0_battleCommandLength[menuConfig1[i]] += battleSetupStringInPolyFT4Large(value, &battleVar0->m641C[menuConfig1[i]][battleVar1->mD0_battleCommandLength[menuConfig1[i]]], it->m2_X, it->m4_Y);

                int uVar2 = it->m6;
                int commandTransparency = (uVar2 & 0xFF) / 2;
                if (it->m6 > 0x1FFF) {
                    commandTransparency = 0x10;
                    if (battleVar2->m0[param_1].m1C_isCommandEnabled[uVar2 & 0xF] == 0) {
                        commandTransparency = (uVar2 & 0xF0) / 2;
                    }
                }
                if (commandTransparency == 0) {
                    // command is solid
                    for (int j = startOfCurrentBattleCommand; j < battleVar1->mD0_battleCommandLength[menuConfig1[i]]; j++) {
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].r0 = 0x80;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].g0 = 0x80;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].b0 = 0x80;
                    }
                }
                else {
                    // command is transparent
                    for (int j = startOfCurrentBattleCommand; j < battleVar1->mD0_battleCommandLength[menuConfig1[i]]; j++) {
                        SetSemiTrans(&battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven], 1);
                        SetShadeTex(&battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven], 0);
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].r0 = commandTransparency;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].g0 = commandTransparency;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].b0 = commandTransparency;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].tpage |= 0x20;
                    }
                }
            }
        }
        return battleOddOrEven;
    }
    else {
        makeAllBattleCommandsDisabled();
        return battleVar1->mA3;
    }
}

u32 handleMenuSelectEnemySub(u32, u32) {
    MissingCode();
    return 0;
}

sBattle800c3e24* battleG3 = nullptr;

s16 updateTargetSelectionMarkerSub0_var0;

void updateTargetSelectionMarkerSub0Sub1() {
    MissingCode();
}

void updateTargetSelectionMarkerSub0Sub2() {
    MissingCode();
}

sTaskHeader* findMatchingGraphicEntity(sTaskHeader* param_1, void (*param_2)(sTaskHeader*)) {
    sTaskHeader* pCurrent = spriteCallback2Var2;
    while (pCurrent) {
        if (pCurrent->m0_owner == param_1) {
            if ((pCurrent->m14 & 0x1fffffff) == (param_1->m10 & 0x1fffffff)) {
                if (pCurrent->m8_updateCallback == param_2) {
                    return pCurrent;
                }
            }
        }

        pCurrent = pCurrent->m18_pNext;
    }

    return nullptr;
}

struct sCustomPolySubBufferPacket {
    std::vector<u8>::iterator init(std::vector<u8>::iterator primBuffer) {
        int size = READ_LE_U8(primBuffer) * 4;
        m_rawData.insert(m_rawData.begin(), primBuffer, primBuffer + size);

        m0_runtimeSizeNeededInWords = READ_LE_U8(m_rawData.begin() + 0);
        m1_offsetToNextPacketSizeInWords = READ_LE_U8(m_rawData.begin() + 1);
        m2_polyType0 = READ_LE_U8(m_rawData.begin() + 2);
        m3_polyType1 = READ_LE_U8(m_rawData.begin() + 3);

        return primBuffer + size;
    }
    u8 m0_runtimeSizeNeededInWords;
    u8 m1_offsetToNextPacketSizeInWords;
    u8 m2_polyType0;
    u8 m3_polyType1;
    std::vector<u8> m_rawData;
};

struct sCustomPolySubBuffer {
    void init(std::vector<u8>::iterator currentSubBuffer) {
        m0_offsetToVertices = READ_LE_U32(currentSubBuffer + 0x00);
        m4_numVertices = READ_LE_U32(currentSubBuffer + 0x04);
        m10_offset = READ_LE_U32(currentSubBuffer + 0x10);
        m14_count = READ_LE_U32(currentSubBuffer + 0x14);
        
        std::vector<u8>::iterator currentPrim = currentSubBuffer + m10_offset;
        m1C_packets.resize(m14_count);
        for (int i = 0; i < m14_count; i++) {
            currentPrim = m1C_packets[i].init(currentPrim);
        }

        m_vertices.resize(m4_numVertices);
        std::vector<u8>::iterator currentVertice = currentSubBuffer + m0_offsetToVertices;
        for (int i = 0; i < m4_numVertices; i++) {
            m_vertices[i].vx = READ_LE_S16(currentVertice + 0);
            m_vertices[i].vy = READ_LE_S16(currentVertice + 2);
            m_vertices[i].vz = READ_LE_S16(currentVertice + 4);
            m_vertices[i].pad = READ_LE_S16(currentVertice + 6);
            currentVertice += 8;
        }

    }
    u32 m0_offsetToVertices;
    u32 m4_numVertices;
    u32 m10_offset;
    u32 m14_count;
    std::vector<sCustomPolySubBufferPacket> m1C_packets;
    std::vector<SVECTOR> m_vertices;
};

struct sCustomPolyBuffer {
    sCustomPolyBuffer(u8* rawBuffer, u32 rawBufferSize) {
        buffer = std::initializer_list<u8>(rawBuffer, rawBuffer + rawBufferSize);

        m8_count = READ_LE_U16(buffer.begin() + 0x8);
        mC_buffers.resize(m8_count);
        assert(m8_count == 1);
        std::vector<u8>::iterator currentSubBuffer = buffer.begin() + 0xC;
        for (int i = 0; i < m8_count; i++) {
            mC_buffers[i].init(currentSubBuffer);
        }
    }
    u32 m8_count;
    std::vector<sCustomPolySubBuffer> mC_buffers;
    std::vector<u8> buffer;
};

struct sAnyPoly {
    POLY_F3 mF3;
    POLY_G3 mG3;
};

struct sBattleCharacterBlinkingTask {
    sTaskHeader m0;
    sTaskHeader m1C;
    sCustomRenderable* m38;
    VECTOR m3C_translation;
    SVECTOR m4C_rotation;
    u32 m54;
    u32 m58_tick;
    std::array<std::vector<sAnyPoly>,2> m5C_buffer;
    const sCustomPolySubBuffer* m64_customPolySubBuffer;
};

void updateColorFromSin(sSpriteActorCore* param_1, int param_2) {
    int iVar1;
    byte bVar2;

    iVar1 = getAngleCos(param_2 << 6);
    iVar1 = (iVar1 + 0x1000 >> 6) + 0x80;
    if (0xff < iVar1) {
        iVar1 = 0xff;
    }
    bVar2 = (byte)iVar1;
    param_1->m28_colorAndCode.m0_r = bVar2;
    param_1->m28_colorAndCode.m1_g = bVar2;
    param_1->m28_colorAndCode.m2_b = bVar2;
    updateAllSubsprites(param_1);
}

void updateBattleCharacterBlinking(sTaskHeader* param_1) {
    sBattleCharacterBlinkingTask* pThis = (sBattleCharacterBlinkingTask*)param_1->m4;

    pThis->m3C_translation.vx = pThis->m38->getAsSpriteActorCore()->m0_position.vx.getIntegerPart();
    pThis->m3C_translation.vy = pThis->m38->getAsSpriteActorCore()->m0_position.vy.getIntegerPart();
    pThis->m3C_translation.vz = pThis->m38->getAsSpriteActorCore()->m0_position.vz.getIntegerPart();

    pThis->m58_tick++;

    pThis->m3C_translation.vy -= pThis->m54 + 0x20;

    updateColorFromSin(pThis->m38->getAsSpriteActorCore(), pThis->m58_tick);

    (pThis->m4C_rotation).vy += 0x10;
}

void renderCustomPolyBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, OTTable* param_3, int param_4, int param_5, int param_6)
{
    if (param_6) {
        assert(0);
    }

    for (int i = 0; i < param_1->m14_count; i++) {
        sAnyPoly& currentAnyPoint = param_2[i];

        const sCustomPolySubBufferPacket* polySource = &param_1->m1C_packets[i];
        u16 polyType = (polySource->m3_polyType1 & 0x1C) | ((polySource->m2_polyType0 ^ 1) & 1) << 8;

        switch (polyType) {
        case 0: // F3
        {
            const SVECTOR* v0 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 8)];
            const SVECTOR* v1 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 10)];
            const SVECTOR* v2 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 12)];

            gte_ldv3(v0, v1, v2);
            gte_rtpt();
            int flag;
            gte_stflg(&flag);
            gte_nclip();
            if (flag & 0x4000) {
                break;
            }
            int z;
            gte_stopz(&z);
            gte_avsz3();
            if (z < 1) break;
            int z2;
            gte_stotz(&z2);

            z2 = (z2 >> (gDepthDivider & 0x1F)) + param_5;
            if (z2 < 5) {
                z2 = 5;
            }
            if (z2 > 0xFFF)
                break;

            POLY_F3* pF3 = &currentAnyPoint.mF3;
            gte_stsxy3(&pF3->x0y0, &pF3->x1y1, &pF3->x2y2);

            pF3->m0_pNext = (*param_3)[z2].m0_pNext;
            (*param_3)[z2].m0_pNext = pF3;

            if (param_6 != 0) {
                assert(0);
            }
            break;
        }
        case 0x10: // G3
        {
            const SVECTOR* v0 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x10)];
            const SVECTOR* v1 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x12)];
            const SVECTOR* v2 = &param_1->m_vertices[READ_LE_U16(polySource->m_rawData.begin() + 0x14)];

            gte_ldv3(v0, v1, v2);
            gte_rtpt();
            int flag;
            gte_stflg(&flag);
            gte_nclip();
            if (flag & 0x4000) {
                break;
            }
            int z;
            gte_stopz(&z);
            gte_avsz3();
            if (z < 1) break;
            int z2;
            gte_stotz(&z2);

            z2 = (z2 >> (gDepthDivider & 0x1F)) + param_5;
            if (z2 < 5) {
                z2 = 5;
            }
            if (z2 > 0xFFF)
                break;

            POLY_G3* pG3 = &currentAnyPoint.mG3;
            gte_stsxy3(&pG3->x0y0, &pG3->x1y1, &pG3->x2y2);

            pG3->m0_pNext = (*param_3)[z2].m0_pNext;
            (*param_3)[z2].m0_pNext = pG3;

            if (param_6 != 0) {
                assert(0);
            }
            break;
        }
        default:
            assert(0);
        }
    }
}

void drawBattleCharacterBlinking(sTaskHeader* param_1) {
    sBattleCharacterBlinkingTask* pThis = (sBattleCharacterBlinkingTask*)param_1->m4;

    MATRIX MStack_40;
    TransMatrix(&MStack_40, &pThis->m3C_translation);
    createRotationMatrix(&pThis->m4C_rotation, &MStack_40);
    CompMatrix(&currentRenderingMatrix, &MStack_40, &MStack_40);

    long lVar1;
    int iVar2;
    lVar1 = ReadGeomScreen();
    iVar2 = lVar1 << 0xc;
    if (MStack_40.t[2] != 0) {
        iVar2 = iVar2 / MStack_40.t[2];
    }
    iVar2 = (0x1000000 / iVar2) / 2;
    VECTOR VStack_20;
    setupVector(&VStack_20, iVar2, iVar2, iVar2);
    ScaleMatrixL(&MStack_40, &VStack_20);
    SetRotMatrix(&MStack_40);
    SetTransMatrix(&MStack_40);
    renderCustomPolyBuffer(pThis->m64_customPolySubBuffer, pThis->m5C_buffer[battleOddOrEven], characterRenderingOT, 0, 0, 0);
}

void clearShapeTransferEntry(void*) {
    MissingCode();
}

void deleteBattleCharacterBlinking(sTaskHeader* param_1) {
    sBattleCharacterBlinkingTask* pThis = (sBattleCharacterBlinkingTask*)param_1->m4;

    pThis->m38->getAsSpriteActorCore()->m28_colorAndCode.m3_code |= 1;
    updateAllSubsprites(pThis->m38->getAsSpriteActorCore());
    clearShapeTransferEntry(&pThis->m5C_buffer);
    registerSpriteCallback2_2(param_1);
    allocateSavePointMeshDataSub0_callback(param_1);
    delete pThis;
}

sCustomPolyBuffer battleBlinkingPolyBuffer(battleCursor_bin, battleCursor_bin_size);

const sCustomPolySubBuffer* getCustomPolySubBuffer(const sCustomPolyBuffer* param_1, int param_2) {
    return &param_1->mC_buffers[param_2];
}

int getCustomPolySubBufferSize(const sCustomPolySubBuffer* param_1) {
    int count = 0;
    for (int i = 0; i < param_1->m14_count; i++) {
        count += (param_1->m1C_packets[i].m0_runtimeSizeNeededInWords + 1) * 4;
    }
    return count;
}

void copyCustomPolySubBuffer(const sCustomPolySubBuffer* param_1, std::vector<sAnyPoly>& param_2, int isTransparent, uint isShaded) {
    for (int i = 0; i < param_1->m14_count; i++) {
        const sCustomPolySubBufferPacket* polySource = &param_1->m1C_packets[i];

        u8 tagSize = polySource->m0_runtimeSizeNeededInWords;
        u8 code = polySource->m3_polyType1;

        u16 polyType = (polySource->m3_polyType1 & 0x1C) | ((polySource->m2_polyType0 ^ 1) & 1) << 8;
        switch (polyType) {
        case 0: // F3
        {
            sAnyPoly& newEntry = param_2.emplace_back();
            POLY_F3* poly = &newEntry.mF3;
            poly->m3_size = polySource->m0_runtimeSizeNeededInWords;
            poly->m_colorAndCode.command = polySource->m3_polyType1;
            SetShadeTex(poly, isShaded);
            if (isTransparent) {
                SetSemiTrans(poly, isTransparent);
            }
            poly->r0 = READ_LE_U8(polySource->m_rawData.begin() + 4);
            poly->g0 = READ_LE_U8(polySource->m_rawData.begin() + 5);
            poly->b0 = READ_LE_U8(polySource->m_rawData.begin() + 6);
            break;
        }
        case 0x10: // G3
        {
            sAnyPoly& newEntry = param_2.emplace_back();
            POLY_G3* poly = &newEntry.mG3;
            poly->m3_size = polySource->m0_runtimeSizeNeededInWords;
            poly->m_colorAndCode.command = polySource->m3_polyType1;
            SetShadeTex(poly, isShaded);
            if (isTransparent) {
                SetSemiTrans(poly, isTransparent);
            }
            poly->r0 = READ_LE_U8(polySource->m_rawData.begin() + 4);
            poly->g0 = READ_LE_U8(polySource->m_rawData.begin() + 5);
            poly->b0 = READ_LE_U8(polySource->m_rawData.begin() + 6);
            poly->r1 = READ_LE_U8(polySource->m_rawData.begin() + 8);
            poly->g1 = READ_LE_U8(polySource->m_rawData.begin() + 9);
            poly->b1 = READ_LE_U8(polySource->m_rawData.begin() + 10);
            poly->r2 = READ_LE_U8(polySource->m_rawData.begin() + 12);
            poly->g2 = READ_LE_U8(polySource->m_rawData.begin() + 13);
            poly->b2 = READ_LE_U8(polySource->m_rawData.begin() + 14);
            break;
        }
        default:
            assert(0);
        }
    }
}

void createCharacterBlinkingTask(sBattleSpriteActor* param_1) {
    sBattleCharacterBlinkingTask* pNewTask = createCustomRenderableEntity<sBattleCharacterBlinkingTask>(0x68, &param_1->m0, updateBattleCharacterBlinking, drawBattleCharacterBlinking, deleteBattleCharacterBlinking);
    pNewTask->m38 = param_1->m0.m4;
    pNewTask->m54 = param_1->m0.m4->getAsSpriteActorCore()->m36;
    if (spriteBytecode2ExtendedE0_Var0) {
        spriteBytecode2ExtendedE0_Var0--;
    }
    pNewTask->m0.m14 &= 0x7fffffff;
    setupSVector(&pNewTask->m4C_rotation, 0, 0, 0);
    int singleBufferSize = getCustomPolySubBufferSize(getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0));
    std::vector<sAnyPoly> dest;
    copyCustomPolySubBuffer(getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0), dest, 0, 1);

    pNewTask->m5C_buffer[0] = dest;
    pNewTask->m5C_buffer[1] = dest;
   
    pNewTask->m64_customPolySubBuffer = getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0);

    param_1->m0.m4->getAsSpriteActorCore()->m28_colorAndCode.m3_code &= 0xFE;
    updateAllSubsprites(param_1->m0.m4->getAsSpriteActorCore());
    updateBattleCharacterBlinking(&pNewTask->m0);
}

void updateCharacterBlinkingTask(u32 param_1) {
    updateTargetSelectionMarkerSub0_var0 = (short)param_1;
    
    if ((param_1 & 0xffff) == 0) {
        updateTargetSelectionMarkerSub0Sub1();
    }
    else {
        updateTargetSelectionMarkerSub0Sub2();
    }

    for (int i = 0; i < 11; i++) {
        if (!(param_1 & 1)) {
            if (battleSpriteActors[i]) {
                sTaskHeader* pTask = findMatchingGraphicEntity(&battleSpriteActors[i]->m0, updateBattleCharacterBlinking);
                if (pTask) {
                    pTask->mC(pTask);
                }
            }
        }
        else {
            if (battleSpriteActors[i]) {
                sTaskHeader* pTask = findMatchingGraphicEntity(&battleSpriteActors[i]->m0, updateBattleCharacterBlinking);
                if (pTask == nullptr) {
                    createCharacterBlinkingTask(battleSpriteActors[i]);
                }
            }
        }
        param_1 >>= 1;
    }
}

u8 selectNewSlotByDirection(byte inputSlot, byte direction) {
    s32 bestResult = 0xffffff;
    byte slot = inputSlot;
    for (int i = 0; i < 12; i++) {
        if (targetsPerPriority[i] != 0xFF) {
            bool bVar1 = false;
            if (targetsPerPriority[i] != slot) {
                s32 lVar3 = ratan2(
                    battleVisualEntities[targetsPerPriority[i]].mC_Z - battleVisualEntities[slot].mC_Z,
                    battleVisualEntities[targetsPerPriority[i]].mA_X - battleVisualEntities[slot].mA_X
                );

                switch (direction) {
                case 0:
                    if ((lVar3 + 0x200) < 0x400)
                        bVar1 = true;
                    break;
                case 1:
                    if ((lVar3 + 0x600) < 0x400)
                        bVar1 = true;
                    break;
                case 2:
                    bVar1 = (lVar3 + 0x800U & 0xffff) < 0x200;
                    if ((lVar3 - 0x600U & 0xffff) < 0x201)
                        bVar1 = true;
                    break;
                case 3:
                    if ((lVar3 - 0x200 & 0xffff) < 0x400)
                        bVar1 = true;
                    break;
                default:
                    assert(0);
                }

                if (bVar1) {
                    s32 zDiff = battleVisualEntities[targetsPerPriority[i]].mC_Z - battleVisualEntities[slot].mC_Z;
                    s32 zDiffSquare = zDiff * zDiff;
                    if (zDiff < 0) {
                        zDiffSquare = battleVisualEntities[slot].mC_Z - battleVisualEntities[targetsPerPriority[i]].mC_Z;
                        zDiffSquare = zDiffSquare * zDiffSquare;
                    }
                    s32 xDiff = (uint)(ushort)battleVisualEntities[targetsPerPriority[i]].mA_X - (uint)(ushort)battleVisualEntities[slot].mA_X;
                    s32 xDiffSquare = xDiff * xDiff;
                    if (xDiff < 0) {
                        xDiffSquare = (uint)(ushort)battleVisualEntities[slot].mA_X - (uint)(ushort)battleVisualEntities[targetsPerPriority[i]].mA_X;
                        xDiffSquare = xDiffSquare * xDiffSquare;
                    }
                    if (zDiffSquare + xDiffSquare < bestResult) {
                        slot = targetsPerPriority[i];
                        bestResult = zDiffSquare + xDiffSquare;
                    }
                }
            }
        }
    }
    return slot;
}

void updateTargetSelectionMarkerSub1(uint param_1) {
    if (battleCharacters[param_1] == 4) {
        assert(0);
    }
}

void updateTargetSelectionMarker(u8 param_1) {
    if (battleVar2->m2E9 == 0) {
        setCameraVisibleEntities(characterIdToTargetBitmask(battleVar2->m2E8_currentTarget));
        updateCharacterBlinkingTask(characterIdToTargetBitmask(battleVar2->m2E8_currentTarget));

        for (int i = 0; i < 4; i++) {
            if (selectNewSlotByDirection(battleVar2->m2E8_currentTarget, i) == battleVar2->m2E8_currentTarget) {
                battleG3->mE6_isDirectionEnabled[i] = 0;
            }
            else {
                battleG3->mE6_isDirectionEnabled[i] = 1;
            }
        }

        if (updateTargetSelectionMarkerVar0 != 4) {
            updateTargetSelectionMarkerSub1(param_1);
        }
    }
}

void drawCircleMenuCallGear(byte param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 7;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = 1;
            }
            else {
                battleSoundEffect2(0x4f);
            }
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 8;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 9;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = 3;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 8:
        return;
    default:
        assert(0);
        break;
    }
}

void handleMenuSelectEnemy_cancel_sub0(u8 param_1)
{
    battleVar48[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -2;
    battleVar48[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    startCharacterJumpToEnemyVar0 = 0;
    return;
}

void removeTargetSelectionCursors(void)
{
    battleVar1->mC6_isTargetSelectionCursorVisible = 0;
    return;
}

void removeBillyWindow(uint param_1, char param_2)
{
    if (battleCharacters[param_1 & 0xff] == 4) {
        assert(0);
    }
    return;
}

void performMechaPlayAnimation(ushort param_1, short param_2, int param_3)
{
    mechaPlayAnimation(param_1, param_2, param_3);
    return;
}

void deleteJumpAnimationControlStruct(void)
{
    delete jumpAnimationControlStruct;
    allocateSavePointMeshDataSub0_var0 = 0;
    jumpAnimationControlStruct = nullptr;
    spriteBytecode2ExtendedE0_Var0 = 0;
    return;
}

void cancelJumpAnim(void)
{
    sJumpAnimationControlStruct* psVar2;
    uint uVar3;
    sSpriteActorCore* psVar4;

    if (jumpAnimationControlStruct != (sJumpAnimationControlStruct*)0x0) {
        getSpriteCoreForAnimation(jumpAnimationControlStruct->m20_actor);
        psVar2 = jumpAnimationControlStruct;
        psVar4 = jumpAnimationControlStruct->m4;
        uVar3 = (psVar4->mAC & 3) << 2 | psVar4->mA8.mx1E;
        if (battleVisualEntities[uVar3].m4_isGear == 0) {
            (psVar4->m0_position).vx = (uint)(ushort)battleVisualEntities[jumpAnimationControlStruct->m24].mA_X << 0x10;
            (psVar4->m0_position).vz = (uint)(ushort)battleVisualEntities[jumpAnimationControlStruct->m24].mC_Z << 0x10;
            savePointCallback8Sub0Sub0_battle(psVar4);
            (psVar4->m0_position).vy = (int)psVar4->m84_maxY << 0x10;
            spriteActorSetPlayingAnimation(psVar4, psVar4->mB0.mx0_animationId);
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(psVar4, 0);
        }
        else {
            performMechaPlayAnimation(uVar3, 0, 0x1f);
        }
        deleteJumpAnimationControlStruct();
    }
    return;
}

void moveEntityToOtherEntity(byte param_1, uint param_2) {
    MissingCode();
}

void performAttackSub2() {
    for (int i = 0; i < 11; i++) {
        battleVar48[battleVar2->m2DA_indexInBattleVar48].m0[i] = 0;
        battleVar48[battleVar2->m2DA_indexInBattleVar48].m18_operationType[i] = 0xFF;
        battleVar48[battleVar2->m2DA_indexInBattleVar48].m24[i] = 0;
        battleVar48[battleVar2->m2DA_indexInBattleVar48].m3C[i] = 0xFF;
    }
}

std::array<std::array<s8, 3>, 8> perApTable = { {
    {1,5,7},
    {2,6,7},
    {3,5,7},
    {4,6,7},
    {1,5,7},
    {2,6,7},
    {3,5,7},
    {1,5,7}
} };

s16 performAttack_type = 0;
u16 performAttackSub3_varBitMask;
u8 performAttackSub3_attacker;

std::array<s8, 11> initBattleAttackStatusArray0;
std::array<u32, 11> damageDonePerAttack;

void initBattleAttackStatus() {
    for (int i = 0; i < 11; i++) {
        initBattleAttackStatusArray0[i] = 0xFF;
        damageDonePerAttack[i] = 0;
    }
}

sBattle800CDD40Sub* battleGetSlotStatusSub_current28Entry;
std::array<sBattle800cdd40, 3> battle800CDD40;
sBattleEntity* battleGetSlotStatusSub_currentBattleEntity;
sGameStateA42* battleGetSlotStatusSub_currentBattleEntityGear;

void startBattleAttackAnimationSub0(u8 attacker, u8 param_2) {
    if ((param_2 == 0) || (battleGetSlotStatusSub_current28Entry->mA & 0x100)) {
        assert(0);
    }
}

byte startBattleAttackAnimationVar0;
byte startBattleAttackAnimationVar1;
byte startBattleAttackAnimationVar2;
byte startBattleAttackAnimationVar3;
byte startBattleAttackAnimationVar4;

void startBattleAttackAnimationVar8Unimplemented() {
    assert(0);
}

void startBattleAttackAnimationVar8_0() {
    MissingCode();
}

typedef void(*startBattleAttackAnimationVar8Type)();
const std::array<startBattleAttackAnimationVar8Type, 8> startBattleAttackAnimationVar8 = { {
     startBattleAttackAnimationVar8_0,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
} };

u32 startBattleAttackAnimation() {
    initBattleAttackStatus();

    startBattleAttackAnimationVar0 = 0;
    startBattleAttackAnimationVar1 = '\0';
    startBattleAttackAnimationVar2 = performAttackSub3_attacker;

    battleGetSlotStatusSub_currentBattleEntity = &battleEntities[performAttackSub3_attacker];
    battleGetSlotStatusSub_currentBattleEntityGear = &battleEntities[performAttackSub3_attacker].mA4_gear;

    if ((battleEntities[performAttackSub3_attacker].m15A_flags & 0x80) == 0) {
        if (performAttackSub3_attacker < 3) {
            battleGetSlotStatusSub_current28Entry = &battle800CDD40[performAttackSub3_attacker].m0[battleGetSlotStatusSub_current28Index];
        }
        else {
            assert(0);
        }
        startBattleAttackAnimationSub0(performAttackSub3_attacker, 1);
        if ((battleGetSlotStatusSub_current28Entry->mA & 0x10) == 0) {
            startBattleAttackAnimationVar3 = 0;
            if (((battleGetSlotStatusSub_current28Entry->mA & 0x100) != 0) &&
                ((battleGetSlotStatusSub_currentBattleEntity->m0_base).m56_battleCommandLoadout == 1)) {
                assert(0);
            }
            if (((battleGetSlotStatusSub_currentBattleEntity->m0_base).m56_battleCommandLoadout == 4) &&
                (battleGetSlotStatusSub_current28Index - 4 < 2)) {
                assert(0);
            }
            if (((((battleGetSlotStatusSub_currentBattleEntity->m0_base).m80 & 0x20) != 0) &&
                ((battleTickMain_var4 & 7) != 0)) && (battleTickMain_var4 < 7)) {
                assert(0);
            }
            for (startBattleAttackAnimationVar4 = 0; startBattleAttackAnimationVar4 < 11; startBattleAttackAnimationVar4++) {
                u16 bitflag = 1 << startBattleAttackAnimationVar4;
                if ((bitflag & battleTickMain_var4) != 0) {
                    startBattleAttackAnimationVar5 = &battleEntities[startBattleAttackAnimationVar4].m0_base;
                    startBattleAttackAnimationVar6 = &battleEntities[startBattleAttackAnimationVar4].mA4_gear;
                    MissingCode();
                    //startBattleAttackAnimationVar7 = startBattleAttackAnimationVar4 * 0x170 + -0x7ff331d0;
                    if (startBattleAttackAnimationVar4 < 3) {
                        assert(0);
                    }
                    startBattleAttackAnimationVar8[battleGetSlotStatusSub_current28Entry->m16](); // This computes the damage done
                    MissingCode();
                    if (initBattleAttackStatusArray0[startBattleAttackAnimationVar4] == 0) {
                    }
                    if ((battleGetSlotStatusSub_current28Entry->mA & 1) == 0) {
                        performAttack_type = battleGetSlotStatusSub_current28Index;
                    }
                    else {
                        performAttack_type = battleGetSlotStatusSub_current28Entry->m2;
                    }
                }
            }
            MissingCode();
            if (((battleGetSlotStatusSub_currentBattleEntity->m0_base).m56_battleCommandLoadout == 4) &&
                (startBattleAttackAnimationVar1 == 0)) {
                assert(0);
            }
            if ((startBattleAttackAnimationVar2 < 3) && (battleGetSlotStatusSub_current28Index < 7) && (battleGetSlotStatusSub_currentBattleEntity->m0_base.m90[battleGetSlotStatusSub_current28Index] < 65000)) {
                (battleGetSlotStatusSub_currentBattleEntity->m0_base).m90[battleGetSlotStatusSub_current28Index] =
                    (battleGetSlotStatusSub_currentBattleEntity->m0_base).m90[battleGetSlotStatusSub_current28Index] +
                    (ushort)(byte)(battleGetSlotStatusSub_currentBattleEntity->m0_base).m55 +
                    (ushort)(byte)(battleGetSlotStatusSub_currentBattleEntity->m0_base).mA1;
            }
            MissingCode();
            return startBattleAttackAnimationVar0;
        }
        else {
            assert(0);
        }
        assert(0);
    }
    else {
        assert(0);
    }

}

void performAttackSub3(s8 param_1, s16 param_2, s16 param_3) {
    performAttackSub3_var0 = 0;
    performAttackSub3_varBitMask = allPlayerCharacterBitmask;
    battleGetSlotStatusSub_current28Index = battleVar2->m2DC + -1;
    battleTickMain_var4 = param_2;
    performAttack_type = param_3;
    performAttackSub3_attacker = param_1;
    startBattleAttackAnimation();
}

void updateMonsterScriptEntitiesVarByAtttack(byte param_1, byte param_2) {
    MissingCode();
}

int executeMonsterScriptWhenAttacked(char param_1) {
    MissingCode();
    return 0;
}

void performAttackSub6(s8 param_1) {
    MissingCode();
}

int performAttack(byte param_1, uint numAPUsed) {
    s32 returnValue = 0;
    if (startCharacterJumpToEnemyVar1 == '\0') {
        if (apConfigArray[param_1].m1 == 0) {
            if (battleCharacters[param_1] != 4) {
                moveEntityToOtherEntity(param_1, battleVar2->m0[param_1].m3C_currentTarget);
            }
        }
        else
        {
            assert(0);
        }
        startCharacterJumpToEnemyVar1 = '\x01';
    }
    performAttackSub2();
    if (apConfigArray[param_1].m1 == 0) {
        if ((byte)battleVar2->m2DC < 8) {
            battleVar2->m2DC = perApTable[battleVar2->m2DC][numAPUsed - 1];
        }
        else {
            assert(0);
            battleVar2->m2DC = 7;
        }
    }
    else {
        battleVar2->m2DC++;
    }
    if ((battleEntities[battleVar2->m0[param_1].m3C_currentTarget].m0_base.m34 & 0x800U) != 0) {
        assert(0);
    }
    performAttackSub3(param_1, characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget), battleVar2->m2DC - 1);
    int oldVar48 = battleVar2->m2DA_indexInBattleVar48;
    battleVar48[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    battleVar48[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = battleTickMain_var4;
    battleVar48[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = performAttack_type;
    battleVar2->m2DA_indexInBattleVar48++;
    updateMonsterScriptEntitiesVarByAtttack(param_1, battleVar2->m0[param_1].m3C_currentTarget);
    if ((2 < battleVar2->m0[param_1].m3C_currentTarget) && (executeMonsterScriptWhenAttacked(battleVar2->m0[param_1].m3C_currentTarget) != '\0'))
    {
        returnValue = 1;
    }
    performAttackSub6(oldVar48);
    return returnValue;
}

void handleMenuSelectEnemy(u8 param_1) {
    startCharacterJumpToEnemyVar0 = 0;
    if (battleVar2->m2E1_waitBeforeNextAttackInput != 0) {
        startCharacterJumpToEnemyVar0 = 0;
        return;
    }
    battleVar2->m2DF_currentAttackNumOfApUsed = 0;
    updateTargetSelectionMarker(param_1);
    switch (battleInputButton) {
    case 5: // cancel attack
        if (battleVar2->m2D4_remainingAP == battleVar2->m2D5_maxAP) {
            // Didn't use any point, so go back as if nothing happened
            cancelJumpAnim();
            startCharacterJumpToEnemyVar0 = 1;
            battleVar1->m7B = 0;
            battleVar1->mAF = 0;
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            removeTargetSelectionCursors();
            removeBillyWindow(param_1, 1);
        }
        else {
            // We used some points, end of turn
            handleMenuSelectEnemy_cancel_sub0(param_1);
            battleVar2->m2E1_waitBeforeNextAttackInput = 1;
        }
        break;
    case 4:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[2]) {
            battleInputButton = 5;
            battleSoundEffect(0x4F);
        }
        break;
    case 6:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0]) {
            battleInputButton = 5;
            battleSoundEffect(0x4F);
        }
        break;
    case 7:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[1]) {
            battleInputButton = 5;
            battleSoundEffect(0x4F);
        }
        break;
    case 0: // right
    case 1: // down
    case 2: // left
    case 3: // up
    case 8:
    case 0xFF:
        break;
    default:
        assert(0);
    }
    if (battleVar2->m2E9 == 0) { // if we can still move
        // handle target change
        switch (battleInputButton) {
        case 0:
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, 0);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case 1:
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, 1);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case 2:
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, 2);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case 3:
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, 3);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        }
        initJumpData(param_1, battleVar2->m2E8_currentTarget);
    }
    battleVar1->mAF = 1;
    if (battleInputButton == 5) {
        return;
    }
    if (battleInputButton < 6) {
        if (battleInputButton != 4) {
            return;
        }
        if ((handleMenuSelectEnemySub(4, param_1) != '\0') && (-1 < (int)((byte)battleVar2->m2D4_remainingAP - 3))) {
            battleVar2->m2E4 = battleVar2->m2E4 + '\x01';
        }
        battleVar2->m2DF_currentAttackNumOfApUsed = battleVar2->m2DF_currentAttackNumOfApUsed + '\x01';
    }
    else {
        if (battleInputButton == 6) goto LAB_Battle__80081e90;
        if (battleInputButton != 7) {
            return;
        }
    }
    battleVar2->m2DF_currentAttackNumOfApUsed = battleVar2->m2DF_currentAttackNumOfApUsed + '\x01';
LAB_Battle__80081e90:
    battleVar2->m2DF_currentAttackNumOfApUsed = battleVar2->m2DF_currentAttackNumOfApUsed + '\x01';
    if ((int)((uint)(byte)battleVar2->m2D4_remainingAP - (uint)(byte)battleVar2->m2DF_currentAttackNumOfApUsed) < 0) {
        battleSoundEffect(0x4f);
    }
    else {
        battleSoundEffect(0x4d);
        MissingCode();
        if (battleVar1->mCB == '\0') {
            battleVar2->m2DD_currentActiveBattleMenu = 5;
            battleVar2->m2E2_previousActiveBattleMenu = 5;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = 100;
            battleVar2->m2E2_previousActiveBattleMenu = 0xff;
            battleVar2->m2E0 = 1;
        }
        battleVar2->m2D4_remainingAP = battleVar2->m2D4_remainingAP - battleVar2->m2DF_currentAttackNumOfApUsed;
        updateAPCounterDisplay();
        MissingCode();
        battleVar2->m2E5 = performAttack(param_1, battleVar2->m2DF_currentAttackNumOfApUsed);
        MissingCode();
        battleVar2->m2E3 = 1;
    }
    if (battleVar2->m2D4_remainingAP == '\0') {
        handleMenuSelectEnemy_cancel_sub0(param_1);
        battleVar2->m2E1_waitBeforeNextAttackInput = 1;
    }
    if (battleVar2->m2E5 != '\0') {
        handleMenuSelectEnemy_cancel_sub0(param_1);
        battleVar2->m2E1_waitBeforeNextAttackInput = 1;
    }
}

const std::array<u16, 16> party1C_InitialValues = { {
        0x8000, 0x4000, 0x2000, 0x1000,
        0x800, 0x400, 0x200, 0x100,
        0x80, 0x40, 0x20, 0x10,
        0x8, 0x4, 0x2, 0x1
} };

void setupTurnRenderLoop(int param_1) {
    battleVar2->m2EA = 0;
    battleVar2->m2E9 = 0;
    battleVar2->m2DE = 0;
    battleVar2->m2E0 = 0;
    battleVar2->m2E1_waitBeforeNextAttackInput = 0;
    battleVar2->m2E2_previousActiveBattleMenu = 0xff;
    battleVar2->m2E3 = 0;
    battleVar2->m2E4 = 0;
    battleVar2->m2DC = 0;

    battleVar2->m2D5_maxAP = apConfigArray[param_1].m4_currentAP;
    battleVar2->m2D4_remainingAP = apConfigArray[param_1].m4_currentAP;

    battleVar1->m90_perPCTimeBarStatus[param_1] = 3;
    MissingCode();
    while (battleVar1->m90_perPCTimeBarStatus[param_1] != 1) {
        battleRenderDebugAndMain();
    }
    startCharacterJumpToEnemyVar0 = 1;
    battleSoundEffect2(0x5a);

    MissingCode();

    for (int i = 0; i < 16; i++) {
        battleVar2->m0[param_1].m1C_isCommandEnabled[i] = battleEntities[param_1].m0_base.m7A_commandEnabledBF & party1C_InitialValues[i];
    }

    MissingCode();

    if (apConfigArray[param_1].m1 == 0) {
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            setupTurnRenderLoop_menuVar = 0;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = 4;
            setupTurnRenderLoop_menuVar = 3;
        }
        if ((battleEntities[param_1].m0_base.m7C & 2) != 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 2;
            setupTurnRenderLoop_menuVar = 1;
        }
        //initBattleRenderStruct2();
        MissingCode();
    }
    else {
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 0x10;
            setupTurnRenderLoop_menuVar = 0;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = 0x13;
            setupTurnRenderLoop_menuVar = 3;
        }
        MissingCode();
    }

    MissingCode();

    if (((battleVar2->m2DE == '\0') && (battleRunningVar1 == '\0')) && (battleVar2->m2DB == '\0')) {
        while (1) {
            if (apConfigArray[param_1].m1 == 0) {
                //setAPMarksPositions(battleVar2->m2D5_maxAP, battleVar2->m2D4_remainingAP);
                MissingCode();
            }
            if (battleVar2->m2E2_previousActiveBattleMenu != battleVar2->m2DD_currentActiveBattleMenu) {
                battleVar2->m2F6 = 0;
                battleVar2->m2E2_previousActiveBattleMenu = battleVar2->m2DD_currentActiveBattleMenu;
                battleVar1->mCB = 0;
                battleVar1->mA3 = updateBattleMenuSpriteForMenu(param_1, battleVar2->m2DD_currentActiveBattleMenu, battleVar2->m2E0);
                battleVar1->mCB = 1;
            }
            if ((battleInputButton == -1) || (battleRenderDebugAndMain(), battleVar2->m2DB != '\0'))
                break;
            MissingCode();
            switch (battleVar2->m2DD_currentActiveBattleMenu) {
            case 1:
                drawCircleMenuAttackActive(param_1);
                break;
            case 2:
                drawCircleMenuItemActive(param_1);
                break;
            case 3:
                drawCircleMenuDefend(param_1);
                break;
            case 4:
                drawCircleMenuChi(param_1);
                break;
            case 7:
                drawBattleMenu7(param_1);
                break;
            case 8:
                drawCircleMenuItemSecondaryActive(param_1);
                break;
            case 9:
                drawBattleMenuEscape(param_1);
                break;
            case 5:
                handleMenuSelectEnemy(param_1);
                break;
            case 10:
                drawCircleMenuCallGear(param_1);
                break;
            case 100:
                battleVar2->m2E2_previousActiveBattleMenu = 0xff;
                handleMenuSelectEnemy(param_1);
                break;
            default:
                assert(0);
            }
            if (((battleVar2->m2DE != '\0') || (battleRunningVar1 != '\0')) || (battleVar2->m2DB != '\0'))
                break;
        }
    }

    battleVar1->mCB = 0;
    battleVar1->mAF = 0;
    battleVar1->m90_perPCTimeBarStatus[param_1] = 0;
    battleVar1->m7F[param_1] = 0;

    MissingCode();
}

bool isTargetValid(uint param_1, uint param_2)
{
    bool bVar1;

    param_2 = param_2 & 0xff;
    bVar1 = false;
    if (isBattleSlotFilled[param_2] != 0) {
        if (battleVisualEntities[param_2].m3 == 0) {
            if (apConfigArray[param_2].m1 == 0) {
                if (battleLoadDataVar2Bis->m140[battleVisualEntities[param_1].m0_positionSlot][battleVisualEntities[param_2].m0_positionSlot][0] == 0) {
                    bVar1 = (battleEntities[param_2].m0_base.m7C & 0xc001) == 0;
                }
            }
            else if ((battleEntities[param_2].mA4_gear.m7C & 0xc001) == 0) {
                bVar1 = true;
            }
        }
    }
    return bVar1;
}

u8 getEntityToFace(u8 param_1) {
    numValidTarget = 0;

    std::array<u8, 12> array1;
    std::array<u8, 12> array2;
    for (int i = 0; i < 12; i++) {
        array2[i] = 0xFF;
        targetsPerPriority[i] = 0xFF;
        array1[i] = 0;
    }

    if (param_1 < 3) {
        // Player
        auto array2It = array2.begin();
        for (int j = 3; j < 11; j++) {
            if (isTargetValid(param_1, j)) {
                *(array2It++) = j;
                numValidTarget++;
            }
        }
    }
    else {
        // Enemy
        auto array2It = array2.begin();
        for (int j = 0; j < 3; j++) {
            if (isTargetValid(param_1, j)) {
                *(array2It++) = j;
                numValidTarget++;
            }
        }
    }

    // add current one first
    {
        auto targetsPerPriorityIt = targetsPerPriority.begin();
        auto array1It = array1.begin();
        for (int i = 0; i < numValidTarget; i++) {
            if (battleVisualEntities[param_1].m0_positionSlot == battleVisualEntities[array2[i]].m0_positionSlot) {
                *(targetsPerPriorityIt++) = array2[i];
                array2[i] = 0xFF; // so that we don't count it twice
                *(array1It++) = 1;
            }
        }
    }

    // add leftovers
    {
        auto targetsPerPriorityIt = targetsPerPriority.begin();
        for (int i = 0; i < numValidTarget; i++) {
            if (array2[i] != 0xFF) {
                *(targetsPerPriorityIt++) = array2[i];
            }
        }
    }

    if (array1[0] == 0) {
        for (int i = 1; i < numValidTarget; i++) {
            if (battleEntities[targetsPerPriority[i]].m0_base.m4C_HP < battleEntities[targetsPerPriority[0]].m0_base.m4C_HP) {
                std::swap<u8>(targetsPerPriority[0], targetsPerPriority[i]);
            }
        }
    }
    else if (numValidTarget > 1) {
        for (int i = 1; i < numValidTarget; i++) {
            if (array1[i] != 0) {
                if (battleEntities[targetsPerPriority[i]].m0_base.m4C_HP < battleEntities[targetsPerPriority[0]].m0_base.m4C_HP) {
                    std::swap<u8>(targetsPerPriority[0], targetsPerPriority[i]);
                }
            }
        }
    }

    return targetsPerPriority[0];
}

bool getDirectionBetween2BattleEntities(uint param_1, uint param_2)
{
    return battleVisualEntities[param_2 & 0xff].mA_X < battleVisualEntities[param_1 & 0xff].mA_X;
}

void battleTickMain(s8 param_1) {
    battleTickMain_var0 = 1;
    battleTickMain_var1 = 0;

    if (battleVar2->m2D3_currentEntityTurn == 0)
        return;

    battleTickMainSub0();
    resetBattleVar2_m2EB();
    makeBattleTimeProgress = 0;
    battleVar2->m2D3_currentEntityTurn = battleVar2->m2D3_currentEntityTurn - 1;
    battleTickMain_var2 = battleVar2->m2D3_currentEntityTurn;
    battleVar2->m2DA_indexInBattleVar48 = 0;
    battleVar2->m2DB = 0;

    for (int i = 4; i > -1; i--) {
        battleTickMain_var3[i] = 0;
    }

    battleTickMain_var4 = 0;
    updateBattleEntityStatus(battleVar2->m2D3_currentEntityTurn);

    int currentEntityTurn = battleVar2->m2D3_currentEntityTurn;

    if (currentEntityTurn < 3) {
        for (int i = 7; i > -1; i--) {
            unknownMonsterStatus0[i].m2 = 0;
        }

        int offsetPerPartyMember = partyMemberSpritesOffset[battlePartyLayoutType][battleVar2->m2D3_currentEntityTurn];
        battleVar0->m63C8[battleOddOrEven].x0y0.vx = battleVar2->m2D3_currentEntityTurn * 0x60 + 0x10 + offsetPerPartyMember;
        battleVar0->m63C8[battleOddOrEven].x0y0.vy = 8;
        battleVar0->m63C8[battleOddOrEven].x1y1.vx = battleVar2->m2D3_currentEntityTurn * 0x60 + 0x28 + offsetPerPartyMember;
        battleVar0->m63C8[battleOddOrEven].x1y1.vy = 8;
        battleVar0->m63C8[battleOddOrEven].x2y2.vx = battleVar2->m2D3_currentEntityTurn * 0x60 + 0x10 + offsetPerPartyMember;
        battleVar0->m63C8[battleOddOrEven].x2y2.vy = 0x20;
        battleVar0->m63C8[battleOddOrEven].x3y3.vx = battleVar2->m2D3_currentEntityTurn * 0x60 + 0x28 + offsetPerPartyMember;
        battleVar0->m63C8[battleOddOrEven].x3y3.vy = 0x20;
        battleVar0->m6414 = battleOddOrEven;
        battleVar0->m6415 = 1;

        if ((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m7C & 0x2080) == 0) {
            if ((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m80 & 0x1000) == 0) {
                if ((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m80 & 0x2000) == 0) {
                    setupTurnRenderLoop(battleVar2->m2D3_currentEntityTurn);
                }
                else {
                    assert(0);
                }
                MissingCode();
            }
        }

        MissingCode();
    }
    else {
        MissingCode();
    }

    MissingCode();
    battleVar0->m6415 = 0;
    for (int i = 0; i < 0xB; i++) {
        battleVar2->m0[i].m3C_currentTarget = getEntityToFace(i);
        battleVisualEntities[i].m6_direction = getDirectionBetween2BattleEntities(currentEntityTurn, battleVar2->m0[i].m3C_currentTarget);
    }

    MissingCode();

    checkWinConditions();

    MissingCode();

    makeBattleTimeProgress = 1;
}

void battleTickGameplay() {

    if (bBattleTickMode0 == 0) {
        if (bBattleTickMode1 == 0) {
            battleVar2->m2D3_currentEntityTurn = 0;
            s32 uVar3 = (uint)currentEntryInRandomTurnOrder;
            s32 uVar2 = (uint)randomTurnOrder[uVar3];
            s32 bVar1 = isEntityReadyForBattle[uVar2];
            do {
                if (bVar1 == 1) {
                    battleVar2->m2D3_currentEntityTurn = (char)uVar2 + 1;
                    currentEntryInRandomTurnOrder = (byte)(uVar3 + 1);
                    if ((uVar3 + 1 & 0xff) == 0xb) {
                        currentEntryInRandomTurnOrder = 0;
                        battleTickMain(0);
                        return;
                    }
                }
                uVar3 = uVar3 + 1;
                if (uVar3 == 11) {
                    uVar3 = 0;
                }
                if (uVar3 == currentEntryInRandomTurnOrder) {
                    battleTickMain(0);
                    return;
                }
                uVar2 = (uint)randomTurnOrder[uVar3];
                bVar1 = isEntityReadyForBattle[uVar2];
            } while (true);
        }
        battleVar2->m2D3_currentEntityTurn = bBattleTickMode1;
        MissingCode();
        battleTickMain(0);
    }
    else {
        assert(0);
    }
}

void init8920() {
    MissingCode();

    (battleVar1->m0).y = 0;
    (battleVar1->m0).x = 0;
    (battleVar1->m0).h = 0x100;
    (battleVar1->m0).w = 0x100;

    MissingCode();

    (battleVar1->m8).x = ((battleVar0->mA244_X & 0x3f) << 1);
    (battleVar1->m8).w = 0x100;
    (battleVar1->m8).h = 0x100;
    (battleVar1->m8).y = battleVar0->mA248_Y;

    MissingCode();

    SetDrawMode(&battleVar0->m8908_drMode[0], 0, 0, GetTPage(battleVar0->mA238, 0, battleVar0->mA244_X, battleVar0->mA248_Y), &battleVar1->m8);
    SetDrawMode(&battleVar0->m8908_drMode[1], 0, 0, GetTPage(battleVar0->mA238, 0, battleVar0->mA244_X, battleVar0->mA248_Y), &battleVar1->m8);
    SetDrawMode(&battleVar0->m8920[0], 0, 0, GetTPage(battleVar0->mA238, 0, battleVar0->mA244_X, battleVar0->mA248_Y), &battleVar1->m0);
    SetDrawMode(&battleVar0->m8920[1], 0, 0, GetTPage(battleVar0->mA238, 0, battleVar0->mA244_X, battleVar0->mA248_Y), &battleVar1->m0);
}

void battleMain() {
    battleVar0 = new sBattleVar0;
    battleVar1 = new sBattleVar1;
    battleVar2 = new sBattleVar2;

    //memset(battleVar0, 0, sizeof(sBattleVar0));
    memset(battleVar1, 0, sizeof(sBattleVar1));
    memset(battleVar2, 0, sizeof(sBattleVar2));

    // TODO: more logic here
    {
        int battleSelected = battleInitVar0 - 1;
        if (battleInitVar0 == '\0') {
            battleDebugSelector();
        }
        else {
            battleInitVar0 = '\0';
            requestedBattleConfig = battleSelected;
        }
    }

    MissingCode();

    currentBattleConfig = battleConfigs[requestedBattleConfig];
    startBattleLoader(battleTransitionEffect);
    checkWinConditions();
    currentBattleMode = 2;
    initBattleGraphics(&battleConfigFile3);

    MissingCode();
    battleLoadDataVar2Bis = battleLoadDataVar2;
    battleLoadDataVar2Ter = battleLoadDataVar2;
    MissingCode();

    init8920();
    makeBattleTimeProgress = 1;
    setCameraVisibleEntities(allPlayerCharacterBitmask);
    battleRenderDebugAndMain();

    MissingCode();
    
    // Wait for battle intro to finish and battle time to start
    while (battleTimeEnabled == '\0') {
        battleRenderDebugAndMain();
    }

    MissingCode();

    if (isBattleAnEvent == '\0') {
        currentBattleMode = 1;
    }

    MissingCode();

    for (int i = 0; i < 3; i++) {
        if (battleCharacters[i] != 0x7F) {
            MissingCode();
        }
    }

    int battleRunningVar0Temp = battleRunningVar0;

    while (!battleRunningVar1 && !isBattleAnEvent) {
        if (battleTimeEnabled != '\0') {
            battleTickGameplay();
        }
        battleRenderDebugAndMain();
    }

    MissingCode();

}

void battleEntryPoint(void) {
    battleDebugDisplay = 1;
    waitReadCompletion(0);
    setCurrentDirectory(0xc, 0);
    Noah_MissingCode("DTL codepath init");
    initGraphicsForBattle();
    battleMain();

    Noah_MissingCode("Battle epilog");

    Hack("Go back to field from battle");
    setGameMode(1);
    MissingCode();
    /*
    if (battleInitVar0 == '\0') {
        battleInitVar1 = 1;
    }*/
    bootGame(0);
}

void battleHandleInput(void) {
    u32 savedPlayTime;
    bool isPaused = true;
    bool pauseSignDisplayed = false;
    do {
        if (isControllerConnected(0) == 0) {
            if (!pauseSignDisplayed) {
                decompressPauseSignToVram(0x88, 100);
                decompressPauseSignToVram(0x88, 0x144);
                pauseSignDisplayed = true;
                pauseMusic();
                savedPlayTime = playTimeInVsync;
            }
        }
        else {
            isPaused = false;
            if (pauseSignDisplayed) {
                resumeMusic();
                playTimeInVsync = savedPlayTime;
            }
        }
    } while (isPaused);

    int newBattleInputButton = 8;

    do {
        if (!getInputOverflowed()) {
            while (loadInputFromVSyncBuffer()) {
                MissingCode();
                if (battleIsPaused == false) {
                    if (newPadButtonForField & controllerButtons::RIGHT) {
                        battleSoundEffect2(0x4C);
                        newBattleInputButton = 0;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::DOWN) {
                        battleSoundEffect2(0x4C);
                        newBattleInputButton = 1;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::LEFT) {
                        battleSoundEffect2(0x4C);
                        newBattleInputButton = 2;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::UP) {
                        battleSoundEffect2(0x4C);
                        newBattleInputButton = 3;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::INTERACT) {
                        newBattleInputButton = 4;
                        battleSoundEffect2(0x4d);
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::CROSS) {
                        newBattleInputButton = 5;
                        battleSoundEffect2(0x4e);
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::JUMP) {
                        newBattleInputButton = 6;
                        battleSoundEffect2(0x4d);
                    }
                    else if (newPadButtonForDialogs & controllerButtons::TRIANGLE) {
                        newBattleInputButton = 7;
                        battleSoundEffect2(0x4d);
                        break;
                    }
                    else if (newPadButtonForDialogs & 1) {
                        newBattleInputButton = 0xc;
                        if (*pRunningOnDTL != -1) {
                            assert(0);
                        }
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::R1) {
                        if (*pRunningOnDTL != -1) {
                            assert(0);
                        }
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::SELECT) {
                        newBattleInputButton = 0xd;
                        battleSoundEffect2(0x4d);
                        break;
                    }
                    else if ((*pRunningOnDTL != -1) && (newPadButtonForField & controllerButtons::R1) && (newPadButtonForField & controllerButtons::L1)) { // debug command
                        assert(0);
                    }
                    else if (newPadButtonForDialogs & controllerButtons::START) {
                        newBattleInputButton = 0xe;
                        if (battleTimeEnabled != '\0') {
                            if (battleIsPaused == false) {
                                decompressPauseSignToVram(0x88, 100);
                                decompressPauseSignToVram(0x88, 0x144);
                                pauseMusic();
                                battleIsPaused = true;
                                savedPlayTime = playTimeInVsync;
                            }
                            else {
                                resumeMusic();
                                battleIsPaused = false;
                                playTimeInVsync = savedPlayTime;
                            }
                        }
                        break;
                    }
                }

            }
        }
        else {
            resetInputs();
        }

        if (!battleIsPaused) {
            battleInputButton = newBattleInputButton;
            return;
        }

    } while (true);
}

void battleSpriteOp89Sub3(sSpriteActorCore* param_1, short param_2)
{
    short sVar1;
    int iVar2;
    int iVar3;

    param_1->m32_direction = param_2;
    iVar3 = param_1->m18_moveSpeed >> 3;
    iVar2 = getAngleSin((int)param_2);
    sVar1 = param_1->m32_direction;
    (param_1->mC_step).vx = (iVar2 >> 1) * iVar3 >> 8;
    iVar2 = getAngleCos((int)sVar1);
    (param_1->mC_step).vz = -((iVar2 >> 1) * iVar3) >> 8;
    return;
}


void battleSpriteOp89(sSpriteActorCore* param_1) {
    param_1->m0_position.vx &= 0xFFFF0000;
    param_1->m0_position.vy &= 0xFFFF0000;
    param_1->m0_position.vz &= 0xFFFF0000;

    VECTOR local_40;
    local_40.vx = param_1->mA0.vx - param_1->m0_position.vx.getIntegerPart();
    local_40.vy = 0;
    local_40.vz = param_1->mA0.vz - param_1->m0_position.vz.getIntegerPart();

    s32 angle = ratan2(local_40.vz, local_40.vx);

    Square0(&local_40, &local_40);
    s32 distance = SquareRoot0(local_40.vx + local_40.vz);
    s32 speed = -(((param_1->mC_step).vy << 1) / param_1->m1C_gravity);
    if (speed == 0) {
        param_1->m18_moveSpeed = 0;
    }
    else {
        param_1->m18_moveSpeed = (distance << 0x10) / speed;
    }

    SVECTOR SStack_30;
    setupSVector(&SStack_30, (param_1->mA0).vx, (param_1->mA0).vy, (param_1->mA0).vz);

    MissingCode(); // compute the vertical step

    battleSpriteOp89Sub3(param_1, -(short)angle);
    savePointCallback8Sub0Sub0(param_1);
}

void executeSpriteBytecode2_battle(sSpriteActorCore* param_1) {
    do
    {
        if (param_1->m9E_wait != 0)
        {
            return;
        }

        sPS1Pointer startOfOpcode = param_1->m64_spriteByteCode;
        u8 bytecode = READ_LE_U8(param_1->m64_spriteByteCode);
        sPS1Pointer pEndOfOpcode = param_1->m64_spriteByteCode + 1;
        if (bytecode < 0x80)
        {
            param_1->m64_spriteByteCode = pEndOfOpcode;

            int waitDelay = 0;

            if (bytecode < 0x10)
            {
                addToSpriteTransferList(param_1, param_1->m34_currentSpriteFrame + 1);
                waitDelay = (bytecode & 0xf) + 1;
            }
            else if (bytecode < 0x20)
            {
                param_1->mA8.mxB++;
                executeSpriteBytecode2Sub1(param_1);
                waitDelay = (bytecode & 0xf) + 1;
            }
            else if (bytecode < 0x30)
            {
                param_1->m34_currentSpriteFrame--;
                executeSpriteBytecode2Sub1(param_1);
                waitDelay = (bytecode & 0xf) + 1;
            }

            if (bytecode < 0x40)
            {
                waitDelay = (bytecode & 0xf) + 1;
            }

            int iVar12 = waitDelay * (param_1->mAC >> 7 & 0xfff);
            if (iVar12 < 0) {
                iVar12 = iVar12 + 0xff;
            }
            iVar12 = iVar12 >> 8;
            if (iVar12 == 0) {
                iVar12 = 1;
            }

            param_1->m9E_wait += iVar12;
            param_1->mA8.mx16++;

            if (param_1->mA8.mx16 != 0)
            {
                return;
            }

            param_1->mA8.mx16 = 0x3F;
            return;
        }

        switch (bytecode)
        {
        case 0x80:
            param_1->mA8.mx1C = 0;
            if (param_1->m68 != nullptr) {
                param_1->m68(param_1);
                return;
            }
            if (param_1->mB0.mx0_animationId > -1) {
                spriteActorSetPlayingAnimation(param_1, param_1->mB0.mx0_animationId);
            }
            param_1->mA8.mx1C = 0;
            return;
        case 0x98:
        case 0xD4:
        case 0xFA:
            assert(0);
            break;
        case 0x85:
            param_1->m64_spriteByteCode = popPointerFromAnimationStack(param_1);
            break;
        case 0xBE:
        {
            bytecode = READ_LE_U8(pEndOfOpcode);
            int iVar15 = (((int)bytecode >> 0xb & 0xfU) + 1) * (param_1->mAC >> 7 & 0xfff);
            int _uVar13 = bytecode & 0x1ff;
            if (false) {
                iVar15 = iVar15 + 0xff;
            }
            int sVar12 = (short)((uint)iVar15 >> 8);
            if (iVar15 >> 8 == 0) {
                sVar12 = 1;
            }
            if ((param_1->m3C & 3) == 1) {
                if (((int)bytecode < 0) && (_uVar13 != 0)) {
                    //_uVar13 = (uint) * (byte*)((int)param_1->m60 + (_uVar13 - 1));
                    assert(0);
                }
                int uVar5 = (int)bytecode >> 6 & 8;
                int uVar10 = param_1->m3C;
                int uVar6 = (uVar5 >> 3 ^ (param_1->mAC & 4) >> 2) << 3;
                int uVar14 = uVar10 & 0xfffffff7 | uVar6;
                param_1->mAC = param_1->mAC & 0xfffffff7 | uVar5;
                param_1->m3C = uVar14;
                if (((int)bytecode >> 10 & 1U) == 0) {
                    uVar14 = uVar10 & 0xffffffe7 | uVar6;
                }
                else {
                    uVar14 = uVar14 | 0x10;
                }
                param_1->m3C = uVar14;
                addToSpriteTransferList(param_1, (short)_uVar13);
            }
            else {
                param_1->m34_currentSpriteFrame = (short)_uVar13;
            }
            param_1->m9E_wait += sVar12;
            param_1->m64_spriteByteCode += 3;
            return;
        }
        case 0x81:
            if (((param_1->mAC >> 24) & 0xFF) == 0x3F)
            {
                assert(0);
            }
            param_1->m9E_wait = 0;
            if (param_1->m68) {
                param_1->m68(param_1);
            }
            param_1->mA8.mx1C = 1;
            return;
        case 0x82: // happen at end of walk cycle animation for player
            if (param_1->m68) {
                param_1->m68(param_1);
            }
            {
                int iVar15 = (param_1->mC_step).vy;
                spriteActorSetPlayingAnimation(param_1, (s8)((param_1->mAC >> 24) & 0xFF));
                (param_1->mC_step).vy = iVar15;
            }
            param_1->m9E_wait = 0;
            executeSpriteBytecode2_battle(param_1);
            return;
        case 0x86: // Happen when Fei jumps in cutscene after lahan battle. (or when jumping during gameplay)
            if (param_1->mC_step.vy < 0)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0x87: // Happen when Fei jumps in cutscene after lahan battle. (or when jumping during gameplay)
            if ((param_1->m0_position.vy.getIntegerPart()) < param_1->m84_maxY)
            {
                param_1->m9E_wait = 1;
                return;
            }
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xA7:
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            if ((READ_LE_U8(pEndOfOpcode) & 0x80) != 0) {
                spriteCallback2Var0 = (READ_LE_U8(pEndOfOpcode) & 0x7f) + 1;
                param_1->m9E_wait = param_1->m9E_wait + 1;
                return;
            }
            {
                int iVar13 = READ_LE_U8(pEndOfOpcode + 2) * (param_1->mAC >> 7 & 0xfff);
                if (false) {
                    iVar13 = iVar13 + 0xff;
                }
                int sVar11 = (short)((uint)iVar13 >> 8);
                if (iVar13 >> 8 == 0) {
                    sVar11 = 1;
                }
                param_1->m9E_wait = param_1->m9E_wait + sVar11;
            }
            return;
        case 0xCA:
            MissingCode();
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xE1: // jump in bytecode
            param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
            break;
        case 0xE2: // Looks like a call
            pushBytecodePointerOnAnimationStack(param_1, param_1->m64_spriteByteCode + 3);
            param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
            break;
        case 0xe4: // loop number of time the last byte on the stack is
        {
            s8 cVar4 = popByteFromAnimationStack(param_1);
            if (cVar4 != 0) {
                pushByteOnAnimationStack(param_1, cVar4 - 1);
                param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
            }
            else
            {
                // end of loop
                param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            }
        }
        break;

        // BATTLE SPECIFIC!
        case 0x8E:
            param_1->m64_spriteByteCode.makeNull();
            return;
        case 0x89: 
            battleSpriteOp89(param_1);
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0x97:
            MissingCode();
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xA4:
            if (param_1->m74_pNextSpriteCore->m48_defaultAnimationbundle == nullptr) {
                assert(0);
            }
            else {
                spriteActorSetPlayingAnimation(param_1->m74_pNextSpriteCore, READ_LE_S8(pEndOfOpcode));
            }
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xC2:
        {
            param_1->m0_position.vy = (param_1->m84_maxY - 1) * 0x10000;
            int scale = READ_LE_S8(pEndOfOpcode) * param_1->m2C_scale;
            if (scale < 0) {
                scale += 0xFFF;
            }
            scale >>= 0xC;
            if (((param_1->mAC >> 2) & 1) == 0) {
                scale = -scale;
            }
            param_1->mA0.vx = param_1->m74_pNextSpriteCore->m0_position.vx.getIntegerPart() + scale;
            param_1->mA0.vy = 0;
            param_1->mA0.vz = param_1->m74_pNextSpriteCore->m0_position.vx.getIntegerPart();
            battleSpriteOp89(param_1);
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        }
        case 0xC3:
            MissingCode(); // battle sprite effect
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xC8:
        {
            MissingCode();
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        }
        case 0xE8: // Battle sprite effect
            MissingCode();
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        case 0xF8: // switch for battle
        {
            u8 attack = battleVar48[allocateJumpAnimationStructVar0 + -1].m18_operationType
                [(param_1->m74_pNextSpriteCore->mAC & 3) << 2 |
                param_1->m74_pNextSpriteCore->mA8.mx1E];

            bool bVar6;
            u8 attackType = READ_LE_U8(pEndOfOpcode + 2) & 0x7F;
            switch (attackType) {
            case 0:
                bVar6 = attack == 0;
                break;
            case 4:
                bVar6 = attack == 4;
                break;
            case 5:
                bVar6 = attack == 5;
                break;
            default:
                assert(0);
            }

            if (READ_LE_U8(pEndOfOpcode + 2) & 0x80) {
                bVar6 = !bVar6;
            }
            if (bVar6) {
                param_1->m64_spriteByteCode += READ_LE_S16(pEndOfOpcode);
            }
            else {
                param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            }
            break;
        }


        default:
            executeSpriteBytecode2Extended(param_1, bytecode, pEndOfOpcode);
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        }
    } while (1);
}