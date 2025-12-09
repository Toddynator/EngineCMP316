/*
Core Reflection File
Implements all the engine functionality for reflected objects.
This is achieved by creating functions then reflecting them for a particular data type (e.g. float).
Examples of use include Serialization functions & Editor UI functions.
Objects should be reflected elsewhere
for the sake organisation, preferably in their class/file of origin.

This is better than implementing functionality for specific objects as it means 
application developers do not need to edit engine functionality to utilize engine
features for their own components. Once the engine has implemented functionality
for a particular data type, whenever that variable is reflected it then works
for every single engine feature which uses reflection.

Key Features:
- Reflection Traits : Used to indicate particular functionality should be used for a reflected object/variable.
- Serialization Functions : Each data type is reflected with compatible serialization functions in this file, allowing for that data type to be saved and loaded.
- Editor Functions : Each data type is reflected with compatible editor functions which allows for the Level Editor to create controls.
*/

#pragma once
#include "entt.hpp"
#include <unordered_map>
#include <DirectXMath.h>
#include "../Utility/BinarySerializeArchive.h"
#include "../Utility/BinaryDeserializeArchive.h"
#include "../Utility/ImGuiHelper.h"

using namespace entt::literals;

#define REFLECT_COMPONENT(TYPE) \
	entt::meta<TYPE>()	\
	.traits(Traits::COMPONENT) \
	.func<static_cast<bool (entt::registry::*)(const entt::entity) const>(&entt::registry::any_of<TYPE>)>("HasComponent"_hs) \
	.func<static_cast<TYPE& (entt::registry::*)(const entt::entity)>(&entt::registry::emplace_or_replace<TYPE>), \
		entt::as_ref_t>("AddComponent"_hs) 

namespace CMP316engine
{
	enum Traits : uint16_t
	{
		EDITOR = 1 << 0, // Reflected member variables that should have an editor control
		NOT_SERIALIZED = 1 << 1, // Reflected member variables that should not be serialized, and should evaluate to default values on loading (initialize bools for example). (Only needed if you actually reflect the variable)
		COMPONENT = 1 << 2, // Add to components so that components can be identified from other reflected data, can be useful for Component specific UI's
		NOT_DESERIALIZED = 1 << 3, // This is primarily for manual forward/backward compatibility, allowing people to update serialized files to the latest version when modifying components.
		NOT_SERIALIZED_OR_DESERIALIZED = 1 << 4 // Technically you could just se both NOT_DESERIALIZED and NOT_SERIALIZED. This is for convenience.
	};
	using PropertiesMap = std::unordered_map<entt::id_type, entt::meta_any>;

	/*
	Where the reflection for the functions will get initialized
	I found it better to just create a static class instead of throwing the initialisation
	call somewhere else in the program.
	*/
	void InitializeReflectionFunctions();
	class FunctionReflector
	{
	public:
		FunctionReflector()
		{
			InitializeReflectionFunctions();
		}
	};
	inline static FunctionReflector reflectorFunctions;

	/// EDITOR UI

	void GetEditorCustomData(const PropertiesMap& properties, const char*& label, float& min, float& max);
	template<typename Type>
	bool DrawEditor(Type& data, const PropertiesMap& properties)
	{
		const char* label = "variable";
		float min = 0;
		float max = 0;
		GetEditorCustomData(properties, label, min, max);

		return ImGuiHelper::InputAny(label, data);
		if (min != 0 || max != 0) {
			return ImGuiHelper::InputAny(label, data, min, max);
		}
		return ImGuiHelper::InputAny(label, data);
	}

	bool DrawEditorString(std::string& f, const PropertiesMap& properties);
	bool DrawEditorBool(bool& f, const PropertiesMap& properties);

	/// SERIALIZATION

	template<typename Type>
	void SerializeAny(Type& data, const PropertiesMap& properties, BinarySerializeArchive& archive)
	{
		/// TODO: File Version, could pass file version that is being save into function param when invoking.
		/// Will be used for backward compatability. Alternatively I try and write bytelength of data somehow.
		/*int version = -1;
		if (auto it = properties.find("version"_hs); it != properties.end())
		{
			version = *it->second.try_cast<const char*>();
		}*/

		archive(data);
	}
	template<typename Type>
	void DeserializeAny(Type& data, const PropertiesMap& properties, BinaryDeserializeArchive& archive)
	{
		archive(data);
	}
}