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

struct sBattleVar2Sub {
    std::array<u8, 8> m0_circleMenuBattleCommandsMapping;
    std::array<s16, 16> m1C;
    u8 m3C;
    // size 0x40
};

struct sBattleVar2 {
    std::array<sBattleVar2Sub, 3> m0;
    std::array<u8, 4> m2C0_circleMenuCommandsNames;
    u8 m2D3_currentEntityTurn;
    u8 m2D4_remainingAP;
    u8 m2D5_maxAP;
    u8 m2DA_indexInBattleVar48;
    u8 m2DB;
    u8 m2DC;
    u8 m2DD_currentActiveBattleMenu;
    u8 m2DE;
    u8 m2DF;
    u8 m2E0;
    u8 m2E1;
    u8 m2E2_previousActiveBattleMenu;
    u8 m2E3;
    u8 m2E4;
    u8 m2E9;
    u8 m2EA;
    std::array<u8, 3> m2EB;
    u8 m2F6;
    // size 0x2F8
}*battleVar2 = nullptr;

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
            battleRenderPolyArray(&battleVar0->m641C[i][0], battleVar1->mD0[i], battleVar1->mA3);
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
    case 8:
        break;
    default:
        assert(0);
    }
}

void drawCircleMenuAttackActive(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = 7;
            return;
        }
        break;
    case 2: // left
        battleVar2->m2DD_currentActiveBattleMenu = 3;
        return;
    case 4:
    case 6:
    case 7:
        param_1 &= 0xFF;
        if (battleVar2->m0[param_1].m3C != 0xFF) {
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

void drawBattleMenu7(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C[9] == 0) {
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

const std::array<const std::vector<sBattleMenuGraphicConfigs2>, 6> battleMenuGraphicConfigs2 = { {
    { // 0
        {0x4000, 0x80, 0x48, 0x2009},
        {0x4001, 0x60, 0x60, 0x208B},
        {0x4002, 0x40, 0x48, 0x0080},
        {0x4003, 0x60, 0x30, 0x208A},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x54, 0x68, 0x48, 0x0},
        {-1}
    },
    { // 1
        {0x4000, 0x80, 0x48, 0x2089},
        {0x4001, 0x60, 0x60, 0x200B},
        {0x4002, 0x40, 0x48, 0x0080},
        {0x4003, 0x60, 0x30, 0x208A},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 2
        {0x4000, 0x80, 0x48, 0x2089},
        {0x4001, 0x60, 0x60, 0x208B},
        {0x4002, 0x40, 0x48, 0x0000},
        {0x4003, 0x60, 0x30, 0x208A},
        {0x56, 0x60, 0x40, 0x80},
        {0x54, 0x68, 0x48, 0x0},
        {0x57, 0x60, 0x50, 0x0},
        {-1}
    },
    { // 3
        {0x4000, 0x80, 0x48, 0x2089},
        {0x4001, 0x60, 0x60, 0x208B},
        {0x4002, 0x40, 0x48, 0x0080},
        {0x4003, 0x60, 0x30, 0x200A},
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
} };

uint updateBattleMenuSpriteForMenu(uint param_1, uint menuId, char param_3) {
    if (param_3 == 0) {
        std::array<u8, 2> menuConfig1;
        std::array<u8, 2> menuConfig2;
        for (int i = 0; i < 2; i++) {
            menuConfig1[i] = battleMenuGraphicConfigs[menuId & 0xFF][i];
            menuConfig2[i] = battleMenuGraphicConfigs[menuId & 0xFF][i + 2];
            battleVar1->mD0[i] = 0;
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
                int iVar10 = battleVar1->mD0[menuConfig1[i]];
                battleVar1->mD0[menuConfig1[i]] += battleSetupStringInPolyFT4Large(value, &battleVar0->m641C[menuConfig1[i]][battleVar1->mD0[menuConfig1[i]]], it->m2_X, it->m4_Y);

                int uVar2 = it->m6;
                int uVar4 = (uVar2 & 0xFF) / 2;
                if (it->m6 > 0x1FFF) {
                    uVar4 = 0x10;
                    if (battleVar2->m0[param_1].m1C[uVar2 & 0xF] == 0) {
                        uVar4 = (uVar2 & 0xF0) / 2;
                    }
                }
                if (uVar4 == 0) {
                    for (int j = iVar10; j < battleVar1->mD0[menuConfig1[i]]; j++) {
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].r0 = 0x80;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].g0 = 0x80;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].b0 = 0x80;
                    }
                }
                else if (iVar10 < battleVar1->mD0[menuConfig1[i]]) {
                    for (int j = iVar10; j < battleVar1->mD0[menuConfig1[i]]; j++) {
                        SetSemiTrans(&battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven], 1);
                        SetShadeTex(&battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven], 0);
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].r0 = uVar4;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].g0 = uVar4;
                        battleVar0->m641C[menuConfig1[i]][j][battleOddOrEven].b0 = uVar4;
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

void battleSoundEffect(u32) {
    MissingCode();
}

u32 handleMenuSelectEnemySub(u32, u32) {
    MissingCode();
    return 0;
}

void handleMenuSelectEnemy(u8 param_1) {
    MissingCode();
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
        if (battleVar2->m0[param_1].m1C[2]) {
            battleInputButton = 5;
            battleSoundEffect(0x4F);
        }
        break;
    case 6:
        if (battleVar2->m0[param_1].m1C[0]) {
            battleInputButton = 5;
            battleSoundEffect(0x4F);
        }
        break;
    case 7:
        if (battleVar2->m0[param_1].m1C[1]) {
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
        battleVar2->m0[param_1].m1C[i] = party1C_InitialValues[i];
    }

    MissingCode();

    if (apConfigArray[param_1].m1 == 0) {
        if (battleVar2->m0[param_1].m1C[9] == 0) {
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
        if (battleVar2->m0[param_1].m1C[9] == 0) {
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
            case 3:
                drawCircleMenuDefend(param_1);
                break;
            case 4:
                drawCircleMenuChi(param_1);
                break;
            case 7:
                drawBattleMenu7(param_1);
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

void playBattleSound(int) {
    MissingCode();
}

s8 newBattleInputButton2;
s8 previousNewBattleInputButton;

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
                        playBattleSound(0x4C);
                        newBattleInputButton = 0;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::DOWN) {
                        playBattleSound(0x4C);
                        newBattleInputButton = 1;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::LEFT) {
                        playBattleSound(0x4C);
                        newBattleInputButton = 2;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::UP) {
                        playBattleSound(0x4C);
                        newBattleInputButton = 3;
                        previousNewBattleInputButton = newBattleInputButton2;
                        newBattleInputButton2 = newBattleInputButton;
                        break;
                    }
                    else if (newPadButtonForField & controllerButtons::INTERACT) {
                        newBattleInputButton = 4;
                        playBattleSound(0x4d);
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::CROSS) {
                        newBattleInputButton = 5;
                        playBattleSound(0x4e);
                        break;
                    }
                    else if (newPadButtonForDialogs & controllerButtons::JUMP) {
                        newBattleInputButton = 6;
                        playBattleSound(0x4d);
                    }
                    else if (newPadButtonForDialogs & controllerButtons::TRIANGLE) {
                        newBattleInputButton = 7;
                        playBattleSound(0x4d);
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
                        playBattleSound(0x4d);
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