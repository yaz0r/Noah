#include "noahLib.h"
#include "mechaOverlay.h"
#include "field/field.h"
#include "kernel/filesystem.h"
#include "kernel/gte.h"

u32 NumMechas;
std::array<s16, 4> mechaList;
std::array<u8, 4> mechaList2;

void waitForLoading(void)
{
    int iVar1;

    while (iVar1 = isLoadCompleted(), iVar1 != 0) {
        VSync(0);
    }
    return;
}

bool isBackBufferRamCopyEnabled = 0;

void reallocBackBufferRamCopy(void)

{
    if (isBackBufferRamCopyEnabled == 1) {
        assert(0);
    }
    return;
}

std::vector<u8> mechaOverlayBuffer;

std::array<sLoadingBatchCommands, 16> mechaOverlayBatchLoadingTable;

struct sMechaDataTable1_C {
    s16 m0;
    s16 m2;
};

struct sMechaDataTable1_sub4 {
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
    sMechaDataTable1_10_4 *m4;

    std::vector<u8> m_raw;
};

struct sMechaDataTable1 {
    sMechaDataTable1(std::vector<u8>& input) {
        m_raw = input;
        std::vector<std::vector<u8>> relocatedData = doPointerRelocationAndSplit(input);

        m8_modelBlocks.init(relocatedData[1].begin(), relocatedData[1].size());
        mC.resize(relocatedData[2].size() / 4);
        for (int i = 0; i < mC.size(); i++) {
            mC[i].m0 = READ_LE_S16(relocatedData[2].begin() + i * 4 + 0);
            mC[i].m2 = READ_LE_S16(relocatedData[2].begin() + i * 4 + 2);
        }

        m10 = new sMechaDataTable1_10(relocatedData[3]);

        return;
    }

    sMechaDataTable1_sub4* m4_textures;
    sModel m8_modelBlocks;
    std::vector<sMechaDataTable1_C> mC;
    sMechaDataTable1_10 *m10;

    std::vector<u8> m_raw;
};

std::array<sMechaDataTable1*, 9> mechaDataTable1;
std::array<std::vector<u8>, 9> mechaDataTable1_raw;

struct sMechaDataTable2 {
    sMechaDataTable2(const std::vector<u8>& input) { m_raw = input; }

    std::vector<u8> m_raw;
};

std::array<sMechaDataTable2*, 9> mechaDataTable2;
std::array<std::vector<u8>, 9> mechaDataTable2_raw;


void loadMechaOverlay_start(void)
{
    if (NumMechas == 0)
        return;

    waitForLoading();
    setCurrentDirectory(4, 0);
    reallocBackBufferRamCopy();
/*    if (mechaOverlayLoadingMode == 0) { // TODO: figure that one out
        sVar4 = (DAT_Field__800adb30 & 0xffffff) - 0x1dc008;
    }
    else*/ {
        mechaOverlayBuffer.resize(getFileSizeAligned(0x6b9));
    }

    reallocBackBufferRamCopy();

    for (int i = 0; i < NumMechas; i++) {
        assert(mechaDataTable1[i] == nullptr);
        mechaOverlayBatchLoadingTable[1 + i*2].m0_fileIndex = mechaList[i] + 0x6BB;
        mechaDataTable1_raw[i].resize(getFileSizeAligned(mechaList[i] + 0x6BB));
        mechaOverlayBatchLoadingTable[1 + i*2].m4_loadPtr = &mechaDataTable1_raw[i];
    }

    for (int i = 0; i < NumMechas; i++) {
        assert(mechaDataTable2[i] == nullptr);
        mechaOverlayBatchLoadingTable[i*2].m0_fileIndex = mechaList[i] + 0x6BA;
        mechaDataTable2_raw[i].resize(getFileSizeAligned(mechaList[i] + 0x6BA));
        mechaOverlayBatchLoadingTable[i*2].m4_loadPtr = &mechaDataTable2_raw[i];
    }

    mechaOverlayBatchLoadingTable[NumMechas * 2].m0_fileIndex = 0x6B9; // the overlay itself
    mechaOverlayBatchLoadingTable[NumMechas * 2].m4_loadPtr = &mechaOverlayBuffer;

    mechaOverlayBatchLoadingTable[NumMechas * 2 + 1].m0_fileIndex = 0;
    mechaOverlayBatchLoadingTable[NumMechas * 2 + 1].m4_loadPtr = nullptr;

    waitForLoading();
    batchStartLoadingFiles(&mechaOverlayBatchLoadingTable[0], 0);
}

int mechaInitVar0 = 0;
int mechaInitVar1 = 0;

struct sMechaInitVar4
{
    std::vector<sModelBlock*> m0;
    u32 m4;
};

std::array<sMechaInitVar4, 8> mechaInitVar4;

struct sMechaInitVar5
{
    s8 m6;
};

std::array<sMechaInitVar5, 2> mechaInitVar5;

struct sMechaInitVar2Sub {
    u8 m0;
    // size 0x14
};

struct sMechaInitVar2 {
    std::vector<sMechaInitVar2Sub> m0;
    s16 m4;
    s16 m6;
}mechaInitVar2;

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
}mechaInitVar3;

struct sLoadedMecha_sub4 {
    sLoadedMecha_sub4* m0_parentBone;
    u8 m4;
    u8 m5;
    u8 m6;
    u8 m7;
    s16 m8;
    s16 mA_numBones;
    std::array<s16, 3> m4C;
    s16 m52;
    std::array<s16, 3> m54;
    std::array<s32, 3> m5C;
    std::array<std::vector<sTag*>, 2> m68;
    std::array<s32, 3> m70;
    //size 0x7C;
};

struct sLoadedMechas {
    sMechaInitVar4* m0;
    std::vector<sLoadedMecha_sub4>*m4;
    s16 m1C;
    u8 m20_mechaEntryId;
    u8 m22;
    std::array<s16, 3> m24;
    u8 m2A;
    u8 m34;
    u16 m4A;
    u8 m62_isSeqLoaded;
    u8 m63;
    std::array<s16, 3> m90;
    s16 m96;
    sModel* mA8;
    sMechaDataTable2* mAC;
    std::array<POLY_FT4, 2> mB8_Polys;
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
    // size 0x134
};

std::array<sLoadedMechas*, 10> loadedMechas;
std::array<int, 10> mechaList3;
s16 numMaxMechaOverlayEntries;


void freeMechaInitVar2(sMechaInitVar2*) {
    MissingCode();
}

void freeMechaInitVar3(sMechaInitVar3*) {
    MissingCode();
}

void initMechaInitVar2Sub0(sMechaInitVar2* param_1) {
    if (param_1->m0.size()) {
        param_1->m4 = 0;
        for (int i = 0; i < param_1->m6; i++) {
            param_1->m0[i].m0 = 0;
        }
    }
}

void initMechaInitVar2(sMechaInitVar2* param_1, int param_2) {
    if (param_2 > 0) {
        param_1->m6 = param_2;
        initModel3(4, 0);
        param_1->m0.resize(param_2);
        if (param_1->m0.size()) {
            initMechaInitVar2Sub0(param_1);
        }
    }
}

void initMechaInitVar3Sub(sMechaInitVar3* param_1) {
    for (int i = 0; i < param_1->m4 + 1; i++) {
        param_1->m0[i].m16 = -1;
        param_1->m0[i].m1E = 0;
        for (int j = 0; j < 2; j++) {
            POLY_FT4* pPoly = &param_1->m0[i].m2C_polys[j];
            SetPolyFT4(pPoly);
            SetSemiTrans(pPoly, 1);
            pPoly->clut = GetClut(0, 0x1cd);
            pPoly->u2 = 0xF;
            pPoly->tpage = GetTPage(0, 1, 0x340, 0x100);
            pPoly->u0 = 0x0;
            pPoly->v0 = 0xbd;
            pPoly->u1 = 0x0;
            pPoly->v1 = 0xbd;
            pPoly->v2 = 0xbd;
            pPoly->u3 = 0xF;
            pPoly->v3 = 0xbd;
        }
    }
}

void initMechaInitVar3(sMechaInitVar3* param_1, int param_2) {
    initModel3(4, 0);
    param_1->m4 = param_2;
    param_1->m6 = 0;
    param_1->m0.resize(param_2 + 1);
    if (param_1->m0.size()) {
        initMechaInitVar3Sub(param_1);
    }
}

void mechaInitOverlay(int mechaMaxCount)
{
    mechaInitVar0 = 0;
    mechaInitVar1 = 0;
    initMechaInitVar2(&mechaInitVar2, mechaMaxCount);
    initMechaInitVar3(&mechaInitVar3, 0x10);

    loadedMechas.fill(nullptr);

    for (int i = 0; i < 8; i++) {
        mechaInitVar4[i].m0.clear();
    }

    for (int i = 0; i < 2; i++) {
        mechaInitVar5[i].m6 = 0;
    }
}

void SetBackColor(long rbk, long gbk, long bbk)
{
    setCopControlWord(2, 0x6800, rbk << 4);
    setCopControlWord(2, 0x7000, gbk << 4);
    setCopControlWord(2, 0x7800, bbk << 4);
    return;
}

std::array<u8, 3> mechaBackColor = { 0,0,0 };

std::array<SFP_VEC4, 4> initMechaTempVar;

void loadMechaTextures(sMechaDataTable1_sub4* param_1, short param_2, short tpageX, short tpageY, short param_5, short clutX, short clutY)
{
    MissingCode();
}

void sMechaModel_init(sModel& modelBlock, sMechaInitVar4* param_2)
{
    param_2->m4 = modelBlock.m0_numBlocks;
    param_2->m0.resize(modelBlock.m0_numBlocks);

    for (int i = 0; i < param_2->m4; i++) {
        param_2->m0[i] = &modelBlock.m10_blocks[i];
    }
}

void initMechSub110(sLoadedMechas* param_1)
{
    if (param_1->m10C) {
        param_1->m110.resize(param_1->m10C);
        for (int i = 0; i < param_1->m10C; i++) {
            param_1->m110[i].m0 = -1;
            param_1->m110[i].m8 = 0;
        }
    }
}

std::vector<sLoadedMecha_sub4>* processMechaMesh(sMechaInitVar4* param_1, std::vector<sMechaDataTable1_C>& param_2, u32 param_3, int useTpageAndClut, short tpageX, short tpageY, short clutX, short clutY)
{
    int numBones = 0;
    std::vector<sMechaDataTable1_C>::iterator bonesIt = param_2.begin();
    while ((bonesIt->m0 < param_1->m4) || (bonesIt->m0 == -1)) {
        bonesIt++;
        numBones++;
    }

    if(numBones == 0) {
        return nullptr;
    }

    std::vector<sLoadedMecha_sub4>* outputVector = new std::vector<sLoadedMecha_sub4>;
    outputVector->resize(numBones + 1);

    std::vector<sLoadedMecha_sub4>::iterator it = outputVector->begin();

    it->m4 = 1;
    it->m5 = 1;
    it->m6 = 1;
    it->m4C[0] = 0x1000;
    it->m4C[1] = 0x1000;
    it->m4C[2] = 0x1000;
    it->m0_parentBone = nullptr;
    it->m7 = 0;
    it->m8 = -1;
    it->mA_numBones = numBones + 1;
    it->m68[0].clear();
    it->m68[1].clear();
    it->m54[0] = 0;
    it->m54[1] = 0;
    it->m54[2] = 0;
    it->m5C[0] = 0;
    it->m5C[1] = 0;
    it->m5C[2] = 0;
    it->m70[0] = 0;
    it->m70[1] = 0;
    it->m70[2] = 0;
    it++;

    std::vector<sMechaDataTable1_C>::iterator it2 = param_2.begin();

    int boneCounter = 1;

    while (true)
    {
        if ((it2->m0 >= param_1->m4) && (it2->m0 != -1))
            break;

        if (it2->m2 == -1) {
            it->m0_parentBone = nullptr;
        }
        else {
            it->m0_parentBone = &(*outputVector)[it2->m2 + 1];
        }

        it->mA_numBones = boneCounter++;
        it->m4 = 1;
        it->m5 = 1;
        it->m7 = 1;
        it->m4C[0] = 0x1000;
        it->m4C[1] = 0x1000;
        it->m4C[2] = 0x1000;
        it->m6 = 0;
        it->m52 = 0;
        it->m8 = it2->m0;
        if (it2->m0 == -1) {
            it->m68[0].clear();
            it->m68[1].clear();
            it->m54[0] = 0;
        }
        else {
            initModel1(*param_1->m0[it2->m0], it->m68[0], it->m68[1]);
            assert(it->m68[0].size());
            if (useTpageAndClut) {
                MissingCode();
            }
            initModel2(param_1->m0[it2->m0], it->m68[0], param_3);
            it->m68[1] = it->m68[0];
            it->m54[0] = 0;
        }
        it->m54[1] = 0;
        it->m54[2] = 0;
        it->m5C[0] = 0;
        it->m5C[1] = 0;
        it->m5C[2] = 0;
        it->m70[0] = 0;
        it->m70[1] = 0;
        it->m70[2] = 0;

        it++;
        it2++;
    }
    return outputVector;
}

void mechaInitNewMecha(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9)
{
    initModel3(4, 0);
    if (9 < entryId) {
        return;
    }
    if (loadedMechas[entryId] != nullptr) {
        return;
    }

    sLoadedMechas* pLoadedMecha = new sLoadedMechas;
    if ((flags & 1) == 0) {
        /*
        doPointerRelocation((uint*)pData1);
        doPointerRelocation((uint*)pData1->m10);
        */
    }
    pLoadedMecha->m62_isSeqLoaded = 0;
    pLoadedMecha->m63 = 0;
    if ((flags & 4) == 0) {
/*
        doPointerRelocation((uint*)param_3);
        doPointerRelocation(*(uint**)(param_3 + 8));
        puVar12 = *(uint**)(param_3 + 4);
        doPointerRelocation(puVar12);
        doPointerRelocation((uint*)puVar12[1]);
        */

#if 0
        MissingCode();
        psVar9 = pData2->m8;
        if ((psVar9->mC != psVar9->m8_seq) && (iVar3 = isSeqValid(psVar9->m8_seq, 0), iVar3 == 0)) {
            startSeq(psVar9->m8_seq);
            pLoadedMecha->m62 = 1;
        }
#endif
    }

    loadedMechas[entryId] = pLoadedMecha;
    pLoadedMecha->m24[0] = pData1->m10->m4->m2[0];
    pLoadedMecha->m24[1] = pData1->m10->m4->m2[1];
    pLoadedMecha->m24[2] = pData1->m10->m4->m2[2];

    pLoadedMecha->m2A = pData1->m10->m4->mA;
    pLoadedMecha->m4A = pData1->m10->m4->mC;

    if (pLoadedMecha->m4A & 0x200) {
        MissingCode();
        //patchSelfModifyingCode(2, 2, 0x40, 0x40);
    }

    sModel mechaModelBlocksBufferForLoading;

    int mechaInitVar4Counter = 0;
    if ((flags & 1) == 0) {
        int uVar29 = 0;
        if ((flags & 0x40) == 0) {
            uVar29 = ((pLoadedMecha->m4A & 4) == 0);
        }
        loadMechaTextures(pData1->m4_textures, uVar29, tpageX, tpageY, uVar29, clutX, clutY);
        mechaModelBlocksBufferForLoading = pData1->m8_modelBlocks;

        int iVar2;
        do {
            iVar2 = mechaInitVar4Counter + 1;
            if (mechaInitVar4[mechaInitVar4Counter].m0.size() == 0)
                break;
            mechaInitVar4Counter = iVar2;
        } while (iVar2 < 8);

        sMechaModel_init(mechaModelBlocksBufferForLoading, &mechaInitVar4[mechaInitVar4Counter]);
    }

    pLoadedMecha->m0 = &mechaInitVar4[mechaInitVar4Counter];


    if ((flags & 0x40) == 0) {
        if ((pLoadedMecha->m4A & 4) == 0) {
            pLoadedMecha->m4 = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 1, tpageX, tpageY, clutX, clutY);

        }
        else
        {
            pLoadedMecha->m4 = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 0, 0, 0, 0, 0);
        }
    }
    else
    {
        pLoadedMecha->m4 = processMechaMesh(pLoadedMecha->m0, pData1->mC, 0, 0, 0, 0, 0, 0);
    }

    if (param_9 != nullptr) {
        (*pLoadedMecha->m4)[0].m5C[0] = param_9->vx;
        (*pLoadedMecha->m4)[0].m5C[1] = param_9->vy;
        (*pLoadedMecha->m4)[0].m5C[2] = param_9->vz;
    }

    if ((flags & 1) == 0) {
        pLoadedMecha->m90[0] = tpageX;
        pLoadedMecha->m90[1] = tpageY;
        pLoadedMecha->m90[2] = clutX;
        pLoadedMecha->m96 = clutY;
    }
    else {
        pLoadedMecha->m90[0] = -1;
    }

    if (((flags & 4) == 0) && ((flags & 0x80) == 0)) {
        pLoadedMecha->mAC = pData2;
    }
    else {
        pLoadedMecha->mAC = nullptr;
    }

    for (int i = 0; i < 2; i++)
    {
        POLY_FT4* p = &pLoadedMecha->mB8_Polys[i];
        SetPolyFT4(p);
        SetSemiTrans(p, 1);
        p->r0 = 0x40;
        p->g0 = 0x40;
        p->b0 = 0x40;
        p->clut = GetClut(0x100, 0xF3);
        p->tpage = GetTPage(0, 2, 0x280, 0x100);
        p->u0 = 0x00;
        p->v0 = 0xE0;
        p->u1 = 0x0F;
        p->v1 = 0xE0;
        p->u2 = 0x00;
        p->v2 = 0xEF;
        p->u1 = 0x0F;
        p->v1 = 0xEF;
    }

    pLoadedMecha->m1C = pData1->m10->m4->m8;
    pLoadedMecha->m10C = pData1->m10->m4->mE;
    initMechSub110(pLoadedMecha);
    pLoadedMecha->m10E = pData1->m10->m4->m10;

    if (pLoadedMecha->m10E != 0) {
        pLoadedMecha->m118.resize(pLoadedMecha->m10E);
        for(int i=0; i<pLoadedMecha->m10E; i++) {
            pLoadedMecha->m118[i].m1A = 0;
            pLoadedMecha->m118[i].m4 = 0;
            pLoadedMecha->m118[i].m8 = 0;
            pLoadedMecha->m118[i].mC = 0;
        }
    }

    pLoadedMecha->m10D = pData1->m10->m4->m12;
    if (pLoadedMecha->m10D != 0) {
        assert(0);
    }

    pLoadedMecha->m22 = 0;
    pLoadedMecha->m34 = 1;
    pLoadedMecha->m20_mechaEntryId = entryId;

    if ((flags & 0x40) == 0) {
        MissingCode();
    }

    if ((flags & 2) == 0) {
        pLoadedMecha->mA8 = new sModel;
        *pLoadedMecha->mA8 = mechaModelBlocksBufferForLoading;
    }
    else
    {
        pLoadedMecha->mA8 = nullptr;
    }
}

void loadMechaOverlay_finalize(void)
{
    if (NumMechas == 0)
        return;

    waitForLoading();
    ClearCacheAfterOverlayLoad();
    mechaInitOverlay(numMaxMechaOverlayEntries);

    MissingCode();

    SetBackColor(mechaBackColor[0], mechaBackColor[1], mechaBackColor[2]);

    for (int i = 0; i < NumMechas; i++) {
        initMechaTempVar[i].vx = 0;
        initMechaTempVar[i].vy = 0;
        initMechaTempVar[i].vz = 0;

        mechaDataTable2[i] = new sMechaDataTable2(mechaDataTable2_raw[i]);
        mechaDataTable1[i] = new sMechaDataTable1(mechaDataTable1_raw[i]);

        mechaInitNewMecha(i, 0, mechaDataTable2[i], mechaDataTable1[i], ((i + mechaList2[i]) * -0x40 + 0x240), 0x100, 0, i + 0xFC, &initMechaTempVar[i]);
        delete mechaDataTable1[i];
        mechaList3[i] = loadedMechas[i]->m1C;
    }

    initModel3(8, 0);
}

void loadMechaOVerlayAndRegisterMechas(void)
{
    loadMechaOverlay_start();
    loadMechaOverlay_finalize();
}

void freeMecha(int index) {
    MissingCode();
}

void freeMechaModule()
{
    for (int i = 0; i < 10; i++) {
        freeMecha(i);
    }
    freeMechaInitVar2(&mechaInitVar2);
    freeMechaInitVar3(&mechaInitVar3);
}