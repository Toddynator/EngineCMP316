#include "TestScene.h"
#include "ECS/GameObject.h"
#include "ECS/EngineECSSystems.h"
#include "ECS/Components.h" // Engine Components
#include "PlayerSystem.h"
#include "Components.h" // Game Components

TestScene::TestScene(CMP316engine::EngineContext& context) : ECSScene(context)
{
	systems.push_back(std::make_unique<CMP316engine::PhysicsSystem>(&registry, engineContext.physicsManager.get()));
	systems.push_back(std::make_unique<PlayerSystem>(&registry, engineContext.inputManager.get()));
}

bool TestScene::Initialize()
{
	CMP316engine::ECSScene::Initialize();

	///// SOUND

	// I yearn for the music
	int audioHandle = engineContext.audioManager->Play("MyJam");
	engineContext.audioManager->SetAudioLoop(audioHandle, true);

	///// SCENE OBJECTS

	auto& modelComponent = CMP316engine::GameObject::AddComponent<CMP316engine::ModelComponent>(&registry, sceneRoot->GetEntityHandle());
	modelComponent.filepath = "data/Models/Dug/Dug.obj";
	auto& meshComponent = CMP316engine::GameObject::AddComponent<CMP316engine::MeshComponent>(&registry, sceneRoot->GetEntityHandle());
	CMP316engine::GameObject::AddComponent<PlayerComponent>(&registry, sceneRoot->GetEntityHandle()); // Make it controllable!! Once hierarchy is setup, should move this from the scene root
	CMP316engine::GameObject::AddComponent<CMP316engine::RigidBodyComponent>(&registry, sceneRoot->GetEntityHandle());
	CMP316engine::GameObject::AddComponent<CMP316engine::MovementComponent>(&registry, sceneRoot->GetEntityHandle());

	/*auto cameraObject = sceneRoot->AddChild();
	cameraObject->AddComponent<CMP316engine::CameraComponent>();
	auto transforms = cameraObject->GetComponent<CMP316engine::TransformComponent>();
	if (transforms) { 
		transforms->position = DirectX::XMFLOAT3(0.f, 0.f, -5.0f); 
	}*/

	return true;
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