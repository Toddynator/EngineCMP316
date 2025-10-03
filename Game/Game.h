#pragma once
#include "Core/Application.h"
#include "Core/EngineContext.h"
/////
#include "Graphics/Camera.h"
#include "Graphics/Model.h"

class Game
	: public CMP316engine::Application
{
public:
	Game(CMP316engine::EngineContext& context) : Application(context) {}

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<CMP316engine::Model> model = nullptr;

	bool wireframeEnabled;

	/// TESTING VARIABLES
	// NOTE: Temp, just for testing
	//JPH::BodyID modelPhysicsBodyID;
};