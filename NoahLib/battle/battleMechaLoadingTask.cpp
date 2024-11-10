#include "noahLib.h"
#include "battleMechaLoadingTask.h"
#include "kernel/taskLists.h"
#include "kernel/memory.h"
#include "battleSpriteLoader.h"
#include "battle.h"
#include "field/mecha/mechaOverlay.h"

struct sBattleMechaLoaderTask : public sTaskHeader {
    s32 m1C;
};

u8 preventGearLoadingInBattle = 0;

std::array<std::array<s8, 2>,4> battleGearLoadTable1 = { { // incomplete
    {1, 0},
    {3, 0},
    {5, 6},
    {0xD, 0x0},
} };

u16 battleMechaPartyMask = 0;

std::vector<sLoadingBatchCommands> battleGearLoadCommands;
std::array<std::array<s16, 2>, 3> battleMechaPartyTextureLocations = { {
    {0x340, 0},
    {0x2C0, 0x100},
    {0x300, 0x100},
} };

void finalizeBattleMechaLoadingCommands(int param_1, short U, short V, short param_4, short param_5) {
    mechaInitEnvironmentMechaMesh(param_1, 0, (sMechaDataTable2*)battleGearLoadCommands[1].m4_loadPtr, (sMechaDataTable1*)battleGearLoadCommands[0].m4_loadPtr, U, V, param_4, param_5, 0);
    battleMechas[param_1]->m38 = 1;
    battleMechas[param_1]->m22 = 1;
    if (battleGearLoadCommands[2].m4_loadPtr) {
        assert(0);
    }

    // FIX: this was the other way around and would have caused a use after free
    delete battleGearLoadCommands[0].m4_loadPtr;
    battleGearLoadCommands.clear();
    DrawSync(0);
}

int initMechaForBattleSub0(int param_1)
{
    int iVar1;
    sLoadedMechas* psVar2;

    psVar2 = battleMechas[param_1];
    iVar1 = 0;
    if (psVar2 != (sLoadedMechas*)0x0) {
        iVar1 = (int)psVar2->m24[0] * ((int)psVar2->m1C_scale * (int)psVar2->m4_bones->at(0).m4C_scale[1] >> 0xc) >>
            0xc;
    }
    return iVar1;
}

void battleMechaUpdate(sTaskHeader*) {
    MissingCode();
}

void battleMechaDraw(sTaskHeader*) {
    MissingCode();
}

void battleMechaDelete(sTaskHeader*) {
    MissingCode();
}

void initMechaForBattle(int param_1) {
    auto backup = spriteBytecode2ExtendedE0_Var0;

    if (battleSpriteActorCores[param_1] == nullptr) {
        spriteBytecode2ExtendedE0_Var0 = 0;
        sBattleSpriteActor* pMecha = createCustomRenderableEntity<sBattleSpriteActor>(0x19c, nullptr, battleMechaUpdate, battleMechaDraw, battleMechaDelete);
        pMecha->m38_spriteActor.m6C_pointerToOwnerStructure = pMecha;
        pMecha->m4 = &pMecha->m38_spriteActor;
        pMecha->m1C.m4 = &pMecha->m38_spriteActor;
        initFieldEntitySub4Sub1(&pMecha->m38_spriteActor);
        initFieldEntitySub4Sub2(&pMecha->m38_spriteActor);

        pMecha->m38_spriteActor.m40 = pMecha->m38_spriteActor.m40 & 0xfffe1fff | 0x8000;
        pMecha->m38_spriteActor.m6C_pointerToOwnerStructure = pMecha;
        pMecha->m38_spriteActor.m3C &= 0xfffffffc;
        pMecha->m38_spriteActor.mA8.mx0 = 0;
        pMecha->m38_spriteActor.m7C->m8 = 0;

        pMecha->m38_spriteActor.m7C->mC = 0;
        pMecha->m38_spriteActor.m82 = battleDefaultEntityScale;

        pMecha->m38_spriteActor.m0_position.vx = (int)battleVisualEntities[param_1].mA_X << 0x10;
        pMecha->m38_spriteActor.m0_position.vy = 0;
        pMecha->m38_spriteActor.m0_position.vz = (int)battleVisualEntities[param_1].mC_Z << 0x10;

        pMecha->m38_spriteActor.m36 = initMechaForBattleSub0(param_1);

        pMecha->m38_spriteActor.m82 = 0x2000;
        pMecha->m38_spriteActor.m38 = pMecha->m38_spriteActor.m36 >> 1;

        setGraphicEntityScale(&pMecha->m38_spriteActor, 0x2000);
        (pMecha->m38_spriteActor).m0_spriteActorCore.m24_vramData = &createSavePointMeshData_mode5;
        battleSpriteActorCores[param_1] = &pMecha->m38_spriteActor;
        battleSpriteActors[param_1] = pMecha;

        pMecha->m38_spriteActor.m4C_specialAnimation = nullptr;
        pMecha->m38_spriteActor.m48_defaultAnimationbundle = nullptr;
        spriteBytecode2ExtendedE0_Var0 = backup;
        (pMecha->m38_spriteActor).m0_spriteActorCore.mA8.mx1E_entityId_bottom2bit = param_1 & 3;
        pMecha->m38_spriteActor.mAC.mx0_entityIdUpper2bit = (param_1 >> 2) & 3;
    }
}

s8 finalizeBattleMechaLoadingVar0 = 0;

void finalizeBattleMechaLoading(sBattleMechaLoaderTask* param_1) {
    ushort uVar1;
    int iVar2 = 0;
    u32 uVar3 = 1;
    do {
        uVar1 = battleMechaPartyMask | (ushort)uVar3;
        if ((battleMechaPartyMask & uVar3) == 0) break;
        iVar2 = iVar2 + 1;
        uVar3 = uVar3 << 1;
        uVar1 = battleMechaPartyMask;
    } while (iVar2 != 3);
    battleMechaPartyMask = uVar1;

    finalizeBattleMechaLoadingCommands(param_1->m1C, battleMechaPartyTextureLocations[iVar2][0], battleMechaPartyTextureLocations[iVar2][1], 0, param_1->m1C + 0x1c0);
    preventGearLoadingInBattle = preventGearLoadingInBattle + -1;
    initMechaForBattle(param_1->m1C);
    param_1->mC_deleteCallback(param_1);
    mainBattleSpriteCallback_phase5Var = mainBattleSpriteCallback_phase5Var + -1;
    if (mainBattleSpriteCallback_phase5Var == 0) {
        finalizeBattleMechaLoadingVar0 = 1;
    }
}

void mechaLoadingTask_phase1(sTaskHeader* param_1) {
    if (!isCDBusy()) {
        finalizeBattleMechaLoading((sBattleMechaLoaderTask*)param_1);
    }
}

void createGearLoadingFileRequests(int battleEntityIndex) {
    std::array<int, 2> currentDir;
    getCurrentDirectory(&currentDir[0], &currentDir[1]);
    setCurrentDirectory(0x28, 1);
    resetMemoryAllocStats(4, 0);

    s32 gearIndex = battleEntities[battleEntityIndex].m0_base.mA0_partyData_gearNum;
    s32 gearExtraFileIndex = battleEntities[battleEntityIndex].mA4_gear.m5C_extraFileIndex;
    if (battleGearLoadTable1[gearIndex][1] < gearExtraFileIndex) {
        gearExtraFileIndex = 0;
    }

    battleGearLoadCommands.resize(4);
    auto it = battleGearLoadCommands.begin();
    it->m0_fileIndex = battleGearLoadTable1[gearIndex][0] + 1;
    it->m4_loadPtr = new sMechaDataTable1;
    it++;
    it->m0_fileIndex = battleGearLoadTable1[gearIndex][0] + 2;
    it->m4_loadPtr = new sMechaDataTable2;
    it++;
    if (gearExtraFileIndex) {
        it->m0_fileIndex = battleGearLoadTable1[gearIndex][0] + 2 + gearExtraFileIndex;
        it->m4_loadPtr = new sLoadableDataRaw;
        it++;
    }
    it->m0_fileIndex = 0;
    it->m4_loadPtr = nullptr;
    batchStartLoadingFiles(battleGearLoadCommands.data(), 0);
    setCurrentDirectory(currentDir[0], currentDir[1]);
}

void startBattleGearLoading(sBattleMechaLoaderTask* param_1) {
    createGearLoadingFileRequests(param_1->m1C);
    preventGearLoadingInBattle++;
    setTaskUpdateFunction(param_1, mechaLoadingTask_phase1);
}

void mechaLoadingTask_phase0(sTaskHeader* param_1) {
    if (!isCDBusy() && !preventGearLoadingInBattle) {
        startBattleGearLoading((sBattleMechaLoaderTask*)param_1);
    }
}

void createMechaLoadingTask(int param_1) {
    u8 uVar1 = spriteBytecode2ExtendedE0_Var0;
    spriteBytecode2ExtendedE0_Var0 = 0;
    battleScriptLoaderTaskCreationInProgress = 1;
    sBattleMechaLoaderTask* psVar2 = battleLoaderAllocateMainBattleSprite<sBattleMechaLoaderTask>(nullptr, 4);
    setTaskUpdateFunction(psVar2, mechaLoadingTask_phase0);
    psVar2->m1C = param_1;
    battleScriptLoaderTaskCreationInProgress = 0;
    mainBattleSpriteCallback_phase5Var++;
    spriteBytecode2ExtendedE0_Var0 = uVar1;
}