#include "LevelEditorScene.h"

namespace CMP316engine
{
	LevelEditorScene::LevelEditorScene(CMP316engine::EngineContext& context) : ECSScene(context)
	{
		// Maybe a LevelEditorSystem? It could encapsulate all the UI logic for interefacing with hierarchyComponent Objects.
	}

	bool LevelEditorScene::Initialize()
	{
		CMP316engine::ECSScene::Initialize();
		return true;
	}

	void LevelEditorScene::HandleImGui()
	{

	}
}