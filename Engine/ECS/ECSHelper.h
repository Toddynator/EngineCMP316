#pragma once
#include <Memory>
#include <vector>
#include <entt.hpp>


namespace CMP316engine::ECS
{
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity, entt::entity entityToAdd);
	entt::entity CreateEntityWithDefaultComponents(entt::registry* registry);
	void DestroyEntity(entt::registry* registry, entt::entity entity);

	/////////////////
	/// TEMPLATES ///

	/*
	@brief Adds a component to the GameObject (Which in reality is actually in the registry handled by entt), has handling for any parameters the Component may take.
	@return reference to the component that was just added for immediate modifications if required.
	*/
	template<typename Component, typename ... Parameters >
	Component& AddComponent(entt::registry* registry, entt::entity entity, Parameters&&... parameters)
	{
		if (registry->any_of<Component>(entity)) {
			//assert("Tried to add a component that already exists, you should only have one component per entity!");
			std::cout << "\nTried to add a component that already exists, you should only have one component per entity!";
			Component& component = registry->get<Component>(entity);
			return component;
		}
		Component& component = registry->emplace<Component>(entity, std::forward<Parameters>(parameters)...);
		return component;
	}
	template<typename Component>
	Component* GetComponent(entt::registry* registry, entt::entity entity)
	{
		if (!registry->any_of<Component>(entity)) { std::cout << "\nEntity does not have that component!"; return nullptr; }
		return &registry->get<Component>(entity);
	}
}