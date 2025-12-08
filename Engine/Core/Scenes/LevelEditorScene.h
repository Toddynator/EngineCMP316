/*
A Level editor scene.
It can:
- Save & Load Scenes. 
No PhysicsSystem means that the scene will be static.
- Renders a full UI Editor for modifying the scene hierarchy, capable of adding and removing GameObjects and their components.
*/

#pragma once
#include "ECSScene.h"
#include "ImGui/ImGuiFileDialog/ImGuiFileDialog.h"

namespace CMP316engine
{
	class LevelEditorScene
		: public ECSScene
	{
	private:
		ImGuiFileDialog fileDialog; // This might not be necessary, since I only ever plan to have one modal file dialog at a time (Could use the singleton dialog)

	public:
		LevelEditorScene(CMP316engine::EngineContext& context);
		bool Initialize() override;

		void HandleImGui() override;

		void Load() override;

	private:
		void createLevelEditorCameraEntity();
	};
}