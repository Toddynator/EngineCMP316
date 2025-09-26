/*
Base class that games utilizing the engine will inherit.
*/

#pragma once
#include <memory>

namespace CMP316engine {
	class Application
	{
	public:
		Application() {}
		~Application() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput() = 0;
		virtual void HandleImgui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
	};

	//extern std::unique_ptr<Application> CreateApp(int argc, char** argv); // From Lab 2 // If I were to static link
}