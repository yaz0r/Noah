#include "noahLib.h"
#include "bgfx/bgfx.h"
#include "imguiBGFX.h"

bool m_isVramDirty = true;
static bgfx::TextureHandle m_vramTextureHandle = BGFX_INVALID_HANDLE;
std::array<u8, 2048 * 512 * 3> convertedVram;


void fieldInspector_vram()
{
    int textureHeight = 512;
    int textureWidth = 2048;

    extern std::array<u8, 2048 * 512> gVram;
    if (m_isVramDirty)
    {
        if (!bgfx::isValid(m_vramTextureHandle))
        {
            m_vramTextureHandle = bgfx::createTexture2D(textureWidth, textureHeight, false, 1, bgfx::TextureFormat::R8, 0, nullptr);
        }

        bgfx::updateTexture2D(m_vramTextureHandle, 0, 0, 0, 0, textureWidth, textureHeight, bgfx::makeRef(&gVram[0], textureWidth * textureHeight));

#if 0
        if (!bgfx::isValid(m_vramTextureHandle))
        {
            m_vramTextureHandle = bgfx::createTexture2D(textureWidth, textureHeight, false, 1, bgfx::TextureFormat::RGB8, 0, nullptr);
        }

        for (int i = 0; i < 2048; i++)
        {
            for (int j = 0; j < 512; j++)
            {
                u8 color = gVram[j * 2048 + i / 2];
                if (i & 1)
                {
                    color >> 4;
                }
                color &= 0xF;
                color <<= 4;
                convertedVram[(j * 2048 + i) * 3 + 0] = color;
                convertedVram[(j * 2048 + i) * 3 + 1] = color;
                convertedVram[(j * 2048 + i) * 3 + 2] = color;
            }
        }

        bgfx::updateTexture2D(m_vramTextureHandle, 0, 0, 0, 0, textureWidth, textureHeight, bgfx::makeRef(&convertedVram[0], textureWidth * textureHeight * 3));
#endif
    }

    ImGui::Image(m_vramTextureHandle, ImVec2(textureWidth / 2, textureHeight / 2));

}

bool bVramDebugOpen = false;

void vramDebug_frame() {
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Vram", nullptr, &bVramDebugOpen);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (bVramDebugOpen) {
        if (ImGui::Begin("Vram viewer", &bVramDebugOpen)) {
            fieldInspector_vram();
        }
        ImGui::End();
    }
}