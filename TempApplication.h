/*
Temporary Application class for testing out engine features,
later should have the engine link to external projects as to separate the game from the engine.
*/

#pragma once
#include "Application.h"
#include "Scene.h"

class TempApplication :
	public CMP316engine::Application
{
public:
	TempApplication() {}
	~TempApplication() = default;

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	std::unique_ptr<CMP316engine::Scene> scene; // Currently just one for now
};

