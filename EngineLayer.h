
#pragma once
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <memory.h>

#include "Application.h"
#include "TempApplication.h"

#include "InputManager.h"
#include "TimeManager.h"
#include "AudioManager.h"
#include "AudioManager_SoLoud.h"
#include "WindowManager.h"
#include "WindowManager_SDL.h"
#include "RenderManager.h"

#include <SDL3/SDL.h>

#include <../ImGui/imgui.h>
#include "ImGui/imgui_impl_sdl3.h"
#include "ImGui/imgui_impl_dx11.h"

#include "d3dclass.h" // RENDERER
#include "Camera.h"
#include "Model.h"
#include "Shader.h"



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
	std::unique_ptr<CMP316engine::Application> application = nullptr; // The actual game

	/// MANAGERS
	std::unique_ptr<CMP316engine::InputManager> inputManager = nullptr;
	std::unique_ptr<CMP316engine::TimeManager> timeManager = nullptr;
	std::unique_ptr<CMP316engine::WindowManager> windowManager = nullptr;
	std::unique_ptr<CMP316engine::AudioManager> audioManager = nullptr;
	//std::unique_ptr<CMP316engine::RenderManager> renderManager = nullptr;

	/// GRAPHICS
	// NOTE: Move to Application class?
	std::unique_ptr<D3DClass> renderer = nullptr;
	std::unique_ptr<Shader> shader = nullptr;

	/// SCENE
	// NOTE: Temp, should be in scene class only
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<CMP316engine::Model> model = nullptr;

	/// SOUND
	// NOTE: Temp, just for testing
	bool audioPlayed = false;

	bool wireframeEnabled = false;
	const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

