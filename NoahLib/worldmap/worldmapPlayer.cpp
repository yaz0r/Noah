#include "noahLib.h"
#include "field/fieldGraphicObject.h"
#include "worldmap.h"
#include "kernel/gameState.h"
#include "kernel/trigo.h"
#include "worldmapWorldStreaming.h"
#include "worldmapExit.h"
#include "worldmapDynamicCollisions.h"
#include "worldmapParticles.h"
#include "worldmapFollowLeader.h"

u8 adjustLocationAfterCollisionVar0;
u8 targetVehicleEntityIndex;

std::array<s16, 6> SHORT_ARRAY_worldmap__8009b180 = { {
        1,1,0,1,1,1
} };

u8* getWorldChunkForPosition(int x, int y)
{
    bool bVar1;
    int iVar3;
    int iVar5;
    ushort uVar8;

    int tileY = y >> 0x14;
    int tileX = x >> 0x14;
    if (tileY < 0) {
        tileY = tileY + 7;
    }
    if (tileX < 0) {
        tileX = tileX + 7;
    }

    uint inTileX = x >> 0xc & 0x7ff;
    uint inTileY = y >> 0xc & 0x7ff;
    bVar1 = 0x3ff < inTileX;
    if (bVar1) {
        inTileX = inTileX - 0x400;
    }
    uVar8 = (ushort)bVar1;
    if (0x3ff < inTileY) {
        uVar8 = bVar1 | 2;
        inTileY = inTileY - 0x400;
    }
    iVar3 = (int)inTileY >> 4;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 7;
    }
    iVar5 = (int)inTileX >> 4;
    if (iVar5 < 0) {
        iVar5 = iVar5 + 7;
    }

    int tileId = (((tileY/8) * worldmapSizeX + (tileX/8)) * 0x10000 >> 0xe) / 4;
    return worldmapChunks[tileId] + ((short)uVar8 * 0x144 + (((iVar3 >> 3) * 9 + (iVar5 >> 3)) * 0x10000 >> 0xe));
}

void averageVector(VECTOR* param_1, VECTOR* param_2, VECTOR* param_3)
{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar2 = param_3->vy;
    iVar1 = -(param_3->vz * (param_1->vz - param_2->vz)) - param_3->vx * (param_1->vx - param_2->vx);
    iVar3 = iVar1 / iVar2;
    if (iVar2 == 0) {
        assert(0);
    }
    if ((iVar2 == -1) && (iVar1 == -(s32)0x80000000)) {
        assert(0);
    }
    param_1->vy = iVar3;
    param_1->vy = iVar3 + param_2->vy;
    return;
}


s32 DAT_worldmap__8009b244 = 0xB50;
s32 DAT_worldmap__8009b24c = 0xFFFFF4B0;
s32 DAT_worldmap__8009b254 = 0xB50;
s32 DAT_worldmap__8009b25c = 0xB50;

void interpolateWorldPosition(VECTOR* param_1, int x, int y)
{
    byte bVar1;
    byte* pcVar2;
    uint uVar2;
    uint uVar3;

    VECTOR DAT_1f800000;
    VECTOR VECTOR_1f800010;

    pcVar2 = getWorldChunkForPosition(x, y);
    if (x < 0) {
        x = x + 7;
    }
    uVar2 = x >> 3 & 0xffff;
    if (y < 0) {
        y = y + 7;
    }
    uVar3 = y >> 3 & 0xffff;
    if ((pcVar2[1] & 0x80) == 0) {
        if ((int)((uVar2 - 0x10000) * DAT_worldmap__8009b244 + -(s32)uVar3 * DAT_worldmap__8009b24c) < 0) {
            DAT_1f800000.vx = -0x10;
            VECTOR_1f800010.vz = 0;
            DAT_1f800000.vy = (int)(char)pcVar2[0x24] - (int)(char)pcVar2[4];
            bVar1 = *pcVar2;
        }
        else {
            DAT_1f800000.vx = 0;
            VECTOR_1f800010.vz = -0x10;
            DAT_1f800000.vy = (int)(char)pcVar2[0x28] - (int)(char)pcVar2[4];
            bVar1 = pcVar2[0x24];
        }
        VECTOR_1f800010.vx = -0x10;
        DAT_1f800000.vz = -0x10;
        VECTOR_1f800010.vy = (int)(char)bVar1 - (int)(char)pcVar2[4];
    }
    else if ((int)(uVar2 * DAT_worldmap__8009b254 + -(s32)uVar3 * DAT_worldmap__8009b25c) < 0) {
        DAT_1f800000.vx = 0x10;
        DAT_1f800000.vz = -0x10;
        VECTOR_1f800010.vx = 0;
        VECTOR_1f800010.vz = -0x10;
        DAT_1f800000.vy = (int)(char)pcVar2[0x28] - (int)(char)*pcVar2;
        VECTOR_1f800010.vy = (int)(char)pcVar2[0x24] - (int)(char)*pcVar2;
    }
    else {
        DAT_1f800000.vx = 0x10;
        VECTOR_1f800010.vx = 0x10;
        DAT_1f800000.vz = 0;
        VECTOR_1f800010.vz = -0x10;
        DAT_1f800000.vy = (int)(char)pcVar2[4] - (int)(char)*pcVar2;
        VECTOR_1f800010.vy = (int)(char)pcVar2[0x28] - (int)(char)*pcVar2;
    }

    VECTOR VECTOR_1f800020;
    OuterProduct0(&VECTOR_1f800010, &DAT_1f800000, &VECTOR_1f800020);
    VectorNormal(&VECTOR_1f800020, param_1);
    return;
}

s32 worldmapGetAltitudeFor2dPoint(s32 x, s32 y) {
    u8 bVar1;
    int iVar2;
    VECTOR local_40;
    VECTOR local_30;
    VECTOR VStack32;

    byte* pcVar2 = getWorldChunkForPosition(x, y);
    if (pcVar2 == nullptr)
        return 0;
    iVar2 = x;
    if (x < 0) {
        iVar2 = x + 7;
    }
    local_40.vx = iVar2 >> 3 & 0xffff;
    iVar2 = y;
    if (y < 0) {
        iVar2 = y + 7;
    }
    local_40.vz = -(s32)(iVar2 >> 3 & 0xffffU);
    if ((pcVar2[1] & 0x80) == 0) {
        local_30.vx = 0x10000;
        bVar1 = pcVar2[4];
    }
    else {
        local_30.vx = 0;
        bVar1 = *pcVar2;
    }
    local_30.vz = 0;
    local_30.vy = (int)(char)bVar1 << 0xc;
    interpolateWorldPosition(&VStack32, x, y);
    averageVector(&local_40, &local_30, &VStack32);
    return local_40.vy << 3;
}

s32 worldmapMode0_taskPlayer_init(int param_1)
{
    sSpriteActor* pSprite = createSpriteActor(worldmapPartySprites[0], 0x100, 0x1e0, 0x140, 0x100, 0x40);
    gWorldmapState->m0[param_1].m4C = pSprite;
    spriteActorSetPlayingAnimation(pSprite, 0);
    setGraphicEntityScale(&pSprite->m0_spriteActorCore, 0x1800);

    (pSprite->m0_spriteActorCore).m3C = (pSprite->m0_spriteActorCore).m3C & 0xfffffffb;
    gWorldmapState->m0[param_1].m24 = 0;

    gWorldmapState->m0[param_1].m28_position.vx = gameState.m1930_fieldVarsBackup[0] << 0xc;
    gWorldmapState->m0[param_1].m28_position.vz = gameState.m1930_fieldVarsBackup[1] << 0xc;
    gWorldmapState->m0[param_1].m28_position.vy = worldmapGetAltitudeFor2dPoint(gWorldmapState->m0[param_1].m28_position.vx, gWorldmapState->m0[param_1].m28_position.vz);

    gWorldmapState->m0[param_1].m38_step.vz = 0;
    gWorldmapState->m0[param_1].m38_step.vy = 0;
    gWorldmapState->m0[param_1].m38_step.vx = 0;
    gWorldmapState->m0[param_1].m4A = 8;
    gWorldmapState->m0[param_1].m48 = worldmapGamestate1824;
    gWorldmapState->m0[param_1].m5C = gWorldmapState->m0[param_1].m48;

    switch (worldMapGearMode) {
    case 1:
    case 2:
    case 3:
        if (gameState.m22B1_isOnGear[0] == 0) {

            gWorldmapState->m0[param_1].m28_position.vx = worldmapPosition.vx;
            gWorldmapState->m0[param_1].m28_position.vz = worldmapGetAltitudeFor2dPoint(worldmapPosition.vx, worldmapPosition.vz);
            gWorldmapState->m0[param_1].m28_position.vz = worldmapPosition.vz;
            gWorldmapState->m0[param_1].m48 = worldmapGamestate1824;

            worldmapRadarPosition.vx = gWorldmapState->m0[param_1].m28_position.vx;
            worldmapRadarPosition.vy = gWorldmapState->m0[param_1].m28_position.vy;
            worldmapRadarPosition.vz = gWorldmapState->m0[param_1].m28_position.vz;
            worldmapRadarPosition.pad = gWorldmapState->m0[param_1].m28_position.pad;
            worldmapVar_8009d52c = gWorldmapState->m0[param_1].m48;
        }
        else {
            gWorldmapState->m0[param_1].m20 = 1;
            gWorldmapState->m0[param_1].m24 = 1;
        }
        break;
    case 7:
        gWorldmapState->m0[param_1].m20 = 2;
        gWorldmapState->m0[param_1].m24 = 1;
        if (gameState.m1D34_currentParty[0] != 0xff) {
            gameState.m22B1_isOnGear[0] = 1;
        }
        break;
    default:
        assert(0);
    }

    worldmapFollowLeaderDataIndex = 0;
    for (int i = 0; i < 0x20; i++) {
        worldmapFollowLeaderData[i].m0_position = gWorldmapState->m0[param_1].m28_position;
        worldmapFollowLeaderData[i].m10 = gWorldmapState->m0[param_1].m48;
    }

    gameState.m1820_worldmapPosition[0] = (short)(gWorldmapState->m0[param_1].m28_position.vx >> 0xc);
    gameState.m1820_worldmapPosition[1] = (short)(gWorldmapState->m0[param_1].m28_position.vz >> 0xc);
    gameState.m1820_worldmapPosition[2] = gWorldmapState->m0[param_1].m48;

    return 1;
}

s32 worldmapUpdatePlayerControls(sWorldmapStateEntry* param_1) {
    switch (worldmapInput1_0 >> 0xc) {
    case 1:
        param_1->m48 = worldmapRotation.vy;
        break;
    case 2:
        param_1->m48 = (worldmapRotation.vy + 0x400) & 0xFFF;
        break;
    case 3:
        param_1->m48 = (worldmapRotation.vy + 0x200) & 0xFFF;
        break;
    case 4:
        param_1->m48 = (worldmapRotation.vy + 0x800) & 0xFFF;
        break;
    case 6:
        param_1->m48 = (worldmapRotation.vy + 0x600) & 0xFFF;
        break;
    case 8:
        param_1->m48 = (worldmapRotation.vy - 0x400) & 0xFFF;
        break;
    case 9:
        param_1->m48 = (worldmapRotation.vy - 0x200) & 0xFFF;
        break;
    case 0xc:
        param_1->m48 = (worldmapRotation.vy - 0x600) & 0xFFF;
    default:
        break;
    }
    if ((worldmapInput1_0 & 0xf000) != 0) {
        param_1->m38_step.vx = getAngleCos((int)param_1->m48);
        param_1->m38_step.vz = -getAngleSin((int)param_1->m48);
    }

    if ((worldmapInput2_0 & 0x20) == 0) {
        if ((worldmapExitVar0 != -1) && (worldmapCurrentExit->mE_type == 1)) {
            return 1;
        }
    }
    else {
        if (adjustLocationAfterCollisionVar0 != '\0') {
            return 3;
        }
        if (worldmapExitVar0 != -1) {
            return 1;
        }
    }
    if ((((worldmapInput2_0 & 0x10) != 0) && (worldmapExitVar1 == -1)) && (worldmapExitVar0 == -1)) {
        worldmapExitVar2 = 1;
    }

    MissingCode();

    return 0;
}

void checkWorldmapPositionSub0(VECTOR* param_1)
{
    if (worldmapSizeX * 0x800000 <= param_1->vx) {
        param_1->vx = param_1->vx + worldmapSizeX * -0x800000;
    }
    if (param_1->vx < 0) {
        param_1->vx = param_1->vx + worldmapSizeX * 0x800000;
    }
    if (worldmapSizeY * 0x800000 <= param_1->vz) {
        param_1->vz = param_1->vz + worldmapSizeY * -0x800000;
    }
    if (param_1->vz < 0) {
        param_1->vz = param_1->vz + worldmapSizeY * 0x800000;
    }
    return;
}

int worldmapCheckCollisionsAgainstWorldmapModel(VECTOR* param_1, int param_2) {
    VECTOR VECTOR_1f800040;

    VECTOR_1f800040.vx = (param_1->vx >> 0xC) - worldmapModels[param_2].m8.vx;
    VECTOR_1f800040.vz = -(param_1->vz >> 0xC) + worldmapModels[param_2].m8.vz;

    s32 diffX = std::abs(VECTOR_1f800040.vx);
    s32 diffZ = std::abs(VECTOR_1f800040.vz);

    if (diffX < 0x800 && diffZ < 0x800) {
        MATRIX MATRIX_1f8000f0 = worldmapModels[param_2].m20_rotationMatrix;
        MATRIX_1f8000f0.t[0] = 0;
        MATRIX_1f8000f0.t[1] = worldmapModels[param_2].m8.vy;
        MATRIX_1f8000f0.t[2] = 0;

        VECTOR scaleVector = { 0x800,0x800, 0x800 };
        ScaleMatrix(&MATRIX_1f8000f0, &scaleVector);
        SetRotMatrix(&MATRIX_1f8000f0);
        SetTransMatrix(&MATRIX_1f8000f0);

        std::vector<u8>::iterator pCollisionMesh = worldmapModels[param_2].m44_collisionMesh;
        s32 numVertices = READ_LE_S32(pCollisionMesh);

        assert(0); // actually not used yet
    }
    else {
        return 0;
    }
}

int worldmapCheckCollisionsAgainstWorldmapModels(VECTOR* param_1, short* param_2) {
    for (int i = 0; i < worldmapNumModels; i++) {
        if (worldmapModels[i].m4_flags & 1) {
            s32 collisionResult = worldmapCheckCollisionsAgainstWorldmapModel(param_1, i);

            if (collisionResult) {
                *param_2 = i;
                return collisionResult;
            }
        }
    }
    return 0;
}

std::array<VECTOR, 4> VECTOR_1f800000;

short getWorldmapMaterial(VECTOR* param_1)
{
    int tileZ;
    int tileX;

    tileZ = param_1->vz >> 0x14;
    if (tileZ < 0) {
        tileZ = tileZ + 7;
    }
    tileX = param_1->vx >> 0x14;
    if (tileX < 0) {
        tileX = tileX + 7;
    }

    int tileId = (((tileZ / 8) * worldmapSizeX + (tileX / 8)) * 0x10000 >> 0xe) / 4;
    return READ_LE_U16(worldmapChunks[tileId] + (((int)(param_1->vz & 0x7ff000U) >> 0x13) << 4 | (int)(param_1->vx & 0x7ff000U) >> 0x13) * 2 + 0x510);
}

static const std::array<VECTOR, 16> VECTOR_ARRAY_worldmap__8009b464 = { {
    { 0xFFFF,0x0, 0x0},
    { 0x0, 0x0, 0x0 },
    { 0x8000, 0x0, 0x0},
    { 0x7FFF, 0x0, 0x0},

    { 0xFFFF,0x0, 0x8000 },
    { 0x0,0x0, 0x8000 },
    { 0x0,0x0, 0x0 },
    { 0xFFFF,0x0, 0x0 },

    { 0xFFFF,0x0, 0x0 },
    { 0x0,0x0, 0x0 },
    { 0x8000,0x0, 0x0 },
    { 0x0,0x0, 0x8000 },

    { 0x7FFF,0x0, 0x0 },
    { 0x8000,0x0, 0x0 },
    { 0x0,0x0, 0x8000 },
    { 0xFFFF,0x0, 0x8000 },
} };

static const std::array<VECTOR, 16> VECTOR_ARRAY_worldmap__8009b364 = { {
    {0xE50, 0x0, 0x728},
    {-0xE50, 0x0, 0x728 },
    { 0xE50, 0x0, -0x728 },
    { -0xE50, 0x0, -0x728 },

    { 0x728, 0x0, 0xE50 },
    { -0x728, 0x0, 0xE50 },
    { 0x728, 0x0, -0xE50 },
    { -0x728, 0x0, -0xE50 },

    { 0xB50, 0x0, 0xB50 },
    { 0xB50, 0x0, 0xB50 },

    { 0x1000, 0x0, 0x0 },
    { 0x0, 0x0, -0x1000 },

    { -0xB50, 0, -0xB50 },
    { 0xB50, 0, -0xB50 },

    { -0xB50, 0x0, 0xB50 },
    { 0xB50, 0x0, 0xB50 },
} };

ushort getWorldmapGroundType(VECTOR* param_1)
{
    ushort uVar1;
    ushort uVar2;
    int iVar3;
    int iVar4;
    uint uVar5;

    uVar2 = getWorldmapMaterial(param_1);
    iVar3 = param_1->vx;
    uVar5 = uVar2 & 0xf;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 7;
    }
    iVar4 = param_1->vz;
    if (iVar4 < 0) {
        iVar4 = iVar4 + 7;
    }
    uVar1 = uVar2 >> 4;
    if (((int)(((iVar3 >> 3 & 0xffffU) - VECTOR_ARRAY_worldmap__8009b464[uVar5].vx) * VECTOR_ARRAY_worldmap__8009b364[uVar5].vx) >> 0xc) +
        ((int)(((iVar4 >> 3 & 0xffffU) - VECTOR_ARRAY_worldmap__8009b464[uVar5].vz) * VECTOR_ARRAY_worldmap__8009b364[uVar5].vz) >> 0xc) < 1) {
        uVar1 = uVar2 >> 7;
    }
    return uVar1 & 7;
}

static const std::array<s16, 64> SHORT_ARRAY_worldmap__8009bac8 = { {
    0,     0,     0,     0,
    0,     0,     0,     0,
    1,     0,     0,     1,
    1,     0,     0,     0,
    1,     0,     0,     1,
    0,     0,     0,     0,
    1,     0,     0,     1,
    0,     0,     0,     0,
    0,     0,     0,     1,
    0,     0,     0,     0,
    0,     0,     1,     1,
    0,     0,     0,     0,
    1,     1,     1,     1,
    1,     0,     0,     0,
    1,     1,     1,     1,
    1,     0,     0,     0,

} };

int checkWorldmapPositionSub1_0_1(int param_1, int param_2)
{
    return SHORT_ARRAY_worldmap__8009bac8[(((param_1 << 0x10) >> 0xc) + ((param_2 << 0x10) >> 0xf))/2];
}

int checkWorldmapPositionSub1_0_2(VECTOR* position, VECTOR* step, std::array<VECTOR, 4>& output, short param_4)
{
    short sVar1;
    int iVar2;
    long lVar3;
    long lVar4;
    long lVar5;
    int iVar6;
    int iVar7;

    iVar2 = step->vz;
    iVar6 = step->vx << 0xc;
    iVar7 = iVar6 / iVar2;
    if (iVar2 == 0) {
        trap(0x1c00);
    }
    if ((iVar2 == -1) && (iVar6 == -(s32)0x80000000)) {
        trap(0x1800);
    }
    iVar2 = (position->vz & 0xfff80000U) - position->vz;
    output[2].vz = iVar2;
    output[0].vz = iVar2;
    output[3].vz = output[0].vz + -1;
    iVar2 = output[3].vz;
    output[2].vx = (iVar7 * output[2].vz >> 0xc) + position->vx;
    output[3].vx = (iVar7 * iVar2 >> 0xc) + position->vx;
    output[2].vz = output[2].vz + position->vz;
    output[3].vz = output[3].vz + position->vz;
    checkWorldmapPositionSub0(&output[2]);
    sVar1 = getWorldmapGroundType(&output[2]);
    sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
    if (sVar1 == 0) {
        lVar3 = output[2].vy;
        lVar4 = output[2].vz;
        lVar5 = output[2].pad;
        output[0].vx = output[2].vx;
        output[0].vy = lVar3;
        output[0].vz = lVar4;
        output[0].pad = lVar5;
        iVar2 = 1;
    }
    else {
        checkWorldmapPositionSub0(&output[3]);
        sVar1 = getWorldmapGroundType(&output[3]);
        sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
        iVar2 = (uint)(sVar1 == 0) << 1;
    }
    return iVar2;
}

int checkWorldmapPositionSub1_0_5(VECTOR* position, VECTOR* step, std::array<VECTOR, 4>& output, short param_4)
{
    short sVar1;
    int iVar2;
    long lVar3;
    long lVar4;
    long lVar5;
    int iVar6;
    int iVar7;

    iVar2 = step->vz;
    iVar6 = step->vx << 0xc;
    iVar7 = iVar6 / iVar2;
    if (iVar2 == 0) {
        trap(0x1c00);
    }
    if ((iVar2 == -1) && (iVar6 == -(s32)0x80000000)) {
        trap(0x1800);
    }
    iVar2 = (output[1].vz & 0xfff80000U) - position->vz;
    output[2].vz = iVar2 + -1;
    output[0].vz = iVar2;
    output[3].vz = output[0].vz;
    iVar2 = output[3].vz;
    output[2].vx = (iVar7 * output[2].vz >> 0xc) + position->vx;
    output[3].vx = (iVar7 * iVar2 >> 0xc) + position->vx;
    output[2].vz = output[2].vz + position->vz;
    output[3].vz = output[3].vz + position->vz;
    checkWorldmapPositionSub0(&output[2]);
    sVar1 = getWorldmapGroundType(&output[2]);
    sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
    if (sVar1 == 0) {
        lVar3 = output[2].vy;
        lVar4 = output[2].vz;
        lVar5 = output[2].pad;
        output[0].vx = output[2].vx;
        output[0].vy = lVar3;
        output[0].vz = lVar4;
        output[0].pad = lVar5;
        iVar2 = 1;
    }
    else {
        checkWorldmapPositionSub0(&output[3]);
        sVar1 = getWorldmapGroundType(&output[3]);
        sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
        iVar2 = (uint)(sVar1 == 0) << 1;
    }
    return iVar2;
}

int checkWorldmapPositionSub1_0_4(VECTOR* position, VECTOR* step, std::array<VECTOR, 4>& output, short param_4)
{
    short sVar1;
    int iVar2;
    uint uVar3;
    long lVar4;
    long lVar5;
    long lVar6;
    int iVar7;
    int iVar8;

    iVar2 = step->vx;
    iVar7 = step->vz << 0xc;
    iVar8 = iVar7 / iVar2;
    if (iVar2 == 0) {
        trap(0x1c00);
    }
    if ((iVar2 == -1) && (iVar7 == -(s32)0x80000000)) {
        trap(0x1800);
    }
    iVar2 = (position->vx & 0xfff80000U) - position->vx;
    output[2].vx = iVar2;
    output[0].vx = iVar2;
    output[3].vx = output[0].vx + -1;
    iVar2 = output[3].vx;
    output[2].vz = (iVar8 * output[2].vx >> 0xc) + position->vz;
    output[3].vz = (iVar8 * iVar2 >> 0xc) + position->vz;
    output[2].vx = output[2].vx + position->vx;
    output[3].vx = output[3].vx + position->vx;
    checkWorldmapPositionSub0(&output[2]);
    sVar1 = getWorldmapGroundType(&output[2]);
    sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
    if (sVar1 == 0) {
        lVar4 = output[2].vy;
        lVar5 = output[2].vz;
        lVar6 = output[2].pad;
        output[0].vx = output[2].vx;
        output[0].vy = lVar4;
        output[0].vz = lVar5;
        output[0].pad = lVar6;
        uVar3 = 1;
    }
    else {
        checkWorldmapPositionSub0(&output[3]);
        sVar1 = getWorldmapGroundType(&output[3]);
        sVar1 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar1);
        uVar3 = -(s32)(sVar1 == 0) & 3;
    }
    return uVar3;
}

int checkWorldmapPositionSub1_0_3(VECTOR* position, VECTOR* step, std::array<VECTOR, 4>& output, short param_4)
{
    byte bVar1;
    short sVar2;
    int iVar3;
    long lVar4;
    long lVar5;
    long lVar6;
    int iVar7;
    int iVar8;

    iVar3 = step->vx;
    iVar7 = step->vz << 0xc;
    iVar8 = iVar7 / iVar3;
    if (iVar3 == 0) {
        trap(0x1c00);
    }
    if ((iVar3 == -1) && (iVar7 == -(s32)0x80000000)) {
        trap(0x1800);
    }
    iVar3 = (output[1].vx & 0xfff80000U) - position->vx;
    output[2].vx = iVar3 + -1;
    output[0].vx = iVar3;
    output[3].vx = output[0].vx;
    iVar3 = output[3].vx;
    output[2].vz = (iVar8 * output[2].vx >> 0xc) + position->vz;
    output[3].vz = (iVar8 * iVar3 >> 0xc) + position->vz;
    output[2].vx = output[2].vx + position->vx;
    output[3].vx = output[3].vx + position->vx;
    checkWorldmapPositionSub0(&output[2]);
    sVar2 = getWorldmapGroundType(&output[2]);
    sVar2 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar2);
    if (sVar2 == 0) {
        lVar4 = output[2].vy;
        lVar5 = output[2].vz;
        lVar6 = output[2].pad;
        output[0].vx = output[2].vx;
        output[0].vy = lVar4;
        output[0].vz = lVar5;
        output[0].pad = lVar6;
        bVar1 = 1;
    }
    else {
        checkWorldmapPositionSub0(&output[3]);
        sVar2 = getWorldmapGroundType(&output[3]);
        sVar2 = checkWorldmapPositionSub1_0_1((int)param_4, (int)sVar2);
        bVar1 = -(sVar2 == 0) & 3;
    }
    return bVar1;
}

int checkWorldmapPositionSub1_0(VECTOR* position, VECTOR* step, int stepScale, short param_4) {
    VECTOR_1f800000[1].vx = position->vx + (step->vx * stepScale >> 0xc);
    VECTOR_1f800000[1].vz = position->vz + (step->vz * stepScale >> 0xc);

    SVECTOR SVECTOR_1f8000a0;
    SVECTOR SVECTOR_1f8000a8;
    SVECTOR_1f8000a0.vx = (short)(position->vx >> 0x13);
    SVECTOR_1f8000a8.vx = (short)(VECTOR_1f800000[1].vx >> 0x13);
    SVECTOR_1f8000a8.vz = (short)(VECTOR_1f800000[1].vz >> 0x13);
    SVECTOR_1f8000a0.vz = (short)(position->vz >> 0x13);

    u32 flag = 0;
    if (VECTOR_1f800000[1].vx >> 0x13 < (position->vx >> 0x13)) {
        flag = 2;
    }
    else if ((position->vx >> 0x13) < VECTOR_1f800000[1].vx >> 0x13) {
        flag = 1;
    }
    if (SVECTOR_1f8000a8.vz < SVECTOR_1f8000a0.vz) {
        flag = flag | 8;
    }
    else if (SVECTOR_1f8000a0.vz < SVECTOR_1f8000a8.vz) {
        flag = flag | 4;
    }

    switch (flag) {
    case 1:
        if (!checkWorldmapPositionSub1_0_3(position, step, VECTOR_1f800000, param_4)) {
            checkWorldmapPositionSub0(&VECTOR_1f800000[1]);
            if (!checkWorldmapPositionSub1_0_1((int)param_4, getWorldmapGroundType(&VECTOR_1f800000[1]))) {
                VECTOR_1f800000[0] = VECTOR_1f800000[1];
                return 1;
            }
        }
        return 0;
    case 2:
        if (!checkWorldmapPositionSub1_0_4(position, step, VECTOR_1f800000, param_4)) {
            checkWorldmapPositionSub0(&VECTOR_1f800000[1]);
            if (!checkWorldmapPositionSub1_0_1((int)param_4, getWorldmapGroundType(&VECTOR_1f800000[1]))) {
                VECTOR_1f800000[0] = VECTOR_1f800000[1];
                return 1;
            }
        }
        return 0;
    case 4:
        if (!checkWorldmapPositionSub1_0_5(position, step, VECTOR_1f800000, param_4)) {
            checkWorldmapPositionSub0(&VECTOR_1f800000[1]);
            if (!checkWorldmapPositionSub1_0_1((int)param_4, getWorldmapGroundType(&VECTOR_1f800000[1]))) {
                VECTOR_1f800000[0] = VECTOR_1f800000[1];
                return 1;
            }
        }
        return 0;
    case 8:
        if (!checkWorldmapPositionSub1_0_2(position, step, VECTOR_1f800000, param_4)) {
            checkWorldmapPositionSub0(&VECTOR_1f800000[1]);
            if (!checkWorldmapPositionSub1_0_1((int)param_4, getWorldmapGroundType(&VECTOR_1f800000[1]))) {
                VECTOR_1f800000[0] = VECTOR_1f800000[1];
                return 1;
            }
        }
        return 0;
    case 0:
    case 3:
    case 7:
        if (true) {
            checkWorldmapPositionSub0(&VECTOR_1f800000[1]);
            if (!checkWorldmapPositionSub1_0_1((int)param_4, getWorldmapGroundType(&VECTOR_1f800000[1]))) {
                VECTOR_1f800000[0] = VECTOR_1f800000[1];
                return 1;
            }
        }
        return 0;
    default:
        //assert(0);
        MissingCode();
    }

    MissingCode();
    return 0;
}

uint getWorldmapMaterial2(VECTOR* param_1)
{
    ushort uVar1;

    uVar1 = getWorldmapMaterial(param_1);
    return uVar1 & 0xf;
}

static const std::array<VECTOR, 16> VECTOR_ARRAY_worldmap__8009b264 = { {
    {-0x728, 0, 0xE50 },
    { 0x728, 0, 0xE50 },
    { 0x728, 0, 0xE50 },
    {-0x728, 0, 0xE50 },

    {-0xE50, 0, 0x728 },
    { 0xE50, 0, 0x728 },
    { 0xE50, 0, 0x728 },
    {-0xE50, 0, 0x728 },

    {-0xB50, 0, 0xB50 },
    { 0xB50, 0, 0xB50 },

    { 0x0, 0, 0x1000 },
    { 0x1000, 0, 0 },

    {-0xB50, 0, 0xB50 },
    { 0xB50, 0, 0xB50 },
    { 0xB50, 0, 0xB50 },
    {-0xB50, 0, 0xB50 },
} };

int checkWorldmapPositionSub1_1(VECTOR* collision, VECTOR* step, VECTOR* outputStep)
{
    uint uVar1;
    int iVar3;
    int iVar5;

    uVar1 = getWorldmapMaterial2(collision);
    iVar5 = (int)(uVar1 << 0x10) >> 0xc;

    iVar3 = step->vx * VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vx + step->vz * VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vz;

    if (iVar3 == 0) {
        outputStep->vx = step->vx;
        outputStep->vz = step->vz;
    }
    else {
        if (iVar3 < 0) {
            outputStep->vx = -VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vx;
            outputStep->vz = -VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vz;
        }
        else {
            outputStep->vx = VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vx;
            outputStep->vz = VECTOR_ARRAY_worldmap__8009b264[iVar5 / 0x10].vz;
        }
        return 1;
    }
    return 0;
}

int checkWorldmapPositionSub1(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, short param_5) {
    int iVar1;
    long lVar2;
    int resultValue;

    iVar1 = checkWorldmapPositionSub1_0(position, step, stepScale, param_5);
    if (iVar1 == 1) {
        iVar1 = checkWorldmapPositionSub1_1(&VECTOR_1f800000[0], step, output);
        resultValue = 0;
        if (iVar1 == 0) {
            output->vz = 0;
            output->vy = 0;
            output->vx = 0;
        }
    }
    else if (iVar1 < 2) {
        if (iVar1 == 0) {
            resultValue = 1;
        }
    }
    else if (iVar1 == 2) {
        lVar2 = 0x1000;
        if (step->vx < 0) {
            lVar2 = -0x1000;
        }
        resultValue = 0;
        output->vx = lVar2;
        output->vz = 0;
        output->vy = 0;
    }
    else if (iVar1 == 3) {
        output->vy = 0;
        output->vx = 0;
        lVar2 = 0x1000;
        if (step->vz < 0) {
            lVar2 = -0x1000;
        }
        output->vz = lVar2;
        resultValue = 0;
    }
    return resultValue;
}

int checkWorldmapPosition(VECTOR* position, VECTOR* step, VECTOR* output, int stepScale, int param_5) {
    VECTOR VECTOR_1f800060;

    VECTOR_1f800060.vx = position->vx + (step->vx * stepScale >> 0xc);
    VECTOR_1f800060.vz = position->vz + (step->vz * stepScale >> 0xc);
    checkWorldmapPositionSub0(&VECTOR_1f800060);
    VECTOR_1f800060.vy = worldmapGetAltitudeFor2dPoint(VECTOR_1f800060.vx, VECTOR_1f800060.vz) + -0x4000;

    output->vx = position->vx + (step->vx * stepScale >> 0xc);
    output->vz = position->vz + (step->vz * stepScale >> 0xc);
    checkWorldmapPositionSub0(output);
    output->vy = worldmapGetAltitudeFor2dPoint(output->vx, output->vz) - 0x4000;

    s32 collisionMode = 1;
    if (worldmapModelVar1 != -1) {
        collisionMode = 3;
    }

    s32 resultValue;

    switch (collisionMode) {
    case 1:
    {
        s16 worldModelCollisionId;
        if (worldmapCheckCollisionsAgainstWorldmapModels(&VECTOR_1f800060, &worldModelCollisionId)) {
            assert(0);
        }
        resultValue = checkWorldmapPositionSub1(position, step, output, stepScale, param_5);
        worldmapModelVar0 = -1;
        worldmapModelVar1 = -1;
        break;
    }
    default:
        assert(0);
    }

    return resultValue;
}

s32 worldmapMode0_taskPlayer_update(int param_1)
{
    sWorldmapStateEntry* pEntry = &gWorldmapState->m0[param_1];

    switch (pEntry->m4) {
    case 0:
        break;
    case 2:
        pEntry->m4 = 0;
        pEntry->m20 = 0x28;
        break;
    case 3:
        pEntry->m4 = 0;
        pEntry->m20 = 1;
        break;
    case 6:
        assert(0);
        break;
    default:
        assert(0);
        break;
    }

    switch (pEntry->m20) {
    case 0:
    case 1: // on foot
        if (gameState.m22B1_isOnGear[0] == 0) {
            switch (worldmapUpdatePlayerControls(pEntry)) {
            case 1:
                pEntry->m20 = 0x40;
                continueWorldmapLoop = 0;
                exitWorldMapMode = 0;
                break;
            case 2:
            case 4:
            case 5:
                break;
            default:
                if ((pEntry->m38_step.vx == 0 && pEntry->m38_step.vy == 0) && pEntry->m38_step.vz == 0) {
                    if ((pEntry->m4C->m0_spriteActorCore.mAC.mx18) != '\0') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 0);
                        MissingCode();
                    }
                }
                else {
                    if ((pEntry->m4C->m0_spriteActorCore.mAC.mx18) != '\x01') {
                        spriteActorSetPlayingAnimation(pEntry->m4C, 1);
                    }
                    MissingCode();
                }

                VECTOR DAT_1f800090;
                s32 collisionResult2 = checkWorldmapPosition(&pEntry->m28_position, &pEntry->m38_step, &DAT_1f800090, (int)pEntry->m4A << 0xc, worldMapGearMode);

                if ((collisionResult2 & 0xffff) == 0) {
                    pEntry->m38_step = DAT_1f800090;
                    collisionResult2 = checkWorldmapPosition(&pEntry->m28_position, &pEntry->m38_step, &DAT_1f800090, (int)pEntry->m4A << 0xc, worldMapGearMode);
                    if ((collisionResult2 & 0xffff) == 0) {
                        (pEntry->m38_step).vz = 0;
                        (pEntry->m38_step).vx = 0;
                    }
                }
                if ((collisionResult2 & 0xffff) == 1) {
                    processWorldmapDynamicCollisions(&DAT_1f800090, 0x10, 0x20, &adjustLocationAfterCollisionVar0, &targetVehicleEntityIndex);

                    if (targetVehicleEntityIndex == 7) {
                        collisionResult2 = adjustLocationAfterCollisionVar0 + 3;
                    }
                    else {
                        collisionResult2 = adjustLocationAfterCollisionVar0;
                    }

                    if (SHORT_ARRAY_worldmap__8009b180[((int)(collisionResult2 << 0x10) >> 0xf) / 2]) {
                        pEntry->m28_position = DAT_1f800090;
                        if (((pEntry->m38_step).vx | (pEntry->m38_step).vz) != 0) {
                            worldmapFollowLeaderDataIndex = (worldmapFollowLeaderDataIndex + 1) & 0x1F;
                            worldmapFollowLeaderData[worldmapFollowLeaderDataIndex].m0_position = pEntry->m28_position;
                            worldmapFollowLeaderData[worldmapFollowLeaderDataIndex].m10 = pEntry->m48;
                            followLeaderPostProcess();
                        }
                    }
                }
                else {
                    MissingCode();
                }

                setupWorldmapExits(&pEntry->m28_position, 0);

                (pEntry->m38_step).vz = 0;
                (pEntry->m38_step).vy = 0;
                (pEntry->m38_step).vx = 0;
                worldmapRadarPosition.vx = (pEntry->m28_position).vx;
                worldmapRadarPosition.vy = (pEntry->m28_position).vy;
                worldmapRadarPosition.vz = (pEntry->m28_position).vz;
                worldmapRadarPosition.pad = (pEntry->m28_position).pad;
                worldmapVar_8009d52c = pEntry->m48;
                break;
            }
            currentWorldmapDynamicCollisionSlot = 0;
            pEntry->m24 = 0;
        }
        else {
            pEntry->m24 = 1;
            (pEntry->m28_position).vx = gWorldmapState->m0[4].m28_position.vx;
            (pEntry->m28_position).vy = gWorldmapState->m0[4].m28_position.vy;
            (pEntry->m28_position).vz = gWorldmapState->m0[4].m28_position.vz;
            pEntry->m48 = gWorldmapState->m0[4].m48;
            clearWorldmapParticles(0x2f);
        }
        break;
    case 2: // yggdrasil 
        (pEntry->m28_position).vx = gWorldmapState->m0[7].m28_position.vx;
        (pEntry->m28_position).vy = gWorldmapState->m0[7].m28_position.vy;
        (pEntry->m28_position).vz = gWorldmapState->m0[7].m28_position.vz;
        pEntry->m48 = gWorldmapState->m0[7].m48;
        break;
    default:
        assert(0);
    }

    gameState.m1820_worldmapPosition[0] = (short)((pEntry->m28_position).vx >> 0xc);
    gameState.m1820_worldmapPosition[1] = (short)((pEntry->m28_position).vz >> 0xc);
    gameState.m1820_worldmapPosition[2] = pEntry->m48;
    if (pEntry->m24 == 0) {
        MissingCode();
    }
    return 1;
}