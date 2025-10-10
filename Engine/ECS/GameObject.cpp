#include "pch.h"
#include "GameObject.h"
#include "Components.h"

namespace CMP316engine {
	GameObject::GameObject(entt::registry* sceneRegistry)
	{
		entityHandle = CreateEntity(sceneRegistry);
	}

	GameObject::GameObject(entt::entity existingEntity)
	{
		entityHandle = existingEntity;
	}

	entt::entity GameObject::AddChild(entt::registry* registry, entt::entity parentEntity, entt::entity entityToAdd)
	{
		return entt::null; //// TODO
	}

	entt::entity GameObject::CreateEntity(entt::registry* registry)
	{
		entt::entity newEntity = registry->create();
		auto& hierarchyComponent = GameObject::AddComponent<HierarchyComponent>(registry, newEntity);
		auto& transformComponent = GameObject::AddComponent<TransformComponent>(registry, newEntity);
		return newEntity;
	}

	void GameObject::DestroyEntity(entt::registry* registry, entt::entity entity)
	{
		registry->destroy(entity);
	}
}