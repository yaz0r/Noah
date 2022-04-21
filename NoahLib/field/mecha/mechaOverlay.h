#pragma once
#include "kernel/math.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"

typedef u16 sMechaDataTable2_4_8;

struct sMechaDataTable2_4_4 {
    void init(std::vector<u8>& input) {
        m_raw = input;
        if (input.size() == 0)
            return;

        m4_flags = READ_LE_U16(input.begin() + 4);
        m6 = READ_LE_U16(input.begin() + 6);
        mC_count0 = READ_LE_U16(input.begin() + 0xC);
        mE_count1 = READ_LE_U16(input.begin() + 0xE);

        int remainingSize = input.size() - 0x18;
        int remainingVectors = (remainingSize / 2) / 3;
        //assert(remainingVectors == mC_count0 + mE_count1);

        m18.resize(remainingVectors);
        for (int i = 0; i < remainingVectors; i++) {
            m18[i][0] = READ_LE_S16(input.begin() + 0x18 + 2 * 3 * i + 0);
            m18[i][1] = READ_LE_S16(input.begin() + 0x18 + 2 * 3 * i + 2);
            m18[i][2] = READ_LE_S16(input.begin() + 0x18 + 2 * 3 * i + 4);
        }
    }

    u16 m4_flags;
    u16 m6;
    u16 mC_count0;
    u16 mE_count1;
    std::vector<std::array<s16, 3>> m18;

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

        {
            std::vector<u8> tempM8 = relocatedData[1];
            int count = tempM8.size() / 2;
            m8.resize(count);
            for (int i = 0; i < count; i++) {
                m8[i] = READ_LE_U16(tempM8.begin() + 2 * i);
            }
        }
        mC = relocatedData[2];
        m10 = relocatedData[3];
    }

    struct sMechaDataTable2_4_4_array {
        // size of array is m0
        std::vector<sMechaDataTable2_4_4> m4;
    } m4;

    std::vector<sMechaDataTable2_4_8> m8;
    std::vector<u8> mC;
    std::vector<u8> m10;

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

        m4.init(relocatedData[0]);
        m8.init(relocatedData[1]);
    }

    sMechaDataTable2_4 m4;
    sMechaDataTable2_8 m8;

    std::vector<u8> m_raw;
};

struct sMechaInitVar2Sub {
    u8 m0;
    s8 m3;
    // size 0x14
};

struct sLoadedMecha_sub4 {
    sLoadedMecha_sub4* m0_parentBone;
    u8 m4;
    u8 m5;
    u8 m6;
    u8 m7;
    s16 m8;
    s16 mA_numBones;
    MATRIX mC;
    MATRIX m2C;
    std::array<s16, 3> m4C;
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
    std::vector<sLoadedMecha_sub4>* m4;
    std::vector<sMechaDataTable2_4_8>* m8;
    u32 mC;
    sMechaDataTable2_4_8* m10_bytecode0;
    sMechaDataTable2_4::sMechaDataTable2_4_4_array* m14;
    std::vector<u8>* m18;
    s16 m1C_moveSpeed;
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

extern std::array<sLoadedMechas*, 10> loadedMechas;
extern std::array<int, 10> mechaList3;

void loadMechaOVerlayAndRegisterMechas();

void freeMechaModule();

void renderMechasInField();
void initMechaFieldArgs2(MATRIX& param_1, short param_2, short param_3, short param_4, short param_5, short param_6, short param_7, short param_8, short param_9, short param_10);

void renderMechasForDebugFieldRenderer(int viewId);

void mechaPlayAnimation(ushort param_1, short param_2, int param_3);

extern std::vector<u8> mechaOverlayBuffer;
extern u32 NumMechas;
extern std::array<s16, 4> mechaList;
extern std::array<u8, 4> mechaList2;
extern s16 numMaxMechaOverlayEntries;
extern std::array<u8, 3> mechaBackColor;
extern std::array<MATRIX, 2> mechaFieldArgs2;
extern bool disableMechaRendering;
