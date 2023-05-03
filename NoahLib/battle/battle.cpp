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

std::array<struct sBackgroundPoly*, 2> battleEnvBackgroundPolys;

std::array<sLoadedMechas*, 32> battleMechas;

void uploadTextureToVram(sPS1Pointer param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY);

std::array<sMechaInitVar4, 0x14> battleMechaInitVar4;
void sMechaModel_init(sModel& modelBlock, sMechaInitVar4* param_2);
std::vector<sMechaBone>* processMechaMesh(sMechaInitVar4* param_1, std::vector<sMechaDataTable1_C>& param_2, u32 param_3, int useTpageAndClut, short tpageX, short tpageY, short clutX, short clutY);
void initMechSub110(sLoadedMechas* param_1);

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
SVECTOR battleCameraEye2;
SVECTOR battleCameraAt;
SVECTOR battleCameraAt2;
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
                MissingCode();
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
        // hack param 5 is not 0, but it's probably garbage (uninitialized) and not user anyway
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

// Compute camera to fit all characters from bitmask in view
void computeBattleCameraParams(uint bitmask) {
    MissingCode();
}

s32 battleCameraMode = 0;
u32 allEntitiesToFitInView = 0xFFFFFFFF;

void updateBattleCamera() {
    switch (battleCameraMode) {
    case 1:
        computeBattleCameraParams(allEntitiesToFitInView);
        break;
    default:
        assert(0);
    }

    MissingCode();
}

void execSpritesCallback(void);
void execSpritesCallbacks2(void);

void battleTimeProgress() {
    if (makeBattleTimeProgress) {
        for (int i = 0; i < 11; i++) {
            if (isBattleSlotFilled[i] && !isEntityReadyForBattle[i]) {
                s8 iVar5 = 1;
                MissingCode();
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

    MissingCode();

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

void battleUpdateInputs(int mode) {
    switch (currentBattleMode) {
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
        MissingCode();
        //battleRenderPolyArray(&battleVar0->m3A88[j][0], battleVar1->mE0[j], battleVar1->m93_oddOrEven[j]);
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

void drawBattleMode1() {
    if (!drawBattleMode1Disabled) {
        MissingCode();
        renderGearHP();
        updatePortraits();
        battleRenderPortraitSelection();
        battleRenderPlayerPortraits();
        battleRenderCommandRing();
        MissingCode();
        battleDrawAPBar();
        MissingCode();
    }
}

void drawBattleFromModes() {
    switch (currentBattleMode)
    {
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

    MissingCode();

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
                    battleEntities[i].m0_base.m24_HP = 0;
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

void initBattleSpriteSystem() {
    isBattleOverlayLoaded = 1;
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 0;
    MissingCode();
    resetSpriteCallbacks();
    MissingCode();
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
    setupSVector(&battleCameraEye2, battleMechaInitData->m482_eye.vx, battleMechaInitData->m482_eye.vy, battleMechaInitData->m482_eye.vz);
    setupSVector(&battleCameraEye, battleMechaInitData->m482_eye.vx, battleMechaInitData->m482_eye.vy, battleMechaInitData->m482_eye.vz);
    setupSVector(&battleCameraAt2, battleMechaInitData->m47C_at.vx, battleMechaInitData->m47C_at.vy, battleMechaInitData->m47C_at.vz);
    setupSVector(&battleCameraAt, battleMechaInitData->m47C_at.vx, battleMechaInitData->m47C_at.vy, battleMechaInitData->m47C_at.vz);
    SetDispMask(1);
}

s32 battleCameraModeSet = 0;
void setBattleCameraMode(int param_1) {
    battleCameraModeSet = 1;
    battleCameraMode = param_1;

    switch (param_1)
    {
    case 2:
    case 4:
        assert(0);
    default:
        MissingCode();
        break;
    }
}

void setCameraVisibleEntities(uint playerBitmask) {
    MissingCode();
    setBattleCameraMode(1);
    computeBattleCameraParams(playerBitmask);
    MissingCode();
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
    case 8:
        break;
    default:
        assert(0);
    }
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
                goto LAB_Battle__80081688;
            }
        LAB_Battle__80081674:
            battleSoundEffect2(0x4f);
        LAB_Battle__80081688:
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
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
        break;
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
    u16 m16_targetBitMask;
    u8 m23_battleEntityIndex;
    u8 m47;

    //size 0x48
};

std::array<sBattleVar48, 32> battleVar48;

void initAnimSeqFromCharacterToCharacter(u8 entity, u8 target) {
    for (int i = 0; i < 0x20; i++) {
        battleVar48[i].m47 = 0xFF;
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
        u8 var2 = battleLoadDataVar2Bis->m140[battleVisualEntities[entity].m0][battleVisualEntities[target].m0][i];
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

u32 allocateJumpAnimationStructVar0;

void setupJumpAnimationStruct1C(uint param_1)
{
    jumpAnimationControlStruct->m1C = param_1;
    return;
}

void allocateJumpAnimationStructCallback(sJumpAnimationControlStruct* param_1) {
    assert(0);
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

sSpriteActorCore* getSpriteCoreForAnimation(int param_1) {
    if (jumpAnimationControlStruct->m4) {
        assert(0);
    }
    jumpAnimationControlStruct->m24 = param_1;
    jumpAnimationControlStruct->m4 = battleSpriteActorCores[param_1];
    return jumpAnimationControlStruct->m4;
}

u16 currentJumpAnimationBitMask;
s16 numActiveSpriteCores;
sSpriteActorCore* pSpriteCoreListHead = nullptr;

int countBattleActorCoresActiveForBitmask(uint bitmask, sSpriteActorCore** param_2, sSpriteActorCore* param_3) {
    sSpriteActorCore* psVar1;
    sSpriteActorCore** ppsVar1;
    sBattleMechaInitData** ppsVar2;
    int iVar3;
    int iVar4;

    iVar3 = 0;
    iVar4 = 0;
    ppsVar2 = &battleLoadDataVar2Ter;
    ppsVar1 = param_2;
    do {
        if (((bitmask & 1) != 0) &&
            (psVar1 = battleSpriteActorCores[0], psVar1 != (sSpriteActorCore*)0x0)) {
            psVar1->m74_pNextSpriteCore = param_3;
            *ppsVar1 = psVar1;
            ppsVar1 = ppsVar1 + 1;
            iVar4 = iVar4 + 1;
        }
        ppsVar2 = ppsVar2 + 1;
        iVar3 = iVar3 + 1;
        bitmask = (bitmask & 0xffff) >> 1;
    } while (iVar3 != 0xb);
    param_2[iVar4] = (sSpriteActorCore*)0x0;
    return iVar4;
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

void setupBattleAnimationSpriteCore(sSpriteActorCore* param_1) {
    currentJumpAnimationBitMask = battleVar48[allocateJumpAnimationStructVar0].m16_targetBitMask;
    numActiveSpriteCores = countBattleActorCoresActiveForBitmask(battleVar48[allocateJumpAnimationStructVar0].m16_targetBitMask, &pSpriteCoreListHead, param_1);

    if (numActiveSpriteCores == 0) {
        pSpriteCoreListHead = param_1;
    }

    param_1->m74_pNextSpriteCore = pSpriteCoreListHead;
    pSpriteCoreListHead->m74_pNextSpriteCore = param_1;
    jumpAnimationControlStruct->m4C = pSpriteCoreListHead;
    jumpAnimationControlStruct->m28 = (pSpriteCoreListHead->mAC & 3) << 2 | pSpriteCoreListHead->mA8.mx1E;
    setSpriteActorAngle(param_1, spriteActorCoreRatan2(param_1, param_1->m74_pNextSpriteCore));
    if ((pSpriteCoreListHead->mAC >> 24) != 0x15) {
        setSpriteActorAngle(pSpriteCoreListHead, spriteActorCoreRatan2(param_1->m74_pNextSpriteCore, param_1));
    }
}
u8 startJumpAnimationVar1;
u16 jumpAnimationActiveActorBF = 0;
s16 startJumpAnimationVar2;
void* loadWdsDataNeeded = nullptr;

void loadWdsDataIfNeeded(void)
{
    if (loadWdsDataNeeded != 0) {
        assert(0);
    }
    loadWdsDataNeeded = 0;
    return;
}

u8 battleAnimationSoundLoaded;
SVECTOR battleCameraParamsVar0;

void setBattleCameraParamX(short param_1)
{
    battleCameraParamsVar0.vx = param_1;
    return;
}

u8 startJumpAnimationVar3 = 0;

void battleIdleDuringLoading(void)
{
    int iVar1;

    while (iVar1 = isCDBusy(), iVar1 != 0) {
        battleRender();
    }
    return;
}

void* sendWds(std::vector<u8>& buffer, u32 param_2) {
    return nullptr;
}

int waitForMusic(uint param_1) {
    return 0;
}

std::vector<u8> battleAnimationLoadingDest;
u32 startReadingBattleJumpAnimationVar0 = 0;

void startReadingBattleJumpAnimation(sSpriteActorCore* param_1) {
    MissingCode();
    setCurrentDirectory(0x2c, 1);
    battleAnimationLoadingDest.resize(getFileSizeAligned(param_1->m7C->m0));
    startReadingBattleJumpAnimationVar0 = (param_1->mAC & 3) << 2 | param_1->mA8.mx1E;
}

void startJumpAnimationCallback(sSpriteActorCore*) {
    jumpAnimationControlStruct->m48 = 1;
    jumpAnimationControlStruct->m49 = jumpAnimationControlStruct->m1C & 0xFF;
}

void jumpUpdatePosition(sSpriteActorCore* param_1);

int getRatan2ToDest(sSpriteActorCore* param_1) {
    return vec2dRatan2(sVec2_s16::fromValue(param_1->mA0.vx, param_1->mA0.vz),
        sVec2_s16::fromValue(param_1->m0_position.vx.getIntegerPart(), param_1->m0_position.vz.getIntegerPart()));
}

s32 computerJumpDistanceToTarget(sVec2_s16 param_1, sVec2_s16 param_2) {
    VECTOR tempVec;
    tempVec.vx = param_1.vx - param_2.vx;
    tempVec.vz = param_1.vy - param_2.vy;

    Square0(&tempVec, &tempVec);
    return SquareRoot0(tempVec.vx + tempVec.vz);
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

        (param_1->mA0).vy = 0;
        (param_1->mA0).vx = param_1->m0_position.vx.getIntegerPart();
        (param_1->mA0).vz = param_1->m0_position.vz.getIntegerPart();

        jumpUpdatePositionSub0(param_1, param_1->m74_pNextSpriteCore);
    }
    else {
        assert(0);
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
            if ((battleAnimationLoadingDest.size() == 0) ||
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
                loadWdsDataNeeded = sendWds(buffer, 0);
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
    MissingCode();
    bool isBilly = initJumpData(param_1, battleVar2->m0[param_1].m3C_currentTarget);
    startJumpAnimation(isBilly, param_1, battleVar2->m0[param_1].m3C_currentTarget, computeFacingForJump(param_1));
    MissingCode();

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
            MissingCode();
            startCharacterJumpToEnemy(param_1);
            MissingCode();
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

void drawBattleMenu7(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            return;
        }
        break;
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

const std::array<const std::vector<sBattleMenuGraphicConfigs2>, 8> battleMenuGraphicConfigs2 = { {
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
} };

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
        assert(0);
        return battleVar1->mA3;
    }
}

u32 handleMenuSelectEnemySub(u32, u32) {
    MissingCode();
    return 0;
}

u8 startCharacterJumpToEnemyVar0;

struct sBattle800c3e24 {
    std::array<u8, 4> mE6_isDirectionEnabled;
};

sBattle800c3e24* battleG3 = nullptr;

void updateTargetSelectionMarkerSub0(u32) {
    MissingCode();
}

u8 selectNewSlotByDirection(byte slot, byte direction) {
    MissingCode();
    return 0;
}

u8 updateTargetSelectionMarkerVar0 = 0;

void updateTargetSelectionMarker(u8 param_1) {
    if (battleVar2->m2E9 == 0) {
        setCameraVisibleEntities(characterIdToTargetBitmask(battleVar2->m2E8));
        updateTargetSelectionMarkerSub0(characterIdToTargetBitmask(battleVar2->m2E8));

        for (int i = 0; i < 4; i++) {
            if (selectNewSlotByDirection(battleVar2->m2E8, i) == battleVar2->m2E8) {
                battleG3->mE6_isDirectionEnabled[i] = 0;
            }
            else {
                battleG3->mE6_isDirectionEnabled[i] = 1;
            }
        }

        if (updateTargetSelectionMarkerVar0 != 4) {
            assert(0);
            //updateTargetSelectionMarkerSub1(param_1);
        }
    }
}

void handleMenuSelectEnemy(u8 param_1) {
    startCharacterJumpToEnemyVar0 = 0;
    if (battleVar2->m2E1 != 0) {
        startCharacterJumpToEnemyVar0 = 0;
        return;
    }
    battleVar2->m2DF = 0;
    updateTargetSelectionMarker(param_1);
    switch (battleInputButton) {
    case 5:
        if (battleVar2->m2D4_remainingAP == battleVar2->m2D5_maxAP) {
            MissingCode();
            battleVar1->m7B = 0;
            battleVar1->mAF = 0;
            battleVar2->m2DD_currentActiveBattleMenu = 1;
            MissingCode();
        }
        else {
            MissingCode();
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
    case 8:
        break;
    default:
        assert(0);
    }
    if (battleVar2->m2E9) {
        MissingCode();
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
        battleVar2->m2DF = battleVar2->m2DF + '\x01';
    }
    else {
        if (battleInputButton == 6) goto LAB_Battle__80081e90;
        if (battleInputButton != 7) {
            return;
        }
    }
    battleVar2->m2DF = battleVar2->m2DF + '\x01';
LAB_Battle__80081e90:
    MissingCode();
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
    battleVar2->m2E1 = 0;
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
            case 9:
                drawBattleMenuEscape(param_1);
                break;
            case 5:
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
            if (-1 < *(char*)&param_1->mB0) {
                spriteActorSetPlayingAnimation(param_1, *(char*)&param_1->mB0);
            }
            param_1->mA8.mx1C = 0;
            return;
        case 0x8E:
        case 0x98:
        case 0xC8:
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
        case 0x89: // BATTLE SPECIFIC!
            battleSpriteOp89(param_1);
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
        default:
            executeSpriteBytecode2Extended(param_1, bytecode, pEndOfOpcode);
            param_1->m64_spriteByteCode += sizePerBytecodeTable[bytecode];
            break;
        }
    } while (1);
}