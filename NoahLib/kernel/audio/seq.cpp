#include "noahLib.h"
#include "seq.h"
#include "soundSystem.h"
#include "spu.h"
#include "wds.h"
#include "soundInstance.h"

sSeqFile* pLoadedSequencesLinkedList = nullptr;

int checkSeqChecksum(sSeqFile* param_1) {
    int checksum = 0;
    int size = (param_1->m8 + 3U) >> 2;

    for (int i = 0; i < size; i++) {
        checksum += READ_LE_S32(param_1->getRawData().begin() + i * 4);
    }

    return checksum;
}

int checkHeader(sSeqFile* param_1, u32 magic, int param_3) {
    if (param_1->m0_magic != magic)
        return 1;

    if (checkSeqChecksum(param_1) == 0)
        return (param_1->mC_checksum != param_3) << 2;

    return 2;
}

void loadSequence(sSeqFile* param_1) {
    if (((musicStatusFlag & 0x80) == 0) && (pLoadedSequencesLinkedList != 0)) {
        auto psVar2 = pLoadedSequencesLinkedList;
        do {
            if (param_1->m14 == psVar2->m14) {
                setSoundError(0x15);
                return;
            }
            psVar2 = psVar2->m1C_pNext;
        } while (psVar2 != (sSeqFile*)0x0);
    }

    int error = checkHeader(param_1, 0x73646573, 0x101);
    if (error) {
        setSoundError(error);
        return;
    }

    DisableEvent(audioTickEvent);
    sSeqFile** ppSeq = &pLoadedSequencesLinkedList;
    while (*ppSeq) {
        ppSeq = &(*ppSeq)->m1C_pNext;
    }
    *ppSeq = param_1;
    param_1->m1C_pNext = nullptr;
    EnableEvent(audioTickEvent);
}

void stopSequence(sSeqFile* param_1) {
    short sVar1;
    sSoundInstance* psVar2;
    sSoundInstanceEvent* psVar4;
    int iVar5;

    psVar2 = pSoundEffectsInstances;
    sVar1 = param_1->m14;
    psVar4 = pSoundEffectsInstances->m94_events.data();
    iVar5 = numMaxSoundEffectInstances;
    do {
        iVar5 = iVar5 + -1;
        if (((psVar4->m0 & 1) != 0) && ((psVar4->m8 >> 16) == sVar1)) { // TODO: unsure about psVar4->m8 >> 16
            psVar4->m0 = 0;
            psVar2->m48_activeVoicesBF = ~(1 << (psVar4->m6_voiceIndex & 0x1f)) & psVar2->m48_activeVoicesBF;
            playSoundEffectSubSub0(&psVar4->m30, psVar4->m27);
        }
        psVar4 = psVar4 + 1;
    } while (iVar5 != 0);
    return;
}
void unloadSequence(sSeqFile* param_1) {
    sSeqFile* pCurrent = pLoadedSequencesLinkedList;
    sSeqFile* pPrevious = nullptr;
    if (pLoadedSequencesLinkedList) {

        do {
            sSeqFile* psVar1 = pCurrent;
            pCurrent = psVar1;
            if (psVar1 == param_1) break;
            pCurrent = psVar1->m1C_pNext;
            pPrevious = psVar1;
        } while (pCurrent != nullptr);

        if (pCurrent) {
            stopSequence(param_1);
            DisableEvent(audioTickEvent);
            if (pPrevious == (sSeqFile*)0x0) {
                pLoadedSequencesLinkedList = param_1->m1C_pNext;
            }
            else {
                pPrevious->m1C_pNext = param_1->m1C_pNext;
            }
            param_1->m1C_pNext = (sSeqFile*)0x0;
            uint uVar3 = checkHeader(param_1, 0x73646573, 0x101);
            if ((uVar3 & 0xffff) != 0) {
                setSoundError(0xb);
                return;
            }
            EnableEvent(audioTickEvent);
            return;
        }
    }
    setSoundError(0x10);
    return;
}

void playSoundEffect(u32 param_1)
{
    if ((musicStatusFlag & 0x800) != 0) {
        playSoundEffectVar0 = 2;
        playSoundEffectSub((int)(short)((short)numMaxSoundEffectInstances - 2U | 0x8000), param_1, 0x6000, 0x4000);
    }
    return;
}