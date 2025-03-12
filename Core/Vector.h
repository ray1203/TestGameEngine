#pragma once
#include <cmath>
#include <cassert>

struct FVector
{
public:
    float X, Y, Z;

    /** �⺻ ������ */
    FVector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : X(_x), Y(_y), Z(_z) {}

    /** ���� ���� */
    FVector operator+(const FVector& other) const noexcept {
        return { X + other.X, Y + other.Y, Z + other.Z };
    }
    /** ���� ���� �� �ڱ� �ڽ� ���� */
    FVector& operator+=(const FVector& other) noexcept {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }
    /** ���� ���� */
    FVector operator-(const FVector& other) const noexcept {
        return { X - other.X, Y - other.Y, Z - other.Z };
    }
    /** ���� �y�� �� �ڱ� �ڽ� ���� */
    FVector& operator-=(const FVector& other) noexcept {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }
    /** ��Į�� �� */
    FVector operator*(float scale) const noexcept {
        return { X * scale, Y * scale, Z * scale };
    }

    /** ��Į�� ������ */
    FVector operator/(float scale) const noexcept {
        assert(scale != 0);
        return { X / scale, Y / scale, Z / scale };
    }
    /** ���� ������ �� �ڱ� �ڽ� ���� */
    FVector& operator/=(const FVector& other) noexcept {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        return *this;
    }
    /** ���� ���� (Dot Product) */
    float Dot(const FVector& other) const noexcept {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    /** ���� ���� (Cross Product) */
    FVector Cross(const FVector& other) const noexcept {
        return {
            Y * other.Z - Z * other.Y,
            Z * other.X - X * other.Z,
            X * other.Y - Y * other.X
        };
    }

    /** ���� ����ȭ */
    void Normalize() {
        float len = Length();
        if (len > 1e-6f) {
            X /= len;
            Y /= len;
            Z /= len;
        }
    }
    FVector GetSafeNormal(float Tolerance= 1.e-8f) const {
        float SquareSum = X * X + Y * Y + Z * Z;

        if (SquareSum > Tolerance) {
            float Scale = 1.0f / sqrtf(SquareSum);
            return FVector(X * Scale, Y * Scale, Z * Scale);
        }
        return FVector(0.0f, 0.0f, 0.0f); // 0 ������ ��� �����ϰ� ��ȯ
    }
    /** ���� ���� ��ȯ */
    float Length() const {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    /** ���� ���� ���� ��ȯ */
    float LengthSquared() const {
        return X * X + Y * Y + Z * Z;
    }

    /** �ε帮�Խ� ȸ�� ���� ��� ���� ȸ�� */
    FVector RotateAround(const FVector& axis, float angle) const {
        float s = sin(angle);
        float c = cos(angle);
        FVector normAxis = axis;
        normAxis.Normalize();
        return *this * c + normAxis.Cross(*this) * s + normAxis * (normAxis.Dot(*this) * (1 - c));
    }

    /** ���� ���� ���� */
    void Set(float x, float y, float z) {
        X = x;
        Y = y;
        Z = z;
    }

    /** ������ 3D ��ȯ */
    FVector TransformPosition(const class FMatrix& matrix) const;

    /** ���� ���� ��ȯ (Translation ����) */
    FVector TransformVector(const class FMatrix& matrix) const;
};

