#pragma once
#include "Core/Application.h"

enum Levels
{
	LEVEL_EDITOR,
	TEST_LEVELFILE_SCENE
};

class Game
	: public NomadEngine::Application
{
public:
	Game(NomadEngine::EngineContext& context) : Application(context) {}

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput(float deltaTime) override;
	void HandleImGui() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	bool wireframeEnabled = false;
};