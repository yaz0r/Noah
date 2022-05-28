#pragma once

void decompress(std::vector<u8>::const_iterator inputStream, std::vector<u8>& output, u32 inputSize = UINT32_MAX);
void fieldDecompress(int size, std::vector<u8>::const_iterator inputStream, std::vector<u8>& output, u32 inputSize = UINT32_MAX);
std::vector<u8> mallocAndDecompress(std::vector<u8>::const_iterator input, u32 inputSize = UINT32_MAX);
