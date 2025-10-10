#include "PlayerSystem.h"
#include "Components.h"

PlayerSystem::PlayerSystem(entt::registry* sceneRegistry, CMP316engine::InputManager* sceneInputManager) : System(sceneRegistry), inputManager(sceneInputManager)
{
}

bool PlayerSystem::Initialize()
{
	return true;
}

void PlayerSystem::Shutdown()
{
}

void PlayerSystem::HandleInput()
{

}

void PlayerSystem::Update(float deltaTime)
{
	auto playerEntities = registry->view<PlayerComponent, CMP316engine::TransformComponent>();
	for (auto& entity : playerEntities) {
		auto [playerComponent, transformComponent] = registry->get<PlayerComponent, CMP316engine::TransformComponent>(entity);

		/*
		TODO: 
		Add Movement Component, it should contain intended movement direction, and 
		the speeds for those directions. (Which may be part of a physics component later).
		Input should then be moved to HandleInput() and all it should set is the intended movement direction.
		*/
		auto& t = transformComponent;
		if (inputManager->IsKeyBindingDown("Move Up"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x, t.position.y + (1.f * deltaTime), t.position.z);
		}
		if (inputManager->IsKeyBindingDown("Move Down"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x, t.position.y - (1.f * deltaTime), t.position.z);
		}
		if (inputManager->IsKeyBindingDown("Move Left"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x - (1.f * deltaTime), t.position.y, t.position.z);
		}
		if (inputManager->IsKeyBindingDown("Move Right"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x + (1.f * deltaTime), t.position.y, t.position.z);
		}
		if (inputManager->IsKeyBindingDown("Move Forward"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x, t.position.y, t.position.z + (1.f * deltaTime));
		}
		if (inputManager->IsKeyBindingDown("Move Backward"))
		{
			t.position = DirectX::XMFLOAT3(t.position.x, t.position.y, t.position.z - (1.f * deltaTime));
		}
	}
}
