#include "pch.h"
#include "Application.h"

bool CMP316engine::Application::Initialize()
{
	return true;
}

void CMP316engine::Application::Shutdown()
{

}

void CMP316engine::Application::HandleInput()
{
	if (engineContext.inputManager->IsKeyBindingPressed("fullscreen")) 
	{
		engineContext.windowManager->FullscreenWindow();
	}
}

void CMP316engine::Application::HandleImGui()
{

}

void CMP316engine::Application::Update(float deltaTime)
{

}

void CMP316engine::Application::Render()
{

}
