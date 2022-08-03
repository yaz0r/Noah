#include "noahLib.h"
#include "battleSpriteLoader.h"
#include "field/fieldGraphicObject.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battle/battle.h"
#include "field/field.h"

bool battleScriptLoaderTaskCreationInProgress = 0;

sBattleSpriteConfigs battleConfigFile3;

struct sBattleSpriteLoaderTask {
    sTaskHeader m0;
    sBattleSpriteConfigs* m20;
    std::array<sLoadingBatchCommands, 4> m30_loadingCommands;
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

void createEnemiesBattleSprites(sBattleSpriteConfigs* param_1) {
    MissingCode();
}

short battleSpriteX = 0;

std::array<u8, 12> battleSpriteWidthPerCharacter = { {
        0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10
} };

struct sBattleSpriteActor {
    sTaskHeader m0;
    sTaskHeader m1C;
    sSpriteActor m38_spriteActor;
};

std::array<sBattleSpriteActor*, 11> battleSpriteActors;
std::array<sSpriteActorCore*, 11> battleSpriteActorCores;

template <typename T>
T* createCustomRenderableEntity(size_t param_1, sSavePointMesh1* param_2, void(*updateCallback)(sTaskHeader*), void(*drawCallback)(sTaskHeader*), void(*deleteCallback)(sTaskHeader*)) {
    T* pNewEntity = new T;
    allocateSavePointMeshDataSub0(param_2, &pNewEntity->m0);
    registerSpriteCallback2(&pNewEntity->m0, &pNewEntity->m1C);
    regCallback8(&pNewEntity->m0, updateCallback);
    spriteBytecode2ExtendedE0_Sub0Sub0Sub0(&pNewEntity->m1C, drawCallback);
    if (deleteCallback == nullptr) {
        assert(0);
    }
    else {
        regCallbackC(&pNewEntity->m0, deleteCallback);
    }

    // TODO: Gross!
    pNewEntity->m0.m4 = (sSpriteActorCore*)pNewEntity;
    pNewEntity->m1C.m4 = (sSpriteActorCore*)pNewEntity;

    return pNewEntity;
}

bool battleSpritesDisabled = false;

void battleSpriteUpdate(sTaskHeader* param_1) {
    sSpriteActorCore* pSprite = param_1->m4->getAsSpriteActorCore();

    if (battleSpritesDisabled == '\0') {
        OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSprite);
        savePointCallback8Sub0(pSprite);
        if ((pSprite->mAC >> 6 & 1) != 0) {
            OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSprite);
            savePointCallback8Sub0(pSprite);
        }
    }
}

void battleSpriteRender(sTaskHeader* param_1) {
    sSpriteActorCore* pSprite = param_1->m4->getAsSpriteActorCore();

    if (battleSpritesDisabled == '\0') {
        SetRotMatrix(&battleRenderingMatrix);
        SetTransMatrix(&battleRenderingMatrix);

        SVECTOR local_30;
        local_30.vx = pSprite->m0_position.vx.getIntegerPart();
        local_30.vy = pSprite->m0_position.vy.getIntegerPart();
        local_30.vz = pSprite->m0_position.vz.getIntegerPart();

        sVec2_s16 dummy;
        long dummy2;
        long flag;
        s32 lVar1 = RotTransPers(&local_30, &dummy, &dummy2, &flag);

        s32 iVar2 = (lVar1 >> (gDepthDivider & 0x1f)) + pSprite->m30;
        if ((flag & 0x8000) != 0) {
            iVar2 = 0;
        }
        pSprite->m2E = iVar2;
        iVar2 = 1;
        if (iVar2 - 1U < 0xfff) {
            renderSpriteActor(pSprite, &(*characterRenderingOT)[iVar2]);
        }
    }
}

void battleSpriteDelete(sTaskHeader*) {
    assert(0);
}

extern int loadVramSpriteParam;
void initBattleSpriteActorVram(sSpriteActor* param_1, sSpriteActorAnimationBundle* param_2, short clutX, short clutY, short vramX, short vramY, s32 param_7, s32 param_8) {
    loadVramSpriteParam = param_8;
    initializeSpriteActor(param_1, param_2, clutX, clutY, vramX, vramY, param_7);
    loadVramSpriteParam = 0;
}

sBattleSpriteActor* allocateBattleSpriteActor(sSpriteActorAnimationBundle* param_1, short param_2, short param_3, short param_4, short param_5, short param_6, int X, int Y, int Z, short animationId, short param_11, int param_12_00, int param_13, u32 param_14) {
    sBattleSpriteActor* pSprite = createCustomRenderableEntity<sBattleSpriteActor>(0x19c, nullptr, battleSpriteUpdate, battleSpriteRender, battleSpriteDelete);

    pSprite->m0.m4 = &pSprite->m38_spriteActor;
    pSprite->m1C.m4 = &pSprite->m38_spriteActor;
    pSprite->m1C.m0_owner = nullptr;
    initBattleSpriteActorVram(&pSprite->m38_spriteActor, param_1, (int)param_2, (int)param_3, (int)param_4, (int)param_5, (int)param_6, param_14);
    pSprite->m38_spriteActor.m0_spriteActorCore.m6C_pointerToOwnerStructure = pSprite;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vx = X << 0x10;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vy = Y << 0x10;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vz = Z << 0x10;
    *(char*)&pSprite->m38_spriteActor.m0_spriteActorCore.mB0 = animationId;
    pSprite->m38_spriteActor.m0_spriteActorCore.m3C |= 4;
    pSprite->m38_spriteActor.m0_spriteActorCore.m32 = param_11;
    setGraphicEntityScale(&pSprite->m38_spriteActor, 0x2000);
    pSprite->m38_spriteActor.m0_spriteActorCore.m82 = 0x2000;
    pSprite->m38_spriteActor.m0_spriteActorCore.m78 = 0;
    spriteActorSetPlayingAnimation(&pSprite->m38_spriteActor, animationId);
    return pSprite;
}

void setSpriteActorCoreXZ(sSpriteActorCore* param_1, int X, int Z) {
    (param_1->m0_position).vz = Z << 0x10;
    (param_1->m0_position).vx = X << 0x10;
}

void createBattleSpriteActor(uint param_1, int param_2, short param_3) {
    battleVisualBuffers[param_2].bundle.init(battleVisualBuffers[param_2].m0_spriteData->begin());
    sBattleSpriteActor* pSprite = allocateBattleSpriteActor(&battleVisualBuffers[param_2].bundle, 0, param_1 + 0x1C0, battleVisualBuffers[param_2].m4_vramX, battleVisualBuffers[param_2].m6_vramY, 0x20, 0, 0, 0, param_3, 0, 0, 0, battleVisualBuffers[param_2].m8);

    sSpriteActorCore* pSpriteCore = pSprite->m0.m4->getAsSpriteActorCore();
    pSpriteCore->m24_vramData->m4_vramLocation.vx = battleVisualBuffers[param_2].m4_vramX;
    pSpriteCore->m24_vramData->m4_vramLocation.vy = battleVisualBuffers[param_2].m6_vramY;
    MissingCode(); // very confusing code here
    pSpriteCore->m7C->mE_vramLocation = pSpriteCore->m24_vramData->m4_vramLocation;

    battleSpriteActorCores[param_1] = pSprite->m0.m4->getAsSpriteActorCore();
    battleSpriteActors[param_1] = pSprite;

    pSpriteCore->mA8.mx1E = param_1;
    pSpriteCore->mAC = (pSpriteCore->mAC & ~3) | ((param_1 >> 2) & 3);

    setSpriteActorCoreXZ(pSpriteCore, battleVisualEntities[param_1].mA_X, battleVisualEntities[param_1].mC_Z);

    s32 direction = (battleVisualEntities[param_1].m6_direction != 0) << 0xB;

    setSpriteActorAngle(pSpriteCore, direction);
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(pSpriteCore, direction);
    if (battleVisualEntities[param_1].m3) {
        pSpriteCore->m9E_wait = 0;
    }
}

void savePointCallback8Sub0Sub0_battle(sSpriteActorCore* param_1);

void createBattlePlayerSpriteActors() {
    for (int i = 0; i < 3; i++) {
        if ((battleVisualEntities[i].m2 < 0x11) && battleVisualEntities[i].m4 == 0) {
            battleVisualBuffers[i].m6_vramY = 0x1C0;
            battleVisualBuffers[i].m4_vramX = battleSpriteX + 0x100;
            battleSpriteX += battleSpriteWidthPerCharacter[battleVisualEntities[i].m2];
            createBattleSpriteActor(i, i, 1);
            MissingCode();
        }
    }

    if (currentBattleLoaderTransitionEffect == 0) {
        for (int i = 0; i < 3; i++) {
            if (battleSpriteActorCores[i]) {
                sSpriteActorCore* pSprite = battleSpriteActorCores[i];
                savePointCallback8Sub0Sub0_battle(pSprite);
                pSprite->mA0.vx = pSprite->m0_position.vx.getIntegerPart();
                pSprite->mA0.vy = pSprite->m0_position.vy.getIntegerPart();
                pSprite->mA0.vz = pSprite->m0_position.vz.getIntegerPart();
                spriteActorSetPlayingAnimation(pSprite, 0x17);
            }
        }
    }
}

void mainBattleSpriteCallback_phase3(sTaskHeader* param_1) {
    MissingCode();
}

void mainBattleSpriteCallback_phase2(sTaskHeader* param_1) {

    setCurrentDirectory(0x2c, 0);
    if (!isCDBusy()) {
        createBattlePlayerSpriteActors();
        MissingCode();
        regCallback8(param_1, mainBattleSpriteCallback_phase3);
    }
}


// actual size is 14, to be filled up later
std::array<std::array<int, 2>, 3> battlePartyFileMapping = { {
    {0x1, 0x12},
    {0x2, 0x13},
    {0x4, 0x15},
} };

void setupPlayerSpriteLoadingCommands(std::array<sLoadingBatchCommands, 4>::iterator& param_1) {
    std::array<sLoadingBatchCommands, 4>::iterator start = param_1;
    setCurrentDirectory(0x2c, 1);
    for (int i = 0; i < 3; i++) {
        if ((battleVisualEntities[i].m2 < 0x11) && battleVisualEntities[i].m4 == 0) {
            param_1->m0_fileIndex = battlePartyFileMapping[battleVisualEntities[i].m2][0];
            sLoadableDataRaw* pNewLoadable = new sLoadableDataRaw(getFileSizeAligned(param_1->m0_fileIndex));
            param_1->m4_loadPtr = pNewLoadable;

            battleVisualBuffers[i].m0_spriteData = &pNewLoadable->mData;
            battleVisualBuffers[i].m8 = 0;

            param_1++;
        }
    }

    param_1->m0_fileIndex = 0;
    param_1->m4_loadPtr = nullptr;

    batchStartLoadingFiles(&(*start), 0);
}

void mainBattleSpriteCallback_phase1(sTaskHeader* param_1) {
    sBattleSpriteLoaderTask* pTask = (sBattleSpriteLoaderTask*)param_1;

    if (isCDBusy() == 0) {
        loadBattleSpriteVram(pTask->m20);
        createEnemiesBattleSprites(pTask->m20);
        DrawSync(0);
        //delete pTask->m20; // hack: this is currently a static
        pTask->m20 = nullptr; // not in original
        regCallback8(&pTask->m0, mainBattleSpriteCallback_phase2);
        setupPlayerSpriteLoadingCommands(pTask->m30_loadingCommands.begin());
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
