#pragma once
#include "Core/ECSScene.h"

class TestScene
	: public CMP316engine::ECSScene
{
public:
	TestScene(CMP316engine::EngineContext& context);

	bool Initialize() override;
	void HandleImGui() override;
};

