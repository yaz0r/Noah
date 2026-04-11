#pragma once

#include "noahLib.h"
#include "kernel/math.h"

struct sBattleTerrainTriangle {
    short m0_vertexIndices[3];
    short m6_neighborIndices[3];
    u8 mC_flags;
    u8 mD_visited;
};

extern short* battleTerrainVertices;
extern sBattleTerrainTriangle* battleTerrainTriangles;
extern int battleTerrainTriangleCount;
extern u8 battleTerrainSearchGeneration;

void initBattleTerrain(short* vertices, sBattleTerrainTriangle* triangles, int triangleCount);
void resetBattleTerrain();
int findBattleTerrainTriangle(SVECTOR* pos, int hintTriangle, int searchRadius, int unused);
int findBattleTerrainTriangleBruteForce(SVECTOR* pos);
uint computeTerrainHeightAtTriangle(SVECTOR* pos, int triangleIndex, u8* outNormal);
