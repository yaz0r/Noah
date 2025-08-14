#include "noahLib.h"
#include "validateFieldEntities.h"
#include "validation/gdbConnection.h"
#include "field/field.h"
#include "field/fieldGraphicObject.h"
#include "kernel/playTime.h"

void validate(u32 psxBase, const SVECTOR& svector) {
    assert(svector.vx == g_gdbConnection->readS16(psxBase + 0));
    assert(svector.vy == g_gdbConnection->readS16(psxBase + 2));
    assert(svector.vz == g_gdbConnection->readS16(psxBase + 4));
    //assert(svector.pad == g_gdbConnection->readS16(psxBase + 6));
}

void validate(u32 psxBase, const VECTOR& vector) {
    assert(vector.vx == g_gdbConnection->readS32(psxBase + 0));
    assert(vector.vy == g_gdbConnection->readS32(psxBase + 4));
    assert(vector.vz == g_gdbConnection->readS32(psxBase + 8));
    //assert(vector.pad == g_gdbConnection->readS32(psxBase + 0xC));
}

void validate(u32 psxBase, const SFP_VEC3& vector) {
    assert(vector.vx == g_gdbConnection->readS32(psxBase + 0));
    assert(vector.vy == g_gdbConnection->readS32(psxBase + 4));
    assert(vector.vz == g_gdbConnection->readS32(psxBase + 8));
}

void validate(u32 psxBase, const MATRIX& matrix) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            assert(g_gdbConnection->readS16(psxBase + 2 * (j * 3 + i)) == matrix.m[j][i]);
        }
    }

    validate(psxBase + 0x14, matrix.t);
}

void validate(u32 psxBase, const sFixedPoint& data) {
    assert(data.m_value == g_gdbConnection->readS32(psxBase));
}

void validate(u32 psxBase, const FP_VEC3& vec3) {
    validate(psxBase + 0x0, vec3.vx);
    validate(psxBase + 0x4, vec3.vy);
    validate(psxBase + 0x8, vec3.vz);
}

void validate(u32 psxBase, const s8& data) {
    assert(data == g_gdbConnection->readS8(psxBase + 0));
}

void validate(u32 psxBase, const u8& data) {
    assert(data == g_gdbConnection->readU8(psxBase + 0));
}

void validate(u32 psxBase, const s16& data) {
    assert(data == g_gdbConnection->readS16(psxBase + 0));
}

void validate(u32 psxBase, const u16& data) {
    assert(data == g_gdbConnection->readU16(psxBase + 0));
}

void validate(u32 psxBase, const s32& data) {
    assert(data == g_gdbConnection->readS32(psxBase + 0));
}

void validate(u32 psxBase, const u32& data) {
    assert(data == g_gdbConnection->readU32(psxBase + 0));
}

void validate(u32 psxBase, const sSpriteActorCore& pSpriteActorCore) {
    validate(psxBase + 0x0, pSpriteActorCore.m0_position);
    validate(psxBase + 0xC, pSpriteActorCore.mC_step);
    validate(psxBase + 0x18, pSpriteActorCore.m18_moveSpeed);
    validate(psxBase + 0x1C, pSpriteActorCore.m1C_gravity);
    //...
    validate(psxBase + 0x2C, pSpriteActorCore.m2C_scale);
    validate(psxBase + 0x2E, pSpriteActorCore.m2E);
    validate(psxBase + 0x30, pSpriteActorCore.m30);
    validate(psxBase + 0x32, pSpriteActorCore.m32_direction);
    validate(psxBase + 0x34, pSpriteActorCore.m34_currentSpriteFrame);
    validate(psxBase + 0x36, pSpriteActorCore.m36);
    validate(psxBase + 0x38, pSpriteActorCore.m38);
    validate(psxBase + 0x3A, pSpriteActorCore.m3A);
    validate(psxBase + 0x3C, pSpriteActorCore.m3C);

}

void validate(u32 psxBase, const sSpriteActor& pSpriteActor) {
    validate(psxBase, (const sSpriteActorCore&)pSpriteActor);
    //...
}

void validate(u32 psxBase, const sFieldScriptEntity& pFieldScriptEntity) {
    validate(psxBase + 0x00, pFieldScriptEntity.m0_fieldScriptFlags.m_rawFlags);
    validate(psxBase + 0x04, pFieldScriptEntity.m4_flags.m_rawFlags);
    validate(psxBase + 0x10, pFieldScriptEntity.m10_walkmeshId);
    validate(psxBase + 0x14, pFieldScriptEntity.m14_currentTriangleFlag);
    //validate(psxBase + 0x18, pFieldScriptEntity.m18_boundingVolume);
    validate(psxBase + 0x1E, pFieldScriptEntity.m1E_collisionRadius);
    //
    validate(psxBase + 0x20, pFieldScriptEntity.m20_position);
    validate(psxBase + 0x30, pFieldScriptEntity.m30_stepVector);
    //...
    validate(psxBase + 0xCC, pFieldScriptEntity.mCC_scriptPC);
    //...
}

template <typename T>
void validate(u32 psxBase, const T* pPtr) {
    if (pPtr) {
        psxBase = g_gdbConnection->readU32(psxBase);
        assert(psxBase);
        validate(psxBase, *pPtr);
    }
    else {
        assert(g_gdbConnection->readU32(psxBase) == 0);
    }
}

void validate(u32 entityBase, const sFieldEntity& entity) {
    validate(entityBase + 0x4, entity.m4_pVramSpriteSheet);
    validate(entityBase + 0xC, entity.mC_matrix);
    validate(entityBase + 0x2C, entity.m2C_matrixBackup);
    validate(entityBase + 0x4C, entity.m4C_scriptEntity);
    validate(entityBase + 0x50, entity.m50_modelRotation);
    validate(entityBase + 0x58, entity.m58_flags);
    validate(entityBase + 0x5A, entity.m5A);
}

void validateFieldEntities() {
    u32 entityArrayBase = g_gdbConnection->readU32(0x800AFB10);

    assert(actorArray.size() == g_gdbConnection->readU32(0x800afb0c));

    for (int i = 0; i < actorArray.size(); i++) {
        u32 entityBase = entityArrayBase + 0x5C * i;
        validate(entityBase, actorArray[i]);
    }

}

void validateFieldVars() {
    validate(0x800af880, g_cameraEye);
    validate(0x800af8b0, g_cameraEye2);
    validate(0x800af890, g_cameraAt);
    validate(0x800af8c0, g_cameraAt2);
    validate(0x800af8a0, g_cameraUp);
    validate(0x800afa54, g_cameraProjectionAngles);
    validate(0x800afa5c, g_cameraRotation);
    validate(0x800AFA64, g_currentProjectionMatrix);
    validate(0x800AF990, g_cameraMatrix);
    validate(0x800af85c, g_cameraMatrix2);
    validate(0x800af8e0, g_fieldCameraOffset);
    validate(0x800adc18, g_updateAllEntitiesSub2Var0);

    validate(0x800b21cf, g_PartyFollowDisabled);
    validate(0x800b21d0, g_menuDisabled);
    validate(0x800b21d1, g_compassDisabled);
    validate(0x800b226c, g_playerControlledActor);

    u32 arrayBase = 0x800c3a68;
    for (int i = 0; i < 0x200; i++) {
        validate(arrayBase + i * 2, fieldVars[i]);
    }
}