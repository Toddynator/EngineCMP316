#include "ImGuiHelper.h"

namespace NomadEngine::ImGuiHelper
{
	void PromptUser(bool& isPrompt, std::function<void()> actionOnConfirm, std::string popupName, std::string popupQuestion)
	{
		if (isPrompt) {
			ImGui::OpenPopup(popupName.c_str());
		}
		// Want to ensure user doesn't accidentally lose view of the prompt, force some settings to prevent this.
		ImGuiWindowFlags popupSettings = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
		// Set Window to Centre of Screen - For same reason as listed above.
		// No idea what middle condition does, just set to 0 as that is default, the 3rd paramater will centre the 
		// window on the position provided by the 1st paramater.
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), 0, { 0.5,0.5 });
		if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, popupSettings)) {
			// Ask user question
			ImGui::Text(popupQuestion.c_str());
			ImGui::Separator();
			// Get Confirmation
			if (ImGui::Button("Yes", { 150, 0 })) {
				actionOnConfirm();
				ImGui::CloseCurrentPopup();
				isPrompt = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("No", { 150, 0 })) {
				ImGui::CloseCurrentPopup();
				isPrompt = false;
			}
			ImGui::EndPopup();
		}
	}
}
