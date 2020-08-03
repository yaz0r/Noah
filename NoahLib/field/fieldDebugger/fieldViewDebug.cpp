#include "noahLib.h"
#include "fieldViewDebug.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "imguiBGFX.h"

#include "field/walkMesh.h"

#include <array>

#include "ImGuizmo.h"
#include "SDL_scancode.h"
#include "field/field.h"

int fieldDebugger_bgfxView = 1;

bgfx::FrameBufferHandle fieldDebugger_FB = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldDebugger_Texture = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldDebugger_Depth = BGFX_INVALID_HANDLE;

ImVec2 oldWindowSize = { -1,-1 };

void fieldViewDebug_step()
{
    if (ImGui::Begin("Field 3d view"))
    {
        ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();

        if ((currentWindowSize[0] != oldWindowSize[0]) || (currentWindowSize[1] != oldWindowSize[1]))
        {
            oldWindowSize = currentWindowSize;

            if (bgfx::isValid(fieldDebugger_FB))
            {
                bgfx::destroy(fieldDebugger_FB);
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
            fieldDebugger_FB = bgfx::createFrameBuffer(2, &attachements[0], true);
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

                if (io.KeysDown[SDL_SCANCODE_A])
                {
                    translationVector.x -= 10;
                }

                if (io.KeysDown[SDL_SCANCODE_D])
                {
                    translationVector.x += 10;
                }

                if (io.KeysDown[SDL_SCANCODE_Q])
                {
                    translationVector.y -= 10;
                }

                if (io.KeysDown[SDL_SCANCODE_E])
                {
                    translationVector.y += 10;
                }

                if (io.KeysDown[SDL_SCANCODE_W])
                {
                    translationVector.z += 10;
                }

                if (io.KeysDown[SDL_SCANCODE_S])
                {
                    translationVector.z -= 10;
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
        bx::mtxProj(mtx_projection, camFovy, camAspect, camNear, camFar, caps->homogeneousDepth, bx::Handness::Right);

        ImVec2 imageStart = ImGui::GetCursorScreenPos();

        // ----------------------------------
        /*
        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
        if (ImGui::IsKeyPressed(90))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(69))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(82)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation, 3);
        ImGui::InputFloat3("Rt", matrixRotation, 3);
        ImGui::InputFloat3("Sc", matrixScale, 3);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        static bool useSnap(false);
        if (ImGui::IsKeyPressed(83))
            useSnap = !useSnap;
        ImGui::Checkbox("", &useSnap);
        ImGui::SameLine();
        /*
        vec_t snap;
        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            snap = config.mSnapTranslation;
            ImGui::InputFloat3("Snap", &snap.x);
            break;
        case ImGuizmo::ROTATE:
            snap = config.mSnapRotation;
            ImGui::InputFloat("Angle Snap", &snap.x);
            break;
        case ImGuizmo::SCALE:
            snap = config.mSnapScale;
            ImGui::InputFloat("Scale Snap", &snap.x);
            break;
        }
        */
        /*
        ImGuizmo::SetRect(imageStart[0], imageStart[1], currentWindowSize[0], currentWindowSize[1]);
        //ImGuizmo::Manipulate(mtx_view, mtx_projection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix.m16, NULL, useSnap ? &snap.x : NULL);

        ImGuizmo::SetDrawlist();
        //ImGuizmo::Manipulate(mtx_view, mtx_projection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, NULL);

        ImGuizmo::ViewManipulate(mtx_view, 1000, ImVec2(imageStart[0] + currentWindowSize[0] - 128, imageStart[1]), ImVec2(128, 128), 0x10101010);

        //ImGuizmo::DrawCube(mtx_view, mtx_projection, matrix);

        //memcpy(mtx_view, matrix, 16 * 4);
        */
        // ----------------------------------

        bgfx::setViewRect(fieldDebugger_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);
        bgfx::setViewTransform(fieldDebugger_bgfxView, mtx_view, mtx_projection);
        //bgfx::setTransform(matrix);

        bgfx::setViewFrameBuffer(fieldDebugger_bgfxView, fieldDebugger_FB);
        bgfx::setViewRect(fieldDebugger_bgfxView, 0, 0, currentWindowSize[0], currentWindowSize[1]);

        bgfx::setViewClear(fieldDebugger_bgfxView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 255);

        bgfx::setViewName(fieldDebugger_bgfxView, "FieldDebugger");
        bgfx::setViewMode(fieldDebugger_bgfxView, bgfx::ViewMode::Sequential);

        bgfx::touch(fieldDebugger_bgfxView);

        walkMesh.bgfxRender(fieldDebugger_bgfxView);

        for (int i=0; i<fieldEntityArray.size(); i++)
        {
            if (!(fieldEntityArray[i].m58_flags & 0x40))
            {
                float translationMatrix[16];
                bx::mtxTranslate(translationMatrix, fieldEntityArray[i].mC_matrix.t[0] / (float)(1 << 16), fieldEntityArray[i].mC_matrix.t[1] / (float)(1 << 16), fieldEntityArray[i].mC_matrix.t[2] / (float)(1 << 16));

                float rot[3];
                rot[0] = bx::kPi * -fieldEntityArray[i].m50_modelRotation[0] / (float)(0x800);
                rot[1] = bx::kPi * -fieldEntityArray[i].m50_modelRotation[1] / (float)(0x800);
                rot[2] = bx::kPi * -fieldEntityArray[i].m50_modelRotation[2] / (float)(0x800);

                float rotationMatrix[16];
                bx::mtxRotateXYZ(rotationMatrix, rot[0], rot[1], rot[2]);

                float finalMatrix[16];
                bx::mtxSRT(finalMatrix, 1, 1, 1, rot[0], rot[1], rot[2], fieldEntityArray[i].mC_matrix.t[0] / (float)(1 << 16), fieldEntityArray[i].mC_matrix.t[1] / (float)(1 << 16), fieldEntityArray[i].mC_matrix.t[2] / (float)(1 << 16));

                fieldEntityArray[i].m0->m4_pModelBlock->bgfxRender(fieldDebugger_bgfxView, finalMatrix);
            }
        }
    }
    ImGui::End();
}
