#include "noahLib.h"
#include "battleDebug.h"
#include "battle.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "imguiBGFX.h"
#include <SDL_scancode.h>
#include <SDL_keyboard.h>
#include "field/fieldModel.h"
#include "kernel/3dModel_bgfxRenderer.h"

static ImVec2 oldWindowSize = { -1,-1 };

static int fieldDebugger_bgfxView = 2;


void battleDebugView_frame() {
    if (environmentModelBlocks == nullptr)
        return;

    static bgfx::FrameBufferHandle battleDebugger_FB = BGFX_INVALID_HANDLE;
    static bgfx::TextureHandle fieldDebugger_Texture = BGFX_INVALID_HANDLE;
    static bgfx::TextureHandle fieldDebugger_Depth = BGFX_INVALID_HANDLE;

    if (ImGui::Begin("Battle scene"))
    {
        ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();

        currentWindowSize[0] = std::max<int>(currentWindowSize[0], 1);
        currentWindowSize[1] = std::max<int>(currentWindowSize[1], 1);

        if ((currentWindowSize[0] != oldWindowSize[0]) || (currentWindowSize[1] != oldWindowSize[1]))
        {
            oldWindowSize = currentWindowSize;

            if (bgfx::isValid(battleDebugger_FB))
            {
                bgfx::destroy(battleDebugger_FB);
            }

            const uint64_t tsFlags = 0
                //| BGFX_SAMPLER_MIN_POINT
                //| BGFX_SAMPLER_MAG_POINT
                //| BGFX_SAMPLER_MIP_POINT
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
                ;

            fieldDebugger_Texture = bgfx::createTexture2D(currentWindowSize[0], currentWindowSize[1], false, 0, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | tsFlags);
            fieldDebugger_Depth = bgfx::createTexture2D(currentWindowSize[0], currentWindowSize[1], false, 0, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT | tsFlags);
            std::array<bgfx::Attachment, 2> attachements;
            attachements[0].init(fieldDebugger_Texture);
            attachements[1].init(fieldDebugger_Depth);
            battleDebugger_FB = bgfx::createFrameBuffer(2, &attachements[0], true);
        }

        static float mtx_view[16];
        static float mtx_projection[16];

        static float matrix[16];

        static bx::Vec3 matrixTranslation(0, 0, 0);
        static bx::Vec3 matrixRotation(0, 0, 0);
        static bx::Vec3 matrixScale(1, 1, 1);

        ImGui::Image(fieldDebugger_Texture, currentWindowSize);

        {
            float rotationMatrix[16];
            bx::mtxRotateXYZ(rotationMatrix, matrixRotation.x, matrixRotation.y, matrixRotation.z);

            bx::Vec3 vAt = bx::mul(bx::Vec3(0, 0, -1), rotationMatrix);
            vAt = bx::add(vAt, matrixTranslation);

            bx::Vec3 vUp = bx::mul(bx::Vec3(0.f, -1.f, 0.f), rotationMatrix);

            bx::mtxLookAt(mtx_view, matrixTranslation, vAt, vUp);

            ImGuiIO& io = ImGui::GetIO();

            if (ImGui::IsItemHovered() && io.MouseDown[1])
            {
                if (io.MouseDown[1])
                {
                    matrixRotation.x += io.MouseDelta[1] / 256.f;
                    matrixRotation.y -= io.MouseDelta[0] / 256.f;
                }

                bx::Vec3 translationVector(0, 0, 0);

                const bool* keyState = SDL_GetKeyboardState(NULL);

                for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
                {
                    if (keyState[i])
                    {
                        switch (i)
                        {
                        case SDL_SCANCODE_A:
                            translationVector.x -= 10;
                            break;
                        case SDL_SCANCODE_D:
                            translationVector.x += 10;
                            break;
                        case SDL_SCANCODE_Q:
                            translationVector.y += 10;
                            break;
                        case SDL_SCANCODE_E:
                            translationVector.y -= 10;
                            break;
                        case SDL_SCANCODE_W:
                            translationVector.z += 10;
                            break;
                        case SDL_SCANCODE_S:
                            translationVector.z -= 10;
                            break;
                        default:
                            break;
                        }
                    }
                }

                matrixTranslation = bx::add(matrixTranslation, bx::mul(translationVector, rotationMatrix));
            }
        }

        const float camFovy = 60.0f;
        const float camAspect = float(int32_t(currentWindowSize[0])) / float(int32_t(currentWindowSize[1]));
        const float camNear = 0.1f;
        const float camFar = 20000.0f;
        const float projHeight = bx::tan(bx::toRad(camFovy) * 0.5f);
        const float projWidth = projHeight * camAspect;

        const bgfx::Caps* caps = bgfx::getCaps();
        bx::mtxProj(mtx_projection, camFovy, camAspect, camNear, camFar, caps->homogeneousDepth, bx::Handedness::Right);

        ImVec2 imageStart = ImGui::GetCursorScreenPos();

        bgfx::setViewRect(fieldDebugger_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);
        bgfx::setViewTransform(fieldDebugger_bgfxView, mtx_view, mtx_projection);
        //bgfx::setTransform(matrix);

        bgfx::setViewFrameBuffer(fieldDebugger_bgfxView, battleDebugger_FB);
        bgfx::setViewRect(fieldDebugger_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);

        bgfx::setViewClear(fieldDebugger_bgfxView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 255);

        bgfx::setViewName(fieldDebugger_bgfxView, "FieldDebugger");
        bgfx::setViewMode(fieldDebugger_bgfxView, bgfx::ViewMode::Sequential);

        bgfx::touch(fieldDebugger_bgfxView);

        //renderMechasForDebugFieldRenderer(fieldDebugger_bgfxView);

        for (int i=0; i< environmentModelBlocks->m0.size(); i++)
        {
            environmentModelBlocks->m0[i]->getBgfxData()->bgfxRender(environmentModelBlocks->m0[i], fieldDebugger_bgfxView, nullptr);
        }

        /*
        {
            std::vector<sLoadedMecha_sub4>::iterator m4It = environmentModelBlocks->m0.begin();
            int numBones = m4It->mA_numBones;
            m4It++;
            for (int i = 1; i < numBones; i++) {
                if ((m4It->m8 != -1) && (m4It->m7)) {
                    float finalMatrix[16];
                    convertMatrixToBgfx(&m4It->m2C, finalMatrix);
                    pMecha->m0->m0[m4It->m8]->bgfxRender(fieldDebugger_bgfxView, finalMatrix);
                }
                m4It++;
            }
        }
        */
    }
    ImGui::End();
}