/*
The Engine itself, it is responsible for the initialization of the systems/managers that I want the application
to inherently have. The application should not need to worry about creating the managers.
It is also responsible for the structuring / ordering of the updates in an application, e.g. HandleInputs() > Update() > Render()

It creates an engineContext so that the application can then easily access all systems on startup, and create pointers to any systems
that they will need during runtime. Having every system contained in a struct means that adding more systems/managers in the future
will not require any modifications to the application or engineLayer.
*/

#pragma once
#include <memory.h>
/////
#include "EngineContext.h"
#include "Application.h"

namespace NomadEngine {
	class EngineLayer
	{
	public:
		EngineLayer();
		~EngineLayer() = default;\

		bool Initialize();
		void Run();
		void Shutdown();

	private:
		bool processEvents();
		void update();
		void render();

		/// STUFF TO MOVE TO MANAGERS
		bool createRenderer(HWND hwnd); // Note: In the future this may need carefully handled by a platformManager of sorts, as HWND won't be relevant to non-windows OS platforms.

	private:
		std::unique_ptr<NomadEngine::Application> application = nullptr; // The actual game
		EngineContext engineContext;

		const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
		const float SCREEN_DEPTH = 1000.0f;
		const float SCREEN_NEAR = 0.3f;
	};
}
