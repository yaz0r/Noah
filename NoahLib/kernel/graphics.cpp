#include "noahLib.h"
#include "graphics.h"

void DrawSync(int)
{
    MissingCode();
}

void VSync(int)
{
    MissingCode();
}

void InitGeom()
{
    MissingCode();
}

void SetPolyFT4(POLY_FT4* p)
{
	p->tag.m3_size = 9;
	p->code = 0x2C;
}

u16 GetTPage(int tp, int abr, int x, int y)
{
	return (u16)((ushort)((tp & 3U) << 7) | (ushort)((abr & 3U) << 5) | (ushort)((int)(y & 0x100U) >> 4) | (ushort)((int)(x & 0x3ffU) >> 6) | (ushort)((y & 0x200U) << 2));
}

void SetTile(TILE* p)
{
	p->tag.m3_size = 3;
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
	p->tag.m3_size = 4;
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

sTag gEndTag = { nullptr, 0 };

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
	p->tag.m3_size = 2;
	p->code[0] = get_mode(dfe, dtd, tpage & 0xffff);
	p->code[1] = get_tw(tw);
}

