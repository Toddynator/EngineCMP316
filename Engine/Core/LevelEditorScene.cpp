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

		/// TEMP TEST
		ECS::AddChild(&registry, sceneRoot);
		ECS::AddChild(&registry, sceneRoot);
		auto ent1 = ECS::AddChild(&registry, sceneRoot);
		ECS::AddChild(&registry, ent1);
		auto ent2 = ECS::AddChild(&registry, sceneRoot);
		auto ent21 = ECS::AddChild(&registry, ent2);
		ECS::AddChild(&registry, ent21);
		/// TEMP TEST

		return true;
	}

	void LevelEditorScene::HandleImGui()
	{
		ECSScene::HandleImGui();
	}
}