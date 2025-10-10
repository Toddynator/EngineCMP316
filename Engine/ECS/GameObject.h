/*
CONCRETE CLASS
Purpose of this class is to store a singular important entity handle (e.g. scene root OR 
a copy of an entity, e.g. for a clipboard system), and to encapsulate
adding essential components to entities that get added to the scene.

NOTE: For temporary entities like bullets, they don't necessarily need to be part of the SceneTree / hierarchy, and can
be created directly through the registry.
An ECS scene MUST HAVE a single GameObject as the root.
After that, any entities that should be added to the scene tree should
be created using GameObject functions in order to encapsulate creating the entity
and adding the common components.


ENTT:
Systems will use entt to handle the lifecycle of components.
GameObjects are purely for adding essential components 
that EVERY object in the scene should have.
This is mainly useful for a level editor, which needs some way to clearly interface with
the scene and visualize the entities.
*/

#pragma once
#include <Memory>
#include <vector>
#include <entt.hpp>

namespace CMP316engine {
	class GameObject
	{
	private:
		entt::entity entityHandle = entt::null;

	public:
		GameObject(entt::registry* sceneRegistry); // FOR CREATING A NEW ENTITY WITH BASIC FUNCTIONALITY
		GameObject(entt::entity existingEntity); // FOR CREATING AN INTERFACE FOR OR AN EXISTING ENTITY
		GameObject() = delete;

		///////////////////
		/// ECS HELPERS ///

		static entt::entity AddChild(entt::registry* registry, entt::entity parentEntity, entt::entity entityToAdd);
		static entt::entity CreateEntity(entt::registry* registry);
		static void DestroyEntity(entt::registry* registry, entt::entity entity);

		template<typename Component, typename ... Parameters> 
		static Component& AddComponent(entt::registry* registry, entt::entity entity, Parameters&&... parameters);
		template<typename Component> 
		static Component* GetComponent(entt::registry* registry, entt::entity entity);

		/////////////////////////
		/// GETTERS & SETTERS ///

		entt::entity GetEntityHandle() { return entityHandle; }
	};
	
	//////////////////////////
	/// TEMPLATE FUNCTIONS ///

	/*
	@brief Adds a component to the GameObject (Which in reality is actually in the registry handled by entt), has handling for any parameters the Component may take.
	@return reference to the component that was just added for immediate modifications if required.
	*/
	template<typename Component, typename ... Parameters >
	Component& GameObject::AddComponent(entt::registry* registry, entt::entity entity, Parameters&&... parameters)
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
	Component* GameObject::GetComponent(entt::registry* registry, entt::entity entity)
	{
		if (!registry->any_of<Component>(entity)) { std::cout << "\nEntity does not have that component!"; return nullptr; }
		return &registry->get<Component>(entity);
	}
}
