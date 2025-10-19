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

		// Serialize entity storage
		auto& entityStorage = registry.storage<entt::entity>();
		archive(entityStorage.size());
		archive(entityStorage.free_list());
		for (auto first = entityStorage.data(), last = first + entityStorage.size(); first != last; ++first) {
			archive(*first);
		}

		// Serialize the total number of storages so that deserialize knows how many to loop through
		std::size_t storageCount = 0;
		for (auto&& [id, storage] : registry.storage()) {
			++storageCount;
		}
		archive(storageCount); //Subtract 1 as entity storage handled outside main loop

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
					std::cout << "\nSerialized Component Name: " << metaType.info().name(); /// DEBUG

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

		/// DESERIALIZE ENTITY STORAGE FIRST

		//std::unordered_map<entt::entity, entt::entity> entityRemap; // OLD to NEW entity
		//entityRemap[entt::null] = entt::null;
		{
			size_t sizeEntityStorage;
			archive(sizeEntityStorage);
			size_t sizeEntityFreeList;
			archive(sizeEntityFreeList);

			// Create storage to store entities in
			auto& entityStorage = newRegistry.storage<entt::entity>();
			entityStorage.reserve(sizeEntityStorage);

			for (int i = 0; i < sizeEntityStorage; i++)
			{
				entt::entity entity;
				archive(entity);
				entityStorage.emplace(entity);

				//entt::entity newEntity = newRegistry.create();
				//entityRemap[entity] = newEntity;
			}
			entityStorage.free_list(sizeEntityFreeList);
		}
		
		/// DESERIALIZE COMPONENTS
		
		size_t totalNumStorages;
		archive(totalNumStorages);

		// Deserialize Components
		for (int i = 0; i < totalNumStorages; i++)
		{
			entt::id_type storageID;
			archive(storageID);
			size_t sizeOfStorage;
			archive(sizeOfStorage);

			for (int entityNum = 0; entityNum < sizeOfStorage; entityNum++)
			{
				entt::entity entity;
				archive(entity);

				// Try to resolve the component in order to deserialize it.
				if (auto metaType = entt::resolve(storageID))
				{
					std::cout << "\nDeserialized Component Name: " << metaType.info().name(); /// DEBUG

					auto addFunc = metaType.func("AddComponent"_hs);
					if (!addFunc)
					{
						std::cout << "\nComponent hasn't got an 'AddComponent' function defined in the reflection system!";
					}
					auto componentInstance = addFunc.invoke({}, entt::forward_as_meta(newRegistry), entity);
					if (!componentInstance)
					{
						std::cout << "\nAddComponent Invoke call did not match reflected function signature";
					}
		
					//auto componentInstance = metaType.construct(); // Create a new instance of the reflected object
					auto componentCustom = metaType.custom(); // The custom data of the reflected object

					// Check if I have serialize handling for this specific component type first
					if (auto func = metaType.func("Deserialize"_hs))
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

							if (auto func = memberVariableType.func("Deserialize"_hs))
							{
								PropertiesMap map = {};
								if (auto* mp = static_cast<const PropertiesMap*>(memberVariableCustom))
								{
									map = *mp;
								}
								func.invoke(memberVariableInstance, map, archive);
							}

							// Special Handling for entt::entity ~ Remap to new handles
							/*if (memberVariableType.info() == entt::type_id<entt::entity>())
							{
								entt::entity oldEntityHandle = memberVariableInstance.cast<entt::entity>();	
								if (oldEntityHandle != entt::null) {
									entt::entity newEntity = entityRemap[oldEntityHandle];
									data.set(memberVariableInstance, newEntity);
								}
							}*/
						}
					}		
				}
			}
		}

		///// TEMP
		/*auto hierarchyEntities = newRegistry.view<HierarchyComponent>();
		for (auto& entity : hierarchyEntities)
		{
			auto& hierarchyComponent = newRegistry.get<HierarchyComponent>(entity);

			hierarchyComponent.parent = entityRemap[hierarchyComponent.parent];
			hierarchyComponent.firstChild = entityRemap[hierarchyComponent.firstChild];
			hierarchyComponent.prevNeighbour = entityRemap[hierarchyComponent.prevNeighbour];
			hierarchyComponent.nextNeighbour = entityRemap[hierarchyComponent.nextNeighbour];

			if (hierarchyComponent.parent == entt::null) {
				sceneRoot = entity;		
			}
		}*/
		///// TEMP

		registry = std::move(newRegistry);
	}

	void ECSScene::Load()
	{
		Scene::Load();

		/// prep mesh components so that they get reinitialized.
		// NOTE TO SELF:
		// This can be replaced if I mark initialize bools as not to be serialized, so that they evaluate to their default state!
		auto meshEntities = registry.view<MeshComponent>();
		for (auto& entity : meshEntities) 
		{
			auto& meshComponent = registry.get<MeshComponent>(entity);

			meshComponent.meshNeedsCalculated = true;
		}
	}
}

