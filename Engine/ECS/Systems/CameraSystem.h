#pragma once
#include "../System.h"

namespace CMP316engine {
	class CameraSystem : public System
	{
	public:
		CameraSystem(entt::registry* sceneRegistry);

		bool Initialize() override;
		void Shutdown() override;

		void Update() override;

		static DirectX::XMMATRIX GetActiveCameraViewMatrix(entt::registry* sceneRegistry);
	};
}