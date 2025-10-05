#include "pch.h"
#include "GameObject.h"
#include "Components.h"

CMP316engine::GameObject::GameObject(entt::registry* sceneRegistry) : registry(sceneRegistry)
{
	CreateEntity();
	auto& hierarchyComponent = AddComponent<HierarchyComponent>();
	hierarchyComponent.parent = entityHandle;
	AddComponent<TransformComponent>();
}

CMP316engine::GameObject::~GameObject()
{
	DestroyEntity();
}

//CMP316engine::GameObject::GameObject(const GameObject& originalGameObject) : name(originalGameObject.name)
//{
//	for (const auto& child : originalGameObject.children) {
//		// Since gameObject clones its children, this means it will recursively do that for ALL children whenever the child is cloned.
//		auto childCopy = child->clone();
//		children.push_back(std::move(childCopy));
//	}
//}

entt::entity CMP316engine::GameObject::AddChild()
{
	std::unique_ptr<GameObject> child = std::make_unique<GameObject>(registry);
	children.push_back(std::move(child));
	return child->GetEntityHandle();
}

void CMP316engine::GameObject::CreateEntity()
{
	entityHandle = registry->create();
}

void CMP316engine::GameObject::DestroyEntity()
{
	registry->destroy(entityHandle);
}
