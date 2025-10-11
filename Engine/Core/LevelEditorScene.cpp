#include "LevelEditorScene.h"
#include "../ECS/Systems/LevelEditorSystem.h"

namespace CMP316engine
{
	LevelEditorScene::LevelEditorScene(CMP316engine::EngineContext& context) : ECSScene(context)
	{
		systems.emplace_back(std::make_unique<LevelEditorSystem>(&registry, sceneRoot));
	}

	bool LevelEditorScene::Initialize()
	{
		ECSScene::Initialize();
		return true;
	}

	void LevelEditorScene::HandleImGui()
	{
		ECSScene::HandleImGui();
	}
}