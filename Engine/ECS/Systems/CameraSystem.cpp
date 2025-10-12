#include "pch.h"
#include "CameraSystem.h"

CMP316engine::CameraSystem::CameraSystem(entt::registry* sceneRegistry) : System(sceneRegistry)
{

}

bool CMP316engine::CameraSystem::Initialize()
{
	return true;
}

void CMP316engine::CameraSystem::Shutdown()
{

}

void CMP316engine::CameraSystem::HandleInput(float deltaTime)
{
	if (activeCamera == entt::null) { return; }

	/// TODO: Use inputManager (should just pass it into every system), it has mouseButtonHold for checking RMB, and also I should add a drag function if I haven't already,
	/// inputManager should just calculate and store the drag delta.

	if (ImGui::GetMouseDragDelta().x != 0.f)
	{
		DirectX::XMFLOAT2 mouseDelta = { ImGui::GetMouseDragDelta().x, ImGui::GetMouseDragDelta().y };
		auto& transformComponent = registry->get<TransformComponent>(activeCamera);
		transformComponent.rotation.x += (-mouseDelta.y) * deltaTime * BASE_CAMERA_ROTATION_SPEED;
		transformComponent.rotation.y += mouseDelta.x * deltaTime * BASE_CAMERA_ROTATION_SPEED;
	}
}

void CMP316engine::CameraSystem::Update(float deltaTime)
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

DirectX::XMMATRIX CMP316engine::CameraSystem::GetActiveCameraViewMatrix(entt::registry* sceneRegistry)
{
	auto cameraEntities = sceneRegistry->view<CameraComponent>();
	for (auto& entity : cameraEntities) {
		auto& cameraComponent = sceneRegistry->get<CameraComponent>(entity);

		if (cameraComponent.active) { return cameraComponent.viewMatrix; }
	}

	//std::cout << "\nNo Active Camera Found";
	return createDefaultViewMatrix(); // Return default matrix
}

DirectX::XMMATRIX CMP316engine::CameraSystem::createDefaultViewMatrix()
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
