#pragma once

typedef std::vector<u8>::iterator(*seqFunction)(std::vector<u8>::iterator, struct sSoundInstance*, struct sSoundInstanceEvent*);
extern const std::array<seqFunction, 0x80> seqOpcodes;
extern const std::array<s8, 0x80> opcodeSizes;