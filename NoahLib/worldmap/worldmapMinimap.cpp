#include "noahLib.h"
#include "worldmap.h"
#include "worldmapMinimap.h"
#include "psx/gpuprims.h"
#include "kernel/graphics.h"
#include "kernel/gte.h"
#include "kernel/gameState.h"

s32 worldmapMinimapScale = 0;

DR_TPAGE worldmapMinimapDrTpage;

std::array<POLY_FT4, 2> worldmapMinimapBackgroundPoly;
std::array<std::array<POLY_G3, 4>, 2> worldmapMinimapRadar;
std::array<std::array<TILE, 32>, 2> worldmapMinimapPOIsPolys;

static const std::array<std::array<SVECTOR, 3>, 4> worldmapMinimapRadarShape = { {
    {{
        {0,0,0,0}, {-8, -8, 0, 0}, {-4, -11, 0, 0},
    }},
    {{
        {0,0,0,0}, {-4, -11, 0, 0}, {0, -12, 0, 0},
    }},
    {{
        {0,0,0,0}, {0x0, -12, 0, 0}, {0x4, -11, 0, 0},
    }},
    {{
        {0,0,0,0}, {0x4, -11, 0, 0}, {0x8, -8, 0, 0},
    }},
} };

static const std::array<std::array<s16, 2>, 32> worldmapMinimapPOIsCoordinates = { {
    {{0x54, 0x24}},
    {{0x44, 0x29}},
    {{0x37, 0x22}},
    {{0x3A, 0x18}},
    {{0x3E, 0x14}},
    {{0x36, 0x14}},
    {{0x4E, 0x14}},
    {{0x4D, 0x09}},
    {{0x39, 0x11}},
    {{0x1D, 0x51}},
    {{0x18, 0x44}},
    {{0x0A, 0x47}},
    {{0x1F, 0x39}},
    {{0x0F, 0x3A}},
    {{0x33, 0x30}},
    {{0x3B, 0x59}},
    {{0x5C, 0x52}},
    {{0x11, 0x06}},
    {{0x10, 0x23}},
    {{0x08, 0x09}},
    {{0x58, 0x45}},
    {{0x5A, 0x31}},
    {{0x5E, 0x1F}},
    {{0x5F, 0x1D}},

    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
    {{-1 , 0}},
} };

void initWorldmapMinimap()
{
    SetPolyFT4(&worldmapMinimapBackgroundPoly[0]);
    worldmapMinimapBackgroundPoly[0].y0 = 0x78;
    worldmapMinimapBackgroundPoly[0].y1 = 0x78;
    worldmapMinimapBackgroundPoly[0].y2 = 0xd7;
    worldmapMinimapBackgroundPoly[0].x0 = 0xd0;
    worldmapMinimapBackgroundPoly[0].x2 = 0xd0;
    worldmapMinimapBackgroundPoly[0].x1 = 0x137;
    worldmapMinimapBackgroundPoly[0].y3 = 0xd7;
    worldmapMinimapBackgroundPoly[0].x3 = 0x137;
    worldmapMinimapBackgroundPoly[0].u0 = 0x00;
    worldmapMinimapBackgroundPoly[0].v0 = 0x80;
    worldmapMinimapBackgroundPoly[0].u1 = 0x7F;
    worldmapMinimapBackgroundPoly[0].v1 = 0x80;
    worldmapMinimapBackgroundPoly[0].u2 = 0x00;
    worldmapMinimapBackgroundPoly[0].v2 = 0xff;
    worldmapMinimapBackgroundPoly[0].u3 = 0x7F;
    worldmapMinimapBackgroundPoly[0].v3 = 0xff;
    worldmapMinimapBackgroundPoly[0].r0 = 0x80;
    worldmapMinimapBackgroundPoly[0].g0 = 0x80;
    worldmapMinimapBackgroundPoly[0].b0 = 0x80;
    worldmapMinimapBackgroundPoly[0].tpage = GetTPage(0, 0, 0x380, 0x100);
    worldmapMinimapBackgroundPoly[0].clut = GetClut(0x100, 0x1fe);
    SetSemiTrans(&worldmapMinimapBackgroundPoly[0], 1);
    worldmapMinimapBackgroundPoly[1] = worldmapMinimapBackgroundPoly[0];

    SetDrawTPage(&worldmapMinimapDrTpage, 1, 0, GetTPage(0, 1, 0x380, 0x100));

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            POLY_G3& p = worldmapMinimapRadar[i][j];

            SetPolyG3(&p);
            p.r0 = 0xFF;
            p.g0 = 0x40;
            p.b0 = 0x40;

            p.r1 = 0;
            p.g1 = 0;
            p.b1 = 0;

            p.r2 = 0;
            p.g2 = 0;
            p.b2 = 0;
            SetSemiTrans(&p, 1);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 32; j++) {
            SetTile(&worldmapMinimapPOIsPolys[i][j]);

            worldmapMinimapPOIsPolys[i][j].wh.vx = 2;
            worldmapMinimapPOIsPolys[i][j].wh.vy= 2;

            worldmapMinimapPOIsPolys[i][j].r0 = 0x80;
            worldmapMinimapPOIsPolys[i][j].g0 = 0x80;
            worldmapMinimapPOIsPolys[i][j].b0 = 0x10;
        }
    }
}

void worldmapRenderMinimap() {
    std::array<POLY_G3, 4>::iterator p = worldmapMinimapRadar[worldmapOddOrEven].begin();
    for (int i = 0; i < 4; i++) {
        SFP_VEC4 rotationAngles;
        rotationAngles.vx = 0;
        rotationAngles.vy = 0;
        rotationAngles.vz = worldmapRotation.vy;

        MATRIX radarMatrix;
        createRotationMatrix(&rotationAngles, &radarMatrix);

        radarMatrix.t[0] = (worldmapRadarPosition.vx >> 0xC) / 0x13B + 0x30;
        radarMatrix.t[1] = ((worldmapRadarPosition.vz >> 0xC) / 0x155 + 0x78) - worldmapGeometryOffsetY;
        radarMatrix.t[2] = worldmapMinimapScale;

        gte_SetRotMatrix(&radarMatrix);
        gte_SetTransMatrix(&radarMatrix);
        gte_ldv3(&worldmapMinimapRadarShape[i][0], &worldmapMinimapRadarShape[i][1], &worldmapMinimapRadarShape[i][2]);
        gte_rtpt();
        gte_stsxy3(&p->x0y0, &p->x1y1, &p->x2y2);

        p->m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
        pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &(*p);
        p++;
    }

    worldmapMinimapDrTpage.m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
    pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapMinimapDrTpage;

    //TODO: Check if that's correct and not endian dependent
    u32 POIsBF = *(u32*)&gameState.m1930_fieldVarsBackup[254];
    for (int i = 0; i < 0x20; i++) {
        if (POIsBF & 1) {
            TILE& p = worldmapMinimapPOIsPolys[worldmapOddOrEven][i];

            switch (i)
            {
            case 0x18:
                p.x0y0.vx = (((gameState.m182C.vx * 0x1A01A01A1) >> 0x21) >> 8) + 0xCF;
                p.x0y0.vy = (((gameState.m182C.vz * 0x180601807) >> 0x21) >> 8) + 0x77;
                break;
            case 0x19:
                p.x0y0.vx = (((gameState.m184E.vx * 0x1A01A01A1) >> 0x21) >> 8) + 0xCF;
                p.x0y0.vy = (((gameState.m184E.vz * 0x180601807) >> 0x21) >> 8) + 0x77;
                break;
            case 0x1A:
                p.x0y0.vx = (((gameState.m1844[0] * 0x1A01A01A1) >> 0x21) >> 8) + 0xCF;
                p.x0y0.vy = (((gameState.m1844[1] * 0x180601807) >> 0x21) >> 8) + 0x77;
                break;
            default:
                p.x0y0.vx = worldmapMinimapPOIsCoordinates[i][0] + 0xD0;
                p.x0y0.vy = worldmapMinimapPOIsCoordinates[i][1] + 0x78;
                break;
            }

            p.m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
            pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &p;
        }
        POIsBF >>= 1;
    }

    worldmapMinimapBackgroundPoly[worldmapOddOrEven].m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
    pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapMinimapBackgroundPoly[worldmapOddOrEven];
}

s32 worldmap_taskMinimapY_init(int param_1) {
    sWorldmapState* psVar1;

    psVar1 = gWorldmapState;
    if (0 < worldMapGearMode) {
        if (worldMapGearMode < 6) {
            worldmapGeometryOffsetY = 0x8c;
        }
        else if (worldMapGearMode < 8) {
            worldmapGeometryOffsetY = 0x78;
            gWorldmapState->m0[param_1].m20 = 1;
        }
    }
    psVar1->m0[param_1].m50 = worldmapGeometryOffsetY << 0xc;
    return 1;
}

s32 worldmap_taskMinimapY_update(int param_1) {
    short sVar1;
    sWorldmapState* psVar2;
    short sVar3;
    int iVar4;

    psVar2 = gWorldmapState;
    sVar1 = gWorldmapState->m0[param_1].m4;
    if (sVar1 == 9) {
        sVar3 = 1;
    }
    else {
        sVar3 = 2;
        if (sVar1 != 10) goto LAB_worldmap__800922e8;
    }
    gWorldmapState->m0[param_1].m4 = 0;
    psVar2->m0[param_1].m20 = sVar3;
LAB_worldmap__800922e8:
    sVar1 = psVar2->m0[param_1].m20;
    if (sVar1 == 1) {
        iVar4 = psVar2->m0[param_1].m50 + -0x1000;
        psVar2->m0[param_1].m50 = iVar4;
        worldmapGeometryOffsetY = iVar4 >> 0xc;
        if (0x77 < worldmapGeometryOffsetY) {
            return 1;
        }
        iVar4 = 0x78000;
        worldmapGeometryOffsetY = 0x78;
    }
    else {
        if (sVar1 < 2) {
            if (sVar1 != 0) {
                return 1;
            }
            return 3;
        }
        if (sVar1 != 2) {
            return 1;
        }
        iVar4 = psVar2->m0[param_1].m50 + 0x1000;
        psVar2->m0[param_1].m50 = iVar4;
        worldmapGeometryOffsetY = iVar4 >> 0xc;
        if (worldmapGeometryOffsetY < 0x8c) {
            return 1;
        }
        iVar4 = 0x8c000;
        worldmapGeometryOffsetY = 0x8c;
    }
    psVar2->m0[param_1].m50 = iVar4;
    psVar2->m0[param_1].m20 = 0;
    return 1;
}