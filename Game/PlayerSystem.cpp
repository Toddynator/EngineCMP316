#include "pch.h"
#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(entt::registry* sceneRegistry) : System(sceneRegistry)
{
}

bool PlayerSystem::Initialize()
{
	return false;
}

void PlayerSystem::Shutdown()
{
}

void PlayerSystem::Update()
{
	std::cout << "\nPlayerSystemWorking";
}
