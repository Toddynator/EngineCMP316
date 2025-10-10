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
	auto playerEntities = registry->view<PlayerComponent>();
	for (auto& entity : playerEntities) {
		auto& playerComponent = registry->get<PlayerComponent>(entity);

		if (auto movementComponent = registry->try_get<CMP316engine::MovementComponent>(entity))
		{
			auto& m = movementComponent;
			if (inputManager->IsKeyBindingDown("Move Up"))
			{
				m->linearVelocity.y = 1.f;
			}
			else if (inputManager->IsKeyBindingDown("Move Down"))
			{
				m->linearVelocity.y = -1.f;
			}
			else {
				m->linearVelocity.y = 0.f;
			}
			if (inputManager->IsKeyBindingDown("Move Left"))
			{
				m->linearVelocity.x = -1.f;
			}
			else if (inputManager->IsKeyBindingDown("Move Right"))
			{
				m->linearVelocity.x = 1.f;
			}
			else {
				m->linearVelocity.x = 0.f;
			}
			if (inputManager->IsKeyBindingDown("Move Forward"))
			{
				m->linearVelocity.z = 1.f;
			}
			else if (inputManager->IsKeyBindingDown("Move Backward"))
			{
				m->linearVelocity.z = -1.f;
			}
			else
			{
				m->linearVelocity.z = 0.f;
			}
		}
	}
}

void PlayerSystem::Update(float deltaTime)
{
	auto playerEntities = registry->view<PlayerComponent, CMP316engine::TransformComponent>();
	for (auto& entity : playerEntities) {
		auto [playerComponent, transformComponent] = registry->get<PlayerComponent, CMP316engine::TransformComponent>(entity);

		// TODO
	}
}
