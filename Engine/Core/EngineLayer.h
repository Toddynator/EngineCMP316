/*
The Engine itself, it is responsible for the initialization of the systems/managers that I want the application
to inherently have. The application should not need to worry about creating the managers.
It is also responsible for the structuring / ordering of the updates in an application, e.g. HandleInputs() > Update() > Render()
*/

#pragma once
#include <memory.h>
/////
#include "EngineContext.h"
#include "Application.h"
/////
#include "../Graphics/Camera.h"
#include "../Graphics/Model.h"

namespace CMP316engine {
	class EngineLayer
	{
	public:
		EngineLayer();
		~EngineLayer() = default;

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
		std::unique_ptr<CMP316engine::Application> application = nullptr; // The actual game
		EngineContext engineContext;



		/// SCENE
		// NOTE: Temp, should be in scene class only
		std::unique_ptr<Camera> camera = nullptr;
		std::unique_ptr<CMP316engine::Model> model = nullptr;

		/// TESTING VARIABLES
		// NOTE: Temp, just for testing
		//JPH::BodyID modelPhysicsBodyID;



		bool wireframeEnabled = false;
		const bool VSYNC_ENABLED = false; // Locks to 60fps ~ Renderer
		const float SCREEN_DEPTH = 1000.0f;
		const float SCREEN_NEAR = 0.3f;
	};
}
