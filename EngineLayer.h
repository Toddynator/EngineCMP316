
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <memory.h>

#include "InputManager.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "EventManager_SDL.h"
#include "WindowManager.h"

#include <SDL3/SDL.h>

#include <../ImGui/imgui.h>
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_dx11.h"

#include "d3dclass.h" // RENDERER
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"

/// WINDOW SETTINGS ///

#pragma once
class EngineLayer
{
public:
	EngineLayer();
	~EngineLayer() = default;

	bool Initialize();
	void Run();
	void Shutdown();

private:
	// STUFF TO MOVE TO MANAGERS
	bool processEvents();
	bool createWindow();
	bool createRenderer(HWND hwnd); // Note: In the future this may need carefully handled by a platformManager of sorts, as HWND won't be relevant to non-windows OS platforms.

private:
	// Managers
	InputManager* inputManager = nullptr;
	TimeManager* timeManager = nullptr;
	std::unique_ptr<CMP316engine::EventManager> eventManager = nullptr;
	//windowManager
	//rendererManager

	SDL_Window* window = nullptr;
	D3DClass* renderer = nullptr;

	// Graphics
	ColorShaderClass* m_ColorShader = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;

	// Scene objects
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;	

	bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

