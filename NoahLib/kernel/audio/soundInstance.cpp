#include "noahLib.h"
#include "soundInstance.h"
#include "spu.h"
#include "seq.h"
#include "wds.h"
#include "soundSystem.h"

int numMaxSoundEffectInstances = 0;
int playSoundEffectVar0 = 0;

sSoundInstance* pPlayingSoundsLinkedList = nullptr;
sSoundInstance* pSoundEffectsInstances = nullptr;

void freeSoundInstanceField4(sSoundInstance* param_1) {
    if (param_1->m4) {
        assert(0);
    }
}

void initSoundInstanceDefaults(sSoundInstance* pSoundInstance) {
    freeSoundInstanceField4(pSoundInstance);
    pSoundInstance->m32 = 1;
    pSoundInstance->m36 = 1;
    pSoundInstance->m3A = 0x30;
    pSoundInstance->m64 = 0x1000000;
    pSoundInstance->m70 = 0x7f000000;
    pSoundInstance->m58 = 0x660000;
    pSoundInstance->m54 = 0x6600;
    pSoundInstance->m1A = 0;
    pSoundInstance->m1B = 0;
    pSoundInstance->m30 = 0;
    pSoundInstance->m34 = 0;
    pSoundInstance->m38 = 4;
    pSoundInstance->m3C = 4;
    pSoundInstance->m3E = 4;
    pSoundInstance->m28 = 0;
    pSoundInstance->m24 = 0;
    pSoundInstance->m20 = 0;
    pSoundInstance->m48_activeVoicesBF = 0;
    pSoundInstance->m7C = 0;
    pSoundInstance->m88 = 0;
    pSoundInstance->m6C = 0;
    pSoundInstance->m78 = 0;
    pSoundInstance->m84 = 0;
    pSoundInstance->m90 = 0;
    pSoundInstance->m5C = 0;
    pSoundInstance->m60 = 0;
    pSoundInstance->m50 = 0x10000;
}

void initSoundEffectSoundInstance(sSoundInstance* pSoundInstance) {
    pSoundInstance->m10_flags = 2;
    pSoundInstance->m12 = 0x7fff;
    pSoundInstance->m16_sampleBank = 0;
    pSoundInstance->m18 = 0x7f;
    pSoundInstance->m14_count = numMaxSoundEffectInstances;
    initSoundInstanceDefaults(pSoundInstance);
}

void addActiveSoundInstance(sSoundInstance* pSoundInstance) {
    DisableEvent(audioTickEvent);
    pSoundInstance->m0_pNext = pPlayingSoundsLinkedList;
    pPlayingSoundsLinkedList = pSoundInstance;
    EnableEvent(audioTickEvent);
}

sSoundInstance* initSoundEffectInstances(uint param_1) {
    numMaxSoundEffectInstances = param_1 & 0xfffffffe;
    sSoundInstance* pSoundInstance = new sSoundInstance(numMaxSoundEffectInstances, 0); // This was allocated a bit differently, and from the sound arena
    initSoundEffectSoundInstance(pSoundInstance);

    for (int i = 0; i < numMaxSoundEffectInstances; i++) {
        pSoundInstance->m94_events[i].m0 = 0;
        pSoundInstance->m94_events[i].m6 = i;
        pSoundInstance->m94_events[i].m27 = 0x18 - numMaxSoundEffectInstances + i;
    }

    addActiveSoundInstance(pSoundInstance);

    return pSoundInstance;
}

void setupAdsr(int param_1, sSoundInstanceEvent* param_2) {
    param_2->m26 = (char)param_1;
    auto pbVar5 = param_2->m2C_pWds->m_rawData.begin() + 0x30 + ((param_1 << 0x10) >> 0xc);
    u32 iVar4 = READ_LE_U32(pbVar5);
    param_2->m4C = iVar4 * 8 + param_2->m2C_pWds->m28;
    param_2->m50 = iVar4 * 8 + READ_LE_U16(pbVar5 + 4) * 8;

    u16 uVar1 = READ_LE_U16(pbVar5 + 0xC);
    param_2->m54 = (byte)uVar1 & 7;
    param_2->m55 = (byte)(uVar1 >> 4) & 7;
    param_2->m56 = (byte)(uVar1 >> 8) & 7;

    u32 uVar6 = READ_LE_U32(pbVar5 + 8);
    param_2->m57 = (byte)uVar6 & 0x7f;
    param_2->m58 = (byte)(uVar6 >> 8) & 0xf;
    param_2->m59 = (byte)(uVar6 >> 0x10) & 0x7f;
    param_2->m5B = (byte)(uVar6 >> 0xc) & 0xf;
    byte bVar3 = (byte)(uVar6 >> 0x18) & 0x1f;
    param_2->m28 = bVar3;
    param_2->m5A = bVar3;

    u16 sVar2 = READ_LE_U16(pbVar5 + 0x6);
    param_2->m0 = param_2->m0 | 0x8000;
    param_2->m6C = sVar2;
}

u32 playSoundEffectSubSub1BF1 = 0;
u32 playSoundEffectSubSub1BF2 = 0;

std::array<sSoundInstanceEvent30*, 0x18> playSoundEffectSubSub1Var0;

void playSoundEffectSubSub0(sSoundInstanceEvent30* param_1, int param_2) {
    if ((param_2 < 0x18) && (playSoundEffectSubSub1Var0[param_2] == param_1)) {
        playSoundEffectSubSub1Var0[param_2] = nullptr;
        u32 uVar1 = 1 << (param_2 & 0x1f);
        playSoundEffectSubSub1BF1 = uVar1 | playSoundEffectSubSub1BF1;
        playSoundEffectSubSub1BF2 = ~uVar1 & playSoundEffectSubSub1BF2;
    }
}

void playSoundEffectSubSub1(sSoundInstanceEvent30* param_1, u32 param_2) {
    if (param_2 < 0x18) {
        if (playSoundEffectSubSub1Var0[param_2] == param_1) {
            playSoundEffectSubSub1BF1 |= 1 << (param_2 & 0x1f);
        }
        else if ((playSoundEffectSubSub1Var0[param_2] == nullptr) || (playSoundEffectSubSub1Var0[param_2]->m4 <= param_1->m4)) {
            param_1->m6 = -1;
            param_1->m0 = param_2;
            playSoundEffectSubSub1Var0[param_2] = param_1;
            playSoundEffectSubSub1BF1 |= (1 << (param_2 & 0x1f));
            playSoundEffectSubSub1BF2 &= ~(1 << (param_2 & 0x1f));
        }
    }
}

void playSoundEffectSub(uint param_1, uint param_2, short param_3, u16 param_4) {
    sSoundInstance* pSoundInstance = pSoundEffectsInstances;

    sSeqFile* pCurrentSeq = pLoadedSequencesLinkedList;
    while (pCurrentSeq) {
        if (pCurrentSeq->m14 == param_2 >> 0x10) {
            sWdsFile* pWds = findInWdsLinkedList(pCurrentSeq->m16_sampleBank);
            if (pWds == nullptr) {
                pWds = pLoadedWdsLinkedList;
            }

            u32 uVar9 = (int)param_3 * READ_LE_U8(pCurrentSeq->m_rawData.begin() + pCurrentSeq->m18 + (param_2 & 0xFFFF));
            s16 uVar8 = (s16)(uVar9 >> 7);
            if ((uVar9 >> 0x16 & 1) != 0) {
                uVar8 = 0x7fff;
            }
            
            DisableEvent(audioTickEvent);
            for (int i = 0; i < playSoundEffectVar0; i++) {
                sSoundInstanceEvent* pSoundEvent = &pSoundEffectsInstances->m94_events[param_1 & 0xFF + i];
                auto pDrumData = pCurrentSeq->m_rawData.begin() + 0x20 + (param_2 & 0xFFFF) * 2 + 2 * i;

                pSoundEvent->m8 = param_2;
                pSoundEvent->mC = spuUpdateCounter;
                pSoundEvent->m7 = param_1 >> 8;
                if (READ_LE_U16(pDrumData) == 0) {
                    pSoundInstance->m48_activeVoicesBF = ~(1 << (pSoundEvent->m6 & 0x1f)) & pSoundInstance->m48_activeVoicesBF;
                    pSoundEvent->m0 = 0;
                    playSoundEffectSubSub0(&pSoundEvent->m30, pSoundEvent->m27);
                }
                else {
                    pSoundInstance->m48_activeVoicesBF = 1 << (pSoundEvent->m6 & 0x1f) | pSoundInstance->m48_activeVoicesBF;
                    pSoundEvent->m0 = 0x409;
                    if ((pCurrentSeq->m10 & 1U) != 0) {
                        pSoundEvent->m0 = 0x40b;
                    }
                    pSoundEvent->m2 = 0x170;
                    pSoundEvent->m4 = 0;
                    pSoundEvent->m66_octave = 0x3C;
                    pSoundEvent->m62 = 0xF;
                    pSoundEvent->m72_callstackDepth = -1;
                    pSoundEvent->m18_infinitLoopStart.reset();
                    pSoundEvent->m1C = 0;
                    pSoundEvent->m20 = 0;
                    pSoundEvent->m22 = 0;

                    pSoundEvent->m5C_deltaTime = 0;
                    pSoundEvent->m60 = 0;
                    pSoundEvent->m6E = 0;
                    pSoundEvent->m64 = 0;
                    pSoundEvent->m76 = uVar8;
                    pSoundEvent->m78_volume = 0x7f000000;

                    pSoundEvent->m70 = 0;
                    pSoundEvent->mD0 = 0;
                    pSoundEvent->mD2 = 0;
                    pSoundEvent->mD4 = 0;
                    pSoundEvent->m3C = 0;
                    pSoundEvent->m3E = 0;
                    pSoundEvent->mCE = 0;
                    pSoundEvent->m74_pan = param_4;
                    pSoundEvent->m10 = pCurrentSeq->m_rawData.begin() + READ_LE_U16(pDrumData);
                    pSoundEvent->m14 = pCurrentSeq->m_rawData.begin() + READ_LE_U16(pDrumData);

                    // Todo: cross-check that
                    for (int i = 0; i < 3; i++) {
                        pSoundEvent->mD8[i].m1E = 0;
                    }

                    pSoundEvent->m2C_pWds = pWds;
                    pSoundEvent->m25 = pCurrentSeq->m16_sampleBank & 0xFF;
                    if (pWds != nullptr) {
                        setupAdsr(0, pSoundEvent);
                    }
                    pSoundEvent->m30.m2 = 0;
                    pSoundEvent->m30.m4 = 0x200;
                    playSoundEffectSubSub1(&pSoundEvent->m30, pSoundEvent->m27);
                }
            }
            pSoundInstance->m10_flags |= 0x8000;
            EnableEvent(audioTickEvent);
            return;
        }
        pCurrentSeq = pCurrentSeq->m1C_pNext;
    }
}

void initSoundInstance(sSoundInstance* param_1) {
    // There was a dummy call here

    auto pSeq = param_1->m8_pSeq;

    param_1->m10_flags = param_1->m10_flags | 1;
    param_1->m12 = pSeq->m10;
    param_1->m14_count = pSeq->m14 & 0xFF;
    param_1->m16_sampleBank = pSeq->m16_sampleBank;
    param_1->m18 = pSeq->m18;
    param_1->m41 = pSeq->m1A;
    param_1->m44 = (ushort)(byte)pSeq->m1B << 8;
    param_1->m42 = pSeq->m1C_Original;
    param_1->m43 = pSeq->m1D_Original;
    if ((musicStatusFlag & 0x1000) != 0) {
        MissingCode();
    }
    initSoundInstanceDefaults(param_1);
}

void initSoundInstanceTracks(sSoundInstance* param_1) {
    if (param_1->m14_count) {
        u32 usedVoicesBitField = 0;
        sWdsFile* pWdsFile = findInWdsLinkedList(param_1->m16_sampleBank);
        if (pWdsFile == nullptr) {
            pWdsFile = pLoadedWdsLinkedList;
        }
        for (int i = 0; i < param_1->m14_count; i++) {
            sSoundInstanceEvent* pSoundEvent = &param_1->m94_events[i];
            auto pDrumData = param_1->m8_pSeq->m_rawData.begin() + 0x22 + i * 2;

            if (READ_LE_U16(pDrumData) == 0) {
                pSoundEvent->m0 = 0;
            }
            else {
                usedVoicesBitField |= 1 << (i & 0x1f);
                if ((1 << (i & 0x1f) & param_1->m4C) == 0) {
                    pSoundEvent->m0 = 0x401;
                }
                else {
                    pSoundEvent->m0 = 0x421;
                }

                if ((param_1->m10_flags & 4) != 0) {
                    pSoundEvent->m0 |= 4;
                }

                pSoundEvent->m2 = 0x170;
                pSoundEvent->m4 = 0;
                pSoundEvent->m7 = 0x10;
                pSoundEvent->m6 = i;
                pSoundEvent->m8 = param_1->m8_pSeq->m10;

                pSoundEvent->m66_octave = 0x3C;
                pSoundEvent->m62 = 0xF;
                pSoundEvent->m72_callstackDepth = -1;

                pSoundEvent->m76 = 0x6000;
                pSoundEvent->m78_volume = 0x7f000000;

                pSoundEvent->m18_infinitLoopStart.reset();
                pSoundEvent->m1C = 0;
                pSoundEvent->m20 = 0;
                pSoundEvent->m22 = 0;
                pSoundEvent->m5C_deltaTime = 0;
                pSoundEvent->m60 = 0;
                pSoundEvent->m6E = 0;
                pSoundEvent->m64 = 0;
                pSoundEvent->m74_pan = 0x4000;
                pSoundEvent->m70 = 0;

                pSoundEvent->mD0 = 0;
                pSoundEvent->mD2 = 0;
                pSoundEvent->mD4 = 0;

                pSoundEvent->m3C = 0;
                pSoundEvent->m3E = 0;
                pSoundEvent->mCE = 0;
                pSoundEvent->m10 = param_1->m8_pSeq->m_rawData.begin() + READ_LE_U16(pDrumData);
                pSoundEvent->m14 = param_1->m8_pSeq->m_rawData.begin() + READ_LE_U16(pDrumData);

                // Todo: cross-check that
                for (int i = 0; i < 3; i++) {
                    pSoundEvent->mD8[i].m1E = 0;
                }

                pSoundEvent->m2C_pWds = pWdsFile;
                pSoundEvent->m25 = param_1->m8_pSeq->m16_sampleBank & 0xFF;
                if (pWdsFile != nullptr) {
                    setupAdsr(0, pSoundEvent);
                }
                pSoundEvent->m27 = i - 1;
                pSoundEvent->m30.m2 = 0;
                pSoundEvent->m30.m4 = 0x200;
                playSoundEffectSubSub1(&pSoundEvent->m30, pSoundEvent->m27);
            }
        }
        param_1->m48_activeVoicesBF = usedVoicesBitField;
    }
}

void initPercussions(sSoundInstance* param_1, sSeqFile* param_2) {
    param_1->mC_pPercussionData = &param_1->mPercussionData;
    int numPercussionData = param_2->m14 >> 8;
    auto pDrumKit = param_2->m_rawData.begin() + param_2->m20_drumKitOffset;
    for (int i = 0; i < numPercussionData; i++) {
        param_1->mC_pPercussionData->data[*pDrumKit].m0_adsr = READ_LE_U8(pDrumKit + 1);
        param_1->mC_pPercussionData->data[*pDrumKit].m1 = READ_LE_U8(pDrumKit + 2);
        param_1->mC_pPercussionData->data[*pDrumKit].m2 = READ_LE_U8(pDrumKit + 3);
        param_1->mC_pPercussionData->data[*pDrumKit].m3_pan = READ_LE_U8(pDrumKit + 4);
        pDrumKit += 5;
    }
}

sSoundInstance* createMusicInstance(sSeqFile* param_1) {
    // There was a dummy call here

    sSoundInstance* pNewSoundInstance = new sSoundInstance(param_1->m14 & 0xFF, param_1->m14 >> 8);
    pNewSoundInstance->m8_pSeq = param_1;
    if (param_1->m14 >> 8) {
        initPercussions(pNewSoundInstance, param_1);
    }
    initSoundInstance(pNewSoundInstance);
    initSoundInstanceTracks(pNewSoundInstance);
    pNewSoundInstance->m4C = 0;
    addActiveSoundInstance(pNewSoundInstance);
    return pNewSoundInstance;
}

void startMusicInstanceSub0Sub(sSoundInstance* param_1)
{
    for (int i = 0; i < param_1->m14_count; i++) {
        auto& voice = param_1->m94_events[i];
        playSoundEffectSubSub0(&voice.m30, voice.m27);
    }
}

void startMusicInstanceSub0(sSoundInstance* param_1) {
    if (param_1 == (sSoundInstance*)0x0) {
        setSoundError(5);
    }
    else {
        param_1->m10_flags = param_1->m10_flags & 0x7fff;
        startMusicInstanceSub0Sub(param_1);
    }
}

void applyFlagToAllVoices(ushort param_1, sSoundInstance* param_2) {
    for (int i = 0; i < param_2->m14_count; i++) {
        if (param_2->m94_events[i].m0) {
            param_2->m94_events[i].m2 |= param_1;
        }
    }
}

void playBattleMusicSub1(sSoundInstance* param_1) {
    DisableEvent(audioTickEvent);
    if ((musicStatusFlag & 0x1000) != 0) {
        assert(0);
    }
    assert(0);
    param_1->m10_flags = param_1->m10_flags & 0xfeff | 0x8000;
    EnableEvent(audioTickEvent);
}

void playBattleMusic(sSoundInstance* param_1, int param_2, int param_3) {
    param_1->m7A = (short)(param_2 << 8);
    if (param_3 == 0) {
        param_1->m70 = param_2 << 0x18;
        param_1->m78 = 0;
        applyFlagToAllVoices(0x100, param_1);
    }
    else {
        int iVar1 = param_2 * 0x10000 - (param_1->m70 >> 8);
        if (iVar1 == 0) {
            return;
        }
        param_1->m78 = (short)param_3;
        param_1->m74 = iVar1 / param_3 << 8;
    }
    if (((param_1->m10_flags & 0x100) != 0) && (param_2 != 0)) {
        playBattleMusicSub1(param_1);
    }
}

void startMusicInstance(sSoundInstance* param_1, int param_2, int param_3) {
    if (param_1 == nullptr) {
        setSoundError(5);
    }
    else {
        param_1->m10_flags = param_1->m10_flags & 0x7fff;
        if(true) // there was a dummy call here
        {
            if (param_1->m10_flags & 0x8000) {
                startMusicInstanceSub0(param_1);
            }
            DisableEvent(audioTickEvent);
            initSoundInstance(param_1);
            initSoundInstanceTracks(param_1);
            param_1->m70 = 0;
            playBattleMusic(param_1, param_2, param_3);
            param_1->m10_flags = param_1->m10_flags | 0x8000;
            EnableEvent(audioTickEvent);
        }
        else {
            setSoundError(10);
        }
    }
}