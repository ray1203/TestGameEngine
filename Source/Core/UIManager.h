#pragma once
#include <vector>
#include <string>
#include <windows.h>

class URenderer;
class UBall;
class UCamera;
class UIManager {
public:
    static UIManager& GetInstance();
    static URenderer* Renderer;
    static UBall* CurrentObject;
    static void Initialize(URenderer& renderer);
    static void Render(HWND hWnd);


private:
    UIManager() = default;
    ~UIManager() = default;


    static void ShowExampleControlPanel();
    static void ShowExamplePropertyWindow(HWND hWnd);

};
