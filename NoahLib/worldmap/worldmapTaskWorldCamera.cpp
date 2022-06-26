#include "noahLib.h"
#include "worldmapTaskWorldCamera.h"
#include "worldmap.h"

void checkWorldmapPositionSub0(VECTOR* param_1);
void computeWormapSpritePosition(VECTOR* v);

s32 worldmap_taskWorldCamera_init(int param_1)
{
    int iVar1;
    sWorldmapState* psVar2;
    long lVar3;
    long lVar4;

    lVar4 = worldmapRadarPosition.vz;
    lVar3 = worldmapRadarPosition.vy;
    psVar2 = gWorldmapState;
    gWorldmapState->m0[param_1].m28_position.vx = worldmapRadarPosition.vx;
    psVar2->m0[param_1].m28_position.vy = lVar3;
    psVar2->m0[param_1].m28_position.vz = lVar4;
    psVar2->m0[param_1].m28_position.pad = worldmapRadarPosition.pad;
    worldmapRotation.vz = 0;
    worldmapRotation.vy = worldmapVar_8009d52c;
    psVar2->m0[param_1].m50 = (int)worldmapVar_8009d52c;
    iVar1 = worldMapGearMode;
    psVar2->m0[param_1].m58 = (int)worldmapRotation.vy << 0xc;
    if ((iVar1 < 8) && (5 < iVar1)) {
        psVar2->m0[param_1].m20 = 3;
    }
    return 1;
}

s32 worldmap_taskWorldCamera_update(int param_1)
{
    switch (gWorldmapState->m0[param_1].m4 - 9) {
    case -9:
    case -6:
        break;

    case 0:
        gWorldmapState->m0[param_1].m4 = 0;
        gWorldmapState->m0[param_1].m20 = 2;
        gWorldmapState->m0[param_1].m50 = worldmapVar_8009d52c;
        gWorldmapState->m0[param_1].m58 = worldmapRotation.vy << 0xC;
        break;
    case 1:
        gWorldmapState->m0[param_1].m4 = 0;
        gWorldmapState->m0[param_1].m20 = 0;
        gWorldmapState->m0[param_1].m50 = worldmapRotation.vy;
        break;
    case 6:
        gWorldmapState->m0[param_1].m20 = 0x10;
        gWorldmapState->m0[param_1].m4 = 0;
        gWorldmapState->m0[param_1].m50 = 0x800;
        break;
    case 7:
        gWorldmapState->m0[param_1].m20 = 0x10;
        gWorldmapState->m0[param_1].m4 = 0;
        gWorldmapState->m0[param_1].m50 = 0xA00;
        break;
    case 8:
        gWorldmapState->m0[param_1].m20 = 0x10;
        gWorldmapState->m0[param_1].m4 = 0;
        gWorldmapState->m0[param_1].m50 = 0xC00;
        break;

    default:
        assert(0);
    }

    switch (gWorldmapState->m0[param_1].m20) {
    case 0:
        switch (worldmapInput1_0 >> 2 & 3) {
        case 1:
        case 3:
            // worlmap camera rotation
            gWorldmapState->m0[param_1].m20 = 1;
            gWorldmapState->m0[param_1].m54 = -0x40;
            gWorldmapState->m0[param_1].m5C = (gWorldmapState->m0[param_1].m50 - 0x200) & 0xfff;
            break;
        case 2:
            // worlmap camera rotation
            gWorldmapState->m0[param_1].m20 = 1;
            gWorldmapState->m0[param_1].m54 = 0x40;
            gWorldmapState->m0[param_1].m5C = (gWorldmapState->m0[param_1].m50 + 0x200) & 0xfff;
            break;
        default:
            if (worldmapRotation.vy == gWorldmapState->m0[param_1].m50) {
                gWorldmapState->m0[param_1].m58 = worldmapRotation.vy << 0xc;
            }
            else {
            LAB_worldmap__8009164c:
                s32 iVar6 = gWorldmapState->m0[param_1].m50 - worldmapRotation.vy;
                s32 iVar5 = iVar6;
                if (iVar5 < 0) {
                    iVar5 = -iVar6;
                }
                if (0xc00 < iVar5) {
                    if (iVar6 < 0) {
                        iVar6 = iVar6 + 0x1000;
                    }
                    else {
                        iVar6 = iVar6 + -0x1000;
                    }
                }
                gWorldmapState->m0[param_1].m58 = gWorldmapState->m0[param_1].m58 + ((iVar6 << 0xc) >> 3) & 0xffffff;
                worldmapRotation.vy = (short)((int)gWorldmapState->m0[param_1].m58 >> 0xc);
            }
            break;
        }
        break;
    case 1:
        gWorldmapState->m0[param_1].m50 = gWorldmapState->m0[param_1].m50 + gWorldmapState->m0[param_1].m54 & 0xfff;
        if (gWorldmapState->m0[param_1].m50 == gWorldmapState->m0[param_1].m5C) {
            gWorldmapState->m0[param_1].m20 = 0;
        }
        goto LAB_worldmap__8009164c;
    case 2: // yggdrasil take off
    {
        s32 iVar8 = gWorldmapState->m0[param_1].m50 - (int)worldmapRotation.vy;
        s32 iVar5 = iVar8;
        if (iVar8 < 0) {
            iVar5 = -iVar8;
        }
        if (0x800 < iVar5) {
            if (iVar8 < 0) {
                iVar8 = iVar8 + 0x1000;
            }
            else {
                iVar8 = iVar8 + -0x1000;
            }
        }
        iVar5 = iVar8;
        if (iVar8 < 0) {
            iVar5 = -iVar8;
        }
        bool bVar1;
        if ((0x180 < iVar5) && (bVar1 = iVar8 < 0, iVar8 = 0x180, bVar1)) {
            iVar8 = -0x180;
        }
        s32 uVar3 = gWorldmapState->m0[param_1].m58 + ((iVar8 << 0xc) >> 3) & 0xffffff;
        gWorldmapState->m0[param_1].m58 = uVar3;
        iVar5 = (int)uVar3 >> 0xc;
        worldmapRotation.vy = (short)iVar5;
        if (iVar5 == gWorldmapState->m0[param_1].m50 && gWorldmapState->m0[param_1].m60 == 0) {
            gWorldmapState->m0[param_1].m20 = 3;
            changeWorldmapEntityState(7, 0xb);
        }
        break;
    }
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
        break;
    default:
        assert(0);
        break;
    }

    switch (gWorldmapState->m0[param_1].m20) {
    case 0:
    case 1:
    case 2:
    case 3:
        if ((gWorldmapState->m0[param_1].m28_position.vx != worldmapRadarPosition.vx || gWorldmapState->m0[param_1].m28_position.vy != worldmapRadarPosition.vy) || gWorldmapState->m0[param_1].m28_position.vz != worldmapRadarPosition.vz) {
            VECTOR local_30;
            local_30.vy = worldmapRadarPosition.vy - gWorldmapState->m0[param_1].m28_position.vy;
            local_30.vz = worldmapRadarPosition.vz - gWorldmapState->m0[param_1].m28_position.vz;
            local_30.vx = worldmapRadarPosition.vx - gWorldmapState->m0[param_1].m28_position.vx;;
            computeWormapSpritePosition(&local_30);
            VECTOR local_20;
            local_20.vx = local_30.vx >> 3;
            local_20.vy = local_30.vy >> 3;
            local_20.vz = local_30.vz >> 3;
            s32 iVar5 = local_20.vx;
            if (local_20.vx < 0) {
                iVar5 = -local_20.vx;
            }
            s32 iVar8 = local_20.vz;
            if (local_20.vz < 0) {
                iVar8 = -local_20.vz;
            }
            if (iVar5 < 0x40 && iVar8 < 0x40) {
                gWorldmapState->m0[param_1].m60 = 0;
                worldmapGridInputPosition.vx += local_30.vx;
                worldmapGridInputPosition.vz += local_30.vz;
                gWorldmapState->m0[param_1].m28_position.vx = worldmapRadarPosition.vx;
                gWorldmapState->m0[param_1].m28_position.vz = worldmapRadarPosition.vz;
            }
            else {
                gWorldmapState->m0[param_1].m60 = 1;
                gWorldmapState->m0[param_1].m28_position.vx += local_20.vx;
                gWorldmapState->m0[param_1].m28_position.vz += local_20.vz;
                worldmapGridInputPosition.vx += local_20.vx;
                worldmapGridInputPosition.vz += local_20.vz;
            }
            gWorldmapState->m0[param_1].m28_position.vy += local_20.vy;
        }
        break;
    case 16:
        assert(0);
    default:
        MissingCode();
        break;
    }

    checkWorldmapPositionSub0(&gWorldmapState->m0[param_1].m28_position);
    worldmapRadarPosition3.vx = gWorldmapState->m0[param_1].m28_position.vx;
    worldmapRadarPosition3.vy = gWorldmapState->m0[param_1].m28_position.vy;
    worldmapRadarPosition3.vz = gWorldmapState->m0[param_1].m28_position.vz;
    worldmapRadarPosition3.pad = gWorldmapState->m0[param_1].m28_position.pad;
    return 1;
}
