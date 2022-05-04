#pragma once

typedef void (*tOpcode)();
extern std::array<tOpcode, 256> fieldScriptOpcodes;
extern std::array<tOpcode, 256> fieldScriptOpcodes_EX;

