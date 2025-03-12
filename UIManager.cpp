#include "UIManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "UCamera.h"
#include "UBall.h"
URenderer* UIManager::Renderer = nullptr;
UCamera* UIManager::Camera = nullptr;
UBall* UIManager::CurrentObject = nullptr;



UIManager& UIManager::GetInstance() {
	static UIManager instance;
	return instance;
}

void UIManager::Initialize(URenderer& renderer,UCamera& camera) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	Renderer = &renderer;
	Camera = &camera;
}

void UIManager::Render(HWND hWnd) {
	ImGui::NewFrame();

	ShowExampleControlPanel();
	ShowExamplePropertyWindow(hWnd);

	ImGui::Render();
}

void UIManager::ShowExampleControlPanel() {
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver); 

	ImGui::Begin("Jungle Control Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);

	
	ImGui::Checkbox("Orthogonal", &Camera->bIsOrthogonal);
	ImGui::SliderFloat("Fov", &Camera->FOV, 5.f, 120.f);
	ImGui::SliderFloat3("Camera Location", &Camera->CameraPosition.X, -2.0f,
		2.0f);
	ImGui::SliderFloat3("Camera Rotation", &Camera->CameraDir.X, -1.0f,
		1.0f);

	ImGui::Separator();

	ImGui::End();
}


void UIManager::ShowExamplePropertyWindow(HWND hWnd) {
	ImGui::SetNextWindowPos(ImVec2(700, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_FirstUseEver);

	ImGui::Begin("Jungle Property Window",nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Object Properties");

	if (CurrentObject) {
		ImGui::SliderFloat3("Velocity", &CurrentObject->Velocity.X, -10.0f, 10.0f);
		ImGui::SliderFloat3("Translation", &CurrentObject->Translation.X, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotation", &CurrentObject->Rotation.X, -180.0f, 180.0f);
		ImGui::SliderFloat3("Scale", &CurrentObject->Scale.X, 0.1f, 5.0f);
	}
	else {
		ImGui::Text("No object selected.");
	}

	ImGui::End();
}

/*
void UIManager::SetCubeData() {

	Cube->CubeTranslation = UIManager::GetInstance().position;
	Cube->CubeRotation = UIManager::GetInstance().rotation;
	Cube->CubeScale = UIManager::GetInstance().scale;

}*/