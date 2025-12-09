#include "SceneManager.h"
#include "Core/Scenes/Scene.h"
#include "Core/Scenes/LevelEditorScene.h"

namespace NomadEngine {
	SceneManager::SceneManager() = default;
	SceneManager::~SceneManager() = default;

	bool SceneManager::Initialize() 
	{ 
		return true; 
	}

	void SceneManager::Shutdown()
	{
		if (activeScene) { activeScene->Shutdown(); }
	}

	void SceneManager::Update(EngineContext& engineContext)
	{
		/// Check for calls to change scene

		if (changeScene)
		{
			changeScene = false;
			if (activeScene) { activeScene->Shutdown(); }

			// If there are no scenes registered at all, then register the level editor and set that as active.
			if (scenes.empty()) {
				RegisterScene<LevelEditorScene>(-1);
				activeScene = scenes.begin()->second.sceneFactoryFunction(engineContext); // Just use the first registered
			}
			else {
				if (scenes.find(idOfSceneToChangeTo) == scenes.end()) {
					std::cout << "\nSceneManager could not find the scene! Defaulting to first scene. Check if you registered the scene yet, or if you have the correct ID.";
					// If it couldn't find the scene, it will default to the first scene found
					activeScene = scenes.begin()->second.sceneFactoryFunction(engineContext);
					std::string levelFileName = scenes.begin()->second.levelFileName;
					if (levelFileName != "") { activeScene->LoadLevelFromFile(levelFileName); }
				}
				else {
					// Scene exists, can now change
					activeScene = scenes[idOfSceneToChangeTo].sceneFactoryFunction(engineContext);
					std::string levelFileName = scenes[idOfSceneToChangeTo].levelFileName;
					if (levelFileName != "") { activeScene->LoadLevelFromFile(levelFileName); }
				}
			}

			activeScene->Initialize();
		}
	}

	void SceneManager::RequestSceneChange(int sceneToChangeTo)
	{ 
		idOfSceneToChangeTo = sceneToChangeTo; 
		changeScene = true; 
	}
}