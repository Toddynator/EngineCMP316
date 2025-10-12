/*
Creates UI for making a scene and provides the ability to save and load scenes
for use by a game.
*/

#pragma once
#include "../System.h"
#include <entt.hpp>

namespace CMP316engine {
	class LevelEditorSystem : public System
	{
	private:
		entt::registry clipboardRegistry; // Stop entities that are copied from being active in the scene until they are pasted.
		entt::entity  sceneRoot = entt::null;
		entt::entity  selectedEntity = entt::null;
		entt::entity  cutEntity = entt::null;
		entt::entity  copiedEntity = entt::null;
		bool deletePrompt = false;

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