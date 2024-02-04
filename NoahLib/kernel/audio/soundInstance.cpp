#include "noahLib.h"
#include "soundInstance.h"
#include "spu.h"
#include "seq.h"
#include "wds.h"

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
    pSoundInstance->m48 = 0;
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
    sSoundInstance* pSoundInstance = new sSoundInstance(numMaxSoundEffectInstances); // This was allocated a bit differently, and from the sound arena
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
                    pSoundInstance->m48 = ~(1 << (pSoundEvent->m6 & 0x1f)) & pSoundInstance->m48;
                    pSoundEvent->m0 = 0;
                    //playSoundEffectSubSub0(&pSoundEvent->m30, pSoundEvent->m27);
                    assert(0);
                }
                else {
                    pSoundInstance->m48 = 1 << (pSoundEvent->m6 & 0x1f) | pSoundInstance->m48;
                    pSoundEvent->m0 = 0x409;
                    if ((pCurrentSeq->m10 & 1U) != 0) {
                        pSoundEvent->m0 = 0x40b;
                    }
                    pSoundEvent->m2 = 0x170;
                    pSoundEvent->m4 = 0;
                    pSoundEvent->m66 = 0x3C;
                    pSoundEvent->m62 = 0xF;
                    pSoundEvent->m72_callstackDepth = -1;
                    pSoundEvent->m18.reset();
                    pSoundEvent->m1C = 0;
                    pSoundEvent->m20 = 0;
                    pSoundEvent->m22 = 0;

                    pSoundEvent->m5C_deltaTime = 0;
                    pSoundEvent->m60 = 0;
                    pSoundEvent->m6E = 0;
                    pSoundEvent->m64 = 0;
                    pSoundEvent->m76 = uVar8;
                    pSoundEvent->m78 = 0x7f000000;

                    pSoundEvent->m70 = 0;
                    pSoundEvent->mD0 = 0;
                    pSoundEvent->mD2 = 0;
                    pSoundEvent->mD4 = 0;
                    pSoundEvent->m3C = 0;
                    pSoundEvent->m3E = 0;
                    pSoundEvent->mCE = 0;
                    pSoundEvent->m74 = param_4;
                    pSoundEvent->m10 = pCurrentSeq->m_rawData.begin() + READ_LE_U16(pDrumData);
                    pSoundEvent->m14 = pCurrentSeq->m_rawData.begin() + READ_LE_U16(pDrumData);

                    // Todo: cross-cheak that
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