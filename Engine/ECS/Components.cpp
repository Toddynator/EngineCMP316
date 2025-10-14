#include "pch.h"
#include "Components.h"
#include "../Utility/ImGuiHelper.h"

namespace CMP316engine
{

	static bool DrawEditorFloat(float& f, const PropertiesMap& properties)
	{
		const char* label = "float";
		float min = 0;
		float max = 1;

		// This is kinda hideous, but it works.
		if (auto it = properties.find("name"_hs); it != properties.end())
		{
			// Note that these casts will return nullptr if you supply the wrong type in the property map.
			label = *it->second.try_cast<const char*>();
		}
		if (auto it = properties.find("min"_hs); it != properties.end())
		{
			min = *it->second.try_cast<float>();
		}
		if (auto it = properties.find("max"_hs); it != properties.end())
		{
			max = *it->second.try_cast<float>();
		}

		//return ImGui::SliderFloat(label, &f, min, max);
		return ImGui::InputFloat(label, &f);
	}
	static bool DrawEditorFloat3(DirectX::XMFLOAT3& f, const PropertiesMap& properties)
	{
		const char* label = "float3";
		float min = 0;
		float max = 1;

		// This is kinda hideous, but it works.
		if (auto it = properties.find("name"_hs); it != properties.end())
		{
			// Note that these casts will return nullptr if you supply the wrong type in the property map.
			label = *it->second.try_cast<const char*>();
		}
		if (auto it = properties.find("min"_hs); it != properties.end())
		{
			min = *it->second.try_cast<float>();
		}
		if (auto it = properties.find("max"_hs); it != properties.end())
		{
			max = *it->second.try_cast<float>();
		}

		//return ImGui::SliderFloat3(label, &f.x, min, max);
		return ImGui::InputFloat3(label, &f.x);
	}

	void CMP316engine::InitializeReflection()
	{
		entt::meta_reset();

		entt::meta<float>().func<&DrawEditorFloat>("DrawEditor"_hs);
		entt::meta<DirectX::XMFLOAT3>().func<&DrawEditorFloat3>("DrawEditor"_hs);
		//entt::meta<int>().func<&DrawEditorFloat>("InputAny"_hs);

		entt::meta<TransformComponent>()
			// Hashed strings are used to give them unique identifiers. A macro could be used...
			// entt::as_ref_t means meta_any objects will not copy the data, which allows our DrawEditor* functions to work as they take a reference and modify it.
			.data<&TransformComponent::position, entt::as_ref_t>("position"_hs)
			// The properties map is used to register the label name of the field in the editor as well as things like the min and max for a slider.
			.custom<PropertiesMap>(PropertiesMap{ { "name"_hs, "position" } })
			.traits(Traits::EDITOR)
			.data<&TransformComponent::rotation, entt::as_ref_t>("rotation"_hs)
			.custom<PropertiesMap>(PropertiesMap{ { "name"_hs, "rotation" } })
			.traits(Traits::EDITOR)
			.data<&TransformComponent::scale, entt::as_ref_t>("scale"_hs)
			.custom<PropertiesMap>(PropertiesMap{ { "name"_hs, "scale" } })
			.traits(Traits::EDITOR)
			.data<&TransformComponent::testFloatInTransform, entt::as_ref_t>("testFloatInTransform"_hs)
			.custom<PropertiesMap>(PropertiesMap{ { "name"_hs, "testFloatInTransform" } })
			.traits(Traits::EDITOR);
	}
}