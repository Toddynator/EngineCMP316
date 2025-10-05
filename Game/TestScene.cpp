#include "pch.h"
#include "TestScene.h"
#include "ECS/GameObject.h"
#include "ECS/EngineECSSystems.h"
#include "ECS/Components.h"

TestScene::TestScene(CMP316engine::EngineContext& context) : Scene(context)
{
	systems.push_back(std::make_unique<CMP316engine::RenderSystem>(&registry, engineContext.renderer.get(), engineContext.shader.get()));
	systems.push_back(std::make_unique<CMP316engine::CameraSystem>(&registry));
	systems.push_back(std::make_unique<CMP316engine::PhysicsSystem>(&registry));
}

bool TestScene::Initialize()
{
	///// SOUND

	// I yearn for the music
	int audioHandle = engineContext.audioManager->Play("MyJam");
	engineContext.audioManager->SetAudioLoop(audioHandle, true);

	///// SHADER

	HWND hwnd = engineContext.windowManager->GetHWND();
	// Create and initialize the texture shader object.
	engineContext.shader = std::make_unique<Shader>();
	if (!engineContext.shader->Initialize(engineContext.renderer->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	///// PHYSICS TEST

	// We'll just associate this with our model for now
	//JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0f, 0.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, 1);
	//modelPhysicsBodyID = engineContext.physicsManager->GetBodyInterface().CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	//engineContext.physicsManager->GetBodyInterface().SetLinearVelocity(modelPhysicsBodyID, JPH::Vec3(0.1f, 0.1f, 0.0f));
	//
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);

	///// SYSTEMS

	for (auto& system : systems)
	{
		system->Initialize();
	}

	///// SCENE OBJECTS

	sceneTree = std::make_unique<CMP316engine::GameObject>(&registry);
	auto& modelComponent = sceneTree->AddComponent<CMP316engine::ModelComponent>();
	modelComponent.filepath = "data/Models/Dug/Dug.obj";
	auto& meshComponent = sceneTree->AddComponent<CMP316engine::MeshComponent>();

	auto cameraObject = sceneTree->AddChild();
	cameraObject->AddComponent<CMP316engine::CameraComponent>();
	auto transforms = cameraObject->GetComponent<CMP316engine::TransformComponent>();
	if (transforms) { 
		transforms->position = DirectX::XMFLOAT3(0.f, 0.f, -5.0f); 
	}

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

void TestScene::HandleImGui()
{
	ImGui::Begin("SceneControls");
	
	auto transformableEntities = registry.view<CMP316engine::TransformComponent>();
	for (auto& entity : transformableEntities) {
		auto& transforms = registry.get<CMP316engine::TransformComponent>(entity);

		ImGui::PushID(entt::to_integral(entity));
		ImGui::InputFloat3("Position: ", &transforms.position.x);
		ImGui::SliderFloat3("Rotation: ", &transforms.rotation.x, 0.f, 6.3f);
		ImGui::PopID();
	}

	ImGui::End();
}

void TestScene::Update(float deltaTime)
{
	for (auto& system : systems)
	{
		system->Update();
	}

	/// PHYSICS TEST
	//engineContext.physicsManager->Update(engineContext.timeManager->getDeltaTime());
	//JPH::RVec3 position = engineContext.physicsManager->GetBodyInterface().GetCenterOfMassPosition(modelPhysicsBodyID);
	//JPH::Vec3 velocity = engineContext.physicsManager->GetBodyInterface().GetLinearVelocity(modelPhysicsBodyID);
	//model->SetPosition(XMFLOAT3(position.GetX(), position.GetY(), position.GetZ()));
}

void TestScene::Render()
{
	XMMATRIX viewMatrix = CMP316engine::CameraSystem::GetActiveCameraViewMatrix(&registry);
	CMP316engine::RenderSystem::RenderModels(&registry, engineContext.renderer.get(), engineContext.shader.get(), viewMatrix);
}
