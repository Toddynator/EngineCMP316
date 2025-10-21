#include "LevelEditorScene.h"
#include "../ECS/Systems/LevelEditorSystem.h"

namespace CMP316engine
{
	LevelEditorScene::LevelEditorScene(CMP316engine::EngineContext& context) : ECSScene(context)
	{		
		saveFileType = ".level";
		saveFolderPath = "data/Levels/";

		systems.emplace_back(std::make_unique<LevelEditorSystem>(&registry, &engineContext, sceneRoot, context.renderer.get()));
	}

	bool LevelEditorScene::Initialize()
	{
		ECSScene::Initialize();

		/// LEVEL EDITOR CAMERA
		// Notably, this is NOT added to the scene tree!

		createLevelEditorCameraEntity();

		/// TEMP TEST
		/*auto cameraEntity = ECS::AddChild(&registry, sceneRoot);
		auto* camComponent = &ECS::AddComponent<CameraComponent>(&registry, cameraEntity);
		camComponent->active = true;
		auto* camHierarchyComponent = &registry.get<HierarchyComponent>(cameraEntity);
		camHierarchyComponent->name = "Camera Entity 1";
		auto* camTransformComponent = &registry.get<TransformComponent>(cameraEntity);
		camTransformComponent->position = { 0.f,0.f,-5.0f };
		auto* levelEditorCamComponent = &ECS::AddComponent<LevelEditorCameraComponent>(&registry, cameraEntity);*/
		
		auto firstChild = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent = &registry.get<HierarchyComponent>(firstChild);
		hierarchyComponent->name = "Test Model Entity";
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

		engineContext.audioManager->LoadAudio("ButtonPress1", "data/Audio/ButtonPress1.wav");

		return true;
	}

	void  LevelEditorScene::HandleImGui()
	{
		ECSScene::HandleImGui();

		ImGui::Begin("Save & Load");
		if (ImGui::Button("Save"))
		{
			Save();
		}
		if (ImGui::Button("Load"))
		{
			Load();
		}
		ImGui::End();
	}

	void LevelEditorScene::Load()
	{
		ECSScene::Load();

		createLevelEditorCameraEntity();
	}

	void LevelEditorScene::createLevelEditorCameraEntity()
	{
		auto levelEditorCameraEntity = ECS::CreateEntityWithDefaultComponents(&registry);
		auto* camComponent = &ECS::AddComponent<CameraComponent>(&registry, levelEditorCameraEntity);
		camComponent->active = true;
		auto* editorCamComponent = &ECS::AddComponent<LevelEditorCameraComponent>(&registry, levelEditorCameraEntity);
		auto* camTransformComponent = &registry.get<TransformComponent>(levelEditorCameraEntity);
		camTransformComponent->position = { 0.f, 0.f, -5.0f };
	}
}