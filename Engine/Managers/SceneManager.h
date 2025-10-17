/*
SceneManager utilizes factory pattern to dynamically register and create scenes. 
https://stackoverflow.com/questions/5120768/how-to-implement-the-factory-method-pattern-in-c-correctly

User should define their own ID system (e.g. enums) for scenes.
On initialization of the application, the scenes should be registered to the SceneManager.

The sceneManager will by default initialize a level editor scene if the application hasn't registered any scenes. 
If a scene is registered, it is up to the application developer to add the level editor to the list of possible scenes, and ensure they 
have set the id of the scene they want to start as (it will use the first scene found in the map otherwise).

HOW TO USE:
- On Initialization of the application, register scenes with an id system of your choice, as long as it maps to an integer.
- Also on Initialization, it is recommended to call RequestSceneChange() so that you explicitly choose your starting scene.
- If you have overidden the engines base definition of the update loops for the application, ensure you get the active scene from the manager and call the update loops.
*/

#pragma once
#include "Manager.h"
#include <unordered_map>

namespace CMP316engine {
	/// FORWARD DECLARATIONS
	class Scene;
	struct EngineContext;

	class SceneManager
		: public Manager
	{
	private:
		std::unordered_map<int, std::function<std::unique_ptr<Scene>(EngineContext&)>> scenes;
		std::unique_ptr<Scene> activeScene = nullptr;
		bool changeScene = true;
		int idOfSceneToChangeTo = -1;

	public:
		SceneManager();
		~SceneManager();

		bool Initialize();
		void Shutdown();
		void Update(EngineContext& engineContext);

		template <typename SceneType>
		void RegisterScene(int id)
		{
			scenes[id] = [&](EngineContext& engineContext) { return std::make_unique<SceneType>(engineContext); };
		}
		void RequestSceneChange(int sceneToChangeTo);

		Scene* GetActiveScene() { return activeScene.get(); }
	};
}
