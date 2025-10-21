#pragma once
#include "Core/ECSScene.h"

class GameScene
	: public CMP316engine::ECSScene
{
public:
	GameScene(CMP316engine::EngineContext& context);

	bool Initialize() override;
};

