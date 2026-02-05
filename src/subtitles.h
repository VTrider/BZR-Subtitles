#pragma once

#include <imgui.h>

#include <chrono>
#include <mutex>
#include <queue>
#include <string>

namespace subtitles
{
	struct subtitle_request
	{
		std::string text;
		double duration;
		std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
		ImVec4 color;

		subtitle_request(const std::string& text, double duration, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
			: text(text), duration(duration), start_time(std::chrono::high_resolution_clock::now()), color(color) {}
	};

	inline double opacity = 0.0;

	inline std::mutex mtx{};
	inline std::queue<subtitle_request> request_queue;
};
