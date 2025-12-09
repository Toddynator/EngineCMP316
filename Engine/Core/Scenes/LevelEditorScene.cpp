#include "LevelEditorScene.h"
#include "ECS/Systems/LevelEditorSystem.h"

namespace NomadEngine
{
	LevelEditorScene::LevelEditorScene(NomadEngine::EngineContext& context) : ECSScene(context)
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

		/// DEBUG OBJECTS

		auto firstChild = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent = &registry.get<HierarchyComponent>(firstChild);
		hierarchyComponent->name = "Test Model Entity";
		auto& modelComponent = NomadEngine::ECS::AddComponent<NomadEngine::ModelComponent>(&registry, firstChild);
		modelComponent.filepath = "data/Models/Dug/Dug.obj";
		auto& meshComponent = NomadEngine::ECS::AddComponent<NomadEngine::MeshComponent>(&registry, firstChild);

		auto secondChild = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent2 = &registry.get<HierarchyComponent>(secondChild);
		hierarchyComponent2->name = "Second Child";

		auto voxelEntity = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponentVoxel = &registry.get<HierarchyComponent>(voxelEntity);
		hierarchyComponentVoxel->name = "Test Voxel Entity";
		auto& meshComponentVoxel = NomadEngine::ECS::AddComponent<NomadEngine::MeshComponent>(&registry, voxelEntity);
		auto* transformComponentVoxel = &registry.get<TransformComponent>(voxelEntity);
		transformComponentVoxel->position = { 0.f, -20.f, 50.f };
		auto& voxelComponent = NomadEngine::ECS::AddComponent<NomadEngine::VoxelComponent>(&registry, voxelEntity);
		voxelComponent.filepath = "data/Models/Fighter Spaceship.vox";

		/// EDITOR SFX

		engineContext.audioManager->LoadAudio("ButtonPress1", "data/Audio/ButtonPress1.wav");

		return true;
	}

	void  LevelEditorScene::HandleImGui()
	{
		ECSScene::HandleImGui();


		auto& io = ImGui::GetIO();
		ImVec2 windowMinSize = ImVec2(io.DisplaySize.x * 0.7f, io.DisplaySize.y * 0.7f);
		ImGui::Begin("Save & Load");
		if (ImGui::Button("Save"))
		{
			IGFD::FileDialogConfig config;
			config.path = "data/Levels/";
			config.fileName = "undefined.level";
			config.countSelectionMax = 1;
			config.flags = ImGuiFileDialogFlags_Modal;
			fileDialog.OpenDialog(
				"SaveFileDlgKey",
				"Save Level As...",
				".level",
				config
			);

			ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
			ImGui::SetNextWindowPos(center, 0, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(windowMinSize);
		}
		if (ImGui::Button("Load"))
		{
			IGFD::FileDialogConfig config;
			config.path = "data/Levels/";
			config.fileName = "undefined.level";
			config.countSelectionMax = 1;
			config.flags = ImGuiFileDialogFlags_Modal |
				ImGuiFileDialogFlags_DisableCreateDirectoryButton |
				ImGuiFileDialogFlags_ReadOnlyFileNameField;
			fileDialog.OpenDialog(
				"LoadFileDlgKey",
				"Load Level As...",
				".level,.save",
				config
			);

			ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
			ImVec2 windowStartSize = ImVec2(io.DisplaySize.x * 0.7f, io.DisplaySize.y * 0.7f);
			ImGui::SetNextWindowPos(center, 0, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(windowStartSize);
		}
		ImGui::End();

		if (fileDialog.Display("SaveFileDlgKey", ImGuiWindowFlags_NoCollapse, windowMinSize)) {
			if (fileDialog.IsOk()) {
				std::string saveFilePath = fileDialog.GetFilePathName();
				std::filesystem::path path(saveFilePath);
				saveFolderPath = path.parent_path().string() + "/";
				saveFileName = path.stem().string();
				saveFileType = path.extension().string();

				/// ACTION

				Save();
			}
			fileDialog.Close();
		}
		if (fileDialog.Display("LoadFileDlgKey", ImGuiWindowFlags_NoCollapse, windowMinSize)) {
			if (fileDialog.IsOk()) {
				std::string saveFilePath = fileDialog.GetFilePathName();
				std::filesystem::path path(saveFilePath);
				saveFolderPath = path.parent_path().string() + "/";
				saveFileName = path.stem().string();
				saveFileType = path.extension().string();

				/// ACTION

				Load();
			}
			fileDialog.Close();
		}
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