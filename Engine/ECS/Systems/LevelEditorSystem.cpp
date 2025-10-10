#include "LevelEditorSystem.h"
#include <ImGui.h>

namespace CMP316engine {
	LevelEditorSystem::LevelEditorSystem(entt::registry* sceneRegistry, GameObject* sceneRootObject) : System(sceneRegistry), sceneRoot(sceneRootObject)
	{

	}

	bool LevelEditorSystem::Initialize()
	{
		return true;
	}

	void LevelEditorSystem::Shutdown()
	{

	}

	void LevelEditorSystem::HandleImGui()
	{
		renderSceneTreeSelectionWindow();
		renderObjectInspectorWindow();
	}

	void LevelEditorSystem::Update(float deltaTime)
	{

	}

	void LevelEditorSystem::renderSceneTreeSelectionWindow()
	{
		ImGui::Begin("Scene GameObject Tree");
		renderSelectionWindowManipulationTools();
		ImGui::Separator();
		renderSelectionWindowObjectTree();
		ImGui::End();
	}

	void LevelEditorSystem::renderObjectInspectorWindow()
	{
		ImGui::Begin("Object Inspector");

		ImGui::End();
	}

	void LevelEditorSystem::renderSelectionWindowManipulationTools()
	{
		bool noSelectedObject = false;
		if (!selectedObject) { noSelectedObject = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Copy"))
		{
			//cutObject = nullptr;
			//copiedObject = selectedObject->Clone();
		}
		ImGui::SameLine();
		bool sceneRootSelected = false;
		if (selectedObject == sceneRoot) { sceneRootSelected = true; ImGui::BeginDisabled(); }
		if (ImGui::Button("Cut"))
		{
			//copiedObject = nullptr;
			//cutObject = selectedObject;
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		bool noObjectToPaste = false;
		if ((!cutObject && !copiedObject)) { noObjectToPaste = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Paste"))
		{
			if (cutObject)
			{
				//selectedObject->AddChild(cutObject->GetParent()->MoveChild(cutObject));
				//cutObject = nullptr;
			}
			else if (copiedObject)
			{
				//selectedObject->AddChild(copiedObject->Clone());
			}
		}
		if (noObjectToPaste) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		if (sceneRootSelected) { ImGui::BeginDisabled(); }
		if (ImGui::Button("Delete"))
		{
			//selectedObject->MarkObjectToBeDeleted();
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		if (noSelectedObject) { ImGui::EndDisabled(); }
	}

	void LevelEditorSystem::renderSelectionWindowObjectTree()
	{

	}
}