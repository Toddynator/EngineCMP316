#include "TransformSystem.h"
#include "../Components.h"

namespace CMP316engine {
	void TransformSystem::Update(float deltaTime)
	{
		auto transformEntities = registry->view<TransformComponent>();
		for (auto& entity : transformEntities) {
			auto& t = registry->get<TransformComponent>(entity);

			// Angle Calculation Variables
			float pitchRadians = DirectX::XMConvertToRadians(t.rotation.x);
			float yawRadians = DirectX::XMConvertToRadians(t.rotation.y);
			float rollRadians = DirectX::XMConvertToRadians(t.rotation.z);

			float const cosY = cosf(yawRadians);
			float const cosP = cosf(pitchRadians);
			float const cosR = cosf(rollRadians);
			float const sinY = sinf(yawRadians);
			float const sinP = sinf(pitchRadians);
			float const sinR = sinf(rollRadians);

			// Up Vector
			t.up.x = -cosY * sinR - sinY * sinP * cosR;
			t.up.y = cosP * cosR;
			t.up.z = sinY * sinR - sinP * cosR * cosY;
			DirectX::XMVECTOR tempUpVec = DirectX::XMLoadFloat3(&t.up);
			DirectX::XMStoreFloat3(&t.up, DirectX::XMVector3Normalize(tempUpVec));

			// Forward Vector
			t.forward.x = sinY * cosP;
			t.forward.y = sinP;
			t.forward.z = cosP * cosY;
			DirectX::XMVECTOR tempForwardVec = DirectX::XMLoadFloat3(&t.forward);
			DirectX::XMStoreFloat3(&t.forward, DirectX::XMVector3Normalize(tempForwardVec));

			// Right Vector
			DirectX::XMStoreFloat3(&t.right, DirectX::XMVector3Cross(tempUpVec, tempForwardVec));

			// World Matrix
			calculateWorldMatrix(t);
		}
	}

	void TransformSystem::calculateWorldMatrix(TransformComponent& transformComponent)
	{
		auto& t = transformComponent;
		auto& position = transformComponent.position;
		auto& rotation = transformComponent.rotation;
		auto& scale = transformComponent.scale;

		/// CALCULATE MATRICES

		DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		/// CLAMP ROTATION

		t.rotation.x = fmod(t.rotation.x, 360.0f);
		if (t.rotation.x < 0) { t.rotation.x += 360.0f; }
		t.rotation.y = fmod(t.rotation.y, 360.0f);
		if (t.rotation.y < 0) { t.rotation.y += 360.0f; }
		t.rotation.z = fmod(t.rotation.z, 360.0f);
		if (t.rotation.z < 0) { t.rotation.z += 360.0f; }

		/// CALCULATE ROTATION MATRIX
		// Doing it myself instead of using directX's method allows me to enforce the order. Which helps with compatability with other libraries as a bonus.

		static const DirectX::XMFLOAT3 directions[3] = { DirectX::XMFLOAT3{1.f,0.f,0.f}, DirectX::XMFLOAT3{0.f,1.f,0.f}, DirectX::XMFLOAT3{0.f,0.f,1.f} };
		DirectX::XMMATRIX rotations[3];
		float axisRotations[3] = { DirectX::XMConvertToRadians(t.rotation.x), DirectX::XMConvertToRadians(t.rotation.y), DirectX::XMConvertToRadians(t.rotation.z) };
		for (int i = 0; i < 3; i++)
		{
			DirectX::XMVECTOR direction = XMLoadFloat3(&directions[i]);
			rotations[i] = DirectX::XMMatrixRotationAxis(direction, axisRotations[i]);
		}
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotations[0], rotations[1]);
		rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotations[2]);

		/// FINAL MATRIX CALCULATION

		transformComponent.worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	}
}