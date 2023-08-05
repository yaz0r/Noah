#pragma once

#include <optional>
#include <array>

#include "sprite/spriteSetup.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "kernel/math.h"
#include "psx/gpuprims.h"
#include "field/fieldGraphicObject.h"

extern std::array<s16, 0xB> battleSlotStatusVar0;
extern std::array<s16, 0xB> battleSlotStatusVar1;
extern std::array<s16, 0xB> numTicksBeforeReady;

extern s8 currentBattleMode;
extern s8 makeBattleTimeProgress;
extern sLoadableDataRaw battleCharacterConfigFile;
extern s8 battlePartyLayoutType;
extern s8 drawBattleMode1Disabled;
extern std::array<std::array<s16, 3>, 3> partyMemberSpritesOffset;

struct sApStruct {
    u8 m0;
    u8 m1;
    u8 m4_currentAP;
    // size 8
};
extern std::array<sApStruct, 11> apConfigArray;

struct sBattleVar2Sub {
    std::array<u8, 8> m0_circleMenuBattleCommandsMapping;
    std::array<u8, 4> m8;
    std::array<u8, 4> mC;
    std::array<s16, 16> m1C_isCommandEnabled;
    u8 m3C_currentTarget;
    // size 0x40
};

struct sBattleVar2 {
    std::array<sBattleVar2Sub, 11> m0;
    std::array<u8, 4> m2C0_circleMenuCommandsNames;
    u8 m2D3_currentEntityTurn;
    u8 m2D4_remainingAP;
    u8 m2D5_maxAP;
    u8 m2DA_indexInBattleVar48;
    u8 m2DB;
    u8 m2DC;
    u8 m2DD_currentActiveBattleMenu;
    u8 m2DE;
    u8 m2DF_currentAttackNumOfApUsed;
    u8 m2E0;
    u8 m2E1_waitBeforeNextAttackInput;
    u8 m2E2_previousActiveBattleMenu;
    u8 m2E3;
    u8 m2E4;
    u8 m2E5;
    u8 m2E8_currentTarget;
    u8 m2E9;
    u8 m2EA;
    std::array<u8, 3> m2EB;
    u8 m2F6;
    // size 0x2F8
};

extern sBattleVar2* battleVar2;

struct sBattleVar0Sub {
    std::array<std::array<POLY_FT4, 2>, 2> m0;
    std::array<std::array<POLY_FT4, 2>, 2> mA0;
    std::array<std::array<POLY_FT4, 2>, 2> m140;
    u8 m1E0;
    u8 m1E1;
    u8 m1E2;
    u8 m1E3;
};

struct sBattleVar0 {
    std::array<std::array<std::array<POLY_FT4, 2>, 6>, 3> m0;
    std::array<std::array<POLY_GT4, 2>, 4> m5A0;
    std::array<std::array<POLY_G4, 2>, 3> m740_APOrFuelPoly;
    std::array<std::array<POLY_FT4, 2>, 3> m818; // unsure size
    std::array < std::array<POLY_FT4, 2>, 3> m9C8_APCounterDisplayPolys; // unsure size
    std::array<std::array<std::array<POLY_FT4, 2>, 10>, 4> m2E08_timeBars;
    std::array<std::array<std::array<POLY_FT4, 2>, 40>, 3> m3A88;
    std::array<std::array<std::array<POLY_FT4, 2>, 4>, 3> m6008;
    std::array<POLY_F4, 2> m63C8;
    std::array<DR_MODE, 2> m63F8;
    s32 m6410 = 0;
    u8 m6414 = 0;
    u8 m6415;
    u8 m6416;
    std::array<std::array<std::array<POLY_FT4, 2>, 50>, 2> m641C;
    std::array<DR_MODE, 2> m8908_drMode;
    std::array<DR_MODE, 2> m8920;
    std::array<sBattleVar0Sub, 3> m835C;
    s32 mA234;
    s32 mA238;
    s32 mA23C;
    s32 mA240;
    s32 mA244_X;
    s32 mA248_Y;
    u16 mA2AC;
    u16 mA2AE;
    u16 mA2B0;
    u16 mA2B2;
    // size 0xA2B4
};
extern sBattleVar0* battleVar0;

struct sBattleVar1 {
    RECT m0;
    RECT m8;
    s32 m34_timeBarScreenPositionX;
    s32 m3C_timeBarAnimationTargetPositionX;
    s32 m44_timeBarScreenPositionY;
    s32 m4C_timeBarAnimationTargetPositionY;
    s32 m54_timeBarAnimationXStep;
    s32 m5C_timeBarAnimationYStep;
    s32 m64_timeBarAnimationX;
    s32 m6C_timeBarAnimationY;
    std::array<u8, 4> m74_timeBarsLengths;
    std::array<u8, 3> m78;
    u8 m7B;
    std::array<u8, 3> m7C;
    std::array<u8, 4> m7F;
    u8 m83;
    std::array<u8, 4> m84_timeBarsOddOrEven;
    std::array<u8, 3> m90_perPCTimeBarStatus;
    std::array<u8, 3> m93_oddOrEven;
    u8 m97;
    std::array<u8, 3> m99_oddOrEven;
    u8 mA2;
    u8 mA3;
    u8 mA4_oddOrEven7B;
    u8 mA9_timeBarNumMoveSteps;
    u8 mAB_timeBarNumAnimationSteps;
    u8 mAF;
    std::array<u8, 7> mB0_isDialogWindowInitialized;
    u8 mC6_isTargetSelectionCursorVisible;
    u8 mCB;
    std::array<u8, 3> mCC;
    u8 mCF;
    std::array<u8, 2> mD0_battleCommandLength;
    std::array<u8, 3> mE0;
    std::array<u8, 3> mEC;
    s16 m104_timeBarAnimationScale;
    s16 m106_timeBarAnimationRotation;
    // size 0x10C
};
extern sBattleVar1* battleVar1;

struct sBattle800c3e24 {
    void reset() {
        MissingCode(); // normally a memset 0
    }

    std::array<std::array<POLY_G3, 2>, 4> m0_poly;
    s32 mE0_fadeValue;
    u8 mE4_oddOrEven;
    u8 mE5_fadeDirection;
    std::array<u8, 4> mE6_isDirectionEnabled;
};
extern sBattle800c3e24* battleG3;

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

        {
            auto it2 = it + 0x140;
            for (int i = 0; i < m140.size(); i++) {
                for (int j = 0; j < m140[i].size(); j++) {
                    for (int k = 0; k < m140[i][j].size(); k++) {
                        m140[i][j][k] = READ_LE_U8(it2); it2++;
                    }
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
    std::array<std::array<std::array<u8, 8>, 8>, 8> m140;
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
extern sBattleMechaInitData* battleLoadDataVar2Ter;

void battleEntryPoint(void);
void battleHandleInput(void);
void battleSoundEffect2(u32);

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
    u8 m0_positionSlot;
    u8 m1;
    u8 m2;
    u8 m3;
    u8 m4_isGear;
    u8 m5;
    s8 m6_direction;
    s16 mA_X;
    s16 mC_Z;

    // size 0x1C
};
extern std::array<sBattleVisualEntity, 0x11> battleVisualEntities;

struct sBattleSpriteActor {
    sTaskHeader m0;
    sTaskHeader m1C;
    sSpriteActor m38_spriteActor;
};

extern std::array<sBattleSpriteActor*, 11> battleSpriteActors;
extern std::array<sSpriteActorCore*, 11> battleSpriteActorCores;

extern MATRIX battleRenderingMatrix;
extern s8 battleTimeEnabled;
extern s8 battleCharacters[3];
extern u8 bBattleTickMode1;
extern std::array<s8, 0xB> isEntityReadyForBattle;
extern std::array<s8, 0xB> randomTurnOrder;
extern s8 currentEntryInRandomTurnOrder;
extern std::array<s8, 11> isBattleSlotFilled;
void mechaInitEnvironmentMechaMesh(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9);
u16 characterIdToTargetBitmask(uint param_1);

struct sBattleEntity {
    sGameStateA4 m0_base;
    sGameStateA42 mA4_gear;
    u8 m15A_flags;
};

extern std::array<sBattleEntity, 11> battleEntities;
extern std::vector<u8> battleFont;

extern u8 battleGetSlotStatusSub_current28Index;
extern bool battleSpritesDisabled;

int battleSetupStringInPolyFT4Small(int character, std::array<POLY_FT4, 2>* polyArray, short x, short y);
int battleSetupStringInPolyFT4Large(int character, std::array<POLY_FT4, 2>* polyArray, short x, short y);
void battleSetupTextPoly(POLY_FT4* param_1);

u8 getEntityToFace(u8 param_1);
bool getDirectionBetween2BattleEntities(uint param_1, uint param_2);
extern const std::array<u16, 16> party1C_InitialValues;

struct sBattle800CDD40Sub {
    s16 m2;
    u16 mA;
    s8 m16;
    s8 m27;
    //size 0x28
};
struct sBattle800cdd40 {
    std::array<sBattle800CDD40Sub, 11> m0;

    // size 0x5f0 ?
};

extern sBattle800CDD40Sub* battleGetSlotStatusSub_current28Entry;
extern std::array<sBattle800cdd40, 3> battle800CDD40;
extern sBattleEntity* battleGetSlotStatusSub_currentBattleEntity;
extern sGameStateA42* battleGetSlotStatusSub_currentBattleEntityGear;
extern u16 jumpAnimationActiveActorBF;

void setupBattleAnimationSpriteCore(sSpriteActorCore* param_1);
void performMechaPlayAnimation(ushort param_1, short param_2, int param_3);
int waitForMusic(uint param_1);
void clearShapeTransferEntry(void*);