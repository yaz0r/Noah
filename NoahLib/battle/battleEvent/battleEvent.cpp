#include "noahLib.h"
#include "battle/battleEvent/battleEvent.h"
#include "battle/battle.h"
#include "kernel/filesystem.h"
#include "battle/battleConfig.h"
#include "kernel/decompress.h"
#include "field/fieldGraphicObject.h"
#include "field/dialogWindows.h"
#include "battle/battleSpriteLoader.h"
#include "battle/battleDialogWindow.h"
#include "kernel/TIM.h"
#include "psx/libgte.h"
#include "battle/battleRenderContext.h"
#include "battle/battleFader.h"

sLoadableDataRaw battleEventFile3;
sBattleRunningVar0* battleEventVar0 = nullptr;

sDialogWindow18* battleEventVar1 = nullptr;

struct sBattleEventVar2 {
    void init(const std::vector<u8> initData) {
        m40 = READ_LE_U32(initData.data() + 0x40);
        m44_scriptBytecodeEntryPoints.resize(m40);
        for (int i = 0; i < m40; i++) {
            for (int j = 0; j < 8; j++) {
                m44_scriptBytecodeEntryPoints[i][j] = READ_LE_S16(initData.begin() + 0x44 + i * 0x10 + j * 2);
            }
        }
        m_scriptByteCode.insert(m_scriptByteCode.begin(), initData.begin() + m40 * 0x10 + 0x44, initData.end());
    }

    u32 m40;
    std::vector<std::array<s16, 8>> m44_scriptBytecodeEntryPoints;

    std::vector<u8> m_scriptByteCode;
};

sBattleEventVar2 battleEventVar2;
std::vector<u8> battleEventVarDialogueBundle;

static const sDialogParams battleEventDefaultDialogParams = {0x7FFF, 0x7FFF, 0x10, 0x8, 0x1F0};

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

    battleEventVar2.init(mallocAndDecompress(file2Reloc[currentBattleConfig.m3_eventDataIndex * 2]));
    battleEventVarDialogueBundle = mallocAndDecompress(file2Reloc[currentBattleConfig.m3_eventDataIndex * 2 + 1]);

    battleEventVar0 = new sBattleRunningVar0;
    battleEventVar1 = new sDialogWindow18;

    battleEventVar0->m390_scriptByteCode = battleEventVar2.m_scriptByteCode.begin();

    for (int i = 0; i < 0x10; i++) {
        battleEventVar0->m794[i] = -1;
    }

    for (int i = 0; i < battleEventVar2.m40; i++) {
        for (int j = 0; j < 8; j++) {
            battleEventVar0->m0_scriptEntities[i].m0_scriptBytecodeOffset[j] = -1;
            battleEventVar0->m0_scriptEntities[i].m10_isScriptSlotActive[j] = -1;
            battleEventVar0->m0_scriptEntities[i].m18[j] = -1;
        }
        battleEventVar0->m794[i] = i;
        battleEventVar0->m0_scriptEntities[i].m10_isScriptSlotActive[0] = 0;
        battleEventVar0->m0_scriptEntities[i].m0_scriptBytecodeOffset[0] = battleEventVar2.m44_scriptBytecodeEntryPoints[i][0];
        battleEventVar0->m0_scriptEntities[i].m22 = -1;
        battleEventVar0->m0_scriptEntities[i].m18[0] = 0;
        battleEventVar0->m0_scriptEntities[i].m23 = -1;
        battleEventVar0->m0_scriptEntities[i].m24 = -1;
    }

    battleEventVar0->m7F5 = 4;
    battleEventVar0->m7F6_dialogParams = battleEventDefaultDialogParams;

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
    int a1 = -1;
    for (int i = 0; i < 8; i++) {
        if (param_1->m10_isScriptSlotActive[i] != -1) {
            a1 = i;
        }
    }
    assert(a1 != -1);
    param_1->m20_currentActiveEntry = a1;
    param_1->m21 = param_1->m18[a1];
    return param_1->m0_scriptBytecodeOffset[a1];
}

void battleEvent_loadArgs(std::vector<u8>::iterator bytecode, byte param_2, uint param_3, char param_4)
{
    for (int i = 0; i < param_2; i++) {
        if (param_4 == 0) {
            if (((param_3 & 0xff) << (i & 0x1f) & 0x80) == 0) {
                // get from script variables
                battleEventVar0->m380_currentArgs[i] = battleEventVar0->m394_vars[READ_LE_U16(bytecode + 1) / 2];
            }
            else {
                // get from bytecode
                battleEventVar0->m380_currentArgs[i] = bytecode[1] + (ushort)(byte)bytecode[2] * 0x100;
            }
        }
        else {
            int iVar3 = (uint)(byte)bytecode[1] + (uint)(byte)bytecode[2] * 0x100;
            int iVar2 = iVar3 * 0x10000;
            if (iVar2 < 0) {
                battleEventVar0->m380_currentArgs[i] = (ushort)iVar3 & 0x7fff;
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

int battleEvent_OP0_Stop(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& entry = battleEventVar0->m0_scriptEntities[currentEntityId];
    entry.m18[entry.m20_currentActiveEntry] = -1;
    entry.m10_isScriptSlotActive[entry.m20_currentActiveEntry] = -1;
    entry.m0_scriptBytecodeOffset[entry.m20_currentActiveEntry] = -1;
    entry.m18[0] = 1;
    entry.m20_currentActiveEntry = 0;
    entry.m10_isScriptSlotActive[0] = 7;
    entry.m23 = -1;
    entry.m0_scriptBytecodeOffset[0] = battleEventVar2.m44_scriptBytecodeEntryPoints[currentEntityId][1];
    return 0;
}

s16 battleEvent_findFreeScriptSlot(sBattleRunningVar0Sub* param_1)
{
    for (int i = 1; i < 8; i++) {
        if (param_1->m10_isScriptSlotActive[i] == -1)
            return i;
    }
    return 8;
}

int battleEvent_OP5_sub0(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& thisEntity = battleEventVar0->m0_scriptEntities[currentEntityId];
    s16 uVar2 = battleEvent_findFreeScriptSlot(&battleEventVar0->m0_scriptEntities[currentEntityId]) & 0xFF;
    if (uVar2 != 8) {
        auto& otherEntity = battleEventVar0->m0_scriptEntities[bytecode[1]];

        thisEntity.m23 = (bytecode[2] & 0x1F);
        otherEntity.m10_isScriptSlotActive[uVar2] = bytecode[2] >> 5;
        otherEntity.m0_scriptBytecodeOffset[uVar2] = battleEventVar2.m44_scriptBytecodeEntryPoints[bytecode[1]][thisEntity.m23];
        otherEntity.m18[uVar2] = thisEntity.m23;
        return 3;
    }
    return 0;
}

int battleEvent_OP5(int currentEntityId, const std::vector<u8>::iterator& bytecode) {

    auto& thisEntity = battleEventVar0->m0_scriptEntities[currentEntityId];
    auto& otherEntity = battleEventVar0->m0_scriptEntities[bytecode[1]];

    if (thisEntity.m23 == (bytecode[2] & 0x1F)) {
        bool bVar2 = true;
        for (int i = 0; i < 8; i++) {
            if (otherEntity.m18[i] == thisEntity.m23) {
                bVar2 = false;
                break;
            }
        }
        if (bVar2) {
            if (thisEntity.m23 != otherEntity.m21) {
                thisEntity.m23 = -1;
                return 3;
            }
        }
    }
    else {
        battleEvent_OP5_sub0(currentEntityId, bytecode);
    }
    return 0;
}

std::vector<std::array<u8, 2>> battleDialogPortraitsTable = { {
    {0, 0}, // Fei
    {6, 6}, // Elly
    {0x11, 0x11}, // Citan
    {0x13, 0x14}, // Bart
    // ... TODO: many more are missing here
} };

void battleDialogLoadPortrait(uint param_1, uint param_2, short x, short y, short width) {
    int portraitId = battleDialogPortraitsTable[param_1 & 0xff][param_2 & 1];
    setCurrentDirectory(4, 0);
    int size = getFileSizeAligned(portraitId + 0x46);

    {
        std::vector<u8> portraitFile;
        readFile(portraitId + 0x46, portraitFile, 0, 0x80);
        idleBattleDuringLoading();
        OpenTIM(portraitFile.begin());
        TIM_IMAGE timImage;
        ReadTIM(&timImage);
        timImage.crect->x = 0;
        timImage.crect->y = 0x1D0;
        timImage.prect->x = 0x3C0;
        timImage.prect->y = 0x100;
        LoadImage(timImage.crect, (const u8*)timImage.caddr);
        LoadImage(timImage.prect, (const u8*)timImage.paddr);
        DrawSync(0);
    }

    if (!(param_2 & 1)) {
        battleEventVar0->m7A4[battleOddOrEven].x0y0.vx = x + 4;
        battleEventVar0->m7A4[battleOddOrEven].x0y0.vy = y + 4;
        battleEventVar0->m7A4[battleOddOrEven].x1y1.vx = x + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x1y1.vy = y + 4;
        battleEventVar0->m7A4[battleOddOrEven].x2y2.vx = x + 4;
        battleEventVar0->m7A4[battleOddOrEven].x2y2.vy = y + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x3y3.vx = x + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x3y3.vy = y + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].u0 = 0;
        battleEventVar0->m7A4[battleOddOrEven].v0 = 0;
        battleEventVar0->m7A4[battleOddOrEven].u1 = 0x40;
        battleEventVar0->m7A4[battleOddOrEven].v1 = 0;
        battleEventVar0->m7A4[battleOddOrEven].u2 = 0;
        battleEventVar0->m7A4[battleOddOrEven].v2 = 0x40;
        battleEventVar0->m7A4[battleOddOrEven].u3 = 0x40;
        battleEventVar0->m7A4[battleOddOrEven].v3 = 0x40;
    }
    else {
        battleEventVar0->m7A4[battleOddOrEven].x0y0.vx = x + width - 4;
        battleEventVar0->m7A4[battleOddOrEven].x0y0.vy = y + 4;
        battleEventVar0->m7A4[battleOddOrEven].x1y1.vx = x + width - 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x1y1.vy = y + 4;
        battleEventVar0->m7A4[battleOddOrEven].x2y2.vx = x + width - 4;
        battleEventVar0->m7A4[battleOddOrEven].x2y2.vy = y + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x3y3.vx = x + width - 0x44;
        battleEventVar0->m7A4[battleOddOrEven].x3y3.vy = y + 0x44;
        battleEventVar0->m7A4[battleOddOrEven].u0 = 0;
        battleEventVar0->m7A4[battleOddOrEven].v0 = 0;
        battleEventVar0->m7A4[battleOddOrEven].u1 = 0x3f;
        battleEventVar0->m7A4[battleOddOrEven].v1 = 0;
        battleEventVar0->m7A4[battleOddOrEven].u2 = 0;
        battleEventVar0->m7A4[battleOddOrEven].v2 = 0x40;
        battleEventVar0->m7A4[battleOddOrEven].u3 = 0x3f;
        battleEventVar0->m7A4[battleOddOrEven].v3 = 0x40;
    }

    battleEventVar0->m7F4_battleDialogPortraitOddOrEven = battleOddOrEven;
}

static int battleDialogCursorSpriteIndex = 4;

void setupBattleDialogCursorSpritePoly(short x, short y) {
    battleDialogCursorSpriteIndex = battleDialogCursorSpriteIndex + -1;
    if (battleDialogCursorSpriteIndex < 0) {
        battleDialogCursorSpriteIndex = 4;
    }
    battleVar1->m100_countOf27C8 = battleSetupStringInPolyFT4Large(battleDialogCursorSpriteIndex + 0xe0, battleVar0->m27C8.data(), x, y);
    
    // swap points 1 and 2
    sVec2_s16 temp = battleVar0->m27C8[battleVar1->m100_countOf27C8][battleOddOrEven].x1y1;
    battleVar0->m27C8[battleVar1->m100_countOf27C8][battleOddOrEven].x1y1 = battleVar0->m27C8[battleVar1->m100_countOf27C8][battleOddOrEven].x2y2;
    battleVar0->m27C8[battleVar1->m100_countOf27C8][battleOddOrEven].x2y2 = temp;

    battleVar1->mA7_27C8OddOrEven = battleOddOrEven;
    battleVar1->m9E_render27C8 = 1;
}

static int battleDialogWindowY2 = 0;
static int battleDialogWindowX2 = 0;
int battleEvent_DisplayDialog(u16 messageIndex, int protraitIndex, int flags) {

    int x = battleEventVar0->m7F6_dialogParams.m0_x;
    int y = battleEventVar0->m7F6_dialogParams.m2_y;

    int sVar6 = battleEventVar0->m7F6_dialogParams.m4_width * 0xC;
    int width = sVar6 + 0x18;

    if ((flags & 0xFFFF) == 0) {
        flags = battleEventVar0->m7F6_dialogParams.m8_flags;
    }
    if (!battleEventVar0->m802_dialogBoxInitialized) {
        battleDialogCursorSpriteIndex = 4;
        if (battleEventVar0->m7F6_dialogParams.m2_y == 0x7fff) {
            y = 0x10;
            if (flags & 4) {
                y = 0x8C;
            }
        }
        int height = 4;
        if (battleEventVar0->m7F6_dialogParams.m6_height < 5) {
            height = battleEventVar0->m7F6_dialogParams.m6_height;
        }
        height = height * 0xD + 0x14;
        if (battleEventVar0->m7F6_dialogParams.m0_x == 0x7FFF) {
            x = 0xA0 - width / 2;
        }

        if (!(flags & 8)) {
            bool displayPortrait = false;
            if (((flags & 2) == 0) && (protraitIndex != -1)) {
                width = sVar6 + 0x58;
                displayPortrait = true;
            }
            if (battleEventVar0->m7F6_dialogParams.m0_x == 0x7fff) {
                x = 0xa0 - (width >> 1);
            }
            if (displayPortrait) {
                battleDialogLoadPortrait(protraitIndex, flags & 0xffff, x, y, width);
                allocateAndSetupBattleDialogWindow(0, x, y, width, height, ((flags & 0xffff) >> 4 ^ 1) & 1, 1);
                while (!battleVar1->mBF_isDialogWindowInitialized3[0]) {
                    battleRenderDebugAndMain();
                }
                battleVar1->mC8_dialogPortrait = 1;
                if ((flags & 1) == 0) {
                    x += 0x40;
                }
            }
            else {
                allocateAndSetupBattleDialogWindow(0, x, y, width, height, ((flags & 0xffff) >> 4 ^ 1) & 1, 1);
                while (!battleVar1->mBF_isDialogWindowInitialized3[0]) {
                    battleRenderDebugAndMain();
                }
            }
        }

        battleDialogWindowY2 = y + 8;
        battleDialogWindowX2 = x + 0xc;
        setupWindowSize2(battleEventVar1, 0x380, 0x100, battleDialogWindowX2, battleDialogWindowY2, battleEventVar0->m7F6_dialogParams.m4_width * 3, battleEventVar0->m7F6_dialogParams.m6_height);
        battleEventVar1->m48_textTile[1].m_colorAndCode.m3_code = 4;
        battleEventVar1->m10_flags |= 2;
        resetDialogWindow18(battleEventVar1);
        addDialogWindowsToOTSub2(battleEventVar1, getDialogParamPointer(battleEventVarDialogueBundle.begin(), messageIndex));
        battleVar1->mC9_dialog = 1;
        battleEventVar0->m802_dialogBoxInitialized = 1;
        battleRenderDebugAndMain();
    }
    if (battleEventVar1->m10_flags & 8) {
        if ((flags & 8) == 0) {
            setupBattleDialogCursorSpritePoly((battleEventVar1->m0).vx * 4 + battleDialogWindowX2 + 2, (battleEventVar1->m0).vy * 0xe + battleDialogWindowY2 + 5);
        }
        battleVar1->mCF = 1;
        if (battleInputButton == 4) {
            addDialogWindowsToOTSub1(battleEventVar1);
            battleVar1->mCF = 0;
            battleVar1->m9E_render27C8 = 0;
        }

    }
    if (!(battleEventVar1->m10_flags & 4)) {
        battleVar1->mC9_dialog = 0;
        dealocateDialogWindow18Memory(battleEventVar1);
        battleRenderDebugAndMain();
        battleVar1->mC8_dialogPortrait = 0;
        if (!(flags & 8)) {
            drawCircleMenuChi_updateSub6(0);
        }
        battleEventVar0->m802_dialogBoxInitialized = 0;
        battleEventVar0->m7F6_dialogParams = battleEventDefaultDialogParams;
        return 1;
    }
    return 0;
}

int battleEvent_OP18_Dialog(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    // print dialog window in battle event
    char isDialogClosed = battleEvent_DisplayDialog(READ_LE_U16(bytecode + 1), battleEventVar0->m0_scriptEntities[currentEntityId].m24, bytecode[3]);
    return (uint)(isDialogClosed != 0) << 2;
}

int battleEvent_OP1A_SetupDialogParams(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 5, 0, 1);

    for (int i = 0; i < 4; i++) {
        if (battleEventVar0->m380_currentArgs[i] == 0) {
            battleEventVar0->m7F6_dialogParams.mRaw[i] = battleEventDefaultDialogParams.mRaw[i];
        }
        else {
            battleEventVar0->m7F6_dialogParams.mRaw[i] = battleEventVar0->m380_currentArgs[i];
        }
    }
    battleEventVar0->m7F6_dialogParams.m8_flags = battleEventVar0->m380_currentArgs[4];

    return 0xB;
}

int battleEvent_OP1B(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    int arg = bytecode[1];
    if (arg > 0xf2) {
        assert(0);
    }
    battleEventVar0->m0_scriptEntities[currentEntityId].m24 = arg;
    return 2;
}

int battleEvent_OP1F(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleCreateFader(battleEventVar0->m380_currentArgs[0], 2, 0, 0, 0);
    return 3;
}

int battleEvent_OP23(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 3, 0, 1);

    int slotIndex = battleEventVar0->m380_currentArgs[0] - 0xF3;
    auto slot = &battleEventVar0->m0_scriptEntities[slotIndex];
    switch (slot->m34) {
    case 0:
        slot->m34 = 2;
        setupMechaForEvent(slotIndex, characterIdToTargetBitmask(battleEventVar0->m380_currentArgs[1] + 0xD), battleEventVar0->m380_currentArgs[2]);
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

    sBattleRunningVar0Sub* entry = &battleEventVar0->m0_scriptEntities[(battleEventVar0->m380_currentArgs[0] + 0xD) & 0xFF];
    if (entry->m35) {
        battleEvent_OP2A_sub0((sBattleSpriteActor*)entry->m30, battleEventVar0->m380_currentArgs[1]);
    }

    return 5;
}

int battleEvent_OP2B(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    if (battleEventVar0->m0_scriptEntities[currentEntityId].m28 == 0) {
        battleEvent_loadArgs(bytecode, 1, 0, 1);
        battleEventVar0->m0_scriptEntities[currentEntityId].m28 = 1;
        battleEventVar0->m0_scriptEntities[currentEntityId].m26 = battleEventVar0->m380_currentArgs[0] << 1;
    }
    int bytecodeSize = 0;
    if (battleEventVar0->m0_scriptEntities[currentEntityId].m26 == 0) {
        battleEventVar0->m0_scriptEntities[currentEntityId].m28 = 0;
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
    sBattleRunningVar0Sub* entry = &battleEventVar0->m0_scriptEntities[(battleEventVar0->m380_currentArgs[0] + 0xD) & 0xFF];
    if (entry->m35 == 0) {
        entry->m2C = new sSpriteActorAnimationBundle;
        std::vector<u8> rawBundle = mallocAndDecompress(battleEventFile3.begin() + READ_LE_U32(battleEventFile3.begin() + 4 + battleEventVar0->m380_currentArgs[1] * 4));
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


int battleEvent_OP38(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 3, 0, 1);

    int mechaIndex = battleEventVar0->m380_currentArgs[0] - 0xF3;
    mechaPlayAnimation_battle(mechaIndex, characterIdToTargetBitmask(battleEventVar0->m380_currentArgs[1] + 0xD), battleEventVar0->m380_currentArgs[2]);
    return 7;
}

void battleFader_update(int) {
    waitBattleAnimationSoundLoaded();
    updateCharacterBlinkingTask(0);

    if (battleEventVar0->m800 == 0) {
        bool continueEvent = true;
        do {
            for (int i = 0; i < battleEventVar2.m40; i++) {
                s8 currentEntityId = battleEventVar0->m794[i];
                battleRenderDebugAndMain();
                for (int j = 0; j < 4; j++) {
                    s16 bytecodeOffset = battleEventGetBytecodeOffset(&battleEventVar0->m0_scriptEntities[currentEntityId]);
                    u8 bytecode = READ_LE_U8(battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                    int bytecodeSize;
                    switch (bytecode) {
                    case 0x0:
                        bytecodeSize = battleEvent_OP0_Stop(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        j = 3; // to break out of the loop
                        break;
                    case 0x5:
                        bytecodeSize = battleEvent_OP5(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x18:
                        bytecodeSize = battleEvent_OP18_Dialog(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x1A:
                        bytecodeSize = battleEvent_OP1A_SetupDialogParams(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x1B:
                        bytecodeSize = battleEvent_OP1B(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x1F:
                        bytecodeSize = battleEvent_OP1F(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x23:
                        bytecodeSize = battleEvent_OP23(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x2A:
                        bytecodeSize = battleEvent_OP2A(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x2B:
                        bytecodeSize = battleEvent_OP2B(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x31:
                        bytecodeSize = battleEvent_OP31(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x35:
                        bytecodeSize = battleEvent_OP35(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x37:
                        bytecodeSize = battleEvent_OP37(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    case 0x38:
                        bytecodeSize = battleEvent_OP38(currentEntityId, battleEventVar0->m390_scriptByteCode + bytecodeOffset);
                        break;
                    default:
                        assert(0);
                    }
                    battleEventVar0->m0_scriptEntities[currentEntityId].m0_scriptBytecodeOffset[battleEventVar0->m0_scriptEntities[currentEntityId].m20_currentActiveEntry] += bytecodeSize;
                }
            }
        } while (continueEvent);
    }
}

void mecha_battle_op3_2(int param_1) {
    battleEventVar0->m0_scriptEntities[param_1].m34 = 1;
}