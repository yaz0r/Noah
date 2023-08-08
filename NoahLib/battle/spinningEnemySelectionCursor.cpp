#include "noahLib.h"
#include "battle.h"
#include "battleRenderContext.h"
#include "field/field.h"
#include "customPolyBuffer.h"

#include "spinningEnemySelectionCursor.h"

#include "battleCursor.generated.h"

struct sBattleSpinningEnemySelectionCursor : public sTaskHeaderPair {
    sSpriteActorCore* m38;
    VECTOR m3C_translation;
    SVECTOR m4C_rotation;
    u32 m54;
    u32 m58_tick;
    std::array<std::vector<sAnyPoly>, 2> m5C_buffer;
    const sCustomPolySubBuffer* m64_customPolySubBuffer;
};

sCustomPolyBuffer battleBlinkingPolyBuffer(battleCursor_bin, battleCursor_bin_size);

void updateColorFromSin(sSpriteActorCore* param_1, int param_2) {
    int iVar1;
    byte bVar2;

    iVar1 = getAngleCos(param_2 << 6);
    iVar1 = (iVar1 + 0x1000 >> 6) + 0x80;
    if (0xff < iVar1) {
        iVar1 = 0xff;
    }
    bVar2 = (byte)iVar1;
    param_1->m28_colorAndCode.m0_r = bVar2;
    param_1->m28_colorAndCode.m1_g = bVar2;
    param_1->m28_colorAndCode.m2_b = bVar2;
    updateAllSubsprites(param_1);
}

void updateSpinningEnemySelectionCursor(sTaskHeader* param_1) {
    sBattleSpinningEnemySelectionCursor* pThis = (sBattleSpinningEnemySelectionCursor*)param_1->m4;

    pThis->m3C_translation.vx = pThis->m38->m0_position.vx.getIntegerPart();
    pThis->m3C_translation.vy = pThis->m38->m0_position.vy.getIntegerPart();
    pThis->m3C_translation.vz = pThis->m38->m0_position.vz.getIntegerPart();

    pThis->m58_tick++;

    pThis->m3C_translation.vy -= pThis->m54 + 0x20;

    updateColorFromSin(pThis->m38, pThis->m58_tick);

    (pThis->m4C_rotation).vy += 0x10;
}

void drawSpinningEnemySelectionCursor(sTaskHeader* param_1) {
    sBattleSpinningEnemySelectionCursor* pThis = (sBattleSpinningEnemySelectionCursor*)param_1->m4;

    MATRIX MStack_40;
    TransMatrix(&MStack_40, &pThis->m3C_translation);
    createRotationMatrix(&pThis->m4C_rotation, &MStack_40);
    CompMatrix(&currentRenderingMatrix, &MStack_40, &MStack_40);

    long lVar1;
    int iVar2;
    lVar1 = ReadGeomScreen();
    iVar2 = lVar1 << 0xc;
    if (MStack_40.t[2] != 0) {
        iVar2 = iVar2 / MStack_40.t[2];
    }
    iVar2 = (0x1000000 / iVar2) / 2;
    VECTOR VStack_20;
    setupVector(&VStack_20, iVar2, iVar2, iVar2);
    ScaleMatrixL(&MStack_40, &VStack_20);
    SetRotMatrix(&MStack_40);
    SetTransMatrix(&MStack_40);
    renderCustomPolyBuffer(pThis->m64_customPolySubBuffer, pThis->m5C_buffer[battleOddOrEven], characterRenderingOT, 0, 0, 0);
}

void deleteSpinningEnemySelectionCursor(sTaskHeader* param_1) {
    sBattleSpinningEnemySelectionCursor* pThis = (sBattleSpinningEnemySelectionCursor*)param_1->m4;

    pThis->m38->m28_colorAndCode.m3_code |= 1;
    updateAllSubsprites(pThis->m38);
    clearShapeTransferEntry(&pThis->m5C_buffer);
    registerSpriteCallback2_2(param_1);
    allocateSavePointMeshDataSub0_callback(param_1);
    delete pThis;
}

void createSpinningEnemySelectionCursor(sBattleSpriteActor* param_1) {
    sBattleSpinningEnemySelectionCursor* pNewTask = createCustomRenderableEntity<sBattleSpinningEnemySelectionCursor>(0x68, &param_1->m0, updateSpinningEnemySelectionCursor, drawSpinningEnemySelectionCursor, deleteSpinningEnemySelectionCursor);
    pNewTask->m38 = (sSpriteActorCore*)param_1->m0.m4;
    pNewTask->m54 = ((sSpriteActorCore*)param_1->m0.m4)->m36;
    if (spriteBytecode2ExtendedE0_Var0) {
        allocateSavePointMeshDataSub0_var0--;
    }
    pNewTask->m0.m14 &= 0x7fffffff;
    setupSVector(&pNewTask->m4C_rotation, 0, 0, 0);
    int singleBufferSize = getCustomPolySubBufferSize(getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0));
    std::vector<sAnyPoly> dest;
    copyCustomPolySubBuffer(getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0), dest, 0, 1);

    pNewTask->m5C_buffer[0] = dest;
    pNewTask->m5C_buffer[1] = dest;

    pNewTask->m64_customPolySubBuffer = getCustomPolySubBuffer(&battleBlinkingPolyBuffer, 0);

    sSpriteActorCore* pSpriteActor = ((sSpriteActorCore*)param_1->m0.m4);
    pSpriteActor->m28_colorAndCode.m3_code &= 0xFE;
    updateAllSubsprites(pSpriteActor);
    updateSpinningEnemySelectionCursor(&pNewTask->m0);
}