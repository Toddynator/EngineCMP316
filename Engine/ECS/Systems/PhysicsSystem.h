/*
System that handles and interfaces with the Engine Physics Manager, 
it updates the physics bodies of the underlying physics system and handles
communication from the physics body to the components that need it.
e.g. Transform Component is updated to the position of the physics body.
On initialization transform component is used for the bodies starting transforms.
*/

#pragma once
#include "../System.h"
#include "Managers/PhysicsManager.h"

namespace NomadEngine {
	class PhysicsSystem : public System
	{
	private:
		NomadEngine::PhysicsManager* physicsManager;

	public:
		PhysicsSystem(entt::registry* sceneRegistry, NomadEngine::EngineContext* engineContext);

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;
	};
}