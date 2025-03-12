#pragma once
#include "Vector.h"

struct FVector4
{
public:
    float X, Y, Z, W;

    /** 기본 생성자 */
    FVector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f)
        : X(_x), Y(_y), Z(_z), W(_w) {
    }

    /** FVector를 기반으로 FVector4 생성 (W 기본값 1) */
    FVector4(const FVector& vec, float _w = 1.0f)
        : X(vec.X), Y(vec.Y), Z(vec.Z), W(_w) {
    }

    /** 벡터 덧셈 */
    FVector4 operator+(const FVector4& other) const noexcept {
        return { X + other.X, Y + other.Y, Z + other.Z, W + other.W };
    }

    /** 벡터 뺄셈 */
    FVector4 operator-(const FVector4& other) const noexcept {
        return { X - other.X, Y - other.Y, Z - other.Z, W - other.W };
    }

    /** 스칼라 곱 */
    FVector4 operator*(float scale) const noexcept {
        return { X * scale, Y * scale, Z * scale, W * scale };
    }

    /** 스칼라 나눗셈 */
    FVector4 operator/(float scale) const noexcept {
        assert(scale != 0);
        return { X / scale, Y / scale, Z / scale, W / scale };
    }
    /** 벡터 나눗셈 및 자기 자신 갱신 */
    FVector4& operator/=(const FVector4& other) noexcept {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        W /= other.W;
        return *this;
    }
    /** 벡터 내적 (Dot Product, 4D) */
    float Dot(const FVector4& other) const noexcept {
        return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
    }

    /** 3D 내적 (W 제외) */
    float Dot3(const FVector4& other) const noexcept {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    /** 벡터 정규화 (W 제외) */
    void Normalize() {
        float len = Length3();
        if (len > 1e-6f) {
            X /= len;
            Y /= len;
            Z /= len;
            W = 1.0f;
        }
    }

    /** 벡터 크기 반환 (4D) */
    float Length() const {
        return sqrt(X * X + Y * Y + Z * Z + W * W);
    }

    /** 벡터 크기 반환 (3D, W 제외) */
    float Length3() const {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    /** 벡터 크기 제곱 반환 */
    float LengthSquared() const {
        return X * X + Y * Y + Z * Z + W * W;
    }


    /** FVector4에서 FVector로 변환하는 함수 */
    FVector ToFVector() const
    {
        if (W != 0.0f)
        {
            return FVector(X / W, Y / W, Z / W);
        }
        return FVector(X, Y, Z);
    }

    /** 벡터 값을 설정 */
    void Set(float x, float y, float z, float w = 1.0f) {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    /** 3D 벡터와 동일한 방향으로 변환 (W 유지) */
    FVector4 TransformVector(const class FMatrix& matrix) const;

    /** 위치 변환 (W가 1일 때) */
    FVector4 TransformPosition(const class FMatrix& matrix) const;
};
