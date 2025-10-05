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

void CMP316engine::CameraSystem::Update()
{
	auto cameraEntities = registry->view<CameraComponent, TransformComponent>();
	for (auto& entity : cameraEntities) {
		auto [cameraComponent, transformComponent] = registry->get<CameraComponent, TransformComponent>(entity);

		/*
		With DirectX math, its better to do calculations using their Vector variable instead of the float variable.
		Apparently. I need to profile this.
		I guess they had a valid reason not to use operators for vector math.
		*/

		cameraComponent.active = true; // TEMP
		transformComponent.up = DirectX::XMFLOAT3(0.f, 1.f, 0.f); // TEMP
		transformComponent.forward = DirectX::XMFLOAT3(0.f, 0.f, 1.f); // TEMP // Default Look direction

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

	std::cout << "\nNo Active Camera Found";
	return DirectX::XMMATRIX{}; // Return default matrix
}
