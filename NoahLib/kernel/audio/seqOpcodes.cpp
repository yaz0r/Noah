#include "noahLib.h"
#include "seqOpcodes.h"
#include "soundInstance.h"
#include "soundSystem.h"
#include "wds.h"

std::vector<u8>::iterator seqOP_0_restAndNoteOff(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m5C_deltaTime = it[0];
    pChannel->m2 |= 2;
    pChannel->m0 |= 0x400;
    return it + 1;
}

std::vector<u8>::iterator seqOP_1_hold(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m0 = pChannel->m0 | 0x100;
    pChannel->m5C_deltaTime = it[0];
    return it + 1;
}

std::vector<u8>::iterator seqOP_10_end(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (!pChannel->m18_infinitLoopStart.has_value()) {
        pChannel->m2 &= 0xFFFC;
        playSoundEffectSubSub0(&pChannel->m30, pChannel->m27);
        pChannel->m0 = 0;
    }
    else {
        pChannel->m20++;
        pChannel->m66_octave = pChannel->m23_infiniteLoopOctave;
        it = pChannel->m18_infinitLoopStart.value();
    }

    return it;
}

std::vector<u8>::iterator seqOP_11_infiniteLoop(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m18_infinitLoopStart = it;
    pChannel->m23_infiniteLoopOctave = pChannel->m66_octave & 0xFF;

    return it;
}

std::vector<u8>::iterator seqOP_14_setOctave(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66_octave = it[0] * 0xc;
    return it + 1;
}

std::vector<u8>::iterator seqOP_15_incOctave(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66_octave += 0xc;
    return it;
}

std::vector<u8>::iterator seqOP_17_timeSignature(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int numerator = it[0];
    int denominator = it[1];
    pInstance->m3A = (short)(0xc0 / denominator);
    pInstance->m3C = (ushort)denominator;
    pInstance->m38 = (ushort)numerator;
    pInstance->m3E = (ushort)numerator;
    pInstance->m36 = pInstance->m3A;
    return it + 2;
}

std::vector<u8>::iterator seqOP_18_loopStart(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m72_callstackDepth++;
    auto& pCallstack = pChannel->m9C_callstack[pChannel->m72_callstackDepth];
    pCallstack.m0_loopCount = *it - 1;
    pCallstack.m4_loopStartIt = it + 1;
    pCallstack.m2 = pChannel->m66_octave;
    return it + 1;
}

std::vector<u8>::iterator seqOP_19_loopEnd(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    auto& pCallstack = pChannel->m9C_callstack[pChannel->m72_callstackDepth];
    if (--pCallstack.m0_loopCount == 0xFF) {
        pChannel->m72_callstackDepth--;
    }
    else {
        pCallstack.m8_loopEndIt = it;
        it = pCallstack.m4_loopStartIt;
        pCallstack.m3 = pChannel->m66_octave;
        pChannel->m66_octave = pCallstack.m2;
    }
    return it;
}

std::vector<u8>::iterator seqOP_20_setTempo(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pInstance->m58 = (int)it[0] << 0x10;
    pInstance->m54 = (int)it[0] * (int)(pInstance->m64 >> 8);
    return it + 1;
}


std::vector<u8>::iterator seqOP_29(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m62 = it[0];
    return it + 1;
}

std::vector<u8>::iterator seqOP_2C_setAdsr(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    setupAdsr(it[0], pChannel);
    return it + 1;
}

std::vector<u8>::iterator seqOP_2D(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (it[0] == 0) {
        pChannel->m60 = 0;
    }
    else {
        pChannel->m60 += it[0];
    }
    return it + 1;
}

std::vector<u8>::iterator seqOP_2E_percussionOn(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (pInstance->mC_pPercussionData) {
        pChannel->m0 |= 0x10;
    }
    return it;
}

std::vector<u8>::iterator seqOP_3A(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (((pInstance->m10_flags & 6) == 0) ||
        (((musicStatusFlag & 0x2000) != 0 && ((pChannel->m0 & 2) == 0)))) {
        pChannel->m30.m6 |= 0x4000;
        pChannel->m30.m2 |= 0x40;
    }
    return it;
}

std::vector<u8>::iterator seqOP_44(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x40;
    pChannel->m59 = *it;
    return it + 1;
}

std::vector<u8>::iterator seqOP_60_setVolume(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m78_volume = (int)it[0] << 0x18;
    pChannel->m4 &= 0xfef7;
    pChannel->m2 |= 0x100;
    return it + 1;
}

std::vector<u8>::iterator seqOP_62_volumeSlide(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int duration = it[0];
    int targetVolume = (int)it[1] * 0x1000000 - pChannel->m78_volume;
    if (duration && targetVolume) {
        pChannel->m96_volumeSlideDuration = duration;
        pChannel->m4 = (pChannel->m4 & 0xfeff) | 8;
        pChannel->m88_volumeSlideDelta = targetVolume / (int)(uint)duration;
    }
    return it + 2;
}

std::vector<u8>::iterator seqOP_68_pan(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m74_pan = (ushort)it[0] << 8;
    pChannel->m2 = pChannel->m2 | 0x100;
    return it + 1;
}

std::vector<u8>::iterator seqOP_7C_changeWdsAndADSR(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m25 = it[0];
    sWdsFile* pWds = findInWdsLinkedList(it[1]);
    if (pWds == nullptr) {
        pWds = pLoadedWdsLinkedList;
    }
    pChannel->m2C_pWds = pWds;
    setupAdsr(it[1], pChannel);
    return it + 2;
}

std::vector<u8>::iterator seqOP_7E_changeBank(std::vector<u8>::iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m25 = it[0];
    sWdsFile* pWds = findInWdsLinkedList(it[1]);
    if (pWds == nullptr) {
        pWds = pLoadedWdsLinkedList;
    }
    pChannel->m2C_pWds = pWds;
    return it + 1;
}


const std::array<seqFunction, 0x80> seqOpcodes = { {
    //0x00
    seqOP_0_restAndNoteOff,
    seqOP_1_hold,
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
    seqOP_10_end,
    seqOP_11_infiniteLoop,
    nullptr,
    nullptr,
    seqOP_14_setOctave,
    seqOP_15_incOctave,
    nullptr,
    seqOP_17_timeSignature,
    seqOP_18_loopStart,
    seqOP_19_loopEnd,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x20
    seqOP_20_setTempo,
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
    seqOP_2C_setAdsr,
    seqOP_2D,
    seqOP_2E_percussionOn,
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
    seqOP_3A,
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
    seqOP_62_volumeSlide,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_68_pan,
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
    seqOP_7C_changeWdsAndADSR,
    nullptr,
    seqOP_7E_changeBank,
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