#include "ECSScene.h"
#include "ECS/EngineECSSystems.h"

namespace CMP316engine {
	ECSScene::ECSScene(CMP316engine::EngineContext& context) : Scene(context)
	{
		sceneRoot = ECS::CreateEntityWithDefaultComponents(&registry);
		systems.push_back(std::make_unique<CMP316engine::RenderSystem>(&registry, engineContext.renderer.get(), engineContext.shader.get()));
		systems.push_back(std::make_unique<CMP316engine::CameraSystem>(&registry));
	}

	bool ECSScene::Initialize()
	{
		for (auto& system : systems)
		{
			system->Initialize();
		}

		auto& hierarchyComp = registry.get<HierarchyComponent>(sceneRoot);
		hierarchyComp.name = "SCENE ROOT";

		return true;
	}

	void ECSScene::Shutdown()
	{
		for (auto& system : systems)
		{
			system->Shutdown();
		}
	}

	void ECSScene::HandleInput()
	{
		for (auto& system : systems)
		{
			system->HandleInput();
		}
	}

	void ECSScene::HandleImGui()
	{
		for (auto& system : systems)
		{
			system->HandleImGui();
		}
	}

	void ECSScene::Update(float deltaTime)
	{
		for (auto& system : systems)
		{
			system->Update(deltaTime);
		}
	}

	void ECSScene::Render()
	{
		XMMATRIX viewMatrix = CMP316engine::CameraSystem::GetActiveCameraViewMatrix(&registry);
		CMP316engine::RenderSystem::RenderModels(&registry, engineContext.renderer.get(), engineContext.shader.get(), viewMatrix);
	}
}