#include "noahLib.h"
#include "libgpu.h"

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
    return 1;
}

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h)
{
    (env->disp).x = (short)x;
    (env->disp).y = (short)y;
    (env->disp).w = (short)w;
    (env->screen).x = 0;
    (env->screen).y = 0;
    (env->screen).w = 0;
    (env->screen).h = 0;
    env->isrgb24 = '\0';
    env->isinter = '\0';
    env->pad1 = '\0';
    env->pad0 = '\0';
    (env->disp).h = (short)h;
    return env;
}

DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h)
{
    bool bVar1;
    long lVar2;

    lVar2 = GetVideoMode();
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
    if (lVar2 == 0) {
        bVar1 = h < 0x101;
    }
    else {
        bVar1 = h < 0x121;
    }
    env->dfe = bVar1;
    env->ofs[0] = (short)x;
    env->ofs[1] = (short)y;
    env->tpage = 10;
    env->isbg = '\0';
    return env;
}
