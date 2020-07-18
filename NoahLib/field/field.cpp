#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"

#include <array>

std::vector<u8> rawFieldBundle;
std::vector<u8> fieldHeader;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldTriggerData;
std::vector<u8> rawFieldDialogBundle;
std::vector<u8> rawFieldWalkMesh;
std::vector<u8> rawFieldActorSetupParams;

std::array<s16, 4> fieldInitVar1;

s32 numFieldEntities;

s32 requestFieldId0 = 0; //TODO: should be -1, but that's to force load first field for now
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;

s32 numActiveFieldScriptEntity;
std::vector<u8>::iterator pCurrentFieldScriptFile;

struct sFP1616
{
    short m0_integer;
    short m2_fraq;
};

struct sMatrix
{
    std::array<sFP1616, 3> t;
};

struct sModelBlock
{
    int m34_count;
    // size 0x38
};

struct sModel
{
    sModel(std::vector<u8>::iterator& inputData)
    {
        m0_numBlocks = READ_LE_U32(inputData);
        inputData += 0x10;

        m10_blocks.resize(m0_numBlocks);
        for (int i = 0; i < m0_numBlocks; i++)
        {
            std::vector<u8>::iterator blockData = inputData + i * 0x38;

            m10_blocks[i].m34_count = READ_LE_U32(blockData + 0x34);
        }
    }

    u32 m0_numBlocks;
    std::vector<sModelBlock> m10_blocks;
};

struct sFieldEntitySub0
{
    std::vector<sModelBlock>::iterator m4_pModelBlock;
    std::vector<s16> m8;
    std::vector<s16>::iterator mC_end;
    //size 0x24
};

struct sFieldEntity2dSprite
{
    //size 0x70
};


struct sFieldScriptEntity
{
    u32 m0_flags;
    u32 m4_flags;
    s16 m18[4];
    std::array<sFP1616, 3> m20_position;
    s16 m5A;
    u16 mCC_scriptPC;

    u8 m126;
    u8 m127;
    struct
    {
        u32 m28 : 2;
    }m130;

    struct
    {
        u32 m0 : 4;
        u32 m4 : 1;
    }m134;

    // size 0x138
};

struct sFieldEntitySub4_F4
{
    s16 m14_actorId;
    //size ???
};

struct sFieldEntitySub4
{
    std::array<sFP1616, 3> m0_position;
    s32 mC;
    s32 m10;
    s32 m14;
    s32 m1C;
    s16 m2C;
    u32 m40;
    sFieldEntitySub4_F4* m7C;
    s16 m82;
    s16 m84;
    u16 m86_thisSize;

    sFieldEntitySub4_F4 mF4;
    //size 0x164
};

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    sFieldEntitySub4* m4_pSub4;
    sFieldEntity2dSprite* m8_2dSprite;
    sMatrix mC_matrix;
    sMatrix m2C_matrixBackup;
    sFieldScriptEntity* m4C_scriptEntity;
    s16 m50_modelRotation[3];
    u16 m58_flags;
    u16 m5A;
    //size 0x5C
};

std::vector<sFieldEntity> fieldEntityArray;

void fieldModelRelocation(std::vector<u8>::iterator& pModelData)
{
    MissingCode();
}

void resetFieldDefault()
{
    MissingCode();
}

void uploadFieldImages(std::vector<u8>::iterator& pImageData)
{
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
            assert(0);
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

std::array<s16, 512> fieldVars;

void setVar(int varIndex, s16 value)
{
    fieldVars[varIndex / 2] = value;
}

std::array<int, 3> currentParty;

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

void initModel3(int, int)
{
    MissingCode();
}

void deleteFieldEntitySub4(sFieldEntitySub4* param_1)
{
    MissingCode();
}

void initFieldEntitySub4Sub1(sFieldEntitySub4* param_1)
{
    MissingCode();
}

void initFieldEntitySub4Sub2(sFieldEntitySub4* pThis)
{
    MissingCode();
}

void initFieldEntitySub4Sub3(sFieldEntitySub4* param_1, int)
{
    MissingCode();
}

sFieldEntitySub4* initFieldEntitySub4(sFieldEntitySub4* param_1, std::vector<u8>::iterator pSetup, int param_3, int param_4, int param_5, int param_6, int param_7)
{
    initFieldEntitySub4Sub1(param_1);
    initFieldEntitySub4Sub2(param_1);
    initFieldEntitySub4Sub3(param_1, 0x10000);

    MissingCode();

    return param_1;
}

sFieldEntitySub4* createFieldEntitySub4(std::vector<u8>::iterator pSetup, int param_2, int param_3, int param_4, int param_5, int param_6)
{
    sFieldEntitySub4* pNewEntry = new sFieldEntitySub4;
    pNewEntry->m86_thisSize = sizeof(sFieldEntitySub4);
    return initFieldEntitySub4(pNewEntry, pSetup, param_2, param_3, param_4, param_5, param_6);
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub4(sFieldEntitySub4* param1, int param2, int* param3, int* param4, int* param5)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub5(sFieldEntitySub4* param1, int param2)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub6(sFieldEntitySub4* param1, int param2)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub7(sFieldEntitySub4* param1, int param2)
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0Sub8(sFieldEntitySub4* param1, void(*callback)(sFieldEntitySub4*))
{
    MissingCode();
}

void fieldActorCallback(sFieldEntitySub4* pThis)
{
    fieldEntityArray[pThis->m7C->m14_actorId].m4C_scriptEntity->m4_flags |= 0x10000;
}

s16 WasOpcodeE5Called = 0;

void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int param_2, std::vector<u8>::iterator pSetup, int param_4, int param_5, int param_6, int param_7)
{
    initModel3(8, 0);
    fieldEntityArray[actorId].m4C_scriptEntity->m127 = param_2;
    fieldEntityArray[actorId].m4C_scriptEntity->m126 = param_6;
    fieldEntityArray[actorId].m4C_scriptEntity->m134.m0 = param_5;
    fieldEntityArray[actorId].m4C_scriptEntity->m130.m28 = param_4;
    fieldEntityArray[actorId].m4C_scriptEntity->m134.m4 = param_7;

    if (param_4 == 0)
    {
        s16 sVar6 = READ_LE_S16(fieldHeader.begin() + 2 + param_2 * 8);
        s16 sVar1 = READ_LE_S16(fieldHeader.begin() + 0 + param_2 * 8);

        if (param_5 == 0)
        {
            if (fieldEntityArray[actorId].m5A & 1)
            {
                deleteFieldEntitySub4(fieldEntityArray[actorId].m4_pSub4);
            }

            fieldEntityArray[actorId].m4_pSub4 = createFieldEntitySub4(pSetup, 0x100, ((param_2 + 480) * 0x10000) >> 0x10, sVar1, sVar6, 0x40);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
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
        pFieldEntitySub4->m84 = fieldEntityArray[actorId].mC_matrix.t[1].m2_fraq;
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

    if (WasOpcodeE5Called)
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
        assert(0);
    }

    fieldEntityArray[actorId].mC_matrix.t[0].m2_fraq = fieldEntityArray[actorId].m4C_scriptEntity->m20_position[0].m2_fraq; // TODO: probably wrong, and if not, that also overwrite the integer part
    fieldEntityArray[actorId].mC_matrix.t[1].m2_fraq = fieldEntityArray[actorId].m4C_scriptEntity->m20_position[1].m2_fraq;
    fieldEntityArray[actorId].mC_matrix.t[2].m2_fraq = fieldEntityArray[actorId].m4C_scriptEntity->m20_position[2].m2_fraq;

    fieldEntityArray[actorId].m2C_matrixBackup.t = fieldEntityArray[actorId].mC_matrix.t;

    pFieldEntitySub4->m84 = fieldEntityArray[actorId].mC_matrix.t[1].m2_fraq;

    pFieldEntitySub4->m0_position = fieldEntityArray[actorId].m4C_scriptEntity->m20_position;
}

void OP_INIT_ENTITY_SCRIPT_sub1()
{
    MissingCode();
}

void OP_INIT_ENTITY_SCRIPT()
{
    OP_INIT_ENTITY_SCRIPT_sub0(currentFieldActorId, 0, rawFieldActorSetupParams.begin() + READ_LE_U32(rawFieldActorSetupParams.begin() + 4), 0, 0, 0x80, 1);
    OP_INIT_ENTITY_SCRIPT_sub1();

    pCurrentFieldScriptActor->m0_flags |= 0x100;
    pCurrentFieldScriptActor->m4_flags |= 0x800;
    pCurrentFieldScriptActor->mCC_scriptPC++;
}

void executeFieldScript(int param)
{
    {
        fieldScriptOpcodes[0xBC] = OP_INIT_ENTITY_SCRIPT;
    }

    breakCurrentScript = 0;
    fieldExectuteMaxCycles = param;
    int cycles = 0;
    if (param > 0)
    {
        while (cycles <= 0x400)
        {
            u8 opcodeId = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC];
            fieldScriptOpcodes[opcodeId]();

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

    fieldHeader.resize(256);
    memcpy(&fieldHeader[0], &rawFieldBundle[0], 256); // TODO: yuk...

    MissingCode();

    {
        // this is malloc/free in original code
        std::vector<u8> rawFieldImageBundle;
        std::vector<u8> rawFieldImageBundle2;
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
                int offset = READ_LE_U32(rawFieldImageBundle2.begin() + i * 4);
                MissingCode();
                //uploadFieldImages(rawFieldImageBundle2.begin() + offset);
            }
        }

        DrawSync(0); // needs to be done to transfert to vram before the bundles get deallocated
    }

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x114]);
        rawFieldModels.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x138]), rawFieldModels);

        int numModels = READ_LE_U32(rawFieldModels.begin());
        for (int i = 0; i < numModels; i++)
        {
            int offset = READ_LE_U32(rawFieldModels.begin() + i * 4);
            fieldModelRelocation(rawFieldModels.begin() + offset);
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
    }

    MissingCode(); // WalkMesh init code

    {
        int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x118]);
        rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
        fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x13C]), rawFieldActorSetupParams);
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

        fieldEntityArray[i].m2C_matrixBackup.t[0].m0_integer = fieldEntityArray[i].mC_matrix.t[0].m0_integer = READ_LE_S16(fieldEntitySetup + 8);
        fieldEntityArray[i].m2C_matrixBackup.t[1].m0_integer = fieldEntityArray[i].mC_matrix.t[1].m0_integer = READ_LE_S16(fieldEntitySetup + 10);
        fieldEntityArray[i].m2C_matrixBackup.t[2].m0_integer = fieldEntityArray[i].mC_matrix.t[2].m0_integer = READ_LE_S16(fieldEntitySetup + 12);

        // Is this a 3d model?
        if (!(fieldEntityArray[i].m58_flags & 0x40))
        {
            fieldEntityArray[i].m0 = new sFieldEntitySub0;
            sModel* pModel = new sModel(rawFieldModels.begin() + READ_LE_U32(rawFieldModels.begin() + 4 + READ_LE_U16(fieldEntitySetup + 14)));
            std::vector<sModelBlock>::iterator pModelBlock = pModel->m10_blocks.begin();

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
        rawFieldBundle.resize(0);
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

void bootField()
{
    MissingCode();

    setCurrentDirectory(4, 0);
    loadInitialField();

    MissingCode();

    waitReadCompletion(0);
    setCurrentDirectory(4, 0);
    initFieldData();


    MissingCode();
}

void fieldEntryPoint()
{
    MissingCode();

    bootField();

    MissingCode();
}
