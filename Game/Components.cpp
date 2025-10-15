#include "Components.h"
#include "Core/Reflection.h"

void InitializeReflection()
{
	entt::meta<PlayerComponent>()
		.traits(CMP316engine::Traits::COMPONENT)
		.func<static_cast<bool (entt::registry::*)(const entt::entity) const>(&entt::registry::any_of<PlayerComponent>)>("HasComponent"_hs)
		.func<static_cast<PlayerComponent& (entt::registry::*)(const entt::entity)>(&entt::registry::emplace_or_replace<PlayerComponent>),
		entt::as_ref_t>("AddComponent"_hs)
		.data<&PlayerComponent::playerNumber, entt::as_ref_t>("playerNumber"_hs)
		.custom<CMP316engine::PropertiesMap>(CMP316engine::PropertiesMap{ { "name"_hs, "playerNumber" }, })
		.traits(CMP316engine::Traits::EDITOR)
		;
}
