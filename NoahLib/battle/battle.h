#pragma once

#include <optional>

#include "sprite/spriteSetup.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"

extern sLoadableDataRaw battleCharacterConfigFile;
extern std::vector<u8>::iterator battleMechaInitData;

#include "field/mecha/mechaOverlay.h"

extern u8 battleInputButton;

extern sLoadableDataRaw* battleLoadDataVar0_raw;
extern std::vector<u8>::iterator battleLoadDataVar1;
extern std::vector<u8>::iterator battleLoadDataVar2;
extern std::vector<u8>::iterator battleLoadDataVar2Bis;

void battleEntryPoint(void);
void battleHandleInput(void);
void playBattleSound(int);

extern u8 battleInitVar0;

extern sMechaInitVar4* environmentModelBlocks;
extern s8 requestedBattleConfig;
extern s8 battleTransitionEffect;
extern s8 currentBattleLoaderTransitionEffect;

struct sBattleVisualBuffer {
    std::optional<std::vector<u8>::iterator> m0_spriteData;
    s16 m4_vramX;
    s16 m6_vramY;
    u32 m8 = 0;

    sSpriteActorAnimationBundle bundle;
};
extern std::array<sBattleVisualBuffer, 0x11> battleVisualBuffers;

struct sBattleVisualEntity {
    u8 m0;
    u8 m1;
    u8 m2;
    u8 m3;
    u8 m4;
    u8 m5;
    s8 m6_direction;
    s16 mA_X;
    s16 mC_Z;

    // size 0x1C
};
extern std::array<sBattleVisualEntity, 0x11> battleVisualEntities;

extern MATRIX battleRenderingMatrix;
extern s8 battleTimeEnabled;
extern s8 battleCharacters[3];
extern u8 bBattleTickMode1;
extern std::array<s8, 0xB> battleEntityTurnIndex3;
extern std::array<s8, 0xB> battleEntityTurnIndex2;
extern s8 battleEntityTurnIndex;
extern std::array<s8, 11> isBattleSlotFilled;
void mechaInitEnvironmentMechaMesh(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9);
u16 characterIdToTargetBitmask(uint param_1);

struct sBattleEntity {
    sGameStateA4 m0_base;
    sGameStateA42 mA4_gear;
    u8 m15A_flags;
};

extern std::array<sBattleEntity, 11> battleEntities;