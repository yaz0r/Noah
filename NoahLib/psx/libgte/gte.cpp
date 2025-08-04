#include "noahLib.h"
#include "../libgte.h"
#include "kernel/gte.h"

s32 RotTransPers3(const SVECTOR* v0, const SVECTOR* v1, const SVECTOR* v2, sVec2_s16* p0, sVec2_s16* p1, sVec2_s16* p2, s32* p, s32* flag) {
    gte_ldv3(v0, v1, v2);
    gte_rtpt_b();
    gte_stsxy3(p0, p1, p2);
    gte_stdp(p);
    s32 lVar2 = gte_stFLAG();
    s32 iVar1 = gte_stSZ3();
    *flag = lVar2;
    return iVar1 >> 2;
}

void InitGeom()
{
    MissingCode();

    setCopControlWord(2, 0xe800, 0x155);
    setCopControlWord(2, 0xf000, 0x100);
    setCopControlWord(2, 0xd000, 1000);
    setCopControlWord(2, 0xd800, 0xffffef9e);
    setCopControlWord(2, 0xe000, 0x1400000);
    setCopControlWord(2, 0xc000, 0);
    setCopControlWord(2, 0xc800, 0);
}

void SetGeomScreen(s32 h)
{
    setCopControlWord(2, 0xd000, h);
}

void SetGeomOffset(int ofx, int ofy)
{
    setCopControlWord(2, 0xc000, ofx << 0x10);
    setCopControlWord(2, 0xc800, ofy << 0x10);
}

s32 ReadGeomScreen() {
    return gte_stH();
}

void ReadGeomOffset(s32* ofx, s32* ofy) {
    *ofx = gte_stOFX() >> 16;
    *ofy = gte_stOFY() >> 16;
}
