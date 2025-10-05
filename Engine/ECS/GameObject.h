/*
CONCRETE CLASS
The Fundamental Object that the sceneTree of an application will use.
A scene should have a single GameObject as the root.

GameObjects can contain multiple children (GameObjects).

ENTT:
Systems will use entt to handle the lifecycle of components.
GameObjects are purely for handling a scene hierarchy, which is useful in something like a Level Editor.
*/

#pragma once
#include <Memory>
#include <vector>
#include <entt.hpp>

namespace CMP316engine {
	class GameObject
	{
	private:
		entt::registry* registry = nullptr;
		std::vector<std::unique_ptr<GameObject>> children;
		std::string name = "Undefined"; // How it is named in the SceneTree, purely for organization purposes.
		entt::entity entityHandle = entt::null;

	public:
		GameObject(entt::registry* sceneRegistry);
		GameObject() = delete;
		~GameObject();
		// MUST IMPLEMENT COPY CONSTRUCTOR AS UNIQUE_POINTERS COPY CONSTRUCTOR IS DELETED
		//GameObject(const GameObject& gameObject); // Copy Constructor
		GameObject(GameObject&& gameObject) = default; // Move Constructor
		//std::unique_ptr<GameObject> clone() const { return std::unique_ptr<GameObject>(new GameObject(*this)); } // Creates a copy

		///////////
		/// ECS ///

		entt::entity AddChild();
		void CreateEntity();
		void DestroyEntity();
		template<typename Component, typename ... Parameters> Component& AddComponent(Parameters&&... parameters);

		/////////////////////////
		/// GETTERS & SETTERS ///

		entt::entity GetEntityHandle() { return entityHandle; }
		std::string GetName() { return name; }
	};
	
	//////////////////////////
	/// TEMPLATE FUNCTIONS ///

	/*
	@brief Adds a component to the GameObject (Which in reality is actually in the registry handled by entt), has handling for any parameters the Component may take.
	@return reference to the component that was just added for immediate modifications if required.
	*/
	template<typename Component, typename ... Parameters >
	Component& GameObject::AddComponent(Parameters&&... parameters)
	{
		if (registry->any_of<Component>(entityHandle)) {
			//assert("Tried to add a component that already exists, you should only have one component per entity!");
			std::cout << "\nTried to add a component that already exists, you should only have one component per entity!";
			Component& component = registry->get<Component>(entityHandle);
			return component;
		}
		Component& component = registry->emplace<Component>(entityHandle, std::forward<Parameters>(parameters)...);
		return component;
	}
}
