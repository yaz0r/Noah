#include "noahLib.h"
#include "battleSpriteLoader.h"
#include "field/fieldGraphicObject.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "battle/battle.h"
#include "field/field.h"

void initMechaForBattle(int param_1);

bool battleScriptLoaderTaskCreationInProgress = 0;

sBattleSpriteConfigs battleConfigFile3;

struct sBattleSpriteLoaderTask : public sTaskHeader{
    sBattleSpriteConfigs* m20;
    sSpriteActorAnimationBundle* m24;
    sLoadableDataRaw* m28;
    sLoadableDataRaw* m2C_seq;
    std::array<sLoadingBatchCommands, 4> m30_loadingCommands;
    std::array<sLoadingBatchCommands, 4> m50_loadingCommandsPhase2;
    s32 m90_frameToDeleteCount;
};

void defaultBattleSpriteDeleteCallback(sTaskHeader* param_1) {
    allocateSavePointMeshDataSub0_callback(param_1);
    delete param_1;
}

std::vector<u8> battleSpriteLoadTempBuffer;

int getSpriteNumVerticalStrides(std::vector<u8>::iterator data) {
    return READ_LE_U32(data);
}

void loadBattleSpriteVram(sBattleSpriteConfigs* param_1) {
    struct sBattleUploadSprite {
        std::array<u16, 11> m0;
        std::array<s32, 11> m20;
    };
    sBattleUploadSprite spriteIndicies;
    spriteIndicies.m0.fill(0);
    spriteIndicies.m20.fill(0);

    for (int i = 0; i < 11; i++) {
        spriteIndicies.m20[i] = battleVisualEntities[i].m2;
    }

    int numCurrentEntity = 0;
    int numMaxEntity = param_1->m0_numEntities;
    int currentSpriteX = param_1->m1 * 0x40 + 0x140;
    int headerSize = numMaxEntity * 0xC + 8;
    int currentSpriteSlot = 0;
    int currentSpriteEntityId = 0;
    int currentSlot = 3;

    battleSpriteLoadTempBuffer = param_1->m_spriteData;

    for (int numCurrentEntity = 0; numCurrentEntity < numMaxEntity; numCurrentEntity++) {
        if (param_1->m8[numCurrentEntity].m8_isMecha == 0) {
            int spriteSlot = param_1->m8[numCurrentEntity].m4_spriteDataOffset;
            if (spriteSlot > 7) {
                std::vector<u8>::iterator pSpriteData = param_1->begin() + param_1->m8[numCurrentEntity].m4_spriteDataOffset;
                uploadNpcSpriteSheet(pSpriteData, currentSpriteX, 0x100);
                spriteIndicies.m0[currentSpriteSlot] = currentSpriteX;
                currentSpriteX += getSpriteNumVerticalStrides(pSpriteData) * 0x40;
                spriteSlot = currentSpriteSlot++;
                if (currentSpriteX > 0x2C0) {
                    currentSpriteX = 0;
                }
            }

            battleVisualBuffers[3 + spriteSlot].m6_vramY = 0x100;
            battleVisualBuffers[3 + spriteSlot].m0_spriteData = param_1->beginSpan() + param_1->m8[currentSpriteEntityId++].m0_spriteControlOffset;
            battleVisualBuffers[3 + spriteSlot].m4_vramX = spriteIndicies.m0[spriteSlot];
            battleVisualBuffers[3 + spriteSlot].m8 = param_1->m8[numCurrentEntity].mB;
        }
        else {
            int iDest = 0;
            int spriteSlot = param_1->m8[numCurrentEntity].m4_spriteDataOffset;
            if (spriteSlot > 7) {
                currentSpriteX += 0x40;
                int numUsedEntries = 0;
                int searchedValue = currentSlot - 3;
                battleVisualBuffers[currentSlot].m0_spriteData.reset();
                for (int iVar3 = 3; iVar3 != 0xB; iVar3++) {
                    if (spriteIndicies.m20[iVar3] == searchedValue) {
                        numUsedEntries++;
                    }
                }
                for (int slot = numCurrentEntity + 1; slot != numMaxEntity; slot++) {
                    std::vector<u8>::iterator pSpriteData = param_1->begin() + param_1->m8[slot].m4_spriteDataOffset;
                    if (READ_LE_U8(pSpriteData + 1) && (READ_LE_U8(pSpriteData + 0) == numCurrentEntity)) {
                        for (int i = 3; i < 0xB; i++) {
                            if (spriteIndicies.m20[i] == slot) {
                                spriteIndicies.m20[i] = searchedValue;
                                numUsedEntries++;
                            }
                        }
                    }
                }
                for (int entryId = 3; entryId != 0xB; entryId++) {
                    int slotValue = spriteIndicies.m20[entryId];
                    if ((slotValue < 8) && (slotValue == searchedValue) && entryId) {
                        int flags = -(numUsedEntries < 2 ^ 1) & 2;
                        if (iDest) {
                            flags = 7;
                            if (numUsedEntries == iDest + 1) {
                                flags = 5;
                            }
                        }

                        int iVar3 = iDest - 0x1C0;
                        iDest++;

                        mechaInitEnvironmentMechaMesh(entryId, flags | 0x80, &param_1->m8[currentSpriteEntityId].m_mechaData2, &param_1->m8[currentSpriteEntityId].m_mechaData1, currentSpriteX, 0x100, 0, ((entryId - iVar3) * 0x10000) >> 0x10, 0);
                        initMechaForBattle(entryId);
                    }
                }
            }
        }
        currentSlot++;
    }
}

void createBattleSpriteActor(uint param_1, int param_2, short animationId);

void createEnemiesBattleSprites(sBattleSpriteConfigs* param_1) {
    for (int i = 3; i < 11; i++) {
        int monsterIndex = battleVisualEntities[i].m2;
        int visualBufferIndex = battleVisualEntities[i].m2 + 3;
        if ((monsterIndex < 8) && (battleVisualBuffers[visualBufferIndex].m0_spriteData)) {
            createBattleSpriteActor(i, visualBufferIndex, 1);
        }
    }
}

short battleSpriteX = 0;

std::array<u8, 12> battleSpriteWidthPerCharacter = { {
        0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10
} };

void battleSpriteUpdate(sTaskHeader* param_1) {
    sSpriteActorCore* pSprite = ((sSpriteActorCore*)param_1->m4);

    if (battleSpritesDisabled == 0) {
        OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSprite);
        savePointCallback8Sub0(pSprite);
        if (pSprite->mAC.mx6) {
            OP_INIT_ENTITY_SCRIPT_sub0Sub9(pSprite);
            savePointCallback8Sub0(pSprite);
        }
    }
}

void battleSpriteRender(sTaskHeader* param_1) {
    sSpriteActorCore* pSprite = ((sSpriteActorCore*)param_1->m4);

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
        int depth = RotTransPers(&local_30, &dummy, &dummy2, &flag);
        int OTIndex = (depth >> (gDepthDivider & 0x1f)) + pSprite->m30;
        if (flag & 0x8000) {
            OTIndex = 0;
        }
        pSprite->m2E = OTIndex;
        if (OTIndex - 1U < 0xfff) {
            renderSpriteActor(pSprite, &(*characterRenderingOT)[OTIndex]);
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

sBattleSpriteActor* allocateBattleSpriteActor(sSpriteActorAnimationBundle* param_1, short clutX, short clutY, short vramX, short vramY, short param_6, int X, int Y, int Z, short animationId, short direction, int param_12_00, int param_13, u32 param_14) {
    sBattleSpriteActor* pSprite = createCustomRenderableEntity<sBattleSpriteActor>(0x19c, nullptr, battleSpriteUpdate, battleSpriteRender, battleSpriteDelete);

    pSprite->m4 = &pSprite->m38_spriteActor;
    pSprite->m1C.m4 = &pSprite->m38_spriteActor;
    pSprite->m1C.m0_owner = nullptr;
    initBattleSpriteActorVram(&pSprite->m38_spriteActor, param_1, clutX, clutY, vramX, vramY, (int)param_6, param_14);
    pSprite->m38_spriteActor.m0_spriteActorCore.m6C_pointerToOwnerStructure = pSprite;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vx = X << 0x10;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vy = Y << 0x10;
    pSprite->m38_spriteActor.m0_spriteActorCore.m0_position.vz = Z << 0x10;
    pSprite->m38_spriteActor.m0_spriteActorCore.mB0.mx0_animationId = animationId;
    pSprite->m38_spriteActor.m0_spriteActorCore.m3C |= 4;
    pSprite->m38_spriteActor.m0_spriteActorCore.m32_direction = direction;
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

void createBattleSpriteActor(uint entityIndex, int visualBufferIndex, short animationId) {
    battleVisualBuffers[visualBufferIndex].bundle.init(battleVisualBuffers[visualBufferIndex].m0_spriteData.value());
    sBattleSpriteActor* pSprite = allocateBattleSpriteActor(&battleVisualBuffers[visualBufferIndex].bundle, 0, entityIndex + 0x1C0, battleVisualBuffers[visualBufferIndex].m4_vramX, battleVisualBuffers[visualBufferIndex].m6_vramY, 0x20, 0, 0, 0, animationId, 0, 0, 0, battleVisualBuffers[visualBufferIndex].m8);

    sSpriteActorCore* pSpriteCore = ((sSpriteActorCore*)pSprite->m4);
    pSpriteCore->m24_vramData->m4_vramLocation.vx = battleVisualBuffers[visualBufferIndex].m4_vramX;
    pSpriteCore->m24_vramData->m4_vramLocation.vy = battleVisualBuffers[visualBufferIndex].m6_vramY;
    MissingCode(); // very confusing code here
    pSpriteCore->m7C->mE_vramLocation = pSpriteCore->m24_vramData->m4_vramLocation;

    battleSpriteActorCores[entityIndex] = pSpriteCore;
    battleSpriteActors[entityIndex] = pSprite;

    pSpriteCore->mA8.mx1E_entityId_bottom2bit = entityIndex;
    pSpriteCore->mAC.mx0_entityIdUpper2bit = entityIndex >> 2;

    setSpriteActorCoreXZ(pSpriteCore, battleVisualEntities[entityIndex].mA_X, battleVisualEntities[entityIndex].mC_Z);

    s32 direction = (battleVisualEntities[entityIndex].m6_direction != 0) << 0xB;

    setSpriteActorAngle(pSpriteCore, direction);
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(pSpriteCore, direction);
    if (battleVisualEntities[entityIndex].m3) {
        pSpriteCore->m9E_wait = 0;
    }
}

std::array<std::array<int, 2>, 14> battlePartyFileMapping = { {
    {0x1, 0x12},
    {0x2, 0x13},
    {0x4, 0x15},
    {0x3, 0x14},
    {0x5, 0x16},
    {0x6, 0x17},
    {0x7, 0x18},
    {0x8, 0x19},
    {0x9, 0x1A},
    {0xA, 0x1B},
    {0xB, 0x1C},
    {0x1, 0x12},
    {0x1, 0x12},
    {0x1, 0x12},
} };

/*
Battle animation list:
0x00: idle (unused?)
0x01: idle ready to fight
0x02: run?
0x03: jump?
0x04: jump?
0x05: run?
0x06: kneeling
0x07: dead (lying down on the floor)
0x08: ? (assert in animation code)
0x09: Dodge?
0x0A: ? (assert in animation code, bytecode 0xC3)
0x0B: ? (assert in animation code, bytecode 0xC3)
0x0C: ? (assert in animation code, bytecode 0xC3)
0x0D: ? (assert in animation code, bytecode 0xC3)
0x0E: ? (assert in animation code, bytecode 0xC3)
0x0F: ? (assert in animation code, bytecode 0xC3)
0x10: stand up
0x11: casting
0x12: casting end?
0x13: ? (assert in animation code, bytecode 0xC3)
0x14: kneeling
0x15: ? (assert in animation code, bytecode 0xC3)
0x16: ? (assert in animation code, bytecode 0xC3)
0x17: Jump for battle intro
0x18: victory
0x19: idle breaker?
0x1A: idle breaker?
0x1B: dodge (crash!)
*/

void createBattlePlayerSpriteActors() {
    for (int i = 0; i < 3; i++) {
        if ((battleVisualEntities[i].m2 < 0x11) && battleVisualEntities[i].m4_isGear == 0) {
            battleVisualBuffers[i].m6_vramY = 0x1C0;
            battleVisualBuffers[i].m4_vramX = battleSpriteX + 0x100;
            battleSpriteX += battleSpriteWidthPerCharacter[battleVisualEntities[i].m2];
            createBattleSpriteActor(i, i, 1);
            sSpriteActorCore* pSprite = battleSpriteActorCores[i];
            pSprite->m7C->m0 = battlePartyFileMapping[battleVisualEntities[i].m2][1];
            pSprite->m20->getAsSprite()->m2C->clear();
            pSprite->m20->getAsSprite()->m2C->resize(32);
        }
    }

    // Start characters jumping in during battle intro
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

void mainBattleSpriteCallback_phase6(sTaskHeader* param_1) {
    if (((sBattleSpriteLoaderTask*)param_1)->m90_frameToDeleteCount == 0) {
        for (int i = 0; i < 3; i++) {
            if (!battleVisualEntities[i].m4_isGear && battleSpriteActorCores[i] && (battleSpriteActorCores[i]->m0_position.vy.getIntegerPart() != battleSpriteActorCores[i]->m84_maxY)) {
                return;
            }
        }
        battleTimeEnabled = 1;
        defaultBattleSpriteDeleteCallback(param_1);
    }
    else {
        ((sBattleSpriteLoaderTask*)param_1)->m90_frameToDeleteCount--;
    }
}

int mainBattleSpriteCallback_phase5Var = 0;
void mainBattleSpriteCallback_phase5(sTaskHeader* param_1) {
    if (mainBattleSpriteCallback_phase5Var == 0) {
        ((sBattleSpriteLoaderTask*)param_1)->m90_frameToDeleteCount = 0x10;
        setTaskUpdateFunction(param_1, &mainBattleSpriteCallback_phase6);
    }
}

void startPlayableCharactersJumpToPosition() {
    for (int i = 0; i < 3; i++) {
        if ((battleVisualEntities[i].m2 < 0x11) && (battleVisualEntities[i].m4_isGear)) {
            createMechaLoadingTask(i);
        }
    }
}

void mainBattleSpriteCallback_phase4(sTaskHeader* param_1) {
    if (waitForMusic(0) == 0) {
        startPlayableCharactersJumpToPosition();
        setTaskUpdateFunction(param_1, &mainBattleSpriteCallback_phase5);
    }
}

void mainBattleSpriteCallback_phase3(sTaskHeader* param_1) {
    if (isCDBusy() == 0) {
        MissingCode();
        initFieldEntitySub4Sub5Sub0(&createSavePointMeshData_mode5, ((sBattleSpriteLoaderTask*)param_1)->m24, sVec2_s16::fromS32(0x380), sVec2_s16::fromS32(0x1d10000));
        setTaskUpdateFunction(param_1, &mainBattleSpriteCallback_phase4);
        MissingCode();
    }
}

void mainBattleSpriteCallback_phase2(sTaskHeader* param_1) {
    sBattleSpriteLoaderTask* pThis = (sBattleSpriteLoaderTask*)param_1;
    setCurrentDirectory(0x2c, 0);
    if (!isCDBusy()) {
        createBattlePlayerSpriteActors();
        pThis->m50_loadingCommandsPhase2[0].m4_loadPtr = pThis->m28 = new sLoadableDataRaw();
        pThis->m50_loadingCommandsPhase2[0].m0_fileIndex = 1;

        pThis->m50_loadingCommandsPhase2[1].m4_loadPtr = pThis->m24 = new sSpriteActorAnimationBundle();
        pThis->m50_loadingCommandsPhase2[1].m0_fileIndex = 2;

        pThis->m50_loadingCommandsPhase2[2].m4_loadPtr = pThis->m2C_seq = new sLoadableDataRaw();
        pThis->m50_loadingCommandsPhase2[2].m0_fileIndex = 3;

        pThis->m50_loadingCommandsPhase2[3].m4_loadPtr = nullptr;
        pThis->m50_loadingCommandsPhase2[3].m0_fileIndex = 0;

        batchStartLoadingFiles(&pThis->m50_loadingCommandsPhase2[0], 0);
        setTaskUpdateFunction(pThis, mainBattleSpriteCallback_phase3);
    }
}

void setupPlayerSpriteLoadingCommands(std::array<sLoadingBatchCommands, 4>::iterator param_1) {
    std::array<sLoadingBatchCommands, 4>::iterator start = param_1;
    setCurrentDirectory(0x2c, 1);
    for (int i = 0; i < 3; i++) {
        if ((battleVisualEntities[i].m2 < 0x11) && battleVisualEntities[i].m4_isGear == 0) {
            param_1->m0_fileIndex = battlePartyFileMapping[battleVisualEntities[i].m2][0];
            sLoadableDataRaw* pNewLoadable = new sLoadableDataRaw(getFileSizeAligned(param_1->m0_fileIndex));
            param_1->m4_loadPtr = pNewLoadable;

            battleVisualBuffers[i].m0_spriteData = std::span<u8>(pNewLoadable->mData.begin(), pNewLoadable->mData.size()).begin();
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
        setTaskUpdateFunction(pTask, mainBattleSpriteCallback_phase2);
        setupPlayerSpriteLoadingCommands(pTask->m30_loadingCommands.begin());
    }
}

void createBattleSpriteLoadingTaskSub(sBattleSpriteConfigs* param_1)
{
    sBattleSpriteLoaderTask* psVar1;

    psVar1 = battleLoaderAllocateMainBattleSprite<sBattleSpriteLoaderTask>(nullptr, 0x78);
    setTaskUpdateFunction(psVar1, mainBattleSpriteCallback_phase1);
    psVar1->m20 = param_1;
    return;
}

void createBattleSpriteLoadingTask(sBattleSpriteConfigs* param_1) {
    battleScriptLoaderTaskCreationInProgress = 1;
    createBattleSpriteLoadingTaskSub(param_1);
    battleScriptLoaderTaskCreationInProgress = 0;
}
