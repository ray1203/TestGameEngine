#include "Matrix.h"
#include "Vector4.h"
/** �⺻ ������ - ���� ��ķ� �ʱ�ȭ */
FMatrix::FMatrix() {
    SetIdentity();
}

/** 16���� float�� �޾Ƽ� ����� �ʱ�ȭ�ϴ� ������ */
FMatrix::FMatrix(float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33)
{
    M[0][0] = m00; M[0][1] = m01; M[0][2] = m02; M[0][3] = m03;
    M[1][0] = m10; M[1][1] = m11; M[1][2] = m12; M[1][3] = m13;
    M[2][0] = m20; M[2][1] = m21; M[2][2] = m22; M[2][3] = m23;
    M[3][0] = m30; M[3][1] = m31; M[3][2] = m32; M[3][3] = m33;
}

/** ���� ��ķ� ���� */
void FMatrix::SetIdentity() {
    *this = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

/** ��� �� ���� */
FMatrix FMatrix::operator*(const FMatrix& other) const {
    FMatrix result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.M[i][j] = M[i][0] * other.M[0][j] + M[i][1] * other.M[1][j] +
                M[i][2] * other.M[2][j] + M[i][3] * other.M[3][j];
        }
    }
    return result;
}

/** �ڱ� �ڽ��� ��� ������ ���� */
FMatrix& FMatrix::operator*=(const FMatrix& other) {
    *this = *this * other;
    return *this;
}

/** ��� ��ġ */
FMatrix FMatrix::GetTransposed() const {
    FMatrix result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.M[i][j] = M[j][i];
    return result;
}

/** ��Ľ� ��� */
float FMatrix::Determinant() const {
    return	M[0][0] * (
        M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
        M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
        M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
        ) -
        M[1][0] * (
            M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
            M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
            M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
            ) +
        M[2][0] * (
            M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
            M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
            M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
            ) -
        M[3][0] * (
            M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
            M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
            M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
            );
}

/** ������ ��� ���� */
FMatrix FMatrix::ApplyScale(float Scale) {
    return FMatrix(
        Scale, 0.0f, 0.0f, 0.0f,
        0.0f, Scale, 0.0f, 0.0f,
        0.0f, 0.0f, Scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
/** ������ ��� ���� */
FMatrix FMatrix::ApplyScale(FVector Scale) {
    return FMatrix(
        Scale.X, 0.0f, 0.0f, 0.0f,
        0.0f, Scale.Y, 0.0f, 0.0f,
        0.0f, 0.0f, Scale.Z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
/** ��ġ ��ȯ (Translation) ���� */
FMatrix FMatrix::ApplyTranslation(float x, float y, float z) {
    FMatrix TranslationMatrix;
    TranslationMatrix.SetIdentity();
    TranslationMatrix.M[3][0] = x;
    TranslationMatrix.M[3][1] = y;
    TranslationMatrix.M[3][2] = z;
    return TranslationMatrix;
}

FMatrix FMatrix::ApplyTranslation(const FVector& Translation) {
    return ApplyTranslation(Translation.X, Translation.Y, Translation.Z);
}

/** X�� ȸ�� ��� ���� */
FMatrix FMatrix::ApplyRotationX(float Angle) {
    float CosAngle = cosf(Angle);
    float SinAngle = sinf(Angle);
    return FMatrix(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, CosAngle, SinAngle, 0.0f,
        0.0f, -SinAngle, CosAngle, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/** Y�� ȸ�� ��� ���� */
FMatrix FMatrix::ApplyRotationY(float Angle) {
    float CosAngle = cosf(Angle);
    float SinAngle = sinf(Angle);
    return FMatrix(
        CosAngle, 0.0f, -SinAngle, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        SinAngle, 0.0f, CosAngle, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/** Z�� ȸ�� ��� ���� */
FMatrix FMatrix::ApplyRotationZ(float Angle) {
    float CosAngle = cosf(Angle);
    float SinAngle = sinf(Angle);
    return FMatrix(
        CosAngle, SinAngle, 0.0f, 0.0f,
        -SinAngle, CosAngle, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
/** �� ��� ���� (Look Direction ���) */
/** �� ��� ���� (Look Direction ���) */
FMatrix FMatrix::LookFromMatrix(const FVector& EyePosition, const FVector& LookDirection, const FVector& UpVector) {
    // Z���� �������� ����ȭ�� ���� ����
    FVector R2 = LookDirection.GetSafeNormal();

    // X�� ���� (UpVector�� LookDirection�� ���� �� ������ ����)
    FVector R0 = UpVector.Cross(R2).GetSafeNormal();

    // Y�� ���� (R2�� R0�� ���� �� ���� ����)
    FVector R1 = R2.Cross(R0);

    // �� ����� ��ġ �κ� ���
    float D0 = -R0.Dot(EyePosition);
    float D1 = -R1.Dot(EyePosition);
    float D2 = -R2.Dot(EyePosition);

    // DirectXMath�� ������ ������� ��� ��ġ
    FMatrix ViewMatrix(
        R0.X, R1.X, R2.X, 0.0f,
        R0.Y, R1.Y, R2.Y, 0.0f,
        R0.Z, R1.Z, R2.Z, 0.0f,
        D0, D1, D2, 1.0f
    );

    return ViewMatrix;
}


/** �� ��� ���� (Look At ���) */
FMatrix FMatrix::LookAtMatrix(const FVector& EyePosition, const FVector& LookAtPosition, const FVector& UpVector) {
    return LookFromMatrix(EyePosition, LookAtPosition - EyePosition, UpVector);
}
/** ���� ���� ��� ���� */
FMatrix FMatrix::ApplyPerspective(float FovY, float AspectRatio, float NearZ, float FarZ) {
    float TanHalfFov = tanf(0.5f * FovY);
    float Width = 1.0f / (AspectRatio * TanHalfFov);
    float Height = 1.0f / TanHalfFov;
    float fRange = FarZ / (FarZ - NearZ);

    return FMatrix(
        Width, 0.0f, 0.0f, 0.0f,
        0.0f, Height, 0.0f, 0.0f,
        0.0f, 0.0f, fRange, 1.0f,
        0.0f, 0.0f, -fRange * NearZ, 0.0f
    );
}

/** ���� ���� ��� ���� */
FMatrix FMatrix::ApplyOrthographic(float Left, float Right, float Bottom, float Top, float NearZ, float FarZ) {
    float InvWidth = 1.0f / (Right - Left);
    float InvHeight = 1.0f / (Top - Bottom);
    float InvDepth = 1.0f / (FarZ - NearZ);

    return FMatrix(
        2.0f * InvWidth, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f * InvHeight, 0.0f, 0.0f,
        0.0f, 0.0f, InvDepth, 0.0f,
        -(Right + Left) * InvWidth, -(Top + Bottom) * InvHeight, -NearZ * InvDepth, 1.0f
    );
}
FMatrix FMatrix::Inverse()
{
    float inv[16], det;
    float m[16] = {
        M[0][0], M[0][1], M[0][2], M[0][3],
        M[1][0], M[1][1], M[1][2], M[1][3],
        M[2][0], M[2][1], M[2][2], M[2][3],
        M[3][0], M[3][1], M[3][2], M[3][3]
    };

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (fabs(det) < 1e-6)
        return FMatrix(); // ���� ��� ��ȯ

    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        inv[i] *= det;

    return FMatrix(inv);
}
