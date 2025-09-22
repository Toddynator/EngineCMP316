
#include <iostream>
#include <Windows.h>
#include <filesystem>

#include <SDL3/SDL.h>

#include <../ImGui/imgui.h>
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_dx11.h"

#include "d3dclass.h" // RENDERER
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "InputManager.h"

/// WINDOW SETTINGS ///

class EngineLayer
{
public:
	EngineLayer();
	~EngineLayer() = default;

	bool Initialize();
	void Run();
	void Shutdown();

private:
	bool pollEvent(SDL_Event* event);

	// STUFF TO MOVE
	bool createWindow();
	bool createRenderer(HWND hwnd);

private:
	SDL_Window* window = nullptr;
	D3DClass* renderer = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	ColorShaderClass* m_ColorShader = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	InputManager* inputManager = nullptr;

	bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

