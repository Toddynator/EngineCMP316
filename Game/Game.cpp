#include "pch.h"
#include "Game.h"

std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp()
{
	return std::make_unique<Game>();
}

Game::Game()
{

}

bool Game::Initialize()
{
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
	std::cout << "\nTEST: The Game Project is successfully Linked!!!";
}

void Game::Render()
{

}
