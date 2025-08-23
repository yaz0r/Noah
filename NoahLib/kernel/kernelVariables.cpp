#include "noahLib.h"
#include "kernelVariables.h"

// TODO: those are located somewhere else, but allocated in this file:
u8 menuReturnState0 = 0;
std::array<int, 3> asyncPartyCharacterLoadingTable;
std::array<int, 3> unkPartyTable;
std::array<int, 3> partyToFieldEntityArrayMapping;
std::array<s32, 3> currentParty;

// Those are in correct order compared to PSX memory:

//DAT_8004f2f4
//DAT_8004f2f8
//DAT_8004f2fc
//DAT_8004f300
int worldmapSoundFilesPendingCount = 0;
s32 fieldMusicLoadPending = -1;
int fieldScriptEntityAlreadyInitialized = 0;
//DAT_8004f310
//DAT_8004f314
//DAT_8004f318
int typeOfPlayableCharacterLoaded = 0;
int fieldRequestedGears = 0;
int currentlyPlayingMusic = 0xFF;
//DAT_8004f328
int loadFieldSeqVar0 = -1;
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;
int currentlyLoadedMusic = -1;
int currentlyLoadedWave = -1;
int musicVar1 = -1;
int compassGraphicDataLoaded = 0;
int updateMusicState2Var2 = 0;
s32 fieldMapNumber = -1;
s32 g_menuOpenCount = 0;
int musicVar2 = 0;
int updateMusicState2Var1 = 0;
int needStartMusicInstanceSub0 = 0;
int needReloadWds = 0;
u32 playMusicSubVar2 = 1;
u32 playMusicSubVar0 = 0;
int updateMusicState2Var3 = 0;
int mechaOverlayLoadingMode = 0;
int characterOrGearsLoadingInProgress = 0;
int fieldCompassVar = 0;
bool disableCharacterShadowsRendering = 0;
bool disableMechaRendering = false;
u16 playWorldmapMusic = 0;


void initKernelVariables() {
    playMusicSubVar2 = 1;
    MissingCode();
    currentlyPlayingMusic = 0xff;
    MissingCode();
    updateMusicState2Var3 = 0;
    MissingCode();
    typeOfPlayableCharacterLoaded = 0;
    fieldRequestedGears = 0;
    MissingCode();
    fieldScriptEntityAlreadyInitialized = 0;
    mechaOverlayLoadingMode = 0;
    needStartMusicInstanceSub0 = 0;
    needReloadWds = 0;
    characterOrGearsLoadingInProgress = 0;
    updateMusicState2Var1 = 0;
    musicVar2 = 0;
    g_menuOpenCount = 0;
    MissingCode();
    compassGraphicDataLoaded = 0;
    updateMusicState2Var2 = 0;
    worldmapSoundFilesPendingCount = 0;
    playMusicSubVar0 = 0;
    MissingCode();
    disableMechaRendering = 0;
    disableCharacterShadowsRendering = 0;
    fieldCompassVar = 0;
    MissingCode();
    menuReturnState0 = 0;
    playWorldmapMusic = 0;
    MissingCode();
    currentFieldId1 = -1;
    fieldMapNumber = -1;
    currentlyLoadedWave = -1;
    currentlyLoadedMusic = -1;
    currentFieldId0 = -1;
    loadFieldSeqVar0 = -1;
    musicVar1 = -1;
    fieldMusicLoadPending = -1;

    asyncPartyCharacterLoadingTable.fill(0);
    unkPartyTable.fill(0);
    partyToFieldEntityArrayMapping.fill(0);
    currentParty.fill(0);
    MissingCode();
}