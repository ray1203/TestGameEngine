#pragma once
#include "Math/Matrix.h"
#include "Math/Vector.h"
class UCamera
{
private:
    static UCamera Instance;

    UCamera();  // 생성자 private 처리하여 외부에서 객체 생성 방지
public:
    static UCamera& GetInstance() { return Instance; }

    FVector CameraPosition{0.0f,0.0f,-1.0f};
    FVector CameraDir{ 0.0f, 0.0f, 1.0f };
    FVector CameraUp{ 0.0f, 1.0f, 0.0f };

    //FVector CameraForward;
    FVector CameraRight {1.0f, 0.0f, 0.0f};

    float FOV=70.0f;
    float AspectRatio = 1;
    float NearPlane=0.1f;
    float FarPlane=1000.0f;

    bool bIsOrthogonal = false;
    FMatrix ViewMatrix;

    void UpdateViewMatrix()
    {
        ViewMatrix = FMatrix::LookAtMatrix(CameraPosition, CameraDir, CameraUp);
    }
};
