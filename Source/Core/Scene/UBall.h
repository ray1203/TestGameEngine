#pragma once
#include "UObject.h"
#include "Renderer/URenderer.h"
#include "UCamera.h"

class UBall : public UObject
{
private:


public:
	UBall();

	virtual void Initialize(URenderer& renderer) override;
	//virtual void Render(URenderer& renderer) override;
	//virtual void Update(URenderer& renderer) override;
	void Move();
	void ApplyGravity();
	void CheckBorder();
	FVector Velocity;
	//virtual void MVP() override;  // MVP 오버라이드 선언

};
