#include "noahLib.h"
#include "validateFieldEntities.h"
#include "validation/gdbConnection.h"
#include "field/field.h"
#include "field/fieldGraphicObject.h"
#include "kernel/playTime.h"

void validate(u32 psxBase, const MATRIX& matrix) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            assert(g_gdbConnection->readS16(psxBase + 2 * (j * 3 + i)) == matrix.m[j][i]);
        }
    }
}

void validate(u32 psxBase, const SVECTOR& svector) {
    assert(svector.vx == g_gdbConnection->readS16(psxBase + 0));
    assert(svector.vy == g_gdbConnection->readS16(psxBase + 2));
    assert(svector.vz == g_gdbConnection->readS16(psxBase + 4));
    assert(svector.pad == g_gdbConnection->readS16(psxBase + 6));
}

void validate(u32 psxBase, const sFixedPoint& data) {
    assert(data.m_value == g_gdbConnection->readS32(psxBase));
}

void validate(u32 psxBase, const FP_VEC3& vec3) {
    validate(psxBase + 0x0, vec3.vx);
    validate(psxBase + 0x4, vec3.vy);
    validate(psxBase + 0x8, vec3.vz);
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
    u32 arrayBase = 0x800c3a68;
    for (int i = 0; i < 0x200; i++) {
        validate(arrayBase + i * 2, fieldVars[i]);
    }
}