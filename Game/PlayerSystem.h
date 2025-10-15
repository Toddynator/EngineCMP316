#pragma once
#include "ECS/System.h"
#include "Managers/InputManager.h"

class PlayerSystem :
	public CMP316engine::System
{
public:
	PlayerSystem(entt::registry* sceneRegistry, CMP316engine::EngineContext* engineContext);

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput(float deltaTime) override;
	void Update(float deltaTime) override;
};
