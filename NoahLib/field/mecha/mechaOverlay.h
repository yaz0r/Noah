#pragma once
#include "kernel/math.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "field/fieldModel.h"

typedef u16 sMechaDataTable2_4_8;

struct sMechaDataTable2_4_4 {
    void init(std::vector<u8>& input) {
        m_raw = input;
        if (input.size() == 0)
            return;

        m2_keyDuration = READ_LE_U16(input.begin() + 2);
        m4_flags = READ_LE_U16(input.begin() + 4);
        m6 = READ_LE_U16(input.begin() + 6);
        mC_numRotations = READ_LE_U16(input.begin() + 0xC);
        mE_numTranslations = READ_LE_U16(input.begin() + 0xE);
        m12 = READ_LE_S16(input.begin() + 0x12);
        m14 = READ_LE_U32(input.begin() + 0x14);
        int remainingSize = input.size() - 0x18;

        m18.resize(remainingSize);
        for (int i = 0; i < remainingSize; i++) {
            m18[i] = READ_LE_U8(input.begin() + 0x18 + i);
        }
    }

    u16 m2_keyDuration;
    u16 m4_flags;
    u16 m6;
    s16 m10;
    u16 mC_numRotations;
    u16 mE_numTranslations;
    s16 m12;
    u32 m14;
    std::vector<u8> m18;

    std::vector<u8> m_raw;
};

struct sMechaDataTable2_4 {
    void init(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);
        //assert(relocatedData.size() == 4);

        {
            std::vector<u8> tempM4 = relocatedData[0];
            std::vector<std::vector<u8>> relocatedDataM4 = doPointerRelocationAndSplit(tempM4);
            m4.m4.resize(relocatedDataM4.size());
            for (int i = 0; i < relocatedDataM4.size(); i++) {
                m4.m4[i].init(relocatedDataM4[i]);
            }
        }

        m8.reserve(relocatedData.size() - 1);
        for (int i = 1; i < relocatedData.size(); i++) {
            std::vector<sMechaDataTable2_4_8> bytecode;
            for (int j = 0; j < relocatedData[i].size() / 2; j++) {
                bytecode.push_back(READ_LE_U16(relocatedData[i].begin() + j * 2));
            }
            m8.push_back(bytecode);
        }
    }

    struct sMechaDataTable2_4_4_array {
        // size of array is m0
        std::vector<sMechaDataTable2_4_4> m4;
    } m4;

    std::vector<std::vector<sMechaDataTable2_4_8>> m8;

    std::vector<u8> m_raw;
};

struct sMechaDataTable2_8 {
    void init(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);
    }
    std::vector<u8> m_raw;
};

struct sMechaDataTable2 {
    sMechaDataTable2(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);

        m4.init(relocatedData[relocatedData.size() - 2]);
        m8.init(relocatedData[relocatedData.size() - 1]);
    }

    sMechaDataTable2_4 m4;
    sMechaDataTable2_8 m8;

    std::vector<u8> m_raw;
};

struct sMechaDataTable1_C {
    s16 m0;
    s16 m2;
};

struct sMechaDataTable1_sub4 {
    void init(std::vector<u8>& inputData) {
        m_raw = inputData;

    }

    std::vector<u8> m_raw;
};

struct sMechaDataTable1_10_4 {
    sMechaDataTable1_10_4(std::vector<u8>& input) {
        m_raw = input;

        std::vector<u8>::iterator it = input.begin();

        m2[0] = READ_LE_S16(it + 2);
        m2[1] = READ_LE_S16(it + 2 + 2);
        m2[2] = READ_LE_S16(it + 2 + 4);

        m8 = READ_LE_S16(it + 8);
        mA = READ_LE_U8(it + 0xA);
        mC = READ_LE_S16(it + 0xC);
        mE = READ_LE_U8(it + 0xE);
        m10 = READ_LE_U8(it + 0x10);
        m12 = READ_LE_U8(it + 0x12);
    }


    std::array<s16, 3> m2;
    s16 m8;
    u8 mA;
    s16 mC;
    u8 mE;
    u8 m10;
    u8 m12;

    std::vector<u8> m_raw;
};

struct sMechaDataTable1_10 {
    sMechaDataTable1_10(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);

        assert(input.size() == 0x20);
        m4 = new sMechaDataTable1_10_4(relocatedData[0]);
    }
    sMechaDataTable1_10_4* m4;

    std::vector<u8> m_raw;
};

struct sMechaDataTable1 {
    sMechaDataTable1(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);

        m4_textures.init(relocatedData[0]);
        m8_modelBlocks.init(relocatedData[1].begin(), relocatedData[1].size());
        mC.resize(relocatedData[2].size() / 4);
        for (int i = 0; i < mC.size(); i++) {
            mC[i].m0 = READ_LE_S16(relocatedData[2].begin() + i * 4 + 0);
            mC[i].m2 = READ_LE_S16(relocatedData[2].begin() + i * 4 + 2);
        }

        m10 = new sMechaDataTable1_10(relocatedData[3]);

        return;
    }

    sMechaDataTable1_sub4 m4_textures;
    sModel m8_modelBlocks;
    std::vector<sMechaDataTable1_C> mC;
    sMechaDataTable1_10* m10;

    std::vector<u8> m_raw;
};

struct sMechaInitVar2Sub {
    u8 m0;
    u8 m1_isLooping;
    s8 m2;
    s8 m3;
    std::vector<u8>::iterator m4;
    std::vector<u8>::iterator m8;
    s16 m4_s16;
    s16 m6_s16;
    s16 m8_s16;
    s16 mA_s16;
    s16 mC;
    s16 mE;
    s16 m10_currentTime;
    s16 m12_maxTime;
    // size 0x14
};

struct sMechaBone {
    sMechaBone* m0_parentBone;
    u8 m4_needTranslationUpdate;
    u8 m5_needRotationUpdate;
    u8 m6;
    u8 m7_isEnabled;
    s16 m8_geometryId;
    s16 mA_numBones;
    MATRIX mC_localMatrix;
    MATRIX m2C_boneFinalMatrix;
    std::array<s16, 3> m4C_scale;
    s16 m52;
    SFP_VEC4 m54_rotationAngles;
    std::array<s32, 3> m5C_translation;
    std::array<std::vector<sTag*>, 2> m68;
    std::array<sMechaInitVar2Sub*, 3> m70_animTracks;
    //size 0x7C;
};

struct sMechaInitVar4
{
    std::vector<struct sModelBlock*> m0;
    u32 m4;
};

struct sLoadedMechas {
    sMechaInitVar4* m0;
    std::vector<sMechaBone>* m4_bones;
    std::vector<std::vector<sMechaDataTable2_4_8>>* m8_bytecodeTable;
    u32 mC;
    sMechaDataTable2_4_8* m10_bytecode0;
    sMechaDataTable2_4::sMechaDataTable2_4_4_array* m14;
    std::vector<u8>* m18;
    s16 m1C_scale;
    s16 m1E;
    u8 m20_mechaEntryId;
    u8 m22;
    u8 m23;
    std::array<s16, 3> m24;
    std::array<u8, 2> m2A;
    u8 m34;
    u8 m35;
    u8 m36;
    u8 m37;
    u8 m38;
    u8 m39;
    s16 m3A;
    s16 m3C;
    s16 m40;
    s16 m42;
    u16 m4A;
    std::vector<u16>* m4C_bytecode3;
    std::vector<u16>* m50_bytecode2;
    std::vector<u16>* m54_bytecode1;
    s16 m58;
    s8 m5C;
    s8 m5D;
    s16 m5E;
    s16 m60;
    u8 m62_isSeqLoaded;
    u8 m63;
    std::array<s16, 3> m70;
    std::array<s16, 3> m76;
    std::array<s16, 3> m7C;
    std::array<s16, 3> m82;
    std::array<s16, 3> m88;
    s16 m8E;
    std::array<s16, 3> m90;
    s16 m96;
    s16 m98;
    s16 m9A;
    s16 m9C;
    s16 m9E;
    u32 mA0;
    u32 mA4;
    struct sModel* mA8;
    sMechaDataTable2* mAC;
    sMechaDataTable2_8* mB0;
    std::array<POLY_FT4, 2> mB8_Polys;
    s16 m10A;
    u8 m10C; // count of m110
    u8 m10D; // count of m114
    u8 m10E; // count of m118
    struct sLoadedMechas_110 {
        s16 m0;
        u32 m8;
        // size 0x70
    };
    std::vector<sLoadedMechas_110> m110;

    struct sLoadedMechas_114 {
        // size 0x24
    };
    std::vector<sLoadedMechas_114> m114;
    struct sLoadedMechas_118 {
        s32 m4;
        s32 m8;
        s32 mC;
        s16 m1A;
        // size 0x30
    };
    std::vector<sLoadedMechas_118> m118;
    std::array<s32, 3> m11C_previousTranslation;
    std::array<s32, 3> m128_deltaTranslation;
    // size 0x134
};

extern std::array<s16, 4> mechaCurrentAnimation;
extern std::array<sLoadedMechas*, 10> loadedMechas;
extern std::array<int, 10> mechaList3;

void loadMechaOVerlayAndRegisterMechas();

void freeMechaModule();

void renderMechasInField();
void initMechaFieldArgs2(MATRIX& param_1, short param_2, short param_3, short param_4, short param_5, short param_6, short param_7, short param_8, short param_9, short param_10);

void renderMechasForDebugFieldRenderer(int viewId);

void mechaPlayAnimation(ushort param_1, short param_2, int param_3);

void getMechaBoneMatrix(MATRIX* param_1, void* param_2, int mechaIndex, int boneIndex);
void freeMecha(int index);
void mechaInitNewMecha(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9);

extern std::array<sMechaDataTable1*, 9> mechaDataTable1;
extern std::array<sLoadableDataRaw, 9> mechaDataTable1_raw;

extern std::array<sMechaDataTable2*, 9> mechaDataTable2;
extern std::array<sLoadableDataRaw, 9> mechaDataTable2_raw;

extern std::array<SFP_VEC4, 4> initMechaTempVar;

extern std::array<sLoadingBatchCommands, 16> mechaOverlayBatchLoadingTable;

extern sLoadableDataRaw mechaOverlayBuffer;
extern u32 NumMechas;
extern std::array<s16, 4> mechaList;
extern std::array<u8, 4> mechaList2;
extern s16 numMaxMechaOverlayEntries;
extern std::array<u8, 3> mechaBackColor;
extern std::array<MATRIX, 2> mechaFieldArgs2;
extern bool disableMechaRendering;

struct sMechaInitVar2 {
    std::vector<sMechaInitVar2Sub> m0;
    s16 m4;
    s16 m6;
};

void initMechaInitVar2(sMechaInitVar2* param_1, int count);

struct sMechaInitVar3Sub {
    s16 m16;
    s16 m1E;
    std::array<POLY_FT4, 2> m2C_polys;
    // size 0x7C
};

struct sMechaInitVar3 {
    std::vector<sMechaInitVar3Sub> m0;
    s16 m4;
    s16 m6;
};

void initMechaInitVar3(sMechaInitVar3* param_1, int param_2);
