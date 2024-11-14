#include "noahLib.h"
#include "soundSystem.h"
#include "wds.h"
#include "spu.h"
#include "seq.h"
#include "soundInstance.h"
#include "kernel/criticalSection.h"
#include "kernel/events.h"
#include "seqOpcodes.h"
#include "spuEmu/spu.h"
#include "pitchTable.h"

static const u16 SPU_REG_KEYON_low = 0x188 / 2;
static const u16 SPU_REG_KEYON_high = 0x18A / 2;
static const u16 SPU_REG_KEYOFF_low = 0x18C / 2;
static const u16 SPU_REG_KEYOFF_high = 0x18E / 2;
static const u16 SPU_REG_CHON_low = 0x19c / 2;
static const u16 SPU_REG_CHON_high = 0x19E / 2;

u16 musicStatusFlag = 0;
std::array<sSoundInstanceEvent30*, 0x18> playSoundEffectSubSub1Var0;

std::mutex spuStatusMutex;
static const int numRegs = 512;
std::array<u16, numRegs> SPURegisterStatus;

void setSoundError(u16 errorCode) {
    assert(0);
}

void sendAdsrToSpu() {
    u16 postUpdateFlag = 0;
    u32 pitchModulationFlag = 0;
    u32 noiseGenerationFlag = 0;
    u32 reverbFlag = 0;
    for (int i = 0; i < 24; i++) {
        sSoundInstanceEvent30* voiceState = playSoundEffectSubSub1Var0[i];
        if(voiceState == nullptr)
            continue;
        u16 dirtyFlag = voiceState->m6;
        if (voiceState) {
            if (dirtyFlag & 1) {
                emulatedSpuDevice.write(0 + i * 8, voiceState->m8_volumeLeft);
                emulatedSpuDevice.write(1 + i * 8, voiceState->mA_volumeRight);
            }
            if (dirtyFlag & 4) {
                emulatedSpuDevice.write(2 + i * 8, voiceState->m14_ADPCM_SampleRate);
            }
            if (dirtyFlag & 8) {
                emulatedSpuDevice.write(3 + i * 8, voiceState->m1C_ADPCM_StartAddress >> 3);
                emulatedSpuDevice.write(7 + i * 8, voiceState->m20_ADPCM_RepeatAddress >> 3);
            }
            if (dirtyFlag & 0x10) { // update attack
                u16 currentValue = emulatedSpuDevice.read(4 + i * 8);
                currentValue &= 0xFF; // clear
                currentValue |= ((u16)voiceState->m27_ADSR_Attack) << 8;
                currentValue |= (voiceState->m24_ADSR_AttackMode >> 2) * -0x8000;
                emulatedSpuDevice.write(4 + i * 8, currentValue);
            }
            if (dirtyFlag & 0x20) { // update decay
                u16 currentValue = emulatedSpuDevice.read(4 + i * 8);
                currentValue &= 0xFF0F;
                currentValue |= ((u16)voiceState->m28_ADSR_Decay) << 4;
                emulatedSpuDevice.write(4 + i * 8, currentValue);
            }
            if (dirtyFlag & 0x40) { // update sustain
                u16 currentValue = emulatedSpuDevice.read(5 + i * 8);
                currentValue &= 0x3F;
                currentValue |= (u16)voiceState->m29_ADSR_Sustain * 0x40;
                currentValue |= (u16)(voiceState->m25_ADSR_SustainMode >> 1) * 0x4000;
                emulatedSpuDevice.write(5 + i * 8, currentValue);
            }
            if (dirtyFlag & 0x80) { // update release
                u16 currentValue = emulatedSpuDevice.read(5 + i * 8);
                currentValue &= 0xFFC0;
                currentValue |= voiceState->m2A_ADSR_Release;
                currentValue |= (u16)(voiceState->m26_ADSR_ReleaseMode >> 2) * 0x20;
                emulatedSpuDevice.write(5 + i * 8, currentValue);
            }
            if (dirtyFlag & 0x100) { // update sustain level
                u16 currentValue = emulatedSpuDevice.read(4 + i * 8);
                currentValue &= 0xFFF0;
                currentValue |= voiceState->m2B_ADSR_SustainLevel;
                emulatedSpuDevice.write(4 + i * 8, currentValue);
            }
            postUpdateFlag = postUpdateFlag | dirtyFlag & 0x7000;
            voiceState->m6 = 0;
        }
        dirtyFlag = voiceState->m2;
        pitchModulationFlag = pitchModulationFlag | (dirtyFlag >> 4 & 1) << (i & 0x1f);
        noiseGenerationFlag = noiseGenerationFlag | (dirtyFlag >> 5 & 1) << (i & 0x1f);
        reverbFlag = reverbFlag | (dirtyFlag >> 6 & 1) << (i & 0x1f);
    }

    if (postUpdateFlag != 0) {
        if (postUpdateFlag & 0x1000) {
            emulatedSpuDevice.write(200, pitchModulationFlag);
            emulatedSpuDevice.write(201, pitchModulationFlag >> 16);
        }
        if (postUpdateFlag & 0x2000) {
            emulatedSpuDevice.write(202, noiseGenerationFlag);
            emulatedSpuDevice.write(203, noiseGenerationFlag >> 16);
        }
        if (postUpdateFlag & 0x4000) {
            emulatedSpuDevice.write(204, reverbFlag);
            emulatedSpuDevice.write(205, reverbFlag >> 16);
        }
    }

    if (pendingKeyOn != 0) {
        emulatedSpuDevice.write(SPU_REG_KEYON_low, pendingKeyOn);
        emulatedSpuDevice.write(SPU_REG_KEYON_high, pendingKeyOn >> 16);
        pendingKeyOn = 0;
    }
}

void executeSequenceEvents2(sSoundInstance* param_1, std::vector<sSoundInstanceEvent>& param_2, short param_3) {
    if (param_1->m60) {
        if (param_1->m60 == 1) {
            param_1->m58 = (uint)(ushort)param_1->m62 << 0x10;
        }
        else {
            param_1->m58 += param_1->m5C;
        }
        param_1->m60--;
        param_1->m54 = (param_1->m58 >> 16) * (param_1->m64.m0.getIntegerPart());
    }

    for (int i = 0; i < param_3; i++) {
        sSoundInstanceEvent& instanceEvent = param_2[i];

        if (instanceEvent.m0) {
            if (instanceEvent.m5C_deltaTime) {
                if (instanceEvent.m4 & 8) { // volume slide
                    instanceEvent.m96_volumeSlideDuration--;
                    instanceEvent.m2 |= 0x100; // update volume
                    if (instanceEvent.m96_volumeSlideDuration == 0) {
                        instanceEvent.m4 &= ~8;
                    }
                    instanceEvent.m78_volume += instanceEvent.m88_volumeSlideDelta;
                }
                if (instanceEvent.m4 & 1) {
                    instanceEvent.m2 |= 0x200;
                    if (!(instanceEvent.m4 & 2)) {
                        instanceEvent.m92_panTarget--;
                        if (instanceEvent.m92_panTarget == 0) {
                            instanceEvent.m4 &= ~0x1;
                        }
                    }
                    instanceEvent.m68_finalNoteToPlay += instanceEvent.m84;
                }
                if (instanceEvent.m4 & 0x10) { // interpolate pan
                    instanceEvent.m98_panDuration--;
                    if (instanceEvent.m98_panDuration == 0) {
                        instanceEvent.m74_pan = instanceEvent.m92_panTarget;
                        instanceEvent.m4 &= ~0x10;
                    }
                    else {
                        instanceEvent.m74_pan += instanceEvent.m90_panDelta;
                    }
                    instanceEvent.m2 |= 0x100;
                }
                if (instanceEvent.m4 & 0x20) {
                    instanceEvent.m9A--;
                    if (instanceEvent.m9A == 0) {
                        instanceEvent.m76 = instanceEvent.m8E;
                        instanceEvent.m4 &= ~0x20;
                    }
                    else {
                        instanceEvent.m76 += instanceEvent.m8C;
                    }
                    instanceEvent.m2 |= 0x100;
                }

                if (--instanceEvent.m5C_deltaTime == 1) {
                    if (instanceEvent.m0 & 0x1000) {
                        instanceEvent.m30.m2A_ADSR_Release = 6;
                        instanceEvent.m30.m6 |= 0x80;
                    }
                }
                if (--instanceEvent.m5E == 0) {
                    instanceEvent.m2 |= 2;
                    instanceEvent.m0 |= 0x400;
                }
            }
        }
    }
}

static const std::array<u8, 19> deltaTimeTable = { { 0, 192, 144, 96, 72, 64, 48, 36, 32, 24, 18, 16, 12, 9, 8, 6, 4, 3, 2 } };

void processPercussions(sSoundInstance* param_1, sSoundInstanceEvent* param_2, u32 param_3) {
    auto& percussionData = param_1->mC_pPercussionData->data[param_3];
    setupAdsr(percussionData.m0_adsr, param_2);
    param_2->m68_finalNoteToPlay = ((uint)percussionData.m1 * 0x100 + (int)param_2->m6E + (int)param_2->m6C) * 0x10000;
    param_2->m2 |= 0x100;
    param_2->m74_pan = percussionData.m3_pan << 8;
}

void executeSequenceEvents(sSoundInstance* param_1, std::vector<sSoundInstanceEvent>& param_2, short param_3) {
    for (int i = 0; i < param_3; i++) {
        sSoundInstanceEvent& instanceEvent = param_2[i];
        if (instanceEvent.m0) {
            bool bNeedPostUpdate = false;
            if ((instanceEvent.m5C_deltaTime) == 0) {
                auto originalM0 = instanceEvent.m0;
                instanceEvent.m0 &= 0xF8FF;
                auto byteCodeIt = instanceEvent.m14;
                do {
                    u8 byteCode = *byteCodeIt;
                    if (byteCode < 0x80) {
                        if ((instanceEvent.m0 & 8) == 0) {
                            instanceEvent.m76 = (ushort)byteCode << 8;
                        }
                        instanceEvent.m2 |= 0x100;
                        int data_byte = byteCodeIt[1];
                        s8 relativeKey = instanceEvent.m66_octave + (data_byte / 19); // Note: this was done with a lookup table in original code
                        byteCodeIt += 2;
                        instanceEvent.m65 = relativeKey;

                        u8 deltaTime = deltaTimeTable[data_byte % 19]; // Note this was done with a repeating table in original code
                        if (deltaTime == 0) {
                            deltaTime = byteCodeIt[0];
                            byteCodeIt++;
                        }
                        instanceEvent.m5C_deltaTime = deltaTime;
                        instanceEvent.m30.m2A_ADSR_Release = instanceEvent.m28_ADSR_ReleaseBackup;
                        instanceEvent.m30.m6 |= 0x80;

                        if ((instanceEvent.m0 & 0x10) == 0) {
                            // no percussion
                            instanceEvent.m68_finalNoteToPlay = (((u32)instanceEvent.m6C) + ((u32)instanceEvent.m65 * 0x100) + ((u32)instanceEvent.m6E)) * 0x10000;
                        }
                        else {
                            // with percussion
                            processPercussions(param_1, &instanceEvent, relativeKey);
                        }
                        instanceEvent.m2 |= 0x200;
                        instanceEvent.m0 |= 0x180;
                        bNeedPostUpdate = true;
                        if (originalM0 & 0x400) {
                            instanceEvent.m2 |= 1;
                        }
                        if (instanceEvent.m0 & 0x8000) {
                            instanceEvent.m0 &= ~0x8000;
                            instanceEvent.m30.m6 = 0xFFFF;
                            instanceEvent.m2 |= 0x300;
                        }
                    }
                    else {
                        auto opcode = seqOpcodes[byteCode - 0x80];
                        if (opcode == nullptr) {
                            return;
                        }
                        byteCodeIt = opcode(byteCodeIt + 1, param_1, &instanceEvent);
                        if (instanceEvent.m0 == 0) { // disable the voice
                            param_1->m48_activeVoicesBF &= ~(1 << (instanceEvent.m6_voiceIndex & 0x1f));
                            break;
                        }
                    }
                } while ((instanceEvent.m0 & 0x500) == 0);
                instanceEvent.m14 = byteCodeIt;
                if (instanceEvent.m0 != 0) {
                    if ((instanceEvent.m0 & 0x800) != 0) {
                        instanceEvent.m0 |= 0x200;
                    }

                    u8 byteCode2 = *byteCodeIt;
                    bool bIsKeyOn = byteCode2 < 0x80;
                    if (!bIsKeyOn) {
                        do {
                            if (byteCode2 != 0x90) {
                                if (byteCode2 != 0x80) {
                                    if (byteCode2 == 0x81) {
                                        instanceEvent.m0 = instanceEvent.m0 | 0x200;
                                        break;
                                    }
                                    if (1 < byteCode2 - 0xb0) {
                                        // We need to do an optional here to account for the fact that we can have a negative index
                                        // as in that case, we should never access the stack
                                        std::optional<std::array<sSoundInstanceEventCallstack, 4>::iterator> pCallstackEntry;
                                        if (instanceEvent.m72_callstackDepth >= 0) {
                                            pCallstackEntry = instanceEvent.m9C_callstack.begin() + instanceEvent.m72_callstackDepth;
                                        }
                                        if (byteCode2 == 0x99) {
                                            
                                            if (pCallstackEntry.value()->m0_loopCount) {
                                                byteCodeIt = pCallstackEntry.value()->m4_loopStartIt;
                                                goto LAB_8003ca48;
                                            }
                                            pCallstackEntry.value()--;
                                        }
                                        if ((byteCode2 == 0x9a) && (pCallstackEntry.value()->m0_loopCount == 0)) {
                                            byteCodeIt = pCallstackEntry.value()->m8_loopEndIt;
                                            pCallstackEntry.value()--;
                                        }
                                        else {
                                            byteCodeIt += opcodeSizes[byteCode2 - 0x80];
                                        }
                                        goto LAB_8003ca48;
                                    }
                                }
                                instanceEvent.m0 &= 0xFDFF;
                                break;
                            }

                            bIsKeyOn = byteCode2 < 0x80;
                            if (!instanceEvent.m18_infiniteLoopStart.has_value()) {
                                goto LAB_8003ca60;
                            }
                            byteCodeIt = instanceEvent.m18_infiniteLoopStart.value();
                        LAB_8003ca48:
                            byteCode2 = *byteCodeIt;
                        } while (0x7F < byteCode2);
                        bIsKeyOn = byteCode2 < 0x80;
                    }

                LAB_8003ca60:
                    if (bIsKeyOn) {
                        instanceEvent.m0 |= 0x1000;
                    }
                    else {
                        instanceEvent.m0 &= ~0x1000;
                    }

                    int time = instanceEvent.m60 + instanceEvent.m5C_deltaTime;
                    if (time * 0x10000 < 1) {
                        time += instanceEvent.m5C_deltaTime;
                        instanceEvent.m60 += instanceEvent.m5C_deltaTime & 0xFF;
                    }
                    u32 minTime = 0x7FFF;
                    if ((instanceEvent.m0 & 0x600) == 0) {
                        if (instanceEvent.m62 == 0xF) {
                            minTime = time - 1;
                        }
                        else {
                            minTime = time;
                            if(instanceEvent.m62 == 0x10)
                                goto LAB_8003cb18;
                            minTime = ((int)time * instanceEvent.m62) >> 4;
                        }
                        if ((minTime & 0xFFFF) == 0) {
                            minTime = 1;
                        }
                    }
                LAB_8003cb18:
                    instanceEvent.m5C_deltaTime = time;
                    instanceEvent.m5E = minTime;
                    if (bNeedPostUpdate) {
                        if (instanceEvent.m4 & 4) {
                            assert(0);
                        }
                        instanceEvent.m64 = instanceEvent.m65;
                        if (instanceEvent.m4 & 0x100) {
                            instanceEvent.m96_volumeSlideDuration = instanceEvent.m80;
                            instanceEvent.m88_volumeSlideDelta = instanceEvent.m7C;
                            instanceEvent.m78_volume = (u32)instanceEvent.m82 << 0x10;
                            instanceEvent.m4 |= 8;
                        }
                        for (int i = 0; i < 4; i++) {
                            instanceEvent.mD8[i].m4 = 0;
                            instanceEvent.mD8[i].m10 = 1;
                            instanceEvent.mD8[i].m14 = instanceEvent.mD8[i].m16;
                            instanceEvent.mD8[i].m18 = instanceEvent.mD8[i].m1A;
                        }
                    }
                }
            }
        }
    }
}

void voicePostUpdate1(sSoundInstance* pSoundInstance, std::vector<sSoundInstanceEvent>& voices, int voicesCount) {
    for (int i = 0; i < voicesCount; i++) {
        sSoundInstanceEvent* pVoice = &voices[i];

        if (pVoice->m0 != 0) {
            pVoice->mD4 = 0;
            pVoice->mD2 = 0;
            pVoice->mD0 = 0;
            if (pVoice->mCE) {
                assert(0);
            }
        }
    }
}

u32 pendingKeyOff2 = 0;
extern u32 pendingKeyOff;

void voicePostUpdate2Sub0(sSoundInstanceEvent30* param_1, uint param_2)
{
    sSoundInstanceEvent30* psVar1;

    if (param_2 < 0x18) {
        psVar1 = playSoundEffectSubSub1Var0[param_2];
        if (psVar1 != param_1) {
            if ((psVar1 != (sSoundInstanceEvent30*)0x0) && (param_1->m4 < psVar1->m4)) {
                return;
            }
            param_1->m6 = -1;
            param_1->m0 = (short)param_2;
            playSoundEffectSubSub1Var0[param_2] = param_1;
            pendingKeyOff = 1 << (param_2 & 0x1f) | pendingKeyOff;
        }
        pendingKeyOn = 1 << (param_2 & 0x1f) | pendingKeyOn;
    }
    return;
}

void voicePostUpdate2Sub1(sSoundInstanceEvent30* param_1, uint param_2)
{
    if ((param_2 < 0x18) && (playSoundEffectSubSub1Var0[param_2] == param_1)) {
        pendingKeyOff2 |= 1 << (param_2 & 0x1f);
    }
    return;
}

// This used to be a lookup table
s8 getPitchTable1(int value) {
    return (value % 0xC) + 0x10 * (value / 0xC);
}

int computeSampleRate(int pitch) {
    int octaveMid = 6 - ((int)(uint)(byte)getPitchTable1((pitch & 0x7fff) >> 8) >> 4);
    if ((int)octaveMid < 0) {
        return (short)((int)(short)pitchTable2
            [(pitch & 0xff) +
            ((byte)getPitchTable1((pitch & 0x7fff) >> 8) & 0xf) * 0x100] <<
            (-octaveMid & 0x1f));
    }
    else {
        return (short)((int)(short)pitchTable2
            [(pitch & 0xff) +
            ((byte)getPitchTable1((pitch & 0x7fff) >> 8) & 0xf) * 0x100] >>
            (octaveMid & 0x1f));
    }
}

void voicePostUpdate2(sSoundInstance* pSoundInstance, std::vector<sSoundInstanceEvent>& voices, int voicesCount) {
    if ((pSoundInstance->m10_flags & 0x20) == 0) {
        for (int i = 0; i < voicesCount; i++) {
            sSoundInstanceEvent* pVoice = &voices[i];

            if (pVoice->m0 != 0) {
                if (pVoice->m2 & 0x100) { // volume update
                    s32 volumeTarget = (pVoice->m78_volume & 0xFFFF) - (((pVoice->m78_volume & 0xFFFF) * pVoice->mD2) >> 0xF);
                    volumeTarget = 0x7FFF;
                    if (volumeTarget > 0x7FFF) {
                        volumeTarget = 0x7FFF;
                    }
                    if (volumeTarget < 0) {
                        volumeTarget = 0;
                    }
                    s32 panTarget = pVoice->m74_pan + pVoice->mD4 + (pSoundInstance->m88.m0.getIntegerPart());
                    
                    if (panTarget > 0x7FFF) {
                        panTarget = 0x7FFF;
                    }
                    if (panTarget < 0) {
                        panTarget = 0;
                    }

                    volumeTarget = ((pSoundInstance->m70.m0.getIntegerPart()) * ((pVoice->m76 * volumeTarget) >> 0xF)) >> 0x10;

                    int volumeLeft;
                    int volumeRight;
                    if ((musicStatusFlag & 0x100) == 0) { // mono?
                        volumeLeft = (volumeTarget * 0x5A00) >> 0xF;
                        volumeRight = volumeLeft;
                    }
                    else {
                        int rawVolumeLeft;
                        int rawVolumeRight;
                        // stereo, handle panning
                        if (panTarget < 0x4000) {
                            rawVolumeLeft = 0x7F00 - ((panTarget * 0x2500) >> 0xE);
                            rawVolumeRight = (panTarget * 0x5A00) >> 0xE;
                        }
                        else {
                            rawVolumeLeft = ((0x8000 - panTarget) * 0x5A00) >> 0xE;
                            rawVolumeRight = 0x7F00 - (((0x8000 - panTarget) * 0x2500) >> 0xE);
                        }
                        volumeLeft = (rawVolumeLeft * 0x5A00) >> 0xF;
                        volumeRight = (rawVolumeRight * 0x5A00) >> 0xF;
                    }
                    pVoice->m30.m8_volumeLeft = volumeLeft;
                    pVoice->m30.mA_volumeRight = volumeRight;
                }
                if (pVoice->m2 & 0x200) {
                    int value = (((pVoice->m68_finalNoteToPlay >> 16) + (int)pVoice->mD0 + (int)(pSoundInstance->m7C.m0.getIntegerPart())) * 0x10000) >> 0x10;
                    int sampleRate = computeSampleRate(value);
                    pVoice->m30.m14_ADPCM_SampleRate = sampleRate & 0x3FFF;
                    pVoice->m30.m6 |= 4;
                }
                if (pVoice->m2 & 1) {
                    voicePostUpdate2Sub0(&pVoice->m30, pVoice->m27);
                }
                if (pVoice->m2 & 2) {
                    voicePostUpdate2Sub1(&pVoice->m30, pVoice->m27);
                }
                pVoice->m2 = 0;
            }
        }
    }
}

u16 needUpdateMasterVolume = 0;
u16 needToUpdateSPUCDVolume = 0;

struct sSpuCommonAttributes {
    u32 mask = 0;
};

sSpuCommonAttributes spuCommonAttributes;

void audioTickSub1() {
    if (pendingKeyOff) {
        for (int i = 0; i < 24; i++) {
            if (pendingKeyOff & (1 << i)) {
                int reg = 5 + i * 8;
                emulatedSpuDevice.write(reg, emulatedSpuDevice.read(reg) & 0xFFC0 | 6); // sustain?
            }
        }
    }

    u32 keyOff = pendingKeyOff2 | pendingKeyOff;
    if (keyOff) {
        emulatedSpuDevice.write(SPU_REG_KEYOFF_low, keyOff);
        emulatedSpuDevice.write(SPU_REG_KEYOFF_high, keyOff>>16);
        pendingKeyOff = 0;
        pendingKeyOff2 = 0;
    }
}

void interpolateAudioParam(sInterpolatableAudioParam* param_1) {
    short sVar1;
    int iVar2;

    sVar1 = param_1->m8;
    param_1->m8 = sVar1 + -1;
    if (sVar1 == 1) {
        iVar2 = (int)param_1->mA << 0x10;
    }
    else {
        iVar2 = param_1->m0 + param_1->m4;
    }
    param_1->m0 = iVar2;
}

void audioTick() {
    spuMutex.lock();
    if ((musicStatusFlag & 0x40) == 0) {
        int oddUpdate = spuUpdateCounter & 1;
        spuUpdateCounter++;

        if (oddUpdate) {
            if (needUpdateMasterVolume != 0) {
                assert(0);
            }
            if (needToUpdateSPUCDVolume != 0) {
                assert(0);
            }
            if (spuCommonAttributes.mask != 0) {
                assert(0);
            }
        }
        sendAdsrToSpu();

        sSoundInstance* pSoundInstance = pPlayingSoundsLinkedList;
        while (pSoundInstance) {
            if (pSoundInstance->m10_flags & 0x8000) {
                if ((pSoundInstance->m2C != 0) && ((uint)pSoundInstance->m2C <= (uint)pSoundInstance->m24)) {
                    assert(0);
                }
                if (pSoundInstance->m64.m8 != 0) {
                    assert(0);
                }
                if (pSoundInstance->m70.m8 != 0) {
                    interpolateAudioParam(&pSoundInstance->m70);
                    applyFlagToAllVoices(0x100, pSoundInstance);
                }
                if (pSoundInstance->m7C.m8 != 0) {
                    assert(0);
                }
                if (pSoundInstance->m88.m8 != 0) {
                    assert(0);
                }

                pSoundInstance->m20++;
                pSoundInstance->m28 += pSoundInstance->m64.m0.getIntegerPart();
                pSoundInstance->m50 -= pSoundInstance->m54;
                while (pSoundInstance->m50 < 0) {
                    pSoundInstance->m36--;
                    pSoundInstance->m50 += 0x10000;
                    if (pSoundInstance->m36 == 0) {
                        pSoundInstance->m36 = pSoundInstance->m3A;
                        pSoundInstance->m34++;
                        if (pSoundInstance->m38 < pSoundInstance->m34) {
                            pSoundInstance->m34 = 1;
                            pSoundInstance->m32++;
                        }
                    }
                    if (pSoundInstance->m14_count) {
                        executeSequenceEvents2(pSoundInstance, pSoundInstance->m94_events, pSoundInstance->m14_count);
                        executeSequenceEvents(pSoundInstance, pSoundInstance->m94_events, pSoundInstance->m14_count);
                    }

                    // if no voices are left active, disable the instance
                    if (pSoundInstance->m48_activeVoicesBF == 0) {
                        pSoundInstance->m10_flags &= 0x7fff;
                        break;
                    }
                    pSoundInstance->m24++;
                    if (pSoundInstance->m70.m0 == 0) {
                        startMusicInstanceSub0(pSoundInstance);
                    }
                    if (pSoundInstance->m32 == pSoundInstance->m1E) {
                        pSoundInstance->m10_flags &= 0xffdf;
                        assert(0);
                        pSoundInstance->m1E = 0;
                    }
                }
            }

            pSoundInstance = pSoundInstance->m0_pNext;
        }

        // second update loop
        pSoundInstance = pPlayingSoundsLinkedList;
        while (pSoundInstance) {
            if ((short)pSoundInstance->m10_flags < 0) {
                voicePostUpdate1(pSoundInstance, pSoundInstance->m94_events, pSoundInstance->m14_count);
                voicePostUpdate2(pSoundInstance, pSoundInstance->m94_events, pSoundInstance->m14_count);
            }

            pSoundInstance = pSoundInstance->m0_pNext;
        }

        audioTickSub1();

        MissingCode();
    }

    std::array<u16, numRegs> tempSPUMemoryStatus;
    for (int i = 0; i < numRegs; i++) {
        tempSPUMemoryStatus[i] = emulatedSpuDevice.read(i);
    }

    spuMutex.unlock();

    spuStatusMutex.lock();
    SPURegisterStatus = tempSPUMemoryStatus;
    spuStatusMutex.unlock();
}

void initSoundSystem(ushort param_1) {
    if ((short)musicStatusFlag < 0) {
        setSoundError(0x28);
    }
    else {
        musicStatusFlag = param_1 | 0xb801;
        MissingCode();
        initLoadedSampleTable();
        loadWdsSub0SubVar1.resize(8);
        MissingCode();
        pPlayingSoundsLinkedList = nullptr;
        pSoundEffectsInstances = nullptr;
        pLoadedSequencesLinkedList = nullptr;
        pLoadedWdsLinkedList = nullptr;
        MissingCode();

        // Setup the audio tick using the Root Counter 2 (ie, SysClk/8)
        // 1 tick of the system clock is approximatively 0.03 microseconds
        // So this should fire ever 0.24 microseconds
        EnterCriticalSection();
        audioTickEvent = OpenEvent(RCntCNT2, EvSpINT, RCntMdINTR, audioTick);
        SetRCnt(RCntCNT2, 0x44e8, 0x1000); 
        StartRCnt(RCntCNT2);
        SpuSetTransferCallback(spuDmaCompletionCallback);
       // SpuSetIRQCallback(spuIrqCallback);
       // SpuSetIRQ(0);
        MissingCode();
        spuUpdateCounter = 0;
       // spuIrqCallbackCount = 0;
        MissingCode();
        ExitCriticalSection();

        MissingCode();
        pSoundEffectsInstances = initSoundEffectInstances(0x10);
        MissingCode();
    }
}

int processSoundMenuSub0(void)
{
    int uVar1;

    if ((musicStatusFlag & 0x700) == 0) {
        uVar1 = 0;
    }
    else {
        uVar1 = 1;
        if ((musicStatusFlag & 0x600) != 0) {
            uVar1 = 2;
        }
    }
    return uVar1;
}

void setupReverb(int mode, s16 param_2, long delay, long feedback) {
    MissingCode();
}

void printTextHex16(const std::string& label, u16 value) {
    ImGui::Text(label.c_str()); ImGui::SameLine(); ImGui::Text("0x%04X", value);
}

void updateSPUDebugger() {
    if (ImGui::Begin("SPU debugger")) {
        spuStatusMutex.lock();

        u32 chanelOn = (((u32)SPURegisterStatus[SPU_REG_CHON_high] << 16) | SPURegisterStatus[SPU_REG_CHON_low]);

        for (int i = 0; i < 24; i++) {
            ImGui::Text((std::string("Voice ") + std::to_string(i)).c_str());

            u32 ADSR = ((u32)SPURegisterStatus[4 + i * 8]) | ((u32)SPURegisterStatus[5 + i * 8]) << 16;
            ImGui::Text("Attack %d", (ADSR >> 8) & 0x7F);
            ImGui::Text("Decay %d", (ADSR >> 4) & 0xF);
            //ImGui::Text("Sustain level  %d", (ADSR & 0xF));
            ImGui::Text("Sustain %d", (ADSR >> 22) & 0x7F);
            ImGui::Text("Release %d", ((ADSR >> 16) & 0x1F));

            ImGui::Text("ChOn  %d", (bool)(chanelOn & (1 << i)));
            printTextHex16("volume left", SPURegisterStatus[0 + i * 8]);
            printTextHex16("volume right", SPURegisterStatus[1 + i * 8]);

            ImGui::Text("Start pos %d", (int)SPURegisterStatus[3 + i * 8] << 3);
            ImGui::Text("Loop pos %d", (int)SPURegisterStatus[7 + i * 8] << 3);

            ImGui::Text("RawPitch 0x%04X", (int)SPURegisterStatus[2 + i * 8]);

            ImGui::Separator();
        }

        spuStatusMutex.unlock();
    }
    ImGui::End();
}