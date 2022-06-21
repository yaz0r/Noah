#include "noahLib.h"
#include "worldmapWorldStreaming.h"
#include "worldmap.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "kernel/isoManager.h"
#include "kernel/gte.h"
#include "kernel/trigo.h"

/*
At any point in time we have 5x5 megatiles active
Each megatile is a 4x4 tile
Each tile is a 8x8 quad
*/

s32 worldmapWaterWave2;
s32 worldmapWaterWave;

std::array<u16, 81> worldmapGrid;
std::array<u16, 81> worldmapGrid2;

struct sWorldmapCellFromCDSub {
    s32 m0_sector;
    s32 m4_size;
    u8* m8_buffer;
};

struct sWorldmapCellFromCD {
    std::array<sWorldmapCellFromCDSub, 88> m0;
};
std::array<sWorldmapCellFromCD*, 16> loadedWorldmapCellsFromCD;

struct sWorldmapCellFromPC {

};
std::array<sWorldmapCellFromPC*, 16> loadedWorldmapCellsFromPC;

s32 worldmapNextCellIndex;
s32 worldmapCellLoadSlot;
s32 worldmapStreamingStateMachineState;
s32 worldmapStreamingStateMachineNumPendingLoads;

std::array<u8*, 256> worldmapChunks;

void loadWorldmapChunkFromCD(sWorldmapCellFromCD* param_1) {
    // NOTE: this is not how the original code works at all
    std::array<sWorldmapCellFromCDSub, 88>::iterator it = param_1->m0.begin();
    while (it->m0_sector) {
        std::vector<u8> buffer;
        c_isoManager::getCurrentDisc()->readData(it->m0_sector, it->m4_size, buffer);
        assert(buffer.size() >= it->m4_size);
        memcpy(it->m8_buffer, &buffer[0], it->m4_size);

        it++;
    }

    worldmapStreamingStateMachineState = 5; // go to the done state
}

void loadWorldmapChunkFromPC(sWorldmapCellFromPC* param_1) {
    MissingCode();
    assert(0);
}

int updateWorldmapStreamingStateMachine(void)
{
    int iVar1;

    switch (worldmapStreamingStateMachineState) {
    case 0:
        iVar1 = 0;
        break;
    case 4:
        worldmapStreamingStateMachineNumPendingLoads = worldmapStreamingStateMachineNumPendingLoads + -1;
        if (worldmapStreamingStateMachineNumPendingLoads == 0) {
            worldmapStreamingStateMachineState = worldmapStreamingStateMachineState + 1;
        }
    case 1:
    case 2:
    case 3:
        iVar1 = 1;
        break;
    case 5:
        worldmapStreamingStateMachineState = 0;
        loadedWorldmapCellsFromCD[worldmapCellLoadSlot] = (sWorldmapCellFromCD*)0x0;
        iVar1 = 2;
        worldmapCellLoadSlot = worldmapCellLoadSlot + 1 & 0xf;
        break;
    default:
        iVar1 = 3;
    }
    return iVar1;
}

int loadWorldmapChunk(void)
{
    int iVar1;
    int iVar2;
    sWorldmapCellFromPC** ppsVar3;

    iVar1 = getCdromFnd();
    iVar2 = getCdromFnd();
    if (iVar1 == 0 || iVar2 == -1) {
        iVar1 = updateWorldmapStreamingStateMachine();
        if ((iVar1 == 0) && (loadedWorldmapCellsFromCD[worldmapCellLoadSlot] != (sWorldmapCellFromCD*)0x0)) {
            loadWorldmapChunkFromCD(loadedWorldmapCellsFromCD[worldmapCellLoadSlot]);
        }
    }
    else {
        iVar1 = 0;
        if (loadedWorldmapCellsFromPC[worldmapCellLoadSlot] != (sWorldmapCellFromPC*)0x0) {
            loadWorldmapChunkFromPC(loadedWorldmapCellsFromPC[worldmapCellLoadSlot]);
            ppsVar3 = &loadedWorldmapCellsFromPC[worldmapCellLoadSlot];
            worldmapCellLoadSlot = worldmapCellLoadSlot + 1 & 0xf;
            *ppsVar3 = (sWorldmapCellFromPC*)0x0;
            iVar1 = 0;
        }
    }
    return iVar1;
}

void streamWorldmap0(void)
{
    int iVar1;
    int iVar2;
    sWorldmapCellFromCD* psVar3;
    sWorldmapCellFromPC* psVar4;

    iVar1 = getCdromFnd();
    iVar2 = getCdromFnd();
    if (iVar1 == 0 || iVar2 == -1) {
        psVar3 = loadedWorldmapCellsFromCD[worldmapNextCellIndex];
        while (psVar3 != nullptr) {
            VSync(0);
            loadWorldmapChunk();
            psVar3 = loadedWorldmapCellsFromCD[worldmapNextCellIndex];
        }
    }
    else {
        psVar4 = loadedWorldmapCellsFromPC[worldmapNextCellIndex];
        while (psVar4 != nullptr) {
            VSync(0);
            loadWorldmapChunk();
            psVar4 = loadedWorldmapCellsFromPC[worldmapNextCellIndex];
        }
    }
    return;
}

std::vector<sWorldmapCellFromCD> worldmapCellsFromCD;
std::vector<sWorldmapCellFromPC> worldmapCellsFromPC;
std::vector<u8> worldmapExtraData;
std::array<u8, 8> worldmapStreamingCDSyncStatus;
s32 worldmapCellLoadingPending;

void allocateWorldmapData(void)
{
    int iVar1;
    int iVar2;
    sWorldmapCellFromCD** ppsVar3;
    sWorldmapCellFromPC** ppsVar4;
    int iVar5;
    byte* pbVar6;

    iVar1 = getCdromFnd();
    iVar2 = getCdromFnd();
    iVar5 = 0xf;
    if (iVar1 == 0 || iVar2 == -1) {
        ppsVar3 = &loadedWorldmapCellsFromCD[0xf];
        worldmapCellLoadSlot = 0;
        worldmapNextCellIndex = 0;
        worldmapStreamingStateMachineState = 0;
        do {
            *ppsVar3 = (sWorldmapCellFromCD*)0x0;
            iVar5 = iVar5 + -1;
            ppsVar3 = ppsVar3 + -1;
        } while (-1 < iVar5);
        worldmapCellsFromCD.resize(0x10);
        worldmapExtraData.resize(0x800);
        iVar1 = 7;
        pbVar6 = &worldmapStreamingCDSyncStatus[7];
        worldmapCellLoadingPending = 0;
        do {
            *pbVar6 = 0;
            iVar1 = iVar1 + -1;
            pbVar6 = pbVar6 + -1;
        } while (-1 < iVar1);
    }
    else {
        ppsVar4 = &loadedWorldmapCellsFromPC[0xf];
        worldmapCellLoadSlot = 0;
        worldmapNextCellIndex = 0;
        worldmapStreamingStateMachineState = 0;
        do {
            *ppsVar4 = (sWorldmapCellFromPC*)0x0;
            iVar5 = iVar5 + -1;
            ppsVar4 = ppsVar4 + -1;
        } while (-1 < iVar5);
        worldmapCellsFromPC.resize(0x10);
        worldmapExtraData.resize(0x800);
        iVar1 = 7;
        pbVar6 = &worldmapStreamingCDSyncStatus[7];
        worldmapCellLoadingPending = 0;
        do {
            *pbVar6 = 0;
            iVar1 = iVar1 + -1;
            pbVar6 = pbVar6 + -1;
        } while (-1 < iVar1);
    }
    return;
}

int setWorldmapCellTriangleValue(int param_1, int param_2, u8* param_3)
{
    int iVar1;
    sWorldmapCellFromCDSub* psVar2;

    if (worldmapCellLoadingPending < 0x58) {
        iVar1 = 0;
        psVar2 = &worldmapCellsFromCD[worldmapNextCellIndex].m0[worldmapCellLoadingPending];
        worldmapCellLoadingPending = worldmapCellLoadingPending + 1;
        psVar2->m0_sector = param_1;
        psVar2->m4_size = param_2;
        psVar2->m8_buffer = param_3;
    }
    else {
        iVar1 = -1;
    }
    return iVar1;
}

std::array<s16, 4> USHORT_ARRAY_worldmap__8009bbac = { {0,0x8,0x48, 0x50} };

int getFilenameFromPC(int index) {
    assert(0);
    return 0;
}

s32 setWorldmapCellQuadValue(int param_1, int param_2, int param_3, byte* param_4)
{
    assert(0);
    return -1;
}

void sortWorldmapStreamingEntries(sWorldmapCellFromCD* param_1)
{
    if (param_1->m0[1].m0_sector != 0) {

        sWorldmapCellFromCDSub* next = &param_1->m0[1];
        sWorldmapCellFromCDSub* current = &param_1->m0[0];

        do 
        {
            if (next->m0_sector < current->m0_sector) {
                sWorldmapCellFromCDSub temp = next[-1];
                next[-1] = *next;
                *next = temp;

                if (&param_1->m0[0] < current) {
                    next = next + -1;
                    current = current + -1;
                }
            }
            else
            {
                next++;
                current++;
            }

        } while (next->m0_sector != 0);
    }
}

int finalizeLoadingWorldmapCell(void)
{
    int iVar1;
    sWorldmapCellFromCD** ppsVar2;
    sWorldmapCellFromCD* psVar3;

    psVar3 = &worldmapCellsFromCD[worldmapNextCellIndex];
    if ((psVar3->m0[0].m0_sector == 0) || (loadedWorldmapCellsFromCD[worldmapNextCellIndex] != (sWorldmapCellFromCD*)0x0)) {
        worldmapCellLoadingPending = 0;
        iVar1 = -1;
    }
    else {
        sortWorldmapStreamingEntries(psVar3);
        worldmapCellLoadingPending = 0;
        ppsVar2 = &loadedWorldmapCellsFromCD[worldmapNextCellIndex];
        worldmapNextCellIndex = worldmapNextCellIndex + 1 & 0xf;
        *ppsVar2 = psVar3;
        iVar1 = 0;
    }
    return iVar1;
}

int finalizeLoadingWorldmapCellFromPC(void) {
    assert(0);
    return -1;
}

void streamWorldmap1(void)
{
    ushort uVar1;
    bool bVar2;
    bool bVar3;
    byte* puVar4;
    byte* pbVar4;
    ushort* puVar5;
    int iVar6;
    byte** ppbVar7;
    ushort* puVar8;
    short* psVar9;
    int iVar10;
    int iVar11;
    int iVar12;
    int iVar13;
    int iVar14;

    iVar10 = 0;
    puVar8 = &worldmapGrid2[0];
    do {
        iVar6 = (int)(short)*puVar8;
        iVar12 = 0;
        puVar5 = &worldmapGrid[0];
        if (worldmapChunks[iVar6] != (byte*)0x0) {
            do {
                if ((short)*puVar5 == iVar6) break;
                iVar12 = iVar12 + 1;
                puVar5 = puVar5 + 1;
            } while (iVar12 < 0x51);
            if (iVar12 == 0x51) {
                free(worldmapChunks[iVar6]);
                worldmapChunks[iVar6] = (byte*)0x0;
            }
        }
        iVar10 = iVar10 + 1;
        puVar8 = puVar8 + 1;
    } while (iVar10 < 0x51);
    bVar2 = false;
    bVar3 = false;
    iVar10 = getCdromFnd();
    iVar6 = getCdromFnd();
    if (iVar10 == 0 || iVar6 == -1) {
        iVar12 = 1;
        iVar10 = getFileStartSector(worldmapFile9);
        iVar6 = 1;
        do {
            iVar11 = 6;
            puVar8 = &worldmapGrid[iVar6];
            do {
                uVar1 = *puVar8;
                if (worldmapChunks[(short)uVar1] == (byte*)0x0) {
                    puVar4 = (byte*)malloc(0x710);
                    worldmapChunks[(short)uVar1] = puVar4;
                    setWorldmapCellTriangleValue(iVar10 + (short)uVar1, 0x710, puVar4);
                    bVar3 = true;
                }
                bVar2 = false;
                iVar11 = iVar11 + -1;
                puVar8 = puVar8 + 1;
            } while (iVar11 != -1);
            iVar12 = iVar12 + -1;
            iVar6 = 0x49;
        } while (iVar12 != -1);
        iVar12 = 1;
        iVar10 = getFileStartSector(worldmapFile10);
        iVar6 = 9;
        do {
            iVar11 = 6;
            puVar8 = &worldmapGrid[iVar6];
            do {
                iVar6 = (int)(short)*puVar8;
                if (worldmapChunks[iVar6] == (byte*)0x0) {
                    pbVar4 = (byte*)malloc(0x710);
                    iVar13 = iVar6 / worldmapSizeX;
                    if (worldmapSizeX == 0) {
                        assert(0);
                    }
                    if ((worldmapSizeX == -1) && (iVar6 == -0x80000000)) {
                        assert(0);
                    }
                    iVar14 = (iVar6 % worldmapSizeX) * worldmapSizeY;
                    bVar2 = true;
                    worldmapChunks[iVar6] = pbVar4;
                    setWorldmapCellTriangleValue(iVar10 + iVar14 + iVar13, 0x710, pbVar4);
                }
                iVar11 = iVar11 + -1;
                puVar8 = puVar8 + 9;
            } while (iVar11 != -1);
            iVar12 = iVar12 + -1;
            iVar6 = 0x11;
        } while (iVar12 != -1);
        iVar10 = 0;
        psVar9 = &USHORT_ARRAY_worldmap__8009bbac[0];
        do {
            iVar6 = (int)(short)worldmapGrid[*psVar9];
            ppbVar7 = &worldmapChunks[iVar6];
            if (*ppbVar7 == (byte*)0x0) {
                pbVar4 = (byte*)malloc(0x710);
                *ppbVar7 = pbVar4;
                if (bVar3) {
                    iVar12 = getFileStartSector(worldmapFile9);
                    iVar12 = iVar12 + iVar6;
                    pbVar4 = *ppbVar7;
                }
                else {
                    if (!bVar2) goto LAB_worldmap__80099018;
                    iVar12 = getFileStartSector(worldmapFile10);
                    if (worldmapSizeX == 0) {
                        assert(0);
                    }
                    if ((worldmapSizeX == -1) && (iVar6 == -0x80000000)) {
                        assert(0);
                    }
                    pbVar4 = *ppbVar7;
                    iVar12 = iVar12 + (iVar6 % worldmapSizeX) * worldmapSizeY + iVar6 / worldmapSizeX;
                }
                setWorldmapCellTriangleValue(iVar12, 0x710, pbVar4);
            }
        LAB_worldmap__80099018:
            iVar10 = iVar10 + 1;
            psVar9 = psVar9 + 1;
        } while (iVar10 < 4);
        setWorldmapCellTriangleValue(0, 0, 0);
        finalizeLoadingWorldmapCell();
    }
    else {
        iVar12 = 1;
        iVar10 = getFilenameFromPC(worldmapFile9);
        iVar6 = 1;
        do {
            iVar11 = 6;
            puVar8 = &worldmapGrid[iVar6];
            do {
                uVar1 = *puVar8;
                if (worldmapChunks[(short)uVar1] == (byte*)0x0) {
                    pbVar4 = (byte*)malloc(0x710);
                    worldmapChunks[(short)uVar1] = pbVar4;
                    setWorldmapCellQuadValue(iVar10, (int)(short)uVar1 << 0xb, 0x710, pbVar4);
                    bVar3 = true;
                }
                iVar11 = iVar11 + -1;
                puVar8 = puVar8 + 1;
            } while (iVar11 != -1);
            iVar12 = iVar12 + -1;
            iVar6 = 0x49;
        } while (iVar12 != -1);
        iVar12 = 1;
        iVar10 = getFilenameFromPC(worldmapFile10);
        iVar6 = 9;
        do {
            iVar11 = 6;
            puVar8 = &worldmapGrid[iVar6];
            do {
                iVar6 = (int)(short)*puVar8;
                if (worldmapChunks[iVar6] == (byte*)0x0) {
                    pbVar4 = (byte*)malloc(0x710);
                    iVar13 = iVar6 / worldmapSizeX;
                    if (worldmapSizeX == 0) {
                        assert(0);
                    }
                    if ((worldmapSizeX == -1) && (iVar6 == -0x80000000)) {
                        assert(0);
                    }
                    iVar14 = (iVar6 % worldmapSizeX) * 0x800 * worldmapSizeY;
                    bVar2 = true;
                    worldmapChunks[iVar6] = pbVar4;
                    setWorldmapCellQuadValue(iVar10, iVar14 + iVar13 * 0x800, 0x710, pbVar4);
                }
                iVar11 = iVar11 + -1;
                puVar8 = puVar8 + 9;
            } while (iVar11 != -1);
            iVar12 = iVar12 + -1;
            iVar6 = 0x11;
        } while (iVar12 != -1);
        iVar6 = 0;
        iVar10 = 0;
        do {
            iVar10 = (int)(short)worldmapGrid[USHORT_ARRAY_worldmap__8009bbac[iVar10/2]];
            ppbVar7 = &worldmapChunks[iVar10];
            if (*ppbVar7 == (byte*)0x0) {
                pbVar4 = (byte*)malloc(0x710);
                *ppbVar7 = pbVar4;
                if (bVar3) {
                    iVar12 = getFilenameFromPC(worldmapFile9);
                    iVar10 = iVar10 << 0xb;
                    pbVar4 = *ppbVar7;
                }
                else {
                    if (!bVar2) goto LAB_worldmap__800992cc;
                    iVar12 = getFilenameFromPC(worldmapFile10);
                    if (worldmapSizeX == 0) {
                        assert(0);
                    }
                    if ((worldmapSizeX == -1) && (iVar10 == -0x80000000)) {
                        assert(0);
                    }
                    pbVar4 = *ppbVar7;
                    iVar10 = (iVar10 % worldmapSizeX) * 0x800 * worldmapSizeY + (iVar10 / worldmapSizeX) * 0x800;
                }
                setWorldmapCellQuadValue(iVar12, iVar10, 0x710, pbVar4);
            }
        LAB_worldmap__800992cc:
            iVar6 = iVar6 + 1;
            iVar10 = iVar6 * 2;
        } while (iVar6 < 4);
        setWorldmapCellQuadValue(0, 0, 0, 0);
        finalizeLoadingWorldmapCellFromPC();
    }
    return;
}

int isWorldStreamingDone(void)
{
    int iVar1;

    iVar1 = worldmapNextCellIndex - worldmapCellLoadSlot;
    if (iVar1 < 0) {
        iVar1 = iVar1 + 0x10;
    }
    return iVar1;
}

MATRIX streamingMatrixIdentity;

u32 worldmapGridUpdateMask;
SVECTOR worldmapGridPosition;

void setWorldmapGridUpdateMask(VECTOR* param_1)
{
    int iVar1;

    iVar1 = param_1->vx;
    worldmapGridUpdateMask = 0;
    if (iVar1 < -0x800000) {
        param_1->vx = iVar1 + 0x800000;
        worldmapGridUpdateMask = 4;
    }
    else if (0x800000 < iVar1) {
        param_1->vx = iVar1 + -0x800000;
        worldmapGridUpdateMask = 8;
    }
    if (param_1->vz < -0x800000) {
        worldmapGridUpdateMask = worldmapGridUpdateMask | 1;
        param_1->vz = param_1->vz + 0x800000;
    }
    else if (0x800000 < param_1->vz) {
        worldmapGridUpdateMask = worldmapGridUpdateMask | 2;
        param_1->vz = param_1->vz + -0x800000;
    }
    worldmapGridPosition.vx = (short)(param_1->vx >> 0x17) + 2;
    worldmapGridPosition.vz = (short)(param_1->vz >> 0x17) + 2;
    return;
}

void updateWorldmapGrids(VECTOR* param_1) {
    s32 iVar6 = worldmapSizeY;
    s32 iVar5 = worldmapSizeX;
    s32 posX = param_1->vx >> 0xc;
    if (posX < 0) {
        posX = posX + 7;
    }
    s32 posZ = param_1->vz >> 0xc;
    posX = (posX >> 3) + (worldmapGridPosition.vx + 2) * -0x100;
    if (posZ < 0) {
        posZ = posZ + 7;
    }
    posZ = (posZ >> 3) + (worldmapGridPosition.vz + 2) * -0x100;
    if (posX < 0) {
        posX = posX + worldmapSizeX * 0x100;
    }
    else if (worldmapSizeX * 0x100 < posX) {
        posX = posX + worldmapSizeX * -0x100;
    }
    if (posZ < 0) {
        posZ = posZ + worldmapSizeY * 0x100;
    }
    else if (worldmapSizeY * 0x100 < posZ) {
        posZ = posZ + worldmapSizeY * -0x100;
    }
    posZ = posZ >> 8;

    worldmapGrid2 = worldmapGrid;

    std::array<u16, 81>::iterator puVar12 = worldmapGrid.begin();
    s32 iVar13 = 8;
    do {
        if (iVar6 <= posZ) {
            posZ = 0;
        }
        s32 iVar9 = 8;
        s32 uVar4 = (short)posZ * (short)iVar5;
        s32 iVar14 = posX >> 8;
        do {
            s32 uVar7 = uVar4 + (short)iVar14;
            if (iVar5 <= iVar14) {
                iVar14 = 0;
                uVar7 = uVar4;
            }
            *puVar12 = uVar7;
            puVar12 = puVar12 + 1;
            iVar9 = iVar9 + -1;
            iVar14 = iVar14 + 1;
        } while (iVar9 != -1);
        iVar13 = iVar13 + -1;
        posZ = posZ + 1;
    } while (iVar13 != -1);
}

void triggerInitialWorldLoading(void) {
    ushort uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    byte* pbVar5;
    int iVar6;
    int iVar7;

    iVar2 = getCdromFnd();
    iVar3 = getCdromFnd();
    iVar7 = 3;
    if (iVar2 == 0 || iVar3 == -1) {
        iVar3 = 0x1b;
        iVar2 = getFileStartSector(worldmapFile9);
        do {
            iVar6 = 3;
            iVar4 = iVar3 + 3;
            do {
                uVar1 = worldmapGrid[iVar4];
                if (worldmapChunks[(short)uVar1] == (byte*)0x0) {
                    pbVar5 = (byte*)malloc(0x710);
                    worldmapChunks[(short)uVar1] = pbVar5;
                    setWorldmapCellTriangleValue(iVar2 + (short)uVar1, 0x710, pbVar5);
                }
                iVar6 = iVar6 + 1;
                iVar4 = iVar3 + iVar6;
            } while (iVar6 < 6);
            iVar7 = iVar7 + 1;
            iVar3 = iVar3 + 9;
        } while (iVar7 < 6);
        setWorldmapCellTriangleValue(0, 0, 0);
        iVar7 = 0;
        finalizeLoadingWorldmapCell();
        iVar3 = 0;
        do {
            iVar6 = 0;
            iVar4 = iVar3;
            do {
                uVar1 = worldmapGrid[iVar4];
                if (worldmapChunks[(short)uVar1] == (byte*)0x0) {
                    pbVar5 = (byte*)malloc(0x710);
                    worldmapChunks[(short)uVar1] = pbVar5;
                    setWorldmapCellTriangleValue(iVar2 + (short)uVar1, 0x710, pbVar5);
                }
                iVar6 = iVar6 + 1;
                iVar4 = iVar3 + iVar6;
            } while (iVar6 < 9);
            iVar7 = iVar7 + 1;
            iVar3 = iVar3 + 9;
        } while (iVar7 < 9);
        setWorldmapCellTriangleValue(0, 0, 0);
        finalizeLoadingWorldmapCell();
    }
    else {
        iVar7 = 0;
        iVar3 = 0;
        iVar2 = getFilenameFromPC(worldmapFile9);
        do {
            iVar6 = 0;
            iVar4 = iVar3;
            do {
                uVar1 = worldmapGrid[iVar4];
                if (worldmapChunks[(short)uVar1] == (byte*)0x0) {
                    pbVar5 = (byte*)malloc(0x710);
                    worldmapChunks[(short)uVar1] = pbVar5;
                    setWorldmapCellQuadValue(iVar2, (int)(short)uVar1 << 0xb, 0x710, pbVar5);
                }
                iVar6 = iVar6 + 1;
                iVar4 = iVar3 + iVar6;
            } while (iVar6 < 9);
            iVar7 = iVar7 + 1;
            iVar3 = iVar3 + 9;
        } while (iVar7 < 9);
        setWorldmapCellQuadValue(0, 0, 0, 0);
        finalizeLoadingWorldmapCellFromPC();
    }
    return;
}

void setupInitialGrid(VECTOR* param_1)

{
    byte** ppbVar1;
    int iVar2;

    streamingMatrixIdentity = worldMapIdentityMatrix;
    iVar2 = 0xff;
    ppbVar1 = &worldmapChunks[0xff];
    do {
        *ppbVar1 = (byte*)0x0;
        iVar2 = iVar2 + -1;
        ppbVar1 = ppbVar1 + -1;
    } while (-1 < iVar2);
    MissingCode();

    worldmapWaterWave = 0;
    worldmapWaterWave2 = 0x400;
    worldmapGridInputPosition.vx = param_1->vx & 0x7fffff;
    worldmapGridPosition.vx = 2;
    worldmapGridPosition.vy = 0;
    worldmapGridPosition.vz = 2;
    worldmapGridInputPosition.vz = param_1->vz & 0x7fffff;
    updateWorldmapGrids(param_1);
    triggerInitialWorldLoading();
    return;
}

struct sVisibilityEntry {
    union {
        std::array<u32, 2> mRawPair;
        std::array<s16, 4> mTile;
    };
};



std::array<std::array<s16, 5>, 5> megatilesVisibility;
std::array<std::array<sVisibilityEntry, 5>, 5> tilesVisibility;

VECTOR VECTOR_8009c828;
VECTOR VECTOR_8009c844;
VECTOR VECTOR_8009c874;
VECTOR VECTOR_8009c7f0;

// -1 means completely clipped
// 0 means partially visible
// 1 means?
s16 computeTileVisibility(SVECTOR* param_1, SVECTOR* param_2, SVECTOR* param_3, SVECTOR* param_4) {
    int iVar1;
    int iVar2;
    uint uVar3;
    VECTOR* pVVar4;
    int iVar5;
    VECTOR* pVVar6;
    int iVar7;
    uint uVar8;
    VECTOR* pVVar9;
    uint uVar10;
    VECTOR* r0;
    uint uVar11;

    VECTOR VECTOR_1f800000;
    VECTOR VECTOR_1f800010;
    VECTOR VECTOR_1f800020;
    VECTOR VECTOR_1f800030;

    r0 = &VECTOR_1f800000;
    gte_ldv0(param_1);
    gte_rt();
    gte_stlvnl(r0);
    gte_ldv0(param_2);
    gte_rt();
    pVVar9 = &VECTOR_1f800010;
    gte_stlvnl(&VECTOR_1f800010);
    gte_ldv0(param_3);
    gte_rt();
    pVVar6 = &VECTOR_1f800020;
    gte_stlvnl(&VECTOR_1f800020);
    gte_ldv0(param_4);
    gte_rt();
    pVVar4 = &VECTOR_1f800030;
    gte_stlvnl(&VECTOR_1f800030);
    uVar11 = 0;
    iVar7 = r0->vx * VECTOR_8009c828.vx + VECTOR_1f800000.vz * VECTOR_8009c828.vz >> 0xc;
    iVar2 = pVVar9->vx * VECTOR_8009c828.vx + VECTOR_1f800010.vz * VECTOR_8009c828.vz >> 0xc;
    iVar5 = pVVar6->vx * VECTOR_8009c828.vx + VECTOR_1f800020.vz * VECTOR_8009c828.vz >> 0xc;
    iVar1 = pVVar4->vx * VECTOR_8009c828.vx + VECTOR_1f800030.vz * VECTOR_8009c828.vz >> 0xc;
    if (iVar7 < 0) {
        if (iVar2 < 0) {
            uVar11 = 1;
            goto LAB_worldmap__8009891c;
        }
    LAB_worldmap__80098930:
        if (-1 < iVar1) goto LAB_worldmap__80098944;
        iVar1 = uVar11 << 0x10;
        if (iVar5 < 0) {
            uVar11 = uVar11 + 1;
            goto LAB_worldmap__80098944;
        }
    }
    else {
    LAB_worldmap__8009891c:
        if (-1 < iVar2) goto LAB_worldmap__80098930;
        if (iVar1 < 0) {
            uVar11 = uVar11 + 1;
            goto LAB_worldmap__80098930;
        }
    LAB_worldmap__80098944:
        iVar1 = uVar11 << 0x10;
        if ((iVar5 < 0) && (iVar7 < 0)) {
            uVar11 = uVar11 + 1;
            iVar1 = uVar11 * 0x10000;
        }
    }
    uVar10 = 0;
    if (iVar1 >> 0x10 == 4) {
        return 0xffffffff;
    }
    iVar7 = r0->vx * VECTOR_8009c844.vx + r0->vz * VECTOR_8009c844.vz >> 0xc;
    iVar2 = r0[1].vx * VECTOR_8009c844.vx + r0[1].vz * VECTOR_8009c844.vz >> 0xc;
    iVar5 = r0[2].vx * VECTOR_8009c844.vx + r0[2].vz * VECTOR_8009c844.vz >> 0xc;
    iVar1 = r0[3].vx * VECTOR_8009c844.vx + r0[3].vz * VECTOR_8009c844.vz >> 0xc;
    if (iVar7 < 0) {
        if (iVar2 < 0) {
            uVar10 = 1;
            goto LAB_worldmap__80098a2c;
        }
    LAB_worldmap__80098a40:
        if (-1 < iVar1) goto LAB_worldmap__80098a54;
        iVar1 = uVar10 << 0x10;
        if (iVar5 < 0) {
            uVar10 = uVar10 + 1;
            goto LAB_worldmap__80098a54;
        }
    }
    else {
    LAB_worldmap__80098a2c:
        if (-1 < iVar2) goto LAB_worldmap__80098a40;
        if (iVar1 < 0) {
            uVar10 = uVar10 + 1;
            goto LAB_worldmap__80098a40;
        }
    LAB_worldmap__80098a54:
        iVar1 = uVar10 << 0x10;
        if ((iVar5 < 0) && (iVar7 < 0)) {
            uVar10 = uVar10 + 1;
            iVar1 = uVar10 * 0x10000;
        }
    }
    uVar8 = 0;
    if (iVar1 >> 0x10 == 4) {
        return 0xffffffff;
    }
    iVar7 = r0->vy * VECTOR_8009c874.vy + r0->vz * VECTOR_8009c874.vz >> 0xc;
    iVar2 = r0[1].vy * VECTOR_8009c874.vy + r0[1].vz * VECTOR_8009c874.vz >> 0xc;
    iVar5 = r0[2].vy * VECTOR_8009c874.vy + r0[2].vz * VECTOR_8009c874.vz >> 0xc;
    iVar1 = r0[3].vy * VECTOR_8009c874.vy + r0[3].vz * VECTOR_8009c874.vz >> 0xc;
    if (iVar7 < 0) {
        if (iVar2 < 0) {
            uVar8 = 1;
            goto LAB_worldmap__80098b3c;
        }
    LAB_worldmap__80098b50:
        if (-1 < iVar1) goto LAB_worldmap__80098b64;
        iVar1 = uVar8 << 0x10;
        if (iVar5 < 0) {
            uVar8 = uVar8 + 1;
            goto LAB_worldmap__80098b64;
        }
    }
    else {
    LAB_worldmap__80098b3c:
        if (-1 < iVar2) goto LAB_worldmap__80098b50;
        if (iVar1 < 0) {
            uVar8 = uVar8 + 1;
            goto LAB_worldmap__80098b50;
        }
    LAB_worldmap__80098b64:
        iVar1 = uVar8 << 0x10;
        if ((iVar5 < 0) && (iVar7 < 0)) {
            uVar8 = uVar8 + 1;
            iVar1 = uVar8 * 0x10000;
        }
    }
    if (iVar1 >> 0x10 == 4) {
        return 0xffffffff;
    }
    uVar3 = 0;
    iVar7 = r0->vy * VECTOR_8009c7f0.vy + r0->vz * VECTOR_8009c7f0.vz >> 0xc;
    iVar2 = r0[1].vy * VECTOR_8009c7f0.vy + r0[1].vz * VECTOR_8009c7f0.vz >> 0xc;
    iVar5 = r0[2].vy * VECTOR_8009c7f0.vy + r0[2].vz * VECTOR_8009c7f0.vz >> 0xc;
    iVar1 = r0[3].vy * VECTOR_8009c7f0.vy + r0[3].vz * VECTOR_8009c7f0.vz >> 0xc;
    if (iVar7 < 0) {
        if (iVar2 < 0) {
            uVar3 = 1;
            goto LAB_worldmap__80098c50;
        }
    LAB_worldmap__80098c64:
        if (iVar1 < 0) {
            iVar1 = uVar3 << 0x10;
            if (-1 < iVar5) goto LAB_worldmap__80098c94;
            uVar3 = uVar3 + 1;
        }
    }
    else {
    LAB_worldmap__80098c50:
        if (-1 < iVar2) goto LAB_worldmap__80098c64;
        if (iVar1 < 0) {
            uVar3 = uVar3 + 1;
            goto LAB_worldmap__80098c64;
        }
    }
    iVar1 = uVar3 << 0x10;
    if ((iVar5 < 0) && (iVar7 < 0)) {
        uVar3 = uVar3 + 1;
        iVar1 = uVar3 * 0x10000;
    }
LAB_worldmap__80098c94:
    if (iVar1 >> 0x10 == 4) {
        return 0xffffffff;
    }
    return (uint)((uVar3 | uVar8 | uVar10 | uVar11) == 0);
}

std::array<std::array<u32, 50>, 4> mapQuadrantTilesPatch = { {
    {{
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFF0000, 0xFFFFFFFF,
    }},
    {{
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF,
    }},
    {{
        0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
        0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF0000, 0xFFFF0000,
    }},
    {{
        0xFFFFFFFF, 0xFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
        0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0, 0, 0,
    }}
} };

VECTOR VECTOR_worldmap__8009bb6c = { -0x87A, 0x0, 0xD92 };
VECTOR VECTOR_worldmap__8009bb4c = { 0x0, -1, 0 };
VECTOR VECTOR_worldmap__8009bb8c = { 0, -0x87A, 0xD92 };
VECTOR VECTOR_worldmap__8009bb5c = { 1,0,0 };
VECTOR VECTOR_worldmap__8009bb7c = { 0x87A, 0, 0xD92 };
VECTOR VECTOR_worldmap__8009bb9c = { 0, 0x87A, 0 };


void worldmapDotInit(void)
{
    OuterProduct0(&VECTOR_worldmap__8009bb6c, &VECTOR_worldmap__8009bb4c, &VECTOR_8009c828);
    OuterProduct0(&VECTOR_worldmap__8009bb4c, &VECTOR_worldmap__8009bb7c, &VECTOR_8009c844);
    OuterProduct0(&VECTOR_worldmap__8009bb8c, &VECTOR_worldmap__8009bb5c, &VECTOR_8009c874);
    OuterProduct0(&VECTOR_worldmap__8009bb5c, &VECTOR_worldmap__8009bb9c, &VECTOR_8009c7f0);
    return;
}

void worldmapGroundPrepareRenderingTable(VECTOR* param_1) {
    MATRIX MATRIX_1f8000f0 = streamingMatrixIdentity;
    MATRIX MATRIX_1f800110;
    CompMatrix(&worldmapMainMatrix2, &MATRIX_1f8000f0, &MATRIX_1f800110);
    SetRotMatrix(&MATRIX_1f800110);
    SetTransMatrix(&MATRIX_1f800110);

    VECTOR VECTOR_1f800040;
    VECTOR_1f800040.vx = -0x1000 - (param_1->vx >> 0xc & 0x7ffU);
    VECTOR_1f800040.vz = -0x1000 - (param_1->vz >> 0xc & 0x7ffU);

    SVECTOR SVECTOR_1f8000a8;
    SVECTOR SVECTOR_1f8000a0;
    SVECTOR SVECTOR_1f8000e0;
    SVECTOR SVECTOR_1f8000d8;
    SVECTOR SVECTOR_1f8000d0;
    SVECTOR SVECTOR_1f8000c8;
    SVECTOR SVECTOR_1f8000c0;
    SVECTOR SVECTOR_1f8000b8;
    SVECTOR SVECTOR_1f8000b0;

    SVECTOR_1f8000e0.vy = 0;
    SVECTOR_1f8000d8.vy = 0;
    SVECTOR_1f8000d0.vy = 0;
    SVECTOR_1f8000c8.vy = 0;
    SVECTOR_1f8000c0.vy = 0;
    SVECTOR_1f8000b8.vy = 0;
    SVECTOR_1f8000b0.vy = 0;
    SVECTOR_1f8000a8.vy = 0;
    SVECTOR_1f8000a0.vy = 0;
    SVECTOR_1f8000a0.vz = -(short)VECTOR_1f800040.vz;


    for(int x = 0; x<5; x++) {
        SVECTOR_1f8000a0.vx = (short)VECTOR_1f800040.vx;
        for (int z = 0; z < 5; z++) {
            SVECTOR_1f8000a8.vx = SVECTOR_1f8000a0.vx + 0x800;
            SVECTOR_1f8000b0.vz = SVECTOR_1f8000a0.vz + -0x800;
            SVECTOR_1f8000a8.vz = SVECTOR_1f8000a0.vz;
            SVECTOR_1f8000b0.vx = SVECTOR_1f8000a0.vx;
            SVECTOR_1f8000b8.vx = SVECTOR_1f8000a0.vx + 0x800;
            SVECTOR_1f8000b8.vz = SVECTOR_1f8000a0.vz + -0x800;
            s16 megatileVisibility = computeTileVisibility(&SVECTOR_1f8000a0, &SVECTOR_1f8000a8, &SVECTOR_1f8000b0, &SVECTOR_1f8000b8);
            megatilesVisibility[x][z] = megatileVisibility;

            // full megatile isn't fully visible, check the individual tiles
            if (megatileVisibility == 0) {
                SVECTOR_1f8000c8.vz = SVECTOR_1f8000a0.vz + -0x400;
                SVECTOR_1f8000c0.vx = SVECTOR_1f8000a0.vx + 0x400;
                SVECTOR_1f8000c8.vx = SVECTOR_1f8000a0.vx;
                SVECTOR_1f8000d0.vx = SVECTOR_1f8000a8.vx;
                SVECTOR_1f8000c0.vz = SVECTOR_1f8000a0.vz;
                SVECTOR_1f8000d8.vz = SVECTOR_1f8000b0.vz;
                SVECTOR_1f8000d0.vz = SVECTOR_1f8000c8.vz;
                SVECTOR_1f8000d8.vx = SVECTOR_1f8000c0.vx;
                SVECTOR_1f8000e0.vx = SVECTOR_1f8000c0.vx;
                SVECTOR_1f8000e0.vz = SVECTOR_1f8000c8.vz;
                tilesVisibility[x][z].mTile[0] = computeTileVisibility(&SVECTOR_1f8000a0, &SVECTOR_1f8000c0, &SVECTOR_1f8000c8, &SVECTOR_1f8000e0);
                tilesVisibility[x][z].mTile[1] = computeTileVisibility(&SVECTOR_1f8000c0, &SVECTOR_1f8000a8, &SVECTOR_1f8000e0, &SVECTOR_1f8000d0);
                tilesVisibility[x][z].mTile[2] = computeTileVisibility(&SVECTOR_1f8000c8, &SVECTOR_1f8000e0, &SVECTOR_1f8000b0, &SVECTOR_1f8000d8);
                tilesVisibility[x][z].mTile[3] = computeTileVisibility(&SVECTOR_1f8000e0, &SVECTOR_1f8000d0, &SVECTOR_1f8000d8, &SVECTOR_1f8000b8);
            }
            else {
                tilesVisibility[x][z].mTile[0] = megatileVisibility;
                tilesVisibility[x][z].mTile[1] = megatileVisibility;
                tilesVisibility[x][z].mTile[2] = megatileVisibility;
                tilesVisibility[x][z].mTile[3] = megatileVisibility;
            }
            SVECTOR_1f8000a0.vx = SVECTOR_1f8000a0.vx + 0x800;
        }
        SVECTOR_1f8000a0.vz = SVECTOR_1f8000a0.vz + -0x800;
    }

    // This patches the tile visibility, so that edges of the map are visible even if we haven't wrapped yet
    u32 mapQuadrant = (param_1->vx >> 0xc & 0x7ffU) < 0x400 ^ 1;
    if (0x3ff < (param_1->vz >> 0xc & 0x7ffU)) {
        mapQuadrant = mapQuadrant | 2;
    }
    std::array<u32, 50>::iterator puVar9 = mapQuadrantTilesPatch[mapQuadrant].begin();
    for (int i = 0; i < 5*5; i++) {
        tilesVisibility[i/5][i%5].mRawPair[0] |= puVar9[0];
        tilesVisibility[i/5][i%5].mRawPair[1] |= puVar9[1];
        puVar9 += 2;
    }
}

int renderWorldmapGroundVar1;

std::array<SVECTOR, 9*9> DAT_1f800000;
static std::array<u16, 8> DAT_1f800308;
static std::array<u16, 0x40> DAT_1f800288;

void drawWorldmapGroundCellSub0(uint* param_1, std::vector<sTag>& OT, std::vector<POLY_FT3>::iterator& outputTriangles)
{
    s32 currentNumUsedTriangles = renderWorldmapGroundVar1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto r0 = &DAT_1f800000[i * 9 + j];

            u32 quadParams = param_1[i * 9 + j];

            u32 quadParam1 = quadParams << 0x10;
            s32 quadParam2 = quadParams >> 0xc & 0xf0 | quadParams >> 8 & 0xf000;
            s32 quadParam3 = quadParams >> 0xd & 3;

            u32 UV0;
            u32 UV1;
            u32 UV2;
            u32 UV3;

            switch (quadParam3) {
            case 0:
                UV0 = quadParam2;
                UV1 = quadParam2 + 0xf;
                UV2 = quadParam2 + 0xf00;
                UV3 = quadParam2 + 0xf0f;
                break;
            case 1:
                UV0 = quadParam2 + 0xf;
                UV1 = quadParam2;
                UV2 = quadParam2 + 0xf0f;
                UV3 = quadParam2 + 0xf00;
                break;
            case 2:
                UV0 = quadParam2 + 0xf00;
                UV1 = quadParam2 + 0xf0f;
                UV2 = quadParam2;
                UV3 = quadParam2 + 0xf;
                break;
            case 3:
                UV0 = quadParam2 + 0xf0f;
                UV1 = quadParam2 + 0xf00;
                UV2 = quadParam2 + 0xf;
                UV3 = quadParam2;
                break;
            }

            // to avoid getting out of bound
            if (0x7fd < currentNumUsedTriangles) {
                renderWorldmapGroundVar1 = currentNumUsedTriangles;
                return;
            }

            gte_ldv0(&(*r0));
            gte_ldVXY2(&(*r0) + 9);
            gte_ldVZ2(&r0[9].vz);

            u32 _triV1;
            if (quadParam1 & 0x80000000) {
                gte_ldVXY1(&(*r0) + 10);
                gte_ldVZ1(&r0[10].vz);
                _triV1 = UV3;
            }
            else {
                gte_ldVXY1(&(*r0) + 1);
                gte_ldVZ1(&r0[1].vz);
                _triV1 = UV1;
            }
            gte_rtpt_b();

            // near clip
            if (gte_stFLAG() & 0x80000000)
                continue;

            sVec2_s16 _triXY0 = sVec2_s16::fromS32(gte_stSXY0());
            sVec2_s16 _triXY1 = sVec2_s16::fromS32(gte_stSXY1());
            sVec2_s16 _triXY2 = sVec2_s16::fromS32(gte_stSXY2());

            POLY_FT3* pOutputTriangle = &(*outputTriangles);

            if (((_triXY0.vx < 0x140) || (_triXY1.vx < 0x140)) || (_triXY2.vx < 0x140)) {
                if (((_triXY0.vy < 0xD8) || (_triXY1.vy < 0xD8)) || (_triXY2.vy < 0xD8)) {
                    s16 sz0;
                    s16 sz1;
                    s16 sz2;
                    read_sz_fifo3(&sz0, &sz1, &sz2);
                    s32 zMin = sz0;
                    if (sz0 <= sz1) {
                        zMin = sz1;
                    }
                    if (zMin <= sz2) {
                        zMin = sz2;
                    }
                    s32 depth = zMin >> 4;
                    if (zMin < 0xF00) {

                        // backface clipping
                        gte_nclip();
                        if (gte_stMAC0() > 0) {
                            s32 IR0 = gte_stIR0();
                            if (IR0 > 0xFFF) {
                                IR0 = 0xffffff >> 0xc;
                            }

                            u16 tpage = DAT_1f800308[quadParam1 >> 0x18 & 0x7];
                            u16 clut = DAT_1f800288[(IR0>>7) + (quadParam1 >> 0x15 & 0x40) / 2];

                            currentNumUsedTriangles++;
                            pOutputTriangle->x0y0 = _triXY0;
                            pOutputTriangle->x1y1 = _triXY1;
                            pOutputTriangle->x2y2 = _triXY2;
                            *(u16*)&pOutputTriangle->u2 = UV2;
                            *(u16*)&pOutputTriangle->u0 = UV0;
                            pOutputTriangle->clut = clut;
                            *(u16*)&pOutputTriangle->u1 = _triV1;
                            pOutputTriangle->tpage = tpage;
                            pOutputTriangle->m3_size = 0x7;
                            pOutputTriangle->m0_pNext = OT[depth].m0_pNext;
                            OT[depth].m0_pNext = pOutputTriangle;
                            outputTriangles = outputTriangles + 1;
                        }
                    }
                }
            }

            {
                gte_ldVXY0(&(*r0) + 1);
                gte_ldVZ0(&r0[1].vz);
                gte_ldVXY1(&(*r0) + 10);
                gte_ldVZ1(&r0[10].vz);

                s32 UV2or0;
                if (quadParam1 & 0x80000000) {
                    gte_ldv2(&(*r0));
                    UV2or0 = (ushort)UV0;
                }
                else {
                    gte_ldVXY2(&(*r0) + 9);
                    gte_ldVZ2(&r0[9].vz);
                    UV2or0 = UV2;
                }
                gte_rtpt_b();

                // near clip
                if (gte_stFLAG() & 0x80000000)
                    continue;

                sVec2_s16 _triXY0 = sVec2_s16::fromS32(gte_stSXY0());
                sVec2_s16 _triXY1 = sVec2_s16::fromS32(gte_stSXY1());
                sVec2_s16 _triXY2 = sVec2_s16::fromS32(gte_stSXY2());

                POLY_FT3* pOutputTriangle = &(*outputTriangles);

                if (((_triXY0.vx < 0x140) || (_triXY1.vx < 0x140)) || (_triXY2.vx < 0x140)) {
                    if (((_triXY0.vy < 0xD8) || (_triXY1.vy < 0xD8)) || (_triXY2.vy < 0xD8)) {
                        s16 sz0;
                        s16 sz1;
                        s16 sz2;
                        read_sz_fifo3(&sz0, &sz1, &sz2);
                        s32 zMin = sz0;
                        if (sz0 <= sz1) {
                            zMin = sz1;
                        }
                        if (zMin <= sz2) {
                            zMin = sz2;
                        }
                        s32 depth = zMin >> 4;
                        if (zMin < 0xF00) {

                            // backface clipping
                            gte_nclip();
                            if (gte_stMAC0() > 0) {

                                s32 IR0 = gte_stIR0();
                                if (IR0 > 0xFFF) {
                                    IR0 = 0xffffff >> 0xc;
                                }

                                u16 tpage = DAT_1f800308[quadParam1 >> 0x18 & 0x7];
                                u16 clut = DAT_1f800288[(IR0>>7) + (quadParam1 >> 0x15 & 0x40) / 2];

                                currentNumUsedTriangles++;
                                pOutputTriangle->x0y0 = _triXY0;
                                pOutputTriangle->x1y1 = _triXY1;
                                pOutputTriangle->x2y2 = _triXY2;
                                *(u16*)&pOutputTriangle->u2 = UV2or0;
                                *(u16*)&pOutputTriangle->u0 = UV1;
                                pOutputTriangle->clut = clut;
                                *(u16*)&pOutputTriangle->u1 = UV3;
                                pOutputTriangle->tpage = tpage;
                                pOutputTriangle->m3_size = 0x7;
                                pOutputTriangle->m0_pNext = OT[depth].m0_pNext;
                                OT[depth].m0_pNext = pOutputTriangle;
                                outputTriangles = outputTriangles + 1;
                            }
                        }
                    }
                }
            }
        }
    }

    renderWorldmapGroundVar1 = currentNumUsedTriangles;
}

void drawWorldmapGroundCell(uint* param_1, std::vector<sTag>& param_2, std::vector<POLY_FT3>::iterator& param_3, SVECTOR* param_4) {
    short sVar2;
    int uVar3;
    uint uVar6;
    uint* puVar8;

    auto pOutputSVector = DAT_1f800000.begin();
    s16 tileStartX = param_4->vx;
    s16 tileStartZ = param_4->vz;
    puVar8 = param_1;
    uint currentWave2 = worldmapWaterWave2;
    for (int i = 0; i < 9; i++) {
        sVar2 = rcossin_tbl[2 * (currentWave2 & 0xfff)];
        s32 currentX = tileStartX;
        uVar6 = worldmapWaterWave;
        for (int j = 0; j < 9; j++) {
            uVar3 = param_1[i * 9 + j];
            if ((uVar3 & 0x1000) == 0) {
                uVar3 = (int)(uVar3 << 0x18) >> 5;
            }
            else {
                uVar3 = (((int)rcossin_tbl[2 * (uVar6 & 0xfff)] * sVar2 * 2 >> 0x14) + ((int)(uVar3 << 0x18) >> 0x15)) * 0x10000;
            }
            pOutputSVector->vx = currentX;
            pOutputSVector->vy = uVar3 >> 16;
            pOutputSVector->vz = tileStartZ;

            currentX = currentX + 0x80;
            pOutputSVector++;
            puVar8 = puVar8 + 1;
            uVar6 = uVar6 + 0x200;

        }
        tileStartZ = tileStartZ + -0x80;
        currentWave2 = currentWave2 + 0x200;
    };
    drawWorldmapGroundCellSub0(param_1, param_2, param_3);
    return;
}

void drawWorldmapGround(std::vector<sTag>& param_1, std::vector<POLY_FT3>::iterator& param_2, VECTOR* param_3) {
    DAT_1f800288 = USHORT_ARRAY_8009ccb4;
    DAT_1f800308 = USHORT_ARRAY_8009cd54;
    MATRIX DAT_1f800350 = streamingMatrixIdentity;
    MATRIX DAT_1f800370;
    CompMatrix(&worldmapMainMatrix2, &DAT_1f800350, &DAT_1f800370);
    SetRotMatrix(&DAT_1f800370);
    SetTransMatrix(&DAT_1f800370);

    VECTOR VECTOR_1f800318;
    VECTOR_1f800318.vx = -0x1000 - (param_3->vx >> 0xc & 0x7ffU);
    VECTOR_1f800318.vz = -0x1000 - (param_3->vz >> 0xc & 0x7ffU);
    renderWorldmapGroundVar1 = 0;

    SVECTOR SVECTOR_1f800328;
    SVECTOR_1f800328.vz = -(short)VECTOR_1f800318.vz;

    for (int iVar6 = 0; iVar6 < 5; iVar6++) {
        SVECTOR_1f800328.vx = (short)VECTOR_1f800318.vx;
        for (int iVar7 = 0; iVar7 < 5; iVar7++) {
            if (megatilesVisibility[iVar6][iVar7] != -1) {
                u32* pbVar4 = (u32*)worldmapChunks[(short)worldmapGrid[(iVar6 + worldmapGridPosition.vz) * 9 + iVar7 + (int)worldmapGridPosition.vx]];
                SVECTOR SVECTOR_1f800330;
                SVECTOR SVECTOR_1f800338;
                SVECTOR SVECTOR_1f800340;
                SVECTOR_1f800330.vx = SVECTOR_1f800328.vx + 0x400;
                SVECTOR_1f800330.vz = SVECTOR_1f800328.vz;
                SVECTOR_1f800338.vx = SVECTOR_1f800328.vx;
                SVECTOR_1f800338.vz = SVECTOR_1f800328.vz + -0x400;
                SVECTOR_1f800340.vx = SVECTOR_1f800330.vx;
                SVECTOR_1f800340.vz = SVECTOR_1f800338.vz;
                if ((ushort)(
                    ((int)tilesVisibility[iVar6][iVar7].mTile[0]) |
                    ((int)tilesVisibility[iVar6][iVar7].mTile[1]) |
                    ((int)tilesVisibility[iVar6][iVar7].mTile[2]) |
                    ((int)tilesVisibility[iVar6][iVar7].mTile[3]
                        )) == 0xffff) {
                    // at least one of the tile isn't visible
                    if (tilesVisibility[iVar6][iVar7].mTile[0] != -1) {
                        drawWorldmapGroundCell(pbVar4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800328);
                    }
                    if (tilesVisibility[iVar6][iVar7].mTile[1] != -1) {
                        drawWorldmapGroundCell(pbVar4 + 0x144 / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800330);
                    }
                    if (tilesVisibility[iVar6][iVar7].mTile[2] != -1) {
                        drawWorldmapGroundCell(pbVar4 + 0x288 / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800338);
                    }
                    if (tilesVisibility[iVar6][iVar7].mTile[3] != -1) {
                        drawWorldmapGroundCell(pbVar4 + 0x3cc / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800340);
                    }
                }
                else {
                    // all tiles are visible
                    drawWorldmapGroundCell(pbVar4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800328);
                    drawWorldmapGroundCell(pbVar4 + 0x144 / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800330);
                    drawWorldmapGroundCell(pbVar4 + 0x288 / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800338);
                    drawWorldmapGroundCell(pbVar4 + 0x3cc / 4, param_1, param_2 + renderWorldmapGroundVar1, &SVECTOR_1f800340);
                }

            }
            SVECTOR_1f800328.vx = SVECTOR_1f800328.vx + 0x800;
        }
        SVECTOR_1f800328.vz = SVECTOR_1f800328.vz + -0x800;
    }
}