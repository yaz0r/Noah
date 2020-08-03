#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "walkMesh.h"
#include "fieldModel.h"
#include "kernel/isoManager.h"
#include "fieldDebugger/fieldInspector.h"

#include <array>

struct sFieldVramMapping
{
    s16 m0_vramX;
    s16 m2_vramY;
    s16 m4;
    s16 m6;
};
std::array<sFieldVramMapping, 32> fieldVramMapping;

std::vector<u8> rawFieldBundle;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldTriggerData;
std::vector<u8> rawFieldDialogBundle;
std::vector<u8> rawFieldWalkMesh;
std::vector<u8> rawFieldActorSetupParams;

// this is malloc/free in original code, I keep it around for debugger
std::vector<u8> rawFieldImageBundle;
std::vector<u8> rawFieldImageBundle2;


std::array<s16, 4> fieldInitVar1;

s32 numFieldEntities;

s32 requestFieldId0 = -1;
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;

s32 numActiveFieldScriptEntity;
std::vector<u8>::iterator pCurrentFieldScriptFile;

std::array<s32, 3> currentParty;

std::vector<sFieldEntity> fieldEntityArray;

std::vector<sFieldActorSetupParams> fieldActorSetupParams;

void fieldModelRelocation(std::vector<u8>::iterator& pModelData)
{
    MissingCode();
}

s32 fieldChangePrevented = -1;
s32 fieldChangePrevented2 = -1;
s32 fieldTransitionMode = 0;

void resetFieldDefault()
{
    MissingCode();

    fieldTransitionMode = 2;

    MissingCode();

    fieldChangePrevented = -1;

    MissingCode();
}

void uploadFieldImages(std::vector<u8>::iterator& pImageData)
{
    MissingCode();
}

struct RECT
{
    s16 x;
    s16 y;
    s16 w;
    s16 h;
};

RECT* currentNpcSpriteUploadRect = nullptr;
std::vector<u8>::iterator currentNpcSpriteUploadDataPtr;

void transfertNpcSpriteSheetElement()
{
    MissingCode();
}

void uploadNpcSpriteSheet(std::vector<u8>::iterator& pImageData, int x, int y)
{
    int count = READ_LE_U32(pImageData);

    int xOffset = 0;

    for (int i=0; i<count; i++)
    {
        int offset = READ_LE_U32(pImageData + 4 + i * 4);
        std::vector<u8>::iterator data = pImageData + offset;

        RECT rect;
        rect.x = x + xOffset;
        rect.y = y;
        rect.w = READ_LE_U16(data);
        rect.h = READ_LE_U16(data + 2);

        xOffset += 0x40;

        currentNpcSpriteUploadDataPtr = data + 4;
        currentNpcSpriteUploadRect = &rect;

        transfertNpcSpriteSheetElement();
    }

    MissingCode();
}

void setupField3d(std::vector<u8>::iterator input)
{
    MissingCode();
}

void initFieldScriptEntityValues(int index)
{
    MissingCode();
}

void initFieldScriptEntity2dSprite(sFieldEntity2dSprite* pSprite)
{
    MissingCode();
}

int numInitializedFieldScriptEntities = 0;

void initFieldScriptEntity(int index)
{
    if (index < numActiveFieldScriptEntity)
    {
        numInitializedFieldScriptEntities++;
        sFieldScriptEntity* pNewFieldScriptEntity = new sFieldScriptEntity;
        fieldEntityArray[index].m4C_scriptEntity = pNewFieldScriptEntity;
        memset(pNewFieldScriptEntity, 0, sizeof(sFieldScriptEntity));

        pNewFieldScriptEntity->m5A = 0;

        if (fieldEntityArray[index].m58_flags & 0x2000)
        {
            MissingCode();
        }

        initFieldScriptEntityValues(index);
        fieldEntityArray[index].m8_2dSprite = new sFieldEntity2dSprite;
        initFieldScriptEntity2dSprite(fieldEntityArray[index].m8_2dSprite);
    }
}

void traceModelFunctionState(int state)
{
    MissingCode();
}

void initModel1(sModelBlock& pModelBlock, std::vector<s16>& outputBuffer, std::vector<s16>::iterator& outputBufferEnd)
{
    traceModelFunctionState(0x25);

    outputBuffer.resize(pModelBlock.m34_count);
    outputBufferEnd = outputBuffer.end();
}

int fieldScriptEntityAlreadyInitialized = 0;

std::array<s16, 1024> fieldVars;

void setVar(int varIndex, s16 value)
{
    fieldVars[varIndex / 2] = value;
}

void setVarsForCurrentParty()
{
    setVar(0x3E, currentParty[0]);
    setVar(0x40, currentParty[1]);
    setVar(0x42, currentParty[2]);
}

sFieldEntity* pCurrentFieldEntity;
sFieldScriptEntity* pCurrentFieldScriptActor;
int currentFieldActorId;
int fieldScriptInitVar0;
int currentScriptFinished;

u16 getScriptEntryPoint(int entityId, int scriptIndex)
{
    return READ_LE_U16(rawFieldScriptData.begin() + (entityId * 0x20 + scriptIndex) * 2 + 0x84);
}

int breakCurrentScript;
int fieldExectuteMaxCycles;

int fieldDebugDisable = 0;

void sprintf_screen(const char* format, ...)
{
    MissingCode();
}

typedef void (*tOpcode)();
std::array<tOpcode, 256> fieldScriptOpcodes;
std::array<tOpcode, 256> fieldScriptOpcodes_EX;

void initModel3(int, int)
{
    MissingCode();
}

void deleteFieldEntitySub4(sFieldEntitySub4* param_1)
{
    MissingCode();
}

void initsFieldEntitySub4_B4(sFieldEntitySub4_B4* pThis)
{
    pThis->m0_rotation[0] = 0;
    pThis->m0_rotation[1] = 0;
    pThis->m0_rotation[2] = 0;
    pThis->m2C = nullptr;
}

void initFieldEntitySub4Sub2(sFieldEntitySub4* pThis)
{
    pThis->m20 = &pThis->mB4;
    initsFieldEntitySub4_B4(&pThis->mB4);
    pThis->m7C = &pThis->mF4;
    pThis->m20->m34 = &pThis->m124;
    pThis->m24 = &pThis->m110;
    pThis->m20->m38 = 0;
}

void initFieldEntitySub4Sub3(sFieldEntitySub4* param_1, int param_2)
{
    sFieldEntitySub4_B4* psVar1;

    psVar1 = param_1->m20;
    if (psVar1 != nullptr) {
        param_1->m2C = param_2;
        psVar1->m6_scale[2] = param_2;
        psVar1->m6_scale[1] = param_2;
        psVar1->m6_scale[0] = param_2;
        param_1->m3C = param_1->m3C | 0x10000000;
    }
}

s8 initFieldVar1 = 0;
s32 initFieldVar2 = 0;

int initFieldEntitySub4Sub4(void* param_1)
{
    return (READ_LE_U16(param_1) >> 9) & 0x3F;
}

int initFieldVar4 = 0;
int initFieldVar5 = 0;

void initFieldEntitySub4Sub5Sub0(sFieldEntitySub4_110* param_1, sFieldActorSetupParams* param_2, sVec2_s16 param_3_vramLocation, sFP1616 param_4)

{
    u16 uVar1;
    char cVar2;
    u16* puVar3;

    param_1->m4_vramLocation = param_3_vramLocation;
    param_1->m8 = param_4;
    param_1->mC = param_2->mC_pData;
    cVar2 = initFieldVar1;
    param_1->m0 = param_2->m8_pData;
    initFieldVar4 = 0;
    param_1->m10 = param_2->m4_pData;

    if ((cVar2 != '\0') && (uVar1 = READ_LE_U16(param_2->m4_pData) >> 6, (uVar1 & 0x3f) != 0)) {
        initFieldVar5 = (u8)uVar1 & 0x3f;
    }
    return;
}

int initFieldEntitySub4Sub5Sub1(void* param_1)
{
    return (((u8*)param_1)[1] >> 7);
}


void initFieldEntitySub4Sub5(sFieldEntitySub4* param_1, sFieldActorSetupParams* param_2)
{
    int iVar1;
    sFieldEntitySub4_110* psVar2;

    psVar2 = param_1->m24;
    if (param_2 != 0) {
        if (param_2 != param_1->m44) {
            initFieldEntitySub4Sub5Sub0(psVar2, param_2, psVar2->m4_vramLocation, psVar2->m8);
            param_1->m44 = param_2;
            param_1->m3C = param_1->m3C | 0x40000000;
        }
        if (initFieldVar1 != '\0') {
            iVar1 = initFieldEntitySub4Sub5Sub1(psVar2->m0);
            if (iVar1 == 0) {
                psVar2->m4_vramLocation[0] = 0x100;
                psVar2->m4_vramLocation[1] = 0x300;
            }
            else {
                psVar2->m4_vramLocation[0] = param_1->m7C->mE_vramLocation[0];
                psVar2->m4_vramLocation[1] = param_1->m7C->mE_vramLocation[1];
            }
        }
    }
}

int OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1_var = 0x2000;

void createRotationMatrix(SVECTOR*, MATRIX*)
{
    MissingCode();
}

void ScaleMatrixL(MATRIX*, VECTOR*)
{
    MissingCode();
}

void ScaleMatrix(MATRIX*, VECTOR*)
{
    MissingCode();
}

void MulMatrix0(MATRIX* m0, MATRIX* m1, MATRIX* m2)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub0(sFieldEntitySub4* param_1)

{
    VECTOR local_70;
    VECTOR local_60;
    MATRIX tempMatrix2;
    MATRIX tempMatrix;

    if ((param_1->m40 & 1) == 0) {
        local_70.vx = param_1->m20->m6_scale[0];
        local_70.vy = param_1->m20->m6_scale[1];
        local_70.vz = param_1->m20->m6_scale[2];
        createRotationMatrix(&param_1->m20->m0_rotation, &param_1->m20->m20);
        ScaleMatrixL(&param_1->m20->m20, &local_70);
    }
    else {
        tempMatrix.m[0][0] = 0x1000;
        tempMatrix.m[0][2] = 0;
        tempMatrix.m[1][1] = 0x1000;
        tempMatrix.m[2][0] = 0;
        tempMatrix.m[2][2] = 0x1000;
        tempMatrix.t[0] = 0;
        tempMatrix.t[1] = 0;
        tempMatrix.t[2] = 0;
        local_60.vx = (int)param_1->m20->m6_scale[0];
        local_60.vy = (int)param_1->m20->m6_scale[1];
        local_60.vz = (int)param_1->m20->m6_scale[2];
        ScaleMatrixL((sMatrix*)&tempMatrix, &local_60);
        createRotationMatrix((SVECTOR*)param_1->m20, &tempMatrix2);
        MulMatrix0(&tempMatrix2, &tempMatrix, &param_1->m20->m20);
    }
    local_70.vx = (uint)((ushort)param_1->m3A >> 1);
    if (param_1->m3A != 0) {
        local_70.vy = (uint)((ushort)param_1->m3A >> 1);
        local_70.vz = (uint)((ushort)param_1->m3A >> 1);
        ScaleMatrix((MATRIX*)&param_1->m20->m20, &local_70);
    }
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(sFieldEntitySub4* param_1)
{
    int iVar1;
    int iVar2;

    iVar1 = 0;
    do {
        param_1->m20->m34[iVar1].m0 = 0;
        param_1->m20->m34[iVar1].m1 = 0;
        param_1->m20->m34[iVar1].m2 = 0;
        param_1->m20->m34[iVar1].m4 = 0;
        iVar2 = iVar1 + 1;
        param_1->m20->m34[iVar1].m6 = 0;
        iVar1 = iVar2;
    } while (iVar2 != 8);
    return;
}

int initFieldVar3 = 0;

//header is
//u16 m0: flags
//u16 m2: offset to byte code
//u16 m4: offset to?

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1(sFieldEntitySub4* param_1, ushort* param_2)
{
    uint uVar1;
    sFieldEntitySub4_B4* psVar2;
    sFieldEntitySub4_F4* psVar3;
    int iVar5;

    u16 flags = READ_LE_U16(param_2);
    u16 offsetToByteCode = READ_LE_U16(((u8*)param_2) + 2);
    u16 offset2 = READ_LE_U16(((u8*)param_2) + 4);

    param_1->m58 = param_2;
    param_1->m64_spriteByteCode = offsetToByteCode + 2 + (u8*)param_2;
    param_1->mA8 = param_1->mA8 & 0xffcfffff | (flags & 3) << 0x14;
    param_1->m54 = offset2 + 4 + (u8*)param_2;

    uVar1 = (uint)(flags >> 2) & 0x3f;
    if ((flags >> 2 & 0x20) != 0) {
        uVar1 = uVar1 | 0xffffffc0;
    }

    param_1->m1C = uVar1 * 0x400;
    int iVar4 = (initFieldVar3 + 1) * (initFieldVar3 + 1) * param_1->m82;
    if (iVar4 < 0) {
        iVar4 = iVar4 + 0xfff;
    }
    iVar4 = uVar1 * 0x400 * (iVar4 >> 0xc);
    param_1->m1C = iVar4;

    iVar5 = 0x10000 / (int)(param_1->mAC >> 7 & 0xfff);
    iVar5 = iVar5 * iVar5;
    if (iVar5 < 0) {
        iVar5 = iVar5 + 0xff;
    }
    iVar4 = iVar4 * (iVar5 >> 8);
    param_1->m1C = iVar4;
    if (iVar4 < 0) {
        iVar4 = iVar4 + 0xff;
    }
    param_1->m1C = iVar4 >> 8;


    if ((flags >> 0xb & 1) == 0) {
        param_1->m14 = 0;
        param_1->m10 = 0;
        param_1->mC = 0;
        param_1->m18 = 0;
    }
    psVar2 = param_1->m20;
    if (psVar2)
    {
        if ((flags >> 0xc & 1) == 0) {
            psVar2->m0_rotation[2] = 0;
            psVar2->m0_rotation[0] = 0;
            psVar2->m0_rotation[1] = 0;
            OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub0(param_1);
        }
        if ((flags >> 0xd & 1) == 0) {
            if (initFieldVar1 != '\0') {
                initFieldEntitySub4Sub3(param_1, (short)OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1_var);
                goto LAB_800236f4;
            }
        }
        else {
        LAB_800236f4:
            if (initFieldVar1 != '\0') {
                OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub0(param_1);
            }
        }
        if ((param_1->m3C & 3) == 1) {
            psVar2 = param_1->m20;
            psVar2->m3D = 0;
            psVar2->m3C = 0;
            if (((param_1->m40 >> 0x14 & 1) == 0) && (param_1->m20->m34 != (sFieldEntitySub4_124*)0x0)) {
                OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
            }
        }
    }
    param_1->m8C = 0x10;
    uVar1 = param_1->mA8;
    param_1->m30 = 0;
    param_1->mA8 = uVar1 & 0xfffff801;
    param_1->m9E = 1;
    psVar3 = param_1->m7C;
    param_1->mA8 = uVar1 & 0xc03ff801 | 0x2001f800;
    if ((psVar3 != (sFieldEntitySub4_F4*)0x0) && ((uVar1 & 1) == 1)) {
        psVar3->m4 = 0;
        psVar3->m0 = 0;
        param_1->m7C->mC = 0;
    }
    return;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub2(sFieldEntitySub4* param_1, short param_2)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6(sFieldEntitySub4* param_1, int param_2)
{
    ushort uVar1;
    uint uVar2;
    int iVar3;
    ushort* puVar4;
    byte* pbVar5;

    if (param_1->m48 == nullptr) {
        param_1->m64_spriteByteCode = 0;
    }
    else {
        if (param_1->m44 == param_1->m48) {
            uVar2 = param_1->mB0 & 0xfffffbff;
        }
        else {
            uVar2 = param_1->mB0 | 0x400;
        }
        param_1->mB0 = uVar2;
        if (param_2 < 0) {
            initFieldEntitySub4Sub5(param_1, param_1->m4C);
            if ((initFieldVar1 != '\0') &&
                (iVar3 = initFieldEntitySub4Sub5Sub1(param_1->m24->m0), iVar3 == 0)) {
                param_1->m24->m4_vramLocation[0] = 0x100;
                param_1->m24->m4_vramLocation[1] = 0x300;
            }
        }
        else {
            initFieldEntitySub4Sub5(param_1, param_1->m48);
            if (initFieldVar1 != '\0') {
                param_1->m24->m4_vramLocation = param_1->m7C->mE_vramLocation;
            }
        }
        param_1->mAC = (param_1->mAC & 0xFF00FFFF) | (((char)param_2) << 16);
        if ((int)param_2 < 0) {
            param_2 = ~param_2;
        }
        pbVar5 = param_1->m24->m10;
        uVar1 = *(ushort*)(pbVar5 + param_2 * 2 + 2);
        param_1->m40 = param_1->m40 | 0x100000;
        puVar4 = (ushort*)(pbVar5 + uVar1);
        param_1->m58 = puVar4;
        OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1(param_1, puVar4);
        OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub2(param_1, param_1->m80);
    }
}

void initFieldEntitySub4Sub1(sFieldEntitySub4* param_1)
{
    param_1->m3C = 0;
    param_1->m2B = 0x2d;
    param_1->m40 = 0;
    param_1->m3A = 0;
    param_1->m30 = 0;
    param_1->m32 = 0;
    param_1->m34 = 0;
    param_1->mA8 = 0;
    param_1->m3C = param_1->m3C & 0xfe00ffe3;
    param_1->m40 = param_1->m40 & 0xfffe0003;
    param_1->mAC = 0;
    param_1->mB0 = 0;
    int iVar1 = (initFieldVar3 + 1) * (initFieldVar3 + 1) * 0x4000 * param_1->m82;
    param_1->mAC = param_1->mAC & 0xfff8007f | 0x8000;
    param_1->mA8 = param_1->mA8 & 0xfff1ffff;
    if (iVar1 < 0) {
        iVar1 = iVar1 + 0xfff;
    }
    param_1->m1C = iVar1 >> 0xc;
    param_1->m64_spriteByteCode = 0;
    param_1->m70 = 0;
    param_1->m44 = 0;
    param_1->m68 = 0;
    param_1->m80 = 0;
    param_1->m8C = 0x10;
    param_1->m84 = 0;
    param_1->m6C = (sFieldEntitySub4*)0x0;
    param_1->m50 = 0;
}

sFieldEntitySub4* initFieldEntitySub4(sFieldEntitySub4* param_1, sFieldActorSetupParams* pSetup, int param_3, int param_4, int vramX, int vramY, int param_7)
{
    initFieldEntitySub4Sub1(param_1);
    initFieldEntitySub4Sub2(param_1);
    initFieldEntitySub4Sub3(param_1, 0x10000);

    param_1->m3C = (param_1->m3C & ~0x3) | 1;
    param_1->m40 &= ~0x0001E000;

    if (initFieldVar1 == 0)
    {
        param_1->mA8 |= 1;
        param_1->m7C->m18 = 0;
    }
    else
    {
        param_1->mA8 &= ~1;
        param_1->m7C->m8 = 0;
        param_1->m7C->mC = 0;
    }

    param_1->m6C = param_1;
    param_1->m3C = param_1->m3C & 0xff00ffff | (initFieldVar2 & 0xf) << 0x14 | (initFieldVar2 & 0xf) << 0x10;;

    int count = initFieldEntitySub4Sub4(pSetup->m8_pData);
    param_1->m20->m2C = param_1->m20->m30 = new u8[count * 0x18]; // TODO: figure that

    param_1->m24->m4_vramLocation[0] = vramX;
    param_1->m24->m4_vramLocation[1] = vramY;
    param_1->m24->m8 = param_3 << 16;
    param_1->m24->m8 |= param_4;

    param_1->m48 = pSetup;

    initFieldEntitySub4Sub5(param_1, pSetup);
    param_1->m60 = (u8*)param_1->m24->m10 + (READ_LE_U16(param_1->m24->m10) & 0x3F) + 1;

    OP_INIT_ENTITY_SCRIPT_sub0Sub6(param_1, 0);

    return param_1;
}

sFieldEntitySub4* createFieldEntitySub4(sFieldActorSetupParams* pSetup, int param_2, int param_3, int vramX, int vramY, int param_6)
{
    sFieldEntitySub4* pNewEntry = new sFieldEntitySub4;
    pNewEntry->m86_thisSize = sizeof(sFieldEntitySub4);

    return initFieldEntitySub4(pNewEntry, pSetup, param_2, param_3, vramX, vramY, param_6);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sFieldEntitySub4* param_1, int param_2, int* param_3, int* param_4, int* param_5)
{
    byte bVar1;
    byte* pbVar2;
    int iVar3;
    int iVar4;
    uint uVar5;

    pbVar2 = param_1->m24->m10;
    pbVar2 = pbVar2 + *(ushort*)(pbVar2 + 2);
    bVar1 = pbVar2[(uint) * (ushort*)(pbVar2 + 4) + 4];
    uVar5 = (uint)bVar1;
    pbVar2 = param_1->m24->m0;
    iVar3 = uVar5 << 1;
    if (bVar1 != 0) {
        uVar5 = uVar5 - 1;
        iVar3 = uVar5 * 2;
    }
    iVar4 = uVar5 << 1;
    if ((int)(uint) * (ushort*)(pbVar2 + iVar3) < (int)uVar5) {
        iVar4 = 0;
    }
    pbVar2 = pbVar2 + *(ushort*)(pbVar2 + iVar4 + 2);
    iVar3 = (uint)pbVar2[3] * (int)(short)param_1->m2C;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 0xfff;
    }
    *param_4 = iVar3 >> 0xc;
    iVar3 = (uint)pbVar2[1] * (int)(short)param_1->m2C;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 0xfff;
    }
    *param_5 = iVar3 >> 0xc;
    iVar3 = (uint)pbVar2[2] * (int)(short)param_1->m2C;
    if (iVar3 < 0) {
        iVar3 = iVar3 + 0xfff;
    }
    *param_3 = iVar3 >> 0xc;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sFieldEntitySub4* param1, int param2)
{
    param1->m40 = param1->m40 & 0xffffe0ff | (param2 & 0x1f) << 8;
}

int getAngleCosFraq(int angle)
{
    MissingCode();
    return 0;
}

int getAngleCosInteger(int angle)
{
    MissingCode();
    return 1;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7Sub0(sFieldEntitySub4* param1)
{
    int iVar1;
    int iVar2;

    iVar2 = ((param1->m18 >> 4) << 8) / (int)(param1->mAC >> 7 & 0xfff);
    iVar1 = getAngleCosFraq((int)param1->m32);
    param1->mC = (iVar1 >> 2) * iVar2 >> 6;
    iVar1 = getAngleCosInteger((int)param1->m32);
    param1->m14 = -((iVar1 >> 2) * iVar2) >> 6;
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sFieldEntitySub4* param1, int param2)
{
    param1->m32 = param2;
    OP_INIT_ENTITY_SCRIPT_sub0Sub7Sub0(param1);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sFieldEntitySub4* param1, void(*callback)(sFieldEntitySub4*))
{
    param1->m68 = callback;
}

void fieldActorCallback(sFieldEntitySub4* pThis)
{
    fieldEntityArray[pThis->m7C->m14_actorId].m4C_scriptEntity->m4_flags |= 0x10000;
}

s16 isFogSetup = 0;

void OP_INIT_ENTITY_SCRIPT_sub0Sub3(sFieldEntitySub4* param_1, int param_2)

{
    sFieldEntitySub4_B4* psVar2;

    delete[] param_1->m20->m2C;
    u8* puVar1 = (u8*)malloc(param_2 * 0x18);
    psVar2 = param_1->m20;
    psVar2->m30 = puVar1;
    psVar2->m2C = puVar1;
    return;
}


void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int param_2, sFieldActorSetupParams* pSetup, int param_4, int param_5, int param_6, int param_7)
{
    initModel3(8, 0);
    fieldEntityArray[actorId].m4C_scriptEntity->m127 = param_2;
    fieldEntityArray[actorId].m4C_scriptEntity->m126 = param_6;
    fieldEntityArray[actorId].m4C_scriptEntity->m134.m0 = param_5;
    fieldEntityArray[actorId].m4C_scriptEntity->m130.m28 = param_4;
    fieldEntityArray[actorId].m4C_scriptEntity->m134.m4 = param_7;

    if (param_4 == 0)
    {
        s16 vramX = fieldVramMapping[param_2].m0_vramX;
        s16 vramY = fieldVramMapping[param_2].m2_vramY;

        if (param_5 == 0)
        {
            if (fieldEntityArray[actorId].m5A & 1)
            {
                deleteFieldEntitySub4(fieldEntityArray[actorId].m4_pSub4);
            }

            fieldEntityArray[actorId].m4_pSub4 = createFieldEntitySub4(pSetup, 0x100, ((param_2 + 480) * 0x10000) >> 0x10, vramX, vramY, 0x40);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        int iVar4;
        short sVar5;

        if ((fieldEntityArray[actorId].m5A & 1U) != 0) {
            deleteFieldEntitySub4(fieldEntityArray[actorId].m4_pSub4);
        }
        if (param_4 == 1) {
            iVar4 = param_2 + 0xe0;
            sVar5 = 0x280;
        }
        else {
            iVar4 = param_2 + 0xe3;
            sVar5 = 0x2a0;
        }
        fieldEntityArray[actorId].m4_pSub4 = createFieldEntitySub4(pSetup, 0x100, (short)((uint)(iVar4 * 0x10000) >> 0x10), sVar5, (short)param_2 * 0x40 + 0x100, 8);
        OP_INIT_ENTITY_SCRIPT_sub0Sub3(fieldEntityArray[actorId].m4_pSub4, 0x20);
    }

    fieldEntityArray[actorId].m5A |= 1;

    sFieldEntitySub4* pFieldEntitySub4 = fieldEntityArray[actorId].m4_pSub4;

    int temp0;
    int temp1;
    int temp2;
    OP_INIT_ENTITY_SCRIPT_sub0Sub4(pFieldEntitySub4, 0, &temp0, &temp1, &temp2);
    OP_INIT_ENTITY_SCRIPT_sub0Sub5(pFieldEntitySub4, 3);

    pFieldEntitySub4->m2C = 0xC00;
    pFieldEntitySub4->m82 = 0x2000;

    if (!fieldScriptEntityAlreadyInitialized)
    {
        pFieldEntitySub4->m0_position = fieldEntityArray[actorId].m4C_scriptEntity->m20_position;
        pFieldEntitySub4->m10 = 0;
        pFieldEntitySub4->mC = 0;
        pFieldEntitySub4->m10 = 0;
        pFieldEntitySub4->m14 = 0;
        pFieldEntitySub4->m1C = 0x10000;
        pFieldEntitySub4->m84 = fieldEntityArray[actorId].mC_matrix.t[1] & 0xFFFF;
        if (!param_4)
        {
            fieldEntityArray[actorId].m4C_scriptEntity->m18[1] = temp1 * 2;
        }
        else
        {
            fieldEntityArray[actorId].m4C_scriptEntity->m18[1] = 0x40;
        }
    }
    else
    {
        assert(0);
    }

    if (isFogSetup)
    {
        pFieldEntitySub4->m40 |= 0x40000;
    }

    OP_INIT_ENTITY_SCRIPT_sub0Sub6(pFieldEntitySub4, 0);
    OP_INIT_ENTITY_SCRIPT_sub0Sub7(pFieldEntitySub4, 0);
    initModel3(8, 0);

    pFieldEntitySub4->m7C->m14_actorId = actorId;
    OP_INIT_ENTITY_SCRIPT_sub0Sub8(pFieldEntitySub4, &fieldActorCallback);

    if (!param_7)
    {
        MissingCode();
    }

    fieldEntityArray[actorId].mC_matrix.t[0] = fieldEntityArray[actorId].m4C_scriptEntity->m20_position.vx >> 16; // TODO: probably wrong, and if not, that also overwrite the integer part
    fieldEntityArray[actorId].mC_matrix.t[1] = fieldEntityArray[actorId].m4C_scriptEntity->m20_position.vy >> 16;
    fieldEntityArray[actorId].mC_matrix.t[2] = fieldEntityArray[actorId].m4C_scriptEntity->m20_position.vz >> 16;

    fieldEntityArray[actorId].m2C_matrixBackup.t = fieldEntityArray[actorId].mC_matrix.t;

    pFieldEntitySub4->m84 = fieldEntityArray[actorId].mC_matrix.t[1] & 0xFFFF;

    pFieldEntitySub4->m0_position = fieldEntityArray[actorId].m4C_scriptEntity->m20_position;
}

void OP_INIT_ENTITY_SCRIPT_sub1()
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT()
{
    OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 1);
    OP_INIT_ENTITY_SCRIPT_sub1();

    pCurrentFieldScriptActor->m0_flags |= 0x100;
    pCurrentFieldScriptActor->m4_flags |= 0x800;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

u16 readU16FromScript(int param_1)
{
    return *(ushort*)
        (&pCurrentFieldScriptFile[0] + (uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1);
}

uint getVariable(int param_1)
{
    uint uVar1;

    uVar1 = param_1 >> 1;
    if ((*(uint*)(&rawFieldScriptData[0] + (param_1 >> 6) * 4) & 1 << (uVar1 & 0x1f)) == 0) {
        uVar1 = fieldVars[uVar1];
    }
    else {
        uVar1 = (uint)(ushort)fieldVars[uVar1];
    }
    return uVar1;
}

int readS16FromScript(int param_1)

{
    return (int)(((uint)pCurrentFieldScriptFile
        [(uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1] +
        (uint)(pCurrentFieldScriptFile +
            (uint)pCurrentFieldScriptActor->mCC_scriptPC + param_1)[1] * 0x100) * 0x10000)
        >> 0x10;
}


int getBytesSign(int param_1)

{
    return -(uint)((*(uint*)(&rawFieldScriptData[0] + (param_1 >> 6) * 4) & 1 << (param_1 >> 1 & 0x1fU))
        != 0);
}


void OP_JUMP_IF()
{
    bool bVar1;
    ushort uVar2;
    int iVar3;
    byte bVar4;
    uint uVar5;
    uint uVar6;

    uVar5 = 0;
    bVar4 = pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf0;
    uVar6 = 0;
    if (bVar4 == 0x40) {
        uVar2 = readU16FromScript(1);
        uVar6 = getVariable((uint)uVar2);
        iVar3 = readS16FromScript(3);
        uVar5 = iVar3;
        uVar2 = readU16FromScript(1);
        iVar3 = getBytesSign((uint)uVar2);
        if (iVar3 != 0) {
        LAB_Field__800a1cd0:
            uVar5 = uVar5 & 0xffff;
        }
    }
    else {
        if (bVar4 < 0x41) {
            if ((pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf0) == 0) {
                uVar2 = readU16FromScript(1);
                uVar6 = getVariable((uint)uVar2);
                uVar2 = readU16FromScript(3);
                uVar5 = getVariable((uint)uVar2);
                uVar2 = readU16FromScript(1);
                iVar3 = getBytesSign((uint)uVar2);
                if (iVar3 != 0) goto LAB_Field__800a1cd0;
                uVar5 = uVar5;
            }
        }
        else {
            if (bVar4 == 0x80) {
                iVar3 = readS16FromScript(1);
                uVar6 = iVar3;
                uVar2 = readU16FromScript(3);
                uVar5 = getVariable((uint)uVar2);
                uVar2 = readU16FromScript(3);
                iVar3 = getBytesSign((uint)uVar2);
                if (iVar3 != 0) {
                    uVar6 = uVar6 & 0xffff;
                }
            }
            else {
                if (bVar4 == 0xc0) {
                    iVar3 = readS16FromScript(1);
                    uVar6 = iVar3;
                    iVar3 = readS16FromScript(3);
                    uVar5 = iVar3;
                }
            }
        }
    }
    bVar1 = false;
    switch ((uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5] & 0xf) {
    case 0:
        if (uVar6 == uVar5) {
            bVar1 = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 1:
        if (uVar6 != uVar5) {
            bVar1 = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 2:
        uVar5 = (uint)((int)uVar5 < (int)uVar6);
        break;
    case 3:
        uVar5 = (uint)((int)uVar6 < (int)uVar5);
        break;
    case 4:
        if ((int)uVar5 <= (int)uVar6) {
            bVar1 = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 5:
        if ((int)uVar6 <= (int)uVar5) {
            bVar1 = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 6:
        uVar5 = uVar6 & uVar5;
        break;
    case 7:
        if (uVar6 != uVar5) {
            bVar1 = true;
        }
        goto switchD_Field_800a1d84_caseD_b;
    case 8:
        uVar5 = uVar6 | uVar5;
        break;
    case 9:
        uVar5 = uVar6 & uVar5;
        break;
    case 10:
        uVar5 = ~uVar6 & uVar5;
        break;
    default:
        goto switchD_Field_800a1d84_caseD_b;
    }
    if (uVar5 != 0) {
        bVar1 = true;
    }
    switchD_Field_800a1d84_caseD_b:
    if (bVar1) {
        uVar2 = pCurrentFieldScriptActor->mCC_scriptPC + 8;
    }
    else {
        uVar2 = readU16FromScript(6);
    }
    pCurrentFieldScriptActor->mCC_scriptPC = uVar2;
    return;
}

int asyncLoadingVar1 = 0xFF;
int asyncLoadingVar2 = 0;

int isAsyncLoadingProhibited()
{
    MissingCode();
    return 0;
}

int findFreePartySlot(int param_1, int* param_2)

{
    int iVar1;

    iVar1 = 0;
    while (true) {
        if (currentParty[iVar1] == param_1) {
            return -1;
        }
        if (currentParty[iVar1])
            break;
        iVar1 = iVar1 + 1;

        if (2 < iVar1) {
            return -1;
        }
    }
    *param_2 = iVar1;
    return 0;
}

int asyncPartyCharacterLoadingCharacterIndex = 0;
int asyncPartyCharacterLoadingIndex = 0;
int fieldExecuteVar1 = 0;
std::array<int, 3> asyncPartyCharacterLoadingTable;
std::vector<u8> asyncPartyCharacterLoadingBuffer;

int getGearForCharacter(int param_1)
{
    return pKernelGameState->m294[param_1].m78_partyData_gearNum;
}

void startPartyCharacterASyncLoading(int partyCharacter, int partySlot)

{
    size_t sVar1;
    void* pvVar2;
    int iVar3;
    int iVar4;

    asyncPartyCharacterLoadingCharacterIndex = partyCharacter;
    asyncPartyCharacterLoadingIndex = partySlot;
    setCurrentDirectory(4, 0);
    if (fieldExecuteVar1 == 0) {
        waitReadCompletion(0);
    }
    iVar3 = partyCharacter + 5;
    if ((requestFieldId0 & 0xc000) == 0) {
        sVar1 = getFileSizeAligned(iVar3);
        asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = partyCharacter;

        asyncPartyCharacterLoadingBuffer.resize(sVar1);
    }
    else {
        iVar3 = getGearForCharacter(partyCharacter);
        iVar4 = iVar3 + 0x10;
        if (iVar3 == 0xff) {
            iVar4 = 0x10;
        }
        iVar3 = iVar4 + 5;
        sVar1 = getFileSizeAligned(iVar3);
        asyncPartyCharacterLoadingBuffer.resize(sVar1);
        asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = iVar4;
    }
    readFile(iVar3, asyncPartyCharacterLoadingBuffer, 0, 0x80);
    if (fieldExecuteVar1 == 0) {
        waitReadCompletion(0);
    }
    asyncLoadingVar1 = 1;
    return;
}

void OP_ADD_TO_CURRENT_PARTY()
{
    if (((asyncLoadingVar1 == 0xff) && (asyncLoadingVar2 == 0)) && (!isAsyncLoadingProhibited()))
    {
        waitReadCompletion(0);

        int freePartySlot;
        int partyIsFullOrCharacterAlreadyInParty = findFreePartySlot((uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1], &freePartySlot);

        sFieldScriptEntity* psVar2 = pCurrentFieldScriptActor;
        if (partyIsFullOrCharacterAlreadyInParty == 0) {
            pKernelGameState->m22B1_isOnGear[freePartySlot] = 0;

            s8 bVar1 = pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1];
            currentParty[freePartySlot] = (uint)bVar1;

            startPartyCharacterASyncLoading((uint)bVar1, freePartySlot);
            pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
        }
        else {
            pKernelGameState->m1D30_partyMemberBitField =
                pKernelGameState->m1D30_partyMemberBitField |
                (ushort)(1 << ((uint)pCurrentFieldScriptFile
                    [(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1] & 0x1f));
            psVar2->mCC_scriptPC = psVar2->mCC_scriptPC + 4;
        }
    }
    else
    {
        breakCurrentScript = 1;
        pCurrentFieldScriptActor->mCC_scriptPC--;
    }
}

void OP_2A(void)
{
    ushort* puVar1;
    sFieldScriptEntity* psVar2;

    psVar2 = pCurrentFieldScriptActor;
    puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
    pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x20000;
    psVar2->mCC_scriptPC = *puVar1 + 1;
    return;
}

void OP_STOP(void)
{
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags |= 0x3C0000;
    pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m3 = 0xff;
    currentScriptFinished = 1;
    breakCurrentScript = 1;
    return;
}

uint getImmediateOrVariableUnsigned(int param_1)
{
    ushort uVar1;
    uint uVar2;

    uVar1 = readU16FromScript(param_1);
    uVar2 = (uint)uVar1 & 0x7fff;
    if ((uVar1 & 0x8000) == 0) {
        uVar2 = getVariable((uint)uVar1);
    }
    return uVar2;
}


void OP_INIT_ENTITY_NPC(void)

{
    uint* puVar1;
    int iVar2;
    sFieldScriptEntity* psVar3;
    uint uVar4;

    fieldEntityArray[currentFieldActorId].m58_flags = fieldEntityArray[currentFieldActorId].m58_flags & 0xf07f | 0x200;
    uVar4 = getImmediateOrVariableUnsigned(1);
    OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, uVar4, &fieldActorSetupParams[uVar4], 0, 0, uVar4 | 0x80, 0);
    OP_INIT_ENTITY_SCRIPT_sub1();
    psVar3 = pCurrentFieldScriptActor;
    puVar1 = &pCurrentFieldScriptActor->m0_flags;
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
    psVar3->m0_flags = *puVar1 & 0xffffff7f | 0x100;
    iVar2 = currentFieldActorId;
    psVar3->m4_flags = psVar3->m4_flags & 0xfffff7ff;
    fieldEntityArray[iVar2].m58_flags = fieldEntityArray[iVar2].m58_flags & 0xffdf;
    return;
}

int getVar80(int param_1, uint param_2)

{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x80) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}

int getVar40(int param_1, uint param_2)

{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x40) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}


int getVar20(int param_1, uint param_2)
{
    ushort uVar1;
    int iVar2;

    if ((param_2 & 0x20) == 0) {
        uVar1 = readU16FromScript(param_1);
        iVar2 = getVariable((uint)uVar1);
    }
    else {
        iVar2 = readS16FromScript(param_1);
        iVar2 = (int)(short)iVar2;
    }
    return iVar2;
}


void setCurrentActor2DPosition(int posX, int posY)
{
    MissingCode();
}

void OP_SET_ACTOR_POSITION_2D(void)
{
    sFieldScriptEntity* psVar1;
    int posX;
    int posZ;

    posX = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    posZ = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    setCurrentActor2DPosition(posX, posZ);
    psVar1 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m4_flags = pCurrentFieldScriptActor->m4_flags & 0xffdfffff;
    psVar1->m0_flags = psVar1->m0_flags & 0xfffeffff;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 6;
    return;
}

std::array<u16, 8> actorDirectionTable3 = {
    0x8C00,  0x8400,  0x8800,  0x8000,
    0x8A00,  0x8E00,  0x8600,  0x8200
};

void OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION(void)
{
    int iVar1;
    sFieldScriptEntity* psVar2;
    ushort uVar3;

    psVar2 = pCurrentFieldScriptActor;
    iVar1 = fieldExecuteVar1;
    uVar3 = actorDirectionTable3[pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 1]] | 0x8000;
    pCurrentFieldScriptActor->m104_rotation = uVar3;
    psVar2->m106_currentRotation = uVar3;
    if (iVar1 == 0) {
        psVar2->m108_rotation3 = uVar3;
    }
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 2;
    return;
}

int getCharacter(int param_1)
{
    int iVar1;

    iVar1 = currentParty[2];
    if ((((param_1 != 0xff) && (iVar1 = currentParty[1], param_1 != 0xfe)) && (iVar1 = currentParty[0], param_1 != 0xfd)) && (iVar1 = 0xff, param_1 != 0xfc)) {
        iVar1 = param_1;
    }
    return iVar1;
}

int findCharacterInParty(int param_1)
{
    int iVar1;
    int* piVar2;

    iVar1 = 0;
    if (param_1 != 0xff) {
        do {
            if (currentParty[iVar1] == 0xff) {
                return 0xffffffff;
            }
            if (currentParty[iVar1] == param_1) {
                return iVar1;
            }
            iVar1 = iVar1 + 1;
        } while (iVar1 < 3);
    }
    return 0xffffffff;
}

std::array<int, 3> partyToFieldEntityArrayMapping;
std::array<std::vector<u8>, 3> partyCharacterBuffersRaw;
std::array<std::vector<u8>, 3> partyCharacterBuffersCompressed;
std::array<sFieldActorSetupParams, 3> partyCharacterBuffers;
s16 pcInitVar0 = 0;
s32 pcInitVar1 = 0;
std::array<int, 11> PCToActorArray;

std::array<s8, 12> characterMappingTable = {
    0,1,2,3,4,5,6,7,8,2,6,0
};

s16 pcInitVar2 = 0;
std::array<int, 3> unkPartyTable;

void OP_INIT_ENTITY_PC_Sub0(int)
{
    MissingCode();
}

void OP_INIT_ENTITY_PC(void)
{
    sGameState* psVar1;
    int iVar2;
    int local_v0_60;
    int iVar3;
    int iVar4;
    int local_v0_684;
    uint uVar5;
    sFieldEntitySub4* psVar6;
    sFieldScriptEntity* psVar7;
    sFieldEntity* psVar8;

    psVar8 = &fieldEntityArray[currentFieldActorId];
    local_v0_60 = getImmediateOrVariableUnsigned(1);
    iVar3 = getCharacter(local_v0_60);
    iVar4 = findCharacterInParty(iVar3);
    psVar7 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->mE4_playableCharacterId = (short)iVar3;
    psVar8->m58_flags = psVar8->m58_flags & 0xf07f | 0x200;
    if (iVar4 == -1) {
        OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, &partyCharacterBuffers[0], 1, 0, 0, 1);
        psVar7 = pCurrentFieldScriptActor;
        pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 1;
        currentScriptFinished = 1;
        breakCurrentScript = 1;
        uVar5 = psVar7->m4_flags | 0x100000;
    }
    else {
        if (iVar4 == 0) {
            PCToActorArray[0] = currentFieldActorId;
            pcInitVar0 = currentFieldActorId;
            psVar7->m0_flags = psVar7->m0_flags & 0xffffff7f | 0x4400;
        }
        iVar2 = currentFieldActorId;
        partyToFieldEntityArrayMapping[iVar4] = currentFieldActorId;
        if (pcInitVar1 == 0) {
            OP_INIT_ENTITY_SCRIPT_sub0(iVar2, iVar4, &partyCharacterBuffers[iVar4], 1, 0, iVar4, 1);
            pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffffcff | 0x400;
        }
        else {
            OP_INIT_ENTITY_SCRIPT_sub0
            (iVar2, (uint)characterMappingTable[iVar3] + pcInitVar1, &fieldActorSetupParams[pcInitVar1 + characterMappingTable[iVar3]], 0, 0,
                (uint)characterMappingTable[iVar3] + pcInitVar1 | 0x80, 1);
            psVar7 = pCurrentFieldScriptActor;
            psVar1 = pKernelGameState;
            pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags & 0xfffffcff | 0x400;
            if (psVar1->m22B1_isOnGear[iVar4] != 0) {
                psVar6 = fieldEntityArray[currentFieldActorId].m4_pSub4;
                fieldEntityArray[currentFieldActorId].m4_pSub4 = fieldEntityArray[unkPartyTable[iVar4]].m4_pSub4;
                fieldEntityArray[unkPartyTable[iVar4]].m4_pSub4 = psVar6;
                psVar7->m0_flags = psVar7->m0_flags & 0xfffffaff | 0x200;
            }
        }
        pcInitVar2 = -0xc0;
        fieldEntityArray[currentFieldActorId].m58_flags = fieldEntityArray[currentFieldActorId].m58_flags & 0xffdf;
        local_v0_684 = getVariable(2);
        OP_INIT_ENTITY_PC_Sub0(local_v0_684);
        OP_INIT_ENTITY_SCRIPT_sub1();
        uVar5 = pCurrentFieldScriptActor->m4_flags & 0xfffff7ff;
        psVar7 = pCurrentFieldScriptActor;
    }
    psVar7->m4_flags = uVar5;
    psVar7 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x20000;
    psVar7->m4_flags = psVar7->m4_flags | 0x400;
    psVar7->mCC_scriptPC = psVar7->mCC_scriptPC + 3;
    return;
}

void OP_SET_DIALOG_AVATAR(void)
{
    pCurrentFieldScriptActor->m80_DialogAvatarFace = getCharacter(getImmediateOrVariableUnsigned(1));
    pCurrentFieldScriptActor->mCC_scriptPC += 3;
}

void OP_EXTENDED_OPCODE()
{
    ushort uVar1;

    uVar1 = pCurrentFieldScriptActor->mCC_scriptPC + 1;
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;

    u8 opcodeEX = pCurrentFieldScriptFile[uVar1];

    if (fieldScriptOpcodes_EX[opcodeEX] == nullptr)
    {
        breakCurrentScript = 1;
        return;
    }

    fieldScriptOpcodes_EX[opcodeEX]();
    return;
}

void OP_46(void)

{
    sFieldScriptEntity* psVar1;
    sFieldEntity* psVar2;
    int iVar3;

    iVar3 = getAngleCosFraq((int)pCurrentFieldEntity->m50_modelRotation[1]);
    psVar2 = pCurrentFieldEntity;
    pCurrentFieldScriptActor->m60 = (short)((uint)(iVar3 * 9) >> 10);
    iVar3 = getAngleCosInteger((int)psVar2->m50_modelRotation[1]);
    psVar1 = pCurrentFieldScriptActor;
    pCurrentFieldScriptActor->m64 = (short)(iVar3 * -0x24 >> 0xc);
    psVar1->m4_flags = psVar1->m4_flags | 0x800;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
    return;
}

void setCurrentActorElevation(ushort param_1)
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    (pCurrentFieldScriptActor->m20_position).vy = (uint)param_1 << 0x10;
    psVar1->mEC_elevation = param_1;
    psVar1->m72_elevation = param_1;
    return;
}


void OP_SET_CURRENT_ACTOR_ELEVATION(void)
{
    ushort* puVar1;
    sFieldScriptEntity* psVar2;
    int local_v0_40;

    local_v0_40 = getVar80(1, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 3]);
    setCurrentActorElevation(local_v0_40);
    psVar2 = pCurrentFieldScriptActor;
    puVar1 = &pCurrentFieldScriptActor->mCC_scriptPC;
    pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x40000;
    psVar2->mCC_scriptPC = *puVar1 + 4;
    return;
}

void OP_JUMP(void)
{
    ushort uVar1;

    uVar1 = readU16FromScript(1);
    pCurrentFieldScriptActor->mCC_scriptPC = uVar1;
    return;
}

void OPX_1C(void)
{
    uint* puVar1;
    ushort uVar2;
    byte* pbVar3;
    sFieldScriptEntity* psVar4;
    int iVar5;
    int iVar6;
    sFieldEntitySub4* psVar7;

    psVar4 = pCurrentFieldScriptActor;
    psVar7 = fieldEntityArray[currentFieldActorId].m4_pSub4;
    puVar1 = &pCurrentFieldScriptActor->m4_flags;
    pCurrentFieldScriptActor->m0_flags = pCurrentFieldScriptActor->m0_flags | 0x10000;
    pbVar3 = &pCurrentFieldScriptFile[0];
    psVar4->m4_flags = *puVar1 | 0x200000;
    iVar5 = getVar80(1, (uint)pbVar3[(uint)psVar4->mCC_scriptPC + 7]);
    pbVar3 = &pCurrentFieldScriptFile[0];
    uVar2 = pCurrentFieldScriptActor->mCC_scriptPC;
    (pCurrentFieldScriptActor->m20_position).vx = iVar5 << 0x10;
    iVar5 = getVar40(3, (uint)pbVar3[(uint)uVar2 + 7]);
    pbVar3 = &pCurrentFieldScriptFile[0];
    uVar2 = pCurrentFieldScriptActor->mCC_scriptPC;
    (pCurrentFieldScriptActor->m20_position).vz = iVar5 << 0x10;
    iVar6 = getVar20(5, (uint)pbVar3[(uint)uVar2 + 7]);
    psVar4 = pCurrentFieldScriptActor;
    iVar5 = currentFieldActorId;
    (pCurrentFieldScriptActor->m20_position).vy = iVar6 << 0x10;
    fieldEntityArray[iVar5].mC_matrix.t[0] = (psVar4->m20_position.vx >> 0x10);
    fieldEntityArray[iVar5].mC_matrix.t[1] = (psVar4->m20_position.vy >> 0x10);
    fieldEntityArray[iVar5].mC_matrix.t[2] = (psVar4->m20_position.vz >> 0x10);
    psVar7->m0_position.vx = (psVar4->m20_position).vx;
    psVar7->m0_position.vy = (psVar4->m20_position).vy;
    psVar7->m0_position.vz = (psVar4->m20_position).vz;
    psVar4->mCC_scriptPC = psVar4->mCC_scriptPC + 8;
    return;
}

void OP_23(void)
{
    sFieldScriptEntity* psVar1;

    psVar1 = pCurrentFieldScriptActor;
    fieldEntityArray[currentFieldActorId].m58_flags = fieldEntityArray[currentFieldActorId].m58_flags | 0x20;
    psVar1->mCC_scriptPC = psVar1->mCC_scriptPC + 1;
    return;
}


void OP_SET_VAR_FROM_S16(void)
{
    ushort uVar1;
    int uVar2;

    uVar1 = readU16FromScript(1);
    uVar2 = getVar40(3, (uint)pCurrentFieldScriptFile[(uint)pCurrentFieldScriptActor->mCC_scriptPC + 5]);
    setVar((uint)uVar1, uVar2);
    pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 6;
    return;
}


void executeFieldScript(int param)
{
    {
        fieldScriptOpcodes[0x00] = OP_STOP;
        fieldScriptOpcodes[0x01] = OP_JUMP;
        fieldScriptOpcodes[0x02] = OP_JUMP_IF;
        fieldScriptOpcodes[0x0B] = OP_INIT_ENTITY_NPC;
        fieldScriptOpcodes[0x16] = OP_INIT_ENTITY_PC;
        fieldScriptOpcodes[0x19] = OP_SET_ACTOR_POSITION_2D;
        fieldScriptOpcodes[0x1C] = OP_SET_CURRENT_ACTOR_ELEVATION;
        fieldScriptOpcodes[0x23] = OP_23;
        fieldScriptOpcodes[0x2A] = OP_2A;
        fieldScriptOpcodes[0x35] = OP_SET_VAR_FROM_S16;
        fieldScriptOpcodes[0x46] = OP_46;
        fieldScriptOpcodes[0x5F] = OP_SET_CURRENT_ACTOR_CARDINAL_DIRECTION;
        fieldScriptOpcodes[0xBC] = OP_INIT_ENTITY_SCRIPT;
        fieldScriptOpcodes[0xFE] = OP_EXTENDED_OPCODE;

        fieldScriptOpcodes_EX[0x0D] = OP_SET_DIALOG_AVATAR;
        fieldScriptOpcodes_EX[0x18] = OP_ADD_TO_CURRENT_PARTY;
        fieldScriptOpcodes_EX[0x1C] = OPX_1C;
    }

    breakCurrentScript = 0;
    fieldExectuteMaxCycles = param;
    int cycles = 0;
    if (param > 0)
    {
        while (cycles <= 0x400)
        {
            u8 opcodeId = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC];

            if (fieldScriptOpcodes[opcodeId] == nullptr)
            {
                breakCurrentScript = 1;
                return;
            }

            //_CrtCheckMemory();
            fieldScriptOpcodes[opcodeId]();
            //_CrtCheckMemory();

            MissingCode();

            cycles++;
            if (cycles >= fieldExectuteMaxCycles)
            {
                return;
            }
        }

        if (fieldDebugDisable == 0) {
            sprintf_screen("EVENTLOOP ERROR ACT=%d\n", currentFieldActorId);
        }
    }
}

void startAllEntityScripts()
{
    if (fieldScriptEntityAlreadyInitialized == 0)
    {
        setVar(0x10, 0);
        setVarsForCurrentParty();

        for (int i = 0; i < numActiveFieldScriptEntity; i++)
        {
            // that was probably some macro
            pCurrentFieldEntity = &fieldEntityArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            currentFieldActorId = i;
            pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 2); // the update script

            // Does the entry point have any code?
            if ((READ_LE_U16(pCurrentFieldScriptFile + pCurrentFieldScriptActor->mCC_scriptPC) == 0))
            {
                pCurrentFieldScriptActor->m4_flags |= 0x4000000;
            }

            // again, this time for the init script
            pCurrentFieldEntity = &fieldEntityArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            currentFieldActorId = i;
            pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 0); // the init script

        }

        // execute the init script
        for (int i = 0; i < numActiveFieldScriptEntity; i++)
        {
            pCurrentFieldEntity = &fieldEntityArray[i];
            pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
            fieldScriptInitVar0 = 0;
            currentScriptFinished = 0;
            currentFieldActorId = i;

            executeFieldScript(0xFFFF);

            if (fieldScriptInitVar0 == 0)
            {
                MissingCode();
            }
        }

        MissingCode();
    }
    else
    {
        assert(0);
    }
}

void initFieldData()
{
    resetFieldDefault();

    for (int i=0; i<32; i++)
    {
        fieldVramMapping[i].m0_vramX = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 0);
        fieldVramMapping[i].m2_vramY = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 2);
        fieldVramMapping[i].m4 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 4);
        fieldVramMapping[i].m6 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 6);
    }

    MissingCode();

    {
        {

            int rawFieldImageBundleSize = READ_LE_U32(&rawFieldBundle[0x10C]);
            rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
            fieldDecompress(rawFieldImageBundleSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x130]), rawFieldImageBundle);

            int numImages = READ_LE_U32(rawFieldImageBundle.begin());
            for (int i = 0; i < numImages; i++)
            {
                int offset = READ_LE_U32(rawFieldImageBundle.begin() + i * 4);
                uploadFieldImages(rawFieldImageBundle.begin() + offset);
            }
        }
        
        {
            int rawFieldImageBundle2Size = READ_LE_U32(&rawFieldBundle[0x11C]);
            rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
            fieldDecompress(rawFieldImageBundle2Size + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x140]), rawFieldImageBundle2);

            int numImages = READ_LE_U32(rawFieldImageBundle2.begin());
            for (int i = 0; i < numImages; i++)
            {
                int offset = READ_LE_U32(rawFieldImageBundle2.begin() + 4 + i * 4);

                if (fieldVramMapping[i].m6 == 0)
                {
                    int x = fieldVramMapping[i].m0_vramX;
                    int y = fieldVramMapping[i].m2_vramY;

                    uploadNpcSpriteSheet(rawFieldImageBundle2.begin() + offset, x, y);
                }
                
            }
        }

        DrawSync(0); // needs to be done to transfert to vram before the bundles get deallocated
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x114]);
        rawFieldModels.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x138]), rawFieldModels);

        int numModels = READ_LE_U32(rawFieldModels.begin());
        gCurrentFieldModels.clear();
        gCurrentFieldModels.resize(numModels);
        for (int i = 0; i < numModels; i++)
        {
            int offset = READ_LE_U32(rawFieldModels.begin() + 4 + i * 4);
            int nextOffset;
            
            if (i < numModels - 1)
            {
                nextOffset = READ_LE_U32(rawFieldModels.begin() + 4 + (i + 1) * 4);
            }
            else
            {
                nextOffset = rawFieldModels.size();
            }
            fieldModelRelocation(rawFieldModels.begin() + offset);
            gCurrentFieldModels[i].init(rawFieldModels.begin() + offset, nextOffset - offset);
        }
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x120]);
        rawFieldScriptData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x144]), rawFieldScriptData);
        numActiveFieldScriptEntity = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
        pCurrentFieldScriptFile = rawFieldScriptData.begin() + 0x84 + numActiveFieldScriptEntity * 0x40;
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x12C]);
        rawFieldTriggerData.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x150]), rawFieldTriggerData);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x128]);
        rawFieldDialogBundle.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x14C]), rawFieldDialogBundle);
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x110]);
        rawFieldWalkMesh.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x134]), rawFieldWalkMesh);
        walkMesh.init(rawFieldWalkMesh);
    }

    MissingCode(); // WalkMesh init code

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x118]);
        rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x13C]), rawFieldActorSetupParams);

        u32 count = READ_LE_U32(rawFieldActorSetupParams.begin());
        fieldActorSetupParams.resize(count);
        for (int i=0; i<count; i++)
        {
            fieldActorSetupParams[i].init(rawFieldActorSetupParams.begin() + READ_LE_U32(4 + rawFieldActorSetupParams.begin() + 4 * i));
        }
    }

    fieldInitVar1[0] = 1;
    fieldInitVar1[1] = 1;
    fieldInitVar1[2] = 1;
    fieldInitVar1[3] = 1;

    setupField3d(rawFieldBundle.begin() + 0x154);

    numFieldEntities = READ_LE_U16(&rawFieldBundle[0x18C]);
    fieldEntityArray.resize(numFieldEntities);

    for (int i = 0; i < numFieldEntities; i++)
    {
        memset(&fieldEntityArray[i], 0, sizeof(sFieldEntity));
    }

    std::vector<u8>::iterator fieldEntitySetup = rawFieldBundle.begin() + 0x190;
    for (int i = 0; i < numFieldEntities; i++)
    {
        fieldEntityArray[i].m58_flags = READ_LE_U16(fieldEntitySetup);
        fieldEntityArray[i].m50_modelRotation[0] = READ_LE_S16(fieldEntitySetup + 2);
        fieldEntityArray[i].m50_modelRotation[1] = READ_LE_S16(fieldEntitySetup + 4);
        fieldEntityArray[i].m50_modelRotation[2] = READ_LE_S16(fieldEntitySetup + 6);

        fieldEntityArray[i].m2C_matrixBackup.t[0] = fieldEntityArray[i].mC_matrix.t[0] = READ_LE_S16(fieldEntitySetup + 8) << 16;
        fieldEntityArray[i].m2C_matrixBackup.t[1] = fieldEntityArray[i].mC_matrix.t[1] = READ_LE_S16(fieldEntitySetup + 10) << 16;
        fieldEntityArray[i].m2C_matrixBackup.t[2] = fieldEntityArray[i].mC_matrix.t[2] = READ_LE_S16(fieldEntitySetup + 12) << 16;

        // Is this a 3d model?
        if (!(fieldEntityArray[i].m58_flags & 0x40))
        {
            u16 modelIndex = READ_LE_U16(fieldEntitySetup + 14);

            fieldEntityArray[i].m0 = new sFieldEntitySub0;
            sModel& pModel = gCurrentFieldModels[modelIndex];
            std::vector<sModelBlock>::iterator pModelBlock = pModel.m10_blocks.begin();

            fieldEntityArray[i].m0->m4_pModelBlock = pModelBlock;

            initModel1(*pModelBlock, fieldEntityArray[i].m0->m8, fieldEntityArray[i].m0->mC_end);

            MissingCode(); // the whole model init stuff here
        }
        else
        {
            fieldEntityArray[i].m50_modelRotation[0] = 0;
            fieldEntityArray[i].m50_modelRotation[1] = 0;
            fieldEntityArray[i].m50_modelRotation[2] = 0;

        }

        initFieldScriptEntity(i);

        fieldEntitySetup += 16;
    }

    MissingCode();

    startAllEntityScripts();

    MissingCode();

    // hack!
    fieldIdForDebugger = currentFieldId0 / 2;
}

void flagAllocation(std::vector<u8>&)
{
    MissingCode();
}

void unflagAllocation(std::vector<u8>&)
{
    MissingCode();
}

int isCDBusy()
{
    MissingCode();
    return 0;
}



void loadRawFieldBundle(int fieldId)
{
    rawFieldBundle.resize(getFileSizeAligned(184 + fieldId));
    flagAllocation(rawFieldBundle);

    readFile(184 + fieldId, rawFieldBundle, 0, 0x80);
}

int loadNewField(int fieldId, int param)
{
    if ((currentFieldId1 == param) && (currentFieldId0 == fieldId))
    {
        return 0;
    }

    if (isCDBusy())
    {
        return -1;
    }

    waitReadCompletion(0);
    if (currentFieldId1 != -1)
    {
        unflagAllocation(rawFieldBundle);
        rawFieldBundle.clear();
    }
    loadRawFieldBundle(fieldId);

    currentFieldId0 = fieldId;
    currentFieldId1 = param;

    return -1;
}

void loadInitialField()
{
    waitReadCompletion(0);

    while (loadNewField((requestFieldId0 & 0xFFF) * 2, 0));
}


void* allocateBufferForVramUpload(int)
{
    MissingCode();
    return nullptr;
}

std::array<u8, 2048 * 512> gVram;
void clearModelTextureCache();

void loadImageFileToVram(int fileId, void* allocation, int, int, int, int, int, int, int, int)
{
    // this is completely different from the original code that was done async and using the cd callbacks
    MissingCode();

    // make sure we recreate all texture after loading this
    clearModelTextureCache();

    std::vector<u8> data;
    c_isoManager::getCurrentDisc()->readData(getFileStartSector(fileId), getFileSize(fileId), data);

    int offset = 0;
    while (offset < data.size() - 0x14)
    {
        std::vector<u8>::iterator texture = data.begin() + offset;
        u32 type = READ_LE_U16(texture + 0x00);
        u16 pos_x = READ_LE_U16(texture + 0x04);
        u16 pos_y = READ_LE_U16(texture + 0x06);
        u16 move_x = READ_LE_U16(texture + 0x08);
        u16 move_y = READ_LE_U16(texture + 0x0a);
        u16 width = READ_LE_U16(texture + 0x0c);
        u16 height = READ_LE_U16(texture + 0x0e);
        u16 chunks = READ_LE_U16(texture + 0x12);

        if (width > 2048 || height > 512 || width == 0 || height == 0) {
            return;
        }
        int blockSize = 0x1C + chunks * 2;
        offset += (blockSize + 2047) & ~2047;
        for (int i = 0; i < chunks; i++) {
            height = READ_LE_U16(texture + 0x1C + i * 2);
            for (int j = 0; j < height; j++) {
                memcpy(&gVram[0] + (pos_y + move_y + j) * 2048 + (pos_x + move_x) * 2, ((u8*)&data[0]) + offset + j * width * 2, width * 2);
            }
            pos_y += height;
            blockSize = width * height * 2;
            offset += (blockSize + 2047) & ~2047;
        }
    }
}

int fieldGraphicsUploaded = 0;
void loadFieldGraphics()
{
    if (!fieldGraphicsUploaded)
    {
        fieldGraphicsUploaded = 1;
        void* fieldGraphicsAllocation = allocateBufferForVramUpload(4);
        loadImageFileToVram((requestFieldId0 & 0xfff) * 2 + 0xb9, fieldGraphicsAllocation, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}


void bootField()
{
    MissingCode();

    setCurrentDirectory(4, 0);
    loadInitialField();

    MissingCode();

    waitReadCompletion(0);
    setCurrentDirectory(4, 0);
    initFieldData();
    loadFieldGraphics();

    MissingCode();

    if (fieldGraphicsUploaded)
    {
        MissingCode();
        fieldGraphicsUploaded = 0;
        MissingCode();
    }

    MissingCode();
}

void allocatePartyCharacterBuffers()
{
    initModel3(8, 0);

    partyCharacterBuffersRaw[0].resize(0x14000);
    partyCharacterBuffersRaw[1].resize(0x14000);
    partyCharacterBuffersRaw[2].resize(0x14000);

    flagAllocation(partyCharacterBuffersRaw[0]);
    flagAllocation(partyCharacterBuffersRaw[1]);
    flagAllocation(partyCharacterBuffersRaw[2]);
}

void waitForReadFinished(void)

{
    int iVar1;

    do {
        iVar1 = isCDBusy();
    } while (iVar1 != 0);
    waitReadCompletion(0);
    return;
}

int characterOrGearsLoadingInProgress = 0;
int fieldRequestedGears = 0;
int typeOfPlayableCharacterLoaded = 0;

void finalizeLoadPlayableCharacters()
{
    if (characterOrGearsLoadingInProgress != 0) {
        waitForReadFinished();

        for (int i = 0; i < 3; i++)
        {
            unflagAllocation(partyCharacterBuffersRaw[i]);
            if (currentParty[i] != -1)
            {
                unflagAllocation(partyCharacterBuffersCompressed[i]);
                decompress(partyCharacterBuffersCompressed[i].begin(), partyCharacterBuffersRaw[i]);
                partyCharacterBuffersCompressed[i].clear();

                partyCharacterBuffers[i].init(partyCharacterBuffersRaw[i].begin());
            }
        }

        characterOrGearsLoadingInProgress = 0;
    }
}

void refinalizePlayableCharacters(int)
{
    MissingCode();
}

void setupFieldCurrentPartyFromKernelAsGears()
{
    MissingCode();
}

struct sLoadingBatchCommands
{
    u16 m0_fileIndex;
    std::vector<u8>* m4_loadPtr;
};

sLoadingBatchCommands playableCharacterLoadingBatchCommands[11];

void batchStartLoadingFiles(sLoadingBatchCommands* pCommands, int param_2)
{
    // TODO: this is a quick and dirty implementation, not how the original code worked
    while (pCommands->m4_loadPtr)
    {
        readFile(pCommands->m0_fileIndex, *pCommands->m4_loadPtr, 0, 0);

        pCommands++;
    }
}

void setupFieldCurrentPartyFromKernelAsCharacters()
{
    pKernelGameState = &gameState;

    for (int i=0; i<3; i++)
    {
        currentParty[i] = -1;
        if (gameState.m1D34_currentParty[i] != -1)
        {
            currentParty[i] = gameState.m1D34_currentParty[i];
        }
    }

    int loadSlot = 0;
    for (int i=0; i<3; i++)
    {
        if (currentParty[i] != -1)
        {
            playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = currentParty[i] + 5;
            asyncPartyCharacterLoadingTable[i] = currentParty[i];
            partyCharacterBuffersCompressed[i].resize(getFileSizeAligned(currentParty[i] + 5));
            playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = &partyCharacterBuffersCompressed[i];
            flagAllocation(partyCharacterBuffersCompressed[i]);
            loadSlot++;
        }
    }

    playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = 0;
    playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = nullptr;

    batchStartLoadingFiles(playableCharacterLoadingBatchCommands, 0);
    typeOfPlayableCharacterLoaded = 1;
}

void startLoadingPlayableCharacters()
{
    waitForReadFinished();
    if (characterOrGearsLoadingInProgress == 1) {
        finalizeLoadPlayableCharacters();
        if (fieldScriptEntityAlreadyInitialized != 0) {
            return;
        }
    }
    else {
        if (fieldScriptEntityAlreadyInitialized != 0) {
            refinalizePlayableCharacters(0);
            return;
        }
    }
    fieldRequestedGears = (requestFieldId0 & 0xc000) != 0;
    characterOrGearsLoadingInProgress = 0;
    if (fieldRequestedGears) {
        if (typeOfPlayableCharacterLoaded != 2) {
            setupFieldCurrentPartyFromKernelAsGears();
            characterOrGearsLoadingInProgress = 1;
        }
    }
    else {
        if (typeOfPlayableCharacterLoaded != 1) {
            setupFieldCurrentPartyFromKernelAsCharacters();
            characterOrGearsLoadingInProgress = 1;
        }
    }
}

void freeFieldData()
{
    MissingCode();

    fieldEntityArray.clear();
    rawFieldTriggerData.clear();
    rawFieldDialogBundle.clear();
    rawFieldScriptData.clear();
    rawFieldWalkMesh.clear();
    walkMesh.clear();
    rawFieldModels.clear();
    gCurrentFieldModels.clear();
    rawFieldActorSetupParams.clear();

    MissingCode();
}

void transitionFields()
{
    MissingCode();

    freeFieldData();

    MissingCode();

    switch (fieldTransitionMode)
    {
    case 2:
        MissingCode();
        startLoadingPlayableCharacters();
        finalizeLoadPlayableCharacters();
        initFieldData();
        loadFieldGraphics();
        if (fieldGraphicsUploaded)
        {
            MissingCode();
            fieldGraphicsUploaded = 0;
            MissingCode();
        }
        MissingCode();
        break;
    default:
        assert(0);
        break;
    }

    fieldTransitionMode = 2;
    MissingCode();

}

void fieldEntryPoint()
{
    MissingCode();

    setCurrentDirectory(4, 0); // TODO: this is not explicitly called at this level

    allocatePartyCharacterBuffers();

    MissingCode();

    requestFieldId0 = gameState.m231A_fieldID;
    pKernelGameState = &gameState;

    MissingCode();

    startLoadingPlayableCharacters();
    finalizeLoadPlayableCharacters();

    MissingCode();

    bootField();

    MissingCode();

    while (!noahFrame_end())
    {
        noahFrame_start();

        ////
        if ((fieldChangePrevented == 0) /*&& (fieldChangePrevented2 == 0)*/)
        {
            //if ((asyncLoadingVar1 == 0xff) && (DAT_Field__800adb90 == 0))
            {
                setCurrentDirectory(4, 0); //TODO: shouldn't be necessary!
                if (loadNewField((requestFieldId0 & 0xfff) * 2, 0) == 0)
                {
                    if (isCDBusy() == 0)
                    {
                        //if (screenEffects[0].m3C_duration == 0)
                        {
                            //fieldTransitionCompleted = 0;
                            //saveStateToKernel();
                            waitReadCompletion(0);
                            transitionFields();
                            //resetInputs();
                            //fieldTransitionCompleted = 1;
                        }
                    }
                }
            }
        }
        ////
    }
}
