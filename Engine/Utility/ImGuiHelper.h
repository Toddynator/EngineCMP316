#pragma once
#include <imgui.h>

namespace CMP316engine::ImGuiHelper
{
	void PromptUser(bool& isPrompt, std::function<void()> actionOnConfirm, std::string popupName, std::string popupQuestion);

	/*
	@brief generic ImGui field function that has overloads for a bunch of different data types.
	This is particularly handy for quickly creating controls for components. You just past each variable in and
	will automatically have controls for them.
	*/
	inline bool InputAny(const char* label, int& data) { return ImGui::InputInt(label, &data); }
	inline bool InputAny(const char* label, int& data, int min, int max) { return ImGui::SliderInt(label, &data, min, max); }
	inline bool InputAny(const char* label, float& data) { return ImGui::InputFloat(label, &data); }
	inline bool InputAny(const char* label, float& data, float min, float max) { return ImGui::SliderFloat(label, &data, min, max); }
	inline bool InputAny(const char* label, DirectX::XMFLOAT3& data) { return ImGui::InputFloat3(label, &data.x); }
	inline bool InputAny(const char* label, DirectX::XMFLOAT3& data, float min, float max) { return ImGui::SliderFloat3(label, &data.x, min, max); }
	inline bool InputAny(const char* label, std::string& data, char* textBuffer, size_t textBufferSize)
	{
		if (ImGui::InputText(label, textBuffer, textBufferSize, ImGuiInputTextFlags_EnterReturnsTrue)) { data = std::string(textBuffer); };
		return false;
	}
}
