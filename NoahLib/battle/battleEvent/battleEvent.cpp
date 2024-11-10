#include "noahLib.h"
#include "battle/battleEvent/battleEvent.h"
#include "battle/battle.h"
#include "kernel/filesystem.h"
#include "battle/battleConfig.h"
#include "kernel/decompress.h"
#include "field/fieldGraphicObject.h"
#include "battle/battleSpriteLoader.h"

sLoadableDataRaw battleEventFile3;
sBattleRunningVar0* battleEventVar0 = nullptr;

struct sBattleRunningVar1 {
    // size 0x98
};

sBattleRunningVar1* battleEventVar1 = nullptr;

std::vector<u8> battleEventVar2;
std::vector<u8> battleEventVar3;

static const std::array<s16, 5> battleEventInitVar0 = { {0x7FFF, 0x7FFF, 0x10, 0x8, 0x1F0} };

std::array<s8, 0x10> battleEventInitVar1;

sSeqFile* gCurrentBattleEventSeq = nullptr;

void battleEventEntry() {
    battleRenderDebugAndMain();
    setCurrentDir_20_0();

    sLoadableDataRaw file2;

    sLoadingBatchCommands loadingCommands[3];
    loadingCommands[0].m0_fileIndex = 2;
    loadingCommands[0].m4_loadPtr = &file2;
    loadingCommands[1].m0_fileIndex = 3;
    loadingCommands[1].m4_loadPtr = &battleEventFile3;
    loadingCommands[2].m0_fileIndex = 0;
    loadingCommands[2].m4_loadPtr = nullptr;
    batchStartLoadingFiles(loadingCommands, 0);
    idleBattleDuringLoading();
    auto file2Reloc = doPointerRelocation(file2.mData);
    auto file3Reloc = doPointerRelocation(battleEventFile3.mData);

    battleEventVar2 = mallocAndDecompress(file2Reloc[currentBattleConfig.m3_eventDataIndex * 2]);
    battleEventVar3 = mallocAndDecompress(file2Reloc[currentBattleConfig.m3_eventDataIndex * 2 + 1]);

    battleEventVar0 = new sBattleRunningVar0;
    battleEventVar1 = new sBattleRunningVar1;

    battleEventVar0->m390 = battleEventVar2.begin() + READ_LE_U32(battleEventVar2.begin() + 0x40) * 0x10 + 0x44;

    for (int i = 0; i < 0x10; i++) {
        battleEventVar0->m794[i] = -1;
    }

    for (int i = 0; i < READ_LE_U32(battleEventVar2.begin() + 0x40); i++) {
        for (int j = 0; j < 8; j++) {
            battleEventVar0->m0[i].m0[j] = -1;
            battleEventVar0->m0[i].m10[j] = -1;
            battleEventVar0->m0[i].m18[j] = -1;
        }
        battleEventVar0->m794[i] = i;
        battleEventVar0->m0[i].m10[0] = 0;
        battleEventVar0->m0[i].m0[0] = READ_LE_S16(battleEventVar2.begin() + 0x44 + i * 0x10);
        battleEventVar0->m0[i].m22 = -1;
        battleEventVar0->m0[i].m18[0] = 0;
        battleEventVar0->m0[i].m23 = -1;
        battleEventVar0->m0[i].m24 = -1;
    }

    battleEventVar0->m7F5 = 4;

    for (int i = 0; i < 5; i++) {
        battleEventVar0->m7F6[i] = battleEventInitVar0[i];
    }

    for (int i = 0; i < 2; i++) {
        SetPolyFT4(&battleEventVar0->m7A4[i]);
        battleEventVar0->m7A4[i].m_colorAndCode.m0_r = 0x80;
        battleEventVar0->m7A4[i].m_colorAndCode.m1_g = 0x80;
        battleEventVar0->m7A4[i].m_colorAndCode.m2_b = 0x80;
        SetSemiTrans(&battleEventVar0->m7A4[i], 0);
        SetShadeTex(&battleEventVar0->m7A4[i], 1);
        battleEventVar0->m7A4[i].clut = GetClut(0, 0x1D0);
        battleEventVar0->m7A4[i].tpage = GetTPage(1, 0, 0x3c0, 0x100);
    }

    battleVar1->mCA_battleEventPoly = 1;
    battleVar1->mCF = 0;

    for (int i = 0; i < 0x10; i++) {
        battleEventVar0->m804[i] = 0;
        battleEventInitVar1[i] = 0;
    }

    battleEventVar0->m81F = 0;

    setCurrentDir_20_0();
    battleEventVar0->m818 = new sSeqFile;
    readFile(4, *battleEventVar0->m818, 0, 0x80);
    idleBattleDuringLoading();
    loadSequence(battleEventVar0->m818);
    waitForMusic(0x10);
    battleEventVar0->m820 = 1;
    gCurrentBattleEventSeq = battleEventVar0->m818;
    waitBattleAnimationSpriteLoading();
}

s16 battleEventGetBytecodeOffset(sBattleRunningVar0Sub* param_1) {
    int a1;
    for (int i = 0; i < 8; i++) {
        if (param_1->m10[i] != -1) {
            a1 = i;
        }
    }
    param_1->m20 = a1;
    param_1->m21 = param_1->m18[a1];
    return param_1->m0[a1];
}

void battleEvent_loadArgs(std::vector<u8>::iterator bytecode, byte param_2, uint param_3, char param_4)
{
    for (int i = 0; i < param_2; i++) {
        if (param_4 == 0) {
            if (((param_3 & 0xff) << (i & 0x1f) & 0x80) == 0) {
                battleEventVar0->m380[i] = battleEventVar0->m394[READ_LE_U16(bytecode + 1) / 2];
            }
            else {
                battleEventVar0->m380[i] = bytecode[1] + (ushort)(byte)bytecode[2] * 0x100;
            }
        }
        else {
            int iVar3 = (uint)(byte)bytecode[1] + (uint)(byte)bytecode[2] * 0x100;
            int iVar2 = iVar3 * 0x10000;
            if (iVar2 < 0) {
                battleEventVar0->m380[i] = (ushort)iVar3 & 0x7fff;
            }
            else {
                assert(0);
            }
        }
        bytecode += 2;
    }
}

sTaskHeader* battleEvent_OP35_createEntity(sSpriteActorAnimationBundle* param_1, SVECTOR* param_2) {
    auto uVar2 = allocateSavePointMeshDataSub0_var0;
    sBattleSpriteActor* psVar3 = createCustomRenderableEntity<sBattleSpriteActor>(0x19c, nullptr, battleSpriteUpdate, battleSpriteRender, battleSpriteDelete);
    sSpriteActor* psVar4 = &psVar3->m38_spriteActor;
    allocateSavePointMeshDataSub0_var0 = uVar2;
    psVar3->m14 &= 0x7fffffff;
    psVar3->m38_spriteActor.m0_spriteActorCore.m6C_pointerToOwnerStructure = psVar3;
    psVar3->m4 = &psVar4->m0_spriteActorCore;
    psVar3->m1C.m4 = &psVar4->m0_spriteActorCore;
    initFieldEntitySub4Sub1(&psVar4->m0_spriteActorCore);
    initFieldEntitySub4Sub2(psVar4);
    psVar3->m38_spriteActor.m48_defaultAnimationbundle = param_1;
    psVar3->m38_spriteActor.m40 = psVar3->m38_spriteActor.m40 & 0xfffe1fff | 0x8000;
    psVar3->m38_spriteActor.m3C &= 0xfffffffc;
    setAnimationBundle(psVar4, param_1);

    psVar3->m38_spriteActor.m0_position.vx = (int)param_2->vx << 0x10;
    psVar3->m38_spriteActor.m0_position.vy = (int)param_2->vy << 0x10;
    psVar3->m38_spriteActor.m0_position.vz = (int)param_2->vz << 0x10;

    psVar3->m38_spriteActor.mB0.mx0_animationId = 0;
    psVar3->m38_spriteActor.m32_direction = 0;

    setGraphicEntityScale(&psVar4->m0_spriteActorCore, 0x2000);
    psVar3->m38_spriteActor.m82 = 0x2000;
    psVar3->m38_spriteActor.m9E_wait = 0;

    startJumpAnimationSub0(0);
    setBattleCameraMode(0);
    battleCameraVar0 = 1;

    return psVar3;
}

int battleEvent_OP0(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& entry = battleEventVar0->m0[currentEntityId];
    entry.m18[entry.m20] = -1;
    entry.m10[entry.m20] = -1;
    entry.m0[entry.m20] = -1;
    entry.m18[0] = 1;
    entry.m20 = 0;
    entry.m10[0] = 7;
    entry.m23 = -1;
    entry.m0[0] = READ_LE_U16(battleEventVar2.begin() + currentEntityId * 0x10 + 0x46);
    return 0;
}

uint battleEvent_OP5_sub0_sub(sBattleRunningVar0Sub* param_1)
{
    uint uVar1;

    uVar1 = 1;
    do {
        if (param_1->m10[uVar1] == -1) break;
        uVar1 = uVar1 + 1;
    } while ((int)uVar1 < 8);
    return uVar1 & 0xffff;
}

int battleEvent_OP5_sub0(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    int uVar2 = battleEvent_OP5_sub0_sub(&battleEventVar0->m0[currentEntityId]) & 0xFF;
    if (uVar2 != 8) {
        battleEventVar0->m0[currentEntityId].m23 = (bytecode[2] & 0x1F);
        battleEventVar0->m0[bytecode[1]].m10[uVar2] = bytecode[2] >> 5;
        battleEventVar0->m0[bytecode[1]].m0[uVar2] = READ_LE_U16(bytecode[1] * 0x10 + battleEventVar2.begin() + 0x44 + battleEventVar0->m0[currentEntityId].m23 * 2);
        battleEventVar0->m0[bytecode[1]].m18[uVar2] = battleEventVar0->m0[currentEntityId].m23;
        return 3;
    }
    return 0;
}

int battleEvent_OP5(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    if (battleEventVar0->m0[currentEntityId].m23 == (bytecode[2] & 0x1F)) {
        bool bVar2 = true;
        for (int i = 0; i < 8; i++) {
            if (battleEventVar0->m0[bytecode[1]].m18[i] == battleEventVar0->m0[currentEntityId].m23) {
                bVar2 = false;
                break;
            }
        }
        if (bVar2) {
            if (battleEventVar0->m0[currentEntityId].m23 != battleEventVar0->m0[bytecode[1]].m21) {
                battleEventVar0->m0[currentEntityId].m23 = -1;
                return 3;
            }
        }
    }
    else {
        battleEvent_OP5_sub0(currentEntityId, bytecode);
    }
    return 0;
}

int battleEvent_OP18_sub(int param_1, int param_2, int param_3) {
    MissingCode();
    return 0;
}

int battleEvent_OP18(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    char cVar1;

    // print dialog window in battle event
    cVar1 = battleEvent_OP18_sub(READ_LE_U16(bytecode + 1), battleEventVar0->m0[currentEntityId].m24, bytecode[3]);
    return (uint)(cVar1 != '\0') << 2;
}

int battleEvent_OP1A(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 5, 0, 1);

    for (int i = 0; i < 4; i++) {
        if (battleEventVar0->m380[i] == 0) {
            battleEventVar0->m7F6[i] = battleEventInitVar0[i];
        }
        else {
            battleEventVar0->m7F6[i] = battleEventVar0->m380[i];
        }
    }
    battleEventVar0->m7F6[4] = battleEventVar0->m380[4];

    return 0xB;
}

int battleEvent_OP1B(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    int arg = bytecode[1];
    if (arg > 0xf2) {
        assert(0);
    }
    battleEventVar0->m0[currentEntityId].m24 = arg;
    return 2;
}

int battleEvent_OP1F(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    MissingCode(); // Create a sprite?
    return 3;
}

int battleEvent_OP23(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 3, 0, 1);

    int slotIndex = battleEventVar0->m380[0] - 0xF3;
    auto slot = &battleEventVar0->m0[slotIndex];
    switch (slot->m34) {
    case 0:
        slot->m34 = 2;
        setupMechaForEvent(slotIndex, characterIdToTargetBitmask(battleEventVar0->m380[1] + 0xD), battleEventVar0->m380[2]);
        return 0;
    case 1:
        waitBattleAnimationSpriteLoading();
        waitBattleAnimationSoundLoaded();
        slot->m34 = 0;
        return 7;
    case 2:
        return 0;
    default:
        assert(0);
        break;
    }

    return 0;
}

void battleEvent_OP2A_sub0(sBattleSpriteActor* pTaskHeader, int index) {
    spriteActorSetPlayingAnimation(&pTaskHeader->m38_spriteActor, index);
}

int battleEvent_OP2A(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);

    sBattleRunningVar0Sub* entry = &battleEventVar0->m0[(battleEventVar0->m380[0] + 0xD) & 0xFF];
    if (entry->m35) {
        battleEvent_OP2A_sub0((sBattleSpriteActor*)entry->m30, battleEventVar0->m380[1]);
    }

    return 5;
}

int battleEvent_OP2B(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    if (battleEventVar0->m0[currentEntityId].m28 == 0) {
        battleEvent_loadArgs(bytecode, 1, 0, 1);
        battleEventVar0->m0[currentEntityId].m28 = 1;
        battleEventVar0->m0[currentEntityId].m26 = battleEventVar0->m380[0] << 1;
    }
    int bytecodeSize = 0;
    if (battleEventVar0->m0[currentEntityId].m26 == 0) {
        battleEventVar0->m0[currentEntityId].m28 = 0;
        bytecodeSize = 3;
    }
    return bytecodeSize;
}

int battleEvent_OP31(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    MissingCode(); // sound related
    return 9;
}

int battleEvent_OP35(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    sBattleRunningVar0Sub* entry = &battleEventVar0->m0[(battleEventVar0->m380[0] + 0xD) & 0xFF];
    if (entry->m35 == 0) {
        entry->m2C = new sSpriteActorAnimationBundle;
        std::vector<u8> rawBundle = mallocAndDecompress(battleEventFile3.begin() + READ_LE_U32(battleEventFile3.begin() + 4 + battleEventVar0->m380[1] * 4));
        entry->m2C->init(rawBundle);
        SVECTOR position = { 0,0,0 };
        entry->m30 = battleEvent_OP35_createEntity(entry->m2C, &position);
        entry->m35 = 1;
    }
    return 5;
}

int battleEvent_OP37(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    isBattleAnEvent = 1;
    battleEventVar0->m800 = 1;
    battleRunningVar1 = 1;
    return 1;
}

void battleEvent_update(int) {
    waitBattleAnimationSoundLoaded();
    updateCharacterBlinkingTask(0);

    if (battleEventVar0->m800 == 0) {
        bool continueEvent = true;
        do {
            for (int i = 0; i < READ_LE_U32(battleEventVar2.begin() + 0x40); i++) {
                s8 currentEntityId = battleEventVar0->m794[i];
                battleRenderDebugAndMain();
                for (int j = 0; j < 4; j++) {
                    s16 bytecodeOffset = battleEventGetBytecodeOffset(&battleEventVar0->m0[currentEntityId]);
                    u8 bytecode = READ_LE_U8(battleEventVar0->m390 + bytecodeOffset);
                    int bytecodeSize;
                    switch (bytecode) {
                    case 0x0:
                        bytecodeSize = battleEvent_OP0(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        j = 3; // to bread out of the loop
                        break;
                    case 0x5:
                        bytecodeSize = battleEvent_OP5(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x18:
                        bytecodeSize = battleEvent_OP18(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x1A:
                        bytecodeSize = battleEvent_OP1A(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x1B:
                        bytecodeSize = battleEvent_OP1B(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x1F:
                        bytecodeSize = battleEvent_OP1F(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x23:
                        bytecodeSize = battleEvent_OP23(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x2A:
                        bytecodeSize = battleEvent_OP2A(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x2B:
                        bytecodeSize = battleEvent_OP2B(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x31:
                        bytecodeSize = battleEvent_OP31(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x35:
                        bytecodeSize = battleEvent_OP35(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    case 0x37:
                        bytecodeSize = battleEvent_OP37(currentEntityId, battleEventVar0->m390 + bytecodeOffset);
                        break;
                    default:
                        assert(0);
                    }
                    battleEventVar0->m0[currentEntityId].m0[battleEventVar0->m0[currentEntityId].m20] += bytecodeSize;
                }
            }
        } while (continueEvent);
    }
}

void mecha_battle_op3_2(int param_1) {
    battleEventVar0->m0[param_1].m34 = 1;
}