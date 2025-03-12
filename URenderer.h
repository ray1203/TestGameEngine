// URenderer.h
#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "Vector.h"
#include "common.h"
#include "FUnrealAliases.h"
class URenderer
{
public:
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    IDXGISwapChain* SwapChain = nullptr;

    ID3D11Texture2D* FrameBuffer = nullptr;
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;
    ID3D11RasterizerState* RasterizerState = nullptr;
    //ID3D11Buffer* ConstantBuffer = nullptr;

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
    D3D11_VIEWPORT ViewportInfo;

    ID3D11VertexShader* SimpleVertexShader;
    ID3D11PixelShader* SimplePixelShader;
    ID3D11InputLayout* SimpleInputLayout=nullptr;
    //ID3D11VertexShader* SimpleVertexShader = nullptr;
    //ID3D11PixelShader* SimplePixelShader = nullptr;
    //ID3D11InputLayout* SimpleInputLayout = nullptr;
    unsigned int Stride = 0;
    /*
    struct FConstants
    {
        FVector Offset;
        float Pad;
        float Radius;
        FVector Color;
    };*/

public:
    void Create(HWND hWindow);
    void Release();
    void SwapBuffer();

    void CreateShader();
    //void CreateShader(const FString& vsPath, const FString& psPath, ID3D11VertexShader*& vertexShader, ID3D11PixelShader*& pixelShader);
    void ReleaseShader();

    void Prepare();
    //void PrepareShader(ID3D11VertexShader& VertexShader, ID3D11PixelShader& PixelShader,ID3D11Buffer& constantVBuffer, ID3D11Buffer& constantPBuffer);
    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);
    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);
    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);

    template <typename T_CONSTANT>
    void CreateConstantBuffer(const T_CONSTANT& constantBufferData, ID3D11Buffer*& constantBuffer)
    {
        D3D11_BUFFER_DESC constantbufferdesc = {};
        constantbufferdesc.ByteWidth = sizeof(constantBufferData) + 0xf & 0xfffffff0; // ensure constant buffer	size is multiple of 16 bytes (최소 16바이트 이상 & 0xfffffff0 = 가장 가깝고 큰 16의 배수)
        constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU frame (매 물체마다 동적으로 갱신 가능)
        constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        Device->CreateBuffer(&constantbufferdesc, nullptr, &constantBuffer);
    }
    template <typename T_DATA>
    void UpdateConstant(const T_DATA &bufferData,ID3D11Buffer* constantBuffer)
    {
        if (constantBuffer) {
            D3D11_MAPPED_SUBRESOURCE constantbufferMSR;			// CPU접근 가능하도록 상수버퍼를 임시로 가져올 버퍼

            DeviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // 매프레임 상수 버퍼 업데이트 (맵핑시 CPU가 데이터 수정 가능)
            memcpy(constantbufferMSR.pData, &bufferData, sizeof(bufferData));
            DeviceContext->Unmap(constantBuffer, 0);		// 리소스를 GPU와의 정상적인 상태로 되돌림 (GPU가 이제 읽기 가능)
        }
    }
    void ReleaseConstantBuffer();
    //void UpdateConstant(FVector Offset);
    //void UpdateConstant(FVector Offset, float Radius);
    //void UpdateConstant(FVector Offset, float Radius, FVector Color);


    struct FConstants {
        FVector Offset;
        float Radius;
    };

    ID3D11Buffer* ConstantBuffer = nullptr;
    void CreateConstantBuffer();
    void PrepareShader();
    void CreateShaderInputLayout(const FString& vsPath, const std::string& psPath, ID3D11VertexShader*& vertexShader, ID3D11PixelShader*& pixelShader/*, D3D11_INPUT_ELEMENT_DESC*& inputElements*/);

private:
    void CreateDeviceAndSwapChain(HWND hWindow);
    void ReleaseDeviceAndSwapChain();

    void CreateFrameBuffer();
    void ReleaseFrameBuffer();

    void CreateRasterizerState();
    void ReleaseRasterizerState();
};
