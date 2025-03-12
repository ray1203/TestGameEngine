#pragma once
#include "Vector4.h"
#include "Vector.h"
struct  alignas(16) FMatrix
{
public:
    float M[4][4];

    FMatrix();

    FMatrix(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33);

    FMatrix(const float* InM)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                M[i][j] = InM[i * 4 + j];
            }
        }
    }

    /** ���� ��ķ� ���� */
    void SetIdentity();

    /** ��� �� ���� */
    FMatrix operator*(const FMatrix& other) const;

    /** �ڱ� �ڽ��� ��� ������ ���� */
    FMatrix& operator*=(const FMatrix& other);

    /** ��� ��ġ */
    FMatrix GetTransposed() const;

    /** ��Ľ� ��� */
    float Determinant() const;

    /** ������ ��� ���� (���� �Լ�) */
    static FMatrix ApplyScale(float Scale);
    static FMatrix ApplyScale(FVector Scale);

    /** ��ġ ��ȯ (Translation) ���� (���� �Լ�) n*/
    static FMatrix ApplyTranslation(float x, float y, float z);
    static FMatrix ApplyTranslation(const FVector& Translation);

    /** X�� ȸ�� ��� ���� (���� �Լ�) n*/
    static FMatrix ApplyRotationX(float Angle);

    /** Y�� ȸ�� ��� ���� (���� �Լ�) n*/
    static FMatrix ApplyRotationY(float Angle);

    /** Z�� ȸ�� ��� ���� (���� �Լ�) n*/
    static FMatrix ApplyRotationZ(float Angle);

    /** �� ��� ���� (Look Direction ���) */
    static FMatrix LookFromMatrix(const FVector& EyePosition, const FVector& LookDirection, const FVector& UpVector);

    /** �� ��� ���� (Look At ���) */
    static FMatrix LookAtMatrix(const FVector& EyePosition, const FVector& LookAtPosition, const FVector& UpVector);

    /** ���� ���� ��� ���� n*/
    static FMatrix ApplyPerspective(float FovY, float AspectRatio, float NearZ, float FarZ);

    /** ���� ���� ��� ���� n*/
    static FMatrix ApplyOrthographic(float Left, float Right, float Bottom, float Top, float NearZ, float FarZ);

    /** FVector�� FMatrix�� ��ȯ�ϴ� �Լ� (��ġ ��ȯ ����) */
    FVector TransformPosition(const FVector& V)
    {
        FVector4 vec = FVector4(V.X, V.Y, V.Z, 1.0f);
        return TransformFVector4(vec).ToFVector();
    }

    /** FVector4�� FMatrix�� ��ȯ�ϴ� �Լ� (Homogeneous transform) */
    FVector4 TransformFVector4(const FVector4& P)
    {
        FVector4 Result;
        Result.X = P.X * M[0][0] + P.Y * M[1][0] + P.Z * M[2][0] + P.W * M[3][0];
        Result.Y = P.X * M[0][1] + P.Y * M[1][1] + P.Z * M[2][1] + P.W * M[3][1];
        Result.Z = P.X * M[0][2] + P.Y * M[1][2] + P.Z * M[2][2] + P.W * M[3][2];
        Result.W = P.X * M[0][3] + P.Y * M[1][3] + P.Z * M[2][3] + P.W * M[3][3];
        return Result;
    }


    /** �� ��� ����*/
    FMatrix Inverse();
};
