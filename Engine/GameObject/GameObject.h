/*
CONCRETE CLASS
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

Plans:
- I'm hoping that I'll eventually be able to serialize an entire sceneTree, but additionally, be able to target a specific branch of the tree / GameObject and serialize it as its own file.
This would be fantastic for a level editor in which I could for example design an enemy then save it and just load in copies from file in new scenes.

- May be smart to have a hash-map somewhere which stores every object by its ID. Whenever a gameObject is added to the scene then a pointer to it will be inserted
into the map with the key as its unique ID. To assist in this, I may want to pass a pointer to the hash-map to each gameObject (do this whenever an object is added) so that quick lookup is maintained.
This will be VITAL for fast lookup of other gameObjects in a large Scene Tree.
*/

#pragma once
#include "Component.h"
#include <Memory>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject() { parent = nullptr; }
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

	std::string name; // How it is named in the SceneTree, purely for organization purposes.
	int uniqueID; // Utilized for finding specific GameObjects in the SceneTree, which may be utilized by scripts, etc.
};

