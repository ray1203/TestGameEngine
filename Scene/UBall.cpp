#include "UBall.h"
#include "Urenderer.h"
#include "common.h"
#include <iostream>
#include <cassert>
#include "Matrix.h"
#include "UCamera.h"
#include "FMath.h"
UCamera* UBall::Camera = nullptr;
UBall::UBall():Translation(),Scale(0.5f,0.5f,0.5f),Rotation() {
	//Radius = ((rand() % 50) + 10) / 40.0f / 10.0f; // (0.25 ~ 1.5)/10
	//Mass = Radius * 2.0f;
	////Location = FVector(((rand() % 100) / 50.0f) - 1.0f, (rand() % 100) / 100.0f, 0.0f);
	////Velocity = FVector(((rand() % 100 - 50) / 1000.0f), ((rand() % 100 - 50) / 1000.0f), 0.0f);
	//Location = FVector(0, 0, 0);
	//Velocity = FVector(0, 0, 0);
	numVertices = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	std::cout << numVertices;
	 
}
void UBall::Initialize(URenderer& renderer) {
	VertexBuffer = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
	renderer.CreateShaderInputLayout(VertexShaderName, PixelShaderName, VertexShader, PixelShader);

	//renderer.CreateShader("ShaderW0.hlsl", "ShaderW0.hlsl", VertexShader,PixelShader);
	if (!VertexBuffer) std::cout << "VertexBuffer is nullptr!" << std::endl;

	//renderer.CreateConstantBuffer();
	renderer.CreateConstantBuffer(VertexConstantData, VertexCBuffer);
	assert(VertexCBuffer != nullptr);
	renderer.CreateConstantBuffer(PixelConstantData, PixelCBuffer);
	assert(PixelCBuffer != nullptr);
	//renderer.CreateShaderInputLayout("ShaderW0.hlsl", "ShaderW0.hlsl", VertexShader, PixelShader);
}

// 예:3
void UBall::Render(URenderer& renderer)
{
	renderer.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderer.DeviceContext->VSSetShader(VertexShader, nullptr, 0);
	renderer.DeviceContext->PSSetShader(PixelShader, nullptr, 0);

	renderer.DeviceContext->IASetInputLayout(renderer.SimpleInputLayout);
	renderer.DeviceContext->VSSetConstantBuffers(0, 1, &VertexCBuffer);
	renderer.DeviceContext->PSSetConstantBuffers(0, 1, &PixelCBuffer);
	if (!VertexBuffer) std::cout << "VertexBuffer is nullptr!" << std::endl;
	if (numVertices == 0) std::cout << "numVertices is 0!" << std::endl;
	renderer.RenderPrimitive(VertexBuffer, numVertices);

}
void UBall::CheckBorder() {
	//float renderRadius = Radius;
	//if (Location.X < leftBorder + renderRadius)
	//{			 
	//	Location.X = leftBorder + renderRadius;
	//	//Velocity.X *= -bounciness;
	//}
	//if (Location.X > rightBorder - renderRadius)
	//{
	//	Location.X = rightBorder - renderRadius;
	//	//Velocity.X *= -bounciness;
	//}
	//if (Location.Y < topBorder + renderRadius)
	//{			 
	//	Location.Y = topBorder + renderRadius;
	//	//Velocity.Y *= -bounciness;
	//}			 
	//if (Location.Y > bottomBorder - renderRadius)
	//{			 
	//	Location.Y = bottomBorder - renderRadius;
	//	//Velocity.Y *= -bounciness;
	//}
}
// 예:5
void UBall::Move()
{
	ApplyGravity();
	Translation.X += Velocity.X;
	Translation.Y += Velocity.Y;
	CheckBorder();
}
void UBall::ApplyGravity() {
	/*
	if (bEnableGravity) {
		Velocity += FVector(0.0f, gravityStrength, 0.0f);
	}*/
}
void UBall::MVP() {
	FMatrix S = FMatrix::ApplyScale(Scale);
	FMatrix R = FMatrix::ApplyRotationX(Rotation.X) *
		FMatrix::ApplyRotationX(Rotation.Y) *
		FMatrix::ApplyRotationX(Rotation.Z);
	FMatrix T = FMatrix::ApplyTranslation(Translation);
	FMatrix M = S * R * T;
	FMatrix V = FMatrix::LookFromMatrix(Camera->CameraPosition, Camera->CameraDir, Camera->CameraUp);
	FMatrix P;
	if (Camera->bIsOrthogonal)
		P = FMatrix::ApplyOrthographic(-1, 1, -1, 1, Camera->NearPlane, Camera->FarPlane);
	else
		P = FMatrix::ApplyPerspective(ConvertToRadians(Camera->FOV), Camera->AspectRatio, Camera->NearPlane, Camera->FarPlane);
	VertexConstantData.MVP = M*V*P;
}
void UBall::Update(URenderer& renderer)
{
	Move();
	MVP();
	renderer.UpdateConstant(VertexConstantData, VertexCBuffer);
	renderer.UpdateConstant(PixelConstantData, PixelCBuffer);
	
	if (!VertexCBuffer) std::cout << "VertexCBuffer is nullptr!" << std::endl;
	if (!PixelCBuffer) std::cout << "PixelCBuffer is nullptr!" << std::endl;
	
}