#pragma once

#include "noahLib.h"

struct sVec2_s16
{
    s16 vx;
    s16 vy;

    void set(s16 x, s16 y)
    {
        vx = x;
        vy = y;
    }

    void get(s16& x, s16& y) const
    {
        x = vx;
        y = vy;
    }

    static sVec2_s16 fromValue(s16 x, s16 y)
    {
        sVec2_s16 newValue;
        newValue.set(x, y);
        return newValue;
    }

    s32 asS32() const
    {
        u32 temp = 0;
        temp = vx;
        temp <<= 16;
        temp |= ((u32)vy) & 0xFFFF;
        return temp;
    }

    static sVec2_s16 fromS32(s32 input)
    {
        sVec2_s16 temp;
        temp.vy = input & 0xFFFF;
        input >>= 16;
        temp.vx = input & 0xFFFF;
        return temp;
    }
};

struct MATRIX
{
    std::array<std::array<s16, 3>, 3> m;
    VECTOR t;
};

#include "libgte/gte.h"
