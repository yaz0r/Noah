#pragma once

#include "bgfx/bgfx.h"

struct sDrawCall {
    bgfx::VertexBufferHandle m_vb;
    bgfx::IndexBufferHandle m_ib;
    bgfx::TextureHandle m_texture;
};

class sBgfxRendererData {
public:
    std::vector<sDrawCall> m_drawCalls;
    void bgfxRender(struct sModelBlock* pModelBlock, int viewIndex, float* modelMatrix);
    void buildDrawcall(struct sModelBlock* pModelBlock, int viewIndex);
};

void bgfxRender(struct sModel* pModel, int viewIndex, float* modelMatrix);
extern std::vector<sModel> gCurrentFieldModels;
