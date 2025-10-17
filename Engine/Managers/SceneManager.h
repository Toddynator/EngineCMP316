/*
User should define their own ID system (e.g. enums) for scenes.
On initialization of the application, the scenes should be registered to the SceneManager.
*/

#pragma once
#include "Manager.h"
#include <unordered_map>

namespace CMP316engine {
	class Scene;
	struct EngineContext;

	class SceneManager
		: public Manager
	{
	private:
		std::unordered_map<int, std::unique_ptr<Scene>> scenes;
		Scene* activeScene = nullptr;
		bool changeScene = false;
		int idOfSceneToChangeTo = -1;

	public:
		SceneManager();
		~SceneManager();

		bool Initialize();
		void Shutdown() {}
		void Update(EngineContext& engineContext);

		void RegisterScene(int id, std::unique_ptr<Scene> scene);
		void RequestSceneChange(int sceneToChangeTo);
	};
}
