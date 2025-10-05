#pragma once
#include "entt.hpp"
#include <directxmath.h>

namespace CMP316engine 
{
	/*
	The component that all entities should use to define their position in the scene hierarchy.
	If I want propagating changes from parent to child, then this must be added to an entity.
	Important for a scene graph if I make a Level Editor.
	*/
	struct HierarchyComponent
	{
		entt::entity parent = entt::null;
	};

	struct TransformComponent
	{
		DirectX::XMFLOAT3 position;
	};
}

