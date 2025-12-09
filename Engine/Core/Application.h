/*
ABSTACT CLASS
Games utilizing the engine will inherit this and define the CreateApp Function with their own implementation.
May have some default implementations that game projects can choose to utilize. E.g. Common Inputs like Fullscreen

It takes a reference to engineContext but this may need replaced with only specific systems that are required by the application,
since ideally everything that needs a particular manager will have gotten a pointer on initialisation, and therefore doesn't need
the context struct any longer.
*/

#pragma once
#include <memory>
#include "EngineContext.h"
#include "Scenes/Scene.h"

namespace CMP316engine {
	class Application
	{
	public:
		explicit Application(EngineContext& context) : engineContext(context) {}
		Application() = delete; // MUST pass the context in ~ Otherwise Game will have no access to the engines systems
		~Application() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput(float deltaTime) = 0;
		virtual void HandleImGui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

	protected:
		EngineContext& engineContext;
	};

	extern std::unique_ptr<Application> CreateApp(EngineContext& engineContext); // This is how the Game links to the Engine! The great divide between the engine and the game.
}