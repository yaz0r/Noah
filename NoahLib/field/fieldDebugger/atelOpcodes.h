#pragma once

#include <string.h>
#include <array>

struct sOpcodeArg
{
    enum type
    {
        unknown = 0,
        entryPoint,
        jumpLocation,
        immediateU16,
        immediateS16,
        immediateS16OrVar,
        immediateU16OrVar,
        controlByte,
        byte,
        character,
        dialogId,
        varIndex,
        playableCharacterIndex,
        triggerIndex,
    } m_type = unknown;
    std::string m_name;
    u8 m_offset;
    u8 m_mask;
};

struct sOpcodeInfo
{
    bool m_isInitialized = false;
    u8 m_opcodeSize = 1;
    bool m_breakFlow = false;
    std::string m_name;
    std::string m_comment;
    std::vector<sOpcodeArg> m_args;

    void end()
    {
        m_isInitialized = true;
    }

    sOpcodeInfo& setName(const char* name)
    {
        m_name = name;
        return *this;
    }

    sOpcodeInfo& setComment(const char* comment)
    {
        m_comment = comment;
        return *this;
    }

    sOpcodeArg& addAgument(sOpcodeArg::type opcodeType, const char* argumentName = nullptr)
    {
        m_args.push_back(sOpcodeArg());

        sOpcodeArg& newArg = m_args[m_args.size() - 1];

        newArg.m_type = opcodeType;
        newArg.m_offset = m_opcodeSize;
        if (argumentName)
        {
            newArg.m_name = argumentName;
        }

        return newArg;
    }

    sOpcodeInfo& addArgumentJumpLocation(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::jumpLocation, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentFunctionEntryPoint(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::entryPoint, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentU16(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateU16, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentS16OrVar(const u8 mask, const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateS16OrVar, argumentName).m_mask = mask;
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentS16(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateS16, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentU16OrVar(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::immediateU16OrVar, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addSignControlByte()
    {
        addAgument(sOpcodeArg::controlByte);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentDialogId(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::dialogId, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentByte(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::byte, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentCharacter(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::character, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentVarIndex(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::varIndex, argumentName);
        m_opcodeSize += 0x2;
        return *this;
    }

    sOpcodeInfo& addArgumentPC(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::playableCharacterIndex, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& addArgumentOffsetToQuadCoordinates(const char* argumentName = nullptr)
    {
        addAgument(sOpcodeArg::triggerIndex, argumentName);
        m_opcodeSize += 0x1;
        return *this;
    }

    sOpcodeInfo& setOpcodeBreakExecutionFlow()
    {
        m_breakFlow = true;
        return *this;
    }

};

extern std::array<sOpcodeInfo, 256> m_opcode;
extern std::array<sOpcodeInfo, 256> m_extendedOpcode;

void initOpcodeTable();
void initExtendedOpcodeTable();
