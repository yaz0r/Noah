#pragma once

#include "math.h"

void setCopControlWord(int cop, int reg, u32 value);
void setCopControlWord(int cop, int reg, sVec2_s16 value);
void setCopReg(int, int, sVec2_s16);
void setCopReg(int, int, u32);
void copFunction(int, u32);
s32 getCopReg(int, int);
s32 getCopControlWord(int, int);

void gte_rtpt();
void gte_nclip();
void gte_rtps();
void gte_rtv0();
void gte_rtir();
void gte_ldv0(std::vector<u8>::iterator& pVertices);
void gte_ldv0(u8* pVertices);
void gte_ldv0(const SFP_VEC4* pVertices0);
void gte_ldv3(std::vector<u8>::iterator& pVertices0, std::vector<u8>::iterator& pVertices1, std::vector<u8>::iterator& pVertices2);
void gte_ldv3(u8* pVertices0, u8* pVertices1, u8* pVertices2);
void gte_ldv3(const SFP_VEC4* pVertices0, const SFP_VEC4* pVertices1, const SFP_VEC4* pVertices2);
void gte_stlzc(int* pFlag);
void gte_getMAC0(int* pOutput);
void gte_stsxy3(sVec2_s16* xy0, sVec2_s16* xy1, sVec2_s16* xy2);
void gte_stsxy2(sVec2_s16* xy);
void gte_stsz3(int* sz0, int* sz1, int* sz2);
void gte_stsz4(int* sz0, int* sz1, int* sz2, int* sz3);
void gte_stsxy(sVec2_s16* xy);

