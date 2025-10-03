/*
Contains all the systems / managers that I want the application to be able to access.
The Engine should handle the Initialization of the struct.
The Application should separate into smaller contexts when a particular manager is not needed for a section of code.
E.g. UIContext, EnemyContext, etc.

IMPORTANT:
- Managers that use third party libraries and API's SHOULD BE Abstracted! This means the Game will not 
need to include the additional dependencies, and only needs to worry about including the engine files.
EngineContext should only be exposing interfaces. Even if I have a library that I don't plan to ever swap out, it should be abstracted so
that the header file does not expose any third party types.
- DO NOT include implementations of managers in this file. The game will include engineContext and will not have access to the files for specific implementations.
Only include abstractions.
*/

#pragma once
#include <memory>
#include "../Managers/InputManager.h"
#include "../Managers/TimeManager.h"
#include "../Managers/AudioManager.h"
#include "../Managers/WindowManager.h"
#include "../Managers/RenderManager.h"
#include "../Managers/PhysicsManager.h"
/////
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

		std::unique_ptr<Renderer_DirectX11> renderer = nullptr; // TODO: Make RenderManager ~ Should not be exposing ANY directX types.
		std::unique_ptr<Shader> shader = nullptr; // TODO: Make AssetManager and Move to AssetManager
	};
}

