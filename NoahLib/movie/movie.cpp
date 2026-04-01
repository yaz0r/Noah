#include "noahLib.h"
#include "movie.h"
#include "strPlayer.h"
#include "kernel/gameMode.h"
#include "kernel/filesystem.h"
#include "kernel/graphics.h"
#include "kernel/memory.h"
#include "kernel/inputs.h"
#include "kernel/TIM.h"
#include "bgfxGlue.h"

#include "psx/libgpu.h"

#include <bgfx/bgfx.h>

void renderTexturedQuadBgfx(bgfx::ViewId outputView, bgfx::TextureHandle sourceTexture);

// Movie parameters (set by main() or field/battle scripts before entering movie mode)
// Ghidra: movieType, movieNumber, movieReturnMode, movieFadeParam (at 8004fe44-47)
u8 movieType = 0xFF;       // 0xFF = show debug menu, otherwise scripted playback
u8 movieNumber = 0;
u8 movieReturnMode = 1;    // default: return to field
u8 movieFadeParam = 0;

static sStrPlayer g_strPlayer;

// Get the file index for the movie based on type and number
// From Ghidra moviePlaybackLoop: type 0 (picture only) uses dir(0x18,0) file=number+3
//                                 type 1 (picture+ADPCM) uses dir(0x18,1) file=number+2
static int getMovieFileIndex(u8 type, u8 number)
{
    if (type == 0) {
        return number + 3;
    }
    return number + 2;
}

static void movieStartPlaybackFromScript(u8 fadeParam)
{
    // Match Ghidra: movieStartPlaybackFromScript
    if (movieType == 0) {
        setCurrentDirectory(0x18, 0);
    } else if (movieType == 1) {
        setCurrentDirectory(0x18, 1);
    } else {
        return;
    }

    int fileIndex = getMovieFileIndex(movieType, movieNumber);
    int fileStartSector = getFileStartSector(fileIndex);
    int fileSize = getFileSizeAligned(fileIndex);
    int fileSectors = (fileSize + 0x7FF) / 0x800;

    SetDispMask(0);

    RECT clearRect = { 0, 0, 0x280, 0x200 };
    ClearImage(&clearRect, 0, 0, 0);
    DrawSync(0);
    VSync(0);

    g_strPlayer.init();
    g_strPlayer.start(fileStartSector, fileSectors);

    SetDispMask(1);

    // Playback loop
    static bgfx::TextureHandle movieTexture = BGFX_INVALID_HANDLE;
    static int texW = 0, texH = 0;

    while (!g_strPlayer.isFinished()) {
        if (!g_strPlayer.decodeNextFrame()) {
            break;
        }

        const std::vector<u8>& rgb = g_strPlayer.getCurrentFrameRGB();
        int w = g_strPlayer.getFrameWidth();
        int h = g_strPlayer.getFrameHeight();

        if (!rgb.empty() && w > 0 && h > 0) {
            // Convert RGB24 to BGRA32 for BGFX
            std::vector<u8> bgra(w * h * 4);
            for (int i = 0; i < w * h; i++) {
                bgra[i * 4 + 0] = rgb[i * 3 + 2]; // B
                bgra[i * 4 + 1] = rgb[i * 3 + 1]; // G
                bgra[i * 4 + 2] = rgb[i * 3 + 0]; // R
                bgra[i * 4 + 3] = 255;             // A
            }

            if (!bgfx::isValid(movieTexture) || texW != w || texH != h) {
                if (bgfx::isValid(movieTexture)) {
                    bgfx::destroy(movieTexture);
                }
                movieTexture = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::BGRA8, 0, nullptr);
                texW = w;
                texH = h;
            }
            bgfx::updateTexture2D(movieTexture, 0, 0, 0, 0, w, h,
                                  bgfx::copy(bgra.data(), (uint32_t)(w * h * 4)));

            StartFrame();
            renderTexturedQuadBgfx(0, movieTexture);
            EndFrame();
        }
    }

    if (bgfx::isValid(movieTexture)) {
        bgfx::destroy(movieTexture);
        movieTexture = BGFX_INVALID_HANDLE;
        texW = texH = 0;
    }

    g_strPlayer.shutdown();
}

void movieEntryPoint()
{
    MissingCode(); // resetMemoryAllocStats
    setCurrentDirectory(0x18, 0);
    MissingCode(); // FUN_80038d18(0,0) - audio fade

    DrawSync(0);
    VSync(0);
    SetDispMask(0);

    // Scripted playback path (movieType != 0xFF and start button not held)
    if (movieType != 0xFF) {
        movieStartPlaybackFromScript(movieFadeParam);

        movieType = 0xFF;

        setGameMode(movieReturnMode);
        bootGame(0);
        return;
    }

    // Debug menu path
    MissingCode(); // movie debug menu (not needed for normal gameplay)

    setGameMode(0);
    bootGame(0);
}
