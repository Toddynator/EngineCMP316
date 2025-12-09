#include "pch.h"
#include "Scene.h"

namespace NomadEngine {
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
		int saveFileVersion = 0;
		deserializeArchive(saveFileVersion);
		deserializeArchive.setFileVersion(saveFileVersion);
		Deserialize(file, deserializeArchive);
		file.close();
	}
	void Scene::LoadLevelFromFile(std::string levelFileName)
	{
		std::string temp1 = saveFileType;
		std::filesystem::path temp2 = saveFolderPath;
		saveFileType = ".level";
		saveFolderPath = "data/Levels/";
		saveFileName = levelFileName;

		Load();

		saveFileType = temp1;
		saveFolderPath = temp2;

	}
	void Scene::Serialize(std::ofstream& file, BinarySerializeArchive& serializeArchive)
	{
			
	}
	void Scene::Deserialize(std::ifstream& file, BinaryDeserializeArchive& deserializeArchive)
	{
		
	}
}