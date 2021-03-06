#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"
#include "kernel/isoManager.h"
#include "kernel/fileSystem.h"
#include "kernel/gameState.h"
#include "field/field.h"
#include "field/fieldDebugger/fieldInspector.h"
#include "field/fieldDebugger/fieldViewDebug.h"

bool noahInit(int argc, char* argv[])
{
    initBgfxGlue(argc, argv);
    c_isoManager::init();

    noahFrame_start();

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

void noahFrame_start()
{
    StartFrame();
}

bool noahFrame_end()
{
    c_filesystemExplorer::frame();

    fieldInspector_frame();

    fieldViewDebug_step();

    EndFrame();

    return gCloseApp;
}

void noahExit()
{
    deleteBgfxGlue();
}

u16 READ_LE_U16(const sPS1Pointer& ptr)
{
    return READ_LE_U16(ptr.getPointer());
}

u8 READ_LE_U8(const sPS1Pointer& ptr)
{
    return *(u8*)(ptr.getPointer());
}

u16 READ_LE_U16(const void* ptr)
{
    return *(u16*)ptr;
}

s16 READ_LE_S16(const void* ptr)
{
    return *(s16*)ptr;
}

u32 READ_LE_U32(const void* ptr)
{
    return *(u32*)ptr;
}

s32 READ_LE_S32(const void* ptr)
{
    return *(s32*)ptr;
}

u8 READ_LE_U8(std::vector<u8>::const_iterator& inputStream)
{
    return *inputStream;
}

s8 READ_LE_S8(std::vector<u8>::const_iterator& inputStream)
{
    return *(s8*)&inputStream[0];
}

u16 READ_LE_U16(std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_U16(&inputStream[0]);
}

s16 READ_LE_S16(std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_S16(&inputStream[0]);
}

u32 READ_LE_U32(std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_U32(&inputStream[0]);
}

s32 READ_LE_S32(std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_S32(&inputStream[0]);
}
