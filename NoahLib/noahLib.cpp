#include "bgfxGlue.h"

#include "bgfxGlue.h"

bool noahInit(int argc, char* argv[])
{
    initBgfxGlue(argc, argv);
    return true;
}

bool noahFrame()
{
    StartFrame();
    EndFrame();

    return gCloseApp;
}