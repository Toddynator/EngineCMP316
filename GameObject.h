/*
The Fundamental Object that the sceneTree of an application will use.
A scene should have a single GameObject as the root.

GameObjects can contain multiple children (GameObjects) and Components.
Components define the functionality of a gameObject.

GameObjects are capable of recursive updates.

Core Component Ideas:
- TransformComponent: It stores the position, rotation, etc.
- RigidBodyComponent: It is handled by the PhysicsManager, it will push updates to a transformComponent if one is available.
- ModelComponent: It is handled by the Renderer, it reads from TransformComponent if there is one.
Other Component Ideas:
- CameraComponent
- LightComponent
- ScriptComponent ~ Should it be a component or maybe be inherent to GameObjects?
*/

#pragma once
#include "Component.h"
#include <Memory>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject() = default;
	// MUST IMPLEMENT COPY CONSTRUCTOR AS UNIQUE_POINTERS COPY CONSTRUCTOR IS DELETED
	GameObject(const GameObject& gameObject); // Copy Constructor
	GameObject(GameObject&& gameObject) = default; // Move Constructor

	std::unique_ptr<GameObject> clone() const { return std::unique_ptr<GameObject>(new GameObject(*this)); } // Creates a copy

	void Update();

	std::string GetName() { return name; }

private:
	GameObject* parent = nullptr;
	std::vector<std::unique_ptr<GameObject>> children;
	std::vector<std::unique_ptr<Component>> components;

	std::string name;
};

