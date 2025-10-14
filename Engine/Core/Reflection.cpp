#include "Reflection.h"
#include "../Utility/ImGuiHelper.h"

namespace CMP316engine
{
	void InitializeReflectionFunctions()
	{
		/// THE FUNCTIONS

		entt::meta<float>().func<&DrawEditorFloat>("DrawEditor"_hs);
		entt::meta<DirectX::XMFLOAT3>().func<&DrawEditorFloat3>("DrawEditor"_hs);
		entt::meta<std::string>().func<&DrawEditorString>("DrawEditor"_hs);
		entt::meta<int>().func<&DrawEditorInt>("DrawEditor"_hs);
		entt::meta<bool>().func<&DrawEditorBool>("DrawEditor"_hs);
	}

	void GetEditorCustomData(const PropertiesMap& properties, const char*& label, float& min, float& max)
	{
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
	}

	bool DrawEditorFloat(float& f, const PropertiesMap& properties)
	{
		const char* label = "float"; // Use the type as the name incase a name wasn't registered in custom data.
		float min = 0;
		float max = 0;
		GetEditorCustomData(properties, label, min, max);

		return ImGuiHelper::InputAny(label, f);
		if (min != 0 || max != 0) {
			return ImGuiHelper::InputAny(label, f, min, max);
		}
		return ImGuiHelper::InputAny(label, f);
	}
	bool DrawEditorFloat3(DirectX::XMFLOAT3& f, const PropertiesMap& properties)
	{
		const char* label = "float3";
		float min = 0;
		float max = 0;
		GetEditorCustomData(properties, label, min, max);

		if (min != 0 || max != 0) {
			return ImGuiHelper::InputAny(label, f, min, max);
		}
		return ImGuiHelper::InputAny(label, f);
	}
	bool DrawEditorString(std::string& f, const PropertiesMap& properties)
	{
		const char* label = "string";
		char textBuffer[256];

		if (auto it = properties.find("name"_hs); it != properties.end())
		{
			label = *it->second.try_cast<const char*>();
		}
		if (auto it = properties.find("textBuffer"_hs); it != properties.end())
		{
			if (auto array = it->second.try_cast<std::array<char, 256>>()) {
				std::memcpy(textBuffer, array->data(), sizeof(textBuffer));
			}
			else
			{
				// Invalid buffer
				ImGui::TextDisabled("Invalid Buffer");
				return false;
			}
		}
		else {
			ImGui::TextDisabled("No Buffer was added to the reflected components custom data!");
			return false;
		}

		/*
		TODO:
		- Filepath Custom Data property ~ Should open a file dialog (Add an ImGui File Dialog Library)
		*/

		ImGui::Separator();
		ImGui::Text(f.c_str());
		bool result = ImGuiHelper::InputAny(label, f, textBuffer, sizeof(textBuffer));
		ImGui::Separator();
		return result;
	}
	bool DrawEditorInt(int& f, const PropertiesMap& properties)
	{
		const char* label = "int";
		float min = 0;
		float max = 0;
		GetEditorCustomData(properties, label, min, max);

		if (min != 0 || max != 0) {
			return ImGuiHelper::InputAny(label, f, min, max);
		}
		return ImGuiHelper::InputAny(label, f);
	}
	bool DrawEditorBool(bool& f, const PropertiesMap& properties)
	{
		const char* label = "bool";

		if (auto it = properties.find("name"_hs); it != properties.end())
		{
			label = *it->second.try_cast<const char*>();
		}
		return ImGui::Checkbox(label, &f);
	}
}