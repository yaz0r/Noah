#include "noahLib.h"
#include "psx/gpuprims.h"
#include "field/particles/particles.h"
#include "kernel/memory.h"
#include "kernel/graphics.h"
#include "field/field.h"
#include "field/mecha/mechaOverlay.h"

s32 disableParticles = 0;

std::array<u8, 0x40> activeParticleEffectsTable;
std::array<s16, 0x40> activeParticleEffectActors;
s32 currentParticleCreatorId = 0;

std::array<sParticleEffect*, 0x40> fieldParticleEffects;
sParticleEffect particleCreationSetup;

struct sParticlePerTypeConfig {
    s16 m0;
    s16 m2;
    s16 m4;
    s16 m6;
    s16 m8;
    s16 mA;
    s16 mC;
    s16 mE;
    s16 m10;
    s16 m12;
    s16 m14;
    s16 m16;
    // size 0x18
};

static const std::vector<sParticlePerTypeConfig> gParticlePerTypeConfig = { {
    {0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40, 0x40}, // 0
    {0x40, 0x40, 0x00,-0x40, 0x40, 0x00, 0x80, 0x00, 0x40, 0x40, 0x80, 0x40}, // 1
    {0x40, 0x40, 0x00,-0x40, 0x40, 0x00, 0x80, 0x00, 0x40, 0x40, 0x80, 0x40}, // 2
    {0x20, 0x20, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x20, 0x60, 0x40, 0x60}, // 3
    {0x20, 0x20, 0x00, 0x00, 0x40, 0x40, 0x60, 0x40, 0x40, 0x60, 0x60, 0x60}, // 4
    {0xFE, 0x1F,0x100, 0x00, 0x00, 0x60, 0xFF, 0x60, 0x00, 0x80, 0xFF, 0x80}, // 5
    {0x10, 0x20, 0x00,-0x20, 0xE0, 0x00, 0xF0, 0x00, 0xE0, 0x20, 0xF0, 0x20}, // 6
    {0x10, 0x60, 0x00,-0x60, 0xF0, 0x00, 0xFF, 0x00, 0xF0, 0x60, 0xFF, 0x60}, // 7
    {0x60, 0x10,-0x60, 0x00, 0xF0, 0x01, 0xF0, 0x60, 0xFF, 0x01, 0xFF, 0x60}, // 8
    {0x40, 0x40, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x00, 0x80, 0x40, 0xC0, 0x40}, // 9
    {0x20, 0x20, 0x00, 0x00, 0x60, 0x40, 0x80, 0x40, 0x60, 0x60, 0x80, 0x60}, // 10
    {0x40, 0x40, 0x00, 0x00, 0x00, 0x80, 0x40, 0x80, 0x00, 0xC0, 0x40, 0xC0}, // 11
    {0x40, 0x40, 0x00, 0x00, 0x40, 0x80, 0x80, 0x80, 0x40, 0xC0, 0x80, 0xC0}, // 12
    {0x20, 0x20, 0x00, 0x00, 0x80, 0x40, 0xA0, 0x40, 0x80, 0x60, 0xA0, 0x60}, // 13
    {0x20, 0x20, 0x00, 0x00, 0xA0, 0x40, 0xC0, 0x40, 0xA0, 0x60, 0xC0, 0x60}, // 14
    {0x20, 0x20, 0x00, 0x00, 0xC0, 0x00, 0xE0, 0x00, 0xC0, 0x20, 0xE0, 0x20}, // 15
    {0x20, 0x20, 0x00, 0x00, 0xC0, 0x20, 0xE0, 0x20, 0xC0, 0x40, 0xE0, 0x40}, // 16
    {0x40, 0x40, 0x00,-0x40, 0x80, 0x80, 0xC0, 0x80, 0x80, 0xC0, 0xC0, 0xC0}, // 17
    {0x20, 0x20, 0x00, 0x00, 0xC0, 0x40, 0xE0, 0x40, 0xC0, 0x60, 0xE0, 0x60}, // 18
    {0x40, 0x40, 0x00, 0x00, 0xC0, 0x80,0x100, 0x80, 0xC0, 0xC0,0x100, 0xC0}, // 19
    {0x10, 0x10, 0x00, 0x00, 0xE0, 0x20, 0xF0, 0x20, 0xE0, 0x30, 0xF0, 0x30}, // 20
} };

void resetParticleEffectsTable() {
    for (int i = 0; i < 0x40; i++) {
        activeParticleEffectsTable[i] = 0;
        activeParticleEffectActors[i] = -1;
    }
}

s32 findFreeParticleEffectSlot() {
    for (int i = 0; i < 0x40; i++) {
        if (activeParticleEffectsTable[i] == 0) {
            return i;
        }
    }
    return -1;
}

void initParticlesPoly(sParticle* pParticle, int type, int particleAbr) {
    SetPolyFT4(&pParticle->m50[0]);

    pParticle->mA0_worldspaceVertices[0].vz = 0;
    pParticle->mA0_worldspaceVertices[1].vz = 0;
    pParticle->mA0_worldspaceVertices[2].vz = 0;
    pParticle->mA0_worldspaceVertices[3].vz = 0;
    pParticle->m50[0].r0 = 0x80;
    pParticle->m50[0].g0 = 0x80;
    pParticle->m50[0].b0 = 0x80;

    s16 sVar1 = gParticlePerTypeConfig[type].m0;
    s16 sVar2 = gParticlePerTypeConfig[type].m2;
    s16 sVar9 = gParticlePerTypeConfig[type].m4;
    s16 sVar11 = gParticlePerTypeConfig[type].m6;

    sVar9 = sVar9 * 0x10;
    s16 sVar7 = sVar9 + sVar1 * -0x10;
    sVar11 = sVar11 * 0x10;
    s16 sVar4 = sVar11 + sVar2 * -0x10;
    sVar9 = sVar1 * 0x10 + sVar9;
    sVar11 = sVar2 * 0x10 + sVar11;

    pParticle->mA0_worldspaceVertices[0].vx = sVar7;
    pParticle->mA0_worldspaceVertices[0].vy = sVar4;
    pParticle->mA0_worldspaceVertices[1].vx = sVar9;
    pParticle->mA0_worldspaceVertices[1].vy = sVar4;
    pParticle->mA0_worldspaceVertices[2].vx = sVar7;
    pParticle->mA0_worldspaceVertices[2].vy = sVar11;
    pParticle->mA0_worldspaceVertices[3].vx = sVar9;
    pParticle->mA0_worldspaceVertices[3].vy = sVar11;

    setPolyUV(&pParticle->m50[0], gParticlePerTypeConfig[type].m8, gParticlePerTypeConfig[type].mA + 0x40, gParticlePerTypeConfig[type].mC + -1, gParticlePerTypeConfig[type].mE + 0x40, gParticlePerTypeConfig[type].m10,
        gParticlePerTypeConfig[type].m12 + 0x3f, gParticlePerTypeConfig[type].m14 + -1, gParticlePerTypeConfig[type].m16 + 0x3f);
    SetSemiTrans(&pParticle->m50[0], 1);

    pParticle->m50[0].tpage = GetTPage(0, particleAbr, 0x3c0, 0x140);
    pParticle->m50[0].clut = GetClut(0x100, 0xf7);
    pParticle->m50[1] = pParticle->m50[0];
}

s32 createParticleEffect(s32 actorId) {
    s32 newParticleSlot = findFreeParticleEffectSlot();
    if (newParticleSlot == -1) {
        return -1;
    }

    resetMemoryAllocStats(8, 0);
    currentParticleCreatorId = actorId;
    activeParticleEffectsTable[newParticleSlot] = 1;
    activeParticleEffectActors[newParticleSlot] = (short)actorId;

    fieldParticleEffects[newParticleSlot] = new sParticleEffect;
    *fieldParticleEffects[newParticleSlot] = particleCreationSetup;

    for (int particleSlot = 0; particleSlot < 8; particleSlot++) {
        sParticleConfig& config = fieldParticleEffects[newParticleSlot]->m0[particleSlot];
        if (config.m6_maxParticles) {
            config.m2C_particles.resize(config.m6_maxParticles);
            for (int particleId = 0; particleId < config.m6_maxParticles; particleId++) {
                config.m2C_particles[particleId].m0_isActive = 0;
                initParticlesPoly(&config.m2C_particles[particleId], config.m54_shape, ((int)((uint)config.m2A_flags << 0x10) >> 0x18) + 1U & 3);
            }
        }
    }
    return 1;
}

void resetParticleCreationSetup(s16 slot)
{
    for (int i = 0; i < 8; i++) {
        particleCreationSetup.m0[i].m52 = slot;
        particleCreationSetup.m0[i].m2_sWait = 0;
        particleCreationSetup.m0[i].m4_eWait = 0x80;
        particleCreationSetup.m0[i].m6_maxParticles = 0;
        particleCreationSetup.m0[i].mC_sPos.vx = 0;
        particleCreationSetup.m0[i].mC_sPos.vy = 0;
        particleCreationSetup.m0[i].mC_sPos.vz = 0;
        particleCreationSetup.m0[i].m14_ePos.vx = 0;
        particleCreationSetup.m0[i].m14_ePos.vy = -0x3E8;
        particleCreationSetup.m0[i].m14_ePos.vz = 0;
        particleCreationSetup.m0[i].m8_speed = 0x8000;
        particleCreationSetup.m0[i].m50 = 0x800;
        particleCreationSetup.m0[i].m24 = 1;
        particleCreationSetup.m0[i].m1C_gravity.vx = 0;
        particleCreationSetup.m0[i].m1C_gravity.vy = 0;
        particleCreationSetup.m0[i].m1C_gravity.vz = 0;
        particleCreationSetup.m0[i].m28_eRange = 0x100;
        particleCreationSetup.m0[i].m58_peWait = 0x1C;
        particleCreationSetup.m0[i].m26_sRange = 0;
        particleCreationSetup.m0[i].m2A_flags = 0;
        particleCreationSetup.m0[i].m76_rotationAngle = 0;
        particleCreationSetup.m0[i].m56_psWait = 1;
        particleCreationSetup.m0[i].m54_shape = 0;
        particleCreationSetup.m0[i].m5A_scale.vx = 0x1C8;
        particleCreationSetup.m0[i].m5A_scale.vy = 0x1C8;
        particleCreationSetup.m0[i].m5A_scale.vz = 0x1C8;
        particleCreationSetup.m0[i].m62_scaleOffset.vx = 0x20;
        particleCreationSetup.m0[i].m62_scaleOffset.vy = 0x20;
        particleCreationSetup.m0[i].m62_scaleOffset.vz = 0x20;
        particleCreationSetup.m0[i].m6A_color[0] = 0x80;
        particleCreationSetup.m0[i].m6A_color[1] = 0x20;
        particleCreationSetup.m0[i].m6A_color[2] = 0x00;
        particleCreationSetup.m0[i].m6E_colorOffset[0] = -0x4;
        particleCreationSetup.m0[i].m6E_colorOffset[1] = -0x1;
        particleCreationSetup.m0[i].m6E_colorOffset[2] = 0x00;
        for (int j = 0; j < 8; j++) {
            particleCreationSetup.m0[i].m30[j].vx = 0;
            particleCreationSetup.m0[i].m30[j].vy = 0;
        }
    }
}
void deleteParticleEffect(int param_1) {
    if (activeParticleEffectsTable[param_1] == 1) {
        for (int i = 0; i < 8; i++) {
            if (fieldParticleEffects[param_1]->m0[i].m6_maxParticles) {
                fieldParticleEffects[param_1]->m0[0].m2C_particles.clear();
            }
        }
        delete fieldParticleEffects[param_1];
        fieldParticleEffects[param_1] = nullptr;
    }
    activeParticleEffectsTable[param_1] = 0;
    activeParticleEffectActors[param_1] = -1;
}

int evalParticleRange(int param_1)
{
    return xenoRand() * param_1 + 1 >> 0xf;
}

static const std::array<u8, 8> cameraToParticleMapping = { {
    2,3,4,5,6,7,0,1
} };

void spawnParticle(sParticleConfig* pParticleEffect, sParticle* pParticle, int* pWait) {
    pParticle->m0_isActive = 1;
    pParticle->m2_life2 = pParticleEffect->m56_psWait + *pWait;
    *pWait = *pWait + pParticleEffect->m56_psWait;
    pParticle->m4_life = pParticleEffect->m58_peWait;
    if ((pParticleEffect->m2A_flags & 1) == 0) {
        pParticle->m6 = pParticleEffect->m76_rotationAngle;
    }
    else {
        pParticle->m6 = (ushort)xenoRand() & 0xfff;
    }

    s32 uVar6;
    if ((pParticleEffect->m2A_flags & 0x80) == 0) {
        uVar6 = evalParticleRange(pParticleEffect->m26_sRange);
    }
    else {
        uVar6 = (uint)(ushort)pParticleEffect->m26_sRange;
    }
    s32 uVar7 = evalParticleRange(0xfff);
    s32 iVar5 = getAngleSin(uVar7);
    s32 iVar8;
    s32 local_30;
    if ((pParticleEffect->m2A_flags & 0x40) == 0) {
        iVar8 = getAngleCos(uVar7);
        local_30 = (int)(iVar8 * uVar6) >> 0xc;
    }
    else {
        local_30 = 0;
    }
    s32 iVar11 = ((int)(iVar5 * uVar6) >> 0xc) +
        (int)(pParticleEffect->mC_sPos).vx +
        (int)pParticleEffect->m30[cameraToParticleMapping[((int)cameraTan + (int)(actorArray[pParticleEffect->m52].m4C_scriptEntity)->m108_rotation3 & 0xfffU) >> 9]].vx;
    local_30 += (int)(pParticleEffect->mC_sPos).vz +
        (int)pParticleEffect->m30[cameraToParticleMapping[((int)cameraTan + (int)(actorArray[pParticleEffect->m52].m4C_scriptEntity)->m108_rotation3 & 0xfffU) >> 9]].vy;
    iVar5 = (int)(pParticleEffect->mC_sPos).vy;
    (pParticle->m8_position).vx = iVar11;
    (pParticle->m8_position).vz = local_30;
    (pParticle->m8_position).vy = iVar5;
    iVar8 = evalParticleRange(pParticleEffect->m28_eRange);
    s32 iVar9 = getAngleSin(uVar7);
    s32 sVar1 = (pParticleEffect->m14_ePos).vx;
    s32 iVar10 = getAngleCos(uVar7);
    s32 sVar2 = (pParticleEffect->m14_ePos).vz;
    s32 sVar3 = (pParticleEffect->m14_ePos).vy;
    (pParticle->m18_positionStep).vx = ((int)sVar1 + (iVar9 * iVar8 >> 0xc)) - iVar11;
    (pParticle->m18_positionStep).vy = sVar3 - iVar5;
    (pParticle->m18_positionStep).vz = ((int)sVar2 + (iVar10 * iVar8 >> 0xc)) - local_30;
    (pParticle->m28_positionStep2).vx = (int)(pParticleEffect->m1C_gravity).vx;
    (pParticle->m28_positionStep2).vy = (int)(pParticleEffect->m1C_gravity).vy;
    (pParticle->m28_positionStep2).vz = (int)(pParticleEffect->m1C_gravity).vz;
    (pParticle->m38_scale).vx = (pParticleEffect->m5A_scale).vx;
    (pParticle->m38_scale).vy = (pParticleEffect->m5A_scale).vy;
    (pParticle->m38_scale).vz = (pParticleEffect->m5A_scale).vz;
    (pParticle->m40_scaleStep).vx = (pParticleEffect->m62_scaleOffset).vx;
    (pParticle->m40_scaleStep).vy = (pParticleEffect->m62_scaleOffset).vy;
    (pParticle->m40_scaleStep).vz = (pParticleEffect->m62_scaleOffset).vz;
    pParticle->m48_color[0] = pParticleEffect->m6A_color[0];
    pParticle->m48_color[1] = pParticleEffect->m6A_color[1];
    pParticle->m48_color[2] = pParticleEffect->m6A_color[2];
    pParticle->m4C_colorStep[0] = pParticleEffect->m6E_colorOffset[0];
    pParticle->m4C_colorStep[1] = pParticleEffect->m6E_colorOffset[1];
    pParticle->m4C_colorStep[2] = pParticleEffect->m6E_colorOffset[2];
}

int interpolateParticleColor(int value, int step)
{
    if (step < 0) {
        value += step;
        if (value < 0) {
            value = 0;
        }
    }
    else {
        value += step;
        if (0xff < value) {
            value = 0xff;
        }
    }
    return value;
}

void projectAndRenderParticle(sParticle* pParticle, MATRIX* projectionMatrix, short param_3, int sort, VECTOR* scale, int attachementMode) {
    SFP_VEC4 local_40;
    MATRIX MStack128;

    local_40.vx = 0;
    local_40.vy = 0;
    local_40.vz = param_3;
    createRotationMatrix(&local_40, &MStack128);
    MStack128.t[0] = (pParticle->m8_position).vx >> 0xc;
    MStack128.t[1] = (pParticle->m8_position).vy >> 0xc;
    MStack128.t[2] = (pParticle->m8_position).vz >> 0xc;

    MATRIX MStack160;
    if (attachementMode == 3) {
        MATRIX local_60 = *projectionMatrix;
        ScaleMatrix(&local_60, scale);
        CompMatrix(&local_60, &MStack128, &MStack160);
        copyRotationMatrix(&MStack160, &MStack128);
        VECTOR local_38;
        local_38.vx = (long)(pParticle->m38_scale).vx;
        local_38.vy = (long)(pParticle->m38_scale).vy;
        local_38.vz = (long)(pParticle->m38_scale).vz;
        ScaleMatrix(&MStack160, &local_38);
        pParticle->m50[g_frameOddOrEven].r0 = pParticle->m48_color[0];
        pParticle->m50[g_frameOddOrEven].g0 = pParticle->m48_color[1];
        pParticle->m50[g_frameOddOrEven].b0 = pParticle->m48_color[2];
        SetTransMatrix(&MStack160);
        ScaleMatrix(&MStack160, scale);
    }
    else {
        CompMatrix(projectionMatrix, &MStack128, &MStack160);
        copyRotationMatrix(&MStack160, &MStack128);
        VECTOR local_38;
        local_38.vx = (long)(pParticle->m38_scale).vx;
        local_38.vy = (long)(pParticle->m38_scale).vy;
        local_38.vz = (long)(pParticle->m38_scale).vz;
        ScaleMatrix(&MStack160, &local_38);
        pParticle->m50[g_frameOddOrEven].r0 = pParticle->m48_color[0];
        pParticle->m50[g_frameOddOrEven].g0 = pParticle->m48_color[1];
        pParticle->m50[g_frameOddOrEven].b0 = pParticle->m48_color[2];
        SetTransMatrix(&MStack160);
    }

    long local_28;

    SetRotMatrix(&MStack160);
    s32 lVar2 = RotAverage4(&pParticle->mA0_worldspaceVertices[0], &pParticle->mA0_worldspaceVertices[1], &pParticle->mA0_worldspaceVertices[2],
        &pParticle->mA0_worldspaceVertices[3], &pParticle->m50[g_frameOddOrEven].x0y0, &pParticle->m50[g_frameOddOrEven].x1y1,
        &pParticle->m50[g_frameOddOrEven].x2y2, &pParticle->m50[g_frameOddOrEven].x3y3, &local_28, &local_28);

    s32 iVar4 = lVar2 >> (gDepthDivider & 0x1f);
    s32 iVar3;
    if (sort == 1) {
        iVar3 = iVar4 + -0x10;
    }
    else {
        if (sort < 2) {
            iVar4 = local_28;
            if (sort == 0) {
                local_28 = 1;
                iVar4 = local_28;
            }
            goto LAB_Field__800a9e74;
        }
        if ((sort == 2) || (iVar3 = iVar4 + 0x10, iVar4 = local_28, sort != 3)) goto LAB_Field__800a9e74;
    }
    iVar4 = iVar3;
LAB_Field__800a9e74:
    local_28 = iVar4;
    if (local_28 - 1U < 0xfff) {
        POLY_FT4* p = &pParticle->m50[g_frameOddOrEven];
        pParticle->m50[g_frameOddOrEven].m0_pNext = pCurrentFieldRenderingContext->mCC_OT[local_28].m0_pNext;
        pCurrentFieldRenderingContext->mCC_OT[local_28].m0_pNext = &pParticle->m50[g_frameOddOrEven];
    }
}

void updateParticle(sParticleConfig* particleEffect, sParticle* pParticle, MATRIX* projectionMatrix) {
    if (pParticle->m2_life2 == 0) {
        (pParticle->m18_positionStep).vx += (pParticle->m28_positionStep2).vx;
        (pParticle->m18_positionStep).vy += (pParticle->m28_positionStep2).vy;
        (pParticle->m18_positionStep).vz += (pParticle->m28_positionStep2).vz;

        (pParticle->m38_scale).vy += (pParticle->m40_scaleStep).vy;
        (pParticle->m38_scale).vx += (pParticle->m40_scaleStep).vx;
        (pParticle->m38_scale).vz += (pParticle->m40_scaleStep).vz;

        (pParticle->m8_position).vx += (pParticle->m18_positionStep).vx;
        (pParticle->m8_position).vy += (pParticle->m18_positionStep).vy;
        (pParticle->m8_position).vz += (pParticle->m18_positionStep).vz;


        pParticle->m48_color[0] = interpolateParticleColor(pParticle->m48_color[0], pParticle->m4C_colorStep[0]);
        pParticle->m48_color[1] = interpolateParticleColor(pParticle->m48_color[1], pParticle->m4C_colorStep[1]);
        pParticle->m48_color[2] = interpolateParticleColor(pParticle->m48_color[2], pParticle->m4C_colorStep[2]);

        FP_VEC4 scale;
        scale.vx = particleEffect->m50;
        scale.vy = particleEffect->m50;
        scale.vz = particleEffect->m50;

        if (pParticle->m4_life != 1) {
            projectAndRenderParticle(pParticle, projectionMatrix, pParticle->m6, particleEffect->m2A_flags >> 1 & 3, &scale, particleEffect->m2A_flags >> 4 & 3);
        }
        pParticle->m4_life--;
        if (pParticle->m4_life != 0) {
            return;
        }
        pParticle->m0_isActive = 0;
        return;
    }

    if (pParticle->m2_life2-- != 1) {
        return;
    }

    MATRIX local_a8;
    local_a8.t[2] = 0;
    local_a8.t[1] = 0;
    local_a8.t[0] = 0;

    bool bVar3 = false;
    VECTOR attachement;

    switch (particleEffect->m2A_flags >> 4 & 3) {
    case 0:
        {
            SVECTOR local_b0;
            local_b0.vx = 0;
            local_b0.vy = (actorArray[particleEffect->m52].m4C_scriptEntity)->m108_rotation3;
            local_b0.vz = 0;
            createRotationMatrix(&local_b0, &local_a8);
            attachement.vx = ((actorArray[particleEffect->m52].m4C_scriptEntity)->m20_position).vx.getIntegerPart();
            attachement.vy = ((actorArray[particleEffect->m52].m4C_scriptEntity)->m20_position).vy.getIntegerPart();
            attachement.vz = ((actorArray[particleEffect->m52].m4C_scriptEntity)->m20_position).vz.getIntegerPart();
        }
        break;
    case 1:
        {
            getMechaBoneMatrix(&local_a8, nullptr, particleEffect->m72, particleEffect->m74);
            SetRotMatrix(&local_a8);
            SetTransMatrix(&local_a8);
            SVECTOR local_b0;
            local_b0.vx = (particleEffect->mC_sPos).vx;
            local_b0.vy = (particleEffect->mC_sPos).vy;
            local_b0.vz = (particleEffect->mC_sPos).vz;
            RotTrans(&local_b0, &attachement, nullptr);
            particleEffect->m50 = 0x1000;
        }
        break;
    case 2:
        {
            local_a8 = actorArray[particleEffect->m52].m2C_matrixBackup;
            SetRotMatrix(&local_a8);
            SetTransMatrix(&local_a8);
            SVECTOR local_b0;
            local_b0.vx = (particleEffect->mC_sPos).vx;
            local_b0.vy = (particleEffect->mC_sPos).vy;
            local_b0.vz = (particleEffect->mC_sPos).vz;
            RotTrans(&local_b0, &attachement, nullptr);
            particleEffect->m50 = 0x1000;
        }
        break;
    default:
        assert(0);
        break;
    }

    local_a8.t[2] = 0;
    local_a8.t[1] = 0;
    local_a8.t[0] = 0;
    SetRotMatrix(&local_a8);
    SetTransMatrix(&local_a8);
    SVECTOR local_b0;
    local_b0.vx = (short)(pParticle->m18_positionStep).vx;
    local_b0.vy = (short)(pParticle->m18_positionStep).vy;
    local_b0.vz = (short)(pParticle->m18_positionStep).vz;
    VECTOR local_c0;
    ApplyRotMatrix(&local_b0, &local_c0);
    VectorNormal(&local_c0, &pParticle->m18_positionStep);
    (pParticle->m18_positionStep).vx = ((pParticle->m18_positionStep).vx * particleEffect->m8_speed >> 0xc) * (int)particleEffect->m24;
    (pParticle->m18_positionStep).vy = ((pParticle->m18_positionStep).vy * particleEffect->m8_speed >> 0xc) * (int)particleEffect->m24;
    (pParticle->m18_positionStep).vz = ((pParticle->m18_positionStep).vz * particleEffect->m8_speed >> 0xc) * (int)particleEffect->m24;
    if (bVar3) {
        (pParticle->m8_position).vx = (pParticle->m8_position).vx * (int)particleEffect->m50 >> 0xc;
        (pParticle->m8_position).vy = (pParticle->m8_position).vy * (int)particleEffect->m50 >> 0xc;
        (pParticle->m8_position).vz = (pParticle->m8_position).vz * (int)particleEffect->m50 >> 0xc;
    }
    SetRotMatrix(&local_a8);
    SetTransMatrix(&local_a8);
    local_b0.vx = (short)(pParticle->m8_position).vx;
    local_b0.vy = (short)(pParticle->m8_position).vy;
    local_b0.vz = (short)(pParticle->m8_position).vz;
    long dummy;
    RotTrans(&local_b0, &local_c0, &dummy);
    if (bVar3) {
        local_b0.vz = 0;
        local_b0.vx = (short)cameraDeltaTan + -0x400;
        local_b0.vy = -cameraTan;
        MATRIX MStack136;
        RotMatrixZYX(&local_b0, &MStack136);
        SetRotMatrix(&MStack136);
        SetTransMatrix(&MStack136);
        VECTOR local_58;
        local_58.vx = 0;
        local_58.vz = 0;
        local_58.vy = local_c0.vy;
        VECTOR local_48;
        ApplyRotMatrixLV(&local_58, &local_48);
        (pParticle->m8_position).vx = (attachement.vx + local_c0.vx + local_48.vx) * (0x1000000 / (int)particleEffect->m50);
        (pParticle->m8_position).vy = (attachement.vy + local_48.vy) * (0x1000000 / (int)particleEffect->m50);
        (pParticle->m8_position).vz = (attachement.vz + local_c0.vz + local_48.vz) * (0x1000000 / (int)particleEffect->m50);
    }
    else {
        (pParticle->m8_position).vx = (attachement.vx + local_c0.vx) * 0x1000;
        (pParticle->m8_position).vy = (attachement.vy + local_c0.vy) * 0x1000;
        (pParticle->m8_position).vz = (attachement.vz + local_c0.vz) * 0x1000;
    }
}

void updateParticles()
{
    if (isBackBufferRamCopyEnabled != 0) {
        return;
    }

    MATRIX localProjectionMatrix = currentProjectionMatrix;
    for (int i = 0; i < 0x40; i++) {
        bool isEffectStillAlive = false;
        if (activeParticleEffectsTable[i] == 1) {
            for (int j = 0; j < 8; j++) {
                int waitLocal = 0;
                sParticleConfig* pParticleEffect = &fieldParticleEffects[i]->m0[j];
                if (pParticleEffect->m6_maxParticles != 0) {
                    if (pParticleEffect->m2_sWait == 0) {
                        for (int k = 0; k < pParticleEffect->m6_maxParticles; k++) {
                            sParticle* pParticle = &pParticleEffect->m2C_particles[k];
                            if (pParticleEffect->m2C_particles[k].m0_isActive == 0) {
                                if (pParticleEffect->m4_eWait != 0) {
                                    spawnParticle(pParticleEffect, pParticle, &waitLocal);
                                    updateParticle(pParticleEffect, pParticle, &localProjectionMatrix);
                                    isEffectStillAlive = true;
                                }
                            }
                            else {
                                updateParticle(pParticleEffect, pParticle, &localProjectionMatrix);
                                isEffectStillAlive = true;
                            }
                        }
                        if (pParticleEffect->m4_eWait) {
                            if (pParticleEffect->m4_eWait != 0x7FFF) {
                                pParticleEffect->m4_eWait--;
                            }
                            isEffectStillAlive = true;
                        }
                    }
                    else {
                        isEffectStillAlive = true;
                        pParticleEffect->m2_sWait--;
                    }
                }
            }
            if (!isEffectStillAlive) {
                deleteParticleEffect(i);
            }
        }
    }

    if (fieldDebugDisable == 0) {
        assert(0);
    }
}

void stopActiveEffects(int param_1)
{
    if (activeParticleEffectsTable[param_1] == 1) {
        for (int i = 0; i < 8; i++) {
            if (fieldParticleEffects[param_1]->m0[i].m6_maxParticles != 0) {
                fieldParticleEffects[param_1]->m0[i].m4_eWait = 0;
            }
        }
    }
}

void stopActiveEffect2(int param_1)
{
    if (activeParticleEffectsTable[param_1] == 1) {
        for (int i = 0; i < 8; i++) {
            if (fieldParticleEffects[param_1]->m0[i].m6_maxParticles) {
                fieldParticleEffects[param_1]->m0[i].m4_eWait = 0;
                for (int j = 0; j < fieldParticleEffects[param_1]->m0[i].m6_maxParticles; j++) {
                    fieldParticleEffects[param_1]->m0[i].m2C_particles[j].m4_life = 0;
                }
            }
        }
    }
}

void stopParticleEffect(int param_1, int param_2)
{
    for (int i = 0; i < 0x40; i++) {
        if (activeParticleEffectActors[i] == param_1) {
            if (param_2 == 0) {
                fieldParticleEffects[i]->m0[0].m4_eWait = 0;
                fieldParticleEffects[i]->m0[0].m2_sWait = 0;
                stopActiveEffects(i);
            }
            else {
                fieldParticleEffects[i]->m0[0].m4_eWait = 0;
                fieldParticleEffects[i]->m0[0].m2_sWait = 0;
                stopActiveEffect2(i);
            }
        }
    }
}

void deleteAllParticleEffects() {
    for (int i = 0; i < 0x40; i++) {
        deleteParticleEffect(i);
    }
}
