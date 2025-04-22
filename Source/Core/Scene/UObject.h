#pragma once

#include <d3d11.h>

#include "FUnrealAliases.h"
#include "Math/Matrix.h"
class URenderer;

class UObject
{
    protected:
    
    ID3D11Buffer* VertexBuffer = nullptr;
    ID3D11Buffer* VertexCBuffer;
    ID3D11Buffer* PixelCBuffer;
    int numVertices;

    FString VertexShaderName = "Shader/BallVertexShader.hlsl";
    FString PixelShaderName = "Shader/BallPixelShader.hlsl";
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader* PixelShader = nullptr;
public:
    virtual void Initialize(URenderer& renderer);
    virtual void Render(URenderer& renderer);
    virtual void Update(URenderer& renderer);
    FVector Translation;
    FVector Rotation;
    FVector Scale;
    struct VertexConstantData {
        FMatrix MVP;
    } VertexConstantData;

    struct PixelConstantData {
    } PixelConstantData;

public:
    UObject();
    virtual ~UObject() = default;


    void SetPosition(const FVector& NewPosition) { Translation = NewPosition; }
    void SetRotation(const FVector& NewRotation) { Rotation = NewRotation; }
    void SetScale(const FVector& NewScale) { Scale = NewScale; }

    FVector GetPosition() const { return Translation; }
    FVector GetRotation() const { return Rotation; }
    FVector GetScale() const { return Scale; }

    virtual void MVP();
};
