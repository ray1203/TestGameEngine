#pragma once
#include "Vector.h"
#include "Matrix.h"
class UCamera
{
public:
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
