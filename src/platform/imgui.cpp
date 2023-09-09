#include "imgui.h"

void PLAT::ImguiSetupSizes() {
	ImGuiStyle& style			  = ImGui::GetStyle();
	// spacing
	style.WindowPadding			  = ImVec2(8.0f, 8.0f);
	style.FramePadding			  = ImVec2(8.0f, 4.0f);
	style.CellPadding			  = ImVec2(8.0f, 0.0f);
	style.ItemSpacing			  = ImVec2(4.0f, 4.0f);
	style.ItemInnerSpacing		  = ImVec2(4.0f, 4.0f);
	style.IndentSpacing			  = 8.0f;
	style.ScrollbarSize			  = 12.0f;
	style.GrabMinSize			  = 24.0f;
	style.SeparatorTextPadding	  = ImVec2(0.0f, 0.0f);
	// borders
	style.WindowBorderSize		  = 0.0f;
	style.TabBorderSize			  = 1.0f;
	style.ChildBorderSize		  = 1.0f;
	style.FrameBorderSize		  = 1.0f;
	style.PopupBorderSize		  = 1.0f;
	style.SeparatorTextBorderSize = 1.0f;
	// rounding
	style.WindowRounding		  = 4.0f;
	style.TabRounding			  = 3.0f;
	style.ChildRounding			  = 3.0f;
	style.FrameRounding			  = 3.0f;
	style.PopupRounding			  = 3.0f;
	style.GrabRounding			  = 2.0f;
	style.ScrollbarRounding		  = 2.0f;
}

void PLAT::ImguiSetupColors() {
	ImVec4* colors						   = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]				   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]		   = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]			   = ImVec4(0.00f, 0.00f, 0.00f, 0.95f);
	colors[ImGuiCol_ChildBg]			   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg]			   = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
	colors[ImGuiCol_Border]				   = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_BorderShadow]		   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]			   = ImVec4(0.04f, 0.07f, 0.20f, 0.65f);
	colors[ImGuiCol_FrameBgHovered]		   = ImVec4(0.04f, 0.07f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgActive]		   = ImVec4(0.07f, 0.12f, 0.35f, 1.00f);
	colors[ImGuiCol_TitleBg]			   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive]		   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]	   = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]			   = ImVec4(0.07f, 0.12f, 0.35f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]		   = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_ScrollbarGrab]		   = ImVec4(0.05f, 0.09f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.10f, 0.18f, 0.50f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.20f, 0.35f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark]			   = ImVec4(0.40f, 0.51f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab]			   = ImVec4(0.12f, 0.21f, 0.60f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]	   = ImVec4(0.20f, 0.35f, 1.00f, 1.00f);
	colors[ImGuiCol_Button]				   = ImVec4(0.07f, 0.12f, 0.35f, 0.65f);
	colors[ImGuiCol_ButtonHovered]		   = ImVec4(0.07f, 0.12f, 0.35f, 1.00f);
	colors[ImGuiCol_ButtonActive]		   = ImVec4(0.11f, 0.19f, 0.55f, 1.00f);
	colors[ImGuiCol_Header]				   = ImVec4(0.04f, 0.07f, 0.20f, 0.65f);
	colors[ImGuiCol_HeaderHovered]		   = ImVec4(0.04f, 0.07f, 0.20f, 1.00f);
	colors[ImGuiCol_HeaderActive]		   = ImVec4(0.07f, 0.12f, 0.35f, 1.00f);
	colors[ImGuiCol_Separator]			   = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]	   = ImVec4(0.04f, 0.07f, 0.20f, 0.70f);
	colors[ImGuiCol_SeparatorActive]	   = ImVec4(0.08f, 0.14f, 0.40f, 0.70f);
	colors[ImGuiCol_ResizeGrip]			   = ImVec4(0.07f, 0.12f, 0.35f, 0.65f);
	colors[ImGuiCol_ResizeGripHovered]	   = ImVec4(0.07f, 0.12f, 0.35f, 1.00f);
	colors[ImGuiCol_ResizeGripActive]	   = ImVec4(0.11f, 0.19f, 0.55f, 1.00f);
	colors[ImGuiCol_Tab]				   = ImVec4(0.04f, 0.07f, 0.20f, 0.65f);
	colors[ImGuiCol_TabHovered]			   = ImVec4(0.04f, 0.07f, 0.20f, 1.00f);
	colors[ImGuiCol_TabActive]			   = ImVec4(0.08f, 0.14f, 0.40f, 1.00f);
	colors[ImGuiCol_TabUnfocused]		   = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]	   = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_DockingPreview]		   = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg]		   = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]			   = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]	   = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]		   = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]		   = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]	   = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight]	   = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg]			   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]		   = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]		   = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]		   = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]		   = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]	   = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]	   = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void PLAT::ImguiSetupFonts(std::unordered_map<std::string, PLAT::ImguiFont>& fonts) {
	ImGuiIO& io = ImGui::GetIO();
	for (auto& font : fonts)
		font.second.font = io.Fonts->AddFontFromFileTTF(font.second.path, font.second.size);
	if (fonts.size() == 0)
		fonts["DEFAULT"] = { io.Fonts->AddFontDefault(), "", 0 };
}