#include "noahLib.h"
#include "fieldViewDebug.h"

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "imguiBGFX.h"

#include "field/walkMesh.h"

#include "SDL_scancode.h"
#include "SDL_keyboard.h"
#include "field/field.h"
#include "field/mecha/mechaOverlay.h"
#include "kernel/3dModel.h"
#include "kernel/3dModel_bgfxRenderer.h"

#include <tgmath.h>

int fieldDebugger_bgfxView = 1;

bgfx::FrameBufferHandle fieldDebugger_FB = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldDebugger_Texture = BGFX_INVALID_HANDLE;
bgfx::TextureHandle fieldDebugger_Depth = BGFX_INVALID_HANDLE;

ImVec2 oldWindowSize = { -1,-1 };

bgfx::ProgramHandle getLineShader();

void drawLine(int viewIndex, const std::array<s16, 3>& v0, const std::array<s16, 3>& v1)
{
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, 2, &indexBuffer, 2);

        struct sVertice
        {
            float v[3];
            float color[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i = 0; i < 2; i++)
        {
            pVertices[i].color[0] = 0.f;
            pVertices[i].color[1] = 1.f;
            pVertices[i].color[2] = 1.f;
            pVertices[i].color[3] = 1.f;
        }

        float halfScale = 10;

        pVertices[0].v[0] = v0[0];
        pVertices[0].v[1] = v0[1];
        pVertices[0].v[2] = v0[2];

        pVertices[1].v[0] = v1[0];
        pVertices[1].v[1] = v1[1];
        pVertices[1].v[2] = v1[2];

        pIndices[0] = 0;
        pIndices[1] = 1;

        bgfx::setState(0 | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LEQUAL
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_LINES
        );

        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);

        bgfx::submit(viewIndex, getLineShader());
    }
}

void drawCube(int viewIndex, float* modelMatrix)
{
    bgfx::setTransform(modelMatrix);
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .end();

        bgfx::TransientVertexBuffer vertexBuffer;
        bgfx::TransientIndexBuffer indexBuffer;
        bgfx::allocTransientBuffers(&vertexBuffer, layout, 8, &indexBuffer, 8 * 3);

        struct sVertice
        {
            float v[3];
            float color[4];
        };

        sVertice* pVertices = (sVertice*)vertexBuffer.data;
        u16* pIndices = (u16*)indexBuffer.data;

        for (int i=0; i<8; i++)
        {
            pVertices[i].color[0] = 1.f;
            pVertices[i].color[1] = 1.f;
            pVertices[i].color[2] = 0.f;
            pVertices[i].color[3] = 1.f;
        }

        float halfScale = 10;

        pVertices[0].v[0] = -halfScale;
        pVertices[0].v[1] = 0;
        pVertices[0].v[2] = -halfScale;

        pVertices[1].v[0] = halfScale;
        pVertices[1].v[1] = 0;
        pVertices[1].v[2] = -halfScale;

        pVertices[2].v[0] = halfScale;
        pVertices[2].v[1] = 0;
        pVertices[2].v[2] = halfScale;

        pVertices[3].v[0] = -halfScale;
        pVertices[3].v[1] = 0;
        pVertices[3].v[2] = halfScale;

        pVertices[4].v[0] = -halfScale;
        pVertices[4].v[1] = -2 * halfScale;
        pVertices[4].v[2] = -halfScale;

        pVertices[5].v[0] = halfScale;
        pVertices[5].v[1] = -2 * halfScale;
        pVertices[5].v[2] = -halfScale;

        pVertices[6].v[0] = halfScale;
        pVertices[6].v[1] = -2 * halfScale;
        pVertices[6].v[2] = halfScale;

        pVertices[7].v[0] = -halfScale;
        pVertices[7].v[1] = -2 * halfScale;
        pVertices[7].v[2] = halfScale;

        pIndices[0] = 0;
        pIndices[1] = 1;
        pIndices[2] = 1;
        pIndices[3] = 2;
        pIndices[4] = 2;
        pIndices[5] = 3;
        pIndices[6] = 3;
        pIndices[7] = 0;

        pIndices[8+0] = 4+0;
        pIndices[8+1] = 4+1;
        pIndices[8+2] = 4+1;
        pIndices[8+3] = 4+2;
        pIndices[8+4] = 4+2;
        pIndices[8+5] = 4+3;
        pIndices[8+6] = 4+3;
        pIndices[8+7] = 4+0;

        pIndices[16 + 0] = 0;
        pIndices[16 + 1] = 4 + 0;
        pIndices[16 + 2] = 1;
        pIndices[16 + 3] = 4 + 1;
        pIndices[16 + 4] = 2;
        pIndices[16 + 5] = 4 + 2;
        pIndices[16 + 6] = 3;
        pIndices[16 + 7] = 4 + 3;

        bgfx::setState(0 | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LEQUAL
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_LINES
        );

        bgfx::setVertexBuffer(0, &vertexBuffer);
        bgfx::setIndexBuffer(&indexBuffer);

        bgfx::submit(viewIndex, getLineShader());
    }
}

void fieldViewDebug_step()
{
    // TODO: Move to a separate file
    if (ImGui::Begin("Field encounter data"))
    {
        ImGui::Checkbox("Disable encounters", &debugEncounterTriggerDisabled);
        ImGui::Text("Timer: %d\nCount: %d", encounterTimer, encounterCount);
        
        ImGui::Text("playMusicAuthorized: %d\nfieldExecuteVar3: %d\nfieldChangePrevented: %d", playMusicAuthorized, fieldExecuteVar3, fieldChangePrevented);
        ImGui::Text("fieldMusicLoadPending: %d\nfieldRandomBattleVar: %d\nloadCompleted: %d", fieldMusicLoadPending, fieldRandomBattleVar, loadCompleted);
                
        ImGui::Text("Encounter timers:");

        for (int encounterId = 0; encounterId < encounterCount; encounterId++)
        {
            ImGui::Text("%d", encounterTriggerTime[encounterId]);
            ImGui::SameLine();
            if (ImGui::Button("Trigger")){
                encounterTriggerTime[encounterId] = 0;
            }
        }

        ImGui::Text("Encounter probabilities:");

        float total = 0;
        for (int i = 0; i < 0x10; i++) {
            total += encounterProbabilityWeight[i];
        }

        if (total > 0) {
            for (int i = 0; i < 0x10; i++) {
                ImGui::Text("%d | %d%%", encounterProbabilityWeight[i], (int)round(encounterProbabilityWeight[i] / total * 100));
                ImGui::SameLine();
                if (ImGui::Button("Trigger")) {
                    debugForcedEncounter = i;
                }
            }
        }
    }
    ImGui::End();

    if (ImGui::Begin("Field 3d view"))
    {
        ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();

        currentWindowSize[0] = std::max<int>(currentWindowSize[0], 1);
        currentWindowSize[1] = std::max<int>(currentWindowSize[1], 1);

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

        for (int i=0; i<fieldTriggerData.size(); i++)
        {
            drawLine(fieldDebugger_bgfxView, fieldTriggerData[i].m0, fieldTriggerData[i].m6);
            drawLine(fieldDebugger_bgfxView, fieldTriggerData[i].m6, fieldTriggerData[i].mC);
            drawLine(fieldDebugger_bgfxView, fieldTriggerData[i].mC, fieldTriggerData[i].m12);
            drawLine(fieldDebugger_bgfxView, fieldTriggerData[i].m12, fieldTriggerData[i].m0);
        }

        for (int i=0; i<actorArray.size(); i++)
        {
            float rot[3];
            rot[0] = bx::kPi * -actorArray[i].m50_modelRotation.vx / (float)(0x800);
            rot[1] = bx::kPi * -actorArray[i].m50_modelRotation.vy / (float)(0x800);
            rot[2] = bx::kPi * -actorArray[i].m50_modelRotation.vz / (float)(0x800);

            float rotationMatrix[16];
            bx::mtxRotateXYZ(rotationMatrix, rot[0], rot[1], rot[2]);

            float position[3];
            if (actorArray[i].m4C_scriptEntity)
            {
                position[0] = actorArray[i].m4C_scriptEntity->m20_position.vx >> 16;
                position[1] = actorArray[i].m4C_scriptEntity->m20_position.vy >> 16;
                position[2] = actorArray[i].m4C_scriptEntity->m20_position.vz >> 16;
            }
            else
            {
                position[0] = actorArray[i].mC_matrix.t[0];
                position[1] = actorArray[i].mC_matrix.t[1];
                position[2] = actorArray[i].mC_matrix.t[2];
            }

            float finalMatrix[16];
            bx::mtxSRT(finalMatrix, 1, 1, 1, rot[0], rot[1], rot[2], position[0], position[1], position[2]);

            if (!(actorArray[i].m58_flags & 0x40))
            {
                actorArray[i].m0->m4_pModelBlock->getBgfxData()->bgfxRender(&(*actorArray[i].m0->m4_pModelBlock), fieldDebugger_bgfxView, finalMatrix);
            }
            else
            {
                drawCube(fieldDebugger_bgfxView, finalMatrix);
            }
        }

        renderMechasForDebugFieldRenderer(fieldDebugger_bgfxView);
    }
    ImGui::End();
}
