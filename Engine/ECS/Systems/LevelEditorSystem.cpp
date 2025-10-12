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
		renderObjectSelectionWindowObjectTree(registry, sceneRoot, selectedEntity);
		ImGui::End();
	}

	void LevelEditorSystem::renderObjectInspectorWindow()
	{
		ImGui::Begin("Object Inspector");
		
		/*
		TODO:
		- Add Reflection, and loop through components, using a ImGuiUIOperator class to generate the ImGui Controls for each variable.
		- An Add Button at the bottom which creates a dropdown of all the components that haven't been added yet to the entity.
		*/

		ImGui::End();
	}

	void LevelEditorSystem::renderSelectionWindowManipulationTools()
	{
		bool noSelectedObject = false;
		if (selectedEntity == entt::null) { noSelectedObject = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Copy"))
		{
			cutEntity = entt::null;
			copiedEntity = selectedEntity; // The actual copy will be created on pasting, unfortunately can't keep the copy if object is deleted.

			//// TEMP
			//clipboardRegistry.clear();
			//copiedObject = ECS::CopyEntityBetweenRegistries(registry, &clipboardRegistry, selectedObject);
			//// TEMP
		}
		ImGui::SameLine();
		bool sceneRootSelected = false;
		if (selectedEntity == sceneRoot) { sceneRootSelected = true; ImGui::BeginDisabled(); }
		if (ImGui::Button("Cut"))
		{
			copiedEntity = entt::null;
			cutEntity = selectedEntity;
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		bool noObjectToPaste = false;
		// ENSURE that the user doesn't paste the cut object onto itself or its own parent.
		if ((cutEntity == entt::null && copiedEntity == entt::null) || (selectedEntity == cutEntity) || ECS::IsDescendant(registry, cutEntity, selectedEntity)) { noObjectToPaste = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Paste"))
		{
			if (cutEntity != entt::null)
			{
				ECS::AddChild(registry, selectedEntity, cutEntity);
				cutEntity = entt::null;
			}
			else if (copiedEntity != entt::null)
			{
				//ECS::AddChild(registry, selectedObject, ECS::CopyEntityBetweenRegistries(registry, &clipboardRegistry, copiedObject));
				ECS::AddChild(registry, selectedEntity, ECS::CopyEntity(registry, copiedEntity));
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
			ECS::AddChild(registry, selectedEntity);
		}
		if (noSelectedObject) { ImGui::EndDisabled(); }

		/// PROMPTS

		ImGuiHelper::PromptUser(deletePrompt, [this]() {
			// Remove CutObject if and only when it is deleted
			if (selectedEntity == cutEntity || ECS::IsDescendant(registry, cutEntity, selectedEntity))
			{
				cutEntity = entt::null;
			}
			ECS::RemoveChild(registry, selectedEntity);
			selectedEntity = entt::null;
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