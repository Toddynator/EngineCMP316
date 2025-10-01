#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(const GameObject& originalGameObject) : name(originalGameObject.name)
{
	for (const auto& child : originalGameObject.children) {
		// Since gameObject clones its children, this means it will recursively do that for ALL children whenever the child is cloned.
		auto childCopy = child->clone();
		children.push_back(std::move(childCopy));
	}

	for (const auto& component : originalGameObject.components) {
		auto componentCopy = component->clone();
		components.push_back(std::move(componentCopy));
	}
}

void GameObject::Update()
{

}
