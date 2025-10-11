/*
NOTE:
- This is one of the few systems (If I can help it) that needs to defer creation and initialization, as it requires the scene root to be made first to be operational.

TODO:
- Make GameObject clone / copy constructor set an initialization bool to false, it needs to create new entities for the copied objects!
*/

#pragma once
#include "../System.h"
#include <entt.hpp>

namespace CMP316engine {
	class LevelEditorSystem : public System
	{
	private:
		entt::entity  sceneRoot = entt::null;
		entt::entity  selectedObject = entt::null;
		entt::entity  cutObject = entt::null;
		entt::entity  copiedObject = entt::null;

	public:
		LevelEditorSystem(entt::registry* sceneRegistry, entt::entity sceneRoot);

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
		static void renderObjectSelectionWindowObjectTree(entt::registry* registry, entt::entity currentObject, entt::entity& selectedObject);
	};
}