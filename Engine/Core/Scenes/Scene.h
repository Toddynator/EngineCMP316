/*
Where levels, aka scenes will go inside an application.
This implements the required framework all scenes should use,
scenes implemented in Game projects should inherit this or one of the derived scene classes.
Any functions that are overridden should call the base implementation from their inherited scene class.

IMPROVEMENT IDEA: Currently passes in Engine Context via constructor, means derivations need to create their own constructor (boilerplate). 
However I could use a setter and have a sceneManager handle this in the future.
*/

#pragma once
#include "Core/EngineContext.h"
#include "Utility/BinarySerializeArchive.h"
#include "Utility/BinaryDeserializeArchive.h"
#include <filesystem>

namespace CMP316engine {
	class Scene
	{
	public:
		explicit Scene(EngineContext& context) : engineContext(context) {}
		Scene() = delete;  // MUST pass the context in ~ Otherwise Game will have no access to the engines systems
		~Scene() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleInput(float deltaTime) = 0;
		virtual void HandleImGui() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		/*
		@brief base implementation opens the file for saving and loading.
		OVERRIDE and call the base implementation in overridden save and load functions in order to add any pre or post processing.
		*/
		virtual void Save();
		virtual void Load();
		void LoadLevelFromFile(std::string levelFileName);
		virtual void Serialize(std::ofstream& file, BinarySerializeArchive& serializeArchive);
		virtual void Deserialize(std::ifstream& file, BinaryDeserializeArchive& deserializeArchive);

	protected:
		EngineContext& engineContext;

		inline static const int FILE_VERSION = 1;
		std::string saveFileName = "Save";
		std::filesystem::path saveFolderPath = "data/Saves/";
		std::string saveFileType = ".scene";
	};
}

