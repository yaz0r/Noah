#include "noahLib.h"
#include "worldmap.h"
#include "worldmapMinimap.h"
#include "psx/gpuprims.h"
#include "kernel/graphics.h"
#include "kernel/gte.h"

s32 worldmapMinimapScale = 0;

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

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            SetPolyG3(&worldmapMinimapRadar[i][j]);
            worldmapMinimapRadar[i][j].r0 = 0xFF;
            worldmapMinimapRadar[i][j].g0 = 0x40;
            worldmapMinimapRadar[i][j].b0 = 0x40;

            worldmapMinimapRadar[i][j].r1 = 0;
            worldmapMinimapRadar[i][j].g1 = 0;
            worldmapMinimapRadar[i][j].b1 = 0;

            worldmapMinimapRadar[i][j].r2 = 0;
            worldmapMinimapRadar[i][j].g2 = 0;
            worldmapMinimapRadar[i][j].b2 = 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 32; j++) {
            SetTile(&worldmapMinimapPOIsPolys[i][j]);

            worldmapMinimapPOIsPolys[i][j].h = 2;
            worldmapMinimapPOIsPolys[i][j].w = 2;

            worldmapMinimapPOIsPolys[i][j].r0 = 0x10;
            worldmapMinimapPOIsPolys[i][j].g0 = 0x80;
            worldmapMinimapPOIsPolys[i][j].b0 = 0x80;
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
        radarMatrix.t[1] = (worldmapRadarPosition.vz >> 0xC) / 0x155 + 0x78 - worldmapGeometryOffsetY;
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

    MissingCode();

    worldmapMinimapBackgroundPoly[worldmapOddOrEven].m0_pNext = pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext;
    pCurrentWorldmapRenderingStruct->m70_OT[0].m0_pNext = &worldmapMinimapBackgroundPoly[worldmapOddOrEven];
}