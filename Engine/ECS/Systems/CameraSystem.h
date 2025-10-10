#pragma once
#include "../System.h"

namespace CMP316engine {
	class CameraSystem : public System
	{
	public:
		CameraSystem(entt::registry* sceneRegistry);

		bool Initialize() override;
		void Shutdown() override;

		void Update(float deltaTime) override;

		static DirectX::XMMATRIX GetActiveCameraViewMatrix(entt::registry* sceneRegistry);

	private:
		// If there is no active camera in the scene, it will return a default view matrix instead.
		static DirectX::XMMATRIX CameraSystem::createDefaultViewMatrix();
	};
}