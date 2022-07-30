#include "noahLib.h"
#include "battleSpriteLoader.h"
#include "field/fieldGraphicObject.h"
#include "kernel/filesystem.h"

bool battleScriptLoaderTaskCreationInProgress = 0;

sBattleSpriteConfigs battleConfigFile3;

struct sBattleSpriteLoaderTask {
    sTaskHeader m0;
    sBattleSpriteConfigs* m20;
};

void defaultBattleSpriteDeleteCallback(sTaskHeader* param_1) {
    assert(0);
}

template <typename T>
T* battleLoaderAllocateMainBattleSprite(sSavePointMesh1* param_1, int param_2)
{
    T* psVar1;

    psVar1 = new T;
    allocateSavePointMeshDataSub0(param_1, &psVar1->m0);
    psVar1->m0.mC = defaultBattleSpriteDeleteCallback;
    psVar1->m0.m4 = nullptr;
    return psVar1;
}

void loadBattleSpriteVram(sBattleSpriteConfigs* param_1) {
    MissingCode();
}

void mainBattleSpriteCallback_phase1(sTaskHeader* param_1) {
    sBattleSpriteLoaderTask* pTask = (sBattleSpriteLoaderTask*)param_1;

    if (isCDBusy() == 0) {
        loadBattleSpriteVram(pTask->m20);
        MissingCode();
    }
}

void createBattleSpriteLoadingTaskSub(sBattleSpriteConfigs* param_1)
{
    sBattleSpriteLoaderTask* psVar1;

    psVar1 = battleLoaderAllocateMainBattleSprite<sBattleSpriteLoaderTask>(0, 0x78);
    regCallback8(&psVar1->m0, mainBattleSpriteCallback_phase1);
    psVar1->m20 = param_1;
    return;
}

void createBattleSpriteLoadingTask(sBattleSpriteConfigs* param_1) {
    battleScriptLoaderTaskCreationInProgress = 1;
    createBattleSpriteLoadingTaskSub(param_1);
    battleScriptLoaderTaskCreationInProgress = 0;
}
