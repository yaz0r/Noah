#pragma once

#include "socket.h"

class GDBConnection {
public:
    bool openConnection(const std::string& server, u16 port);
    void closeConnection();

    std::vector<u8> readMemory(u64 address, size_t size);
    bool writeMemory(u64 address, const void* buffer, size_t size);

    u16 readU16(u64 address);

    void sendMonitorCommand(const std::string& command);
    void resetTarget();
    void setBreakpoint(u64 address, int length = 4);
    void removeBreakpoint(u64 address, int length = 4);

    u32 pauseExecution();
    u32 resumeExecution();
    void executeUntilAddress(u32 address);
    void stepOut();

    struct sRegs {
        union {
            u32 rawRegs[72];
            struct {
                u32 r0;
                u32 at;
                u32 v0;
                u32 v1;
                u32 a0;
                u32 a1;
                u32 a2;
                u32 a3;
                u32 t0;
                u32 t1;
                u32 t2;
                u32 t3;
                u32 t4;
                u32 t5;
                u32 t6;
                u32 t7;
                u32 s0;
                u32 s1;
                u32 s2;
                u32 s3;
                u32 s4;
                u32 s5;
                u32 s6;
                u32 s7;
                u32 s8;
                u32 s9;
                u32 k0;
                u32 k1;
                u32 gp;
                u32 sp;
                u32 fp;
                u32 ra;

                u32 sr;
                u32 lo;
                u32 hi;
                u32 badv;
                u32 cause;
                u32 pc; // 37
            };
        };
    };

    enum REG_Names {
        V0 = 2,
        A0 = 4,
        RA = 31,
        PC = 37,
    };

    void getRegisters(sRegs& outputRegs);
    u32 getRegister(int index);
    void setRegister(int index, u32 value);
private:
    hSocket m_socket;
};
