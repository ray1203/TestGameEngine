#pragma once
#include <d3d11.h>
#include "Vertices.h"
#include "FUnrealAliases.h"
#include "Math/Matrix.h"
class URenderer;
class UCamera;
class UBall
{


public:
	int numVertices;
	FVector Velocity;
	FVector Translation;
	FVector Rotation;
	FVector Scale;
	static UCamera* Camera;
	FString VertexShaderName = "BallVertexShader.hlsl";
	FString PixelShaderName = "BallPixelShader.hlsl";
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	struct VertexConstantData {
		FMatrix MVP;
	}VertexConstantData;

	struct PixelConstantData {
	}PixelConstantData;
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* VertexCBuffer;
	ID3D11Buffer* PixelCBuffer;
	// 이후 추가할 변수와 함수 이름은 자유롭게 정하세요.

	// 예:1
	//float Index;

	// 예:2
	//int NumHits;
	UBall();
	void Initialize(URenderer& renderer);
	//void CreateShader();
	// 예:3
	void Render(URenderer& renderer);
	void CheckBorder();
	// 예:5
	void Move();
	void ApplyGravity();
	// 예:6
	void Update(URenderer& renderer);
	void MVP();
};
