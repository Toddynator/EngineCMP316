#include "LevelEditorSystem.h"
#include <ImGui.h>

namespace CMP316engine {
	LevelEditorSystem::LevelEditorSystem(entt::registry* sceneRegistry, entt::entity sceneRootObject) : System(sceneRegistry), sceneRoot(sceneRootObject)
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
		renderSelectionWindowObjectTree(sceneRoot, selectedObject);
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
		if (selectedObject == entt::null) { noSelectedObject = true;  ImGui::BeginDisabled(); }
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
		if ((cutObject == entt::null && copiedObject == entt::null)) { noObjectToPaste = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Paste"))
		{
			if (cutObject != entt::null)
			{
				//selectedObject->AddChild(cutObject->GetParent()->MoveChild(cutObject));
				//cutObject = nullptr;
			}
			else if (copiedObject != entt::null)
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

	void LevelEditorSystem::renderSelectionWindowObjectTree(entt::entity& currentObject, entt::entity& selectedObject)
	{
		auto& rootHierarchyComponent = registry->get<HierarchyComponent>(currentObject);

		/// OBJECT SELECT BUTTON

		bool buttonWasHighlighted = false;
		if (currentObject == selectedObject) { buttonWasHighlighted = true;  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f)); }
		if (ImGui::Button(rootHierarchyComponent.name.c_str())) {
			// Select Object if pressed
			selectedObject = currentObject;
		}
		if (buttonWasHighlighted) { ImGui::PopStyleColor(); }

		/// OBJECTS CHILDREN







		/// FROM CMP301:
		/*// Render button for object
		bool buttonWasHighlighted = false;
		if (currentObject == selectedObject) { buttonWasHighlighted = true;  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f)); }
		if (ImGui::Button(currentObject->GetName().c_str())) {
			// Select Object if pressed
			selectedObject = currentObject;
		}
		if (buttonWasHighlighted) { ImGui::PopStyleColor(); }

		if (!currentObject->GetChildren().empty()) {
			ImGui::SameLine();
			if (ImGui::TreeNodeEx("##ChildrenDropdown", ImGuiTreeNodeFlags_DefaultOpen)) {
				// Render for children - Note the indent as you get deeper into the hierarchy.
				std::unordered_map<std::string, int> childCount;
				currentObject->CallForAllChildren([&childCount, &selectedObject](SceneObject* object) {
					ImGui::PushID(GenerateObjectID(childCount, object->GetName()).c_str());
					ImGui::Indent();
					RenderObjectAndChildrenImGuiSelection(object, selectedObject);
					ImGui::Unindent();
					ImGui::PopID();
					});
				ImGui::TreePop();
			}
		}*/
	}
}