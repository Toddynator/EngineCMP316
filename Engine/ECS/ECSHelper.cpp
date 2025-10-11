#include "ECSHelper.h"
#include "Components.h"

namespace CMP316engine::ECS
{
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity)
	{
		entt::entity entityToAdd = CreateEntityWithDefaultComponents(registry);

		auto& parentHierarchyComponent = registry->get<HierarchyComponent>(parentEntity);
		auto& newChildHierarchyComponent = registry->get<HierarchyComponent>(entityToAdd);

		newChildHierarchyComponent.parent = parentEntity;

		/// Find the last child

		if (parentHierarchyComponent.firstChild == entt::null)
		{
			parentHierarchyComponent.firstChild = entityToAdd;
		}
		else
		{
			entt::entity prevLastChildEntity = parentHierarchyComponent.firstChild;
			auto& childHierarchyComponent = registry->get<HierarchyComponent>(prevLastChildEntity);
			while (childHierarchyComponent.nextNeighbour != entt::null)
			{
				prevLastChildEntity = childHierarchyComponent.nextNeighbour;
				childHierarchyComponent = registry->get<HierarchyComponent>(childHierarchyComponent.nextNeighbour);
			}
			childHierarchyComponent.nextNeighbour = entityToAdd;
			newChildHierarchyComponent.prevNeighbour = prevLastChildEntity;
		}

		return entityToAdd;
	}

	void RemoveChild(entt::registry* registry, entt::entity entityToRemove)
	{
		auto& hierarchyComponent = registry->get<HierarchyComponent>(entityToRemove);
		auto& childEntity = hierarchyComponent.firstChild;
		while (childEntity != entt::null)
		{
			auto& childHierarchyComponent = registry->get<HierarchyComponent>(childEntity);
			RemoveChild(registry, childEntity);
			childEntity = childHierarchyComponent.nextNeighbour;
		}
		DestroyEntity(registry, entityToRemove);
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

	void CallForAllChildren(entt::registry* registry, entt::entity parentEntity, const std::function<void(entt::registry*,entt::entity)>& functionToCall)
	{
		auto& hierarchyComponent = registry->get<HierarchyComponent>(parentEntity);
		auto childEntity = hierarchyComponent.firstChild;
		while (childEntity != entt::null)
		{
			auto& childHierarchyComponent = registry->get<HierarchyComponent>(childEntity);
			functionToCall(registry, childEntity);
			childEntity = childHierarchyComponent.nextNeighbour;
		}
	}
}