#pragma once
#include "Core/Application.h"

enum Levels
{
	LEVEL_EDITOR,
	TEST_SCENE,
	TEST_LEVELFILE_SCENE
};

class Game
	: public CMP316engine::Application
{
public:
	Game(CMP316engine::EngineContext& context) : Application(context) {}

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput(float deltaTime) override;
	void HandleImGui() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	bool wireframeEnabled = false;
};