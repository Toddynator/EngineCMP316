#pragma once
#include "Core/Application.h"

class Game
	: public CMP316engine::Application
{
public:
	Game();

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;
};