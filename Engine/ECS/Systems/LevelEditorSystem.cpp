#include "LevelEditorSystem.h"
#include <ImGui.h>
#include "../ECSHelper.h"
#include "../../Utility/ImGuiHelper.h"

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
		renderObjectSelectionWindowObjectTree(registry, sceneRoot, selectedObject);
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
			cutObject = entt::null;
			//copiedObject = selectedObject->Clone(); //// TODO
		}
		ImGui::SameLine();
		bool sceneRootSelected = false;
		if (selectedObject == sceneRoot) { sceneRootSelected = true; ImGui::BeginDisabled(); }
		if (ImGui::Button("Cut"))
		{
			copiedObject = entt::null;
			cutObject = selectedObject;
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		bool noObjectToPaste = false;
		// ENSURE that the user doesn't paste the cut object onto itself or its own parent.
		if ((cutObject == entt::null && copiedObject == entt::null) || (selectedObject == cutObject) || ECS::IsDescendant(registry, cutObject, selectedObject)) { noObjectToPaste = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Paste"))
		{
			if (cutObject != entt::null)
			{
				ECS::AddChild(registry, selectedObject, cutObject);
				cutObject = entt::null;
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
			deletePrompt = true;
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			//// TODO
		}
		if (noSelectedObject) { ImGui::EndDisabled(); }

		/// PROMPTS

		ImGuiHelper::PromptUser(deletePrompt, [this]() {
			// Remove CutObject if and only when it is deleted
			if (selectedObject == cutObject || ECS::IsDescendant(registry, cutObject, selectedObject))
			{
				cutObject = entt::null;
			}
			ECS::RemoveChild(registry, selectedObject);
			selectedObject = entt::null;
			},
			"Deletion Confirmation", "Are you sure you want to delete the entity?");
	}

	void LevelEditorSystem::renderObjectSelectionWindowObjectTree(entt::registry* registry, entt::entity currentObject, entt::entity& selectedObject)
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

		if (rootHierarchyComponent.firstChild != entt::null) 
		{
			ImGui::SameLine();
			if (ImGui::TreeNodeEx("##ChildrenDropdown", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				int childNum = 0; // For ImGui ID
				ECS::CallForAllChildren(registry, currentObject, [&childNum, &selectedObject](entt::registry* registry, entt::entity childEntity) {
					ImGui::PushID(("." + std::to_string(childNum)).c_str());
					ImGui::Indent();
					renderObjectSelectionWindowObjectTree(registry, childEntity, selectedObject);
					ImGui::Unindent();
					ImGui::PopID();
					childNum++;
					});
				ImGui::TreePop();
			}
		}
	}
}