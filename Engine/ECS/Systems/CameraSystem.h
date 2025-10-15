#pragma once
#include "../System.h"
#include "Managers/WindowManager.h"

namespace CMP316engine {
	class CameraSystem : public System
	{
	private:
		WindowManager* windowManager = nullptr;

		entt::entity activeCamera = entt::null;
		inline static constexpr float BASE_CAMERA_ROTATION_SPEED = 40.f;

	public:
		CameraSystem(entt::registry* sceneRegistry, EngineContext* engineContext);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput(float deltaTime) override;
		void Update(float deltaTime) override;

		static DirectX::XMMATRIX GetActiveCameraViewMatrix(entt::registry* sceneRegistry);

	private:
		// If there is no active camera in the scene, it will return a default view matrix instead.
		static DirectX::XMMATRIX CameraSystem::createDefaultViewMatrix();

		void move_forward(TransformComponent& transforms, float deltaTime);
		void move_backward(TransformComponent& transforms, float deltaTime);
		void move_left(TransformComponent& transforms, float deltaTime);
		void move_right(TransformComponent& transforms, float deltaTime);
		void move_up(TransformComponent& transforms, float deltaTime);
		void move_down(TransformComponent& transforms, float deltaTime);
		void roll_anti_clockwise(TransformComponent& transforms, float deltaTime);
		void roll_clockwise(TransformComponent& transforms, float deltaTime);
		void zoom_in(TransformComponent& transforms);
		void zoom_out(TransformComponent& transforms);
	};
}