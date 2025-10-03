#include "pch.h"
#include "Game.h"

/////

std::unique_ptr<CMP316engine::Application> CMP316engine::CreateApp(CMP316engine::EngineContext& engineContext)
{
	return std::make_unique<Game>(engineContext);
}

/////

bool Game::Initialize()
{
	// I yearn for the music
	int audioHandle = engineContext.audioManager->Play("MyJam");
	engineContext.audioManager->SetAudioLoop(audioHandle, true);

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
