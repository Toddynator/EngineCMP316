/*
For Testing out game objects & engine features
*/

#pragma once
#include "Scene.h"

class TestScene :
	public CMP316engine::Scene
{
	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;
};

