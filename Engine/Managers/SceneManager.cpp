#include "SceneManager.h"
#include "../Core/Scene.h"
#include "../Core/LevelEditorScene.h"

namespace CMP316engine {
	SceneManager::SceneManager() = default;
	SceneManager::~SceneManager() = default;

	bool SceneManager::Initialize() 
	{ 
		return true; 
	}

	void SceneManager::Update(EngineContext& engineContext)
	{
		/// Check for calls to change scene

		// Handle case if no starting scene was set
		if (idOfSceneToChangeTo == -1)
		{
			// If there are no scenes registered at all, then
			if (scenes.empty()) {
				RegisterScene(-1, std::make_unique<LevelEditorScene>(engineContext));
				activeScene = scenes[-1].get();
			}
			activeScene = scenes.begin()->second.get(); // Just use the first registered
		}

		// Handle scene change calls
		if (changeScene)
		{
			activeScene = scenes[idOfSceneToChangeTo].get();
		}
	}

	void SceneManager::RegisterScene(int id, std::unique_ptr<Scene> scene) 
	{ 
		scenes[id] = std::move(scene); 
	}
	void SceneManager::RequestSceneChange(int sceneToChangeTo)
	{ 
		idOfSceneToChangeTo = sceneToChangeTo; 
		changeScene = true; 
	}
}