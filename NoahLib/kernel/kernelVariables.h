#pragma once

extern u32 playMusicSubVar2;
extern int currentlyPlayingMusic;
extern int updateMusicState2Var3;
extern int typeOfPlayableCharacterLoaded;
extern int fieldRequestedGears;
extern int fieldScriptEntityAlreadyInitialized;
extern int mechaOverlayLoadingMode;
extern int needStartMusicInstanceSub0;
extern int needReloadWds;
extern int characterOrGearsLoadingInProgress;
extern int updateMusicState2Var1;
extern int musicVar2;
extern s32 g_menuOpenCount;
extern int compassGraphicDataLoaded;
extern int updateMusicState2Var2;
extern int worldmapSoundFilesPendingCount;
extern u32 playMusicSubVar0;
extern bool disableMechaRendering;
extern bool disableCharacterShadowsRendering;
extern int fieldCompassVar;
extern u8 menuReturnState0;
extern u16 playWorldmapMusic;
extern s32 currentFieldId1;
extern s32 fieldMapNumber;
extern int currentlyLoadedWave;
extern int currentlyLoadedMusic;
extern s32 currentFieldId0;
extern int loadFieldSeqVar0;
extern int musicVar1;
extern s32 fieldMusicLoadPending;
extern std::array<int, 3> asyncPartyCharacterLoadingTable;
extern std::array<int, 3> unkPartyTable;
extern std::array<int, 3> partyToFieldEntityArrayMapping;
extern std::array<s32, 3> currentParty;

void initKernelVariables();