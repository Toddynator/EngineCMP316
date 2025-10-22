#pragma once
#include "../System.h"

namespace CMP316engine {
	class TransformSystem
		: public System
	{
	public:
		TransformSystem(entt::registry* sceneRegistry, CMP316engine::EngineContext* engineContext) : System(sceneRegistry, engineContext) {}

		bool Initialize() override { return true; }
		void Shutdown() override {}

		void Update(float deltaTime) override;

	private:
		void calculateWorldMatrix(TransformComponent& transformComponent, TransformComponent* parentTransformComponent);
		DirectX::XMMATRIX calculateRotationMatrix(TransformComponent& transformComponent);
	};
}