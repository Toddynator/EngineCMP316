/*

*/

#pragma once
#include "entt.hpp"
#include <unordered_map>
#include <DirectXMath.h>
#include "../Utility/BinarySerializeArchive.h"
#include "../Utility/BinaryDeserializeArchive.h"
#include "../Utility/ImGuiHelper.h"

using namespace entt::literals;

namespace CMP316engine
{
	enum Traits : uint16_t
	{
		EDITOR = 1 << 0, // Reflected member variables that should have an editor control
		NOT_SERIALIZED = 1 << 1, // Reflected member variables that should not be serialized, and should evaluate to default values on loading (initialize bools for example). (Only needed if you actually reflect the variable)
		COMPONENT = 1 << 2 // Add to components so that components can be identified from other reflected data, can be useful for Component specific UI's
	};
	using PropertiesMap = std::unordered_map<entt::id_type, entt::meta_any>;

	/*
	Where the reflection for the functions will get initialized
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
		const char* label = "float"; // Use the type as the name incase a name wasn't registered in custom data.
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
		/// Will be used for backward compatability.
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