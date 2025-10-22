#include "pch.h"
#include "CameraSystem.h"

namespace CMP316engine {

	CameraSystem::CameraSystem(entt::registry* sceneRegistry, CMP316engine::EngineContext* engineContext) : System(sceneRegistry, engineContext), windowManager(engineContext->windowManager.get())
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
		//if (activeCamera == entt::null || !registry->any_of<TransformComponent>(activeCamera)) { return; }
		//if (!registry->valid(activeCamera)) { activeCamera = entt::null; }

		auto editorCameraEntities = registry->view<LevelEditorCameraComponent>();
		for (auto& entity : editorCameraEntities) {
			auto& levelEditorCameraComponent = registry->get<LevelEditorCameraComponent>(entity);
			auto& transformComponent = registry->get<TransformComponent>(entity);

			if (!levelEditorCameraComponent.canMove) { continue; }

			/// CAMERA TURNING

			float mouseDeltaX = inputManager->GetMouseDeltaX();
			float mouseDeltaY = inputManager->GetMouseDeltaY();
			if (inputManager->IsMouseButtonPressed(SDL_BUTTON_RIGHT))
			{
				inputManager->SaveCurrentMouseWindowPosition();
			}
			if (inputManager->IsMouseButtonDown(SDL_BUTTON_RIGHT) && (mouseDeltaX != 0.f || mouseDeltaY != 0.f))
			{		
				/// KEEPS READING MOUSE INPUT EVEN AFTER REACHING WINDOW EDGE.
				inputManager->SetWindowRelativeMouseMode(static_cast<SDL_Window*>(windowManager->GetNativeWindow()), true);
				inputManager->SetMouseToSavedPosition(static_cast<SDL_Window*>(windowManager->GetNativeWindow()));

				/// TURN CAMERA
				transformComponent.rotation.x += -mouseDeltaY * deltaTime * BASE_CAMERA_ROTATION_SPEED;
				transformComponent.rotation.y += -mouseDeltaX * deltaTime * BASE_CAMERA_ROTATION_SPEED;
			}
			else if (inputManager->IsMouseButtonReleased(SDL_BUTTON_RIGHT))
			{
				inputManager->SetWindowRelativeMouseMode(static_cast<SDL_Window*>(windowManager->GetNativeWindow()), false);
				//inputManager->SetMouseToSavedPosition(static_cast<SDL_Window*>(windowManager->GetNativeWindow()));
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
	}

	void CameraSystem::Update(float deltaTime)
	{
		auto cameraEntities = registry->view<CameraComponent, TransformComponent>();
		for (auto& entity : cameraEntities) {
			auto [cameraComponent, transformComponent] = registry->get<CameraComponent, TransformComponent>(entity);

			if (cameraComponent.active) { activeCamera = entity; }

			calculateCameraViewMatrix(transformComponent, cameraComponent);
		}
	}

	DirectX::XMMATRIX CameraSystem::GetActiveCameraViewMatrix(entt::registry* sceneRegistry)
	{
		/*
		Prioritize always using levelEditorCamera if it exists, otherwise whatever camera is active (ideally should only be one, but
		otherwise it always picks the first active camera it finds. If no camera is active then it returns a default view matrix.
		*/

		auto editorCameraEntities = sceneRegistry->view<LevelEditorCameraComponent>();
		for (auto& entity : editorCameraEntities) {
			// Just assume levelEditorCamera always has a camera component (since this is created ONCE by the engine).
			auto& cameraComponent = sceneRegistry->get<CameraComponent>(entity);
			return cameraComponent.viewMatrix;
		}

		auto cameraEntities = sceneRegistry->view<CameraComponent>();
		for (auto& entity : cameraEntities) {
			auto& cameraComponent = sceneRegistry->get<CameraComponent>(entity);

			if (cameraComponent.active) { return cameraComponent.viewMatrix; }
		}

		//std::cout << "\nNo Active Camera Found";
		return createDefaultViewMatrix(); // Return default matrix
	}

	DirectX::XMFLOAT3 CameraSystem::GetActiveCameraPosition(entt::registry* sceneRegistry)
	{
		auto editorCameraEntities = sceneRegistry->view<LevelEditorCameraComponent>();
		for (auto& entity : editorCameraEntities) {
			auto& transformComponent = sceneRegistry->get<TransformComponent>(entity);
			return transformComponent.position;
		}

		auto cameraEntities = sceneRegistry->view<CameraComponent, TransformComponent>();
		for (auto& entity : cameraEntities) {
			auto [cameraComponent, transformComponent] = sceneRegistry->get<CameraComponent, TransformComponent>(entity);

			if (cameraComponent.active) { 
				return transformComponent.position; 
			}
		}
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

	void CameraSystem::calculateCameraViewMatrix(TransformComponent& transformComponent, CameraComponent& cameraComponent)
	{
		XMVECTOR scale;
		XMVECTOR rotationQuat;
		XMVECTOR translation;
		XMMatrixDecompose(&scale, &rotationQuat, &translation, transformComponent.worldMatrix);

		// Load into XMVECTOR structures.
		DirectX::XMVECTOR upVector = XMLoadFloat3(&transformComponent.up);
		DirectX::XMVECTOR positionVector = translation;
		DirectX::XMVECTOR lookAtVector = XMLoadFloat3(&transformComponent.forward);

		// Translate the rotated camera position to the location of the viewer.
		lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

		// Finally create the view matrix from the three updated vectors.
		cameraComponent.viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
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