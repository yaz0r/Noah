#include "noahLib.h"
#include "libgpu.h"
#include "kernel/graphics.h"

void SetGraphDebug(int level) {
    MissingCode();
}

s32 currentVideoMode = 0;

s32 GetVideoMode()
{
    return currentVideoMode;
}

s32 SetVideoMode(s32 mode) {
    currentVideoMode = mode;
    return currentVideoMode;
}

s32 ResetGraph(s32 mode) {
    MissingCode();
    return 1;
}

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h)
{
    (env->disp).x = (short)x;
    (env->disp).y = (short)y;
    (env->disp).w = (short)w;
    (env->disp).h = (short)h;
    (env->screen).x = 0;
    (env->screen).y = 0;
    (env->screen).w = 0;
    (env->screen).h = 0;
    env->isrgb24 = 0;
    env->isinter = 0;
    env->pad1 = 0;
    env->pad0 = 0;
    return env;
}

DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h)
{
    (env->clip).x = (short)x;
    (env->clip).y = (short)y;
    (env->clip).w = (short)w;
    (env->tw).x = 0;
    (env->tw).y = 0;
    (env->tw).w = 0;
    (env->tw).h = 0;
    env->r0 = 0;
    env->g0 = 0;
    env->b0 = 0;
    env->dtd = 1;
    (env->clip).h = (short)h;
    if (GetVideoMode() == 0) {
        env->dfe = h < 0x101;
    }
    else {
        env->dfe = h < 0x121;
    }
    env->ofs[0] = (short)x;
    env->ofs[1] = (short)y;
    env->tpage = 10;
    env->isbg = 0;
    return env;
}

u16 GetClut(int x, int y)
{
    return (u16)((u16)(y << 6) | (u16)(x >> 4) & 0x3f);
}

u16 GetTPage(int tp, int abr, int x, int y)
{
    return (u16)((ushort)((tp & 3U) << 7) | (ushort)((abr & 3U) << 5) | (ushort)((int)(y & 0x100U) >> 4) | (ushort)((int)(x & 0x3ffU) >> 6) | (ushort)((y & 0x200U) << 2));
}

void DrawSync(int)
{
    MissingCode();
}

int VSync(int)
{
    MissingCode();
    return 0;
}

sTag* ClearOTagR(std::vector<sTag>& ot, int n) {
    std::vector<sTag>::iterator pCurrent = ot.begin();
    for (int i = 0; i < n - 1; i++)
    {
        pCurrent++;
        pCurrent->m0_pNext = &(pCurrent[-1]);
        pCurrent->m3_size = 0;
    }

    TermPrim(&ot[0]);

    return &ot[0];
}

sTag* ClearOTagR(sTag* ot, int n)
{
    sTag* pCurrent = ot;
    for (int i = 0; i < n - 1; i++)
    {
        pCurrent++;
        pCurrent->m0_pNext = pCurrent - 1;
        pCurrent->m3_size = 0;
    }

    TermPrim(ot);

    return ot;
}

void DrawOTag(sTag* ot)
{
    updatePSXVram();
    while (ot)
    {
        ot->execute();
        ot = ot->m0_pNext;
    }
}

void AddPrim(sTag* ot, sTag* p)
{
    p->m0_pNext = ot->m0_pNext;
    ot->m0_pNext = p;
}