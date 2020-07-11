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

u16 READ_LE_U16(void* ptr);
s16 READ_LE_S16(void* ptr);
u32 READ_LE_U32(void* ptr);
s32 READ_LE_S32(void* ptr);

u8 READ_LE_U8(std::vector<u8>::iterator& inputStream);
u16 READ_LE_U16(std::vector<u8>::iterator& inputStream);
s16 READ_LE_S16(std::vector<u8>::iterator& inputStream);
u32 READ_LE_U32(std::vector<u8>::iterator& inputStream);
