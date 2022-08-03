#pragma once

#include "sprite/spriteSetup.h"

extern std::vector<u8> battleCharacterConfigFile;
extern std::vector<u8>::iterator battleMechaInitData;

#include "field/mecha/mechaOverlay.h"

extern std::vector<u8>* battleLoadDataVar0_raw;
extern std::vector<u8>::iterator battleLoadDataVar1;
extern std::vector<u8>::iterator battleLoadDataVar2;
extern std::vector<u8>::iterator battleLoadDataVar2Bis;

void battleEntryPoint(void);

extern sMechaInitVar4* environmentModelBlocks;
extern s8 requestedBattleConfig;
extern s8 battleTransitionEffect;
extern s8 currentBattleLoaderTransitionEffect;

struct sBattleVisualBuffer {
    std::vector<u8>* m0_spriteData;
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
};
extern std::array<sBattleVisualEntity, 0x11> battleVisualEntities;

extern MATRIX battleRenderingMatrix;