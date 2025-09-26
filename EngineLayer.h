
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <memory.h>

#include "InputManager.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "EventManager_SDL.h"
#include "WindowManager.h"
#include "WindowManager_SDL.h"
#include "RenderManager.h"

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
	EngineLayer() {};
	~EngineLayer() = default;

	bool Initialize();
	void Run();
	void Shutdown();

private:
	bool processEvents();
	void Update();
	void Render();

	/// STUFF TO MOVE TO MANAGERS
	bool createRenderer(HWND hwnd); // Note: In the future this may need carefully handled by a platformManager of sorts, as HWND won't be relevant to non-windows OS platforms.

private:
	/// Managers
	std::unique_ptr<CMP316engine::InputManager> inputManager = nullptr;
	std::unique_ptr<CMP316engine::TimeManager> timeManager = nullptr;
	std::unique_ptr<CMP316engine::EventManager> eventManager = nullptr;
	std::unique_ptr<CMP316engine::WindowManager> windowManager = nullptr;
	//std::unique_ptr<CMP316engine::RenderManager> renderManager = nullptr;

	/// Graphics
	std::unique_ptr<D3DClass> renderer = nullptr;
	std::unique_ptr<ColorShaderClass> colorShader = nullptr;
	std::unique_ptr<TextureShaderClass> textureShader = nullptr;

	/// Scene objects
	std::unique_ptr<CameraClass> camera = nullptr;
	std::unique_ptr<ModelClass> model = nullptr;

	const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

