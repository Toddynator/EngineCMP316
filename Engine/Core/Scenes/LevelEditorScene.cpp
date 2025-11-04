#include "LevelEditorScene.h"
#include "ECS/Systems/LevelEditorSystem.h"

#include "Utility/VoxelHelper.h" // TEST
#include "ResourceLoading/VoxImporter.h" // TEST
#include <chrono>
#define BM_IMPLEMENTATION
#include "mesher.h" // TEST

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

		testVoxelStuff();
		/// TEMP TEST

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



	void LevelEditorScene::testVoxelStuff()
	{
		/// CREATE VOXEL ENTITY

		auto voxelEntity = ECS::AddChild(&registry, sceneRoot);
		auto* hierarchyComponent = &registry.get<HierarchyComponent>(voxelEntity);
		hierarchyComponent->name = "Test Voxel Entity";
		auto& meshComponent = CMP316engine::ECS::AddComponent<CMP316engine::MeshComponent>(&registry, voxelEntity);
		auto* transformComponent = &registry.get<TransformComponent>(voxelEntity);
		transformComponent->position = { 0.f, -20.f, 50.f};

		//std::vector<uint8_t> voxelGrid;
		//voxelGrid.resize(32);
		//for (auto& row : voxelGrid)
		//{
		//	row = std::numeric_limits<int>::max(); // Every bit is a 1
		//}

		/// NAIVE MESHING

		auto& meshStruct = meshComponent.meshes.emplace_back();
		auto start = std::chrono::high_resolution_clock::now();
		//VoxelResource voxelModel = VoxImporter::LoadVox("data/Models/Fighter Spaceship.vox");
		VoxelResource* voxelModel = engineContext.assetManager->GetResource<VoxelResource>("data/Models/Fighter Spaceship.vox");
		meshStruct.textureName = "data/Models/Fighter Spaceship.vox";
		auto end = std::chrono::high_resolution_clock::now();
		auto& voxels = voxelModel->voxels;
		auto& modelSize = voxelModel->modelSize;
		Vector3Int halfModelSizeOffset = { modelSize.x / 2, modelSize.y / 2, modelSize.z / 2 }; // So that I can centre the mesh
		auto start2 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < voxels.size(); i++)
		{
			auto& voxel = voxels[i];
			if (voxel == 0) { continue; }

			Vector3Int position = VoxelHelper::ConvertIndexTo3DPosition(i, modelSize) - halfModelSizeOffset;
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Front);
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Back);
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Left);
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Right);
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Top);
			VoxelHelper::GenerateVoxelFaceVertices(voxel, position, meshStruct.vertices, meshStruct.indices, VoxelHelper::VoxelFace::Bottom);
		}
		auto end2 = std::chrono::high_resolution_clock::now();
		auto ns = duration_cast<std::chrono::nanoseconds>(end - start).count();
		auto ms = duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "\nVoxel Model Import Time: " << ns << " ns (" << ms << " ms)";
		ns = duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
		ms = duration_cast<std::chrono::milliseconds>(end2 - start2).count();
		std::cout << "\nNaive Voxel Mesh Calculation Time: " << ns << " ns (" << ms << " ms)";

		/// BITWISE GREEDY MESHING

		/*start = std::chrono::high_resolution_clock::now();
		MeshData meshData{};
		meshData.opaqueMask = new uint64_t[CS_P2];
		meshData.faceMasks = new uint64_t[CS_2 * 6];
		meshData.forwardMerged = new uint8_t[CS_2];
		meshData.rightMerged = new uint8_t[CS];
		meshData.vertices = new BM_VECTOR<uint64_t>();

		for (int x = 1; x < CS_P; x++) {
			for (int y = 1; y < CS_P; y++) {
				for (int z = 1; z < CS_P; z++) {
					if (x % 2 == 0 && y % 2 == 0 && z % 2 == 0) {
						mainThreadMeshData.opaqueMask[(y * CS_P) + x] |= 1ull << z;
						mainThreadMeshData.opaqueMask[((y - 1) * CS_P) + (x - 1)] |= 1ull << z;
						mainThreadMeshData.opaqueMask[(y * CS_P) + (x - 1)] |= 1ull << (z - 1);
						mainThreadMeshData.opaqueMask[((y - 1) * CS_P) + x] |= 1ull << (z - 1);
					}
				}
			}
		}
		
		mesh(voxels.data(), meshData);
		end = std::chrono::high_resolution_clock::now();
		ns = duration_cast<std::chrono::nanoseconds>(end - start).count();
		ms = duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Bitwise Greedy Voxel Mesh Calculation Time: " << ns << " ns (" << ms << " ms)\n";*/
		//meshComponent.meshes.emplace_back().vertices = meshData.vertices;
	}
}