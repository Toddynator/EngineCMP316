/*

*/

#pragma once
#include "entt.hpp"
#include <unordered_map>
#include <DirectXMath.h>

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

	/*
	Where the reflection for the functions will get initialized
	*/
	void InitializeReflectionFunctions();
	static class FunctionReflector
	{
	public:
		FunctionReflector()
		{
			InitializeReflectionFunctions();
		}
	};
	inline static FunctionReflector reflectorFunctions;

	void GetEditorCustomData(const PropertiesMap& properties, const char*& label, float& min, float& max);

	bool DrawEditorFloat(float& f, const PropertiesMap& properties);
	bool DrawEditorFloat3(DirectX::XMFLOAT3& f, const PropertiesMap& properties);
	bool DrawEditorString(std::string& f, const PropertiesMap& properties);
	bool DrawEditorInt(int& f, const PropertiesMap& properties);
	bool DrawEditorBool(bool& f, const PropertiesMap& properties);
}