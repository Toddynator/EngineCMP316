#include "Reflection.h"
#include "../ImGui/ImGuiFileDialog/ImGuiFileDialog.h"

namespace CMP316engine
{
	/*
	By creating meta objects for the dataTypes, 
	when I recursively resolve reflected objects, I can then access functionality defined for a type of data.

	NOTE:
	You could technically have one meta object for the data type and append .func() to the one metaObject call,
	but I've chosen to spread them out based on the function.

	NOTE TO SELF:
	I could add custom data to the data type meta objects such as names.
	Means if for example a variable of a reflected object doesn't have a name defined for the editor,
	it will then be called by the type of data instead of being completely undefined and being called something unrecognizable
	like "variable" or "unknown variable name".
	*/
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
		/*
		TODO:
		- Could add support for a fileFilter customData property, which allows the dialog to be further customized.
		*/

		const char* label = "string";
		char textBuffer[256];
		const char* filepath = "Undefined";

		if (auto it = properties.find("name"_hs); it != properties.end())
		{
			label = *it->second.try_cast<const char*>();
		}
		// If there is a filepath, open a filedialog, otherwise
		if (auto it = properties.find("filepath"_hs); it != properties.end())
		{
			filepath = *it->second.try_cast<const char*>();
		}
		else if (auto it = properties.find("textBuffer"_hs); it != properties.end())
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

		/// DRAW IMGUI WIDGET

		if (filepath != "Undefined")
		{
			/// IMGUI FILE DIALOG

			auto& io = ImGui::GetIO();
			ImVec2 windowMinSize = ImVec2(io.DisplaySize.x * 0.7f, io.DisplaySize.y * 0.7f);
		
			// Copy string into buffer so that InputText displays the current string.
			std::memcpy(textBuffer, f.data(), sizeof(textBuffer));
			if (ImGui::InputText(label, textBuffer, sizeof(textBuffer), ImGuiInputTextFlags_ReadOnly))
			{
			}
			// InputText is only readonly, detect if it was pressed then open file dialog.
			if (ImGui::IsItemClicked())
			{
				IGFD::FileDialogConfig config;
				config.path = filepath;
				config.fileName = "";
				config.countSelectionMax = 1;
				config.flags = ImGuiFileDialogFlags_Modal |
					ImGuiFileDialogFlags_DisableCreateDirectoryButton |
					ImGuiFileDialogFlags_ReadOnlyFileNameField;
				ImGuiFileDialog::Instance()->OpenDialog(
					"ChooseFileDlgKey",
					"Select File",
					".*",
					config
				);

				ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
				ImVec2 windowStartSize = ImVec2(io.DisplaySize.x * 0.7f, io.DisplaySize.y * 0.7f);
				ImGui::SetNextWindowPos(center, 0, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(windowStartSize);
			}

			if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, windowMinSize)) {
				if (ImGuiFileDialog::Instance()->IsOk()) {
					std::filesystem::path absolutePath{ ImGuiFileDialog::Instance()->GetFilePathName() };
					std::filesystem::path basePath{ filepath };
					// Because the FileDialog returns an absolute path from the Drive, I need to get the relative path from the executable directory
					// This unfortunately meant the hacky parent_path() calls.
					std::filesystem::path relativePath = std::filesystem::relative(absolutePath, std::filesystem::absolute(basePath).parent_path().parent_path().parent_path());
					f = relativePath.generic_string(); //.string() seems to use backward slashes which causes issues
				}
				ImGuiFileDialog::Instance()->Close();
			}
		}
		else
		{
			/// SIMPLE TEXT INPUT

			ImGui::Separator();
			ImGui::Text(f.c_str());
			bool result = ImGuiHelper::InputAny(label, f, textBuffer, sizeof(textBuffer));
			ImGui::Separator();
			return result;
		}
		return false;
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