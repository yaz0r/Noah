#pragma once

s32 RotTransPers3(const SVECTOR* v0, const SVECTOR* v1, const SVECTOR* v2, sVec2_s16* p0, sVec2_s16* p1, sVec2_s16* p2, s32* p, s32* flag);

void InitGeom();

void SetGeomScreen(s32);
void SetGeomOffset(s32, s32);
s32 ReadGeomScreen();
void ReadGeomOffset(s32*, s32*);
