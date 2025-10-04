/*
ABSTACT CLASS
Games utilizing the engine will inherit this and define the CreateApp Function with their own implementation.
May have some default implementations that game projects can choose to utilize. E.g. Common Inputs like Fullscreen
*/

#pragma once
#include <memory>
#include "EngineContext.h"
#include "Scene.h"

namespace CMP316engine {
	class Application
	{
	public:
		explicit Application(EngineContext& context) : engineContext(context) {}
		Application() = delete; // MUST pass the context in ~ Otherwise Game will have no access to the engines systems
		~Application() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput() = 0;
		virtual void HandleImgui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

	protected:
		EngineContext& engineContext;
		std::unique_ptr<Scene> activeScene = nullptr; // REPLACE WITH SCENE MANAGER, IT WILL STORE AN ACTIVE SCENE UNIQUE_PTR THAT APPLICATION CAN GET.
	};

	extern std::unique_ptr<Application> CreateApp(EngineContext& engineContext); // This is how the Game links to the Engine! The great divide between the engine and the game.
}