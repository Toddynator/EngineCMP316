#pragma once
#include "ECSScene.h"

namespace CMP316engine
{
	class LevelEditorScene
		: public ECSScene
	{
		LevelEditorScene(CMP316engine::EngineContext& context);
		bool Initialize() override;
		void HandleImGui() override;
	};
}