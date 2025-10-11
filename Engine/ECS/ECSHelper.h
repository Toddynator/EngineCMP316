#pragma once
#include <Memory>
#include <vector>
#include <entt.hpp>


namespace CMP316engine::ECS
{
	/*
	@brief handles adding a child and correctly setting the hierarchy component of the parent and children.
	*/
	entt::entity AddChild(entt::registry* registry, entt::entity parentEntity);
	/*
	@brief handles recursively removing a child and all its children, will adjust the hierarchyComponents accordingly.
	Compared to OOP style scene graphs, you can't delete the parent and expect all the children to be automatically
	destroyed. With ECS, you must delete from the ends of the sceneGraph up.
	NOTE: May want to defer deletion by adding a markedForDeletion bool to hierarchyComponent and have an ECSCoreSystem handle cleanup of
	marked entities. I may need to have the ECSCoreSystem at a slightly different update step to other systems.
	*/
	void RemoveChild(entt::registry* registry, entt::entity entityToRemove);
	entt::entity CreateEntityWithDefaultComponents(entt::registry* registry);
	void DestroyEntity(entt::registry* registry, entt::entity entity);
	/*
	@brief calls for every child direclty under the parent entity. For recursive operations, the function that is called should call the CallForAllChildren() function again.
	@param functionToCall can be created by defining a lambda directly in the parameter list, MUST have the registy pointer and the root entity passed in, in the lambda capture list you can then pass in any additional
	variables that need modified.
	*/
	void CallForAllChildren(entt::registry* registry, entt::entity parentEntity, const std::function<void(entt::registry*, entt::entity)>& functionToCall);


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