#include "noahLib.h"
#include "seqOpcodes.h"
#include "soundInstance.h"
#include "wds.h"

std::vector<u8>::iterator seqOP_14_setOctave(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66 = it[0] * 0xc;
    return it + 1;
}

std::vector<u8>::iterator seqOP_18_loopStart(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m72_callstackDepth++;
    auto& pCallstack = pChannel->m90_callstack[pChannel->m72_callstackDepth];
    pCallstack.m0 = *it - 1;
    pCallstack.m4 = it + 1;
    pCallstack.m2 = pChannel->m66;
    return it + 1;
}


std::vector<u8>::iterator seqOP_29(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m62 = it[0];
    return it + 1;
}

std::vector<u8>::iterator seqOP_44(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x40;
    pChannel->m59 = *it;
    return it + 1;
}

std::vector<u8>::iterator seqOP_60_setVolume(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m78 = (int)it[0] << 0x18;
    pChannel->m4 &= 0xfef7;
    pChannel->m2 |= 0x100;
    return it + 1;
}

std::vector<u8>::iterator seqOP_7C_changeWds(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m25 = it[0];
    sWdsFile* pWds = findInWdsLinkedList(it[1]);
    if (pWds == nullptr) {
        pWds = pLoadedWdsLinkedList;
    }
    pChannel->m2C_pWds = pWds;
    setupAdsr(it[1], pChannel);
    return it + 2;
}


const std::array<seqFunction, 0x80> seqOpcodes = { {
    //0x00
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x10
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_14_setOctave,
    nullptr,
    nullptr,
    nullptr,
    seqOP_18_loopStart,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x20
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_29,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x30
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x40
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_44,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x50
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x60
    seqOP_60_setVolume,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x70
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_7C_changeWds,
    nullptr,
    nullptr,
    nullptr,
} };

const std::array<s8, 0x80> opcodeSizes = { {
    0x02,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x00,
    0x02,
    0x04,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x02,
    0x01,
    0x01,
    0x03,
    0x02,
    0x01,
    0x01,
    0x00,
    0x04,
    0x04,
    0x04,
    0x00,
    0x02,
    0x02,
    0x03,
    0x00,
    0x02,
    0x02,
    0x02,
    0x03,
    0x00,
    0x02,
    0x02,
    0x00,
    0x02,
    0x02,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x02,
    0x02,
    0x01,
    0x01,
    0x04,
    0x00,
    0x01,
    0x01,
    0x04,
    0x01,
    0x01,
    0x00,
    0x01,
    0x04,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x03,
    0x02,
    0x02,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x03,
    0x03,
    0x01,
    0x02,
    0x02,
    0x04,
    0x04,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x02,
    0x02,
    0x03,
    0x02,
    0x04,
    0x04,
    0x01,
    0x01,
    0x02,
    0x02,
    0x03,
    0x02,
    0x04,
    0x04,
    0x01,
    0x01,
    0x04,
    0x04,
    0x03,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x04,
    0x03,
    0x00,
    0x00,
    0x03,
    0x02,
    0x02,
    0x01,
} };