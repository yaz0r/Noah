#include "noahLib.h"
#include "battle.h"
#include "battleLoader.h"
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
#include "battle/spinningEnemySelectionCursor.h"
#include "battle/damageDisplay.h"
#include "battle/damageDisplay2.h"
#include "battle/enemyScript.h"
#include "battle/battleDialogWindow.h"
#include "field/dialogWindows.h"
#include "menus/menuHandler.h"
#include "kernel/TIM.h"
#include "kernel/audio/wds.h"
#include "kernel/audio/seq.h"
#include "kernel/audio/soundInstance.h"
#include "battle/battleEvent/battleEvent.h"
#include "battle/battleFader.h"

#include "psx/libgpu.h"
#include "psx/libapi.h"

#include "battle/menu_chi.h"

void updatePortraits();
void render_BA8_27C8_1E68(void);
void battleDrawAPBar();
void drawMonsterNames();
void freeMechaModelBlocks(sMechaInitVar4* param_1, int param_2);
u16 allPlayerCharacterBitmask = 0;
u8 battleInitVar0 = 0;
u8 battleInitVar1 = 0;
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

std::array<s8, 12> targetsPerPriority;

std::array<sBattleSpriteActor*, 11> battleSpriteActors;
std::array<sSpriteActorCore*, 11> battleSpriteActorCores;

sFont battleFont;

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
std::array<s8, 4> battleCharacters;
std::array<s8, 11> isBattleSlotFilled;

sMechaDataTable1* battleLoadDataVar0;
sLoadableDataRaw* battleLoadDataVar0_raw;
std::vector<u8>::iterator battleLoadDataVar1;
sBattleMechaInitData* battleLoadDataVar2;
sBattleMechaInitData* battleLoadDataVar2Bis;
sBattleMechaInitData* battleLoadDataVar2Ter;

std::array<s8, 11> isEntityReadyForBattle;
std::array<s8, 11> randomTurnOrder;
s8 currentEntryInRandomTurnOrder;

std::array<sBattleVisualBuffer, 11> battleVisualBuffers;
std::array<sBattleVisualEntity, 11> battleVisualEntities;
std::array<sBattleEntity, 11> battleEntities;

sGameStateA4* startBattleAttackAnimationVar5;
sGameStateA42* startBattleAttackAnimationVar6;

s32 backupCameraMode = 1;

bool battleSpritesDisabled = false;

extern sSoundInstance* pMusic;
sSoundInstance* pCurrentBattleMusic;

void jumpUpdatePositionSub0(sSpriteActorCore* param_1, sSpriteActorCore* param_2);
void jumpUpdatePosition(sSpriteActorCore* param_1);
void playBattleMusic(sSoundInstance* param_1, int param_2, int param_3);

extern int spriteCallback2Var4;

std::array<s8, 11> battleMonsterMapping;

ushort characterOdToInverseTargetBitmask(uint param_1)
{
    return ~bitmaskCharacter[param_1 & 0xff];
}

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
sWdsFile* loadWdsDataNeeded = nullptr;
u8 startJumpAnimationVar3 = 0;
u32 startReadingBattleJumpAnimationVar0 = 0;

void loadWdsDataIfNeeded(void)
{
    if (loadWdsDataNeeded != 0) {
        unloadWds(loadWdsDataNeeded);
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

sSeqFile battleStartSeq;
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

    for (int i = 0; i < 0x20; i++) {
        battleMechas[i] = nullptr;
    }


    MissingCode();
}

void loadBattleLoader() {
    resetMemoryAllocStats(2, 0);
    setCurrentDirectory(0xc, 0);

    Noah_MissingCode("Code to get the amount of free memory before battle loader");

    //battleStartSeq.resize(getFileSizeAligned(2));
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

    loadSequence(&battleStartSeq);

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

int waitForMusic(uint param_1) {
    return 0;
}

void initModel5(sModel* pModelBlock);

void mechaInitEnvironmentMechaMesh(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SVECTOR* param_9)
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
        if ((psVar9->mC != psVar9->m8_seq) && (iVar3 = findSequenceInLoadedList(psVar9->m8_seq, 0), iVar3 == 0)) {
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

    // HACK:
    // This should be global, but there is some weird weird side effect when mechas are loading with flag 0x5
    // as it creates battleMechaModelBlocksBufferForLoading but doesn't cleanup.
    // Later a mecha with flag with 0x2 set will skip over the battleMechaModelBlocksBufferForLoading init
    // but still use it in the (flags & 2) == 0 block, and then freeing it
    sModel* battleMechaModelBlocksBufferForLoading = nullptr;

    int battleMechaInitVar4Counter = 0;
    if ((flags & 1) == 0) {
        int uVar29 = 0;
        if ((flags & 0x40) == 0) {
            uVar29 = ((pLoadedMecha->m4A & 4) == 0);
        }

        // TODO: convert that properly
        std::span<u8> temp(pData1->m4_textures.m_raw.begin(), pData1->m4_textures.m_raw.size());
        uploadTextureToVram(temp.begin(), uVar29, tpageX, tpageY, uVar29, clutX, clutY);
        battleMechaModelBlocksBufferForLoading = new sModel;
        *battleMechaModelBlocksBufferForLoading = pData1->m8_modelBlocks;

        int iVar2;
        do {
            iVar2 = battleMechaInitVar4Counter + 1;
            if (battleMechaInitVar4[battleMechaInitVar4Counter].m0.size() == 0)
                break;
            battleMechaInitVar4Counter = iVar2;
        } while (iVar2 < 8);

        sMechaModel_init(*battleMechaModelBlocksBufferForLoading, &battleMechaInitVar4[battleMechaInitVar4Counter]);
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
        pLoadedMecha->mB0 = &pData2->m8;
        initMechaTransforms1(pLoadedMecha, &battleMechaInitVar2, &pData2->m4.m8, &pData2->m4.m4);
        initMechaAnimation(pLoadedMecha, pLoadedMecha, &battleMechaInitVar2, 0);
        MissingCode();
    }

    if ((flags & 2) == 0) {
        if(battleMechaModelBlocksBufferForLoading) // Hack, this should not be necessary
        {
            initModel5(battleMechaModelBlocksBufferForLoading);
            sModel* pNewModel = new sModel;
            *pNewModel = *battleMechaModelBlocksBufferForLoading;
            // Fixup
            for (int i = 0; i < pNewModel->m0_numBlocks; i++) {
                pNewModel->m10_blocks[i].m_baseItForRelocation = pNewModel->mRawData.data();
            }

            delete battleMechaModelBlocksBufferForLoading;
            battleMechaModelBlocksBufferForLoading = nullptr;
            freeMechaModelBlocks(pLoadedMecha->m0, 0);
            sMechaModel_init(*pNewModel, pLoadedMecha->m0);
            pLoadedMecha->mA8 = pNewModel;
        }
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
            switch (pBone->m52) {
            case 0:
                break;
            case 1:
                m1F800040.m[0][0] = 0x1000;
                m1F800040.m[0][2] = 0;
                m1F800040.m[1][0] = 0;
                m1F800040.m[1][2] = 0;
                m1F800040.m[2][0] = 0;
                m1F800040.m[2][2] = 0x1000;
                m1F800040.m[0][1] = matrix->m[0][1];
                m1F800040.m[1][1] = matrix->m[1][1];
                m1F800040.m[2][1] = matrix->m[2][1];
                break;
            case 2:
                m1F800040.m[0][0] = 0x1000;
                m1F800040.m[0][2] = 0;
                m1F800040.m[1][0] = 0;
                m1F800040.m[1][2] = 0;
                m1F800040.m[2][0] = 0;
                m1F800040.m[2][2] = 0x1000;
                m1F800040.m[0][1] = 0;
                m1F800040.m[1][1] = 0x1000;
                m1F800040.m[2][1] = 0;
                break;
            default:
                assert(0);

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
    battleMechaVar0 += 1;// +DAT_Battle__800ccc5c;
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
                    MissingCode();
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
    case 0:
        break;
    case 1:
        computeBattleCameraParams(allEntitiesToFitInView);
        break;
    case 2:
        battleCameraEyeTarget.vy = previousCameraEye2.vy.getIntegerPart();
        battleCameraEyeTarget.vx = previousCameraEye2.vx.getIntegerPart();
        battleCameraEyeTarget.vz = previousCameraEye2.vz.getIntegerPart();
        battleCameraAtTarget.vy = previousCameraAt2.vy.getIntegerPart();
        battleCameraAtTarget.vx = previousCameraAt2.vx.getIntegerPart();
        battleCameraAtTarget.vz = previousCameraAt2.vz.getIntegerPart();
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

void battleUpdateInputs_mode2() {
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
    MissingCode();
    if (battleVar1->mCA_battleEventPoly) {
        for (int i = 0; i < 0x10; i++) {
            if (battleEventVar0->m0_scriptEntities[i].m28) {
                battleEventVar0->m0_scriptEntities[i].m26--;
                if (battleEventVar0->m0_scriptEntities[i].m26 < 0) {
                    battleEventVar0->m0_scriptEntities[i].m26 = 0;
                }
            }
        }
    }

    do {
        if (battleVar1->mCF == 0) {
            battleInputButton = 0xFF;
        }
        if (getInputOverflowed() == 0) {
            while (loadInputFromVSyncBuffer()) {
                if (!battleIsPaused) {
                    if ((newPadButtonForDialogs & 0x20) != 0) {
                        battleInputButton = 4;
                    }
                }
                else if (*pRunningOnDTL != -1) {
                    assert(0);
                }
                else if (newPadButtonForDialogs & 0x800) {
                    if (battleTimeEnabled != '\0') {
                        if (battleIsPaused == '\0') {
                            assert(0);
                        }
                    }
                }
            }
        }
        else {
            resetInputs();
        }
    } while (battleIsPaused);
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
        battleUpdateInputs_mode2();
        break;
    default:
        assert(0);
    }
}

void drawDialogPortrait() {
    if (battleVar1->mC8_dialogPortrait) {
        AddPrim(&(*pCurrentBattleOT)[1], &battleEventVar0->m7A4[battleEventVar0->m7F4_battleDialogPortraitOddOrEven]);
    }
    if (battleVar1->mC9_dialog) {
        updateAndRenderTextForDialogWindow(battleEventVar1, &(*pCurrentBattleOT)[1], battleOddOrEven);
    }
}

void drawBattleMode2() {
    drawBattleDialogWindows();
    updatePortraits();
    render_BA8_27C8_1E68();
    drawMonsterNames();
    drawDialogPortrait();
    battleDrawAPBar();
    //FUN_Battle__80088b80();
    //battleDrawOverallUI();
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
        for (int i = 0; i < battleVar1->m97; i++) {
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->m908[i][battleVar1->m98]);
        }
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
        battleRenderPolyArray(&battleVar0->m9C8_APCounterDisplayPolys[0], battleVar1->m7B, battleVar1->mA4_oddOrEven7B);
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

int battleSetupStringInPolyFTRot(sFont& font, int param_2, std::array<POLY_FT4,2>* param_3, int param_4, short posX, short posY, short scaleX, short scaleY, ushort rotZ)
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

    for (int i = 0; i < font.m4[param_2].m0_polyCount; i++) {
        POLY_FT4* p = &param_3[i][param_4];
        SetPolyFT4(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 1);

        sFontGlyphPoly& poly = font.m4[param_2].m4_polys[i];
        p->tpage = GetTPage(poly.m10_colorDepth, 0, poly.m16_tpageX, poly.m18_tpageY);
        p->clut = GetClut(poly.m12_clutX, poly.m14_clutY);

        std::array<SVECTOR, 4> tempVectorArray;
        tempVectorArray.fill({ 0,0,0x1000,0x0 });

        tempVectorArray[3].vy = poly.mA_height;
        tempVectorArray[3].vx = poly.m8_width + poly.m4_textureWidth;
        tempVectorArray[2].vx = poly.m8_width;
        if (poly.m1A_flipX == 0) {
            tempVectorArray[2].vx = tempVectorArray[3].vx;
            tempVectorArray[3].vx = poly.m8_width;
        }
        tempVectorArray[1].vy = tempVectorArray[3].vy + poly.m6_textureHeight;
        if (poly.m1B_flipY == 0) {
            tempVectorArray[1].vy = tempVectorArray[3].vy;
            tempVectorArray[3].vy = tempVectorArray[3].vy + poly.m6_textureHeight;
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

        s16 uVar6 = poly.m0_textureU;
        s16 uVar7 = poly.m2_textureV;
        s16 cVar8 = poly.m4_textureWidth;
        s16 cVar9 = poly.m6_textureHeight;
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
    return font.m4[param_2].m0_polyCount;
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
        case 2: // Fuel
        {
            int fuelXRatio = (((battleEntities[i].mA4_gear.m38_fuel * 100) / battleEntities[i].mA4_gear.m3C_maxFuel) * 0x15E0) / 10000;

            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x0 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y0 = 0x22;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x1 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + fuelXRatio;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y1 = 0x22;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x2 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + 0x28;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y2 = 0x26;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].x3 = i * 0x60 + partyMemberSpritesOffset[battlePartyLayoutType][i] + fuelXRatio;
            battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].y3 = 0x26;
            if (battleEntities[i].mA4_gear.m38_fuel < battleEntities[i].mA4_gear.m3C_maxFuel / 4) {
                if (battleEntities[i].mA4_gear.m3C_maxFuel / 8 <= battleEntities[i].mA4_gear.m38_fuel) {
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r0 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g0 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b0 = '\0';
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r1 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g1 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b1 = '\0';
                }
                else {
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r0 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g0 = '\x7f';
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b0 = '\x7f';
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r1 = 0xff;
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g1 = '\x7f';
                    battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b1 = '\x7f';
                }
            }
            else {
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r0 = '\0';
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g0 = '\0';
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b0 = 0xff;
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].r1 = '\0';
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].g1 = '\0';
                battleVar0->m740_APOrFuelPoly[i][battleOddOrEven].b1 = 0xff;
            }
        }
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
            auto* pWindows = battleDialogWindows[i];
            battleRenderPolyArray(&pWindows->m0_corners[0], battleDialogWindows[i]->m5A0, pWindows->m5A4_oddOrEven);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m280[0][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m280[1][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m320[0][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m320[1][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m140[0][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m140[1][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m1E0[0][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m1E0[1][pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m3C0_polyG4[pWindows->m5A4_oddOrEven]);
            AddPrim(&(*pCurrentBattleOT)[1], &pWindows->m408_drawMode[pWindows->m5A4_oddOrEven]);
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
        currentHP = battleEntities[param_1].mA4_gear.m38_fuel;
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
    currentEntityGearMaxHpForDisplay = battleEntities[param_1].mA4_gear.m60_hp;
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

void renderOpenMenuSub0(void) {
    if (battleVar0->mA230->m669_drawEPCost) {
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0][battleVar0->mA230->m66C]);
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[1][battleVar0->mA230->m66C]);
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[2][battleVar0->mA230->m66C]);
        AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[3][battleVar0->mA230->m66C]);
    }
}

void drawDescriptionText(void)
{
    AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[8][battleVar0->mA230->m66A_oddOrEven]);
    AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[9][battleVar0->mA230->m66A_oddOrEven]);
}

void renderOpenMenu() {
    switch (battleVar1->mB7) {
    case 0:
        break;
    case 1:
        renderOpenMenuSub0();
        if (battleVar0->mA230->m669_drawEPCost) {
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0xd][battleVar0->mA230->m66C]);
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0xe][battleVar0->mA230->m66C]);
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0xf][battleVar0->mA230->m66C]);
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0x10][battleVar0->mA230->m66C]);
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0x11][battleVar0->mA230->m66C]);
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m5A0[0][battleVar0->mA230->m66C]);
        }
        if (battleVar0->mA230->m66B) {
            if (battleVar0->mA230->m66E) {
                AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[0xc][battleVar0->mA230->m66C]);
            }
            AddPrim(&(*pCurrentBattleOT)[1], &battleVar0->mA230->m0_polys[4][battleVar0->mA230->m66A_oddOrEven]);
            battleRenderPolyArray(&battleVar0->mA230->m0_polys[5], battleVar0->mA230->m66E, battleVar0->mA230->m66A_oddOrEven);
            drawDescriptionText();
            drawDescriptionText();
        }
        break;
    default:
        assert(0);
        break;
    }
}

void repositionTextRenderingPlanePrim(POLY_FT4* param_1, short x, short y, u8 u, u8 v, byte width)
{
    param_1->x0 = x;
    param_1->y0 = y;
    param_1->y1 = y;
    param_1->x2 = x;
    param_1->y2 = y + 0xd;
    param_1->y3 = y + 0xd;
    param_1->u0 = u;
    param_1->u2 = u;
    param_1->x1 = x + (ushort)width;
    param_1->x3 = x + (ushort)width;
    param_1->v0 = v;
    param_1->u1 = u + width;
    param_1->v1 = v;
    param_1->v2 = v + '\r';
    param_1->u3 = u + width;
    param_1->v3 = v + '\r';
    return;
}

struct sMonsterTurnSub {
    std::array<POLY_FT4, 2> m0_polys;
    RECT m50_monsterNameVRamRect;
    sRamTexture* m58_ramBufferFromMonsterName;
    s8 m5C;
    s8 m5D_monsterNameIsDisplayed;
    s8 m5E_currentMonsterNameStringWidth;
    // size 0x60
};

struct sMonsterTurnDisplay {
    s8 m0;
    std::array<sMonsterTurnSub, 8> m8;
    // size 0x300
};

sMonsterTurnDisplay previousEntityTurn;

void drawMonsterNames() {
    for (int i = 0; i < 8; i++) {
        if (previousEntityTurn.m8[i].m5D_monsterNameIsDisplayed) {
            if (i == 0) {
                int x = 0x40 - (ushort)(previousEntityTurn.m8[i].m5E_currentMonsterNameStringWidth >> 1);
                repositionTextRenderingPlanePrim(&previousEntityTurn.m8[i].m0_polys[battleOddOrEven], x, 0x2C, 0, 0, previousEntityTurn.m8[i].m5E_currentMonsterNameStringWidth);
            }
            else {
                int x = 0x9A - (ushort)(previousEntityTurn.m8[i].m5E_currentMonsterNameStringWidth >> 1);
                repositionTextRenderingPlanePrim(&previousEntityTurn.m8[i].m0_polys[battleOddOrEven], x, 0xCA, 0, (i / 2) * 0xD , previousEntityTurn.m8[i].m5E_currentMonsterNameStringWidth);
            }
            AddPrim(&(*pCurrentBattleOT)[1], &previousEntityTurn.m8[i].m0_polys[battleOddOrEven]);
        }
    }
}

void render_BA8_27C8_1E68(void)
{
    if (battleVar1->m9C_renderBA8 != '\0') {
        battleRenderPolyArray(&battleVar0->mBA8[0], battleVar1->mF8_countOfBA8, battleVar1->mA5_ba8OddOrEven);
    }
    if (battleVar1->m9E_render27C8 != '\0') {
        battleRenderPolyArray(&battleVar0->m27C8[0], battleVar1->m100_countOf27C8, battleVar1->mA7_27C8OddOrEven);
    }
    if (battleVar1->m9D_render1E68 != '\0') {
        battleRenderPolyArray(&battleVar0->m1E68[0], battleVar1->mFC_countOF1E68, battleVar1->mA6_1E68OddOrEven);
    }
    return;
}

void drawBattleMode1() {
    if (!drawBattleMode1Disabled) {
        drawBattleDialogWindows();
        updatePolyArray3A88();
        renderGearHP();
        updatePortraits();
        battleRenderPortraitSelection();
        battleRenderPlayerPortraits();
        battleRenderCommandRing();
        render_BA8_27C8_1E68();
        drawMonsterNames();
        MissingCode();
        renderOpenMenu();
        renderTargetSelectionCursor();
        battleDrawAPBar();
        MissingCode();
    }
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
    execSpritesCallbacksList1();
    execSpritesCallbacksList2();
    MissingCode();
    /*
    int iVar3 = (int)spriteCallback2Var4;
    while (iVar3 = iVar3 + -1, iVar3 != -1) {
        updateBattleCamera();
        execSpritesCallbacksList2();
    }*/

    drawBattleFromModes();
    battleUpdateInputs(0);
    MissingCode();
    /*
    while (spriteCallback2Var4 = spriteCallback2Var4 + -1, spriteCallback2Var4 != -1) {
        battleUpdateInputs(1);
    }*/
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
    for (s8 i = 0; i < 3; i++) {
        if (isBattleSlotFilled[i]) {
            if ((battleEntities[i].m0_base.m7C & 0xC000) == 0) {
                if (battleVisualEntities[i].m3 == 0) {
                    allPlayerCharacterBitmask |= characterIdToTargetBitmask(i);
                }
            }
            else {
                if ((battleEntities[i].m0_base.m7C & 0x8000) == 0) {
                    battleEntities[i].m0_base.m4C_HP = 0;
                }
                isEntityReadyForBattle[i] = -1;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (isBattleSlotFilled[i + 3]) {
            if (battleVisualEntities[i].m4_isGear == 0) {
                if (battleEntities[i].m0_base.m7C & 0xC000) {
                    if (bitmaskCharacterCheck(jumpAnimationActiveActorBF, i + 3)) {
                        allPlayerCharacterBitmask |= characterIdToTargetBitmask(i + 3);
                    }
                    else {
                        battleEntities[i + 3].m0_base.m4C_HP = 0;
                        isEntityReadyForBattle[i + 3] = -1;
                    }
                }
                else {
                    if ((battleVisualEntities[i + 3].m3) || (unknownMonsterStatus0[i].m3)) {
                        continue;
                    }
                    else {
                        allPlayerCharacterBitmask |= characterIdToTargetBitmask(i + 3);
                    }
                }
            }
            else {
                if (battleEntities[i].mA4_gear.m7C & 0xC000) {
                    if (bitmaskCharacterCheck(jumpAnimationActiveActorBF, i + 3)) {
                        allPlayerCharacterBitmask |= characterIdToTargetBitmask(i + 3);
                    }
                    else {
                        battleEntities[i + 3].mA4_gear.m60_hp = 0;
                        isEntityReadyForBattle[i + 3] = -1;
                    }
                }
                else {
                    if (battleVisualEntities[i + 3].m3 || unknownMonsterStatus0[i].m3) {
                        continue;
                    }
                    else {
                        allPlayerCharacterBitmask |= characterIdToTargetBitmask(i + 3);
                    }
                }
            }
        }
    }

    if ((allPlayerCharacterBitmask & 0x7f8) == 0) {
        battleRunningVar1 = '\x01';
    }
    if ((allPlayerCharacterBitmask & 7) == 0) {
        battleRunningVar1 = -0x7f;
    }

    if (battleRunningVar1 == '\0') {
        u32 uVar2 = allPlayerCharacterBitmask;
        for (int i = 0; i < 11; i++) {
            if (bitmaskCharacterCheck(uVar2, i)) {
                if (battleEntities[i].m0_base.m80 & 0x1000) {
                    uVar2 &= characterOdToInverseTargetBitmask(i);
                }
            }
        }

        if (uVar2 == 0) {
            for (int i = 0; i < 11; i++) {
                if (bitmaskCharacterCheck(allPlayerCharacterBitmask, i)) {
                    if (battleEntities[i].m0_base.m80 & 0x1000) {
                        battleEntities[i].m0_base.m80 &= ~0x1000;
                        return;
                    }
                }
            }
        }
    }
}

void initBattle3dRendering(void) {
    MissingCode();
    gDepthDivider = 2;
    MissingCode();
}

sSavePointMeshAbstract* battleCameraModeCallback1 = nullptr;
sSavePointMeshAbstract* battleCameraModeCallback2 = nullptr;

void setBattleCameraMode(int param_1) {
    battleCameraModeInterpolation = 1;
    battleCameraMode = param_1;

    switch (param_1)
    {
    case 2:
        previousCameraEye2.vx = (int)battleCameraEyeTarget.vx << 0x10;
        previousCameraEye2.vy = (int)battleCameraEyeTarget.vy << 0x10;
        previousCameraEye2.vz = (int)battleCameraEyeTarget.vz << 0x10;
        previousCameraAt2.vx = (int)battleCameraAtTarget.vx << 0x10;
        previousCameraAt2.vy = (int)battleCameraAtTarget.vy << 0x10;
        previousCameraAt2.vz = (int)battleCameraAtTarget.vz << 0x10;
        break;
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

int JumpAnimationStuffVar0;
s16 processBattleAnimationSub0_var0 = 0;
void initJumpAnimationStuff(void)
{
    JumpAnimationStuffVar0 = 0;
    jumpAnimationControlStruct = (sJumpAnimationControlStruct*)0x0;
    processBattleAnimationSub0_var0 = 0;
    return;
}
s16 updateTargetSelectionMarkerSub0_var0;

void initUpdateTargetSelectionMarkerSub0_var0(void)
{
    updateTargetSelectionMarkerSub0_var0 = 0;
    return;
}

void initBattleSpriteSystem() {
    isBattleOverlayLoaded = 1;
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 0;
    battleDefaultEntityScale = 0x2000;
    initJumpAnimationStuff();
    initBattleUnkData0();
    resetSpriteCallbacks();
    resetBattleCameraMode();
    allocateShapeTransfert(0x5000);
    initUpdateTargetSelectionMarkerSub0_var0();
    MissingCode();
    objectClippingMask = 0;
}

extern sWdsFile* wdsFile5;
void initBattleGraphics(sBattleSpriteConfigs* param_1) {
    initBattle3dRendering();
    initBattleSpriteSystem();
    createBattleSpriteLoadingTask(param_1);
    unloadWds(wdsFile5);
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

std::array<u8, 5> battleTickMain_var3;
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

s16 entitiesHitInCurrentAttackBF;

void updateBattleEntityStatus(int index) {
    MissingCode();
}

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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemHighlighted;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_EscapeHighlighted;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
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
    battleVar1->m7B = 0;
    battleVar1->m7B += battleSetupStringInPolyFT4Large(battleVar2->m2D4_remainingAP + 0xF, &battleVar0->m9C8_APCounterDisplayPolys[battleVar1->m7B], 0x2A, 0xD0);
    battleVar1->m7B += battleSetupStringInPolyFT4Large(0x19, &battleVar0->m9C8_APCounterDisplayPolys[battleVar1->m7B], 0x32, 0xD0);
    battleVar1->m7B += battleSetupStringInPolyFT4Large(battleVar2->m2D5_maxAP + 0xF, &battleVar0->m9C8_APCounterDisplayPolys[battleVar1->m7B], 0x3A, 0xD0);
    battleVar1->mA4_oddOrEven7B = battleOddOrEven;
}

std::array<sBattleVar48, 32> battleCurrentDamages;

void initAnimSeqFromCharacterToCharacter(u8 entity, u8 target) {
    for (int i = 0; i < 0x20; i++) {
        battleCurrentDamages[i].m47_battleAnimationToPlay = -1;
        battleCurrentDamages[i].m23_battleEntityIndex = entity;
        battleCurrentDamages[i].m16_targetBitMask = characterIdToTargetBitmask(target);
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

    // HACK this would get out of bound for enemies
    if(entity < battleCharacters.size())
        return battleCharacters[entity] == 4; // return if billy
    return 0;
}


uint computeFacingForJump(uint param_1)
{
    uint uVar1;
    uint uVar2;
    ushort uVar3;
    uint in_t0;

    uVar3 = 0xff;
    uVar2 = (uint)currentEntryInRandomTurnOrder;
    do {
        uVar1 = (uint)randomTurnOrder[uVar2];
        uVar2 = uVar2 + 1;
        if ((numTicksBeforeReady[uVar1] < (short)uVar3) && (uVar1 != (param_1 & 0xff))) {
            uVar3 = numTicksBeforeReady[uVar1];
            in_t0 = uVar1;
        }
        if (uVar2 == 0xb) {
            uVar2 = 0;
        }
    } while (uVar2 != currentEntryInRandomTurnOrder);
    return in_t0;
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
        if ((param_2->mAC.mx18) != '\x15') {
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

sSpriteActorCore* needFxFragments = nullptr;
int isInJumpAnimationCallbackCode = 0;
std::array<sSpriteActorCore*, 11> pSpriteCoreListHead;

void setNeedFxFragments(sSpriteActorCore* param_1)
{
    needFxFragments = param_1;
    return;
}

int abilityPlayAnimationVar0 = 0;

void abilityPlayAnimationSub0(sSpriteActorCore*)
{
    abilityPlayAnimationVar0 = abilityPlayAnimationVar0 + 1;
    return;
}

void clearBattleAnimationData() {
    MissingCode();
    waitBattleAnimationSpriteLoading();
}

byte loadingFXFramentVar0 = 0;
byte loadingFXFramentVar1 = 0;

sSpriteActorAnimationBundle* FxFragmentSpecialAnimation = nullptr;
sSpriteActorAnimationBundle* battleAnimationLoadingDest = nullptr;
std::vector<u8> fxUploadBuffer;

void abilityPlayAnimationSub1(int param_1) {
    if (param_1 == 0xe3) {
        MissingCode();
    }
    clearBattleAnimationData();
    setCurrentDirectory(0xc, 2);
    int iVar5 = param_1 * 2 + 0x22;
    loadingFXFramentVar0 = 1;
    int iVar6 = param_1 * 2 + 0x23;
    loadingFXFramentVar1 = 1;
    FxFragmentSpecialAnimation = new sSpriteActorAnimationBundle;
    readFile(iVar5, *FxFragmentSpecialAnimation, 0, 0x80);
    battleIdleDuringLoading();

    int hasExtraData = (READ_LE_U16(FxFragmentSpecialAnimation->m_rawData.begin() + READ_LE_U32(FxFragmentSpecialAnimation->m_rawData.begin() + 4)) >> 0xC) & 3;
    if (hasExtraData) {
        assert(0);
    }
    fxUploadBuffer = allocateBufferForVramUpload(8);
    if (getFileSizeAligned(iVar6) > 0x10) {
        loadImageFileToVram(iVar6, fxUploadBuffer.data(), 0, 0, 0, 0, 0, 0, 0, 0);
    }
    battleIdleDuringLoading();
    DrawSync(0);
    battleAnimationLoadingDest = FxFragmentSpecialAnimation;
    fxUploadBuffer.clear();
}

void abilityPlayAnimation(int param_1, sSpriteActorCore* param_2)
{
    if (param_2->m48_defaultAnimationbundle == nullptr) {
        abilityPlayAnimationSub1(param_1);
    }
    else {
        abilityPlayAnimationVar0 = 0;
        if (param_2->mAC.mx18 != '\0') {
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_2, abilityPlayAnimationSub0);
            spriteActorSetPlayingAnimation(param_2, param_2->mAC.mx18);
        }
        abilityPlayAnimationSub1(param_1);
        if ((param_2->mAC.mx18) != '\0') {
            while (abilityPlayAnimationVar0 == 0) {
                battleRender();
            }
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_2, 0);
        }
    }
    return;
}

sSpriteActorCore* processBattleAnimationSub0_var1 = nullptr;

void setRenderingInfoForItemUser(sSpriteActorCore* param_1) {
    processBattleAnimationSub0_var1 = param_1;
}

sSpriteActorCore* getSpriteCoreForAnimation(int param_1) {
    if (jumpAnimationControlStruct->m4 && (jumpAnimationControlStruct->m24 != param_1) && (battleVisualEntities[(jumpAnimationControlStruct->m4->mAC.mx0_entityIdUpper2bit) << 2 | jumpAnimationControlStruct->m4->mA8.mx1E_entityId_bottom2bit].m3 == 0)) {
        spriteActorSetPlayingAnimation(jumpAnimationControlStruct->m4, jumpAnimationControlStruct->m4->mB0.mx0_animationId);
    }
    jumpAnimationControlStruct->m24 = param_1;
    jumpAnimationControlStruct->m4 = battleSpriteActorCores[param_1];
    return jumpAnimationControlStruct->m4;
}

int countBattleActorCoresActiveForBitmask(uint bitmask, std::array<sSpriteActorCore*, 11>& param_2, sSpriteActorCore* param_3) {
    sBattleMechaInitData** ppsVar2;
    int iVar4;

    iVar4 = 0;
    ppsVar2 = &battleLoadDataVar2Ter;
    std::array<sSpriteActorCore*, 11>::iterator ppsVar1 = param_2.begin();
    for(int i=0; i<11; i++) {
        if ((bitmask & 1) && battleSpriteActorCores[i]) {
            battleSpriteActorCores[i]->m74_pTargetEntitySprite = param_3;
            *ppsVar1 = battleSpriteActorCores[i];
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
            if ((jumpAnimationActiveActorBF >> ((pSlotEntry->mAC.mx0_entityIdUpper2bit) << 2 | (uint)pSlotEntry->mA8.mx1E_entityId_bottom2bit) & 1U) == 0) {

                // start playing death animation

                pSlotEntry->m9E_wait = 0;
                int entityId = (pSlotEntry->mAC.mx0_entityIdUpper2bit) << 2 | (uint)pSlotEntry->mA8.mx1E_entityId_bottom2bit;
                if (battleVisualEntities[entityId].m4_isGear == 0) {
                    if (((int)pSlotEntry->mAC.mx18) != '\x15') {
                        spriteActorSetPlayingAnimation(pSlotEntry, 0x15); // death animation
                    }
                }
                else {
                    hasPlayedAnimationOnMecha = true;
                    battleMechas[entityId]->m38 = 1;
                    entityId = (pSlotEntry->mAC.mx0_entityIdUpper2bit) << 2 | (uint)pSlotEntry->mA8.mx1E_entityId_bottom2bit;
                    performMechaPlayAnimation(entityId, entityId, 0x15);
                }
                count++;
                processBattleAnimationSub0_var0 |= 1 << ((pSlotEntry->mAC.mx0_entityIdUpper2bit) << 2 | (uint)pSlotEntry->mA8.mx1E_entityId_bottom2bit);
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

s8 fxFragmentLoaded = 0;
s16 attackInProgress = 0;

s16 delayBattleAnimationCounter = 0;

void loadEffectFragmentsAndAudioModel(sSpriteActorAnimationBundle& buffer, u32 offset) {
    MissingCode();
}

std::optional<std::vector<u8>::iterator> loadEffectFragmentsAndAudio(sSpriteActorAnimationBundle& buffer) {
    std::optional<std::vector<u8>::iterator> pWds;
    int numEntries = READ_LE_U32(buffer.m_rawData.begin());
    for (int i = 3; i< numEntries; i++) {
        u32 offset = READ_LE_U32(buffer.m_rawData.begin() + 0x4 + 4 * i);
        std::vector<u8>::iterator entry = buffer.m_rawData.begin() + offset;
        u32 magic = READ_LE_U32(entry);
        if (magic == ' sdw') {
            loadWdsDataIfNeeded();
            battleAnimationSoundLoaded = 0;
            startJumpAnimationVar3 = 0;
            sWdsFile tempWds;
            tempWds.init(entry, buffer.m_rawData.size() - offset);
            loadWdsDataNeeded = loadWds(tempWds, 0);
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
            loadSequence(&buffer.m4_entriesAsSequences[i]);
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
        fragmentPtr = loadEffectFragmentsAndAudio(*battleAnimationLoadingDest);
        fxFragmentLoaded = 1;
    }
    return fragmentPtr;
}

void processBattleAnimation_subType_minus6(uint param_1) {
    battleVar1->mB0_isDialogWindowInitialized[4] = 1;
    previousEntityTurn.m8[param_1].m5D_monsterNameIsDisplayed = 1;
}

void processBattleAnimation_subType(sSpriteActorCore* param_1, int param_2) {
    switch (param_2) {
    case -13:
        jumpAnimationControlStruct->m48 = 1;
        jumpAnimationActiveActorBF |= battleCurrentDamages[allocateJumpAnimationStructVar0].m3A;
        break;
    case -10:
        setCameraVisibleEntities(battleCurrentDamages[allocateJumpAnimationStructVar0].m16_targetBitMask);
        break;
    case -9:
        jumpAnimationControlStruct->m48 = 1;
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1, 0);
        delayBattleAnimationCounter = battleCurrentDamages[allocateJumpAnimationStructVar0].m3A;
        break;
    case -6:
        jumpAnimationControlStruct->m48 = 1;
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1, 0);
        processBattleAnimation_subType_minus6(battleCurrentDamages[allocateJumpAnimationStructVar0].m3A);
        break;
    default:
        assert(0);
        break;
    }
}

const std::array<s16, 12> abilityAnimationTable0 = { {
     0x0, 0x16,    0x2F,    0x4A,
    0x5E, 0x71,    0x7C,    0x8F,
    0x97, 0x2F,    0x7C,     0x0
} };

const std::array<s16, 12> abilityAnimationTable1 = { {
     0x6,    0x1A,    0x37,    0x4E,
    0x61,    0x74,    0x83,    0x8F,
    0x97,    0x37,    0x83,     0x0
} };

void processBattleAnimation(sSpriteActorCore* param_1) {
    if (delayBattleAnimationCounter != 0) {
        delayBattleAnimationCounter = delayBattleAnimationCounter + -1;
        return;
    }

    s32 battleAnimationToPlay = battleCurrentDamages[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay;
    switch (battleAnimationToPlay) {
    case -13:
    case -10:
    case -9:
    case -6:
        processBattleAnimation_subType(param_1, battleAnimationToPlay);
        break;

    case -1: // waiting for attack prompt
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1, 0);
        jumpAnimationControlStruct->m48 = 1;
        if (processBattleAnimationSub0() == 0) {
            if (param_1->mA8.mx1C != 0) {
                return;
            }
            if (battleVisualEntities[(param_1->mAC.mx0_entityIdUpper2bit) << 2 | param_1->mA8.mx1E_entityId_bottom2bit].m3 != 0) {
                return;
            }
            spriteActorSetPlayingAnimation(param_1, param_1->mB0.mx0_animationId);
            return;
        }
        assert(0);
    case -2: // jump back to initial position at end of turn
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
    case -3:
        jumpAnimationControlStruct->m48 = 1;
        if (battleCurrentDamages[allocateJumpAnimationStructVar0].m3A == 0) {
            jumpAnimationControlStruct->m48 = 0;
            jumpUpdatePosition(param_1);
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1, startJumpAnimationCallback);
        }
        allocateJumpAnimationStructVar0 = allocateJumpAnimationStructVar0 + 1;
        return;
    //start attack animation
    case 4: // 1ap
    case 5: // 2ap
    case 6: // 3ap
    case 17:
    case 24: // healing ability?
    case 29:
    case 30:
    case 31:
        attackInProgress = 0;
        setRenderingInfoForItemUser(param_1);
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1, startJumpAnimationCallback);
        (param_1->m7C)->m8 =
            (uint)battleCurrentDamages[allocateJumpAnimationStructVar0].m18_damageType
            [(param_1->m74_pTargetEntitySprite->mAC.mx0_entityIdUpper2bit) << 2 | param_1->m74_pTargetEntitySprite->mA8.mx1E_entityId_bottom2bit];
        if (!isVramPreBacked((param_1->m24_vramData)->m0_spriteData)) {
            if (battleAnimationToPlay < 0x10) {
                if ((fxFragmentLoaded == '\0') && (battleAnimationLoadingDest != nullptr)) {
                    battleIdleDuringLoading();
                    (param_1)->m50 = loadFragmentIsNotLoadedYet();
                    setupSpecialAnimation(param_1, battleAnimationLoadingDest);
                }
                int expectActorId = (((param_1)->mAC.mx0_entityIdUpper2bit) << 2 | (uint)(param_1)->mA8.mx1E_entityId_bottom2bit);
                if (jumpAnimationControlStruct->m20_actor == expectActorId) {
                    spriteActorSetPlayingAnimation(param_1, ~battleAnimationToPlay);
                }
                else {
                    spriteActorSetPlayingAnimation(param_1, battleAnimationToPlay);
                }
            }
            else {
                // play ability animation
                int iVar6 = (battleAnimationToPlay - 0x10) + abilityAnimationTable0[(uint)battleVisualEntities[(param_1->mAC.mx0_entityIdUpper2bit) << 2 | param_1->mA8.mx1E_entityId_bottom2bit].m2 * 2];
                int uVar3 = 0x1C;
                if (abilityAnimationTable1[battleVisualEntities[(param_1->mAC.mx0_entityIdUpper2bit) << 2 | param_1->mA8.mx1E_entityId_bottom2bit].m2 * 2] <= iVar6) {
                    uVar3 = 0x11;
                }
                param_1->mAC.mx18 = uVar3;
                abilityPlayAnimation(iVar6, param_1);
                setNeedFxFragments(param_1);
            }
        }
        else {
            spriteActorSetPlayingAnimation(param_1, battleCurrentDamages[allocateJumpAnimationStructVar0].m47_battleAnimationToPlay);
            sSpriteActorCore* psVar10 = param_1->m74_pTargetEntitySprite;
            computeBattleCameraParams
            (1 << ((param_1->mAC.mx0_entityIdUpper2bit) << 2 |
                (uint)param_1->mA8.mx1E_entityId_bottom2bit) |
                1 << ((psVar10->mAC.mx0_entityIdUpper2bit) << 2 | (uint)psVar10->mA8.mx1E_entityId_bottom2bit));
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
    ((uint)(byte)battleCurrentDamages[allocateJumpAnimationStructVar0].m23_battleEntityIndex);
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
    if ((psVar2->mAC.mx18) != '\x15') {
        sVar1 = (ushort)(battleVisualEntities[param_1].m6_direction != 0) << 0xb;
        setSpriteActorAngle(psVar2, sVar1);
        OP_INIT_ENTITY_SCRIPT_sub0Sub7(psVar2, sVar1);
    }
}

void deleteJumpAnimationControlStruct(void)
{
    delete jumpAnimationControlStruct;
    allocateSavePointMeshDataSub0_var0 = 0;
    jumpAnimationControlStruct = nullptr;
    spriteBytecode2ExtendedE0_Var0 = 0;
    return;
}

void setBattleCameraParamX(short param_1)
{
    battleCameraParamsVar0.vx = param_1;
    return;
}

void freeBattleVar1_96() {
    if (battleVar1->m96) {
        assert(0);
    }
}

struct sBattleRenderStruct2 {
    s16 m5D9C;
    s16 m5D9E;
    // size 0x5DA4
};

sBattleRenderStruct2* pBattleRenderStruct2 = nullptr;

void freeBattleRenderStruct2(u32 param_1) {
    battleVar1->mAD = 0;
    battleVar1->mC7 = 0;
    battleVar1->mA8 = 0;
    if (apConfigArray[param_1 & 0xff].m1 == 2) {
        apConfigArray[param_1 & 0xff].m1 = 1;
    }
    delete pBattleRenderStruct2;
}

void freeBillyBattleStructs(u32 param_1) {
    if (battleCharacters[param_1 & 0xff] == 4) {
        assert(0);
    }
}

u8 battleVar800c204c = 0;

void freePlayerTurnStructs(void) {
    battleVar2->m2DB = 1;
    if ((battleVar2->m2D3_currentEntityTurn < 3) && (battleVar800c204c == '\0')) {
        freeBattleVar1_AE();
        freeBattleVar1_96();
        freeBattleRenderStruct2(battleVar2->m2D3_currentEntityTurn);
        freeBillyBattleStructs(battleVar2->m2D3_currentEntityTurn);
    }
}

void clearFragmentSeq(sSpriteActorAnimationBundle* pBundle) {
    for (int i = 3; i < pBundle->m0_numEntries; i++) {
        u32 offset = READ_LE_U32(pBundle->m_rawData.begin() + 0x4 + 4 * i);
        std::vector<u8>::iterator entry = pBundle->m_rawData.begin() + offset;
        u32 magic = READ_LE_U32(entry);
        if (magic == 'sdes') {
            unloadSequence(&pBundle->m4_entriesAsSequences[i]);
        }
    }
}

void clearFxFragment(void)
{
    if (fxFragmentLoaded != '\0') {
        clearFragmentSeq(battleAnimationLoadingDest);
        fxFragmentLoaded = '\0';
    }
    return;
}

void waitBattleAnimationSpriteLoading() {
    battleIdleDuringLoading();
    updateBattleAnimationDataLoading();
    while (allocateSavePointMeshDataSub0_var0 !=isDamageDisplayPolysTask2Running()) {
        battleRender();
    }
    clearFxFragment();
    if (battleAnimationLoadingDest) {
        delete battleAnimationLoadingDest;
        battleAnimationLoadingDest = nullptr;
    }
}

void startJumpAnimationSub0(int param_1)
{
    backupCameraMode = param_1;
}


void deleteJumpAnimationControlStructEndOfAttack() {
    if (jumpAnimationControlStruct != nullptr) {
        startJumpAnimationVar1 = 0;
        startJumpAnimationVar2 = 0;
        startJumpAnimationVar3 = 0;
        freePlayerTurnStructs();
        DrawSync(0);
        auto uVar5 = jumpAnimationControlStruct->m40_facing;
        OP_INIT_ENTITY_SCRIPT_sub0Sub8(jumpAnimationControlStruct->m4, 0);
        processBattleAnimationSub0();
        while (isDamageDisplayPolysTask2Running() != allocateSavePointMeshDataSub0_var0)
        {
            battleRender();
        }

        MissingCode();
        clearBattleAnimationData();
        startJumpAnimationSub0(1);
        MissingCode();
        freeDamageDisplayPolysTask2();
        updateBattleAnimationDataLoading();
        MissingCode();

        setBattleCameraParamX(0xc0);
        deleteJumpAnimationControlStruct();

        MissingCode();
    }
}

byte currentFileFrom0xC = 0;
std::vector<u8> fragmentFile;

void battleLoadFileFrom0xC(void)
{
    uint uVar1;
    int local_10;
    int local_c;

    uVar1 = (uint)initFieldVar5;
    if (currentFileFrom0xC != uVar1) {
        currentFileFrom0xC = initFieldVar5;
        battleIdleDuringLoading();
        getCurrentDirectory(&local_10, &local_c);
        setCurrentDirectory(0xc, 2);
        readFile(uVar1 + 2, fragmentFile, 0, 0x80);
        battleIdleDuringLoading();
        setCurrentDirectory(local_10, local_c);
        DrawSync(0);
        VSync(0);
        EnterCriticalSection();
        FlushCache();
        ExitCriticalSection();
    }
    initFieldVar4 = 1;
    return;
}

bool initFxFragments(void)
{
    int iVar2;
    sFieldEntitySub4_110* psVar3;
    sTaskHeader sStack_60;
    sVec2_s16 local_3c;
    sVec2_s16 local_38;
    byte* local_34;
    byte* local_30;

    allocateSavePointMeshDataSub0(nullptr, &sStack_60); // TODO: missing custom task initFxFragmentTask here, but seems unused?
    sStack_60.m8_updateCallback = 0;
    battleIdleDuringLoading();
    startJumpAnimationVar1 = 0;
    initFieldEntitySub4Sub5Sub0(&sFieldEntitySub4_110_8005a474, FxFragmentSpecialAnimation, sVec2_s16::fromValue(0x380, 0x100), sVec2_s16::fromValue(0x0, 0x1F4));
    battleLoadFileFrom0xC();
    bool isVramPrebacked = isVramPreBacked(sFieldEntitySub4_110_8005a474.m0_spriteData);
    if (!isVramPrebacked) {
        setupSpecialAnimation(processBattleAnimationSub0_var1, FxFragmentSpecialAnimation);
        spriteActorSetPlayingAnimation(processBattleAnimationSub0_var1, -1);
    }
    else {
        psVar3 = processBattleAnimationSub0_var1->m24_vramData;
        //auto local_40 = psVar3->m0_spriteData;
        //local_3c = psVar3->m4_vramLocation;
        //local_38 = psVar3->m8_clut;
        //local_34 = psVar3->mC;
        //local_30 = psVar3->m10_startOfAnimationContainer;
        std::span<u8>::iterator psVar4 = sFieldEntitySub4_110_8005a474.m10_startOfAnimationContainer->getAnimation(startJumpAnimationVar2);
        spriteBytecode2ExtendedE0(processBattleAnimationSub0_var1, psVar4, &sFieldEntitySub4_110_8005a474);
    }
    processBattleAnimationSub0_var1->m50 = loadEffectFragmentsAndAudio(*FxFragmentSpecialAnimation);
    MissingCode(); // Stuff are pretty weird here, not sure what this is trying to do
    //psVar4->m50 = (int)puVar5;
    //psVar1->m50 = (int)puVar5;
    fxFragmentLoaded = 1;
    //DAT_Battle__800c35d4 = 1;
    playBattleMusic(pCurrentBattleMusic, 0x60, 0x78);
    MissingCode();
    allocateSavePointMeshDataSub0_callback(&sStack_60);
    return !isVramPrebacked;
}

byte allocateJumpAnimationStructCallbackSpecial0 = 0;
void allocateJumpAnimationStructCallback(sJumpAnimationControlStruct* param_1) {
    if (isInJumpAnimationCallbackCode) {
        return;
    }
    isInJumpAnimationCallbackCode = 1;
    jumpAnimationControlStruct = param_1;
    if (needFxFragments != 0) {
        needFxFragments = nullptr;
        if (!initFxFragments()) {
            if (((param_1->m4->mAC.mx0_entityIdUpper2bit) << 2 | (param_1->m4->mA8.mx1E_entityId_bottom2bit)) < 3) {
                int uVar5;
                if (allocateJumpAnimationStructCallbackSpecial0 == '\0') {
                    uVar5 = 0x12;
                }
                else {
                    uVar5 = 0x13;
                }
                spriteActorSetPlayingAnimation(param_1->m4, uVar5);
                allocateJumpAnimationStructCallbackSpecial0 = '\0';
            }
            if (param_1->m4->m48_defaultAnimationbundle != nullptr) {
                setupJumpAnimationStruct1C(7);
            }
;        }
        else {
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(param_1->m4, startJumpAnimationCallback);
        }
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
            battleAnimationCallback_2(param_1->m4, param_1->m4->m74_pTargetEntitySprite);
            break;
        case 4:
        {
            int index = (pSpriteCoreListHead[0]->mAC.mx0_entityIdUpper2bit) << 2 | (uint)pSpriteCoreListHead[0]->mA8.mx1E_entityId_bottom2bit;
            if ((index < 3) && (battleVisualEntities[index].m4_isGear == 0) && (battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m18_damageType[index] == 7)) {
                setRenderingInfoForItemUser(pSpriteCoreListHead[0]);
                spriteActorSetPlayingAnimation(pSpriteCoreListHead[0], 0x1B);
                while (pSpriteCoreListHead[0]->mAC.mx18 == 0x1B) {
                    battleRender();
                }
            }
            break;
        }
        case 5: // end of attack
            if (battleVisualEntities[(param_1->m4->mAC.mx0_entityIdUpper2bit) << 2 | (uint)param_1->m4->mA8.mx1E_entityId_bottom2bit].m3 == 0) {
                spriteActorSetPlayingAnimation(param_1->m4, param_1->m4->mB0.mx0_animationId);
            }
            battleAnimationCallback_5(jumpAnimationControlStruct->m24);
            setupJumpAnimationStruct1C(10);
            break;
        case 6:
            jumpUpdatePositionSub0(param_1->m4, param_1->m4->m74_pTargetEntitySprite);
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
    case 7: // using ability
        if (allocateSavePointMeshDataSub0_var0 == isDamageDisplayPolysTask2Running()) {
            setupJumpAnimationStruct1C(4);
            jumpAnimationControlStruct->m48 = 1;
            updateBattleAnimationDataLoading();
        }
        break;
    case 8: // Gear attack
        MissingCode();
        break;
    case 10: // during attack
        if (allocateSavePointMeshDataSub0_var0 == isDamageDisplayPolysTask2Running()) {
            deleteJumpAnimationControlStructEndOfAttack();
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
    currentJumpAnimationBitMask = battleCurrentDamages[allocateJumpAnimationStructVar0].m16_targetBitMask;
    numActiveSpriteCores = countBattleActorCoresActiveForBitmask(battleCurrentDamages[allocateJumpAnimationStructVar0].m16_targetBitMask, pSpriteCoreListHead, param_1);

    if (numActiveSpriteCores == 0) {
        pSpriteCoreListHead[0] = param_1;
    }

    param_1->m74_pTargetEntitySprite = pSpriteCoreListHead[0];
    pSpriteCoreListHead[0]->m74_pTargetEntitySprite = param_1;
    jumpAnimationControlStruct->m4C = pSpriteCoreListHead[0];
    jumpAnimationControlStruct->m28 = (pSpriteCoreListHead[0]->mAC.mx0_entityIdUpper2bit) << 2 | pSpriteCoreListHead[0]->mA8.mx1E_entityId_bottom2bit;
    setSpriteActorAngle(param_1, spriteActorCoreRatan2(param_1, param_1->m74_pTargetEntitySprite));
    if ((pSpriteCoreListHead[0]->mAC.mx18) != 0x15) {
        setSpriteActorAngle(pSpriteCoreListHead[0], spriteActorCoreRatan2(param_1->m74_pTargetEntitySprite, param_1));
    }
}

void startReadingBattleJumpAnimation(sSpriteActorCore* param_1) {
    MissingCode();
    setCurrentDirectory(0x2c, 1);
    battleAnimationLoadingDest = new sSpriteActorAnimationBundle;
    readFile(param_1->m7C->m0, *battleAnimationLoadingDest, 0, 0x80);
    startReadingBattleJumpAnimationVar0 = (param_1->mAC.mx0_entityIdUpper2bit) << 2 | param_1->mA8.mx1E_entityId_bottom2bit;
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

// param_1 is the entity we are jumping next to
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

        // Are we at the correct location for the attack?
        if (((param_1->mA0).vx == (param_1->m0_position).vx.getIntegerPart()) &&
            ((param_1->mA0).vz == (param_1->m0_position).vz.getIntegerPart())) {
            battleAnimationCallback_2(param_1, param_2); // we are already at the right spot!
        }
        else {
            jumpUpdatePositionSub1(param_1, 3); // need to move to the correct location, that happens when changing target after entering attack mode
            setupJumpAnimationStruct1C(2);
        }
    }
    else {
        jumpUpdatePosition(param_1);
    }
}

// param_1 is the entity we're jumping next to
void jumpUpdatePosition(sSpriteActorCore* param_1) {
    if ((battleJumpData[jumpAnimationControlStruct->m2C].m0_X == -1) &&
        (battleJumpData[jumpAnimationControlStruct->m2C].m2_Z == -1)) {

        (param_1->mA0).vx = param_1->m0_position.vx.getIntegerPart();
        (param_1->mA0).vy = 0;
        (param_1->mA0).vz = param_1->m0_position.vz.getIntegerPart();

        jumpUpdatePositionSub0(param_1, param_1->m74_pTargetEntitySprite);
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
    startJumpAnimationSub0(0);
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
            if ((battleAnimationLoadingDest == 0) ||
                (((jumpAnimationControlStruct->m4->mAC.mx0_entityIdUpper2bit) << 2 | jumpAnimationControlStruct->m4->mA8.mx1E_entityId_bottom2bit ) != startReadingBattleJumpAnimationVar0)) {
                startReadingBattleJumpAnimation(jumpAnimationControlStruct->m4);
            }
        }
        else {
            if (startJumpAnimationVar3 == 0) {
                battleIdleDuringLoading();
                setCurrentDirectory(0x2c, 0);
                sWdsFile buffer;
                readFile(7, buffer, 0, 0x80);
                battleIdleDuringLoading();
                loadWdsDataIfNeeded();
                loadWdsDataNeeded = loadWds(buffer, 0);
                while (waitForMusic(0)) {
                    battleRender();
                }
            }
            startJumpAnimationVar3 = 1;
        }
    }
    else {
        waitBattleAnimationSpriteLoading();
        waitBattleAnimationSoundLoaded();
        setupJumpAnimationStruct1C(8);
        performMechaPlayAnimation(actor, jumpTarget, 0x1a);
        setBattleCameraParamX(0xc0);
    }

    if (isBilly == 0) {
        if (battleVisualEntities[actor].m4_isGear == 0) {
            OP_INIT_ENTITY_SCRIPT_sub0Sub8(jumpAnimationControlStruct->m4, &startJumpAnimationCallback);
            jumpUpdatePosition(jumpAnimationControlStruct->m4);
        }
        else {
            performMechaPlayAnimation
            (actor, battleCurrentDamages[allocateJumpAnimationStructVar0].m16_targetBitMask, 2);

        }
    }
    else {
        int uVar6 = 4;
        if (battleVisualEntities[actor].m4_isGear != 0) {
            uVar6 = 9;
        }
        setupJumpAnimationStruct1C(uVar6);
    }
}

void startCharacterJumpToEnemy(int param_1) {
    updateAPCounterDisplay();
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
            return;
        }
        break;
    case 1: // down
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemHighlighted;
            return;
        }
        break;
    case 2: // left
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
        return;
    case 3: // up
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
            }
            else {
                battleSoundEffect2(0x4f);
            }
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackSelectEnemy;
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
    battleSoundEffect2(0x4f);
}

void drawCircleMenuItemActive(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
            return;
        }
        break;
    case 1:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemAltHighlighted;
            return;
        }
        break;
    case 2:
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
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
    battleSoundEffect2(0x4f);
}

void drawCircleMenuItemSecondaryActive(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemHighlighted;
            return;
        }
        break;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_EscapeHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        break;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
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

bool useCombo(s8 param_1) {
    MissingCode();
    return false;
}

void drawCircleMenuCombo(int param_1) {
    switch (battleInputButton) {
    case 0:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
            return;
        }
        break;
    case 1:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemAltHighlighted;
            return;
        }
        break;
    case 2:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_EscapeHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x03')) {
            if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] != 0) {
                battleSoundEffect2(0x4f);
                battleVar2->m2F6 = 0;
                return;
            }
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 4:
    case 6:
    case 7:
        if (battleVar2->m0[param_1].m3C_currentTarget != -1) {
            if (useCombo(param_1)) {
                battleVar2->m2E1_waitBeforeNextAttackInput = 1;
                return;
            }
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
            battleVar2->m2E2_previousActiveBattleMenu = eActiveBattleMenu::Undefined;
            return;
        }
        break;
    case 8:
    case 0xE:
        return;
    default:
        assert(0);
    }
    battleSoundEffect2(0x4f);
}


void drawBattleMenuEscape(int param_1) {
    switch (battleInputButton) {
    case 0: // right
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[5] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
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
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
        battleVar2->m2F6 = 0;
        break;
    case 1:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[0xb] != 0) {
            battleSoundEffect2(0x4f);
            return;
        }
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemAltHighlighted;
        return;
    case 2: // left
        battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
        return;
    case 3:
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[7] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_CallGear;
            return;
        }
        if ((battleVar2->m2F6 == 0) || (newBattleInputButton2 != '\x03')) {
            battleVar2->m2F6 = 1;
            battleSoundEffect2(0x4f);
            return;
        }
        if (battleVar2->m0[param_1 & 0xff].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
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
    battleVar2->m2F6 = 0;
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
    bool hasColor = false;
    for (int i = 0; i < 2; i++) {
        for(int j=0; j < battleVar1->mD0_battleCommandLength[i]; j++) {
            SetSemiTrans(&battleVar0->m641C[i][j][battleVar1->mA3], 1);
            SetShadeTex(&battleVar0->m641C[i][j][battleVar1->mA3], 0);

            battleVar0->m641C[i][j][battleVar1->mA3].tpage |= 0x20;
            u8 color = battleVar0->m641C[i][j][battleVar1->mA3].r0;
            if (color) {
                hasColor = true;
                battleVar0->m641C[i][j][battleVar1->mA3].r0 = color - 0x10 * battleVar2->m2E0_isBattleMenuDisabled;
                battleVar0->m641C[i][j][battleVar1->mA3].g0 = color - 0x10 * battleVar2->m2E0_isBattleMenuDisabled;
                battleVar0->m641C[i][j][battleVar1->mA3].b0 = color - 0x10 * battleVar2->m2E0_isBattleMenuDisabled;
            }
        }
    }

    if (!hasColor) {
        battleVar1->mCB = 0;
    }
}

uint updateBattleMenuSpriteForMenu(uint param_1, uint menuId, char isMenuDisabled) {
    if (isMenuDisabled == 0) {
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

void clearShapeTransferEntry(void*) {
    MissingCode();
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
                sTaskHeader* pTask = findMatchingGraphicEntity(battleSpriteActors[i], updateSpinningEnemySelectionCursor);
                if (pTask) {
                    pTask->mC_deleteCallback(pTask);
                }
            }
        }
        else {
            if (battleSpriteActors[i]) {
                sTaskHeader* pTask = findMatchingGraphicEntity(battleSpriteActors[i], updateSpinningEnemySelectionCursor);
                if (pTask == nullptr) {
                    createSpinningEnemySelectionCursor(battleSpriteActors[i]);
                }
            }
        }
        param_1 >>= 1;
    }
}

u8 selectNewSlotByDirection(byte inputSlot, battleInputDirection direction) {
    s32 bestResult = 0xffffff;
    byte slot = inputSlot;
    for (int i = 0; i < 12; i++) {
        if (targetsPerPriority[i] != -1) {
            bool bVar1 = false;
            if (targetsPerPriority[i] != slot) {
                s32 lVar3 = ratan2(
                    battleVisualEntities[targetsPerPriority[i]].mC_Z - battleVisualEntities[slot].mC_Z,
                    battleVisualEntities[targetsPerPriority[i]].mA_X - battleVisualEntities[slot].mA_X
                );

                switch (direction) {
                case 0:
                    if (((lVar3 + 0x200) & 0xFFFF) < 0x400)
                        bVar1 = true;
                    break;
                case 1:
                    if (((lVar3 + 0x600) & 0xFFFF) < 0x400)
                        bVar1 = true;
                    break;
                case 2:
                    bVar1 = ((lVar3 + 0x800U) & 0xffff) < 0x200;
                    if (((lVar3 - 0x600U) & 0xffff) < 0x201)
                        bVar1 = true;
                    break;
                case 3:
                    if (((lVar3 - 0x200) & 0xffff) < 0x400)
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
            if (selectNewSlotByDirection(battleVar2->m2E8_currentTarget, (battleInputDirection)i) == battleVar2->m2E8_currentTarget) {
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ComboHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\0')) {
            if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
                battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemAltHighlighted;
            return;
        }
        battleSoundEffect2(0x4f);
        return;
    case 2:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[8] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_EscapeHighlighted;
            return;
        }
        if ((battleVar2->m2F6 != 0) && (newBattleInputButton2 == '\x02')) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_DefendHighlighted;
            battleVar2->m2F6 = 0;
            return;
        }
        battleVar2->m2F6 = 1;
        battleSoundEffect2(0x4f);
        return;
    case 3:
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[10] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
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
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -2;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
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
    mechaPlayAnimation_battle(param_1, param_2, param_3);
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
        uVar3 = (psVar4->mAC.mx0_entityIdUpper2bit) << 2 | psVar4->mA8.mx1E_entityId_bottom2bit;
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
void markEnemyDead(uint param_1);
void moveEntityToOtherEntity(byte param_1, uint param_2) {
    byte bVar1;
    sBattleMechaInitData* psVar2;
    byte bVar3;
    short sVar4;
    int iVar5;
    int iVar6;
    uint uVar7;
    uint uVar8;

    if (battleVisualEntities[param_1].m0_positionSlot !=
        battleVisualEntities[param_2 & 0xff].m0_positionSlot) {
        iVar6 = 0;
        if ((param_2 & 0xff) < 3) {
            iVar6 = (param_1 < 3 ^ 1) << 3;
        }
        param_2 = param_2 & 0xff;
        if (battleSlotLayout[(uint)battleVisualEntities[param_2].m0_positionSlot + iVar6][0] < 4) {
            markEnemyDead(param_1);
            uVar8 = 0;
            iVar5 = (uint)battleVisualEntities[param_2].m0_positionSlot + iVar6;
            battleSlotLayout[iVar5][0] = battleSlotLayout[iVar5][0] + 1;
            do {
                sVar4 = bitmaskCharacterCheck
                (battleSlotLayout
                    [(uint)battleVisualEntities[param_2].m0_positionSlot + iVar6][1],
                    uVar8 & 0xff);
                uVar7 = (uint)param_1;
                if (sVar4 == 0) break;
                uVar8 = uVar8 + 1;
            } while ((int)uVar8 < 4);
            bVar3 = battleVisualEntities[param_2].m0_positionSlot;
            battleVisualEntities[uVar7].m1 = (byte)uVar8;
            bVar1 = battleVisualEntities[uVar7].m1;
            battleVisualEntities[uVar7].m0_positionSlot = bVar3;
            bVar3 = characterIdToTargetBitmask(bVar1);
            iVar6 = (uint)battleVisualEntities[uVar7].m0_positionSlot + iVar6;
            battleSlotLayout[iVar6][1] = battleSlotLayout[iVar6][1] | bVar3;
            psVar2 = battleLoadDataVar2Bis;
            if (uVar7 < 3) {
                bVar3 = battleVisualEntities[uVar7].m1;
                battleVisualEntities[uVar7].mA_X =
                    battleLoadDataVar2Bis->m4[battleVisualEntities[uVar7].m0_positionSlot]
                    [battleVisualEntities[uVar7].m1].vx;
                sVar4 = psVar2->m4[battleVisualEntities[uVar7].m0_positionSlot][bVar3].vy;
            }
            else {
                bVar3 = battleVisualEntities[uVar7].m1;
                battleVisualEntities[uVar7].mA_X =
                    battleLoadDataVar2Bis->m4[battleVisualEntities[uVar7].m0_positionSlot]
                    [battleVisualEntities[uVar7].m1 + 3].vx;
                sVar4 = psVar2->m4[battleVisualEntities[uVar7].m0_positionSlot][bVar3 + 3].vy;
            }
            battleVisualEntities[uVar7].mC_Z = sVar4;
        }
    }
}

void moveEntityToShootEntity(uint param_1, uint param_2) {
    assert(0);
}

void performAttackSub2() {
    for (int i = 0; i < 11; i++) {
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m0_damageValue[i] = 0;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m18_damageType[i] = -1;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m24[i] = 0;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3C[i] = -1;
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

sBattle800CDD40Sub* currentEntityBattleStats;
std::array<sBattle800cdd40, 3> partyBattleStats;
std::array<sBattle800CEF10, 3> mechaBattleStats;
sBattle800D02C0 enemiesBattleStats;
sGameStateA4* battleGetSlotStatusSub_currentBattleEntity;
sGameStateA42* battleGetSlotStatusSub_currentBattleEntityGear;

void startBattleAttackAnimationSub0(u8 attacker, u8 param_2) {
    if ((param_2 == 0) || (currentEntityBattleStats->mA & 0x100)) {
        u16 uVar1;
        if ((battleEntities[attacker].m15A_flags & 0x80) == 0) {
            uVar1 = battleEntities[attacker].m0_base.m88 & 0x400;
        }
        else {
            assert(0);
        }
        attacker = attacker & 0xff;
        if (uVar1 != 0) {
            if ((battleEntities[attacker].m15A_flags & 0x80) == 0) {
                MissingCode();
                battleEntities[attacker].m0_base.m88 = battleEntities[attacker].m0_base.m88 & 0xfbff;
            }
            else {
                assert(0);
            }
        }
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

s8 computeBattleDamageSub0() {
    int iVar5 = 0;
    int iVar6 = 0;
    if (((currentEntityBattleStats->mA & 0x40) != 0) &&
        ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) != 0)) {
        return 3;
    }

    if (battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 4) {
        assert(0);
    }
    else {
        if ((currentEntityBattleStats->mA & 0x200) != 0) {
            if ((startBattleAttackAnimationVar5->m34 & 8U) != 0) {
                return 3;
            }
            if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) != 0) {
                return 3;
            }
        }
        s16 uVar1 = currentEntityBattleStats->mA;
        if ((uVar1 & 0x1000) != 0) {
            return 3;
        }
        s16 uVar2 = startBattleAttackAnimationVar5->m84 | startBattleAttackAnimationVar5->m86;
        if ((uVar2 & 0x100) != 0) {
            return 3;
        }
        if ((startBattleAttackAnimationVar5->m7C & 0x2000) != 0) {
            return 1;
        }
        if ((startBattleAttackAnimationVar5->m80 & 0x1000) != 0) {
            return 1;
        }
        if ((uVar1 & 0x8000) != 0) {
            return 1;
        }
        if ((uVar1 & 2) != 0) {
            return 5;
        }
        if ((battleGetSlotStatusSub_currentBattleEntity->m7C & 0x400) != 0) {
            iVar5 = -0x32;
        }
        if (((battleGetSlotStatusSub_currentBattleEntity->m84 |
            battleGetSlotStatusSub_currentBattleEntity->m86) & 0x1000) != 0) {
            iVar5 = iVar5 + 0x1e;
        }
        if ((uVar2 & 0x800) != 0) {
            iVar6 = 0x32;
        }
        int iVar4 = ((uint)battleGetSlotStatusSub_currentBattleEntity->m5E_hitPercentage +
            (int)(char)currentEntityBattleStats->m15) - (uint)startBattleAttackAnimationVar5->m5F_evadePercentage;
        if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 1) != 0) {
            iVar5 = xenoRand();
            if (0x5e < iVar5 % 100) {
                return 1;
            }
            return 2;
        }
        if ((uVar2 & 0x20) != 0) {
            iVar5 = xenoRand();
            if ((iVar5 % 100 - iVar4) * 0x10000 >> 0x10 < 50) {
                return 1;
            }
            return 3;
        }
        if (((startBattleAttackAnimationVar5->m84 | startBattleAttackAnimationVar5->m86) & 0x40) != 0) {
            iVar5 = xenoRand();
            if ((iVar5 % 100 - iVar4) * 0x10000 >> 0x10 < 50) {
                return 1;
            }
            return 2;
        }
        int iVar3 = xenoRand();
        if ((iVar3 % 100 - iVar4) * 0x10000 >> 0x10 < (iVar5 - (iVar6 + -0x5a)) * 0x10000 >> 0x10) {
            iVar3 = xenoRand();
            if ((iVar5 - (iVar6 + -0x55)) * 0x10000 >> 0x10 <= (iVar3 % 100 - iVar4) * 0x10000 >> 0x10) {
                return 2;
            }
            return 1;
        }
    }
    return 3;
}

int computeBattleDamageSub1() {
    byte bVar4 = 0;
    std::array<byte, 5> local_30;
    u32 uVar2;

    if ((battleEntities[startBattleAttackAnimationVar2].m15A_flags & 0x80) == 0) {
        uVar2 = 0;
        do {
            bVar4 = bVar4 + 1;
            local_30[uVar2] = battleGetSlotStatusSub_currentBattleEntity->m4[uVar2].m0;
            uVar2 = (uint)bVar4;
        } while (bVar4 < 4);
        uVar2 = (uint)(byte)battleGetSlotStatusSub_currentBattleEntity->m58_attack;
    }
    else {
        assert(0); // on gear
    }

    bVar4 = currentEntityBattleStats->m10;
    uint uVar6 = (uint)battleGetSlotStatusSub_currentBattleEntity->m5B_ether;
    uint uVar5 = 0;
    if ((bVar4 & 0x80) != 0) {
        uVar5 = (uint)local_30[0];
    }
    if ((bVar4 & 0x40) != 0) {
        uVar5 = local_30[1] + uVar5;
    }
    if ((bVar4 & 0x20) != 0) {
        uVar5 = local_30[2] + uVar5;
    }
    if ((bVar4 & 0x10) != 0) {
        uVar5 = local_30[3] + uVar5;
    }
    if ((bVar4 & 8) != 0) {
        assert(0);  // isn't local_30[4] uninitialized?
        uVar5 = local_30[4] + uVar5;
    }
    if (((battleGetSlotStatusSub_currentBattleEntity->m8C |
        battleGetSlotStatusSub_currentBattleEntity->m8E) & 1U) != 0) {
        uVar5 = uVar5 + (uVar5 >> 1);
    }
    u16 uVar1;

    u32 uVar3;
    if ((currentEntityBattleStats->mA & 0x100) == 0) {
        if ((battleEntities[startBattleAttackAnimationVar2].m15A_flags & 0x80) == 0) {
            uVar1 = battleGetSlotStatusSub_currentBattleEntity->m84 |
                battleGetSlotStatusSub_currentBattleEntity->m86;
            uVar3 = 4;
            if ((uVar1 & 0x2000) != 0) {
                uVar3 = 5;
            }
            if ((battleGetSlotStatusSub_currentBattleEntity->m7C & 0x200) != 0) {
                uVar3 = uVar3 - 1;
            }
            if ((uVar1 & 0x400) != 0) {
                uVar3 = (uVar3 + 10) -
                    (uint)(ushort)battleGetSlotStatusSub_currentBattleEntity->m4C_HP /
                    ((ushort)battleGetSlotStatusSub_currentBattleEntity->m4E_MaxHP / 10);
            }
            uVar3 = (uVar2 & 0xffff) * (uVar3 & 0xff);
            uVar2 = uVar3 >> 2;
            if (false) { // ?
                uVar2 = uVar3 + 3 >> 2;
            }
        }
    }
    else {
        uVar3 = 4;
        if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
            battleGetSlotStatusSub_currentBattleEntity->m8A) & 0x8000) != 0) {
            uVar3 = 5;
        }
        if ((battleGetSlotStatusSub_currentBattleEntity->m80 & 0x400) != 0) {
            uVar3 = uVar3 - 1;
        }
        uVar6 = uVar6 * (uVar3 & 0xff);
        if (false) {
            uVar6 = uVar6 + 3;
        }
        uVar6 = uVar6 >> 2;
        if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
            battleGetSlotStatusSub_currentBattleEntity->m8A) & 0x2000) != 0) {
            uVar6 = uVar6 << 1;
        }
    }

    ushort local_28 = 0;

    if ((currentEntityBattleStats->mA & 0x20) != 0) {
        uVar2 = 0;
    }

    switch (currentEntityBattleStats->m1A) {
    case 1:
        local_28 = (ushort)uVar6;
        break;
    case 0:
        uVar1 = (short)uVar5 + (short)uVar2;
        local_28 = uVar1;
        break;
    default:
        assert(0);
        break;
    }
    

    if (startBattleAttackAnimationVar2 < 3) {
        if (battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 7) {
            assert(0);
        }
        if ((battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 4) ||
            ((currentEntityBattleStats->m22 & 0x20) != 0)) {
            assert(0);
        }
        if (((battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 8) &&
            ((battleEntities[startBattleAttackAnimationVar2].m15A_flags & 0x80) == 0)) &&
            ((currentEntityBattleStats->mA & 0x100) != 0)) {
            assert(0);
        }
        if (battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 10) {
            local_28 = local_28 + local_28 / 5;
        }
        if ((((currentEntityBattleStats->m22 & 0x10) != 0) &&
            ((startBattleAttackAnimationVar5->m38 & 0x10) != 0)) &&
            (local_28 = local_28 + (local_28 >> 2), (startBattleAttackAnimationVar5->m38 & 0x10) != 0)) {
            local_28 = local_28 + (local_28 >> 2);
        }
    }

    return (int)(short)local_28;
}

u16 computeBattleDamageSub2() {
    u16 uVar5;
    uint in_a2;
    if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) == 0) {
        in_a2 = (uint)startBattleAttackAnimationVar5->m59_defence;
        uVar5 = (uint)startBattleAttackAnimationVar5->m2D;
    }
    else {
        uVar5 = (uint)startBattleAttackAnimationVar6->m70;
    }
    uint uVar2 = (uint)startBattleAttackAnimationVar5->m5C_etherDefence;
    if ((currentEntityBattleStats->mA & 0x100) == 0) {
        if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) == 0) {
            if (((startBattleAttackAnimationVar5->m84 | startBattleAttackAnimationVar5->m86) & 0x100) != 0
                ) {
                in_a2 = (in_a2 & 0xffff) * 3 >> 1;
            }
            goto LAB_Battle__80097730;
        }
    }
    else {
        if (((startBattleAttackAnimationVar5->m88 | startBattleAttackAnimationVar5->m8A) & 0x4000) != 0)
        {
            uVar2 = uVar2 * 3 >> 1;
        }
    LAB_Battle__80097730:
        if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) == 0)
            goto LAB_Battle__80097774;
    }

    in_a2 = 0;
LAB_Battle__80097774:
    switch (currentEntityBattleStats->m1B) {
    case 1:
        break;
    case 0:
        uVar2 = uVar5 + in_a2;
        break;
    default:
        assert(0);
    }

    s16 sVar4 = (short)uVar2;
    if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) == 0) {
        uint uVar3 = 0;
        byte bVar1 = 0;
        uVar5 = 0;
        do {
            bVar1 = bVar1 + 1;
            if ((battleEntities[uVar5].m0_base.m7C & 0x8000) != 0) {
                uVar3 = uVar3 + 1;
            }
            uVar5 = (uint)bVar1;
        } while (bVar1 < 3);
        if ((((startBattleAttackAnimationVar5->m32 & 4U) != 0) && ((uVar3 & 0xff) != 0)) &&
            ((currentEntityBattleStats->mA & 0x100) == 0)) {
            uVar5 = (uVar2 & 0xffff) * (4 - (uVar3 & 0xff));
            uVar2 = uVar5 >> 2;
            if ((int)uVar5 < 0) {
                uVar2 = uVar5 + 3 >> 2;
            }
        }
        if ((((startBattleAttackAnimationVar5->m32 & 2U) != 0) && ((uVar3 & 0xff) != 0)) &&
            ((currentEntityBattleStats->mA & 0x100) == 0)) {
            uVar2 = (uVar2 & 0xffff) * ((uVar3 & 0xff) + 2) >> 1;
        }
        sVar4 = (short)uVar2;
        if (((startBattleAttackAnimationVar5->m32 & 1U) != 0) && ((uVar3 & 0xff) != 0)) {
            sVar4 = (short)((uVar2 & 0xffff) * ((uVar3 & 0xff) + 2) >> 1);
        }
    }
    return (int)sVar4;
}

void computeBattleDamageSub3(ushort* param_1, ushort* param_2, s8* param_3)

{
    ushort uVar1;
    uint uVar2;
    uint uVar3;
    int iVar4;
    char cVar5;
    int iVar6;
    ushort uVar7;
    uint uVar8;
    int iVar9;

    uVar8 = (byte)currentEntityBattleStats->m22 & 0x3f;
    if ((char)battleEntities[startBattleAttackAnimationVar2].m15A_flags < '\0') {
        uVar7 = battleGetSlotStatusSub_currentBattleEntityGear->m84 |
            battleGetSlotStatusSub_currentBattleEntityGear->m86;
    }
    else {
        uVar7 = battleGetSlotStatusSub_currentBattleEntity->m8C |
            battleGetSlotStatusSub_currentBattleEntity->m8E;
    }
    uVar3 = (uint)(uVar7 >> 0xc);
    uVar2 = uVar8;
    if (((currentEntityBattleStats->m22 & 0x3f) != 0) || (uVar2 = uVar3, uVar3 != 0)) {
        uVar8 = uVar2;
    }
    iVar6 = 10;
    iVar9 = 10;
    if ((uVar8 & *(byte*)&startBattleAttackAnimationVar5->m38 & 0x3f) != 0) {
        iVar6 = 0xf;
        if ((startBattleAttackAnimationVar5->m38 & 0x40) != 0) {
            iVar6 = 0x12;
        }
        if ((uVar8 & uVar3) != 0) {
            iVar6 = iVar6 + 2;
        }
    }
    uVar7 = 0;
    if ((char)battleEntities[startBattleAttackAnimationVar4].m15A_flags < '\0') {
        uVar1 = startBattleAttackAnimationVar6->m84 | startBattleAttackAnimationVar6->m86;
        uVar2 = (uint)uVar1;
        uVar3 = (uVar2 & 0xf00) >> 8;
        if ((uVar1 & 2) != 0) {
            uVar7 = 1;
        }
    }
    else {
        uVar7 = startBattleAttackAnimationVar5->m8C | startBattleAttackAnimationVar5->m8E;
        uVar2 = (uint)uVar7;
        uVar3 = (uVar2 & 0xf00) >> 8;
        uVar7 = uVar7 >> 1 & 1;
    }
    if ((uVar8 & uVar3) != 0) {
        iVar4 = iVar6 + -3;
        if ((currentEntityBattleStats->mA & 0x100) != 0) {
            if ((uVar2 & 4) != 0) {
                iVar4 = iVar6 + -6;
            }
            iVar6 = iVar4;
            if ((uVar2 & 8) != 0) {
                *param_3 = 4;
            }
        }
        if (((uVar8 & uVar3) != 0) && (iVar4 = iVar6 + -3, uVar7 != 0)) {
            if ((uVar2 & 4) != 0) {
                iVar4 = iVar6 + -6;
            }
            iVar6 = iVar4;
            if ((uVar2 & 8) != 0) {
                *param_3 = 4;
            }
        }
    }
    cVar5 = (char)iVar6;
    uVar3 = uVar3 << 8;
    if (uVar8 == 2) {
        uVar3 = uVar3 & 0x100;
    }
    else if (uVar8 < 3) {
        uVar3 = uVar3 & 0x200;
        if (uVar8 != 1) {
            iVar4 = iVar6 << 0x18;
            goto LAB_Battle__80096724;
        }
    }
    else if (uVar8 == 4) {
        uVar3 = uVar3 & 0x800;
    }
    else {
        iVar4 = iVar6 << 0x18;
        if (uVar8 != 8) goto LAB_Battle__80096724;
        uVar3 = uVar3 & 0x400;
    }
    iVar4 = iVar6 << 0x18;
    if (uVar3 != 0) {
        cVar5 = (char)(iVar6 + 3);
        iVar4 = (iVar6 + 3) * 0x1000000;
    }
LAB_Battle__80096724:
    if (iVar4 < 1) {
        cVar5 = '\x01';
    }
    if (false) {
        iVar9 = 1;
    }
    *param_1 = (ushort)((int)((uint)*param_1 * (int)cVar5) / 10);
    *param_2 = (ushort)((iVar9 * (uint)*param_2) / 10);
    if ((battleGetSlotStatusSub_currentBattleEntity->m32 & 0x10) != 0) {
        *param_1 = *param_1 + *param_1 / 5;
    }
    if ((startBattleAttackAnimationVar5->m32 & 0x10) != 0) {
        *param_2 = *param_2 + *param_2 / 5;
    }
    return;
}

int startBattleAttackAnimationVar8_2Sub0(uint param_1, byte param_2, ushort param_3)
{
    char cVar1;
    ushort uVar2;
    int iVar3;
    sGameStateA4* psVar4;

    if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) != 0) {
        return 0;
    }
    iVar3 = xenoRand();
    if ((int)(param_1 & 0xff) < iVar3 % 100) {
        return 0;
    }
    if (true) {
        switch (param_2)
        {
        case 9:
            if ((param_3 & 0xf000) != 0) {
                if ((startBattleAttackAnimationVar5->m8E & 0xf000) != 0) {
                    battleTickMain_var1 = 0x39;
                    return 0;
                }
                startBattleAttackAnimationVar5->m8C = startBattleAttackAnimationVar5->m8C & 0xfff;
            }
            if ((param_3 & 0xf00) != 0) {
                if ((startBattleAttackAnimationVar5->m8E & 0xf00) != 0) {
                    battleTickMain_var1 = 0x39;
                    return 0;
                }
                startBattleAttackAnimationVar5->m8C = startBattleAttackAnimationVar5->m8C & 0xf0ff;
            }
            break;
        default:
            assert(0);
            break;
        }
    }
    psVar4 = startBattleAttackAnimationVar5;

    if (true) {
        switch (param_2) {
        case 9:
            //psVar4 = (sGameStateA4*)((int)startBattleAttackAnimationVar5->m4 + (uint)param_2 * 2 + -4);
            assert(0);
            uVar2 = param_3 | psVar4->m7A_commandEnabledBF;
        default:
            assert(0);
            break;
        }
        psVar4->m7A_commandEnabledBF = uVar2;
    }
    assert(0);
    return 1;
}

void startBattleAttackAnimationVar8_2Sub1(uint param_1, byte param_2, ushort param_3, uint param_4) {
    ushort uVar1;
    byte uVar2;
    uint uVar3;

    uVar3 = 0xf;
    if ((battleEntities[startBattleAttackAnimationVar2].m0_base.m8A & 0x2000) != 0) {
        param_4 = (param_4 & 0xff) << 1;
    }
    switch (param_2) {
    default:
        assert(0);
    }
    assert(0);
}

void startBattleAttackAnimationVar8_2() {
    char cVar1;

    cVar1 = startBattleAttackAnimationVar8_2Sub0
    (currentEntityBattleStats->m1C, currentEntityBattleStats->m1D,
        currentEntityBattleStats->m1E);
    if ((currentEntityBattleStats->mA & 0x4000) == 0) {
        startBattleAttackAnimationVar8_2Sub1
        (startBattleAttackAnimationVar4, currentEntityBattleStats->m1D,
            currentEntityBattleStats->m1E, currentEntityBattleStats->m11);
        if (cVar1 != '\x01') {
            initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 6;
        }
    }
    else {
        startBattleAttackAnimationVar8_2Sub1
        (startBattleAttackAnimationVar4, currentEntityBattleStats->m1D,
            currentEntityBattleStats->m1E, 5);
    }
}

void startBattleAttackAnimationVar8_1() { // healing (used in Citan's Sazanami)
    ushort uVar1;
    short sVar2;

    sVar2 = (ushort)battleGetSlotStatusSub_currentBattleEntity->m5B_ether *
        (ushort)(byte)currentEntityBattleStats->m11;
    if ((battleGetSlotStatusSub_currentBattleEntity->m8A & 0x2000) != 0) {
        sVar2 = sVar2 * 2;
    }
    uVar1 = startBattleAttackAnimationVar5->m8C | startBattleAttackAnimationVar5->m8E;
    if ((uVar1 & 0x100) != 0) {
        sVar2 = (short)((sVar2 * 7) / 10);
    }
    if ((uVar1 & 0x200) != 0) {
        sVar2 = (short)((sVar2 * 0xd) / 10);
    }
    if ((battleEntities[startBattleAttackAnimationVar4].m15A_flags & 0x80) != 0) {
        sVar2 = 0;
    }
    initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 2;
    damageDonePerAttack[startBattleAttackAnimationVar4] = (int)sVar2;
}

void startBattleAttackAnimationVar8_0() {
    u32 uVar5 = currentEntityBattleStats->m11;

    u16 uVar1;
    if ((currentEntityBattleStats->mA & 0x100) == 0) {
        uVar1 = startBattleAttackAnimationVar5->m34 & 0x8000;
    }
    else {
        uVar1 = startBattleAttackAnimationVar5->m34 & 0x4000;
    }
    if (uVar1 != 0) {
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 0;
        return;
    }

    s8 local_14 = computeBattleDamageSub0();
    u16 local_18 = computeBattleDamageSub1();
    u16 local_16 = computeBattleDamageSub2();

    if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
        battleGetSlotStatusSub_currentBattleEntity->m8A) & 8) != 0) {
        local_18 = local_18 + local_18 / 5;
    }
    if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
        battleGetSlotStatusSub_currentBattleEntity->m8A) & 2) != 0) {
        local_18 = local_18 + local_18 / 10;
    }
    if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
        battleGetSlotStatusSub_currentBattleEntity->m8A) & 4) != 0) {
        local_18 = local_18 - local_18 / 5;
    }
    if (((battleGetSlotStatusSub_currentBattleEntity->m88 |
        battleGetSlotStatusSub_currentBattleEntity->m8A) & 1) != 0) {
        local_18 = local_18 - local_18 / 10;
    }
    if (((startBattleAttackAnimationVar5->m88 | startBattleAttackAnimationVar5->m8A) & 4) != 0) {
        local_16 = local_16 + local_16 / 5;
    }
    if (((startBattleAttackAnimationVar5->m88 | startBattleAttackAnimationVar5->m8A) & 1) != 0) {
        local_16 = local_16 + local_16 / 10;
    }
    if (((startBattleAttackAnimationVar5->m88 | startBattleAttackAnimationVar5->m8A) & 8) != 0) {
        local_16 = local_16 - local_16 / 5;
    }
    if (((startBattleAttackAnimationVar5->m88 | startBattleAttackAnimationVar5->m8A) & 2) != 0) {
        local_16 = local_16 - local_16 / 10;
    }

    if ((currentEntityBattleStats->m22 & 0x10) != 0) {
        if ((startBattleAttackAnimationVar5->m82 & 0x40) == 0) {
            startBattleAttackAnimationVar5->m80 = startBattleAttackAnimationVar5->m80 | 0x40;
        }
        if (((battleGetSlotStatusSub_currentBattleEntity->m8C |
            battleGetSlotStatusSub_currentBattleEntity->m8E) & 0x4000) != 0) {
            initBattleAttackStatusArray0[startBattleAttackAnimationVar2] = 3;
            damageDonePerAttack[startBattleAttackAnimationVar2] =
                (ushort)battleGetSlotStatusSub_currentBattleEntity->m52_MaxMP / 10 << 1;
        }
        if (((battleGetSlotStatusSub_currentBattleEntity->m8C |
            battleGetSlotStatusSub_currentBattleEntity->m8E) & 0x1000) != 0) {
            initBattleAttackStatusArray0[startBattleAttackAnimationVar2] = 2;
            damageDonePerAttack[startBattleAttackAnimationVar2] =
                (ushort)battleGetSlotStatusSub_currentBattleEntity->m4E_MaxHP / 10 << 1;
        }
    }

    if (((currentEntityBattleStats->m22 & 0x20) != 0) &&
        ((startBattleAttackAnimationVar5->m82 & 0x80) == 0)) {
        startBattleAttackAnimationVar5->m80 = startBattleAttackAnimationVar5->m80 | 0x80;
    }
    if ((startBattleAttackAnimationVar5->m80 & 0x40) != 0) {
        local_16 = local_16 - (local_16 >> 2);
        startBattleAttackAnimationVar5->m80 = startBattleAttackAnimationVar5->m80 & 0xffbf;
    }
    if ((battleGetSlotStatusSub_currentBattleEntity->m80 & 0x80) != 0) {
        local_18 = local_18 - (local_18 >> 2);
        battleGetSlotStatusSub_currentBattleEntity->m80 =
            battleGetSlotStatusSub_currentBattleEntity->m80 & 0xff7f;
    }
    if ((currentEntityBattleStats->mA & 0x400) != 0) {
        uVar5 = 0x14;
    }

    computeBattleDamageSub3(&local_18, &local_16, &local_14);

    int iVar3 = 5;
    int iVar4;
    if ((currentEntityBattleStats->mA & 0x100) == 0) {
        iVar3 = 4;
        iVar4 = 3;
    }
    else {
        iVar4 = 4;
    }
    if (local_16 == 0) {
        iVar3 = iVar3 * (uint)local_18;
    }
    else {
        iVar3 = iVar3 * (uint)local_18 - iVar4 * (uint)local_16;
    }
    if ((true) && (currentEntityBattleStats->m1A < 2)) {
        iVar3 = (int)(uVar5 * iVar3) / 0x14;
    }
    int damageDone;
    if (iVar3 < 1) {
        damageDone = 0;
    }
    else if (iVar3 < 10) {
        iVar4 = xenoRand();
        damageDone = iVar3 + iVar4 % 2;
    }
    else {
        iVar4 = xenoRand();
        damageDone = iVar3 + iVar4 % (iVar3 / 10 + 2);
    }
    switch ((int)((local_14 - 1) * 0x1000000) >> 0x18) {
    case 0:
        if (damageDone < 1) {
            damageDone = 1;
        }
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 0;
        break;
    case 1:
        damageDone = damageDone / 2;
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 5;
        break;
    case 2:
        damageDone = 0;
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 4;
        break;
    case 3:
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 2;
        break;
    case 4:
        initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 7;
        break;
    default:
        assert(0);
    }

    if (((startBattleAttackAnimationVar3 != '\0') && ((currentEntityBattleStats->mA & 0x100) != 0)) &&
        (damageDone != 0)) {
        damageDone = damageDone / 3;
    }

    //Hack("Hacked damage to 1000");
    //damageDone = 1000;

    if (9999 < damageDone) {
        damageDone = 9999;
    }
    if (damageDone < 0) {
        damageDone = 0;
    }

    damageDonePerAttack[startBattleAttackAnimationVar4] = damageDone;
    return;
}

typedef void(*startBattleAttackAnimationVar8Type)();
const std::array<startBattleAttackAnimationVar8Type, 8> startBattleAttackAnimationVar8 = { {
     startBattleAttackAnimationVar8_0,
     startBattleAttackAnimationVar8_1,
     startBattleAttackAnimationVar8_2,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
     startBattleAttackAnimationVar8Unimplemented,
} };


void postComputeAttachDone(void)
{
    short sVar1;
    sGameStateA4* psVar2;
    int iVar3;
    int iVar4;
    uint uVar5;
    byte bVar6;
    ushort uVar7;

    psVar2 = battleGetSlotStatusSub_currentBattleEntity;
    if (initBattleAttackStatusArray0[startBattleAttackAnimationVar4] == 0) {
        if ((damageDonePerAttack[startBattleAttackAnimationVar4] ==
            (uint)(ushort)startBattleAttackAnimationVar5->m4C_HP) &&
            (startBattleAttackAnimationVar2 < 3)) {
            sVar1 = battleGetSlotStatusSub_currentBattleEntity->m3A;
            uVar7 = sVar1 + 1;
            battleGetSlotStatusSub_currentBattleEntity->m3A = uVar7;
            if (65000 < uVar7) {
                psVar2->m3A = sVar1;
            }
        }
        uVar7 = startBattleAttackAnimationVar5->m80;
        startBattleAttackAnimationVar5->m80 = uVar7 & 0xefff;
        if (((uVar7 & 0x2000) != 0) && (iVar3 = xenoRand(), iVar3 % 100 < 0x46)) {
            startBattleAttackAnimationVar5->m80 = startBattleAttackAnimationVar5->m80 & 0xdfff;
        }
        if ((currentEntityBattleStats->mA & 0x100) != 0) {
            if ((startBattleAttackAnimationVar5->m36 & 0x4000) != 0) {
                initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 2;
            }
            if (((startBattleAttackAnimationVar5->m36 & 0x2000) != 0) &&
                ((currentEntityBattleStats->m22 & 0xf) == 0)) {
                initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 2;
            }
        }
        if ((startBattleAttackAnimationVar5->m32 & 0x80) != 0) {
            iVar3 = 0x3c;
            if (startBattleAttackAnimationVar5->m56_battleCommandLoadout == 0) {
                iVar3 = 0x50;
            }
            iVar4 = rand();
            if (iVar4 % 100 < iVar3) {
                damageDonePerAttack[startBattleAttackAnimationVar4] =
                    damageDonePerAttack[startBattleAttackAnimationVar4] >> 1;
            }
            else {
                damageDonePerAttack[startBattleAttackAnimationVar4] =
                    (damageDonePerAttack[startBattleAttackAnimationVar4] >> 1) +
                    damageDonePerAttack[startBattleAttackAnimationVar4];
            }
        }
        if ((startBattleAttackAnimationVar5->m32 & 0x20) != 0) {
            initBattleAttackStatusArray0[startBattleAttackAnimationVar2] = 0;
            damageDonePerAttack[startBattleAttackAnimationVar2] =
                damageDonePerAttack[startBattleAttackAnimationVar4];
        }
        if (startBattleAttackAnimationVar2 < 3) {
            uVar5 = (uint)startBattleAttackAnimationVar4;
            if (startBattleAttackAnimationVar2 != uVar5) {
                uVar7 = battleEntities[uVar5].m0_base.m7C;
                if ((uVar7 & 2) != 0) {
                    battleEntities[uVar5].m0_base.m7C = uVar7 & 0xfffd;
                    //battleEntities[uVar5].m0_base.m7A_commandEnabledBF = SHORT_ARRAY_Battle__800c3aa4[uVar5];
                    assert(0);
                }
            }
        }
        if ((startBattleAttackAnimationVar5->m56_battleCommandLoadout == 3) &&
            ((uint)(ushort)startBattleAttackAnimationVar5->m4C_HP <=
                damageDonePerAttack[startBattleAttackAnimationVar4])) {
            bVar6 = 3;
            uVar5 = 3;
            do {
                bVar6 = bVar6 + 1;
                battleEntities[uVar5].m0_base.m80 = battleEntities[uVar5].m0_base.m80 & 0xffdf;
                uVar5 = (uint)bVar6;
            } while (bVar6 < 0xb);
        }
    }
    if ((startBattleAttackAnimationVar5->m36 & 0x8000) != 0) {
        bVar6 = initBattleAttackStatusArray0[startBattleAttackAnimationVar4];
        if (bVar6 == 2) {
            initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 0;
        }
        else {
            if (bVar6 < 3) {
                if (bVar6 != 0) goto LAB_Battle__80094bb8;
            }
            else if (bVar6 != 5) goto LAB_Battle__80094bb8;
            initBattleAttackStatusArray0[startBattleAttackAnimationVar4] = 2;
        }
    }
LAB_Battle__80094bb8:
    if (((startBattleAttackAnimationVar5->m86 & 0x80) != 0) &&
        (uVar5 = (uint)startBattleAttackAnimationVar4, initBattleAttackStatusArray0[uVar5] == 2)) {
        damageDonePerAttack[uVar5] = damageDonePerAttack[uVar5] << 1;
    }
    if (((startBattleAttackAnimationVar5->m8A & 0x200) != 0) &&
        (initBattleAttackStatusArray0[startBattleAttackAnimationVar4] == 1)) {
        damageDonePerAttack[startBattleAttackAnimationVar4] = 0;
    }
    return;
}

void onPlayerAttackedByEnemy() {
    byte bVar1;
    sGameStateA4* psVar2;
    int iVar3;
    uint uVar4;
    int iVar5;

    if (((startBattleAttackAnimationVar5->m7C & 0xa000) == 0) &&
        ((startBattleAttackAnimationVar5->m80 & 0x1000) == 0)) {
        if (((2 < startBattleAttackAnimationVar2) ||
            ((startBattleAttackAnimationVar5->m80 & 0x2000) != 0)) &&
            (((((startBattleAttackAnimationVar5->m88 & 0x1000) != 0 &&
                ((currentEntityBattleStats->mA & 0x2000) == 0)) &&
                ((currentEntityBattleStats->mA & 0x100) == 0)) &&
                ((battleEntities[startBattleAttackAnimationVar2].m15A_flags & 0x80) == 0)))) {
            iVar5 = 0x32;
            if (startBattleAttackAnimationVar5->m56_battleCommandLoadout == 0) {
                iVar5 = 0x3c;
            }
            iVar3 = xenoRand();
            psVar2 = startBattleAttackAnimationVar5;
            bVar1 = startBattleAttackAnimationVar2;
            if ((iVar3 % 100 <= iVar5) && (currentEntityBattleStats->m16 != '\x02')) {
                startBattleAttackAnimationVar2 = startBattleAttackAnimationVar4;
                uVar4 = (uint)startBattleAttackAnimationVar4;
                startBattleAttackAnimationVar5 = battleGetSlotStatusSub_currentBattleEntity;
                startBattleAttackAnimationVar4 = bVar1;
                battleGetSlotStatusSub_currentBattleEntity = psVar2;
                currentEntityBattleStats = &partyBattleStats[uVar4].m0[20];
                initBattleAttackStatusArray0[uVar4] = 7;
                damageDonePerAttack[startBattleAttackAnimationVar2] = 0;
                battleTickMain_var1 = 0x34;
            }
        }
    }
}

u32 startBattleAttackAnimation() {
    initBattleAttackStatus();

    startBattleAttackAnimationVar0 = 0;
    startBattleAttackAnimationVar1 = 0;
    startBattleAttackAnimationVar2 = performAttackSub3_attacker;

    battleGetSlotStatusSub_currentBattleEntity = &battleEntities[performAttackSub3_attacker].m0_base;
    battleGetSlotStatusSub_currentBattleEntityGear = &battleEntities[performAttackSub3_attacker].mA4_gear;

    if ((battleEntities[performAttackSub3_attacker].m15A_flags & 0x80) == 0) {
        if (performAttackSub3_attacker < 3) {
            currentEntityBattleStats = &partyBattleStats[performAttackSub3_attacker].m0[battleGetSlotStatusSub_current28Index];
        }
        else {
            currentEntityBattleStats = &enemiesBattleStats.m0[battleGetSlotStatusSub_current28Index];
        }
        startBattleAttackAnimationSub0(performAttackSub3_attacker, 1);
        if ((currentEntityBattleStats->mA & 0x10) == 0) {
            startBattleAttackAnimationVar3 = 0;
            if (((currentEntityBattleStats->mA & 0x100) != 0) &&
                (battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 1)) {
                assert(0);
            }
            if ((battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 4) &&
                (battleGetSlotStatusSub_current28Index - 4 < 2)) {
                assert(0);
            }
            if ((((battleGetSlotStatusSub_currentBattleEntity->m80 & 0x20) != 0) &&
                ((entitiesHitInCurrentAttackBF & 7) != 0)) && (entitiesHitInCurrentAttackBF < 7)) {
                assert(0);
            }
            for (startBattleAttackAnimationVar4 = 0; startBattleAttackAnimationVar4 < 11; startBattleAttackAnimationVar4++) {
                u16 bitflag = 1 << startBattleAttackAnimationVar4;
                if ((bitflag & entitiesHitInCurrentAttackBF) != 0) {
                    startBattleAttackAnimationVar5 = &battleEntities[startBattleAttackAnimationVar4].m0_base;
                    startBattleAttackAnimationVar6 = &battleEntities[startBattleAttackAnimationVar4].mA4_gear;
                    MissingCode();
                    //startBattleAttackAnimationVar7 = startBattleAttackAnimationVar4 * 0x170 + -0x7ff331d0;
                    if (startBattleAttackAnimationVar4 < 3) {
                        onPlayerAttackedByEnemy();
                    }
                    startBattleAttackAnimationVar8[currentEntityBattleStats->m16](); // This computes the damage done
                    postComputeAttachDone();
                    if (initBattleAttackStatusArray0[startBattleAttackAnimationVar4] == 0) {
                        if ((currentEntityBattleStats->mA & 0x800) == 0) {
                            MissingCode();
                        }
                        else {
                            MissingCode();
                        }
                    }
                    if ((currentEntityBattleStats->mA & 1) == 0) {
                        performAttack_type = battleGetSlotStatusSub_current28Index;
                    }
                    else {
                        performAttack_type = currentEntityBattleStats->m2;
                    }
                }
            }
            MissingCode();
            if ((battleGetSlotStatusSub_currentBattleEntity->m56_battleCommandLoadout == 4) &&
                (startBattleAttackAnimationVar1 == 0)) {
                assert(0);
            }
            if ((startBattleAttackAnimationVar2 < 3) && (battleGetSlotStatusSub_current28Index < 7) && (battleGetSlotStatusSub_currentBattleEntity->m90[battleGetSlotStatusSub_current28Index] < 65000)) {
                battleGetSlotStatusSub_currentBattleEntity->m90[battleGetSlotStatusSub_current28Index] =
                    battleGetSlotStatusSub_currentBattleEntity->m90[battleGetSlotStatusSub_current28Index] +
                    (ushort)(byte)battleGetSlotStatusSub_currentBattleEntity->m55 +
                    (ushort)(byte)battleGetSlotStatusSub_currentBattleEntity->mA1;
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

void performAttackSub3(s8 param_1, s16 hitEntitiesBF, s16 param_3) {
    performAttackSub3_var0 = 0;
    performAttackSub3_varBitMask = allPlayerCharacterBitmask;
    battleGetSlotStatusSub_current28Index = battleVar2->m2DC + -1;
    entitiesHitInCurrentAttackBF = hitEntitiesBF;
    performAttack_type = param_3;
    performAttackSub3_attacker = param_1;
    startBattleAttackAnimation();
}

void updateMonsterScriptEntitiesVarByAtttack(byte param_1, byte targetId) {
    if (targetId >= 3) { // don't do anything for players
        targetId -= 3;
        monstersScriptsEntities[targetId].m20_s16Vars[7] = characterIdToTargetBitmask(param_1);
        for (int i = 0; i < 5; i++) {
            monstersScriptsEntities[targetId].m30_s8Vars[9 + i] = battleTickMain_var3[i];

            if ((performAttackSub3_var0 & characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget)) == 0) {
                monstersScriptsEntities[targetId].m30_s8Vars[0xe] = 0;
            }
            else {
                monstersScriptsEntities[targetId].m30_s8Vars[0xe] = 1;
            }
        }
    }
}

void setDamageDone(uint param_1)
{
    for (int i = 0; i < 11; i++) {
        battleCurrentDamages[param_1].m0_damageValue[i] = damageDonePerAttack[i];
        battleCurrentDamages[param_1].m18_damageType[i] = initBattleAttackStatusArray0[i];

        switch (initBattleAttackStatusArray0[i]) {
        case -1:
            break;
        case 0:
            switch (battleTickMainSub0Var1[i]) {
            case -1:
                if (damageDonePerAttack[i] - battleTickMainSub0Var0[i] >= 0) {
                    battleTickMainSub0Var0[i] = damageDonePerAttack[i] - battleTickMainSub0Var0[i];
                }
                else {
                    battleTickMainSub0Var0[i] = battleTickMainSub0Var0[i] - damageDonePerAttack[i];
                }
                battleTickMainSub0Var1[i] = 0;
                break;
            }
            break;
        case 2:
            switch (battleTickMainSub0Var1[i]) {
            case -1:
                battleTickMainSub0Var0[i] = damageDonePerAttack[i];
                battleTickMainSub0Var1[i] = initBattleAttackStatusArray0[i];
                break;
            default:
                assert(0);
            }
            break;
        case 4:
            break; // TODO: is this correct?
        case 5:
            switch (battleTickMainSub0Var1[i]) {
            case -1:
                battleTickMainSub0Var0[i] = damageDonePerAttack[i]; // TODO; casting here?
                battleTickMainSub0Var1[i] = initBattleAttackStatusArray0[i];
                break;
            }
            break;
        default:
            assert(0);
            break;
        }

        battleCurrentDamages[param_1].m24[i] = battleTickMainSub0Var0[i];
        battleCurrentDamages[param_1].m3C[i] = battleTickMainSub0Var1[i];
    }
}

void markEnemyDead(uint param_1)
{
    param_1 &= 0xFF;
    uint uVar2 = ((param_1 < 3) ^ 1) << 3;
    if (apConfigArray[param_1].m1 != 0) {
        uVar2 = uVar2 | 0x10;
    }

    auto& slot = battleSlotLayout[battleVisualEntities[param_1].m0_positionSlot + uVar2];

    slot[0]--;
    slot[1] &= characterOdToInverseTargetBitmask(battleVisualEntities[param_1].m1);
    return;
}

bool healWhileOnGear = false;

void applyChangeToHpOrMp(uint param_1) {
    for (int i = 0; i < 11; i++) {
        if (isBattleSlotFilled[i] == 0)
            continue;
        if (battleEntities[i].m0_base.m7C & 0x8000) {
            performAttackSub3_var0 |= characterIdToTargetBitmask(i);
            continue;
        }

        switch (battleCurrentDamages[param_1].m18_damageType[i]) {
        case 0:
        case 5:
        case 7:
        case 8:
            if (battleVisualEntities[i].m4_isGear == 0) {
                if (battleEntities[i].m0_base.m4C_HP - battleCurrentDamages[param_1].m0_damageValue[i] < 1) {
                    battleEntities[i].m0_base.m4C_HP = 0;
                    performAttackSub3_var0 |= characterIdToTargetBitmask(i);
                    battleEntities[i].m0_base.m7C |= 0x8000;
                    if (i > 2) { // if it's an enemy
                        markEnemyDead(i);
                    }
                }
                else {
                    battleEntities[i].m0_base.m4C_HP -= battleCurrentDamages[param_1].m0_damageValue[i];
                }
            }
            else {
                assert(0);
            }
            battleVar2->m2EB[i] = 1;
            break;
        case 2: // healing
            if (!battleVisualEntities[i].m4_isGear || healWhileOnGear) {
                battleEntities[i].m0_base.m4C_HP += battleCurrentDamages[param_1].m0_damageValue[0];
                if (battleEntities[i].m0_base.m4E_MaxHP < battleEntities[i].m0_base.m4C_HP) {
                    battleEntities[i].m0_base.m4C_HP = battleEntities[i].m0_base.m4E_MaxHP;
                }
            }
            else {
                assert(0);
            }
            break;
        case 4:
        case -1:
            break;
        default:
            assert(0);
        }
    }
}

void performAttackSub6(s8 param_1) {
    setDamageDone(param_1);
    applyChangeToHpOrMp(param_1);
    battleVar1->mAD = 0;
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
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -0xd;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget);
        battleVar2->m2DA_indexInBattleVar48++;
    }
    performAttackSub3(param_1, characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget), battleVar2->m2DC - 1);
    int oldVar48 = battleVar2->m2DA_indexInBattleVar48;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = entitiesHitInCurrentAttackBF;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = performAttack_type;
    battleVar2->m2DA_indexInBattleVar48++;
    updateMonsterScriptEntitiesVarByAtttack(param_1, battleVar2->m0[param_1].m3C_currentTarget);
    if ((2 < battleVar2->m0[param_1].m3C_currentTarget) && (executeMonsterScriptWhenAttacked(battleVar2->m0[param_1].m3C_currentTarget) != 0))
    {
        returnValue = 1;
    }
    performAttackSub6(oldVar48);
    return returnValue;
}
void removeBattleCursorDuringAttack(u8 param_1) {
    if (battleVar2->m2E9 == 0) {
        battleVar2->m2E9 = 1;
        updateCharacterBlinkingTask(0);
        battleVar2->m0[param_1].m3C_currentTarget = battleVar2->m2E8_currentTarget;
        freeBattleVar1_AE();
        freeBattleVar1_96();
        removeTargetSelectionCursors();

        setCameraVisibleEntities((characterIdToTargetBitmask(param_1) | characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget)) & 0xffff);

        if (battleCharacters[param_1] != 4) {
            battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
            battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -3;
            battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m3A = 0;
            battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m16_targetBitMask = characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget);
            battleVar2->m2DA_indexInBattleVar48 = battleVar2->m2DA_indexInBattleVar48 + 1;
        }
        removeBillyWindow(param_1, 0);
    }
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
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
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
        case battleInputDirection::BDIR_RIGHT: // right
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, battleInputDirection::BDIR_RIGHT);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case battleInputDirection::BDIR_DOWN: // down
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, battleInputDirection::BDIR_DOWN);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case battleInputDirection::BDIR_LEFT: // left
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, battleInputDirection::BDIR_LEFT);
            initAnimSeqFromCharacterToCharacter(battleVar2->m2D3_currentEntityTurn, battleVar2->m2E8_currentTarget);
            battleSoundEffect(0x4c);
            break;
        case battleInputDirection::BDIR_UP: // up
            battleVar2->m2E8_currentTarget = selectNewSlotByDirection(battleVar2->m2E8_currentTarget, battleInputDirection::BDIR_UP);
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
        removeBattleCursorDuringAttack(param_1);
        if (battleVar1->mCB == '\0') {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackSelectEnemy;
            battleVar2->m2E2_previousActiveBattleMenu = eActiveBattleMenu::Menu_AttackSelectEnemy;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackSelectEnemy_bis;
            battleVar2->m2E2_previousActiveBattleMenu = eActiveBattleMenu::Undefined;
            battleVar2->m2E0_isBattleMenuDisabled = 1;
        }
        battleVar2->m2D4_remainingAP = battleVar2->m2D4_remainingAP - battleVar2->m2DF_currentAttackNumOfApUsed;
        updateAPCounterDisplay();
        MissingCode();
        battleVar2->m2E5 = performAttack(param_1, battleVar2->m2DF_currentAttackNumOfApUsed);
        MissingCode();
        battleVar2->m2E3 = 1;
    }
    if (battleVar2->m2D4_remainingAP == 0) {
        handleMenuSelectEnemy_cancel_sub0(param_1);
        battleVar2->m2E1_waitBeforeNextAttackInput = 1;
    }
    if (battleVar2->m2E5 != 0) {
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

void setAPMarksPositions(uint max_ap, byte remaining_ap) {
    max_ap = max_ap & 0xff;
    if (max_ap == remaining_ap) {
        remaining_ap = remaining_ap - 1;
    }

    for (int i = 0; i < max_ap - 1; i++) {
        MissingCode();
    }

    battleVar1->m97 = remaining_ap;
    battleVar1->m98 = battleOddOrEven;
}

u8 setupTurnRenderLoopVar0;

void initBattleRenderStruct2(void) {
    pBattleRenderStruct2 = new sBattleRenderStruct2;
    pBattleRenderStruct2->m5D9C = 0xA0;
    pBattleRenderStruct2->m5D9E = 100;
}

void resetBattleVar2_2CC(void)
{
    for (int i = 0; i < 7; i++) {
        battleVar2->m2CC[i] = -1;
    }
    battleVar2->m2D6 = 0;
}

void clearEntityFlag15A_1(int param_1) {
    battleEntities[param_1].m15A_flags &= ~1;
}

void dummyFunction(void)
{
    return;
}

void setupTurnRenderLoop(int param_1) {
    battleVar2->m2EA = 0;
    battleVar2->m2E9 = 0;
    battleVar2->m2DE = 0;
    battleVar2->m2E0_isBattleMenuDisabled = 0;
    battleVar2->m2E1_waitBeforeNextAttackInput = 0;
    battleVar2->m2E2_previousActiveBattleMenu = eActiveBattleMenu::Undefined;
    battleVar2->m2E3 = 0;
    battleVar2->m2E4 = 0;
    battleVar2->m2DC = 0;

    battleVar2->m2D5_maxAP = apConfigArray[param_1].m4_currentAP;
    battleVar2->m2D4_remainingAP = apConfigArray[param_1].m4_currentAP;

    battleVar1->m90_perPCTimeBarStatus[param_1] = 3;
    battleVar2->m2E7 = 0;
    newBattleInputButton2 = -1;
    setupTurnRenderLoopVar0 = 0;
    while (battleVar1->m90_perPCTimeBarStatus[param_1] != 1) {
        battleRenderDebugAndMain();
    }
    startCharacterJumpToEnemyVar0 = 1;
    battleSoundEffect2(0x5a);

    startCharacterJumpToEnemyVar0 = 0;

    for (int i = 0; i < 16; i++) {
        battleVar2->m0[param_1].m1C_isCommandEnabled[i] = battleEntities[param_1].m0_base.m7A_commandEnabledBF & party1C_InitialValues[i];
    }

    if ((battleCharacters[param_1] == 7) && (apConfigArray[param_1].m1 != 0)) {
        battleVar2->m0[param_1].m1C_isCommandEnabled[0xd] = party1C_InitialValues[13];
        battleVar2->m0[param_1].m1C_isCommandEnabled[4] = party1C_InitialValues[4];
    }
    if (battleCharacters[param_1] == 4) {
        assert(0);
    }
    if (apConfigArray[param_1].m1 == 0) {
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_AttackHighlighted;
            setupTurnRenderLoop_menuVar = 0;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ChiHighlighted;
            setupTurnRenderLoop_menuVar = 3;
        }
        if ((battleEntities[param_1].m0_base.m7C & 2) != 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_ItemHighlighted;
            setupTurnRenderLoop_menuVar = 1;
        }
        initBattleRenderStruct2();
    }
    else {
        if (battleVar2->m0[param_1].m1C_isCommandEnabled[9] == 0) {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_16;
            setupTurnRenderLoop_menuVar = 0;
        }
        else {
            battleVar2->m2DD_currentActiveBattleMenu = eActiveBattleMenu::Menu_19;
            setupTurnRenderLoop_menuVar = 3;
        }
        assert(0);
        MissingCode();
    }
    resetBattleVar2_2CC();
    clearEntityFlag15A_1(param_1);
    setCameraVisibleEntities((characterIdToTargetBitmask(param_1) | characterIdToTargetBitmask(battleVar2->m0[param_1].m3C_currentTarget)));

    int characterToUpdate = battleVar2->m0[param_1].m3C_currentTarget;
    if (characterToUpdate == -1) {
        characterToUpdate = 0;
    }
    updateCharacterBlinkingTask(characterIdToTargetBitmask(characterToUpdate));

    if (((battleVar2->m2DE == '\0') && (battleRunningVar1 == '\0')) && (battleVar2->m2DB == '\0')) {
        while (1) {
            if (apConfigArray[param_1].m1 == 0) {
                setAPMarksPositions(battleVar2->m2D5_maxAP, battleVar2->m2D4_remainingAP);
            }
            if (battleVar2->m2E2_previousActiveBattleMenu != battleVar2->m2DD_currentActiveBattleMenu) {
                battleVar2->m2F6 = 0;
                battleVar2->m2E2_previousActiveBattleMenu = battleVar2->m2DD_currentActiveBattleMenu;
                battleVar1->mCB = 0;
                battleVar1->mA3 = updateBattleMenuSpriteForMenu(param_1, battleVar2->m2DD_currentActiveBattleMenu, battleVar2->m2E0_isBattleMenuDisabled);
                battleVar1->mCB = 1;
            }
            if ((battleInputButton == -1) || (battleRenderDebugAndMain(), battleVar2->m2DB != '\0'))
                break;
            if (battleVar2->m2E0_isBattleMenuDisabled == '\0') {
                startCharacterJumpToEnemyVar0 = 1;
            }
            switch (battleVar2->m2DD_currentActiveBattleMenu) {
            case eActiveBattleMenu::Menu_AttackHighlighted:
                drawCircleMenuAttackActive(param_1);
                break;
            case eActiveBattleMenu::Menu_ItemHighlighted:
                drawCircleMenuItemActive(param_1);
                break;
            case eActiveBattleMenu::Menu_DefendHighlighted:
                drawCircleMenuDefend(param_1);
                break;
            case eActiveBattleMenu::Menu_ChiHighlighted:
                drawCircleMenuChi(param_1);
                break;
            case eActiveBattleMenu::Menu_ComboHighlighted:
                drawCircleMenuCombo(param_1);
                break;
            case eActiveBattleMenu::Menu_ItemAltHighlighted:
                drawCircleMenuItemSecondaryActive(param_1);
                break;
            case eActiveBattleMenu::Menu_EscapeHighlighted:
                drawBattleMenuEscape(param_1);
                break;
            case eActiveBattleMenu::Menu_AttackSelectEnemy:
                handleMenuSelectEnemy(param_1);
                break;
            case eActiveBattleMenu::Menu_CallGear:
                drawCircleMenuCallGear(param_1);
                break;
            case eActiveBattleMenu::Menu_AttackSelectEnemy_bis:
                battleVar2->m2E2_previousActiveBattleMenu = eActiveBattleMenu::Undefined;
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

    if (battleVar2->m2E3) {
        apConfigArray[param_1].m0 += battleVar2->m2D4_remainingAP;
        if (apConfigArray[param_1].m0 > 0x1C) {
            apConfigArray[param_1].m0 = 0x1C;
        }
    }

    startCharacterJumpToEnemyVar0 = 0;
    dummyFunction();
    if ((battleVar2->m2DE != '\0') || (battleRunningVar1 != '\0')) {
        assert(0);
    }

    numTicksBeforeReady[param_1] = battleSlotStatusVar0[param_1];
    battleVar1->m7C[param_1] = 1;
    startPlayingAttackAnimationVar0 = 0;
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

s8 getEntityToFace(u8 param_1) {
    numValidTarget = 0;

    std::array<s8, 12> array1;
    std::array<s8, 12> array2;
    for (int i = 0; i < 12; i++) {
        array2[i] = -1;
        targetsPerPriority[i] = -1;
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
                array2[i] = -1; // so that we don't count it twice
                *(array1It++) = 1;
            }
        }
    }

    // add leftovers
    {
        auto targetsPerPriorityIt = targetsPerPriority.begin();
        for (int i = 0; i < numValidTarget; i++) {
            if (array2[i] != -1) {
                *(targetsPerPriorityIt++) = array2[i];
            }
        }
    }

    if (array1[0] == 0) {
        for (int i = 1; i < numValidTarget; i++) {
            if (battleEntities[targetsPerPriority[i]].m0_base.m4C_HP < battleEntities[targetsPerPriority[0]].m0_base.m4C_HP) {
                std::swap<s8>(targetsPerPriority[0], targetsPerPriority[i]);
            }
        }
    }
    else if (numValidTarget > 1) {
        for (int i = 1; i < numValidTarget; i++) {
            if (array1[i] != 0) {
                if (battleEntities[targetsPerPriority[i]].m0_base.m4C_HP < battleEntities[targetsPerPriority[0]].m0_base.m4C_HP) {
                    std::swap<s8>(targetsPerPriority[0], targetsPerPriority[i]);
                }
            }
        }
    }

    return targetsPerPriority[0];
}

bool getDirectionBetween2BattleEntities(s8 param_1, s8 param_2)
{
    if ((param_1 != -1) && (param_2 != -1)) {
        return battleVisualEntities[param_2].mA_X < battleVisualEntities[param_1].mA_X;
    }
    else {
        Hack("Prevented out of bound access");
        return false;
    }
}

void finishMonsterTurn(byte param_1) {
    if (apConfigArray[param_1].m1 != 0) {
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
        battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = 0x1b;
        battleVar2->m2DA_indexInBattleVar48 = battleVar2->m2DA_indexInBattleVar48 + 1;
    }
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m47_battleAnimationToPlay = -2;
    startCharacterJumpToEnemyVar0 = 0;
}

void monsterJumpToTarget(byte param_1) {
    battleVar2->m2DA_indexInBattleVar48 = 0;
    battleTickMainSub0();
    performAttackSub2();
    battleCurrentDamages[battleVar2->m2DA_indexInBattleVar48].m23_battleEntityIndex = param_1;
    u32 actor = (uint)param_1;
    battleRenderDebugAndMain();
    int facing = computeFacingForJump(actor);
    startJumpAnimation(1, actor, 0, facing);
    executeBattleCode(actor);
    finishMonsterTurn(actor);
    while (battleVar2->m2DB == '\0') {
        battleRenderDebugAndMain();
    }
}

void loadMonsterAttackName() {
    MissingCode();
}

void displayAndWaitMonsterAttackName() {
    MissingCode();
}

void removeCurrentMonsterNameString() {
    MissingCode();
}

extern bool needBattleEventOverlay;

void loadBattleEventOverlay() {
    if (needBattleEventOverlay) {
        MissingCode();
        battleEventEntry();
    }
}

void updateBattleEventOverlay(int param) {
    if (needBattleEventOverlay != '\0') {
        battleEvent_update(param);
    }
}

void execMonsterScriptEndOfPlayerTurn() {
    MissingCode();
}

void updateRunningAnimations() {
    MissingCode();
}

void copyBattleVar1_m7C(void) {
    for (int i = 0; i < 3; i++) {
        battleVar1->m7C[i] = battleVar2->m2EB[i];
    }
    battleRenderDebugAndMain();
}

u16 battleEffectBF = 0;

void applyBattleEffectBF7() {
    if (battleEffectBF & 7) {
        assert(0);
    }
    battleEffectBF = 0;
}

void applyPassiveModifiersAtEndOfTurn(byte param_1){
    MissingCode();
}
u32 battleGetSlotStatusSub(u32 param_1);

void updateTicksEndOfTurn() {
    sBattleVar2* psVar1;
    uint uVar2;

    if (isEntityReadyForBattle[battleVar2->m2D3_currentEntityTurn] != -1) {
        isEntityReadyForBattle[battleVar2->m2D3_currentEntityTurn] = 0;
    }
    uVar2 = battleGetSlotStatusSub((uint)battleVar2->m2D3_currentEntityTurn);
    psVar1 = battleVar2;
    numTicksBeforeReady[battleVar2->m2D3_currentEntityTurn] = (short)uVar2;
    battleSlotStatusVar0[psVar1->m2D3_currentEntityTurn] =
        numTicksBeforeReady[psVar1->m2D3_currentEntityTurn];
}

void clearAnimationsEndOfTurnSub() {
    MissingCode();
}

void waitBattleAnimationSoundLoaded() {
    if (!battleAnimationSoundLoaded) {
        battleIdleDuringLoading();
        setCurrentDirectory(0x2c, 0);
        //sVar2 = getFileSizeAligned(5);
        sWdsFile wdsFile;
        readFile(5, wdsFile, 0, 0x80);
        battleIdleDuringLoading();
        sWdsFile* psVar3 = findInWdsLinkedList(wdsFile.m20_bankNumber);
        if (psVar3 == nullptr) {
            loadWdsDataIfNeeded();
            loadWdsDataNeeded = loadWds(wdsFile, 0);
            while (waitForMusic(0) != 0) {
                battleRender();
            }
            battleAnimationSoundLoaded = '\x01';
            startJumpAnimationVar3 = 0;
        }
    }
}

void clearAnimationsEndOfTurn(int param_1) {
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 0;
    clearAnimationsEndOfTurnSub();
    if (battleVisualEntities[param_1].m4_isGear == 0) {
        if (param_1 < 3) {
            if (battleSpriteActorCores[param_1] != (sSpriteActorCore*)0x0) {
                startReadingBattleJumpAnimation(battleSpriteActorCores[param_1]);
            }
        }
        else {
            clearBattleAnimationData();
        }
    }
    else {
        clearBattleAnimationData();
        waitBattleAnimationSoundLoaded();
    }
    setBattleCameraParamX(0xc0);
}

void loadImageSync(RECT* param_1, std::vector<u16>& param_2)
{
    LoadImage(param_1, (u8*)param_2.data());
    DrawSync(0);
    return;
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

    entitiesHitInCurrentAttackBF = 0;
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
                if (battleVar2->m2EA != 0) {
                    assert(0);
                }
                previousEntityTurn.m0 = battleVar2->m2D3_currentEntityTurn;
                battleVar1->m97 = 0;
                updateCharacterBlinkingTask(0);
            }
        }

        loadMonsterAttackName();
        displayAndWaitMonsterAttackName();
        removeCurrentMonsterNameString();
        updateBattleEventOverlay(0);

        for (int i = 0; i < 8; i++) {
            if (bitmaskCharacterCheck(entitiesHitInCurrentAttackBF, i + 3)) {
                unknownMonsterStatus0[i].m2 = 1;
            }
        }

        execMonsterScriptEndOfPlayerTurn();
    }
    else {
        if (param_1 == '\0') {
            executeMonsterScript(currentEntityTurn, battleEntities[currentEntityTurn].m0_base.m80 & 0x2000);

            if (((battleVisualEntities[battleVar2->m2D3_currentEntityTurn].m3 & 0x80) == 0) &&
                ((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m34 & 0x400U) == 0)) {
                // Display the enemy's name
                battleVar1->mB0_isDialogWindowInitialized[5] = 1;
                std::vector<u8>::iterator pString = getDialogParamPointer(currentBattleSpecificStrings, battleMonsterMapping[battleVar2->m2D3_currentEntityTurn]);
                previousEntityTurn.m8[0].m5E_currentMonsterNameStringWidth = renderString(pString, *previousEntityTurn.m8[0].m58_ramBufferFromMonsterName, 0x39, 0);
                loadImageSync(&previousEntityTurn.m8[0].m50_monsterNameVRamRect, *previousEntityTurn.m8[0].m58_ramBufferFromMonsterName);
                previousEntityTurn.m8[0].m5D_monsterNameIsDisplayed = 1;
            }
        }
        if (((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m7C & 0x2080) == 0) &&
            ((battleEntities[battleVar2->m2D3_currentEntityTurn].m0_base.m80 & 0x1000) == 0)) {
            monsterJumpToTarget(battleVar2->m2D3_currentEntityTurn);
            removeCurrentMonsterNameString();
            loadMonsterAttackName();
            displayAndWaitMonsterAttackName();
        }
        removeCurrentMonsterNameString();
        updateBattleEventOverlay(0);
    }
    copyBattleVar1_m7C();
    applyBattleEffectBF7();
    battleVar0->m6415 = 0;
    for (int i = 0; i < 0xB; i++) {
        battleVar2->m0[i].m3C_currentTarget = getEntityToFace(i);
        battleVisualEntities[i].m6_direction = getDirectionBetween2BattleEntities(currentEntityTurn, battleVar2->m0[i].m3C_currentTarget);
    }

    clearAnimationsEndOfTurn(computeFacingForJump(battleVar2->m2D3_currentEntityTurn));
    resetBattleVar2_m2EB();
    checkWinConditions();
    if (battleRunningVar1 == '\0') {
        applyPassiveModifiersAtEndOfTurn(battleVar2->m2D3_currentEntityTurn);
    }
    checkWinConditions();
    copyBattleVar1_m7C();
    updateRunningAnimations();
    updateTicksEndOfTurn();

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
        uint uVar2 = (uint)bBattleTickMode1;
        bBattleTickMode1 = 0;
        battleSlotStatusVar0[uVar2 + 10] = 0;
        battleTickMain(0);
    }
    else {
        assert(0);
    }
}


void computeMenuBorder(sFont& param_1, int param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7, int* param_8);

void init8920() {
    MissingCode();
    computeMenuBorder(battleFont, 0x4b, &battleVar0->mA234[1].m0, &battleVar0->mA234[1].m4_tpage_tp, &battleVar0->mA234[1].m8_clutX, &battleVar0->mA234[1].mC_clutY, &battleVar0->mA234[1].m10_tpage_X, &battleVar0->mA234[1].m14_tpage_Y);
    computeMenuBorder(battleFont, 0x50, &battleVar0->mA234[2].m0, &battleVar0->mA234[2].m4_tpage_tp, &battleVar0->mA234[2].m8_clutX, &battleVar0->mA234[2].mC_clutY, &battleVar0->mA234[2].m10_tpage_X, &battleVar0->mA234[2].m14_tpage_Y);
    computeMenuBorder(battleFont, 0x4d, &battleVar0->mA234[3].m0, &battleVar0->mA234[3].m4_tpage_tp, &battleVar0->mA234[3].m8_clutX, &battleVar0->mA234[3].mC_clutY, &battleVar0->mA234[3].m10_tpage_X, &battleVar0->mA234[3].m14_tpage_Y);
    computeMenuBorder(battleFont, 0x4e, &battleVar0->mA234[4].m0, &battleVar0->mA234[4].m4_tpage_tp, &battleVar0->mA234[4].m8_clutX, &battleVar0->mA234[4].mC_clutY, &battleVar0->mA234[4].m10_tpage_X, &battleVar0->mA234[4].m14_tpage_Y);
    battleVar1->m0.y = 0;
    battleVar1->m0.x = 0;
    battleVar1->m0.h = 0x100;
    battleVar1->m0.w = 0x100;

    battleVar1->m8.x = (battleVar0->mA234[0].m10_tpage_X & 0x3F) * 2;
    battleVar1->m8.y = battleVar0->mA234[0].m14_tpage_Y;
    battleVar1->m8.h = 0x100;
    battleVar1->m8.w = 0x100;

    battleVar0->mA234[1].m10_tpage_X = 0x3c0;
    battleVar0->mA234[2].m10_tpage_X = 0x3c8;

    MissingCode();
    battleVar0->mA234[1].m14_tpage_Y = 0x34;
    battleVar0->mA234[2].m14_tpage_Y = 0x34;
    MissingCode();

    (battleVar1->m8).x = ((battleVar0->mA234[0].m10_tpage_X & 0x3f) << 1);
    (battleVar1->m8).w = 0x100;
    (battleVar1->m8).h = 0x100;
    (battleVar1->m8).y = battleVar0->mA234[0].m14_tpage_Y;

    MissingCode();

    SetDrawMode(&battleVar0->m8908_drMode[0], 0, 0, GetTPage(battleVar0->mA234[0].m4_tpage_tp, 0, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y), &battleVar1->m8);
    SetDrawMode(&battleVar0->m8908_drMode[1], 0, 0, GetTPage(battleVar0->mA234[0].m4_tpage_tp, 0, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y), &battleVar1->m8);
    SetDrawMode(&battleVar0->m8920[0], 0, 0, GetTPage(battleVar0->mA234[0].m4_tpage_tp, 0, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y), &battleVar1->m0);
    SetDrawMode(&battleVar0->m8920[1], 0, 0, GetTPage(battleVar0->mA234[0].m4_tpage_tp, 0, battleVar0->mA234[0].m10_tpage_X, battleVar0->mA234[0].m14_tpage_Y), &battleVar1->m0);
}

void initPolyMonsterName(sMonsterTurnSub* param_1, int index) {
    for (int i = 0; i < 2; i++) {
        POLY_FT4* p = &param_1->m0_polys[i];
        SetPolyFT4(p);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        SetSemiTrans(p, 0);
        SetShadeTex(p, 1);
        param_1->m5C = index & 1;
        p->clut = param_1->m5C ? textSpriteMode1 : textSpriteMode0;
        p->tpage = GetTPage(0, 0, 0x3c0, (index / 2) * 0xd);
    }
    param_1->m5D_monsterNameIsDisplayed = 0;
}

void setupStringsForMonsterNames() {
    allocateAndSetupBattleDialogWindow(5, 8, 0x2a, 0x70, 0x12, 0, 0);
    battleVar1->mB0_isDialogWindowInitialized[5] = 0;

    allocateAndSetupBattleDialogWindow(4, 0x20, 200, 0xf4, 0x12, 0, 0);
    battleVar1->mB0_isDialogWindowInitialized[4] = 0;

    for (int i = 0; i < 8; i+=2) {
        sRamTexture* pRamTexture = allocateTextureRamForText(0x39);
        previousEntityTurn.m8[i].m58_ramBufferFromMonsterName = pRamTexture;
        previousEntityTurn.m8[i+1].m58_ramBufferFromMonsterName = previousEntityTurn.m8[i].m58_ramBufferFromMonsterName;

        previousEntityTurn.m8[i].m50_monsterNameVRamRect.x = 0x3C0;
        previousEntityTurn.m8[i].m50_monsterNameVRamRect.y = (i / 2) * 0xD;
        previousEntityTurn.m8[i].m50_monsterNameVRamRect.w = 0x3C;
        previousEntityTurn.m8[i].m50_monsterNameVRamRect.h = 0xD;
        previousEntityTurn.m8[i+1].m50_monsterNameVRamRect = previousEntityTurn.m8[i].m50_monsterNameVRamRect;

        initPolyMonsterName(&previousEntityTurn.m8[i], i);
        initPolyMonsterName(&previousEntityTurn.m8[i+1], i+1);
    }
}

sSoundInstance* realStartBattleMusic(sSeqFile* param_1, int param_2, int param_3) {
    sSoundInstance* psVar1;

    psVar1 = createMusicInstance(param_1);
    startMusicInstance(psVar1, param_2, param_3);
    return psVar1;
}

void battleMain() {

#if 0
    randSeed = 0x73D63221;
#endif

    battleVar0 = new sBattleVar0;
    battleVar1 = new sBattleVar1;
    battleVar2 = new sBattleVar2;

    memset(battleVar0, 0, sizeof(sBattleVar0));
    memset(battleVar1, 0, sizeof(sBattleVar1));
    memset(battleVar2, 0, sizeof(sBattleVar2));

    battleDebugDisplay = 0;
    newBattleInputButton2 = 0xff;
    previousNewBattleInputButton = 0xff;
    startCharacterJumpToEnemyVar0 = 0;
    pCurrentBattleMusic = pMusic;

    // TODO: more logic here
    if (battleInitVar0 == '\0') {
        if (battleInitVar1 != '\0') goto LAB_Battle__80071064;
    }
    else
    {
        LAB_Battle__80071064:
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
    battleCreateFader(0, 2, 0xff, 0xff, 0xff);
    if (!needBattleEventOverlay) {
        battleCreateFader(0x14, 2, 0, 0, 0);
    }
    if (battleInitVar1 != 0) {
        pCurrentBattleMusic = realStartBattleMusic(&battleMusic, 0x7f, 0);
    }
    battleLoadDataVar2Bis = battleLoadDataVar2;
    battleLoadDataVar2Ter = battleLoadDataVar2;
    init8920();
    makeBattleTimeProgress = 1;
    setCameraVisibleEntities(allPlayerCharacterBitmask);
    battleRenderDebugAndMain();
    setupStringsForMonsterNames();
    // Wait for battle intro to finish and battle time to start
    while (battleTimeEnabled == '\0') {
        battleRenderDebugAndMain();
    }

    // This would cause a use after free
    //battleStartSeq.clear();
    if (battleTransitionEffect != 4) {
        stopSequence(&battleStartSeq);
    }
    unloadSequence(&battleStartSeq);

    MissingCode();

    loadBattleEventOverlay();
    updateBattleEventOverlay(1);
    if (isBattleAnEvent == '\0') {
        currentBattleMode = 1;
    }

    MissingCode();

    for (int i = 0; i < 3; i++) {
        if (battleCharacters[i] != 0x7F) {
            MissingCode();
        }
    }

    struct sBattleRunningVar0* battleRunningVar0Temp = battleEventVar0;

    while (!battleRunningVar1 && !isBattleAnEvent) {
        if (battleTimeEnabled != '\0') {
            battleTickGameplay();
        }
        battleRenderDebugAndMain();
    }
    if (((!needBattleEventOverlay) && ((battleRunningVar1 & 0x40) == 0)) &&
        ((currentBattleConfig.m1_flags & 8) == 0)) {
        battleCreateFader(0x40, 2, 0x40, 0x40, 0x40);
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

// compute the direction and step for the jump
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

void createDamageDisplayIfValid(int param_1) {
    if (battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m18_damageType[param_1] != -1) {
        if ((battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m0_damageValue[param_1] != -1) && battleSpriteActorCores[param_1]) {
            damageDisplayVar0 = battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m24[param_1];
            damageDisplayVar1 = battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m3C[param_1];
            createDamageDisplay(battleSpriteActorCores[param_1],
                battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m0_damageValue[param_1],
                battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m18_damageType[param_1]);
            battleCurrentDamages[allocateJumpAnimationStructVar0 + -1].m0_damageValue[param_1] = -1;
        }
    }
}

void createAllDamageDisplays() {
    bool bVar1 = false;
    for (int i = 0; i < 11; i++) {
        if (battleCurrentDamages[allocateJumpAnimationStructVar0 - 1].m18_damageType[i] == 7) {
            bVar1 = true;
        }
        if (battleSpriteActorCores[i]) {
            if ((processBattleAnimationSub0_var1 != battleSpriteActorCores[i]) || (!bVar1)) {
                createDamageDisplayIfValid(i);
            }
        }
    }
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

void idleBattleDuringLoading(void)
{
    int iVar1;

    while (iVar1 = isCDBusy(), iVar1 != 0) {
        battleRenderDebugAndMain();
    }
    return;
}

void battleSpriteEffect_20_update(sTaskHeader* param_1)
{
    MATRIX MStack_50;
    SVECTOR local_30;
    SVECTOR local_28;
    VECTOR local_20;

    sSpriteActorCore* psVar2 = (sSpriteActorCore*)param_1->m4;
    OP_INIT_ENTITY_SCRIPT_sub0Sub9(psVar2);
    local_28.vy = (short)((psVar2->mC_step).vy >> 0xd);
    local_28.vx = 0;
    local_28.vz = (short)((psVar2->mC_step).vz >> 0xd);
    local_30.vx = modulateSpeed(psVar2, (psVar2->mC_step).vx >> 0xd);
    local_30.vy = 0;
    local_30.vz = 0;
    createRotationMatrix(&local_28, &MStack_50);
    ApplyMatrix(&MStack_50, &local_30, &local_20);
    local_20.vx = local_20.vx + (psVar2->mA0).vx;
    local_20.vy = local_20.vy + (psVar2->mA0).vy;
    local_20.vz = local_20.vz + (psVar2->mA0).vz;
    (psVar2->m0_position).vx = local_20.vx * 0x10000;
    (psVar2->m0_position).vy = local_20.vy * 0x10000;
    auto puVar1 = psVar2->m64_spriteByteCode;
    (psVar2->m0_position).vz = local_20.vz * 0x10000;
    if (puVar1.has_value()) {
        param_1->mC_deleteCallback(param_1);
    }
    return;
}

void battleSetupTextPolySub(POLY_FT4* param_1)
{
    SetSemiTrans(param_1, 1);
    SetShadeTex(param_1, 0);
}

void battleSpriteEffect_20(sSpriteActorCore* param_1)
{
    param_1->m34_currentSpriteFrame = 1;
    setTaskUpdateFunction(param_1->m6C_pointerToOwnerStructure, battleSpriteEffect_20_update);
    return;
}

struct sBattleSpriteEffect_3A_sub_entity : public sTaskHeaderPair {
    s32 m38;
    s32 m3C;
    s16 m40;
    s16 m42;
    s16 m44;
    s16 m46;
    s16 m48;
    s16 m4A;
    s16 m4C;
    s16 m4E;
};

sBattleSpriteEffect_3A_sub_entity* battleSpriteEffect_3A_sub_entity = nullptr;

void sBattleSpriteEffect_3A_sub_entity_update(sTaskHeader* param_1_)
{
    sBattleSpriteEffect_3A_sub_entity* param_1 = (sBattleSpriteEffect_3A_sub_entity*)param_1_;
    int iVar1;
    int iVar2;

    if (param_1->m3C == 0) {
        if (param_1->m46 == 0) {
            param_1->mC_deleteCallback(param_1);
        }
    }
    else {
        iVar1 = param_1->m3C + -1;
        iVar2 = ((int)param_1->m42 - (int)param_1->m40) * ((iVar1 * 0x20) / param_1->m38);
        param_1->m3C = iVar1;
        if (iVar2 < 0) {
            iVar2 = iVar2 + 0x1f;
        }
        param_1->m46 = param_1->m42 - (short)(iVar2 >> 5);
    }
    return;
}

void sBattleSpriteEffect_3A_sub_entity_delete(sTaskHeader* param_1_)
{
    sBattleSpriteEffect_3A_sub_entity* param_1 = (sBattleSpriteEffect_3A_sub_entity*)param_1_;
    registerSpriteCallback2Sub0(&param_1->m1C);
    allocateSavePointMeshDataSub0_callback(param_1);
    delete param_1;
    battleSpriteEffect_3A_sub_entity = nullptr;
    //FUN_Battle__800a6f98();
    MissingCode();
    return;
}

void sBattleSpriteEffect_3A_sub_entity_draw(sTaskHeader* param_1_)
{
    sBattleSpriteEffect_3A_sub_entity* param_1 = (sBattleSpriteEffect_3A_sub_entity*)param_1_;
    MissingCode();
}

void battleSpriteEffect_3A_sub(short param_1, int param_2, short param_3, short param_4, short param_5, short param_6) {
    if (battleSpriteEffect_3A_sub_entity == nullptr) {
        battleSpriteEffect_3A_sub_entity = createCustomRenderableEntity<sBattleSpriteEffect_3A_sub_entity>(0x50, nullptr,
            sBattleSpriteEffect_3A_sub_entity_update,
            sBattleSpriteEffect_3A_sub_entity_draw,
            sBattleSpriteEffect_3A_sub_entity_delete);

        MissingCode();

        battleSpriteEffect_3A_sub_entity->m40 = 0;
        battleSpriteEffect_3A_sub_entity->m44 = 0;
        battleSpriteEffect_3A_sub_entity->m46 = 0;
    }
    else {
        battleSpriteEffect_3A_sub_entity->m40 = battleSpriteEffect_3A_sub_entity->m46;
    }
    battleSpriteEffect_3A_sub_entity->m42 = param_1;
    battleSpriteEffect_3A_sub_entity->m38 = param_2 << 1;
    battleSpriteEffect_3A_sub_entity->m3C = param_2 << 1;
    battleSpriteEffect_3A_sub_entity->m4A = param_4;
    battleSpriteEffect_3A_sub_entity->m4C = param_5;
    battleSpriteEffect_3A_sub_entity->m4E = param_6;
    battleSpriteEffect_3A_sub_entity->m48 = param_3;
    sBattleSpriteEffect_3A_sub_entity_update(battleSpriteEffect_3A_sub_entity);
}

void battleSpriteEffect_2D(sSpriteActorCore* param_1, std::span<u8>::iterator param_2) {
    std::span<u8>::iterator dataLocation = param_2 + READ_LE_S16(param_2);
    param_1->m0_position.vx = ((int)READ_LE_S16(dataLocation + 0)) << 0x10;
    param_1->m0_position.vy = ((int)READ_LE_S16(dataLocation + 2)) << 0x10;
    param_1->m0_position.vz = ((int)READ_LE_S16(dataLocation + 4)) << 0x10;
}

void battleSpriteEffect_2E(SVECTOR* param_1, std::span<u8>::iterator param_2) {
    std::span<u8>::iterator dataLocation = param_2 + READ_LE_S16(param_2);
    param_1->vx = ((int)READ_LE_S16(dataLocation + 0));
    param_1->vy = ((int)READ_LE_S16(dataLocation + 2));
    param_1->vz = ((int)READ_LE_S16(dataLocation + 4));
}

void battleSpriteEffect_3A(sSpriteActorCore* param_1, std::span<u8>::iterator param_2) {
    std::span<u8>::iterator dataLocation = param_2 + READ_LE_S16(param_2);
    battleSpriteEffect_3A_sub(dataLocation[5], dataLocation[3], dataLocation[4], (int)*dataLocation, (int)dataLocation[1], (int)dataLocation[2]);
}

void battleSpriteEffect_40(sSpriteActorCore* param_1, std::span<u8>::iterator param_2) {
    VECTOR local_80;
    local_80.vx = param_1->mA0.vx - param_1->m0_position.vx.getIntegerPart();
    local_80.vy = param_1->mA0.vy - param_1->m0_position.vy.getIntegerPart();
    local_80.vz = param_1->mA0.vz - param_1->m0_position.vz.getIntegerPart();

    VECTOR local_70;
    Square0(&local_80, &local_70);

    int sqRoot = SquareRoot0(local_70.vx + local_70.vz);
    int atanVal1 = ratan2(local_80.vz, local_80.vx);
    int atanVal2 = ratan2(local_80.vy, (int)(short)sqRoot);

    VECTOR local_60;
    local_60.vx = (param_1->mC_step).vx >> 7;
    local_60.vy = (param_1->mC_step).vy >> 7;
    local_60.vz = (param_1->mC_step).vz >> 7;
    Square0(&local_60, &local_70);

    sqRoot = SquareRoot0(local_70.vy + local_70.vx + local_70.vz);
    int sqRoot2 = SquareRoot0(local_70.vx + local_70.vz);
    int atanVal3 = ratan2(local_60.vz, local_60.vx);

    SVECTOR local_88;
    local_88.vy = -(short)atanVal3;
    int atanVal4 = ratan2(local_60.vy, (int)(short)sqRoot2);
    local_88.vx = 0;

    byte extraArg = (uint)*param_2;
    int iVar2 = (int)(((uint)(ushort)-(short)atanVal1 - (uint)(ushort)local_88.vy) * 0x100000) >> 0x14;
    int iVar3 = extraArg * 4;
    int iVar1 = iVar2;
    if (iVar2 < 0) {
        iVar1 = -iVar2;
    }
    int iVar4 = iVar2;
    if ((iVar3 < iVar1) && (iVar4 = iVar3, iVar2 < 0)) {
        iVar4 = extraArg * -4;
    }
    local_88.vy = local_88.vy + (short)iVar4;
    iVar2 = (int)(((atanVal2 & 0xffff) - (atanVal4 & 0xffff)) * 0x100000) >> 0x14;
    iVar1 = iVar2;
    if (iVar2 < 0) {
        iVar1 = -iVar2;
    }
    iVar4 = iVar2;
    if ((iVar3 < iVar1) && (iVar4 = iVar3, iVar2 < 0)) {
        iVar4 = extraArg * -4;
    }
    local_88.vz = (short)atanVal4 + (short)iVar4;
    SVECTOR SStack_50;
    setupSVector(&SStack_50, (short)sqRoot, 0, 0);
    MATRIX MStack_48;
    createRotationMatrix(&local_88, &MStack_48);
    VECTOR local_28;
    ApplyMatrix(&MStack_48, &SStack_50, &local_28);
    (param_1->mC_step).vx = local_28.vx << 7;
    (param_1->mC_step).vy = local_28.vy << 7;
    (param_1->mC_step).vz = local_28.vz << 7;
}


void battleSpriteEffect_5_draw(sTaskHeader* param_1) {
    sSpriteActorCore* pSprite = (sSpriteActorCore*)param_1->m4;
    if (pSprite->m34_currentSpriteFrame == 0) {
        if ((shapeTransfertTableCurrentEntry + sizeof(TILE) < shapeTransfertTableEnd)) {
            TILE* pDestTile = (TILE*)shapeTransfertTableCurrentEntry;
            shapeTransfertTableCurrentEntry += sizeof(TILE);

            SVECTOR tempVector;
            tempVector.vx = pSprite->m0_position.vx.getIntegerPart();
            tempVector.vy = pSprite->m0_position.vy.getIntegerPart();
            tempVector.vz = pSprite->m0_position.vz.getIntegerPart();
            

            MATRIX* m;
            if (((pSprite->m3C >> 24) & 1) == 0) {
                m = &currentRenderingMatrix;
            }
            else {
                assert(0);
            }

            SetRotMatrix(m);
            SetTransMatrix(m);
            long dummy;
            s32 depth = RotTransPers(&tempVector, &pDestTile->x0y0, &dummy, &dummy);
            int otDepth = depth >> (gDepthDivider & 0x1f);
            pSprite->m2E = otDepth;
            tempVector.vx -= pSprite->mC_step.vx >> ((pSprite->m36 + 4) & 0x1F);
            tempVector.vy -= pSprite->mC_step.vy >> ((pSprite->m36 + 4) & 0x1F);
            tempVector.vz -= pSprite->mC_step.vz >> ((pSprite->m36 + 4) & 0x1F);

            RotTransPers(&tempVector, &pDestTile->wh, &dummy, &dummy);
            pDestTile->m3_size = 3;
            pDestTile->m_colorAndCode = pSprite->m28_colorAndCode;
            AddPrim(&(*characterRenderingOT)[otDepth], pDestTile);

            if ((shapeTransfertTableCurrentEntry + sizeof(DR_TPAGE) < shapeTransfertTableEnd)) {
                int transparency = ((pSprite->m3C& 0xFF) >> 5); // TODO: correct?
                if (transparency) {
                    DR_TPAGE* pDestRdTPage = (DR_TPAGE*)shapeTransfertTableCurrentEntry;
                    shapeTransfertTableCurrentEntry += sizeof(DR_TPAGE);

                    pDestRdTPage->m3_size = 1;
                    pDestRdTPage->m_rawCode = (transparency - 1 & 3) << 5 | 0xe1000000;
                    AddPrim(&(*characterRenderingOT)[otDepth], pDestRdTPage);
                }
            }
        }
    }
}

void battleSpriteEffect_5(sSpriteActorCore* param_1) {
    param_1->m34_currentSpriteFrame = 1;
    setTaskDrawFunction(&param_1->m6C_pointerToOwnerStructurePair->m1C, battleSpriteEffect_5_draw);
    param_1->m28_colorAndCode.m3_code = 0x40;
}

void battleSpriteEffect_23(sSpriteActorCore* param_1)
{
    short sVar1;
    sVec2_s16 sVar2;
    sVec2_s16 local_20;

    sVar2.vy = param_1->m0_position.vz.getIntegerPart();
    sVar2.vx = param_1->m0_position.vx.getIntegerPart();
    local_20.vy = (param_1->mA0).vz;
    local_20.vx = (param_1->mA0).vx;
    sVar1 = vec2dRatan2(local_20, sVar2);
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, sVar1);
    setSpriteActorAngle(param_1, sVar1);
}

void battleSpriteEffect_1C(sSpriteActorCore* param_1) {
    VECTOR difference;
    difference.vx = param_1->mA0.vx - param_1->m0_position.vx.getIntegerPart();
    difference.vy = param_1->mA0.vy - param_1->m0_position.vy.getIntegerPart();
    difference.vz = param_1->mA0.vz - param_1->m0_position.vz.getIntegerPart();

    VECTOR differenceSquare;
    Square0(&difference, &differenceSquare);

    SVECTOR newDirection;
    newDirection.vx = 0;
    newDirection.vy = -ratan2(difference.vz, difference.vx);
    newDirection.vz = ratan2(difference.vy, SquareRoot0(differenceSquare.vx + differenceSquare.vz));

    SVECTOR newVector;
    setupSVector(&newVector, (short)((uint)(param_1->m18_moveSpeed << 9) >> 0x10), 0, 0);

    MATRIX rotationMatrix;
    createRotationMatrix(&newDirection, &rotationMatrix);
    VECTOR transformedVector;
    ApplyMatrix(&rotationMatrix, &newVector, &transformedVector);

    (param_1->mC_step).vx = transformedVector.vx << 7;
    (param_1->mC_step).vy = transformedVector.vy << 7;
    (param_1->mC_step).vz = transformedVector.vz << 7;
}

void battleSpriteEffect(sSpriteActorCore* param_1, s8 param_2, std::span<u8>::iterator endOfOpcode) {
    switch (param_2) {
    case 5:
        battleSpriteEffect_5(param_1);
        return;
    case 7:
        setCameraVisibleEntities(1 << ((param_1->m74_pTargetEntitySprite->mAC.mx0_entityIdUpper2bit) << 2 | (uint)param_1->m74_pTargetEntitySprite->mA8.mx1E_entityId_bottom2bit));
        break;
    case 9:
        param_1->m32_direction += 0x800;
        param_1->mC_step.vx -= param_1->mC_step.vx;
        param_1->mC_step.vy -= param_1->mC_step.vy;
        param_1->mC_step.vz -= param_1->mC_step.vz;
        return;
    case 0xe:
        OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, (*endOfOpcode) << 4);
        return;
    case 0x17:
    {
        int iVar3 = (char)*endOfOpcode * 4;
        int iVar11 = param_1->mC_step.vx * iVar3;
        if (iVar11 < 0) {
            iVar11 = iVar11 + 0xff;
        }
        int iVar12 = param_1->mC_step.vy * iVar3;
        param_1->mC_step.vx = iVar11 >> 8;
        if (iVar12 < 0) {
            iVar12 = iVar12 + 0xff;
        }
        iVar3 = param_1->mC_step.vz * iVar3;
        param_1->mC_step.vy = iVar12 >> 8;
        if (iVar3 < 0) {
            iVar3 = iVar3 + 0xff;
        }
        param_1->mC_step.vz = iVar3 >> 8;
        return;
    }
    case 0x1C:
        battleSpriteEffect_1C(param_1);
        return;
    case 0x1f:
        param_1->m3C = param_1->m3C & 0xfbffffff;
        savePointCallback8Sub0Sub0_battle(param_1);
        return;
    case 0x20:
        battleSpriteEffect_20(param_1);
        return;
    case 0x23:
        battleSpriteEffect_23(param_1);
        return;
    case 0x28:
    {
        int iVar11 = (char)*endOfOpcode * 0x10 * param_1->m82;
        if (iVar11 < 0) {
            iVar11 = iVar11 + 0xfff;
        }
        param_1->mC_step.vx = (iVar11 >> 0xc) << 8;
        return;
    }
    case 0x2D:
        battleSpriteEffect_2D(param_1, endOfOpcode);
        return;
    case 0x2E:
        battleSpriteEffect_2E(&param_1->mA0, endOfOpcode);
        return;
    case 0x31:
        SetGeomOffset(0xa0, 0x70);
        return;
    case 0x32:
        SetGeomOffset(0xa0, 0xa4);
        return;
    case 0x3a:
        battleSpriteEffect_3A(param_1, endOfOpcode);
        return;
    case 0x40:
        battleSpriteEffect_40(param_1, endOfOpcode);
        return;
    case 0x55:
        setCameraVisibleEntities(1 << ((processBattleAnimationSub0_var1->mAC.mx0_entityIdUpper2bit) << 2 |(uint)processBattleAnimationSub0_var1->mA8.mx1E_entityId_bottom2bit) | (uint)currentJumpAnimationBitMask);
        return;
    case 0x56:
        MissingCode(); // sound related
        break;
    case 0x64:
        param_1->mB0.mx9 = 1;
        if (battleCameraVar1 < 0x201) {
            return;
        }
        param_1->m3A = battleCameraVar1;
        param_1->m40 = param_1->m40 & 0xffffe0ff | 0x300;
        param_1->m3C |= 0x10000000;
        break;
    case 0x67:
    {
        param_1->m9E_wait++;
        VECTOR local_30;
        local_30.vx = (int)battleCameraEyeTarget.vx - (int)battleCameraEye.vx;
        local_30.vy = (int)battleCameraEyeTarget.vy - (int)battleCameraEye.vy;
        local_30.vz = (int)battleCameraEyeTarget.vz - (int)battleCameraEye.vz;
        Square0(&local_30, &local_30);
        VECTOR local_20;
        local_20.vx = (int)battleCameraAtTarget.vx - (int)battleCameraAt.vx;
        local_20.vy = (int)battleCameraAtTarget.vy - (int)battleCameraAt.vy;
        local_20.vz = (int)battleCameraAtTarget.vz - (int)battleCameraAt.vz;
        Square0(&local_20, &local_20);
        int lVar2 = SquareRoot0(local_30.vx + local_30.vy + local_30.vz);
        if ((lVar2 < 4) && (lVar2 = SquareRoot0(local_20.vx + local_20.vy + local_20.vz), lVar2 < 4)) {
            return;
        }
        param_1->m64_spriteByteCode.value() -= 2;
        return;
    }
    default:
        assert(0);
    }
}

FP_VEC3 previousCameraEye2;
FP_VEC3 previousCameraAt2;
SVECTOR battleCameraEyeTarget_backup;
SVECTOR battleCameraAtTarget_backup;
byte spriteBytecode2ExtendedE0_Sub0_10_battle_var0 = 0;

void spriteBytecode2ExtendedE0_Sub0_10_battle_delete(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteCore = (sSpriteActorCore*)param_1->m4;
    if ((pSpriteCore->m40 >> 0xd & 0xf) == 10) {
        if (battleCameraModeCallback1 != param_1)
            goto LAB_Battle__800bbfa0;
        battleCameraModeCallback1 = nullptr;
        if (battleCameraVar0)
            goto LAB_Battle__800bbfa0;
        battleCameraEyeTarget = battleCameraEyeTarget_backup;
    }
    else {
        if (battleCameraModeCallback1 != param_1)
            goto LAB_Battle__800bbfa0;
        battleCameraModeCallback1 = nullptr;
        if (battleCameraVar0)
            goto LAB_Battle__800bbfa0;
        battleCameraAtTarget = battleCameraAtTarget_backup;
    }

LAB_Battle__800bbfa0:
    if ((pSpriteCore->mAC.mx5) != 0) {
        registerSpriteCallback2_2(param_1);
    }
    allocateSavePointMeshDataSub0_callback(param_1);
    registerSpriteCallback2Sub0(&((sTaskHeaderPair*)param_1)->m1C);
    delete param_1;
    spriteBytecode2ExtendedE0_Sub0_10_battle_var0--;
    if (spriteBytecode2ExtendedE0_Sub0_10_battle_var0 == 0) {
        setBattleCameraMode(backupCameraMode);
    }
}

void spriteBytecode2ExtendedE0_Sub0_10_battle_update(sTaskHeader* param_1) {
    sSpriteActorCore* pSpriteCore = (sSpriteActorCore*)param_1->m4;
    OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSpriteCore);
    savePointCallback8Sub0(pSpriteCore);
    if ((pSpriteCore->m40 >> 0xd & 0xf) == 10) {
        previousCameraEye2 = pSpriteCore->m0_position;
    }
    else {
        previousCameraAt2 = pSpriteCore->m0_position;
    }
    if (pSpriteCore->m64_spriteByteCode.has_value()) {
        if (pSpriteCore->mAC.mx6 == 0) {
            return;
        }
        OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSpriteCore);
        savePointCallback8Sub0(pSpriteCore);
        if ((pSpriteCore->m40 >> 0xd & 0xf) == 10) {
            previousCameraEye2 = pSpriteCore->m0_position;
        }
        else {
            previousCameraAt2 = pSpriteCore->m0_position;
        }
        if (pSpriteCore->m64_spriteByteCode.has_value()) {
            return;
        }
    }
    param_1->mC_deleteCallback(param_1);
}

void spriteBytecode2ExtendedE0_Sub0_10_battle(sSavePointMeshAbstract* param_1) {
    assert(isBattleOverlayLoaded);
    if (((param_1->m38_spriteActorCore.m40 >> 0xD) & 0xF) == 10) {
        if (battleCameraModeCallback1 == nullptr) {
            battleCameraEyeTarget_backup = battleCameraEyeTarget;
            battleCameraModeCallback1 = param_1;
            goto LAB_Battle__800bc27c;
        }
        if ((battleCameraModeCallback1->m38_spriteActorCore.m34_currentSpriteFrame != 1) && (param_1->m38_spriteActorCore.m34_currentSpriteFrame == 1)) {
            battleCameraModeCallback1->mC_deleteCallback(battleCameraModeCallback1);
            battleCameraModeCallback1 = param_1;
            goto LAB_Battle__800bc27c;
        }
        assert(0);
    }
    else {
        if (battleCameraModeCallback2 == nullptr) {
            battleCameraModeCallback2 = param_1;
            battleCameraAtTarget_backup.vx = battleCameraAtTarget.vx;
            battleCameraAtTarget_backup.vy = battleCameraAtTarget.vy;
            battleCameraAtTarget_backup.vz = battleCameraAtTarget.vz;
            goto LAB_Battle__800bc27c;
        }
        assert(0);
    }
    assert(0);

LAB_Battle__800bc27c:
    spriteBytecode2ExtendedE0_Sub0_10_battle_var0++;
    if (((param_1->m38_spriteActorCore).mAC.mx2_facing) == 0) {
        (param_1->m38_spriteActorCore).m32_direction = 0;
    }
    else {
        (param_1->m38_spriteActorCore).m32_direction = 0x800;
    }
    setTaskDeleteFunction(param_1, spriteBytecode2ExtendedE0_Sub0_10_battle_delete);
    setTaskUpdateFunction(param_1, spriteBytecode2ExtendedE0_Sub0_10_battle_update);
    setBattleCameraMode(2);
}

struct sBattleSpriteOpcode_FB_Task : public sTaskHeader {
    // size: 0x1C + 0x18
    sSpriteActorCore* m1C;
    s32 m24;
    s32 m28;
    s32 m2C;
    std::span<u8>::iterator m30;
};

s32 battleSpriteOpcode_FB_sub0(sSpriteActorCore* param_1)
{
    VECTOR local_28;
    VECTOR local_18;

    local_28.vx = (int)(param_1->mA0).vx - (int)(param_1->m0_position).vx.getIntegerPart();
    local_28.vy = (int)(param_1->mA0).vy - (int)(param_1->m0_position).vy.getIntegerPart();
    local_28.vz = (int)(param_1->mA0).vz - (int)(param_1->m0_position).vz.getIntegerPart();
    Square0(&local_28, &local_18);
    return SquareRoot0(local_18.vx + local_18.vz + local_18.vy);
}

void battleSpriteOpcode_FB_taskUpdate(sTaskHeader* param_1_) {
    bool bVar1;
    int iVar2;
    int iVar4;
    sSpriteActorCore* psVar5;
    sBattleSpriteOpcode_FB_Task* param_1 = (sBattleSpriteOpcode_FB_Task*)param_1_;
    bVar1 = false;
    psVar5 = param_1->m1C;
    iVar4 = param_1->m28;
    iVar2 = battleSpriteOpcode_FB_sub0(psVar5);
    param_1->m28 = iVar2;
    if ((iVar4 < iVar2) || (iVar2 < param_1->m2C)) {
        bVar1 = true;
        auto puVar3 = param_1->m30;
        psVar5->m9E_wait = 1;
        psVar5->m64_spriteByteCode = puVar3;
    }
    if (psVar5->m9E_wait == 0) {
        bVar1 = true;
    }
    if (psVar5->mAC.mx18 != param_1->m24) {
        bVar1 = true;
    }
    if (bVar1) {
        param_1->mC_deleteCallback(param_1);
    }
}

void battleSpriteOpcode_FB(sSpriteActorCore* param_1, int param_2, std::span<u8>::iterator param_3) {
    sBattleSpriteOpcode_FB_Task* pTask = battleLoaderAllocateMainBattleSprite<sBattleSpriteOpcode_FB_Task>(param_1->m6C_pointerToOwnerStructure, 0x18);
    setTaskUpdateFunction(pTask, battleSpriteOpcode_FB_taskUpdate);

    pTask->m1C = param_1;
    pTask->m28 = battleSpriteOpcode_FB_sub0(param_1);
    pTask->m2C = param_2;
    pTask->m30 = param_3;
    pTask->m24 = param_1->mAC.mx18;
    param_1->mAC.mx5 = 1;
}

s16 setupMechaForEventVar0 = 0;
s16 setupMechaForEventVar1 = 0;

u32 setupMechaForEventSub0(void) {
    uint uVar1;

    uVar1 = 0;
    do {
        if (((int)(uint)setupMechaForEventVar1 >> (uVar1 & 0x1f) & 1U) != 0) {
            return uVar1;
        }
        uVar1 = uVar1 + 1;
    } while ((int)uVar1 < 0xd);
    return uVar1;
}

int battleGetMechaBitfieldForAnim(sLoadedMechas* param_1, int target, u16* result) {
    int index;
    switch (target) {
    case 0xFF:
        index = setupMechaForEventSub0();
        break;
    case 0xFD:
    case 0xF9:
        index = param_1->m20_mechaEntryId;
        break;
    default:
        assert(0);
    }

    *result = 1 << (index& 0x1F);
    return index;
}

int getBattleSlotLayout(int index) {
    return battleSlotLayout[battleVisualEntities[index].m0_positionSlot + 0x18][0];
}

void executeSpriteBytecode2_battle(sSpriteActorCore* param_1) {
#define IMPLEMENT_BATTLE_SPECIFIC_CASES
#include "kernel/spriteVM.h"
}

void setCurrentDir_20_0(void)
{
    setCurrentDirectory(0x20, 0);
    return;
}

void setupMechaForEventSub1(int param_1, uint param_2)
{
    sSpriteActorCore* psVar1;

    psVar1 = battleSpriteActorCores[param_1];
    if (psVar1 != (sSpriteActorCore*)0x0) {
        currentJumpAnimationBitMask = (u16)(1 << (param_2 & 0x1f));
        processBattleAnimationSub0_var1 = psVar1;
        psVar1->m74_pTargetEntitySprite = battleSpriteActorCores[param_2];
        pSpriteCoreListHead[0] = battleSpriteActorCores[param_2];
        pSpriteCoreListHead[1] = nullptr;
    }
    return;
}

void setupMechaForEvent(s16 param_1, s16 param_2, int animationIndex) {
    battleMechaVar3 = 1;
    setupMechaForEventVar0 = param_1;
    setupMechaForEventVar1 = param_2;
    battleMechas[param_1]->m35 = 1;
    allocateSavePointMeshDataSub0_var0 = 0;
    spriteBytecode2ExtendedE0_Var0 = 1;
    setupMechaForEventSub1(param_1, setupMechaForEventSub0());
    allocateJumpAnimationStructVar0 = 1;
    battleCurrentDamages[0].m18_damageType[setupMechaForEventSub0()] = 0;
    if (battleMechas[param_1]) {
        initMechaAnimation(battleMechas[param_1], battleMechas[param_1], &battleMechaInitVar2, animationIndex);
    }
}

int mecha_battle_op3() {
    return allocateSavePointMeshDataSub0_var0 - isDamageDisplayPolysTask2Running();
}

void mechaPlayAnimation_battle(ushort mechaId, short mechaId2, int animationId) {
    setupMechaForEventVar0 = mechaId2;
    setupMechaForEventVar1 = mechaId;
    battleMechas[mechaId]->m35 = 0;
    if (battleMechas[mechaId]) {
        initMechaAnimation(battleMechas[mechaId], battleMechas[mechaId], &battleMechaInitVar2, animationId);
    }
}