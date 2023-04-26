#pragma once

#include <optional>
#include <array>

#include "sprite/spriteSetup.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "kernel/math.h"

extern sLoadableDataRaw battleCharacterConfigFile;

struct sBattleMechaInitData {
    void init(std::vector<u8>::iterator it, u32 size) {
        mData.insert(mData.begin(), it, it+size);

        {
            auto it2 = it + 4;
            for (int i = 0; i < m4.size(); i++) {
                for (int j = 0; j < m4[i].size(); j++) {
                    m4[i][j].vx = READ_LE_S16(it2); it2 += 2;
                    m4[i][j].vy = READ_LE_S16(it2); it2 += 2;
                }
            }
        }
        {
            auto it2 = it + 0x100;
            for (int i = 0; i < m100.size(); i++) {
                for (int j = 0; j < m100[i].size(); j++) {
                    m100[i][j].vx = READ_LE_S16(it2); it2 += 2;
                    m100[i][j].vy = READ_LE_S16(it2); it2 += 2;
                }
            }
        }

        m348 = READ_LE_S16(it + 0x348);
        m34A = READ_LE_S16(it + 0x34A);
        m47C_at.vx = READ_LE_S16(it + 0x47C + 0);
        m47C_at.vy = READ_LE_S16(it + 0x47C + 2);
        m47C_at.vz = READ_LE_S16(it + 0x47C + 4);
        m482_eye.vx = READ_LE_S16(it + 0x482 + 0);
        m482_eye.vy = READ_LE_S16(it + 0x482 + 2);
        m482_eye.vz = READ_LE_S16(it + 0x482 + 4);
    }
    std::array<std::array<sVec2_s16, 8>, 4> m4;
    std::array<std::array<sVec2_s16, 2>, 2> m100;
    s16 m348;
    s16 m34A;
    SFP_VEC3 m47C_at;
    SFP_VEC3 m482_eye;

    std::vector<u8> mData;
};
extern sBattleMechaInitData* battleMechaInitData;

struct sBattleMechaInitDataFile : public sLoadableData {
    sBattleMechaInitDataFile(size_t size) {
    }
    virtual void init(std::vector<u8>& data) override {
        m4.init(data.begin() + 4, data.size() - 4);
    }

    sBattleMechaInitData m4;
};

#include "field/mecha/mechaOverlay.h"

extern u8 battleInputButton;

extern sLoadableDataRaw* battleLoadDataVar0_raw;
extern std::vector<u8>::iterator battleLoadDataVar1;
extern sBattleMechaInitData* battleLoadDataVar2;
extern sBattleMechaInitData* battleLoadDataVar2Bis;

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