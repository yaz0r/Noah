#pragma once

void decompress(std::vector<u8>::iterator& inputStream, std::vector<u8>& output);
void fieldDecompress(int size, std::vector<u8>::iterator& inputStream, std::vector<u8>& output);
