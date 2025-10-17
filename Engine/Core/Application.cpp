#include "pch.h"
#include "Application.h"

bool CMP316engine::Application::Initialize()
{
	return true;
}

void CMP316engine::Application::Shutdown()
{
	//engineContext.sceneManager->GetActiveScene()->Shutdown();
}

void CMP316engine::Application::HandleInput(float deltaTime)
{
	if (engineContext.inputManager->IsKeyBindingPressed("fullscreen")) 
	{
		engineContext.windowManager->FullscreenWindow();
	}

	engineContext.sceneManager->GetActiveScene()->HandleInput(deltaTime);
}

void CMP316engine::Application::HandleImGui()
{
	engineContext.sceneManager->GetActiveScene()->HandleImGui();
}

void CMP316engine::Application::Update(float deltaTime)
{
	engineContext.sceneManager->GetActiveScene()->Update(deltaTime);
}

void CMP316engine::Application::Render()
{
	engineContext.sceneManager->GetActiveScene()->Render();
}
