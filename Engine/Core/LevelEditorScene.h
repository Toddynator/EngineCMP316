/*
A Level editor scene.
It can:
- Save & Load Scenes. 
No PhysicsSystem means that the scene will be static.
- Renders a full UI Editor for modifying the scene hierarchy, capable of adding and removing GameObjects and their components.

TODO:
- LevelEditorSystem
- ObjectTree Selection Window
- Copy, Cut & Paste Buttons on Selection Window
- Object Inspector Window
- Reflection (either use Entt or look for potentially better alternatives)
- Generate the UI for components via reflection.
- ImGuizmo
*/

#pragma once
#include "ECSScene.h"

namespace CMP316engine
{
	class LevelEditorScene
		: public ECSScene
	{
	public:
		LevelEditorScene(CMP316engine::EngineContext& context);
		bool Initialize() override;
		void HandleImGui() override;
	};
}