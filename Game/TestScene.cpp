#include "TestScene.h"
#include "ECS/GameObject.h"
#include "ECS/EngineECSSystems.h"
#include "ECS/Components.h" // Engine Components
#include "PlayerSystem.h"
#include "Components.h" // Game Components

TestScene::TestScene(CMP316engine::EngineContext& context) : Scene(context)
{
	systems.push_back(std::make_unique<CMP316engine::RenderSystem>(&registry, engineContext.renderer.get(), engineContext.shader.get()));
	systems.push_back(std::make_unique<CMP316engine::CameraSystem>(&registry));
	systems.push_back(std::make_unique<CMP316engine::PhysicsSystem>(&registry, engineContext.physicsManager.get()));
	systems.push_back(std::make_unique<PlayerSystem>(&registry, engineContext.inputManager.get()));
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
		//MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		std::cout << "\nCould not initialize the shader object.";
		return false;
	}

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
	sceneTree->AddComponent<PlayerComponent>(); // Make it controllable!! Once hierarchy is setup, should move this from the scene root
	sceneTree->AddComponent<CMP316engine::RigidBodyComponent>();
	sceneTree->AddComponent<CMP316engine::MovementComponent>();

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
	for (auto& system : systems)
	{
		system->HandleInput();
	}
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
		system->Update(deltaTime);
	}
}

void TestScene::Render()
{
	XMMATRIX viewMatrix = CMP316engine::CameraSystem::GetActiveCameraViewMatrix(&registry);
	CMP316engine::RenderSystem::RenderModels(&registry, engineContext.renderer.get(), engineContext.shader.get(), viewMatrix);
}
