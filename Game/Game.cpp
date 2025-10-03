#include "pch.h"
#include "Game.h"

//std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
//{
//	return std::make_unique<Game>(engineContext);
//}
std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp()
{
	return std::make_unique<Game>();
}



bool Game::Initialize()
{
	std::cout << "\nTEST: The Game Project is successfully Linked!!!";
	return true;
}

void Game::Shutdown()
{

}

void Game::HandleInput()
{

}

void Game::HandleImgui()
{

}

void Game::Update(float deltaTime)
{

}

void Game::Render()
{

}
