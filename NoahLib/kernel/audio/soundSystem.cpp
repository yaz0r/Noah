#include "noahLib.h"
#include "soundSystem.h"
#include "wds.h"
#include "spu.h"
#include "seq.h"
#include "soundInstance.h"
#include "kernel/criticalSection.h"
#include "kernel/events.h"
#include "seqOpcodes.h"

u16 musicStatusFlag = 0;

void setSoundError(u16 errorCode) {
    //assert(0);
}

void sendAdsrToSpu() {
    MissingCode();
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
        param_1->m54 = (param_1->m58 >> 16) * (param_1->m64 >> 16);
    }

    for (int i = 0; i < param_3; i++) {
        sSoundInstanceEvent& instanceEvent = param_2[i];

        if (instanceEvent.m0) {
            u16 flags = instanceEvent.m2;
            if (instanceEvent.m5C_deltaTime) {
                s16 var4 = instanceEvent.m4;
                if (var4 & 8) { // volume slide
                    instanceEvent.m96_volumeSlideDuration--;
                    flags |= 0x100; // update volume
                    if (instanceEvent.m96_volumeSlideDuration == 0) {
                        var4 &= ~8;
                    }
                    instanceEvent.m78_volume += instanceEvent.m88_volumeSlideDelta;
                }
                if (var4 & 1) {
                    assert(0);
                }
                if (var4 & 0x10) {
                    assert(0);
                }
                if (var4 & 0x20) {
                    assert(0);
                }
                instanceEvent.m4 = var4;

                if (--instanceEvent.m5C_deltaTime == 1) {
                    if (instanceEvent.m0 & 0x1000) {
                        instanceEvent.m5A = 6;
                        instanceEvent.m30.m6 |= 0x80;
                    }
                }
                if (--instanceEvent.m5E == 0) {
                    flags |= 2;
                    instanceEvent.m0 |= 0x400;
                }
            }
            instanceEvent.m2 = flags;
        }
    }
}

static const std::array<u8, 19> deltaTimeTable = { { 0, 192, 144, 96, 72, 64, 48, 36, 32, 24, 18, 16, 12, 9, 8, 6, 4, 3, 2 } };

void processPercussions(sSoundInstance* param_1, sSoundInstanceEvent* param_2, u32 param_3) {
    auto& percussionData = param_1->mC_pPercussionData->data[param_3];
    setupAdsr(percussionData.m0_adsr, param_2);
    param_2->m68 = ((uint)percussionData.m1 * 0x100 + (int)param_2->m6E + (int)param_2->m6C) * 0x10000;
    param_2->m2 |= 0x100;
    param_2->m74_pan = percussionData.m3_pan << 8;
}

void executeSequenceEvents(sSoundInstance* param_1, std::vector<sSoundInstanceEvent>& param_2, short param_3) {
    for (int i = 0; i < param_3; i++) {
        sSoundInstanceEvent& instanceEvent = param_2[i];
        if (instanceEvent.m0) {
            bool bNeedPostUpdate = false;
            if ((instanceEvent.m5C_deltaTime & 0xFFFF) == 0) {
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
                        s8 relativeKey = (data_byte / 19); // Note: this was done with a lookup table in original code
                        byteCodeIt += 2;
                        instanceEvent.m65 = instanceEvent.m64 + relativeKey;

                        u8 deltaTime = deltaTimeTable[data_byte % 19]; // Note this was done with a repeating table in original code
                        if (deltaTime == 0) {
                            deltaTime = byteCodeIt[0];
                            byteCodeIt++;
                        }
                        instanceEvent.m5C_deltaTime = deltaTime;
                        instanceEvent.m5A = instanceEvent.m28;
                        instanceEvent.m30.m6 |= 0x80;

                        if ((instanceEvent.m0 & 0x10) == 0) {
                            // no percussion
                            instanceEvent.m68 = ((s32)instanceEvent.m6C * 0x10000) + ((s32)instanceEvent.m66_octave * 0x100) + ((s32)instanceEvent.m6E);
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
                        byteCodeIt = seqOpcodes[byteCode - 0x80](byteCodeIt + 1, param_1, &instanceEvent);
                        if (instanceEvent.m0 == 0) { // disable the voice
                            param_1->m48_activeVoicesBF &= ~(1 << (instanceEvent.m6 & 0x1f));
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
                            if (!instanceEvent.m18_infinitLoopStart.has_value()) {
                                goto LAB_8003ca60;
                            }
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

                    int time = (int)instanceEvent.m60 + (int)instanceEvent.m5C_deltaTime;
                    if (time * 0x10000 < 1) {
                        time += instanceEvent.m5C_deltaTime;
                        instanceEvent.m60 += instanceEvent.m5C_deltaTime;
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
                            assert(0);
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

void audioTick() {
    if ((musicStatusFlag & 0x40) == 0) {
        int oddUpdate = spuUpdateCounter & 1;
        spuUpdateCounter++;

        if (oddUpdate) {
            MissingCode();
        }
        sendAdsrToSpu();

        sSoundInstance* pSoundInstance = pPlayingSoundsLinkedList;
        while (pSoundInstance) {
            if (pSoundInstance->m10_flags & 0x8000) {
                if ((pSoundInstance->m2C != 0) && ((uint)pSoundInstance->m2C <= (uint)pSoundInstance->m24)) {
                    assert(0);
                }
                if (pSoundInstance->m6C != 0) {
                    assert(0);
                }
                if (pSoundInstance->m78 != 0) {
                    assert(0);
                }
                if (pSoundInstance->m84 != 0) {
                    assert(0);
                }
                if (pSoundInstance->m90 != 0) {
                    assert(0);
                }

                pSoundInstance->m20++;
                pSoundInstance->m28 = (pSoundInstance->m64 >> 16) + pSoundInstance->m28;
                pSoundInstance->m50 -= pSoundInstance->m54;
                while (pSoundInstance->m50) {
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

                    // if not voices are left active, disable the instance
                    if (pSoundInstance->m48_activeVoicesBF == 0) {
                        pSoundInstance->m10_flags &= 0x7fff;
                        break;
                    }
                    pSoundInstance->m24++;
                    if (pSoundInstance->m70 == 0) {
                        assert(0);
                    }
                    if (pSoundInstance->m32 == pSoundInstance->m1E) {
                        pSoundInstance->m10_flags &= 0xffdf;
                        //assert(0);
                        MissingCode();
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
                assert(0);
            }

            pSoundInstance = pSoundInstance->m0_pNext;
        }

        MissingCode();
    }
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