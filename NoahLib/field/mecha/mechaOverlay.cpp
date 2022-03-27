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

struct sMechaDataTable1 {

};

std::array<sMechaDataTable1, 9> mechaDataTable1;
std::array<std::vector<u8>, 9> mechaDataTable1_raw;

struct sMechaDataTable2 {

};

std::array<sMechaDataTable2, 9> mechaDataTable2;
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
        mechaOverlayBatchLoadingTable[1 + i*2].m0_fileIndex = mechaList[i] + 0x6BB;
        mechaDataTable1_raw[i].resize(getFileSizeAligned(mechaList[i] + 0x6BB));
        mechaOverlayBatchLoadingTable[1 + i*2].m4_loadPtr = &mechaDataTable1_raw[i];
    }

    for (int i = 0; i < NumMechas; i++) {
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

struct sLoadedMechas {

    // size 0x134
};

std::array<sLoadedMechas*, 10> loadedMechas;

s16 numMaxMechaOverlayEntries;

struct sMechaInitVar4
{
    void* m0;
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
        mechaInitVar4[i].m0 = nullptr;
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

void mechaInitNewMecha(int entryId, ushort param_2, sMechaDataTable2* param_3, sMechaDataTable1* param_4, ushort param_5, ushort param_6, ushort param_7, short param_8, SFP_VEC3* param_9)
{
    initModel3(4, 0);
    if (9 < entryId) {
        return;
    }
    if (loadedMechas[entryId] != nullptr) {
        return;
    }

    sLoadedMechas* pLoadedMecha = new sLoadedMechas;
    if ((param_2 & 1) == 0) {
        //doPointerRelocation(param_4);
        //doPointerRelocation((uint*)param_4[4]);
    }
    //pLoadedMecha->m62 = 0;
    //pLoadedMecha->m63 = 0;
    if ((param_2 & 4) == 0) {
/*
        doPointerRelocation((uint*)param_3);
        doPointerRelocation(*(uint**)(param_3 + 8));
        puVar12 = *(uint**)(param_3 + 4);
        doPointerRelocation(puVar12);
        doPointerRelocation((uint*)puVar12[1]);
        iVar9 = *(int*)(param_3 + 8);
        if ((*(int*)(iVar9 + 0xc) != *(int*)(iVar9 + 8)) && (iVar3 = FUN_8003864c(*(int*)(iVar9 + 8), 0), iVar3 == 0)) {
            startSeq(*(undefined4*)(iVar9 + 8));
            pLoadedMecha->m62 = 1;
        }*/
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

        //mechaInitNewMecha(i, 0, mechaDataTable2[i], mechaDataTable1[i], ((i + mechaList2[i]) * -0x40 + 0x240), 0x100, 0, i + 0xFC, &initMechaTempVar[i]);
        //mechaDataTable1[i].clear();
        MissingCode();
        //mechaList3[i2] = (undefined*)(int)*(short*)((&loadedMechas)[i2] + 0x1c);
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