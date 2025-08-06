#pragma once

#include <functional>

template< class ReturnType, class... Args >
class interceptor {
public:
    typedef std::function<ReturnType(Args...)> functionSignature;
    interceptor(functionSignature originalFunction, functionSignature interceptedFunction) :
        m_originalFunction(originalFunction),
        m_interceptedFunction(interceptedFunction)
    {

    }

    typedef ReturnType(* const* targetType)(Args...);

    u8* unpackFunction(functionSignature& functionToUnpack) {
        auto target = functionToUnpack.target<ReturnType(*)(Args...)>();
        u8* ptr = (u8*)*target;
        // In case of edit and continue
        if (*ptr == 0xE9) {
            s32 offset = *(s32*)(ptr + 1);
            ptr += offset + 5;
        }
        return ptr;
    }

    void enable() {
        if (!m_enabled) {
            u8* originalFunctionPtr = unpackFunction(m_originalFunction);
            u8* detourFunctionPtr = unpackFunction(m_interceptedFunction);

            DWORD old_flags;
            VirtualProtect(originalFunctionPtr, m_savedBytes.size(), PAGE_EXECUTE_READWRITE, &old_flags);

            memcpy(m_savedBytes.data(), originalFunctionPtr, m_savedBytes.size());
            std::array<u8, 6> call = { 0xFF, 0x25, 0x0, 0x0, 0x0, 0x0 }; // call the the address immediately following
            memcpy(originalFunctionPtr, call.data(), call.size());
            memcpy(originalFunctionPtr + call.size(), &detourFunctionPtr, sizeof(uintptr_t));

            VirtualProtect(originalFunctionPtr, m_savedBytes.size(), old_flags, &old_flags);

            m_enabled = true;
        }
    }
    void disable() {
        if (m_enabled) {
            u8* originalFunctionPtr = unpackFunction(m_originalFunction);

            DWORD old_flags;
            VirtualProtect(originalFunctionPtr, m_savedBytes.size(), PAGE_EXECUTE_READWRITE, &old_flags);

            memcpy(originalFunctionPtr, m_savedBytes.data(), m_savedBytes.size());

            VirtualProtect(originalFunctionPtr, m_savedBytes.size(), old_flags, &old_flags);

            m_enabled = false;
        }
    }
    bool m_enabled = false;

    ReturnType callUndetoured(Args... args) {
        bool wasEnabled = m_enabled;
        assert(wasEnabled);
        if constexpr (std::is_void_v<ReturnType>) {
            disable();
            m_originalFunction(std::forward<Args>(args)...);
            enable();
        }
        else {
            disable();
            ReturnType returnValue = m_originalFunction(std::forward<Args>(args)...);
            enable();
            return returnValue;
        }
    }

    std::array<u8, 16> m_savedBytes;

    functionSignature m_originalFunction;
    functionSignature m_interceptedFunction;
};
