#include "ECSScene.h"
#include "ECS/EngineECSSystems.h"
#include "Reflection.h"

namespace CMP316engine {
	ECSScene::ECSScene(CMP316engine::EngineContext& context) : Scene(context)
	{
		sceneRoot = ECS::CreateEntityWithDefaultComponents(&registry);
		systems.push_back(std::make_unique<CMP316engine::RenderSystem>(&registry, &engineContext, engineContext.renderer.get(), engineContext.shader.get()));
		systems.push_back(std::make_unique<CMP316engine::CameraSystem>(&registry, &engineContext));
		systems.push_back(std::make_unique<CMP316engine::TransformSystem>(&registry, &engineContext));
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
		Scene::Shutdown();
		for (auto& system : systems)
		{
			system->Shutdown();
		}
	}

	void ECSScene::HandleInput(float deltaTime)
	{
		for (auto& system : systems)
		{
			system->HandleInput(deltaTime);
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

	void ECSScene::Serialize(std::ofstream& file, BinarySerializeArchive& serializeArchive)
	{
		Scene::Serialize(file, serializeArchive);

		/// Loop through all entities
		for (auto entity : registry.view<entt::entity>()) {
			/// Loop through all storages and serialize any components the entity has in them
			for (auto&& [id, storage] : registry.storage())
			{
				// Entity does not have the component
				if (!storage.contains(entity)) { continue; }
				// Entity has component, reflect the component
				if (auto metaType = entt::resolve(id))
				{
					auto componentInstance = metaType.from_void(storage.value(entity)); // Instance of the reflected object
					auto componentCustom = metaType.custom(); // The custom data of the reflected object
	
					// Check if I have serialize handling for this specific component type first
					if (auto func = metaType.func("Serialize"_hs))
					{
						PropertiesMap map = {};
						if (auto* mp = static_cast<const PropertiesMap*>(componentCustom))
						{
							map = *mp;
						}
						func.invoke(componentInstance, map, serializeArchive);
					}
					else
					{
						// Loop through member variable types (TODO: Make this recursive, currently only one level deep!)
						for (auto [id2, data] : metaType.data())
						{
							auto memberVariableInstance = data.get(componentInstance);
							auto memberVariableCustom = data.custom();
							auto memberVariableType = memberVariableInstance.type(); // Get the reflected type
							if (auto func = memberVariableType.func("Serialize"_hs))
							{
								PropertiesMap map = {};
								if (auto* mp = static_cast<const PropertiesMap*>(memberVariableCustom))
								{
									map = *mp;
								}
								func.invoke(memberVariableInstance, map, serializeArchive);
							}
						}
					}
				}
			}
		}
	}
	void ECSScene::Deserialize(std::ifstream& file, BinaryDeserializeArchive& deserializeArchive)
	{
		Scene::Deserialize(file, deserializeArchive);

		entt::registry newRegistry;

		/// TODO

		registry = std::move(newRegistry);
	}
}