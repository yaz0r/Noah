#pragma once

void decompress(std::vector<u8>::const_iterator inputStream, std::vector<u8>& output);
void fieldDecompress(int size, std::vector<u8>::const_iterator inputStream, std::vector<u8>& output);
std::vector<u8> mallocAndDecompress(std::vector<u8>::const_iterator input);
