#pragma once

struct sParticle {
    s16 m0_isActive;
    s16 m2_life2;
    s16 m4_life;
    s16 m6;
    VECTOR m8_position;
    VECTOR m18_positionStep;
    VECTOR m28_positionStep2;
    SVECTOR m38_scale;
    SVECTOR m40_scaleStep;
    std::array<u8, 4> m48_color;
    std::array<s8, 4> m4C_colorStep;
    std::array<POLY_FT4, 2> m50;
    std::array<SVECTOR, 4> mA0_worldspaceVertices;
    // size 0xC0
};

struct sParticleConfig {
    s16 m0;
    s16 m2_sWait;
    s16 m4_eWait;
    s16 m6_maxParticles;
    s32 m8_speed;
    SVECTOR mC_sPos;
    SVECTOR m14_ePos;
    SVECTOR m1C_gravity;
    s16 m24;
    s16 m26_sRange;
    s16 m28_eRange;
    u16 m2A_flags; // &1 = randomRot, >>1 & 3 = SORT (0:TOP, 1:MID, 2:NORMAL, 3:BACK), >>8 & 3 = COLMODE (normal blendin modes), >>6&3=RANGEMOD (0:RANDOM, 1:LINE, 2:CIRCLE)
    // >> 4 & 3: attachement mode (0: actor position, 1: mecha position, 2: actor matrix, 3: actor positin and scale) 
    std::vector<sParticle> m2C_particles;
    std::array<sVec2_s16, 8> m30;
    s16 m50;
    s16 m52;
    s16 m54_shape;
    s16 m56_psWait;
    s16 m58_peWait;
    SVECTOR m5A_scale;
    SVECTOR m62_scaleOffset;
    std::array<u8, 4> m6A_color;
    std::array<s8, 4> m6E_colorOffset;
    s16 m72;
    s16 m74;
    s16 m76_rotationAngle;
    // size 0x78
};

struct sParticleEffect {
    std::array<sParticleConfig, 8> m0;
    // size 0x3C0
};
extern std::array<sParticleEffect*, 0x40> fieldParticleEffects;
extern sParticleEffect particleCreationSetup;

extern s32 disableParticles;
extern s32 currentParticleCreatorId;

void resetParticleEffectsTable();
s32 createParticleEffect(s32 actorId);
void resetParticleCreationSetup(s16 slot);
void deleteAllParticleEffects();
void updateParticles();
void stopParticleEffect(int param_1, int param_2);