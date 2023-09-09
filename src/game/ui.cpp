#include "ui.h"

#include <game/input.h>
#include <game/player.h>
#include <game/utility.h>
#include <platform/imgui.h>
#include <platform/timing.h>
#include <platform/window.h>

void GAME::UIInit(
	UIState& ui, GLFWwindow* window, std::unordered_map<std::string, PLAT::ImguiFont>* fonts
) {
	ui.window = window;
	ui.fonts  = fonts;
	PLAT::ImguiSetupSizes();
	PLAT::ImguiSetupColors();
}

void GAME::UIRender(
	UIState& ui, PlayerObject& player, InputManager& input, PLAT::TimeControl& fpsCtrl, float dt
) {
	auto& fonts{ *ui.fonts };
	static int windowW, windowH;
	PLAT::GetWindowSize(ui.window, &windowW, &windowH);
	static ImVec2 winPos{ ImVec2(0, 0) };
	static ImVec2 winSize{ ImVec2(0, 0) };
	winSize = { (float)windowW, (float)windowH };
	static bool showDebugUI{ false };

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(winPos);
	ImGui::SetNextWindowSize(winSize);
	ImGui::Begin(
		"MainWindow", nullptr, //
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
	);

	if (ImGui::BeginTable("mainlayout", 4, 0 /*ImGuiTableFlags_Resizable*/)) {
		ImGui::TableSetupColumn("PaddingL");
		ImGui::TableSetupColumn("Test Setup", ImGuiTableColumnFlags_WidthFixed, 256.0f);
		ImGui::TableSetupColumn("Test Area", ImGuiTableColumnFlags_WidthFixed, 768.0f);
		ImGui::TableSetupColumn("PaddingR");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		// test setup column
		ImGui::TableNextColumn();
		static bool catTech{}, catMgmt{}, catStrat{};
		ImGui::PushFont(fonts["SectionTitle"].font);
		ImGui::Text("Topics");
		ImGui::PopFont();
		ImGui::Checkbox("Technology", &catTech);
		ImGui::Indent();
		ImGui::Text("Basic Theory");
		// Basic Theory
		// Algorithms and Programming
		ImGui::Text("Computer Systems");
		// Computer Components
		// System Components
		// Software
		// Hardware
		ImGui::Text("Technical Elements");
		// Human Interfaces
		// Multimedia
		// Databases
		// Networks
		// Security
		ImGui::Text("Development Techniques");
		// System Development Techniques
		// Software Development Management Techniques
		ImGui::Unindent();
		ImGui::Checkbox("Management", &catMgmt);
		ImGui::Indent();
		ImGui::Text("Project Management");
		// Project Management
		ImGui::Text("Service Management");
		// Service Management
		// System Auditing
		ImGui::Unindent();
		ImGui::Checkbox("Strategy", &catStrat);
		ImGui::Indent();
		ImGui::Text("Systems Strategy");
		// System Strategy
		// System Planning
		ImGui::Text("Management Strategy");
		// Management Strategy
		// Technology Strategy Management
		// Business Industry
		ImGui::Text("Corporate & Legal Affairs");
		// Corporate Activities
		// Legal Affairs
		ImGui::Unindent();
		ImGui::Separator();
		ImGui::Checkbox("Debug Dialog", &showDebugUI);

		// test area column
		ImGui::TableNextColumn();
		static ImVec2 quizCellPadding{ 24, 24 };
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, quizCellPadding);
		static size_t questionIdx{ 0 };
		ImGui::Spacing();
		if (ImGui::Button("PREV"))
			questionIdx = questionIdx > 0 ? questionIdx - 1 : player.questions.size() - 1;
		ImGui::SameLine();
		if (ImGui::Button("NEXT"))
			questionIdx = (questionIdx + 1) % player.questions.size();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		// FIXME: figure out how to render all the questions at once without the buttons
		// being fucky
		auto* q = player.questions[questionIdx].get();
		ImGui::PushFont(fonts["SectionTitle"].font);
		ImGui::Text("%03llu/%llu", questionIdx + 1, player.questions.size());
		ImGui::Spacing();
		ImGui::PushFont(fonts["QuestionText"].font);
		ImGui::Text("%s\t%s\t%s", q->strVerb, q->strResult, q->strMethod);
		ImGui::PopFont();
		ImGui::PopFont();
		ImGui::Spacing();
		static ImVec2 tablePadding{ 6, 2 };
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, tablePadding);

		q->RenderQuestion(fonts);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button(q->showHint ? "Hide Hint" : "Show Hint"))
			q->SetShowHint(!q->showHint);
		if (q->showHint)
			q->RenderHint(fonts);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (!q->showAnswer) {
			if (ImGui::Button("Show Answer"))
				q->SetShowAnswer(!q->showAnswer);
		} else {
			if (!q->marked) {
				if (ImGui::Button("PASS"))
					q->SetMarked(true, true);
				ImGui::SameLine();
				if (ImGui::Button("FAIL"))
					q->SetMarked(true, false);
			} else {
				if (ImGui::Button("Try Again")) {
					q->SetMarked(false, false);
					q->SetShowHint(false);
					q->SetShowAnswer(false);
					q->GenerateProblem(player.rng);
				}
				ImGui::SameLine();
				ImGui::Text(q->markedCorrect ? "PASSED" : "FAILED");
			}
			q->RenderAnswer(fonts);
		}

		ImGui::PopStyleVar();
		ImGui::Spacing();

		ImGui::PopStyleVar();
		ImGui::TableNextColumn();

		ImGui::EndTable();
	}

	if (showDebugUI)
		UIDebug(ui, windowW, windowH, dt);

	ImGui::End();

	ImGui::Render();
}

void GAME::UIDebug(UIState& ui, int winW, int winH, float dt) {
	auto& fonts{ *ui.fonts };
	static bool showImguiDemo{ false };
	if (showImguiDemo)
		ImGui::ShowDemoWindow();

	ImGui::PushFont(fonts["SectionTitle"].font);
	ImGui::Begin("Debug Dialog");
	ImGui::PushFont(fonts["MainText"].font);
	ImGui::PushItemWidth(128);

	ImGui::Checkbox("ImGui Demo Window", &showImguiDemo);

	ImGui::SeparatorText("Screen");
	ImGui::Text("FPS: %4.2f", 1.0f / dt);
	ImGui::Text("Window: %4i, %4i", winW, winH);
	// ImGui::DragInt("Physics FPS", &fpsCtrl.fixedFPS, 4, 12, 240, "%d");
	// if (ImGui::Button("Apply"))
	// 	PLAT::TimeCtrlSetFPS(fpsCtrl, fpsCtrl.fixedFPS);

	// ImGui::SeparatorText("Mouse");
	// ImGui::DragFloat("cm/360", &input.mouse.cm360, 0.1f, 0.1f, 64.0f, "%.1f");
	// ImGui::DragFloat("DPI", &input.mouse.dpi, 100.0f, 100.0f, 6400.0f, "%.0f");

	ImGui::PopItemWidth();
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopFont();
}