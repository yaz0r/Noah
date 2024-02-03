#include "noahLib.h"
#include "seq.h"
#include "soundSystem.h"
#include "spu.h"

sSeqFile* pLoadedSequencesLinkedList = nullptr;

int checkHeaderSub0(sSeqFile* param_1) {
    int checksum = 0;
    int size = param_1->m8 + 3U >> 2;

    for (int i = 0; i < size; i++) {
        checksum += READ_LE_S32(param_1->m_rawData.begin() + i * 4);
    }

    return checksum;
}

int checkHeader(sSeqFile* param_1, u32 magic, int param_3) {
    if (param_1->m0_magic != magic)
        return 1;

    if (checkHeaderSub0(param_1) == 0)
        return (param_1->mC != param_3) << 2;

    return 2;
}

void loadSequence(sSeqFile& param_1) {
    if (((musicStatusFlag & 0x80) == 0) && (pLoadedSequencesLinkedList != 0)) {
        assert(0);
    }

    int error = checkHeader(&param_1, 0x73646573, 0x101);
    if (error) {
        setSoundError(error);
        return;
    }

    DisableEvent(spuEvent);
    sSeqFile** ppSeq = &pLoadedSequencesLinkedList;
    while (*ppSeq) {
        ppSeq = &(*ppSeq)->m1C_pNext;
    }
    *ppSeq = &param_1;
    param_1.m1C_pNext = nullptr;
    EnableEvent(spuEvent);
}