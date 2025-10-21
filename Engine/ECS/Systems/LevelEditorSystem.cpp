#include "LevelEditorSystem.h"
#include <ImGui.h>
#include "../ECSHelper.h"
#include "../../Utility/ImGuiHelper.h"
#include "CameraSystem.h"
#include "Core/Reflection.h"

namespace CMP316engine {
	LevelEditorSystem::LevelEditorSystem(entt::registry* sceneRegistry, EngineContext* engineContext, entt::entity sceneRootEntity, Renderer_DirectX11* sceneRenderer) :
		System(sceneRegistry, engineContext), sceneRoot(sceneRootEntity), renderer(sceneRenderer), windowManager(engineContext->windowManager.get()), audioManager(engineContext->audioManager.get())
	{
		
	}

	bool LevelEditorSystem::Initialize()
	{
		IGFD::FileDialogConfig config;
		config.path = ".";
		config.countSelectionMax = -1;
		config.flags = ImGuiFileDialogFlags_NoDialog |
			ImGuiFileDialogFlags_DisableCreateDirectoryButton |
			ImGuiFileDialogFlags_ReadOnlyFileNameField;
		fileDialog.OpenDialog("embedded", "Select File", ".*", config);

		return true;
	}

	void LevelEditorSystem::Shutdown()
	{

	}

	void LevelEditorSystem::HandleInput(float deltaTime)
	{
		/// EDITOR SELECT RAYCAST

		// Will only cast if mouse is not over the UI.
		ImGuiIO& io = ImGui::GetIO();
		if (inputManager->IsMouseButtonPressed(SDL_BUTTON_LEFT) && !io.WantCaptureMouse)
		{
			raycastToSelectEntities();
		}

		/// EDITOR CLIPBOARD

		if (selectedEntity != entt::null)
		{
			if (inputManager->IsKeyBindingPressed("Copy"))
			{
				performCopy();
			}
			if (inputManager->IsKeyBindingPressed("Cut") && canCut())
			{
				performCut();
			}
			if (inputManager->IsKeyBindingPressed("Paste") && canPaste())
			{
				performPaste();
			}
			if (inputManager->IsKeyBindingPressed("Delete") && selectedEntity != sceneRoot)
			{
				deletePrompt = true;
			}
		}	

		/// CHANGE IMGUIZMO MODE / OPERATION

		if (ImGui::IsKeyPressed(ImGuiKey_1)) { currentImGuizmoOperation = ImGuizmo::TRANSLATE; }
		if (ImGui::IsKeyPressed(ImGuiKey_2)) { currentImGuizmoOperation = ImGuizmo::ROTATE; }
		if (ImGui::IsKeyPressed(ImGuiKey_3)) { currentImGuizmoOperation = ImGuizmo::SCALE; }
		if (ImGui::IsKeyPressed(ImGuiKey_4)) { currentImGuizmoMode = ImGuizmo::WORLD; }
		if (ImGui::IsKeyPressed(ImGuiKey_5)) { currentImGuizmoMode = ImGuizmo::LOCAL; }
		if (ImGui::IsKeyPressed(ImGuiKey_6)) { useImGuizmoSnapping = !useImGuizmoSnapping; }
	}

	void LevelEditorSystem::HandleImGui()
	{
		renderImGuizmoManipulateTool();
		renderImGuizmoStatusWindow();
		renderSceneTreeSelectionWindow();
		renderObjectInspectorWindow();
		renderFileExplorer();

		ImGui::ShowDemoWindow();
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

	static void DrawComponentHelper(entt::meta_any instance, entt::meta_custom custom, int& guiId)
	{
		auto meta = instance.type();

		// If the type has a bespoke DrawEditor function, use that. Otherwise, recurse over data members.
		// Currently, there is no behavior if the type/member has no DrawEditor function or any registered data members.
		if (auto func = meta.func("DrawEditor"_hs))
		{
			PropertiesMap map = {};
			if (auto* mp = static_cast<const PropertiesMap*>(custom))
			{
				map = *mp;
			}
			func.invoke(instance, map);
		}
		else
		{
			for (auto [id, data] : meta.data())
			{
				if (data.traits<Traits>() & Traits::EDITOR)
				{
					ImGui::PushID(guiId++);
					DrawComponentHelper(data.get(instance), data.custom(), guiId);
					ImGui::PopID();
				}
			}
		}
	}

	void LevelEditorSystem::renderObjectInspectorWindow()
	{
		ImGui::Begin("Object Inspector");
		
		// No Selected Entity
		if (selectedEntity == entt::null)
		{
			ImGui::TextDisabled("No Entity Selected");
			ImGui::Separator();
			ImGui::End();
			return;
		}

		/// COMPONENT CONTROLS

		// Iterate over all components in the registry.
		// https://gist.github.com/JuanDiegoMontoya/f6002350a9f5e64c962ee52d7e879922
		int i = 0;
		for (auto&& [id, storage] : registry->storage())
		{
			// The entity does not have the component
			if (!storage.contains(selectedEntity)) { continue; }

			// The name of the component is stored in the registry (not reflection as it turns out!) Create a header for the component.
			ImGui::PushID(("." + std::to_string(i)).c_str());
			//ImGui::SeparatorText(std::string(storage.type().name()).c_str());
			//ImGui::SameLine();
			// PREVENT REMOVING HIERARCHY COMPONENT IN LEVEL EDITOR
			if (id != entt::type_hash<HierarchyComponent>() && ImGui::Button("Remove"))
			{
				componentDeletePrompt = true;
				componentToDelete = id;
			}
			ImGui::SameLine();
			ImGui::SeparatorText(std::string(storage.type().name()).c_str());

			// Reflect the components
			if (auto meta = entt::resolve(id))
			{
				DrawComponentHelper(meta.from_void(storage.value(selectedEntity)), meta.custom(), i);
			}

			ImGui::PopID();
			i++;
		}
		
		/// ADD COMPONENT BUTTON

		ImGui::SeparatorText("");
		if (ImGui::BeginCombo("##AddComponentCombo", "Add Component"))
		{
			bool anyComponentsToAdd = false;
			for (auto [id, meta] : entt::resolve())
			{
				std::string_view name = meta.info().name();
				if (meta.traits<Traits>() & Traits::COMPONENT)
				{
					if (auto func = meta.func("HasComponent"_hs))
					{
						if (auto result = func.invoke({}, entt::forward_as_meta(*registry), selectedEntity); result) {
							if (result.cast<bool>() == true) {
								// Entity already has this component
								continue;
							}
						}
					}
					anyComponentsToAdd = true;

					if (ImGui::Selectable(name.data()))
					{
						if (auto func = meta.func("AddComponent"_hs))
						{
							if (auto result = func.invoke({}, entt::forward_as_meta(*registry), selectedEntity); result) {
								// Success
							}
							else {
								// Fail
								std::cout << "\nAddComponent Invoke call did not match reflected function signature";
							}
						}
						else {
							std::cout << "\nComponent hasn't got an 'AddComponent' function defined in the reflection system!";
						}
					}
				}
			}
			if (anyComponentsToAdd == false)
			{
				ImGui::TextDisabled("No Components Left to Add");
			}

			ImGui::EndCombo();
		}

		/// PROMPTS

		ImGuiHelper::PromptUser(componentDeletePrompt, [this]() {
			if (auto storage = registry->storage(componentToDelete)) {
				storage->remove(selectedEntity);
			}
			componentToDelete = entt::null;
			},
			"Deletion Confirmation", "Are you sure you want to delete the component?");

		ImGui::End();
	}

	void LevelEditorSystem::renderSelectionWindowManipulationTools()
	{
		bool noSelectedObject = false;
		if (selectedEntity == entt::null) { noSelectedObject = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Copy"))
		{
			performCopy();
		}
		ImGui::SameLine();
		bool sceneRootSelected = false;
		if (!canCut()) { sceneRootSelected = true; ImGui::BeginDisabled(); }
		if (ImGui::Button("Cut"))
		{
			performCut();
		}
		if (sceneRootSelected) { ImGui::EndDisabled(); }
		ImGui::SameLine();
		bool noObjectToPaste = false;
		// ENSURE that the user doesn't paste the cut object onto itself or its own parent.
		if (!canPaste()) { noObjectToPaste = true;  ImGui::BeginDisabled(); }
		if (ImGui::Button("Paste"))
		{
			performPaste();
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

		/// OBJECTS CHILDREN

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (currentObject == selectedObject) { treeNodeFlags |= ImGuiTreeNodeFlags_Selected; }
		if (rootHierarchyComponent.firstChild != entt::null) 
		{
			if (ImGui::TreeNodeEx((rootHierarchyComponent.name + "##ChildrenDropdown").c_str(), treeNodeFlags))
			{
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					selectedObject = currentObject;
				}

				int childNum = 0; // For ImGui ID
				ECS::CallForAllChildren(registry, currentObject, [&childNum, &selectedObject](entt::registry* registry, entt::entity childEntity) {
					ImGui::PushID(("." + std::to_string(childNum)).c_str());
					//ImGui::Indent();
					renderObjectSelectionWindowObjectTree(registry, childEntity, selectedObject);
					//ImGui::Unindent();
					ImGui::PopID();
					childNum++;
					});
				ImGui::TreePop();
			}
		}
		else {
			treeNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
			if (ImGui::TreeNodeEx((rootHierarchyComponent.name + "##ChildrenDropdown").c_str(), treeNodeFlags))
			{
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					selectedObject = currentObject;
				}

				ImGui::TreePop();
			}
		}
	}
	void LevelEditorSystem::renderImGuizmoManipulateTool()
	{
		/// IMGUIZMO MANIPULATOR

		TransformComponent* transformComponent = registry->try_get<TransformComponent>(selectedEntity);
		if (transformComponent)
		{
			DirectX::XMMATRIX viewMatrix = CameraSystem::GetActiveCameraViewMatrix(registry);
			DirectX::XMMATRIX projectionMatrix = renderer->GetProjectionMatrix();
			DirectX::XMMATRIX worldMatrix = transformComponent->worldMatrix;

			// ImGuizmo takes the matrices as an array of floats, so I need to convert.
			DirectX::XMFLOAT4X4 viewArray;
			DirectX::XMStoreFloat4x4(&viewArray, viewMatrix);
			DirectX::XMFLOAT4X4 projectionArray;
			DirectX::XMStoreFloat4x4(&projectionArray, projectionMatrix);
			DirectX::XMFLOAT4X4 worldArray;
			DirectX::XMStoreFloat4x4(&worldArray, worldMatrix);

			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

			/// DELTA WORLD MATRIX (array for manipulate tool)
			// May not use it after all, just creating it just incase. May be useful for rotation if I change up the matrix calculation.
			DirectX::XMFLOAT4X4 deltaArray;

			/// CREATE MANIPULATE TOOL
			ImGuizmo::Manipulate(
				*viewArray.m,
				*projectionArray.m,
				currentImGuizmoOperation,
				currentImGuizmoMode,
				*worldArray.m,
				*deltaArray.m,
				useImGuizmoSnapping ? &snapImGuizmo[0] : NULL
			);

			if (ImGuizmo::IsOver()) {
			}
			if (ImGuizmo::IsUsing())
			{
				/// UPDATE TRANSFORMS
				float position[3];
				float rotation[3];
				float scale[3];
				ImGuizmo::DecomposeMatrixToComponents(*worldArray.m, position, rotation, scale);
				auto* t = transformComponent;
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::TRANSLATE) { t->position = { position[0], position[1], position[2] }; }
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::ROTATE) { t->rotation = { rotation[0], rotation[1], rotation[2] }; } // NOTE: ImGuizmo uses degrees, 0-360
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE) { t->scale = { scale[0], scale[1], scale[2] }; }
			}
		}
	}

	void LevelEditorSystem::renderImGuizmoStatusWindow()
	{
		/// IMGUIZMO STATUS

		ImGui::Begin("ImGuizmo");
		auto DrawToggleButton = [](const char* label, bool active) {
			ImGui::PushStyleColor(ImGuiCol_Button, active ? ImVec4(0.2f, 0.6f, 1.0f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, active ? ImVec4(0.4f, 0.7f, 1.0f, 1.0f) : ImVec4(0.4f, 0.4f, 0.4f, 0.8f));
			bool clicked = ImGui::Button(label, ImVec2(80, 0));
			ImGui::PopStyleColor(2);
			return clicked;
			};

		ImGui::SameLine();
		ImGui::Text("OPERATION:");
		ImGui::SameLine();
		if (DrawToggleButton("Translate", currentImGuizmoOperation == IMGUIZMO_NAMESPACE::TRANSLATE)) {
			currentImGuizmoOperation = IMGUIZMO_NAMESPACE::TRANSLATE;
		}
		ImGui::SameLine();
		if (DrawToggleButton("Rotate", currentImGuizmoOperation == IMGUIZMO_NAMESPACE::ROTATE)) {
			currentImGuizmoOperation = IMGUIZMO_NAMESPACE::ROTATE;
		}
		ImGui::SameLine();
		if (DrawToggleButton("Scale", currentImGuizmoOperation == IMGUIZMO_NAMESPACE::SCALE)) {
			currentImGuizmoOperation = IMGUIZMO_NAMESPACE::SCALE;
		}

		ImGui::SameLine();
		ImGui::Text("MODE:");
		ImGui::SameLine();
		if (DrawToggleButton("World", currentImGuizmoMode == IMGUIZMO_NAMESPACE::WORLD)) {
			currentImGuizmoMode = IMGUIZMO_NAMESPACE::WORLD;
		}
		ImGui::SameLine();
		if (DrawToggleButton("Local", currentImGuizmoMode == IMGUIZMO_NAMESPACE::LOCAL)) {
			currentImGuizmoMode = IMGUIZMO_NAMESPACE::LOCAL;
		}

		ImGui::SameLine();
		ImGui::Text("			");
		ImGui::SameLine();
		bool snappingActive = useImGuizmoSnapping;
		if (DrawToggleButton("Snap", snappingActive)) {
			useImGuizmoSnapping = !useImGuizmoSnapping;
		}
		ImGui::End();
	}

	void LevelEditorSystem::renderFileExplorer()
	{
		if (ImGui::Begin("File Explorer##OpenDialogCommand")) {
			if (fileDialog.Display("embedded", ImGuiWindowFlags_NoCollapse, ImVec2(0, 350))) { // => will show a dialog
				if (fileDialog.IsOk()) { // action if OK
					std::string filePathName = fileDialog.GetFilePathName();
					std::string filePath = fileDialog.GetCurrentPath();

					/// ACTION

					/*
					TODO:
					e.g. save or load, maybe grab asset
					or select a file / asset for an engine importer window
					*/
				}
			}
		}
		ImGui::End();
	}
	void LevelEditorSystem::raycastToSelectEntities()
	{
		/*
		Based of : https://medium.com/@logandvllrd/how-to-pick-a-3d-object-using-raycasting-in-c-39112aed1987

		TODO:
		I REALLY need to clean up the math here. I was fumbling around with DirectX math just trying to get it to work. Currently a lot of inefficiencies,
		like barely even using the Ray struct.
		*/

		DirectX::XMFLOAT2 mousePosition = inputManager->GetMousePositionOnWindow();
		//std::cout << "\nMousePos: " << mousePosition.x << ", " << mousePosition.y; ///DEBUG

		/// GET WORLD POSITION BASED ON SCREEN POSITION

		auto viewMatrix = CameraSystem::GetActiveCameraViewMatrix(registry);
		auto viewport = renderer->GetViewport();
		DirectX::XMFLOAT3 window = { mousePosition.x, mousePosition.y, 0.f };
		DirectX::XMVECTOR pointVector = DirectX::XMVector3Unproject(DirectX::XMLoadFloat3(&window), viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, renderer->GetProjectionMatrix(), viewMatrix, XMMatrixIdentity());
		DirectX::XMFLOAT3 point = {};
		DirectX::XMStoreFloat3(&point, pointVector);

		/// CREATE RAY

		Ray ray;
		ray.origin = point;
		// Get direction of the ray based on camera's position
		DirectX::XMFLOAT3 cameraPosition = CameraSystem::GetActiveCameraPosition(registry);
		DirectX::XMStoreFloat3(&ray.direction, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(pointVector, DirectX::XMLoadFloat3(&cameraPosition))));
		//std::cout << "\nRAYSTART\nRayOrigin: " << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << "\nDirection: " << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << "\nRAYEND"; /// DEBUG

		/// CAST RAY AND CHECK ALL BOUNDING BOX ENTITIES FOR COLLISION

		RayHit rayHit;
		auto collidableEntities = registry->view<LevelEditorColliderComponent, TransformComponent>();
		for (auto& entity : collidableEntities) {
			auto [colliderComponent, transformComponent] = registry->get<LevelEditorColliderComponent, TransformComponent>(entity);

			// Skip the camera
			if (registry->try_get<LevelEditorCameraComponent>(entity)) { continue; }

			RayHit localRayHit;

			/// CONVERT TO LOCAL SPACE

			XMMATRIX inverseWorld = XMMatrixInverse(nullptr, transformComponent.worldMatrix);
			XMVECTOR localOrigin = XMVector3Transform(DirectX::XMLoadFloat3(&ray.origin), inverseWorld);
			XMVECTOR localDirection = XMVector3TransformNormal(DirectX::XMLoadFloat3(&ray.direction), inverseWorld);
			Ray localRay;
			DirectX::XMStoreFloat3(&localRay.origin, localOrigin);
			DirectX::XMStoreFloat3(&localRay.direction, localDirection);

			/// CAST AND CHECK FOR COLLISION

			XMVECTOR boundingBoxMin = DirectX::XMLoadFloat3(&colliderComponent.min);
			XMVECTOR boundingBoxMax = DirectX::XMLoadFloat3(&colliderComponent.max);
			XMVECTOR scaleVector = DirectX::XMLoadFloat3(&transformComponent.scale);
			boundingBoxMin = DirectX::XMVectorMultiply(boundingBoxMin, scaleVector);
			boundingBoxMax = DirectX::XMVectorMultiply(boundingBoxMax, scaleVector);

			XMVECTOR minVector = DirectX::XMVectorDivide(DirectX::XMVectorSubtract(boundingBoxMin, localOrigin), localDirection);
			XMVECTOR maxVector = DirectX::XMVectorDivide(DirectX::XMVectorSubtract(boundingBoxMax, localOrigin), localDirection);

			// Find the nearest and farthest intersections
			XMFLOAT3 nearVector;
			DirectX::XMStoreFloat3(&nearVector, DirectX::XMVectorMin(minVector, maxVector));
			XMFLOAT3 farVector;
			DirectX::XMStoreFloat3(&farVector, DirectX::XMVectorMax(minVector, maxVector));

			// Find the maximum of the nearest intersections
			float tNearMax = std::max(std::max(nearVector.x, nearVector.y), nearVector.z);
			// Find the minimum of the farthest intersections
			float tFarMin = std::min(std::min(farVector.x, farVector.y), farVector.z);

			// Check if the ray intersects the bounding box
			if (tNearMax <= tFarMin && localRayHit.distance > tNearMax)
			{
				localRayHit.distance = tNearMax;
				localRayHit.hit = true;

				if (localRayHit.distance < rayHit.distance)
				{
					rayHit = localRayHit;
					selectedEntity = entity;
				}
			}
		}

		if (rayHit.hit == false) {
			selectedEntity = entt::null;
		}
		else {
			audioManager->Play("ButtonPress1");
		}
	}

	bool LevelEditorSystem::canPaste() 
	{ 
		return !((cutEntity == entt::null && 
			copiedEntity == entt::null) || 
			(selectedEntity == cutEntity) || 
			ECS::IsDescendant(registry, cutEntity, selectedEntity)); 
	}
	void LevelEditorSystem::performCopy()
	{
		cutEntity = entt::null;
		copiedEntity = selectedEntity; // The actual copy will be created on pasting, unfortunately can't keep the copy if object is deleted.

		//// TEMP
		//clipboardRegistry.clear();
		//copiedObject = ECS::CopyEntityBetweenRegistries(registry, &clipboardRegistry, selectedObject);
		//// TEMP
	}
	void LevelEditorSystem::performCut()
	{
		copiedEntity = entt::null;
		cutEntity = selectedEntity;
	}
	void LevelEditorSystem::performPaste()
	{
		if (cutEntity != entt::null)
		{
			selectedEntity = ECS::AddChild(registry, selectedEntity, cutEntity);
			cutEntity = entt::null;
		}
		else if (copiedEntity != entt::null)
		{
			//selectedEntity = ECS::AddChild(registry, selectedObject, ECS::CopyEntityBetweenRegistries(registry, &clipboardRegistry, copiedObject));
			selectedEntity = ECS::AddChild(registry, selectedEntity, ECS::CopyEntity(registry, copiedEntity));
		}
	}
}