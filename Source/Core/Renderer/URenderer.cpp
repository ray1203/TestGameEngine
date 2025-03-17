// URenderer.cpp
#include "URenderer.h"
#include <iostream>
void URenderer::Create(HWND hWindow)
{
    CreateDeviceAndSwapChain(hWindow);
    CreateFrameBuffer();
    CreateRasterizerState();
}

void URenderer::CreateDeviceAndSwapChain(HWND hWindow)
{
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.OutputWindow = hWindow;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
        &swapChainDesc, &SwapChain, &Device, nullptr, &DeviceContext);

    SwapChain->GetDesc(&swapChainDesc);
    ViewportInfo = { 0.0f, 0.0f, (float)swapChainDesc.BufferDesc.Width, (float)swapChainDesc.BufferDesc.Height, 0.0f, 1.0f };
}

void URenderer::ReleaseDeviceAndSwapChain()
{
    if (DeviceContext) DeviceContext->Flush();
    if (SwapChain) { SwapChain->Release(); SwapChain = nullptr; }
    if (Device) { Device->Release(); Device = nullptr; }
    if (DeviceContext) { DeviceContext->Release(); DeviceContext = nullptr; }
}

void URenderer::CreateFrameBuffer()
{
    SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);
    D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
    framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth) {
    // 2. Create a vertex buffer
    D3D11_BUFFER_DESC vertexbufferdesc = {};
    vertexbufferdesc.ByteWidth = byteWidth;
    vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;			// will never be updated
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexBufferSRD = { vertices };
    ID3D11Buffer* vertexBuffer;
    Device->CreateBuffer(&vertexbufferdesc, &vertexBufferSRD, &vertexBuffer);

    return vertexBuffer;
}
void URenderer::ReleaseFrameBuffer()
{
    if (FrameBuffer) { FrameBuffer->Release(); FrameBuffer = nullptr; }
    if (FrameBufferRTV) { FrameBufferRTV->Release(); FrameBufferRTV = nullptr; }
}

void URenderer::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    Device->CreateRasterizerState(&rasterizerDesc, &RasterizerState);
}

void URenderer::ReleaseRasterizerState()
{
    if (RasterizerState) { RasterizerState->Release(); RasterizerState = nullptr; }
}

void URenderer::Release()
{
    ReleaseRasterizerState();
    DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    ReleaseFrameBuffer();
    ReleaseDeviceAndSwapChain();
}

void URenderer::SwapBuffer()
{
    SwapChain->Present(1, 0);
}
/*
void URenderer::CreateShader(const FString& vsPath, const FString& psPath, ID3D11VertexShader*& vertexShader, ID3D11PixelShader*& pixelShader) {
    
    ID3DBlob* vertexShaderCSO;
    ID3DBlob* pixelShaderCSO;
    std::wstring wVsPath = std::wstring(vsPath.begin(), vsPath.end());
    std::wstring wPsPath = std::wstring(psPath.begin(), psPath.end());
    D3DCompileFromFile(wVsPath.c_str(), nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexShaderCSO, nullptr);
    Device->CreateVertexShader(vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), nullptr, &vertexShader);
    D3DCompileFromFile(wPsPath.c_str(), nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelShaderCSO, nullptr);
    Device->CreatePixelShader(pixelShaderCSO->GetBufferPointer(), pixelShaderCSO->GetBufferSize(), nullptr, &pixelShader);
    if (!vertexShader) std::cout << "VertexShader is nullptr!" << std::endl;
    if (!pixelShader) std::cout << "PixelShader is nullptr!" << std::endl;

    if (!SimpleInputLayout) {
        D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        Device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vertexShaderCSO->GetBufferPointer()
            ,vertexShaderCSO->GetBufferSize(), &SimpleInputLayout);

        DeviceContext->IASetInputLayout(SimpleInputLayout);
        Stride = sizeof(FVertexSimple);
    }

}*/
void URenderer::CreateShader()
{
    ID3DBlob* vertexShaderCSO;
    ID3DBlob* pixelShaderCSO;

    D3DCompileFromFile(L"Shader/ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexShaderCSO, nullptr);					// ������ ���� : ������ ���н� Blob ��ü ���ڿ��� ���� �޽��� ����
    Device->CreateVertexShader(vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);
    D3DCompileFromFile(L"Shader/ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelShaderCSO, nullptr);
    Device->CreatePixelShader(pixelShaderCSO->GetBufferPointer(), pixelShaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), &SimpleInputLayout);

    Stride = sizeof(FVertexSimple);
}
void URenderer::Prepare()
{
    DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);
    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DeviceContext->RSSetViewports(1, &ViewportInfo);
    DeviceContext->RSSetState(RasterizerState);
    DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
    DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}



void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
    UINT offset = 0;
    DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
    //std::cout << "Drawing " << numVertices << " vertices." << std::endl;
    DeviceContext->Draw(numVertices, 0);
}


void URenderer::ReleaseConstantBuffer()
{
    //if () { ConstantBuffer->Release(); ConstantBuffer = nullptr; }
}


void URenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC constantbufferdesc = {};
    constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer	size is multiple of 16 bytes (�ּ� 16����Ʈ �̻� & 0xfffffff0 = ���� ������ ū 16�� ���)
    constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU frame (�� ��ü���� �������� ���� ����)
    constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}
void URenderer::PrepareShader()
{
    DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
    DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
    DeviceContext->IASetInputLayout(SimpleInputLayout);

    // ���ؽ� ���̴��� ��� ���� ����
    if (ConstantBuffer) {
        DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
    }
}
void URenderer::CreateShaderInputLayout(const std::string& vsPath, const std::string& psPath, ID3D11VertexShader*& vertexShader, ID3D11PixelShader*& pixelShader/*, D3D11_INPUT_ELEMENT_DESC*& inputElements*/)
{
    ID3DBlob* vertexShaderCSO;
    ID3DBlob* pixelShaderCSO;
    ID3DBlob* errorCSO;

    std::wstring wVsPath = std::wstring(vsPath.begin(), vsPath.end());
    std::wstring wPsPath = std::wstring(psPath.begin(), psPath.end());
    std::string err;

    D3DCompileFromFile(wVsPath.c_str(), nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexShaderCSO, &errorCSO);					// ������ ���� : ������ ���н� Blob ��ü ���ڿ��� ���� �޽��� ����
    //std::cout << errorCSO->
    Device->CreateVertexShader(vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), nullptr, &vertexShader);
    D3DCompileFromFile(wPsPath.c_str(), nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelShaderCSO, nullptr);
    Device->CreatePixelShader(pixelShaderCSO->GetBufferPointer(), pixelShaderCSO->GetBufferSize(), nullptr, &pixelShader);

    /*D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };*/

    //Device->CreateInputLayout(inputElements, ARRAYSIZE(layout), vertexShaderCSO->GetBufferPointer(), vertexShaderCSO->GetBufferSize(), &SimpleInputLayout);

    Stride = sizeof(FVertexSimple);
}