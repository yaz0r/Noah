#pragma once

#include <cstdint>
#include <assert.h>
#include <string>
#include <vector>

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

u16 READ_LE_U16(const void* ptr);
s16 READ_LE_S16(const void* ptr);
u32 READ_LE_U32(const void* ptr);
s32 READ_LE_S32(const void* ptr);

u8 READ_LE_U8(std::vector<u8>::const_iterator& inputStream);
s8 READ_LE_S8(std::vector<u8>::const_iterator& inputStream);
u16 READ_LE_U16(std::vector<u8>::const_iterator& inputStream);
s16 READ_LE_S16(std::vector<u8>::const_iterator& inputStream);
u32 READ_LE_U32(std::vector<u8>::const_iterator& inputStream);
s32 READ_LE_S32(std::vector<u8>::const_iterator& inputStream);

#define MissingCode()
