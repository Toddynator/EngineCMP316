#include "GameScene.h"
#include "ECS/EngineECSSystems.h"
#include "ECS/Components.h" // Engine Components
#include "PlayerSystem.h"
#include "Components.h" // Game Components

GameScene::GameScene(CMP316engine::EngineContext& context) : ECSScene(context)
{
	systems.push_back(std::make_unique<CMP316engine::PhysicsSystem>(&registry, &engineContext));
	systems.push_back(std::make_unique<PlayerSystem>(&registry, &engineContext));
}

bool GameScene::Initialize()
{
	CMP316engine::ECSScene::Initialize();

	///// SOUND

	// I yearn for the music
	int audioHandle = engineContext.audioManager->Play("MyJam");
	engineContext.audioManager->SetAudioLoop(audioHandle, true);

	return true;
}