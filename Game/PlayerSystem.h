#pragma once
#include "ECS/System.h"
#include "Managers/InputManager.h"

class PlayerSystem :
	public NomadEngine::System
{
public:
	PlayerSystem(entt::registry* sceneRegistry, NomadEngine::EngineContext* engineContext);

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput(float deltaTime) override;
	void Update(float deltaTime) override;
};
