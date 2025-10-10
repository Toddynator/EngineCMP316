#pragma once
#include "../System.h"

namespace CMP316engine {
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(entt::registry* sceneRegistry) ;

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;
	};
}