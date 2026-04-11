#include "noahLib.h"
#include "battle/battleEvent/battleEvent.h"
#include "battle/battle.h"
#include "battle/battleLoader.h"
#include "kernel/filesystem.h"
#include "battle/battleConfig.h"
#include "kernel/decompress.h"
#include "field/fieldGraphicObject.h"
#include "field/dialogWindows.h"
#include "battle/battleSpriteLoader.h"
#include "battle/battleDialogWindow.h"
#include "kernel/TIM.h"
#include "kernel/audio/seq.h"
#include "kernel/audio/soundInstance.h"
#include "movie/movie.h"
#include "psx/libgte.h"
#include "battle/battleRenderContext.h"
#include "battle/battleFader.h"

extern sSeqFile* gBattleCurrentSoundEffectSeq;
// PSX 0x800c3e54 — the battle event overlay's currently-playing music instance
// (OP2D loads/replaces it, OP2F starts playback, OP33 stops and frees it).
sSoundInstance* gBattleEventMusicInstance = nullptr;
extern sSeqFile battleMusic;
void clearMusic();
sSoundInstance* realStartBattleMusic(sSeqFile* param_1, int param_2, int param_3);
extern u8 returnToFieldFromBattleSpecial;

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
    auto file2Reloc = doPointerRelocation(file2.getRawData());
    auto file3Reloc = doPointerRelocation(battleEventFile3.getRawData());

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

    battleEventVar0->m81f_hasLoadedMusic = 0;

    setCurrentDir_20_0();
    battleEventVar0->m818 = new sSeqFile;
    readFile(4, *battleEventVar0->m818, 0, 0x80);
    idleBattleDuringLoading();
    loadSequence(battleEventVar0->m818);
    waitForMusic(0x10);
    battleEventVar0->m820 = 1;
    gCurrentBattleEventSeq = battleEventVar0->m818;
    waitBattleAnimationSoundLoaded();
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
                battleEventVar0->m380_currentArgs[i] = battleEventVar0->m394_vars[((iVar2 >> 16) - (iVar2 >> 31)) * 0x8000 >> 16];
            }
        }
        bytecode += 2;
    }
}

sTaskHeader* battleEvent_createSpriteEntity(sSpriteActorAnimationBundle* param_1, SVECTOR* param_2) {
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

// Forward declarations for opcodes that reference helpers defined further below.
int battleEvent_startScriptSlot(int currentEntityId, const std::vector<u8>::iterator& bytecode);
int battleEvent_DisplayDialog(u16 messageIndex, int protraitIndex, int flags);
u32 getRandomValueInRange(u32 param_1, u32 param_2);

// PSX globals set by OP20/OP21; mirrored in PC port. Both are zeroed by
// batteLoaderPhase1_0 at the start of every battle.
bool battleEventEndBattleFlag = false;
bool battleEventIsRunningFlag = false;

// Script variable helper: destination var index is the raw u16 at bytecode+1
// (byte offset into m394_vars, rounded down to u16 alignment).
static inline s16& battleEvent_varAt(const std::vector<u8>::iterator& bytecode) {
    u16 byteOffset = READ_LE_U16(bytecode + 1) & 0xfffe;
    return battleEventVar0->m394_vars[byteOffset >> 1];
}

// PSX 0x801e58ec: compares two s16 values with a comparator selector.
// 0=eq, 1=ne, 2=a<b, 3=b<a, 4=a<=b, 5=b<=a, 6=(a&b)!=0, 7=ne, 8=(a|b)!=0,
// 9=bitmaskCharacterCheck, 10=!bitmaskCharacterCheck
bool battleEvent_compareValues(u16 a, u16 b, u32 comparator) {
    switch (comparator & 0xf) {
    case 0: return a == b;
    case 1: return a != b;
    case 2: return (s16)b < (s16)a;
    case 3: return (s16)a < (s16)b;
    case 4: return (s16)b <= (s16)a;
    case 5: return (s16)a <= (s16)b;
    case 6: return (a & b) != 0;
    case 7: return a != b;
    case 8: return a != 0 || b != 0;
    case 9:  return bitmaskCharacterCheck(a, (u8)b) != 0;
    case 10: return bitmaskCharacterCheck(a, (u8)b) == 0;
    }
    return false;
}

int battleEvent_OP1_jump(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& entry = battleEventVar0->m0_scriptEntities[currentEntityId];
    entry.m0_scriptBytecodeOffset[entry.m20_currentActiveEntry] = READ_LE_U16(bytecode + 1);
    return 0;
}

int battleEvent_OP2_jumpIf(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    if (!battleEvent_compareValues(battleEventVar0->m380_currentArgs[0],
                                   battleEventVar0->m380_currentArgs[1],
                                   (u8)bytecode[5])) {
        auto& entry = battleEventVar0->m0_scriptEntities[currentEntityId];
        entry.m0_scriptBytecodeOffset[entry.m20_currentActiveEntry] =
            (u16)bytecode[6] | ((u16)(u8)bytecode[7] << 8);
        return 0;
    }
    return 8;
}

int battleEvent_OP4_waitScriptDone(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& thisEntity = battleEventVar0->m0_scriptEntities[currentEntityId];
    u8 waitTag = bytecode[2] & 0x1f;
    if (thisEntity.m23 == waitTag) {
        // Already waiting — check if the target script slot finished.
        auto& otherEntity = battleEventVar0->m0_scriptEntities[bytecode[1]];
        if (thisEntity.m23 == otherEntity.m21) {
            thisEntity.m23 = -1;
            return 3;
        }
        return 0;
    }
    battleEvent_startScriptSlot(currentEntityId, bytecode);
    return 0;
}

int battleEvent_OP6_setVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OP7_setVarTrue(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_varAt(bytecode) = 1;
    return 3;
}

int battleEvent_OP8_setVarFalse(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_varAt(bytecode) = 0;
    return 3;
}

int battleEvent_OP9_addVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) += battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OPA_subVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) -= battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OPB_orVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = (u16)battleEvent_varAt(bytecode) | (u16)battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OPC_clearBitsVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = (u16)battleEvent_varAt(bytecode) & ~(u16)battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OPD_incVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_varAt(bytecode) += 1;
    return 3;
}

int battleEvent_OPE_decVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_varAt(bytecode) -= 1;
    return 3;
}

int battleEvent_OPF_andVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = (u16)battleEvent_varAt(bytecode) & (u16)battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OP10_orVar2(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = (u16)battleEvent_varAt(bytecode) | (u16)battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OP11_xorVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = (u16)battleEvent_varAt(bytecode) ^ (u16)battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OP12_shlVar(int, const std::vector<u8>::iterator& bytecode) {
    u16 destRaw = READ_LE_U16(bytecode + 1);
    battleEvent_loadArgs(bytecode, 2, 0, 0);
    battleEventVar0->m394_vars[destRaw >> 1] <<= (battleEventVar0->m380_currentArgs[1] & 0x1f);
    return 5;
}

int battleEvent_OP13_shrVar(int, const std::vector<u8>::iterator& bytecode) {
    u16 destRaw = READ_LE_U16(bytecode + 1);
    battleEvent_loadArgs(bytecode, 2, 0, 0);
    battleEventVar0->m394_vars[destRaw >> 1] =
        (s16)((u16)battleEventVar0->m394_vars[destRaw >> 1] >> (battleEventVar0->m380_currentArgs[1] & 0x1f));
    return 5;
}

int battleEvent_OP14_randomVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_varAt(bytecode) = (s16)(getRandomValueInRange(0, 0x7fff));
    return 3;
}

int battleEvent_OP15_randomRangeVar(int, const std::vector<u8>::iterator& bytecode) {
    u16 upper = READ_LE_U16(bytecode + 1);
    u16 destRaw = READ_LE_U16(bytecode + 3);
    battleEventVar0->m394_vars[destRaw >> 1] = (s16)(getRandomValueInRange(0, upper));
    return 5;
}

int battleEvent_OP16_mulVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    battleEvent_varAt(bytecode) = battleEventVar0->m380_currentArgs[0] * battleEventVar0->m380_currentArgs[1];
    return 6;
}

int battleEvent_OP17_divVar(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, (u8)bytecode[5], 0);
    u16 divisor = (u16)battleEventVar0->m380_currentArgs[1];
    if (divisor != 0) {
        battleEvent_varAt(bytecode) = (s16)((u16)battleEventVar0->m380_currentArgs[0] / divisor);
    }
    return 6;
}

int battleEvent_OP19_showDialogAlt(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    u16 msgId = READ_LE_U16(bytecode + 2);
    int done = battleEvent_DisplayDialog(msgId, bytecode[1], bytecode[4]);
    return done != 0 ? 5 : 0;
}

int battleEvent_OP1C_setCameraMode2(int, const std::vector<u8>::iterator&) {
    currentBattleMode = 2;
    return 1;
}

int battleEvent_OP1D_setCameraMode1(int, const std::vector<u8>::iterator&) {
    currentBattleMode = 1;
    return 1;
}

int battleEvent_OP1E_fadeOut(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleCreateFader(battleEventVar0->m380_currentArgs[0], 2, 0xff, 0xff, 0xff);
    return 3;
}

int battleEvent_OP20_endBattle(int, const std::vector<u8>::iterator&) {
    battleEventEndBattleFlag = true;
    battleEventVar0->m800 = 1;
    return 1;
}

int battleEvent_OP21_setBattleRunning(int, const std::vector<u8>::iterator&) {
    battleEventIsRunningFlag = true;
    return 1;
}

int battleEvent_OP22_pauseEvent(int, const std::vector<u8>::iterator&) {
    battleEventVar0->m801 = 2;
    return 1;
}

int battleEvent_OP32_nop(int, const std::vector<u8>::iterator&) {
    return 1;
}

int battleEvent_OP34_nop2(int, const std::vector<u8>::iterator&) {
    return 1;
}

// PSX 0x8007ff14/0x800800e8: toggles the battle scene render. On PC the flag
// `renderBattleSceneDisabled` is already wired into battleDrawEnv.
extern s8 renderBattleSceneDisabled;

int battleEvent_OP42_disableBattleRendering(int, const std::vector<u8>::iterator&) {
    renderBattleSceneDisabled = 1;
    return 1;
}

int battleEvent_OP43_enableBattleRendering(int, const std::vector<u8>::iterator&) {
    renderBattleSceneDisabled = 0;
    return 1;
}

// PSX 0x801e7cd0 — load a music file into the battle event's music slot,
// freeing any previously-loaded instance first.
void battleEventLoadMusicFile(s16 musicId, u8 volume) {
    clearMusic();
    if (battleEventVar0->m81f_hasLoadedMusic) {
        freeSoundInstance(gBattleEventMusicInstance);
        battleRenderDebugAndMain();
    }
    setCurrentDir_20_0();
    battleEventVar0->m814_pLoadedMusic = &battleMusic;
    readFile(musicId + 4, battleMusic, 0, 0x80);
    idleBattleDuringLoading();
    battleEventVar0->m81f_hasLoadedMusic = 1;
    battleEventVar0->m81c_loadedMusicId = musicId;
    battleEventVar0->m81e_loadedMusicVol = (s8)volume;
    gBattleEventMusicInstance = realStartBattleMusic(&battleMusic, volume, 0);
}

int battleEvent_OP2D_loadMusic(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleEventLoadMusicFile(battleEventVar0->m380_currentArgs[0], 0x7f);
    return 3;
}

int battleEvent_OP2E_loadMusicQuiet(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleEventLoadMusicFile(battleEventVar0->m380_currentArgs[0], 0);
    return 3;
}

int battleEvent_OP2F_playLoadedMusic(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    battleEventVar0->m81e_loadedMusicVol = (s8)battleEventVar0->m380_currentArgs[0];
    playBattleMusic(gBattleEventMusicInstance,
                    battleEventVar0->m380_currentArgs[0],
                    battleEventVar0->m380_currentArgs[1]);
    return 5;
}

int battleEvent_OP30_stopMusic(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    s8 vol = 0;
    if (battleEventVar0->m380_currentArgs[0] == 0) {
        vol = battleEventVar0->m81e_loadedMusicVol;
    }
    playBattleMusic(gBattleEventMusicInstance, vol, 0);
    return 3;
}

int battleEvent_OP33_stopBattleMusic(int, const std::vector<u8>::iterator&) {
    if (battleEventVar0->m81f_hasLoadedMusic != 0) {
        startMusicInstanceSub0(gBattleEventMusicInstance);
        battleRenderDebugAndMain();
        freeSoundInstance(gBattleEventMusicInstance);
        battleEventVar0->m81f_hasLoadedMusic = 0;
    }
    return 1;
}

int battleEvent_OP41_playSong(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 3, 0, 1);
    sSeqFile* pSeq = gBattleCurrentSoundEffectSeq;
    if (battleEventVar0->m380_currentArgs[2] == 0) {
        pSeq = battleEventVar0->m818;
    }
    if (pSeq) {
        // PSX calls FUN_8003a2e4(songId, param) — a sequence-start variant
        // that isn't yet wired up on PC. Fall back to playSoundEffect for now.
        u32 songId = ((u32)pSeq->m14 << 16) | (u16)battleEventVar0->m380_currentArgs[0];
        playSoundEffect(songId);
    }
    return 7;
}

int battleEvent_OP48_setMusicVolume(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    // PSX calls func_0x800b838c(vol, pan). No C++ equivalent yet — this is
    // the battle music instance's volume/pan setter.
    MissingCode();
    return 5;
}

// PSX 0x801e5abc: maps a script-provided character id into its physical
// battleSpriteActorCores[] slot. Character ids below 0x10 are resolved
// through battleCharacters[]; higher ids are shifted by -0xd (enemy block).
u32 battleEvent_resolveEntityIndex(u32 param_1) {
    u8 c = (u8)param_1;
    if (c < 0x10) {
        for (u32 i = 0; i < 3; i++) {
            if ((u8)battleCharacters[i] != 0xff && (u8)battleCharacters[i] == c) {
                return i;
            }
        }
        return 0;
    }
    return (u32)((s32)param_1 - 0xd) & 0xff;
}

// PSX 0x801e950c: reasserts the sprite actor's stashed "idle" animation id
// (the high byte of the mAC bitfield), which is spriteActorSetPlayingAnimation's
// way of rewinding the current anim to its first frame.
void battleEventHideSpriteEntity(u32 slotIndex) {
    sSpriteActorCore* pCore = battleSpriteActorCores[slotIndex];
    if (pCore != nullptr) {
        spriteActorSetPlayingAnimation(pCore, (s8)pCore->mAC.mx18);
    }
}

// PSX 0x801e9550: makes the sprite actor visible again — zero the frame-wait
// counter and current frame, and clear the bottom-byte render bits of m40.
void battleEventShowSpriteEntity(u32 slotIndex) {
    sSpriteActorCore* pCore = battleSpriteActorCores[slotIndex];
    if (pCore != nullptr) {
        pCore->m9E_wait = 0;
        pCore->m34_currentSpriteFrame = 0;
        pCore->m40 &= 0xffffff03u;
    }
}

// PSX 0x801e958c: just zeroes the frame-wait counter (clears any active
// mirror/flip transition).
void battleEventSetSpriteFlip(u32 slotIndex) {
    sSpriteActorCore* pCore = battleSpriteActorCores[slotIndex];
    if (pCore != nullptr) {
        pCore->m9E_wait = 0;
    }
}

// PSX 0x801e9430: rebinds the sprite actor's cell/special animation to a new
// value (used by OP3A to steer the sprite's camera target animation).
void battleEventSetSpriteCameraAnim(u32 slotIndex, int animId) {
    (void)slotIndex;
    (void)animId;
    MissingCode();
}

// PSX 0x801e9ad4 — the sprite-destroy delete callback. Unregisters the task
// from the sprite callback lists, frees it, and resets the OP3A camera-anim
// bookkeeping. Matches the inner body of FUN_Battle_event__801e9ad4.
void battleEventDestroySpriteTaskCallback(sTaskHeader* pTask) {
    registerSpriteCallback2_2(pTask);
    registerSpriteCallback2Sub0(&((sTaskHeaderPair*)pTask)->m1C);
    allocateSavePointMeshDataSub0_callback(pTask);
    delete pTask;
    // PSX follows with func_0x800bc3f8(1)/func_0x800bc2f0(1) and DAT_800c37c8 = 0;
    // those are battle-camera reset helpers that aren't yet ported.
    MissingCode();
}

// PSX 0x801e7a5c: frees the sprite actor owned by the script entity at
// (scriptSlot + 0xd) and clears the m35 alive flag.
void battleEventDestroyScriptEntitySprite(int scriptSlot) {
    auto& entry = battleEventVar0->m0_scriptEntities[(scriptSlot + 0xd) & 0xff];
    if (entry.m35 != 0) {
        battleEventDestroySpriteTaskCallback(entry.m30);
        delete entry.m2C;
        entry.m2C = nullptr;
        entry.m30 = nullptr;
        entry.m35 = 0;
    }
}

// PSX 0x801e9b2c: zeroes the battle event's camera/animation state so the
// next sprite dispose starts with a clean slate.
void battleEventResetSpriteDisposeState() {
    MissingCode();
}

int battleEvent_OP29_setCameraTarget(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    // PSX builds an SVECTOR from args[0..2] and calls func_0x800b3658(&sv, args[3]) —
    // the battle camera target setter. Not wired up on PC.
    MissingCode();
    return 9;
}

int battleEvent_OP36_destroySprite(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleEventDestroyScriptEntitySprite(battleEventVar0->m380_currentArgs[0]);
    (void)currentEntityId;
    return 3;
}

int battleEvent_OP40_destroySpriteAndReset(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleEventDestroyScriptEntitySprite(battleEventVar0->m380_currentArgs[0]);
    battleEventResetSpriteDisposeState();
    (void)currentEntityId;
    return 3;
}

int battleEvent_OP3A_setCameraForEntity(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]);
    battleEventSetSpriteCameraAnim(slot, battleEventVar0->m380_currentArgs[1]);
    return 5;
}

int battleEvent_OP3B_hideEntity(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]);
    battleEventHideSpriteEntity(slot);
    return 3;
}

int battleEvent_OP3C_showEntity(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]);
    battleEventShowSpriteEntity(slot);
    return 3;
}

int battleEvent_OP3D_setEntityFlip(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]);
    battleEventSetSpriteFlip(slot);
    return 3;
}

// PSX 0x800c3d5c: battle menu enable flag. Written by OP25 and the loader,
// but not read anywhere else in the PSX binary — tracked here for parity.
u8 battleEventMenuEnabled = 0;

// PSX 0x801e95e4 — starts a linear move for the sprite at `slot` toward
// (targetX, targetY, targetZ), running over the next few frames. Details of
// the per-frame stepping aren't yet modelled on PC.
void battleEventStartSpriteMove(u32 slot, int targetX, int targetY, int targetZ) {
    (void)slot; (void)targetX; (void)targetY; (void)targetZ;
    MissingCode();
}

// PSX 0x801e9694 — alternate move, matching OP3F's handler (different easing).
void battleEventStartSpriteMoveAlt(u32 slot, int targetX, int targetY, int targetZ) {
    (void)slot; (void)targetX; (void)targetY; (void)targetZ;
    MissingCode();
}

// PSX 0x801e9894 — begins loading a character sprite bundle into `slot`,
// replacing whatever was there. Runs asynchronously; the opcode polls CD busy.
void battleEventStartCharacterSpriteLoad(u32 slot, u32 characterSourceSlot) {
    (void)slot; (void)characterSourceSlot;
    MissingCode();
}

// PSX 0x801e9700 — part of OP46; marks the source sprite with a swap flag.
void battleEventSwapSpritePrepare(u32 slot, u16 flags) {
    (void)slot; (void)flags;
    MissingCode();
}

// PSX 0x801e9760 — part of OP46; completes the swap between two sprites.
void battleEventSwapSpriteCommit(u32 dstSlot, u32 srcSlot) {
    (void)dstSlot; (void)srcSlot;
    MissingCode();
}

int battleEvent_OP24_setBattleTransition(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    battleInitVar0 = (u8)battleEventVar0->m380_currentArgs[0] + 1;
    battleTransitionEffect = (s8)battleEventVar0->m380_currentArgs[1];
    return 5;
}

int battleEvent_OP25_enableBattleMenu(int, const std::vector<u8>::iterator&) {
    battleEventMenuEnabled = 1;
    return 1;
}

int battleEvent_OP26_setFieldReturn(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    gameState.m231A_fieldID                     = (u16)battleEventVar0->m380_currentArgs[0];
    gameState.m231C_CameraYaw                   = (s16)battleEventVar0->m380_currentArgs[1];
    gameState.m231E_worldmapInitialPositionIndex = (s16)battleEventVar0->m380_currentArgs[2];
    gameState.m2320_worldmapMode                = (s16)battleEventVar0->m380_currentArgs[3];
    // PSX then calls FUN_8001ac94 — a game-state commit helper. Not wired on PC.
    MissingCode();
    return 9;
}

int battleEvent_OP27_playMovie(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    movieType = (u8)battleEventVar0->m380_currentArgs[0] | 0x80;
    movieReturnMode = 1;
    movieNumber = (u8)battleEventVar0->m380_currentArgs[1];
    movieFadeParam = (u8)battleEventVar0->m380_currentArgs[2];
    // PSX sets two more globals at 0x800d3338 / 0x80062514 (movie pre-clear &
    // a secondary movie arg). Not wired on PC.
    MissingCode();
    return 9;
}

int battleEvent_OP28_playSound(int, const std::vector<u8>::iterator& bytecode) {
    // PSX passes the 5 bytecode bytes verbatim to the fader routine at
    // 0x800b39c0 (battleCreateFader). Args: flag byte at bc[5], four params
    // at bc[1..4].
    battleCreateFader((u8)bytecode[1], (u8)bytecode[2], (u8)bytecode[3], (u8)bytecode[4], (u8)bytecode[5]);
    return 6;
}

int battleEvent_OP2C_setPriority(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    // Set this script entity's priority value.
    battleEventVar0->m0_scriptEntities[currentEntityId].m22 = (s8)bytecode[1];

    // If priority == -2, promote this entity to the front of the m794 queue.
    if ((s8)bytecode[1] == -2) {
        std::array<s8, 0x10> backup;
        for (int i = 0; i < 0x10; i++) {
            backup[i] = battleEventVar0->m794[i];
        }
        battleEventVar0->m794[0] = (s8)currentEntityId;
        int dst = 1;
        for (int i = 0; i < 0x10; i++) {
            if (backup[i] != (s8)currentEntityId) {
                if (dst < 0x10) {
                    battleEventVar0->m794[dst++] = backup[i];
                }
            }
        }
    }
    return 3;
}

int battleEvent_OP39_transformToGear(int, const std::vector<u8>::iterator&) {
    // PSX touches a cluster of gear-specific globals and helpers
    // (DAT_800ccd88, gameState.m26C_party[0].mA0_partyData_gearNum,
    // func_0x80088490, func_0x800baf48, battleVar2->m2EB, ...). The gear
    // transformation subsystem is largely unported on PC.
    MissingCode();
    return 1;
}

int battleEvent_OP3E_moveEntity(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]) & 0xff;
    if (battleEventInitVar1[slot] == 0) {
        battleEventVar0->m804[slot] = 1;
        battleEventInitVar1[slot] = 1;
        battleEventStartSpriteMove(slot,
                                   (s16)battleEventVar0->m380_currentArgs[1],
                                   (s16)battleEventVar0->m380_currentArgs[2],
                                   (s16)battleEventVar0->m380_currentArgs[3]);
        return 0;
    }
    if (battleEventVar0->m804[slot] == 0) {
        battleEventInitVar1[slot] = 0;
        return 9;
    }
    return 0;
}

int battleEvent_OP3F_moveEntityAlt(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]) & 0xff;
    if (battleEventInitVar1[slot] == 0) {
        battleEventVar0->m804[slot] = 1;
        battleEventInitVar1[slot] = 1;
        battleEventStartSpriteMoveAlt(slot,
                                      (s16)battleEventVar0->m380_currentArgs[1],
                                      (s16)battleEventVar0->m380_currentArgs[2],
                                      (s16)battleEventVar0->m380_currentArgs[3]);
        return 0;
    }
    if (battleEventVar0->m804[slot] == 0) {
        battleEventInitVar1[slot] = 0;
        return 9;
    }
    return 0;
}

int battleEvent_OP44_resetAllSprites(int, const std::vector<u8>::iterator&) {
    // PSX iterates an 11-entry pointer array at 0x800d3368 and clears each
    // entry's m35 alive flag. That table maps to battleSpriteActors[] on PC.
    for (int i = 0; i < 0xB; i++) {
        if (battleSpriteActors[i] != nullptr) {
            // m35 is a nested script-entity flag, not a direct sBattleSpriteActor
            // field on PC — gated on the sprite loader port.
            MissingCode();
        }
    }
    return 1;
}

int battleEvent_OP45_loadCharacterSprite(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);
    u32 destSlot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]) & 0xff;
    u32 srcSlot  = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[1]) & 0xff;

    // PSX sets battleVar2->m2DA_indexInBattleVar48 = 0 and calls func_0x80085388
    // (a battle-state commit helper). Wire the second, leave the first as a hint.
    MissingCode();

    // PSX also writes a flag byte at (srcSlot + 0x800c4000) — a monster/party
    // display byte somewhere in battleVar0. Not yet modelled on PC.
    if (battleEventInitVar1[destSlot] == 0) {
        battleEventVar0->m804[destSlot] = 1;
        battleEventInitVar1[destSlot] = 1;
        battleEventStartCharacterSpriteLoad(destSlot, srcSlot);
        while (isCDBusy()) {
            battleRenderDebugAndMain();
        }
        battleEventSetSpriteCameraAnim(destSlot, (s16)battleEventVar0->m380_currentArgs[2]);
        return 0;
    }
    if (battleEventVar0->m804[destSlot] == 0) {
        battleEventInitVar1[destSlot] = 0;
        return 9;
    }
    return 0;
}

int battleEvent_OP46_swapCharacterSprite(int, const std::vector<u8>::iterator& bytecode) {
    // PSX also zeroes battleVar2->m2DA_indexInBattleVar48 and calls
    // func_0x80085388; unported here.
    MissingCode();
    battleEvent_loadArgs(bytecode, 3, 0, 1);
    u32 dstSlot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]);
    u32 srcSlot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[1]);
    battleEventSwapSpritePrepare(dstSlot, (u16)battleEventVar0->m380_currentArgs[2]);
    battleEventSwapSpriteCommit(dstSlot, srcSlot);
    return 7;
}

int battleEvent_OP47_resetInputState(int, const std::vector<u8>::iterator&) {
    // PSX calls func_0x800b8d7c — the battle input state reset helper.
    MissingCode();
    return 1;
}

int battleEvent_OP49_setReturnMode(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    returnToFieldFromBattleSpecial = (u8)battleEventVar0->m380_currentArgs[0];
    return 3;
}

int battleEvent_OP4A_disablePlayerControl(int, const std::vector<u8>::iterator&) {
    // PSX calls func_0x8009c0e0(0) — sets the battle player-control
    // disabled flag. Not wired up on PC.
    MissingCode();
    return 1;
}

int battleEvent_OP4B_markCharacterDead(int, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    u32 slot = battleEvent_resolveEntityIndex((u8)battleEventVar0->m380_currentArgs[0]) & 0xff;
    // PSX writes a 16-bit OR 1 at (slot * 0x170 + 0x800CCD1E) — that's
    // battleEntities[slot].m0_base.m34 (or m36 depending on base). Guard with
    // a slot check and set the HP-zero / dead flag.
    if (slot < 11) {
        battleEntities[slot].m0_base.m36 |= 1;
    }
    return 3;
}

int battleEvent_OP0_endScript(int currentEntityId) {
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

int battleEvent_startScriptSlot(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    auto& thisEntity = battleEventVar0->m0_scriptEntities[currentEntityId];
    s16 uVar2 = battleEvent_findFreeScriptSlot(&battleEventVar0->m0_scriptEntities[bytecode[1]]) & 0xFF;
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

int battleEvent_OP5_waitScript(int currentEntityId, const std::vector<u8>::iterator& bytecode) {

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
        battleEvent_startScriptSlot(currentEntityId, bytecode);
    }
    return 0;
}

// Battle dialog portrait table: one (normal, mirrored) pair per character.
// Extracted from PSX Battle_event::801e9b5c (90 entries).
std::vector<std::array<u8, 2>> battleDialogPortraitsTable = { {
    {0x00, 0x00}, // Fei
    {0x06, 0x06}, // Elly
    {0x11, 0x11}, // Citan
    {0x13, 0x14}, // Bart (asymmetric)
    {0x15, 0x15},
    {0x17, 0x17},
    {0x18, 0x18},
    {0x1C, 0x1C},
    {0x1B, 0x1B},
    {0x11, 0x11},
    {0x19, 0x19},
    {0x22, 0x22},
    {0x23, 0x23},
    {0x24, 0x24},
    {0x25, 0x25},
    {0x4F, 0x4F},
    {0x52, 0x52},
    {0x53, 0x53},
    {0x1A, 0x1A},
    {0x34, 0x34},
    {0x51, 0x51},
    {0x4D, 0x4D},
    {0x4E, 0x4E},
    {0x21, 0x21},
    {0x29, 0x29},
    {0x1D, 0x1D},
    {0x2B, 0x2B},
    {0x32, 0x33}, // asymmetric
    {0x2A, 0x2A},
    {0x35, 0x35},
    {0x38, 0x38},
    {0x26, 0x26},
    {0x01, 0x01},
    {0x02, 0x02},
    {0x03, 0x03},
    {0x04, 0x04},
    {0x05, 0x05},
    {0x07, 0x07},
    {0x08, 0x08},
    {0x09, 0x09},
    {0x0A, 0x0A},
    {0x0B, 0x0B},
    {0x0C, 0x0C},
    {0x0D, 0x0D},
    {0x0E, 0x0E},
    {0x0F, 0x0F},
    {0x10, 0x10},
    {0x12, 0x12},
    {0x1E, 0x1E},
    {0x1F, 0x1F},
    {0x20, 0x20},
    {0x27, 0x27},
    {0x28, 0x28},
    {0x2C, 0x2C},
    {0x2D, 0x2D},
    {0x2E, 0x2E},
    {0x2F, 0x2F},
    {0x30, 0x30},
    {0x31, 0x31},
    {0x36, 0x36},
    {0x16, 0x16},
    {0x39, 0x39},
    {0x3A, 0x3A},
    {0x3B, 0x3B},
    {0x3C, 0x3C},
    {0x3D, 0x3D},
    {0x3E, 0x3E},
    {0x3F, 0x3F},
    {0x40, 0x40},
    {0x41, 0x41},
    {0x42, 0x42},
    {0x43, 0x43},
    {0x44, 0x44},
    {0x45, 0x45},
    {0x46, 0x46},
    {0x47, 0x47},
    {0x48, 0x48},
    {0x49, 0x49},
    {0x4A, 0x4A},
    {0x4B, 0x4B},
    {0x4C, 0x4C},
    {0x50, 0x50},
    {0x37, 0x37},
    {0x54, 0x54},
    {0x55, 0x55},
    {0x56, 0x56},
    {0x57, 0x57},
    {0x58, 0x58},
    {0x59, 0x59},
    {0x5A, 0x5A},
} };

void battleDialogLoadPortrait(uint param_1, uint param_2, short x, short y, short width) {
    int portraitId = battleDialogPortraitsTable[param_1 & 0xff][param_2 & 1];
    setCurrentDirectory(4, 0);
    int size = getFileSizeAligned(portraitId + 0x46);

    {
        sLoadableDataRaw portraitFile;
        readFile(portraitId + 0x46, portraitFile, 0, 0x80);
        idleBattleDuringLoading();
        OpenTIM(portraitFile.getRawData().begin());
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
        battleEventVar1->m10_flags.m02 = 1;
        resetDialogWindow18(battleEventVar1);
        addDialogWindowsToOTSub2(battleEventVar1, getDialogParamPointer(battleEventVarDialogueBundle.begin(), messageIndex));
        battleVar1->mC9_dialog = 1;
        battleEventVar0->m802_dialogBoxInitialized = 1;
        battleRenderDebugAndMain();
    }
    if (battleEventVar1->m10_flags.m08) {
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
    if (!battleEventVar1->m10_flags.m04) {
        battleVar1->mC9_dialog = 0;
        dealocateDialogWindow18Memory(battleEventVar1);
        battleRenderDebugAndMain();
        battleVar1->mC8_dialogPortrait = 0;
        if (!(flags & 8)) {
            clearBattleDialogWindow(0);
        }
        battleEventVar0->m802_dialogBoxInitialized = 0;
        battleEventVar0->m7F6_dialogParams = battleEventDefaultDialogParams;
        return 1;
    }
    return 0;
}

int battleEvent_OP18_showDialog(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    // print dialog window in battle event
    char isDialogClosed = battleEvent_DisplayDialog(READ_LE_U16(bytecode + 1), battleEventVar0->m0_scriptEntities[currentEntityId].m24, bytecode[3]);
    return (uint)(isDialogClosed != 0) << 2;
}

int battleEvent_OP1A_setDialogParams(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
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

int battleEvent_OP1B_setPortrait(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    int arg = bytecode[1];
    // PSX loads *(u8*)(0x800d2c31 + arg) when arg > 0xf2; for arg 0xf3..0xf5
    // that maps to the three battle party slots at 0x800d2d24..26, which is
    // the battleCharacters[] array on PC. Higher arg values spill into
    // neighbouring battle state and are not used by shipped scripts.
    if (arg > 0xf2) {
        int slot = arg - 0xf3;
        if (slot < 3) {
            arg = battleCharacters[slot];
        }
    }
    battleEventVar0->m0_scriptEntities[currentEntityId].m24 = (u8)arg;
    return 2;
}

int battleEvent_OP1F_fadeIn(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 1, 0, 1);
    battleCreateFader(battleEventVar0->m380_currentArgs[0], 2, 0, 0, 0);
    return 3;
}

int battleEvent_OP23_setupMecha(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
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

void battleEvent_setSpriteAnimation(sBattleSpriteActor* pTaskHeader, int index) {
    spriteActorSetPlayingAnimation(&pTaskHeader->m38_spriteActor, index);
}

int battleEvent_OP2A_playAnimation(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);

    sBattleRunningVar0Sub* entry = &battleEventVar0->m0_scriptEntities[(battleEventVar0->m380_currentArgs[0] + 0xD) & 0xFF];
    if (entry->m35) {
        battleEvent_setSpriteAnimation((sBattleSpriteActor*)entry->m30, battleEventVar0->m380_currentArgs[1]);
    }

    return 5;
}

int battleEvent_OP2B_wait(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
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

int battleEvent_OP31_playSoundEffect(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 4, 0, 1);

    // args[0] = sfx id, args[1] = volume, args[2] = pan, args[3] = bank select
    sSeqFile* pSeq = gBattleCurrentSoundEffectSeq;
    if (battleEventVar0->m380_currentArgs[3] != 0) {
        pSeq = battleEventVar0->m818;
    }
    if (pSeq) {
        u32 sfxId = ((u32)pSeq->m14 << 16) | (u16)battleEventVar0->m380_currentArgs[0];
        // TODO: vol (args[1]) and pan (args[2]) are ignored — playSoundEffect
        // uses fixed 0x6000/0x4000. Needs a playSoundEffectVolPan variant.
        playSoundEffect(sfxId);
    }
    return 9;
}

int battleEvent_OP35_createSprite(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 2, 0, 1);
    sBattleRunningVar0Sub* entry = &battleEventVar0->m0_scriptEntities[(battleEventVar0->m380_currentArgs[0] + 0xD) & 0xFF];
    if (entry->m35 == 0) {
        entry->m2C = new sSpriteActorAnimationBundle;
        std::vector<u8> rawBundle = mallocAndDecompress(battleEventFile3.getRawData().begin() + READ_LE_U32(battleEventFile3.getRawData().begin() + 4 + battleEventVar0->m380_currentArgs[1] * 4));
        entry->m2C->init(rawBundle);
        SVECTOR position = { 0,0,0 };
        entry->m30 = battleEvent_createSpriteEntity(entry->m2C, &position);
        entry->m35 = 1;
    }
    return 5;
}

int battleEvent_OP37_startBattle(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    isBattleAnEvent = 1;
    battleEventVar0->m800 = 1;
    battleRunningVar1 = 1;
    return 1;
}


int battleEvent_OP38_playMechaAnim(int currentEntityId, const std::vector<u8>::iterator& bytecode) {
    battleEvent_loadArgs(bytecode, 3, 0, 1);

    int mechaIndex = battleEventVar0->m380_currentArgs[0] - 0xF3;
    mechaPlayAnimation_battle(mechaIndex, characterIdToTargetBitmask(battleEventVar0->m380_currentArgs[1] + 0xD), battleEventVar0->m380_currentArgs[2]);
    return 7;
}

void battleEvent_update(int) {
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
                    auto bc = battleEventVar0->m390_scriptByteCode + bytecodeOffset;
                    switch (bytecode) {
                    case 0x00: bytecodeSize = battleEvent_OP0_endScript(currentEntityId); j = 3; break;
                    case 0x01: bytecodeSize = battleEvent_OP1_jump(currentEntityId, bc); break;
                    case 0x02: bytecodeSize = battleEvent_OP2_jumpIf(currentEntityId, bc); break;
                    case 0x04: bytecodeSize = battleEvent_OP4_waitScriptDone(currentEntityId, bc); break;
                    case 0x05: bytecodeSize = battleEvent_OP5_waitScript(currentEntityId, bc); break;
                    case 0x06: bytecodeSize = battleEvent_OP6_setVar(currentEntityId, bc); break;
                    case 0x07: bytecodeSize = battleEvent_OP7_setVarTrue(currentEntityId, bc); break;
                    case 0x08: bytecodeSize = battleEvent_OP8_setVarFalse(currentEntityId, bc); break;
                    case 0x09: bytecodeSize = battleEvent_OP9_addVar(currentEntityId, bc); break;
                    case 0x0A: bytecodeSize = battleEvent_OPA_subVar(currentEntityId, bc); break;
                    case 0x0B: bytecodeSize = battleEvent_OPB_orVar(currentEntityId, bc); break;
                    case 0x0C: bytecodeSize = battleEvent_OPC_clearBitsVar(currentEntityId, bc); break;
                    case 0x0D: bytecodeSize = battleEvent_OPD_incVar(currentEntityId, bc); break;
                    case 0x0E: bytecodeSize = battleEvent_OPE_decVar(currentEntityId, bc); break;
                    case 0x0F: bytecodeSize = battleEvent_OPF_andVar(currentEntityId, bc); break;
                    case 0x10: bytecodeSize = battleEvent_OP10_orVar2(currentEntityId, bc); break;
                    case 0x11: bytecodeSize = battleEvent_OP11_xorVar(currentEntityId, bc); break;
                    case 0x12: bytecodeSize = battleEvent_OP12_shlVar(currentEntityId, bc); break;
                    case 0x13: bytecodeSize = battleEvent_OP13_shrVar(currentEntityId, bc); break;
                    case 0x14: bytecodeSize = battleEvent_OP14_randomVar(currentEntityId, bc); break;
                    case 0x15: bytecodeSize = battleEvent_OP15_randomRangeVar(currentEntityId, bc); break;
                    case 0x16: bytecodeSize = battleEvent_OP16_mulVar(currentEntityId, bc); break;
                    case 0x17: bytecodeSize = battleEvent_OP17_divVar(currentEntityId, bc); break;
                    case 0x18: bytecodeSize = battleEvent_OP18_showDialog(currentEntityId, bc); break;
                    case 0x19: bytecodeSize = battleEvent_OP19_showDialogAlt(currentEntityId, bc); break;
                    case 0x1A: bytecodeSize = battleEvent_OP1A_setDialogParams(currentEntityId, bc); break;
                    case 0x1B: bytecodeSize = battleEvent_OP1B_setPortrait(currentEntityId, bc); break;
                    case 0x1C: bytecodeSize = battleEvent_OP1C_setCameraMode2(currentEntityId, bc); break;
                    case 0x1D: bytecodeSize = battleEvent_OP1D_setCameraMode1(currentEntityId, bc); break;
                    case 0x1E: bytecodeSize = battleEvent_OP1E_fadeOut(currentEntityId, bc); break;
                    case 0x1F: bytecodeSize = battleEvent_OP1F_fadeIn(currentEntityId, bc); break;
                    case 0x20: bytecodeSize = battleEvent_OP20_endBattle(currentEntityId, bc); break;
                    case 0x21: bytecodeSize = battleEvent_OP21_setBattleRunning(currentEntityId, bc); break;
                    case 0x22: bytecodeSize = battleEvent_OP22_pauseEvent(currentEntityId, bc); break;
                    case 0x23: bytecodeSize = battleEvent_OP23_setupMecha(currentEntityId, bc); break;
                    case 0x24: bytecodeSize = battleEvent_OP24_setBattleTransition(currentEntityId, bc); break;
                    case 0x25: bytecodeSize = battleEvent_OP25_enableBattleMenu(currentEntityId, bc); break;
                    case 0x26: bytecodeSize = battleEvent_OP26_setFieldReturn(currentEntityId, bc); break;
                    case 0x27: bytecodeSize = battleEvent_OP27_playMovie(currentEntityId, bc); break;
                    case 0x28: bytecodeSize = battleEvent_OP28_playSound(currentEntityId, bc); break;
                    case 0x29: bytecodeSize = battleEvent_OP29_setCameraTarget(currentEntityId, bc); break;
                    case 0x2A: bytecodeSize = battleEvent_OP2A_playAnimation(currentEntityId, bc); break;
                    case 0x2B: bytecodeSize = battleEvent_OP2B_wait(currentEntityId, bc); break;
                    case 0x2C: bytecodeSize = battleEvent_OP2C_setPriority(currentEntityId, bc); break;
                    case 0x2D: bytecodeSize = battleEvent_OP2D_loadMusic(currentEntityId, bc); break;
                    case 0x2E: bytecodeSize = battleEvent_OP2E_loadMusicQuiet(currentEntityId, bc); break;
                    case 0x2F: bytecodeSize = battleEvent_OP2F_playLoadedMusic(currentEntityId, bc); break;
                    case 0x30: bytecodeSize = battleEvent_OP30_stopMusic(currentEntityId, bc); break;
                    case 0x31: bytecodeSize = battleEvent_OP31_playSoundEffect(currentEntityId, bc); break;
                    case 0x32: bytecodeSize = battleEvent_OP32_nop(currentEntityId, bc); break;
                    case 0x33: bytecodeSize = battleEvent_OP33_stopBattleMusic(currentEntityId, bc); break;
                    case 0x34: bytecodeSize = battleEvent_OP34_nop2(currentEntityId, bc); break;
                    case 0x35: bytecodeSize = battleEvent_OP35_createSprite(currentEntityId, bc); break;
                    case 0x36: bytecodeSize = battleEvent_OP36_destroySprite(currentEntityId, bc); break;
                    case 0x37: bytecodeSize = battleEvent_OP37_startBattle(currentEntityId, bc); break;
                    case 0x38: bytecodeSize = battleEvent_OP38_playMechaAnim(currentEntityId, bc); break;
                    case 0x39: bytecodeSize = battleEvent_OP39_transformToGear(currentEntityId, bc); break;
                    case 0x3A: bytecodeSize = battleEvent_OP3A_setCameraForEntity(currentEntityId, bc); break;
                    case 0x3B: bytecodeSize = battleEvent_OP3B_hideEntity(currentEntityId, bc); break;
                    case 0x3C: bytecodeSize = battleEvent_OP3C_showEntity(currentEntityId, bc); break;
                    case 0x3D: bytecodeSize = battleEvent_OP3D_setEntityFlip(currentEntityId, bc); break;
                    case 0x3E: bytecodeSize = battleEvent_OP3E_moveEntity(currentEntityId, bc); break;
                    case 0x3F: bytecodeSize = battleEvent_OP3F_moveEntityAlt(currentEntityId, bc); break;
                    case 0x40: bytecodeSize = battleEvent_OP40_destroySpriteAndReset(currentEntityId, bc); break;
                    case 0x41: bytecodeSize = battleEvent_OP41_playSong(currentEntityId, bc); break;
                    case 0x42: bytecodeSize = battleEvent_OP42_disableBattleRendering(currentEntityId, bc); break;
                    case 0x43: bytecodeSize = battleEvent_OP43_enableBattleRendering(currentEntityId, bc); break;
                    case 0x44: bytecodeSize = battleEvent_OP44_resetAllSprites(currentEntityId, bc); break;
                    case 0x45: bytecodeSize = battleEvent_OP45_loadCharacterSprite(currentEntityId, bc); break;
                    case 0x46: bytecodeSize = battleEvent_OP46_swapCharacterSprite(currentEntityId, bc); break;
                    case 0x47: bytecodeSize = battleEvent_OP47_resetInputState(currentEntityId, bc); break;
                    case 0x48: bytecodeSize = battleEvent_OP48_setMusicVolume(currentEntityId, bc); break;
                    case 0x49: bytecodeSize = battleEvent_OP49_setReturnMode(currentEntityId, bc); break;
                    case 0x4A: bytecodeSize = battleEvent_OP4A_disablePlayerControl(currentEntityId, bc); break;
                    case 0x4B: bytecodeSize = battleEvent_OP4B_markCharacterDead(currentEntityId, bc); break;
                    default:
                        Noah_CategorizedLog(log_warning, "Unimplemented battleEvent opcode 0x%02X", (u32)bytecode);
                        assert(0);
                    }
                    battleEventVar0->m0_scriptEntities[currentEntityId].m0_scriptBytecodeOffset[battleEventVar0->m0_scriptEntities[currentEntityId].m20_currentActiveEntry] += bytecodeSize;
                }
            }
            if (battleEventVar0->m801 != 0 && --battleEventVar0->m801 == 1) {
                continueEvent = false;
            }
        } while (continueEvent);
    }
}

void mecha_battle_op3_2(int param_1) {
    battleEventVar0->m0_scriptEntities[param_1].m34 = 1;
}