#include "TempApplication.h"
#include "TestScene.h"

bool TempApplication::Initialize()
{
	scene = std::make_unique<TestScene>();

	return true;
}

void TempApplication::Shutdown()
{

}

void TempApplication::HandleInput()
{
	scene->HandleInput();
}

void TempApplication::HandleImgui()
{
	scene->HandleImgui();
}

void TempApplication::Update(float deltaTime)
{
	scene->Update(deltaTime);
}

void TempApplication::Render()
{
	scene->Render();
}
