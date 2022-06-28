#pragma once

#include "math.h"

void setCopControlWord(int cop, int reg, u32 value);
void setCopControlWord(int cop, int reg, sVec2_s16 value);
void setCopReg(int, int, sVec2_s16);
void setCopReg(int, int, u32);
void copFunction(int, u32);
s32 getCopReg(int, int);
s32 getCopControlWord(int, int);

void gte_SetRotMatrix(const MATRIX* m);
void gte_SetTransMatrix(const MATRIX* m);

void gte_rtpt();
void gte_rt();
void gte_op0();
void gte_nclip();
void gte_rtps();
void gte_rtv0();
void gte_rtir();
void gte_rtir_sf0();
void gte_ldv0(std::vector<u8>::iterator& pVertices);
void gte_ldv0(u8* pVertices);
void gte_ldv0(const SFP_VEC4* pVertices0);
void gte_ldlv0(const VECTOR* pVertices0);
void gte_ldv2(SVECTOR*);
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
void gte_stlvnl(VECTOR* pOutput);
void gte_ldlvl(VECTOR* pInput);

void gte_stsz(s32* output);

void gte_ldVXY0(SVECTOR*);
void gte_ldVZ0(s16*);
void gte_ldVXY1(SVECTOR*);
void gte_ldVZ1(s16*);
void gte_ldVXY2(SVECTOR*);
void gte_ldVZ2(s16*);

void gte_stdp(s32* output);
s32 gte_stSZ3();

s32 gte_stFLAG();
s32 gte_stSXY0();
s32 gte_stSXY1();
s32 gte_stSXY2();

s32 gte_stMAC0();
s32 gte_stIR0();

void SetDQA(s32);
void SetDQB(s32);

void read_sz_fifo3(s16*, s16*, s16*);

#define gte_rtpt_b gte_rtpt