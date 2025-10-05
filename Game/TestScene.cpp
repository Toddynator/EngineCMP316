#include "pch.h"
#include "TestScene.h"
#include "ECS/GameObject.h"
#include "ECS/EngineECSSystems.h"
#include "ECS/Components.h"

bool TestScene::Initialize()
{
	///// MUSIC

	// I yearn for the music
	int audioHandle = engineContext.audioManager->Play("MyJam");
	engineContext.audioManager->SetAudioLoop(audioHandle, true);

	/////

	HWND hwnd = engineContext.windowManager->GetHWND();
	// Create and initialize the texture shader object.
	engineContext.shader = std::make_unique<Shader>();
	if (!engineContext.shader->Initialize(engineContext.renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = std::make_unique<Camera>();
	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -5.0f);


	////////////////////
	/// PHYSICS TEST ///

	// We'll just associate this with our model for now
	//JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
	//modelPhysicsBodyID = engineContext.physicsManager->GetBodyInterface().CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	//engineContext.physicsManager->GetBodyInterface().SetLinearVelocity(modelPhysicsBodyID, JPH::Vec3(0.1f, 0.1f, 0.0f));
	//
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);



	////////// ECS TEST

	systems.push_back(std::make_unique<CMP316engine::RenderSystem>(&registry, engineContext.renderer.get(), engineContext.shader.get()));
	systems.push_back(std::make_unique<CMP316engine::CameraSystem>(&registry));
	for (auto& system : systems)
	{
		system->Initialize();
	}

	sceneTree = std::make_unique<CMP316engine::GameObject>(&registry);
	auto& modelComponent = sceneTree->AddComponent<CMP316engine::ModelComponent>();
	modelComponent.filepath = "data/Models/Dug/Dug.obj";
	auto& meshComponent = sceneTree->AddComponent<CMP316engine::MeshComponent>();

	return true;
}

void TestScene::Shutdown()
{
	for (auto& system : systems)
	{
		system->Shutdown();
	}
}

void TestScene::HandleInput()
{

}

void TestScene::HandleImgui()
{
	ImGui::Begin("SceneControls");
	
	ImGui::End();
}

void TestScene::Update(float deltaTime)
{
	for (auto& system : systems)
	{
		system->Update();
	}

	camera->Update(); // Generate the view matrix based on the camera's position.

	/// PHYSICS TEST

	//engineContext.physicsManager->Update(engineContext.timeManager->getDeltaTime());
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	//model->SetPosition(XMFLOAT3(position.GetX(), position.GetY(), position.GetZ()));
}

void TestScene::Render()
{
	XMMATRIX viewMatrix = camera->GetViewMatrix();
	CMP316engine::RenderSystem::RenderModels(&registry, engineContext.renderer.get(), engineContext.shader.get(), viewMatrix);
}
