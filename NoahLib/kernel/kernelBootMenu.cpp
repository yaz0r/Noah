#include "noahLib.h"
#include "kernelBootMenu.h"
#include "kernel/graphics.h"
#include "kernel/memory.h"
#include "psx/libgpu.h"
#include "kernel/gameMode.h"
#include "kernel/debugText.h"
#include "kernel/inputs.h"
#include "kernel/playTime.h"
#include "kernel/DTL.h"

struct sKernelDisplayContext {
    DRAWENV m0_DrawEnv;
    DISPENV m5C_DispEnv;
    sTag m70_Tag;
    POLY_F3 m74_Poly;

};

std::array<sKernelDisplayContext, 2> kernelMenuDrawEnvDoubleBuffer;

sKernelDisplayContext* gKernelDisplayContext = nullptr;

void initKernelMenuDrawContext(int param_1)
{
    kernelMenuDrawEnvDoubleBuffer[param_1].m0_DrawEnv.isbg = 1;
    kernelMenuDrawEnvDoubleBuffer[param_1].m0_DrawEnv.dtd = 1;
    kernelMenuDrawEnvDoubleBuffer[param_1].m0_DrawEnv.r0 = 0;
    kernelMenuDrawEnvDoubleBuffer[param_1].m0_DrawEnv.g0 = 0;
    kernelMenuDrawEnvDoubleBuffer[param_1].m0_DrawEnv.b0 = 0x20;
    SetPolyF3(&kernelMenuDrawEnvDoubleBuffer[param_1].m74_Poly);
    kernelMenuDrawEnvDoubleBuffer[param_1].m74_Poly.r0 = 0xff;
    kernelMenuDrawEnvDoubleBuffer[param_1].m74_Poly.g0 = 0xff;
    kernelMenuDrawEnvDoubleBuffer[param_1].m74_Poly.b0 = 0xff;
}


void setupKernelMenuDraw(void)
{
    resetMemoryAllocStats(6, 0);
    initDebugFont(8, 0x10, 0x170, 0x1e0, 1000, 1, 0x3c0, 0x100, 0x3c0, 0x1ff, 0);
    SetDefDrawEnv(&kernelMenuDrawEnvDoubleBuffer[0].m0_DrawEnv, 0, 0, 0x140, 0xe0);
    SetDefDrawEnv(&kernelMenuDrawEnvDoubleBuffer[1].m0_DrawEnv, 0, 0xf0, 0x140, 0xe0);
    SetDefDispEnv(&kernelMenuDrawEnvDoubleBuffer[0].m5C_DispEnv, 0, 0xf0, 0x140, 0xe0);
    SetDefDispEnv(&kernelMenuDrawEnvDoubleBuffer[1].m5C_DispEnv, 0, 0, 0x140, 0xe0);
    initKernelMenuDrawContext(0);
    initKernelMenuDrawContext(1);
    return;
}

s32 menuQuit = 0;
s32 frameCount = 0;
s32 oddOrEven = 0;
s32 kernelMenuCurrentSelectedEntry = 0;

void printKernelMenu(void)
{
    char timeString[24];

    if (((newPadButtonForField & 0x1000) != 0) && (kernelMenuCurrentSelectedEntry += -1, kernelMenuCurrentSelectedEntry < 0)) {
        kernelMenuCurrentSelectedEntry = 5;
    }
    if (((newPadButtonForField & 0x4000) != 0) && (kernelMenuCurrentSelectedEntry += 1, 5 < kernelMenuCurrentSelectedEntry)) {
        kernelMenuCurrentSelectedEntry = 0;
    }
    if ((newPadButtonForDialogs & 0x20) != 0) {
        setGameMode(kernelMenuCurrentSelectedEntry + 1);
        menuQuit = 0;
    }
    sprintf(timeString, "%02d:%02d:%02d", g_playTimeHours, g_playTimeMinutes, g_playTimeSeconds);
    const char* diskMode = "CD EMU";
    if (runningOnDTL != 0) {
        diskMode = "PC HDD";
    }
    logToScreen(" XENOGEARS Kernel MENU\n  %s %s MODE\n\n", timeString, diskMode);
    logToScreen("    Field\n    Battle\n    Worldmap\n    Battling\n    Menu\n    Movie\n\n");

    gKernelDisplayContext->m74_Poly.x0y0.vx = 0x20;
    gKernelDisplayContext->m74_Poly.x0y0.vy = (kernelMenuCurrentSelectedEntry * 8 + 0x28);
    gKernelDisplayContext->m74_Poly.x1y1.vx = 0x27;
    gKernelDisplayContext->m74_Poly.x1y1.vy = (kernelMenuCurrentSelectedEntry * 8 + 0x2c);
    gKernelDisplayContext->m74_Poly.x2y2.vx = 0x20;
    gKernelDisplayContext->m74_Poly.x2y2.vy = (kernelMenuCurrentSelectedEntry * 8 + 0x30);
}

void kernelBootMenu(void) {
    setupKernelMenuDraw();
    menuQuit = 1;
    do {
        do {
            frameCount += 1;
            oddOrEven = frameCount & 1;
            gKernelDisplayContext = &kernelMenuDrawEnvDoubleBuffer[oddOrEven];
            sTag* p = &kernelMenuDrawEnvDoubleBuffer[oddOrEven].m70_Tag;
            TermPrim(p);
            printDebugTextOnScreen(p);
            printKernelMenu();
            AddPrim(p, &gKernelDisplayContext->m74_Poly);
            DrawSync(0);
            VSync(0);
            PutDrawEnv(&gKernelDisplayContext->m0_DrawEnv);
            PutDispEnv(&gKernelDisplayContext->m5C_DispEnv);
            DrawOTag(p);
        } while (menuQuit != 0);
    } while (oddOrEven == 0);
    DrawSync(0);
    bootGame(0);
}