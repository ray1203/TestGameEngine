#include "UBall.h"
#include "Core/Renderer/URenderer.h"
#include "Core/common.h"
#include <iostream>
#include <cassert>

#include "Vertices.h"



UBall::UBall()
    : Velocity(FVector(0, 0, 0))
{
    Translation = FVector(0, 0, 0);
    Scale = FVector(0.5f, 0.5f, 0.5f);
    Rotation = FVector(0, 0, 0);

    numVertices = sizeof(sphere_vertices) / sizeof(FVertexSimple);
}
void UBall::Initialize(URenderer& renderer)
{
    // 부모 클래스(UObject)의 기본적인 초기화 수행
    UObject::Initialize(renderer);

    // 추가적으로 공(Ball)만의 초기화 작업 수행
    Velocity = FVector(0.1f, 0.1f, 0.0f); // 기본 속도 설정

    // 공의 정점 데이터 로드
    VertexBuffer = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));
    numVertices = sizeof(sphere_vertices) / sizeof(FVertexSimple);

    if (!VertexBuffer) 
        std::cout << "VertexBuffer is nullptr!" << std::endl;

    std::cout << "UBall Initialized with " << numVertices << " vertices." << std::endl;
}



void UBall::CheckBorder()
{
    // 경계 체크 (충돌 처리 가능)
}

void UBall::Move()
{
    ApplyGravity();
    Translation.X += Velocity.X;
    Translation.Y += Velocity.Y;
    CheckBorder();
}

void UBall::ApplyGravity()
{
    // 중력 적용 (필요 시 추가)
}


