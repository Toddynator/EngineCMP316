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

	void ECSScene::Serialize(std::ofstream& file, BinarySerializeArchive& archive)
	{
		Scene::Serialize(file, archive);

		// Serialize the total number of storages FIRST so that deserialize knows how many to loop through
		std::size_t storageCount = 0;
		for (auto&& [id, storage] : registry.storage()) {
			++storageCount;
		}
		archive(storageCount-1); //Subtract 1 as entity storage handled outside main loop

		// Serialize entity storage
		auto& entityStorage = registry.storage<entt::entity>();
		archive(entityStorage.size());
		archive(entityStorage.free_list());
		for (auto first = entityStorage.data(), last = first + entityStorage.size(); first != last; ++first) {
			archive(*first);
		}

		// Loop through all storages
		for (auto&& [id, storage] : registry.storage())
		{
			// Skip the entity storage
			if (id == entt::type_hash<entt::entity>::value()) continue;

			// Save the size of the storage
			archive(id);
			archive(storage.size());

			for (auto entity : storage)
			{
				archive(entity);

				// Try to resolve the component in order to serialize it.
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
						func.invoke(componentInstance, map, archive);
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
								func.invoke(memberVariableInstance, map, archive);
							}
						}
					}
				}
			}
		}
	}
	void ECSScene::Deserialize(std::ifstream& file, BinaryDeserializeArchive& archive)
	{
		Scene::Deserialize(file, archive);

		entt::registry newRegistry;

		/// TODO
		// Notably, registry.emplace seems to only create a new entity if one doesn't already exist, I believe that is was entt::snapshot_loader does.

		//registry = std::move(newRegistry);
	}
}