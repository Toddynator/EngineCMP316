#pragma once
#include "Core/Scene.h"

class TestScene
	: public CMP316engine::Scene
{
public:
	TestScene(CMP316engine::EngineContext& context) : Scene(context) {}

	bool Initialize() override;
	void Shutdown() override;

	void HandleInput() override;
	void HandleImgui() override;
	void Update(float deltaTime) override;
	void Render() override;

private:
	/// TESTING VARIABLES
	// NOTE: Temp, just for testing
	//JPH::BodyID modelPhysicsBodyID;
};

