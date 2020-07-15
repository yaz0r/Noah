#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"

#include <array>

std::vector<u8> rawFieldBundle;
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

struct sMatrix
{
    int t[3];
};

struct sFieldEntitySub0
{
    u8* m4_pModel; //TODO: wrap that in the proper class
    //size 0x24
};

struct sFieldEntity
{
    sFieldEntitySub0* m0;
    sMatrix mC_matrix;
    sMatrix m2C_matrixBackup;
    s16 m50[3];
    u16 m58_flags;
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

void initFieldScriptEntity(int)
{
    MissingCode();
}

void initFieldData()
{
    resetFieldDefault();

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

        DrawSync(0); // needs to be done before the bundles get deallocated
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
        fieldEntityArray[i].m50[0] = READ_LE_S16(fieldEntitySetup + 2);
        fieldEntityArray[i].m50[1] = READ_LE_S16(fieldEntitySetup + 4);
        fieldEntityArray[i].m50[2] = READ_LE_S16(fieldEntitySetup + 6);

        fieldEntityArray[i].m2C_matrixBackup.t[0] = fieldEntityArray[i].mC_matrix.t[0] = READ_LE_S16(fieldEntitySetup + 8);
        fieldEntityArray[i].m2C_matrixBackup.t[1] = fieldEntityArray[i].mC_matrix.t[1] = READ_LE_S16(fieldEntitySetup + 10);
        fieldEntityArray[i].m2C_matrixBackup.t[2] = fieldEntityArray[i].mC_matrix.t[2] = READ_LE_S16(fieldEntitySetup + 12);

        if (!(fieldEntityArray[i].m58_flags & 0x40))
        {
            fieldEntityArray[i].m0 = new sFieldEntitySub0;
            fieldEntityArray[i].m0->m4_pModel = &rawFieldModels[0] + READ_LE_U32(rawFieldModels.begin() + 4 + READ_LE_U16(fieldEntitySetup + 14));

            MissingCode(); // the whole model init stuff here
        }
        else
        {
            fieldEntityArray[i].m50[0] = 0;
            fieldEntityArray[i].m50[1] = 0;
            fieldEntityArray[i].m50[2] = 0;

        }

        initFieldScriptEntity(i);

        fieldEntitySetup += 16;
    }

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
