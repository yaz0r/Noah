#include "noahLib.h"
#include "mechaOverlay.h"
#include "field/field.h"
#include "kernel/filesystem.h"
#include "kernel/gte.h"
#include "bx/math.h"
#include "field/fieldGraphicObject.h"
#include "kernel/memory.h"
#include "battle/battle.h"
#include "kernel/3dModel_psxRenderer.h"
#include "kernel/3dModel_bgfxRenderer.h"
#include "kernel/kernelVariables.h"

u32 NumMechas;
std::array<s16, 4> mechaList;
std::array<u8, 4> mechaList2;

s16 mechaPlayAnimationVar0 = 0;
s16 mechaPlayAnimationVar1 = 0;

std::array<s16, 4> mechaCurrentAnimation;
u32 mechaBytecodeToggle = 0;

void initModel5(sModel* pModelBlock);

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

sLoadableDataRaw mechaOverlayBuffer;

std::array<sLoadingBatchCommands, 16> mechaOverlayBatchLoadingTable;

std::array<sMechaDataTable1*, 9> mechaDataTable1;
std::array<sLoadableDataRaw, 9> mechaDataTable1_raw;

std::array<sMechaDataTable2*, 9> mechaDataTable2;
std::array<sLoadableDataRaw, 9> mechaDataTable2_raw;


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

sMechaInitVar3 mechaInitVar3;

std::array<sLoadedMechas*, 10> loadedMechas;
std::array<int, 10> mechaList3;
s16 numMaxMechaOverlayEntries;


void freeMechaInitVar2(sMechaInitVar2* param_1) {
    param_1->m4 = 0;
    param_1->m0.clear();
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

sMechaInitVar2 mechaInitVar2;

void initMechaInitVar2(sMechaInitVar2* param_1, int count) {
    count = 0x7FFF;
    Hack("Num bone buffer");
    if (count > 0) {
        param_1->m6 = count;
        resetMemoryAllocStats(4, 0);
        param_1->m0.resize(count);
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

std::array<u8, 3> mechaBackColor = { 0,0,0 };
std::array<MATRIX, 2> mechaFieldArgs2;

std::array<SVECTOR, 4> initMechaTempVar;

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

std::vector<sMechaBone>* processMechaMesh(sMechaInitVar4* param_1, std::vector<sMechaDataTable1_C>& param_2, u32 param_3, int useTpageAndClut, short tpageX, short tpageY, short clutX, short clutY)
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

    std::vector<sMechaBone>* outputVector = new std::vector<sMechaBone>;
    outputVector->resize(numBones + 1);

    std::vector<sMechaBone>::iterator it = outputVector->begin();

    it->m4_needTranslationUpdate = 1;
    it->m5_needRotationUpdate = 1;
    it->m6 = 1;
    it->m4C_scale[0] = 0x1000;
    it->m4C_scale[1] = 0x1000;
    it->m4C_scale[2] = 0x1000;
    it->m0_parentBone = nullptr;
    it->m7_isEnabled = 0;
    it->m8_geometryId = -1;
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
        it->m4_needTranslationUpdate = 1;
        it->m5_needRotationUpdate = 1;
        it->m7_isEnabled = 1;
        it->m4C_scale[0] = 0x1000;
        it->m4C_scale[1] = 0x1000;
        it->m4C_scale[2] = 0x1000;
        it->m6 = 0;
        it->m52 = 0;
        it->m8_geometryId = it2->m0;
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

int releaseMechaAnimTrack(sMechaInitVar2* param_1, sMechaInitVar2Sub* param_2)
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

sMechaInitVar2Sub* allocateAnimTrack(sMechaInitVar2* param_1) {
    sMechaInitVar2Sub* psVar1;
    uint uVar2;
    sMechaInitVar2Sub* psVar3;

    if ((uint)(ushort)param_1->m4 < (uint)(ushort)param_1->m6) {
        psVar3 = &param_1->m0[param_1->m4];
        psVar1 = (sMechaInitVar2Sub*)0x0;
        if (psVar3->m0 == 0) {
            param_1->m4 = param_1->m4 + 1;
            uVar2 = (uint)(ushort)param_1->m4;
            psVar1 = psVar3;
            if (uVar2 < (ushort)param_1->m6) {
                do {
                    if (param_1->m0[uVar2].m0 == 0) {
                        psVar3->m3 = 0; // hack
                        return psVar3;
                    }
                    param_1->m4 = param_1->m4 + 1;
                    uVar2 = (uint)(ushort)param_1->m4;
                } while (uVar2 < (ushort)param_1->m6);
            }
        }
    }
    else {
        psVar1 = (sMechaInitVar2Sub*)0x0;
    }
    return psVar1;
}

void mechaOP_8(sMechaInitVar2* param_1, std::vector<sMechaBone>* param_2) {
    for (int i = 0; i < (*param_2)[0].mA_numBones; i++) {
        sMechaBone* pEntry = &(*param_2)[i];
        if (pEntry->m70_animTracks[0] && (pEntry->m70_animTracks[0]->m3 != -1)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[0]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != -1)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[1]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != -1)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[1]);
        }
    }
}

void freeAnimTracks2(sMechaInitVar2* param_1, std::vector<sMechaBone>* param_2, s8 value) {
    for (int i = 0; i < (*param_2)[0].mA_numBones; i++) {
        sMechaBone* pEntry = &(*param_2)[i];
        if (pEntry->m70_animTracks[0] && (pEntry->m70_animTracks[0]->m3 != value)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[0]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != value)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[1]);
        }
        if (pEntry->m70_animTracks[1] && (pEntry->m70_animTracks[1]->m3 != value)) {
            releaseMechaAnimTrack(param_1, pEntry->m70_animTracks[1]);
        }
    }
}

void mechaOP_A(sMechaInitVar2* param_1, sMechaBone* param_2, int param_3, uint param_4) {
    if (param_3 < (int)(uint)(ushort)param_2->mA_numBones) {
        if ((param_2[param_3].m70_animTracks[0] != 0) && ((param_4 & 1) != 0)) {
            releaseMechaAnimTrack(param_1, param_2[param_3].m70_animTracks[0]);
            param_2[param_3].m70_animTracks[0] = 0;
        }
        if ((param_2[param_3].m70_animTracks[1] != 0) && ((param_4 & 2) != 0)) {
            releaseMechaAnimTrack(param_1, param_2[param_3].m70_animTracks[1]);
            param_2[param_3].m70_animTracks[1] = 0;
        }
        if ((param_2[param_3].m70_animTracks[2] != 0) && ((param_4 & 4) != 0)) {
            releaseMechaAnimTrack(param_1, param_2[param_3].m70_animTracks[2]);
            param_2[param_3].m70_animTracks[2] = 0;
        }
    }
}

void mechaSetPoseFromKeyframe(std::vector<sMechaBone>& param_1, sMechaDataTable2_4_4* param_2)
{
    std::vector<u8>::iterator pShortData = param_2->m18.begin();
    if (param_2->m6 == 0) {
        pShortData += (param_2->mC_numRotations + 1) * 3 * 2;
    }

    int rotationCount = 0;
    int translationCount = 0;

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        sMechaBone* pBone = &param_1[i];

        if (((param_2->m4_flags & 1) == 0) && (rotationCount < param_2->mC_numRotations)) {
            std::vector<u8>::iterator sVar1 = pShortData;
            pShortData+=3*2;
            rotationCount++;

            if ((pBone->m54_rotationAngles[0] != READ_LE_S16(sVar1 + 0)) || (pBone->m54_rotationAngles[1] != READ_LE_S16(sVar1 + 2)) || (pBone->m54_rotationAngles[2] != READ_LE_S16(sVar1 + 4))) {
                if ((pBone->m70_animTracks[0] == nullptr) || (pBone->m70_animTracks[0]->m3 != -1))
                {
                    pBone->m54_rotationAngles[0] = READ_LE_S16(sVar1 + 0);
                    pBone->m54_rotationAngles[1] = READ_LE_S16(sVar1 + 2);
                    pBone->m54_rotationAngles[2] = READ_LE_S16(sVar1 + 4);
                    pBone->m4_needTranslationUpdate = 1;
                    pBone->m5_needRotationUpdate = 1;
                }
            }
        }

        if (((param_2->m4_flags & 2) == 0) && (translationCount < param_2->mE_numTranslations)) {
            std::vector<u8>::iterator sVar1 = pShortData;
            pShortData += 3 * 2;
            translationCount++;

            if ((pBone->m5C_translation[0] != READ_LE_S16(sVar1 + 0)) || (pBone->m5C_translation[1] != READ_LE_S16(sVar1 + 2)) || (pBone->m5C_translation[2] != READ_LE_S16(sVar1 + 4))) {
                if ((pBone->m70_animTracks[1] == nullptr) || (pBone->m70_animTracks[1]->m3 != -1))
                {
                    pBone->m5C_translation[0] = READ_LE_S16(sVar1 + 0);
                    pBone->m5C_translation[1] = READ_LE_S16(sVar1 + 2);
                    pBone->m5C_translation[2] = READ_LE_S16(sVar1 + 4);
                    pBone->m4_needTranslationUpdate = 1;
                }
            }
        }
    }
}

sMechaDataTable2_4_4* mechaGetKeyframe(sLoadedMechas* param_1, uint param_2, u32* param_3)
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

void mechaOP_13(sMechaInitVar2* param_1, std::vector<sMechaBone>& param_2, sMechaDataTable2_4_4* param_3, int transitionLength, byte interpolationType, byte isLooping, char param_7) {
    s32 local_68 = transitionLength;
    if (local_68 == 0) {
        local_68 = 1;
    }

    std::vector<u8>::iterator trackHeader = param_3->m18.begin();
    if (param_3->m6 == 0) {
        trackHeader = trackHeader + (param_3->mC_numRotations + 1) * 6;
    }

    s32 rotationCount = 0;
    s32 translationCount = 0;

    for (int i = 1; i < param_2[0].mA_numBones; i++) {
        sMechaBone* pBone = &param_2[i];

        if (((param_3->m4_flags & 1) == 0) && (rotationCount < param_3->mC_numRotations)) {
            SVECTOR values;
            values[0] = READ_LE_S16(trackHeader + 0);
            values[1] = READ_LE_S16(trackHeader + 2);
            values[2] = READ_LE_S16(trackHeader + 4);
            trackHeader += 3 * 2;
            rotationCount++;
            if ((pBone->m54_rotationAngles[0] == values[0]) && (pBone->m54_rotationAngles[1] == values[1]) && (pBone->m54_rotationAngles[2] == values[2])) {
                if ((pBone->m70_animTracks[0] != nullptr && pBone->m70_animTracks[0]->m3 != -1)) {
                    releaseMechaAnimTrack(param_1, pBone->m70_animTracks[0]);
                    pBone->m70_animTracks[0] = nullptr;
                }
            }
            else {
                if (pBone->m70_animTracks[0] == nullptr) {
                    pBone->m70_animTracks[0] = allocateAnimTrack(param_1);
                }
                if (pBone->m70_animTracks[0] && pBone->m70_animTracks[0]->m3 != -1) {
                    sMechaInitVar2Sub* pTrack = pBone->m70_animTracks[0];
                    pTrack->m0 = 1;
                    pTrack->m2 = (interpolationType & 1) + 3;
                    pTrack->m1_isLooping = isLooping;
                    pTrack->m3 = param_7;
                    pTrack->m4_s16 = pBone->m54_rotationAngles[0];
                    pTrack->m6_s16 = pBone->m54_rotationAngles[1];
                    pTrack->m8_s16 = pBone->m54_rotationAngles[2];

                    {
                        s32 delta = values[0] - pBone->m54_rotationAngles[0] & 0xFFF;
                        if (delta > 0x7FF) {
                            delta -= 0x1000;
                        }
                        pTrack->mA_s16 = delta;
                    }
                    {
                        s32 delta = values[1] - pBone->m54_rotationAngles[1] & 0xFFF;
                        if (delta > 0x7FF) {
                            delta -= 0x1000;
                        }
                        pTrack->mC = delta;
                    }
                    {
                        s32 delta = values[2] - pBone->m54_rotationAngles[2] & 0xFFF;
                        if (delta > 0x7FF) {
                            delta -= 0x1000;
                        }
                        pTrack->mE = delta;
                    }
                    pTrack->m10_currentTime = 0;
                    pTrack->m12_maxTime = local_68;
                    pBone->m70_animTracks[0] = pTrack;
                }
            }
        }
        else {
            if ((pBone->m70_animTracks[0] != nullptr && pBone->m70_animTracks[0]->m3 != -1)) {
                releaseMechaAnimTrack(param_1, pBone->m70_animTracks[0]);
                pBone->m70_animTracks[0] = nullptr;
            }
        }

        if (((param_3->m4_flags & 2) == 0) && (translationCount < param_3->mE_numTranslations)) {
            SVECTOR values;
            values[0] = READ_LE_S16(trackHeader + 0);
            values[1] = READ_LE_S16(trackHeader + 2);
            values[2] = READ_LE_S16(trackHeader + 4);
            trackHeader += 3 * 2;
            translationCount++;
            if ((pBone->m5C_translation[0] == values[0]) && (pBone->m5C_translation[1] == values[1]) && (pBone->m5C_translation[2] == values[2])) {
                if ((pBone->m70_animTracks[1] != nullptr && pBone->m70_animTracks[1]->m3 != -1)) {
                    releaseMechaAnimTrack(param_1, pBone->m70_animTracks[1]);
                    pBone->m70_animTracks[1] = nullptr;
                }
            }
            else {
                if (pBone->m70_animTracks[1] == nullptr) {
                    pBone->m70_animTracks[1] = allocateAnimTrack(param_1);
                }
                if (pBone->m70_animTracks[1] && pBone->m70_animTracks[1]->m3 != -1) {
                    sMechaInitVar2Sub* pTrack = pBone->m70_animTracks[1];
                    pTrack->m0 = 1;
                    pTrack->m2 = (interpolationType & 1) + 3;
                    pTrack->m1_isLooping = isLooping;
                    pTrack->m3 = param_7;
                    pTrack->m4_s16 = pBone->m5C_translation[0];
                    pTrack->m6_s16 = pBone->m5C_translation[1];
                    pTrack->m8_s16 = pBone->m5C_translation[2];

                    if ((interpolationType & 1) == 0) {
                        pTrack->mA_s16 = values[0] - pBone->m5C_translation[0];
                        pTrack->mC = values[1] - pBone->m5C_translation[1];
                        pTrack->mE = values[2] - pBone->m5C_translation[2];
                    }
                    else {
                        pTrack->mA_s16 = values[0];
                        pTrack->mC = values[1];
                        pTrack->mE = values[2];
                    }

                    pTrack->m10_currentTime = 0;
                    pTrack->m12_maxTime = local_68;
                    pBone->m70_animTracks[1] = pTrack;
                }
            }
        }
        else {
            if ((pBone->m70_animTracks[1] != nullptr && pBone->m70_animTracks[1]->m3 != -1)) {
                releaseMechaAnimTrack(param_1, pBone->m70_animTracks[1]);
                pBone->m70_animTracks[1] = nullptr;
            }
        }
    }
}


int mechaOP_11(sMechaInitVar2* param_1, std::vector<sMechaBone>& param_2, sMechaDataTable2_4_4* param_3, uint isLooping, u16 param_5)
{
    if (param_3->m6 == 0) {
        s32 numRotations = param_3->mC_numRotations;
        s32 numBones = param_2[0].mA_numBones;
        s32 numRotation2 = numRotations + 1;
        isLooping = isLooping & 1;
        if (numRotation2 < numBones) {
            numBones = numRotations + 1;
        }
        s32 animationDuration = param_3->m2_keyDuration;
        if (isLooping == 0) {
            animationDuration = animationDuration + -1;
        }
        std::vector<u8>::iterator trackHeader = param_3->m18.begin();
        std::vector<u8>::iterator psVar8 = trackHeader + numRotation2 * 3 * 2;
        if ((param_3->m4_flags & 1) == 0) {
            psVar8 = psVar8 + (uint)numRotations * 3 * 2;
        }
        if ((param_3->m4_flags & 2) == 0) {
            psVar8 = psVar8 + (uint)(ushort)param_3->mE_numTranslations * 3 * 2;
        }
        for (int i = 0; i < numBones; i++) {
            std::array<sMechaInitVar2Sub*, 3>& tracks = param_2[i].m70_animTracks;
            {
                // rotations
                sMechaInitVar2Sub*& track = tracks[0];
                if (READ_LE_S16(trackHeader + 0) == -1) { // if there is no track for this bone and the bone had a track allocated already, release it
                    if (track && i && track->m3 != -1) {
                        releaseMechaAnimTrack(param_1, track);
                        track = nullptr;
                    }
                }
                else {
                    if (track == nullptr) {
                        track = allocateAnimTrack(param_1);
                    }
                    if (track && track->m3 != -1) {
                        // init track
                        track->m0 = 1;
                        track->m1_isLooping = isLooping;
                        track->m3 = param_5;
                        track->m2 = READ_LE_U8(trackHeader + 4);
                        track->m10_currentTime = 0;
                        track->m12_maxTime = animationDuration;
                        int offsetToData = READ_LE_U16(trackHeader + 0);
                        track->m4 = psVar8 + offsetToData;
                        track->m8 = psVar8 + offsetToData;
                    }
                }
            }
            {
                // translations
                sMechaInitVar2Sub*& track = tracks[1];
                if (READ_LE_S16(trackHeader + 2) == -1) { // if there is no track for this bone and the bone had a track allocated already, release it
                    if (track && i && track->m3 != -1) {
                        releaseMechaAnimTrack(param_1, track);
                        track = nullptr;
                    }
                }
                else {
                    if (track == nullptr) {
                        track = allocateAnimTrack(param_1);
                    }
                    if (track && track->m3 != -1) {
                        // init track
                        track->m0 = 1;
                        track->m1_isLooping = isLooping;
                        track->m3 = param_5;
                        track->m2 = READ_LE_U8(trackHeader + 5);
                        track->m10_currentTime = 0;
                        track->m12_maxTime = animationDuration;
                        int offsetToData = READ_LE_U16(trackHeader + 2);
                        track->m4 = psVar8 + offsetToData;
                        track->m8 = psVar8 + offsetToData;
                    }

                }
            }
            LAB_Mecha__801dfa74:
            trackHeader+=3*2;
        }
        return 0;
    }
    else
    {
        mechaOP_8(param_1, &param_2);
        mechaSetPoseFromKeyframe(param_2, param_3);
        return 1;
    }
}

void mechaOp_62(sLoadedMechas* param_1, sMechaBone* param_2, byte param_3, short param_4,
    short param_5, short param_6)

{
    short sVar1;
    int iVar3;

    if ((param_3 & 7) == 0) {
        if ((param_3 & 0x20) == 0) {
            (param_2->m54_rotationAngles).vx = param_4;
            (param_2->m54_rotationAngles).vy = param_5;
            (param_2->m54_rotationAngles).vz = param_6;
        }
        else {
            (param_2->m54_rotationAngles).vx = param_4 + (param_2->m54_rotationAngles).vx;
            (param_2->m54_rotationAngles).vz = param_6 + (param_2->m54_rotationAngles).vz;
            (param_2->m54_rotationAngles).vy = param_5 + (param_2->m54_rotationAngles).vy;
        }
    }
    else if ((param_3 & 7) == 1) {
        if ((param_3 & 0x20) == 0) {
            param_2->m5C_translation[0] = (int)param_4;
            param_2->m5C_translation[1] = (int)param_5;
            param_2->m5C_translation[2] = (int)param_6;
        }
        else {
            param_2->m5C_translation[0] = (int)param_4 + param_2->m5C_translation[0];
            param_2->m5C_translation[1] = (int)param_5 + param_2->m5C_translation[1];
            param_2->m5C_translation[2] = (int)param_6 + param_2->m5C_translation[2];
        }
    }
    else if ((param_3 & 0x20) == 0) {
        param_2->m4C_scale[0] = param_4;
        param_2->m4C_scale[1] = param_5;
        param_2->m4C_scale[2] = param_6;
    }
    else {
        sVar1 = param_2->m4C_scale[2];
        param_2->m4C_scale[0] = param_4 + param_2->m4C_scale[0];
        param_2->m4C_scale[2] = param_6 + sVar1;
        param_2->m4C_scale[1] = param_5 + param_2->m4C_scale[1];
    }
    param_2->m4_needTranslationUpdate = 1;
    param_2->m5_needRotationUpdate = 1;
    iVar3 = 1;

    // if 0x80 is set, apply recursively to sub bones
    if (param_3 & 0x80) {
        for (int i = 1; i < (*param_1->m4_bones)[0].mA_numBones; i++) {
            if ((*param_1->m4_bones)[i].m0_parentBone == param_2) {
                mechaOp_62(param_1, &(*param_1->m4_bones)[i], param_3, param_4, param_5, param_6);
            }
        }

    }
    return;
}


void setBoneVisibility(sLoadedMechas* param_1, sMechaBone* param_2, u32 param_3) {
    param_2->m7_isEnabled = param_3 & 1;

    // if 0x80 is set, apply recursively to sub bones
    if ((param_3 & 0x80) != 0) {
        for (int i = 1; i < (*param_1->m4_bones)[0].mA_numBones; i++) {
            if ((*param_1->m4_bones)[i].m0_parentBone == param_2) {
                setBoneVisibility(param_1, &(*param_1->m4_bones)[i], param_3);
            }
        }
    }
}

void mechaOP_19(sLoadedMechas* param_1)
{
    param_1->m98 = -1;
    return;
}

void mechaOP_11_b(sLoadedMechas* param_1, sMechaDataTable2_4_4* param_2, int param_3)
{
    if (param_2->m12 == 0) {
        param_1->m98 = -1;
    }
    else {
        param_1->m98 = 0;
        if (param_3 == 0) {
            param_1->m9A = -1;
        }
        else {
            param_1->m9A = param_2->m2_keyDuration;
        }
        param_1->m9C = 0;
        param_1->m9E = param_2->m12;
        param_1->mA0 = param_2->m_raw.begin() + param_2->m14;
        param_1->mA4 = param_2->m_raw.begin() + param_2->m14;
    }
    return;
}

static const std::vector<s8> DAT_Battle__800c3530 = { {
        0x1, 0x6C,
} };

extern s16 setupMechaForEventVar1;

void processMechaAnimData(sLoadedMechas* pMecha, sMechaInitVar2* param_2, int param_3, int param_4)
{
    if (param_4 == 0) {
        return;
    }
    if (pMecha->m10_bytecode0 == 0) {
        return;
    }

    resetMemoryAllocStats(4, 0);

    // step the mecha forward?
    for (int i = 0; i < param_4; i++) {
        pMecha->m70[0] += pMecha->m76[0];
        pMecha->m70[1] += pMecha->m76[1];
        pMecha->m70[2] += pMecha->m76[2];

        pMecha->m7C[0] += pMecha->m82[0];
        pMecha->m7C[1] += pMecha->m82[1];
        pMecha->m7C[2] += pMecha->m82[2];

        (*pMecha->m4_bones)[0].m54_rotationAngles[0] += pMecha->m70[0] >> 3;
        (*pMecha->m4_bones)[0].m54_rotationAngles[1] += pMecha->m70[1] >> 3;
        (*pMecha->m4_bones)[0].m54_rotationAngles[2] += pMecha->m70[2] >> 3;

        SVECTOR local_120;
        local_120.vx = ((int)pMecha->m7C[0] * (int)(*pMecha->m4_bones)[0].m4C_scale[0] >> 0xc);
        local_120.vy = ((int)pMecha->m7C[1] * (int)(*pMecha->m4_bones)[0].m4C_scale[1] >> 0xc);
        local_120.vz = ((int)pMecha->m7C[2] * (int)(*pMecha->m4_bones)[0].m4C_scale[2] >> 0xc);

        VECTOR local_130;
        ApplyMatrix(&(*pMecha->m4_bones)[0].m2C_boneFinalMatrix, &local_120, &local_130);

        (*pMecha->m4_bones)[0].m5C_translation[0] += (pMecha->m1C_scale * local_130.vx >> 0xc);
        (*pMecha->m4_bones)[0].m5C_translation[1] += (pMecha->m1C_scale * local_130.vy >> 0xc);
        (*pMecha->m4_bones)[0].m5C_translation[2] += (pMecha->m1C_scale * local_130.vz >> 0xc);
    }

    bool continueBytecodeExecution = true;
    sMechaDataTable2_4_8* pNextBytecode = pMecha->m10_bytecode0;
    bool local_78 = 0;
    while (1) {
        sMechaDataTable2_4_8* pCurrentByteCodePtr = pNextBytecode;
        if (!continueBytecodeExecution) {
            pMecha->m10_bytecode0 = pCurrentByteCodePtr;
            if (local_78 == 0) {
                return;
            }
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
                pMecha->m40 += param_3;
                if (_currentByteCode < pMecha->m40) {
                    pMecha->m40 = 0;
                    param_3 = 0;
                    pNextBytecode = pCurrentByteCodePtr + 2;
                    break;
                }
            }
            continueBytecodeExecution = 0;
            break;
        case 2:
            assert(isBattleOverlayLoaded == 0); // there is some code different in battle
            continueBytecodeExecution = 0;
            local_78 = 1;
            break;
        case 3:
            // battle only
            assert(isBattleOverlayLoaded);
            if (pMecha->m35) {
                if (mecha_battle_op3() == 0) {
                    allocateSavePointMeshDataSub0_var0 = 0;
                    spriteBytecode2ExtendedE0_Var0 = 0;
                    pMecha->m35 = 0;
                    mecha_battle_op3_2(pMecha->m20_mechaEntryId);
                }
                else {
                    continueBytecodeExecution = 0;
                }
            }
            break;
        case 4:
            // battle only
            assert(isBattleOverlayLoaded);
            if (pMecha->mC == nullptr) {
                int local_10;
                int local_c;
                getCurrentDirectory(&local_10, &local_c);
                setCurrentDirectory(0x28, 2);
                u16 nextValue = *pNextBytecode;
                pNextBytecode++;
                int fileIndex = bytecodeHigher + DAT_Battle__800c3530[nextValue];
                //sVar12 = getFileSizeAligned(fileIndex);
                //piVar13 = (int*)malloc(sVar12);
                pMecha->mC = new sLoadableDataRaw;
                readFile(fileIndex, *pMecha->mC, 0, 0);
                setCurrentDirectory(local_10, local_c);
            }
        case 0x8:
            mechaOP_8(param_2, pMecha->m4_bones); // release all tracks disabled
            break;
        case 0xA:
            mechaOP_A(param_2, &(*pMecha->m4_bones)[0], bytecodeHigher, 7); // release tracks on a specific mask
            break;
        case 0xC:
            pMecha->m70[0] = 0;
            pMecha->m70[1] = 0;
            pMecha->m70[2] = 0;
            pMecha->m76[0] = 0;
            pMecha->m76[1] = 0;
            pMecha->m76[2] = 0;
            pMecha->m7C[0] = 0;
            pMecha->m7C[1] = 0;
            pMecha->m7C[2] = 0;
            pMecha->m82[0] = 0;
            pMecha->m82[1] = 0;
            pMecha->m82[2] = 0;
            break;
        case 0xF:
            break;
        case 0x10: // set mecha at keyframe
            mechaSetPoseFromKeyframe(*pMecha->m4_bones, mechaGetKeyframe(pMecha, bytecodeHigher, &local_100));
            break;
        case 0x11: // interpolate to keyframe
            {
                u32 local_100;
                sMechaDataTable2_4_4* pKeyframe = mechaGetKeyframe(pMecha, bytecodeHigher, &local_100);
                _currentByteCode = *pNextBytecode;
                pNextBytecode = pCurrentByteCodePtr + 2;
                if (local_100 == 0) {
                    bytecodeHigher = _currentByteCode >> 8;
                    mechaOP_11(param_2, *pMecha->m4_bones, pKeyframe, bytecodeHigher, _currentByteCode);
                    param_3 = -1;
                    int iVar6 = (int)*(short*)&pKeyframe->m10 *
                        ((int)pMecha->m1C_scale * (int)(*pMecha->m4_bones)[0].m4C_scale[2] >> 0xc) >> 0xc;
                    if (iVar6 < 0) {
                        iVar6 = -iVar6;
                    }
                    pMecha->m8E = (short)iVar6;
                    mechaOP_11_b(pMecha, pKeyframe, bytecodeHigher);
                }
            }
            break;
        case 0x13:
            {
                u8 interpolationType = pCurrentByteCodePtr[0] >> 8;
                u8 frameId = pCurrentByteCodePtr[1] & 0xFF;
                u8 unk = pCurrentByteCodePtr[1] >> 8;
                u8 loop = pCurrentByteCodePtr[2] & 0xFF;
                u8 interpolationLength = pCurrentByteCodePtr[2] >> 8;
                pNextBytecode = pCurrentByteCodePtr + 3;

                sMechaDataTable2_4_4* pKeyframe = mechaGetKeyframe(pMecha, frameId, &local_100);
                if (mechaBytecodeToggle == 0) {
                    mechaOP_13(param_2, *pMecha->m4_bones, pKeyframe, interpolationLength, interpolationType, loop, unk);
                }
                else {
                    mechaSetPoseFromKeyframe(*pMecha->m4_bones, pKeyframe);
                }
                
                param_3 = -1;
                mechaOP_19(pMecha);
            }
            break;
        case 0x14:
            MissingCode();
            pNextBytecode++;
            break;
        case 0x19:
            mechaOP_19(pMecha);
            break;
        case 0x1D:
            param_3 = -1;
            MissingCode();
            pNextBytecode = pCurrentByteCodePtr + 10;
            break;
        case 0x1e:
            pMecha->m37 = bytecodeHigher;
            break;
        case 0x21:
            pMecha->m3C = bytecodeHigher;
            if (param_3 != -1) {
                if ((param_3 & 1) == 0)
                    break;
            }
            continueBytecodeExecution = 0;
            pNextBytecode = pCurrentByteCodePtr;
            break;
        case 0x23:
            setBoneVisibility(pMecha, &(*pMecha->m4_bones)[*pNextBytecode], bytecodeHigher);
            pNextBytecode = pCurrentByteCodePtr + 2;
            break;
        case 0x24:
            if (pMecha) {
                pMecha->m34 = bytecodeHigher & 1;
            }
            break;
        case 0x28: // distance to target?
        {
            //int distance = computeMechaDistanceToTarget(pMecha);
            if (pMecha->m8E == 0) {
                pMecha->m8E = 1;
            }
            MissingCode();
            pNextBytecode++;
            break;
        }
        case 0x2E:
            MissingCode();
            pNextBytecode++;
            break;
        case 0x2F:
            if (mecha_battle_op3()) { // wait for damage and other displays to be done?
                continueBytecodeExecution = 0;
                pNextBytecode = pCurrentByteCodePtr;
            }
            break;
        case 0x32:
            _currentByteCode = *pNextBytecode;
            pNextBytecode = pCurrentByteCodePtr + _currentByteCode / 2;
            break;
        case 0x36:
            pMecha->m44 = 0;
            pMecha->m46 = *pNextBytecode++;
            pMecha->m9C = *pNextBytecode++;
            MissingCode();
            break;
        case 0x39:
            MissingCode();
            pNextBytecode++;
            break;
        case 0x42:
        case 0x43:
            MissingCode("Mecha angle");
            break;
        case 0x48:
            pMecha->m36 = bytecodeHigher;
            break;
        case 0x4A:
            if (bytecodeHigher == 0xFB) {
                assert(0); // this exist in field
            }
            else {
                // battle only
                u16 output;
                int index = battleGetMechaBitfieldForAnim(pMecha, bytecodeHigher, &output);
                (*pMecha->m4_bones)[0].m5C_translation[0] = battleVisualEntities[index].mA_X;
                (*pMecha->m4_bones)[0].m5C_translation[2] = battleVisualEntities[index].mC_Z;
            }
            break;
        case 0x4B:
            pMecha->m7C[0] = *pNextBytecode++;
            pMecha->m7C[1] = *pNextBytecode++;
            pMecha->m7C[2] = *pNextBytecode++;
            break;
        case 0x4D:
            pMecha->m82[0] = *pNextBytecode++;
            pMecha->m82[1] = *pNextBytecode++;
            pMecha->m82[2] = *pNextBytecode++;
            break;
        case 0x4F:
            pNextBytecode++;
            break;
        case 0x53:
        {
            int sp120 = pMecha->m88[0];
            int sp122 = 0;
            int sp124 = pMecha->m88[2];
            MissingCode();
        }
        break;
        case 0x54:
            MissingCode();
            pNextBytecode++;
            break;
        case 0x57:
            MissingCode();
            break;
        case 0x58:
            pMecha->m58 = bytecodeHigher & 0xFF;
            pMecha->m5A = 0;
            pMecha->m64 = 0;
            pMecha->m66 = 0;
            pMecha->m68 = 0;
            if (bytecodeHigher & 0xFF) {
                MissingCode();
            }
            break;
        case 0x5A:
            MissingCode();
            break;
        case 0x60:
            pMecha->m88[0] = (battleMechaInitData->m100[battleVisualEntities[pMecha->m20_mechaEntryId].m0_positionSlot][0].vx + battleMechaInitData->m100[battleVisualEntities[pMecha->m20_mechaEntryId].m0_positionSlot][1].vx) / 2;
            pMecha->m88[1] = 0;
            pMecha->m88[2] = (battleMechaInitData->m100[battleVisualEntities[pMecha->m20_mechaEntryId].m0_positionSlot][0].vy + battleMechaInitData->m100[battleVisualEntities[pMecha->m20_mechaEntryId].m0_positionSlot][1].vy) / 2;
            break;
        case 0x61:
        {
            u16 nextValue = *pNextBytecode;
            pNextBytecode++;
            if (getBattleSlotLayout(pMecha->m20_mechaEntryId)) {
                assert((nextValue % 2) == 0);
                pNextBytecode = pCurrentByteCodePtr + nextValue / 2;
            }
            break;
        }
        case 0x62:
            mechaOp_62(pMecha, &(*pMecha->m4_bones)[*pNextBytecode], bytecodeHigher,
                (int)(short)pCurrentByteCodePtr[2], (int)(short)pCurrentByteCodePtr[3],
                (int)(short)pCurrentByteCodePtr[4]);
            pNextBytecode = pCurrentByteCodePtr + 5;
            break;
        case 0x6E:
        {
            u16 bitfield;
            int index = battleGetMechaBitfieldForAnim(pMecha, bytecodeHigher, &bitfield);
            u16 nextValue = *pNextBytecode;
            pNextBytecode++;
            if (battleMechas[index]) {
                if (battleMechas[index]->m38 == (nextValue & 1)) {
                    continueBytecodeExecution = 0;
                    pNextBytecode = pCurrentByteCodePtr;
                }
            }
            break;
        }
        break;
        case 0x6F:
            if ((bytecodeHigher && 0xFF) == 0) {
                pMecha->m3A = -1;
            }
            else {
                pMecha->m3A = setupMechaForEventVar1;
            }
            break;
        case 0x70:
        {
            u16 nextValue = *pNextBytecode;
            pNextBytecode++;
            if (pMecha->m3A == setupMechaForEventVar1) {
                assert((nextValue % 2) == 0);
                pNextBytecode = pCurrentByteCodePtr + nextValue / 2;
                continueBytecodeExecution = 0;
            }
            break;
        }
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

void freeModelBlock(sModelBlock* param_1) {
    if (param_1->m0_flags & 1) {
        param_1->m18.clear();
        param_1->m0_flags &= ~1;
    }
}

void freeMechaModelBlocks(sMechaInitVar4* param_1, int param_2) {
    if (param_1) {
        for (int i = 0; i < param_1->m4; i++) {
            if (param_1->m0.size()) {
                if (param_1->m0[i]) {
                    if (param_2) {
                        freeModelBlock(param_1->m0[i]);
                    }
                }
            }
        }
        if (param_1->m0.size()) {
            param_1->m0.clear();
        }
    }
}

void mechaInitNewMecha(int entryId, ushort flags, sMechaDataTable2* pData2, sMechaDataTable1* pData1, ushort tpageX, ushort tpageY, ushort clutX, short clutY, SVECTOR* param_9)
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
        doPointerRelocation((uint*)pData2);
        doPointerRelocation(*(uint**)(pData2 + 8));
        puVar12 = *(uint**)(pData2 + 4);
        doPointerRelocation(puVar12);
        doPointerRelocation((uint*)puVar12[1]);
        */

#if 0
        MissingCode();
        psVar9 = pData2->m8;
        if ((psVar9->mC != psVar9->m8_seq) && (iVar3 = findSequenceInLoadedList(psVar9->m8_seq, 0), iVar3 == 0)) {
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

    static sModel* mechaModelBlocksBufferForLoading = nullptr;

    int mechaInitVar4Counter = 0;
    if ((flags & 1) == 0) {
        int uVar29 = 0;
        if ((flags & 0x40) == 0) {
            uVar29 = ((pLoadedMecha->m4A & 4) == 0);
        }

        // TODO: convert that properly
        std::span<u8> tempSpan(pData1->m4_textures.m_raw.begin(), pData1->m4_textures.m_raw.size());
        uploadTextureToVram(tempSpan.begin(), uVar29, tpageX, tpageY, uVar29, clutX, clutY);
        mechaModelBlocksBufferForLoading = new sModel;
        *mechaModelBlocksBufferForLoading = pData1->m8_modelBlocks;

        int iVar2;
        do {
            iVar2 = mechaInitVar4Counter + 1;
            if (mechaInitVar4[mechaInitVar4Counter].m0.size() == 0)
                break;
            mechaInitVar4Counter = iVar2;
        } while (iVar2 < 8);

        sMechaModel_init(*mechaModelBlocksBufferForLoading, &mechaInitVar4[mechaInitVar4Counter]);
    }

    pLoadedMecha->m0 = &mechaInitVar4[mechaInitVar4Counter];


    if ((flags & 0x40) == 0) {
        if ((pLoadedMecha->m4A & 4) == 0) {
            pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 1, tpageX, tpageY, clutX, clutY);

        }
        else
        {
            pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 2, 0, 0, 0, 0, 0);
        }
    }
    else
    {
        pLoadedMecha->m4_bones = processMechaMesh(pLoadedMecha->m0, pData1->mC, 0, 0, 0, 0, 0, 0);
    }

    if (param_9 != nullptr) {
        (*pLoadedMecha->m4_bones)[0].m5C_translation[0] = param_9->vx;
        (*pLoadedMecha->m4_bones)[0].m5C_translation[1] = param_9->vy;
        (*pLoadedMecha->m4_bones)[0].m5C_translation[2] = param_9->vz;
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

    pLoadedMecha->m1C_scale = pData1->m10->m4->m8;
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
        initModel5(mechaModelBlocksBufferForLoading);

        sModel* newModel = new sModel;
        *newModel = *mechaModelBlocksBufferForLoading;
        // Fixup
        for (int i = 0; i < newModel->m0_numBlocks; i++) {
            newModel->m10_blocks[i].m_baseItForRelocation = newModel->mRawData.data();
        }

        delete mechaModelBlocksBufferForLoading;
        mechaModelBlocksBufferForLoading = nullptr;

        freeMechaModelBlocks(pLoadedMecha->m0, 0);

        sMechaModel_init(*newModel, pLoadedMecha->m0);
        pLoadedMecha->mA8 = newModel;
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

        mechaDataTable2[i] = new sMechaDataTable2;
        mechaDataTable2[i]->init(mechaDataTable2_raw[i].getRawData().begin());
        mechaDataTable1[i] = new sMechaDataTable1;
        mechaDataTable1[i]->init(mechaDataTable1_raw[i].getRawData().begin());

        mechaInitNewMecha(i, 0, mechaDataTable2[i], mechaDataTable1[i], ((i + mechaList2[i]) * -0x40 + 0x240), 0x100, 0, i + 0xFC, &initMechaTempVar[i]);
        delete mechaDataTable1[i];
        mechaDataTable1[i] = nullptr;
        mechaDataTable1_raw[i].clear();
        mechaList3[i] = loadedMechas[i]->m1C_scale;
    }

    resetMemoryAllocStats(8, 0);
}

void loadMechaOVerlayAndRegisterMechas(void)
{
    loadMechaOverlay_start();
    loadMechaOverlay_finalize();
}

void freeMecha(int index) {
    if(loadedMechas[index])
    {
        mechaOP_8(&mechaInitVar2, loadedMechas[index]->m4_bones); // hack!
        freeAnimTracks2(&mechaInitVar2, loadedMechas[index]->m4_bones, -1); // hack!
        loadedMechas[index] = nullptr;
    }
    MissingCode(); // don't forget to also clear mechaDataTable1 mechaDataTable2 (raw versions)
}

void freeMechaModule()
{
    for (int i = 0; i < 10; i++) {
        freeMecha(i);
    }
    freeMechaInitVar2(&mechaInitVar2);
    freeMechaInitVar3(&mechaInitVar3);
}

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
        std::vector<sMechaBone>::iterator m4It = (*pMecha->m4_bones).begin();

        CompMatrix(pMatrix, &m4It->mC_localMatrix, &matrixSP_1f800040);
        if ((pMecha->m4A & 1) == 0) {
            MissingCode();
        }
        MulMatrix0(param_3, &m4It->m2C_boneFinalMatrix, &matrixSP_1f800020);
        int numBones = m4It->mA_numBones;
        m4It++;
        for (int i = 1; i < numBones; i++) {
            if ((m4It->m8_geometryId != -1) && (m4It->m7_isEnabled)) {
                MulMatrix0(&matrixSP_1f800020, &m4It->m2C_boneFinalMatrix, &matrixSP_1f800000);
                //SetLightMatrix(&matrixSP_1f800000);
                CompMatrix(&matrixSP_1f800040, &m4It->m2C_boneFinalMatrix, &matrixSP_1f800000);
                MissingCode();
                SetTransMatrix(&matrixSP_1f800000);
                SetRotMatrix(&matrixSP_1f800000);
                submitModelForRendering(pMecha->m0->m0[m4It->m8_geometryId], m4It->m68[oddOrEven], OT.begin(), param_4);
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
                    m0 = &(*loadedMechas[param_1->m5C]->m4_bones)[0].m2C_boneFinalMatrix;
                }
                else {
                    MulMatrix0(&(*loadedMechas[param_1->m5C]->m4_bones)[0].m2C_boneFinalMatrix, &(*loadedMechas[param_1->m5C]->m4_bones)[param_1->m5E].m2C_boneFinalMatrix, &scratchPadMatrix);
                }
                MulRotationMatrix(m0, &(*param_1->m4_bones)[0].mC_localMatrix);
                MulRotationMatrix(m0, &(*param_1->m4_bones)[0].m2C_boneFinalMatrix);
            }
            if (param_1->m5E == 0) {
                m0 = &(*loadedMechas[param_1->m5C]->m4_bones)[0].mC_localMatrix;
            }
            else {
                CompMatrix(&(*loadedMechas[param_1->m5C]->m4_bones)[0].mC_localMatrix, &(*loadedMechas[param_1->m5C]->m4_bones)[param_1->m5E].m2C_boneFinalMatrix, m0);
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

void updateMechAnimSetupRootAltitude(sLoadedMechas* param_1)
{
    if (param_1->m36 == 0) {
        (*param_1->m4_bones)[0].m5C_translation[1] = param_1->m60;
    }
    return;
}

void updateMechAnimSub2(std::vector<sMechaBone>& param_1, int param_2) {
    param_1[0].m2C_boneFinalMatrix.t[0] = param_1[0].m5C_translation[0];
    param_1[0].m2C_boneFinalMatrix.t[1] = param_1[0].m5C_translation[1];
    param_1[0].m2C_boneFinalMatrix.t[2] = param_1[0].m5C_translation[2];

    if (param_1[0].m6 == 0) {
        createRotationMatrix(&param_1[0].m54_rotationAngles, &param_1[0].m2C_boneFinalMatrix);
    }
    else {
        RotMatrixYXZ(&param_1[0].m54_rotationAngles, &param_1[0].m2C_boneFinalMatrix);
    }

    MATRIX MATRIX_1f800000;
    MATRIX_1f800000.m[0][1] = 0;
    MATRIX_1f800000.m[0][2] = 0;
    MATRIX_1f800000.m[1][0] = 0;
    MATRIX_1f800000.m[0][0] = (short)(param_2 * param_1[0].m4C_scale[0] >> 0xc);
    MATRIX_1f800000.m[1][2] = 0;
    MATRIX_1f800000.m[2][0] = 0;
    MATRIX_1f800000.m[2][1] = 0;
    MATRIX_1f800000.m[1][1] = (short)(param_2 * param_1[0].m4C_scale[1] >> 0xc);
    MATRIX_1f800000.m[2][2] = (short)(param_2 * param_1[0].m4C_scale[2] >> 0xc);

    MulMatrix0(&param_1[0].m2C_boneFinalMatrix, &MATRIX_1f800000, &param_1[0].mC_localMatrix);

    param_1[0].mC_localMatrix.t = param_1[0].m2C_boneFinalMatrix.t;

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        sMechaBone* pBone = &param_1[i];

        if (pBone->m0_parentBone) {
            if (pBone->m0_parentBone->m5_needRotationUpdate) {
                pBone->m5_needRotationUpdate = 1;
            }
            if (pBone->m0_parentBone->m4_needTranslationUpdate) {
                pBone->m4_needTranslationUpdate = 1;
            }
        }

        if (pBone->m5_needRotationUpdate) {
            if (pBone->m6 == 0) {
                createRotationMatrix(&pBone->m54_rotationAngles, &pBone->mC_localMatrix);
            }
            else {
                RotMatrixYXZ(&pBone->m54_rotationAngles, &pBone->mC_localMatrix);
            }

            MATRIX scaleMatrix;
            scaleMatrix.m[0][0] = pBone->m4C_scale[0];
            scaleMatrix.m[0][1] = 0;
            scaleMatrix.m[0][2] = 0;
            scaleMatrix.m[1][0] = 0;
            scaleMatrix.m[1][1] = pBone->m4C_scale[1];
            scaleMatrix.m[1][2] = 0;
            scaleMatrix.m[2][0] = 0;
            scaleMatrix.m[2][1] = 0;
            scaleMatrix.m[2][2] = pBone->m4C_scale[2];
            MulMatrix0(&pBone->mC_localMatrix, &scaleMatrix, &pBone->mC_localMatrix);
            if (pBone->m0_parentBone) {
                MATRIX parentScaleMatrix;
                parentScaleMatrix.m[0][0] = 0x1000000 / pBone->m0_parentBone->m4C_scale[0];
                parentScaleMatrix.m[0][1] = 0;
                parentScaleMatrix.m[0][2] = 0;
                parentScaleMatrix.m[1][0] = 0;
                parentScaleMatrix.m[1][1] = 0x1000000 / pBone->m0_parentBone->m4C_scale[1];
                parentScaleMatrix.m[1][2] = 0;
                parentScaleMatrix.m[2][0] = 0;
                parentScaleMatrix.m[2][1] = 0;
                parentScaleMatrix.m[2][2] = 0x1000000 / pBone->m0_parentBone->m4C_scale[2];
                MulMatrix0(&parentScaleMatrix, &pBone->mC_localMatrix, &pBone->mC_localMatrix);
            }
        }

        if (pBone->m4_needTranslationUpdate) {
            pBone->mC_localMatrix.t[0] = pBone->m5C_translation[0];
            pBone->mC_localMatrix.t[1] = pBone->m5C_translation[1];
            pBone->mC_localMatrix.t[2] = pBone->m5C_translation[2];

            if (pBone->m0_parentBone == nullptr) {
                pBone->m2C_boneFinalMatrix = pBone->mC_localMatrix;
            }
            else
            {
                CompMatrix(&pBone->m0_parentBone->m2C_boneFinalMatrix, &pBone->mC_localMatrix, &pBone->m2C_boneFinalMatrix);
            }
        }
    }

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        param_1[i].m4_needTranslationUpdate = 0;
        param_1[i].m5_needRotationUpdate = 0;
    }
}


void updateMechAnimSub1(std::vector<sMechaBone>& param_1, int scale) {
    param_1[0].m2C_boneFinalMatrix.t[0] = param_1[0].m5C_translation[0];
    param_1[0].m2C_boneFinalMatrix.t[1] = param_1[0].m5C_translation[1];
    param_1[0].m2C_boneFinalMatrix.t[2] = param_1[0].m5C_translation[2];

    if (param_1[0].m6 == 0) {
        createRotationMatrix(&param_1[0].m54_rotationAngles, &param_1[0].m2C_boneFinalMatrix);
    }
    else {
        RotMatrixYXZ(&param_1[0].m54_rotationAngles, &param_1[0].m2C_boneFinalMatrix);
    }

    MATRIX scaleMatrix;
    scaleMatrix.m[0][1] = 0;
    scaleMatrix.m[0][2] = 0;
    scaleMatrix.m[1][0] = 0;
    scaleMatrix.m[1][2] = 0;
    scaleMatrix.m[2][0] = 0;
    scaleMatrix.m[2][1] = 0;
    scaleMatrix.m[0][0] = (short)(scale * param_1[0].m4C_scale[0] >> 0xc);
    scaleMatrix.m[1][1] = (short)(scale * param_1[0].m4C_scale[1] >> 0xc);
    scaleMatrix.m[2][2] = (short)(scale * param_1[0].m4C_scale[2] >> 0xc);

    MulMatrix0(&param_1[0].m2C_boneFinalMatrix, &scaleMatrix, &param_1[0].mC_localMatrix);

    param_1[0].mC_localMatrix.t = param_1[0].m2C_boneFinalMatrix.t;

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        sMechaBone* pBone = &param_1[i];
        if (pBone->m5_needRotationUpdate) {
            if (pBone->m6 == 0) {
                createRotationMatrix(&pBone->m54_rotationAngles, &pBone->mC_localMatrix);
            }
            else {
                RotMatrixYXZ(&pBone->m54_rotationAngles, &pBone->mC_localMatrix);
            }
            pBone->m5_needRotationUpdate = 0;
        }
        if ((pBone->m0_parentBone != nullptr) && (pBone->m0_parentBone->m4_needTranslationUpdate == 1)) {
            pBone->m4_needTranslationUpdate = 1;
        }

        if (pBone->m4_needTranslationUpdate) {
            // TODO: recheck all of this!
            pBone->mC_localMatrix.t[0] = pBone->m5C_translation[0];
            pBone->mC_localMatrix.t[1] = pBone->m5C_translation[1];
            pBone->mC_localMatrix.t[2] = pBone->m5C_translation[2];

            if (pBone->m0_parentBone == nullptr) {
                pBone->m2C_boneFinalMatrix = pBone->mC_localMatrix;
            }
            else
            {
                CompMatrix(&pBone->m0_parentBone->m2C_boneFinalMatrix, &pBone->mC_localMatrix, &pBone->m2C_boneFinalMatrix);
            }
        }
    }

    for (int i = 1; i < param_1[0].mA_numBones; i++) {
        param_1[i].m4_needTranslationUpdate = 0;
    }
}

u32 updateMechAnimSub3(sMechaInitVar2* param_1, std::vector<sMechaBone>& param_2, uint param_3, int speed) {
    u32 returnFlag = 0;
    for (int i = 0; i < param_2[0].mA_numBones; i++) {
        if (param_2[i].m70_animTracks[0]) {
            sMechaInitVar2Sub* pTrack = param_2[i].m70_animTracks[0];
            switch (pTrack->m2 & 0xF) {
            case 0: // s16
                if ((pTrack->m2 & 0x10) == 0) {
                    param_2[i].m54_rotationAngles[0] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                if ((pTrack->m2 & 0x20) == 0) {
                    param_2[i].m54_rotationAngles[1] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                if ((pTrack->m2 & 0x40) == 0) {
                    param_2[i].m54_rotationAngles[2] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                break;
            case 1: // s8 (unless it's a 0x80)
                if ((pTrack->m2 & 0x10) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m54_rotationAngles[0] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m54_rotationAngles[0] += psVar9;
                    }
                }
                if ((pTrack->m2 & 0x20) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m54_rotationAngles[1] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m54_rotationAngles[1] += psVar9;
                    }
                }
                if ((pTrack->m2 & 0x40) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m54_rotationAngles[2] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m54_rotationAngles[2] += psVar9;
                    }
                }
                break;
            case 3:
            {
                s32 ratio = ((((s32)pTrack->m10_currentTime + 1) * 0x10000) >> 0x10);
                param_2[i].m54_rotationAngles[0] = pTrack->m4_s16 + (ratio * pTrack->mA_s16) / pTrack->m12_maxTime;
                param_2[i].m54_rotationAngles[1] = pTrack->m6_s16 + (ratio * pTrack->mC) / pTrack->m12_maxTime;
                param_2[i].m54_rotationAngles[2] = pTrack->m8_s16 + (ratio * pTrack->mE) / pTrack->m12_maxTime;
                break;
            }
            break;
            default:
                assert(0);
            }

            pTrack->m10_currentTime++;
            if (pTrack->m10_currentTime < pTrack->m12_maxTime) {
                if (pTrack->m3 == param_3) {
                    returnFlag |= 0x1;
                }
                returnFlag |= 0x100;
            }
            else if (pTrack->m1_isLooping == '\0') {
                if (pTrack->m3 == param_3) {
                    returnFlag = returnFlag | 2;
                }
                returnFlag = returnFlag | 0x200;
                releaseMechaAnimTrack(param_1, pTrack);
                param_2[i].m70_animTracks[0] = nullptr;
            }
            else {
                //looping
                if (pTrack->m3 == param_3) {
                    returnFlag = returnFlag | 4;
                }
                returnFlag = returnFlag | 0x400;
                if ((pTrack->m2 & 0xf) < 3) {
                    pTrack->m10_currentTime = 0;
                    pTrack->m8 = pTrack->m4;
                }
                else {
                    pTrack->m10_currentTime = -1;
                    if ((pTrack->m2 & 0xf) == 5) {
                        pTrack->mA_s16 = 0;
                        pTrack->mC = 0;
                        pTrack->mE = 0;
                    }
                }
            }

            param_2[i].m4_needTranslationUpdate = 1;
            param_2[i].m5_needRotationUpdate = 1;
        }
        if (param_2[i].m70_animTracks[1]) {
            sMechaInitVar2Sub* pTrack = param_2[i].m70_animTracks[1];
            switch (pTrack->m2 & 0xF) {
            case 0: // s16
                if ((pTrack->m2 & 0x10) == 0) {
                    param_2[i].m5C_translation[0] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                if ((pTrack->m2 & 0x20) == 0) {
                    param_2[i].m5C_translation[1] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                if ((pTrack->m2 & 0x40) == 0) {
                    param_2[i].m5C_translation[2] += READ_LE_S16(pTrack->m8);
                    pTrack->m8 += 2;
                }
                break;
            case 1: // s8 (unless it's a 0x80)
                if ((pTrack->m2 & 0x10) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m5C_translation[0] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m5C_translation[0] += psVar9;
                    }
                }
                if ((pTrack->m2 & 0x20) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m5C_translation[1] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m5C_translation[1] += psVar9;
                    }
                }
                if ((pTrack->m2 & 0x40) == 0) {
                    s8 psVar9 = READ_LE_S8(pTrack->m8);
                    pTrack->m8++;
                    if (psVar9 == (s8)0x80) {
                        param_2[i].m5C_translation[2] = READ_LE_S16(pTrack->m8);
                        pTrack->m8 += 2;
                    }
                    else {
                        param_2[i].m5C_translation[2] += psVar9;
                    }
                }
                break;
            case 3:
            {
                s32 ratio = ((((s32)pTrack->m10_currentTime + 1) * 0x10000) >> 0x10);
                param_2[i].m5C_translation[0] = pTrack->m4_s16 + (ratio * pTrack->mA_s16) / pTrack->m12_maxTime;
                param_2[i].m5C_translation[1] = pTrack->m6_s16 + (ratio * pTrack->mC) / pTrack->m12_maxTime;
                param_2[i].m5C_translation[2] = pTrack->m8_s16 + (ratio * pTrack->mE) / pTrack->m12_maxTime;
                break;
            }
            default:
                assert(0);
            }

            pTrack->m10_currentTime++;
            if (pTrack->m10_currentTime < pTrack->m12_maxTime) {
                if (pTrack->m3 == param_3) {
                    returnFlag |= 0x1;
                }
                returnFlag |= 0x100;
            }
            else if (pTrack->m1_isLooping == '\0') {
                // no loop, stop the track
                if (pTrack->m3 == param_3) {
                    returnFlag = returnFlag | 2;
                }
                returnFlag = returnFlag | 0x200;
                releaseMechaAnimTrack(param_1, pTrack);
                param_2[i].m70_animTracks[1] = nullptr;
            }
            else {
                //looping
                if (pTrack->m3 == param_3) {
                    returnFlag = returnFlag | 4;
                }
                returnFlag = returnFlag | 0x400;
                if ((pTrack->m2 & 0xf) < 3) {
                    pTrack->m10_currentTime = 0;
                    pTrack->m8 = pTrack->m4;
                }
                else {
                    assert(0);
                }
            }

            param_2[i].m4_needTranslationUpdate = 1;
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
            updateMechAnimSetupRootAltitude(param_1);
            if (param_1->m37 == 0) {
                updateMechAnimSub1(*param_1->m4_bones, (int)param_1->m1C_scale);
            }
            else {
                updateMechAnimSub2(*param_1->m4_bones, (int)param_1->m1C_scale);
            }
            for(int i=0; i<iterations; i++) {
                updateFlag |= updateMechAnimSub3(param_2, *param_1->m4_bones, param_1->m3C, (int)param_1->m1C_scale);
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
            loadedMechas[i]->m11C_previousTranslation = (*loadedMechas[i]->m4_bones)[0].m5C_translation;
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
            loadedMechas[i]->m128_deltaTranslation[0] = loadedMechas[i]->m11C_previousTranslation[0] - (*loadedMechas[i]->m4_bones)[0].m5C_translation[0];
            loadedMechas[i]->m128_deltaTranslation[1] = loadedMechas[i]->m11C_previousTranslation[1] - (*loadedMechas[i]->m4_bones)[0].m5C_translation[1];
            loadedMechas[i]->m128_deltaTranslation[2] = loadedMechas[i]->m11C_previousTranslation[2] - (*loadedMechas[i]->m4_bones)[0].m5C_translation[2];
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
            renderMechas(&g_currentProjectionMatrix, &mechaFieldArgs2[0], pCurrentFieldRenderingContext->mCC_OT, g_frameOddOrEven, 1);
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

            std::vector<sMechaBone>::iterator m4It = (*pMecha->m4_bones).begin();
            int numBones = m4It->mA_numBones;
            m4It++;
            for (int i = 1; i < numBones; i++) {
                if ((m4It->m8_geometryId != -1) && (m4It->m7_isEnabled)) {
                    float finalMatrix[16];
                    convertMatrixToBgfx(&m4It->m2C_boneFinalMatrix, finalMatrix);
                    pMecha->m0->m0[m4It->m8_geometryId]->getBgfxData()->bgfxRender(pMecha->m0->m0[m4It->m8_geometryId], viewId, finalMatrix);
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
            *param_1 = (*pMecha->m4_bones)[0].mC_localMatrix;
        }
        else {
            CompMatrix(&(*pMecha->m4_bones)[0].mC_localMatrix, &(*pMecha->m4_bones)[boneIndex].m2C_boneFinalMatrix, param_1);
        }
    }
}