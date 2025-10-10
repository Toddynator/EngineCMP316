/*
NOTE:
- This is one of the few systems (If I can help it) that needs to defer creation and initialization, as it requires the scene root to be made first to be operational.

TODO:
- Make GameObject clone / copy constructor set an initialization bool to false, it needs to create new entities for the copied objects!
*/

#pragma once
#include "../System.h"
#include "../GameObject.h"

namespace CMP316engine {
	class LevelEditorSystem : public System
	{
	private:
		GameObject* sceneRoot = nullptr;
		GameObject* selectedObject = nullptr;
		GameObject* cutObject = nullptr;
		std::unique_ptr<GameObject> copiedObject = nullptr;

	public:
		LevelEditorSystem(entt::registry* sceneRegistry, GameObject* sceneRoot);

		bool Initialize() override;
		void Shutdown() override;

		void HandleImGui() override;
		void Update(float deltaTime) override;

	private:
		// Display the hierarchy, no extra details
		void renderSceneTreeSelectionWindow();
		// Display the selected objects details
		void renderObjectInspectorWindow();
		// Cut, Copy, Paste, Delete
		void renderSelectionWindowManipulationTools();
		void renderSelectionWindowObjectTree(GameObject* currentObject, GameObject* selectedObject);
	};
}