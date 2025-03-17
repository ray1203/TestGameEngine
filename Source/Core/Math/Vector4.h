#pragma once
#include "Vector.h"

struct FVector4
{
public:
    float X, Y, Z, W;

    /** �⺻ ������ */
    FVector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 1.0f)
        : X(_x), Y(_y), Z(_z), W(_w) {
    }

    /** FVector�� ������� FVector4 ���� (W �⺻�� 1) */
    FVector4(const FVector& vec, float _w = 1.0f)
        : X(vec.X), Y(vec.Y), Z(vec.Z), W(_w) {
    }

    /** ���� ���� */
    FVector4 operator+(const FVector4& other) const noexcept {
        return { X + other.X, Y + other.Y, Z + other.Z, W + other.W };
    }

    /** ���� ���� */
    FVector4 operator-(const FVector4& other) const noexcept {
        return { X - other.X, Y - other.Y, Z - other.Z, W - other.W };
    }

    /** ��Į�� �� */
    FVector4 operator*(float scale) const noexcept {
        return { X * scale, Y * scale, Z * scale, W * scale };
    }

    /** ��Į�� ������ */
    FVector4 operator/(float scale) const noexcept {
        assert(scale != 0);
        return { X / scale, Y / scale, Z / scale, W / scale };
    }
    /** ���� ������ �� �ڱ� �ڽ� ���� */
    FVector4& operator/=(const FVector4& other) noexcept {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        W /= other.W;
        return *this;
    }
    /** ���� ���� (Dot Product, 4D) */
    float Dot(const FVector4& other) const noexcept {
        return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
    }

    /** 3D ���� (W ����) */
    float Dot3(const FVector4& other) const noexcept {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    /** ���� ����ȭ (W ����) */
    void Normalize() {
        float len = Length3();
        if (len > 1e-6f) {
            X /= len;
            Y /= len;
            Z /= len;
            W = 1.0f;
        }
    }

    /** ���� ũ�� ��ȯ (4D) */
    float Length() const {
        return sqrt(X * X + Y * Y + Z * Z + W * W);
    }

    /** ���� ũ�� ��ȯ (3D, W ����) */
    float Length3() const {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    /** ���� ũ�� ���� ��ȯ */
    float LengthSquared() const {
        return X * X + Y * Y + Z * Z + W * W;
    }


    /** FVector4���� FVector�� ��ȯ�ϴ� �Լ� */
    FVector ToFVector() const
    {
        if (W != 0.0f)
        {
            return FVector(X / W, Y / W, Z / W);
        }
        return FVector(X, Y, Z);
    }

    /** ���� ���� ���� */
    void Set(float x, float y, float z, float w = 1.0f) {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    /** 3D ���Ϳ� ������ �������� ��ȯ (W ����) */
    FVector4 TransformVector(const class FMatrix& matrix) const;

    /** ��ġ ��ȯ (W�� 1�� ��) */
    FVector4 TransformPosition(const class FMatrix& matrix) const;
};
