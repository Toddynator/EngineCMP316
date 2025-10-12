/*
Creates UI for making a scene and provides the ability to save and load scenes
for use by a game.
*/

#pragma once
#include "../System.h"
#include <entt.hpp>
#include "../../Graphics/Renderer_DirectX11.h"
#include "../../ImGui/ImGuizmo/ImGuizmo.h"

namespace CMP316engine {
	class LevelEditorSystem : public System
	{
	private:
		/// REQUIRED SERVICES
		Renderer_DirectX11* renderer;

		/// EDITOR
		entt::registry clipboardRegistry; // Stop entities that are copied from being active in the scene until they are pasted.
		entt::entity  sceneRoot = entt::null;
		entt::entity  selectedEntity = entt::null;
		entt::entity  cutEntity = entt::null;
		entt::entity  copiedEntity = entt::null;
		bool deletePrompt = false;

		/// IMGUIZMO
		IMGUIZMO_NAMESPACE::OPERATION currentImGuizmoOperation = IMGUIZMO_NAMESPACE::TRANSLATE;
		IMGUIZMO_NAMESPACE::MODE currentImGuizmoMode = IMGUIZMO_NAMESPACE::WORLD;
		bool useImGuizmoSnapping = true;
		float snapImGuizmo[3] = { 1.f, 1.f, 1.f };

	public:
		LevelEditorSystem(entt::registry* sceneRegistry, entt::entity sceneRoot, Renderer_DirectX11* renderer);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput() override;
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
		// ImGuizmo
		void renderImGuizmoManipulateTool();
		void renderImGuizmoStatusWindow();
	};
}