#include <windows.h>

// 여기에 아래 코드를 추가 합니다.

// D3D 사용에 필요한 라이브러리들을 링크합니다.
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// D3D 사용에 필요한 헤더파일들을 포함합니다.
#include <d3d11.h>
#include <d3dcompiler.h>
#include<ctime>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"

// 1. Define the triangle vertices
struct FVertexSimple
{
	float x, y, z;    // Position
	float r, g, b, a; // Color
};
// Structure for a 3D vector
struct FVector3
{
	float x, y, z;
	FVector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
	// 덧셈 연산자 오버로딩
	FVector3 operator+(const FVector3& other) const {
		return FVector3(x + other.x, y + other.y, z + other.z);
	}

	// 뺄셈 연산자 오버로딩
	FVector3 operator-(const FVector3& other) const {
		return FVector3(x - other.x, y - other.y, z - other.z);
	}

	// 스칼라 곱셈 연산자 오버로딩
	FVector3 operator*(float scalar) const {
		return FVector3(x * scalar, y * scalar, z * scalar);
	}

	// 스칼라 나눗셈 연산자 오버로딩
	FVector3 operator/(float scalar) const {
		return FVector3(x / scalar, y / scalar, z / scalar);
	}

	// 자기 자신을 더하는 연산
	FVector3& operator+=(const FVector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	// 자기 자신을 빼는 연산
	FVector3& operator-=(const FVector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	// 자기 자신을 스칼라 곱하는 연산
	FVector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// 자기 자신을 스칼라 나누는 연산
	FVector3& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
};

#include "Sphere.h"
// 삼각형을 하드 코딩
FVertexSimple triangle_vertices[] =
{
	{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
	{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
	{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
};
FVertexSimple cube_vertices[] =
{
	// Front face (Z+)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Bottom-left (red)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
	{  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-right (blue)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

	// Back face (Z-)
	{ -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-left (cyan)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
	{  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)

	// Left face (X-)
	{ -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-left (purple)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)
	{ -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

	// Right face (X+)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f }, // Bottom-left (orange)
	{  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
	{  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
	{  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
	{  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
	{  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.5f, 1.0f }, // Top-right (dark blue)

	// Top face (Y+)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f, 1.0f }, // Bottom-left (light green)
	{ -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
	{  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)
	{ -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
	{  0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Top-right (brown)
	{  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)

	// Bottom face (Y-)
	{ -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Bottom-left (brown)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Top-right (green)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
};

class URenderer
{
public:
	// Direct3D 11 장치(Device)와 장치 컨텍스트(Device Context) 및 스왑 체인(Swap Chain)을 관리하기 위한 포인터들
	ID3D11Device* Device = nullptr; // GPU와 통신하기 위한 Direct3D 장치
	ID3D11DeviceContext* DeviceContext = nullptr; // GPU 명령 실행을 담당하는 컨텍스트
	IDXGISwapChain* SwapChain = nullptr; // 프레임 버퍼를 교체하는 데 사용되는 스왑 체인

	// 렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
	ID3D11Texture2D* FrameBuffer = nullptr; // 화면 출력용 텍스처
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; // 텍스처를 렌더 타겟으로 사용하는 뷰
	ID3D11RasterizerState* RasterizerState = nullptr; // 래스터라이저 상태(컬링, 채우기 모드 등 정의)
	ID3D11Buffer* ConstantBuffer = nullptr; // 쉐이더에 데이터를 전달하기 위한 상수 버퍼

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // 화면을 초기화(clear)할 때 사용할 색상 (RGBA)
	D3D11_VIEWPORT ViewportInfo; // 렌더링 영역을 정의하는 뷰포트 정보

public:
	// 렌더러 초기화 함수
	void Create(HWND hWindow)
	{
		// Direct3D 장치 및 스왑 체인 생성
		CreateDeviceAndSwapChain(hWindow);

		// 프레임 버퍼 생성
		CreateFrameBuffer();

		// 래스터라이저 상태 생성
		CreateRasterizerState();

		// 깊이 스텐실 버퍼 및 블렌드 상태는 이 코드에서는 다루지 않음
	}

	// Direct3D 장치 및 스왑 체인을 생성하는 함수
	void CreateDeviceAndSwapChain(HWND hWindow)
	{
		// 지원하는 Direct3D 기능 레벨을 정의
		D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

		// 스왑 체인 설정 구조체 초기화
		DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
		swapchaindesc.BufferDesc.Width = 0; // 창 크기에 맞게 자동으로 설정
		swapchaindesc.BufferDesc.Height = 0; // 창 크기에 맞게 자동으로 설정
		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 색상 포맷
		swapchaindesc.SampleDesc.Count = 1; // 멀티 샘플링 비활성화
		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
		swapchaindesc.BufferCount = 2; // 더블 버퍼링
		swapchaindesc.OutputWindow = hWindow; // 렌더링할 창 핸들
		swapchaindesc.Windowed = TRUE; // 창 모드
		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 스왑 방식

		// Direct3D 장치와 스왑 체인을 생성
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
			&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

		// 생성된 스왑 체인의 정보 가져오기
		SwapChain->GetDesc(&swapchaindesc);

		// 뷰포트 정보 설정
		ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };


	}

	// Direct3D 장치 및 스왑 체인을 해제하는 함수
	void ReleaseDeviceAndSwapChain()
	{
		if (DeviceContext)
		{
			DeviceContext->Flush(); // 남아있는 GPU 명령 실행
		}

		if (SwapChain)
		{
			SwapChain->Release();
			SwapChain = nullptr;
		}

		if (Device)
		{
			Device->Release();
			Device = nullptr;
		}

		if (DeviceContext)
		{
			DeviceContext->Release();
			DeviceContext = nullptr;
		}
	}

	// 프레임 버퍼를 생성하는 함수
	void CreateFrameBuffer()
	{
		// 스왑 체인으로부터 백 버퍼 텍스처 가져오기
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

		// 렌더 타겟 뷰 생성
		D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
		framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
		framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처

		Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
	}

	// 프레임 버퍼를 해제하는 함수
	void ReleaseFrameBuffer()
	{
		if (FrameBuffer)
		{
			FrameBuffer->Release();
			FrameBuffer = nullptr;
		}

		if (FrameBufferRTV)
		{
			FrameBufferRTV->Release();
			FrameBufferRTV = nullptr;
		}
	}

	// 래스터라이저 상태를 생성하는 함수
	void CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc = {};
		rasterizerdesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
		rasterizerdesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링

		Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
	}

	// 래스터라이저 상태를 해제하는 함수
	void ReleaseRasterizerState()
	{
		if (RasterizerState)
		{
			RasterizerState->Release();
			RasterizerState = nullptr;
		}
	}

	// 렌더러에 사용된 모든 리소스를 해제하는 함수
	void Release()
	{
		RasterizerState->Release();

		// 렌더 타겟을 초기화
		DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ReleaseFrameBuffer();
		ReleaseDeviceAndSwapChain();
	}

	// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
	void SwapBuffer()
	{
		SwapChain->Present(1, 0); // 1: VSync 활성화
	}
	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;

	void CreateShader()
	{
		ID3DBlob* vertexshaderCSO;
		ID3DBlob* pixelshaderCSO;

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

		Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

		Stride = sizeof(FVertexSimple);

		vertexshaderCSO->Release();
		pixelshaderCSO->Release();
	}

	void ReleaseShader()
	{
		if (SimpleInputLayout)
		{
			SimpleInputLayout->Release();
			SimpleInputLayout = nullptr;
		}

		if (SimplePixelShader)
		{
			SimplePixelShader->Release();
			SimplePixelShader = nullptr;
		}

		if (SimpleVertexShader)
		{
			SimpleVertexShader->Release();
			SimpleVertexShader = nullptr;
		}
	}
	// URenderer Class에 아래 함수를 추가 하세요.
	void Prepare()
	{
		DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetViewports(1, &ViewportInfo);
		DeviceContext->RSSetState(RasterizerState);

		DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void PrepareShader()
	{
		DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
		DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
		DeviceContext->IASetInputLayout(SimpleInputLayout);

		// 버텍스 쉐이더에 상수 버퍼를 설정합니다.
		if (ConstantBuffer)
		{
			DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
			DeviceContext->PSSetConstantBuffers(0, 1, &ConstantBuffer);
		}
	}

	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}

	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
	{
		// 2. Create a vertex buffer
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = byteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		return vertexBuffer;
	}

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
	{
		vertexBuffer->Release();
	}

	struct FConstants
	{
		FVector3 Offset;
		float Pad;
		float Radius;
		FVector3 Color;
	};


	void CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantbufferdesc = {};
		constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
		constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
		constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
	}

	void ReleaseConstantBuffer()
	{
		if (ConstantBuffer)
		{
			ConstantBuffer->Release();
			ConstantBuffer = nullptr;
		}
	}

	void UpdateConstant(FVector3 Offset)
	{
		if (ConstantBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
			FConstants* constants = (FConstants*)constantbufferMSR.pData;
			{
				constants->Offset = Offset;
			}
			DeviceContext->Unmap(ConstantBuffer, 0);
		}
	}
	void UpdateConstant(FVector3 Offset, float Radius)
	{
		if (ConstantBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
			FConstants* constants = (FConstants*)constantbufferMSR.pData;
			{
				constants->Offset = Offset;
				constants->Radius = Radius;
			}
			DeviceContext->Unmap(ConstantBuffer, 0);
		}
	}
	void UpdateConstant(FVector3 Offset, float Radius, FVector3 Color)
	{
		if (ConstantBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);
			FConstants* constants = (FConstants*)constantbufferMSR.pData;
			{
				constants->Offset = Offset;
				constants->Radius = Radius;
				constants->Color = Color;
			}
			DeviceContext->Unmap(ConstantBuffer, 0);
		}
	}
};
ID3D11Buffer* vertexBufferSphere;
UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
const float leftBorder = -1.0f;
const float rightBorder = 1.0f;
const float topBorder = -1.0f;
const float bottomBorder = 1.0f;
float gravityStrength = -0.005f;  // 기본 중력 값
FVector3 Gravity(0.0f, gravityStrength, 0.0f); // 중력 벡터
float bounciness = 1.0f; // 1.0 = 완전 탄성 충돌, 0.0 = 완전 비탄성 충돌
bool bEnableGravity = false; // 중력 활성화 여부

bool bUseCustomColor = false;
float BallColor[3] = { 1.0f, 1.0f, 1.0f }; // 기본 흰색 (RGB)
class UBall
{
public:
	// 클래스 이름과, 아래 두개의 변수 이름은 변경하지 않습니다.
	FVector3 Location;
	FVector3 Velocity;
	float Radius;
	float Mass;
	
	UBall* NextBall;

	// 이후 추가할 변수와 함수 이름은 자유롭게 정하세요.

	// 예:1
	//float Index;

	// 예:2
	//int NumHits;
	UBall() {
		Radius = ((rand() % 50) + 10) / 40.0f/10.0f; // (0.25 ~ 1.5)/10
		Mass = Radius * 2.0f;
		Location = FVector3(((rand() % 100) / 50.0f) - 1.0f, (rand()%100)/100.0f, 0.0f);
		Velocity = FVector3(((rand() % 100 - 50) / 1000.0f), ((rand() % 100 - 50) / 1000.0f), 0.0f);
		NextBall = NULL;
	}
	// 예:3
	void Render(URenderer* renderer)
	{
		FVector3 Color = bUseCustomColor ? FVector3(BallColor[0], BallColor[1], BallColor[2]) : FVector3(-1.0f, -1.0f, -1.0f);
		renderer->UpdateConstant(Location,Radius,Color);
		renderer->RenderPrimitive(vertexBufferSphere, numVerticesSphere);
	}
	void CheckBorder() {
		float renderRadius = Radius;
		if (Location.x < leftBorder + renderRadius)
		{
			Location.x = leftBorder + renderRadius;
			Velocity.x *= -bounciness;
		}
		if (Location.x > rightBorder - renderRadius)
		{
			Location.x = rightBorder - renderRadius;
			Velocity.x *= -bounciness;
		}
		if (Location.y < topBorder + renderRadius)
		{
			Location.y = topBorder + renderRadius;
			Velocity.y *= -bounciness;
		}
		if (Location.y > bottomBorder - renderRadius)
		{
			Location.y = bottomBorder - renderRadius;
			Velocity.y *= -bounciness;
		}
	}
	// 예:5
	void Move()
	{
		ApplyGravity();
		Location.x += Velocity.x;
		Location.y += Velocity.y;
		CheckBorder();
	}
	void ApplyGravity() {
		if (bEnableGravity) {
			Velocity += FVector3(0.0f, gravityStrength, 0.0f);
		}
	}
	// 예:6
	void Update()
	{
	}
};
class UBallManager
{
public:
	UBall* HeadBall; // 리스트의 첫 번째 공
	int BallCount;   // 현재 공 개수

	UBallManager() : HeadBall(nullptr), BallCount(0) {}

	// 새로운 공을 추가하는 함수
	void AddBall()
	{
		UBall* NewBall = new UBall();
		NewBall->NextBall = HeadBall;
		HeadBall = NewBall;
		BallCount++;
	}

	// 랜덤한 공을 삭제하는 함수
	void RemoveBall()
	{
		if (!HeadBall) return; // 리스트가 비어 있으면 리턴

		int TargetIndex = rand() % BallCount;
		UBall* Prev = nullptr;
		UBall* Current = HeadBall;

		for (int i = 0; i < TargetIndex; i++)
		{
			Prev = Current;
			Current = Current->NextBall;
		}

		// 첫 번째 공을 제거하는 경우
		if (!Prev)
		{
			HeadBall = Current->NextBall;
		}
		else
		{
			Prev->NextBall = Current->NextBall;
		}

		delete Current;
		BallCount--;
	}

	// 모든 공을 업데이트 (이동)
	void UpdateBalls()
	{
		UBall* Current = HeadBall;
		while (Current)
		{
			Current->Move();
			Current = Current->NextBall;
		}    
		for (UBall* ball1 = HeadBall; ball1 != nullptr; ball1 = ball1->NextBall) {
			for (UBall* ball2 = ball1->NextBall; ball2 != nullptr; ball2 = ball2->NextBall) {
				if (IsColliding(*ball1, *ball2)) {
					HandleCollision(*ball1, *ball2);
				}
			}
		}
	}

	// 모든 공을 렌더링
	void RenderBalls(URenderer* renderer)
	{
		UBall* Current = HeadBall;
		while (Current)
		{
			Current->Render(renderer);
			Current = Current->NextBall;
		}
	}

	// 메모리 해제
	void ClearBalls()
	{
		while (HeadBall)
		{
			UBall* Temp = HeadBall;
			HeadBall = HeadBall->NextBall;
			delete Temp;
		}
		BallCount = 0;
	}
	bool IsColliding(const UBall& ball1, const UBall& ball2) {
		FVector3 delta = ball1.Location - ball2.Location;
		float distanceSquared = delta.x * delta.x + delta.y * delta.y;  // 제곱 연산 최적화
		float radiusSum = ball1.Radius + ball2.Radius;
		return distanceSquared < (radiusSum * radiusSum);
	}

	void HandleCollision(UBall& ball1, UBall& ball2) {
		FVector3 delta = ball1.Location - ball2.Location;
		float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
		FVector3 normal = delta / distance;  // 정규화된 방향 벡터

		FVector3 relativeVelocity = ball1.Velocity - ball2.Velocity;
		float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

		if (velocityAlongNormal > 0) return;  // 이미 멀어지는 경우 충돌 처리 안 함

		float m1 = ball1.Mass;
		float m2 = ball2.Mass;

		// 탄성도 적용 (기존 e=1.0을 bounciness 변수로 변경)
		float impulse = (-(1 + bounciness) * velocityAlongNormal) / (1 / m1 + 1 / m2);
		FVector3 impulseVector = normal * impulse;

		ball1.Velocity += impulseVector / m1;
		ball2.Velocity -= impulseVector / m2;

		// 공 위치 보정 (겹쳐짐 방지)
		float overlap = (ball1.Radius + ball2.Radius) - distance;
		FVector3 correction = normal * (overlap / 2);
		ball1.Location += correction;
		ball2.Location -= correction;
	}



};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}
	switch (message)
	{
	case WM_DESTROY:
		// Signal that the app should quit
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	srand(time(NULL));
	// 윈도우 클래스 이름
	WCHAR WindowClass[] = L"JungleWindowClass";

	// 윈도우 타이틀바에 표시될 이름
	WCHAR Title[] = L"Game Tech Lab";

	// 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// 윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024 x 1024 크기에 윈도우 생성
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	// 각종 생성하는 코드를 여기에 추가합니다.
	// Renderer Class를 생성합니다.
	URenderer renderer;
	// D3D11 생성하는 함수를 호출합니다.
	renderer.Create(hWnd);
	// 렌더러 생성 직후에 쉐이더를 생성하는 함수를 호출합니다.
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	// 여기에서 ImGui를 생성합니다.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	UBallManager uBallManager;
	uBallManager.AddBall();
	// 실습2 마지막장에서 삼각형을 그릴 때 버텍스 버퍼 생성 하는 부분을 아래와 같이 수정합니다.
	// Vertex 배열 변수 이름이 triangle_vertices에서 cube_vertices로 변경된것 입니다.
	/*
		FVertexSimple* vertices = triangle_vertices;
		UINT ByteWidth = sizeof(triangle_vertices);
		UINT numVertices = sizeof(triangle_vertices) / sizeof(FVertexSimple);
	*/

	// 버텍스 버퍼로 넘기기 전에 Scale Down합니다.
	//float scaleMod = 0.1f;
	//UINT numVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);
	//UINT numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
	//UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);
	/*
	for (UINT i = 0; i < numVerticesSphere; ++i)
	{
		sphere_vertices[i].x *= scaleMod;
		sphere_vertices[i].y *= scaleMod;
		sphere_vertices[i].z *= scaleMod;
	}*/
	//ID3D11Buffer* vertexBufferTriangle = renderer.CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
	//ID3D11Buffer* vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
	// 여기에 추가합니다.

	// FPS 제한을 위한 설정
	const int targetFPS = 30;
	const double targetFrameTime = 1000.0 / targetFPS; // 한 프레임의 목표 시간 (밀리초 단위)

	// 고성능 타이머 초기화
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;

	// Main Loop (Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨)
	while (bIsExit == false)
	{
		QueryPerformanceCounter(&startTime);
		MSG msg;

		// 처리할 메시지가 더 이상 없을때 까지 수행
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 키 입력 메시지를 번역
			TranslateMessage(&msg);

			// 메시지를 적절한 윈도우 프로시저에 전달, 메시지가 위에서 등록한 WndProc 으로 전달됨
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
			else if (msg.message == WM_KEYDOWN) // 키보드 눌렸을 때
			{
				
			}
		}
		////////////////////////////////////////////
		// 매번 실행되는 코드를 여기에 추가합니다.
		// 준비 작업
		renderer.Prepare();
		renderer.PrepareShader();
		uBallManager.UpdateBalls();
		uBallManager.RenderBalls(&renderer);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Jungle Property Window"); 
		ImGui::Checkbox("Gravity", &bEnableGravity);
		ImGui::SameLine();
		ImGui::SliderFloat("Gravity Strength", &gravityStrength, -0.05f, 0.05f, "%.3f");
		Gravity.y = gravityStrength;  // 중력 값 적용
		int newBallCount = uBallManager.BallCount;
		ImGui::InputInt("Number of Balls", &newBallCount);
		if (newBallCount < 0) {
			newBallCount = 0;
		}
		//if (ImGui::Button("+"))	uBallManager.AddBall();
		// 현재 공 개수보다 크면 공 추가
		
		while (newBallCount > uBallManager.BallCount) {
			uBallManager.AddBall();
		}

		// 현재 공 개수보다 작으면 공 제거
		while (newBallCount < uBallManager.BallCount) {
			uBallManager.RemoveBall();
		}
		ImGui::SliderFloat("Bounciness", &bounciness, 0.0f, 1.0f, "%.2f");

		ImGui::Checkbox("Use Custom Color", &bUseCustomColor);
		if (bUseCustomColor) {
			ImGui::SliderFloat("R", &BallColor[0], 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("G", &BallColor[1], 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("B", &BallColor[2], 0.0f, 1.0f, "%.2f");
		}
		ImGui::End();

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// 현재 화면에 보여지는 버퍼와 그리기 작업을 위한 버퍼를 서로 교환합니다.
		renderer.SwapBuffer();
		////////////////////////////////////////////
		do
		{
			Sleep(0);

			// 루프 종료 시간 기록
			QueryPerformanceCounter(&endTime);

			// 한 프레임이 소요된 시간 계산 (밀리초 단위로 변환)
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;

		} while (elapsedTime < targetFrameTime);

	}
	uBallManager.ClearBalls();
	// 여기에서 ImGui 소멸
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	// D3D11 소멸 시키는 함수를 호출합니다.
	// 렌더러 소멸 직전에 쉐이더를 소멸 시키는 함수를 호출합니다.
	// 버텍스 버퍼 소멸은 Renderer 소멸전에 처리합니다.
	renderer.ReleaseVertexBuffer(vertexBufferSphere);

	renderer.ReleaseConstantBuffer();
	renderer.ReleaseShader();
	renderer.Release();

	return 1;
}
