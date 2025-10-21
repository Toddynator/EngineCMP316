#include "Game.h"
#include "GameScene.h"
#include "Core/LevelEditorScene.h"

std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
{
	return std::make_unique<Game>(engineContext);
}

bool Game::Initialize()
{
	Application::Initialize();
	
	/// REGISTER GAME SCENES

	engineContext.sceneManager->RequestSceneChange(LEVEL_EDITOR); // Sets starting scene
	engineContext.sceneManager->RegisterScene<CMP316engine::LevelEditorScene>(LEVEL_EDITOR);
	engineContext.sceneManager->RegisterScene<GameScene>(TEST_LEVELFILE_SCENE, "Placeholder");

	return true;
}

void Game::Shutdown()
{
	Application::Shutdown();
}

void Game::HandleInput(float deltaTime)
{
	Application::HandleInput(deltaTime);

	/// TEMP // TESTING SCENE MANAGER
	if (engineContext.inputManager->IsKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		engineContext.sceneManager->RequestSceneChange(LEVEL_EDITOR);
	}
	if (engineContext.inputManager->IsKeyPressed(SDL_SCANCODE_L))
	{
		engineContext.sceneManager->RequestSceneChange(TEST_LEVELFILE_SCENE);
	}
	/// TEMP
}

void Game::HandleImGui()
{
	Application::HandleImGui();

	ImGui::Begin("ApplicationControls");
	ImGui::Text("FPS: %.2f", engineContext.timeManager->GetFPS());
	if (ImGui::Checkbox("Wireframe", &wireframeEnabled))
	{
		engineContext.renderer->ToggleWireframe();
	}
	ImGui::End();
}

void Game::Update(float deltaTime)
{
	Application::Update(deltaTime);
}

void Game::Render()
{
	Application::Render();
}
