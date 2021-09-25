#include "noahLib.h"
#include "graphics.h"
#include "bgfx/bgfx.h"
#include <bx/math.h>
#include "imguiBGFX.h"

bgfx::ProgramHandle getLineShader();

ImVec2 PSXResolution = { 320,240 };

void DrawSync(int)
{
    MissingCode();
}

int VSync(int)
{
    MissingCode();
	return 0;
}

void InitGeom()
{
    MissingCode();
}

void SetPolyFT4(POLY_FT4* p)
{
	p->m3_size = 9;
	p->code = 0x2C;
}

u16 GetTPage(int tp, int abr, int x, int y)
{
	return (u16)((ushort)((tp & 3U) << 7) | (ushort)((abr & 3U) << 5) | (ushort)((int)(y & 0x100U) >> 4) | (ushort)((int)(x & 0x3ffU) >> 6) | (ushort)((y & 0x200U) << 2));
}

void SetTile(TILE* p)
{
	p->m3_size = 3;
	p->code = 0x60;
}

void SetSemiTrans(TILE* p, int abe)
{
	if (abe == 0)
	{
		p->code &= ~2;
	}
	else
	{
		p->code |= 2;
	}
}

void SetSprt(SPRT* p)
{
	p->m3_size = 4;
	p->code = 0x64;
}

void SetSemiTrans(SPRT* p, int abe)
{
	if (abe == 0)
	{
		p->code &= ~2;
	}
	else
	{
		p->code |= 2;
	}
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

u16 GetClut(int x, int y)
{
	return (u16)((u16)(y << 6) | (u16)(x >> 4) & 0x3f);
}

s32 currentVideoMode = 0;

long GetVideoMode(void)

{
	return currentVideoMode;
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

DISPENV currentDispEnv;
DRAWENV currentDrawEnv;

DISPENV* PutDispEnv(DISPENV* env)
{
	MissingCode();

	currentDispEnv = *env;
	return env;

}
DRAWENV* PutDrawEnv(DRAWENV* env)
{
	MissingCode();

	currentDrawEnv = *env;
	return env;
}

sTag gEndTag;

sTag* ClearOTagR(sTag* ot, int n)
{
	sTag* pCurrent = ot;
	for (int i=0; i<n; i++)
	{
		pCurrent++;
		pCurrent->m0_pNext = pCurrent - 1;
		pCurrent->m3_size = 0;
	}

	ot->m0_pNext = &gEndTag;
	ot->m3_size = 0;

	return ot;
}

void DrawOTag(sTag* ot)
{
	while (ot)
	{
		ot->execute();
		ot = ot->m0_pNext;
	}
}

u32 get_mode(int dfe, int dtd, int tpage)
{
	MissingCode();
	return 0;
}

u32 get_tw(RECT* param_1)
{
	uint uVar1;

	if (param_1 == (RECT*)0x0) {
		uVar1 = 0;
	}
	else {
		uVar1 = (uint)(*(byte*)&param_1->y >> 3) << 0xf | (uint)(*(byte*)&param_1->x >> 3) << 10 | 0xe2000000 | ((int)(-(int)param_1->h & 0xffU) >> 3) << 5 | (int)(-(int)param_1->w & 0xffU) >> 3;
	}
	return uVar1;
}

void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw)
{
	p->m3_size = 2;
	p->code[0] = get_mode(dfe, dtd, tpage & 0xffff);
	p->code[1] = get_tw(tw);
}

bgfx::FrameBufferHandle PSXOutput_FB = BGFX_INVALID_HANDLE;
bgfx::TextureHandle PSXOutput_Texture = BGFX_INVALID_HANDLE;
int PSXOutput_bgfxView = 3;

void initPSXFB()
{
	if (!bgfx::isValid(PSXOutput_FB))
	{
		const uint64_t tsFlags = 0
			//| BGFX_SAMPLER_MIN_POINT
			//| BGFX_SAMPLER_MAG_POINT
			//| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		PSXOutput_Texture = bgfx::createTexture2D(PSXResolution[0], PSXResolution[1], false, 0, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT | tsFlags);
		std::array<bgfx::Attachment, 1> attachements;
		attachements[0].init(PSXOutput_Texture);
		PSXOutput_FB = bgfx::createFrameBuffer(1, &attachements[0], true);
	}

	bgfx::setViewRect(PSXOutput_bgfxView, 0, 0, PSXResolution[0], PSXResolution[1]);

	float mtx_view[16];
	float mtx_projection[16];

	bx::mtxIdentity(mtx_view);
	bx::mtxOrtho(mtx_projection, 0, PSXResolution[0], PSXResolution[1], 0, -1, 1, 0, 0);

	bgfx::setViewTransform(PSXOutput_bgfxView, mtx_view, mtx_projection);
	//bgfx::setTransform(matrix);

	bgfx::setViewFrameBuffer(PSXOutput_bgfxView, PSXOutput_FB);
	bgfx::setViewRect(PSXOutput_bgfxView, 0, 0, PSXResolution[0], PSXResolution[1]);

	bgfx::setViewClear(PSXOutput_bgfxView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 255);

	bgfx::setViewName(PSXOutput_bgfxView, "PSX Output");
	bgfx::setViewMode(PSXOutput_bgfxView, bgfx::ViewMode::Sequential);

	bgfx::touch(PSXOutput_bgfxView);
}

void drawPSXFB()
{
	if (ImGui::Begin("PSX Output"))
	{
		ImGui::Image(PSXOutput_Texture, ImVec2(PSXResolution[0]*2, PSXResolution[1]*2));
	}
	ImGui::End();
}

void DR_MODE::execute()
{
}

void DR_ENV::execute()
{
}

void SPRT::execute()
{
	float matrix[16];
	bx::mtxIdentity(matrix);

	bgfx::setTransform(matrix);
	{
		bgfx::VertexLayout layout;
		layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.end();

		bgfx::TransientVertexBuffer vertexBuffer;
		bgfx::TransientIndexBuffer indexBuffer;
		bgfx::allocTransientBuffers(&vertexBuffer, layout, 4, &indexBuffer, 2 * 4);

		struct sVertice
		{
			float v[3];
			float color[4];
		};

		sVertice* pVertices = (sVertice*)vertexBuffer.data;
		u16* pIndices = (u16*)indexBuffer.data;

		for (int i = 0; i < 4; i++)
		{
			pVertices[i].color[0] = 1.f;
			pVertices[i].color[1] = 1.f;
			pVertices[i].color[2] = 0.f;
			pVertices[i].color[3] = 1.f;
		}

		pVertices[0].v[0] = x0;
		pVertices[0].v[1] = y0;
		pVertices[0].v[2] = 0;

		pVertices[1].v[0] = x0 + w;
		pVertices[1].v[1] = y0;
		pVertices[1].v[2] = 0;

		pVertices[2].v[0] = x0 + w;
		pVertices[2].v[1] = y0 + h;
		pVertices[2].v[2] = 0;

		pVertices[3].v[0] = x0;
		pVertices[3].v[1] = y0 + h;
		pVertices[3].v[2] = 0;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 1;
		pIndices[3] = 2;
		pIndices[4] = 2;
		pIndices[5] = 3;
		pIndices[6] = 3;
		pIndices[7] = 0;

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

		bgfx::submit(PSXOutput_bgfxView, getLineShader());
	}

}

void TILE::execute()
{
}

void POLY_FT4::execute()
{
}
