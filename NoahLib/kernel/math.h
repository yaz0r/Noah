#pragma once

#include <array>

struct sFP1616
{
    u32 value;

    void set(s16 x, s16 y)
    {
        value = ((u16)x) * 0x10000 + (u16)y;
    }

    void get(s16& x, s16& y)
    {
        x = (s16)(value >> 16);
        y = (s16)(value & 0xFFFF);
    }
};

struct MATRIX
{
    std::array<std::array<s16, 3>, 3> m;
    std::array<s32, 3> t;
};

struct sVec3
{
    s32 vx;
    s32 vy;
    s32 vz;
};

struct sVec2_s16
{
    s32 vx;
    s32 vy;
};

typedef MATRIX sMatrix;
