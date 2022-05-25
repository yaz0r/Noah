#include "noahLib.h"
#include "mechaOverlay.h"
#include "field/field.h"
#include "kernel/filesystem.h"
#include "kernel/gte.h"
#include "bx/math.h"
#include "field/fieldGraphicObject.h"
#include "kernel/memory.h"

u32 NumMechas;
std::array<s16, 4> mechaList;
std::array<u8, 4> mechaList2;

s16 mechaPlayAnimationVar0 = 0;
s16 mechaPlayAnimationVar1 = 0;

std::array<s16, 4> mechaCurrentAnimation;
u32 mechaBytecodeToggle = 0;

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
    sMechaDataTable1_10_4 *m4;

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
    sMechaDataTable1_10 *m10;

    std::vector<u8> m_raw;
};

std::array<sMechaDataTable1*, 9> mechaDataTable1;
std::array<std::vector<u8>, 9> mechaDataTable1_raw;

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
        //assert(mechaDataTable2[i] == nullptr);
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

std::array<sMechaInitVar4, 8> mechaInitVar4;

struct sMechaInitVar5
{
    s8 m6;
};

std::array<sMechaInitVar5, 2> mechaInitVar5;

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
        resetMemoryAllocStats(4, 0);
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
    resetMemoryAllocStats(4, 0);
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
std::array<MATRIX, 2> mechaFieldArgs2;

std::array<SFP_VEC4, 4> initMechaTempVar;

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
    it->m54_rotationAngles[0] = 0;
    it->m54_rotationAngles[1] = 0;
    it->m54_rotationAngles[2] = 0;
    it->m5C_translation[0] = 0;
    it->m5C_translation[1] = 0;
    it->m5C_translation[2] = 0;
    it->m70_animTracks[0] = 0;
    it->m70_animTracks[1] = 0;
    it->m70_animTracks[2] = 0;
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
        }
        else {
            initModel1(*param_1->m0[it2->m0], it->m68[0], it->m68[1]);
            assert(it->m68[0].size());
            if (useTpageAndClut) {
                setupOverrideTPage(tpageX, tpageY);
                setupOverrideClut(clutX, clutY);
            }
            initModel2(param_1->m0[it2->m0], it->m68[0], param_3);
            it->m68[1] = it->m68[0];
        }
        it->m54_rotationAngles[0] = 0;
        it->m54_rotationAngles[1] = 0;
        it->m54_rotationAngles[2] = 0;
        it->m5C_translation[0] = 0;
        it->m5C_translation[1] = 0;
        it->m5C_translation[2] = 0;
        it->m70_animTracks[0] = 0;
        it->m70_animTracks[1] = 0;
        it->m70_animTracks[2] = 0;

        it++;
        it2++;
    }
    return outputVector;
}

int mechaOP_A_sub0(sMechaInitVar2* param_1, sMechaInitVar2Sub* param_2)
{
    int uVar1;

    if (param_2 == nullptr) {
        uVar1 = -1;
    }
    else {
        uVar1 = (uint)(param_2 - &param_1->m0[0]);
        if ((uint)uVar1 < (uint)(ushort)param_1->m4) {
            param_1->m4 = (short)uVar1;
        }
        param_2->m0 = 0;
    }
    return uVar1;
}

void mechaOP_8(sMechaInitVar2* param_1, std::vector<sLoadedMecha_sub4>* param_2) {
    for (int i = 0; i < (*param_2)[0].mA_numBones; i++) {
        sLoadedMecha_sub4* pEntry = &(*param_2)[i];
        if (pEntry->m70_animTracks[0] && (pEntry->m70_animTracks[0]->m3 != -1)) {
            mechaOP_A_sub0(param_1, pEntry->m70_animTracks[0]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != -1)) {
            mechaOP_A_sub0(param_1, pEntry->m70_animTracks[1]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != -1)) {
            mechaOP_A_sub0(param_1, pEntry->m70_animTracks[1]);
        }
    }
}

void mechaOP_A(sMechaInitVar2* param_1, sLoadedMecha_sub4* param_2, int param_3, uint param_4) {
    if (param_3 < (int)(uint)(ushort)param_2->mA_numBones) {
        if ((param_2[param_3].m70_animTracks[0] != 0) && ((param_4 & 1) != 0)) {
            mechaOP_A_sub0(param_1, param_2[param_3].m70_animTracks[0]);
            param_2[param_3].m70_animTracks[0] = 0;
        }
        if ((param_2[param_3].m70_animTracks[1] != 0) && ((param_4 & 2) != 0)) {
            mechaOP_A_sub0(param_1, param_2[param_3].m70_animTracks[1]);
            param_2[param_3].m70_animTracks[1] = 0;
        }
        if ((param_2[param_3].m70_animTracks[2] != 0) && ((param_4 & 4) != 0)) {
            mechaOP_A_sub0(param_1, param_2[param_3].m70_animTracks[2]);
            param_2[param_3].m70_animTracks[2] = 0;
        }
    }
}

void mechaOP_10_b(std::vector<sLoadedMecha_sub4>& param_1, sMechaDataTable2_4_4* param_2)
{
    std::vector<std::array<s16, 3>>::iterator pShortData = param_2->m18.begin();
    if (param_2->m6 == 0) {
        pShortData += param_2->mC_count0 + 1;
    }

    int counter0 = 0;
    int counter1 = 0;

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        sLoadedMecha_sub4* pBone = &param_1[i];

        if (((param_2->m4_flags & 1) == 0) && (counter0 < param_2->mC_count0)) {
            std::array<s16, 3> sVar1 = *pShortData;
            pShortData++;
            counter0++;

            if ((pBone->m4C[0] != sVar1[0]) || (pBone->m4C[1] != sVar1[1]) || (pBone->m4C[2] != sVar1[2])) {
                if ((pBone->m68[0].size() == 0) || (pBone->m68[0][0]->m3_size != -1))
                {
                    pBone->m54_rotationAngles[0] = sVar1[0];
                    pBone->m54_rotationAngles[1] = sVar1[1];
                    pBone->m54_rotationAngles[2] = sVar1[2];
                    pBone->m4 = 1;
                    pBone->m5 = 1;
                }
            }
        }

        if (((param_2->m4_flags & 2) == 0) && (counter0 < param_2->mE_count1)) {
            std::array<s16, 3> sVar1 = *pShortData;
            pShortData++;
            counter1++;

            if ((pBone->m5C_translation[0] != sVar1[0]) || (pBone->m5C_translation[1] != sVar1[1]) || (pBone->m5C_translation[2] != sVar1[2])) {
                if ((pBone->m68[1].size() == 0) || (pBone->m68[1][0]->m3_size != -1))
                {
                    pBone->m5C_translation[0] = sVar1[0];
                    pBone->m5C_translation[1] = sVar1[1];
                    pBone->m5C_translation[2] = sVar1[2];
                    pBone->m4 = 1;
                }
            }
        }
    }
}

sMechaDataTable2_4_4* mechaOP_10_a(sLoadedMechas* param_1, uint param_2, u32* param_3)
{
    *param_3 = 0;
    if (0xfd < (param_2 & 0xff)) {
        param_2 = param_1->m2A[0] & 0x7f;
        *param_3 = param_1->m2A[0] & 0x80;
    }
    int iVar1 = (param_2 & 0xff);
    if ((param_2 & 0xff) < 0x40) {
        return &param_1->m14->m4[iVar1];
    }
    else {
        assert(0);
        //uVar2 = *(undefined4*)(param_1->m18 + iVar1 + -0xfc);
        return nullptr;
    }
}

void mechaAnimOp23(sLoadedMechas* param_1, sLoadedMecha_sub4* param_2, u32 param_3) {
    param_2->m7 = param_3 & 1;
    if ((param_3 & 0x80) != 0) {
        std::vector<sLoadedMecha_sub4>::iterator psVar1 = (*param_1->m4).begin();
        int iVar2 = 1;
        if (1 < psVar1->mA_numBones) {
            do {
                psVar1 = psVar1 + 1;
                iVar2 = iVar2 + 1;
                if (psVar1->m0_parentBone == param_2) {
                    mechaAnimOp23(param_1, &(*psVar1), param_3);
                }
            } while (iVar2 < (*param_1->m4)[0].mA_numBones);
        }
    }
}

void mechaOP_13(sMechaInitVar2* param_1, std::vector<sLoadedMecha_sub4>& param_2, sMechaDataTable2_4_4* param_3, int param_4, byte param_5, byte param_6, char param_7) {
    MissingCode();
}

void mechaOP_19(sLoadedMechas* param_1)
{
    param_1->m98 = -1;
    return;
}

void processMechaAnimData(sLoadedMechas* param_1, sMechaInitVar2* param_2, int param_3, int param_4)
{
    if (param_4 == 0) {
        return;
    }
    if (param_1->m10_bytecode0 == 0) {
        return;
    }

    resetMemoryAllocStats(4, 0);

    // step the mecha forward?
    for (int i = 0; i < param_4; i++) {
        param_1->m70[0] += param_1->m76[0];
        param_1->m70[1] += param_1->m76[1];
        param_1->m70[2] += param_1->m76[2];

        param_1->m7C[0] += param_1->m82[0];
        param_1->m7C[1] += param_1->m82[1];
        param_1->m7C[2] += param_1->m82[2];

        (*param_1->m4)[0].m54_rotationAngles[0] += param_1->m70[0] >> 3;
        (*param_1->m4)[0].m54_rotationAngles[1] += param_1->m70[1] >> 3;
        (*param_1->m4)[0].m54_rotationAngles[2] += param_1->m70[2] >> 3;

        SFP_VEC4 local_120;
        local_120.vx = ((int)param_1->m7C[0] * (int)(*param_1->m4)[0].m4C[0] >> 0xc);
        local_120.vy = ((int)param_1->m7C[1] * (int)(*param_1->m4)[0].m4C[1] >> 0xc);
        local_120.vz = ((int)param_1->m7C[2] * (int)(*param_1->m4)[0].m4C[2] >> 0xc);

        FP_VEC4 local_130;
        rotateVectorByMatrix(&(*param_1->m4)[0].m2C, &local_120, &local_130);

        (*param_1->m4)[0].m5C_translation[0] += (param_1->m1C_moveSpeed * local_130.vx >> 0xc);
        (*param_1->m4)[0].m5C_translation[1] += (param_1->m1C_moveSpeed * local_130.vy >> 0xc);
        (*param_1->m4)[0].m5C_translation[2] += (param_1->m1C_moveSpeed * local_130.vz >> 0xc);
    }

    bool continueBytecodeExecution = true;
    sMechaDataTable2_4_8* pNextBytecode = param_1->m10_bytecode0;

    while (1) {
        sMechaDataTable2_4_8* pCurrentByteCodePtr = pNextBytecode;
        if (!continueBytecodeExecution) {
            param_1->m10_bytecode0 = pCurrentByteCodePtr;
            MissingCode();
            return;
        }

        u16 _currentByteCode = *pCurrentByteCodePtr;
        pNextBytecode = pCurrentByteCodePtr + 1;

        u8 bytecodeLower = _currentByteCode & 0xFF;
        u16 bytecodeHigher = (_currentByteCode >> 8) & 0xFF;

        u32 local_100;

        switch (_currentByteCode & 0xFF) {
        case 0:
            continueBytecodeExecution = 0;
            pNextBytecode = pCurrentByteCodePtr;
            break;
        case 1:
            if (param_3 != -1) {
                _currentByteCode = *pNextBytecode;
                param_1->m40 += param_3;
                if (_currentByteCode < param_1->m40) {
                    param_1->m40 = 0;
                    param_3 = 0;
                    pNextBytecode = pCurrentByteCodePtr + 2;
                    break;
                }
            }
            continueBytecodeExecution = 0;
            break;
        case 0x8:
            mechaOP_8(param_2, param_1->m4);
            break;
        case 0xA:
            mechaOP_A(param_2, &(*param_1->m4)[0], bytecodeHigher, 7);
            break;
        case 0xC:
            param_1->m70[0] = 0;
            param_1->m70[1] = 0;
            param_1->m70[2] = 0;
            param_1->m76[0] = 0;
            param_1->m76[1] = 0;
            param_1->m76[2] = 0;
            param_1->m7C[0] = 0;
            param_1->m7C[1] = 0;
            param_1->m7C[2] = 0;
            param_1->m82[0] = 0;
            param_1->m82[1] = 0;
            param_1->m82[2] = 0;
            break;
        case 0xF:
            break; // TODO: really?
        case 0x10:
            mechaOP_10_b(*param_1->m4, mechaOP_10_a(param_1, bytecodeHigher, &local_100));
            break;
        case 0x11:
            {
                u32 local_100;
                sMechaDataTable2_4_4* pbVar8 = mechaOP_10_a(param_1, bytecodeHigher, &local_100);
                _currentByteCode = *pNextBytecode;
                pNextBytecode = pCurrentByteCodePtr + 2;
                if (local_100 == 0) {
                    MissingCode();
                }
            }
            break;
        case 0x13:
            {
                u8 previousByteCode = _currentByteCode;
                u8 preBytecodeHigher = bytecodeHigher;
                bytecodeHigher = *pNextBytecode;
                _currentByteCode = pCurrentByteCodePtr[2];
                pNextBytecode = pCurrentByteCodePtr + 3;
                sMechaDataTable2_4_4* psVar7 = mechaOP_10_a(param_1, bytecodeHigher, &local_100);
                if (mechaBytecodeToggle == 0) {
                    mechaOP_13(param_2, *param_1->m4, psVar7, _currentByteCode >> 8, preBytecodeHigher, previousByteCode, bytecodeHigher >> 8);
                }
                else {
                    mechaOP_10_b(*param_1->m4, psVar7);
                }
                param_3 = 0xffffffff;
                mechaOP_19(param_1);
            }
            break;
        case 0x1D:
            MissingCode();
            pNextBytecode = pCurrentByteCodePtr + 10;
            break;
        case 0x21:
            param_1->m3C = bytecodeHigher;
            if (param_3 != -1) {
                if (bytecodeHigher == 0)
                    break;
            }
            continueBytecodeExecution = 0;
            pNextBytecode = pCurrentByteCodePtr;
            break;
        case 0x23:
            mechaAnimOp23(param_1, &(*param_1->m4)[*pNextBytecode], bytecodeHigher);
            pNextBytecode = pCurrentByteCodePtr + 2;
            break;
        case 0x48:
            param_1->m36 = bytecodeHigher;
            break;
        case 0x62:
            MissingCode();
            pNextBytecode = pCurrentByteCodePtr + 5;
            break;
        default:
            assert(0);
        }
    }
}

void initMechaAnimation(sLoadedMechas* param_1, sLoadedMechas* param_2, sMechaInitVar2* param_3, int animationId)
{
    if ((param_1 != (sLoadedMechas*)0x0) && (param_2 != (sLoadedMechas*)0x0)) {
        if (param_1->m2A[1] == 0) {
            if (animationId < 0x50) {
                param_1->m10_bytecode0 = &(*param_2->m8_bytecodeTable)[animationId][0];
            }
            else {
                assert(0);
            }
            param_1->m42 = 0;
            param_1->m40 = 0;
            param_1->m50_bytecode2 = nullptr;
            param_1->m54_bytecode1 = nullptr;
            param_1->m4C_bytecode3 = nullptr;
            param_1->m23 = 0;
            param_1->m10A = mechaPlayAnimationVar0;
            processMechaAnimData(param_1, param_3, -1, 1);
        }
        else {
            if (param_1->m2A[1] < 5) {
                param_1->m2A[1] = param_1->m2A[1] + 1;
            }
            param_1->m2A[param_1->m2A[1]] = param_2->m20_mechaEntryId;
            param_1->m2A[param_1->m2A[1] + 4] = animationId;
        }
    }
}

void initMechaTransforms1(sLoadedMechas* param_1, sMechaInitVar2* param_2, std::vector<std::vector<sMechaDataTable2_4_8>>* param_3, sMechaDataTable2_4::sMechaDataTable2_4_4_array* param_4)
{
    param_1->m3C = -1;
    param_1->m5C = -1;
    param_1->m39 = 0x6b;
    param_1->m8_bytecodeTable = param_3;
    param_1->mC = 0;
    param_1->m10_bytecode0 = nullptr;
    param_1->m14 = param_4;
    param_1->m18 = 0;
    param_1->m2A[1] = 0;
    param_1->m98 = -1;
    param_1->m58 = 0;
    param_1->m35 = 0;
    param_1->m37 = 0;
    param_1->m38 = 0;
    param_1->m3A = -1;
    param_1->m70[0] = 0;
    param_1->m70[1] = 0;
    param_1->m70[2] = 0;
    param_1->m76[0] = 0;
    param_1->m76[1] = 0;
    param_1->m76[2] = 0;
    param_1->m7C[0] = 0;
    param_1->m7C[1] = 0;
    param_1->m7C[2] = 0;
    param_1->m82[0] = 0;
    param_1->m82[1] = 0;
    param_1->m82[2] = 0;
    param_1->m88[0] = 0;
    param_1->m88[1] = 0;
    param_1->m88[2] = 0;
    param_1->m8E = 1;
    param_1->m36 = 0;
    param_1->m1E = -1;
}

void mechaInitNewMecha(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SFP_VEC3* param_9)
{
    resetMemoryAllocStats(4, 0);
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

    pLoadedMecha->m2A[0] = pData1->m10->m4->mA;
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

        // TODO: convert that properly
        sPS1Pointer temp;
        temp.setPointer(&pData1->m4_textures.m_raw[0]);
        uploadTextureToVram(temp, uVar29, tpageX, tpageY, uVar29, clutX, clutY);
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
        (*pLoadedMecha->m4)[0].m5C_translation[0] = param_9->vx;
        (*pLoadedMecha->m4)[0].m5C_translation[1] = param_9->vy;
        (*pLoadedMecha->m4)[0].m5C_translation[2] = param_9->vz;
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

    pLoadedMecha->m1C_moveSpeed = pData1->m10->m4->m8;
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
        pLoadedMecha->mB0 = &pData2->m8;
        initMechaTransforms1(pLoadedMecha, &mechaInitVar2, &pData2->m4.m8, &pData2->m4.m4);
        initMechaAnimation(pLoadedMecha, pLoadedMecha, &mechaInitVar2, 0);
    }

    if ((flags & 2) == 0) {
        pLoadedMecha->mA8 = new sModel;
        *pLoadedMecha->mA8 = mechaModelBlocksBufferForLoading;
        // Fixup
        for (int i = 0; i < pLoadedMecha->mA8->m0_numBlocks; i++) {
            pLoadedMecha->mA8->m10_blocks[i].m_baseItForRelocation = &pLoadedMecha->mA8->mRawData[0];
        }

        sMechaModel_init(*pLoadedMecha->mA8, pLoadedMecha->m0);
        MissingCode();
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
        mechaDataTable1[i] = nullptr;
        mechaDataTable1_raw[i].clear();
        mechaList3[i] = loadedMechas[i]->m1C_moveSpeed;
    }

    resetMemoryAllocStats(8, 0);
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

bool disableMechaRendering = false;

void updateMechasPosForField()
{
    MissingCode();
}

s16 gMechaAngle;

MATRIX matrixSP_1f800040;
MATRIX matrixSP_1f800020;
MATRIX matrixSP_1f800000;

void submitMechaForRendering(sLoadedMechas* pMecha, MATRIX* pMatrix, MATRIX* param_3, int param_4, int param_5, OTTable& OT, int oddOrEven)
{
    if (pMecha->m34 != 0) {
        std::vector<sLoadedMecha_sub4>::iterator m4It = (*pMecha->m4).begin();

        CompMatrix(pMatrix, &m4It->mC, &matrixSP_1f800040);
        if ((pMecha->m4A & 1) == 0) {
            MissingCode();
        }
        MulMatrix0(param_3, &m4It->m2C, &matrixSP_1f800020);
        int numBones = m4It->mA_numBones;
        m4It++;
        for (int i = 1; i < numBones; i++) {
            if ((m4It->m8 != -1) && (m4It->m7)) {
                MulMatrix0(&matrixSP_1f800020, &m4It->m2C, &matrixSP_1f800000);
                //SetLightMatrix(&matrixSP_1f800000);
                CompMatrix(&matrixSP_1f800040, &m4It->m2C, &matrixSP_1f800000);
                MissingCode();
                SetTransMatrix(&matrixSP_1f800000);
                SetRotMatrix(&matrixSP_1f800000);
                submitModelForRendering(pMecha->m0->m0[m4It->m8], m4It->m68[oddOrEven], OT, param_4);
            }
            m4It++;
        }

        if (pMecha->m10D) {
            MissingCode();
        }

        if (pMecha->m10E) {
            MissingCode();
        }

        if (pMecha->m10C) {
            MissingCode();
        }
    }
}

void updateMechasBaseMatrixIsAttached(sLoadedMechas* param_1)
{
    MATRIX scratchPadMatrix;
    MATRIX* m0 = &scratchPadMatrix;

    if (loadedMechas[param_1->m5C] == nullptr) {
        param_1->m5C = -1;
        return;
    }
    else {
        if ((param_1->m4A & 0x10) == 0) {
            param_1->m34 = loadedMechas[param_1->m5C]->m34;
        }
        if (param_1->m34) {
            if (param_1->m5D) {
                if (param_1->m5E == 0) {
                    m0 = &(*loadedMechas[param_1->m5C]->m4)[0].m2C;
                }
                else {
                    MulMatrix0(&(*loadedMechas[param_1->m5C]->m4)[0].m2C, &(*loadedMechas[param_1->m5C]->m4)[param_1->m5E].m2C, &scratchPadMatrix);
                }
                MulRotationMatrix(m0, &(*param_1->m4)[0].mC);
                MulRotationMatrix(m0, &(*param_1->m4)[0].m2C);
            }
            if (param_1->m5E == 0) {
                m0 = &(*loadedMechas[param_1->m5C]->m4)[0].mC;
            }
            else {
                CompMatrix(&(*loadedMechas[param_1->m5C]->m4)[0].mC, &(*loadedMechas[param_1->m5C]->m4)[param_1->m5E].m2C, m0);
            }

            MissingCode();
            /*
            SetRotMatrix(m0);
            SetTransMatrix(m0);
            gte_ldv0();
            gte_rt();
            gte_stlvnl();
            uVar1 = gte_stlvnl();
            */
        }
    }
}

void updateMechAnimSub0(sLoadedMechas* param_1)
{
    if (param_1->m36 == 0) {
        (*param_1->m4)[0].m5C_translation[1] = param_1->m60;
    }
    return;
}

void RotMatrixYXZ(SFP_VEC4* r, MATRIX* m)
{
    MissingCode();
    return createRotationMatrix(r, m);
}

void updateMechAnimSub1(std::vector<sLoadedMecha_sub4>& param_1, int param_2) {
    param_1[0].m2C.t[0] = param_1[0].m5C_translation[0];
    param_1[0].m2C.t[1] = param_1[0].m5C_translation[1];
    param_1[0].m2C.t[2] = param_1[0].m5C_translation[2];

    if (param_1[0].m6 == 0) {
        createRotationMatrix(&param_1[0].m54_rotationAngles, &param_1[0].m2C);
    }
    else {
        RotMatrixYXZ(&param_1[0].m54_rotationAngles, &param_1[0].m2C);
    }

    MATRIX MATRIX_1f800000;
    MATRIX_1f800000.m[0][1] = 0;
    MATRIX_1f800000.m[0][2] = 0;
    MATRIX_1f800000.m[1][0] = 0;
    MATRIX_1f800000.m[0][0] = (short)(param_2 * param_1[0].m4C[0] >> 0xc);
    MATRIX_1f800000.m[1][2] = 0;
    MATRIX_1f800000.m[2][0] = 0;
    MATRIX_1f800000.m[2][1] = 0;
    MATRIX_1f800000.m[1][1] = (short)(param_2 * param_1[0].m4C[1] >> 0xc);
    MATRIX_1f800000.m[2][2] = (short)(param_2 * param_1[0].m4C[2] >> 0xc);

    MulMatrix0(&param_1[0].m2C, &MATRIX_1f800000, &param_1[0].mC);

    param_1[0].mC.t = param_1[0].m2C.t;

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        sLoadedMecha_sub4* pBone = &param_1[i];
        if (pBone->m5) {
            if (pBone->m6 == 0) {
                createRotationMatrix(&pBone->m54_rotationAngles, &pBone->mC);
            }
            else {
                RotMatrixYXZ(&pBone->m54_rotationAngles, &pBone->mC);
            }
            pBone->m5 = 0;
        }
        if ((pBone->m0_parentBone != nullptr) && (pBone->m0_parentBone->m4 == 1)) {
            pBone->m4 = 1;
        }

        if (pBone->m4) {
            // TODO: recheck all of this!
            pBone->mC.t[0] = pBone->m5C_translation[0];
            pBone->mC.t[1] = pBone->m5C_translation[1];
            pBone->mC.t[2] = pBone->m5C_translation[2];

            if (pBone->m0_parentBone == nullptr) {
                pBone->m2C = pBone->mC;
            }
            else
            {
                CompMatrix(&pBone->m0_parentBone->m2C, &pBone->mC, &pBone->m2C);
            }
        }
    }

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        param_1[i].m4 = 0;
    }
}

u32 updateMechAnimSub3(sMechaInitVar2* param_1, std::vector<sLoadedMecha_sub4>& param_2, uint param_3, int speed) {
    u32 returnFlag = 0;
    for (int i = 0; i < param_2[0].mA_numBones; i++) {
        if (param_2[i].m70_animTracks[0]) {
            assert(0);
        }
        if (param_2[i].m70_animTracks[1]) {
            assert(0);
        }
        if (param_2[i].m70_animTracks[2]) {
            assert(0);
        }
    }

    return returnFlag;
}

void updateMechAnimSub4(sLoadedMechas* param_1, sMechaInitVar2* param_2, int oddOrEven) {
    if (param_1->m98 <= -1)
        return;

    assert(0);

    while (param_1->m9C < param_1->m9E) {
        MissingCode();
        param_1->m9C++;
    }

    param_1->m98++;
    if ((param_1->m9A < -1) && (param_1->m9A <= param_1->m98)) {
        param_1->m98 = 0;
        param_1->m9C = 0;
        param_1->mA0 = param_1->mA4;
    }
}

void updateMechAnim(sLoadedMechas* param_1, sMechaInitVar2* param_2, int iterations, int oddOrEven) {
    if (param_1->m0 != nullptr) {
        int updateFlag = 0;
        if (param_1->m34 != 0) {
            updateMechAnimSub0(param_1);
            if (param_1->m37 == 0) {
                updateMechAnimSub1(*param_1->m4, (int)param_1->m1C_moveSpeed);
            }
            else {
                assert(0);
                //FUN_Mecha__801dc848(param_1->m4, (int)param_1->m1C_moveSpeed);
            }
            for(int i=0; i<iterations; i++) {
                updateFlag |= updateMechAnimSub3(param_2, *param_1->m4, param_1->m3C, (int)param_1->m1C_moveSpeed);
                updateMechAnimSub4(param_1, param_2, oddOrEven);
            }
        }
        processMechaAnimData(param_1, param_2, updateFlag, iterations);
    }
}

void renderMechas(MATRIX* pMatrix, MATRIX* param_2, OTTable& OT, int oddOrEven, int param_5)
{
    mechaInitVar0 += 1 + param_5;
    if (mechaInitVar0 > 6) {
        mechaInitVar0 = 6;
    }
    int mechaIterationCount = 0;
    int mechaIterationCount2 = 0;
    if (mechaInitVar0 > 1) {
        do {
            mechaInitVar0 = mechaInitVar0 + -2;
            mechaIterationCount = mechaIterationCount + 1;
        } while (1 < mechaInitVar0);
        mechaIterationCount2 = mechaIterationCount * 8;
    }
    mechaIterationCount2 = (uint)mechaInitVar1 + (mechaIterationCount2 - mechaIterationCount) * 8;
    mechaInitVar1 = (ushort)mechaIterationCount2;
    int angleForMecha = getAngleSin(mechaIterationCount2 * 0x10000 >> 0x10);
    gMechaAngle = ((short)(((angleForMecha + 0x1000) * 0x51eb851f) >> 0x28) - (short)(angleForMecha + 0x1000 >> 0x1f)) + 4;

    for (int i = 0; i < 10; i++) {
        if (loadedMechas[i]) {
            loadedMechas[i]->m11C_previousTranslation = (*loadedMechas[i]->m4)[0].m5C_translation;
            updateMechAnim(loadedMechas[i], &mechaInitVar2, mechaIterationCount, oddOrEven);
        }
    }

    MissingCode();

    for (int i = 0; i < 10; i++) {
        if (loadedMechas[i] && loadedMechas[i]->m5C != -1) {
            updateMechasBaseMatrixIsAttached(loadedMechas[i]);
        }
    }

    MissingCode();

    //SetColorMatrix(mechaFinalizeVar0);

    for (int i = 0; i < 10; i++) {
        if (loadedMechas[i]) {
            loadedMechas[i]->m128_deltaTranslation[0] = loadedMechas[i]->m11C_previousTranslation[0] - (*loadedMechas[i]->m4)[0].m5C_translation[0];
            loadedMechas[i]->m128_deltaTranslation[1] = loadedMechas[i]->m11C_previousTranslation[1] - (*loadedMechas[i]->m4)[0].m5C_translation[1];
            loadedMechas[i]->m128_deltaTranslation[2] = loadedMechas[i]->m11C_previousTranslation[2] - (*loadedMechas[i]->m4)[0].m5C_translation[2];
            submitMechaForRendering(loadedMechas[i], pMatrix, param_2, 1, 1, OT, oddOrEven);
        }
    }
    MissingCode();
}

void renderMechasInField()
{
    if (disableMechaRendering == 0) {
        if (NumMechas != 0) {
            updateMechasPosForField();
            SetBackColor((uint)mechaBackColor[0], (uint)mechaBackColor[1], (uint)mechaBackColor[2]);
            renderMechas(&currentProjectionMatrix, &mechaFieldArgs2[0], pCurrentFieldRenderingContext->mCC_OT, g_frameOddOrEven, 1);
        }
        if (fieldDebugDisable == 0) {
            assert(0);
        }
    }
}

void initMechaFieldArgs2(MATRIX& param_1, short param_2, short param_3, short param_4, short param_5, short param_6, short param_7, short param_8, short param_9, short param_10)
{
    param_1.m[0][0] = param_2;
    param_1.m[0][1] = param_3;
    param_1.m[0][2] = param_4;
    param_1.m[1][0] = param_5;
    param_1.m[1][1] = param_6;
    param_1.m[1][2] = param_7;
    param_1.m[2][0] = param_8;
    param_1.m[2][1] = param_9;
    param_1.m[2][2] = param_10;
    return;
}

void mechaPlayAnimation(ushort mechaId, short mechaId2, int animationId) {
    mechaPlayAnimationVar0 = mechaId2;
    mechaPlayAnimationVar1 = mechaId;
    loadedMechas[mechaId]->m35 = 0;
    if (loadedMechas[mechaId]) {
        initMechaAnimation(loadedMechas[mechaId], loadedMechas[mechaId], &mechaInitVar2, animationId);
    }
}

void convertMatrixToBgfx(MATRIX* pInputMatrix, float* finalMatrix)
{
    bx::mtxIdentity(finalMatrix);

    bx::mtxTranslate(finalMatrix, pInputMatrix->t[0], pInputMatrix->t[1], pInputMatrix->t[2]);
}

void renderMechasForDebugFieldRenderer(int viewId)
{
    for (int i = 0; i < 10; i++)
    {
        if (loadedMechas[i]) {
            sLoadedMechas* pMecha = loadedMechas[i];

            std::vector<sLoadedMecha_sub4>::iterator m4It = (*pMecha->m4).begin();
            int numBones = m4It->mA_numBones;
            m4It++;
            for (int i = 1; i < numBones; i++) {
                if ((m4It->m8 != -1) && (m4It->m7)) {
                    float finalMatrix[16];
                    convertMatrixToBgfx(&m4It->m2C, finalMatrix);
                    pMecha->m0->m0[m4It->m8]->bgfxRender(viewId, finalMatrix);
                }
                m4It++;
            }
        }
    }
}

void getMechaBoneMatrix(MATRIX* param_1, void* param_2, int mechaIndex, int boneIndex) {
    sLoadedMechas* pMecha = loadedMechas[mechaIndex];
    if (pMecha != (sLoadedMechas*)0x0) {
        if (boneIndex == 0) {
            *param_1 = (*pMecha->m4)[0].mC;
        }
        else {
            CompMatrix(&(*pMecha->m4)[0].mC, &(*pMecha->m4)[boneIndex].m2C, param_1);
        }
    }
}