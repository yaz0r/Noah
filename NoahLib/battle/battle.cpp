#include "noahLib.h"
#include "battle.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battleRenderContext.h"
#include "kernel/memory.h"
#include "battleStartEffect.h"
#include "battleConfig.h"
#include "field/mecha/mechaOverlay.h"
#include "kernel/gameMode.h"

s8 battleDebugDisplay = 0;
s8 requestedBattleConfig = 0;

sMechaDataTable1* battleLoadDataVar0;
std::vector<u8>* battleLoadDataVar0_raw;
std::vector<u8>::iterator battleLoadDataVar1;
std::vector<u8>::iterator battleLoadDataVar2;

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

struct sBattleVar0 {
    // size 0xA2B4
}*battleVar0 = nullptr;

struct sBattleVar1 {
    // size 0x10C
}*battleVar1 = nullptr;

struct sBattleVar2 {
    // size 0x2F8
}*battleVar2 = nullptr;

s8 battleTransitionEffect = 0;
s8 currentBattleLoaderTransitionEffect;

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

std::vector<u8> battleStartSeq;
std::vector<u8> battleCharacterConfigFile;
std::vector<u8> battleLoaderBinary;

s32 battleMechaVar0;
s16 battleMechaVar1;
s8 battleMechaVar2;
s8 battleMechaVar3;
s16 battleMechaVar4;
s8 battleMechaVar5;

sMechaInitVar2 battleMechaInitVar2;
sMechaInitVar3 battleMechaInitVar3;
std::vector<u8>::iterator battleMechaInitData;

void mechaInitForBattle() {
    battleMechaVar0 = 0;
    battleMechaVar1 = 0;
    battleMechaVar2 = 0;
    battleMechaVar3 = 0;
    battleMechaVar4 = 0;
    battleMechaVar5 = 1;
    initMechaInitVar2(&battleMechaInitVar2, READ_LE_S16(battleMechaInitData + 0x348));
    initMechaInitVar3(&battleMechaInitVar3, READ_LE_S16(battleMechaInitData + 0x34A));

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
        assert(0);
    }

    waitReadCompletion(0);
    mechaInitForBattle();

    Hack("Deserialize mecha data for battle env");
    battleLoadDataVar0 = new sMechaDataTable1(*battleLoadDataVar0_raw);

    battleRenderStructs[0].m0_drawEnv.isbg = loadBattleEnvironment(battleLoadDataVar2, battleLoadDataVar1, battleLoadDataVar0, &battleMatrix800CCB94, &battleMatrix800CCBB4, &battleRenderStructs[0].m0_drawEnv.color);

    MissingCode();
}

void battleMain() {
    battleVar0 = new sBattleVar0;
    battleVar1 = new sBattleVar1;
    battleVar2 = new sBattleVar2;

    memset(battleVar0, 0, sizeof(sBattleVar0));
    memset(battleVar1, 0, sizeof(sBattleVar1));
    memset(battleVar2, 0, sizeof(sBattleVar2));

    MissingCode();

    currentBattleConfig = battleConfigs[requestedBattleConfig];
    startBattleLoader(battleTransitionEffect);

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