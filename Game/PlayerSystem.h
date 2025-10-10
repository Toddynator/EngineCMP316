#pragma once
#include "ECS/System.h"

class PlayerSystem :
	public CMP316engine::System
{
public:
	PlayerSystem(entt::registry* sceneRegistry);

	bool Initialize() override;
	void Shutdown() override;

	void Update() override;
};
