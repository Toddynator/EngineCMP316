#include "pch.h"
#include "Application.h"

bool NomadEngine::Application::Initialize()
{
	return true;
}

void NomadEngine::Application::Shutdown()
{
	//engineContext.sceneManager->GetActiveScene()->Shutdown();
}

void NomadEngine::Application::HandleInput(float deltaTime)
{
	if (engineContext.inputManager->IsKeyBindingPressed("fullscreen")) 
	{
		engineContext.windowManager->FullscreenWindow();
	}

	engineContext.sceneManager->GetActiveScene()->HandleInput(deltaTime);
}

void NomadEngine::Application::HandleImGui()
{
	engineContext.sceneManager->GetActiveScene()->HandleImGui();
}

void NomadEngine::Application::Update(float deltaTime)
{
	engineContext.sceneManager->GetActiveScene()->Update(deltaTime);
}

void NomadEngine::Application::Render()
{
	engineContext.sceneManager->GetActiveScene()->Render();
}
