#include "pch.h"
#include "CameraSystem.h"

namespace CMP316engine {

	CameraSystem::CameraSystem(entt::registry* sceneRegistry, InputManager* sceneInputManager) : System(sceneRegistry, sceneInputManager)
	{

	}

	bool CameraSystem::Initialize()
	{
		return true;
	}

	void CameraSystem::Shutdown()
	{

	}

	void CameraSystem::HandleInput(float deltaTime)
	{
		if (activeCamera == entt::null || !registry->any_of<TransformComponent>(activeCamera)) { return; }
		if (!registry->valid(activeCamera)) { activeCamera = entt::null; }

		auto& transformComponent = registry->get<TransformComponent>(activeCamera);

		/// CAMERA TURNING

		float mouseDeltaX = inputManager->GetMouseDeltaX();
		float mouseDeltaY = inputManager->GetMouseDeltaY();
		if (inputManager->IsMouseButtonDown(SDL_BUTTON_RIGHT) && (mouseDeltaX != 0.f || mouseDeltaY != 0.f))
		{		
			transformComponent.rotation.x += mouseDeltaY * deltaTime * BASE_CAMERA_ROTATION_SPEED;
			transformComponent.rotation.y += mouseDeltaX * deltaTime * BASE_CAMERA_ROTATION_SPEED;
		}

		/// LATERAL CAMERA MOVEMENT

		if (inputManager->IsKeyBindingDown("Move Forward"))
		{
			move_forward(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Move Backward"))
		{
			move_backward(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Move Left"))
		{
			move_left(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Move Right"))
		{
			move_right(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Move Up"))
		{
			move_up(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Move Down"))
		{
			move_down(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Roll Anti-Clockwise"))
		{
			roll_anti_clockwise(transformComponent, deltaTime);
		}
		if (inputManager->IsKeyBindingDown("Roll Clockwise"))
		{
			roll_clockwise(transformComponent, deltaTime);
		}
		/*if ()
		{
			zoom_in(transformComponent);
		}
		if ()
		{
			zoom_out(transformComponent);
		}*/
	}

	void CameraSystem::Update(float deltaTime)
	{
		auto cameraEntities = registry->view<CameraComponent, TransformComponent>();
		for (auto& entity : cameraEntities) {
			auto [cameraComponent, transformComponent] = registry->get<CameraComponent, TransformComponent>(entity);

			/*
			With DirectX math, its better to do calculations using their Vector variable instead of the float variable.
			Apparently. I need to profile this.
			I guess they had a valid reason not to use operators for vector math.
			*/

			if (cameraComponent.active) { activeCamera = entity; }

			// Load into XMVECTOR structures.
			DirectX::XMVECTOR upVector = XMLoadFloat3(&transformComponent.up);
			DirectX::XMVECTOR positionVector = XMLoadFloat3(&transformComponent.position);
			//DirectX::XMVECTOR forwardVector = XMLoadFloat3(&transformComponent.forward);
			//DirectX::XMVECTOR lookAtVector = DirectX::XMVectorAdd(forwardVector, positionVector);
			DirectX::XMVECTOR lookAtVector = XMLoadFloat3(&transformComponent.forward);

			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
			float pitch = transformComponent.rotation.x * 0.0174532925f;
			float yaw = transformComponent.rotation.y * 0.0174532925f;
			float roll = transformComponent.rotation.z * 0.0174532925f;

			// Create the rotation matrix from the yaw, pitch, and roll values.
			DirectX::XMMATRIX rotationMatrix;
			rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

			// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
			lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
			upVector = XMVector3TransformCoord(upVector, rotationMatrix);

			// Translate the rotated camera position to the location of the viewer.
			lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

			// Finally create the view matrix from the three updated vectors.
			cameraComponent.viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
		}
	}

	DirectX::XMMATRIX CameraSystem::GetActiveCameraViewMatrix(entt::registry* sceneRegistry)
	{
		auto cameraEntities = sceneRegistry->view<CameraComponent>();
		for (auto& entity : cameraEntities) {
			auto& cameraComponent = sceneRegistry->get<CameraComponent>(entity);

			if (cameraComponent.active) { return cameraComponent.viewMatrix; }
		}

		//std::cout << "\nNo Active Camera Found";
		return createDefaultViewMatrix(); // Return default matrix
	}

	DirectX::XMMATRIX CameraSystem::createDefaultViewMatrix()
	{
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, -5.0f);
		DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
		DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.f, 0.f, 1.f);

		DirectX::XMVECTOR positionVector = XMLoadFloat3(&position);
		DirectX::XMVECTOR upVector = XMLoadFloat3(&up);
		DirectX::XMVECTOR lookAtVector = XMLoadFloat3(&forward);

		float pitch = 0.f;
		float yaw = 0.f;
		float roll = 0.f;
		DirectX::XMMATRIX rotationMatrix;

		rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
		upVector = XMVector3TransformCoord(upVector, rotationMatrix);
		lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);
		return DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	}

	//////////// Controls //////

	void CameraSystem::move_forward(TransformComponent& transforms, float deltaTime)
	{
		/*float radians = DirectX::XMConvertToRadians(transforms.rotation.y);
		transforms.position.x += sinf(radians) * deltaTime;
		transforms.position.z += cosf(radians) * deltaTime;*/

		DirectX::XMVECTOR forwardVector = DirectX::XMLoadFloat3(&transforms.forward);
		forwardVector = DirectX::XMVectorScale(forwardVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(forwardVector, positionVector));
	}

	void CameraSystem::move_backward(TransformComponent& transforms, float deltaTime)
	{
		DirectX::XMVECTOR backwardVector = DirectX::XMLoadFloat3(&transforms.forward);
		backwardVector = DirectX::XMVectorScale(backwardVector, -1); // Convert from front to backward
		backwardVector = DirectX::XMVectorScale(backwardVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(backwardVector, positionVector));
	}

	void CameraSystem::move_left(TransformComponent& transforms, float deltaTime)
	{
		DirectX::XMVECTOR leftVector = DirectX::XMLoadFloat3(&transforms.right);
		leftVector = DirectX::XMVectorScale(leftVector, -1); // Convert from right to left
		leftVector = DirectX::XMVectorScale(leftVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(leftVector, positionVector));
	}

	void CameraSystem::move_right(TransformComponent& transforms, float deltaTime)
	{
		DirectX::XMVECTOR rightVector = DirectX::XMLoadFloat3(&transforms.right);
		rightVector = DirectX::XMVectorScale(rightVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(rightVector, positionVector));
	}

	void CameraSystem::move_up(TransformComponent& transforms, float deltaTime)
	{
		DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&transforms.up);
		upVector = DirectX::XMVectorScale(upVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(upVector, positionVector));
	}

	void CameraSystem::move_down(TransformComponent& transforms, float deltaTime)
	{
		DirectX::XMVECTOR downVector = DirectX::XMLoadFloat3(&transforms.up);
		downVector = DirectX::XMVectorScale(downVector, -1); // Convert from Up to Down Vector
		downVector = DirectX::XMVectorScale(downVector, deltaTime);
		DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&transforms.position);
		DirectX::XMStoreFloat3(&transforms.position, DirectX::XMVectorAdd(downVector, positionVector));
	}

	void CameraSystem::roll_anti_clockwise(TransformComponent& transforms, float deltaTime)
	{
		transforms.rotation.z += deltaTime * BASE_CAMERA_ROTATION_SPEED;
	}

	void CameraSystem::roll_clockwise(TransformComponent& transforms, float deltaTime)
	{
		transforms.rotation.z -= deltaTime * BASE_CAMERA_ROTATION_SPEED;
	}

	void CameraSystem::zoom_in(TransformComponent& transforms)
	{

	}

	void CameraSystem::zoom_out(TransformComponent& transforms)
	{

	}
}