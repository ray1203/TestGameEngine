#include "UCamera.h"
UCamera UCamera::Instance;


UCamera::UCamera()
{
    // 기본값 설정
    CameraPosition = FVector(0.0f, 0.0f, -1.0f);
    CameraDir = FVector(0.0f, 0.0f, 1.0f);
    CameraUp = FVector(0.0f, 1.0f, 0.0f);
    CameraRight = FVector(1.0f, 0.0f, 0.0f);

    FOV = 70.0f;
    AspectRatio = 1;
    NearPlane = 0.1f;
    FarPlane = 1000.0f;
    bIsOrthogonal = false;

    UpdateViewMatrix();
}
