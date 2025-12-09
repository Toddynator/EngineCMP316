/*
Handles logic in a scene for the active camera.
It is used for determining the active camera and for controlling it.
*/

#pragma once
#include "../System.h"
#include "Managers/WindowManager.h"

namespace CMP316engine {
	class CameraSystem : public System
	{
	private:
		WindowManager* windowManager = nullptr;

		entt::entity activeCamera = entt::null;
		inline static constexpr float BASE_CAMERA_ROTATION_SPEED = 100.f;
		inline static float currentCameraSpeed = 1.5f;
		inline static constexpr float BASE_CAMERA_MOVEMENT_SPEED = 1.5f;
		inline static constexpr float BASE_CAMERA_SPRINT_MOVEMENT_SPEED = 10.f;

	public:
		CameraSystem(entt::registry* sceneRegistry, EngineContext* engineContext);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput(float deltaTime) override;
		void Update(float deltaTime) override;

		static DirectX::XMMATRIX GetActiveCameraViewMatrix(entt::registry* sceneRegistry);
		static DirectX::XMFLOAT3 GetActiveCameraPosition(entt::registry* sceneRegistry);

	private:
		// If there is no active camera in the scene, it will return a default view matrix instead.
		static DirectX::XMMATRIX createDefaultViewMatrix();
		
		void calculateCameraViewMatrix(TransformComponent& transforms, CameraComponent& cameraComponent);

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