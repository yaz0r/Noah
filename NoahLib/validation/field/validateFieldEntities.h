#pragma once

void validateFieldEntities();
void validateFieldVars();
void validateWalkMeshBundle();

template <typename T>
void validate(u32 psxBase, const T* pPtr);

void validate(u32 psxBase, const s16&);
void validate(u32 psxBase, const SVECTOR&);
void validate(u32 psxBase, const FP_VEC3&);
void validate(u32 psxBase, const struct sFieldScriptEntity&);

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