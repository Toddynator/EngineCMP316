#include "TransformSystem.h"
#include "TransformSystem.h"
#include "../Components.h"

namespace CMP316engine {
	void TransformSystem::Update(float deltaTime)
	{
		auto transformEntities = registry->view<TransformComponent>();
		for (auto& entity : transformEntities) {
			auto& transformComponent = registry->get<TransformComponent>(entity);

			/// GET GLOBAL TRANSFORMS

			XMVECTOR scale;
			XMVECTOR rotationQuat;
			XMVECTOR translation;
			XMMatrixDecompose(&scale, &rotationQuat, &translation, transformComponent.worldMatrix);

			/// ORTHOGONAL DIRECTION VECTORS

			XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuat);
			//DirectX::XMMATRIX rotationMatrix = calculateRotationMatrix(transformComponent);

			DirectX::XMVECTOR forward = XMVector3TransformCoord(DirectX::XMVectorSet(0, 0, 1, 0), rotationMatrix);
			DirectX::XMVECTOR up = XMVector3TransformCoord(DirectX::XMVectorSet(0, 1, 0, 0), rotationMatrix);
			DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);

			forward = DirectX::XMVector3Normalize(forward);
			up = DirectX::XMVector3Normalize(up);
			right = DirectX::XMVector3Normalize(right);

			DirectX::XMStoreFloat3(&transformComponent.forward, forward);
			DirectX::XMStoreFloat3(&transformComponent.up, up);
			DirectX::XMStoreFloat3(&transformComponent.right, right);

			/// WORLD MATRIX

			entt::entity* parentEntity = &registry->try_get<HierarchyComponent>(entity)->parent;
			auto* parentTransformComponent = parentEntity ? registry->try_get<TransformComponent>(*parentEntity) : nullptr;
			calculateWorldMatrix(transformComponent, parentTransformComponent);
		}
	}

	void TransformSystem::calculateWorldMatrix(TransformComponent& transformComponent, TransformComponent* parentTransformComponent)
	{
		auto& t = transformComponent;
		auto& position = transformComponent.position;
		auto& rotation = transformComponent.rotation;
		auto& scale = transformComponent.scale;

		/// CALCULATE MATRICES

		DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX rotationMatrix = calculateRotationMatrix(transformComponent);

		/// FINAL MATRIX CALCULATION

		if (parentTransformComponent)
		{
			transformComponent.worldMatrix =  scaleMatrix * rotationMatrix * translationMatrix * parentTransformComponent->worldMatrix;
		}
		else
		{
			transformComponent.worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
		}
	}
	// Doing it myself instead of using directX's method allows me to enforce the order. Which helps with compatability with other libraries as a bonus.
	DirectX::XMMATRIX CMP316engine::TransformSystem::calculateRotationMatrix(TransformComponent& transformComponent)
	{
		auto& t = transformComponent;

		/// CLAMP ROTATION

		t.rotation.x = fmod(t.rotation.x, 360.f);
		if (t.rotation.x < -180.f) { t.rotation.x += 360.0f; }
		t.rotation.y = fmod(t.rotation.y, 360.f);
		if (t.rotation.y < -180.f) { t.rotation.y += 360.0f; }
		t.rotation.z = fmod(t.rotation.z, 360.f);
		if (t.rotation.z < -180.f) { t.rotation.z += 360.0f; }

		static const DirectX::XMFLOAT3 directions[3] = { DirectX::XMFLOAT3{1.f,0.f,0.f}, DirectX::XMFLOAT3{0.f,1.f,0.f}, DirectX::XMFLOAT3{0.f,0.f,1.f} };
		DirectX::XMMATRIX rotations[3];
		float axisRotations[3] = { DirectX::XMConvertToRadians(t.rotation.x), DirectX::XMConvertToRadians(t.rotation.y), DirectX::XMConvertToRadians(t.rotation.z) };
		for (int i = 0; i < 3; i++)
		{
			DirectX::XMVECTOR direction = XMLoadFloat3(&directions[i]);
			rotations[i] = DirectX::XMMatrixRotationAxis(direction, axisRotations[i]);
		}
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotations[0], rotations[1]);
		return rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotations[2]);
	}
}