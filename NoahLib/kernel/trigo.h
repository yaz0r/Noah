#pragma once

extern const std::array<s16, 1025> atanTable;
extern const std::array<u16, 8192> rcossin_tbl;
int getAngleSin(u32 param_1);
int getAngleCos(u32 param_1);
int fp_atan2(int* param_1);
int ratan2(int x, int y);

