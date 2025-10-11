#include "ECSHelper.h"
#include "Components.h"

namespace CMP316engine::ECS
{
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity, entt::entity entityToAdd)
	{
		return entt::null; //// TODO
	}

	entt::entity CreateEntityWithDefaultComponents(entt::registry* registry)
	{
		entt::entity newEntity = registry->create();
		auto& hierarchyComponent = AddComponent<HierarchyComponent>(registry, newEntity);
		auto& transformComponent = AddComponent<TransformComponent>(registry, newEntity);
		return newEntity;
	}

	void DestroyEntity(entt::registry* registry, entt::entity entity)
	{
		registry->destroy(entity);
	}
}