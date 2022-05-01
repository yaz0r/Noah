#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"
#include "kernel/isoManager.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/font.h"
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

    setCurrentDirectory(0, 1);

    MissingCode();

    // load and init font
	{
        std::vector<u8> compressedFontData;
		readFile(6, compressedFontData, 0, 0);
		waitReadCompletion(0);
		traceNextAlloc(0x30);
        std::vector<u8> fontData = mallocAndDecompress(compressedFontData.begin());
		initFont(fontData);
	}
    {
        std::vector<u8> compressedData;
        readFile(7, compressedData, 0, 0);
        waitReadCompletion(0);
        traceNextAlloc(0x30);
        std::vector<u8> data = mallocAndDecompress(compressedData.begin());
        setupPrintDialogTextVar(data);
    }
    if (1) {
        {
            std::vector<u8> overlay;
            readFile(0x10, overlay, 0, 0);
            waitReadCompletion(0);
            std::vector<u8> overlayDecompressed = mallocAndDecompress(overlay.begin());
            FILE* fHandle = fopen((std::string("battle") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlayDecompressed[0], 1, overlayDecompressed.size(), fHandle);
                fclose(fHandle);
            }
        }

        setCurrentDirectory(0x20, 0);
        {
            std::vector<u8> overlay;
            readFile(0x1, overlay, 0, 0);
            waitReadCompletion(0);
            FILE* fHandle = fopen((std::string("battle_1") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlay[0], 1, overlay.size(), fHandle);
                fclose(fHandle);
            }
        }
        setCurrentDirectory(0x10, 2);
        {
            std::vector<u8> overlay;
            readFile(0x1, overlay, 0, 0);
            waitReadCompletion(0);
            FILE* fHandle = fopen((std::string("battle_10_2_1") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlay[0], 1, overlay.size(), fHandle);
                fclose(fHandle);
            }
        }
        setCurrentDirectory(0x10, 0);
        {
            std::vector<u8> overlay;
            readFile(0x4, overlay, 0, 0);
            waitReadCompletion(0);
            FILE* fHandle = fopen((std::string("battle_10_0_4") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlay[0], 1, overlay.size(), fHandle);
                fclose(fHandle);
            }
        }
        setCurrentDirectory(0x10, 2);
        {
            std::vector<u8> overlay;
            readFile(0x6, overlay, 0, 0);
            waitReadCompletion(0);
            FILE* fHandle = fopen((std::string("battle_debug") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlay[0], 1, overlay.size(), fHandle);
                fclose(fHandle);
            }
        }
        setCurrentDirectory(0, 1);
    }

    if (0) {
        {
            std::vector<u8> overlay;
            readFile(0xF, overlay, 0, 0);
            waitReadCompletion(0);
            std::vector<u8> overlayDecompressed = mallocAndDecompress(overlay.begin());
            FILE* fHandle = fopen((std::string("worldmap") + std::string(".ovl")).c_str(), "wb+");
            if (fHandle) {
                fwrite(&overlayDecompressed[0], 1, overlayDecompressed.size(), fHandle);
                fclose(fHandle);
            }
        }
    }

    MissingCode();

    initGameState();

    MissingCode();

    while (!gCloseApp) {
        fieldEntryPoint();
    }

    return true;
}

void noahFrame_start()
{
    StartFrame();

    initPSXFB();

    vsyncCallback();
}

bool noahFrame_end()
{
    c_filesystemExplorer::frame();

    fieldInspector_frame();

    fieldViewDebug_step();

    drawPSXFB();

    EndFrame();

    return gCloseApp;
}

void noahExit()
{
    deleteBgfxGlue();
}

u32 READ_LE_U32(const sPS1Pointer& ptr)
{
    return READ_LE_U32(ptr.getPointer());
}

s32 READ_LE_S32(const sPS1Pointer& ptr)
{
    return READ_LE_S32(ptr.getPointer());
}

u16 READ_LE_U16(const sPS1Pointer& ptr)
{
    return READ_LE_U16(ptr.getPointer());
}

s16 READ_LE_S16(const sPS1Pointer& ptr)
{
	return READ_LE_S16(ptr.getPointer());
}

u8 READ_LE_U8(const sPS1Pointer& ptr)
{
    return *(u8*)(ptr.getPointer());
}

s8 READ_LE_S8(const sPS1Pointer& ptr)
{
    return *(s8*)(ptr.getPointer());
}

u8 READ_LE_U8(const void* ptr)
{
    return *(u8*)ptr;
}

s8 READ_LE_S8(const void* ptr)
{
    return *(s8*)ptr;
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

u8 READ_LE_U8(const std::vector<u8>::const_iterator& inputStream)
{
    return *inputStream;
}

s8 READ_LE_S8(const std::vector<u8>::const_iterator& inputStream)
{
    return *(s8*)&inputStream[0];
}

u16 READ_LE_U16(const std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_U16(&inputStream[0]);
}

s16 READ_LE_S16(const std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_S16(&inputStream[0]);
}

u32 READ_LE_U32(const std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_U32(&inputStream[0]);
}

s32 READ_LE_S32(const std::vector<u8>::const_iterator& inputStream)
{
    return READ_LE_S32(&inputStream[0]);
}
