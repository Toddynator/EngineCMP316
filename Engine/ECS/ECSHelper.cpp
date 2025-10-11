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
			auto* childHierarchyComponent = &registry->get<HierarchyComponent>(prevLastChildEntity);
			while (childHierarchyComponent->nextNeighbour != entt::null)
			{
				prevLastChildEntity = childHierarchyComponent->nextNeighbour;
				childHierarchyComponent = &registry->get<HierarchyComponent>(childHierarchyComponent->nextNeighbour);
			}
			childHierarchyComponent->nextNeighbour = entityToAdd;
			newChildHierarchyComponent.prevNeighbour = prevLastChildEntity;
		}

		return entityToAdd;
	}

	void RemoveChild(entt::registry* registry, entt::entity entityToRemove)
	{
		/*
		NOTE: Function assumes that parent is always valid, this is because the scene root should never be deleted.
		The scene should ensure this!
		*/

		auto& hierarchyComponent = registry->get<HierarchyComponent>(entityToRemove);
		auto childEntity = hierarchyComponent.firstChild;
		while (childEntity != entt::null) 
		{
			entt::entity nextChild = registry->get<HierarchyComponent>(childEntity).nextNeighbour;
			RemoveChildWithoutUpdatingHierarchy(registry, childEntity);
			childEntity = nextChild;
		}
		// Ensure parent of deleted node no longer points to invalid data.
		if (hierarchyComponent.prevNeighbour == entt::null)
		{
			// Entity was the first child of the parent node, need to check if there was another after the entity.
			auto& parentHierarchyComponent = registry->get<HierarchyComponent>(hierarchyComponent.parent);

			if (hierarchyComponent.nextNeighbour != entt::null)
			{
				// Parent has more children, the neighbour of removed entity is now the first child
				auto& nextNeighbourHierarchyComponent = registry->get<HierarchyComponent>(hierarchyComponent.nextNeighbour);
				nextNeighbourHierarchyComponent.prevNeighbour = entt::null;
				parentHierarchyComponent.firstChild = hierarchyComponent.nextNeighbour;
			}
			else
			{
				// Parent has no other children
				parentHierarchyComponent.firstChild = entt::null;
			}
		}
		else // Ensure the previous neighbour no longer points to invalid data
		{
			auto& prevNeighbourHierarchyComponent = registry->get<HierarchyComponent>(hierarchyComponent.prevNeighbour);
			
			// Check if there is a child after the removed entity, if so, join previous neighbour to next neighbour.
			if (hierarchyComponent.nextNeighbour != entt::null)
			{
				auto& nextNeighbourHierarchyComponent = registry->get<HierarchyComponent>(hierarchyComponent.nextNeighbour);
				nextNeighbourHierarchyComponent.prevNeighbour = hierarchyComponent.prevNeighbour;
				prevNeighbourHierarchyComponent.nextNeighbour = hierarchyComponent.nextNeighbour;
			}
			else
			{
				// No child after removed entity.
				prevNeighbourHierarchyComponent.nextNeighbour = entt::null;
			}
		}
		DestroyEntity(registry, entityToRemove);
	}

	void RemoveChildWithoutUpdatingHierarchy(entt::registry* registry, entt::entity entityToRemove)
	{
		auto& hierarchyComponent = registry->get<HierarchyComponent>(entityToRemove);
		auto childEntity = hierarchyComponent.firstChild;
		while (childEntity != entt::null)
		{
			entt::entity nextChild = registry->get<HierarchyComponent>(childEntity).nextNeighbour;
			RemoveChildWithoutUpdatingHierarchy(registry, childEntity);
			childEntity = nextChild;
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