/*
Contains all the systems / managers that I want the application to be able to access.
The Engine should handle the Initialization of the struct.
The Application should separate into smaller contexts when a particular manager is not needed for a section of code.
E.g. UIContext, EnemyContext, etc.
*/

#pragma once
#include <memory>
#include "../Managers/InputManager.h"
#include "../Managers/TimeManager.h"
#include "../Managers/AudioManager.h"
#include "../Managers/AudioManager_SoLoud.h"
#include "../Managers/WindowManager.h"
#include "../Managers/WindowManager_SDL.h"
#include "../Managers/RenderManager.h"
#include "../Managers/PhysicsManager.h"
#include "../Graphics/Renderer_DirectX11.h" // RENDERER
#include "../Graphics/Shader.h"

namespace CMP316engine {
	struct EngineContext
	{
		std::unique_ptr<InputManager> inputManager = nullptr;
		std::unique_ptr<TimeManager> timeManager = nullptr;
		std::unique_ptr<WindowManager> windowManager = nullptr;
		std::unique_ptr<AudioManager> audioManager = nullptr;
		std::unique_ptr<PhysicsManager> physicsManager = nullptr;

		std::unique_ptr<Renderer_DirectX11> renderer = nullptr; // TODO: Make RenderManager
		std::unique_ptr<Shader> shader = nullptr; // TODO: Make AssetManager and Move to AssetManager
	};
}

