#include "ImguiManager.h"
#include "imgui-1.88/imgui.h"

ImguiManager::ImguiManager() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager() {
	ImGui::DestroyContext();
}
