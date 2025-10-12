#pragma once
#include "ECS/System.h"
#include "Managers/InputManager.h"

class PlayerSystem :
	public CMP316engine::System
{
private:
	CMP316engine::InputManager* inputManager;

public:
	PlayerSystem(entt::registry* sceneRegistry, CMP316engine::InputManager* sceneInputManager);

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput(float deltaTime) override;
	void Update(float deltaTime) override;
};
