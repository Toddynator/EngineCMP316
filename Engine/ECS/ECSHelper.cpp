#include "ECSHelper.h"
#include "Components.h"

namespace CMP316engine::ECS
{
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity)
	{
		entt::entity entityToAdd = CreateEntityWithDefaultComponents(registry);
		return AddChild(registry, parentEntity, entityToAdd);
	}
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity, entt::entity entityToAdd)
	{
		auto& parentHierarchyComponent = registry->get<HierarchyComponent>(parentEntity);
		auto& newChildHierarchyComponent = registry->get<HierarchyComponent>(entityToAdd);

		/// Check if the entity had a previous parent and neighbours

		if (newChildHierarchyComponent.parent != entt::null)
		{
			// Had a parent
			auto& previousParentHierarchyComponent = registry->get<HierarchyComponent>(newChildHierarchyComponent.parent);

			if (newChildHierarchyComponent.prevNeighbour == entt::null)
			{
				// Entity is the first child
				if (newChildHierarchyComponent.nextNeighbour != entt::null) {
					// Parents first child is now the neighbour
					registry->get<HierarchyComponent>(newChildHierarchyComponent.nextNeighbour).prevNeighbour = entt::null;
					previousParentHierarchyComponent.firstChild = newChildHierarchyComponent.nextNeighbour;
				}
				// Entity was the first AND only child
				else {
					// Parent no longer has any children
					previousParentHierarchyComponent.firstChild = entt::null;
				}
			}
			else {
				// The previous child now links to the next child (if one exists)
				auto& prevNeighbourHierarchyComponent = registry->get<HierarchyComponent>(newChildHierarchyComponent.prevNeighbour);
				prevNeighbourHierarchyComponent.nextNeighbour = newChildHierarchyComponent.nextNeighbour;

				if (newChildHierarchyComponent.nextNeighbour != entt::null)
				{
					// There was a child after, it now links to the previous neighbour
					auto& nextNeighbourHierarchyComponent = registry->get<HierarchyComponent>(newChildHierarchyComponent.nextNeighbour);
					nextNeighbourHierarchyComponent.prevNeighbour = newChildHierarchyComponent.prevNeighbour;
				}
			}
		}
		// Child now points to the new parent and has different neighbours
		newChildHierarchyComponent.parent = parentEntity;
		newChildHierarchyComponent.prevNeighbour = entt::null;
		newChildHierarchyComponent.nextNeighbour = entt::null;

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

	bool IsDescendant(entt::registry* registry, entt::entity ancestor, entt::entity entityToCheck)
	{
		if (entityToCheck == entt::null) { return false; }

		auto& hierarchyComponent = registry->get<HierarchyComponent>(entityToCheck);
		entt::entity parent = hierarchyComponent.parent;
		while (parent != entt::null)
		{
			if (parent == ancestor) {
				return true;
			}
			parent = registry->get<HierarchyComponent>(parent).parent;
		}
		return false;
	}

	entt::entity CopyEntity(entt::registry* homeRegistry, entt::entity entityToCopy)
	{
		/*
		Copying of all entities that are children of the copied entity, and adjusting all the hierarchy components to the new entity handles.
		*/

		// Copy every component into the new entity
		entt::entity copyEntity = homeRegistry->create();
		for (auto [id, storage] : homeRegistry->storage()) {
			// Checks every component storage, and copies to the new entity
			if (storage.contains(entityToCopy)) {
				storage.push(copyEntity, storage.value(entityToCopy));
			}
		}
		return copyEntity;
	}

	entt::entity CopyEntityBetweenRegistries(entt::registry* homeRegistry, entt::registry* newRegistry, entt::entity entityToCopy)
	{
		/*
		Creates a temporary entity in current registry by copying all the components from every storage.
		Then it inserts the entity into the new reigistry and deletes the entity in the current.

		PROBLEM:
		Don't know how to dynamically create the storages in the new registry without knowing the types of the components.
		One solution is that I maybe just create a dummy entity in the clipboard registry that has ALL the components. Still requires
		manual work though so that every time a new component is added that it must be added to the clipboard registry. Not Ideal.
		ALTERNATIVE: If I add/use reflection then that might solve this issue.
		*/

		// Copy every component into the new entity

		/* /// TEMP
		entt::entity copyEntity = homeRegistry->create();
		for (auto [id, storage] : homeRegistry->storage()) {
			// Checks every component storage, and copies to the new entity
			if (storage.contains(entityToCopy)) {
				storage.push(copyEntity, storage.value(entityToCopy));
			}
		}
		*/ /// TEMP

		entt::entity copyEntity = newRegistry->create();
		for (auto [id, storage] : homeRegistry->storage()) {
			// Checks every component storage, and copies to the new entity
			if (storage.contains(entityToCopy)) {
				auto* destinationStorage = newRegistry->storage(id);
				destinationStorage->push(copyEntity, storage.value(entityToCopy));
			}
		}

		// Assign entity // TODO
		//newRegistry->
		
		// Delete Temp Entity // TODO
		//// TODO

		return copyEntity;
	}
}