#include "UObject.h"

#include <iostream>

#include "Core/UCamera.h"
#include "Math/FMath.h"
#include "Renderer/URenderer.h"

UObject::UObject() 
    : Translation(FVector(0, 0, 0)), 
      Rotation(FVector(0, 0, 0)), 
      Scale(FVector(1, 1, 1)) 
{}
void UObject::Initialize(URenderer& renderer) {
    VertexBuffer = renderer.CreateVertexBuffer(nullptr, 0);  // 빈 버퍼 생성 가능
    renderer.CreateShaderInputLayout(VertexShaderName, PixelShaderName, VertexShader, PixelShader);

    if (!VertexBuffer) 
        std::cout << "VertexBuffer is nullptr!" << std::endl;

    renderer.CreateConstantBuffer(VertexConstantData, VertexCBuffer);
    assert(VertexCBuffer != nullptr);
    renderer.CreateConstantBuffer(PixelConstantData, PixelCBuffer);
    assert(PixelCBuffer != nullptr);
}

void UObject::MVP()
{
    FMatrix S = FMatrix::ApplyScale(Scale);
    FMatrix R = FMatrix::ApplyRotationX(Rotation.X) *
                FMatrix::ApplyRotationY(Rotation.Y) *
                FMatrix::ApplyRotationZ(Rotation.Z);
    FMatrix T = FMatrix::ApplyTranslation(Translation);
    FMatrix M = S * R * T;

    FMatrix V = FMatrix::LookFromMatrix(UCamera::GetInstance().CameraPosition, 
                                        UCamera::GetInstance().CameraDir, 
                                        UCamera::GetInstance().CameraUp);
    FMatrix P;
    if (UCamera::GetInstance().bIsOrthogonal)
        P = FMatrix::ApplyOrthographic(-1, 1, -1, 1, UCamera::GetInstance().NearPlane, UCamera::GetInstance().FarPlane);
    else
        P = FMatrix::ApplyPerspective(ConvertToRadians(UCamera::GetInstance().FOV), 
                                      UCamera::GetInstance().AspectRatio, 
                                      UCamera::GetInstance().NearPlane, UCamera::GetInstance().FarPlane);
    VertexConstantData.MVP=M*V*P;
    // 변환 결과를 쉐이더에 전달 (상속 클래스에서 구현 필요)
}
void UObject::Render(URenderer& renderer)
{
    renderer.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    renderer.DeviceContext->VSSetShader(VertexShader, nullptr, 0);
    renderer.DeviceContext->PSSetShader(PixelShader, nullptr, 0);

    renderer.DeviceContext->IASetInputLayout(renderer.SimpleInputLayout);
    renderer.DeviceContext->VSSetConstantBuffers(0, 1, &VertexCBuffer);
    renderer.DeviceContext->PSSetConstantBuffers(0, 1, &PixelCBuffer);

    if (!VertexBuffer) 
        std::cout << "VertexBuffer is nullptr!" << std::endl;
    if (numVertices == 0) 
        std::cout << "numVertices is 0!" << std::endl;

    renderer.RenderPrimitive(VertexBuffer, numVertices);
}
void UObject::Update(URenderer& renderer)
{
    UObject::MVP();
    renderer.UpdateConstant(VertexConstantData, VertexCBuffer);
    renderer.UpdateConstant(PixelConstantData, PixelCBuffer);
    if (!VertexCBuffer) 
        std::cout << "VertexCBuffer is nullptr!" << std::endl;
    if (!PixelCBuffer) 
        std::cout << "PixelCBuffer is nullptr!" << std::endl;
}