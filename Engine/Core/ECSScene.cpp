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
				
				// Don't serialize the level editors camera
				bool skipEntity = false;
				if (registry.any_of<LevelEditorCameraComponent>(entity))
				{
					skipEntity = true;
					archive(skipEntity);
					continue;
				}
				archive(skipEntity);

				// Try to resolve the component in order to serialize it.
				if (auto type = entt::resolve(id))
				{
					std::cout << "\nSerialized Component Name: " << type.info().name(); /// DEBUG
					auto componentInstance = type.from_void(storage.value(entity)); // Instance of the reflected object
					recursiveReflectionSerialize(componentInstance, type.custom(), archive);
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

				bool skipEntity = false;
				archive(skipEntity);
				if (skipEntity) { continue; }

				// Try to resolve the component in order to deserialize it.
				if (auto type = entt::resolve(storageID))
				{
					std::cout << "\nDeserialized Component Name: " << type.info().name(); /// DEBUG

					auto addFunc = type.func("AddComponent"_hs);
					if (!addFunc)
					{
						std::cout << "\nComponent hasn't got an 'AddComponent' function defined in the reflection system!";
					}
					entt::meta_any componentInstance = addFunc.invoke({}, entt::forward_as_meta(newRegistry), entity);
					if (!componentInstance)
					{
						std::cout << "\nAddComponent Invoke call did not match reflected function signature";
					}	
					recursiveReflectionDeserialize(componentInstance, type.custom(), archive);
				}
			}
		}

		///// TEMP
		///// NOTE: KEEP INCASE ENTITIES ON DESERIALIZE END UP BEING DIFFERENT TO THEIR OLD HANDLES, THIS MIGHT SAVE ME A LOT OF PAIN
		///// I ALSO HAVE THE ALTERNATIVE (CLEANER) METHOD IN THE REFLECTION LOOP WHICH MIGHT WORK.
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
	}
	void ECSScene::recursiveReflectionSerialize(entt::meta_any& instance, entt::meta_custom customData, BinarySerializeArchive& archive)
	{
		/*
		Type / meta_type represents what object holds the data. If a meta_factory object was created for the data, then on recursion it will be
		resolved into its mmember variables as well.
		Instance / meta_any represents one instance of that data in the reflection system.
		The engine will provide reflected serialize functions for specific types of data, e.g. int, floats, etc. If there is a serialize function reflected for the
		data's type, then it will be serialized unless explicitly set to not be serialized by the NOT_SERIALIZED trait. You won't need to set that trait if the
		member variables have not been reflected at all, in which case the component will likely use default values on deserialization.
		*/
		
		entt::meta_type type = instance.type();

		auto func = type.func("Serialize"_hs);
		// Check if I have serialize handling for this specific component type first
		if (!(type.traits<Traits>() & Traits::NOT_SERIALIZED) && func)
		{
			PropertiesMap propertiesMap = {};
			if (auto* map = static_cast<const PropertiesMap*>(customData))
			{
				propertiesMap = *map;
			}
			func.invoke(instance, propertiesMap, archive);
		}
		else
		{
			// Loop through member variable types (TODO: Make this recursive, currently only one level deep!)
			for (auto [id, data] : type.data())
			{
				if (!(data.traits<Traits>() & Traits::NOT_SERIALIZED))
				{
					auto dataInstance = data.get(instance);
					recursiveReflectionSerialize(dataInstance, data.custom(), archive);
				}
			}
		}
	}
	void ECSScene::recursiveReflectionDeserialize(entt::meta_any& instance, entt::meta_custom customData, BinaryDeserializeArchive& archive)
	{
		entt::meta_type type = instance.type();

		// Check if I have serialize handling for this specific component type first
		auto func = type.func("Deserialize"_hs);
		if (!(type.traits<Traits>() & Traits::NOT_SERIALIZED) && func)
		{
			PropertiesMap propertiesMap = {};
			if (auto* map = static_cast<const PropertiesMap*>(customData))
			{
				propertiesMap = *map;
			}
			func.invoke(instance, propertiesMap, archive);
		}
		else
		{
			// Loop through member variable types (TODO: Make this recursive, currently only one level deep!)
			for (auto [id, data] : type.data())
			{
				if (!(data.traits<Traits>() & Traits::NOT_SERIALIZED))
				{
					auto dataInstance = data.get(instance);
					recursiveReflectionDeserialize(dataInstance, data.custom(), archive);
				}
			}
		}

		// Special Handling for entt::entity ~ Remap to new handles
		/*if (type.info() == entt::type_id<entt::entity>())
		{
			entt::entity oldEntityHandle = instance.cast<entt::entity>();
			if (oldEntityHandle != entt::null) {
				entt::entity newEntity = entityRemap[oldEntityHandle];
				instance.set(type.id(), newEntity);
			}
		}*/
	}
}

