#include "pch.h"
#include "Game.h"
#include "TestScene.h"

/////

std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
{
	return std::make_unique<Game>(engineContext);
}

/////

bool Game::Initialize()
{
	activeScene = std::make_unique<TestScene>(engineContext);
	activeScene->Initialize();
	return true;
}

void Game::Shutdown()
{
	if (activeScene) { activeScene->Shutdown(); }
}

void Game::HandleInput()
{
	Application::HandleInput();
	activeScene->HandleInput();
}

void Game::HandleImgui()
{
	activeScene->HandleImgui();

	ImGui::Begin("ApplicationControls");
	if (ImGui::Checkbox("Wireframe", &wireframeEnabled))
	{
		engineContext.renderer->ToggleWireframe();
	}
	ImGui::End();
}

void Game::Update(float deltaTime)
{
	activeScene->Update(deltaTime);
}

void Game::Render()
{
	activeScene->Render();
}
