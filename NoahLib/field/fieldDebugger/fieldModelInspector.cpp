#include "noahLib.h"
#include "fieldModelInspector.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "imguiBGFX.h"

#include "field/walkMesh.h"

#include "SDL_scancode.h"
#include "SDL_keyboard.h"
#include "field/fieldModel.h"

int fieldModelInspector_bgfxView = 2;

bgfx::FrameBufferHandle fieldModelInspector_FB = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldModelInspector_Texture = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldModelInspector_Depth = BGFX_INVALID_HANDLE;

void fieldModelInspector_step(int modelId)
{
    static ImVec2 oldWindowSize = { -1,-1 };
    //if (ImGui::Begin("Model 3d view"))
    {
        ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();

        if ((currentWindowSize[0] != oldWindowSize[0]) || (currentWindowSize[1] != oldWindowSize[1]))
        {
            currentWindowSize[0] = std::max<int>(currentWindowSize[0], 1);
            currentWindowSize[1] = std::max<int>(currentWindowSize[1], 1);

            oldWindowSize = currentWindowSize;

            if (bgfx::isValid(fieldModelInspector_FB))
            {
                bgfx::destroy(fieldModelInspector_FB);
            }

            const uint64_t tsFlags = 0
                //| BGFX_SAMPLER_MIN_POINT
                //| BGFX_SAMPLER_MAG_POINT
                //| BGFX_SAMPLER_MIP_POINT
                | BGFX_SAMPLER_U_CLAMP
                | BGFX_SAMPLER_V_CLAMP
                ;

            fieldModelInspector_Texture = bgfx::createTexture2D(currentWindowSize[0], currentWindowSize[1], false, 0, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | tsFlags);
            fieldModelInspector_Depth = bgfx::createTexture2D(currentWindowSize[0], currentWindowSize[1], false, 0, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT | tsFlags);
            std::array<bgfx::Attachment, 2> attachements;
            attachements[0].init(fieldModelInspector_Texture);
            attachements[1].init(fieldModelInspector_Depth);
            fieldModelInspector_FB = bgfx::createFrameBuffer(2, &attachements[0], true);
        }

        static float mtx_view[16];
        static float mtx_projection[16];

        static float matrix[16];

        static bx::Vec3 matrixTranslation(0, 0, 0);
        static bx::Vec3 matrixRotation(0, 0, 0);
        static bx::Vec3 matrixScale(1, 1, 1);

        ImVec2 imageStart = ImGui::GetCursorScreenPos();

        ImGui::Image(fieldModelInspector_Texture, currentWindowSize);

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

        bgfx::setViewRect(fieldModelInspector_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);
        bgfx::setViewTransform(fieldModelInspector_bgfxView, mtx_view, mtx_projection);
        //bgfx::setTransform(matrix);

        bgfx::setViewFrameBuffer(fieldModelInspector_bgfxView, fieldModelInspector_FB);
        bgfx::setViewRect(fieldModelInspector_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);

        bgfx::setViewClear(fieldModelInspector_bgfxView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 255);

        bgfx::setViewName(fieldModelInspector_bgfxView, "FieldModelDebugger");
        bgfx::setViewMode(fieldModelInspector_bgfxView, bgfx::ViewMode::Sequential);

        bgfx::touch(fieldModelInspector_bgfxView);

        if (gCurrentFieldModels.size() > modelId)
        {
            gCurrentFieldModels[modelId].bgfxRender(fieldModelInspector_bgfxView, nullptr);
        }
    }
    //ImGui::End();
}


