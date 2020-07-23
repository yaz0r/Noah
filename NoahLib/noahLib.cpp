#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"
#include "kernel/isoManager.h"
#include "kernel/fileSystem.h"
#include "kernel/gameState.h"
#include "field/fieldInspector/fieldInspector.h"
#include "field/field.h"

bool noahInit(int argc, char* argv[])
{
    initBgfxGlue(argc, argv);
    c_isoManager::init();

    //////////////////////////////////////////////////////////////////////////
    // start the original code

    MissingCode();

    initCDAndFileSystem(&fatFileTableBuffer, &fatDirectoryTableBuffer, -1);

    MissingCode();

    initGameState();

    MissingCode();


    fieldEntryPoint();

    return true;
}

bool noahFrame()
{
    StartFrame();

    c_filesystemExplorer::frame();

    fieldInspector_frame();

    EndFrame();

    return gCloseApp;
}

void noahExit()
{
    deleteBgfxGlue();
}

u16 READ_LE_U16(void* ptr)
{
    return *(u16*)ptr;
}

s16 READ_LE_S16(void* ptr)
{
    return *(s16*)ptr;
}

u32 READ_LE_U32(void* ptr)
{
    return *(u32*)ptr;
}

s32 READ_LE_S32(void* ptr)
{
    return *(s32*)ptr;
}

u8 READ_LE_U8(std::vector<u8>::iterator& inputStream)
{
    return *inputStream;
}

s8 READ_LE_S8(std::vector<u8>::iterator& inputStream)
{
    return *(s8*)&inputStream[0];
}

u16 READ_LE_U16(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_U16(&inputStream[0]);
}

s16 READ_LE_S16(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_S16(&inputStream[0]);
}

u32 READ_LE_U32(std::vector<u8>::iterator& inputStream)
{
    return READ_LE_U32(&inputStream[0]);
}
