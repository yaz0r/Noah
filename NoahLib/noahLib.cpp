#include "noahLib.h"
#include "bgfxGlue.h"
#include "imgui.h"

bool noahInit(int argc, char* argv[])
{
    initBgfxGlue(argc, argv);
    return true;
}

bool noahFrame()
{
    StartFrame();

    ImGui::Begin("Test");
    ImGui::Text("Testing..");
    ImGui::End();


    EndFrame();

    return gCloseApp;
}