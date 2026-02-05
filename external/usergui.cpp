#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "subtitles.h"

#include <optional>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

bool demoOpen = true;

std::optional<subtitles::subtitle_request> current_subtitle = std::nullopt;

void gui::UserGui() noexcept
{
	if (!current_subtitle)
	{
		std::lock_guard lock(subtitles::mtx);
		if (!subtitles::request_queue.empty())
		{
			current_subtitle = subtitles::request_queue.front();
			current_subtitle->start_time = std::chrono::high_resolution_clock::now();
			subtitles::request_queue.pop();
		}
		else
		{
			return;
		}
	}

	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(now - current_subtitle->start_time);
	if (elapsed_time.count() > current_subtitle->duration)
	{
		current_subtitle = std::nullopt;
		return;
	}

	if (bzoneFont)
	{
		ImGui::PushFont(bzoneFont);
	}

	auto size = ImGui::CalcTextSize(current_subtitle->text.c_str());
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2((io.DisplaySize.x - size.x)* 0.5f, io.DisplaySize.y * 0.85f), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(static_cast<float>(subtitles::opacity));
	auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize;
	if (subtitles::opacity == 0.0)
	{
		flags |= ImGuiWindowFlags_NoBackground;
	}
	ImGui::Begin("Subtitle", nullptr, flags);

	ImGui::TextColored(current_subtitle->color, current_subtitle->text.c_str());

	ImGui::End();

	if (bzoneFont)
	{
		ImGui::PopFont();
	}
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return true;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}