#pragma once
#include "Components.h"
#include "entt.hpp"
#include "Core/EngineContext.h"

/*
ABSTRACT BASE CLASS
Base System that ALL system classes should inherit.
ANY FUNCTIONS outside of the ones listed in this class should be either private or protected, YOU SHOULD NEVER be using systems outside of the scenes system container.
*/

namespace CMP316engine {
	class System
	{
	protected:
		entt::registry* registry;
		InputManager* inputManager;

	public:
		System(entt::registry* sceneRegistry, EngineContext* engineContext) : registry(sceneRegistry), inputManager(engineContext->inputManager.get()) {}
		System() = delete;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput(float deltaTime) {};
		virtual void HandleImGui() {};
		virtual void Update(float deltaTime) = 0;
	};
}

