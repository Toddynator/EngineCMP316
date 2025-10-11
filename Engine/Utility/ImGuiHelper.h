#pragma once
#include <imgui.h>

namespace CMP316engine::ImGuiHelper
{
	void PromptUser(bool& isPrompt, std::function<void()> actionOnConfirm, std::string popupName, std::string popupQuestion);
}
