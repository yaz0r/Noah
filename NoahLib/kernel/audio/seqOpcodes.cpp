#include "noahLib.h"
#include "seqOpcodes.h"
#include "soundInstance.h"
#include "soundSystem.h"
#include "wds.h"
#include "seq.h"

std::vector<u8>::const_iterator seqOP_0_restAndNoteOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m5C_deltaTime = it[0];
    pChannel->m2 |= 2;
    pChannel->m0 |= 0x400;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_1_hold(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m0 = pChannel->m0 | 0x100;
    pChannel->m5C_deltaTime = it[0];
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_10_end(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (!pChannel->m18_infiniteLoopStart.has_value()) {
        pChannel->m2 &= 0xFFFC;
        releaseSpuVoice(&pChannel->m30, pChannel->m27);
        pChannel->m0 = 0;
    }
    else {
        pChannel->m20++;
        pChannel->m66_octave = pChannel->m23_infiniteLoopOctave;
        it = pChannel->m18_infiniteLoopStart.value();
    }

    return it;
}

std::vector<u8>::const_iterator seqOP_11_infiniteLoop(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m18_infiniteLoopStart = it;
    pChannel->m23_infiniteLoopOctave = pChannel->m66_octave;

    return it;
}

std::vector<u8>::const_iterator seqOP_14_setOctave(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66_octave = it[0] * 0xc;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_15_incOctave(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66_octave += 0xc;
    return it;
}

std::vector<u8>::const_iterator seqOP_16_decOctave(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m66_octave -= 0xc;
    return it;
}

std::vector<u8>::const_iterator seqOP_17_timeSignature(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int numerator = it[0];
    int denominator = it[1];
    pInstance->m3A = (short)(0xc0 / denominator);
    pInstance->m3C = (ushort)denominator;
    pInstance->m38 = (ushort)numerator;
    pInstance->m3E = (ushort)numerator;
    pInstance->m36 = pInstance->m3A;
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_18_loopStart(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m72_callstackDepth++;
    auto& pCallstack = pChannel->m9C_callstack[pChannel->m72_callstackDepth];
    pCallstack.m0_loopCount = *it - 1;
    pCallstack.m4_loopStartIt = it + 1;
    pCallstack.m2 = pChannel->m66_octave;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_19_loopEnd(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
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

std::vector<u8>::const_iterator seqOP_1A_loopBreak(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    auto& pCallstack = pChannel->m9C_callstack[pChannel->m72_callstackDepth];
    if (pCallstack.m0_loopCount == 0) {
        it = pCallstack.m8_loopEndIt;
        pChannel->m66_octave = pCallstack.m3;
        pChannel->m72_callstackDepth--;
    }
    return it;
}

std::vector<u8>::const_iterator seqOP_1C_playSoundEffect(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // FUN_80039f18(READ_LE_U16(it), 0x7f, 0x40)
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_1D_stopSoundEffect(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // FUN_8003a14c(READ_LE_U16(it)) - stops sound effect by ID
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_1E_jumpToTrack(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // jump to another sequence's track using drumKitOffset table
    return it + 4;
}

std::vector<u8>::const_iterator seqOP_20_setTempo(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pInstance->m58 = (int)it[0] << 0x10;
    pInstance->m54 = (int)it[0] * (pInstance->m64.m0_currentValue.getIntegerPart());
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_22_tempoSlide(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int duration = it[0];
    int tempoOffset = it[1];

    pInstance->m62 = tempoOffset;
    int targetTempo = tempoOffset * 0x10000 - pInstance->m58;

    if ((duration != 0) && (targetTempo != 0)) {
        pInstance->m60 = (ushort)duration;
        pInstance->m5C = targetTempo / (int)(uint)duration;
    }
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_26_setMasterVolume(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pInstance->m70.m0_currentValue = (u32)it[0] << 0x18;
    applyFlagToAllVoices(0x100, pInstance);
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_29_setTranspose(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m62 = it[0];
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_2A_changeVoiceChannel(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    u8 newChannel = it[0];
    if (newChannel < 0x19) {
        releaseSpuVoice(&pChannel->m30, pChannel->m27);
        pChannel->m27 = newChannel;
        claimSpuVoice(&pChannel->m30, newChannel);
    }
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_2C_setAdsr(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    setupAdsr(it[0], pChannel);
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_2D_adjustNoteLength(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (it[0] == 0) {
        pChannel->m60 = 0;
    }
    else {
        pChannel->m60 += it[0];
    }
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_2E_percussionOn(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (pInstance->mC_pPercussionData) {
        pChannel->m0 |= 0x10;
    }
    return it;
}

std::vector<u8>::const_iterator seqOP_2F_percussionOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m0 &= ~0x10;
    return it;
}

std::vector<u8>::const_iterator seqOP_30_legatoOn(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m0 |= 0x800;
    return it;
}

std::vector<u8>::const_iterator seqOP_31_legatoOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m0 &= ~0x800;
    return it;
}

std::vector<u8>::const_iterator seqOP_32_noiseOn(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if ((pChannel->m27 & 1) != 0) {
        (pChannel->m30).m6 = (pChannel->m30).m6 | 0x1000;
        (pChannel->m30).m2 = (pChannel->m30).m2 | 0x10;
    }
    return it;
}

std::vector<u8>::const_iterator seqOP_34_setNoiseClock(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pInstance->m1E = it[0];
    MissingCode(); // SpuSetNoiseClock(pInstance->m1E)
    pChannel->m30.m6 |= 0x2000;
    pChannel->m30.m2 |= 0x20;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_35_adjustNoiseClock(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pInstance->m1E = ((u16)it[0] + pInstance->m1E) & 0x3f;
    MissingCode(); // SpuSetNoiseClock(pInstance->m1E)
    pChannel->m30.m6 |= 0x2000;
    pChannel->m30.m2 |= 0x20;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_38_setupReverb(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    u8 mode = it[0];
    s8 delay = (s8)it[1];
    s8 feedback = (s8)it[2];
    pInstance->m44 = (u16)mode << 8;
    pInstance->m42 = delay;
    pInstance->m43 = feedback;
    setupReverb(-1, (s16)((uint)mode * 0x1000000 >> 0x10), (int)delay, (int)feedback);
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_3A_reverbOn(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    if (((pInstance->m10_flags & 6) == 0) ||
        (((musicStatusFlag & 0x2000) != 0 && ((pChannel->m0 & 2) == 0)))) {
        pChannel->m30.m6 |= 0x4000;
        pChannel->m30.m2 |= 0x40;
    }
    return it;
}

std::vector<u8>::const_iterator seqOP_40_ADSRReset(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    setupAdsr(pChannel->m26_ADSRIndex, pChannel);
    return it;
}

std::vector<u8>::const_iterator seqOP_49_setSustainMode(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x40;
    pChannel->m30.m25_ADSR_SustainMode = *it;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_4A_setReleaseMode(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x80;
    pChannel->m30.m26_ADSR_ReleaseMode = *it;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_42_setAttack(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x10;
    pChannel->m30.m27_ADSR_Attack = *it;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_44_setSustain(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x40;
    pChannel->m30.m29_ADSR_Sustain = *it;
    return it + 1;
}
std::vector<u8>::const_iterator seqOP_45_setRelease(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m30.m6 |= 0x80;
    pChannel->m28_ADSR_ReleaseBackup = *it;
    pChannel->m30.m2A_ADSR_Release = *it;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_50_setPitchBend(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m6E = ((int)it[0] << 0x18) >> 0x13;
    pChannel->m2 |= 0x200;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_51_adjustPitchBend(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m6E += ((int)it[0] << 0x18) >> 0x13;
    pChannel->m2 |= 0x200;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_54_pitchSlide(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    u8 duration = it[0];
    s8 target = (s8)it[1];
    int targetPitch = (int)target << 0x18;
    if (duration == 0 || targetPitch == 0) {
        pChannel->m4 &= ~1;
    }
    else {
        pChannel->m9A = duration;
        pChannel->m4 |= 1;
        pChannel->m84 = targetPitch / (int)(uint)duration;
    }
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_57_pitchLFOSpeed(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    uint speed = (it[0] + 1) & 0xff;
    if (speed != 0) {
        u16 val = (u16)(0x400 / (speed << 2));
        pChannel->mD8[0].m1A = val;
        pChannel->mD8[0].m18 = val;
    }
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_58_pitchLFO(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // complex pitch LFO setup using FUN_8003e290/FUN_8003e3e0
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_59_pitchLFOWaveform(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // pitch LFO with waveform table selection
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_5B_pitchLFOOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->mCE &= ~1;
    pChannel->mD8[0].m1E &= ~1;
    return it;
}

std::vector<u8>::const_iterator seqOP_60_setVolume(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m78_volume = (int)it[0] << 0x18;
    pChannel->m4 &= 0xfef7;
    pChannel->m2 |= 0x100;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_61_adjustVolume(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m78_volume = (s8)it[0] * 0x1000000 + pChannel->m78_volume & 0x7fffffff;
    pChannel->m2 |= 0x100;
    pChannel->m4 &= 0xfef7;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_62_volumeSlide(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int duration = it[0];
    int targetVolume = (s8)it[1] * 0x1000000 - pChannel->m78_volume;
    if (duration && targetVolume) {
        pChannel->m96_volumeSlideDuration = duration;
        pChannel->m4 = (pChannel->m4 & 0xfeff) | 8;
        pChannel->m88_volumeSlideDelta = targetVolume / (int)(uint)duration;
    }
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_63_volumeLFOSpeed(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    uint speed = (it[0] + 1) & 0xff;
    if (speed != 0) {
        u16 val = (u16)(0x400 / (speed << 2));
        pChannel->mD8[1].m1A = val;
        pChannel->mD8[1].m18 = val;
    }
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_64_volumeLFO(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // complex volume LFO setup using FUN_8003e290/FUN_8003e3e0
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_65_volumeLFOWaveform(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // volume LFO with waveform table selection
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_67_volumeLFOOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->mCE &= 0xFFFD;
    pChannel->mD8[1].m1E &= 0xFFFE;
    return it;
}

std::vector<u8>::const_iterator seqOP_68_setPan(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m74_pan = (ushort)it[0] << 8;
    pChannel->m2 |= 0x100;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_6A_panSlide(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int duration = it[0];
    int targetPan = (s8)it[1] - ((((int)pChannel->m74_pan) << 0x10) >> 0x18);
    if (duration && targetPan) {
        pChannel->m92_panTarget = targetPan * 0x100;
        pChannel->m98_panDuration = duration;
        pChannel->m4 |= 0x10;
        pChannel->m90_panDelta = pChannel->m92_panTarget / duration;
    }
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_6C_panLFO(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // complex pan LFO setup using FUN_8003e290/FUN_8003e3e0
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_6D_panLFOWaveform(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // pan LFO with waveform table selection
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_6F_panLFOOff(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->mCE &= 0xFFFB;
    pChannel->mD8[2].m1E &= 0xFFFE;
    return it;
}

std::vector<u8>::const_iterator seqOP_71_LFOUpdate(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // update active LFO parameters using FUN_8003e290
    return it + 3;
}

std::vector<u8>::const_iterator seqOP_76_LFORestart(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // restart LFO on specified channel using FUN_8003e3e0
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_78_volumeSlideMulti(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    int bVar1 = it[1];
    int iVar2 = (uint)it[2] * 0x1000000 + (uint)it[0] * -0x1000000;

    if ((iVar2 == 0) || (bVar1 == 0)) {
        pChannel->m4 = pChannel->m4 & 0xfeff;
    }
    else {
        pChannel->m82 = (short)((uint)it[0] * 0x1000000 >> 0x10);
        pChannel->m80 = (ushort)bVar1;
        pChannel->m4 = pChannel->m4 & 0xfff7 | 0x100;
        pChannel->m7C = iVar2 / (int)(uint)bVar1;
    }

    return it + 3;
}

std::vector<u8>::const_iterator seqOP_7C_changeWdsAndADSR(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m25 = it[0];
    sWdsFile* pWds = findInWdsLinkedList(it[0]);
    if (pWds == nullptr) {
        pWds = pLoadedWdsLinkedList;
    }
    pChannel->m2C_pWds = pWds;
    setupAdsr(it[1], pChannel);
    return it + 2;
}

std::vector<u8>::const_iterator seqOP_7E_changeBank(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    pChannel->m25 = it[0];
    sWdsFile* pWds = findInWdsLinkedList(it[0]);
    if (pWds == nullptr) {
        pWds = pLoadedWdsLinkedList;
    }
    pChannel->m2C_pWds = pWds;
    return it + 1;
}

std::vector<u8>::const_iterator seqOP_7F_waitRelease(std::vector<u8>::const_iterator it, sSoundInstance* pInstance, sSoundInstanceEvent* pChannel) {
    MissingCode(); // SpuGetVoiceEnvelopeAttr to check if envelope finished
    // When envelope level reaches 0:
    //   pChannel->m2 &= 0xFFFC;
    //   releaseSpuVoice(&pChannel->m30, pChannel->m27);
    //   pChannel->m0 = 0;
    return it;
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
    seqOP_16_decOctave,
    seqOP_17_timeSignature,
    seqOP_18_loopStart,
    seqOP_19_loopEnd,
    seqOP_1A_loopBreak,
    nullptr,
    seqOP_1C_playSoundEffect,
    seqOP_1D_stopSoundEffect,
    seqOP_1E_jumpToTrack,
    nullptr,

    //0x20
    seqOP_20_setTempo,
    nullptr,
    seqOP_22_tempoSlide,
    nullptr,
    nullptr,
    nullptr,
    seqOP_26_setMasterVolume,
    nullptr,
    nullptr,
    seqOP_29_setTranspose,
    seqOP_2A_changeVoiceChannel,
    nullptr,
    seqOP_2C_setAdsr,
    seqOP_2D_adjustNoteLength,
    seqOP_2E_percussionOn,
    seqOP_2F_percussionOff,

    //0x30
    seqOP_30_legatoOn,
    seqOP_31_legatoOff,
    seqOP_32_noiseOn,
    nullptr,
    seqOP_34_setNoiseClock,
    seqOP_35_adjustNoiseClock,
    nullptr,
    nullptr,
    seqOP_38_setupReverb,
    nullptr,
    seqOP_3A_reverbOn,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x40
    seqOP_40_ADSRReset,
    nullptr,
    seqOP_42_setAttack,
    nullptr,
    seqOP_44_setSustain,
    seqOP_45_setRelease,
    nullptr,
    nullptr,
    nullptr,
    seqOP_49_setSustainMode,
    seqOP_4A_setReleaseMode,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x50
    seqOP_50_setPitchBend,
    seqOP_51_adjustPitchBend,
    nullptr,
    nullptr,
    seqOP_54_pitchSlide,
    nullptr,
    nullptr,
    seqOP_57_pitchLFOSpeed,
    seqOP_58_pitchLFO,
    seqOP_59_pitchLFOWaveform,
    nullptr,
    seqOP_5B_pitchLFOOff,
    nullptr,
    nullptr,
    nullptr,
    nullptr,

    //0x60
    seqOP_60_setVolume,
    seqOP_61_adjustVolume,
    seqOP_62_volumeSlide,
    seqOP_63_volumeLFOSpeed,
    seqOP_64_volumeLFO,
    seqOP_65_volumeLFOWaveform,
    nullptr,
    seqOP_67_volumeLFOOff,
    seqOP_68_setPan,
    nullptr,
    seqOP_6A_panSlide,
    nullptr,
    seqOP_6C_panLFO,
    seqOP_6D_panLFOWaveform,
    nullptr,
    seqOP_6F_panLFOOff,

    //0x70
    nullptr,
    seqOP_71_LFOUpdate,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    seqOP_76_LFORestart,
    nullptr,
    seqOP_78_volumeSlideMulti,
    nullptr,
    nullptr,
    nullptr,
    seqOP_7C_changeWdsAndADSR,
    nullptr,
    seqOP_7E_changeBank,
    seqOP_7F_waitRelease,
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