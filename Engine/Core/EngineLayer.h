/*

*/

#pragma once
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <memory.h>

#include "Application.h"

#include "../Managers/InputManager.h"
#include "../Managers/TimeManager.h"
#include "../Managers/AudioManager.h"
#include "../Managers/AudioManager_SoLoud.h"
#include "../Managers/WindowManager.h"
#include "../Managers/WindowManager_SDL.h"
#include "../Managers/RenderManager.h"
#include "../Managers/PhysicsManager.h"

#include <SDL3/SDL.h>

#include <ImGui.h>
#include "../ImGui/imgui_impl_sdl3.h"
#include "../ImGui/imgui_impl_dx11.h"

#include "../Graphics/Renderer_DirectX11.h" // RENDERER
#include "../Graphics/Camera.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shader.h"



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
	std::unique_ptr<CMP316engine::PhysicsManager> physicsManager = nullptr;
	//std::unique_ptr<CMP316engine::RenderManager> renderManager = nullptr;

	/// GRAPHICS
	// NOTE: Move to Application class?
	std::unique_ptr<Renderer_DirectX11> renderer = nullptr;
	std::unique_ptr<Shader> shader = nullptr;

	/// SCENE
	// NOTE: Temp, should be in scene class only
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<CMP316engine::Model> model = nullptr;

	/// TESTING VARIABLES
	// NOTE: Temp, just for testing
	bool audioPlayed = false;
	JPH::BodyID modelPhysicsBodyID;

	bool wireframeEnabled = false;
	const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.3f;
};

