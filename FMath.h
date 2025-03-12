#pragma once
const float PI = 3.141592654f;

constexpr inline float ConvertToRadians(float fDegrees) noexcept {
    return fDegrees * (PI / 180.0f);
}

constexpr inline float ConvertToDegrees(float fRadians) noexcept {
    return fRadians * (180.0f / PI);
}
