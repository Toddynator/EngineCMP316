#pragma once
#include "Core/Application.h"
#include "Core/EngineContext.h"

class Game
	: public CMP316engine::Application
{
public:
	Game(CMP316engine::EngineContext& context) : Application(context) {}

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;
};