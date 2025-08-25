#pragma once

// Encounter timers and data
extern s32 g_encounterTimer;
extern s32 fieldTransitionCompleted;
extern s32 g_encounterDataCountdown;
extern int g_encounterCount;
extern std::array<s32, 32> g_encounterTriggerTime;
extern std::array<u8, 16> encounterProbabilityWeight;
extern bool bBattleSequenceInitialized;

void checkForRandomEncounter();
void setInitialEncounterData();