#include "pch.h"
#include "Scene.h"

namespace CMP316engine {
	bool Scene::Initialize()
	{
		return true;
	}

	void Scene::Shutdown()
	{
		if (engineContext.audioManager) { engineContext.audioManager->StopAll(); }
	}

	void Scene::HandleInput(float deltaTime)
	{

	}

	void Scene::HandleImGui()
	{

	}

	void Scene::Update(float deltaTime)
	{

	}

	void Scene::Render()
	{

	}

	void Scene::Save()
	{
		std::filesystem::path filePath = saveFolderPath.string() + saveFileName + saveFileType;
		std::ofstream file(filePath.string(), std::ios::binary);
		if (!file.is_open()) {
			std::cout << "\nFailed to open file for writing";
			return;
		}
		BinarySerializeArchive serializeArchive(file, FILE_VERSION);
		Serialize(file, serializeArchive);
		file.close();
	}
	void Scene::Load()
	{
		std::filesystem::path filePath = saveFolderPath.string() + saveFileName + saveFileType;
		std::ifstream file(filePath.string(), std::ios::binary);
		if (!file.is_open()) {
			std::cout << "\nFailed to open file for reading";
			return;
		}
		BinaryDeserializeArchive deserializeArchive(file);
		Deserialize(file, deserializeArchive);
		file.close();
	}
	void Scene::Serialize(std::ofstream& file, BinarySerializeArchive& serializeArchive)
	{
			
	}
	void Scene::Deserialize(std::ifstream& file, BinaryDeserializeArchive& deserializeArchive)
	{
		
	}
}