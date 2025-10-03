/*
Base class that games utilizing the engine will inherit.
*/

#pragma once
#include <memory>
//#include "EngineContext.h"

namespace CMP316engine {
	class Application
	{
	public:
		//Application(EngineContext& context) : engineContext(context) {}
		Application() {}
		~Application() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput() = 0;
		virtual void HandleImgui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

	private:
		//EngineContext& engineContext;
	};

	//extern std::unique_ptr<Application> CreateApp(EngineContext& engineContext); // This is how the Game links to the Engine! The engine does not need to know anything about the game.
	extern std::unique_ptr<Application> CreateApp();
}