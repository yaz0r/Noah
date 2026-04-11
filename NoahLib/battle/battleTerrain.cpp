#include "noahLib.h"
#include "battleTerrain.h"
#include "kernel/math.h"

short* battleTerrainVertices = nullptr;   // DAT_Battle__800d3344 - array of vertices (8 bytes each: x,y,z,pad)
sBattleTerrainTriangle* battleTerrainTriangles = nullptr; // DAT_Battle__800d39cc
int battleTerrainTriangleCount = 0;       // DAT_Battle__800d3348
u8 battleTerrainSearchGeneration = 0;     // DAT_Battle__800d2f64

void initBattleTerrain(short* vertices, sBattleTerrainTriangle* triangles, int triangleCount) {
    battleTerrainSearchGeneration = 1;
    battleTerrainVertices = vertices;
    battleTerrainTriangleCount = triangleCount;
    battleTerrainTriangles = triangles;
    if (triangles != nullptr && triangleCount > 0) {
        for (int i = 0; i < triangleCount; i++) {
            battleTerrainTriangles[i].mD_visited = 0;
        }
    }
    if (triangleCount == 0) {
        battleTerrainVertices = nullptr;
        battleTerrainTriangles = nullptr;
    }
}

void resetBattleTerrain() {
    battleTerrainVertices = nullptr;
    battleTerrainTriangles = nullptr;
    battleTerrainTriangleCount = 0;
}

static int isPointInsideTriangleXZ(short* v0, short* v1, short* v2, short* point) {
    VECTOR edge;
    VECTOR toPoint;
    VECTOR cross;

    edge.vy = 0;
    edge.vx = (int)v1[0] - (int)v0[0];
    edge.vz = (int)v1[2] - (int)v0[2];
    toPoint.vy = 0;
    toPoint.vx = (int)point[0] - (int)v0[0];
    toPoint.vz = (int)point[2] - (int)v0[2];
    OuterProduct0(&edge, &toPoint, &cross);
    if (cross.vy < 0) {
        return 0;
    }

    edge.vy = 0;
    edge.vx = (int)v2[0] - (int)v1[0];
    edge.vz = (int)v2[2] - (int)v1[2];
    toPoint.vy = 0;
    toPoint.vx = (int)point[0] - (int)v1[0];
    toPoint.vz = (int)point[2] - (int)v1[2];
    OuterProduct0(&edge, &toPoint, &cross);
    if (cross.vy < 0) {
        return 0;
    }

    edge.vy = 0;
    edge.vx = (int)v0[0] - (int)v2[0];
    edge.vz = (int)v0[2] - (int)v2[2];
    toPoint.vy = 0;
    toPoint.vx = (int)point[0] - (int)v2[0];
    toPoint.vz = (int)point[2] - (int)v2[2];
    OuterProduct0(&edge, &toPoint, &cross);
    return ~cross.vy >> 31;
}

static int findBattleTerrainTriangleRecursive(SVECTOR* pos, int triangleIndex, int depth) {
    if (triangleIndex < 0) {
        return -1;
    }
    sBattleTerrainTriangle* tri = &battleTerrainTriangles[triangleIndex];
    if (tri->mD_visited != battleTerrainSearchGeneration) {
        tri->mD_visited = battleTerrainSearchGeneration;
        short* v0 = battleTerrainVertices + tri->m0_vertexIndices[0] * 4;
        short* v1 = battleTerrainVertices + tri->m0_vertexIndices[1] * 4;
        short* v2 = battleTerrainVertices + tri->m0_vertexIndices[2] * 4;
        int result = isPointInsideTriangleXZ(v0, v1, v2, &pos->vx);
        if (result == -1) {
            return triangleIndex;
        }
    }
    if (depth > 0) {
        int nextDepth = depth - 1;
        int result = findBattleTerrainTriangleRecursive(pos, tri->m6_neighborIndices[0], nextDepth);
        if (result >= 0) return result;
        result = findBattleTerrainTriangleRecursive(pos, tri->m6_neighborIndices[1], nextDepth);
        if (result >= 0) return result;
        result = findBattleTerrainTriangleRecursive(pos, tri->m6_neighborIndices[2], nextDepth);
        if (result >= 0) return result;
    }
    return -1;
}

int findBattleTerrainTriangle(SVECTOR* pos, int hintTriangle, int searchRadius, int /*unused*/) {
    if (battleTerrainVertices == nullptr) {
        return -1;
    }
    if (battleTerrainTriangles == nullptr) {
        return -1;
    }
    if (hintTriangle >= battleTerrainTriangleCount) {
        return -1;
    }

    int result = -1;
    for (int i = 0; i < searchRadius; i++) {
        result = findBattleTerrainTriangleRecursive(pos, hintTriangle, searchRadius);
        if (result >= 0) break;
    }

    if ((u8)(battleTerrainSearchGeneration + 1) != 0) {
        battleTerrainSearchGeneration = battleTerrainSearchGeneration + 1;
        return result;
    }
    battleTerrainSearchGeneration = 1;
    for (int i = 0; i < battleTerrainTriangleCount; i++) {
        battleTerrainTriangles[i].mD_visited = 0;
    }
    return result;
}

int findBattleTerrainTriangleBruteForce(SVECTOR* pos) {
    if (battleTerrainVertices == nullptr) {
        return -1;
    }
    if (battleTerrainTriangles == nullptr) {
        return -1;
    }
    for (int i = 0; i < battleTerrainTriangleCount; i++) {
        sBattleTerrainTriangle* tri = &battleTerrainTriangles[i];
        short* v0 = battleTerrainVertices + tri->m0_vertexIndices[0] * 4;
        short* v1 = battleTerrainVertices + tri->m0_vertexIndices[1] * 4;
        short* v2 = battleTerrainVertices + tri->m0_vertexIndices[2] * 4;
        int result = isPointInsideTriangleXZ(v0, v1, v2, &pos->vx);
        if (result == -1) {
            return i;
        }
    }
    return -1;
}

uint computeTerrainHeightAtTriangle(SVECTOR* pos, int triangleIndex, u8* outNormal) {
    if (battleTerrainVertices == nullptr || battleTerrainTriangles == nullptr || triangleIndex < 0) {
        return -1;
    }
    sBattleTerrainTriangle* tri = &battleTerrainTriangles[triangleIndex];
    short* v0 = battleTerrainVertices + tri->m0_vertexIndices[0] * 4;
    short* v1 = battleTerrainVertices + tri->m0_vertexIndices[1] * 4;
    short* v2 = battleTerrainVertices + tri->m0_vertexIndices[2] * 4;

    VECTOR edge0;
    edge0.vx = (int)v1[0] - (int)v0[0];
    edge0.vy = (int)v1[1] - (int)v0[1];
    edge0.vz = (int)v1[2] - (int)v0[2];

    VECTOR edge1;
    edge1.vx = (int)v2[0] - (int)v0[0];
    edge1.vy = (int)v2[1] - (int)v0[1];
    edge1.vz = (int)v2[2] - (int)v0[2];

    VECTOR normEdge0;
    VectorNormal(&edge0, &normEdge0);

    VECTOR normEdge1;
    VectorNormal(&edge1, &normEdge1);

    OuterProduct12(&normEdge0, &normEdge1, (VECTOR*)outNormal);

    int ny = ((VECTOR*)outNormal)->vy;
    if (ny == 0) {
        pos->vy = 0;
    }
    else {
        int num = -(((int)pos->vz - (int)v0[2]) * ((VECTOR*)outNormal)->vz)
                  - ((int)pos->vx - (int)v0[0]) * ((VECTOR*)outNormal)->vx;
        pos->vy = v0[1] + (short)(num / ny);
    }

    return (uint)tri->mC_flags;
}
