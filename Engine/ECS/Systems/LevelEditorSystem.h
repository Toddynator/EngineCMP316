/*
Creates an Editor UI for creating a scene and provides the ability to save and load scenes.
To be utilized, it should added as a system in a Level Editor Scene. A base level editor scene
will be added, but developers can choose to create new level editor scenes in case they want
to add their own systems that should be running during the editing process.
HOWEVER this could be improved by adding functionality in the editor for adding systems.

Key Features:
- Object Tree for selection of entities and creating, copying & cutting Entities.
- Object Inspector for editing of entities via modifying, removing and adding components.
- ImGuizmo Manipulator for convenient control over the placement of objects in the scene,
allows translation, rotation & scale, as well as snapping or world vs local space transforms.
- Raycast for convenient selection of objects instead of being forced to use the object tree.
Notably objects will need a bounding box created to use this, which is best handled by a system
which creates the Mesh.
- Saving & Loading, lets you load existing Level files as well as save Files which can then
be utilized by the Game via a SceneManager or alternative use.

TODO:
- Saving & Loading of entities (Which includes any children). This could work similar
to Unity Prefabs, where you can save a particular preset of an entity with any components and children entities,
then repeatedly paste that into the scene. This would be like the copy mechanic, but better since it now persists
between sessions of running the application, and means you can save multiple configurations and access them again easily.
- Improved copying : Currently copying is not storing a unique instance, what I will need to do is instead create a temporary registry
to then copy over the entity, then I can copy it back into the registry as a child of whatever entity I paste it onto. Alternatively
I could just serialize and deserialize it which may be far easier, means I also can have the copy persist between sessions.
- Some way of adding and removing systems required by the scene being created, this means the game can then utilize just the ECS scene
and don't need to create a class with the sole purpose of pushing their systems to the system stack.
*/

#pragma once
#include "../System.h"
#include <entt.hpp>
#include "../../Graphics/Renderer_DirectX11.h"
#include "../../ImGui/ImGuizmo/ImGuizmo.h"
#include "../../ImGui/ImGuiFileDialog/ImGuiFileDialog.h"

namespace NomadEngine {
	class LevelEditorSystem : public System
	{
	private:
		/// REQUIRED SERVICES
		Renderer_DirectX11* renderer;
		WindowManager* windowManager;
		AudioManager* audioManager;

		/// EDITOR
		ImGuiFileDialog fileDialogFileExplorer;
		entt::registry clipboardRegistry; // Stop entities that are copied from being active in the scene until they are pasted.
		entt::entity  sceneRoot = entt::null;
		entt::entity  selectedEntity = entt::null;
		entt::entity  cutEntity = entt::null;
		entt::entity  copiedEntity = entt::null;
		bool deletePrompt = false;
		bool componentDeletePrompt = false;
		entt::id_type componentToDelete = entt::null;

		/// IMGUIZMO
		IMGUIZMO_NAMESPACE::OPERATION currentImGuizmoOperation = IMGUIZMO_NAMESPACE::TRANSLATE;
		IMGUIZMO_NAMESPACE::MODE currentImGuizmoMode = IMGUIZMO_NAMESPACE::WORLD;
		bool useImGuizmoSnapping = false;
		float snapImGuizmo[3] = { 1.f, 1.f, 1.f };

	public:
		LevelEditorSystem(entt::registry* sceneRegistry, EngineContext* engineContext, entt::entity sceneRoot, Renderer_DirectX11* renderer);

		bool Initialize() override;
		void Shutdown() override;

		void HandleInput(float deltaTime) override;
		void HandleImGui() override;
		void Update(float deltaTime) override;

	private:
		// Display the hierarchy, no extra details
		void renderSceneTreeSelectionWindow();
		// Display the selected objects details
		void renderObjectInspectorWindow();
		// Cut, Copy, Paste, Delete
		void renderSelectionWindowManipulationTools();
		static void renderObjectSelectionWindowObjectTree(entt::registry* registry, entt::entity currentObject, entt::entity& selectedObject, AudioManager* audioManager);
		// ImGuizmo
		void renderImGuizmoManipulateTool();
		void renderImGuizmoStatusWindow();
		void renderFileExplorer();

		void raycastToSelectEntities();

		/// CLIPBOARD HELPER FUNCTIONS
		bool canCut() { return !(selectedEntity == sceneRoot); }
		bool canPaste();
		void performCopy();
		void performCut();
		void performPaste();
	};
}