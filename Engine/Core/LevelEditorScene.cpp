#include "LevelEditorScene.h"
#include "../ECS/Systems/LevelEditorSystem.h"

namespace CMP316engine
{
	LevelEditorScene::LevelEditorScene(CMP316engine::EngineContext& context) : ECSScene(context)
	{		
		systems.emplace_back(std::make_unique<LevelEditorSystem>(&registry, sceneRoot, context.renderer.get()));
	}

	bool LevelEditorScene::Initialize()
	{
		ECSScene::Initialize();

		/// TEMP TEST
		auto firstChild = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent = &registry.get<HierarchyComponent>(firstChild);
		hierarchyComponent->name = "First Child";
		ECS::AddChild(&registry, firstChild);
		auto& modelComponent = CMP316engine::ECS::AddComponent<CMP316engine::ModelComponent>(&registry, firstChild);
		modelComponent.filepath = "data/Models/Dug/Dug.obj";
		auto& meshComponent = CMP316engine::ECS::AddComponent<CMP316engine::MeshComponent>(&registry, firstChild);

		auto secondChild = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent2 = &registry.get<HierarchyComponent>(secondChild);
		hierarchyComponent2->name = "Second Child";
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