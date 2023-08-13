#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"
#include "kernel/isoManager.h"
#include "kernel/filesystem.h"
#include "kernel/gameState.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/gameMode.h"
#include "kernel/font.h"
#include "debug/vram.h"
#include "field/field.h"
#include "field/fieldDebugger/fieldInspector.h"
#include "field/fieldDebugger/fieldViewDebug.h"
#include "battle/battleDebug.h"

ImLogger Noah_Logger[eLogCategories::log_max];

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

    setGameMode(6); // for into movie?

    while (!gCloseApp) {
        bootGame(0);
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
    if (ImGui::Begin("Loggers")) {
        if (ImGui::BeginTabBar("tabs")) {

            for (int i = 0; i < eLogCategories::log_max; i++)
            {
                switch (i)
                {
                case eLogCategories::log_default:
                    if (ImGui::BeginTabItem("Default")) {
                        Noah_Logger[i].Draw();
                        ImGui::EndTabItem();
                    }
                    break;
                case eLogCategories::log_warning:
                    if (ImGui::BeginTabItem("Warning")) {
                        Noah_Logger[i].Draw();
                        ImGui::EndTabItem();
                    }
                    break;
                case eLogCategories::log_unimlemented:
                    if (ImGui::BeginTabItem("Unimplemented")) {
                        Noah_Logger[i].Draw();
                        ImGui::EndTabItem();
                    }
                    break;
                case eLogCategories::log_hacks:
                    if (ImGui::BeginTabItem("Hacks")) {
                        Noah_Logger[i].Draw();
                        ImGui::EndTabItem();
                    }
                    break;
                default:
                    assert(0);
                    break;
                }
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();


    c_filesystemExplorer::frame();

    fieldInspector_frame();

    fieldViewDebug_step();

    battleDebugView_frame();

    vramDebug_frame();

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

void UnimplementedImpl(const char* functionName) {
    Noah_Logger[eLogCategories::log_unimlemented].AddLog("Unimplemented code in %s\n", functionName);
}

void HackImpl(const char* functionName) {
    Noah_Logger[eLogCategories::log_hacks].AddLog("Hack in %s\n", functionName);
}

uint randSeed = 0;

int xenoRand() {
    randSeed = randSeed * 0x41c64e6d + 0x3039;
    return randSeed >> 0x10 & 0x7fff;
}