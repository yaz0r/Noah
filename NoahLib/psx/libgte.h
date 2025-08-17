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
        temp = vy;
        temp <<= 16;
        temp |= ((u32)vx) & 0xFFFF;
        return temp;
    }

    static sVec2_s16 fromS32(s32 input)
    {
        sVec2_s16 temp;
        temp.vx = input & 0xFFFF;
        input >>= 16;
        temp.vy = input & 0xFFFF;
        return temp;
    }
};

struct sGTE_XY {
    sGTE_XY() {
        m_value = 0;
    }
    sGTE_XY(s16 x, s16 y) {
        set(x, y);
    }
    void set(s16 x, s16 y) {
        // This is kinda buggy, but it's how it is in the PSX original code
        // if y is negative, it can overflow and be added to the x part
        m_value = (s32)x * 0x10000 + (s32)y; 
    }
    u32 getRawValue() const {
        return m_value;
    }
    s16 getX() const {
        return (s16)(m_value >> 16);
    }
    s16 getY() const {
        return (s16)(m_value);
    }
private:
    u32 m_value = 0;
};

struct MATRIX
{
    std::array<std::array<s16, 3>, 3> m;
    VECTOR t;
};

#include "libgte/gte.h"
