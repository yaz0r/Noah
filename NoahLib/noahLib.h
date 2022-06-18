#pragma once

#include <cstdint>
#include <assert.h>
#include <string>
#include <vector>
#include <array>
#include <cstring>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef u8 byte;
typedef u16 ushort;
typedef u32 uint;

struct sPS1Pointer
{
public:
    void setPointer(u8* pointer)
    {
        m_pointer = pointer;
    }
    u8* getPointer() const
    {
        return m_pointer;
    }

    bool operator == (const sPS1Pointer& other) {
        return (other.m_pointer == m_pointer);
    }

    sPS1Pointer operator + (size_t i) const
    {
        sPS1Pointer newPtr = *this;
        newPtr.m_pointer += i;
        return newPtr;
    }
	sPS1Pointer& operator += (size_t i)
	{
		m_pointer += i;
		return *this;
	}

    void makeNull()
    {
        m_pointer = nullptr;
    }
private:
    u8* m_pointer = nullptr;
};

u8 READ_LE_U8(const sPS1Pointer&);
s8 READ_LE_S8(const sPS1Pointer&);
u16 READ_LE_U16(const sPS1Pointer&);
s16 READ_LE_S16(const sPS1Pointer&);
u32 READ_LE_U32(const sPS1Pointer&);
s32 READ_LE_S32(const sPS1Pointer&);

u8 READ_LE_U8(const void* ptr);
s8 READ_LE_S8(const void* ptr);
u16 READ_LE_U16(const void* ptr);
s16 READ_LE_S16(const void* ptr);
u32 READ_LE_U32(const void* ptr);
s32 READ_LE_S32(const void* ptr);

u8 READ_LE_U8(const std::vector<u8>::const_iterator& inputStream);
s8 READ_LE_S8(const std::vector<u8>::const_iterator& inputStream);
u16 READ_LE_U16(const std::vector<u8>::const_iterator& inputStream);
s16 READ_LE_S16(const std::vector<u8>::const_iterator& inputStream);
u32 READ_LE_U32(const std::vector<u8>::const_iterator& inputStream);
s32 READ_LE_S32(const std::vector<u8>::const_iterator& inputStream);

void noahFrame_start();
bool noahFrame_end();

struct sFixedPoint
{
    s32 m_value;
    operator const s32&() const { return m_value; }
    operator s32& () { return m_value; }
    sFixedPoint& operator=(s32 value) { m_value = value; return *this; }

    s32 getIntegerPart() { return m_value >> 16; }
    static sFixedPoint fromValue(s32 value) {
        sFixedPoint temp;
        temp.m_value = value;
        return temp;
    }
};

struct FP_VEC3
{
    sFixedPoint vx;
    sFixedPoint vy;
    sFixedPoint vz;

    sFixedPoint operator[](size_t idx) const
    {
        switch (idx)
        {
        case 0:
            return vx;
        case 1:
            return vy;
        case 2:
            return vz;
        default:
            assert(0);
            return sFixedPoint::fromValue(0);
        }
    }

    sFixedPoint& operator[](size_t idx)
    {
        switch (idx)
        {
        case 0:
            return vx;
        case 1:
            return vy;
        case 2:
            return vz;
        default:
            assert(0);
            return vx;
        }
    }
};

struct FP_VEC4 : public FP_VEC3
{
    s32 pad;
};

struct SFP_VEC3
{
    s16 vx;
    s16 vy;
    s16 vz;
};

struct SFP_VEC4 : public SFP_VEC3
{
    s16 pad;

    static SFP_VEC4 FromIt(u8* it)
    {
        SFP_VEC4 temp;
        temp.vx = READ_LE_S16(it); it += 2;
        temp.vy = READ_LE_S16(it); it += 2;
        temp.vz = READ_LE_S16(it); it += 2;
        return temp;
    }

    static SFP_VEC4 FromIt(std::vector<u8>::iterator it)
    {
        SFP_VEC4 temp;
        temp.vx = READ_LE_S16(it); it += 2;
        temp.vy = READ_LE_S16(it); it += 2;
        temp.vz = READ_LE_S16(it); it += 2;
        return temp;
    }

    s16 operator[](size_t idx) const
    {
        switch (idx)
        {
        case 0:
            return vx;
        case 1:
            return vy;
        case 2:
            return vz;
        default:
            assert(0);
            return pad;
        }
    }

    s16& operator[](size_t idx)
    {
        switch (idx)
        {
        case 0:
            return vx;
        case 1:
            return vy;
        case 2:
            return vz;
        case 3:
            return pad;
        default:
            assert(0);
            return pad;
        }
    }
};

typedef FP_VEC4 VECTOR;
typedef SFP_VEC4 SVECTOR;

void MissingCode(const char* arg = "");
void Hack(const char* arg = "");

#define trap(x) assert(0)

