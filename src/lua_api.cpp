#include "subtitles.h"

#include <imgui.h>
#include <lua.hpp>

#include <queue>

#define EXPORT(symbol) { #symbol, symbol },

namespace subtitles
{
	int submit(lua_State* L)
	{
		const char* text = luaL_checkstring(L, 1);
		double duration = luaL_checknumber(L, 2);

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		if (int argc = lua_gettop(L); argc > 2)
		{
			if (argc > 5)
			{
				return luaL_argerror(L, argc, "subtitles: invalid number of arguments");
			}
			r = (float)luaL_checkinteger(L, 3);
			g = (float)luaL_checkinteger(L, 4);
			b = (float)luaL_checkinteger(L, 5);
		}

		std::lock_guard lock(mtx);
		request_queue.emplace(text, duration, ImVec4(r, g, b, 1.0f));

		return 0;
	}

	int set_opacity(lua_State* L)
	{
		double o = luaL_checknumber(L, 1);
		std::lock_guard lock(mtx);
		opacity = o;
		return 0;
	}

	int clear_queue([[maybe_unused]] lua_State* L)
	{
		std::lock_guard lock(mtx);
		std::queue<subtitle_request>().swap(request_queue);
		return 0;
	}

	int clear_current([[maybe_unused]] lua_State* L)
	{
		request_clear.store(true, std::memory_order_relaxed);
		return 0;
	}

	extern "C" int __declspec(dllexport) luaopen_subtitles(lua_State* L)
	{
		constexpr luaL_Reg export_lib[] = {
			EXPORT(submit)
			EXPORT(set_opacity)
			EXPORT(clear_queue)
			EXPORT(clear_current)
			{ 0, 0 }
		};
		luaL_register(L, "subtitles", export_lib);
		return 0;
	}
}