#pragma once
#include "../System.h"
#include "Managers/PhysicsManager.h"

namespace CMP316engine {
	class PhysicsSystem : public System
	{
	private:
		CMP316engine::PhysicsManager* physicsManager;

	public:
		PhysicsSystem(entt::registry* sceneRegistry, CMP316engine::PhysicsManager* scenePhysicsManager);

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;
	};
}