#pragma once
#include "../System.h"

namespace CMP316engine {
	class TransformSystem
		: public System
	{
	public:
		TransformSystem(entt::registry* sceneRegistry, InputManager* sceneInputManager) : System(sceneRegistry, sceneInputManager) {}

		bool Initialize() override { return true; }
		void Shutdown() override {}

		void Update(float deltaTime) override;
	};
}