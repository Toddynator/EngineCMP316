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
		}
	}
}