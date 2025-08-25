#pragma once

enum fieldValidationContexts {
    FCT_Base = 0,
    FCT_Init,
    FCT_MoveCheck,
    FCT_Script,
    FCT_Rendering,
    FCT_KernelSpriteVM, // TODO: move that to kernel
    
    FCT_Undefined,
};

bool isFieldValidationContextEnabled(fieldValidationContexts context);
void enableFieldValidationContext(fieldValidationContexts context);
void disableFieldValidationContext(fieldValidationContexts context);

void validateField();
void validateField_init();
void validateField_shutdown();

#define VALIDATE_FIELD(context, x) \
    if (g_gdbConnection && isFieldValidationContextEnabled(context)) \
    { \
        void validateField();\
        g_gdbConnection->executeUntilAddress(x); \
        validateField(); \
    }

#define VALIDATE_REG(context, reg, value) \
    if (g_gdbConnection && isFieldValidationContextEnabled(context)) \
        assert(g_gdbConnection->getRegister(GDBConnection::reg) == value);

void validate(u32, const u16&);
void validate(u32, const s16&);
void validate(u32, const u32&);
void validate(u32, const s32&);
void validate(u32, const VECTOR&);
void validate(u32, const SVECTOR&);
void validate(u32, const FP_VEC3&);
void validate(u32, const sGTE_XY&);
void validate(u32, const MATRIX&);
void validate(u32, const long&);

void validateValue(u32, const sGTE_XY&);

template <typename T>
void validate(u32 psxBase, const T* pPtr);

template <typename T>
size_t GetPSXSize();

template <typename T, size_t N>
void validate(u32 psxBase, const std::array<T, N>& array) {
    if constexpr (!std::is_compound<T>()) {
        for (int i = 0; i < N; i++) {
            validate(psxBase + i * sizeof(T), array[i]);
        }
    }
    else {
        for (int i = 0; i < N; i++) {
            validate(psxBase + i * (u32)GetPSXSize<T>(), array[i]);
        }
    }
}

#define VALIDATE_REG_PTR(context, reg, value) \
    if (g_gdbConnection && isFieldValidationContextEnabled(context)) \
        validate(g_gdbConnection->getRegister(GDBConnection::reg), value);

#define VALIDATE_XY(context, reg, value) \
    if (g_gdbConnection && isFieldValidationContextEnabled(context)) \
        validateValue(g_gdbConnection->getRegister(GDBConnection::reg), value);

#define VALIDATE_STACK_VAR(context, stackOffset, value) \
    if(g_gdbConnection && isFieldValidationContextEnabled(context)) \
        validate(g_gdbConnection->getRegister(GDBConnection::REG_Names::SP) + stackOffset, value);