#include "Reflection.h"

namespace CMP316engine
{
	void InitializeReflectionFunctions()
	{
		/// EDITOR UI

		entt::meta<float>().func<&DrawEditor<float>>("DrawEditor"_hs);
		entt::meta<DirectX::XMFLOAT3>().func<&DrawEditor<DirectX::XMFLOAT3>>("DrawEditor"_hs);
		entt::meta<std::string>().func<&DrawEditorString>("DrawEditor"_hs);
		entt::meta<int>().func<&DrawEditor<int>>("DrawEditor"_hs);
		entt::meta<bool>().func<&DrawEditorBool>("DrawEditor"_hs);

		/// SERIALIZE & DESERIALIZE

		entt::meta<float>().func<&SerializeAny<float>>("Serialize"_hs);
		entt::meta<DirectX::XMFLOAT3>().func<&SerializeAny<DirectX::XMFLOAT3>>("Serialize"_hs);
		entt::meta<std::string>().func<&SerializeAny<std::string>>("Serialize"_hs);
		entt::meta<int>().func<&SerializeAny<int>>("Serialize"_hs);
		entt::meta<bool>().func<&SerializeAny<bool>>("Serialize"_hs);
		entt::meta<entt::entity>().func<&SerializeAny<entt::entity>>("Serialize"_hs);

		entt::meta<float>().func<&DeserializeAny<float>>("Deserialize"_hs);
		entt::meta<DirectX::XMFLOAT3>().func<&DeserializeAny<DirectX::XMFLOAT3>>("Deserialize"_hs);
		entt::meta<std::string>().func<&DeserializeAny<std::string>>("Deserialize"_hs);
		entt::meta<int>().func<&DeserializeAny<int>>("Deserialize"_hs);
		entt::meta<bool>().func<&DeserializeAny<bool>>("Deserialize"_hs);
		entt::meta<entt::entity>().func<&DeserializeAny<entt::entity>>("Deserialize"_hs);
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