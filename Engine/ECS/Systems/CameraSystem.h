#pragma once
#include "../System.h"

namespace CMP316engine {
	class CameraSystem : public System
	{
	private:
		entt::entity activeCamera = entt::null;
		inline static constexpr float BASE_CAMERA_ROTATION_SPEED = 1.f;

	public:
		CameraSystem(entt::registry* sceneRegistry, InputManager* sceneInputManager);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput(float deltaTime) override;
		void Update(float deltaTime) override;

		static DirectX::XMMATRIX GetActiveCameraViewMatrix(entt::registry* sceneRegistry);

	private:
		// If there is no active camera in the scene, it will return a default view matrix instead.
		static DirectX::XMMATRIX CameraSystem::createDefaultViewMatrix();
	};
}