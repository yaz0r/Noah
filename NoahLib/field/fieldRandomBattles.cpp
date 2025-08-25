#include "noahLib.h"
#include "fieldRandomBattles.h"
#include "field.h"
#include "kernel/kernelVariables.h"

s32 g_encounterTimer = 0;
int g_encounterDataCountdown = 0;
int g_encounterCount = 0;
std::array<s32, 32> g_encounterTriggerTime;
std::array<u8, 16> encounterProbabilityWeight;
bool bBattleSequenceInitialized = false;

// Initialize `encounterCount` timers
// Every timers is an unique value in range [0, encouterTimer + 1] 
void setInitialEncounterData()
{
    g_encounterDataCountdown = g_encounterTimer;
    if (g_encounterCount == 0) {
        g_encounterTimer = 0;
        return;
    }

    for (int i = 0; i < 32; i++)
    {
        g_encounterTriggerTime[i] = -1;
    }

    if (g_encounterCount > 0) {
        // Ensure that encounterTickData contains unique random numbers in range [0, encouterTimer + 1]
        for (int encounterId = 0; encounterId < g_encounterCount; encounterId++)
        {
        LAB_Field__8008e798:

            int s1 = 0;
            int r = xenoRand() * (g_encounterTimer + 1);
            int r2 = (r >> 0xf) & 0xffff;

            for (int i = 0; i < 32; i++)
            {
                if (g_encounterTriggerTime[i] == r2) goto LAB_Field__8008e798;
            }
            g_encounterTriggerTime[s1] = r2;
            s1++;
        }
    }

    if (g_encounterCount > 0) {
        for (int encounterId = 0; encounterId < g_encounterCount; encounterId++)
        {
            g_encounterTriggerTime[encounterId]++;
        }
    }
}

void checkForRandomEncounter()
{
    if (((((playMusicAuthorized != 0) && (fieldExecuteVar3 != 0)) && (fieldChangePrevented != 0)) &&
        ((fieldMusicLoadPending != -1 && (g_encounterTimer != 0)))) &&
        ((fieldRandomBattleVar != -1 && ((loadCompleted != 1 && (fieldTransitionCompleted != 0))))))
    {
        g_encounterDataCountdown--;
        if (g_encounterDataCountdown == 0) {
            setInitialEncounterData();
        }

        // Decrease every encounter counter by -1 unless already -1
        if (g_encounterCount > 0) {
            for (int s0 = 0; s0 < g_encounterCount; s0++) {
                if (g_encounterTriggerTime[s0] != -1) {
                    g_encounterTriggerTime[s0]--;
                }
            }
        }

        // Exit unless we have at least one encounter entry
        if (g_encounterCount <= 0) {
            return;
        }

        // Find last entry and set it to -1
        int encounterId = 0;
        while (g_encounterTriggerTime[encounterId] != 0) {
            encounterId++;
            if (encounterId == g_encounterCount) {
                // Exit if no entries have any data
                return;
            }
        }
        g_encounterTriggerTime[encounterId] = -1;

        // Select random encounter using weighted encounter chance:
        //   Get total sum of encounter variables
        //   Find a random value between 0 and total sum
        //   Build a sequence where every element N is sum of previous ones
        //   For every element from end if sum value is less than rand, set encounter start flag
        //   If flag is set - break and activate battle

        int iTotalEncounterSum = 0;
        for (int v1 = 0; v1 < 16; v1++) {
            iTotalEncounterSum += encounterProbabilityWeight[v1];
        }

        std::array<int, 16> encounterProbabilitySum;
        int iPreviousSum = 0;
        for (int v1 = 0; v1 < 16; v1++) {
            encounterProbabilitySum[v1] = iPreviousSum;
            iPreviousSum += encounterProbabilityWeight[v1];
        }

        int iVar5 = (xenoRand() * (iTotalEncounterSum + 1)) >> 0xf;
        bool bShouldTriggerEncounter = false;
        int selectedEncounter = 0;

        for (int v1 = 15; v1 >= 0; v1--) {
            if (encounterProbabilityWeight[v1] != 0 && encounterProbabilitySum[v1] < iVar5) {
                bShouldTriggerEncounter = true;
                selectedEncounter = v1;
                break;
            }
        }

        if (bShouldTriggerEncounter) {
            //SHORT_Field__800b2290 = SHORT_ARRAY_Field__800b2270[iVar6];
            //battleInitVar2 = selectedEncounter;
            //battleInitVar1 = 0;
            //if (mechaOverlayLoadingMode == 0) {
            //    readGameOverlay(2);
            // }
            playMusicAuthorized = 0;
            bBattleSequenceInitialized = true;
            if (fieldDebugDisable != 0) {
                playMusicAuthorized = 0;
                return;
            }
            //recordEncounterDebugData(selectedEncounter);
        }
    }
}