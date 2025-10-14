#pragma once
#include "entt.hpp"
#include <unordered_map>

/*

*/
using namespace entt::literals;

namespace CMP316engine
{
	enum Traits : uint16_t
	{
		EDITOR = 1 << 0, // Reflected member variables that should have an editor control
		SERIALIZE = 1 << 1, // Reflected member variables that should be serialized
		COMPONENT = 1 << 2 // Add to components so that components can be identified from other reflected data, can be useful for Component specific UI's
	};
	using PropertiesMap = std::unordered_map<entt::id_type, entt::meta_any>;
}