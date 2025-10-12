#include "Game.h"
#include "TestScene.h"
#include "Core/LevelEditorScene.h"

std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
{
	return std::make_unique<Game>(engineContext);
}

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

void Game::HandleInput(float deltaTime)
{
	Application::HandleInput(deltaTime);
	activeScene->HandleInput(deltaTime);

	/// TEMP
	if (engineContext.inputManager->IsKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		activeScene->Shutdown();
		activeScene = std::make_unique<CMP316engine::LevelEditorScene>(engineContext);
		activeScene->Initialize();
	}
	/// TEMP
}

void Game::HandleImGui()
{
	activeScene->HandleImGui();

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
